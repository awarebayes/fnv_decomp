#ifndef FNV_NIPSYSBOUNDUPDATEMODIFIER_DECLS_H
#define FNV_NIPSYSBOUNDUPDATEMODIFIER_DECLS_H

#include "../../../../include/NiBound.h"
#include "../../../../include/NiPSysBoundUpdateModifier.h"
#include "../../../../include/NiPSysModifier.h"
#include "MemoryManager.h"

extern MemoryManager g_memoryManager;
extern "C" const NiPoint3 NiPoint3_ZERO;

typedef char BoundUpdate_NiBound_size_check[1];
typedef char BoundUpdate_NiPSysModifier_size_check[1];
typedef char BoundUpdate_size_check[1];
typedef char BoundUpdate_skip_offset_check[1];
typedef char BoundUpdate_bounds_offset_check[1];
#endif
