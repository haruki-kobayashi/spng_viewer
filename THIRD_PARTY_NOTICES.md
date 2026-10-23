# Third-party notices

The application implementation and palette designs are licensed under MIT. Runtime dependencies are distributed under their respective licenses.

## Original palettes

The kingfisher, meadow, ember and heat palettes in spng_palettes.py are original
procedural designs, distributed under the application's MIT license.
rainbow is a generic HSV-to-RGB construction using Python colorsys.
Standard sRGB and CIE lightness equations are used for color calculations.
See docs/palettes_ja.md for design details and limitations.

## Runtime dependencies

- PySide6 / Shiboken / Qt: LGPL-3.0 and other upstream terms, depending on the components. See https://doc.qt.io/qtforpython-6/licenses.html and https://doc.qt.io/qt-6/licensing.html . Upstream sources: https://code.qt.io/cgit/pyside/pyside-setup.git/ and https://code.qt.io/cgit/qt/qtbase.git/ .
- Pillow: HPND-style license; see the installed distribution and https://github.com/python-pillow/Pillow .
- NumPy: BSD-3-Clause plus bundled third-party components; see the installed distribution and https://github.com/numpy/numpy .
- OpenCV / opencv-python-headless: Apache-2.0 and bundled component terms; see the installed distribution and https://github.com/opencv/opencv-python .
- Python: PSF license and included third-party notices.
- PyInstaller (build tool): GPL with its bootloader exception; see the installed distribution and https://pyinstaller.org/en/stable/license.html .

Executable distributions include license notices collected from the installed dependencies, the license texts under licenses/, application source, and rebuild instructions.

The GPL-3.0 text is included because the LGPL-3.0 text incorporates applicable GPL terms. Its inclusion does not change the application's MIT implementation license.
