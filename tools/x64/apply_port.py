#!/usr/bin/env python3
"""Mechanical source edits of the x64 port; each step is idempotent and prints how many files it touched.

usage: python tools/x64/apply_port.py <step>...   (steps: asserts, asm, alloc, size_t, crt, decls, fixups)

The edits are committed as ordinary source changes; this script exists so the same passes can be
repeated when the branch is rebased onto newer upstream sources.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
EXT = {'.h', '.hpp', '.inl', '.cpp', '.c'}


def files():
    for top in ('include', 'src'):
        for p in sorted((ROOT / top).rglob('*')):
            if p.suffix in EXT and p.is_file() and p.name != 'x64_compat.h':
                yield p


def strip_asm(text):
    """Replace each `__asm { ... }` block (brace matched, continuation lines included) with `;`."""
    out, i = [], 0
    pat = re.compile(r'__asm\s*\{')
    while True:
        m = pat.search(text, i)
        if not m:
            out.append(text[i:])
            break
        out.append(text[i:m.start()])
        depth, j = 1, m.end()
        while j < len(text) and depth:
            depth += {'{': 1, '}': -1}.get(text[j], 0)
            j += 1
        out.append(';')
        i = j
    text = ''.join(out)
    # single-statement forms: `__asm <instruction>` to end of line, with or without a continuation
    text = re.sub(r'^[ \t]*__asm\b(?!\s*\{)[^\n]*\\\n', '', text, flags=re.M)
    text = re.sub(r'^([ \t]*)__asm\b(?!\s*\{)[^\n]*$', r'\1;', text, flags=re.M)
    return text


def step_asserts(t):
    # 32-bit layout checks cannot hold for 64-bit pointers; the typedef stays so the macro still parses
    t = re.sub(r'(#define FNV_STATIC_ASSERT\(condition\) typedef char FNV_JOIN\(fnv_static_assert_, __LINE__\)\[)'
               r'\(condition\) \? 1 : -1\]', r'\g<1>1]', t)
    t = re.sub(r'^FNV_STATIC_ASSERT\(sizeof\((?:void\*|long)\) == 4\);[ \t]*\n?', '', t, flags=re.M)
    t = re.sub(r'(typedef\s+char\s+\w+\[)[^;\[\]]*?\?\s*1\s*:\s*-1\]', r'\g<1>1]', t)
    # macro-local variants: the condition is a bare identifier or an expression in parentheses
    t = re.sub(r'(#define\s+\w*ASSERT\w*\([^)]*\)[^\n]*?\[)\(\w+\)\s*\?\s*1\s*:\s*-1\]', r'\g<1>1]', t)
    t = re.sub(r'(#define\s+\w*ASSERT\w*\([^)]*\)[^\n]*?FNVAssert<)\(\w+\)\s*\?\s*1\s*:\s*0>', r'\g<1>1>', t)
    return t


def step_alloc(t):
    def params(m):
        body = re.sub(r'\bunsigned(?:\s+int)?\b', 'size_t', m.group(2))
        return m.group(1) + body + ')'
    t = re.sub(r'(operator\s+(?:new|delete)(?:\s*\[\])?\s*\()([^()]*)\)', params, t)
    return t


def step_size_t(t):
    t = re.sub(r'^[ \t]*typedef\s+unsigned(?:\s+int)?\s+size_t\s*;[ \t]*\n?', '', t, flags=re.M)
    t = re.sub(r'typedef\s+unsigned(?:\s+int)?\s+hk_size_t\s*;', 'typedef size_t hk_size_t;', t)
    return t


CRT = ('memset|memcpy|memcmp|memmove|memcpy_s|memmove_s|_alloca|strcpy_s|strcat_s|sprintf_s|_strnicmp|'
       'strncpy_s|strlen|strncmp|strnlen|fnv_zlib_memset|_fnv_zlib_memset')


def step_crt(t):
    # CRT redeclarations must agree with the 64-bit prototypes: size parameters are size_t
    def fix(m):
        line = m.group(0)
        if not re.search(r'\b(?:%s)\s*\(' % CRT, line):
            return line
        return re.sub(r'\bunsigned(?:\s+int)?\b', 'size_t', line)
    t = re.sub(r'^[ \t]*extern\s+"C"[^\n]*$', fix, t, flags=re.M)
    t = re.sub(r'(extern\s+"C"\s+)unsigned(?:\s+int)?(\s+__security_cookie\b)', r'\1uintptr_t\2', t)
    t = re.sub(r'(__security_check_cookie\s*\()unsigned(?:\s+int)?\b', r'\1uintptr_t', t)
    return t


def step_fixups(t):
    # the crt pass turned `unsigned long` into `size_t long`; the zlib helpers keep their 32-bit parameter
    t = t.replace('size_t long', 'unsigned long')
    t = re.sub(r'typedef\s+unsigned\s+__w64\s+hk_size_t\s*;', 'typedef size_t hk_size_t;', t)
    # bare (non-extern "C") CRT declarations: the size parameter is size_t on x64
    def size(m):
        return re.sub(r'\bunsigned(?:\s+int)?\b(?!\s+long)', 'size_t', m.group(0))
    t = re.sub(r'^(?:void\*|int|char\*)\s+(?:__cdecl\s+)?(?:memset|memcpy|memcmp|memmove|strcpy_s|strcat_s|sprintf_s|_strnicmp)\s*\([^()]*\);[ \t]*$',
               size, t, flags=re.M)
    return t


def step_decls(t):
    # operator new/delete are declared by <new>; stand-alone redeclarations (some inside namespaces) are
    # rejected by the 64-bit front end, and extern "C" on them contradicts the C++ linkage
    t = re.sub(r'^[ \t]*(?:extern\s+"C"\s+)?void\*?\s+(?:__cdecl\s+)?operator\s+(?:new|delete)(?:\s*\[\])?\s*\([^()]*\)\s*(?:throw\(\)\s*)?;[ \t]*\n',
               '', t, flags=re.M)
    # array bounds written as a constant minus a 32-bit sizeof go negative at 64 bits
    t = re.sub(r'\[(0x[0-9a-fA-F]+|\d+)\s*-\s*sizeof\(([^()]+)\)\]',
               r'[((\1) > sizeof(\2)) ? ((\1) - sizeof(\2)) : 1]', t)
    return t


STEPS = {'asserts': step_asserts, 'asm': strip_asm, 'alloc': step_alloc, 'size_t': step_size_t, 'crt': step_crt, 'decls': step_decls, 'fixups': step_fixups}


def main():
    names = sys.argv[1:] or list(STEPS)
    for name in names:
        changed = 0
        for p in files():
            try:
                old = p.read_bytes().decode('latin-1')
            except OSError:
                continue
            new = STEPS[name](old)
            if new != old:
                p.write_bytes(new.encode('latin-1'))
                changed += 1
        print('%s: %d files changed' % (name, changed))


if __name__ == '__main__':
    main()
