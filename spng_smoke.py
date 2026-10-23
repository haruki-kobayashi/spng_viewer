"""Packaged-app integration check, usable from the standalone executable."""
from pathlib import Path
from io import BytesIO
import json
import struct
import sys
import time
import traceback
import numpy as np
from PIL import Image
from PySide6.QtCore import QThread
from PySide6.QtGui import QFontDatabase
from spng_core import SpngStack, save_png, bottom_left_roi_to_box


def run_smoke(app, viewer_class, report_path, input_path=None):
    report_path=Path(report_path).resolve()
    if report_path.suffix.lower()!='.json':
        raise ValueError('Self-test report must have .json extension')
    report_path.parent.mkdir(parents=True,exist_ok=True)
    work=report_path.parent/'smoke_files';work.mkdir(exist_ok=True)
    result={'frozen':bool(getattr(sys,'frozen',False)),'executable':sys.executable,'bundle_root':getattr(sys,'_MEIPASS',None),'checks':{}}
    win=None
    try:
        import PySide6,PIL,numpy
        from spng_ops import cv2
        result['module_locations']={m.__name__:str(m.__file__) for m in [PySide6,PIL,numpy,cv2]}
        if result['frozen']:
            bundle=Path(sys._MEIPASS).resolve()
            assert all(Path(p).resolve().is_relative_to(bundle) for p in result['module_locations'].values())
        result['checks']['bundled_modules_only']=True
        standard=QFontDatabase.systemFont(QFontDatabase.SystemFont.GeneralFont)
        assert app.font().family()==standard.family() and app.font().pointSizeF()==standard.pointSizeF()
        result['system_font']={'family':app.font().family(),'points':app.font().pointSizeF()}
        result['checks']['system_font']=True
        a=np.arange(35,dtype=np.uint8).reshape(5,7)
        source=work/'synthetic.spng'
        with source.open('wb') as f:
            for pixels in [a,None,a+100]:
                if pixels is None:
                    f.write(struct.pack('<Q',(1<<64)-1));continue
                bio=BytesIO();Image.fromarray(pixels).save(bio,'PNG');payload=bio.getvalue()
                f.write(struct.pack('<Q',len(payload)));f.write(payload)
        win=viewer_class();win.show();app.processEvents()
        def wait_for(condition):
            until=time.monotonic()+25
            while time.monotonic()<until:
                app.processEvents()
                if condition():return
                QThread.msleep(5)
            raise RuntimeError('GUI timeout: '+win.state_label.text())
        win.open_path(source);wait_for(lambda:win.image is not None)
        np.testing.assert_array_equal(np.asarray(win.image),a)
        win.cursor_position((.1,4.1))
        assert 'X: 0　Y: 0' in win.cursor_label.text() and win.cursor_label.text().endswith('28')
        result['checks']['bottom_left_cursor']=True
        box=bottom_left_roi_to_box(1,0,3,2,win.image.size)
        saved=save_png(win.image,work/'bottom_left_crop.png',box)
        np.testing.assert_array_equal(np.asarray(Image.open(saved)),a[3:5,1:4])
        result['checks']['original_pixel_crop']=True
        before=win.view.pixmap_item.pixmap().toImage()
        win.layer_number.setValue(2)
        assert win.view.pixmap_item.opacity()==1 and win.view.pixmap_item.pixmap().toImage()==before
        assert not win.full_action.isEnabled()
        wait_for(lambda:win.loaded_index==2 and win.image is not None)
        np.testing.assert_array_equal(np.asarray(win.image),a+100)
        result['checks']['unchanged_brightness_and_layer_switch']=True
        win.layer_number.setValue(1);wait_for(lambda:win.loaded_index==1)
        assert win.image is None and not win.full_action.isEnabled()
        result['checks']['missing_layer']=True
        dataset=work/'folder_test'
        for sensor,view in [('00_00',0),('00_01',12)]:
            folder=dataset/'IMAGE'/sensor;folder.mkdir(parents=True,exist_ok=True)
            for layer in [0,1]:
                (folder/f'Raw_0_{view:08}_{layer}_003.spng').write_bytes(source.read_bytes())
        (dataset/'DATA'/'00_00').mkdir(parents=True,exist_ok=True)
        paramdir=dataset/'PARAMS';paramdir.mkdir(exist_ok=True)
        param={'ImageFilterParam':{'Algorithm':'ImageFilterWithInterpolation_GPU','CommonParamArray':[
            dict(ResizeFactor=1.41421356237309,NormCutOffFreq=.1,WeightZFilter=-.26,ThrBinary=14,ThrBinaryPlus=-5,Expansion=2),
            dict(ResizeFactor=1.5,NormCutOffFreq=.12,WeightZFilter=-.28,ThrBinary=13,ThrBinaryPlus=-6,Expansion=3)]}}
        (paramdir/'CombinedParam.json').write_text(json.dumps(param),encoding='utf-8')
        win.open_path(dataset);wait_for(lambda:win.image is not None and not win._scan_pending)
        assert win.container_combo.currentText()=='IMAGE'
        win.sensor_combo.setCurrentText('00_01')
        wait_for(lambda:win.image is not None and win.view_combo.currentText()=='12')
        assert win.current_path.name=='Raw_0_00000012_0_003.spng'
        result['checks']['folder_sensor_view_browsing']=True
        wait_for(lambda:not win._scan_pending)
        assert win.threshold_box.value()==9 and win.filter_config().scale==1.41421356237309
        win.layer_combo.setCurrentText('Layer1')
        wait_for(lambda:win.image is not None and not win._scan_pending)
        assert win._scan_settings.layer==1 and win.threshold_box.value()==7 and win.z_weight.value()==-.28, (str(win.current_path), win.layer_combo.currentText(), win._scan_settings.metadata() if win._scan_settings else None, win.threshold_box.value(), win.z_weight.value())
        win.layer_combo.setCurrentText('Layer0')
        wait_for(lambda:win.image is not None and not win._scan_pending)
        assert win.threshold_box.value()==9
        result['checks']['layer_specific_scan_parameter_loading']=True
        win.scan_auto.setChecked(False)
        # Exercise stack processing from the packaged executable too.
        win.resize_check.setChecked(False);win.highpass_check.setChecked(False);win.z_check.setChecked(False);win.threshold_box.setValue(5);win.dilate_box.setValue(1)
        win.start_processing();wait_for(lambda:win._processed is not None and win.image is not None and not win.loading_image and not win._bulk_running)
        assert win.stage_combo.currentText()=='二値化後'
        np.testing.assert_array_equal(np.asarray(win.image),(a>5).astype(np.uint8))
        result['checks']['whole_stack_processing_and_stage_cache']=True
        win.stage_combo.setCurrentText('元画像');wait_for(lambda:win.image is not None and not win.loading_image)
        win.layer_spec.setText('0-2');win.start_projection('max');wait_for(lambda:win._projection is not None and not win._bulk_running)
        np.testing.assert_array_equal(win._projection.values,a+100)
        np.testing.assert_array_equal(win._projection.layers,np.full(a.shape,2,np.int32))
        win.depth_check.setChecked(True);assert win.image.mode=='RGB'
        result['checks']['max_projection_and_depth_color']=True
        win.return_to_slice();wait_for(lambda:win.image is not None and not win.loading_image)
        from spng_ops import export_layers,line_position
        export_dir=work/('batch_'+str(time.time_ns()))
        exported=export_layers(win.read_source()[0],[0,1,2],export_dir,'test')
        assert exported['saved']==2 and exported['missing']==[1]
        np.testing.assert_array_equal(np.asarray(Image.open(export_dir/'test_z002.png')),a+100)
        result['checks']['multiple_layer_PNG_export']=True
        for tag,values in enumerate([(0,1,1),(2,4,3)]):
            for spin,val in zip(win.point_fields[tag],values):spin.setValue(val)
            win.register_point(tag)
        win.layer_number.setValue(2);wait_for(lambda:win.loaded_index==2 and not win.loading_image)
        from PySide6.QtCore import QPointF
        actual=win.view.mapFromScene(QPointF(4.5,5-.5-3));center=win.view.viewport().rect().center()
        assert abs(actual.x()-center.x())<=2 and abs(actual.y()-center.y())<=2
        result['checks']['two_point_center_tracking']=True
        win.follow_check.setChecked(False);win.preload_check.setChecked(False);win.loop_check.setChecked(False);win.play_start.setValue(0);win.play_end.setValue(2);win.play_step.setValue(2);win.play_rate.setValue(60)
        win.toggle_play();wait_for(lambda:not win._playing and win.loaded_index==2)
        result['checks']['automatic_playback_and_stop']=True
        if input_path:
            external=Path(input_path);stat=(external.stat().st_size,external.stat().st_mtime_ns)
            win.open_path(external);wait_for(lambda:win.image is not None)
            index=min(25,len(win.stack)-1)
            if win.stack.frames[index].missing:index=win.stack.first_available()
            win.layer_number.setValue(index);wait_for(lambda:win.image is not None and win.loaded_index==index)
            w,h=win.image.size
            size=(min(80,w),min(60,h));crop=bottom_left_roi_to_box(0,0,*size,win.image.size)
            out=save_png(win.image,work/'real_source_crop.png',crop)
            np.testing.assert_array_equal(np.asarray(Image.open(out)),np.asarray(win.image)[h-size[1]:h,:size[0]])
            assert stat==(external.stat().st_size,external.stat().st_mtime_ns)
            result['checks']['real_SP​NG_open_and_crop'.replace('\u200b','')]=True
            result['real_input']={'path':str(external),'layers':len(win.stack),'tested_layer':index,'size':list(win.image.size),'source_stat_unchanged':True}
            app.processEvents();win.grab().save(str(report_path.parent/'packaged_real_window.png'))
        result['success']=True
    except Exception:
        result['success']=False;result['error']=traceback.format_exc()
    finally:
        if win is not None:
            win.pool.waitForDone(30000);app.processEvents();win.close();app.processEvents()
        report_path.write_text(json.dumps(result,ensure_ascii=False,indent=2),'utf8')
    return 0 if result['success'] else 1
