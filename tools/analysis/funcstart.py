"""Is an address a plausible function start? Used to stop vtable scans that
run into non-code data whose words happen to fall into the .text range
(e.g. UTF-16 strings like 0x003A0063)."""
import struct
from elfmem import is_uncond_return, is_push


def looks_like_text(v):
    """all four bytes are NUL or printable ASCII and at least two are printable"""
    bs = [(v >> s) & 0xFF for s in (0, 8, 16, 24)]
    return all(b == 0 or 0x20 <= b < 0x7F for b in bs) and sum(1 for b in bs if b) >= 2


def trim_vtables(m, x, vts, min_run=3, ratio=0.75):
    """Cut a vtable at the first implausible entry after which (almost) only
    implausible entries follow - that is where the scan ran into data.
    Single odd entries inside an otherwise plausible vtable are kept.
    x must be built WITHOUT the vtable entries as extra starts."""
    cut_tables = cut_entries = 0
    for vt in vts:
        fl = [plausible_start(m, x, f) for f in vt.funcs]
        for i, ok in enumerate(fl):
            # only a text-looking value (ASCII / UTF-16 bytes) marks the start of
            # data; real functions may follow embedded strings and look odd otherwise
            # data never comes as a single word: require two text-like entries in a row
            if (ok or not looks_like_text(vt.funcs[i]) or i + 1 >= len(vt.funcs)
                    or not looks_like_text(vt.funcs[i + 1])):
                continue
            rest = fl[i:]
            bad = sum(1 for z in rest if not z)
            if (len(rest) >= min_run and bad >= ratio * len(rest)) or (len(rest) < min_run and bad == len(rest)):
                cut_entries += len(rest)
                cut_tables += 1
                vt.funcs = vt.funcs[:i]
                break
    return cut_tables, cut_entries


def _h(m, a):
    return struct.unpack_from('<H', m.mem, a - m.base)[0]


def plausible_start(m, x, v):
    if not m.in_text(v & ~1):
        return False
    starts = x._startset()
    if v in starts or (v & ~1) in starts:
        return True
    if v & 1:                                   # Thumb
        a = v & ~1
        first = _h(m, a)
        if (first & 0xFE00) == 0xB400:          # push {..}
            return True
        prev = _h(m, a - 2)
        # previous function ended with bx lr / pop {..,pc}, or we follow padding
        return prev == 0x4770 or (prev & 0xFF00) == 0xBD00 or prev == 0x0000
    if v & 3:
        return False                            # ARM code is word aligned
    if is_push(m.u32(v)):
        return True
    prev = v - 4
    p = m.u32(prev)
    return (is_uncond_return(p) or prev in x.lit_addrs or prev - 4 in x.lit_addrs
            or (p & 0xFF000000) == 0xEA000000          # b ... (tail call ends a function)
            or (p & 0xFFFFFFF0) == 0xE12FFF10          # bx rN
            or (p & 0xFE108000) == 0xE8108000          # ldm ..., {..., pc}
            or p in (0xE1A00000, 0x00000000))          # nop / zero padding between functions
