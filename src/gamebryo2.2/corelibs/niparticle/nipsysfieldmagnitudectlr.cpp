// src/gamebryo2.2/corelibs/niparticle/nipsysfieldmagnitudectlr.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/niparticle/nipsysfieldmagnitudectlr.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/NiPSysModifierFloatCtlr.h"
#include "../../../../include/StrArr.h"

// pdb: class NiPSysFieldMagnitudeCtlr, size 0x40, bases: NiPSysModifierFloatCtlr, 0 members
struct NiPSysFieldMagnitudeCtlr : NiPSysModifierFloatCtlr
{
    void __thiscall UpdateFields_00c29ea0(float& fValue);
};

// @fnv 0x00c29ea0
// src: GUESS 0x00c29ea0 (confidence: low)
void __thiscall NiPSysFieldMagnitudeCtlr::UpdateFields_00c29ea0(float& fValue)
{
    fValue = *(float*)((char*)m_pkModifier + 0x1C);
}

extern "C" void* NiPSysFieldMagnitudeCtlr_ms_RTTI; // src: XBOX_RETAIL_PDB 0x01202718 (old: g_01202718)

// @fnv 0x00c2b140
// src: COMMUNITY 0x00c2b140
void* __cdecl NiPSysFieldMagnitudeCtlr_GetRTTI()
{
    return &NiPSysFieldMagnitudeCtlr_ms_RTTI;
}

struct NiPSysFieldMagnitudeCtlr_shell : NiPSysFieldMagnitudeCtlr
{
    void __thiscall SetTargetModifierFloat7(float value);
};

// @fnv 0x00c2b150
// src: GUESS 0x00c2b150 (confidence: medium)
void __thiscall NiPSysFieldMagnitudeCtlr_shell::SetTargetModifierFloat7(float value)
{
    ((float*)m_pkModifier)[7] = value;
}

void __stdcall NiPSysModifierFloatCtlr_GetViewerStrings(StrArr*);
char* __cdecl NiViewerStrings_NiGetViewerString_Title(const char*);
// @fnv 0x00c2b1b0
// src: XBOX_PDB 0x00c2b1b0
// pdb: NiPSysFieldMagnitudeCtlr::GetViewerStrings | NiPSysFieldMagnitudeCtlr.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void __stdcall NiPSysFieldMagnitudeCtlr_GetViewerStrings(StrArr* pkStrings)
{
    NiPSysModifierFloatCtlr_GetViewerStrings(pkStrings);
    char* pcStr = NiViewerStrings_NiGetViewerString_Title((const char*)NiPSysFieldMagnitudeCtlr_ms_RTTI);
    unsigned int uiSize = pkStrings->m_usSize;
    unsigned int uiCap = pkStrings->m_usMaxSize;
    if (uiSize >= uiCap)
        pkStrings->SetSize(pkStrings->m_usGrowBy + uiSize);
    pkStrings->SetAt(uiSize, pcStr);
}

struct Base
{
    int unk04;
    Base() throw();
    virtual void vf();
};

struct Node : Base
{
    int unk08[14];
    static void* operator new(size_t size) throw();
    Node() throw()
    {}
    virtual void vf();
};

struct Owner
{
    void FUN_00c32de0(Node* node, int arg);
    Node* NiPSysFieldMagnitudeCtlr_CreateClone(int kCloning);
};

// @fnv 0x00c2b160
// src: XBOX_PDB 0x00c2b160
// pdb: NiPSysFieldMagnitudeCtlr::CreateClone | NiPSysFieldMagnitudeCtlr.obj | retail, upstream name already matches, high
// pdb params: kCloning
// pdb locals: pkObject
Node* Owner::NiPSysFieldMagnitudeCtlr_CreateClone(int kCloning)
{
    Node* node = new Node();
    FUN_00c32de0(node, kCloning);
    return node;
}
