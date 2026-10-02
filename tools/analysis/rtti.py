"""Parse Itanium C++ RTTI (typeinfo + vtables) out of the ELF image."""
from collections import defaultdict
from elfmem import demangle

CXXABI = {
    'class': 'N10__cxxabiv117__class_type_infoE',
    'si':    'N10__cxxabiv120__si_class_type_infoE',
    'vmi':   'N10__cxxabiv121__vmi_class_type_infoE',
}


class TypeInfo:
    def __init__(self, addr, kind, mangled):
        self.addr, self.kind, self.mangled = addr, kind, mangled
        self.name = mangled
        self.bases = []      # (typeinfo addr, offset, is_virtual, is_public)
        self.size = 8
        self.vtables = []    # list of VTable
        self.children = []

    @property
    def primary_base(self):
        for b, off, virt, pub in self.bases:
            if off == 0 and not virt:
                return b
        return None


class VTable:
    def __init__(self, addr, ti, offset_to_top, funcs):
        self.addr = addr                 # start (offset_to_top word) == _ZTV symbol
        self.addrpoint = addr + 8        # value stored in object vptr
        self.ti = ti
        self.offset_to_top = offset_to_top
        self.funcs = funcs


def build_word_index(m):
    """value -> [addr] for every aligned word in rodata + data."""
    idx = defaultdict(list)
    for lo, hi in (m.ro, (m.data[0], m.data_init_end)):
        for a, v in m.words(lo, hi):
            idx[v].append(a)
    return idx


def find_cxxabi_vptrs(m, widx):
    """Locate the vtable address points of the three cxxabiv1 typeinfo classes."""
    ro = bytes(m.mem[m.ro[0] - m.base:m.ro[1] - m.base])
    vptrs = {}
    for kind, s in CXXABI.items():
        sa = ro.find(s.encode() + b'\0')
        assert sa >= 0, s
        sa += m.ro[0]
        # typeinfo struct: [vptr][name*] ; its name* word is at ti+4
        for na in widx.get(sa, []):
            ti = na - 4
            # vtable for this class: [0][ti][funcs...] -> address point = +8
            for ta in widx.get(ti, []):
                if m.u32(ta - 4) == 0 and m.in_text(m.u32(ta + 4)):
                    vptrs[kind] = ta + 4
    return vptrs


def parse_typeinfos(m, widx, vptrs):
    kinds = {v: k for k, v in vptrs.items()}
    tis = {}
    for vp, kind in kinds.items():
        for a in widx.get(vp, []):
            name = m.cstr(m.u32(a + 4))
            if not name:
                continue
            t = TypeInfo(a, kind, name)
            if kind == 'si':
                t.bases.append((m.u32(a + 8), 0, False, True))
                t.size = 12
            elif kind == 'vmi':
                cnt = m.u32(a + 12)
                if cnt > 64:
                    continue
                for i in range(cnt):
                    b = m.u32(a + 16 + i * 8)
                    of = m.s32(a + 20 + i * 8)
                    t.bases.append((b, of >> 8, bool(of & 1), bool(of & 2)))
                t.size = 16 + cnt * 8
            tis[a] = t
    # demangle the type names
    order = list(tis.values())
    dem = demangle(['_ZTS' + t.mangled for t in order])
    for t, d in zip(order, dem):
        d = d.strip()
        t.name = d[len('typeinfo name for '):] if d.startswith('typeinfo name for ') else t.mangled
    for t in order:
        for b, *_ in t.bases:
            if b in tis:
                tis[b].children.append(t.addr)
    return tis


def parse_vtables(m, widx, tis):
    inside_ti = set()
    for t in tis.values():
        for i in range(0, t.size, 4):
            inside_ti.add(t.addr + i)
    vts = []
    for ta, t in tis.items():
        for a in widx.get(ta, []):
            if a in inside_ti:
                continue
            ott = m.s32(a - 4)
            if not (-0x10000 < ott <= 0):
                continue
            funcs = []
            p = a + 4
            while m.valid(p) and m.in_text(m.u32(p)):
                funcs.append(m.u32(p))
                p += 4
            if not funcs:
                continue
            vt = VTable(a - 4, t, ott, funcs)
            t.vtables.append(vt)
            vts.append(vt)
    # vtables that ran into text data (e.g. an UTF-16 path right behind a
    # one-entry vtable) are cut where the text starts
    import xrefs, funcstart
    funcstart.trim_vtables(m, xrefs.Xrefs(m), vts)
    vts = [v for v in vts if v.funcs]
    for t in tis.values():
        t.vtables = [v for v in t.vtables if v.funcs]
        t.vtables.sort(key=lambda v: -v.offset_to_top)
    return vts


def primary_vtable(t):
    for v in t.vtables:
        if v.offset_to_top == 0:
            return v
    return None
