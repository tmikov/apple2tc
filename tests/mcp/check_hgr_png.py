#!/usr/bin/env python3
"""Minimal PNG decoder + pixel checks for the a2mcp HGR render-mode tests.

Only what tests/run-tests.sh needs: an 8-bit RGB (colour type 2), non-
interlaced PNG -- exactly what lib/a2mcp/mcp_screen.cpp's stb_image_write
call always produces. No external dependencies beyond the standard library
(zlib, struct), same spirit as the od/dd byte-poking the image test's
neighbours already do in run-tests.sh.

Usage:
    check_hgr_png.py bars <path.png>   -- HCOLOR colour-bar oracle check
    check_hgr_png.py mono <path.png>   -- only pure black/white pixels
    check_hgr_png.py pairs <path.png>  -- columns 2k and 2k+1 are identical
"""
import struct
import sys
import zlib
from collections import Counter

PNG_SIG = b"\x89PNG\r\n\x1a\n"


def read_png(path):
    data = open(path, "rb").read()
    assert data[:8] == PNG_SIG, "not a PNG file: %s" % path
    pos = 8
    width = height = bitdepth = colortype = None
    idat = bytearray()
    while pos < len(data):
        (length,) = struct.unpack(">I", data[pos : pos + 4])
        ctype = data[pos + 4 : pos + 8]
        cdata = data[pos + 8 : pos + 8 + length]
        pos += 12 + length  # length + type + data + crc
        if ctype == b"IHDR":
            width, height, bitdepth, colortype = struct.unpack(">IIBB", cdata[:10])
        elif ctype == b"IDAT":
            idat += cdata
        elif ctype == b"IEND":
            break
    assert bitdepth == 8 and colortype == 2, (
        "expected 8-bit RGB (colour type 2), got bitdepth=%r colortype=%r" % (bitdepth, colortype)
    )
    raw = zlib.decompress(bytes(idat))

    bpp = 3
    stride = width * bpp
    rows = []
    prev = bytearray(stride)
    off = 0
    for _y in range(height):
        filter_type = raw[off]
        off += 1
        line = bytearray(raw[off : off + stride])
        off += stride
        for x in range(stride):
            a = line[x - bpp] if x >= bpp else 0
            b = prev[x]
            c = prev[x - bpp] if x >= bpp else 0
            if filter_type == 0:
                pass
            elif filter_type == 1:
                line[x] = (line[x] + a) & 0xFF
            elif filter_type == 2:
                line[x] = (line[x] + b) & 0xFF
            elif filter_type == 3:
                line[x] = (line[x] + (a + b) // 2) & 0xFF
            elif filter_type == 4:
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[x] = (line[x] + pr) & 0xFF
            else:
                raise ValueError("unknown PNG filter type %d" % filter_type)
        prev = line
        rows.append([tuple(line[x * 3 : x * 3 + 3]) for x in range(width)])
    return width, height, rows


# The Apple II HGR palette, as apple2_render_hgr_screen_mode()'s
# s_hgr_artifact_colors[]/white/black. Ground truth for what HCOLOR draws is
# documented Applesoft behaviour: 1=green, 2=violet, 3=white, 5=orange,
# 6=blue.
COLOR_NAMES = {
    (0, 0, 0): "black",
    (255, 255, 255): "white",
    (255, 68, 253): "violet",
    (20, 245, 60): "green",
    (20, 207, 253): "blue",
    (255, 106, 60): "orange",
}


def dominant_nonblack(row):
    counts = Counter(px for px in row if px != (0, 0, 0))
    if not counts:
        return None
    return counts.most_common(1)[0][0]


def check_bars(path):
    _w, _h, rows = read_png(path)
    # Row -> HCOLOR value -> expected colour name, matching the BASIC program
    # in docs/a2mcp-playing.md / the hgr-bars test fixture.
    expected = {10: "green", 40: "violet", 70: "orange", 100: "blue", 130: "white"}
    ok = True
    for y, want in sorted(expected.items()):
        dom = dominant_nonblack(rows[y])
        name = COLOR_NAMES.get(dom, "unknown%r" % (dom,))
        print("row %3d: want=%-7s got=%-7s %s" % (y, want, name, "OK" if name == want else "FAIL"))
        ok = ok and name == want
    sys.exit(0 if ok else 1)


def check_mono(path):
    _w, _h, rows = read_png(path)
    bad = sum(1 for row in rows for px in row if px not in ((0, 0, 0), (255, 255, 255)))
    print("non-black/white pixels: %d" % bad)
    sys.exit(0 if bad == 0 else 1)


def check_pairs(path):
    w, _h, rows = read_png(path)
    bad = sum(1 for row in rows for k in range(w // 2) if row[2 * k] != row[2 * k + 1])
    print("mismatched column pairs: %d" % bad)
    sys.exit(0 if bad == 0 else 1)


def main(argv):
    if len(argv) != 3:
        print("usage: check_hgr_png.py {bars|mono|pairs} <path.png>", file=sys.stderr)
        return 2
    mode, path = argv[1], argv[2]
    if mode == "bars":
        check_bars(path)
    elif mode == "mono":
        check_mono(path)
    elif mode == "pairs":
        check_pairs(path)
    else:
        print("unknown mode %r" % mode, file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
