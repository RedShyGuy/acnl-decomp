"""Decompress a backward-LZ (ExeFS .code) image stored in an ELF's text segment
and write a proper ELF with text / rodata / data segments.

usage: python blz_fix_elf.py <in.elf> <out.elf>
Segment bases are taken from the input ELF's program headers.
"""
import struct, sys


def blz_decompress(comp):
    top, extra = struct.unpack_from('<II', comp, len(comp) - 8)
    hdr, enc = top >> 24, top & 0xFFFFFF
    out = bytearray(comp) + bytearray(extra)
    end = len(comp) - enc
    src = len(comp) - hdr
    dst = len(out)
    while src > end:
        src -= 1
        flags = out[src]
        for _ in range(8):
            if src <= end:
                break
            if flags & 0x80:
                src -= 2
                v = out[src] | (out[src + 1] << 8)
                n = ((v >> 12) & 0xF) + 3
                disp = (v & 0xFFF) + 3
                for _ in range(n):
                    dst -= 1
                    out[dst] = out[dst + disp]
            else:
                src -= 1
                dst -= 1
                out[dst] = out[src]
            flags = (flags << 1) & 0xFF
    return bytes(out)


def main(inp, outp):
    d = open(inp, 'rb').read()
    phoff = struct.unpack_from('<I', d, 0x1c)[0]
    phsz, phn = struct.unpack_from('<HH', d, 0x2a)
    ph = [struct.unpack_from('<8I', d, phoff + i * phsz) for i in range(phn)]
    loads = [p for p in ph if p[0] == 1]
    text = loads[0]
    img = blz_decompress(d[text[1]:text[1] + text[4]])
    base = text[2]
    ro_va = loads[1][2]
    rw_va = loads[2][2]
    bss = [p for p in loads if p[4] == 0 and p[5]]
    bss_sz = bss[-1][5] if bss else 0
    img_end = base + len(img)
    segs = [  # (va, filesz, memsz, flags)
        (base, ro_va - base, ro_va - base, 5),
        (ro_va, rw_va - ro_va, rw_va - ro_va, 4),
        (rw_va, img_end - rw_va, img_end - rw_va + bss_sz, 6),
    ]
    hdr_size = 0x34 + 0x20 * len(segs)
    data_off = 0x1000
    e = bytearray(d[:0x34])
    struct.pack_into('<IIII', e, 0x1c, 0x34, 0, 0, 0)          # phoff, shoff=0, flags
    struct.pack_into('<HHHHHH', e, 0x28, 0x34, 0x20, len(segs), 0x28, 0, 0)
    for va, fsz, msz, fl in segs:
        e += struct.pack('<8I', 1, data_off + (va - base), va, va, fsz, msz, fl, 4)
    e += bytes(data_off - len(e))
    e += img
    open(outp, 'wb').write(e)
    print(f'decompressed {len(img):#x} bytes -> {outp}')
    for va, fsz, msz, fl in segs:
        print(f'  seg {va:#x} filesz {fsz:#x} memsz {msz:#x} flags {fl}')


if __name__ == '__main__':
    main(*sys.argv[1:3])
