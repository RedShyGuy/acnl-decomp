"""RTTI analysis of ACNL CRO modules.

usage: python crortti.py <ACNL elf> <functions.txt of the ELF> <out dir> <cro files...>

CRO internal relocations are applied with the CRO placed at CRO_BASE (so local
addresses never collide with ELF addresses), imports from |static| are resolved
to real ELF addresses. Afterwards typeinfos / vtables are parsed exactly like
for the ELF; inherited functions from the ELF keep their ELF names and slot
names of ELF base classes are propagated onto CRO overrides.

Addresses of CRO functions are printed as file offsets: runtime address =
CRO load address + offset.
"""
import os, re, struct, sys
from collections import defaultdict
from elfmem import Mem, demangle
import rtti

CRO_BASE = 0x40000000
ABS32, REL32 = 2, 3


class Cro:
    def __init__(self, path, static_segs):
        d = bytearray(open(path, 'rb').read())
        self.name = os.path.basename(path)
        u = lambda o: struct.unpack_from('<I', d, o)[0]
        segs = []
        for i in range(u(0xCC)):
            off, size, sid = struct.unpack_from('<3I', d, u(0xC8) + i * 12)
            segs.append([off, size, sid])
        bss_off = (len(d) + 0xFFF) & ~0xFFF
        for s in segs:
            if s[2] == 3 and s[1]:
                s[0] = bss_off
        d += bytes(bss_off + sum(s[1] for s in segs if s[2] == 3) - len(d) + 4)
        self.segs = segs
        self.mem = d

        def seg_addr(enc, table):
            idx, off = enc & 0xF, enc >> 4
            return table[idx] + off if idx < len(table) else None

        local = [CRO_BASE + s[0] for s in segs]

        self.patched = set()

        def patch(target, typ, value):
            if target is None:
                return
            self.patched.add(target)
            o = target - CRO_BASE
            if typ == ABS32:
                struct.pack_into('<I', d, o, value & 0xFFFFFFFF)
            elif typ == REL32:
                struct.pack_into('<I', d, o, (value - target) & 0xFFFFFFFF)

        # internal relocations
        ro, rn = u(0x128), u(0x12C)
        for i in range(rn):
            t, typ, src, _, _, add = struct.unpack_from('<IBBBBI', d, ro + i * 12)
            if src < len(local):
                patch(seg_addr(t, local), typ, local[src] + add)
        # imports from |static| (anonymous = encoded segment offset)
        mt, mn = u(0xF0), u(0xF4)
        for i in range(mn):
            no, io, ic, ao, ac = struct.unpack_from('<5I', d, mt + i * 20)
            if d[no:d.find(b'\0', no)] != b'|static|':
                continue
            for k in range(ac):
                so, po = struct.unpack_from('<2I', d, ao + k * 8)
                val = seg_addr(so, static_segs)
                while val is not None:
                    t, typ, last, _, _, add = struct.unpack_from('<IBBBBI', d, po)
                    patch(seg_addr(t, local), typ, val + add)
                    if last:
                        break
                    po += 12
        text = [s for s in segs if s[2] == 0 and s[1]]
        ro_ = [s for s in segs if s[2] == 1 and s[1]]
        rw = [s for s in segs if s[2] in (2, 3) and s[1]]
        self.text = (CRO_BASE + text[0][0], CRO_BASE + text[0][0] + text[0][1])
        self.ro = (CRO_BASE + ro_[0][0], CRO_BASE + ro_[0][0] + ro_[0][1]) if ro_ else self.text
        self.data = (CRO_BASE + rw[0][0], CRO_BASE + rw[0][0] + rw[0][1]) if rw else self.ro
        self.data_init_end = self.data[1]
        self.base = CRO_BASE
        self.end = CRO_BASE + len(d)

    # Mem-compatible accessors
    def u32(self, a):
        return struct.unpack_from('<I', self.mem, a - CRO_BASE)[0]

    def s32(self, a):
        return struct.unpack_from('<i', self.mem, a - CRO_BASE)[0]

    def valid(self, a):
        return CRO_BASE <= a < self.end - 3

    def in_text(self, a):
        return self.text[0] <= a < self.text[1]

    def in_data(self, a):
        return self.data[0] <= a < self.data[1]

    def cstr(self, a, maxlen=512):
        if not self.valid(a):
            return None
        o = a - CRO_BASE
        e = self.mem.find(b'\0', o, o + maxlen)
        s = self.mem[o:e] if e >= 0 else b''
        if not s or any(c < 0x20 or c > 0x7e for c in s):
            return None
        return s.decode()

    def words(self, lo, hi):
        lo = (lo + 3) & ~3
        n = (hi - lo) // 4
        vals = struct.unpack_from('<%dI' % n, self.mem, lo - CRO_BASE)
        for i, v in enumerate(vals):
            yield lo + i * 4, v


def load_labels(path):
    lab = {}
    for line in open(path, encoding='utf-8', errors='replace'):
        if line.startswith('0x'):
            a, rest = line.split('  ', 1)
            lab[int(a, 16)] = rest.split('   --')[0].strip()
    return lab


def method_of(label):
    from analyze import split_qual
    if not label or '::vf_0x' in label or label.startswith('sub_') or '[ctor' in label:
        return None
    c, m, p = split_qual(label)
    return m + p


def main(elf, functions, outdir, *cros):
    m = Mem(elf)
    w = rtti.build_word_index(m)
    vptrs = rtti.find_cxxabi_vptrs(m, w)
    etis = rtti.parse_typeinfos(m, w, vptrs)
    rtti.parse_vtables(m, w, etis)
    labels = load_labels(functions)
    # names that an earlier run of this script contributed must be reproduced,
    # otherwise elf_from_cro.tsv would lose them once they are in functions.txt
    from_cro = {int(l.split('  ', 1)[0], 16) for l in open(functions, encoding='utf-8', errors='replace')
                if l.startswith('0x') and '-- cro-vtable' in l}
    elf_names = {t.mangled for t in etis.values()}
    static_segs = [m.text[0], m.ro[0], m.data[0], m.data[0]]
    os.makedirs(outdir, exist_ok=True)
    summary = []
    all_new = {}
    elf_new = {}

    for path in cros:
        c = Cro(path, static_segs)
        # typeinfos: vptr words equal the ELF cxxabi vptrs
        kinds = {v: k for k, v in vptrs.items()}
        widx = defaultdict(list)
        for lo, hi in (c.ro, c.data, c.text):
            for a, v in c.words(lo, hi):
                widx[v].append(a)
        tis = {}
        for vp, kind in kinds.items():
            for a in widx.get(vp, []):
                nm = c.cstr(c.u32(a + 4))
                if not nm:
                    continue
                t = rtti.TypeInfo(a, kind, nm)
                if kind == 'si':
                    t.bases.append((c.u32(a + 8), 0, False, True))
                    t.size = 12
                elif kind == 'vmi':
                    cnt = c.u32(a + 12)
                    if cnt > 64:
                        continue
                    for i in range(cnt):
                        of = c.s32(a + 20 + i * 8)
                        t.bases.append((c.u32(a + 16 + i * 8), of >> 8, bool(of & 1), bool(of & 2)))
                    t.size = 16 + cnt * 8
                tis[a] = t
        order = list(tis.values())
        for t, dd in zip(order, demangle(['_ZTS' + t.mangled for t in order])):
            dd = dd.strip()
            t.name = dd[18:] if dd.startswith('typeinfo name for ') else t.mangled

        def tname(a):
            return tis[a].name if a in tis else (etis[a].name + ' (ELF)' if a in etis else hex(a))

        # vtables (entries may be CRO-local or ELF text addresses)
        inside = {t.addr + i for t in tis.values() for i in range(0, t.size, 4)}
        def is_code(v):
            return c.in_text(v) or m.in_text(v)
        for ta, t in tis.items():
            for a in widx.get(ta, []):
                if a in inside or not (-0x10000 < c.s32(a - 4) <= 0):
                    continue
                funcs, p = [], a + 4
                while c.valid(p) and p in c.patched and is_code(c.u32(p)):
                    funcs.append(c.u32(p)); p += 4
                if funcs:
                    t.vtables.append(rtti.VTable(a - 4, t, c.s32(a - 4), funcs))
            t.vtables.sort(key=lambda v: -v.offset_to_top)

        # slot names from the (ELF or CRO) primary base chain
        def primary_chain(t):
            out, cur = [], t
            while True:
                b = cur.primary_base
                if b in tis:
                    cur = tis[b]; out.append(('cro', cur))
                elif b in etis:
                    cur = etis[b]; out.append(('elf', cur))
                    while cur.primary_base in etis:
                        cur = etis[cur.primary_base]; out.append(('elf', cur))
                    return out
                else:
                    return out

        def slot_name(t, i):
            for kind, b in primary_chain(t):
                pv = rtti.primary_vtable(b)
                if pv and i < len(pv.funcs):
                    f = pv.funcs[i]
                    mt = method_of(labels.get(f)) if kind == 'elf' else None
                    if mt:
                        return mt
            return None

        fn_labels = {}
        lines = []
        jclasses = []
        for t in sorted(tis.values(), key=lambda t: t.name):
            new = t.mangled not in elf_names
            if new:
                all_new[t.name] = all_new.get(t.name, []) + [c.name]
            bases = ', '.join(tname(b) + (f' @+0x{o:X}' if o else '') for b, o, v, p in t.bases)
            lines.append(f'class {t.name}' + (f' : {bases}' if bases else '') + ('' if new else '   [also in ELF]'))
            lines.append(f'  typeinfo +0x{t.addr - CRO_BASE:X}')
            for vt in t.vtables:
                lines.append(f'  vtable   +0x{vt.addr - CRO_BASE:X} (offset_to_top {vt.offset_to_top}, {len(vt.funcs)} entries)')
                for i, f in enumerate(vt.funcs):
                    if m.in_text(f):
                        if (f not in labels or f in from_cro) and m.u32(f & ~1) != 0xE12FFF1E:   # skip shared 'bx lr' stubs
                            elf_new.setdefault(f, (f'{t.name}::vf_0x{i * 4:02X}', c.name))
                        desc = f'ELF 0x{f:06X}  {labels.get(f, elf_new[f][0] if f in elf_new else "?")}'
                    else:
                        mt = slot_name(t, i) if vt.offset_to_top == 0 else None
                        if mt and mt.startswith('~'):
                            from analyze import short
                            rest = mt[mt.index('('):] if '(' in mt else '()'
                            mt = '~' + short(t.name) + rest
                        nm = f'{t.name}::{mt}' if mt else f'{t.name}::vf_0x{i * 4:02X}'
                        fn_labels.setdefault(f, nm)
                        desc = f'CRO +0x{f - CRO_BASE:X}  {fn_labels[f]}'
                    lines.append(f'    [0x{i * 4:02X}] {desc}')
            lines.append('')
            jclasses.append({
                'name': t.name, 'mangled': t.mangled, 'typeinfo_offset': t.addr - CRO_BASE,
                'also_in_elf': not new,
                'bases': [{'name': tis[b].name if b in tis else (etis[b].name if b in etis else None),
                           'in': 'cro' if b in tis else ('elf' if b in etis else None),
                           'offset': o, 'virtual': v} for b, o, v, p in t.bases],
                'vtables': [{
                    'offset': vt.addr - CRO_BASE, 'offset_to_top': vt.offset_to_top,
                    'entries': [({'slot': i, 'in': 'elf', 'addr': f, 'label': labels.get(f)}
                                 if m.in_text(f) else
                                 {'slot': i, 'in': 'cro', 'offset': f - CRO_BASE, 'label': fn_labels.get(f)})
                                for i, f in enumerate(vt.funcs)]}
                    for vt in t.vtables],
            })
        import json
        from crothunks import thunks as find_thunks
        th = find_thunks(c, m)
        with open(os.path.join(outdir, os.path.splitext(c.name)[0] + '.json'), 'w', encoding='utf-8') as fp:
            json.dump({'module': os.path.splitext(c.name)[0],
                       'segments': [{'offset': s[0], 'size': s[1], 'id': s[2]} for s in c.segs],
                       'classes': jclasses,
                       'functions': [{'offset': f - CRO_BASE, 'name': n} for f, n in sorted(fn_labels.items())],
                       'imports': [{'thunk_offset': a - CRO_BASE, 'target': tgt, 'label': labels.get(tgt)}
                                   for a, tgt in sorted(th.items())]}, fp)
        base = os.path.splitext(c.name)[0]
        open(os.path.join(outdir, base + '.classes.txt'), 'w', encoding='utf-8').write('\n'.join(lines))
        with open(os.path.join(outdir, base + '.functions.txt'), 'w', encoding='utf-8') as fp:
            fp.write('# CRO file offset (runtime = load address + offset)  name\n')
            for f in sorted(fn_labels):
                fp.write(f'+0x{f - CRO_BASE:06X}  {fn_labels[f]}\n')
        named = sum(1 for v in fn_labels.values() if '::vf_0x' not in v)
        summary.append((c.name, len(tis), sum(1 for t in tis.values() if t.mangled not in elf_names),
                        sum(len(t.vtables) for t in tis.values()), len(fn_labels), named))

    with open(os.path.join(outdir, 'cro_only_classes.txt'), 'w', encoding='utf-8') as fp:
        fp.write('# classes whose RTTI exists only in CRO modules (not in code.bin)\n')
        for n in sorted(all_new):
            fp.write(f'{n:70s} {", ".join(sorted(set(all_new[n])))}\n')
    print(f'{"module":28s} types  new  vtables  funcs  named')
    for s in summary:
        print(f'{s[0]:28s} {s[1]:5d} {s[2]:4d} {s[3]:8d} {s[4]:6d} {s[5]:6d}')
    print('classes only in CROs:', len(all_new))
    with open(os.path.join(outdir, 'elf_from_cro.tsv'), 'w', encoding='utf-8') as fp:
        fp.write('# ELF functions only referenced by CRO vtables (TSV for analyze.py --ref)\n')
        for f, (n, mod) in sorted(elf_new.items()):
            fp.write(f'0x{f:06X}\t{n}\tcro-vtable {mod}\t0x0\t\n')
    print('ELF functions found only via CRO vtables:', len(elf_new),
          ', '.join(f'0x{f:X} {n}' for f, (n, _) in sorted(elf_new.items())))


if __name__ == '__main__':
    main(*sys.argv[1:4], *sys.argv[4:])
