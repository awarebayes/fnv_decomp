#include "../../../../include/NiTStringPointerMap.h"
// gamebryo2.2/corelibs/niparticle/nipsysvolumeemitter.cpp: NiPSysVolumeEmitter (10 functions)
#include "NiRefObject_inline.h"

#include "../../../../include/NiPSysModifier.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiColorA.h"
#include "../../../../include/NiPSysEmitter.h"
#include "../../../../include/NiPSysVolumeEmitter.h"
#include "../../../../include/NiTransform.h"
#include <string.h>
#include "../../../../include/NiTStringPointerMap.h"

// @fnv 0x00c31b20
// pdb: NiPSysVolumeEmitter::LoadBinary | NiPSysVolumeEmitter.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiPSysVolumeEmitter::LoadBinary(NiStream& kStream)
{
    NiPSysEmitter::LoadBinary(kStream);
    kStream.ResetStreamState();
}

// @fnv 0x00c31b40
// pdb: NiPSysVolumeEmitter::LinkObject | NiPSysVolumeEmitter.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiPSysVolumeEmitter::LinkObject(NiStream& kStream)
{
    NiPSysEmitter::LinkObject(kStream);
    m_pkEmitterObj = (NiAVObject*)kStream.GetLoadedObject();
}

struct CloningProcess
{
    void __dflt_ctor_closure();
    void* __vecDelDtor(unsigned int);
    void* m_pCloneMap;
    void* m_pProcessMap;
    int m_eCloneCopyMode;
    char m_bExactType;
    char _pad_d[3];
};
typedef char _sizecheck_NiCloningProcess[1];

class ShaderBufferRecord;

// @fnv 0x00c31c90
// pdb: NiPSysVolumeEmitter::ProcessClone | NiPSysVolumeEmitter.obj | retail, upstream name already matches, high
// pdb params: kCloning
// pdb locals: bCloned, pkClone, pkDest
void NiPSysVolumeEmitter::ProcessClone(CloningProcess& kCloning)
{
    NiPSysModifier::ProcessClone(kCloning);
    NiObject* pkClone = 0;
    ((NiTStringPointerMap<ShaderBufferRecord*>*)kCloning.m_pCloneMap)
        ->GetAt((const char*)this, (ShaderBufferRecord*&)pkClone);
    NiPSysVolumeEmitter* pkDest = (NiPSysVolumeEmitter*)pkClone;
    if (m_pkEmitterObj)
    {
        if (((NiTStringPointerMap<ShaderBufferRecord*>*)kCloning.m_pCloneMap)
                ->GetAt((const char*)m_pkEmitterObj, (ShaderBufferRecord*&)pkClone))
        {
            pkDest->m_pkEmitterObj = (NiAVObject*)pkClone;
        }
        else
        {
            pkDest->m_pkEmitterObj = m_pkEmitterObj;
        }
    }
}

// Builds the emitter-to-PSys transform from the emitter and target transforms (or identity) and calls the volume emitter's virtual compute function with it.
// @fnv 0x00c31cf0
// pdb: NiPSysVolumeEmitter::ComputeInitialPositionAndVelocity | NiPSysVolumeEmitter.obj | retail, vtable slot, high
// pdb params: kPosition, kVelocity
// pdb locals: kEmitter, kEmitterToPSys, kInvPSys, kPSys
void NiPSysVolumeEmitter::GetPositionAndDirection(NiPoint3& kPosition, NiPoint3& kVelocity)
{
    if (m_pkEmitterObj)
    {
        // The original uses uninitialized scratch transforms and raw transform copies.
        unsigned int emitterStorage[sizeof(NiTransform) / sizeof(unsigned int)];
        NiTransform& kEmitter = *(NiTransform*)emitterStorage;
        memcpy(&kEmitter, (char*)m_pkEmitterObj + 0x68, sizeof(NiTransform));
        unsigned int psysStorage[sizeof(NiTransform) / sizeof(unsigned int)];
        NiTransform& kPSys = *(NiTransform*)psysStorage;
        memcpy(&kPSys, (char*)m_pkTarget + 0x68, sizeof(NiTransform));
        unsigned int inverseStorage[sizeof(NiTransform) / sizeof(unsigned int)];
        NiTransform& kInvPSys = *(NiTransform*)inverseStorage;
        kPSys.CalculateInverse(kInvPSys);
        NiTransform kEmitterToPSys = kInvPSys * kEmitter;
        vf44(kEmitterToPSys, &kPosition, &kVelocity);
    }
    else
    {
        unsigned int identityStorage[sizeof(NiTransform) / sizeof(unsigned int)];
        NiTransform& kEmitterToPSys = *(NiTransform*)identityStorage;
        kEmitterToPSys.ResetToIdentity();
        vf44(kEmitterToPSys, &kPosition, &kVelocity);
    }
}

extern char const* const g_01202508_arg1;

// @fnv 0x00c31bc0
// src: COMMUNITY 0x00c31bc0
char const* const* __cdecl NiPSysVolumeEmitter_GetRTTI()
{
    return &g_01202508_arg1;
}

void __cdecl FUN_00c31750(void);

extern "C" void __cdecl operator delete(void*, unsigned int);

// @fnv 0x00c31c70
void* NiPSysVolumeEmitter::DestroyIfFlagged(unsigned int flags)
{
    FUN_00c31750();
    if (flags & 1)
        ::operator delete(this, 0x58);
    return this;
}

// @fnv 0x00c31b60
// pdb: NiPSysVolumeEmitter::SaveBinary | NiPSysVolumeEmitter.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiPSysVolumeEmitter::SaveBinary(NiStream& kStream)
{
    NiPSysEmitter::SaveBinary(kStream);
    kStream.SaveLinkID(reinterpret_cast<const NiObject*>(m_pkEmitterObj));
}

// @fnv 0x00c31b80
// pdb: NiPSysVolumeEmitter::IsEqual | NiPSysVolumeEmitter.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkDest
bool NiPSysVolumeEmitter::IsEqual(NiObject* pkObject) const
{
    NiPSysVolumeEmitter* pkDest = (NiPSysVolumeEmitter*)pkObject;
    if (!NiPSysEmitter::IsEqual(pkObject))
        return false;
    if (m_pkEmitterObj)
    {
        if (!pkDest->m_pkEmitterObj)
            return false;
    }
    if (!m_pkEmitterObj)
    {
        if (pkDest->m_pkEmitterObj)
            return false;
    }
    return true;
}

struct NameArray_b9457
{
    char unk00[8];
    unsigned short m_usMax;
    unsigned short m_usNum;
    char unk0c[2];
    unsigned short m_usGrow;
    void Resize(unsigned int newSize);
    void SetAt(unsigned int index, void* item);
};

struct NameHolder_b9457
{
    char unk00[8];
    void* m_pIntegerData;
};

extern void* DAT_012027d8;
void* __cdecl NiViewerStrings_NiGetViewerString_Title(void* p);
void* __cdecl NiGetViewerString_00a7d5e0(const char* pcPrefix, void* pcValue);

// @fnv 0x00c31db0
// pdb: NiPSysVolumeEmitter::GetViewerStrings | NiPSysVolumeEmitter.obj | retail, string-set bridge, high
// pdb params: pkStrings
void NiPSysVolumeEmitter::GetViewerStrings(NiViewerStringsArray* pkStrings)
{
    NameArray_b9457* arr = (NameArray_b9457*)pkStrings;
    NiPSysEmitter::GetViewerStrings(pkStrings);
    void* name = NiViewerStrings_NiGetViewerString_Title(DAT_012027d8);
    unsigned int uiIndex = arr->m_usNum;
    if (uiIndex >= arr->m_usMax)
    {
        arr->Resize(uiIndex + arr->m_usGrow);
    }
    arr->SetAt(uiIndex, &name);
    if (m_pkEmitterObj != 0)
    {
        void* holderVal = ((NameHolder_b9457*)m_pkEmitterObj)->m_pIntegerData;
        name = NiGetViewerString_00a7d5e0("Emitter Object", holderVal);
        uiIndex = arr->m_usNum;
        if (uiIndex >= arr->m_usMax)
        {
            arr->Resize(uiIndex + arr->m_usGrow);
        }
        arr->SetAt(uiIndex, &name);
    }
}

NiPSysVolumeEmitter::NiPSysVolumeEmitter(const char* pcName, NiAVObject* pkEmitterObj, float fSpeed, float fSpeedVar,
                           float fDeclination, float fDeclinationVar, float fPlanarAngle, float fPlanarAngleVar,
                           NiColorA kInitialColor, float fInitialRadius, float fLifeSpan, float fLifeSpanVar,
                           float fRadiusVar)
    // Constructs a particle system modifier by delegating to the base class and storing the emitter object pointer.
// @fnv 0x00c31bd0
// pdb: NiPSysVolumeEmitter::NiPSysVolumeEmitter | NiPSysVolumeEmitter.obj | debug, call graph, medium
// pdb params: pcName, pkEmitterObj, fSpeed, fSpeedVar, fDeclination, fDeclinationVar, fPlanarAngle, fPlanarAngleVar, kInitialColor, fInitialRadius, fLifeSpan, fLifeSpanVar, fRadiusVar
    : NiPSysEmitter(pcName, 1000, fSpeed, fSpeedVar, fDeclination, fDeclinationVar, fPlanarAngle, fPlanarAngleVar,
                        kInitialColor, fInitialRadius, fLifeSpan, fLifeSpanVar, fRadiusVar)
{
    m_pkEmitterObj = pkEmitterObj;
}
