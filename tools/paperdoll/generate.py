#!/usr/bin/env python3
"""
Pokemon Gangster Outbreak - character builder ("paper doll") generator.

Reads the player/villain overworld sheets, sorts every pixel into a colour role
(outline, skin, hair, hat, accent, top, bottom, white) and writes:
  graphics/object_events/pics/player_parts/<name>.png          18 walking/running frames, standard palette
  graphics/object_events/pics/player_parts/states/<name>_*.png bike/surf/fish/field move (Pokabbie heroes)
  src/data/player_custom_parts.h                               tables used by src/player_custom.c

Standard palette layout (index: role):
  0 transparent | 1 outline | 2-3 skin | 4-6 hair | 7-8 hat | 9 accent | 10-12 top | 13-14 bottom | 15 white
Walking frames are mixed row-wise: rows above the neck line come from the head template,
rows below from the outfit template. Run from the repository root:  python3 tools/paperdoll/generate.py
"""
import os, sys
from PIL import Image

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from spec import SPEC, PAL_FILES, PIC_ROLE_FIX  # noqa: E402

PICS = "graphics/object_events/pics/"
OUT = PICS + "player_parts/"
HEADER = "src/data/player_custom_parts.h"
NFRAMES = 18
SLOTS = {"outline": [1], "skin": [2, 3], "hair": [4, 5, 6], "hat": [7, 8], "accent": [9],
         "top": [10, 11, 12], "bottom": [13, 14], "white": [15]}

# id, display name, walking sheet, female?, template whose bike/surf/fish/field-move sprites are used
TEMPLATES = [
    ("red", "RED", "people/red/red_normal.png", False, "hilbert"),
    ("leaf", "LEAF", "people/leaf/green_normal.png", True, "hilda"),
    ("lucas", "LUCAS", "people/lucas.png", False, "lucas"),
    ("dawn", "DAWN", "people/dawn.png", True, "dawn"),
    ("hilbert", "HILBERT", "people/hilbert.png", False, "hilbert"),
    ("hilda", "HILDA", "people/hilda.png", True, "hilda"),
    ("nate", "NATE", "people/nate.png", False, "nate"),
    ("rosa", "ROSA", "people/rosa.png", True, "rosa"),
    ("calem", "CALEM", "people/calem.png", False, "calem"),
    ("serena", "SERENA", "people/serena.png", True, "serena"),
    ("elio", "ELIO", "people/elio.png", False, "elio"),
    ("selene", "SELENE", "people/selene.png", True, "selene"),
    ("victor", "VICTOR", "people/victor.png", False, "victor"),
    ("gloria", "GLORIA", "people/gloria.png", True, "gloria"),
    ("rocket_m", "ROCKET♂", "people/rocket_m.png", False, "hilbert"),
    ("rocket_f", "ROCKET♀", "people/rocket_f.png", True, "hilbert"),
    ("giovanni", "GIOVANNI", "people/giovanni.png", False, "nate"),
    ("cyrus", "CYRUS", "people/cyrus.png", False, "nate"),
    ("magma_oras", "MAGMA HOOD", "people/magma_grunt_oras.png", False, "dawn"),
    ("magma_m", "MAGMA♂", "people/team_magma/magma_member_m.png", False, "dawn"),
    ("magma_f", "MAGMA♀", "people/team_magma/magma_member_f.png", True, "dawn"),
    ("maxie", "MAXIE", "people/team_magma/maxie.png", False, "nate"),
    ("aqua_m", "AQUA♂", "people/team_aqua/aqua_member_m.png", False, "lucas"),
    ("aqua_f", "AQUA♀", "people/team_aqua/aqua_member_f.png", True, "lucas"),
    ("archie", "ARCHIE", "people/team_aqua/archie.png", False, "lucas"),
]
# trainer pics: (front pic id, front palette symbol, back pic id, back palette symbol); villains borrow a hero back pic
PICS_FOR = {
    "red": ("RED", "Red", "RED", "Red"), "leaf": ("LEAF", "Leaf", "LEAF", "Leaf"),
    "lucas": ("LUCAS", "Lucas", "LUCAS", "Lucas"), "dawn": ("DAWN", "Dawn", "DAWN", "Dawn"),
    "hilbert": ("HILBERT", "Hilbert", "HILBERT", "Hilbert"), "hilda": ("HILDA", "Hilda", "HILDA", "Hilda"),
    "nate": ("NATE", "Nate", "NATE", "Nate"), "rosa": ("ROSA", "Rosa", "ROSA", "Rosa"),
    "calem": ("CALEM", "Calem", "CALEM", "Calem"), "serena": ("SERENA", "Serena", "SERENA", "Serena"),
    "elio": ("ELIO", "Elio", "ELIO", "Elio"), "selene": ("SELENE", "Selene", "SELENE", "Selene"),
    "victor": ("VICTOR", "Victor", "VICTOR", "Victor"), "gloria": ("GLORIA", "Gloria", "GLORIA", "Gloria"),
    "rocket_m": ("ROCKET_GRUNT_M_FRLG", "RocketGruntMFrlg", "HILBERT", "Hilbert"),
    "rocket_f": ("ROCKET_GRUNT_F_FRLG", "RocketGruntFFrlg", "HILDA", "Hilda"),
    "giovanni": ("LEADER_GIOVANNI_FRLG", "LeaderGiovanniFrlg", "NATE", "Nate"),
    "cyrus": ("CYRUS", "Cyrus", "NATE", "Nate"),
    "magma_oras": ("MAGMA_GRUNT_M", "MagmaGruntM", "HILBERT", "Hilbert"),
    "magma_m": ("MAGMA_GRUNT_M", "MagmaGruntM", "HILBERT", "Hilbert"),
    "magma_f": ("MAGMA_GRUNT_F", "MagmaGruntF", "HILDA", "Hilda"),
    "maxie": ("MAGMA_LEADER_MAXIE", "MagmaLeaderMaxie", "NATE", "Nate"),
    "aqua_m": ("AQUA_GRUNT_M", "AquaGruntM", "HILBERT", "Hilbert"),
    "aqua_f": ("AQUA_GRUNT_F", "AquaGruntF", "HILDA", "Hilda"),
    "archie": ("AQUA_LEADER_ARCHIE", "AquaLeaderArchie", "LUCAS", "Lucas"),
}
# pic palette roles: 0 keep, 1 skin, 2 hair, 3 hat, 4 top, 5 bottom (stored as role << 2 | shade)
PIC_ROLES = {"skin": 1, "hair": 2, "hat": 3, "top": 4, "bottom": 5}
SLOT_ROLE = {s: r for r, slots in SLOTS.items() for s in slots}

STATE_HEADS = ["lucas", "dawn", "hilbert", "hilda", "nate", "rosa", "calem", "serena", "elio", "selene", "victor", "gloria"]
STATES = [("bike", "mach_bike", 9), ("surf", "surfing", 6), ("fish", "fishing", 12), ("fieldmove", "field_move", 5)]


def lum(c):
    return 0.3 * c[0] + 0.59 * c[1] + 0.11 * c[2]


def read_jasc(path):
    v = list(map(int, open(path).read().split()[3:3 + 48]))
    return [tuple(v[i:i + 3]) for i in range(0, 48, 3)]


def palette_of(name, im):
    if name in PAL_FILES:
        return read_jasc("graphics/object_events/" + PAL_FILES[name])
    p = im.getpalette()
    return [tuple(p[i * 3:i * 3 + 3]) for i in range(16)]


def head_top(frame):
    p = frame.load()
    for y in range(32):
        if any(p[x, y] for x in range(frame.width)):
            return y
    return 12


def pixel_role(name, idx, y, neck):
    sp = SPEC[name]
    for role in ("outline", "white", "skin", "accent"):
        if idx in sp.get(role, ()):
            return role
    if y < neck:
        return "hat" if idx in sp["hat"] else "hair"
    return "bottom" if idx in sp["bottom"] else "top"


def standardise(name):
    """walking sheet -> (frames in standard slots, standard palette, slot of each original index, neck rows)"""
    im = Image.open(PICS + dict((t[0], t[2]) for t in TEMPLATES)[name])
    pal = palette_of(name, im)
    frames = [im.crop((i * 16, 0, (i + 1) * 16, 32)) for i in range(im.width // 16)]
    while len(frames) < NFRAMES:  # 9-frame sheets: running reuses the walking frames
        frames.append(frames[len(frames) - 9])
    usage, roles, necks = {}, [], []
    for f in frames:
        neck = head_top(f) + 11
        necks.append(neck)
        p = f.load()
        rf = {}
        for y in range(32):
            for x in range(16):
                v = p[x, y]
                if v:
                    r = pixel_role(name, v, y, neck)
                    rf[x, y] = (r, v)
                    usage.setdefault(r, set()).add(v)
        roles.append(rf)
    slotmap, stdpal = {}, [(255, 0, 255)] + [None] * 15
    for role, slots in SLOTS.items():
        cols = sorted(usage.get(role, ()), key=lambda i: -lum(pal[i]))
        groups = [[] for _ in slots]
        for k, i in enumerate(cols):
            groups[min(len(slots) - 1, k * len(slots) // max(1, len(cols)))].append(i)
        for g, slot in zip(groups, slots):
            for i in g:
                slotmap[role, i] = slot
            if g:
                stdpal[slot] = tuple(sum(pal[i][k] for i in g) // len(g) for k in range(3))
    # fill empty roles so every slot has a sensible colour (e.g. no hat -> hair colours)
    fallback = {"hat": "hair", "accent": "top", "bottom": "top", "white": "skin"}
    for role, other in fallback.items():
        for k, s in enumerate(SLOTS[role]):
            if stdpal[s] is None:
                src = SLOTS[other][min(k, len(SLOTS[other]) - 1)]
                stdpal[s] = stdpal[src]
    stdpal = [c if c else (0, 0, 0) for c in stdpal]
    out = []
    count = {}
    for rf in roles:
        g = Image.new("P", (16, 32), 0)
        gp = g.load()
        for (x, y), (r, v) in rf.items():
            s = slotmap[r, v]
            gp[x, y] = s
            count.setdefault(v, {}).setdefault(s, 0)
            count[v][s] += 1
        out.append(g)
    by_index = {v: max(c, key=c.get) for v, c in count.items()}
    standardise.origpal[name] = pal
    return out, stdpal, by_index, necks


standardise.origpal = {}


def pal_path(symbol, back):
    import re
    src = open("src/data/graphics/trainers.h").read()
    var = ("gTrainerBackPicPalette_" if back else "gTrainerPalette_") + symbol
    m = re.search(re.escape(var) + r'\[\] = INCGFX_U16\("([^"]+)"', src)
    assert m, var
    path = m.group(1)
    if path.endswith(".pal"):
        return read_jasc(path)
    p = Image.open(path).getpalette()
    return [tuple(p[i * 3:i * 3 + 3]) for i in range(16)]


def pic_gfx_path(symbol, back):
    import re
    src = open("src/data/graphics/trainers.h").read()
    var = ("gTrainerBackPic_" if back else "gTrainerFrontPic_") + symbol
    m = re.search(re.escape(var) + r'\[\] = INCGFX_U(?:8|32)\("([^"]+)"', src)
    assert m, var
    return m.group(1)


def is_skin(c):
    r, g, b = c
    return r >= 0xa0 and r > g > b and 0.45 <= b / r <= 0.8 and 0.6 <= g / r <= 0.92


def pic_roles(ow_name, symbol, back):
    """sort each colour of a trainer pic into a role by its colour and where it is used on the body
    (head / torso / legs); colours used all over the body (outlines, shared shades) are kept.
    PIC_ROLE_FIX in spec.py corrects single colours by hand."""
    pal = pal_path(symbol, back)
    im = Image.open(pic_gfx_path(symbol, back)).crop((0, 0, 64, 64))
    px = im.load()
    bbox = im.point(lambda v: 255 if v else 0).getbbox()
    top, height = bbox[1], bbox[3] - bbox[1]
    zones = {}
    for y in range(64):
        for x in range(64):
            v = px[x, y]
            if v:
                ny = (y - top) / height
                if back:
                    z = "head" if ny < 0.5 else "torso"
                else:
                    z = "head" if ny < 0.25 else "torso" if ny < 0.55 else "legs" if ny < 0.92 else "feet"
                zones.setdefault(v, {}).setdefault(z, 0)
                zones[v][z] += 1
    ow = standardise.origpal[ow_name]
    sp = SPEC[ow_name]
    members = {}
    for i in range(1, 16):
        if i not in zones:
            continue
        c = pal[i]
        z = zones[i]
        total = sum(z.values())
        main, cnt = max(z.items(), key=lambda kv: kv[1])
        role = None
        if lum(c) < 45 or min(c) > 225:
            role = None
        elif is_skin(c):
            role = "skin"
        elif cnt >= 0.7 * total:
            if main == "head":
                # hat or hair: closest colour of the walking sheet decides
                best, bestd = "hair", 1e9
                for r in ("hat", "hair"):
                    for idx in (sp.get("hat", []) if r == "hat" else range(1, 16)):
                        if r == "hair" and (idx in sp.get("hat", []) or idx in sp.get("skin", []) or idx in sp.get("outline", []) or idx in sp.get("white", [])):
                            continue
                        d = sum((a - b) ** 2 for a, b in zip(c, ow[idx]))
                        if d < bestd:
                            best, bestd = r, d
                role = best
            elif main == "torso":
                role = "top"
            elif main == "legs":
                role = "bottom"
        role = PIC_ROLE_FIX.get((symbol, back), {}).get(i, role)
        if role:
            members.setdefault(role, []).append(i)
    roles = [0] * 16
    for role, idxs in members.items():
        n = len(SLOTS[role])
        idxs.sort(key=lambda i: -lum(pal[i]))
        for k, i in enumerate(idxs):
            roles[i] = (PIC_ROLES[role] << 2) | min(n - 1, k * n // len(idxs))
    return roles


def save_sheet(frames, pal, path):
    w = sum(f.width for f in frames)
    sheet = Image.new("P", (w, 32), 0)
    sheet.putpalette([v for c in pal for v in c])
    x = 0
    for f in frames:
        sheet.paste(f, (x, 0))
        x += f.width
    sheet.save(path)


def gba(c):
    return (c[0] >> 3) | ((c[1] >> 3) << 5) | ((c[2] >> 3) << 10)


def cname(name):
    return "".join(p.capitalize() for p in name.split("_"))


def main():
    os.makedirs(OUT + "states", exist_ok=True)
    std = {}
    for name, *_ in TEMPLATES:
        frames, pal, by_index, necks = standardise(name)
        std[name] = (pal, by_index, necks)
        save_sheet(frames, pal, OUT + name + ".png")
    picinfo = {}
    for name, *_ in TEMPLATES:
        fpic, fsym, bpic, bsym = PICS_FOR[name]
        back_owner = next(t[0] for t in TEMPLATES if PICS_FOR[t[0]][1] == bsym)
        picinfo[name] = (fpic, pic_roles(name, fsym, False),
                         bpic, pic_roles(back_owner, bsym, True), back_owner == name)
    for name in STATE_HEADS:
        pal, by_index, _ = std[name]
        for short, suffix, nframes in STATES:
            im = Image.open(PICS + "people/states/%s_%s.png" % (name, suffix))
            frames = [im.crop((i * 32, 0, (i + 1) * 32, 32)) for i in range(im.width // 32)]
            assert len(frames) == nframes, (name, suffix, len(frames))
            out = []
            for f in frames:
                g = Image.new("P", (32, 32), 0)
                gp, fp = g.load(), f.load()
                for y in range(32):
                    for x in range(32):
                        v = fp[x, y]
                        if v:
                            gp[x, y] = by_index.get(v, 1)
                out.append(g)
            save_sheet(out, pal, OUT + "states/%s_%s.png" % (name, short))

    L = ["// Generated by tools/paperdoll/generate.py - do not edit by hand.", ""]
    for name, *_ in TEMPLATES:
        L.append('static const u32 sPlayerPartGfx_%s[] = INCGFX_U32("%s%s.png", ".4bpp", "-mwidth 2 -mheight 4");' % (cname(name), OUT, name))
    for name in STATE_HEADS:
        for short, _, _ in STATES:
            L.append('static const u32 sPlayerPartGfx_%s_%s[] = INCGFX_U32("%sstates/%s_%s.png", ".4bpp", "-mwidth 4 -mheight 4");'
                     % (cname(name), cname(short), OUT, name, short))
    L.append("")
    for name in STATE_HEADS:
        for short, _, nframes in STATES:
            L.append("static const struct SpriteFrameImage sPlayerPartPics_%s_%s[] = {" % (cname(name), cname(short)))
            L.append("    overworld_ascending_frames(sPlayerPartGfx_%s_%s, 4, 4)," % (cname(name), cname(short)))
            L.append("};")
    L.append("")
    L.append("enum {")
    for name in STATE_HEADS:
        L.append("    PLAYER_STATE_HEAD_%s," % name.upper())
    L.append("};")
    L.append("")
    L.append("static const struct SpriteFrameImage *const sPlayerPartStatePics[][PLAYER_CUSTOM_STATE_COUNT] = {")
    for name in STATE_HEADS:
        L.append("    [PLAYER_STATE_HEAD_%s] = {" % name.upper())
        for short, _, _ in STATES:
            L.append("        [PLAYER_CUSTOM_STATE_%s] = sPlayerPartPics_%s_%s," % (short.upper(), cname(name), cname(short)))
        L.append("    },")
    L.append("};")
    L.append("")
    L.append("static const struct PlayerPartData sPlayerParts[PLAYER_PART_COUNT] = {")
    for name, disp, _, female, state_head in TEMPLATES:
        pal, _, necks = std[name]
        L.append("    [PLAYER_PART_%s] = {" % name.upper())
        L.append('        .name = COMPOUND_STRING("%s"),' % disp)
        L.append("        .gfx = sPlayerPartGfx_%s," % cname(name))
        L.append("        .palette = {%s}," % ", ".join("0x%04X" % gba(c) for c in pal))
        L.append("        .neck = {%s}," % ", ".join(str(n) for n in necks))
        L.append("        .stateHead = PLAYER_STATE_HEAD_%s," % state_head.upper())
        L.append("        .female = %s," % ("TRUE" if female else "FALSE"))
        fpic, froles, bpic, broles, ownback = picinfo[name]
        L.append("        .frontPic = TRAINER_PIC_%s," % fpic)
        L.append("        .backPic = TRAINER_PIC_%s," % bpic)
        L.append("        .ownBackPic = %s," % ("TRUE" if ownback else "FALSE"))
        L.append("        .frontRoles = {%s}," % ", ".join(str(r) for r in froles))
        L.append("        .backRoles = {%s}," % ", ".join(str(r) for r in broles))
        L.append("    },")
    L.append("};")
    open(HEADER, "w").write("\n".join(L) + "\n")

    with open("include/constants/player_custom.h", "w") as f:
        f.write("#ifndef GUARD_CONSTANTS_PLAYER_CUSTOM_H\n#define GUARD_CONSTANTS_PLAYER_CUSTOM_H\n\n")
        f.write("// Generated by tools/paperdoll/generate.py - do not edit by hand.\n")
        f.write("enum PlayerPart\n{\n")
        for name, *_ in TEMPLATES:
            f.write("    PLAYER_PART_%s,\n" % name.upper())
        f.write("    PLAYER_PART_COUNT\n};\n\n#endif // GUARD_CONSTANTS_PLAYER_CUSTOM_H\n")
    print("templates:", len(TEMPLATES), "state heads:", len(STATE_HEADS))


if __name__ == "__main__":
    main()
