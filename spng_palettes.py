# SPDX-License-Identifier: MIT
"""Original procedural palettes for SPNG Viewer.

Hue paths and lightness curves define the palette designs. Relative luminance
is prescribed independently of hue. Gamut bounds are solved analytically
instead of clipping out-of-gamut colors.
"""
import colorsys
from functools import lru_cache
import numpy as np

PALETTE_NAMES = ("kingfisher", "meadow", "ember", "heat", "rainbow")
# name: CIE L* endpoints, unwrapped HSV hue path (degrees), chroma fraction.
_DESIGNS = {
    "kingfisher":  ((14.0, 95.0), (235.0, 200.0, 176.0, 60.0), 0.94),
    "meadow": ((10.0, 94.0), (309.0, 248.0, 160.0, 60.0), 0.90),
    "ember":  ((5.0, 97.0), (282.0, 310.0, 365.0, 425.0), 0.91),
    "heat":   ((0.0, 100.0), (8.0, 18.0, 43.0, 73.0), 0.93),
}
_Y = np.array([0.2126, 0.7152, 0.0722], dtype=np.float64)

def srgb_to_linear(rgb):
    rgb = np.asarray(rgb, dtype=np.float64)
    return np.where(rgb <= 0.04045, rgb / 12.92,
                    ((rgb + 0.055) / 1.055) ** 2.4)

def relative_luminance(rgb):
    return srgb_to_linear(rgb) @ _Y

def _encode(rgb):
    return np.where(rgb <= 0.0031308, 12.92 * rgb,
                    1.055 * np.maximum(rgb, 0) ** (1 / 2.4) - 0.055)

def _lightness_to_y(lightness):
    f = (lightness + 16) / 116
    return np.where(lightness > 8, f ** 3, lightness / (24389 / 27))

def _hue_path(t, controls):
    position = t * (len(controls) - 1)
    index = np.minimum(np.floor(position).astype(int), len(controls) - 2)
    u = position - index
    smooth = u * u * (3 - 2 * u)
    values = np.asarray(controls)
    return values[index] + (values[index + 1] - values[index]) * smooth

def _quantize_monotonic(rgb, target_y):
    """8-bit quantization with local +/-1-code corrections if Y would reverse."""
    quantized = np.rint(np.clip(rgb, 0, 1) * 255).astype(np.int16)
    previous = -1.0
    offsets = np.array([(r, g, b) for r in (-1, 0, 1)
                        for g in (-1, 0, 1) for b in (-1, 0, 1)])
    for i in range(len(quantized)):
        y = float(relative_luminance(quantized[i] / 255))
        if y <= previous:
            best = None
            # Normally a single code value is sufficient; retain a bounded fallback.
            for radius in range(1, 9):
                candidates = np.clip(quantized[i] + offsets * radius, 0, 255)
                values = relative_luminance(candidates / 255)
                valid = values > previous + 1e-12
                if np.any(valid):
                    candidates, values = candidates[valid], values[valid]
                    cost = np.sum((candidates / 255 - rgb[i]) ** 2, axis=1)
                    cost += 10 * (values - target_y[i]) ** 2
                    j = int(np.argmin(cost))
                    best = candidates[j], float(values[j])
                    break
            if best is None:
                raise ValueError("Too many samples for strictly monotonic 8-bit luminance")
            quantized[i], y = best
        previous = y
    return quantized.astype(np.uint8)

@lru_cache(maxsize=32)
def _table(name, n):
    if name not in PALETTE_NAMES:
        raise ValueError(f"Unknown palette: {name}")
    if not isinstance(n, int) or not 1 <= n <= 4096:
        raise ValueError("Palette size must be an integer in 1..4096")
    t = np.linspace(0, 1, n)
    if name == "rainbow":
        # A generic HSV rainbow with non-monotonic luminance.
        return np.array([colorsys.hsv_to_rgb((2 / 3) * (1 - x), .95, 1)
                         for x in t], dtype=np.float32)
    limits, controls, chroma = _DESIGNS[name]
    # Allocate more lightness contrast to low values in sequential palettes.
    # Derivative 0.65..1.35 preserves increasing lightness and both endpoints.
    lightness_position = t + 0.35 * t * (1 - t)
    target = _lightness_to_y(limits[0] + (limits[1] - limits[0]) * lightness_position)
    hue = _hue_path(t, controls)
    if name == "kingfisher":
        # Orange precedes the brighter yellow; move yellow-green to lower values.
        positions = np.array([0.0, .22, .42, .52, .70, 1.0])
        hues = np.array([235.0, 200.0, 155.0, 105.0, 48.0, 60.0])
    hue_rgb = np.array([colorsys.hsv_to_rgb((h % 360) / 360, 1, 1) for h in hue])
    linear = srgb_to_linear(hue_rgb)
    if name == "kingfisher":
        # Blend hue directions in linear RGB for a gradual green/orange transition.
        anchor_rgb = np.array([colorsys.hsv_to_rgb(h / 360, 1, 1) for h in hues])
        anchor_linear = srgb_to_linear(anchor_rgb)
        linear = np.column_stack([np.interp(t, positions, anchor_linear[:,c])
                                  for c in range(3)])
    displacement = linear - (linear @ _Y)[:, None]
    # Largest legal chroma at the prescribed Y. Weighted sum of displacement is 0.
    positive = np.divide(1 - target[:, None], displacement,
                         out=np.full_like(displacement, np.inf), where=displacement > 1e-12)
    negative = np.divide(-target[:, None], displacement,
                         out=np.full_like(displacement, np.inf), where=displacement < -1e-12)
    maximum = np.minimum(positive, negative).min(axis=1)
    amplitude = chroma * (.72 + .28 * np.sin(np.pi * t))
    if name == "kingfisher":
        # Vivid kingfisher blue/cyan/orange, ending in yellow with no white mix.
        # Changing chroma here leaves the prescribed luminance unchanged.
        amplitude = np.minimum(chroma * (.90 + .10 * np.sin(np.pi * t))
                               + .15 * t ** 2 + .08 * t ** 8, 1.0)
    rgb_linear = target[:, None] + (maximum * amplitude)[:, None] * displacement
    rgb = _encode(np.clip(rgb_linear, 0, 1))  # only roundoff at the gamut boundary
    return _quantize_monotonic(rgb, target).astype(np.float32) / 255

def palette_lut(name="kingfisher", n=256):
    """Return an independent n x 3 float32 array in the sRGB 0..1 range."""
    return _table(name, n).copy()

# Compatibility registry of the four original sequential palettes.
PALETTES = {name: palette_lut(name).tolist() for name in PALETTE_NAMES if name != "rainbow"}
