from pathlib import Path
from io import BytesIO
import json,os,struct,time,unittest
import numpy as np
from PIL import Image
from spng_scanparams import read_settings,find_parameter_file,image_layer,ScanParameterError

OUT=Path(__file__).parent/'_generated'/'scan_parameters'

def parameters():
    return {'ImageFilterParam':{'Algorithm':'ImageFilterWithInterpolation_GPU','CommonParamArray':[
        dict(WeightZFilter=-.26,ThrBinary=14,ThrBinaryPlus=-5,Expansion=2,NormCutOffFreq=.1,ResizeFactor=1.41421356237309),
        dict(WeightZFilter=-.28,ThrBinary=13,ThrBinaryPlus=-6,Expansion=3,NormCutOffFreq=.12,ResizeFactor=1.5)]}}

def write_json(path,data):
    path.parent.mkdir(parents=True,exist_ok=True);path.write_text(json.dumps(data),encoding='utf-8');return path

def stack(path):
    path.parent.mkdir(parents=True,exist_ok=True)
    with path.open('wb') as f:
        for z in range(3):
            a=np.full((24,32),200,np.uint8);a[8+z:12+z,12:16]=80
            buf=BytesIO();Image.fromarray(a).save(buf,'PNG');data=buf.getvalue()
            f.write(struct.pack('<Q',len(data)));f.write(data)
    return path

class ScanSettingsTests(unittest.TestCase):
    def setUp(self):
        self.directory=OUT/self._testMethodName;self.path=write_json(self.directory/'PARAMS/CombinedParam.json',parameters())
    def test_per_layer_values_and_threshold_sum(self):
        a=read_settings(self.path,0);b=read_settings(self.path,1)
        self.assertEqual((a.config.threshold,b.config.threshold),(9,7))
        self.assertEqual((a.config.z_weight,b.config.z_weight),(-.26,-.28))
        self.assertEqual(a.config.scale,1.41421356237309)
        self.assertEqual(b.config.cutoff,.12);self.assertEqual(b.config.dilation,3)
    def test_missing_layer_does_not_reuse_other_layer(self):
        with self.assertRaises(ScanParameterError):read_settings(self.path,2)
    def test_explicit_layer_ids_are_not_array_indices(self):
        data=parameters();array=data['ImageFilterParam']['CommonParamArray']
        array[0]['LayerID']=1;array[1]['LayerID']=0
        write_json(self.path,data)
        self.assertEqual(read_settings(self.path,0).config.threshold,7)
    def test_missing_and_invalid_values_fail(self):
        for key,value in [('WeightZFilter',float('nan')),('Expansion',2.5),('NormCutOffFreq',.9),('ThrBinary',500)]:
            with self.subTest(key=key):
                data=parameters();data['ImageFilterParam']['CommonParamArray'][0][key]=value;write_json(self.path,data)
                with self.assertRaises(ScanParameterError):read_settings(self.path,0)
        data=parameters();del data['ImageFilterParam']['CommonParamArray'][0]['ThrBinary'];write_json(self.path,data)
        with self.assertRaises(ScanParameterError):read_settings(self.path,0)
    def test_combined_priority_and_image_data_paths(self):
        write_json(self.directory/'ScanControllParam.json',{})
        for container in ['IMAGE','DATA']:
            image=self.directory/container/'00_00/Raw_0_00000000_1_003.spng'
            self.assertEqual(find_parameter_file(image),self.path)
            self.assertEqual(image_layer(image),1)
    def test_unrecognized_filename_and_no_cross_acquisition_search(self):
        image=self.directory/'other_scan/IMAGE/00_00/Raw_0_00000000_0_003.spng'
        write_json(self.directory/'other_scan/PARAMS/UnrelatedSettings.json',parameters())
        self.assertIsNone(find_parameter_file(image))
    def test_unknown_algorithm_and_layer_rejected(self):
        data=parameters();data['ImageFilterParam']['Algorithm']='SomethingElse';write_json(self.path,data)
        with self.assertRaises(ScanParameterError):read_settings(self.path,0)
        with self.assertRaises(ScanParameterError):read_settings(self.path,None)

class ScanSettingsGuiTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        os.environ.setdefault('QT_QPA_PLATFORM','offscreen')
        from PySide6.QtWidgets import QApplication
        cls.app=QApplication.instance() or QApplication([])
    def setUp(self):
        from spng_viewer import Viewer
        self.directory=OUT/self._testMethodName
        write_json(self.directory/'PARAMS/CombinedParam.json',parameters())
        self.paths=[stack(self.directory/f'IMAGE/00_00/Raw_0_00000000_{i}_003.spng') for i in range(3)]
        self.w=Viewer();self.w.show()
    def wait(self,condition):
        end=time.monotonic()+12
        while time.monotonic()<end:
            self.app.processEvents()
            if condition():return
            time.sleep(.005)
        self.fail(self.w.scan_info.text())
    def open(self,path):
        self.w.open_path(path)
        self.wait(lambda:self.w.image is not None and not self.w._scan_pending)
    def tearDown(self):
        self.w.pool.waitForDone(10000);self.app.processEvents();self.w.close();self.app.processEvents()
    def test_layer_change_exact_precision_and_processing_provenance(self):
        self.open(self.paths[0]);self.assertEqual(self.w.filter_config().scale,1.41421356237309)
        self.assertEqual(self.w.threshold_box.value(),9)
        self.open(self.paths[1]);self.assertEqual(self.w.threshold_box.value(),7)
        self.assertEqual(self.w.z_weight.value(),-.28);self.assertEqual(self.w.cutoff_box.value(),.12)
        self.w.start_processing();self.wait(lambda:self.w._processed is not None)
        context=self.w._processed.metadata['scan_parameters']
        self.assertEqual(context['layer'],1);self.assertEqual(context['effective_threshold'],7)
    def test_missing_layer_blocks_processing_and_clears_previous_values(self):
        self.open(self.paths[1]);self.open(self.paths[2])
        self.assertTrue(self.w._scan_error);self.assertFalse(self.w.process_button.isEnabled())
        self.assertEqual(self.w.threshold_box.value(),9)
        self.w.scan_auto.setChecked(False);self.assertTrue(self.w.process_button.isEnabled())
    def test_manual_mode_survives_file_changes(self):
        self.open(self.paths[0]);self.w.scan_auto.setChecked(False)
        self.w.threshold_box.setValue(20);self.open(self.paths[1])
        self.assertEqual(self.w.threshold_box.value(),20)
        self.w.scan_auto.setChecked(True)
        self.wait(lambda:not self.w._scan_pending)
        self.assertEqual(self.w.threshold_box.value(),7)
    def test_unknown_filename_requires_layer_selection(self):
        self.open(stack(self.directory/'IMAGE/00_00/unnamed.spng'))
        self.assertTrue(self.w._scan_error);self.assertEqual(self.w.scan_layer.value(),-1)
        self.w.scan_layer.setValue(1);self.wait(lambda:not self.w._scan_pending)
        self.assertFalse(self.w._scan_error);self.assertEqual(self.w.threshold_box.value(),7)
    def test_pending_result_cannot_override_new_layer(self):
        self.w.open_path(self.paths[0]);self.w.open_path(self.paths[1])
        self.wait(lambda:self.w.image is not None and not self.w._scan_pending)
        self.assertEqual(self.w.threshold_box.value(),7)
        self.assertEqual(self.w._scan_settings.layer,1)
    def test_manual_json_override_is_scoped_to_acquisition(self):
        from unittest.mock import patch
        self.open(self.paths[0])
        data=parameters();data['ImageFilterParam']['CommonParamArray'][0]['ThrBinary']=25
        override=write_json(self.directory/'custom.json',data)
        with patch('spng_paramui.QFileDialog.getOpenFileName',return_value=(str(override),'')):
            self.w.choose_scan_parameters()
        self.wait(lambda:not self.w._scan_pending)
        self.assertEqual(self.w.threshold_box.value(),20)
        other=self.directory/'another_acquisition'
        write_json(other/'PARAMS/CombinedParam.json',parameters())
        self.open(stack(other/'DATA/00_00/Raw_0_00000000_0_003.spng'))
        self.assertEqual(self.w.threshold_box.value(),9)
        self.assertIsNone(self.w._scan_override)

    def test_folder_layer_dropdown_updates_parameters(self):
        self.open(self.directory)
        self.assertEqual(self.w.threshold_box.value(),9)
        self.w.layer_combo.setCurrentText('Layer1')
        self.wait(lambda:self.w.image is not None and not self.w._scan_pending)
        self.assertEqual(self.w._scan_settings.layer,1)
        self.assertEqual(self.w.threshold_box.value(),7)
