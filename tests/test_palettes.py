"""Properties required by the original 8-bit sequential palettes."""
import unittest
import numpy as np
from spng_palettes import PALETTES, palette_lut, relative_luminance

class PaletteTests(unittest.TestCase):
    def test_quantized_luminance_and_gamut(self):
        for name in PALETTES:
            with self.subTest(name=name):
                rgb = palette_lut(name)
                self.assertEqual(rgb.shape, (256, 3))
                self.assertTrue(np.isfinite(rgb).all())
                self.assertTrue(((rgb >= 0) & (rgb <= 1)).all())
                quantized = np.rint(rgb * 255).astype(np.uint8)
                self.assertEqual(len(np.unique(quantized, axis=0)), 256)
                self.assertTrue((np.diff(relative_luminance(quantized / 255)) > 0).all())

    def test_copy_and_alternate_sizes(self):
        for name in PALETTES:
            for n in (1, 2, 16, 32, 256):
                original = palette_lut(name, n)
                copy = palette_lut(name, n)
                copy[:] = 0
                np.testing.assert_array_equal(original, palette_lut(name, n))
                self.assertEqual(original.shape, (n, 3))

    def test_reject_unknown_palette(self):
        with self.assertRaises(ValueError):
            palette_lut('not_a_palette')
