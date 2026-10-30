"""Thumb-1 (ARMv6) to ARM translation for the comparison (used by check.py).

fuzzy.extract only decodes ARM. Thumb-1 was designed as a compressed form of ARM: almost every Thumb
instruction is one ARM instruction. translate() turns a Thumb function into a list of ARM words with the
same registers, memory accesses, constants and control flow, so that both sides (the original Thumb code
and GCC's Thumb code) go through the unchanged ARM feature extraction:

- every instruction becomes one ARM word (a bl/blx pair, too); branches inside the function are
  re-targeted to the ARM index of their target
- the literals that "ldr rX, [pc, #imm]" loads are appended behind the code, each once, and the load
  points there
- calls (bl/blx and b out of the function) are reported by ARM index with the Thumb offset of the
  instruction (for relocations) and the target address (for the original)
- "ldr rX, =offset ... add rX, pc" (position independent addressing) is reported with the pc value

The result: Translation(words, calls, literals, pcrel), see there.
"""

COND_AL = 0xE
NEUTRAL = 0xE1A0B00B          # mov fp, fp: a word for halfwords that are not translated


class Translation:
    def __init__(self):
        self.words = []           # ARM words
        self.calls = {}           # ARM index -> (Thumb offset of the instruction, target address or None)
        self.literals = {}        # ARM index of an appended literal -> Thumb offset of the literal
        self.pcrel = {}           # ARM index of an appended literal -> pc that "add rX, pc" adds to it
        self.offsets = []         # ARM index -> Thumb offset (code words only)


def _imm(value):
    """ARM rotated immediate operand for value (None if it does not fit)"""
    value &= 0xFFFFFFFF
    for rot in range(16):
        v = ((value << (2 * rot)) | (value >> (32 - 2 * rot))) & 0xFFFFFFFF if rot else value
        if v < 256:
            return (rot << 8) | v
    return None


def _dp(op, s, rn, rd, operand, imm=False):
    return (COND_AL << 28) | (0x02000000 if imm else 0) | (op << 21) | (s << 20) | (rn << 16) | (rd << 12) | operand


def _dp_imm(op, s, rn, rd, value):
    enc = _imm(value)
    if enc is None:
        return NEUTRAL
    return _dp(op, s, rn, rd, enc, True)


AND, EOR, SUB, RSB, ADD, ADC, SBC, RSC, TST, TEQ, CMP, CMN, ORR, MOV, BIC, MVN = range(16)


def _sext(value, bits):
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def _decode(h, h2, addr):
    """one Thumb instruction at addr: (kind, ARM word or None, extra). kind: 'op', 'branch' (extra = target,
    word = condition), 'call' (extra = target, word = 'bl'/'blx'), 'literal' (extra = (rd, address)),
    'adr' (extra = (rd, address)), 'addpc' (extra = rd), 'pair' (32 bit), None = not translated"""
    top = h >> 11
    if top < 3:                                                   # lsl/lsr/asr #imm
        imm, rm, rd = (h >> 6) & 31, (h >> 3) & 7, h & 7
        return 'op', _dp(MOV, 1, 0, rd, rm | (top << 5) | (imm << 7)), None
    if top == 3:                                                  # add/sub register or imm3
        rn_imm, rs, rd = (h >> 6) & 7, (h >> 3) & 7, h & 7
        op = SUB if h & 0x200 else ADD
        if h & 0x400:
            return 'op', _dp(op, 1, rs, rd, rn_imm, True), None
        return 'op', _dp(op, 1, rs, rd, rn_imm), None
    if top < 8:                                                   # mov/cmp/add/sub #imm8
        op, rd, imm = (h >> 11) & 3, (h >> 8) & 7, h & 0xFF
        arm = (MOV, CMP, ADD, SUB)[op]
        return 'op', _dp(arm, 1, 0 if arm == MOV else rd, 0 if arm == CMP else rd, imm, True), None
    if h >> 10 == 0x10:                                           # ALU register
        op, rs, rd = (h >> 6) & 15, (h >> 3) & 7, h & 7
        if op in (2, 3, 4, 7):                                    # lsl/lsr/asr/ror by register
            stype = {2: 0, 3: 1, 4: 2, 7: 3}[op]
            return 'op', _dp(MOV, 1, 0, rd, rd | (stype << 5) | 0x10 | (rs << 8)), None
        if op == 9:                                               # neg = rsbs rd, rs, #0
            return 'op', _dp(RSB, 1, rs, rd, 0, True), None
        if op == 13:                                              # muls rd, rs, rd
            return 'op', (COND_AL << 28) | 0x00100090 | (rd << 16) | (rd << 8) | rs, None
        arm = {0: AND, 1: EOR, 5: ADC, 6: SBC, 8: TST, 10: CMP, 11: CMN, 12: ORR, 14: BIC, 15: MVN}[op]
        if arm in (TST, CMP, CMN):
            return 'op', _dp(arm, 1, rd, 0, rs), None
        if arm == MVN:
            return 'op', _dp(MVN, 1, 0, rd, rs), None
        return 'op', _dp(arm, 1, rd, rd, rs), None
    if h >> 10 == 0x11:                                           # hi register ops / bx
        op = (h >> 8) & 3
        rm = (h >> 3) & 15
        rd = (h & 7) | ((h >> 4) & 8)
        if op == 3:
            if h & 0x80:
                return 'op', 0xE12FFF30 | rm, None                # blx rm
            return 'op', 0xE12FFF10 | rm, None                    # bx rm
        if op == 0 and rm == 15:
            return 'addpc', 0xE08F0000 | (rd << 12) | rd, rd      # add rd, pc
        if op == 0:
            return 'op', _dp(ADD, 0, rd, rd, rm), None
        if op == 1:
            return 'op', _dp(CMP, 1, rd, 0, rm), None
        return 'op', _dp(MOV, 0, 0, rd, rm), None
    if h >> 11 == 9:                                              # ldr rd, [pc, #imm]
        rd, imm = (h >> 8) & 7, (h & 0xFF) * 4
        return 'literal', None, (rd, ((addr + 4) & ~3) + imm)
    if h >> 12 == 5:                                              # load/store register offset
        ro, rb, rd = (h >> 6) & 7, (h >> 3) & 7, h & 7
        op = (h >> 9) & 7
        if op in (0, 2, 4, 6):                                    # str strb ldr ldrb
            byte = op in (2, 6)
            load = op in (4, 6)
            return 'op', 0xE7800000 | (byte << 22) | (load << 20) | (rb << 16) | (rd << 12) | ro, None
        sh = {1: (0, 0xB0), 3: (1, 0xD0), 5: (1, 0xB0), 7: (1, 0xF0)}[op]   # strh ldrsb ldrh ldrsh
        return 'op', 0xE1800000 | (sh[0] << 20) | (rb << 16) | (rd << 12) | sh[1] | ro, None
    if h >> 13 == 3:                                              # ldr/str(b) #imm5
        byte, load = (h >> 12) & 1, (h >> 11) & 1
        imm, rb, rd = (h >> 6) & 31, (h >> 3) & 7, h & 7
        imm = imm if byte else imm * 4
        return 'op', 0xE5800000 | (byte << 22) | (load << 20) | (rb << 16) | (rd << 12) | imm, None
    if h >> 12 == 8:                                              # ldrh/strh #imm5
        load, imm, rb, rd = (h >> 11) & 1, ((h >> 6) & 31) * 2, (h >> 3) & 7, h & 7
        return 'op', 0xE1C000B0 | (load << 20) | (rb << 16) | (rd << 12) | ((imm >> 4) << 8) | (imm & 15), None
    if h >> 12 == 9:                                              # ldr/str rd, [sp, #imm]
        load, rd, imm = (h >> 11) & 1, (h >> 8) & 7, (h & 0xFF) * 4
        return 'op', 0xE5800000 | (load << 20) | (13 << 16) | (rd << 12) | imm, None
    if h >> 12 == 10:                                             # add rd, pc/sp, #imm
        rd, imm = (h >> 8) & 7, (h & 0xFF) * 4
        if h & 0x800:
            return 'op', _dp_imm(ADD, 0, 13, rd, imm), None
        return 'adr', None, (rd, ((addr + 4) & ~3) + imm)
    if h >> 8 == 0xB0:                                            # add/sub sp, #imm7
        imm = (h & 0x7F) * 4
        return 'op', _dp_imm(SUB if h & 0x80 else ADD, 0, 13, 13, imm), None
    if h >> 8 == 0xB2:                                            # sxth sxtb uxth uxtb
        rm, rd = (h >> 3) & 7, h & 7
        base = (0xE6BF0070, 0xE6AF0070, 0xE6FF0070, 0xE6EF0070)[(h >> 6) & 3]
        return 'op', base | (rd << 12) | rm, None
    if h >> 8 == 0xBA:                                            # rev rev16 - revsh
        rm, rd = (h >> 3) & 7, h & 7
        base = {0: 0xE6BF0F30, 1: 0xE6BF0FB0, 3: 0xE6FF0FB0}.get((h >> 6) & 3)
        return ('op', base | (rd << 12) | rm, None) if base else (None, None, None)
    if (h >> 9) & 0x7B == 0x5A:                                   # push/pop (1011 x10x)
        rlist = h & 0xFF
        if h & 0x800:
            return 'op', 0xE8BD0000 | rlist | ((h & 0x100) << 7), None          # pop {.., pc}
        return 'op', 0xE92D0000 | rlist | ((h & 0x100) << 6), None              # push {.., lr}
    if h >> 12 == 12:                                             # ldmia/stmia rb!, {list}
        load, rb, rlist = (h >> 11) & 1, (h >> 8) & 7, h & 0xFF
        wb = 0 if load and rlist & (1 << rb) else 1
        return 'op', 0xE8800000 | (load << 20) | (wb << 21) | (rb << 16) | rlist, None
    if h >> 12 == 13:                                             # b<cond> / svc
        cond = (h >> 8) & 15
        if cond == 15:
            return 'op', 0xEF000000 | (h & 0xFF), None
        if cond == 14:
            return None, None, None
        return 'branch', cond, addr + 4 + _sext(h & 0xFF, 8) * 2
    if h >> 11 == 0x1C:                                           # b
        return 'branch', COND_AL, addr + 4 + _sext(h & 0x7FF, 11) * 2
    if h >> 11 == 0x1E and h2 is not None and h2 >> 11 in (0x1F, 0x1D):   # bl / blx pair
        offset = (_sext(h & 0x7FF, 11) << 12) | ((h2 & 0x7FF) << 1)
        target = addr + 4 + offset
        if h2 >> 11 == 0x1D:
            return 'pair', 'blx', target & ~3
        return 'pair', 'bl', target
    if h == 0xBF00 or h >> 8 == 0xBE:                             # nop / bkpt
        return 'op', NEUTRAL, None
    return None, None, None


def translate(read_half, start, size, read_word=None, calls_at=()):
    """read_half(offset) -> halfword at start + offset (offsets from 0 to size); read_word(offset) -> 32 bit
    word at start + offset (literals); calls_at: offsets of branches with a relocation (an object file: their
    target is a symbol). Returns a Translation."""
    if read_word is None:
        def read_word(o):
            return read_half(o) | (read_half(o + 2) << 16)
    # pass 1: the literals and the data behind the code (from the first literal on)
    data = set()
    o = 0
    while o < size:
        h = read_half(o)
        if (h >> 11) == 9:
            lit = ((start + o + 4) & ~3) + (h & 0xFF) * 4 - start
            if 0 <= lit < size:
                data.update((lit, lit + 2))
        o += 4 if (h >> 11 == 0x1E and o + 2 < size and read_half(o + 2) >> 11 in (0x1F, 0x1D)) else 2
    first_data = min(data) if data else size
    # pass 2: translate the code; branch targets are fixed up afterwards
    t = Translation()
    index_of = {}
    fixups = []
    literal_index = {}
    pending_literals = []
    last_literal = {}                                             # register -> literal address loaded
    o = 0
    while o < min(size, first_data):
        h = read_half(o)
        h2 = read_half(o + 2) if o + 2 < size else None
        addr = start + o
        kind, word, extra = _decode(h, h2, addr)
        index_of[o] = len(t.words)
        t.offsets.append(o)
        if kind == 'op':
            t.words.append(word)
            rd = (word >> 12) & 15
            last_literal.pop(rd, None)
        elif kind == 'branch':
            target = extra
            if o in calls_at:
                t.calls[len(t.words)] = (o, None)
                t.words.append((word << 28) | 0x0A000000)
            elif start <= target < start + size:
                fixups.append((len(t.words), word, target - start))
                t.words.append(0)
            else:
                # a tail call (b out of the function)
                t.calls[len(t.words)] = (o, target)
                t.words.append((word << 28) | 0x0A000000)
        elif kind == 'pair':
            t.calls[len(t.words)] = (o, None if o in calls_at else extra)
            t.words.append(0xEB000000)                            # bl (target from t.calls)
            index_of[o + 2] = len(t.words) - 1
            o += 4
            continue
        elif kind in ('literal', 'adr'):
            rd, address = extra
            pending_literals.append((len(t.words), rd, address, kind))
            t.words.append(0)
            if kind == 'literal':
                last_literal[rd] = address
            else:
                last_literal.pop(rd, None)
        elif kind == 'addpc':
            rd = extra
            if rd in last_literal:
                pending_literals.append((None, rd, last_literal.pop(rd), addr + 4))
            t.words.append(word)
        else:
            t.words.append(NEUTRAL)
        o += 2
    n_code = len(t.words)
    for i, cond, target_off in fixups:
        j = index_of.get(target_off)
        if j is None:
            j = index_of.get(target_off - 2, n_code)
        t.words[i] = (cond << 28) | 0x0A000000 | ((j - i - 2) & 0xFFFFFF)
    # the literals behind the code
    pc_of = {}
    for i, rd, address, kind in pending_literals:
        if i is None:
            pc_of[address] = kind                                 # (kind is the pc of "add rX, pc")
            continue
    for i, rd, address, kind in pending_literals:
        if i is None:
            continue
        key = (address, kind)
        if key not in literal_index:
            literal_index[key] = len(t.words)
            if kind == 'literal':
                t.words.append(read_word(address - start) if 0 <= address - start <= size - 4 else 0)
                t.literals[literal_index[key]] = address - start
                if address in pc_of:
                    t.pcrel[literal_index[key]] = pc_of[address]
            else:
                t.words.append(address)                           # adr: the address itself
        j = literal_index[key]
        t.words[i] = 0xE59F0000 | (rd << 12) | ((j - i - 2) * 4)  # ldr rd, [pc, #...]
    return t
