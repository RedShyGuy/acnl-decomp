#!/usr/bin/env python3
"""Syntax-check every C++ source with devkitARM's GCC (no code generation).

    python tools/decomp/syntax_check.py [path filter] [--jobs N] [--show N]

Useful as a quick check without configuring the CMake build. Prints a summary of the most common errors.
"""
import argparse, collections, concurrent.futures, glob, os, re, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..', '..'))


def find_gxx():
    cands = [os.environ.get('DEVKITARM', ''), r'C:\devkitPro\devkitARM', '/opt/devkitpro/devkitARM']
    for c in cands:
        for exe in ('arm-none-eabi-g++.exe', 'arm-none-eabi-g++'):
            p = os.path.join(c, 'bin', exe)
            if c and os.path.exists(p):
                return p
    return 'arm-none-eabi-g++'


def include_dirs():
    dirs = [os.path.join(ROOT, 'include')]
    dirs += sorted(glob.glob(os.path.join(ROOT, 'lib', '*', 'include')))
    dirs += sorted(glob.glob(os.path.join(ROOT, 'modules', '*', 'include')))
    return dirs


def check(args):
    gxx, src, incs = args
    cmd = [gxx, '-fsyntax-only', '-std=gnu++17', '-march=armv6k', '-mfloat-abi=hard', '-mfpu=vfp',
           '-fno-rtti', '-fno-exceptions', '-w', '-ferror-limit=5' if False else '-fmax-errors=5']
    cmd += [f'-I{d}' for d in incs] + [src]
    p = subprocess.run(cmd, capture_output=True, text=True)
    return src, p.returncode, p.stderr


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('filter', nargs='?', default='')
    ap.add_argument('--jobs', type=int, default=os.cpu_count())
    ap.add_argument('--show', type=int, default=15)
    a = ap.parse_args()
    gxx = find_gxx()
    srcs = [s for s in glob.glob(os.path.join(ROOT, '**', '*.cpp'), recursive=True)
            if os.sep + 'build' + os.sep not in s and os.sep + 'tools' + os.sep not in s
            and a.filter in s.replace('\\', '/')]
    incs = include_dirs()
    ok, bad = 0, []
    errs = collections.Counter()
    examples = {}
    with concurrent.futures.ThreadPoolExecutor(a.jobs) as ex:
        for src, rc, err in ex.map(check, [(gxx, s, incs) for s in srcs]):
            if rc == 0:
                ok += 1
                continue
            bad.append(src)
            for line in err.splitlines():
                mm = re.search(r'error: (.*)', line)
                if mm:
                    key = re.sub(r"'[^']*'", "'X'", mm.group(1))
                    errs[key] += 1
                    examples.setdefault(key, f'{os.path.relpath(src, ROOT)}: {mm.group(1)}')
    print(f'{ok} of {len(srcs)} files compile, {len(bad)} fail')
    for k, n in errs.most_common(a.show):
        print(f'{n:6d}  {k}\n        e.g. {examples[k][:200]}')
    return 0 if not bad else 1


if __name__ == '__main__':
    sys.exit(main())
