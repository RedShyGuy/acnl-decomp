"""The hand named symbols (config/<version>/inputs/manual_names.tsv).

Decodes hand named functions and globals (EXTRA, EXTRA_GLOBALS: sead heap
creation, nn::os helpers of the heap setup).

usage: python manual_names.py <ACNL elf> [out dir]
    out dir gets manual_names.tsv and manual_ghidra_symbols.txt
"""
import struct, sys
from elfmem import Mem

HELPERS = [0x100048, 0x10018c, 0x1002e8, 0x100214, 0x100448, 0x100528, 0x1003bc,
           0x1004dc, 0x1005c0, 0x100490, 0x100574, 0x1001d0, 0x1000c4, 0x10032c,
           0x100104, 0x1002a4, 0x100148, 0x100088, 0x100378, 0x100260, 0x100400]


def h16(m, a):
    return struct.unpack_from('<H', m.mem, a - m.base)[0]


def blx_target(m, a):
    h1, h2 = h16(m, a), h16(m, a + 2)
    s = (h1 >> 10) & 1
    j1, j2 = (h2 >> 13) & 1, (h2 >> 11) & 1
    i1, i2 = 1 - (j1 ^ s), 1 - (j2 ^ s)
    off = (s << 24) | (i1 << 23) | (i2 << 22) | ((h1 & 0x3FF) << 12) | (((h2 >> 1) & 0x3FF) << 2)
    if s:
        off -= 1 << 25
    exchange = not (h2 & 0x1000)          # BLX (to ARM) vs BL (stay Thumb)
    base = ((a + 4) & ~3) if exchange else a + 4
    return base + off


def decode(m, start):
    regs, stack, calls, stores = {}, {}, [], []
    a = start
    while a < start + 0x80:
        h = h16(m, a)
        if (h & 0xF800) == 0x2000:                       # movs rd, #imm8
            regs[(h >> 8) & 7] = h & 0xFF
        elif (h & 0xF800) == 0x0000 and h:               # lsls rd, rm, #imm5
            rm, rd, sh = (h >> 3) & 7, h & 7, (h >> 6) & 31
            if rm in regs and regs[rm] is not None:
                regs[rd] = (regs[rm] << sh) & 0xFFFFFFFF
        elif (h & 0xF800) == 0x4800:                     # ldr rd, [pc, #imm]
            lit = ((a + 4) & ~3) + (h & 0xFF) * 4
            regs[(h >> 8) & 7] = m.u32(lit)
        elif (h & 0xF800) == 0xA000:                     # adr rd, label
            regs[(h >> 8) & 7] = ('str', m.cstr(((a + 4) & ~3) + (h & 0xFF) * 4))
        elif (h & 0xFFC0) == 0x4600 or (h & 0xFF00) == 0x4600:   # mov rd, rm
            rd = (h & 7) | ((h >> 4) & 8)
            rm = (h >> 3) & 15
            regs[rd] = regs.get(rm, ('reg', rm))
        elif (h & 0xF800) == 0x9000:                     # str rt, [sp, #imm]
            stack[(h & 0xFF) * 4] = regs.get((h >> 8) & 7)
        elif (h & 0xF800) == 0x6000:                     # str rt, [rn, #imm]
            rt, rn, off = h & 7, (h >> 3) & 7, ((h >> 6) & 31) * 4
            if rt == 0 and isinstance(regs.get(rn), int):
                stores.append(regs[rn] + off)
        elif (h & 0xF800) == 0xF000:                     # bl / blx
            t = blx_target(m, a)
            calls.append((t, dict(regs), dict(stack)))
            regs[0] = ('ret', t)
            a += 4
            continue
        elif (h & 0xFF00) == 0xBD00:                     # pop {..., pc}
            break
        a += 2
    return calls, stores


CREATE_FN = {0x2F6D8C: 'ExpHeap', 0x2F6FC0: 'UnitHeap', 0x13351C: 'ExpHeap (root)'}

# functions around the helpers; verified by hand (see README / chat):
#  0x12CC60  calls HeapMgr::GetCurrentHeap if parent == 0, constructs sead::ExpHeap (0x133578 stores its vtable)
#  0x2F6D8C  forwards (size, name, parent, direction, flag) to 0x12CC60
#  0x2F6FC0  constructs sead::UnitHeap (stores its vtable)
#  0x11DE60  allocates the 0x3CD800 byte block via nn::os::AddressSpaceManager::Allocate
#  0x11EE4C  builds a SafeString from a C string and calls 0x2F6D8C (size, parent, name)
#  0x11EEC8  creates ExpHeap "SaveAllocator" (0xBB800) + ssys::ma::Allocator, stores it at 0x95CF94+8
EXTRA = [   # (address, name, info, source)
    (0x11D5C0, 'HeapInit::CreateAllHeaps', 'boot heap setup: memory block, NoDevice root heap, all named game heaps', 'manual:heapinit'),
    (0x11EEC8, 'HeapInit::CreateSaveAllocator', 'ExpHeap "SaveAllocator" (0xBB800) wrapped in a ssys::ma::Allocator', 'manual:heapinit'),
    # generic helpers (many callers all over the game), not heap-init specific
    (0x11EE4C, 'HeapUtil::CreateExpHeapWithCName', 'ACNL helper, 9 callers: (size, parent, const char* name) -> '
               'builds a SafeString and calls sead::ExpHeap::create', 'manual:heapinit'),
    (0x12CC60, 'sead::ExpHeap::tryCreate', 'same callee sequence as Nintendogs sead::ExpHeap::tryCreate(u32, const SafeString&, '
               'Heap*, HeapDirection, bool): getCurrentHeap -> ExpHeap ctor (stores sead::ExpHeap vtable) -> '
               'createMaxSizeFreeMemBlock_ -> IDisposer; newer code (opcode similarity 0.67)', 'manual:ref-verified'),
    (0x2F6FC0, 'sead::UnitHeap::tryCreateWithBlockNum', 'identical to Nintendogs sead::UnitHeap::tryCreateWithBlockNum '
               '(first 20 instructions equal, similarity 0.92, same callees)', 'manual:ref-verified'),
    (0x2F6D8C, 'sead::ExpHeap::create', '61 callers; forwards all 5 arguments unchanged to tryCreate = sead create() in a '
               'release build (debug assert removed); not present in any reference game', 'manual:inferred'),
    # descriptive names (ours) for the nn::os functions the heap setup uses
    (0x11DE60, 'nn::os::MemoryBlock::AllocateBlock', 'page aligns the size and allocates the block from the '
               'memory block space (result 0xD8601837 when there is no space)', 'manual:ours'),
    (0x34C4AC, 'nn::os::detail::StartAlarmThreadPool', 'creates the thread pool of the alarms once: 2 x 4096 byte stacks, '
               'a 0x1E0 byte memory block for the pool, then ThreadPool::Setup', 'manual:ours'),
    (0x34B84C, 'nn::os::ThreadPool::Setup', '(work buffer, wait tasks, threads, stacks, priority, priority); called '
               'from the inlined ThreadPool constructor in StartAlarmThreadPool', 'manual:ours'),
]
EXTRA_GLOBALS = [
    (0x00A1967C, 'HeapInit::g_NoDeviceMemoryBlock'),          # nn::os::MemoryBlock (0x3CD800 bytes)
    (0x0097F014, 'nn::os::detail::s_AlarmThreadPool'),        # ours
    (0x00AEB600, 'nn::os::detail::s_AlarmThreadPoolMemory'),  # ours
]
KNOWN_GLOBALS = {0x953C4C: 'StageCommon::s_pStageHeap'}   # libgarden


def main(elf, out_dir=None):
    m = Mem(elf)
    rows = []
    print(f'{"helper":10s} {"name":18s} {"create fn":10s} {"size":>10s}  args            global')
    for f in HELPERS:
        calls, stores = decode(m, f)
        name = next((r[1][1] for _, r, _ in calls if isinstance(r.get(1), tuple) and r[1][0] == 'str'), None)
        create = [(t, r, s) for t, r, s in calls if t not in (0x2F6D78, 0x2F6DA0)]
        main_call = next(((t, r, s) for t, r, s in create if isinstance(r.get(1), tuple)), create[-1] if create else None)
        if main_call:
            t, r, s = main_call
            size = r.get(0)
            size = f'{size:#x}' if isinstance(size, int) else ('runtime' if size else '?')
            args = f'r2={r.get(2) if not isinstance(r.get(2), tuple) else "param"} r3={r.get(3)} sp0={s.get(0)}'
            print(f'{f:#010x} {str(name):18s} {t:#010x} {size:>10s}  {args:15s} '
                  + ', '.join(f'{g:#x}' for g in stores))
            rows.append((f, name, CREATE_FN.get(t, hex(t)), size, stores[0] if stores else None))
    if not out_dir:
        return
    import os
    fn_rows, globals_ = [], []
    for f, name, kind, size, glob in rows:
        base = 'NoDeviceRootHeap' if name == 'NoDevice' else name
        fn_rows.append((f, f'HeapInit::Create{base}',
                        (f'creates {kind} "{name}" ({"block size" if kind == "UnitHeap" else "size"} {size}), '
                         f'stores it at {glob:#x}') if glob else '',
                        'manual:heapinit'))
        if glob:
            globals_.append((glob, KNOWN_GLOBALS.get(glob, f'HeapInit::g_p{base}')))
    fn_rows += EXTRA
    globals_ += EXTRA_GLOBALS
    with open(os.path.join(out_dir, 'manual_names.tsv'), 'w', encoding='utf-8') as fp:
        fp.write('# target_addr\tname\tsource\tref_addr\tinfo\n'
                 '# manual:heapinit / manual:ours = descriptive names, manual:ref-verified = names checked against '
                 'reference game code\n')
        for a, n, info, src in fn_rows:
            fp.write(f'0x{a:06X}\t{n}\t{src}\t0x0\t{info}\n')
    with open(os.path.join(out_dir, 'manual_ghidra_symbols.txt'), 'w', encoding='utf-8') as fp:
        for a, n, _, _ in fn_rows:
            fp.write(f'{n} 0x{a:08X} f\n')
        for a, n in globals_:
            fp.write(f'{n} 0x{a:08X} l\n')
    print(f'written {len(fn_rows)} functions, {len(globals_)} globals to {out_dir}')


if __name__ == '__main__':
    main(*sys.argv[1:3])
