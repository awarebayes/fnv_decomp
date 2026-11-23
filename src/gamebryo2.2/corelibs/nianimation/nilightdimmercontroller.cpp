
// src/gamebryo2.2/corelibs/nianimation/nilightdimmercontroller.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nianimation/nilightdimmercontroller.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/BSFadeNode.h"
#include "../../../../include/NiFloatInterpController.h"
#include "../../../../include/NiLight.h"
#include "../../../../include/NiLightDimmerController.h"

extern "C" char __stdcall FUN_00a57c10(void* pkObject);

// @fnv 0x00a4cf50
// src: XBOX_PDB 0x00a4cf50
// pdb: NiLightDimmerController::IsEqual | NiLightDimmerController.obj | retail, upstream name already matches, high
// pdb params: pkObject
char __stdcall NiLightDimmerController_IsEqual(void* pkObject)
{
    return FUN_00a57c10(pkObject) != 0;
}

// @fnv 0x00a4cf70
void NiLightDimmerController::GetLightDimmer(float& fValue)
{
    NiLight* apTargetLight = static_cast<NiLight*>(m_pkTarget);
    if (apTargetLight)
    {
        fValue = apTargetLight->m_fDimmer;
    }
}

struct GlobalObjectO2_00a7b610;

extern GlobalObjectO2_00a7b610 NiLightDimmerController_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011f3fa0 (old: g_011f3fa0)

// @fnv 0x00a4cf90
// src: COMMUNITY 0x00a4cf90
GlobalObjectO2_00a7b610* __cdecl NiLightDimmerController_GetRTTI()
{
    return &NiLightDimmerController_ms_RTTI;
}

extern int DAT_01098ccc;
inline void* __cdecl operator new(size_t, void* p)
{
    return p;
}
// @fnv 0x00a4d0d0
// src: EXE 0x00a4d0d0
// pdb: NiLightDimmerController::CreateObject | NiLightDimmerController.obj | retail, upstream name already matches, high
// pdb locals: pkObject
void* NiLightDimmerController_CreateObject()
{
    void* p = BSFadeNode::operator new(0x38);
    if (p)
    {
        ::new (p) NiFloatInterpController();
        *(void**)p = &DAT_01098ccc;
        return p;
    }
    return 0;
}

#include "../../../../include/NiTPrimitiveArray.h"
void __stdcall NiFloatInterpController_GetViewerStrings(NiTPrimitiveArray<char*>*);
char* __cdecl NiViewerStrings_NiGetViewerString_Title(const char*);
// @fnv 0x00a4d100
// src: XBOX_PDB 0x00a4d100
// pdb: NiLightDimmerController::GetViewerStrings | NiLightDimmerController.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void __stdcall NiLightDimmerController_GetViewerStrings(NiTPrimitiveArray<char*>* pkStrings)
{
    NiFloatInterpController_GetViewerStrings(pkStrings);
    char* pszStr = NiViewerStrings_NiGetViewerString_Title(*(const char**)&NiLightDimmerController_ms_RTTI);
    unsigned int uIdx = pkStrings->m_usSize;
    if (uIdx >= pkStrings->m_usMaxSize)
        pkStrings->SetSize(uIdx + pkStrings->m_usGrowBy);
    pkStrings->SetAt(uIdx, pszStr);
}

extern "C" void* __cdecl Object_DispatchCommandFour(unsigned int size);

struct FloatInterpOwner
{
    void FUN_00a57620(void* obj, int arg);
    void* CreateFloatInterpController(int arg);
};

// @fnv 0x00a4d080
// pdb: NiLightDimmerController::CreateClone | NiLightDimmerController.obj | retail, object-file run extension, low
// pdb note: debug build links it to NiLightDimmerController::CopyMembers
// pdb params: kCloning
// pdb locals: pkObject
void* FloatInterpOwner::CreateFloatInterpController(int arg)
{
    NiFloatInterpController* obj = (NiFloatInterpController*)Object_DispatchCommandFour(0x38);
    if (obj != 0)
    {
        obj->NiFloatInterpController::NiFloatInterpController();
        *(int*)obj = (int)&DAT_01098ccc;
    }
    else
    {
        obj = 0;
    }
    FUN_00a57620(obj, arg);
    return obj;
}

extern NiRTTI DAT_011f4a28;
extern const float NiInterpController_INVALID_TIME; // src: XBOX_RETAIL_PDB 0x01096324 (old: DAT_01096324/INVALID_TIME_A4F/g_fInvalidTimeBC3/kInvalidTime)

inline bool GetManagerControlled(const NiLightDimmerController* controller)
{
    return ((controller->m_uiObjectFlags >> 5) & 1) != 0;
}

// @fnv 0x00a4cfa0
// pdb: NiLightDimmerController::Update | NiLightDimmerController.obj | retail, object-file run extension, low
// pdb params: UpdateData
// pdb locals: fValue, pLight
void __fastcall UpdateDimmer(NiLightDimmerController* controller, void*, const float& fTime)
{
    if (GetManagerControlled(controller))
    {
        controller->m_fScaledTime = NiInterpController_INVALID_TIME;
    }
    else if (controller->DontDoUpdate(fTime) && (!controller->m_spInterpolator.m_pObject || !controller->m_spInterpolator.m_pObject->AlwaysUpdate()))
    {
        return;
    }

    if (controller->m_spInterpolator.m_pObject)
    {
        float fValue;
        if (controller->m_spInterpolator.m_pObject->UpdateFloat(controller->m_fScaledTime, controller->m_pkTarget, fValue))
        {
            if (controller->m_pkTarget)
            {
                NiLight* pkLight = static_cast<NiLight*>(controller->m_pkTarget->DynamicCast(&DAT_011f4a28));
                pkLight->m_fDimmer = fValue;
                pkLight->revID++;
            }
        }
    }
}

extern void FUN_00a57c70();

// @fnv 0x00a4cf40
// src: XBOX_PDB 0x00a4cf40
// pdb: NiLightDimmerController::LoadBinary | NiLightDimmerController.obj | debug, upstream name already matches, high
// pdb params: kStream
void NiLightDimmerController_LoadBinary()
{
    FUN_00a57c70();
}

#include <windows.h>
extern int DAT_01098ccc;

void __cdecl FUN_00aa1460(void* apMemory, unsigned int auiSize);
extern void BaseCleanup();

// Releases the interpolator, cleans up the base, and frees the controller when bit 0 is set.
// @fnv 0x00a4d030
// pdb: NiLightDimmerController::`scalar deleting destructor' | NiLightDimmerController.obj | retail, object-file run extension, low
// pdb note: debug build links it to NiLightDimmerController::CreateClone
NiLightDimmerController* __fastcall FUN_00a4d030(NiLightDimmerController* controller, void*, unsigned char acFlags)
{
    *(volatile int*)controller = (int)&DAT_01098ccc;
    NiInterpolator* pCounted = *(NiInterpolator* volatile*)&controller->m_spInterpolator.m_pObject;
    if (pCounted != 0)
    {
        if (InterlockedDecrement((long volatile*)&pCounted->m_uiRefCount) == 0)
        {
            pCounted->DeleteThis();
        }
    }
    reinterpret_cast<void (__thiscall*)(NiLightDimmerController*)>(&BaseCleanup)(controller);
    if (acFlags & 1)
    {
        FUN_00aa1460(controller, 0x38);
    }
    return controller;
}
