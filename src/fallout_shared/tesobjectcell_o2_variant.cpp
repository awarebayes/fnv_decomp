
// src/fallout_shared/tesobjectcell_o2_variant.cpp - flag-variant unit created 2026-10-17 by flag_variants.py for src/fallout_shared/tesobjectcell.cpp functions whose prologue its flags cannot
// produce (frameless code in an /Od unit). Declarations copied from that unit's skeleton; move the functions to their real unit once identified.

#include "tesobjectcell_packet.h"
const char kTESObjectCELLCellRefLockEnter[] = "TESObjectCELL::CellRefLockEnter()";
unsigned int TESTerrainLODManager_GetGrassNode();
NiAVObject* TESMain_GetWeatherRoot();

struct FUN_00b5a9a0_Node
{
    void* m_pListObject;
    void* m_pNext;
    unsigned int m_uiCount;
};

struct FUN_00b5a9a0_List
{
    FUN_00b5a9a0_Node* m_pHead;
    FUN_00b5a9a0_Node* m_pTail;
    unsigned int m_uiCount;
    void __thiscall NiTListBase_AddNodeTail(FUN_00b5a9a0_Node* node);
};

// @fnv 0x00b5a9a0
// src: COMMUNITY 0x00b5a9a0
void __thiscall FUN_00b5a9a0_List::NiTListBase_AddNodeTail(FUN_00b5a9a0_Node* node)
{
    node->m_pListObject = 0;
    node->m_pNext = m_pTail;
    if (m_pTail)
    {
        m_pTail->m_pListObject = node;
        ++m_uiCount;
        m_pTail = node;
    }
    else
    {
        ++m_uiCount;
        m_pHead = node;
        m_pTail = node;
    }
}

void* __cdecl _memmove(void* dst, const void* src, unsigned int count);
void* __cdecl _fnv_zlib_memset(void* ptr, int value, unsigned int count);

struct FUN_00bb39e0_shell
{
    int m_00;
    int m_04;
    void* m_08;
    void STLSPData_SetLeafMaps(const void* apLeafMaps, unsigned int aiCount);
};

// @fnv 0x00bb39e0
// src: XBOX_DEBUG_PDB 0x00bb39e0
// pdb: STLSPData::SetLeafMaps | SpeedTreeLeafShaderProperty.obj | debug, call graph (shared callers), high
// pdb params: apLeafMaps, aiCount
// pdb locals: icount
void FUN_00bb39e0_shell::STLSPData_SetLeafMaps(const void* apLeafMaps, unsigned int aiCount)
{
    if (apLeafMaps && m_08)
    {
        if (aiCount)
        {
            if (aiCount >= 192)
                aiCount = 192;
            _memmove(m_08, apLeafMaps, aiCount * 4);
            if (aiCount < 192)
                _fnv_zlib_memset((char*)m_08 + aiCount * 4, 0, (192 - aiCount) * 4);
        }
    }
}
