"""Cross-check a match TSV (config/<version>/inputs) against other name sources.

usage: python validate_refs.py <candidate.tsv> <other.tsv | libgarden symbols.txt>...
A pair agrees when the last name component (method name without parameters
and template arguments) is equal in any of the listed alternatives.
"""
import re, sys
from collections import defaultdict


def base(s):
    s = s.replace(' ', '')
    if 'operator' not in s:
        s = s.split('(')[0]
    else:
        s = s[:s.find('(', s.find('operator') + 10)] if '(' in s[s.find('operator') + 10:] else s
    s = re.sub(r'<[^<>]*>', '', re.sub(r'<[^<>]*>', '', s))
    s = s.replace('__sub_object', '').replace('__deallocating', '')
    return s.split('::')[-1].strip().lower()


def same_name(x, y):
    """equal method names; C aliases like nngxlowInitialize == gxlow::Initialize"""
    bx, by = base(x), base(y)
    if bx == by:
        return True
    short, long_ = sorted((bx, by), key=len)
    return len(short) >= 5 and long_.endswith(short) and long_.startswith('nn')


def load_tsv(p):
    out = {}
    for line in open(p, encoding='utf-8'):
        if line.startswith('#'):
            continue
        c = line.rstrip('\n').split('\t')
        out[int(c[0], 16)] = ([c[1]] + [x for x in c[4].split(' | ') if x] if len(c) > 4 else [c[1]], c[2])
    return out


def load_any(p):
    if p.endswith('.tsv'):
        return load_tsv(p)
    from analyze import load_symbols
    return {a: ([e[1] for e in lst], 'libgarden') for a, lst in load_symbols([p]).items()}


def main(cand, *others):
    C = load_tsv(cand)
    for o in others:
        O = load_any(o)
        stat = defaultdict(lambda: [0, 0])
        diffs = []
        for a, (names, src) in C.items():
            if a not in O:
                continue
            ok = any(same_name(x, y) for x in names for y in O[a][0])
            stat[src.split(':')[-1]][0 if ok else 1] += 1
            if not ok:
                diffs.append((a, src, names[0][:60], O[a][0][0][:60]))
        tot = [sum(v[0] for v in stat.values()), sum(v[1] for v in stat.values())]
        print(f'vs {o.split(chr(92))[-1].split("/")[-1]}: {tot[0]} agree, {tot[1]} differ  ',
              {k: tuple(v) for k, v in stat.items()})
        for d in diffs[:12]:
            print(f'   0x{d[0]:06X} {d[1]:22s} {d[2]}  <->  {d[3]}')


if __name__ == '__main__':
    main(*sys.argv[1:])
