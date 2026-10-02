"""ACNL RTTI analysis: names virtual functions, ctors, dtors and singletons.

usage: python analyze.py <elf> <libgarden symbols...> [--ref <matches.tsv>]... --out <dir>
                         [--json config/<version>/symbols.json]

symbols.json is maintained by hand now (see README.md, "Regenerating"); a new run would
overwrite it.
"""
import re, sys, os, argparse
from collections import defaultdict
from elfmem import Mem, demangle
import rtti, xrefs

OP_DELETE = 0x2f8958   # operator delete(void*) (libgarden symbols_us_std)
SYM_RE = re.compile(r'^\s*([A-Za-z_][\w.$]*)\s*=\s*(0x[0-9a-fA-F]+)\s*;')


def load_symbols(paths):
    raw = []
    for p in paths:
        for line in open(p, encoding='utf-8', errors='replace'):
            mm = SYM_RE.match(line)
            if mm:
                raw.append((mm.group(1), int(mm.group(2), 16)))
    dem = demangle([n for n, _ in raw])
    known = defaultdict(list)
    for (n, a), d in zip(raw, dem):
        known[a].append((n, d.strip() or n, 'libgarden'))
    return known


STRONG_SINGLE = ('bytes', 'bytes-fuzzy')


def name_tier(lst):
    """lst = known[addr] = [(mangled, name, source), ...] in priority order.
    returns (tier, note):
      A  libgarden / >= 2 independent sources agree / manual descriptive names
      B  one source, exact or fuzzy byte match
      C  one source, derived from calls only (callgraph, callseq, ...)
      X  sources contradict each other (never exported to Ghidra)"""
    from validate_refs import same_name
    family = lambda e: e[2].split(':')[0].split(' ')[0]
    first = lst[0]
    agree = {family(e) for e in lst if same_name(e[1], first[1])}
    other = [f'{e[2]}: {e[1][:70]}' for e in lst[1:]
             if family(e) != family(first) and not same_name(e[1], first[1])]
    note = first[2]
    if other:
        return 'X', note + ' [CONFLICT with ' + '; '.join(dict.fromkeys(other)) + ']'
    src = first[2]
    if src.startswith('manual:ref-verified'):
        return 'A', note + ' [original name, verified against reference game code]'
    if src.startswith('manual:inferred'):
        return 'B', note + ' [original name inferred from library conventions - not directly verified]'
    if src.startswith('manual:3dbrew'):
        return 'B', note + ' [IPC command name after 3dbrew, class by the IPC session]'
    if src.startswith('manual'):
        return 'A', note + ' [descriptive name, not an original symbol]'
    if src == 'libgarden' or len(agree) >= 2:
        if len(agree) >= 2:
            note += ' [confirmed by ' + ', '.join(sorted(agree - {family(first)})) + ']'
        return 'A', note
    method = src.split(':')[1].split(' ')[0] if ':' in src else ''
    if method in STRONG_SINGLE or src.startswith('cro-vtable'):
        return 'B', note
    return 'C', note


def load_ref_matches(paths, known):
    """match TSV (config/<version>/inputs): addr, demangled name, source, ref addr (appended
    after libgarden names, so libgarden keeps priority).
    returns {target addr: reference addr}"""
    ref_of = {}
    for p in paths:
        for line in open(p, encoding='utf-8'):
            if line.startswith('#'):
                continue
            cols = line.rstrip('\n').split('\t')
            a = int(cols[0], 16)
            ra = int(cols[3], 16)
            if ra and a not in ref_of and a not in known:
                ref_of[a] = ra
            if not any(e[2] == 'libgarden' for e in known.get(a, [])):
                known[a].append(('', cols[1], cols[2]))
    return ref_of


def split_qual(d):
    """'a::B<x::y>::f(int) const' -> ('a::B<x::y>', 'f', '(int) const')"""
    depth, i = 0, 0
    while i < len(d):
        c = d[i]
        if c == '<': depth += 1
        elif c == '>': depth -= 1
        elif c == '(' and depth == 0 and not d[:i].endswith('operator'):
            break
        i += 1
    q, params = d[:i], d[i:]
    parts, depth, last = [], 0, 0
    j = 0
    while j < len(q):
        if q[j] == '<': depth += 1
        elif q[j] == '>': depth -= 1
        elif q.startswith('::', j) and depth == 0:
            parts.append(q[last:j]); last = j + 2; j += 1
        j += 1
    parts.append(q[last:])
    return '::'.join(parts[:-1]), parts[-1], params


def short(name):
    return split_qual(name + '()')[1] if '::' in name else name


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('elf')
    ap.add_argument('symbols', nargs='*')
    ap.add_argument('--out', default='out')
    ap.add_argument('--ref', action='append', default=[],
                    help='name matches (TSV, config/<version>/inputs/*_matches.tsv), may be repeated')
    ap.add_argument('--json', help='write a machine readable database (classes, vtables, functions)')
    args = ap.parse_args()
    os.makedirs(args.out, exist_ok=True)

    m = Mem(args.elf)
    widx = rtti.build_word_index(m)
    tis = rtti.parse_typeinfos(m, widx, rtti.find_cxxabi_vptrs(m, widx))
    vts = rtti.parse_vtables(m, widx, tis)
    x = xrefs.Xrefs(m, {f for v in vts for f in v.funcs})
    known = load_symbols(args.symbols)
    ref_of = load_ref_matches(args.ref, known)
    print(f'typeinfos={len(tis)} vtables={len(vts)} funcs~{len(x.starts)} known={len(known)}')

    # ---- slot identity: (introducing class, slot index) ----
    def chain(t, vt):
        start = t
        if vt.offset_to_top != 0:
            start = None
            for b, off, virt, pub in t.bases:
                if off == -vt.offset_to_top and not virt and b in tis:
                    start = tis[b]
            if start is None:
                return [t]
        out = [start]
        while out[-1].primary_base in tis:
            out.append(tis[out[-1].primary_base])
        return out

    def intro(ch, i):
        for c in reversed(ch):
            pv = rtti.primary_vtable(c)
            if pv and len(pv.funcs) > i:
                return c
        return ch[0]

    def owner(ch, i, f):
        own = ch[0]
        for c in ch:
            pv = rtti.primary_vtable(c)
            if pv and len(pv.funcs) > i and pv.funcs[i] == f:
                own = c
        return own

    slots = []   # (t, vt, i, f, intro, owner)
    for t in tis.values():
        for vt in t.vtables:
            ch = chain(t, vt)
            for i, f in enumerate(vt.funcs):
                slots.append((t, vt, i, f, intro(ch, i), owner(ch, i, f)))

    # ---- method names per slot identity ----
    mnames = {}
    for t, vt, i, f, it, ow in slots:
        # only reliable names may be propagated to other classes
        if f in known and name_tier(known[f])[0] in 'AB':
            _, meth, params = split_qual(known[f][0][1])
            mnames.setdefault((it.addr, i), meth + params)

    # destructor pairs: slot i+1 is a short stub that calls slot i and then
    # branches/calls into an operator delete
    def branch_targets(f, n=12):
        out = []
        for k in range(n):
            ins = m.u32(f + k * 4)
            if (ins & 0x0E000000) == 0x0A000000 and (ins >> 28) == 0xE:
                off = ins & 0xFFFFFF
                off = off - 0x1000000 if off & 0x800000 else off
                out.append((bool(ins & 0x01000000), f + k * 4 + 8 + off * 4))
                if not ins & 0x01000000:      # plain B = tail call, end
                    break
            elif xrefs.is_uncond_return(ins):
                break
        return out

    deleters = defaultdict(int)
    dtor_found = 0
    for t in tis.values():
        pv = rtti.primary_vtable(t)
        if not pv:
            continue
        fs = pv.funcs
        for i in range(len(fs) - 1):
            bt = branch_targets(fs[i + 1])
            tg = [a for _, a in bt]
            if fs[i] != fs[i + 1] and fs[i] in tg and any(a != fs[i] for a in tg[tg.index(fs[i]) + 1:]):
                for a in tg[tg.index(fs[i]) + 1:]:
                    deleters[a] += 1
                ch = chain(t, pv)
                it = intro(ch, i)
                mnames.setdefault((it.addr, i), '~dtor()')
                mnames.setdefault((it.addr, i + 1), '~dtor() [deleting]')
                dtor_found += 1
                break
    print(f'destructor pairs detected: {dtor_found}')
    print('  operator delete candidates:', ', '.join(f'0x{a:X}x{n}' for a, n in
                                                  sorted(deleters.items(), key=lambda kv: -kv[1])[:6]))

    # ---- label every function ----
    labels = defaultdict(set)     # addr -> {(prio, label, note)}
    owners = defaultdict(set)

    def known_note(lst):
        tier, note = name_tier(lst)
        return f'{note} [tier {tier}]'

    tiers = defaultdict(int)
    for f, lst in known.items():
        if m.in_text(f):
            note = known_note(lst)
            tiers[name_tier(lst)[0]] += 1
            labels[f].add((0, lst[0][1], note))
    print('name tiers (A confirmed, B strong single source, C weak single source, X conflict):',
          dict(sorted(tiers.items())))
    for t, vt, i, f, it, ow in slots:
        owners[f].add(ow.name)
    for t, vt, i, f, it, ow in slots:
        off = f'vf_0x{i * 4:02X}'
        if f in known:
            labels[f].add((0, known[f][0][1], known_note(known[f])))
            continue
        meth = mnames.get((it.addr, i))
        secondary = vt.offset_to_top != 0
        if meth:
            if meth.startswith('~'):
                rest = meth[meth.index('('):]
                nm = '~' + short(ow.name) + rest
            else:
                nm = meth
            lab = f'{ow.name}::{nm}'
            note = f'slot {off} of {it.name}'
            prio = 1
        else:
            lab = f'{ow.name}::{off}'
            note = f'virtual slot, introduced by {it.name}'
            prio = 2
        if secondary:
            note += f' (secondary vtable, this-adjust {vt.offset_to_top})'
            prio += 1
        if len(owners[f]) > 2:
            note += f' [shared by {len(owners[f])} classes - likely stub/ICF]'
            prio += 2
        labels[f].add((prio, lab, note))

    # ---- constructors: functions that load a vtable address point ----
    in_vtable = {f for v in vts for f in v.funcs}
    anc_cache = {}

    def ancestors(t):
        if t.addr not in anc_cache:
            s = set()
            for b, *_ in t.bases:
                if b in tis:
                    s.add(b)
                    s |= ancestors(tis[b])
            anc_cache[t.addr] = s
        return anc_cache[t.addr]

    loads = defaultdict(set)      # function -> typeinfos whose vptr it loads
    for vt in vts:
        for site in x.lit_loads.get(vt.addrpoint, []):
            loads[x.func_start(site)].add(vt.ti.addr)
    ctors = defaultdict(set)
    for f, ts in loads.items():
        if f in in_vtable:
            continue
        # leaves = classes in the set that are no base of another one in the set;
        # a leaf whose bases are also loaded here is the class being constructed
        # (its members' vtables show up as unrelated leaves)
        leaves = [t for t in ts if not any(t in ancestors(tis[o]) for o in ts if o != t)]
        strong = [t for t in leaves if ancestors(tis[t]) & ts]
        pick = strong if len(strong) == 1 else leaves if len(leaves) == 1 else None
        if pick:
            ctors[pick[0]].add(f)
        else:
            names = ', '.join(sorted(tis[t].name for t in leaves)[:4])
            labels[f].add((6, f'sub_{f:06X}', f'ctor of one of / builds inline: {names}'))
    for ta, fs in ctors.items():
        t = tis[ta]
        for f in fs:
            note = 'stores vtable ptr' + (' (one of several candidates)' if len(fs) > 1 else '')
            labels[f].add((1 if len(fs) == 1 else 3, f'{t.name}::{short(t.name)}() [ctor?]', note))

    # ---- singletons via sead SingletonDisposer_ ----
    singletons = {}
    for t in tis.values():
        if not t.name.endswith('::SingletonDisposer_') or not t.vtables:
            continue
        outer = t.name[:-len('::SingletonDisposer_')]
        vals = []
        for f in t.vtables[0].funcs:
            for _, v in x.loaded_values(f):
                if m.in_data(v) and v not in vals:
                    vals.append(v)
        singletons[outer] = vals

    # ---- validation against libgarden ----
    kf = {a for a in known if m.in_text(a) and known[a][0][2] == 'libgarden'}
    hit = kf & set(labels)
    print(f'libgarden text symbols: {len(kf)}, reached via vtables/ctors: {len(hit)}')
    ok = bad = 0
    for ta, fs in ctors.items():
        for f in fs:
            for d in [e[1] for e in known.get(f, []) if e[2] == 'libgarden']:
                c, meth, _ = split_qual(d)
                if short(c) == meth:
                    ok += 1
                else:
                    bad += 1
                    print('  ctor mismatch', hex(f), tis[ta].name, '<->', d)
    print(f'ctor candidates confirmed by libgarden: {ok}, contradicted: {bad}')
    sok = 0
    for outer, vals in singletons.items():
        for v in vals:
            for d in [e[1] for e in known.get(v, []) if e[2] == 'libgarden']:
                if 'Instance' in d:
                    sok += 1
                    print(f'  singleton ok: {outer} -> {hex(v)} {d}')
    print(f'singletons found: {len(singletons)}, confirmed: {sok}')

    write_outputs(args.out, m, tis, labels, singletons, known, ctors)
    if args.json:
        write_json(args.json, m, tis, slots, labels, singletons, known, ctors, x)


def write_json(path, m, tis, slots, labels, singletons, known, ctors, x):
    """Machine readable database for code generation / tooling."""
    import json

    def tier_of(note):
        mm = re.search(r'\[tier (\w)\]', note)
        return mm.group(1) if mm else None

    slot_info = {}
    for t, vt, i, f, it, ow in slots:
        slot_info[(t.addr, vt.addr, i)] = (it.name, ow.name)
    classes = []
    for t in sorted(tis.values(), key=lambda t: t.name):
        vtabs = []
        for vt in t.vtables:
            ents = []
            for i, f in enumerate(vt.funcs):
                it, ow = slot_info.get((t.addr, vt.addr, i), (None, None))
                p, lab, note = best(labels[f]) if f in labels else (9, None, '')
                ents.append({'slot': i, 'addr': f, 'label': lab, 'note': note,
                             'tier': tier_of(note), 'owner': ow, 'intro': it,
                             'shared': 'shared by' in note})
            vtabs.append({'addr': vt.addr, 'vptr': vt.addrpoint,
                          'offset_to_top': vt.offset_to_top, 'entries': ents})
        sing = singletons.get(t.name)
        classes.append({
            'name': t.name, 'mangled': t.mangled, 'typeinfo': t.addr, 'kind': t.kind,
            'bases': [{'name': tis[b].name if b in tis else None, 'typeinfo': b, 'offset': o,
                       'virtual': v, 'public': p} for b, o, v, p in t.bases],
            'vtables': vtabs,
            'ctor_candidates': sorted(ctors.get(t.addr, ())),
            'singleton_instance': sing[0] if sing else None,
        })
    funcs = []
    for f in sorted(labels):
        ls = sorted(labels[f])
        p, lab, note = ls[0]
        src = known[f][0][2] if f in known else note.split(' [')[0].split('   ')[0]
        funcs.append({'addr': f, 'name': lab, 'prio': p, 'tier': tier_of(note), 'source': src,
                      'note': note, 'alternatives': [l for _, l, _ in ls[1:6]]})
    starts = sorted(set(x.starts) | set(labels))
    db = {'binary': {'text': list(m.text), 'ro': list(m.ro), 'data': list(m.data)},
          'classes': classes, 'functions': funcs, 'function_starts': starts}
    with open(path, 'w', encoding='utf-8') as fp:
        json.dump(db, fp)
    print(f'json database: {len(classes)} classes, {len(funcs)} named functions -> {path}')


def best(labset):
    return sorted(labset)[0]


def write_outputs(out, m, tis, labels, singletons, known, ctors):
    # 1) flat function list
    with open(os.path.join(out, 'functions.txt'), 'w') as fp:
        fp.write('# address   name   -- note   (+ alternative names)\n')
        for f in sorted(labels):
            ls = sorted(labels[f])
            p, lab, note = ls[0]
            alt = '' if len(ls) == 1 else '   | also: ' + '; '.join(l for _, l, _ in ls[1:6]) + (' ...' if len(ls) > 6 else '')
            fp.write(f'0x{f:06X}  {lab}   -- {note}{alt}\n')

    # 2) per-class listing
    with open(os.path.join(out, 'classes.txt'), 'w') as fp:
        for t in sorted(tis.values(), key=lambda t: t.name):
            bases = ', '.join(
                (('virtual ' if v else '') + (tis[b].name if b in tis else hex(b)) + (f' @+0x{o:X}' if o else ''))
                for b, o, v, p in t.bases)
            fp.write(f'class {t.name}' + (f' : {bases}' if bases else '') + '\n')
            fp.write(f'  typeinfo 0x{t.addr:06X} ({t.mangled})\n')
            for f in sorted(ctors.get(t.addr, ())):
                fp.write(f'  ctor?    0x{f:06X}\n')
            for vt in t.vtables:
                fp.write(f'  vtable   0x{vt.addr:06X} (vptr value 0x{vt.addrpoint:06X}, offset_to_top {vt.offset_to_top}, {len(vt.funcs)} entries)\n')
                for i, f in enumerate(vt.funcs):
                    lab = best(labels[f])[1] if f in labels else '?'
                    fp.write(f'    [0x{i * 4:02X}] 0x{f:06X}  {lab}\n')
            if t.name in singletons:
                fp.write('  singleton instance candidates: ' + ', '.join(f'0x{v:06X}' for v in singletons[t.name]) + '\n')
            fp.write('\n')

    # 3) singletons
    with open(os.path.join(out, 'singletons.txt'), 'w') as fp:
        fp.write('# class  -> data addresses referenced by its SingletonDisposer_ dtor\n'
                 '# (usually sInstance first; a second one is the static disposer)\n')
        for k in sorted(singletons):
            v = singletons[k]
            fp.write(f'{k:50s} ' + ', '.join(f'0x{x:06X}' for x in v) + '\n')

    # 4) Ghidra ImportSymbolsScript.py format: "<name> <addr> f|l"
    def san(s):
        s = re.sub(r'\s*\[.*?\]', '', s)
        return re.sub(r'[^\w:~<>,*&]', '_', s.replace(' ', ''))

    OPS = {'()': 'call', '[]': 'index', '==': 'eq', '!=': 'ne', '=': 'assign',
           '<': 'lt', '>': 'gt', '<=': 'le', '>=': 'ge', '+': 'add', '-': 'sub',
           '*': 'mul', '/': 'div', '+=': 'add_assign', '-=': 'sub_assign',
           '++': 'inc', '--': 'dec', '!': 'not', '->': 'arrow', '<<': 'shl', '>>': 'shr'}

    def func_name(lab):
        """'ns::Cls::Meth(int, char) const [ctor?]' -> 'ns::Cls::Meth'"""
        deleting = '[deleting]' in lab
        base = re.sub(r'\s*\[.*?\]', '', lab).strip()
        # "(anonymous namespace)" would otherwise be taken for a parameter list
        base = base.replace('(anonymous namespace)', 'anonymous_namespace')
        if '(' in base:
            cls, meth, _ = split_qual(base)
            if meth.startswith('operator'):
                op = meth[len('operator'):].strip()
                meth = 'operator_' + OPS.get(op, op.replace(' ', '_'))
            base = f'{cls}::{meth}' if cls else meth
        return san(base) + ('_deleting' if deleting else '')

    with open(os.path.join(out, 'ghidra_symbols.txt'), 'w') as fp:
        skipped = 0
        for f in sorted(labels):
            # only names we can stand behind: no weak single-source names, no
            # conflicts, no constructor guesses; fall back to the next safe label
            for p, lab, note in sorted(labels[f]):
                if (p > 3 or '[tier C]' in note or '[tier X]' in note or '[ctor?]' in lab
                        or 'shared by' in note):     # ICF stub used by many classes
                    continue
                # Thumb functions carry bit 0 in pointers; Ghidra places them at the even address
                fp.write(f'{func_name(lab)} 0x{f & ~1:08X} f\n')
                break
            else:
                skipped += 1
        print(f'ghidra export: {skipped} addresses left unnamed (weak / conflicting / ctor guess)')
        for t in tis.values():
            fp.write(f'typeinfo_for_{san(t.name)} 0x{t.addr:08X} l\n')
            for vt in t.vtables:
                sfx = '' if vt.offset_to_top == 0 else f'_off{-vt.offset_to_top}'
                fp.write(f'vtable_for_{san(t.name)}{sfx} 0x{vt.addr:08X} l\n')
        # s_pInstance = libgarden's naming; an address claimed by several
        # classes is not a per-class instance pointer -> left out
        from collections import Counter
        claims = Counter(v[0] for v in singletons.values() if v)
        for k, v in singletons.items():
            if v and claims[v[0]] == 1:
                fp.write(f'{san(k)}::s_pInstance 0x{v[0]:08X} l\n')
    print('written to', os.path.abspath(out))


if __name__ == '__main__':
    main()
