"""Verify the one-file executable using synthetic SPNG data."""
from pathlib import Path
import json
import subprocess
import sys

root = Path(__file__).resolve().parents[1]
exe = root / "dist" / "SPNGViewer.exe"
report = root / "build" / "packaged_smoke" / "result.json"
if not exe.is_file():
    raise SystemExit("dist/SPNGViewer.exe was not found. Run build_exe.py --onefile first.")
report.parent.mkdir(parents=True, exist_ok=True)
# Remove only this generated diagnostic report, never any image.
if report.exists():
    report.unlink()
subprocess.run([str(exe), "--self-test", str(report)], check=True, timeout=120, cwd=root)
if not json.loads(report.read_text(encoding="utf-8")).get("success"):
    raise SystemExit("Packaged application self-test failed")
print("Packaged application self-test passed")
