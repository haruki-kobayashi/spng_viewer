"""Output guards use only generated files; never acquisition data."""
from pathlib import Path
import hashlib
import os
import subprocess
import sys
import tempfile
import unittest
from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from spng_core import SpngStack, SpngError, save_png
from spng_smoke import run_smoke
OUT = ROOT / "tests" / "_generated"
OUT.mkdir(exist_ok=True)

class OutputSafetyTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="safety_", dir=OUT)
        self.folder = Path(self.temp.name)
        self.source = self.folder / "input.spng"
        self.source.write_bytes(b"generated invalid SPNG; must remain unchanged")
        self.digest = hashlib.sha256(self.source.read_bytes()).hexdigest()
    def tearDown(self):
        self.assertEqual(hashlib.sha256(self.source.read_bytes()).hexdigest(), self.digest)
        self.temp.cleanup()
    def test_cli_diagnostic_report_cannot_be_spng(self):
        env = os.environ.copy()
        env["QT_QPA_PLATFORM"] = "offscreen"
        result = subprocess.run(
            [sys.executable, "-B", str(ROOT / "spng_viewer.py"),
             "--self-test", str(self.source)], cwd=self.folder, env=env,
            capture_output=True, text=True, timeout=30,
        )
        self.assertEqual(result.returncode, 2)
        self.assertIn(".json", result.stderr)
    def test_direct_diagnostic_guard_precedes_any_output(self):
        with self.assertRaisesRegex(ValueError, ".json"):
            run_smoke(None, None, self.source)
        self.assertFalse((self.folder / "smoke_files").exists())
    def test_png_export_uses_separate_extension(self):
        output = save_png(Image.new("L", (4, 3), 7), self.source)
        self.assertEqual(output, self.folder / "input.png")
        self.assertEqual(Image.open(output).getpixel((0, 0)), 7)
    def test_read_error_does_not_write_input(self):
        with self.assertRaises(SpngError):
            SpngStack(self.source)

if __name__ == "__main__":
    unittest.main(verbosity=2)
