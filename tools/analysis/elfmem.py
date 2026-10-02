"""Minimal ELF loader + ARM helpers for the ACNL code.bin ELF."""
import struct, subprocess, bisect

class Mem:
    def __init__(self, path):
        d = open(path, 'rb').read()
        phoff = struct.unpack_from('<I', d, 0x1c)[0]
        phsz, phn = struct.unpack_from('<HH', d, 0x2a)
        self.segs = []
        for i in range(phn):
            t, off, va, pa, fsz, msz, fl, al = struct.unpack_from('<8I', d, phoff + i * phsz)
            if t == 1:
                self.segs.append((va, fsz, msz, off, fl))
        self.base = min(s[0] for s in self.segs)
        end = max(s[0] + s[2] for s in self.segs)
        self.mem = bytearray(end - self.base)
        for va, fsz, msz, off, fl in self.segs:
            self.mem[va - self.base: va - self.base + fsz] = d[off:off + fsz]
        text = [s for s in self.segs if s[4] & 1][0]
        self.text = (text[0], text[0] + text[1])
        ro = [s for s in self.segs if s[4] == 4][0]
        self.ro = (ro[0], ro[0] + ro[1])
        rw = [s for s in self.segs if s[4] == 6]
        self.data = (min(s[0] for s in rw), max(s[0] + s[2] for s in rw))
        self.data_init_end = max(s[0] + s[1] for s in rw)
        self.end = end

    def u32(self, a):
        return struct.unpack_from('<I', self.mem, a - self.base)[0]

    def s32(self, a):
        return struct.unpack_from('<i', self.mem, a - self.base)[0]

    def valid(self, a):
        return self.base <= a < self.end - 3

    def in_text(self, a):
        return self.text[0] <= a < self.text[1]

    def in_data(self, a):
        return self.data[0] <= a < self.data[1]

    def cstr(self, a, maxlen=512):
        if not self.valid(a):
            return None
        o = a - self.base
        e = self.mem.find(b'\0', o, o + maxlen)
        if e < 0:
            return None
        s = self.mem[o:e]
        if not s or any(c < 0x20 or c > 0x7e for c in s):
            return None
        return s.decode()

    def words(self, lo, hi):
        """yield (addr, value) for each aligned word in [lo, hi)"""
        lo = (lo + 3) & ~3
        n = (hi - lo) // 4
        vals = struct.unpack_from('<%dI' % n, self.mem, lo - self.base)
        for i, v in enumerate(vals):
            yield lo + i * 4, v


# ---- ARM decoding helpers ----
def is_ldr_pc(ins):
    # LDR Rd, [PC, #+/-imm12]
    return (ins & 0x0F7F0000) == 0x051F0000

def ldr_pc_target(a, ins):
    imm = ins & 0xFFF
    return a + 8 + imm if ins & (1 << 23) else a + 8 - imm

def is_bl(ins):
    return (ins & 0x0F000000) == 0x0B000000 and (ins >> 28) != 0xF

def bl_target(a, ins):
    off = ins & 0xFFFFFF
    if off & 0x800000:
        off -= 0x1000000
    return a + 8 + off * 4

def is_uncond_return(ins):
    return (ins == 0xE12FFF1E or (ins & 0xFFFF8000) == 0xE8BD8000
            or ins == 0xE49DF004)

def is_push(ins):
    return (ins & 0xFFFF0000) == 0xE92D0000


def demangle(names):
    """Demangle a list of mangled names in one c++filt call."""
    if not names:
        return []
    p = subprocess.run(['arm-none-eabi-c++filt'], input='\n'.join(names),
                       capture_output=True, text=True)
    out = p.stdout.split('\n')
    return out[:len(names)]
