#!/usr/bin/env python3
"""Create a Ghidra project for ACNL with all names applied (headless).

    python ghidra/create_project.py --ghidra <Ghidra install dir> [--version 0004000000086300] [--out build/ghidra]

Steps (analyzeHeadless):
  1. import orig/<version>/code.elf (ARM:LE:32:v6, image base 0x00100000) and auto-analyse it
  2. ghidra/scripts/ACNLSvcNames.py     - svc numbers -> names (equates, comments, nn::svc wrappers)
  3. ghidra/scripts/ACNLSyncSymbols.py  - names from ghidra/symbols/code_<version>.txt (mode 3: replace old labels)

Afterwards open the project in Ghidra and load the save structures with
File > Parse C Source... > ghidra/types/acnl_save.h (see ghidra/README.md).
"""
import argparse, os, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..'))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--ghidra', required=True, help='Ghidra installation directory')
    ap.add_argument('--version', default='0004000000086300')
    ap.add_argument('--out', default=os.path.join(ROOT, 'build', 'ghidra'))
    a = ap.parse_args()

    exe = os.path.join(a.ghidra, 'support', 'analyzeHeadless' + ('.bat' if os.name == 'nt' else ''))
    elf = os.path.join(ROOT, 'orig', a.version, 'code.elf')
    if not os.path.exists(exe):
        raise SystemExit(f'not found: {exe}')
    if not os.path.exists(elf):
        raise SystemExit(f'not found: {elf} (see README.md, "Setup")')
    os.makedirs(a.out, exist_ok=True)
    symbols = [os.path.join(ROOT, 'ghidra', 'symbols', f'code_{a.version}.txt')]
    symbols = [s for s in symbols if os.path.exists(s)]
    cmd = [exe, a.out, f'ACNL_{a.version}',
           '-import', elf,
           '-processor', 'ARM:LE:32:v6',
           '-scriptPath', os.path.join(ROOT, 'ghidra', 'scripts'),
           '-postScript', 'ACNLSvcNames.py', 'rename',
           '-postScript', 'ACNLSyncSymbols.py', '3', os.path.join(a.out, 'sync_report.csv'), *symbols]
    print(' '.join(f'"{c}"' if ' ' in c else c for c in cmd))
    sys.exit(subprocess.call(cmd))


if __name__ == '__main__':
    main()
