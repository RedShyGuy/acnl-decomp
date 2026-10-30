#!/usr/bin/env python3
"""Decompilation progress: done functions / bytes per unit.

    python tools/decomp/progress.py [--build-dir build/gcc] [--version 0004000000086300]
                                    [--json out.json] [--functions-csv out.csv]

Reads <build dir>/check/results.json (written by check.py). A function is done when check.py
grades it equivalent (or its bytes match, for functions written in assembly); close ones are
shown separately. The total is every function
start known in config/<version>/symbols.json (the static code.bin; CRO modules are counted
from their module json files).

--functions-csv writes one line per known function (address, size, name, tier, status),
handy for spreadsheets or for picking the next function to decompile.
"""
import argparse, bisect, collections, csv, json, os, re, sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.normpath(os.path.join(HERE, '..', '..'))

LIBRARIES = ('nn', 'nw', 'sead', 'pead', 'imgdb', 'mw', 'libms', 'cfl', 'clib', 'std', '__rw', '__cxxabiv1')

# the ARM C library of the game (lib/clib): compiler helpers, C functions and the C++ ABI helpers of the
# ARM runtime
CLIB_FUNCTION = re.compile(
    r'(__aeabi_|__rt_|_ll_|__ARM_|__hardfp_|__softfp_|__fpl_|__mathlib_|__btod_|__support_|__memmove_|_memcpy_|__cxa_|__I\$)'
    r'|(_memset|_e2d|_e2e|_frem|bsearch|wcslen|mbsinit|mbrlen|mbrtowc|memcmp|memcpy|memset|strncat|tolower|towupper'
    r'|snprintf|sprintf|vsprintf|vsnprintf|swprintf|vswprintf|strtoull|wcstoul|wcstombs|wcscpy|wcsncpy|wmemcpy'
    r'|wmemmove|wcscat|wcscmp|wcsncmp|wcsstr|wcsicmp|strcat|strncmp|strlcpy|strncpy|strcmp|strlen|strcpy'
    r'|abort|memmoveWithResult|memcpyWithResult|memcpyWithResultEntry|__read_errno|__set_errno|wcstok|wmemset|memchr|towlower|__mbrtowc_byte|__cpp_initialize__aeabi_|__printf_\w+|_udiv10|__wcrtomb_char|__isdigit|ferror|printf|__vfprintf|_btod_etento|__fp_status|fputc|__flsbuf|__digit_value|__strtoul_core|__wcstoul_core|__strtoull_core|strtol|atoi|strtoul|__strtoull|__wctype_classes|iswspace|llabs|__scanf_\w+|sscanf|__stdio_\w+|__qsort_partitions|qsort|strchr|strstr|strrchr|strcoll|ftell|__fseek|wcrtomb|fclose|freopen|fopen|setvbuf|__raise_signal|__stdin|__stdout|__stderr)$')


# the C functions of nn (nngxInitialize, nncxZlib_deflate, nninitSetup, nndbgPanic, ...)
NN_C_FUNCTION = re.compile(r'nn(gx|gxlow|cx|init|dbg|nstd|os)[A-Z_]|nexZlib_|gl[A-Z]|__(sta|fb|fbv|vb|vbv|shm|shv|tx|txv|vldtr|sys|cb|mem|srf)_')


def unit_of(name):
    if NN_C_FUNCTION.match(name):
        return 'nn'
    if CLIB_FUNCTION.match(name):
        return 'clib'
    name = name.replace('(anonymous namespace)::', '')
    # a template function with its return type in the name ("void sead::PrintFormatter::out<int>(...)"): the
    # qualified name is the last word before the parameters (outside of template arguments)
    head = name.split('(', 1)[0]
    depth, cut = 0, -1
    for i, ch in enumerate(head):
        if ch == '<':
            depth += 1
        elif ch == '>':
            depth -= 1
        elif ch == ' ' and depth == 0 and not head.startswith('operator', i + 1):
            cut = i
    if cut >= 0 and not head[:cut].endswith('operator'):
        name = name[cut + 1:]
    top = re.split(r'::|\(', name, maxsplit=1)[0]
    return top if top in LIBRARIES else 'game'


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build-dir', default=os.path.join(ROOT, 'build', 'gcc'))
    ap.add_argument('--version', default='0004000000086300')
    ap.add_argument('--json')
    ap.add_argument('--functions-csv')
    a = ap.parse_args()

    db = json.load(open(os.path.join(ROOT, 'config', a.version, 'symbols.json'), encoding='utf-8'))
    starts = sorted(set(x & ~1 for x in db['function_starts']))
    names = {}
    tiers = {}
    for f in db['functions']:
        names.setdefault(f['addr'] & ~1, f['name'])
        tiers.setdefault(f['addr'] & ~1, f.get('tier') or '')

    res_path = os.path.join(a.build_dir, 'check', 'results.json')
    results = {}
    source_names = {}   # names from the sources (for starts that symbols.json leaves unnamed)
    source_units = {}   # the library whose sources define the function (lib_<unit>.dir of the object)
    if os.path.exists(res_path):
        for k, r in json.load(open(res_path, encoding='utf-8'))['functions'].items():
            if k.startswith('0x'):
                source_names.setdefault(int(k, 16), re.sub(r' \[(?:deleting|thunk|deleting thunk)\]$', '', r['name']))
                mo = re.search(r'lib_(\w+)\.dir', r.get('object') or '')
                if mo and mo.group(1) in LIBRARIES:
                    source_units.setdefault(int(k, 16), mo.group(1))
                if r['status'] == 'match' or r.get('grade') == 'equivalent':
                    results[int(k, 16)] = 'done'
                elif r.get('grade') == 'close':
                    results[int(k, 16)] = 'close'
                else:
                    results[int(k, 16)] = r.get('grade') or r['status']
    else:
        print(f'note: {res_path} not found - run the check target first; showing totals only')

    end = starts[-1] + 4
    total = collections.Counter()
    done = collections.Counter()
    close = collections.Counter()
    rows = []
    for i, addr in enumerate(starts):
        size = (starts[i + 1] if i + 1 < len(starts) else end) - addr
        name = names.get(addr) or source_names.get(addr, '')
        # a placeholder or guessed name of symbols.json ("sub_125D9C", "X::X() [ctor?]") gives way to the
        # name of the sources
        if addr in source_names and (re.fullmatch(r'sub_[0-9A-Fa-f]+', name) or name.endswith('[ctor?]')):
            name = source_names[addr]
        unit = unit_of(name) if name else 'unnamed'
        # a name outside the namespaces of the libraries (global operators of sead/pead): the library whose
        # sources define it
        if unit == 'game' and addr in source_units:
            unit = source_units[addr]
        status = results.get(addr, 'todo')
        total[unit] += size
        total['all'] += size
        if status == 'done':
            done[unit] += size
            done['all'] += size
        elif status == 'close':
            close[unit] += size
            close['all'] += size
        rows.append((f'0x{addr:08X}', size, name, tiers.get(addr, ''), status, unit))

    def pct(u):
        return 100.0 * done[u] / total[u] if total[u] else 0.0
    print(f'{"unit":10s} {"done":>12s} {"close":>10s} {"total":>12s}  progress')
    for u in sorted(total, key=lambda u: (u == 'all', -total[u])):
        print(f'{u:10s} {done[u]:12,d} {close[u]:10,d} {total[u]:12,d}  {pct(u):6.2f}%')
    done_funcs = sum(1 for r in rows if r[4] == 'done')
    close_funcs = sum(1 for r in rows if r[4] == 'close')
    print(f'\nfunctions: {done_funcs} of {len(rows)} done (equivalent), {close_funcs} close')

    # CRO modules: functions listed in the module json files
    mod_total = 0
    for p in sorted(os.listdir(os.path.join(ROOT, 'config', a.version, 'modules'))):
        mod = json.load(open(os.path.join(ROOT, 'config', a.version, 'modules', p), encoding='utf-8'))
        mod_total += len(mod.get('functions', []))
    print(f'CRO modules: {mod_total} named functions (byte progress of modules needs the CRO build)')

    if a.json:
        with open(a.json, 'w', encoding='utf-8') as fp:
            json.dump({u: {'done': done[u], 'close': close[u], 'total': total[u]} for u in total}, fp, indent=1)
    if a.functions_csv:
        with open(a.functions_csv, 'w', newline='', encoding='utf-8') as fp:
            w = csv.writer(fp)
            w.writerow(['address', 'size', 'name', 'tier', 'status', 'unit'])
            w.writerows(rows)
        print(f'wrote {a.functions_csv}')


if __name__ == '__main__':
    main()
