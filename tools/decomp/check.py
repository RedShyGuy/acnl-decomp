#!/usr/bin/env python3
"""Compare compiled functions with the original code.elf.

    python tools/decomp/check.py [--build-dir build/gcc] [--version USA_1_5] [--show N]
    python tools/decomp/check.py --function "SvFgName::IsEmpty() const"   (can be repeated)
    python tools/decomp/check.py --explain nn::uds   (differences of every not equivalent function
                                                      whose name contains the text)
    python tools/decomp/check.py --all --show 2000   (list the equivalent ones, too)

Every function symbol of every object in the build directory gets its original address from
the "// 0x<address>" comment above its definition in the sources (so renaming vf_0x40 to a
real name keeps working; a "// 0x..." behind a declaration in a header counts, too), or else
by its (demangled) name in config/<version>/symbols.json. The bytes are compared with the
original at that address; words the linker fills in (relocations: calls, literal pool
addresses) are masked on both sides.

Status per function (bytes; GCC's code differs from the original ARMCC code, so only functions
written in assembly reach "match" - what counts is the grade below):
  match     same size, same bytes
  differs   same size, different bytes        (shows the first differing offset)
  size      different size
  prefix    identical, but the original seems to continue (a function start is probably
            missing in symbols.json - check in Ghidra; not counted as matched)
  stub      the body in the source is still empty
  unknown   no address for the name (new name, typo, or not in symbols.json)

Results go to <build dir>/check/results.json (read by progress.py).

Grade: every implemented function gets a compiler independent similarity score
(tools/decomp/fuzzy.py: calls, member offsets, constants, control flow), graded
equivalent >= 0.97, close >= 0.85, else far. With --function the differences are listed
feature by feature. A function is done when it is equivalent.
"""
import argparse, bisect, copy, glob, json, os, re, subprocess, sys
from collections import Counter

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, '..', '..'))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(ROOT, 'tools', 'analysis'))
import elf32                                    # noqa: E402
import elfmem                                   # noqa: E402
import fuzzy                                    # noqa: E402

STUB_WORDS = {(0xE12FFF1E,), (0xE7F000F0,)}     # bx lr, GCC's trap for a missing return


def norm(name):
    """compare names independent of spelling details of the demanglers"""
    n = name.replace('(anonymous namespace)', '@anon')
    n = re.sub(r'\bunsigned int\b', 'unsigned', n)
    n = re.sub(r'\bsigned char\b', 'schar', n)
    n = re.sub(r'\blong long\b', 'longlong', n)
    n = n.replace('(void)', '()')
    n = re.sub(r'\bconst ([\w:]+)(?=[*&])', r'\1 const', n)    # 'const T*' == 'T const*'
    return re.sub(r'\s+', '', n)


def split_signature(sig):
    """'void A::B::f(int, X<a, b>) const' -> ('A::B::f', ['int', 'X<a, b>'], True)"""
    sig = sig.strip().replace('(anonymous namespace)', '@anon')
    const = sig.endswith(' const') or sig.endswith(')const')
    if const:
        sig = sig[:sig.rindex('const')].rstrip()
    if not sig.endswith(')'):
        return None
    depth = 0
    for i in range(len(sig) - 1, -1, -1):
        depth += {')': 1, '(': -1}.get(sig[i], 0)
        if depth == 0:
            break
    head, inner = sig[:i], sig[i + 1:-1]
    m = re.search(r'([\w:~@<>]+|operator\s*\S+)\s*$', head)
    if not m:
        return None
    params, depth, cur = [], 0, ''
    for ch in inner:
        if ch in '<(':
            depth += 1
        elif ch in '>)':
            depth -= 1
        if ch == ',' and depth == 0:
            params.append(cur.strip())
            cur = ''
        else:
            cur += ch
    if cur.strip() and cur.strip() != 'void':
        params.append(cur.strip())
    return m.group(1), params, const


def sig_key(qualname, params, const):
    return (re.sub(r'\s+', '', qualname), len(params), const)


def source_addresses():
    """(name, parameter count, const) -> address, from the '// 0x...' comments in the sources.
    Also returns the keys whose body is still empty (generated stubs), and for destructors the
    address of the deleting variant (a second comment line that says 'deleting')."""
    out, dup, empty, deleting = {}, set(), set(), {}
    for d in ('src', 'lib', 'modules'):
        for path in glob.glob(os.path.join(ROOT, d, '**', '*.cpp'), recursive=True):
            ns, pending, sig, body_of, pending_del = [], None, None, None, None
            for line in open(path, encoding='utf-8', errors='replace'):
                st = line.strip()
                if body_of is not None and st:
                    if st == '}':
                        empty.add(body_of)
                    body_of = None
                m = re.match(r'namespace\s*(\w*)\s*\{', st)
                if m:
                    ns.append(m.group(1) or '@anon')
                    continue
                if re.match(r'\}\s*//\s*namespace', st) and ns:
                    ns.pop()
                    continue
                m = re.match(r'//\s*0x([0-9A-Fa-f]{6,8})(?![0-9A-Fa-f])', st)
                if m:
                    if 'deleting' in st:
                        # GCC's D0 variant; the line of the complete destructor may come before or after
                        pending_del, sig = int(m.group(1), 16) & ~1, ''
                        if pending is None:
                            pending = -1
                    else:
                        pending, sig = int(m.group(1), 16) & ~1, ''
                    continue
                if re.match(r'//\s*ctor (candidate|address unknown)', st):
                    pending, sig = -1, ''                 # no verified address: only track empty bodies
                    continue
                if pending is None or not st or st.startswith('//'):
                    continue
                sig += ' ' + st
                if '{' in st or st.endswith(';'):
                    parsed = split_signature(sig.split('{')[0].rstrip().rstrip(';'))   # also declarations
                    pending_addr, pending, sig = pending, None, None
                    del_addr, pending_del = pending_del, None
                    if not parsed:
                        continue
                    name, params, const = parsed
                    # definitions inside "namespace a {" may or may not repeat "a::"
                    full = name if not ns or name.startswith(ns[0] + '::') else '::'.join(ns + [name])
                    key = sig_key(full, params, const)
                    if del_addr is not None:
                        deleting[key] = del_addr
                    if pending_addr >= 0:
                        if key in out and out[key] != pending_addr:
                            dup.add(key)
                        out[key] = pending_addr
                    if re.search(r'\{\s*\}', st):
                        empty.add(key)
                    elif st.endswith('{'):
                        body_of = key
    for k in dup:
        out.pop(k, None)                          # ambiguous overloads: fall back to symbols.json
    # declarations in headers with the address behind them ("void f(int); // 0x00123456"):
    # used where no definition in a .cpp has the address (inline or not yet written functions,
    # and names that correct symbols.json)
    for key, address in header_addresses().items():
        out.setdefault(key, address)
    return out, empty, deleting


def header_addresses():
    """(name, parameter count, const) -> address of the declarations in the headers that end
    with a '// 0x...' comment; names are qualified by the enclosing namespaces and classes"""
    out, dup = {}, set()
    for d in ('src', 'lib', 'modules', 'include'):
        for path in glob.glob(os.path.join(ROOT, d, '**', '*.h'), recursive=True):
            scopes = []                           # (name or None, brace depth inside it)
            depth = 0
            pending_scope = None
            stmt = ''
            for line in open(path, encoding='utf-8', errors='replace'):
                code, _, comment = line.partition('//')
                st = code.strip()
                m = re.match(r'(?:namespace|class|struct)\s+(\w*)', st)
                if m and not st.endswith(';'):
                    pending_scope = m.group(1) or '@anon'
                if st and not st.startswith('#'):
                    stmt += ' ' + st
                for ch in code:
                    if ch == '{':
                        depth += 1
                        scopes.append((pending_scope, depth))
                        pending_scope = None
                        stmt = ''
                    elif ch == '}':
                        if scopes and scopes[-1][1] == depth:
                            scopes.pop()
                        depth -= 1
                        stmt = ''
                if st.endswith(';') or st.endswith(':'):
                    decl, stmt = stmt.strip().rstrip(';'), ''
                    m = re.match(r'\s*0x([0-9A-Fa-f]{8})(?![0-9A-Fa-f])', comment)
                    if not m or '(' not in decl or decl.startswith(('typedef', 'using', 'return')):
                        continue
                    decl = re.sub(r'^(?:(?:static|virtual|inline|explicit|extern\s+"C")\s+)+', '', decl)
                    parsed = split_signature(decl)
                    if not parsed:
                        continue
                    name, params, const = parsed
                    prefix = [n for n, _ in scopes if n]
                    if prefix and not name.startswith('::'.join(prefix) + '::'):
                        name = '::'.join(prefix + [name.split('::')[-1]]) if '::' not in name else name
                    key = sig_key(name, params, const)
                    address = int(m.group(1), 16) & ~1
                    if key in out and out[key] != address:
                        dup.add(key)
                    out[key] = address
    for k in dup:
        out.pop(k, None)
    return out


VARIABLE_RE = re.compile(r'([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*(?:\[[^\]]*\])?\s*(?:\)\s*\(.*\))?\s*(?:=.*)?;\s*(?://.*)?$')


def source_variables():
    """normalized qualified name -> address of the global variables defined in the sources under a
    '// 0x...' comment (e.g. "// 0x00975F84" above "AutoStackManager* Thread::s_pAutoStackManager;")"""
    out = {}
    for d in ('src', 'lib', 'modules'):
        for path in glob.glob(os.path.join(ROOT, d, '**', '*.cpp'), recursive=True):
            ns, pending = [], None
            for line in open(path, encoding='utf-8', errors='replace'):
                st = line.strip()
                m = re.match(r'namespace\s*(\w*)\s*\{', st)
                if m:
                    ns.append(m.group(1) or '@anon')
                    continue
                if re.match(r'\}\s*//\s*namespace', st) and ns:
                    ns.pop()
                    continue
                m = re.match(r'//\s*0x([0-9A-Fa-f]{6,8})\s*(?:\(.*\))?\s*$', st)
                if m:
                    pending = int(m.group(1), 16)
                    continue
                if pending is None or not st or st.startswith('//'):
                    continue
                address, pending = pending, None
                if '{' in st or st.startswith('extern'):
                    continue
                m = VARIABLE_RE.search(st)
                if not m or '(' in st.split(m.group(1))[0].replace('(*', ''):
                    continue
                name = m.group(1)
                full = name if not ns or name.startswith(ns[0] + '::') else '::'.join(ns + [name])
                out[norm(full)] = address
    return out


def tool(name):
    for c in (os.environ.get('DEVKITARM', ''), r'C:\devkitPro\devkitARM', '/opt/devkitpro/devkitARM'):
        p = os.path.join(c, 'bin', 'arm-none-eabi-' + name + ('.exe' if os.name == 'nt' else ''))
        if c and os.path.exists(p):
            return p
    return 'arm-none-eabi-' + name


def demangle(names):
    if not names:
        return {}
    p = subprocess.run([tool('c++filt')], input='\n'.join(names), capture_output=True, text=True)
    return dict(zip(names, p.stdout.splitlines()))


def masked_words(data, relocs, size):
    words = []
    rmask = {off: elf32.reloc_mask(t) for off, t, _ in relocs}
    for o in range(0, size - size % 4, 4):
        w = int.from_bytes(data[o:o + 4], 'little')
        words.append(w & ~rmask.get(o, 0) & 0xFFFFFFFF)
    return words


def load_targets(version):
    db = json.load(open(os.path.join(ROOT, 'config', version, 'symbols.json'), encoding='utf-8'))
    by_name = {}
    # the same name can come from several sources: the better tier wins (A before B before C)
    for f in sorted(db['functions'], key=lambda f: (f.get('tier') or 'Z', f['addr'])):
        by_name.setdefault(norm(f['name']), f['addr'] & ~1)
    for c in db['classes']:
        for vt in c['vtables']:
            for e in vt['entries']:
                if e.get('label') and e.get('owner') == c['name']:
                    by_name.setdefault(norm(e['label']), e['addr'] & ~1)
    starts = sorted(set(a & ~1 for a in db['function_starts']))
    return by_name, starts, db


def is_ctor_dtor(name):
    """ARM C++ ABI: constructors and destructors return 'this'"""
    m = re.match(r'(?:.*::)?([~\w]+)::(~?)(\w+)\(', name or '')
    return bool(m) and (m.group(2) == '~' or m.group(1) == m.group(3))


def orig_size(starts, addr, limit):
    i = bisect.bisect_right(starts, addr)
    nxt = starts[i] if i < len(starts) else limit
    return nxt - addr


def make_fuzzy_pair(mem, db, starts, names, by_source, by_name, variables=None):
    """-> f(data, size, syminfo, addr, orig_size) = (features of the object function,
    features of the original), both with call targets / literals in the same terms:
    ('fn', address) ('addr', vptr) ('str', text) ('k', value) ('data',) ('rodata',) ('name', n)"""
    start_set = set(starts)
    # functions the sources name by address (e.g. Thumb veneers like nnosDefaultUnexpectedHandler)
    source_set = {a for a in by_source.values() if a >= 0}
    addr_name = {}
    for f in sorted(db['functions'], key=lambda f: (f.get('tier') or 'Z', f['addr'])):
        addr_name.setdefault(f['addr'] & ~1, f['name'])
    vptrs = {}
    for c in db['classes']:
        for vt in c['vtables']:
            if vt.get('offset_to_top', 0) == 0:
                vptrs.setdefault(c['name'], vt['addr'])
    vptr_set = {vt['vptr'] for c in db['classes'] for vt in c['vtables']}
    data_end = db['binary']['data'][1]
    # system calls that ARMCC inlines (__svc) but GCC calls as tiny wrapper functions
    inline_svc = {}
    svc_h = os.path.join(ROOT, 'lib', 'nn', 'include', 'nn', 'svc', 'svc_Api.h')
    if os.path.exists(svc_h):
        for m in re.finditer(r'NN_SVC_INLINE\((0x[0-9A-Fa-f]+)\)[^;(]*?(\w+)\(', open(svc_h, encoding='utf-8').read()):
            inline_svc[f'nn::svc::{m.group(2)}'] = int(m.group(1), 16)

    def svc_stub(target):
        """number of the system call if the function at target is just 'svc N; bx lr'"""
        if not mem.in_text(target):
            return None
        w0, w1 = mem.u32(target), mem.u32(target + 4)
        return w0 & 0xFFFFFF if (w0 & 0xFF000000) == 0xEF000000 and w1 == 0xE12FFF1E else None

    def mine_key(info, is_call=False):
        name, typ = info[0], info[1]
        if not name or typ == 3:
            return None
        dn = names.get(name, name)
        if dn.split('(')[0] in inline_svc:
            return ('svc', inline_svc[dn.split('(')[0]])
        parsed = split_signature(dn)
        addr = by_source.get(sig_key(*parsed)) if parsed else None
        if addr is None and not parsed:
            # a C function (no parameter list in the name): by its name alone
            addr = next((a for k, a in by_source.items() if k[0] == re.sub(r'\s+', '', dn)), None)
        if addr is None:
            addr = by_name.get(norm(dn))
        if addr is None and '(' in dn:
            addr = by_name.get(norm(dn.split('(')[0]))     # symbols.json name without parameters
        fam = fuzzy.runtime_family(dn) or (fuzzy.runtime_family(addr_name.get(addr & ~1)) if addr is not None else None)
        if fam:
            return ('rt', fam)
        if addr is not None:
            return orig_fn(addr & ~1)
        if '(' not in dn and not is_call and typ != 2:
            return ('data',)                       # global variable defined in another object
        return ('name', norm(dn))

    def orig_fn(target):
        fam = fuzzy.runtime_family(addr_name.get(target))
        if fam:
            return ('rt', fam)
        n = svc_stub(target)
        return ('svc', n) if n is not None else ('fn', target)

    variables = variables or {}
    section_names = {}

    def global_address(name, typ, secname, value, addend):
        """address of a global variable of the sources a data relocation points into, or None"""
        if typ == 2 or name.startswith('_Z') and names.get(name, name).startswith(('vtable for', 'typeinfo')):
            return None
        if name and typ != 3:
            return variable_address(names.get(name, name), addend)
        # static variable: relocation against its section (.bss.<mangled name> with -fdata-sections)
        parts = secname.split('.', 2)
        if len(parts) == 3 and parts[1] in ('bss', 'data', 'rodata') and parts[2].startswith('_Z'):
            if parts[2] not in section_names:
                section_names[parts[2]] = demangle([parts[2]]).get(parts[2], parts[2])
            return variable_address(section_names[parts[2]], value + addend)
        return None

    def variable_address(dn, addend):
        """address of the source variable dn (+ addend), or None"""
        guard = dn.startswith('guard variable for ')
        if guard:
            dn = dn[len('guard variable for '):]
        # "(anonymous namespace)" is not a parameter list
        dn = dn.replace('(anonymous namespace)', '@anon')
        # a function's static: "a::f()::s_x" -> "a::s_x" (the comment is inside the function)
        dn = re.sub(r'::[^:()]*\([^()]*\)(?: const)?::', '::', dn)
        if '(' in dn:
            return None
        a = variables.get(norm(dn))
        if a is None:
            return None
        return a + addend + (4 if guard else 0)          # ARMCC puts the guard after the static

    def returns_this(key):
        n = addr_name.get(key[1]) if key[0] == 'fn' else key[1] if key[0] == 'name' else None
        return is_ctor_dtor(n)

    def pair(data, size, syminfo, addr, osize, name=None):
        structor = is_ctor_dtor(name)
        n_mine = size // 4
        mine_words = [int.from_bytes(data[o:o + 4], 'little') for o in range(0, n_mine * 4, 4)]

        def mine_call(i, w):
            info = syminfo.get(i * 4)
            return mine_key(info, is_call=True) if info else None

        def mine_word(j, w):
            info = syminfo.get(j * 4)
            if not info:
                return ('k', w)
            name, typ, secname, secdata, value = info
            g = global_address(name, typ, secname, value, w)
            if g is not None:
                return ('gaddr', g)
            if typ == 3 and secname.startswith('.rodata._ZTV'):
                name = secname[len('.rodata.'):]            # section symbol of a local vtable
            if name.startswith('_ZTV'):
                cls = names.get(name, name).replace('vtable for ', '')
                vt = vptrs.get(cls)
                return ('addr', vt + w) if vt is not None else ('name', norm(cls) + '::vtable')
            if typ == 2:
                return mine_key(info) or ('rodata',)
            if secname.startswith('.rodata'):
                off = (value if typ != 3 else 0) + w
                end = secdata.find(b'\0', off)
                text = secdata[off:end] if end >= 0 else b''
                if text and all(0x20 <= c < 0x7F or c in (9, 10, 13) for c in text):
                    return ('str', text.decode('ascii'))
                return ('rodata',)
            if secname.startswith(('.data', '.bss')) or typ == 1:
                return ('data',)
            return mine_key(info) or ('data',)

        f_mine = fuzzy.extract(mine_words, 0, mine_call, mine_word, returns_this, structor)
        f_orig = orig_features(addr, osize, structor)
        inline_expand(f_mine, f_orig)
        if name and '::~' in name:
            fuzzy.ignore_dtor_vptr(f_mine, f_orig)
        return f_mine, f_orig

    def orig_features(addr, osize, structor=False):
        n_orig = osize // 4

        def orig_call(i, w):
            target = addr + i * 4 + 8 + fuzzy._sext24(w & 0xFFFFFF) * 4
            if w >> 28 == 0xF:
                target |= (w >> 23) & 2                     # blx: H bit
            elif not w & (1 << 24) and addr <= target < addr + n_orig * 4:
                return None                                  # branch inside the function
            return orig_fn(target & ~1)

        def orig_word(j, w):
            # a known function, or code that starts like one (push {..., lr}); other values in the
            # code range are constants (IPC headers like 0x001C0040 are)
            if mem.in_text(w & ~1) and ((w & ~1) in start_set or (w & ~1) in addr_name or (w & ~1) in source_set or
                                        (w & 3 == 0 and (mem.u32(w) & 0xFFFF4000) == 0xE92D4000)):
                return orig_fn(w & ~1)
            if w in vptr_set:
                return ('addr', w)
            if mem.ro[0] <= w < mem.ro[1]:
                s = mem.cstr(w)
                return ('str', s) if s else ('rconst', w)
            if mem.data[0] <= w < data_end + 0x100000:
                return ('gaddr', w)
            return ('k', w)

        orig_words = [mem.u32(addr + k * 4) for k in range(n_orig)]
        return fuzzy.extract(orig_words, addr, orig_call, orig_word, returns_this, structor,
                             next_key=orig_fn(addr + n_orig * 4),
                             resolve_target=lambda v: orig_fn(v & ~1),
                             read_const=lambda a: mem.u32(a) if mem.ro[0] <= a < mem.ro[1] - 3 else 0)

    def expand(f, surplus, label):
        """replace calls in f (as many as surplus says) by the original body of the callee; its
        accesses through its this are moved to where r0 pointed at the call (member objects)"""
        calls, call_this = [], []
        for key, this_off in zip(f['calls'], f.get('call_this') or [None] * len(f['calls'])):
            if key[0] == 'fn' and surplus[key] > 0 and mem.in_text(key[1]):
                surplus[key] -= 1
                callee = orig_features(key[1], orig_size(starts, key[1], mem.text[1]))
                calls.extend(callee['calls'])
                call_this.extend([None] * len(callee['calls']))
                moved = callee['mem']
                if this_off == 'sp':
                    # r0 pointed to a buffer on the stack (ignored like all stack accesses): only
                    # the callee's accesses of globals count
                    moved = Counter({k: n for k, n in moved.items() if isinstance(k[2], tuple)})
                elif this_off:
                    moved = moved - callee['mem_this']
                    for (sign, width, off), n in callee['mem_this'].items():
                        moved[(sign, width, off + this_off if isinstance(off, int) else off)] += n
                f['mem'].update(moved)
                for cat in ('consts', 'flow'):
                    f[cat].update(callee[cat])
                f.setdefault(label, []).append(addr_name.get(key[1], f'0x{key[1]:X}'))
            else:
                calls.append(key)
                call_this.append(this_off)
        f['calls'], f['call_this'] = calls, call_this

    def call_similarity(f_mine, f_orig):
        return fuzzy.compare(f_mine, f_orig)[1]['calls'] or 0

    def inline_expand(f_mine, f_orig):
        """Inlining differs between the compilers (ARMCC inlined across files, GCC within a file):
        a call that only one side has is replaced by the original body of the callee (one level),
        as long as that makes the calls of both sides more alike"""
        for _ in range(8):
            best = None
            for side, other, label in ((f_mine, f_orig, 'inlined'), (f_orig, f_mine, 'inlined_by_you')):
                for key in Counter(side['calls']) - Counter(other['calls']):
                    if key[0] != 'fn' or not mem.in_text(key[1]):
                        continue
                    trial = copy.deepcopy(side)
                    expand(trial, Counter({key: 1}), label)
                    pair_ = (trial, other) if side is f_mine else (other, trial)
                    gain = call_similarity(*pair_) - call_similarity(f_mine, f_orig)
                    if gain > 1e-9 and (best is None or gain > best[0]):
                        best = (gain, side, trial)
            if best is None:
                break
            best[1].clear()
            best[1].update(best[2])
        # where the callee was inlined, the test of its result ("if (TryX())") went into the
        # callee's own branches: the side that calls it has one decision more per such call
        for side, other, label in ((f_mine, f_orig, 'inlined'), (f_orig, f_mine, 'inlined_by_you')):
            extra = side['flow']['decision'] - other['flow']['decision']
            n = min(extra, len(side.get(label, [])))
            if n > 0:
                side['flow']['decision'] -= n
                if not side['flow']['decision']:
                    del side['flow']['decision']

    return pair


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build-dir', default=os.path.join(ROOT, 'build', 'gcc'))
    ap.add_argument('--version', default='USA_1_5')
    ap.add_argument('--show', type=int, default=20)
    ap.add_argument('--all', action='store_true', help='list the equivalent functions, too')
    ap.add_argument('--function', action='append', default=[],
                    help='print the differences of a function (demangled name); can be given several times')
    ap.add_argument('--explain', metavar='TEXT',
                    help='print the differences of every implemented, not equivalent function whose name contains TEXT')
    ap.add_argument('--fuzzy', action='store_true', help=argparse.SUPPRESS)   # always on; kept for old commands
    a = ap.parse_args()

    mem = elfmem.Mem(os.path.join(ROOT, 'orig', a.version, 'code.elf'))
    by_name, starts, db = load_targets(a.version)
    objs = [p for p in glob.glob(os.path.join(a.build_dir, '**', '*.o*'), recursive=True)
            if p.endswith(('.o', '.obj'))]
    if not objs:
        raise SystemExit(f'no objects in {a.build_dir} - build first')

    funcs = []                                    # (obj, mangled, section, value, size, relocs)
    for path in objs:
        try:
            e = elf32.Elf(path)
        except ValueError:
            continue
        per_section = {}
        for s in e.symbols:
            if s.type == elf32.STT_FUNC and 0 < s.shndx < len(e.sections):
                per_section.setdefault(s.shndx, []).append(s)
        for shndx, syms in per_section.items():
            sec = e.sections[shndx]
            relocs = e.relocations(sec)
            for s in syms:
                start = s.value & ~1
                # one function per section (split sections): the literal pool belongs to it
                size = sec.size - start if len(syms) == 1 else s.size
                data = sec.data[start:start + size]
                rel = [(o - start, t, x) for o, t, x in relocs if start <= o < start + size]
                syminfo = {}
                for o, t, x in rel:
                    if x < len(e.symbols):
                        rs = e.symbols[x]
                        sec2 = e.sections[rs.shndx] if 0 < rs.shndx < len(e.sections) else None
                        syminfo[o] = (rs.name, rs.type, sec2.name if sec2 else '',
                                      sec2.data if sec2 else b'', rs.value)
                funcs.append((os.path.relpath(path, a.build_dir), s.name, data, size, rel, syminfo))

    # vtables of classes in an unnamed namespace are local: GCC refers to them through the section
    # symbol of '.rodata._ZTV...', so demangle the section names, too
    vtable_sections = {i[2][len('.rodata.'):] for f in funcs for i in f[5].values() if i[2].startswith('.rodata._ZTV')}
    names = demangle(sorted({f[1] for f in funcs} | {i[0] for f in funcs for i in f[5].values()} | vtable_sections))
    by_source, empty_bodies, by_source_deleting = source_addresses()
    thumb = {s & ~1 for s in db['function_starts'] if s & 1}
    fuzzy_pair = make_fuzzy_pair(mem, db, starts, names, by_source, by_name, source_variables())
    results, counts = {}, {}
    details = []
    for obj, mangled, data, size, rel, syminfo in funcs:
        if re.search(r'(C2|D2)E', mangled):
            continue                              # GCC's base-object variants
        is_deleting = re.search(r'D0E', mangled) is not None
        name = names.get(mangled, mangled)
        parsed = split_signature(name)
        skey = sig_key(*parsed) if parsed else None
        if is_deleting:
            # only compared where the source names the address of the deleting destructor
            addr = by_source_deleting.get(skey)
            if addr is None:
                continue
            name += ' [deleting]'
        else:
            addr = by_source.get(skey) if parsed else None
        if addr is None:
            addr = by_name.get(norm(name))
        mine = masked_words(data, rel, size)
        if addr is None:
            status, osize = 'unknown', None
        else:
            osize = orig_size(starts, addr, mem.text[1])
            if tuple(mine) in STUB_WORDS or skey in empty_bodies:
                status = 'stub'
            elif osize != size:
                status = 'size'
                if size < osize:
                    # identical as far as it goes: probably a function start missing in symbols.json
                    rmask = {off: elf32.reloc_mask(t) for off, t, _ in rel}
                    theirs = [mem.u32(addr + o) & ~rmask.get(o, 0) & 0xFFFFFFFF for o in range(0, size - size % 4, 4)]
                    if theirs == mine:
                        status = 'prefix'
            else:
                theirs = [mem.u32(addr + o) for o in range(0, size, 4)]
                # mask the same words in the original
                rmask = {off: elf32.reloc_mask(t) for off, t, _ in rel}
                theirs = [w & ~rmask.get(i * 4, 0) & 0xFFFFFFFF for i, w in enumerate(theirs)]
                diff = next((i for i, (x, y) in enumerate(zip(mine, theirs)) if x != y), None)
                status = 'match' if diff is None else 'differs'
        counts[status] = counts.get(status, 0) + 1
        key = f'0x{addr:08X}' if addr is not None else f'?{mangled}'
        results[key] = {'name': name, 'mangled': mangled, 'status': status, 'size': size,
                        'orig_size': osize, 'object': obj}
        if status == 'differs':
            results[key]['first_difference'] = diff * 4
        fz = None
        if status in ('differs', 'size', 'prefix', 'match'):
            if addr in thumb:
                results[key]['grade'] = 'thumb'
            else:
                # prefix: the original range runs into the next (unknown) function
                fz = fuzzy_pair(data, size, syminfo, addr, size if status == 'prefix' else osize, name)
                score, cats = fuzzy.compare(*fz)
                results[key].update(score=round(score, 3), grade=fuzzy.grade(score),
                                    categories={k: (round(v, 3) if v is not None else None)
                                                for k, v in cats.items()})
        wanted = any(norm(f) == norm(name) for f in a.function)
        if a.explain and a.explain in name and fz is not None and results.get(key, {}).get('grade') != 'equivalent':
            wanted = True
        if wanted:
            details.append((name, addr, mine, size, fz))

    out_dir = os.path.join(a.build_dir, 'check')
    os.makedirs(out_dir, exist_ok=True)
    with open(os.path.join(out_dir, 'results.json'), 'w', encoding='utf-8') as fp:
        json.dump({'version': a.version, 'functions': results}, fp, indent=1)

    total = sum(counts.values())
    print(f'{total} compiled functions: ' + ', '.join(f'{k} {v}' for k, v in sorted(counts.items())))
    scored = [(k, r) for k, r in results.items() if 'score' in r]
    grades = Counter(r['grade'] for r in results.values() if 'grade' in r)
    if scored:
        mean = sum(r['score'] for _, r in scored) / len(scored)
        print(f'fuzzy: {len(scored)} implemented functions, mean score {mean:.3f}: '
              + ', '.join(f'{g} {grades[g]}' for g in ('equivalent', 'close', 'far', 'thumb') if grades[g]))
        for k, r in sorted(scored, key=lambda kr: kr[1]['score'])[:a.show]:
            if r['grade'] == 'equivalent' and not a.all:
                break
            cats = ' '.join(f'{c}={v:.2f}' for c, v in r['categories'].items() if v is not None)
            print(f'  {r["score"]:.3f} {r["grade"]:10s} {k}  {r["name"]}   [{cats}]')
    else:
        print('fuzzy: no implemented functions with a known address')
    for name, addr, mine, size, fz in details:
        print(f'\n{name} @ 0x{addr:08X}' if addr is not None else f'\n{name}: no address')
        if fz is not None:
            score, cats = fuzzy.compare(*fz)
            print(f'fuzzy score {score:.3f} ({fuzzy.grade(score)}): '
                  + ' '.join(f'{c}={v:.2f}' for c, v in cats.items() if v is not None))
            lines = fuzzy.explain(*fz)
            print('\n'.join(lines) if lines else '  no differences in calls, member accesses, constants or control flow')
        elif addr is not None:
            for i, w in enumerate(mine):
                o = mem.u32(addr + i * 4)
                print(f'  +0x{i * 4:03X}  {w:08X}  {o:08X}  {"" if w == o else "<--"}')


if __name__ == '__main__':
    main()
