"""Meaningful regression tests. Fixtures and exports are generated locally; no acquisition data is used."""
from pathlib import Path
from io import BytesIO
import json
import os
import struct
import sys
import time
import unittest
import numpy as np
from PIL import Image
ROOT=Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT))
from spng_core import *
OUT=ROOT/'tests'/'_generated';OUT.mkdir(exist_ok=True)
FIX=OUT/'fixtures'
FIX.mkdir(exist_ok=True)

def write_stack(path,images):
    path=Path(path);path.parent.mkdir(parents=True,exist_ok=True)
    with path.open('wb') as f:
        for im in images:
            if im is None:f.write(struct.pack('<Q',MISSING));continue
            b=BytesIO();im.save(b,'PNG');p=b.getvalue()
            f.write(struct.pack('<Q',len(p)));f.write(p)
    return path

A=np.arange(35,dtype=np.uint8).reshape(5,7)
SOURCE=Image.fromarray(A)
STACK=write_stack(FIX/'sample.spng',[SOURCE,None,Image.fromarray(A+100)])

class CoreTests(unittest.TestCase):
    def test_missing_index_and_raw_pixels(self):
        s=SpngStack(STACK)
        self.assertEqual(len(s),3)
        self.assertIsNone(s.read_frame(1))
        np.testing.assert_array_equal(np.asarray(s.read_frame(0)),A)
        np.testing.assert_array_equal(np.asarray(s.read_frame(2)),A+100)

    def test_zero_length_missing(self):
        p=FIX/'zero_missing.spng';p.write_bytes(struct.pack('<Q',0)+STACK.read_bytes())
        s=SpngStack(p);self.assertEqual(s.first_available(),1);self.assertIsNone(s.read_frame(0))
        np.testing.assert_array_equal(np.asarray(s.read_frame(1)),A)

    def test_truncated_container(self):
        p=FIX/'truncated.spng';p.write_bytes(STACK.read_bytes()[:-1])
        with self.assertRaises(SpngError):SpngStack(p)
        p.write_bytes(b'abc')
        with self.assertRaises(SpngError):SpngStack(p)

    def test_invalid_png_header(self):
        p=FIX/'invalid.spng';p.write_bytes(struct.pack('<Q',33)+b'x'*33)
        with self.assertRaises(SpngError):SpngStack(p)

    def test_bottom_left_roi_and_original_export(self):
        box=bottom_left_roi_to_box(1,0,3,2,SOURCE.size)
        self.assertEqual(box,(1,3,4,5))
        self.assertEqual(box_to_bottom_left_roi(box,SOURCE.size),(1,0,3,2))
        p=save_png(SOURCE,FIX/'bottom.png',box)
        np.testing.assert_array_equal(np.asarray(Image.open(p)),A[3:5,1:4])
        self.assertEqual(bottom_left_roi_to_box(0,4,7,1,SOURCE.size),(0,0,7,1))
        for roi in [(-1,0,2,2),(0,-1,1,1),(6,0,2,1),(0,4,1,2),(0,0,0,1)]:
            with self.assertRaises(ValueError):bottom_left_roi_to_box(*roi,SOURCE.size)

    def test_reverse_drag_and_clamp(self):
        self.assertEqual(drag_roi((4.1,3.2),(1.1,1.2),(7,5)),(1,1,5,4))
        self.assertEqual(drag_roi((1,1),(20,20),(7,5)),(1,1,7,5))
        self.assertIsNone(drag_roi((-2,-2),(-1,-1),(7,5)))

    def test_uint16_rgb_bit_and_binary(self):
        images=[Image.fromarray(np.array([[0,256,65535]],np.uint16)),
            Image.fromarray(np.array([[[1,2,3],[254,253,252]]],np.uint8)),
            Image.fromarray(np.array([[False,True]],bool)),
            Image.fromarray(np.array([[0,1]],np.uint8))]
        p=write_stack(FIX/'types.spng',images);s=SpngStack(p)
        for i,im in enumerate(images):
            loaded=s.read_frame(i);out=save_png(loaded,FIX/f'type{i}.png')
            np.testing.assert_array_equal(np.asarray(Image.open(out)),np.asarray(im))
        self.assertEqual(default_levels(s.read_frame(0)),(0,65535))
        self.assertEqual(default_levels(s.read_frame(3)),(0,1))
        np.testing.assert_array_equal(display_array(s.read_frame(3),0,1),[[0,255]])
        np.testing.assert_array_equal(np.asarray(s.read_frame(3)),[[0,1]])

    def test_16bit_color_is_rejected_without_lossy_conversion(self):
        import zlib
        def chunk(t,p):return struct.pack('>I',len(p))+t+p+struct.pack('>I',zlib.crc32(t+p)&0xffffffff)
        png=PNG_SIGNATURE+chunk(b'IHDR',struct.pack('>IIBBBBB',1,1,16,2,0,0,0))+chunk(b'IDAT',zlib.compress(b'\0'+struct.pack('>HHH',1024,32768,65535)))+chunk(b'IEND',b'')
        path=FIX/'rgb16.spng';path.write_bytes(struct.pack('<Q',len(png))+png)
        with self.assertRaisesRegex(SpngError,'16bit'):SpngStack(path).read_frame(0)

    def test_catalog_and_shallow_discovery(self):
        root=FIX/'dataset'
        for child in ['IMAGE/00_00','IMAGE/00_01','DATA/00_00']:(root/child).mkdir(parents=True,exist_ok=True)
        write_stack(root/'IMAGE/00_00/Raw_0_00000000_0_003.spng',[SOURCE,None,Image.fromarray(A+100)])
        write_stack(root/'IMAGE/00_00/Raw_0_00000002_0_001.spng',[Image.fromarray(A+2)])
        write_stack(root/'IMAGE/00_00/Raw_0_00000003_1_001.spng',[Image.fromarray(A+3)])
        write_stack(root/'IMAGE/00_00/IF1_Filtered_2_00000000_0_001.spng',[Image.fromarray(A+4)])
        write_stack(root/'IMAGE/00_01/Raw_0_00000010_0_001.spng',[Image.fromarray(A+10)])
        info=discover_root(root);self.assertEqual([x[0] for x in info['containers']],['IMAGE','DATA'])
        sensors=discover_sensors(root/'IMAGE');self.assertEqual([x[0] for x in sensors],['00_00','00_01'])
        files=list_sensor_files(root/'IMAGE/00_00');self.assertEqual(len(files),4)
        r=next(f for f in files if f.kind=='Raw_0' and f.view=='2')
        self.assertEqual(r.layer,'Layer0')
        self.assertEqual(list_sensor_files(root/'DATA/00_00'),[])
        self.assertEqual(FileRecord.from_path('arbitrary.spng').kind,'その他のファイル')

os.environ.setdefault('QT_QPA_PLATFORM','offscreen')
from PySide6.QtWidgets import QApplication,QFileDialog
from PySide6.QtCore import Qt,QPointF
from PySide6.QtTest import QTest
from spng_viewer import Viewer,initialize_fonts

class GuiTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.app=QApplication.instance() or QApplication([])
        initialize_fonts(cls.app);cls.app.setStyle('Fusion')
    def setUp(self):
        self.win=Viewer();self.win.show();self.app.processEvents()
    def tearDown(self):
        self.win.pool.waitForDone(10000);self.app.processEvents();self.win.close();self.app.processEvents()
    def wait(self,predicate,seconds=10):
        end=time.monotonic()+seconds
        while time.monotonic()<end:
            self.app.processEvents()
            if predicate():return
            QTest.qWait(10)
        self.fail('GUI state timeout: '+self.win.state_label.text())

    def test_file_layers_cursor_and_mouse_crop_export(self):
        w=self.win;w.open_path(STACK)
        self.wait(lambda:w.image is not None)
        np.testing.assert_array_equal(np.asarray(w.image),A)
        w.cursor_position((0.1,4.1));self.assertIn('X: 0　Y: 0',w.cursor_label.text())
        self.assertTrue(w.cursor_label.text().endswith('28'))
        w.cursor_position((6.1,.1));self.assertIn('X: 6　Y: 4',w.cursor_label.text())
        self.assertTrue(w.cursor_label.text().endswith('6'))
        w.cursor_position((-1,0));self.assertIn('X: ―',w.cursor_label.text())
        start=w.view.mapFromScene(QPointF(1.2,1.2));end=w.view.mapFromScene(QPointF(4.2,3.2))
        QTest.mousePress(w.view.viewport(),Qt.MouseButton.LeftButton,Qt.KeyboardModifier.ControlModifier,pos=start)
        QTest.mouseMove(w.view.viewport(),end)
        QTest.mouseRelease(w.view.viewport(),Qt.MouseButton.LeftButton,Qt.KeyboardModifier.ControlModifier,pos=end)
        self.assertEqual(w.view.roi,(1,1,5,4))
        self.assertEqual((w.roi_x.value(),w.roi_y.value(),w.roi_w.value(),w.roi_h.value()),(1,1,4,3))
        w.low.setValue(10);w.high.setValue(20);w.invert.setChecked(True)
        original=QFileDialog.getSaveFileName
        try:
            QFileDialog.getSaveFileName=lambda *a,**k:(str(FIX/'gui_crop.png'),'PNG')
            w.save_dialog(True)
            np.testing.assert_array_equal(np.asarray(Image.open(FIX/'gui_crop.png')),A[1:4,1:5])
            QFileDialog.getSaveFileName=lambda *a,**k:(str(FIX/'gui_full.png'),'PNG')
            w.save_dialog(False)
            np.testing.assert_array_equal(np.asarray(Image.open(FIX/'gui_full.png')),A)
        finally:QFileDialog.getSaveFileName=original
        w.layer_number.setValue(1)
        self.wait(lambda:w.loaded_index==1)
        self.assertIsNone(w.image);self.assertFalse(w.full_action.isEnabled())
        w.layer_number.setValue(2)
        self.wait(lambda:w.loaded_index==2 and w.image is not None)
        np.testing.assert_array_equal(np.asarray(w.image),A+100)
        # A superseded asynchronous request cannot replace the latest layer.
        w.layer_number.setValue(0);w.load_requested_layer();w.layer_number.setValue(2)
        self.wait(lambda:w.loaded_index==2 and w.image is not None)
        self.assertEqual(w.layer_number.value(),2)
        w.grab().save(str(OUT/'gui_synthetic.png'))

    def test_plain_left_drag_pans_without_changing_selection(self):
        w=self.win;w.open_path(STACK)
        self.wait(lambda:w.image is not None)
        w.view.set_image(np.zeros((1200,1600),np.uint8),reset=True)
        w.view.set_zoom(200)
        w.view.set_roi((500,500,550,550))
        self.app.processEvents()
        h=w.view.horizontalScrollBar();v=w.view.verticalScrollBar()
        h.setValue((h.minimum()+h.maximum())//2);v.setValue((v.minimum()+v.maximum())//2)
        before=(h.value(),v.value())
        from PySide6.QtCore import QPoint
        start=QPoint(w.view.viewport().width()//2,w.view.viewport().height()//2)
        end=start+QPoint(50,30)
        QTest.mousePress(w.view.viewport(),Qt.MouseButton.LeftButton,pos=start)
        QTest.mouseMove(w.view.viewport(),end)
        QTest.mouseRelease(w.view.viewport(),Qt.MouseButton.LeftButton,pos=end)
        self.assertEqual((h.value(),v.value()),(before[0]-50,before[1]-30))
        self.assertEqual(w.view.roi,(500,500,550,550))
        self.assertIsNone(w.view.drag_start)

    def test_pending_layer_keeps_previous_display_and_roi(self):
        import threading
        from PySide6.QtGui import QImage
        w=self.win;w.open_path(STACK)
        self.wait(lambda:w.image is not None)
        w.view.set_roi((1,1,5,4))
        self.app.processEvents()
        def snapshot():
            q=w.view.viewport().grab().toImage().convertToFormat(QImage.Format.Format_RGBA8888)
            return bytes(q.constBits())
        before=snapshot()
        gate=threading.Event();started=threading.Event()
        read=w.stack.read_frame
        def slow_read(index):
            started.set()
            gate.wait(5)
            return read(index)
        w.stack.read_frame=slow_read
        try:
            w.layer_number.setValue(2)
            w.debounce.stop();w.load_requested_layer()
            self.wait(started.is_set)
            self.assertTrue(w.loading_image)
            self.assertFalse(w.full_action.isEnabled())
            self.assertEqual(snapshot(),before)
            self.assertTrue(w.view.roi_item.isVisible())
        finally:gate.set()
        self.wait(lambda:w.loaded_index==2 and w.image is not None)
        np.testing.assert_array_equal(np.asarray(w.image),A+100)
        self.assertEqual(w.view.roi,(1,1,5,4))
        self.assertIn('8bitグレースケール',w.meta.text())

    def test_folder_sensor_layer_kind_view_switches(self):
        CoreTests().test_catalog_and_shallow_discovery()
        w=self.win;w.open_path(FIX/'dataset')
        self.wait(lambda:w.image is not None)
        self.assertEqual(w.container_combo.currentText(),'IMAGE')
        self.assertEqual(w.kind_combo.currentText(),'Raw_0')
        self.assertEqual(w.sensor_combo.currentText(),'00_00')
        w.view_combo.setEditText('00000002');w.typed_view()
        self.wait(lambda:w.image is not None and w.current_path.name.startswith('Raw_0_00000002'))
        np.testing.assert_array_equal(np.asarray(w.image),A+2)
        w.layer_combo.setCurrentText('Layer1')
        self.wait(lambda:w.image is not None and '_00000003_1_' in w.current_path.name)
        np.testing.assert_array_equal(np.asarray(w.image),A+3)
        w.layer_combo.setCurrentText('Layer0');w.kind_combo.setCurrentText('IF1_Filtered_2')
        self.wait(lambda:w.image is not None and w.current_path.name.startswith('IF1_'))
        np.testing.assert_array_equal(np.asarray(w.image),A+4)
        w.sensor_combo.setCurrentText('00_01')
        self.wait(lambda:w.image is not None and '_00000010_' in w.current_path.name)
        self.assertEqual(w.kind_combo.currentText(),'Raw_0')
        w.container_combo.setCurrentText('DATA')
        self.wait(lambda:'spngがありません' in w.state_label.text())
        self.assertIsNone(w.image)

if __name__=='__main__':
    suite=unittest.defaultTestLoader.loadTestsFromModule(sys.modules[__name__])
    runner=unittest.TextTestRunner(verbosity=2)
    result=runner.run(suite)
    (OUT/'regression_result.json').write_text(json.dumps({'tests':result.testsRun,'failures':[(str(t),e) for t,e in result.failures],'errors':[(str(t),e) for t,e in result.errors],'successful':result.wasSuccessful()},ensure_ascii=False,indent=2),'utf8')
    sys.exit(not result.wasSuccessful())
