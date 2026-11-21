#!/usr/bin/env python3
"""Port Pokémon Tribunal Arma's species changes (base stats, abilities, evolutions) into this project.

Tribunal Arma runs on an older pokeemerald-expansion, so files are not copied. Instead each species block is
compared field by field between Tribunal's untouched expansion data (TRIBUNAL_BASE) and its current data
(TRIBUNAL_HEAD); only fields Tribunal changed are written into our species_info files.

Usage: species_port.py TRIBUNAL_REPO [--apply]
"""
import collections, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TRIBUNAL_BASE = '73eb419e7~1'   # expansion data before Tribunal's own species edits
TRIBUNAL_HEAD = 'origin/HEAD'
FILES = ['src/data/pokemon/species_info/gen_%d_families.h' % g for g in range(1, 10)]
PORT_FIELDS = {'baseHP', 'baseAttack', 'baseDefense', 'baseSpeed', 'baseSpAttack', 'baseSpDefense',
               'abilities', 'evolutions', 'types'}

SPECIES_RE = re.compile(r'^\s*\[(SPECIES_\w+)\]\s*=\s*\n?\s*\{', re.M)
FIELD_RE = re.compile(r'\n([ \t]*)\.(\w+)\s*=')


def blocks(text):
    """species -> (body_start, body_end) of the {...} block."""
    out = {}
    for m in SPECIES_RE.finditer(text):
        i = m.end()
        depth = 1
        while depth:
            c = text[i]
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
            elif c == '/' and text[i + 1] == '/':
                i = text.index('\n', i) - 1
            i += 1
        out.setdefault(m.group(1), (m.end(), i - 1))
    return out


def fields(text, start, end):
    """[(name, value_start, value_end)] at depth 0 of the block; value excludes trailing #-lines/blank/comma."""
    body = text[start:end]
    found = []
    depth = 0
    i = 0
    positions = []
    while i < len(body):
        c = body[i]
        if c in '({':
            depth += 1
        elif c in ')}':
            depth -= 1
        elif c == '/' and body[i:i + 2] == '//':
            nl = body.find('\n', i)
            i = nl if nl >= 0 else len(body)
            continue
        elif c == '"':
            j = body.index('"', i + 1)
            while body[j - 1] == '\\':
                j = body.index('"', j + 1)
            i = j
        if c == '\n' and depth == 0:
            m = FIELD_RE.match(body, i)
            if m:
                positions.append((m.group(2), i + m.end() - i))
        i += 1
    for k, (name, vs) in enumerate(positions):
        ve = positions[k + 1][1] if k + 1 < len(positions) else len(body)
        # cut back to the start of the next field's line
        if k + 1 < len(positions):
            ve = body.rfind('\n', 0, body.rfind('.', 0, ve))
        val = body[vs:ve]
        # strip trailing preprocessor lines / blanks / comma
        lines = val.split('\n')
        while len(lines) > 1 and (not lines[-1].strip() or lines[-1].strip().startswith('#')):
            lines.pop()
        val = '\n'.join(lines).rstrip()
        if val.endswith(','):
            val = val[:-1]
        found.append((name, start + vs, start + vs + len(val)))
    return found


def norm(s):
    return re.sub(r'\s+', '', s)


def field_map(text):
    res = {}
    for sp, (s, e) in blocks(text).items():
        counts = collections.Counter()
        d = {}
        for name, vs, ve in fields(text, s, e):
            d[(name, counts[name])] = (vs, ve)
            counts[name] += 1
        res[sp] = d
    return res


def git_show(repo, rev, path):
    return subprocess.run(['git', '-C', repo, 'show', '%s:%s' % (rev, path)], capture_output=True, text=True, check=True).stdout


def main():
    repo = sys.argv[1]
    apply = '--apply' in sys.argv
    stats = collections.Counter()
    conflicts = []
    missing = []
    changes = {}  # species -> {(field, n): new_text}
    for path in FILES:
        base = git_show(repo, TRIBUNAL_BASE, path)
        head = git_show(repo, TRIBUNAL_HEAD, path)
        fb, fh = field_map(base), field_map(head)
        for sp, hf in fh.items():
            bf = fb.get(sp, {})
            for key, (vs, ve) in hf.items():
                if key[0] not in PORT_FIELDS:
                    continue
                new = head[vs:ve]
                old = base[bf[key][0]:bf[key][1]] if key in bf else None
                if old is not None and norm(old) == norm(new):
                    continue
                changes.setdefault(sp, {})[key] = (old, new)
                stats[key[0]] += 1
    # apply to our files
    for path in FILES:
        p = os.path.join(ROOT, path)
        ours = open(p).read()
        fo = field_map(ours)
        edits = []
        for sp, ch in changes.items():
            if sp not in fo:
                continue
            for key, (old, new) in ch.items():
                if key not in fo[sp]:
                    missing.append((sp, key))
                    continue
                vs, ve = fo[sp][key]
                cur = ours[vs:ve]
                if old is not None and norm(cur) != norm(old):
                    # our newer expansion changed this field itself (e.g. official Mega abilities): keep ours
                    if norm(cur) != norm(new):
                        conflicts.append((sp, key[0]))
                    continue
                edits.append((vs, ve, new))
        for vs, ve, new in sorted(edits, reverse=True):
            ours = ours[:vs] + new + ours[ve:]
        if apply:
            open(p, 'w').write(ours)
    found = {sp for path in FILES for sp in field_map(open(os.path.join(ROOT, path)).read())}
    not_here = sorted(sp for sp in changes if sp not in found)
    print('changed fields:', dict(stats))
    print('species changed:', len(changes))
    print('species not in our data:', len(not_here), not_here[:10])
    print('fields missing in our block:', len(missing), missing[:10])
    print('conflicts (our upstream differs from Tribunal base):', len(conflicts), conflicts[:20])


if __name__ == '__main__':
    main()
