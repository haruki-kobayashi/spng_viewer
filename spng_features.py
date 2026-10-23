"""Playback, tracking, filtering, projection and export controls for SPNG images."""
from pathlib import Path
import dataclasses,datetime,json,math,sys,tempfile,threading,traceback
import numpy as np
from PIL import Image
from PySide6.QtCore import Qt,QObject,Signal,Slot,QRunnable,QTimer,QSignalBlocker,QPointF,QRectF,QSize
from PySide6.QtGui import QImage,QPixmap,QPen,QColor,QIcon
from PySide6.QtWidgets import (QWidget,QVBoxLayout,QHBoxLayout,QFormLayout,QGroupBox,QTabWidget,QScrollArea,
    QPushButton,QLabel,QSpinBox,QDoubleSpinBox,QCheckBox,QComboBox,QLineEdit,QFileDialog,QMessageBox,QProgressBar,QGraphicsLineItem)
from spng_core import default_levels,save_png,box_to_bottom_left_roi
from spng_paramui import ScanParameterMixin
from spng_scanparams import image_layer
from spng_ops import (Cancelled,FilterConfig,parse_layers,line_position,preload_stack,process_stack,
    project_stack,projection_image,depth_lut,export_layers)
APP_DIR=Path(sys.executable).resolve().parent if getattr(sys,'frozen',False) else Path(__file__).resolve().parent

class ProgressBridge(QObject):
    progress=Signal(int,int,int,str)
    done=Signal(int,object,object)
    def __init__(self,viewer):
        super().__init__(viewer);self.viewer=viewer
        self.progress.connect(self.deliver_progress,Qt.ConnectionType.QueuedConnection)
        self.done.connect(self.deliver_result,Qt.ConnectionType.QueuedConnection)
    @Slot(int,int,int,str)
    def deliver_progress(self,number,done,total,message):
        self.viewer.bulk_progress(number,done,total,message)
    @Slot(int,object,object)
    def deliver_result(self,token,value,error):
        self.viewer.finish_feature_job(token,value,error)

class FeatureTask(QRunnable):
    def __init__(self,token,fn,bridge):
        super().__init__();self.token=token;self.fn=fn;self.bridge=bridge
    def run(self):
        try:value=self.fn()
        except Exception as exc:self.bridge.done.emit(self.token,None,(str(exc),traceback.format_exc()))
        else:self.bridge.done.emit(self.token,value,None)

class EnhancementMixin(ScanParameterMixin):
    def __init__(self):
        self._raw_bank=None;self._processed=None;self._projection=None;self._playing=False
        self._bulk_running=False;self._bulk_serial=0;self._cancel=None;self._points=[None,None]
        super().__init__()

    def scroll_page(self):
        content=QWidget();layout=QVBoxLayout(content);layout.setContentsMargins(10,10,10,10)
        sc=QScrollArea();sc.setWidgetResizable(True);sc.setWidget(content)
        return sc,layout
    def note(self,layout,text):
        label=QLabel(text);label.setWordWrap(True);layout.addWidget(label);return label
    def intbox(self,low=0,high=100000,value=0):
        b=QSpinBox();b.setRange(low,high);b.setValue(value);b.setKeyboardTracking(False);return b
    def realbox(self,low,high,value,decimals=2):
        b=QDoubleSpinBox();b.setRange(low,high);b.setDecimals(decimals);b.setValue(value);b.setKeyboardTracking(False);return b

    def build_ui(self):
        super().build_ui()
        split=self.centralWidget();old=split.widget(0)
        self.feature_tabs=QTabWidget();self.feature_tabs.setMinimumWidth(420)
        split.replaceWidget(0,self.feature_tabs);self.feature_tabs.addTab(old,'画像')
        play_page,pl=self.scroll_page();self.feature_tabs.addTab(play_page,'再生・追従')
        out_page,ol=self.scroll_page();self.feature_tabs.addTab(out_page,'重ね合わせ・出力')
        filter_page,fl=self.scroll_page();self.feature_tabs.addTab(filter_page,'画像処理')
        self.processing_page=filter_page
        self.bridge=ProgressBridge(self)
        self.play_timer=QTimer(self);self.play_timer.setSingleShot(True);self.play_timer.timeout.connect(self.play_tick)
        self.play_button=self.button('▶ 再生',self.toggle_play)
        self.play_rate=self.realbox(.2,60,8,1);self.play_rate.setSuffix(' 層/秒')
        self.loop_check=QCheckBox('ループ');self.loop_check.setChecked(True)
        mini=QHBoxLayout();mini.addWidget(self.play_button);mini.addWidget(self.play_rate);mini.addWidget(self.loop_check);mini.addStretch()
        self.centralWidget().widget(1).layout().insertLayout(2,mini)
        jobrow=QHBoxLayout();self.progress_bar=QProgressBar();self.progress_bar.hide()
        self.cancel_button=self.button('中止',self.cancel_bulk);self.cancel_button.hide();jobrow.addWidget(self.progress_bar,1);jobrow.addWidget(self.cancel_button)
        self.centralWidget().widget(1).layout().insertLayout(4,jobrow)
        replay=QGroupBox('自動層送り');rf=QFormLayout(replay)
        self.play_start=self.intbox();self.play_end=self.intbox();self.play_step=self.intbox(1,10000,1)
        rf.addRow('開始層（含む）',self.play_start);rf.addRow('終了層（含む）',self.play_end);rf.addRow('層の間隔',self.play_step)
        self.preload_check=QCheckBox('再生前に全層を先読みする');self.preload_check.setChecked(True);rf.addRow(self.preload_check)
        self.preload_button=self.button('全層を先読み',lambda:self.start_preload(False));rf.addRow(self.preload_button);pl.addWidget(replay)
        self.note(pl,'再生ボタン・速度・ループは画像下にも表示します。読み込みが遅い場合は指定速度より遅く再生し、層を飛ばしません。')
        tracking=QGroupBox('2点を結ぶ経路を画面中央へ');tf=QVBoxLayout(tracking)
        self.note(tf,'X・Yは元のspngのpixel座標（左下原点）。異なる層で1点ずつ指定してください。')
        self.point_fields=[]
        for tag in range(2):
            group=QGroupBox(f'点{tag+1}');form=QFormLayout(group)
            z=self.intbox();x=self.realbox(-100000,100000,0);y=self.realbox(-100000,100000,0)
            form.addRow('層番号',z);form.addRow('X',x);form.addRow('Y',y)
            pick=self.button('画像上をクリックして指定',lambda checked=False,t=tag:self.arm_point(t));form.addRow(pick)
            save=self.button('入力した座標を登録',lambda checked=False,t=tag:self.register_point(t));form.addRow(save)
            form.addRow(self.button('この点をリセット',lambda checked=False,t=tag:self.reset_point(t)))
            self.point_fields.append((z,x,y));tf.addWidget(group)
        self.follow_check=QCheckBox('経路に沿って中心を追従');self.follow_check.toggled.connect(self.center_follow);tf.addWidget(self.follow_check)
        self.extend_check=QCheckBox('2点の外側にも直線を延長');self.extend_check.setChecked(True);self.extend_check.toggled.connect(self.center_follow);tf.addWidget(self.extend_check)
        self.track_label=self.note(tf,'2点未登録')
        tf.addWidget(self.button('再生範囲を2点の層に合わせる',self.range_to_points));pl.addWidget(tracking);pl.addStretch()
        self.view.point_picked.connect(self.point_picked)
        self.view.zoom_changed.connect(lambda *_:self.center_follow())
        self.view.viewport_resized.connect(lambda:QTimer.singleShot(0,self.center_follow))
        self.center_cross=[]
        for _ in range(2):
            item=QGraphicsLineItem();pen=QPen(QColor('#20cfe4'),1);pen.setCosmetic(True);item.setPen(pen);item.setZValue(20);item.hide();self.view.scene().addItem(item);self.center_cross.append(item)
        self.note(ol,'対象は、現在のファイルと「表示データ」で選択した処理段階です。層番号は0始まり・両端を含みます。')
        self.layer_spec=QLineEdit('0');self.layer_spec.setPlaceholderText('例: 0-15,20,25-31:2')
        form=QFormLayout();form.addRow('層の指定',self.layer_spec);ol.addLayout(form)
        self.note(ol,'カンマで追加、a-bで範囲、a-b:2で2層おき。欠損層は除外します。')
        composite=QGroupBox('最大値・最小値を重ね合わせ');cf=QVBoxLayout(composite)
        row=QHBoxLayout();self.max_button=self.button('最大値を表示',lambda:self.start_projection('max'));self.min_button=self.button('最小値を表示',lambda:self.start_projection('min'));row.addWidget(self.max_button);row.addWidget(self.min_button);cf.addLayout(row)
        self.depth_check=QCheckBox('最大・最小になった層の番号で色分け');self.depth_check.toggled.connect(self.render_projection);cf.addWidget(self.depth_check)
        self.palette_combo=QComboBox();self.palette_combo.setIconSize(QSize(260,22))
        for name in ['kingfisher','meadow','ember','heat','rainbow']:
            rgb=np.tile(np.rint(depth_lut(palette=name)*255).astype(np.uint8)[None,:,:],(22,1,1))
            image=QImage(rgb.data,256,22,rgb.strides[0],QImage.Format.Format_RGB888).copy()
            self.palette_combo.addItem(QIcon(QPixmap.fromImage(image)),name,name)
            self.palette_combo.setItemData(self.palette_combo.count()-1,name,Qt.ItemDataRole.ToolTipRole)
            self.palette_combo.setItemData(self.palette_combo.count()-1,name,Qt.ItemDataRole.AccessibleTextRole)
        self.palette_combo.currentIndexChanged.connect(self.render_projection);cf.addWidget(self.palette_combo)
        self.depth_weight=QCheckBox('値で明度を変える（最小値は暗さで明度）');self.depth_weight.toggled.connect(self.render_projection);cf.addWidget(self.depth_weight)
        self.depth_legend=QLabel();cf.addWidget(self.depth_legend);self.depth_text=self.note(cf,'同値は最も小さい層番号を採用します。')
        cf.addWidget(self.button('通常の層表示へ戻る',self.return_to_slice));ol.addWidget(composite)
        ex=QGroupBox('指定した層をそれぞれPNG出力');ef=QVBoxLayout(ex)
        self.batch_crop=QCheckBox('選択範囲だけを書き出す');ef.addWidget(self.batch_crop)
        self.export_button=self.button('保存先を選んで一括出力',self.start_export);ef.addWidget(self.export_button)
        self.note(ef,'保存先内に新しい専用フォルダを作成します。元画像／処理後の画素値を保持し、層ごとにPNGを保存します。処理条件と欠損層の記録も保存します。')
        ol.addWidget(ex);ol.addWidget(self.button('表示層をPNG保存',lambda:self.save_dialog(False)));ol.addStretch()
        form=QFormLayout();self.stage_combo=QComboBox();self.stage_combo.addItem('元画像');self.stage_combo.currentTextChanged.connect(self.stage_changed)
        form.addRow('表示データ',self.stage_combo);fl.addLayout(form)
        self.build_parameter_ui(fl)
        self.process_info=self.note(fl,'処理は未適用。設定を変更したら「全層へ適用」を押してください。')
        self.resize_check=QCheckBox('画素補間');self.resize_check.setChecked(True);fl.addWidget(self.resize_check)
        form=QFormLayout();self.scale_box=self.realbox(.25,3,1.4142,4);self.scale_box.setSingleStep(.0001);self.interp_combo=QComboBox();self.interp_combo.addItems(['CPU cubic','GPU形式 A=-0.5']);self.interp_combo.setCurrentIndex(1)
        self.resize_check.toggled.connect(self.scale_box.setEnabled);self.resize_check.toggled.connect(self.interp_combo.setEnabled)
        form.addRow('補間倍率',self.scale_box);form.addRow('補間方式',self.interp_combo);fl.addLayout(form)
        self.highpass_check=QCheckBox('2D filter：FIR背景 B − I');self.highpass_check.setChecked(True);fl.addWidget(self.highpass_check)
        form=QFormLayout();self.tap_box=self.intbox(3,101,15);self.cutoff_box=self.realbox(.001,.499,.1,3);self.cutoff_box.setSingleStep(.01)
        form.addRow('FIR tap数（奇数）',self.tap_box);form.addRow('NormCutOffFreq',self.cutoff_box);fl.addLayout(form)
        self.z_check=QCheckBox('Z filter');self.z_check.setChecked(True);fl.addWidget(self.z_check)
        form=QFormLayout();self.z_weight=self.realbox(-5,5,-.2,3);self.z_weight.setSingleStep(.01);self.z_step=QComboBox();self.z_step.addItems(['前後1層（通常）','前後2層（1層飛ばし）'])
        form.addRow('Z filter係数',self.z_weight);form.addRow('Z参照間隔',self.z_step);fl.addLayout(form)
        self.binary_check=QCheckBox('二値化（値 > 閾値を1）');self.binary_check.setChecked(True);fl.addWidget(self.binary_check)
        form=QFormLayout();self.threshold_box=self.realbox(-1,255,9,2);form.addRow('二値化閾値',self.threshold_box);fl.addLayout(form)
        self.dilate_check=QCheckBox('膨張');self.dilate_check.setChecked(True);fl.addWidget(self.dilate_check)
        self.dilate_box=self.intbox(1,31,2);self.dilate_check.toggled.connect(self.dilate_box.setEnabled)
        form=QFormLayout();form.addRow('膨張 n×n',self.dilate_box);fl.addLayout(form)
        self.process_button=self.button('設定を現在のspngの全層へ適用',self.start_processing);fl.addWidget(self.process_button)
        self.note(fl,'8bitグレースケールに対応。全層と全処理段階を一度計算し、ディスクキャッシュから表示します。元データは変更しません。')
        self.note(fl,'補間 → 2D → Z → 二値化 → 膨張。途中はuint8へ丸め・飽和。二値画像は0/1。XY端はreflect101、Z端は片側の層を全係数で使用し、欠損隣接層は中心層で代用します。')
        self.note(fl,'前後2層は偶数層・奇数層を別々に処理します。16層模擬では再生・出力も0-31:2など同じ偶奇で選択してください。')
        self.note(fl,'可視化用フィルタです。GPU形式のA=-0.5補間をCPUで模擬します。dead pixel補正・affine・走査マスクには対応していません。')
        fl.addStretch();self.feature_tabs.setCurrentIndex(0)
        split.setSizes([460,990]);self.update_controls()

    def run_job(self,channel,fn,callback):
        # Explicit QObject slots guarantee that every QWidget update occurs on the GUI thread.
        self.serial+=1;token=self.serial;self.tokens[channel]=token
        job=FeatureTask(token,fn,self.bridge);self.jobs[token]=(job,channel,callback)
        self.pool.start(job);return token

    def finish_feature_job(self,token,value,error):
        entry=self.jobs.pop(token,None)
        if entry is None:return
        job,channel,callback=entry
        if self.tokens.get(channel)!=token:return
        if error:self.on_job_error(channel,error)
        else:callback(value)

    def cache_directory(self):
        try:
            root=APP_DIR/'cache';root.mkdir(exist_ok=True);return root
        except OSError:return Path(tempfile.gettempdir())/'SPNGViewer_cache'

    def read_source(self):
        stage=self.stage_combo.currentText()
        if stage!='元画像' and self._processed is not None:
            bank=self._processed
            return lambda i,b=bank,s=stage:b.read_frame(i,s),stage
        if self._raw_bank is not None:
            bank=self._raw_bank;return lambda i,b=bank:b.read_frame(i,'元画像'),'元画像'
        stack=self.stack;return lambda i,s=stack:s.read_frame(i),'元画像'

    def clear_stack(self):
        if hasattr(self,'scan_auto'):self.invalidate_scan_parameters()
        if hasattr(self,'play_timer'):self.stop_play()
        if self._cancel is not None:self._cancel.set()
        self._bulk_serial+=1;self.invalidate('bulk');self._bulk_running=False
        self._raw_bank=None;self._processed=None;self._projection=None;self._points=[None,None]
        if hasattr(self,'stage_combo'):
            with QSignalBlocker(self.stage_combo):self.stage_combo.clear();self.stage_combo.addItem('元画像')
            with QSignalBlocker(self.follow_check):self.follow_check.setChecked(False)
            for item in self.center_cross:item.hide()
            self.track_label.setText('2点未登録');self.process_info.setText('処理は未適用。')
            self.progress_bar.hide();self.cancel_button.hide()
        self.view.tracking_active=False
        super().clear_stack()

    def stack_ready(self,stack):
        n=len(stack)-1
        for b in [self.play_start,self.play_end]+[p[0] for p in self.point_fields]:b.setMaximum(n)
        self.play_start.setValue(0);self.play_end.setValue(n);self.layer_spec.setText(f'0-{n}')
        super().stack_ready(stack)
        if image_layer(self.current_path) is None:
            with QSignalBlocker(self.scan_layer):self.scan_layer.setValue(-1)
        self.reload_scan_parameters()

    def load_requested_layer(self):
        if not self.stack:return
        self._projection=None
        index=self.layer_number.value();self.loading_image=True;self.update_controls()
        if index in self.cache:
            image=self.cache.pop(index);self.cache[index]=image;self.frame_ready((index,image));return
        read,_=self.read_source();self.run_job('frame',lambda:(index,read(index)),self.frame_ready)

    def request_layer(self):
        super().request_layer()
        if self._playing:self.debounce.stop();self.load_requested_layer()

    def frame_ready(self,pair):
        super().frame_ready(pair)
        if self.image is not None:
            stage=self.stage_combo.currentText()
            self.filename.setText(self.current_path.name+'　／　'+stage)
            if stage!='元画像':self.meta.setText(self.meta.text().split('\n形式：')[0]+f'\n表示：{stage}（処理後）　／　原点：左下')
            self.center_follow()

    def stage_changed(self,*_):
        if not self.stack:return
        self.stop_play();self._projection=None;self.cache.clear();self.cache_bytes=0;self.levels_initialized=False
        self.invalidate('frame');self.request_layer()

    def toggle_play(self):
        if self._playing:self.stop_play();return
        if not self.stack or self._bulk_running:return
        if self.play_start.value()>self.play_end.value():QMessageBox.warning(self,'再生範囲','開始層≦終了層にしてください。');return
        if self.preload_check.isChecked() and self._raw_bank is None and self.stage_combo.currentText()=='元画像':self.start_preload(True);return
        self.begin_play()

    def begin_play(self):
        was_projection=self._projection is not None
        self._projection=None;self._playing=True;self.play_button.setText('⏸ 一時停止')
        a,b=self.play_start.value(),self.play_end.value();i=self.layer_number.value()
        if i<a or i>=b:self.layer_number.setValue(a)
        if was_projection:self.request_layer()
        elif self.image is None and not self.loading_image:self.load_requested_layer()
        self.play_timer.start(max(1,round(1000/self.play_rate.value())))

    def stop_play(self):
        self._playing=False
        if hasattr(self,'play_timer'):self.play_timer.stop();self.play_button.setText('▶ 再生')

    def play_tick(self):
        if not self._playing or not self.stack:return
        if self.loading_image:self.play_timer.start(10);return
        a,b=self.play_start.value(),self.play_end.value()
        if a>b:self.stop_play();self.state_label.setText('再生範囲は開始層≦終了層にしてください。');return
        n=self.layer_number.value()+self.play_step.value()
        if n>b:
            if self.loop_check.isChecked():n=a
            else:self.stop_play();return
        self.layer_number.setValue(n)
        self.play_timer.start(max(1,round(1000/self.play_rate.value())))

    def reset_point(self,tag):
        self.follow_check.setChecked(False);self._points[tag]=None
        for field in self.point_fields[tag]:
            field.setValue(0);field.clear()
        self.track_label.setText('\n'.join(f'点{i+1}: 層{p[0]}、X={p[1]:.2f}、Y={p[2]:.2f}' if p else f'点{i+1}: 未登録' for i,p in enumerate(self._points)))
        self.center_follow()

    def arm_point(self,tag):
        if self.image is None:return
        if self._projection is not None:QMessageBox.information(self,'点の指定','通常の層表示へ戻してから点を指定してください。');return
        self.view.arm_point(tag);self.state_label.setText(f'点{tag+1}：画像内の位置を左クリックしてください（現在の層 {self.layer_number.value()}）。')

    def point_picked(self,picked):
        tag,x,y=picked
        if self.image is None:return
        rec=self.stack.frames[self.layer_number.value()];sx=self.image.width/rec.width;sy=self.image.height/rec.height
        z,xx,yy=self.point_fields[tag]
        z.setValue(self.layer_number.value());xx.setValue((x+.5)/sx-.5);yy.setValue((y+.5)/sy-.5);self.register_point(tag)

    def register_point(self,tag):
        if not self.stack:return
        z,x,y=self.point_fields[tag];self._points[tag]=(z.value(),x.value(),y.value())
        self.track_label.setText('\n'.join(f'点{i+1}: 層{p[0]}、X={p[1]:.2f}、Y={p[2]:.2f}' if p else f'点{i+1}: 未登録' for i,p in enumerate(self._points)))
        if all(p is not None for p in self._points):
            if self._points[0][0]==self._points[1][0]:self.follow_check.setChecked(False);QMessageBox.warning(self,'点の層','2点は異なる層にしてください。');return
            self.follow_check.setChecked(True);self.center_follow()

    def range_to_points(self):
        if all(p is not None for p in self._points):
            self.play_start.setValue(min(p[0] for p in self._points));self.play_end.setValue(max(p[0] for p in self._points))

    def center_follow(self,*_):
        if not hasattr(self,'follow_check') or getattr(self,'_closing_features',False):return
        enabled=self.follow_check.isChecked() and self.image is not None and self._projection is None and all(p is not None for p in self._points)
        if not enabled:
            for item in self.center_cross:item.hide()
            if not self.follow_check.isChecked():self.view.tracking_active=False
            return
        try:x,y=line_position(*self._points,self.layer_number.value(),self.extend_check.isChecked())
        except ValueError:return
        rec=self.stack.frames[self.layer_number.value()]
        if not rec.width or not rec.height:return
        x=(x+.5)*self.image.width/rec.width-.5;y=(y+.5)*self.image.height/rec.height-.5
        self.view.center_on_pixel(x,y)
        scale=max(.001,self.view.transform().m11());radius=7/scale;cx=x+.5;cy=self.image.height-.5-y
        self.center_cross[0].setLine(cx-radius,cy,cx+radius,cy);self.center_cross[1].setLine(cx,cy-radius,cx,cy+radius)
        for item in self.center_cross:item.show()

    def fit_all(self):
        if hasattr(self,'follow_check'):self.follow_check.setChecked(False)
        super().fit_all()

    def begin_bulk(self,label,operation,callback):
        if self._bulk_running:return
        self.stop_play();self.debounce.stop();self.invalidate('frame');self.loading_image=False
        self._bulk_running=True;self._bulk_serial+=1;number=self._bulk_serial;self._cancel=threading.Event();cancel=self._cancel
        self.progress_bar.setRange(0,100);self.progress_bar.setValue(0);self.progress_bar.show();self.cancel_button.show();self.state_label.setText(label)
        def progress(done,total,message):self.bridge.progress.emit(number,done,total,message)
        def finish(result):
            self._bulk_running=False;self.progress_bar.hide();self.cancel_button.hide();self.update_controls();callback(result)
        self.update_controls();self.run_job('bulk',lambda:operation(progress,cancel),finish)

    def bulk_progress(self,number,done,total,message):
        if number==self._bulk_serial and self._bulk_running:
            self.progress_bar.setValue(round(done/max(1,total)*100));self.state_label.setText(f'{message}　{done}/{total}')

    def cancel_bulk(self):
        if self._cancel is not None:self._cancel.set();self.state_label.setText('中止を待っています…')

    def on_job_error(self,channel,error):
        if channel=='bulk':
            self._bulk_running=False;self.progress_bar.hide();self.cancel_button.hide();self.update_controls()
            self.state_label.setText(error[0])
            if error[0]!='処理を中止しました。':QMessageBox.warning(self,'処理できませんでした',error[0])
            if self.image is None and self.stack:self.load_requested_layer()
        else:super().on_job_error(channel,error)

    def start_preload(self,play_after=False):
        if not self.stack:return
        stack=self.stack;directory=self.cache_directory()
        def finished(bank):
            self._raw_bank=bank;self.state_label.setText(f'全層先読み完了（{bank.metadata["elapsed_seconds"]:.2f}秒）')
            if play_after:self.begin_play()
            else:self.load_requested_layer()
        self.begin_bulk('全層を先読み中…',lambda p,c:preload_stack(stack,directory,p,c),finished)

    def filter_config(self):
        return self.exact_parameter_config(FilterConfig(scale=self.scale_box.value() if self.resize_check.isChecked() else 1.0,interpolation=self.interp_combo.currentText(),highpass=self.highpass_check.isChecked(),taps=self.tap_box.value(),cutoff=self.cutoff_box.value(),z_filter=self.z_check.isChecked(),z_weight=self.z_weight.value(),z_step=self.z_step.currentIndex()+1,binary=self.binary_check.isChecked(),threshold=self.threshold_box.value(),dilation=self.dilate_box.value() if self.dilate_check.isChecked() else 1))

    def start_processing(self):
        if not self.stack:return
        if self.scan_auto.isChecked() and (self._scan_pending or self._scan_error):
            QMessageBox.warning(self,'スキャン設定','読み込みが完了していません。設定を確認するか、自動読み込みをOFFにしてください。');return
        try:config=self.filter_config();config.validate()
        except ValueError as e:QMessageBox.warning(self,'処理条件',str(e));return
        stack=self.stack;directory=self.cache_directory()
        scan_context=self._scan_settings.metadata() if self._scan_settings else None
        if scan_context:
            scan_context['applied_threshold']=config.threshold
            scan_context['applied_config_matches_loaded']=config==self._scan_settings.config
        def finished(bank):
            if scan_context:bank.metadata['scan_parameters']=scan_context
            self._processed=bank
            with QSignalBlocker(self.stage_combo):self.stage_combo.clear();self.stage_combo.addItems(['元画像']+bank.stages);self.stage_combo.setCurrentText(bank.stages[-1])
            self.process_info.setText(f'全{len(bank)}層の処理完了：{bank.metadata["elapsed_seconds"]:.2f}秒。\n適用済み条件：'+json.dumps(bank.metadata['config'],ensure_ascii=False))
            self.stage_changed()
        self.begin_bulk('全層の画像処理を開始…',lambda p,c:process_stack(stack,config,directory,p,c),finished)

    def selected_layers(self):return parse_layers(self.layer_spec.text(),len(self.stack))

    def start_projection(self,kind):
        if not self.stack:return
        try:indices=self.selected_layers()
        except ValueError as e:QMessageBox.warning(self,'層の指定',str(e));return
        read,source=self.read_source()
        def finished(result):self._projection=result;self.render_projection()
        self.begin_bulk('重ね合わせを計算中…',lambda p,c:project_stack(read,indices,kind,source,p,c),finished)

    def render_projection(self,*_):
        p=self._projection
        if p is None:return
        self.stop_play();self.invalidate('frame');self.debounce.stop();self.loading_image=False
        self.image=projection_image(p,self.depth_check.isChecked(),self.depth_weight.isChecked(),self.palette_combo.currentData());self.loaded_index=self.layer_number.value()
        if self.depth_check.isChecked():
            with QSignalBlocker(self.invert):self.invert.setChecked(False)
        self.levels_initialized=True;self.set_levels(*default_levels(self.image))
        self.refresh_display()
        w,h=self.image.size
        for b,m in [(self.roi_x,w-1),(self.roi_y,h-1),(self.roi_w,w),(self.roi_h,h)]:b.setMaximum(m)
        name='最大値' if p.kind=='max' else '最小値'
        self.filename.setText(f'{self.current_path.name}　／　{p.source}　／　{name}：{min(p.requested)}-{max(p.requested)}')
        self.meta.setText(f'{w} × {h} pixel\n{name}合成：指定{len(p.requested)}層、使用{len(p.used)}層\n同値は最も小さい層番号を採用')
        self.state_label.setText('重ね合わせを表示中。通常の層に戻すには層送り、または「通常の層表示へ戻る」。')
        lut=np.rint(depth_lut(palette=self.palette_combo.currentData())*255).astype(np.uint8)[None,:,:];q=QImage(lut.data,256,1,lut.strides[0],QImage.Format.Format_RGB888).copy()
        self.depth_legend.setPixmap(QPixmap.fromImage(q).scaled(300,12));self.depth_legend.setVisible(self.depth_check.isChecked())
        self.depth_text.setText(f'左端：層{min(p.requested)} → 右端：層{max(p.requested)}（{self.palette_combo.currentData()}）\n同値は最も小さい層番号。色分けの保存はRGB画像です。')
        for item in self.center_cross:item.hide()
        self.update_controls()

    def return_to_slice(self):
        self._projection=None
        if self.stack:self.request_layer()

    def cursor_position(self,point):
        super().cursor_position(point)
        p=self._projection
        if p is not None and point is not None and self.image is not None:
            x,y=math.floor(point[0]),math.floor(point[1]);h,w=p.values.shape
            if 0<=x<w and 0<=y<h:self.cursor_label.setText(f'カーソル［左下原点・pixel］ X: {x}　Y: {h-1-y}　値: {p.values[y,x]}　選択層: {p.layers[y,x]}')

    def start_export(self):
        if not self.stack:return
        try:indices=self.selected_layers()
        except ValueError as e:QMessageBox.warning(self,'層の指定',str(e));return
        roi=self.view.roi if self.batch_crop.isChecked() else None
        if self.batch_crop.isChecked() and roi is None:QMessageBox.information(self,'範囲未選択','画像上で範囲を選択してください。');return
        parent=QFileDialog.getExistingDirectory(self,'PNG出力先（中に専用フォルダを作ります）',self.last_save_dir)
        if not parent:return
        read,source=self.read_source();stem=self.current_path.stem;tag=datetime.datetime.now().strftime('%Y%m%d_%H%M%S_%f')
        folder=Path(parent)/(stem+'_png_'+tag)
        meta={'input_spng':str(self.current_path),'stage':source,'processed':self._processed.metadata if source!='元画像' and self._processed is not None else None,'coordinate_origin':'bottom-left','crop_bottom_left_xywh':box_to_bottom_left_roi(roi,self.image.size) if roi is not None and self.image else None}
        def finished(r):self.last_save_dir=str(folder.parent);self.state_label.setText(f'PNG出力完了：{r["saved"]}層、欠損{len(r["missing"])}層　{r["folder"]}')
        self.begin_bulk('PNGを書き出し中…',lambda p,c:export_layers(read,indices,folder,stem,source,roi,meta,p,c),finished)

    def save_dialog(self,crop):
        if self._projection is None and self.stage_combo.currentText()=='元画像':return super().save_dialog(crop)
        if self.image is None:return
        roi=self.view.roi if crop else None
        if crop and roi is None:return
        p=self._projection;stage=self.stage_combo.currentText()
        if p is not None:
            name=f'{self.current_path.stem}_{p.kind}_z{min(p.requested)}-{max(p.requested)}'+('_color' if self.depth_check.isChecked() else '')
        else:name=f'{self.current_path.stem}_{stage.replace(" ","_")}_z{self.loaded_index:03d}'
        if roi:
            x,y,w,h=box_to_bottom_left_roi(roi,self.image.size);name+=f'_x{x}_y{y}_w{w}_h{h}'
        path,_=QFileDialog.getSaveFileName(self,'表示データをPNG保存',str(Path(self.last_save_dir)/(name+'.png')),'PNG画像 (*.png)')
        if not path:return
        target=Path(path).with_suffix('.png')
        if str(target)!=path and target.exists() and QMessageBox.question(self,'上書き確認',f'{target.name} は存在します。上書きしますか？')!=QMessageBox.StandardButton.Yes:return
        try:
            saved=save_png(self.image,target,roi)
            metadata={'input_spng':str(self.current_path),'display_stage':p.source if p is not None else stage,'layer':None if p is not None else self.loaded_index,'projection':{'kind':p.kind,'requested':p.requested,'used':p.used,'depth_colored':self.depth_check.isChecked(),'colormap':self.palette_combo.currentData(),'brightness_weighted':self.depth_weight.isChecked(),'ties':'lowest layer index'} if p is not None else None,'ROI_top_left_box':roi,'ROI_bottom_left_xywh':box_to_bottom_left_roi(roi,self.image.size) if roi else None,'processing':self._processed.metadata if self._processed is not None and (p.source if p is not None else stage)!='元画像' else None}
            saved.with_suffix('.json').write_text(json.dumps(metadata,ensure_ascii=False,indent=2),'utf8')
        except Exception as e:QMessageBox.warning(self,'保存エラー',str(e));return
        self.last_save_dir=str(saved.parent);self.state_label.setText('PNGと処理記録を保存しました：'+str(saved))

    def update_controls(self):
        super().update_controls()
        if not hasattr(self,'play_button'):return
        ready=self.stack is not None and not self._bulk_running
        for b in [self.play_button,self.preload_button,self.process_button,self.max_button,self.min_button,self.export_button,self.stage_combo]:b.setEnabled(ready)
        if hasattr(self,'scan_auto') and self.scan_auto.isChecked():
            self.process_button.setEnabled(ready and not self._scan_pending and not self._scan_error)
        self.processing_page.setEnabled(not self._bulk_running)
        fixed_colors=self._projection is not None and self.depth_check.isChecked()
        for b in [self.low,self.high,self.invert]:b.setEnabled(not fixed_colors)
        if self._bulk_running:self.full_action.setEnabled(False);self.crop_action.setEnabled(False);self.crop_button.setEnabled(False);self.apply_roi_button.setEnabled(False)

    def closeEvent(self,event):
        self._closing_features=True
        self.stop_play()
        if self._cancel is not None:self._cancel.set()
        self._bulk_serial+=1;self.invalidate('bulk')
        self.pool.waitForDone(30000)
        self._raw_bank=None;self._processed=None;self._projection=None
        super().closeEvent(event)
