"""Read-only SPNG access and PNG export; no Qt dependency.

SPNG: repeated little-endian uint64 payload length followed by PNG bytes.
UINT64_MAX or zero denotes an unavailable image and preserves its layer index.
"""
from __future__ import annotations
from dataclasses import dataclass
from pathlib import Path
from io import BytesIO
import math
import os
import re
import struct
from typing import Optional
import numpy as np
from PIL import Image

PNG_SIGNATURE = b'\x89PNG\r\n\x1a\n'
MISSING = (1 << 64) - 1
SENSOR_RE = re.compile(r'^\d{2}_\d{2}$')
FILE_RE = re.compile(r'^(?P<kind>.+)_(?P<view>\d{8})_(?P<layer>\d+)_(?P<count>\d+)\.spng$', re.I)

class SpngError(ValueError):
    pass

@dataclass(frozen=True)
class FrameRecord:
    offset: int
    length: int
    width: int = 0
    height: int = 0
    depth: int = 0
    color_type: int = 0
    @property
    def missing(self):
        return self.length in (0, MISSING)

class SpngStack:
    """Index headers only. Decode exactly one PNG per read_frame call."""
    def __init__(self, path):
        self.path = Path(path)
        self.frames: list[FrameRecord] = []
        self.size_bytes = self.path.stat().st_size
        self.mtime_ns = self.path.stat().st_mtime_ns
        with self.path.open('rb') as f:
            while f.tell() < self.size_bytes:
                header_at = f.tell()
                header = f.read(8)
                if len(header) != 8:
                    raise SpngError(f'層ヘッダが途中で切れています: byte {header_at}')
                length = struct.unpack('<Q', header)[0]
                offset = f.tell()
                if length in (0, MISSING):
                    self.frames.append(FrameRecord(offset, length))
                    continue
                if length > self.size_bytes - offset or length < 33:
                    raise SpngError(f'PNGデータの長さが不正です: 層 {len(self.frames)}, byte {header_at}')
                ihdr = f.read(33)
                if ihdr[:8] != PNG_SIGNATURE or ihdr[12:16] != b'IHDR' or ihdr[8:12] != struct.pack('>I', 13):
                    raise SpngError(f'PNGのヘッダが不正です: 層 {len(self.frames)}')
                width, height, depth, color, *_ = struct.unpack('>IIBBBBB', ihdr[16:29])
                if not width or not height:
                    raise SpngError(f'画像サイズが不正です: 層 {len(self.frames)}')
                self.frames.append(FrameRecord(offset, length, width, height, depth, color))
                f.seek(offset + length)
        if not self.frames:
            raise SpngError('画像が1枚もありません。')

    def __len__(self):
        return len(self.frames)

    def read_frame(self, index: int) -> Optional[Image.Image]:
        record = self.frames[index]
        if record.missing:
            return None
        if record.depth == 16 and record.color_type != 0:
            raise SpngError('16bitカラー／alpha付き16bit PNGは未対応です。画素値を保つため変換せず停止しました。16bitグレースケールは開けます。')
        with self.path.open('rb') as f:
            f.seek(record.offset)
            payload = f.read(record.length)
        if len(payload) != record.length:
            raise SpngError('画像の読み込み途中でファイル長が変化しました。開き直してください。')
        try:
            with Image.open(BytesIO(payload)) as im:
                if im.format != 'PNG':
                    raise SpngError('PNG以外の画像が格納されています。')
                im.load()
                return im.copy()
        except Exception as exc:
            if isinstance(exc, SpngError):
                raise
            raise SpngError(f'PNGを展開できません: {exc}') from exc

    def first_available(self):
        return next((i for i, r in enumerate(self.frames) if not r.missing), 0)

@dataclass(frozen=True)
class FileRecord:
    path: Path
    kind: str
    view: str
    layer: str

    @classmethod
    def from_path(cls, path):
        path = Path(path)
        match = FILE_RE.match(path.name)
        if match:
            return cls(path, match['kind'], str(int(match['view'])), 'Layer' + match['layer'])
        return cls(path, 'その他のファイル', path.stem, '指定なし')


def natural_key(value):
    return [int(t) if t.isdigit() else t.casefold() for t in re.split(r'(\d+)', str(value))]


def discover_root(path):
    """Only list direct child folders; never recurse into the huge dataset."""
    root = Path(path)
    if not root.is_dir():
        raise SpngError('フォルダが見つかりません。')
    with os.scandir(root) as it:
        entries = list(it)
    containers = {e.name.upper(): Path(e.path) for e in entries if e.is_dir() and e.name.upper() in ('IMAGE', 'DATA')}
    if containers:
        return {'root': root, 'containers': [(k, containers[k]) for k in ('IMAGE', 'DATA') if k in containers]}
    return {'root': root, 'containers': [(root.name or str(root), root)]}


def discover_sensors(path):
    root = Path(path)
    with os.scandir(root) as it:
        entries = list(it)
    sensors = sorted([(e.name, Path(e.path)) for e in entries if e.is_dir() and SENSOR_RE.match(e.name)], key=lambda p: natural_key(p[0]))
    has_direct = any(e.is_file() and e.name.lower().endswith('.spng') for e in entries)
    if has_direct or not sensors:
        sensors.insert(0, (root.name if SENSOR_RE.match(root.name) else 'このフォルダ', root))
    return sensors


def list_sensor_files(path):
    with os.scandir(path) as it:
        records = [FileRecord.from_path(e.path) for e in it if e.is_file() and e.name.lower().endswith('.spng')]
    return sorted(records, key=lambda r: natural_key(r.path.name))


def normalize_roi(x, y, width, height, image_size):
    """Pixel-edge coordinates, 0-based origin, exclusive right/bottom edge."""
    x, y, width, height = map(int, (x, y, width, height))
    iw, ih = map(int, image_size)
    if width <= 0 or height <= 0:
        raise ValueError('幅と高さは1 pixel以上にしてください。')
    if x < 0 or y < 0 or x + width > iw or y + height > ih:
        raise ValueError(f'範囲が画像の外です。画像サイズは {iw} × {ih} pixelです。')
    return (x, y, x + width, y + height)


def drag_roi(start, end, image_size):
    iw, ih = image_size
    x0 = max(0, min(iw, math.floor(min(start[0], end[0]))))
    y0 = max(0, min(ih, math.floor(min(start[1], end[1]))))
    x1 = max(0, min(iw, math.ceil(max(start[0], end[0]))))
    y1 = max(0, min(ih, math.ceil(max(start[1], end[1]))))
    if x1 <= x0 or y1 <= y0:
        return None
    return (x0, y0, x1, y1)


def save_png(image, path, roi=None):
    """Save source pixels, not the screen/contrast/overlays. Always PNG."""
    path = Path(path)
    if path.suffix.lower() != '.png':
        path = path.with_suffix('.png')
    if roi:
        x0, y0, x1, y1 = map(int, roi)
        normalize_roi(x0, y0, x1-x0, y1-y0, image.size)
        image = image.crop((x0, y0, x1, y1))
    image.save(path, format='PNG')
    return path


def display_array(image, low=0.0, high=255.0, invert=False):
    """Make an 8-bit display image; never alter the source PIL image."""
    if image.mode in ('P', 'LA'):
        image = image.convert('RGBA' if image.mode == 'LA' or 'transparency' in image.info else 'RGB')
    array = np.asarray(image)
    if array.dtype == np.bool_:
        array = array.astype(np.uint8)
    high = max(float(high), float(low)+1e-9)
    if array.dtype == np.uint8 and low == 0 and high == 255:
        out = np.array(array, copy=True)
    else:
        out = np.clip((array.astype(np.float64)-low)*(255.0/(high-low)), 0, 255).round().astype(np.uint8)
        if out.ndim == 3 and out.shape[2] == 4:
            out[..., 3] = array[..., 3].astype(np.uint8)
    if invert:
        if out.ndim == 3 and out.shape[2] == 4:
            out[..., :3] = 255-out[..., :3]
        else:
            out = 255-out
    return np.ascontiguousarray(out)


def default_levels(image):
    if image.mode == 'P':
        return (0, 255)
    a = np.asarray(image)
    if image.mode in ('1',) or (a.ndim == 2 and a.size and int(a.min()) >= 0 and int(a.max()) <= 1):
        return (0, 1)
    if image.mode.startswith('I;16') or (a.ndim == 2 and a.dtype.itemsize > 1):
        return (0, 65535)
    return (0, 255)


def bottom_left_roi_to_box(x, y, width, height, image_size):
    """Convert user bottom-left pixel-edge ROI to PIL top-left box."""
    x, y, width, height = map(int, (x, y, width, height))
    iw, ih = image_size
    normalize_roi(x, y, width, height, image_size)
    return (x, ih-y-height, x+width, ih-y)


def box_to_bottom_left_roi(box, image_size):
    x0, top, x1, bottom = map(int, box)
    return (x0, int(image_size[1])-bottom, x1-x0, bottom-top)
