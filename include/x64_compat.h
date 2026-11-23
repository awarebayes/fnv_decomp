// Force-included when the tree is compiled as x64 (see tools/x64/build_x64.py).
// The sources target a 32-bit compiler; this header absorbs what a 64-bit target cannot express.
#ifndef X64_COMPAT_H
#define X64_COMPAT_H
#ifdef _WIN64
#define FNV_X64 1
#include <stddef.h>
#endif
#endif
