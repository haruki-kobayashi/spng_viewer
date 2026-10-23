"""Scan-parameter UI integrated with the asynchronous viewer job lifecycle."""
import dataclasses
from pathlib import Path
from PySide6.QtCore import QSignalBlocker
from PySide6.QtWidgets import QCheckBox,QSpinBox,QHBoxLayout,QFileDialog,QLineEdit,QSizePolicy
from spng_ops import FilterConfig
from spng_scanparams import image_layer,find_parameter_file,read_settings,acquisition_directory

class ScanParameterMixin:
    def build_parameter_ui(self,layout):
        self._scan_settings=None;self._scan_exact={};self._scan_pending=False;self._scan_error=False
        self._scan_override=None;self._scan_override_scope=None
        self.scan_auto=QCheckBox('スキャン設定を自動読み込み（Layer別）');self.scan_auto.setChecked(True)
        layout.addWidget(self.scan_auto)
        row=QHBoxLayout()
        row.addWidget(self.button('設定JSONを選択',self.choose_scan_parameters))
        row.addWidget(self.button('自動検出をやり直す',self.reset_scan_parameters))
        self.scan_layer=QSpinBox();self.scan_layer.setRange(-1,999);self.scan_layer.setValue(-1)
        self.scan_layer.setSpecialValueText('設定Layer未指定');self.scan_layer.setPrefix('設定Layer ')
        layout.addLayout(row);layout.addWidget(self.scan_layer)
        self.scan_source=QLineEdit();self.scan_source.setReadOnly(True)
        self.scan_source.setPlaceholderText('読み込み元の設定ファイル');layout.addWidget(self.scan_source)
        self.scan_info=self.note(layout,'画像を開くとPARAMS/CombinedParam.jsonを検索します。')
        self.scan_info.setSizePolicy(QSizePolicy.Policy.Ignored,QSizePolicy.Policy.Preferred)
        self.scan_auto.toggled.connect(self.scan_mode_changed)
        self.scan_layer.valueChanged.connect(lambda _:self.reload_scan_parameters())

    def parameter_widgets(self):
        return {'scale':self.scale_box,'cutoff':self.cutoff_box,'z_weight':self.z_weight,
                'threshold':self.threshold_box,'dilation':self.dilate_box,'taps':self.tap_box}

    def set_parameter_widgets(self,config):
        self._scan_exact={}
        for name,widget in self.parameter_widgets().items():
            value=getattr(config,name)
            with QSignalBlocker(widget):widget.setValue(value)
            self._scan_exact[name]=(widget.value(),value)
        for widget,value in [(self.resize_check,config.scale!=1),(self.highpass_check,config.highpass),
                             (self.z_check,config.z_filter),(self.binary_check,config.binary),
                             (self.dilate_check,config.dilation>1)]:widget.setChecked(value)
        self.interp_combo.setCurrentText(config.interpolation);self.z_step.setCurrentIndex(config.z_step-1)

    def exact_parameter_config(self,config):
        values={}
        for name,(display,exact) in self._scan_exact.items():
            if self.parameter_widgets()[name].value()==display:values[name]=exact
        if not self.resize_check.isChecked():values.pop('scale',None)
        if not self.dilate_check.isChecked():values.pop('dilation',None)
        return dataclasses.replace(config,**values)

    def invalidate_scan_parameters(self):
        self.invalidate('params');self._scan_pending=False;self._scan_error=False
        self._scan_settings=None;self._scan_exact={}
        if hasattr(self,'scan_info'):
            self.scan_source.clear();self.scan_source.setToolTip('');self.scan_info.setText('画像を選択してください。')

    def scan_mode_changed(self,checked):
        if checked:self.reload_scan_parameters()
        else:
            self.invalidate('params');self._scan_pending=False;self._scan_error=False
            self.scan_info.setText('手動設定。現在の入力値を使用します。');self.update_controls()

    def choose_scan_parameters(self):
        filename,_=QFileDialog.getOpenFileName(self,'スキャン設定を選択','','JSON (*.json)')
        if filename:
            self._scan_override=Path(filename)
            self._scan_override_scope=acquisition_directory(self.current_path) if self.current_path else None
            with QSignalBlocker(self.scan_auto):self.scan_auto.setChecked(True)
            self.reload_scan_parameters()

    def reset_scan_parameters(self):
        self._scan_override=None;self._scan_override_scope=None
        with QSignalBlocker(self.scan_auto):self.scan_auto.setChecked(True)
        self.reload_scan_parameters()

    def reload_scan_parameters(self):
        if not getattr(self,'stack',None) or not self.scan_auto.isChecked():return
        path=Path(self.current_path);known_layer=image_layer(path)
        scope=acquisition_directory(path)
        if self._scan_override is not None:
            if self._scan_override_scope is not None and self._scan_override_scope!=scope:
                self._scan_override=None;self._scan_override_scope=None
            else:self._scan_override_scope=scope
        if known_layer is not None:
            with QSignalBlocker(self.scan_layer):self.scan_layer.setValue(known_layer)
        self.scan_layer.setEnabled(known_layer is None)
        layer=known_layer if known_layer is not None else self.scan_layer.value()
        self._scan_pending=True;self._scan_error=False;self._scan_settings=None
        self.scan_source.clear();self.scan_source.setToolTip('')
        self.set_parameter_widgets(FilterConfig(scale=1.4142))
        self.scan_info.setText(f'Layer{layer}のスキャン設定を検索中…');self.update_controls()
        override=self._scan_override
        def load():
            try:
                source=override or find_parameter_file(path)
                return (read_settings(source,layer) if source else None,None)
            except Exception as exc:return None,str(exc)
        def loaded(result):
            settings,error=result;self._scan_pending=False;self._scan_error=bool(error)
            if error:
                self.scan_info.setText(f'Layer{layer}: {error}\n別Layerの値は適用していません。手動設定は自動読み込みをOFFにしてください。')
            elif settings is None:
                self.scan_info.setText(f'Layer{layer}: 設定JSONが見つかりません。ビューア初期値を表示しています。')
            else:
                self._scan_settings=settings;self.set_parameter_widgets(settings.config)
                self.scan_source.setText(str(settings.source));self.scan_source.setToolTip(str(settings.source))
                self.scan_info.setText(f'Layer{layer}: 設定を読み込みました。\n二値化閾値 = {settings.threshold_base:g} + ({settings.threshold_plus:g}) = {settings.config.threshold:g}\nFIR tap=15、Z参照間隔=1。画像処理は「全層へ適用」で実行します。')
            self.update_controls()
        self.run_job('params',load,loaded)
