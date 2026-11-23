// Shared game classes and declarations used by the effect-setting unit.
#ifndef EFFECTSETTING_MATCH_H
#define EFFECTSETTING_MATCH_H

#include "../include/TESFile.h"
#include "../include/TESFullName.h"

class TESFullNameLoader
{
  public:
    static void LoadFullNameChunk(TESFullName*, TESFile*);
};
template <int n> struct EndianSizeTag
{
};
void BSEndian(unsigned int*, EndianSizeTag<4>);
extern "C" void* __cdecl _alloca(size_t);
extern "C" void* __cdecl memset(void*, int, size_t);
int __cdecl Warning(const char*, ...);
#endif

double NiAbs_00408820(float value);
