#include "../../../../include/NiBSBoneLODController.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiRTTI.h"
#include "../../../../include/NiTPrimitiveArray.h"
#include "../../../../include/NiTPrimitiveSet.h"


// src/gamebryo2.2/corelibs/nianimation/nibonelodcontroller_od_variant.cpp - flag-variant unit created 2026-10-17 by flag_variants.py for src/gamebryo2.2/corelibs/nianimation/nibonelodcontroller.cpp functions whose prologue its flags cannot
// produce (ebp frame in an /O2 unit). Declarations copied from that unit's skeleton; move the functions to their real unit once identified.

// src/gamebryo2.2/corelibs/nianimation/nibonelodcontroller.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nianimation/nibonelodcontroller.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.
// The shared header supplies the controller layout. These method-only
// declarations cover implementations it does not declare yet.
struct NiBSBoneLODControllerMethods : NiBSBoneLODController
{
    bool NiTimeController_RegisterStreamables(int);
    bool NiBSBoneLODController_RegisterStreamables(int);
    NiRTTI* __thiscall NiBSBoneLODController_GetRTTI();
    void FUN_00a6cfe0(NiBSBoneLODController*, CloningProcess*);
    void NiBSBoneLODController_CopyMembers(NiBSBoneLODController*, CloningProcess*);
    void FUN_00c53e40();
    void* FUN_00c53e10(unsigned int);
    bool NiBSBoneLODController_TargetIsRequiredType();
    void __thiscall FUN_00a6d730(NiStream*);
    void __thiscall NiBSBoneLODController_SaveBinary(NiStream*);
    void FUN_00a6d500(NiStream*);
    void NiBSBoneLODController_LinkObject(NiStream*);
    void NiBSBoneLODController_GetViewerStrings(NiTPrimitiveArray<unsigned int>* pkStrings);
};
// @fnv 0x00c536a0
// src: XBOX_PDB 0x00c536a0
// pdb: NiBSBoneLODController::RegisterStreamables | NiBSBoneLODController.obj | retail, upstream name already matches, high
// pdb params: kStream
bool NiBSBoneLODControllerMethods::NiBSBoneLODController_RegisterStreamables(int kStream)
{
    if (NiTimeController_RegisterStreamables(kStream) == 0)
        return false;
    return true;
}

// @fnv 0x00c53ab0
// src: XBOX_PDB 0x00c53ab0
// pdb: NiBSBoneLODController::PostLinkObject | NiBSBoneLODController.obj | retail, vtable slot, high
// pdb params: kStream
// pdb locals: iLOD
void NiBSBoneLODController::PostLinkObject_00c53ab0(NiStream& kStream)
{
    NiObject::LinkObject(kStream);
    void* old = m_iLOD;
    m_iLOD = (void*)0xffffffff;
    NiBSBoneLODController_SetBoneLOD(old);
}

namespace fnv_00c53cd0 {
extern const char g_012031b8_arg0[];
NiObject* __cdecl NiBSBoneLODController_CreateObject();
// @fnv 0x00c53cd0
// src: GUESS 0x00c53cd0 (confidence: medium)
// pdb: NiBSBoneLODController::InitLoaders | NiBSBoneLODController.obj | debug, object-file run extension, low
int __cdecl NiStream_RegisterObjectLoader_Thunk()
{
    return NiStream::RegisterObjectLoader(g_012031b8_arg0, NiBSBoneLODController_CreateObject);
}
}  // namespace fnv_00c53cd0

extern NiRTTI g_012031b8;
// @fnv 0x00c53e00
// src: COMMUNITY 0x00c53e00
NiRTTI* __thiscall NiBSBoneLODControllerMethods::NiBSBoneLODController_GetRTTI()
{
    return &g_012031b8;
}

// @fnv 0x00c533f0
// src: XBOX_PDB 0x00c533f0
// pdb: NiBSBoneLODController::CopyMembers | NiBSBoneLODController.obj | debug, upstream name already matches, high
// pdb params: pkDest, arCloneData
void NiBSBoneLODControllerMethods::NiBSBoneLODController_CopyMembers(NiBSBoneLODController* pkDest, CloningProcess* arCloneData)
{
    FUN_00a6cfe0(pkDest, arCloneData);
    pkDest->m_iLOD = m_iLOD;
    pkDest->m_uiNumLODs = m_uiNumLODs;
}

// @fnv 0x00c53e10
// pdb: NiBSBoneLODController::`scalar deleting destructor' | NiBSBoneLODController.obj | retail, vtable slot, high
void* NiBSBoneLODControllerMethods::FUN_00c53e10(unsigned int flags)
{
    int pad[2];
    this->FUN_00c53e40();
    if (flags & 1)
        ::operator delete(this, 0x4c);
    return this;
}

// @fnv 0x00c53ed0
// src: XBOX_PDB 0x00c53ed0
// pdb: NiBSBoneLODController::TargetIsRequiredType | NiBSBoneLODController.obj | retail, upstream name already matches, high
bool NiBSBoneLODControllerMethods::NiBSBoneLODController_TargetIsRequiredType()
{
    if (m_pkTarget != 0 && m_pkTarget->IsSceneNode() != 0)
        return true;
    return false;
}

extern "C" void __cdecl CallAnimCallback(void*, const void*);
extern "C" void __cdecl NiStreamSaveBinary_unsigned_int(void*, const void*);
// @fnv 0x00c536d0
// src: XBOX_PDB 0x00c536d0
// pdb: NiBSBoneLODController::SaveBinary | NiBSBoneLODController.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: ui, pkSet, uj, pkBone
void __thiscall NiBSBoneLODControllerMethods::NiBSBoneLODController_SaveBinary(NiStream* kStream)
{
    NiTPrimitiveSet<void*>* i;
    unsigned int n;
    unsigned int groupTotal;
    void* group;
    unsigned int savedCount;
    unsigned int zero;
    unsigned int object;
    unsigned int innerCount;
    unsigned int j;

    FUN_00a6d730(kStream);
    CallAnimCallback(kStream, &m_iLOD);
    NiStreamSaveBinary_unsigned_int(kStream, &m_uiNumLODs);
    savedCount = m_kBoneArray.high;
    NiStreamSaveBinary_unsigned_int(kStream, &savedCount);

    for (n = 0; n < m_kBoneArray.high; ++n)
    {
        i = reinterpret_cast<NiTPrimitiveSet<void*>**>(m_kBoneArray.data)[n];
        if (i)
        {
            innerCount = i->m_uiUsed;
            zero = innerCount;
            NiStreamSaveBinary_unsigned_int(kStream, &zero);
            for (groupTotal = 0; groupTotal < (j = i->m_uiUsed); ++groupTotal)
            {
                group = i->m_pBase[groupTotal];
                kStream->SaveLinkID(reinterpret_cast<const NiObject*>(group));
            }
        }
        else
        {
            object = 0;
            CallAnimCallback(kStream, &object);
        }
    }
}

// @fnv 0x00c53a10
// src: XBOX_PDB 0x00c53a10
// pdb: NiBSBoneLODController::LinkObject | NiBSBoneLODController.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: ui, pkSet, uj
void NiBSBoneLODControllerMethods::NiBSBoneLODController_LinkObject(NiStream* kStream)
{
    FUN_00a6d500(kStream);
    for (unsigned int i = 0; i < m_kBoneArray.high; ++i)
    {
        NiTPrimitiveSet<unsigned int>* list = reinterpret_cast<NiTPrimitiveSet<unsigned int>**>(m_kBoneArray.data)[i];
        if (list != 0)
        {
            unsigned int value = 0;
            unsigned int size;
            unsigned int j;
            for (; value < (j = list->m_uiUsed); ++value)
            {
                size = reinterpret_cast<unsigned int>(kStream->FUN_00a63430());
                if (value >= list->m_uiUsed)
                {
                }
                else
                {
                    list->m_pBase[value] = size;
                }
            }
        }
    }
}

// @fnv 0x00c53cf0
// src: GUESS 0x00c53cf0 (confidence: low)
// pdb: NiBSBoneLODController::GetHasBones | NiBSBoneLODController.obj | debug, object-file run extension, low
// pdb locals: ui, pkSet, uj
bool NiBSBoneLODController::ProcessEntries_00c53cf0()
{
    for (unsigned int i = 0; i < m_kBoneArray.high; i++)
    {
        NiTPrimitiveSet<void*>* a = reinterpret_cast<NiTPrimitiveSet<void*>**>(m_kBoneArray.data)[i];
        if (a)
        {
            unsigned int idx;
            unsigned int size;
            for (idx = 0; idx < (size = a->m_uiUsed); idx++)
            {
                if (a->m_pBase[idx])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

extern "C" char* __cdecl NiViewerStrings_NiGetViewerString_Title(unsigned int);
extern "C" char* __cdecl NiGetViewerString_00a7d4d0(char const*, unsigned int);
extern "C" char* __cdecl NiGetViewerString_00a7d510(char const*, unsigned int);

extern "C" char str_0109a388[];
extern "C" char str_0109a394[];

// The /Od original reserves 0x30 unused stack bytes before its four string
// temporaries; these helpers describe that stack frame, not a game class.
struct ViewerStringStackPadding
{
    unsigned int p00;
    unsigned int p04;
    unsigned int p08;
    unsigned int p0c;
    unsigned int p10;
    unsigned int p14;
    unsigned int p18;
    unsigned int p1c;
    unsigned int p20;
    unsigned int p24;
    unsigned int p28;
    unsigned int p2c;
};

struct ViewerStringValues
{
    unsigned int value3;
    unsigned int value2;
    unsigned int value1;
    unsigned int value0;
};

// @fnv 0x00c53c50
// src: XBOX_PDB 0x00c53c50
// pdb: NiBSBoneLODController::GetViewerStrings | NiBSBoneLODController.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void __thiscall NiBSBoneLODControllerMethods::NiBSBoneLODController_GetViewerStrings(NiTPrimitiveArray<unsigned int>* pkStrings)
{
    ViewerStringStackPadding padding;
    ViewerStringValues values;

    values.value3 = *(unsigned int*)&g_012031b8;
    values.value0 = (unsigned int)NiViewerStrings_NiGetViewerString_Title(values.value3);
    pkStrings->AddFirstEmpty(values.value0);

    values.value1 = (unsigned int)NiGetViewerString_00a7d4d0(str_0109a394, (unsigned int)this->m_iLOD);
    pkStrings->AddFirstEmpty(values.value1);

    values.value2 = (unsigned int)NiGetViewerString_00a7d510(str_0109a388, this->m_uiNumLODs);
    pkStrings->AddFirstEmpty(values.value2);
}
