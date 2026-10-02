"""Code cross references: literal-pool loads, BL call graph, function bounds."""
import bisect
from collections import defaultdict
from elfmem import (is_ldr_pc, ldr_pc_target, is_bl, bl_target,
                    is_uncond_return, is_push)


class Xrefs:
    def __init__(self, m, extra_starts=()):
        self.m = m
        self.lit_loads = defaultdict(list)   # loaded value -> [instr addr]
        self.lit_addrs = set()                # addresses that are literal pool words
        self.calls = defaultdict(list)        # callee -> [call site]
        self.callees = {}                     # call site -> callee
        for a, ins in m.words(*m.text):
            if is_ldr_pc(ins):
                t = ldr_pc_target(a, ins)
                if m.in_text(t):
                    self.lit_addrs.add(t)
                    self.lit_loads[m.u32(t)].append(a)
            elif is_bl(ins):
                t = bl_target(a, ins)
                if m.in_text(t):
                    self.calls[t].append(a)
                    self.callees[a] = t
            elif (ins & 0x0F3F0E00) == 0x0D1F0A00:
                # VLDR Sd/Dd, [PC, #+/-imm8*4]: float/double literal pool entries
                off = (ins & 0xFF) * 4
                t = ((a + 8) & ~3) + (off if ins & (1 << 23) else -off)
                if m.in_text(t):
                    self.lit_addrs.add(t)
                    if ins & 0x100:                     # double: two words
                        self.lit_addrs.add(t + 4)
        # BL targets that are literal words are decode noise
        starts = {t for t in self.calls if t not in self.lit_addrs}
        starts.update(extra_starts)
        self.starts = sorted(starts)

    def func_start(self, a):
        """Best guess for the start of the function containing a."""
        m = self.m
        i = bisect.bisect_right(self.starts, a) - 1
        cand = self.starts[i] if i >= 0 else m.text[0]
        # walk back from a; stop at a push or an unconditional return
        p = a - 4
        while p >= cand and p >= a - 0x2000:
            ins = m.u32(p)
            if p in self.lit_addrs:
                break
            if is_uncond_return(ins):
                break
            if is_push(ins):
                # a known call target a few instructions earlier wins (prologue before push)
                if 0 <= p - cand <= 16 and not any(
                        is_uncond_return(m.u32(q)) or q in self.lit_addrs
                        for q in range(cand, p, 4)):
                    return cand
                return p
            p -= 4
        s = p + 4
        while s in self.lit_addrs and s < a:
            s += 4
        return max(s, cand) if cand > p else s

    def func_body(self, start, limit=0x800):
        """yield (addr, ins) from start until the first unconditional return
        (continues past it if the code keeps going with non-pool words)."""
        m = self.m
        p = start
        while p < start + limit and m.in_text(p):
            if p in self.lit_addrs:
                return
            ins = m.u32(p)
            yield p, ins
            if is_uncond_return(ins):
                # tail: another return path may follow; stop if next is pool / push
                n = p + 4
                if n in self.lit_addrs or is_push(m.u32(n)) or n in self._startset():
                    return
            p += 4

    def _startset(self):
        if not hasattr(self, '_ss'):
            self._ss = set(self.starts)
        return self._ss

    def loaded_values(self, start):
        """literal values loaded inside the function at start"""
        out = []
        for a, ins in self.func_body(start):
            if is_ldr_pc(ins):
                t = ldr_pc_target(a, ins)
                if self.m.in_text(t):
                    out.append((a, self.m.u32(t)))
        return out

    def called(self, start):
        return [(a, self.callees[a]) for a, _ in self.func_body(start) if a in self.callees]
