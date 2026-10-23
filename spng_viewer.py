"""SPNG Viewer: lazy per-layer reading, bottom-left coordinates, exact PNG export."""
from __future__ import annotations
from collections import OrderedDict
from pathlib import Path
import argparse
import logging
import math
import sys
import traceback
import numpy as np
from PIL import Image
from PySide6.QtCore import Qt, Signal, QObject, QRunnable, QThreadPool, QTimer, QRectF, QPointF, QSignalBlocker
from PySide6.QtGui import QImage, QPixmap, QPen, QColor, QFontDatabase, QAction, QKeySequence, QPainter
from PySide6.QtWidgets import (QApplication, QMainWindow, QWidget, QHBoxLayout, QVBoxLayout,
    QFormLayout, QPushButton, QComboBox, QLabel, QSpinBox, QDoubleSpinBox, QSlider,
    QGroupBox, QCheckBox, QFileDialog, QMessageBox, QGraphicsView, QGraphicsScene,
    QGraphicsPixmapItem, QGraphicsRectItem, QSplitter, QToolBar, QStyle, QSizePolicy,
    QAbstractSpinBox, QScrollArea)
from spng_core import (SpngStack, SpngError, natural_key, discover_root, discover_sensors,
    list_sensor_files, display_array, default_levels, save_png, drag_roi,
    bottom_left_roi_to_box, box_to_bottom_left_roi)

APP_DIR = Path(sys.executable).resolve().parent if getattr(sys,'frozen',False) else Path(__file__).resolve().parent

class JobSignals(QObject):
    done = Signal(int, object, object)

class Job(QRunnable):
    def __init__(self, token, fn):
        super().__init__()
        self.token, self.fn = token, fn
        self.signals = JobSignals()
    def run(self):
        try:
            value = self.fn()
        except Exception as exc:
            self.signals.done.emit(self.token, None, (str(exc), traceback.format_exc()))
        else:
            self.signals.done.emit(self.token, value, None)

class ImageView(QGraphicsView):
    roi_changed = Signal(object)
    cursor_changed = Signal(object)
    layer_step = Signal(int)
    zoom_changed = Signal(float)
    point_picked = Signal(object)
    viewport_resized = Signal()
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setScene(QGraphicsScene(self))
        self.pixmap_item = QGraphicsPixmapItem()
        self.scene().addItem(self.pixmap_item)
        self.roi_item = QGraphicsRectItem()
        pen=QPen(QColor('#ffb448'), 2); pen.setCosmetic(True)
        self.roi_item.setPen(pen)
        self.roi_item.setBrush(Qt.BrushStyle.NoBrush)
        self.roi_item.setZValue(10)
        self.scene().addItem(self.roi_item)
        self.roi_item.hide()
        self.setBackgroundBrush(QColor('#202834'))
        self.setRenderHint(QPainter.RenderHint.SmoothPixmapTransform, False)
        self.setTransformationAnchor(QGraphicsView.ViewportAnchor.AnchorUnderMouse)
        self.setResizeAnchor(QGraphicsView.ViewportAnchor.AnchorViewCenter)
        self.setMouseTracking(True)
        self.viewport().setMouseTracking(True)
        self.setFocusPolicy(Qt.FocusPolicy.StrongFocus)
        self.image_size = None
        self.roi = None
        self.drag_start = None
        self.pan_start = None
        self.fit_mode = True
        self.pick_mode = None

    def set_image(self, array, reset=False):
        h, w = array.shape[:2]
        if array.ndim == 2:
            fmt = QImage.Format.Format_Grayscale8
        elif array.shape[2] == 4:
            fmt = QImage.Format.Format_RGBA8888
        else:
            fmt = QImage.Format.Format_RGB888
        qimage = QImage(array.data, w, h, array.strides[0], fmt).copy()
        self.pixmap_item.setPixmap(QPixmap.fromImage(qimage))
        self.image_size=(w,h)
        if not getattr(self,"tracking_active",False):self.scene().setSceneRect(QRectF(0,0,w,h))
        if reset:
            self.set_roi(None, emit=False)
            self.fit_mode=True
        if self.fit_mode:
            self.fit_all()

    def clear_image(self):
        self.pixmap_item.setPixmap(QPixmap())
        self.image_size = None
        self.set_roi(None, emit=False)
        self.scene().setSceneRect(QRectF())
        self.cursor_changed.emit(None)

    def fit_all(self):
        self.tracking_active=False
        if self.image_size:self.scene().setSceneRect(QRectF(0,0,*self.image_size))
        self.fit_mode=True
        if self.image_size:
            self.fitInView(self.pixmap_item.boundingRect(),Qt.AspectRatioMode.KeepAspectRatio)
            self.zoom_changed.emit(self.transform().m11()*100)

    def actual_size(self):
        self.fit_mode=False
        self.resetTransform()
        self.zoom_changed.emit(100)

    def set_zoom(self, percent):
        self.fit_mode=False
        current=self.transform().m11()
        target=max(.02,min(32,float(percent)/100))
        self.scale(target/current,target/current)
        self.zoom_changed.emit(target*100)

    def fit_roi(self):
        if self.roi:
            x0,y0,x1,y1=self.roi
            self.fit_mode=False
            self.fitInView(QRectF(x0,y0,x1-x0,y1-y0).adjusted(-2,-2,2,2),Qt.AspectRatioMode.KeepAspectRatio)
            self.zoom_changed.emit(self.transform().m11()*100)

    def set_roi(self, box, emit=True):
        if box is None and emit:self.pick_mode=None;self.unsetCursor()
        self.roi=tuple(map(int,box)) if box else None
        if self.roi:
            x0,y0,x1,y1=self.roi
            self.roi_item.setRect(QRectF(x0,y0,x1-x0,y1-y0))
            self.roi_item.show()
        else:
            self.roi_item.hide()
        if emit:
            self.roi_changed.emit(self.roi)

    def resizeEvent(self,event):
        super().resizeEvent(event)
        self.viewport_resized.emit()
        if self.fit_mode:
            self.fit_all()

    def wheelEvent(self,event):
        delta=event.angleDelta().y()
        if not delta:
            return
        if event.modifiers() & Qt.KeyboardModifier.ControlModifier:
            self.set_zoom(self.transform().m11()*100*(1.2 if delta>0 else 1/1.2))
        else:
            self.layer_step.emit(-1 if delta>0 else 1)
        event.accept()

    def center_on_pixel(self,x,y):
        if not self.image_size:return
        self.fit_mode=False;self.tracking_active=True
        w,h=self.image_size;scale=max(.001,self.transform().m11())
        mx=self.viewport().width()/scale+abs(x)+2;my=self.viewport().height()/scale+abs(y)+2
        self.scene().setSceneRect(QRectF(-mx,-my,w+2*mx,h+2*my))
        self.centerOn(QPointF(x+.5,h-.5-y))

    def arm_point(self,tag):
        self.pick_mode=tag;self.setCursor(Qt.CursorShape.CrossCursor)

    def mousePressEvent(self,event):
        if self.image_size and self.pick_mode is not None and event.button()==Qt.MouseButton.LeftButton and not event.modifiers() & Qt.KeyboardModifier.ControlModifier:
            p=self.mapToScene(event.position().toPoint());w,h=self.image_size
            if 0<=p.x()<w and 0<=p.y()<h:
                tag=self.pick_mode;self.pick_mode=None;self.unsetCursor()
                self.point_picked.emit((tag,math.floor(p.x()),h-1-math.floor(p.y())))
                event.accept();return
        if self.image_size and event.button()==Qt.MouseButton.LeftButton and event.modifiers() & Qt.KeyboardModifier.ControlModifier:
            p=self.mapToScene(event.position().toPoint())
            w,h=self.image_size
            if 0<=p.x()<w and 0<=p.y()<h:
                self.drag_start=(p.x(),p.y())
                self.setCursor(Qt.CursorShape.CrossCursor)
                event.accept();return
        elif self.image_size and event.button() in (Qt.MouseButton.LeftButton,Qt.MouseButton.RightButton,Qt.MouseButton.MiddleButton):
            self.pan_start=event.position()
            self.setCursor(Qt.CursorShape.ClosedHandCursor)
            event.accept();return
        super().mousePressEvent(event)

    def mouseMoveEvent(self,event):
        p=self.mapToScene(event.position().toPoint())
        self.cursor_changed.emit((p.x(),p.y()))
        if self.pan_start is not None:
            delta=event.position()-self.pan_start
            self.horizontalScrollBar().setValue(self.horizontalScrollBar().value()-round(delta.x()))
            self.verticalScrollBar().setValue(self.verticalScrollBar().value()-round(delta.y()))
            self.pan_start=event.position()
            p=self.mapToScene(event.position().toPoint())
            self.cursor_changed.emit((p.x(),p.y()))
            event.accept();return
        if self.drag_start is not None:
            box=drag_roi(self.drag_start,(p.x(),p.y()),self.image_size)
            self.set_roi(box)
            event.accept();return
        super().mouseMoveEvent(event)

    def mouseReleaseEvent(self,event):
        if self.pan_start is not None:
            self.pan_start=None
            self.unsetCursor()
            event.accept();return
        if self.drag_start is not None:
            p=self.mapToScene(event.position().toPoint())
            self.set_roi(drag_roi(self.drag_start,(p.x(),p.y()),self.image_size))
            self.drag_start=None
            self.unsetCursor()
            event.accept();return
        super().mouseReleaseEvent(event)

    def leaveEvent(self,event):
        self.cursor_changed.emit(None)
        super().leaveEvent(event)

class Viewer(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle('SPNG Viewer')
        self.resize(1450,950)
        self.pool=QThreadPool(self);self.pool.setMaxThreadCount(2)
        self.jobs={};self.tokens={};self.serial=0
        self.records=[];self.stack=None;self.image=None;self.loaded_index=None
        self.cache=OrderedDict();self.cache_bytes=0
        self.cache_limit=96*1024*1024
        self.current_path=None;self.last_open_dir=str(APP_DIR)
        self.last_save_dir=str(APP_DIR/'exports' if (APP_DIR/'exports').is_dir() else APP_DIR)
        self.loading_image=False;self.levels_initialized=False
        self.debounce=QTimer(self);self.debounce.setSingleShot(True);self.debounce.setInterval(35)
        self.debounce.timeout.connect(self.load_requested_layer)
        self.build_ui()

    def button(self,text,slot):
        b=QPushButton(text);b.clicked.connect(slot);return b

    def build_ui(self):
        bar=QToolBar('ファイルと表示',self);bar.setMovable(False)
        self.addToolBar(bar)
        self.open_action=bar.addAction('ファイルを開く',self.open_file_dialog)
        self.open_action.setShortcut(QKeySequence.StandardKey.Open)
        a=bar.addAction('フォルダを開く',self.open_folder_dialog);a.setShortcut('Ctrl+Shift+O')
        bar.addSeparator()
        self.full_action=bar.addAction('表示層をPNG保存',lambda:self.save_dialog(False));self.full_action.setShortcut('Ctrl+S')
        self.crop_action=bar.addAction('選択範囲をPNG保存',lambda:self.save_dialog(True));self.crop_action.setShortcut('Ctrl+Shift+S')
        bar.addSeparator()
        a=bar.addAction('全体表示',self.fit_all);a.setShortcut('F')
        a=bar.addAction('100%',lambda:self.view.actual_size());a.setShortcut('1')
        self.zoom_label=QLabel('  ― % ');bar.addWidget(self.zoom_label)
        bar.addSeparator()
        a=bar.addAction('使い方',self.show_help)
        split=QSplitter(Qt.Orientation.Horizontal)
        self.setCentralWidget(split)
        panel=QWidget();left=QVBoxLayout(panel);left.setContentsMargins(12,12,12,12);left.setSpacing(12)
        title=QLabel('SPNG Viewer');title.setStyleSheet('font-size:21px;font-weight:600;color:#174b69')
        left.addWidget(title)
        self.source_label=QLabel('spngファイル、またはDATA / IMAGEフォルダを開いてください。')
        self.source_label.setWordWrap(True);self.source_label.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse)
        left.addWidget(self.source_label)
        group=QGroupBox('画像の選択');form=QFormLayout(group)
        self.container_combo=QComboBox();self.sensor_combo=QComboBox();self.layer_combo=QComboBox()
        self.kind_combo=QComboBox();self.view_combo=QComboBox();self.file_combo=QComboBox()
        for combo in (self.container_combo,self.sensor_combo,self.layer_combo,self.kind_combo,self.view_combo,self.file_combo):
            combo.setMaxVisibleItems(12)
            combo.setStyleSheet('QComboBox { combobox-popup: 0; }')
            combo.setMinimumContentsLength(10)
            combo.setSizeAdjustPolicy(QComboBox.SizeAdjustPolicy.AdjustToMinimumContentsLengthWithIcon)
        self.view_combo.setEditable(True);self.view_combo.setInsertPolicy(QComboBox.InsertPolicy.NoInsert)
        self.view_combo.completer().setFilterMode(Qt.MatchFlag.MatchContains)
        for label,widget in [('画像フォルダ',self.container_combo),('センサー',self.sensor_combo),('Layer',self.layer_combo),('画像種別',self.kind_combo),('View（入力可）',self.view_combo),('ファイル',self.file_combo)]:form.addRow(label,widget)
        self.container_combo.currentIndexChanged.connect(self.container_changed)
        self.sensor_combo.currentIndexChanged.connect(self.sensor_changed)
        self.layer_combo.currentIndexChanged.connect(self.layer_changed)
        self.kind_combo.currentIndexChanged.connect(self.kind_changed)
        self.view_combo.currentIndexChanged.connect(self.view_changed)
        self.view_combo.lineEdit().returnPressed.connect(self.typed_view)
        self.file_combo.currentIndexChanged.connect(self.file_changed)
        left.addWidget(group)
        self.meta=QLabel('画像なし');self.meta.setWordWrap(True);self.meta.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse)
        left.addWidget(self.meta)
        roi_group=QGroupBox('切り出し範囲［pixel］');rf=QFormLayout(roi_group)
        origin=QLabel('原点：左下　／　X → 右、Y → 上\nX・Yは範囲の左下。幅・高さは画素数。');origin.setWordWrap(True)
        rf.addRow(origin)
        self.roi_x=QSpinBox();self.roi_y=QSpinBox();self.roi_w=QSpinBox();self.roi_h=QSpinBox()
        for box in (self.roi_x,self.roi_y,self.roi_w,self.roi_h):box.setRange(0,1000000);box.setKeyboardTracking(False)
        self.roi_w.setMinimum(1);self.roi_h.setMinimum(1)
        for key,box in [('X',self.roi_x),('Y',self.roi_y),('幅',self.roi_w),('高さ',self.roi_h)]:rf.addRow(key,box)
        row=QHBoxLayout();self.apply_roi_button=self.button('座標から範囲を指定',self.apply_roi);row.addWidget(self.apply_roi_button)
        row.addWidget(self.button('解除',lambda:self.view.set_roi(None)));rf.addRow(row)
        row=QHBoxLayout();row.addWidget(self.button('選択範囲へ拡大',lambda:self.view.fit_roi()))
        self.crop_button=self.button('範囲をPNG保存',lambda:self.save_dialog(True));row.addWidget(self.crop_button);rf.addRow(row)
        self.roi_label=QLabel('範囲未選択');self.roi_label.setWordWrap(True);rf.addRow(self.roi_label)
        left.addWidget(roi_group)
        levels_group=QGroupBox('表示の濃淡（保存画素値には影響しません）');lf=QFormLayout(levels_group)
        self.low=QDoubleSpinBox();self.high=QDoubleSpinBox()
        for b in (self.low,self.high):b.setRange(-2147483648,2147483647);b.setDecimals(1);b.setKeyboardTracking(False)
        self.high.setValue(255)
        self.low.valueChanged.connect(self.refresh_display);self.high.valueChanged.connect(self.refresh_display)
        lf.addRow('黒に対応する値',self.low);lf.addRow('白に対応する値',self.high)
        self.invert=QCheckBox('表示だけ白黒反転');self.invert.toggled.connect(self.refresh_display);lf.addRow(self.invert)
        lf.addRow(self.button('元の階調へ戻す',self.reset_levels))
        hint=QLabel('0/1画像は白＝1、黒＝0で表示します。\n保存するPNGは元の値（0/1を含む）です。');hint.setWordWrap(True);lf.addRow(hint)
        left.addWidget(levels_group)
        tip=QLabel('左ドラッグ：画像を移動\nCtrl＋左ドラッグ：範囲選択\nホイール：層の切り替え\nCtrl＋ホイール：拡大・縮小\n← → ／ PageUp・PageDown：層の切り替え')
        tip.setWordWrap(True);left.addWidget(tip);left.addStretch(1)
        scroller=QScrollArea();scroller.setWidgetResizable(True);scroller.setWidget(panel);scroller.setMinimumWidth(400)
        split.addWidget(scroller)
        right=QWidget();rl=QVBoxLayout(right);rl.setContentsMargins(8,10,12,8)
        self.filename=QLabel('画像を開くと、ここにファイル名が表示されます。')
        self.filename.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse)
        self.filename.setWordWrap(True);rl.addWidget(self.filename)
        self.view=ImageView();self.view.setMinimumSize(300,250);rl.addWidget(self.view,1)
        self.view.roi_changed.connect(self.roi_changed);self.view.cursor_changed.connect(self.cursor_position)
        self.view.layer_step.connect(self.step_layer);self.view.zoom_changed.connect(lambda z:self.zoom_label.setText(f'  {z:.0f}% '))
        navigation=QHBoxLayout()
        self.prev_button=self.button('◀ 前の層',lambda:self.step_layer(-1))
        self.next_button=self.button('次の層 ▶',lambda:self.step_layer(1))
        self.layer_slider=QSlider(Qt.Orientation.Horizontal);self.layer_slider.setRange(0,0)
        self.layer_number=QSpinBox();self.layer_number.setRange(0,0);self.layer_number.setKeyboardTracking(False)
        self.layer_range=QLabel('/ ―')
        self.layer_slider.valueChanged.connect(self.slider_changed)
        self.layer_number.valueChanged.connect(self.spin_changed)
        navigation.addWidget(self.prev_button);navigation.addWidget(self.layer_slider,1)
        navigation.addWidget(QLabel('層'));navigation.addWidget(self.layer_number);navigation.addWidget(self.layer_range);navigation.addWidget(self.next_button)
        rl.addLayout(navigation)
        self.state_label=QLabel('準備完了');self.state_label.setWordWrap(True);rl.addWidget(self.state_label)
        self.cursor_label=QLabel('カーソル［左下原点・pixel］ X: ―　Y: ―　画素値: ―')
        self.cursor_label.setStyleSheet('background:#eaf2f6;padding:8px;font-size:14px;color:#16384d;')
        self.cursor_label.setTextInteractionFlags(Qt.TextInteractionFlag.TextSelectableByMouse);rl.addWidget(self.cursor_label)
        split.addWidget(right);split.setSizes([440,1010]);split.setStretchFactor(0,0);split.setStretchFactor(1,1)
        self.setAcceptDrops(True)
        self.update_controls()
        for key,step in [('Left',-1),('Right',1),('PgUp',-1),('PgDown',1)]:
            a=QAction(self);a.setShortcut(QKeySequence(key));a.triggered.connect(lambda checked=False,s=step:self.step_layer(s));self.addAction(a)
        a=QAction(self);a.setShortcut('Esc');a.triggered.connect(lambda:self.view.set_roi(None));self.addAction(a)

    def run_job(self, channel, fn, callback):
        self.serial+=1;token=self.serial;self.tokens[channel]=token
        job=Job(token,fn);self.jobs[token]=job
        def finish(t,value,error):
            self.jobs.pop(t,None)
            if self.tokens.get(channel)!=t:
                return
            if error:
                self.on_job_error(channel,error)
                return
            callback(value)
        job.signals.done.connect(finish)
        self.pool.start(job)
        return token

    def invalidate(self,*channels):
        for ch in channels:self.tokens[ch]=-1

    def on_job_error(self,channel,error):
        self.loading_image=False
        if channel in ('open','frame'):
            self.image=None;self.loaded_index=None;self.view.clear_image()
        self.state_label.setText('読み込みに失敗しました。')
        logging.error('%s: %s\n%s',channel,error[0],error[1])
        self.update_controls()
        QMessageBox.warning(self,'読み込みエラー',error[0])

    def open_file_dialog(self):
        path,_=QFileDialog.getOpenFileName(self,'spngファイルを開く',self.last_open_dir,'SPNG画像 (*.spng *.SPNG);;すべてのファイル (*)')
        if path:self.open_path(path)

    def open_folder_dialog(self):
        path=QFileDialog.getExistingDirectory(self,'DATA / IMAGE / センサーフォルダを開く',self.last_open_dir)
        if path:self.open_path(path)

    def open_path(self,path):
        path=Path(path)
        self.last_open_dir=str(path if path.is_dir() else path.parent)
        self.invalidate('root','sensors','files','open','frame');self.debounce.stop()
        self.clear_stack()
        for c in (self.container_combo,self.sensor_combo,self.layer_combo,self.kind_combo,self.view_combo,self.file_combo):
            with QSignalBlocker(c):c.clear()
            c.setEnabled(False)
        self.records=[]
        self.source_label.setText(self.fontMetrics().elidedText(str(path),Qt.TextElideMode.ElideMiddle,340));self.source_label.setToolTip(str(path))
        if path.is_dir():
            self.state_label.setText('フォルダを確認中…')
            self.run_job('root',lambda:discover_root(path),self.root_ready)
        else:
            self.open_stack(path)

    def root_ready(self,info):
        self.set_combo(self.container_combo,[name for name,path in info['containers']],None,[str(path) for name,path in info['containers']])
        self.container_changed()

    def set_combo(self,combo,texts,preferred=None,data=None):
        with QSignalBlocker(combo):
            combo.clear()
            for i,text in enumerate(texts):combo.addItem(str(text),data[i] if data is not None else str(text))
            idx=combo.findText(str(preferred)) if preferred is not None else -1
            combo.setCurrentIndex(idx if idx>=0 else (0 if texts else -1))
            if combo.isEditable() and combo.count()==0:combo.setEditText('')
        combo.setEnabled(bool(texts))

    def container_changed(self,*_):
        path=self.container_combo.currentData()
        if not path:return
        self.invalidate('sensors','files','open','frame');self.clear_stack()
        self.sensor_combo.setEnabled(False)
        for c in (self.layer_combo,self.kind_combo,self.view_combo,self.file_combo):c.setEnabled(False)
        self.state_label.setText('センサーを確認中…')
        self.run_job('sensors',lambda:discover_sensors(path),self.sensors_ready)

    def sensors_ready(self,sensors):
        self.set_combo(self.sensor_combo,[s[0] for s in sensors],None,[str(s[1]) for s in sensors])
        self.sensor_changed()

    def sensor_changed(self,*_):
        path=self.sensor_combo.currentData()
        if not path:return
        self.invalidate('files','open','frame');self.clear_stack()
        for c in (self.layer_combo,self.kind_combo,self.view_combo,self.file_combo):c.setEnabled(False)
        self.state_label.setText('このセンサーのファイルを一覧中…')
        self.run_job('files',lambda:list_sensor_files(path),self.files_ready)

    def files_ready(self,records):
        old=self.layer_combo.currentText();self.records=records
        self.set_combo(self.layer_combo,sorted(set(r.layer for r in records),key=natural_key),old)
        self.layer_changed()
        if not records:
            self.state_label.setText('このセンサーにはspngがありません。別のセンサーまたはフォルダを選んでください。')

    def layer_changed(self,*_):
        layer=self.layer_combo.currentText()
        kinds=sorted(set(r.kind for r in self.records if r.layer==layer),key=natural_key)
        old=self.kind_combo.currentText()
        preferred=old if old in kinds else next((k for k in kinds if k.lower().startswith('raw')),None)
        self.set_combo(self.kind_combo,kinds,preferred)
        self.kind_changed()

    def kind_changed(self,*_):
        layer,kind=self.layer_combo.currentText(),self.kind_combo.currentText()
        views=sorted(set(r.view for r in self.records if r.layer==layer and r.kind==kind),key=natural_key)
        self.set_combo(self.view_combo,views,self.view_combo.currentText())
        self.view_changed()

    def typed_view(self):
        text=self.view_combo.currentText().strip()
        if text.isdigit():text=str(int(text))
        idx=self.view_combo.findText(text,Qt.MatchFlag.MatchExactly)
        if idx<0:
            self.state_label.setText(f'View {text} は、このLayer・画像種別にはありません。')
            return
        if idx==self.view_combo.currentIndex():self.view_changed()
        else:self.view_combo.setCurrentIndex(idx)

    def view_changed(self,*_):
        layer,kind,view=self.layer_combo.currentText(),self.kind_combo.currentText(),self.view_combo.currentText()
        recs=[r for r in self.records if r.layer==layer and r.kind==kind and r.view==view]
        self.set_combo(self.file_combo,[r.path.name for r in recs],None,[str(r.path) for r in recs])
        self.file_changed()

    def file_changed(self,*_):
        path=self.file_combo.currentData()
        if path:self.open_stack(Path(path))
        else:self.clear_stack()

    def clear_stack(self):
        self.debounce.stop();self.stack=None;self.image=None;self.loaded_index=None;self.current_path=None
        self.cache.clear();self.cache_bytes=0;self.view.clear_image();self.levels_initialized=False
        self.meta.setText('画像なし');self.roi_label.setText('範囲未選択')
        self.filename.setText('画像を選択してください。')
        self.layer_range.setText('/ ―');self.update_controls()

    def open_stack(self,path):
        self.invalidate('open','frame');self.clear_stack()
        self.current_path=Path(path)
        self.filename.setText(self.current_path.name);self.filename.setToolTip(str(self.current_path))
        self.state_label.setText('層の索引を読み込み中…')
        self.setWindowTitle(f'{self.current_path.name} — SPNG Viewer')
        self.run_job('open',lambda:SpngStack(path),self.stack_ready)

    def stack_ready(self,stack):
        self.stack=stack
        with QSignalBlocker(self.layer_slider):self.layer_slider.setRange(0,len(stack)-1);self.layer_slider.setValue(stack.first_available())
        with QSignalBlocker(self.layer_number):self.layer_number.setRange(0,len(stack)-1);self.layer_number.setValue(stack.first_available())
        self.layer_range.setText(f'/ 0-{len(stack)-1}')
        self.update_controls();self.load_requested_layer()

    def slider_changed(self,value):
        with QSignalBlocker(self.layer_number):self.layer_number.setValue(value)
        self.request_layer()

    def spin_changed(self,value):
        with QSignalBlocker(self.layer_slider):self.layer_slider.setValue(value)
        self.request_layer()

    def request_layer(self):
        self.invalidate('frame')
        self.image=None;self.loaded_index=None;self.loading_image=True
        # Keep the last decoded frame and its ROI at unchanged brightness until the new frame is ready.
        self.cursor_position(None)
        self.state_label.setText(f'層 {self.layer_number.value()} を読み込み中…')
        self.update_controls();self.debounce.start()

    def step_layer(self,step):
        if self.stack:self.layer_number.setValue(max(0,min(len(self.stack)-1,self.layer_number.value()+step)))

    def load_requested_layer(self):
        if not self.stack:return
        index=self.layer_number.value();stack=self.stack
        self.loading_image=True;self.update_controls()
        if index in self.cache:
            im=self.cache.pop(index);self.cache[index]=im
            self.frame_ready((index,im));return
        self.run_job('frame',lambda:(index,stack.read_frame(index)),self.frame_ready)

    def frame_ready(self,pair):
        index,image=pair
        if not self.stack or index!=self.layer_number.value():return
        self.loading_image=False
        previous_size=self.view.image_size
        self.image=image;self.loaded_index=index
        if image is None:
            self.view.clear_image()
            self.state_label.setText(f'層 {index} は未保存・欠損です。別の層を選択してください。')
            self.meta.setText(f'{len(self.stack)} 層（0-{len(self.stack)-1}）／ この層は欠損')
            self.roi_label.setText('範囲未選択');self.update_controls();return
        if index not in self.cache:
            amount=np.asarray(image).nbytes
            if amount<=self.cache_limit:
                while self.cache and (self.cache_bytes+amount>self.cache_limit or len(self.cache)>=8):
                    _,old=self.cache.popitem(last=False);self.cache_bytes-=np.asarray(old).nbytes
                self.cache[index]=image;self.cache_bytes+=amount
        if not self.levels_initialized:
            self.levels_initialized=True;self.set_levels(*default_levels(image))
        array=display_array(image,self.low.value(),self.high.value(),self.invert.isChecked())
        self.view.pixmap_item.setOpacity(1)
        self.view.set_image(array,reset=(previous_size!=image.size))
        if self.view.roi:self.view.roi_item.show()
        else:self.roi_label.setText('範囲未選択')
        w,h=image.size
        for b,m in [(self.roi_x,w-1),(self.roi_y,h-1),(self.roi_w,w),(self.roi_h,h)]:b.setMaximum(m)
        record=self.stack.frames[index]
        missing=sum(f.missing for f in self.stack.frames)
        format_name={'L':'8bitグレースケール','1':'1bit二値','RGB':'RGBカラー','RGBA':'RGBカラー＋透明度','LA':'グレースケール＋透明度','P':'パレットカラー'}.get(image.mode,f'{record.depth}bitグレースケール' if image.mode.startswith('I') else image.mode)
        self.meta.setText(f'{w} × {h} pixel　／　PNG {record.depth} bit\n{len(self.stack)} 層（0-{len(self.stack)-1}）　欠損 {missing} 層\n形式：{format_name}　／　原点：左下')
        self.state_label.setText(f'層 {index} / 0-{len(self.stack)-1}　表示中')
        self.update_controls()

    def set_levels(self,low,high):
        with QSignalBlocker(self.low):self.low.setValue(low)
        with QSignalBlocker(self.high):self.high.setValue(high)

    def reset_levels(self):
        if self.image:
            self.set_levels(*default_levels(self.image))
            with QSignalBlocker(self.invert):self.invert.setChecked(False)
            self.refresh_display()

    def refresh_display(self,*_):
        if not self.image:return
        if self.low.value()>=self.high.value():
            self.state_label.setText('白に対応する値を、黒に対応する値より大きくしてください。')
            return
        self.view.set_image(display_array(self.image,self.low.value(),self.high.value(),self.invert.isChecked()))

    def fit_all(self):self.view.fit_all()

    def apply_roi(self):
        if not self.image:return
        try:box=bottom_left_roi_to_box(self.roi_x.value(),self.roi_y.value(),self.roi_w.value(),self.roi_h.value(),self.image.size)
        except ValueError as exc:
            QMessageBox.warning(self,'範囲を確認してください',str(exc));return
        self.view.set_roi(box)

    def roi_changed(self,box):
        if not box or not self.image:
            self.roi_label.setText('範囲未選択');self.update_controls();return
        x,y,w,h=box_to_bottom_left_roi(box,self.image.size)
        for spin,value in [(self.roi_x,x),(self.roi_y,y),(self.roi_w,w),(self.roi_h,h)]:
            with QSignalBlocker(spin):spin.setValue(value)
        self.roi_label.setText(f'左下 ({x}, {y})　／　{w} × {h} pixel\nX: {x}-{x+w-1}、Y: {y}-{y+h-1}')
        self.update_controls()

    def cursor_position(self,point):
        text='カーソル［左下原点・pixel］ X: ―　Y: ―　画素値: ―'
        if point is not None and self.image:
            x,top=math.floor(point[0]),math.floor(point[1]);w,h=self.image.size
            if 0<=x<w and 0<=top<h:
                y=h-1-top;value=self.image.getpixel((x,top))
                text=f'カーソル［左下原点・pixel］ X: {x}　Y: {y}　画素値: {value}'
        self.cursor_label.setText(text)

    def update_controls(self):
        has_image=self.image is not None and not self.loading_image
        for widget in [self.full_action,self.apply_roi_button]:widget.setEnabled(has_image)
        for widget in [self.crop_action,self.crop_button]:widget.setEnabled(has_image and bool(self.view.roi))
        for widget in [self.layer_slider,self.layer_number,self.prev_button,self.next_button]:widget.setEnabled(self.stack is not None)
        if self.stack:
            self.prev_button.setEnabled(self.layer_number.value()>0)
            self.next_button.setEnabled(self.layer_number.value()<len(self.stack)-1)

    def save_dialog(self,crop):
        if self.image is None or self.loaded_index!=self.layer_number.value():return
        roi=self.view.roi if crop else None
        if crop and not roi:return
        name=f'{self.current_path.stem}_z{self.loaded_index:03}'
        if roi:
            x,y,w,h=box_to_bottom_left_roi(roi,self.image.size)
            name+=f'_x{x}_y{y}_w{w}_h{h}'
        path,_=QFileDialog.getSaveFileName(self,'選択範囲をPNG保存' if crop else '表示している層をPNG保存',str(Path(self.last_save_dir)/(name+'.png')),'PNG画像 (*.png)')
        if not path:return
        if Path(path).suffix.lower()!='.png':
            target=Path(path).with_suffix('.png')
            if target.exists() and QMessageBox.question(self,'上書き確認',f'{target.name} は存在します。上書きしますか？')!=QMessageBox.StandardButton.Yes:return
            path=str(target)
        try:
            saved=save_png(self.image,path,roi)
        except Exception as exc:
            QMessageBox.warning(self,'保存できませんでした',str(exc));return
        self.last_save_dir=str(saved.parent)
        self.state_label.setText(f'元の画素値でPNG保存しました：{saved}')

    def dragEnterEvent(self,event):
        if event.mimeData().hasUrls() and any(u.isLocalFile() for u in event.mimeData().urls()):event.acceptProposedAction()

    def dropEvent(self,event):
        paths=[u.toLocalFile() for u in event.mimeData().urls() if u.isLocalFile()]
        if paths:self.open_path(paths[0]);event.acceptProposedAction()

    def show_help(self):
        QMessageBox.information(self,'SPNG Viewer の使い方',
            '1. spngファイル、DATA / IMAGE、またはセンサーフォルダを開きます。\n'
            '2. フォルダを開いた場合はセンサー・Layer・画像種別・Viewを選びます。Viewは番号を入力してEnterでも切り替えられます。\n'
            '3. ホイール・矢印・スライダーで層を切り替えます。層番号はファイル中の0始まりの番号です。\n'
            '4. Ctrl＋左ドラッグ、またはX・Y・幅・高さの入力で範囲を指定します。X・Yは左下原点のpixel座標です。\n'
            '5. 表示層全体、または範囲内だけをPNGに保存できます。選択した画像データの画素値が保存され、枠や表示濃淡は入りません。\n\n'
            '左下pixelの座標は(0,0)、左上は(0,高さ−1)です。PNGの見た目の上下は反転しません。\n'
            'Ctrl＋ホイールで拡大縮小、左ドラッグで移動、Fで全体表示、1で100%表示、Escで選択解除。\n'
            '初期状態では0-255、16bit画像は0-65535、0/1画像は0-1を表示範囲にします。保存は元の階調です。\n\n'
            '各タブの機能：再生・追従タブで自動再生・ループ・2点による中心追従。重ね合わせ・出力タブで任意の層の最大/最小合成、層番号色分け、複数PNGの出力。画像処理タブでLayer別のスキャン設定を読み込み、全層へフィルタを適用して段階ごとに表示できます。')

    def closeEvent(self,event):
        self.invalidate('root','sensors','files','open','frame')
        self.debounce.stop()
        event.accept()


BaseViewer=Viewer
from spng_features import EnhancementMixin
class Viewer(EnhancementMixin,BaseViewer):
    pass


def initialize_fonts(app):
    # Use the OS general UI font and its configured size; no fixed family or font files.
    app.setFont(QFontDatabase.systemFont(QFontDatabase.SystemFont.GeneralFont))


def main():
    parser=argparse.ArgumentParser(description='SPNG Viewer — file/folder browsing and original-pixel PNG export')
    parser.add_argument('path',nargs='?',help='spngファイル、DATA/IMAGE/センサーフォルダ')
    parser.add_argument('--self-test',metavar='REPORT_JSON',help=argparse.SUPPRESS)
    parser.add_argument('--self-test-input',metavar='SPNG',help=argparse.SUPPRESS)
    args=parser.parse_args()
    if args.self_test and Path(args.self_test).suffix.lower()!='.json':
        parser.error('--self-test requires a .json report path; image paths are not output paths')
    try:
        logging.basicConfig(filename=APP_DIR/'viewer.log',level=logging.WARNING,encoding='utf8',format='%(asctime)s %(levelname)s %(message)s')
    except OSError:
        # Opening images must also work from a read-only application folder.
        logging.basicConfig(handlers=[logging.NullHandler()],level=logging.WARNING)
    app=QApplication(sys.argv[:1]);app.setApplicationName('SPNG Viewer');initialize_fonts(app)
    app.setStyle('Fusion')
    if args.self_test:
        from spng_smoke import run_smoke
        return run_smoke(app,Viewer,args.self_test,args.self_test_input)
    win=Viewer();win.show()
    if args.path:QTimer.singleShot(0,lambda:win.open_path(args.path))
    return app.exec()

if __name__=='__main__':
    raise SystemExit(main())
