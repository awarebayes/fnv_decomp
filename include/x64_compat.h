// Force-included when the tree is compiled as x64 (see tools/x64/build_x64.py).
// The sources target a 32-bit compiler; this header absorbs what a 64-bit target cannot express.
#ifndef X64_COMPAT_H
#define X64_COMPAT_H
#ifdef _WIN64
#define FNV_X64 1
#include <stddef.h>
// The compiler declares this name itself for classes with a vftable; the sources declare it by hand.
#define __local_vftable_ctor_closure x64_local_vftable_ctor_closure
// Prototypes of x86-only or header-provided intrinsics the sources call without including <intrin.h>
// (forcing that header in conflicts with their own declarations).
extern "C" long _InterlockedIncrement(long volatile*);
extern "C" long _InterlockedDecrement(long volatile*);
extern "C" unsigned __int64 __readgsqword(unsigned long);
// fs:[0x2c] is the thread-local-storage array on x86; on x64 it is gs:[0x58].
#define __readfsdword(offset) ((size_t)__readgsqword(0x58))
#endif
#endif
