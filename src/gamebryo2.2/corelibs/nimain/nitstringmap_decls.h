#ifndef FNV_NITSTRINGMAP_DECLS_H
#define FNV_NITSTRINGMAP_DECLS_H

#include "../../../../include/NiTPointerMap.h"
#include "../../../../include/NiTStringTemplateMap.h"
#include "../../../../include/NiTStringPointerMap.h"

extern "C" size_t __cdecl strlen(const char*);
extern "C" int __cdecl strcpy_s(char*, size_t, const char*);
void* __cdecl _NiMalloc(unsigned int);
class NiShader;

class ShaderBufferRecord;
void __cdecl NiFree(void*);
int NiStricmp(const char* s1, const char* s2);
bool __stdcall Object_ProcessAndTransformResult_00b7fce0(const char* left, const char* right);
#endif
