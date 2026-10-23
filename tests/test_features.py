from pathlib import Path
from io import BytesIO
import gc,json,os,struct,sys,threading,time,unittest
import numpy as np
from PIL import Image
ROOT=Path(__file__).resolve().parents[1];sys.path.insert(0,str(ROOT))
from spng_core import SpngStack,bottom_left_roi_to_box
from spng_ops import *
OUT=ROOT/'tests'/'_generated';OUT.mkdir(exist_ok=True)
FIX=OUT/'advanced';FIX.mkdir(exist_ok=True);CACHE=FIX/'cache';CACHE.mkdir(exist_ok=True)

def write_stack(name,arrays):
 p=FIX/name
 with p.open('wb') as f:
  for a in arrays:
   if a is None:f.write(struct.pack('<Q',(1<<64)-1));continue
   stream=BytesIO();Image.fromarray(a).save(stream,'PNG');payload=stream.getvalue();f.write(struct.pack('<Q',len(payload)));f.write(payload)
 return p

class OpsTests(unittest.TestCase):
 def test_range_parser(self):
  self.assertEqual(parse_layers('0-4:2,3,8',10),[0,2,3,4,8])
  for spec in ['','4-2','0-9:0','0-12','a','0,,1']:
   with self.assertRaises(ValueError):parse_layers(spec,10)
 def test_projection_values_owner_ties_missing(self):
  arrays=[np.array([[3,5],[9,1]],np.uint8),None,np.array([[3,6],[2,0]],np.uint8),np.array([[4,4],[8,2]],np.uint8)]
  s=SpngStack(write_stack('projection.spng',arrays))
  maximum=project_stack(s.read_frame,[3,0,2,1],'max');minimum=project_stack(s.read_frame,[3,0,2,1],'min')
  np.testing.assert_array_equal(maximum.values,[[4,6],[9,2]]);np.testing.assert_array_equal(maximum.layers,[[3,2],[0,3]])
  np.testing.assert_array_equal(minimum.values,[[3,4],[2,0]]);np.testing.assert_array_equal(minimum.layers,[[0,3],[2,2]])
  self.assertEqual(maximum.used,[0,2,3]);self.assertEqual(maximum.requested,[0,1,2,3])
  for p in [maximum,minimum]:
   colored=np.asarray(projection_image(p,True,False));self.assertEqual(colored.shape,(2,2,3))
   np.testing.assert_array_equal(colored,np.rint(depth_lut()[np.rint(p.layers/3*255).astype(np.uint8)]*255).astype(np.uint8))
  with self.assertRaises(ValueError):project_stack(s.read_frame,[1])
 def test_preload_uint16_missing_and_cleanup(self):
  arr=np.arange(35,dtype=np.uint16).reshape(5,7)*1000
  s=SpngStack(write_stack('uint16.spng',[arr,None,arr+1]))
  bank=preload_stack(s,CACHE);directory=Path(bank.owner.name)
  np.testing.assert_array_equal(np.asarray(bank.read_frame(0)),arr);self.assertIsNone(bank.read_frame(1))
  del bank;gc.collect();self.assertFalse(directory.exists())
 def test_highpass_constant_and_fir(self):
  a=np.full((16,20),200,np.uint8);s=SpngStack(write_stack('constant.spng',[a,a,a]))
  b=process_stack(s,FilterConfig(),CACHE)
  for stage in ['2D filter後','Z filter後','二値化後','膨張後']:self.assertEqual(np.asarray(b.read_frame(1,stage)).sum(),0)
  k=fir_kernel(15,.1);np.testing.assert_allclose(k,k[::-1]);self.assertAlmostEqual(float(k.sum()),1,places=6)
 def test_z_endpoints_and_saturation(self):
  images=[np.full((5,7),v,np.uint8) for v in [200,150,200]]
  b=process_stack(SpngStack(write_stack('z.spng',images)),FilterConfig(highpass=False,z_weight=-.5,binary=False,dilation=1),CACHE)
  for i,value in enumerate([125,86,125]):np.testing.assert_array_equal(np.asarray(b.read_frame(i)),np.full((5,7),value,np.uint8))
 def test_binary_strict_and_dilation(self):
  a=np.zeros((9,9),np.uint8);a[4,4]=6;a[0,0]=5
  b=process_stack(SpngStack(write_stack('binary.spng',[a])),FilterConfig(highpass=False,z_filter=False,threshold=5,dilation=3),CACHE)
  expected=np.zeros_like(a);expected[3:6,3:6]=1
  np.testing.assert_array_equal(np.asarray(b.read_frame(0,'膨張後')),expected)
  self.assertEqual(int(np.asarray(b.read_frame(0,'二値化後')).sum()),1)
 def test_cancel_and_invalid_config(self):
  a=np.zeros((5,7),np.uint8);s=SpngStack(write_stack('cancel.spng',[a]))
  cancel=threading.Event();cancel.set()
  before=set(CACHE.iterdir())
  with self.assertRaises(Cancelled):process_stack(s,FilterConfig(),CACHE,cancel=cancel)
  self.assertEqual(set(CACHE.iterdir()),before)
  for config in [FilterConfig(taps=14),FilterConfig(cutoff=.5),FilterConfig(z_step=3),FilterConfig(binary=False,dilation=2)]:
   with self.assertRaises(ValueError):config.validate()
 def test_stride_two_matches_independent_parities(self):
  arrays=[np.full((5,7),v,np.uint8) for v in [220,80,140,190,180,90]]
  c=dict(highpass=False,z_weight=-.3,binary=False,dilation=1)
  full=process_stack(SpngStack(write_stack('stride_full.spng',arrays)),FilterConfig(z_step=2,**c),CACHE)
  for parity in (0,1):
   sub=process_stack(SpngStack(write_stack(f'stride_{parity}.spng',arrays[parity::2])),FilterConfig(z_step=1,**c),CACHE)
   for j,i in enumerate(range(parity,len(arrays),2)):np.testing.assert_array_equal(np.asarray(full.read_frame(i)),np.asarray(sub.read_frame(j)))
   sub.close()
  full.close()
 def test_named_palettes(self):
  from spng_palettes import PALETTES
  p=Projection(np.array([[40,90]],np.uint8),np.array([[0,31]]),[0,31],[0,31],'max','test')
  for name in PALETTES:
   lut=depth_lut(palette=name);self.assertEqual(lut.shape,(256,3));self.assertTrue(((lut>=0)&(lut<=1)).all())
   np.testing.assert_array_equal(np.asarray(projection_image(p,True,False,name)),np.rint(lut[[[0,255]]]*255).astype(np.uint8))

 def test_batch_output_exact_crop_missing(self):
  a=np.arange(35,dtype=np.uint8).reshape(5,7);s=SpngStack(write_stack('export.spng',[a,None,a+1]))
  path=FIX/('out_'+str(time.time_ns()));box=bottom_left_roi_to_box(1,0,3,2,(7,5))
  result=export_layers(s.read_frame,[0,1,2],path,'example',roi=box)
  self.assertEqual(result['saved'],2);self.assertEqual(result['missing'],[1])
  np.testing.assert_array_equal(np.asarray(Image.open(path/'example_z002.png')),(a+1)[3:5,1:4])
  manifest=json.loads((path/'export_manifest.json').read_text('utf8'));self.assertEqual(manifest['status'],'complete')
 def test_line_interpolate_extrapolate(self):
  self.assertEqual(line_position((2,10,20),(6,30,40),4),(20,30))
  self.assertEqual(line_position((2,10,20),(6,30,40),0),(0,10))
  self.assertEqual(line_position((2,10,20),(6,30,40),0,False),(10,20))
  with self.assertRaises(ValueError):line_position((0,1,2),(0,3,4),1)

os.environ.setdefault('QT_QPA_PLATFORM','windows')
from PySide6.QtWidgets import QApplication,QFileDialog
from PySide6.QtCore import Qt,QPointF
from PySide6.QtTest import QTest
from spng_viewer import Viewer,initialize_fonts

class GuiFeatureTests(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.app=QApplication.instance() or QApplication([]);initialize_fonts(cls.app)
  cls.path=write_stack('Raw_0_00000000_0_005.spng',[np.full((24,32),50+20*z,np.uint8) for z in range(5)])
 def setUp(self):
  self.w=Viewer();self.w.show();self.w.open_path(self.path);self.wait(lambda:self.w.image is not None)
 def tearDown(self):
  self.w.stop_play()
  if self.w._cancel:self.w._cancel.set()
  self.w.pool.waitForDone(10000);self.app.processEvents();self.w.close();self.app.processEvents()
 def wait(self,condition,seconds=15):
  end=time.monotonic()+seconds
  while time.monotonic()<end:
   self.app.processEvents()
   if condition():return
   time.sleep(.005)
  self.fail(self.w.state_label.text())
 def test_auto_stop_loop_and_preload(self):
  w=self.w;w.loop_check.setChecked(False);w.play_start.setValue(0);w.play_end.setValue(2);w.play_rate.setValue(60)
  w.toggle_play();self.wait(lambda:w._raw_bank is not None and not w._playing and w.loaded_index==2)
  seen=[];original=w.frame_ready
  def record(pair):seen.append(pair[0]);original(pair)
  w.frame_ready=record;w.loop_check.setChecked(True);w.toggle_play()
  self.wait(lambda:seen.count(0)>=2);self.assertTrue(w._playing);w.stop_play()
 def test_two_point_follow_center_and_pick(self):
  w=self.w
  for tag,vals in enumerate([(0,4,6),(4,24,16)]):
   for box,v in zip(w.point_fields[tag],vals):box.setValue(v)
   w.register_point(tag)
  w.view.set_zoom(200);w.layer_number.setValue(2);self.wait(lambda:w.loaded_index==2)
  point=w.view.mapFromScene(QPointF(14.5,24-.5-11))
  center=w.view.viewport().rect().center();self.assertLessEqual(abs(point.x()-center.x()),2);self.assertLessEqual(abs(point.y()-center.y()),2)
  w.arm_point(0)
  where=w.view.mapFromScene(QPointF(8.2,12.2));QTest.mouseClick(w.view.viewport(),Qt.MouseButton.LeftButton,pos=where)
  self.assertEqual(w._points[0],(2,8.,11.))
 def test_projection_color_values_and_return(self):
  w=self.w;w.layer_spec.setText('0,2-4');w.start_projection('max');self.wait(lambda:w._projection is not None and not w._bulk_running)
  self.assertEqual(int(w._projection.values[0,0]),130);self.assertEqual(int(w._projection.layers[0,0]),4)
  w.invert.setChecked(True);w.depth_check.setChecked(True);self.assertEqual(w.image.mode,'RGB');self.assertFalse(w.invert.isChecked());self.assertFalse(w.invert.isEnabled());w.cursor_position((1,2));self.assertIn('選択層: 4',w.cursor_label.text())
  w.return_to_slice();self.wait(lambda:w._projection is None and w.image is not None and not w.loading_image)
  self.assertEqual(w.image.mode,'L');self.assertEqual(int(np.asarray(w.image)[0,0]),50)
 def test_filter_all_stages_and_no_recompute(self):
  w=self.w;w.resize_check.setChecked(False);w.highpass_check.setChecked(False);w.z_check.setChecked(False);w.threshold_box.setValue(100);w.dilate_box.setValue(1)
  w.start_processing();self.wait(lambda:w._processed is not None and w.image is not None and not w._bulk_running and not w.loading_image)
  self.assertEqual(w.stage_combo.currentText(),'二値化後');self.assertEqual((w.low.value(),w.high.value()),(0,1))
  original=w.stack.read_frame
  def fail(*a):raise RuntimeError('Should not re-read SPNG for processed layers')
  w.stack.read_frame=fail
  try:
   w.layer_number.setValue(4);self.wait(lambda:w.loaded_index==4 and not w.loading_image)
   np.testing.assert_array_equal(np.asarray(w.image),np.ones((24,32),np.uint8))
  finally:w.stack.read_frame=original
 def test_processed_scale_keeps_original_point_coordinates_and_saves_parameters(self):
  w=self.w;w.resize_check.setChecked(False);w.highpass_check.setChecked(False);w.z_check.setChecked(False);w.binary_check.setChecked(False);w.dilate_box.setValue(1);w.resize_check.setChecked(True);w.scale_box.setValue(2)
  w.start_processing();self.wait(lambda:w._processed is not None and w.image is not None and not w.loading_image and not w._bulk_running)
  self.assertEqual(w.image.size,(64,48))
  for tag,vals in enumerate([(0,4,6),(4,24,16)]):
   for spin,value in zip(w.point_fields[tag],vals):spin.setValue(value)
   w.register_point(tag)
  w.view.set_zoom(200);w.layer_number.setValue(2);self.wait(lambda:w.loaded_index==2 and not w.loading_image)
  point=w.view.mapFromScene(QPointF((14+.5)*2,48-(11+.5)*2));center=w.view.viewport().rect().center()
  self.assertLessEqual(abs(point.x()-center.x()),2);self.assertLessEqual(abs(point.y()-center.y()),2)
  original=QFileDialog.getSaveFileName;target=FIX/'processed_saved.png'
  try:
   QFileDialog.getSaveFileName=lambda *a,**k:(str(target),'PNG');w.save_dialog(False)
  finally:QFileDialog.getSaveFileName=original
  np.testing.assert_array_equal(np.asarray(Image.open(target)),np.full((48,64),90,np.uint8))
  data=json.loads(target.with_suffix('.json').read_text('utf8'));self.assertEqual(data['processing']['config']['scale'],2)

 def test_GUI_multi_export(self):
  w=self.w;w.layer_spec.setText('0,2,4');w.view.set_roi((1,20,5,24));w.batch_crop.setChecked(True)
  original=QFileDialog.getExistingDirectory;QFileDialog.getExistingDirectory=lambda *a,**k:str(FIX)
  existing=set(FIX.glob('*_png_*'))
  try:w.start_export();self.wait(lambda:not w._bulk_running and 'PNG出力完了' in w.state_label.text())
  finally:QFileDialog.getExistingDirectory=original
  made=set(FIX.glob('*_png_*'))-existing;self.assertEqual(len(made),1)
  folder=made.pop();self.assertEqual(len(list(folder.glob('*.png'))),3)
  np.testing.assert_array_equal(np.asarray(Image.open(folder/'Raw_0_00000000_0_005_z004.png')),np.full((4,4),130,np.uint8))
 def test_reset_points_defaults_and_popup(self):
  w=self.w
  self.assertTrue(w.interp_combo.currentText().startswith('GPU'))
  self.assertEqual(w.sensor_combo.maxVisibleItems(),12)
  for tag,point in enumerate([(0,2,3),(4,8,9)]):
   for field,value in zip(w.point_fields[tag],point):field.setValue(value)
   w.register_point(tag)
  self.assertTrue(w.follow_check.isChecked())
  w.reset_point(0);self.assertIsNone(w._points[0]);self.assertFalse(w.follow_check.isChecked())
  self.assertTrue(all(not field.text() for field in w.point_fields[0]))
  self.assertIsNotNone(w._points[1]);w.reset_point(1);self.assertEqual(w._points,[None,None])

 def test_parameter_steps_defaults_toggles_and_palette_bars(self):
  self.assertEqual(FilterConfig().z_weight,-.2)
  w=self.w
  for widget,value,step in [(w.scale_box,1.4142,.0001),(w.cutoff_box,.1,.01),(w.z_weight,-.2,.01)]:
   self.assertAlmostEqual(widget.value(),value);self.assertAlmostEqual(widget.singleStep(),step)
   widget.stepUp();self.assertAlmostEqual(widget.value(),value+step);widget.stepDown();self.assertAlmostEqual(widget.value(),value)
  self.assertTrue(w.resize_check.isChecked());self.assertTrue(w.dilate_check.isChecked())
  w.resize_check.setChecked(False);w.dilate_check.setChecked(False)
  self.assertEqual(w.filter_config().scale,1);self.assertEqual(w.filter_config().dilation,1)
  self.assertFalse(w.scale_box.isEnabled());self.assertFalse(w.dilate_box.isEnabled())
  w.resize_check.setChecked(True);w.dilate_check.setChecked(True)
  self.assertEqual(w.filter_config().scale,1.4142);self.assertEqual(w.filter_config().dilation,2)
  self.assertTrue(w.scale_box.isEnabled());self.assertTrue(w.dilate_box.isEnabled())
  w._projection=project_stack(w.stack.read_frame,[0,2,4])
  w.depth_check.setChecked(True)
  for i,name in enumerate(['kingfisher','meadow','ember','heat','rainbow']):
   self.assertEqual(w.palette_combo.itemText(i),name);self.assertFalse(w.palette_combo.itemIcon(i).isNull())
   w.palette_combo.setCurrentIndex(i);self.assertEqual(w.palette_combo.currentData(),name)
   np.testing.assert_array_equal(np.asarray(w.image),np.asarray(projection_image(w._projection,True,False,name)))
  w.feature_tabs.setCurrentIndex(2);self.app.processEvents();w.grab().save(str(OUT/'colorbar_combo_window.png'))
  w.palette_combo.showPopup();self.app.processEvents();w.palette_combo.view().grab().save(str(OUT/'colorbar_combo_popup.png'));w.palette_combo.hidePopup()

 def test_file_change_while_background_job_finishes(self):
  w=self.w;gate=threading.Event();called=[]
  def op(progress,cancel):gate.wait(3);return 'old'
  w.begin_bulk('test',op,lambda value:called.append(value));w.open_path(self.path);gate.set()
  self.wait(lambda:w.image is not None and not w.loading_image);w.pool.waitForDone(5000);self.app.processEvents()
  self.assertEqual(called,[]);self.assertFalse(w._bulk_running)

if __name__=='__main__':
 result=unittest.TextTestRunner(verbosity=2).run(unittest.defaultTestLoader.loadTestsFromModule(sys.modules[__name__]))
 (OUT/'advanced_result.json').write_text(json.dumps({'tests':result.testsRun,'success':result.wasSuccessful(),'failures':[(str(t),e) for t,e in result.failures],'errors':[(str(t),e) for t,e in result.errors]},ensure_ascii=False,indent=2),'utf8')
 sys.exit(not result.wasSuccessful())
