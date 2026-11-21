#!/usr/bin/env python3
"""Port Pokémon Tribunal Arma's species changes (base stats, abilities, evolutions) into this project.

Tribunal Arma runs on an older pokeemerald-expansion, so files are not copied. Instead each species block is
compared field by field between Tribunal's untouched expansion data (TRIBUNAL_BASE) and its current data
(TRIBUNAL_HEAD); only fields Tribunal changed are written into our species_info files.

The changes are not written into the species files themselves: they go into
src/data/pokemon/species_info_tribunal_arma.h, which species_info.h includes after the vanilla data
(later designated initializers override earlier ones). Test builds skip it, so the expansion's battle
tests keep running on vanilla data.

Usage: species_port.py TRIBUNAL_REPO [--apply]
"""
import collections, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TRIBUNAL_BASE = '73eb419e7~1'   # expansion data before Tribunal's own species edits
TRIBUNAL_HEAD = 'origin/HEAD'
FILES = ['src/data/pokemon/species_info/gen_%d_families.h' % g for g in range(1, 10)]
OUT = 'src/data/pokemon/species_info_tribunal_arma.h'
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

        def open_ifs(ls):
            n = 0
            for l in ls:
                t = l.strip()
                if t.startswith('#if'):
                    n += 1
                elif t.startswith('#endif'):
                    n -= 1
            return n

        while len(lines) > 1 and (not lines[-1].strip() or lines[-1].strip().startswith('#')):
            t = lines[-1].strip()
            if t.startswith('#endif') and open_ifs(lines[:-1]) > 0:
                break  # closes an #if inside this value
            lines.pop()
        val = '\n'.join(lines).rstrip()
        if val.endswith(','):
            val = val[:-1]
        found.append((name, start + vs, start + vs + len(val)))
    return found


def condition_index(text):
    """[(offset, [conditions])]: active #if conditions at the start of every line."""
    stack = []  # per nesting level: (conditions of earlier branches, condition of the current branch or None)
    out = []
    pos = 0

    def render():
        conds = []
        for prior, cur in stack:
            parts = ['!(%s)' % p for p in prior]
            if cur is not None:
                parts.append('(%s)' % cur)
            conds.append(' && '.join(parts))
        return conds

    for line in text.split('\n'):
        out.append((pos, render()))
        t = line.strip()
        if t.startswith('#ifdef '):
            stack.append(([], 'defined(%s)' % t[7:].split('//')[0].strip()))
        elif t.startswith('#ifndef '):
            stack.append(([], '!defined(%s)' % t[8:].split('//')[0].strip()))
        elif t.startswith('#if '):
            stack.append(([], t[4:].split('//')[0].strip()))
        elif t.startswith('#elif '):
            prior, cur = stack.pop()
            stack.append((prior + [cur], t[6:].split('//')[0].strip()))
        elif t.startswith('#else'):
            prior, cur = stack.pop()
            stack.append((prior + [cur], None))
        elif t.startswith('#endif'):
            stack.pop()
        pos += len(line) + 1
    return out


def conditions_at(index, offset):
    import bisect
    i = bisect.bisect_right([o for o, _ in index], offset) - 1
    return index[i][1]


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
    # write the override header from our (vanilla) species files
    out = ['// Generated by tools/tribunal_port/species_port.py - do not edit by hand.',
           '// Pokémon Tribunal Arma species changes (base stats, abilities, types, single-player evolutions).',
           '// Included at the end of gSpeciesInfo; later designated initializers override the vanilla values.',
           '// Test builds skip it: the battle tests are written for vanilla data.', '', '#if !TESTING']
    current_cond = ''
    for path in FILES:
        ours = open(os.path.join(ROOT, path)).read()
        fo = field_map(ours)
        index = condition_index(ours)
        for sp, ch in changes.items():
            if sp not in fo:
                continue
            for key, (old, new) in sorted(ch.items()):
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
                conds = conditions_at(index, vs)
                cond = ' && '.join('(%s)' % c for c in conds)
                if cond != current_cond:
                    if current_cond:
                        out.append('#endif')
                    out.append('')
                    if cond:
                        out.append('#if ' + cond)
                    current_cond = cond
                out.append('    [%s].%s =%s,' % (sp, key[0], new.rstrip()))
        if current_cond:
            out.append('#endif')
        current_cond = ''
    out.append('')
    out.append('#endif // !TESTING')
    if apply:
        with open(os.path.join(ROOT, OUT), 'w') as f:
            f.write('\n'.join(out) + '\n')
    found = {sp for path in FILES for sp in field_map(open(os.path.join(ROOT, path)).read())}
    not_here = sorted(sp for sp in changes if sp not in found)
    print('changed fields:', dict(stats))
    print('species changed:', len(changes))
    print('species not in our data:', len(not_here), not_here[:10])
    print('fields missing in our block:', len(missing), missing[:10])
    print('conflicts (our upstream differs from Tribunal base):', len(conflicts), conflicts[:20])


if __name__ == '__main__':
    main()
