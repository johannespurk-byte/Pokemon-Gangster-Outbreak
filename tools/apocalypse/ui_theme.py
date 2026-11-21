#!/usr/bin/env python3
"""Gangster Outbreak: villain look for text boxes and text colors.

Dark boxes with a blood-red frame, light text with a dark shadow. Text colors keep their meaning
(red / green / blue speakers) but are brightened so they stay readable on black.

Always starts from the ORIGINAL files in git (BASE_REF), so it can be re-run with new colors.
"""
import colorsys, io, os, subprocess
from PIL import Image

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BASE_REF = 'ddfe1f3d'

BG = (24, 20, 24)
TEXT = (232, 224, 216)
SHADOW = (72, 64, 64)
RED, RED_SH = (248, 96, 80), (104, 24, 24)
GREEN, GREEN_SH = (136, 216, 104), (32, 80, 24)
BLUE, BLUE_SH = (144, 176, 248), (40, 48, 104)
FRAME = [(96, 16, 16), (152, 24, 24), (200, 48, 32), (232, 96, 64)]  # dark -> light

# The standard text color slots shared by all window palettes
TEXT_SLOTS = {1: BG, 2: TEXT, 3: SHADOW, 4: RED, 5: RED_SH, 6: GREEN, 7: GREEN_SH, 8: BLUE, 9: BLUE_SH}


def snap(c):
    return tuple(max(0, min(255, int(v))) // 8 * 8 for v in c)


def theme_color(rgb):
    """Frame / decoration colors: whites become dark panel, colored parts become the red frame."""
    r, g, b = (v / 255 for v in rgb)
    h, s, v = colorsys.rgb_to_hsv(r, g, b)
    if s < 0.12:
        if v > 0.85:
            return BG
        if v > 0.6:
            return (64, 56, 56)
        return (40, 32, 32)
    # colored frame parts -> red ramp by brightness
    return FRAME[min(3, int(v * 4))]


def theme_palette(colors, text_slots=True):
    out = []
    for i, c in enumerate(colors):
        if i == 0:
            out.append(c)
        elif text_slots and i in TEXT_SLOTS:
            out.append(TEXT_SLOTS[i])
        else:
            out.append(theme_color(c))
    return [snap(c) if i else c for i, c in enumerate(out)]


def git_bytes(path):
    return subprocess.run(['git', '-C', ROOT, 'show', '%s:%s' % (BASE_REF, path)], capture_output=True, check=True).stdout


def do_png(rel, text_slots=True):
    im = Image.open(io.BytesIO(git_bytes(rel)))
    pal = im.getpalette()[:48]
    colors = [tuple(pal[i * 3:i * 3 + 3]) for i in range(16)]
    new = theme_palette(colors, text_slots)
    full = im.getpalette()
    for i, c in enumerate(new):
        full[i * 3:i * 3 + 3] = list(c)
    im.putpalette(full)
    im.save(os.path.join(ROOT, rel), bits=4 if im.getextrema()[1] < 16 else 8)


def do_pal(rel, fn):
    text = git_bytes(rel).decode().replace('\r', '')
    lines = text.split('\n')
    n = int(lines[2])
    colors = [tuple(int(v) for v in l.split()) for l in lines[3:3 + n]]
    new = fn(colors)
    with open(os.path.join(ROOT, rel), 'w', newline='\r\n') as f:
        f.write('JASC-PAL\n0100\n%d\n' % n + ''.join('%d %d %d\n' % c for c in new))


def battle_textbox_0(colors):
    out = list(colors)
    out[15] = BG               # message box fill (was teal)
    out[1] = TEXT              # message text
    for i in (12, 13, 14):     # frame (red/pink) -> blood red
        out[i] = FRAME[i - 12 + 1]
    return [snap(c) if i else c for i, c in enumerate(out)]


def battle_text(colors):
    out = list(colors)
    out[13], out[14], out[15] = TEXT, BG, SHADOW  # action menu / move list: text, background, shadow
    return [snap(c) if i else c for i, c in enumerate(out)]


def main():
    for rel in ('graphics/text_window/message_box.png', 'graphics/text_window/name_box.png',
                'graphics/text_window/std.png'):
        do_png(rel)
    for n in range(1, 21):  # window frames selectable in the options
        do_png('graphics/text_window/%d.png' % n, text_slots=False)
    for n in range(1, 5):
        do_pal('graphics/text_window/text_pal%d.pal' % n, theme_palette)
    do_pal('graphics/interface/std_menu.pal', theme_palette)
    do_pal('graphics/battle_interface/textbox_0.pal', battle_textbox_0)
    do_pal('graphics/battle_interface/text.pal', battle_text)


if __name__ == '__main__':
    main()
