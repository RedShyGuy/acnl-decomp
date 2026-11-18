"""The imports of the static module: which module export is patched into which place of code.bin.

code.bin itself imports a few functions from the modules (static.crs has an import table like a CRO). The places
are `ldr pc, [pc, #-4]` thunks in code.bin whose word points to an "unresolved import" stub until the module is
loaded; ro then writes the address of the module function into the word.

usage: python tools/analysis/static_imports.py <lo> <hi>
prints every patched place in [lo, hi) with module, kind (indexed/anonymous), file offset in the .cro and the name of
the module function if config/<version>/modules/<Module>.json knows it.
"""
import json, os, struct, sys
ROMFS = 'orig/0004000000086300/romfs'
d = open(os.path.join(ROMFS, 'static.crs'), 'rb').read()
u = lambda b, o: struct.unpack_from('<I', b, o)[0]
STATIC_SEGS = [0x00100000, 0x0083A000, 0x00946000, 0x00946000]


def cstr(b, o):
    return b[o:b.find(b'\0', o)].decode()


def segs_of(b):
    return [struct.unpack_from('<3I', b, u(b, 0xC8) + i * 12) for i in range(u(b, 0xCC))]


mods = {}
for f in os.listdir(ROMFS):
    if f.endswith('.cro'):
        b = open(os.path.join(ROMFS, f), 'rb').read()
        mods[f[:-4]] = b

out = []
mt, mn = u(d, 0xF0), u(d, 0xF4)
for i in range(mn):
    no, io, ic, ao, ac = struct.unpack_from('<5I', d, mt + i * 20)
    mname = cstr(d, no)
    mb = mods.get(mname)
    entries = []
    for k in range(ic):
        idx, po = struct.unpack_from('<2I', d, io + k * 8)
        enc = u(mb, u(mb, 0xD8) + idx * 4) if mb else None
        entries.append(('idx%d' % idx, enc, po))
    for k in range(ac):
        so, po = struct.unpack_from('<2I', d, ao + k * 8)
        entries.append(('anon', so, po))
    for kind, enc, po in entries:
        src = None
        if enc is not None and mb is not None:
            segs = segs_of(mb)
            s = segs[enc & 0xF]
            src = s[0] + (enc >> 4)
        while True:
            t, typ, last, _, _, add = struct.unpack_from('<IBBBBI', d, po)
            target = STATIC_SEGS[t & 0xF] + (t >> 4)
            out.append((target, mname, kind, src, add))
            if last:
                break
            po += 12

names = {}
for m in mods:
    p = 'config/0004000000086300/modules/%s.json' % m
    if os.path.exists(p):
        j = json.load(open(p))
        names[m] = {f['offset']: f['name'] for f in j['functions']}

lo, hi = (int(x, 16) for x in sys.argv[1:3])
for target, m, kind, src, add in sorted(out):
    if lo <= target < hi:
        nm = names.get(m, {}).get(src) if src is not None else None
        print(hex(target), m, kind, hex(src) if src is not None else None, nm)
