"""Convert ACNL CRO modules into ELF files that Ghidra imports cleanly.

usage: python cro2elf.py [--elf orig/0004000000086300/code.elf] [--version 0004000000086300] <out dir> <cro files...>

Per module <out dir>/<Module>.elf:
  - the image is relocated like ldr:ro does it (internal relocations applied,
    imports from |static| patched with real code.bin addresses), see crortti.Cro
  - address = CRO_BASE (0x40000000) + CRO file offset, so ghidra/symbols/cro
    offsets and the "+0x..." comments map 1:1
  - sections .cro_header (r), .text (rx), .rodata (r), .cro_tables (r),
    .data (rw), .bss (rw, nobits)
  - symbols: ghidra/symbols/cro/<Module>.txt, named exports, the control
    object / OnLoad / OnExit / OnUnresolved entries and import thunks named
    thunk_<code.bin name>

Import in Ghidra: drag & drop the .elf, language ARM:LE:32:v6 is detected.
Pointers into code.bin stay real code.bin addresses (outside the module).
"""
import argparse, glob, os, re, struct, sys
from elfmem import Mem
import crortti
import crothunks
from crortti import CRO_BASE

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..', '..'))

SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_NOBITS = 1, 2, 3, 8
SHF_WRITE, SHF_ALLOC, SHF_EXEC = 1, 2, 4
PF_X, PF_W, PF_R = 1, 2, 4
STT_NOTYPE, STT_OBJECT, STT_FUNC = 0, 1, 2
STB_GLOBAL = 1
IMAGE_OFF = 0x1000      # file offset of the CRO image inside the ELF


def load_symbol_file(path):
    out = []
    if not os.path.exists(path):
        return out
    for line in open(path, encoding='utf-8'):
        p = line.split()
        if len(p) >= 3 and p[-1] in ('f', 'l'):
            out.append((' '.join(p[:-2]), int(p[-2], 16), p[-1]))
    return out


def code_names(version):
    """code.bin address -> name (Ghidra symbol files first, analysis output as fallback)."""
    names = {}
    fn = os.path.join(ROOT, 'build', 'analysis', 'functions.txt')
    if os.path.exists(fn):
        for a, lab in crortti.load_labels(fn).items():
            names[a & ~1] = crothunks.ghidra_name(lab)
    for name, a, _ in load_symbol_file(os.path.join(ROOT, 'ghidra', 'symbols', f'code_{version}.txt')):
        names[a & ~1] = name
    return names


class Strtab:
    def __init__(self):
        self.data = bytearray(b'\0')
        self.idx = {}

    def add(self, s):
        if s not in self.idx:
            self.idx[s] = len(self.data)
            self.data += s.encode('utf-8') + b'\0'
        return self.idx[s]


def convert(path, m, names, out_path):
    static_segs = [m.text[0], m.ro[0], m.data[0], m.data[0]]
    c = crortti.Cro(path, static_segs)
    d = c.mem
    raw = open(path, 'rb').read()
    u = lambda o: struct.unpack_from('<I', raw, o)[0]
    module = os.path.splitext(c.name)[0]
    seg_off = [s[0] for s in c.segs]

    def seg_addr(enc):
        if enc == 0xFFFFFFFF:
            return None
        idx, off = enc & 0xF, enc >> 4
        return CRO_BASE + seg_off[idx] + off if idx < len(seg_off) else None

    # sections: (name, file offset, size, flags, type)
    by_id = {}
    for off, size, sid in c.segs:
        if size and sid not in by_id:
            by_id[sid] = (off, size)
    secs = []
    if 0 in by_id and by_id[0][0] > 0:
        secs.append(('.cro_header', 0, by_id[0][0], SHF_ALLOC, SHT_PROGBITS))
    if 0 in by_id:
        secs.append(('.text', *by_id[0], SHF_ALLOC | SHF_EXEC, SHT_PROGBITS))
    if 1 in by_id:
        secs.append(('.rodata', *by_id[1], SHF_ALLOC, SHT_PROGBITS))
    tables = u(0xC0)      # module name: first of the CRO tables
    tables_end = by_id[2][0] if 2 in by_id and by_id[2][0] > tables else len(raw)
    taken = [(o, o + s) for _, o, s, _, _ in secs]
    if tables_end > tables and not any(lo < tables_end and tables < hi for lo, hi in taken):
        secs.append(('.cro_tables', tables, tables_end - tables, SHF_ALLOC, SHT_PROGBITS))
    if 2 in by_id:
        secs.append(('.data', *by_id[2], SHF_ALLOC | SHF_WRITE, SHT_PROGBITS))
    if 3 in by_id:
        secs.append(('.bss', *by_id[3], SHF_ALLOC | SHF_WRITE, SHT_NOBITS))
    secs.sort(key=lambda s: s[1])

    # symbols: (name, address, type)
    syms = []
    named = set()
    for name, off, kind in load_symbol_file(os.path.join(ROOT, 'ghidra', 'symbols', 'cro', module + '.txt')):
        syms.append((name, CRO_BASE + off, STT_FUNC if kind == 'f' else STT_OBJECT))
        named.add((CRO_BASE + off) & ~1)
    for i in range(u(0xD4)):
        no, enc = struct.unpack_from('<2I', raw, u(0xD0) + i * 8)
        a = seg_addr(enc)
        if a is not None:
            nm = raw[no:raw.find(b'\0', no)].decode('ascii', 'replace')
            syms.append((nm, a, STT_FUNC if c.in_text(a & ~1) else STT_OBJECT))
    for i in range(u(0xDC)):
        a = seg_addr(u(u(0xD8) + i * 4))
        if a is not None and a & ~1 not in named:
            syms.append((f'{module}_export_{i}', a, STT_FUNC if c.in_text(a & ~1) else STT_OBJECT))
    for hdr, nm in ((0xA0, 'nnroControlObject'), (0xA4, 'nnroOnLoad'),
                    (0xA8, 'nnroOnExit'), (0xAC, 'nnroOnUnresolved')):
        a = seg_addr(u(hdr))
        if a is not None:
            syms.append((f'{module}_{nm}', a, STT_FUNC if c.in_text(a & ~1) else STT_OBJECT))
    n_thunks = 0
    for a, t in sorted(crothunks.thunks(c, m).items()):
        if a in named:
            continue
        syms.append(('thunk_' + names.get(t & ~1, f'FUN_{t & ~1:08X}'), a, STT_FUNC))
        n_thunks += 1

    # ELF layout: header, phdrs, image, shstrtab, strtab, symtab, shdrs
    shstr, strtab = Strtab(), Strtab()
    loads = [s for s in secs]
    ehsize, phsize, shsize = 52, 32, 40
    image_end = IMAGE_OFF + len(raw)
    out = bytearray(IMAGE_OFF)
    out += raw
    for name, off, size, flags, typ in secs:
        if typ == SHT_PROGBITS:
            out[IMAGE_OFF + off:IMAGE_OFF + off + size] = d[off:off + size]

    symtab = bytearray(16)
    for name, a, typ in sorted(syms, key=lambda s: s[1]):
        shndx = next((i + 1 for i, s in enumerate(secs) if s[1] <= (a & ~1) - CRO_BASE < s[1] + max(s[2], 1)), 0xFFF1)
        symtab += struct.pack('<IIIBBH', strtab.add(name), a, 0, (STB_GLOBAL << 4) | typ, 0, shndx)

    shdrs = [bytes(shsize)]
    for name, off, size, flags, typ in secs:
        shdrs.append(struct.pack('<10I', shstr.add(name), typ, flags, CRO_BASE + off,
                                 IMAGE_OFF + off if typ != SHT_NOBITS else image_end,
                                 size, 0, 0, 4, 0))
    i_symtab = len(shdrs)
    i_strtab = i_symtab + 1
    i_shstr = i_symtab + 2
    sym_name, str_name, shs_name = shstr.add('.symtab'), shstr.add('.strtab'), shstr.add('.shstrtab')

    def append(blob, align=4):
        while len(out) % align:
            out.append(0)
        o = len(out)
        out.extend(blob)
        return o

    o_sym = append(symtab)
    o_str = append(strtab.data, 1)
    shdrs.append(struct.pack('<10I', sym_name, SHT_SYMTAB, 0, 0, o_sym, len(symtab), i_strtab, 1, 4, 16))
    shdrs.append(struct.pack('<10I', str_name, SHT_STRTAB, 0, 0, o_str, len(strtab.data), 0, 0, 1, 0))
    o_shs = append(shstr.data, 1)
    shdrs.append(struct.pack('<10I', shs_name, SHT_STRTAB, 0, 0, o_shs, len(shstr.data), 0, 0, 1, 0))
    o_sh = append(b''.join(shdrs))

    phdrs = bytearray()
    for name, off, size, flags, typ in loads:
        pf = PF_R | (PF_X if flags & SHF_EXEC else 0) | (PF_W if flags & SHF_WRITE else 0)
        filesz = 0 if typ == SHT_NOBITS else size
        fo = image_end if typ == SHT_NOBITS else IMAGE_OFF + off
        phdrs += struct.pack('<8I', 1, fo, CRO_BASE + off, CRO_BASE + off, filesz, size, pf, 4)
    assert 52 + len(phdrs) <= IMAGE_OFF
    out[52:52 + len(phdrs)] = phdrs

    entry = seg_addr(u(0xA4)) or 0
    ident = b'\x7fELF' + bytes([1, 1, 1, 0]) + bytes(8)
    out[0:52] = ident + struct.pack('<HHIIIIIHHHHHH', 2, 40, 1, entry, 52, o_sh, 0x05000000,
                                    ehsize, phsize, len(loads), shsize, len(shdrs), i_shstr)
    with open(out_path, 'wb') as fp:
        fp.write(out)
    return len(syms), n_thunks


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--version', default='0004000000086300')
    ap.add_argument('--elf', help='code.elf (default orig/<version>/code.elf)')
    ap.add_argument('outdir')
    ap.add_argument('cros', nargs='+')
    a = ap.parse_args()
    m = Mem(a.elf or os.path.join(ROOT, 'orig', a.version, 'code.elf'))
    names = code_names(a.version)
    os.makedirs(a.outdir, exist_ok=True)
    cros = [p for pat in a.cros for p in (glob.glob(pat) or [pat])]
    for path in cros:
        base = os.path.splitext(os.path.basename(path))[0]
        n, t = convert(path, m, names, os.path.join(a.outdir, base + '.elf'))
        print(f'{base}: {n} symbols ({t} thunks)')


if __name__ == '__main__':
    main()
