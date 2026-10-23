#!/usr/bin/env python3
"""Disassemble a range of the original code.elf, with names.

    python tools/analysis/disasm.py <start> <end> [--version 0004000000086300]
    python tools/analysis/disasm.py 0x47E788 0x47E8DC

Function starts from config/<version>/symbols.json get a "--- <address> <name>" header, calls
are named, and every pc-relative literal is shown with its value and what it points to
(function name, vtable, or string). Uses devkitARM's objdump.
"""
import argparse, json, os, re, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..', '..'))
sys.path.insert(0, os.path.dirname(__file__))
import elfmem


def objdump():
    for base in (os.environ.get('DEVKITARM', ''), r'C:\devkitPro\devkitARM', '/opt/devkitpro/devkitARM'):
        exe = os.path.join(base, 'bin', 'arm-none-eabi-objdump' + ('.exe' if os.name == 'nt' else ''))
        if base and os.path.exists(exe):
            return exe
    return 'arm-none-eabi-objdump'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('start')
    ap.add_argument('end')
    ap.add_argument('--version', default='0004000000086300')
    a = ap.parse_args()
    elf = os.path.join(ROOT, 'orig', a.version, 'code.elf')
    db = json.load(open(os.path.join(ROOT, 'config', a.version, 'symbols.json'), encoding='utf-8'))
    names = {}
    for f in sorted(db['functions'], key=lambda f: (f.get('tier') or 'Z')):
        names.setdefault(f['addr'] & ~1, f['name'])
    vptr = {vt['vptr']: c['name'] for c in db['classes'] for vt in c['vtables']}
    m = elfmem.Mem(elf)
    lo, hi = int(a.start, 16), int(a.end, 16)
    out = subprocess.run([objdump(), '-d', '-marm', f'--start-address={lo}', f'--stop-address={hi}', elf],
                         capture_output=True, text=True).stdout
    # jump tables: `cmp rX, #N; ldrcc pc, [pc, rX, lsl #2]; b default` and N words of targets
    last_cmp, table = None, None
    for line in out.splitlines():
        mm = re.match(r'\s+([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not mm:
            continue
        addr, txt = int(mm.group(1), 16), mm.group(3)
        if addr in names:
            print(f'\n--- {addr:#x} {names[addr]}')
        if table and table[0] <= addr < table[1]:
            print(f'{addr:06x}: {".word":40s} case {(addr - table[0]) // 4}: {m.u32(addr):#x}')
            continue
        t = re.match(r'cmp\s+r\d+, #(\d+)', txt)
        if t:
            last_cmp = int(t.group(1))
        if re.match(r'ldr[a-z]*\s+pc, \[pc, r\d+, lsl #2\]', txt) and last_cmp:
            table = (addr + 8, addr + 8 + 4 * last_cmp)
        note = ''
        t = re.search(r'\b(?:bl|b|blx)[a-z]*\s+0x([0-9a-f]+)', txt)
        if t and int(t.group(1), 16) in names:
            note = names[int(t.group(1), 16)]
        t = re.search(r'\[pc, #(-?\d+)\]', txt)
        if t:
            v = m.u32(addr + 8 + int(t.group(1)))
            note = f'={v:#x} ' + (names.get(v & ~1) or (vptr[v] + ' vptr' if v in vptr else '')
                                   or (repr(m.cstr(v)) if m.cstr(v) else ''))
        txt = re.sub(r'\s*@.*', '', txt)
        print(f'{addr:06x}: {txt:40s} {note}')


if __name__ == '__main__':
    main()
