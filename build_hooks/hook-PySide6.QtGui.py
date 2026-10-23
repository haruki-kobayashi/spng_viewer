"""Keep the Qt plugins needed for an image viewer, without optional viewers/input servers."""
from pathlib import Path
from PyInstaller.utils.hooks.qt import add_qt6_dependencies
hiddenimports,binaries,datas=add_qt6_dependencies(__file__)
unneeded={'qtvirtualkeyboardplugin.dll','qpdf.dll','qtuiotouchplugin.dll'}
binaries=[(src,dst) for src,dst in binaries if Path(src).name.lower() not in unneeded]
