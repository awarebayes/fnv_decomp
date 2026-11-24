#!/usr/bin/env python3
"""Compile every translation unit of this tree as x64 with MSVC 19.16 (/O2), on Windows.

usage: python build_x64.py --msvc <dir holding VC/Tools/MSVC/<ver> and Windows Kits/10> [--jobs 6]
                           [--out build-x64] [--only TEXT] [--list FILE] [--limit N] [--max-lines N]

This is a port experiment, not a byte-matching build: the sources were written for a 32-bit
compiler, and include/x64_compat.h (force-included) absorbs what x64 cannot express. Only the
compile step runs; objects land under --out. The flags are the ones the Skyrim SE reconstruction
recipe uses so the generated code is comparable: /O2 /MD /GS- /EHsc /GR /fp:precise /Gy.
"""
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

# Some sources define a global `operator new(size_t, <integer>)` next to the sized `operator delete`, which
# this compiler refuses to pair; those units are compiled again without sized deallocation.
RETRY = {'C2956': ['/Zc:sizedDealloc-']}
FLAGS = ['/nologo', '/c', '/O2', '/MD', '/GS-', '/EHsc', '/GR', '/fp:precise', '/Gy', '/w']


def find_toolchain(root):
    msvc = sorted((root / 'VC' / 'Tools' / 'MSVC').iterdir())[-1]
    kits = root / 'Windows Kits' / '10'
    sdk = sorted((kits / 'Include').iterdir())[-1].name
    inc = [msvc / 'include'] + [kits / 'Include' / sdk / d for d in ('ucrt', 'um', 'shared')]
    cl = msvc / 'bin' / 'Hostx64' / 'x64' / 'cl.exe'
    return cl, inc, msvc


def compile_unit(cl, env, repo, out, unit, timeout):
    src = Path(unit['source'])
    stem = re.sub(r'[^A-Za-z0-9_]', '_', str(src.with_suffix('')))
    if len(stem) > 90:  # Windows path limit: keep names unique but short
        stem = stem[:60] + '_' + hashlib.sha1(stem.encode()).hexdigest()[:10]
    obj = out / 'obj' / (stem + '.obj')
    # preprocessor defines are part of what a unit means (zlib units rename the CRT copies); other flags are replaced
    defines = [f for f in unit.get('flags', []) if f.startswith('/D')]
    start = time.time()
    extra, retried = [], []
    while True:
        cmd = [str(cl)] + FLAGS + extra + defines + ['/FI', str(repo / 'include' / 'x64_compat.h'),
                                   '/I', str(repo / 'include'), '/I', str(repo / src.parent), '/I', str(repo / 'src'),
                                   '/Fo' + str(obj), str(repo / src)]
        try:
            p = subprocess.run(cmd, env=env, capture_output=True, text=True, timeout=timeout, stdin=subprocess.DEVNULL)
            text, rc = p.stdout + p.stderr, p.returncode
        except subprocess.TimeoutExpired:
            text, rc = 'TIMEOUT', 99
        more = [f for code, flags in RETRY.items() if re.search(r'error ' + code, text) for f in flags if f not in extra]
        if rc == 0 or not more:
            break
        extra += more
        retried += more
    (out / 'logs' / (stem + '.log')).write_text(text)
    errors = [l.strip() for l in text.splitlines() if re.search(r'error C\d+|fatal error', l)]
    return {'source': unit['source'], 'ok': rc == 0 and obj.exists(), 'seconds': round(time.time() - start, 1),
            'errors': errors[:5], 'nerrors': len(errors), 'extra_flags': retried}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--msvc', required=True, type=Path)
    ap.add_argument('--jobs', type=int, default=6)
    ap.add_argument('--out', type=Path, default=Path('build-x64'))
    ap.add_argument('--only', default='')
    ap.add_argument('--list', type=Path, help='file with one source path per line: compile only those')
    ap.add_argument('--limit', type=int, default=0)
    ap.add_argument('--max-lines', type=int, default=0)
    ap.add_argument('--timeout', type=int, default=600)
    a = ap.parse_args()
    repo = Path(__file__).resolve().parent.parent.parent
    cl, inc, msvc = find_toolchain(a.msvc)
    env = dict(os.environ, INCLUDE=os.pathsep.join(map(str, inc)))
    out = a.out.resolve()
    for d in ('obj', 'logs'):
        (out / d).mkdir(parents=True, exist_ok=True)
    units = json.load(open(repo / 'build' / 'units.json'))['units']
    units = [u for u in units if a.only in u['source'] and (repo / u['source']).exists()]
    seen = set()  # a source listed under several recipes is compiled once, or the objects race
    units = [u for u in units if not (u['source'] in seen or seen.add(u['source']))]
    if a.list:
        wanted = {l.strip().replace('\\', '/') for l in a.list.read_text().splitlines() if l.strip()}
        units = [u for u in units if u['source'] in wanted]
    if a.max_lines:
        units = [u for u in units if sum(1 for _ in open(repo / u['source'], errors='replace')) <= a.max_lines]
    if a.limit:
        units = units[:a.limit]
    results = []
    with ThreadPoolExecutor(a.jobs) as ex:
        for r in ex.map(lambda u: compile_unit(cl, env, repo, out, u, a.timeout), units):
            results.append(r)
            print('OK ' if r['ok'] else 'ERR', r['source'], r['seconds'], (r['errors'] or [''])[0][:160], flush=True)
    (out / 'build_results.json').write_text(json.dumps(results, indent=1))
    ok = sum(r['ok'] for r in results)
    print('built %d of %d units' % (ok, len(results)))
    return 0 if ok == len(results) else 1


if __name__ == '__main__':
    sys.exit(main())
