#!/usr/bin/env python3
"""Minimal PNG decoder + pixel checks for the a2mcp HGR render-mode tests.

Only what tests/run-tests.sh needs: an 8-bit RGB (colour type 2), non-
interlaced PNG -- exactly what lib/a2mcp/mcp_screen.cpp's stb_image_write
call always produces. No external dependencies beyond the standard library
(zlib, struct), same spirit as the od/dd byte-poking the image test's
neighbours already do in run-tests.sh.

Usage:
    check_hgr_png.py bars <path.png>          -- HCOLOR colour-bar oracle check
    check_hgr_png.py mono <path.png>          -- only pure black/white pixels
    check_hgr_png.py pairs <path.png>         -- columns 2k and 2k+1 are identical
    check_hgr_png.py row <path.png> <y> <white|black>
                                               -- row y is solid white/black
    check_hgr_png.py dims <path.png> <width> <height>
                                               -- IHDR reports exactly this size
    check_hgr_png.py downsample <path140.png> <path280.png>
                                               -- column k of the 140-wide image equals
                                                  columns 2k and 2k+1 of the 280-wide one,
                                                  for every row -- scale:1 is losslessly
                                                  the un-doubled form of scale:2, not a
                                                  resample, a crop, or a different decode.
    check_hgr_png.py rows-equal <pathA.png> <pathB.png> <y0> <y1>
                                               -- rows y0..y1 (inclusive) are pixel-identical
                                                  between the two same-size images.
    check_hgr_png.py rows-differ <pathA.png> <pathB.png> <y0> <y1>
                                               -- rows y0..y1 (inclusive) have at least one
                                                  differing pixel between the two images.
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


def check_dims(path, want_w, want_h):
    w, h, _rows = read_png(path)
    print("dims: got %dx%d want %dx%d" % (w, h, want_w, want_h))
    sys.exit(0 if (w, h) == (want_w, want_h) else 1)


def check_downsample(path140, path280):
    w140, h140, rows140 = read_png(path140)
    w280, h280, rows280 = read_png(path280)
    if w280 != w140 * 2 or h280 != h140:
        print(
            "shape mismatch: %dx%d (scale 1) vs %dx%d (scale 2)"
            % (w140, h140, w280, h280)
        )
        sys.exit(1)
    bad = 0
    for y in range(h140):
        for k in range(w140):
            a = rows140[y][k]
            b0 = rows280[y][2 * k]
            b1 = rows280[y][2 * k + 1]
            if not (a == b0 == b1):
                bad += 1
    print("cells where scale:1 disagrees with scale:2's pair: %d" % bad)
    sys.exit(0 if bad == 0 else 1)


def check_rows(mode, path_a, path_b, y0, y1):
    """Compare two same-size PNGs over screen rows y0..y1 inclusive.

    "rows-equal" wants every pixel in that band identical between the two
    images -- the mixed-mode text window, which the renderer draws the same
    way (draw_glyph_cb) no matter which a2_hgr_mode_t produced the rest of
    the screen. "rows-differ" wants at least one mismatch -- used on the
    graphics rows above the text window, to confirm the two renders actually
    decode the bitmap differently rather than the band under test happening
    to be blank in both.
    """
    wa, ha, rows_a = read_png(path_a)
    wb, hb, rows_b = read_png(path_b)
    if (wa, ha) != (wb, hb):
        print("shape mismatch: %dx%d vs %dx%d" % (wa, ha, wb, hb))
        sys.exit(1)
    bad = sum(1 for y in range(y0, y1 + 1) for x in range(wa) if rows_a[y][x] != rows_b[y][x])
    print(
        "rows %d-%d: %d differing pixels out of %d (want %s)"
        % (y0, y1, bad, wa * (y1 - y0 + 1), "0" if mode == "rows-equal" else ">0")
    )
    ok = bad == 0 if mode == "rows-equal" else bad > 0
    sys.exit(0 if ok else 1)


def check_row(path, y, expect):
    _w, _h, rows = read_png(path)
    row = rows[y]
    black = sum(1 for px in row if px == (0, 0, 0))
    white = sum(1 for px in row if px == (255, 255, 255))
    other = len(row) - black - white
    print("row %3d: black=%d white=%d other=%d (want solid %s)" % (y, black, white, other, expect))
    if expect == "white":
        ok = black == 0 and other == 0
    elif expect == "black":
        ok = white == 0 and other == 0
    else:
        print("unknown expect %r" % expect, file=sys.stderr)
        sys.exit(2)
    sys.exit(0 if ok else 1)


def main(argv):
    if len(argv) < 2:
        print(__doc__, file=sys.stderr)
        return 2
    mode = argv[1]
    if mode == "row":
        if len(argv) != 5:
            print("usage: check_hgr_png.py row <path.png> <y> <white|black>", file=sys.stderr)
            return 2
        check_row(argv[2], int(argv[3]), argv[4])
        return 0
    if mode == "dims":
        if len(argv) != 5:
            print("usage: check_hgr_png.py dims <path.png> <width> <height>", file=sys.stderr)
            return 2
        check_dims(argv[2], int(argv[3]), int(argv[4]))
        return 0
    if mode == "downsample":
        if len(argv) != 4:
            print("usage: check_hgr_png.py downsample <path140.png> <path280.png>", file=sys.stderr)
            return 2
        check_downsample(argv[2], argv[3])
        return 0
    if mode in ("rows-equal", "rows-differ"):
        if len(argv) != 6:
            print(
                "usage: check_hgr_png.py %s <pathA> <pathB> <y0> <y1>" % mode,
                file=sys.stderr,
            )
            return 2
        check_rows(mode, argv[2], argv[3], int(argv[4]), int(argv[5]))
        return 0
    if len(argv) != 3:
        print("usage: check_hgr_png.py {bars|mono|pairs} <path.png>", file=sys.stderr)
        return 2
    path = argv[2]
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
