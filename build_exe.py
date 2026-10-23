"""Build the Windows executable; stop immediately if PyInstaller fails."""
from pathlib import Path
import argparse
import datetime
import hashlib
import importlib.metadata as metadata
import json
import os
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
MODULES = ["spng_viewer.py", "spng_core.py", "spng_features.py", "spng_ops.py",
           "spng_palettes.py", "spng_scanparams.py", "spng_paramui.py", "spng_smoke.py"]
DISTRIBUTIONS = ["PySide6", "PySide6_Essentials", "shiboken6", "numpy",
                 "Pillow", "opencv-python-headless", "pyinstaller"]

def collect_licenses(destination):
    destination.mkdir(parents=True, exist_ok=True)
    for source in (ROOT / "licenses").rglob("*"):
        if source.is_file():
            target = destination / "upstream" / source.relative_to(ROOT / "licenses")
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(source, target)
    packages = {}
    for name in DISTRIBUTIONS:
        dist = metadata.distribution(name)
        packages[name] = dist.version
        for entry in dist.files or []:
            normalized = str(entry).replace("\\", "/")
            base = Path(normalized).name.lower()
            if not ("license" in normalized.lower() or base.startswith(("copying", "notice"))):
                continue
            source = Path(dist.locate_file(entry))
            if source.is_file() and source.suffix.lower() not in (".py", ".pyc", ".dll", ".so"):
                target = destination / name / normalized.replace("/", "_")
                target.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(source, target)
    python_license = Path(sys.base_prefix) / "LICENSE.txt"
    if python_license.is_file():
        shutil.copy2(python_license, destination / "Python_LICENSE.txt")
    (destination / "packages.json").write_text(json.dumps(packages, indent=2), encoding="utf-8")
    shutil.copy2(ROOT / "THIRD_PARTY_NOTICES.md", destination / "NOTICE.md")
    return packages

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--onefile", action="store_true", help="Build a single executable")
    args = parser.parse_args()
    if sys.platform != "win32":
        parser.error("The executable build is supported on Windows only.")
    import PySide6
    import PyInstaller
    kind = "onefile" if args.onefile else "onedir"
    work = ROOT / "build"
    work.mkdir(exist_ok=True)
    dist = ROOT / "dist"
    dist.mkdir(exist_ok=True)
    notices = work / "licenses"
    versions = collect_licenses(notices)
    argv = [sys.executable, "-B", "-m", "PyInstaller", "--noconfirm", "--clean",
            "--windowed", "--noupx", "--name", "SPNGViewer",
            "--additional-hooks-dir", str(ROOT / "build_hooks"),
            "--onefile" if args.onefile else "--onedir",
            "--distpath", str(dist), "--workpath", str(work / "pyinstaller"),
            "--specpath", str(work / "spec"),
            "--add-data", str(notices) + ";THIRD_PARTY_LICENSES",
            "--add-data", str(ROOT / "LICENSE") + ";.",
            "--add-data", str(ROOT / "README.md") + ";.",
            "--exclude-module", "tkinter", "--exclude-module", "PyQt5",
            "--exclude-module", "PyQt6", "--exclude-module", "PySide2"]
    pyside = Path(PySide6.__file__).resolve().parent
    for binary in sorted(pyside.glob("*140*.dll")):
        argv += ["--add-binary", str(binary) + ";."]
    argv.append(str(ROOT / "spng_viewer.py"))
    env = os.environ.copy()
    windows = Path(env.get("SystemRoot", r"C:\Windows"))
    env["PATH"] = os.pathsep.join([str(Path(sys.executable).parent), sys.base_prefix,
                                  str(windows / "System32"), str(windows)])
    env["PYTHONDONTWRITEBYTECODE"] = "1"
    env["PYINSTALLER_CONFIG_DIR"] = str(work / "pyinstaller_config")
    log = work / f"build_{kind}.log"
    started = datetime.datetime.now(datetime.timezone.utc).isoformat()
    print(f"Building {kind}; log: {log}", flush=True)
    try:
        with log.open("w", encoding="utf-8") as stream:
            subprocess.run(argv, cwd=ROOT, env=env, stdout=stream,
                           stderr=subprocess.STDOUT, check=True)
    except subprocess.CalledProcessError as exc:
        print("\n".join(log.read_text("utf-8", errors="replace").splitlines()[-35:]),
              file=sys.stderr)
        print("BUILD FAILED. See the build log for details.", file=sys.stderr)
        return exc.returncode or 1
    exe = dist / "SPNGViewer.exe" if args.onefile else dist / "SPNGViewer" / "SPNGViewer.exe"
    if not exe.is_file():
        raise RuntimeError("PyInstaller reported success but the executable is missing")
    record = {"kind": kind, "started_utc": started, "versions": versions,
              "exe": exe.relative_to(ROOT).as_posix(),
              "sha256": hashlib.sha256(exe.read_bytes()).hexdigest(),
              "source_hashes": {name: hashlib.sha256((ROOT / name).read_bytes()).hexdigest()
                                for name in MODULES + ["build_exe.py"]}}
    (dist / "build_manifest.json").write_text(json.dumps(record, indent=2), encoding="utf-8")
    for name in ["README.md", "LICENSE", "THIRD_PARTY_NOTICES.md"]:
        shutil.copy2(ROOT / name, dist / name)
    shutil.copytree(notices, dist / "licenses", dirs_exist_ok=True)
    shutil.copytree(ROOT / "docs", dist / "docs", dirs_exist_ok=True)
    source = dist / "source"
    source.mkdir(exist_ok=True)
    for name in MODULES + ["build_exe.py", "requirements.txt", "requirements-tested.txt",
                            "requirements-build.txt", "requirements-build-tested.txt",
                            "LICENSE", "README.md", "THIRD_PARTY_NOTICES.md"]:
        shutil.copy2(ROOT / name, source / name)
    for folder in ["build_hooks", "licenses", "docs", "scripts", "tests"]:
        shutil.copytree(ROOT / folder, source / folder, dirs_exist_ok=True,
                        ignore=shutil.ignore_patterns("__pycache__", "_generated", "*.pyc"))
    print(f"Build succeeded: {exe}", flush=True)
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
