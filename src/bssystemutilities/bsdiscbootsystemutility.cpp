// bssystemutilities/bsdiscbootsystemutility.cpp: BSDiscBootSystemUtility (3 functions)

#include "../../include/BSDiscBootSystemUtility.h"
#include "../../include/MemoryManager.h"

extern "C" int __cdecl strcpy_s(char*, size_t, const char*);

// @fnv 0x00af3ae0
// pdb: BSDiscBootSystemUtility::Destroy | BSDiscBootSystemUtility.obj | debug, upstream name already matches, high
void BSDiscBootSystemUtility::Destroy()
{}

// @fnv 0x00af3aa0
// pdb: BSDiscBootSystemUtility::Initialize | BSDiscBootSystemUtility.obj | retail, upstream name already matches, high
// pdb params: apcDefaultTitleID, auiDefaultParentalLevel
void BSDiscBootSystemUtility::Initialize(const char* apcDefaultTitleID, unsigned int auiDefaultParentalLevel)
{
    Destroy();
    if (apcDefaultTitleID)
    {
        strcpy_s(cTitleID, 0x40, apcDefaultTitleID);
    }
    uiParentalLevel = auiDefaultParentalLevel;
}

extern MemoryManager g_memoryManager;
// The original destructor resets the vtable and expects this in ECX.
extern "C" void BSDiscBootSystemUtility_ResetVtable();
// @fnv 0x00af3af0
// pdb: BSDiscBootSystemUtility::`scalar deleting destructor' | BSDiscBootSystemUtility.obj | debug, object-file run extension, low
void* BSDiscBootSystemUtility::vf00(unsigned int flags)
{
    ;
    if (flags & 1)
        g_memoryManager.Deallocate(this);
    return this;
}
