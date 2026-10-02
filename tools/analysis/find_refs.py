#!/usr/bin/env python3
"""Find references to addresses in the original code.elf: bl / b / blx targets and literal words.

    python tools/analysis/find_refs.py <address> [<address> ...] [--version USA_1_5]
    python tools/analysis/find_refs.py 0x469EA0 0x97F090

Useful for callers of an unnamed function, users of a global, or who registers a destructor.
Scans the whole code range, so it takes a few seconds.
"""
import argparse, os, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, os.path.dirname(__file__))
import elfmem


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('addresses', nargs='+')
    ap.add_argument('--version', default='USA_1_5')
    a = ap.parse_args()
    m = elfmem.Mem(os.path.join(ROOT, 'orig', a.version, 'code.elf'))
    targets = {int(x, 16) for x in a.addresses}
    for addr in range(0x100000, 0x8A0000, 4):
        try:
            w = m.u32(addr)
        except Exception:
            continue
        if w in targets or (w & ~1) in targets:
            print(f'{addr:#x}: literal {w:#x}')
        if (w >> 25) & 7 == 0b101:
            off = w & 0xFFFFFF
            if off & 0x800000:
                off -= 1 << 24
            t = addr + 8 + off * 4
            if (w >> 28) == 0xF:
                t |= (w >> 23) & 2
            if t in targets:
                print(f'{addr:#x}: branch {w:#010x} -> {t:#x}')


if __name__ == '__main__':
    main()
