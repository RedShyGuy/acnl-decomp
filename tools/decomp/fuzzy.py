"""Compiler independent comparison of two ARM functions (used by check.py).

The project builds with GCC, Nintendo used ARMCC 4.1, so a byte match is impossible. A correct
decompilation still has to agree with the original in everything the compiler cannot choose:

  calls     which functions are called, in which order (bl / b to other functions, svc)
  mem       member accesses: width, signedness, direction and offset from the base object
            (register + constant tracking, so "add r0,r0,#0x1000 ; ldr r1,[r0,#0x90]"
            and "ldr r1,[r4,#0x1090]" are the same access)
  consts    literal pool values, compare / move immediates, referenced strings and addresses
  flow      decisions (a compare and its first conditional use - branch or conditional
            execution, the compiler decides which), switch tables
  shape     rough instruction class sequence (informational only, mostly compiler style)

Each category gives a similarity in [0, 1] (sum of minimum / sum of maximum counts of the
feature multisets); the score is the weighted mean of calls / mem / consts / flow over the
categories present on either side (shape only when none of them is).
Only ARM state is decoded (the original has 134 Thumb functions out of 32 000).
"""
import difflib
from collections import Counter

# shape is reported, but not scored: it mostly shows the compiler's style
WEIGHTS = {'calls': 0.30, 'mem': 0.30, 'consts': 0.20, 'flow': 0.20}

# C library / runtime helpers that ARMCC and GCC pick differently for the same source
# (memset(p, 0, n) -> __rt_memclr, a / b -> __aeabi_idiv or __rt_sdiv, ...)
RUNTIME_FAMILIES = {
    'memclr': ('memset', 'memclr', 'bzero'),
    'memcpy': ('memcpy', 'memmove'),
    'sdiv': ('idiv', 'sdiv', 'idivmod'),
    'udiv': ('uidiv', 'udiv', 'uidivmod'),
    'ldiv': ('ldivmod', 'sdiv64', 'divdi3'),
    'uldiv': ('uldivmod', 'udiv64', 'udivdi3'),
    'strcmp': ('strcmp',),
    'strlen': ('strlen',),
    # ARMCC constructs an array of class objects with this helper (array, ctor, size, count);
    # GCC writes the loop or the stores itself (see extract)
    'vec_ctor': ('vec_ctor_nocookie_nodtor',),
}


def runtime_family(name):
    """'__rt_memclr_w' / '__aeabi_memclr4' / 'memset' -> 'memclr'; None for other names"""
    if not name:
        return None
    base = name.split('(')[0].strip()
    if '::' in base or ' ' in base:
        return None                                   # methods, never runtime helpers
    core = base
    for p in ('__aeabi_', '__rt_', '__'):
        if core.startswith(p):
            core = core[len(p):]
            break
    core = core.rstrip('0123456789').removesuffix('_w')
    if base == core and base not in ('memset', 'memcpy', 'memmove', 'strcmp', 'strlen'):
        return None                                   # plain names other than the C library ones
    for fam, members in RUNTIME_FAMILIES.items():
        if core in members:
            return fam
    return None
SP, LR, PC = 13, 14, 15
COND_AL = 0xE


def _ror(v, r):
    return ((v >> r) | (v << (32 - r))) & 0xFFFFFFFF if r else v


def _sext24(v):
    return v - (1 << 24) if v & 0x800000 else v


def _is_exit(w, external_branch):
    """unconditional return or tail call"""
    if w >> 28 != COND_AL:
        return False
    return (w == 0xE12FFF1E                                              # bx lr
            or (w & 0x0E108000) == 0x08108000                            # ldm ...{..., pc}
            or (w & 0xFFFF0FFF) == 0xE49D0004 and (w >> 12) & 0xF == PC  # ldr pc,[sp],#4
            or w == 0xE1A0F00E                                           # mov pc,lr
            or (w & 0x0F000000) == 0x0A000000 and external_branch)       # b <other function>


def function_end(words, pool, resolve_call):
    """number of words that belong to the function: ends after an exit that no internal branch
    jumps past, plus its literal pool (guards against a missing function start in the original)"""
    n = len(words)
    reach = -1
    for i, w in enumerate(words):
        if i in pool:
            continue
        external = False
        if w >> 28 != 0xF and (w >> 25) & 7 == 0b101 and not w & (1 << 24):
            if resolve_call(i, w) is None:
                reach = max(reach, i + 2 + _sext24(w & 0xFFFFFF))
            else:
                external = True
        elif (w & 0x0000F000) == 0x0000F000 and w >> 28 != COND_AL and ((w >> 25) & 7) in (0, 1, 2, 3):
            reach = n                                                     # conditional pc write: jump table
        # bx rX: a tail call, or ARMCC's "ldr lr, =f2 ; bx rX" (returns to f2, not here); only
        # "mov lr, pc ; bx rX" comes back
        bx_reg = (w & 0xFFFFFFF0) == 0xE12FFF10 and not (i > 0 and words[i - 1] == 0xE1A0E00F)
        if (_is_exit(w, external) or bx_reg) and reach <= i:
            j = i + 1
            while j < n:
                if j in pool:
                    j += 1
                elif words[j] in (NOP, NOP_HINT) and any(k in pool for k in range(j + 1, min(n, j + 4))):
                    j += 1                                                # alignment before the pool
                else:
                    break
            return j
    return n


NOP = 0xE1A00000                                                         # mov r0, r0
NOP_HINT = 0xE320F000                                                    # nop {0}


def _arg_feature(words, pool, i, reg, resolve_word):
    """The constant that argument register reg holds at the call at index i, if one of the few
    instructions before set it ("mov / mvn rX, #imm" or "ldr rX, =literal"): its feature key as
    extract counts it, else None"""
    for j in range(i - 1, max(-1, i - 9), -1):
        if j in pool:
            continue
        w = words[j]
        if w >> 28 != COND_AL or (w >> 25) & 7 == 0b101:
            return None                                   # conditional code, branch or call
        rd = (w >> 12) & 0xF
        op3 = (w >> 25) & 7
        if op3 == 0b001 and rd == reg:                    # data processing, immediate
            opc = (w >> 21) & 0xF
            val = _ror(w & 0xFF, ((w >> 8) & 0xF) * 2)
            if opc == 13:
                return ('k', val) if val else None
            if opc == 15:
                return ('k', ~val & 0xFFFFFFFF)
            if 8 <= opc <= 11:
                continue                                  # tst / teq / cmp / cmn write no register
            return None
        if (w & 0x0F7F0000) == 0x051F0000 and w & (1 << 20) and rd == reg:     # ldr rX, [pc, #imm]
            off = (w & 0xFFF) * (1 if w & (1 << 23) else -1)
            k = j + 2 + off // 4
            return resolve_word(k, words[k]) if 0 <= k < len(words) else None
        if op3 == 0b100:
            if w & (1 << 20) and w & (1 << reg):          # ldm that loads reg
                return None
            continue
        if op3 in (0b000, 0b001) and rd == reg and not 8 <= (w >> 21) & 0xF <= 11:
            return None                                   # another write to reg
        if op3 in (0b010, 0b011) and w & (1 << 20) and rd == reg:
            return None                                   # a load into reg
    return None


def extract(words, base, resolve_call, resolve_word, returns_this=lambda key: False, structor=False,
            next_key=None, resolve_target=lambda value: ('?',), read_const=None):
    """Features of one ARM function.

    words           list of 32 bit words of the function (code + literal pool)
    base            address of words[0] (only used for pc relative arithmetic)
    resolve_call    (index, word) -> key of the call target; for bl None means unknown, for b
                    None means a branch inside the function (anything else is a tail call)
    resolve_word    (index, word) -> literal value as a feature tuple
    returns_this    key -> True if the callee returns its first argument (ctors / dtors)
    structor        the function is a ctor / dtor itself: a call with r0 = this + x is taken as a
                    member ctor / dtor, which returns this (ARMCC chains r0 through those calls)
    next_key        key of the function right after this one: armlink replaces a tail call to the
                    next function by a nop, so a nop as the last instruction is that tail call
    resolve_target  address -> call key; for ARMCC's "ldr lr, =f2 ; bx r3" (call r3, then f2)
    read_const      address -> word, for literals that point to read-only data (resolve_word gives
                    ('rconst', address)): a word loaded from there is a constant (ARMCC does not
                    fold const objects of class type, e.g. a const nn::Handle)
    """
    n = len(words)
    pool = set()
    # pass 1: literal pool words (targets of ldr rX,[pc,#imm])
    for i, w in enumerate(words):
        if w >> 28 == 0xF:
            continue
        if (w & 0x0F7F0000) == 0x051F0000 and w & (1 << 20):            # ldr rX,[pc,#+-imm]
            off = (w & 0xFFF) * (1 if w & (1 << 23) else -1)
            j = i + 2 + off // 4
            if 0 <= j < n and off % 4 == 0:
                pool.add(j)
        elif (w & 0x0E000000) == 0x0C000000 and (w >> 16) & 0xF == PC and w & (1 << 20) \
                and (w >> 8) & 0xE == 0xA:                                # vldr from pool
            off = (w & 0xFF) * 4 * (1 if w & (1 << 23) else -1)
            j = i + 2 + off // 4
            for k in (j, j + 1) if w & 0x100 else (j,):
                if 0 <= k < n:
                    pool.add(k)

    n = function_end(words, pool, resolve_call)
    words = words[:n]
    code = [i for i in range(n) if i not in pool]
    fallthrough = next_key is not None and code and words[code[-1]] == NOP and code[-1] == n - 1
    calls, mem, consts, flow, shape = [], [], Counter(), Counter(), []     # mem: (origin, L/S, width, offset)
    call_this = []                        # per call: offset of r0 from this (arg0), 'sp', or None
    lr_literal = None                     # value of a literal loaded into lr (return address trick)
    # register tracking: reg -> (origin, offset); origins are arg0..3, or a fresh id
    fresh = [0]

    def new():
        fresh[0] += 1
        return ('v', fresh[0])

    regs = {r: (f'arg{r}', 0) for r in range(4)}

    def get(r):
        if r not in regs:
            regs[r] = (new(), 0)
        return regs[r]

    def clobber_call(key):
        org0, off0 = regs.get(0, (None, 0))
        # r0 at the call: offset from this, 'sp' for a buffer on the stack, None if unknown
        call_this.append(off0 if org0 == 'arg0' else 'sp' if org0 == 'sp' else None)
        keep = None
        if key is not None and returns_this(key):
            keep = regs.get(0)
        elif structor and regs.get(0, (None,))[0] == 'arg0':
            keep = regs[0]
        for r in (0, 1, 2, 3, 12, LR):
            regs.pop(r, None)
        regs[0] = keep if keep else (new(), 0)

    dropped = Counter()                   # constants that were only arguments of the array helper

    def call_key(i, w):
        key = resolve_call(i, w)
        if key == ('rt', 'vec_ctor'):
            # (array, ctor, element size, count): compared as a call of the element's ctor, the
            # size and the count are not compared (GCC writes the loop, or the stores, itself)
            args = [_arg_feature(words, pool, i, r, resolve_word) for r in (1, 2, 3)]
            for f in args:
                if f is not None:
                    dropped[f] += 1
            ctor = args[0]
            if ctor is not None and ctor[0] == 'k':
                key = resolve_target(ctor[1])
            elif ctor is not None and ctor[0] == 'fn':
                key = ctor
        return key

    flags_set = False                     # a compare / flag setting op waits for its first use
    seen_literals = set()
    # registers at forward branches: code after an unconditional exit is reached by a branch,
    # with the registers of that branch (not those left by the code just before)
    branch_regs = {}
    prev = None
    restore = None                        # registers before an instruction for the branch path only
    for i, w in enumerate(words):
        if i in pool:
            continue
        if restore is not None:
            regs.clear()
            regs.update(restore)
            restore = None
        if prev is not None and i in branch_regs and prev >> 28 == COND_AL and \
                (_is_exit(prev, False) or (prev >> 25) & 7 == 0b101 and not prev & (1 << 24) or
                 (prev & 0xFFFFFFF0) == 0xE12FFF10):
            regs.clear()
            regs.update(branch_regs[i])
        prev = w
        cond = w >> 28
        # "ldreq r5, =result ; beq exit": the register is set for the branch path only, the code
        # that falls through still has the old value
        if cond not in (COND_AL, 0xF):
            j = i + 1
            while j < n and j in pool:
                j += 1
            if j < n and words[j] >> 28 == cond and (words[j] & 0x0F000000) == 0x0A000000:
                restore = dict(regs)
        if cond not in (COND_AL, 0xF) and flags_set:
            # one decision, whether the compiler branches or uses conditional execution
            flow['decision'] += 1
            flags_set = False
        if cond == 0xF:
            if (w & 0xFE000000) == 0xFA000000:                            # blx imm
                key = call_key(i, w)
                calls.append(key if key is not None else ('?',))
                shape.append('bl')
                clobber_call(key)
            continue
        op3 = (w >> 25) & 7
        rd = (w >> 12) & 0xF
        rn = (w >> 16) & 0xF
        if op3 == 0b101:                                                  # b / bl
            off = _sext24(w & 0xFFFFFF) * 4 + 8
            target = i * 4 + off
            if w & (1 << 24):
                key = call_key(i, w)
                calls.append(key if key is not None else ('?',))
                shape.append('bl')
                clobber_call(key)
                continue
            key = resolve_call(i, w)                                      # None: branch inside
            if key is not None:                                           # tail call = call + return
                org0, off0 = regs.get(0, (None, 0))
                call_this.append(off0 if org0 == 'arg0' else None)
                calls.append(key)
                shape.append('bl')
                shape.append('ret')
                flow['return'] += 1
            else:
                shape.append('b')
                if target <= i * 4:
                    flow['loop'] += 1
                elif target // 4 < n:
                    branch_regs.setdefault(target // 4, dict(regs))
            continue
        if (w & 0x0FFFFFD0) == 0x012FFF10:                                # bx / blx reg
            if w & 0x20:
                calls.append(('indirect',))
                shape.append('bl')
                clobber_call(None)
            elif w & 0xF == LR:
                flow['return'] += 1
                shape.append('ret')
            elif lr_literal is not None:                                  # call rN, "return" into lr_literal
                call_this.extend([None, None])
                calls.append(('indirect',))
                calls.append(resolve_target(lr_literal))
                shape.append('bl')
                lr_literal = None
            else:
                flow['jump_reg'] += 1
            continue
        if (w & 0x0F000000) == 0x0F000000:                                # svc
            call_this.append(None)
            calls.append(('svc', w & 0xFFFFFF))
            shape.append('svc')
            continue
        if op3 == 0b100:                                                  # ldm / stm
            reglist = w & 0xFFFF
            if w & (1 << 20) and reglist & (1 << PC):
                flow['return'] += 1
                shape.append('ret')
            elif rn != SP and get(rn)[0] != 'sp':
                shape.append('ldm' if w & (1 << 20) else 'stm')
                org, base_off = get(rn)
                count = bin(reglist).count('1')
                up, pre = bool(w & (1 << 23)), bool(w & (1 << 24))
                first = (4 if pre else 0) if up else (-4 * count if pre else -4 * count + 4)
                for k in range(count):
                    mem.append((org, 'L' if w & (1 << 20) else 'S', 'w', base_off + first + 4 * k))
                if w & (1 << 21) and not (w & (1 << 20) and reglist & (1 << rn)):
                    regs[rn] = (org, base_off + (4 * count if up else -4 * count))
            if w & (1 << 20) and not reglist & (1 << PC):         # after a return the next code is
                for r in range(16):                                   # reached from elsewhere: keep the
                    if reglist >> r & 1:                              # registers as they were there
                        regs.pop(r, None)
            continue
        if op3 in (0b010, 0b011) and not (op3 == 0b011 and w & 0x10):     # ldr/str(b)
            load = bool(w & (1 << 20))
            width = 'b' if w & (1 << 22) else 'w'
            if rn == PC and load and op3 == 0b010:
                off = (w & 0xFFF) * (1 if w & (1 << 23) else -1)
                j = i + 2 + off // 4
                if rd == PC:
                    flow['jump_table'] += 1
                elif rd == LR and 0 <= j < n and any(
                        (words[k] & 0xFFFFFFF0) == 0xE12FFF10 and words[k] & 0xF != LR
                        for k in range(i + 1, min(n, i + 5)) if k not in pool):
                    lr_literal = words[j]                                 # "ldr lr, =f2 ; bx rX"
                elif 0 <= j < n:
                    key = resolve_word(j, words[j])
                    if key[0] == 'rconst' and read_const is not None:
                        regs[rd] = (('r', key[1]), 0)
                        shape.append('lit')
                        continue
                    if key[0] == 'gaddr':
                        # a global: accesses through rd are compared by absolute address; which
                        # address the literal holds (the variable or the start of the file's
                        # data, as ARMCC does) is the compiler's choice
                        if j not in seen_literals:
                            consts[('data', key[1])] += 1
                        seen_literals.add(j)
                        regs[rd] = (('g', key[1]), 0)
                        shape.append('lit')
                        continue
                    # "ldr r1, =0x04xxxxxx ; mov r0, #0 ; nop": a debug log call that armlink
                    # removed (the sources leave it out), its message id is no constant
                    nxt = [words[k] for k in range(i + 1, min(n, i + 6)) if k not in pool][:2]
                    removed_log = rd == 1 and key[0] == 'k' and 0x04000000 <= key[1] < 0x05000000 \
                        and nxt == [0xE3A00000, NOP]
                    if j not in seen_literals and not removed_log:   # reloading a literal is the compiler's choice
                        consts[key] += 1
                    seen_literals.add(j)
                regs[rd] = (new(), 0)
                shape.append('lit')
                continue
            if rd == PC and load:
                if rn == SP:
                    flow['return'] += 1
                    shape.append('ret')
                else:
                    flow['jump_table'] += 1
                continue
            _access(w, op3 == 0b010, rn, rd, load, width, False, regs, get, new, mem, shape, read_const, consts)
            continue
        if op3 == 0b000 and (w & 0x90) == 0x90 and (w >> 5) & 3:          # ldrh / ldrsb / ldrd ...
            load = bool(w & (1 << 20))
            sh = (w >> 5) & 3
            width = {(1, 1): 'h', (1, 2): 'sb', (1, 3): 'sh', (0, 1): 'h', (0, 2): 'd', (0, 3): 'd'}[(int(load), sh)]
            is_load = load or sh == 2                                     # ldrd has L=0, sh=2
            _access(w, bool(w & (1 << 22)), rn, rd, is_load, width, True, regs, get, new, mem, shape)
            continue
        if (w & 0x0E000E00) == 0x0C000A00 and (w & 0x01200000) == 0x01000000:  # vldr / vstr
            load = bool(w & (1 << 20))
            if rn == PC:
                off = (w & 0xFF) * 4 * (1 if w & (1 << 23) else -1)
                j = i + 2 + off // 4
                if load and not w & 0x100 and 0 <= j < n and words[j]:   # 0.0 is everywhere
                    consts[('k', words[j])] += 1
                shape.append('lit')
                continue
            if rn != SP and get(rn)[0] != 'sp':
                org, base_off = get(rn)
                off = (w & 0xFF) * 4 * (1 if w & (1 << 23) else -1)
                mem.append((org, 'L' if load else 'S', 'v64' if w & 0x100 else 'v32', base_off + off))
            shape.append('ld' if load else 'st')
            continue
        if (w & 0x0E000E00) == 0x0C000A00 and (w >> 21) & 0xD in (0x4, 0x5, 0x9):  # vldm / vstm ia, db!
            load = bool(w & (1 << 20))
            count = w & 0xFF                                              # words
            up, wb = bool(w & (1 << 23)), bool(w & (1 << 21))
            if rn != SP and get(rn)[0] != 'sp':
                org, base_off = get(rn)
                first = base_off if up else base_off - 4 * count
                for k in range(count):
                    mem.append((org, 'L' if load else 'S', 'w', first + 4 * k))
                if wb:
                    regs[rn] = (org, base_off + (4 * count if up else -4 * count))
            shape.append('ldm' if load else 'stm')
            continue
        if op3 in (0b000, 0b001) and not (op3 == 0b000 and (w & 0x90) == 0x90):  # data processing
            opc = (w >> 21) & 0xF
            imm = op3 == 0b001
            val = _ror(w & 0xFF, ((w >> 8) & 0xF) * 2) if imm else None
            rm = w & 0xF
            if opc in (8, 9, 10, 11):                                     # tst teq cmp cmn
                shape.append('cmp')
                flags_set = True
                if imm:
                    consts[('cmp', val if opc != 11 else (-val) & 0xFFFFFFFF)] += 1
                continue
            shape.append('dp')
            if w & (1 << 20):                                             # movs / subs ...
                flags_set = True
            if rd == PC:                                                  # addls pc,pc,r0,lsl #2 / mov pc,lr
                if opc == 13 and not imm and rm == LR:
                    flow['return'] += 1
                else:
                    flow['jump_table'] += 1
                continue
            if rd == SP:
                continue
            if rn == SP and opc in (2, 4):                                # add rX, sp, #n: a stack address
                regs[rd] = ('sp', 0)
                continue
            if opc == 13 and imm and val == 0 and cond != COND_AL and rd in regs:
                continue                                                  # moveq rX, #0: keep addne's pointer
            if opc == 13:                                                 # mov
                if imm:
                    if val:                                               # 0 is everywhere and compiler dependent
                        consts[('k', val)] += 1
                    regs[rd] = (new(), 0)
                elif (w >> 4) & 0xFF == 0:
                    regs[rd] = ('sp', 0) if rm == SP else get(rm)
                else:
                    regs[rd] = (new(), 0)
            elif opc == 15 and imm:                                       # mvn
                consts[('k', ~val & 0xFFFFFFFF)] += 1
                regs[rd] = (new(), 0)
            elif opc in (4, 2) and imm:                                   # add / sub imm
                org, o = get(rn)
                regs[rd] = (org, o + (val if opc == 4 else -val))
            elif opc == 4 and not imm and (w >> 4) & 0x9 != 0x9:            # add rd, rn, rm (lsl #n): &array[i]
                base = get(rn)
                if isinstance(base[0], tuple) and base[0][0] == 'g' or \
                        isinstance(base[0], str) and base[0].startswith('arg'):
                    # an array of a global or behind an argument: compared like its first element
                    # (a loop with a pointer that moves on is seen at its first element, too)
                    regs[rd] = base
                else:
                    regs[rd] = (('idx', fresh[0] + 1), 0)
                    fresh[0] += 1
            else:
                regs[rd] = (new(), 0)
            continue
        if (w & 0x0FF00FFF) == 0x01900F9F or (w & 0x0FF00FF0) == 0x01800F90:   # ldrex / strex
            load = bool(w & (1 << 20))
            org, base_off = get(rn)
            mem.append((org, 'L' if load else 'S', 'x', base_off))
            regs[rd] = (new(), 0)                                         # loaded value / strex status
            shape.append('atomic')
            continue
        if (w & 0x0FB00FF0) == 0x01000090:                                # swp
            shape.append('atomic')
            continue
        if (w & 0x0FFFFFFF) == 0x0EF1FA10:                                # vmrs APSR_nzcv, fpscr
            shape.append('cmp')
            flags_set = True
            continue
        shape.append('other')
        if (w & 0x0F000010) == 0x0E000000:                                # vfp data processing
            continue
        if rd != PC:
            regs.pop(rd, None)

    if fallthrough:
        call_this.append(None)
        calls.append(next_key)
        flow['return'] += 1
    # returns, early or not, are the compiler's business (and noreturn calls have none); a
    # backward branch without condition is block placement (ARMCC puts rare paths at the end and
    # jumps back, GCC tests loops at the top): the condition of a loop is a decision anyway
    flow.pop('return', None)
    flow.pop('loop', None)
    # how often a constant is materialized, and compares with 0 (cmp / subs / tst / lsrs), too
    consts.subtract(dropped)
    consts = +consts
    for k in list(consts):
        if k == ('cmp', 0):
            consts[('cmp0',)] += consts.pop(k)
        elif k[0] == 'k':
            consts[k] = 1
    return {'calls': calls, 'call_this': call_this, 'mem': _mem_features(mem),
            'mem_this': _mem_features([m for m in mem if m[0] == 'arg0']),
            'consts': consts, 'flow': flow, 'shape': shape}


def _mem_features(raw):
    """(origin, L/S, width, offset) -> Counter of (L/S, width, offset). Offsets from the arguments
    (this) stay as they are. A pointer the function computed itself can be addressed from a
    different point by another compiler (GCC: "start = paramCopy - 24" and then [paramCopy, #-24]):
    if such an origin has negative offsets, they are shifted so that the lowest is 0."""
    lowest = {}
    for org, _, _, off in raw:
        if isinstance(off, int) and not (isinstance(org, str) and org.startswith('arg')) \
                and not (isinstance(org, tuple) and org[0] == 'g'):
            lowest[org] = min(lowest.get(org, 0), off)
    out = Counter()
    for org, sign, width, off in raw:
        if width in ('v32', 'v64'):
            width = 'w' if width == 'v32' else 'd'    # a float in s0 or in r0 is the compiler's choice
        if width == 'sb':
            width = 'b'                    # ARMCC loads bool with ldrsb, GCC with ldrb
        if isinstance(org, tuple) and org[0] == 'g' and isinstance(off, int):
            out[(sign, width, ('g', org[1] + off))] = 1     # a global, by its address
            continue
        if isinstance(off, int) and lowest.get(org, 0) < 0:
            off -= lowest[org]
        if isinstance(org, tuple) and org[0] == 'idx' and isinstance(off, int):
            off = 'reg' if off == 0 else f'reg+0x{off:X}'
        # a set: whether a store sits in one merged block or in both branches is the compiler's choice
        out[(sign, width, off)] = 1
    return out


def _access(w, is_imm, rn, rd, load, width, misc, regs, get, new, mem, shape, read_const=None, consts=None):
    """record one ldr/str style access"""
    if misc:
        imm = ((w >> 8) & 0xF) << 4 | (w & 0xF) if is_imm else None
    else:
        imm = (w & 0xFFF) if is_imm else None
    if imm is not None and not w & (1 << 23):
        imm = -imm
    pre = bool(w & (1 << 24))
    wb = bool(w & (1 << 21)) or not pre
    sign = 'S' if not load else 'L'
    if rn == SP or get(rn)[0] == 'sp':
        shape.append('ld' if load else 'st')
        if load:
            regs.pop(rd, None)
        return
    org, base_off = get(rn)
    if isinstance(org, tuple) and org[0] == 'r' and load and imm is not None and width == 'w' and pre:
        value = read_const(org[1] + base_off + imm)
        if value:
            consts[('k', value)] += 1
        regs[rd] = (new(), 0)
        shape.append('ld')
        return
    if imm is None:
        if isinstance(org, tuple) and org[0] == 'g' or isinstance(org, str) and org.startswith('arg'):
            # array[i] of a global or behind an argument: like its first element (as a pointer
            # that moves through the array is seen)
            mem.append((org, sign, width, base_off))
        else:
            mem.append((org, sign, width, 'reg'))
    else:
        mem.append((org, sign, width, base_off + (imm if pre else 0)))
        if wb:
            regs[rn] = (org, base_off + imm)
    if load:
        regs[rd] = (new(), 0)
        if width == 'd':
            regs[rd + 1] = (new(), 0)
    shape.append('ld' if load else 'st')


def _multiset(a, b):
    a, b = Counter(a), Counter(b)
    if not a and not b:
        return None
    keys = set(a) | set(b)
    return sum(min(a[k], b[k]) for k in keys) / sum(max(a[k], b[k]) for k in keys)


def _s32(v):
    return v - (1 << 32) if v & 0x80000000 else v


def _near(a, b):
    """compare constants a compiler may rewrite into each other: x > 1 <-> x >= 2 <-> -x < -1"""
    a, b = _s32(a), _s32(b)
    return abs(abs(a) - abs(b)) <= 1


def _near_cmp(a, b):
    """pairs of leftover compare constants of both sides that are near each other; a compare
    with 0 (kept apart as ('cmp0',), see extract) can stand for 1 or -1 (x + 1 == 1, i < 1)"""
    a, b = Counter(a), Counter(b)
    left_a = [f for f, k in (a - b).items() for _ in range(k) if f[0] == 'cmp']
    left_b = [f for f, k in (b - a).items() for _ in range(k) if f[0] == 'cmp']
    zeros_a, zeros_b = a[('cmp0',)], b[('cmp0',)]
    pairs = []
    for fa in list(left_a):
        fb = next((x for x in left_b if _near(fa[1], x[1])), None)
        if fb is not None:
            pairs.append((fa, fb))
            left_a.remove(fa)
            left_b.remove(fb)
    # literal constants a little apart: the compiler folded an addition into the constant
    left_ka = [f for f, k in (a - b).items() for _ in range(k) if f[0] == 'k']
    left_kb = [f for f, k in (b - a).items() for _ in range(k) if f[0] == 'k']
    for fa in left_ka:
        fb = next((x for x in left_kb if 0 < abs(_s32(fa[1]) - _s32(x[1])) <= 0x100), None)
        if fb is not None:
            pairs.append((fa, fb))
            left_kb.remove(fb)
    for fa in left_a:
        if zeros_b and abs(_s32(fa[1])) <= 1:
            zeros_b -= 1
            pairs.append((fa, ('cmp0',)))
    for fb in left_b:
        if zeros_a and abs(_s32(fb[1])) <= 1:
            zeros_a -= 1
            pairs.append((('cmp0',), fb))
    return pairs


def ignore_dtor_vptr(mine, theirs):
    """A destructor of GCC stores the vptr of its class before the base destructors run; ARMCC
    leaves that out when nothing virtual is called. Such a store (+0 with a vtable address) is
    dropped when only one side has it. The same for the store of a base class's vptr when the
    base destructor is inlined (GCC keeps it after the body, ARMCC leaves it out): when both
    sides store the same vptr and one side has another vtable as well, that one is dropped."""
    def is_vtable(k):
        return k[0] == 'addr' or k[0] == 'name' and k[1].endswith('::vtable')

    for x, y in ((mine, theirs), (theirs, mine)):
        vptrs = [k for k in x['consts'] if is_vtable(k) and not y['consts'][k]]
        if not vptrs or not x['mem'][('S', 'w', 0)]:
            continue
        if not y['mem'][('S', 'w', 0)]:
            del x['mem'][('S', 'w', 0)]
            for k in vptrs:
                del x['consts'][k]
        elif any(is_vtable(k) and x['consts'][k] for k in y['consts']):
            for k in vptrs:
                del x['consts'][k]


def _data_literals(a, b):
    """('data', w): a literal in the data range. If the other side has the constant w instead, it
    was a constant after all (IPC descriptors like 0x00A00002 fall into .bss); else only the fact
    that a global is addressed counts, not which address the literal holds"""
    a, b = Counter(a), Counter(b)
    for x, y in ((a, b), (b, a)):
        for f in [f for f in x if f[0] == 'data' and len(f) == 2]:
            n = x.pop(f)
            k = min(n, y[('k', f[1])])
            if k:
                x[('k', f[1])] += k
            if n - k:
                x[('data',)] += n - k
    # how many literals address globals is the compiler's choice (ARMCC: one base for the
    # variables of a file, GCC: one per variable); the accesses are compared by address in mem
    for x in (a, b):
        if x[('data',)] > 1:
            x[('data',)] = 1
    return a, b


def _cmp_literal(a, b):
    """'cmp K' on one side and a literal K on the other are the same constant: whether it fits
    into the compare depends on the compiler (ARMCC compares a float with 1.0f as an integer)"""
    a, b = Counter(a), Counter(b)
    for x, y in ((a, b), (b, a)):
        for f in list(x):
            if f[0] == 'cmp' and x[f] > y[f] and y[('k', f[1])] > x[('k', f[1])]:
                x[f] -= 1
                x[('k', f[1])] += 1
    return +a, +b


def _range_checks(a, b):
    """x < lo || x > hi is compiled as one compare (x - lo) > (hi - lo) by GCC: a 'cmp v' on one
    side and 'cmp lo' + 'cmp hi' on the other with hi - lo = v (+-1 for < / <=) are the same
    check"""
    a, b = Counter(a), Counter(b)
    for x, y in ((a, b), (b, a)):
        for f in [f for f in x if f[0] == 'cmp']:
            if x[f] <= y[f]:
                continue
            v = f[1]
            left = [g[1] for g in y if g[0] == 'cmp' and y[g] > x[g]]
            pair = next(((lo, hi) for lo in left for hi in left
                         if hi > lo and abs((hi - lo) - v) <= 1), None)
            if pair:
                lo, hi = pair
                y[('cmp', lo)] -= 1
                y[('cmp', hi)] -= 1
                y[f] += 1
    return +a, +b


def _halfword_consts(a, b):
    """A value for a 16 bit store only needs its low half: GCC makes 0x7FFF as mvn #0x8000
    (0xFFFF7FFF) where ARMCC loads 0x7FFF. Such a pair counts as the same constant."""
    a, b = Counter(a), Counter(b)
    for x, y in ((a, b), (b, a)):
        for f in list(x - y):
            if f[0] == 'k' and f[1] >= 0xFFFF0000:
                low = ('k', f[1] & 0xFFFF)
                if (y - x)[low] > 0:
                    x[f] -= 1
                    x[low] += 1
    return +a, +b


def _consts(a, b):
    a, b = _range_checks(*_cmp_literal(*_data_literals(*_halfword_consts(a, b))))
    pairs = _near_cmp(a, b)
    # compares with 0 only count when they stand in for another compare
    a.pop(('cmp0',), None)
    b.pop(('cmp0',), None)
    zero_pairs = sum(1 for fa, fb in pairs if ('cmp0',) in (fa, fb))
    if not a and not b:
        return None
    keys = set(a) | set(b)
    same = sum(min(a[k], b[k]) for k in keys)
    total = sum(max(a[k], b[k]) for k in keys)
    near = len(pairs) - zero_pairs
    # a near pair counts half and occupies one slot instead of two; a pair with a compare with 0
    # counts half of its one slot
    return (same + 0.5 * near + 0.5 * zero_pairs) / (total - near)


# one wide access = these narrow ones (strh = 2 x strb, str = 2 x strh or 4 x strb,
# strd / ldrd = 2 x str)
_PARTS = {'h': [('b', 2, 1)], 'w': [('h', 2, 2), ('b', 4, 1)], 'd': [('w', 2, 4)]}


def _split_wide(a, b):
    """Compilers merge or split neighbouring accesses (GCC: two strb of constants -> one strh;
    ARMCC: two str -> strd). Where one side has a wide access and the other side has exactly
    the narrow ones instead, count the wide one as those."""
    a, b = Counter(a), Counter(b)
    for _ in range(3):                                   # d -> w -> h -> b
        changed = False
        for x, y in ((a, b), (b, a)):
            for f in list(x):
                sign, width, off = f
                is_global = isinstance(off, tuple) and off[0] == 'g'
                if width not in _PARTS or not (isinstance(off, int) or is_global) or x[f] <= y[f] or x[f] == 0:
                    continue
                for part, count, size in _PARTS[width]:
                    if is_global:
                        pieces = [(sign, part, ('g', off[1] + k * size)) for k in range(count)]
                    else:
                        pieces = [(sign, part, off + k * size) for k in range(count)]
                    if all(y[p] for p in pieces):
                        x[f] -= 1
                        for p in pieces:
                            x[p] = max(x[p], 1)
                        changed = True
                        break
            x += Counter()                               # drop zero counts
        if not changed:
            break
    return _forwarded(+a, +b)


_WIDTH = {'b': 1, 'h': 2, 'sh': 2, 'w': 4, 'd': 8}


def _span(f):
    """(base, first byte, end) of an access with a known offset, else None"""
    sign, width, off = f
    if width not in _WIDTH:
        return None
    if isinstance(off, int):
        return None, off, off + _WIDTH[width]
    if isinstance(off, tuple) and off[0] == 'g':
        return 'g', off[1], off[1] + _WIDTH[width]
    return None


def _forwarded(a, b):
    """Accesses on one side only whose bytes a store of both sides covers: a load there takes
    the stored value (ARMCC reloads a member after a store to another one, GCC forwards it), a
    narrower store there was merged into the wider one by the other compiler (ARMCC: memcpy, then
    XOR of the halfwords in memory; GCC: XOR in registers, one store). They are left out."""
    a, b = Counter(a), Counter(b)
    common = [f for f in a if f[0] == 'S' and b[f] and _span(f)]
    for x, y in ((a, b), (b, a)):
        for f in list(x):
            if y[f] or not _span(f):
                continue
            base, start, end = _span(f)
            for g in common:
                gbase, gstart, gend = _span(g)
                if g != f and gbase == base and gstart <= start and end <= gend and \
                        (f[0] == 'L' or _WIDTH[g[1]] > _WIDTH[f[1]]):
                    del x[f]
                    break
    return a, b


def _merge_repeated_calls(a, b):
    """runs of the same call (a call per return path) count once when the two sides call it a
    different number of times"""
    ca, cb = Counter(a), Counter(b)
    differ = {k for k in ca if cb[k] and ca[k] != cb[k]}

    def merge(calls):
        out = []
        for k in calls:
            if out and k == out[-1] and k in differ:
                continue
            out.append(k)
        return out
    return merge(a), merge(b)


def compare(mine, theirs):
    """-> (score, {category: similarity or None})"""
    cats = {}
    if mine['calls'] or theirs['calls']:
        # order matters: sequence ratio, but never better than the multiset overlap
        calls_a, calls_b = _merge_repeated_calls(mine['calls'], theirs['calls'])
        seq = difflib.SequenceMatcher(None, calls_a, calls_b, autojunk=False).ratio()
        cats['calls'] = min(seq, _multiset(calls_a, calls_b))
    else:
        cats['calls'] = None
    cats['mem'] = _multiset(*_split_wide(mine['mem'], theirs['mem']))
    cats['consts'] = _consts(mine['consts'], theirs['consts'])
    cats['flow'] = _multiset(mine['flow'], theirs['flow'])
    if mine['shape'] or theirs['shape']:
        cats['shape'] = difflib.SequenceMatcher(None, mine['shape'], theirs['shape'], autojunk=False).ratio()
    else:
        cats['shape'] = None
    scored = {k: v for k, v in cats.items() if k in WEIGHTS and v is not None}
    total = sum(WEIGHTS[k] for k in scored)
    if total:
        score = sum(WEIGHTS[k] * v for k, v in scored.items()) / total
    else:
        score = cats['shape'] if cats['shape'] is not None else 1.0
    return score, cats


def grade(score):
    return 'equivalent' if score >= 0.97 else 'close' if score >= 0.85 else 'far'


def fmt_feature(f):
    if isinstance(f, tuple) and len(f) == 3 and f[0] in ('L', 'S'):
        off = f[2]
        if isinstance(off, tuple):
            where = f'global 0x{off[1]:08X}'
        else:
            where = off if isinstance(off, str) else (f'+0x{off:X}' if off >= 0 else f'-0x{-off:X}')
        return f'{"load" if f[0] == "L" else "store"} {f[1]} {where}'
    if isinstance(f, tuple) and len(f) == 2 and isinstance(f[1], int):
        return f'{f[0]} 0x{f[1]:X}'
    if isinstance(f, tuple) and len(f) == 2:
        return f'{f[0]} {f[1]!r}' if f[0] == 'str' else f'{f[0]} {f[1]}'
    return str(f)


def explain(mine, theirs):
    """lines describing what differs, per category"""
    lines = []
    for name in mine.get('inlined', []):
        lines.append(f'  inlined in the original (compared with its body): {name}')
    for name in theirs.get('inlined_by_you', []):
        lines.append(f'  inlined in yours, called in the original (compared with its body): {name}')
    for cat in ('calls', 'mem', 'consts', 'flow'):
        a, b = Counter(mine[cat]), Counter(theirs[cat])
        if cat == 'mem':
            a, b = _split_wide(a, b)
        missing = b - a
        extra = a - b
        if cat == 'consts':
            a, b = _range_checks(*_cmp_literal(*_data_literals(a, b)))
            missing, extra = b - a, a - b
            for fa, fb in _near_cmp(a, b):
                extra[fa] -= 1
                missing[fb] -= 1
                lines.append(f'  consts: near  yours {fmt_feature(fa)} ~ original {fmt_feature(fb)}'
                             ' (often the same value rewritten by the compiler, check it)')
            extra.pop(('cmp0',), None)
            missing.pop(('cmp0',), None)
            missing, extra = +missing, +extra
        if not missing and not extra:
            if cat == 'calls' and mine['calls'] != theirs['calls']:
                lines.append('  calls: same targets, different order')
                lines.append('    original: ' + ', '.join(fmt_feature(c) for c in theirs['calls']))
                lines.append('    yours:    ' + ', '.join(fmt_feature(c) for c in mine['calls']))
            continue
        lines.append(f'  {cat}:')
        for f, k in sorted(missing.items(), key=lambda x: str(x[0])):
            lines.append(f'    missing  {fmt_feature(f)}' + (f'  x{k}' if k > 1 else ''))
        for f, k in sorted(extra.items(), key=lambda x: str(x[0])):
            lines.append(f'    extra    {fmt_feature(f)}' + (f'  x{k}' if k > 1 else ''))
    return lines
