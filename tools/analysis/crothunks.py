"""Resolve CRO import thunks (`ldr pc, [pc, #-4]` + literal patched by the
loader) to their code.bin targets, exactly like ldr:ro does at load time.

usage: python crothunks.py <ACNL elf> <functions.txt of the ELF> <out dir> <cro files...>

Per module:
  <Module>.imports.txt        thunk offset -> code.bin address -> name
  <Module>.ghidra_symbols.txt thunks named after their target (ImportSymbolsScript.py,
                              addresses = CRO file offsets)
Also reports whether RTTI-named CRO methods point at thunks, which would give
names for the code.bin target.
"""
import os, re, sys
from collections import defaultdict
from elfmem import Mem
import crortti

LDR_PC_PC_M4 = 0xE51FF004       # ldr pc, [pc, #-4]


def thunks(c, m):
    out = {}
    for a, w in c.words(*c.text):
        if w == LDR_PC_PC_M4 and (a + 4) in c.patched:
            t = c.u32(a + 4)
            if m.in_text(t & ~1):
                out[a] = t
    return out


def ghidra_name(label):
    from analyze import split_qual
    base = re.sub(r'\s*\[.*?\]', '', label).strip()
    if '(' in base:
        cls, meth, _ = split_qual(base)
        base = f'{cls}::{meth}' if cls else meth
    return re.sub(r'[^\w:~<>,*&]', '_', base.replace(' ', ''))


def main(elf, functions, outdir, *cros, ghidra_base=0):
    m = Mem(elf)
    segs = [m.text[0], m.ro[0], m.data[0], m.data[0]]
    labels = crortti.load_labels(functions)
    os.makedirs(outdir, exist_ok=True)
    targets = set()
    total = 0
    via_rtti = defaultdict(set)     # ELF target -> {CRO method names}
    for path in cros:
        c = crortti.Cro(path, segs)
        th = thunks(c, m)
        total += len(th)
        targets |= set(th.values())
        base = os.path.splitext(c.name)[0]
        # names the CRO RTTI analysis gave to CRO functions (from crortti output)
        cro_fn = {}
        fpath = os.path.join(outdir, base + '.functions.txt')
        if os.path.exists(fpath):
            for line in open(fpath, encoding='utf-8'):
                if line.startswith('+0x'):
                    off, name = line.split('  ', 1)
                    cro_fn[crortti.CRO_BASE + int(off[1:], 16)] = name.strip()
        for a, t in th.items():
            if a in cro_fn and '::vf_0x' not in cro_fn[a]:
                via_rtti[t].add(cro_fn[a])
        with open(os.path.join(outdir, base + '.imports.txt'), 'w', encoding='utf-8') as fp:
            fp.write('# thunk (CRO file offset) -> code.bin address  name\n')
            for a in sorted(th):
                t = th[a]
                fp.write(f'+0x{a - crortti.CRO_BASE:06X} -> 0x{t:06X}  {labels.get(t, "?")}\n')
        with open(os.path.join(outdir, base + '.ghidra_symbols.txt'), 'w', encoding='utf-8') as fp:
            for a in sorted(th):
                t = th[a]
                nm = ghidra_name(labels[t]) if t in labels else f'FUN_{t:06X}'
                fp.write(f'thunk_{nm} 0x{ghidra_base + a - crortti.CRO_BASE:08X} f\n')
    named = sum(1 for t in targets if t in labels)
    print(f'thunks: {total}, distinct code.bin targets: {len(targets)}, '
          f'already named: {named}, unnamed: {len(targets) - named}')
    print(f'code.bin targets reached from RTTI-named CRO methods: {len(via_rtti)}')
    for t, ns in sorted(via_rtti.items())[:20]:
        print(f'  0x{t:06X} {labels.get(t, "?")}  <- {", ".join(sorted(ns))[:80]}')


if __name__ == '__main__':
    main(*sys.argv[1:4], *sys.argv[4:])
