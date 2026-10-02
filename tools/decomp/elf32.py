"""Minimal reader for 32 bit little endian ARM ELF files (objects and executables).

Only what the decomp tools need: sections, symbols and REL relocations.
"""
import struct

SHT_SYMTAB, SHT_REL, SHT_RELA = 2, 9, 4
STT_FUNC, STT_OBJECT = 2, 1


class Section:
    def __init__(self, idx, name, type_, flags, addr, offset, size, link, info, entsize, data):
        self.idx, self.name, self.type, self.flags = idx, name, type_, flags
        self.addr, self.offset, self.size = addr, offset, size
        self.link, self.info, self.entsize = link, info, entsize
        self.data = data


class Symbol:
    def __init__(self, name, value, size, info, shndx):
        self.name, self.value, self.size, self.shndx = name, value, size, shndx
        self.type = info & 0xF
        self.bind = info >> 4


class Elf:
    def __init__(self, path):
        d = open(path, 'rb').read()
        if d[:4] != b'\x7fELF' or d[4] != 1 or d[5] != 1:
            raise ValueError(f'{path}: not a 32 bit little endian ELF')
        self.data = d
        self.type = struct.unpack_from('<H', d, 16)[0]
        shoff = struct.unpack_from('<I', d, 0x20)[0]
        shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2E)
        raw = [struct.unpack_from('<10I', d, shoff + i * shentsize) for i in range(shnum)]
        strtab = raw[shstrndx]
        names = d[strtab[4]:strtab[4] + strtab[5]]
        self.sections = []
        for i, (nm, t, fl, addr, off, size, link, info, align, ent) in enumerate(raw):
            name = names[nm:names.index(b'\0', nm)].decode('ascii', 'replace')
            body = d[off:off + size] if t != 8 else b''              # 8 = NOBITS
            self.sections.append(Section(i, name, t, fl, addr, off, size, link, info, ent, body))
        self.symbols = []
        for s in self.sections:
            if s.type != SHT_SYMTAB:
                continue
            st = self.sections[s.link].data
            for k in range(s.size // 16):
                nm, value, size, info, other, shndx = struct.unpack_from('<IIIBBH', s.data, k * 16)
                name = st[nm:st.index(b'\0', nm)].decode('ascii', 'replace')
                self.symbols.append(Symbol(name, value, size, info, shndx))

    def relocations(self, section):
        """[(offset, type, symbol)] that apply to the given section"""
        out = []
        for s in self.sections:
            if s.type == SHT_REL and s.info == section.idx:
                for k in range(s.size // 8):
                    off, info = struct.unpack_from('<II', s.data, k * 8)
                    out.append((off, info & 0xFF, info >> 8))
            elif s.type == SHT_RELA and s.info == section.idx:
                for k in range(s.size // 12):
                    off, info, add = struct.unpack_from('<IIi', s.data, k * 12)
                    out.append((off, info & 0xFF, info >> 8))
        return out


# relocation type -> bits of the word that the linker fills in
R_ARM_ABS32, R_ARM_REL32, R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24, R_ARM_PREL31 = 2, 3, 28, 29, 1, 42
R_ARM_THM_CALL, R_ARM_MOVW_ABS_NC, R_ARM_MOVT_ABS, R_ARM_TARGET1, R_ARM_V4BX = 10, 43, 44, 38, 40


def reloc_mask(rtype):
    if rtype in (R_ARM_CALL, R_ARM_JUMP24, R_ARM_PC24):
        return 0x00FFFFFF
    if rtype == R_ARM_THM_CALL:
        return 0x07FF07FF
    if rtype in (R_ARM_MOVW_ABS_NC, R_ARM_MOVT_ABS):
        return 0x000F0FFF
    if rtype == R_ARM_V4BX:
        return 0
    return 0xFFFFFFFF
