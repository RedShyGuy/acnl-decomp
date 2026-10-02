#!/usr/bin/env python3
"""Run check.py and show which scores changed since the previous run of this script.

    python tools/decomp/score_diff.py [--build-dir build/gcc]

Use it after every change to fuzzy.py (no function may get worse without a reason) and after a
batch of decompiled functions. The previous scores are kept in <build dir>/check/scores_prev.txt.
"""
import argparse, os, re, shutil, subprocess, sys

ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..', '..'))


def load(path):
    res = {}
    if os.path.exists(path):
        for line in open(path, encoding='utf-8'):
            m = re.match(r'\s+([\d.]+)\s+\S+\s+(0x\w+)\s+(.*?)\s+\[', line)
            if m:
                res[m.group(2)] = (float(m.group(1)), m.group(3))
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build-dir', default=os.path.join(ROOT, 'build', 'gcc'))
    a = ap.parse_args()
    out_dir = os.path.join(a.build_dir, 'check')
    os.makedirs(out_dir, exist_ok=True)
    cur, prev = os.path.join(out_dir, 'scores.txt'), os.path.join(out_dir, 'scores_prev.txt')
    if os.path.exists(cur):
        shutil.copy(cur, prev)
    text = subprocess.run([sys.executable, os.path.join(ROOT, 'tools', 'decomp', 'check.py'),
                           '--build-dir', a.build_dir, '--all', '--show', '100000'],
                          capture_output=True, text=True, cwd=ROOT).stdout
    open(cur, 'w', encoding='utf-8').write(text)
    print(next((l for l in text.splitlines() if l.startswith('fuzzy:')), 'fuzzy: no result'))
    before, after = load(prev), load(cur)
    for k in sorted(set(before) | set(after)):
        x, y = before.get(k, (None, ''))[0], after.get(k, (None, ''))[0]
        if x != y:
            tag = 'NEW ' if x is None else 'GONE' if y is None else 'DOWN' if y < x else 'UP  '
            print(tag, k, x, '->', y, (after.get(k) or before.get(k))[1][:80])


if __name__ == '__main__':
    main()
