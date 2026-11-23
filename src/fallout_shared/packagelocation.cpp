// Distinct original implementations share reconstructed identifiers.
// Keep each matched body under its own address-qualified name.
#define SetMustComplete SetMustComplete_fnv_00826b90

// fallout_shared/packagelocation.cpp: LocationDescriptor, TESPackage, BGSMessageInitAccess, OwnQ (31 functions)

#include "../../include/BGSCameraPath.h"
#include "../../include/BaseProcess.h"
#include "../../include/MobileObject.h"
#include "../../include/TESObjectCELL.h"
#include "../../include/TESObjectREFR.h"
#include "../../include/TESPackage.h"
#include "packagelocation_decls.h"
#include "../../include/ActiveEffect.h"
#include "../../include/Actor.h"
#include "../../include/BSFaceGenAnimationData.h"
#include "../../include/BSThread.h"
#include "../../include/Creature.h"
#include "../../include/MagicTarget.h"
#include "../../include/TESNPC.h"
#include "../../include/NiMatrix3.h"
#include "../../include/bhkCharacterController.h"
#include "../../include/bhkRigidBody.h"
#include "../../include/hkRotation.h"
#include "../../include/NiNode.h"
#include "../../include/ExtraDataList.h"
#include "../../include/TESFullName.h"
#include "../../include/TESDescription.h"

#define FNV_JOIN_IMPL(a, b) a##b
#define FNV_JOIN(a, b) FNV_JOIN_IMPL(a, b)
#define FNV_STATIC_ASSERT(condition) typedef char FNV_JOIN(fnv_static_assert_, __LINE__)[1]
#include "packagelocation_package.h"

int Warning(const char* format, ...);

// @fnv 0x0067f390
TESObjectREFR* LocationDescriptor::GetLocationReference()
{
    if (eLocType == 0)
        return pRef;
    else
        return 0;
}

// @fnv 0x0067f3e0
TESObjectCELL* LocationDescriptor::GetLocationCellReference()
{
    if (eLocType == 1)
        return pCell;
    else
        return 0;
}

// @fnv 0x0067f3c0
// src: XBOX_PDB 0x0067f3c0
void LocationDescriptor::PackageLocation_SetLocReference(TESObjectREFR* reference)
{
    eLocType = PLT_REFERENCE;
    pRef = reference;
}

// Resolves the stored form ID to a cell, object or reference by location type and stores it, warning if the form is missing.
// @fnv 0x0067f760
// pdb: PackageLocation::InitItem | PackageLocation.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: tempint, pCell, pobj, pRef
void LocationDescriptor::InitItem(TESForm* apOwner)
{
    if (GetLocType() == PLT_TYPE)
        return;

    unsigned int formID = reinterpret_cast<unsigned int>(m_pLocationObject);
    TESForm::RemapFormIDForFile(formID, apOwner->GetFile(-1));
    switch (eLocType)
    {
    case PLT_CELL: {
        TESObjectCELL* cell = dynamic_cast<TESObjectCELL*>(TESForm::GetFormByID(formID));
        if (cell)
            PackageLocation_SetLocCell(cell);
        else
        {
            if (static_cast<BGSCameraPath*>(apOwner)->HasNameForWarning())
                Warning("MASTERFILE: Unable to find Package Location Cell (%08X) on owner object \"%s\".", formID,
                        apOwner->GetName());
            else
                Warning("MASTERFILE: Unable to find Package Location Cell (%08X) on owner object (%08X).", formID,
                        apOwner->GetFormID());
            PackageLocation_SetLocCell(0);
        }
    }
    break;
    case PLT_OBJECT: {
        TESForm* object = TESForm::GetFormByID(formID);
        if (object)
            SetValue(object);
        else
        {
            if (static_cast<BGSCameraPath*>(apOwner)->HasNameForWarning())
                Warning("MASTERFILE: Unable to find Package Location Object (%08X) on owner object \"%s\".", formID,
                        apOwner->GetName());
            else
                Warning("MASTERFILE: Unable to find Package Location Object (%08X) on owner object (%08X).", formID,
                        apOwner->GetFormID());
            SetValue(0);
        }
    }
    break;
    case PLT_REFERENCE: {
        TESObjectREFR* reference = dynamic_cast<TESObjectREFR*>(TESForm::GetFormByID(formID));
        if (reference)
            PackageLocation_SetLocReference(reference);
        else
        {
            if (static_cast<BGSCameraPath*>(apOwner)->HasNameForWarning())
                Warning("MASTERFILE: Unable to find Package Location Reference (%08X) on owner object \"%s\".", formID,
                        apOwner->GetName());
            else
                Warning("MASTERFILE: Unable to find Package Location Reference (%08X) on owner object (%08X).", formID,
                        apOwner->GetFormID());
            PackageLocation_SetLocReference(0);
        }
    }
    break;
    }
}

// Returns a TESObjectREFR based on the location type: the object itself, its linked ref, a stored reference, or a process reference.
// @fnv 0x0067f2a0
// pdb: PackageLocation::GetRunningPackageLocReference | PackageLocation.obj | retail, call graph (shared callers), high
// pdb params: apActor
// pdb locals: pRef, pref
TESObjectREFR* LocationDescriptor::ResolveLocationReference(MobileObject* apActor)
{
    TESObjectREFR* reference = 0;
    TESObjectREFR* processReference;
    if (GetLocType() != PLT_CURRLOC)
    {
        switch (GetLocType())
        {
        case PLT_CELL:
        case PLT_CURRLOC:
            break;
        case PLT_REFERENCE:
            if (GetLocationReference())
                reference = GetLocationReference();
            break;
        case PLT_LINKEDREF:
            if (apActor && apActor->GetLinkedRef())
                reference = apActor->GetLinkedRef();
            break;
        case PLT_EDITORLOC:
            if (apActor)
                reference = apActor;
            break;
        case PLT_OBJECT:
        case PLT_TYPE: {
            processReference = reinterpret_cast<TESObjectREFR*>(
                apActor->FUN_008d8520()->GetActiveCombatTarget());
            if (processReference)
                reference = processReference;
            else
                reference = apActor;
        }
        break;
        }
    }
    else if (apActor)
        reference = apActor;
    return reference;
}

// @fnv 0x00826b40
void TESPackage::SetMustReachLocation(bool enabled)
{
    enabled ? PackData |= 2 : PackData &= ~2;
}

// @fnv 0x00826b90
void TESPackage::SetMustComplete(bool enabled)
{
    enabled ? PackData |= 4 : PackData &= ~4;
}

// @fnv 0x00984f60
// pdb: FORenderedTerminal::FORenderedTerminal | Interface.obj | debug, call graph, medium
void TESPackage::SetProcedureType(PACKAGE_PROCEDURE_TYPE procedure)
{
    ePROCEDURE_TYPE = procedure;
}

// @fnv 0x0055b980
unsigned int BGSMessageInitAccess::GetIconFormID()
{
    return iconFormID;
}

// Sets the first byte of the object to 3 and zeroes the two 32-bit fields at offsets 4 and 8, then returns the object.
// @fnv 0x0067f030
// src: GUESS 0x0067f030 (confidence: low)
LocationDescriptor* __fastcall Packagelocation_UpdateState_0067f030(LocationDescriptor* pObject)
{
    *(char*)pObject = 3;
    *(unsigned int*)((char*)pObject + 4) = 0;
    *(unsigned int*)((char*)pObject + 8) = 0;
    return pObject;
}

struct FUN_0067f110_shell : public LocationDescriptor
{
    void InitializeFields_0067f110();
};

// @fnv 0x0067f110
// src: GUESS 0x0067f110 (confidence: low)
void FUN_0067f110_shell::InitializeFields_0067f110()
{
    eLocType = -1;
    iRad = 0;
    pObject = 0;
}

struct FUN_0067f140_shell : public LocationDescriptor
{
    void PackageLocation_SetLocType(int);
};

// @fnv 0x0067f140
// src: XBOX_PDB 0x0067f140
void FUN_0067f140_shell::PackageLocation_SetLocType(int aeLoctype)
{
    if (aeLoctype != eLocType)
    {
        eLocType = aeLoctype;
        switch (eLocType)
        {
        case 0:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            pObject = 0;
            break;
        case 1:
            pObject = 0;
            iRad = 0;
            break;
        }
    }
}

struct FUN_0067f1c0_shell : public LocationDescriptor
{
    unsigned int PackageLocation_GetRadius();
};

// @fnv 0x0067f1c0
// src: XBOX_PDB 0x0067f1c0
unsigned int FUN_0067f1c0_shell::PackageLocation_GetRadius()
{
    switch (eLocType)
    {
    case -1:
    case 1:
        return 0;
    default:
        return iRad;
    }
}

struct FUN_0067f1f0_shell : public LocationDescriptor
{
    void PackageLocation_SetRadius(unsigned int);
};

// Stores aiRad at offset 4 unless the byte at offset 0 is -1 or 1.
// @fnv 0x0067f1f0
// src: XBOX_PDB 0x0067f1f0
void FUN_0067f1f0_shell::PackageLocation_SetRadius(unsigned int aiRad)
{
    switch (*reinterpret_cast<char*>(this))
    {
    case -1:
    case 1:
        break;
    default:
        *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(this) + 4) = aiRad;
        break;
    }
}

struct FUN_0067f410_shell : public LocationDescriptor
{
    void PackageLocation_SetLocCell(TESObjectCELL*);
};

// @fnv 0x0067f410
// src: XBOX_PDB 0x0067f410
void FUN_0067f410_shell::PackageLocation_SetLocCell(TESObjectCELL* apCell)
{
    eLocType = 1;
    pCell = apCell;
}

struct FUN_0067f970_shell : public LocationDescriptor
{
    bool PackageLocation_IsLocationGeneric();
};

// @fnv 0x0067f970
// src: COMMUNITY 0x0067f970
// pdb: PackageLocation::IsLocationGeneric | PackageLocation.obj | debug, upstream name already matches, high
bool FUN_0067f970_shell::PackageLocation_IsLocationGeneric()
{
    if (GetLocType() > PLT_EDITORLOC)
        return true;
    else
        return false;
}

class NiAVObject;
class NiQuaternion;
class TESEyes;
class BSAnimGroupSequence;
#include "../../include/ACTION_FLAGS.h"
#include "../../include/ANIM_GROUP_SECTION.h"
// pdb: class Animation, size 0x13c (here 0x1), 33 members
class Animation
{
  public:
    BSAnimGroupSequence* PlayGroup(unsigned short, ACTION_FLAGS, int, ANIM_GROUP_SECTION);
};
float __cdecl BSMIN(float, float);
float __cdecl NI2HK_004a3e90(float);
float __cdecl NiATan2(float, float);
void __cdecl NI2HK_00561500(void*, const NiQuaternion*);
void __cdecl bhkBlendController_DoGetUp(NiAVObject*, float);
extern const double DAT_01020998;
extern float runtimeInitializationFloat3452;
extern TESEyes* DAT_011cb600;

hkVector4& __cdecl NI2HK(hkVector4&, const NiPoint3&);
hkRotation& __cdecl VisitAndFetchThree(hkRotation&, const NiMatrix3&);
// The shared quaternion has no rotation constructor; retain its external
// construction entry point through a layout-identical derived quaternion.
struct __declspec(align(16)) RotationQuaternion : public hkQuaternion
{
    RotationQuaternion(const hkRotation&);
};
struct B006815c0 : public NiPoint3
{
    B006815c0();
};
struct NiPoint3_sub : public NiPoint3
{
    NiPoint3 operator-(const NiPoint3&) const;
};
struct ExtraDataList_helper : public ExtraDataList
{
    void ExtraDataList_SetRagDollData(TESObjectREFR*);
};
class P01ObjA
{
  public:
    TESForm* GetNext();
};
#include "../../include/TESResponse.h"
void __cdecl CommandEffect_InitCommandedActor(Actor*, Actor*);
void __cdecl CommandEffect_UpdateCommandedActor(Actor*, Actor*);
struct FUN_00826be0_shell : public ActiveEffect
{
    void ReanimateEffect_Update(float);
};

// Preserve the original stack cookie now that the shared matrix uses named entries.
#pragma strict_gs_check(push, on)
// @fnv 0x00826be0
// src: XBOX_PDB 0x00826be0
// pdb: ReanimateEffect::Update | ReanimateEffect.obj | retail, upstream name already matches, high
// pdb params: afSecondsElapsed
// pdb locals: pcaster, ptarget, t, matrix, endPos, fy, startRot, fz, hkmatrix, startPos, dirToCaster, hkpos, pprocess, currentPos, fx, hkquat, endRot, pav, panim, pnpc, panimData
void FUN_00826be0_shell::ReanimateEffect_Update(float afSecondsElapsed)
{
    Actor* ptarget =
        ((BSThread*)this)->GetThreadID() ? ((MagicTarget*)((BSThread*)this)->GetThreadID())->GetOwnerActor() : 0;
    Actor* pcaster =
        ((TESResponse*)this)->GetNext() ? ((MagicCaster*)((TESResponse*)this)->GetNext())->GetCasterActor() : 0;
    if (!pcaster)
    {
        Dispel(true);
    }
    else if (*(bhkRigidBody**)((char*)this + 0x48) && ptarget)
    {
        if (ptarget->HasHealth(false))
        {
            Dispel(false);
            return;
        }
        if (*(int*)((char*)this + 0x4c) == 0)
        {
            *(float*)((char*)this + 0x50) = 0.0f;
            *(int*)((char*)this + 0x4c) = 10;
        }
        if (*(int*)((char*)this + 0x4c) == 10)
        {
            BaseProcess* pprocess = static_cast<MobileObject*>(ptarget)->FUN_008d8520();
            if (!pprocess->FUN_0045cd60())
                pprocess->SetIdleStateFlag(true);
            if (*(float*)((char*)this + 4) - *(float*)((char*)this + 0x50) > DAT_01020998)
                *(int*)((char*)this + 0x4c) = 20;
            float fLerp = BSMIN(1.0f, (*(float*)((char*)this + 4) - *(float*)((char*)this + 0x50)) / 4.0);
            hkVector4 endPos;
            hkVector4 startPos;
            NI2HK(endPos, *(NiPoint3*)((char*)this + 0x54));
            NI2HK(startPos, *(NiPoint3*)((char*)this + 0x54));
            ((bhkCharacterController*)&endPos)->GetVectorElement(2) += NI2HK_004a3e90(100.0f);
            hkVector4 hkpos;
            hkpos.setInterpolate4(startPos, endPos, fLerp);
            B006815c0 currentPos;
            (*(bhkRigidBody**)((char*)this + 0x48))->bhkRigidBody_GetPosition(currentPos);
            NiPoint3 dirToCaster = *(const NiPoint3_sub*)static_cast<const Actor*>(pcaster)->GetPos() - currentPos;
            dirToCaster.z = 0.0f;
            float fx = -NiATan2(dirToCaster.y, dirToCaster.x);
            float afDepth = runtimeInitializationFloat3452;
            float fy = 0.5f;
            NiMatrix3 matrix;
            matrix.FromEulerAnglesZYX(fx, fy, afDepth);
            hkRotation kHkRotation;
            VisitAndFetchThree(kHkRotation, matrix);
            __declspec(align(16)) hkQuaternion startRot;
            NI2HK_00561500(&startRot, (const NiQuaternion*)((char*)this + 0x60));
            RotationQuaternion endRot(kHkRotation);
            __declspec(align(16)) hkQuaternion hkquat;
            hkquat.D3D_ComputeClearColor(startRot, endRot, fLerp);
            (*(bhkRigidBody**)((char*)this + 0x48))->bhkRigidBody_ApplyKeyframe(hkpos, hkquat, 1.0f / afSecondsElapsed, 1.0f);
            // The shared slot B0 declaration omits the reference argument used here.
            reinterpret_cast<void (__thiscall*)(Actor*, TESObjectREFR*)>(
                (*reinterpret_cast<void***>(ptarget))[0xB0])(
                    ptarget, ((MagicCaster*)((TESResponse*)this)->GetNext())->GetCasterReference());
        }
        else if (*(int*)((char*)this + 0x4c) == 20)
        {
            Animation* panim = reinterpret_cast<Animation*>(ptarget->GetAnimData());
            NiAVObject* pNode = ptarget->GetNiNode();
            if (pNode && panim)
            {
                *(int*)((char*)this + 0x4c) = 30;
                ptarget->SetReloadState(true);
                panim->PlayGroup(0, (ACTION_FLAGS)0, -1, (ANIM_GROUP_SECTION)-1);
                bhkBlendController_DoGetUp(pNode, 0.0f);
                BaseProcess* pprocess = static_cast<MobileObject*>(ptarget)->FUN_008d8520();
                if (!pprocess->FUN_0045cd60())
                    pprocess->SetIdleStateFlag(false);
                ((ExtraDataList_helper*)((TESObjectREFR*)ptarget)->GetExtraData())->ExtraDataList_SetRagDollData(0);
                *(float*)((char*)this + 0x50) = *(float*)((char*)this + 4);
            }
        }
        else if (*(int*)((char*)this + 0x4c) == 30)
        {
            if (*(float*)((char*)this + 4) - *(float*)((char*)this + 0x50) > 2.0)
            {
                *(float*)((char*)this + 0x50) = *(float*)((char*)this + 4);
                *(int*)((char*)this + 0x4c) = 40;
                NiAVObject* pNode = ptarget->GetNiNode();
                if (pNode)
                    Creature::SetNodeFlagsRecursive(pNode, 4, true, false, true);
                TESNPC* pnpc = dynamic_cast<TESNPC*>((TESBoundObject*)((P01ObjA*)ptarget)->GetNext());
                if (pnpc)
                    pnpc->SwapEyes(DAT_011cb600);
                BSFaceGenAnimationData* pFaceGenAnimData = ptarget->GetActorFaceGenData();
                if (pFaceGenAnimData)
                    pFaceGenAnimData->SetStateFlags(false, false);
            }
        }
        else if (*(int*)((char*)this + 0x4c) == 40)
        {
            if (*(float*)((char*)this + 4) - *(float*)((char*)this + 0x50) > 1.0)
            {
                *(int*)((char*)this + 0x4c) = 50;
                ptarget->SetAnimationGroupAction(4);
                ptarget->Actor_SetGhost(false);
                CommandEffect_InitCommandedActor(ptarget, pcaster);
            }
        }
        else if (*(int*)((char*)this + 0x4c) == 50)
        {
            static_cast<MobileObject*>(ptarget)->FUN_008d8520()->ClearCachedTargetReferences(false);
            CommandEffect_UpdateCommandedActor(ptarget, pcaster);
        }
    }
}

#pragma strict_gs_check(pop)

struct FUN_0067f430_shell
{
    char m_ucLocationType;
    char pad0[7];
    TESForm* m_pLocationObject;

    TESForm* GetLocationObject();
};

// Returns the stored TESForm pointer if the leading byte equals 4, otherwise returns null.
// @fnv 0x0067f430
TESForm* FUN_0067f430_shell::GetLocationObject()
{
    if (m_ucLocationType == 4)
        return m_pLocationObject;
    else
        return 0;
}

struct FUN_0067f460_shell
{
    char b[8];
    int v;
    void SetValue(int a);
};

// @fnv 0x0067f460
void FUN_0067f460_shell::SetValue(int aiValue)
{
    *(char*)this = 4;
    v = aiValue;
}

namespace fnv_0067f600 {
struct FUN_0067f430_shell : public LocationDescriptor
{
    TESForm* GetLocationObject();
};

struct FUN_0067f600_shell : public LocationDescriptor
{
    bool PackageLocation_Compare(LocationDescriptor* apCompare);
    unsigned int PackageLocation_GetRadius();
    int GetObjectType();
};

// @fnv 0x0067f600
// src: XBOX_DEBUG_PDB 0x0067f600
// pdb: PackageLocation::Compare | PackageLocation.obj | debug, call graph, high
// pdb params: apCompare
bool FUN_0067f600_shell::PackageLocation_Compare(LocationDescriptor* apCompare)
{
    if (!apCompare)
        return true;
    if (GetLocType() != apCompare->GetLocType())
        return true;
    if (PackageLocation_GetRadius() != reinterpret_cast<FUN_0067f600_shell*>(apCompare)->PackageLocation_GetRadius())
        return true;
    if (GetLocationReference() != apCompare->GetLocationReference())
        return true;
    if (GetLocationCellReference() != apCompare->GetLocationCellReference())
        return true;
    if (reinterpret_cast<FUN_0067f430_shell*>(this)->GetLocationObject() !=
        reinterpret_cast<FUN_0067f430_shell*>(apCompare)->GetLocationObject())
        return true;
    if (GetObjectType() != reinterpret_cast<FUN_0067f600_shell*>(apCompare)->GetObjectType())
        return true;
    return false;
}
}  // namespace fnv_0067f600


struct FUN_0067f480_shell
{
    char m_ucLocationType;
    char pad1[3];
    char pad2[4];
    PACKAGE_OBJECT_TYPE eObjType;
    PACKAGE_OBJECT_TYPE GetObjectType();
};

// Returns eObjType when the leading byte equals 5, otherwise returns 0.
// @fnv 0x0067f480
PACKAGE_OBJECT_TYPE FUN_0067f480_shell::GetObjectType()
{
    if (m_ucLocationType == 5)
        return eObjType;
    else
        return (PACKAGE_OBJECT_TYPE)0;
}

namespace fnv_0067f6c0 {
struct FUN_0067eff0_Class
{
    char m_ucLocationType;
    unsigned int m_pLocationData;
    unsigned int m_iSearchRadius;

    void PACK_LOC_FILE_Endian();
};

typedef FUN_0067eff0_Class ChunkPayloadHandler;

struct FUN_0067f480_shell
{
    int GetObjectType();
};

struct FUN_0067f1c0_shell
{
    unsigned int PackageLocation_GetRadius();
};

void* __cdecl memset(void* buffer, int value, unsigned int count);
unsigned char __cdecl _FUN_00401500();
void __cdecl _FUN_00485990(unsigned int chunkId, ChunkPayloadHandler* location, unsigned int size);

struct FUN_0067f6c0_shell : public LocationDescriptor
{
    void PackageLocation_Save(unsigned int aeChunk);
};

// @fnv 0x0067f6c0
// src: XBOX_PDB 0x0067f6c0
// pdb: PackageLocation::Save | PackageLocation.obj | retail, upstream name already matches, high
// pdb params: aeChunk
// pdb locals: locStruct
void FUN_0067f6c0_shell::PackageLocation_Save(unsigned int aeChunk)
{
    ChunkPayloadHandler locStruct;
    memset(&locStruct, 0, sizeof(locStruct));

    locStruct.m_ucLocationType = (char)GetLocType();

    if (GetLocType() == PLT_TYPE)
        locStruct.m_pLocationData = reinterpret_cast<FUN_0067f480_shell*>(this)->GetObjectType();
    else if (pObject != 0)
        locStruct.m_pLocationData = pObject->GetFormID();

    locStruct.m_iSearchRadius = reinterpret_cast<FUN_0067f1c0_shell*>(this)->PackageLocation_GetRadius();

    if (_FUN_00401500())
        locStruct.PACK_LOC_FILE_Endian();

    _FUN_00485990(aeChunk, &locStruct, 0xc);

    if (_FUN_00401500())
        locStruct.PACK_LOC_FILE_Endian();
}
}  // namespace fnv_0067f6c0

struct FUN_0067f060_Source
{
    char unk00;
    int unk04;
    int unk08;
};

struct FUN_0067f060_Class
{
    char unk00;
    int unk04;
    int unk08;

    FUN_0067f060_Class* __thiscall CopyFromSource(FUN_0067f060_Source* source);
};

// Copies the source's type byte and value into this object, then sets the remaining fields based on the type byte.
// @fnv 0x0067f060
FUN_0067f060_Class* __thiscall FUN_0067f060_Class::CopyFromSource(FUN_0067f060_Source* source)
{
    this->unk00 = source->unk00;
    this->unk04 = source->unk08;

    switch (this->unk00)
    {
    case 1:
        this->unk08 = source->unk04;
        this->unk04 = 0;
        break;
    case 0:
        this->unk08 = source->unk04;
        break;
    case 4:
        this->unk08 = source->unk04;
        break;
    case 5:
        this->unk08 = source->unk04;
        break;
    case 2:
    case 3:
        this->unk08 = 0;
        break;
    }

    return this;
}

class PkgLocNode;
class PkgLocChild;
class PkgLocSink;

class FUN_0067f220_cls
{
  public:
    unsigned int m_00;
    PkgLocNode* Lookup(int apActor);
    bool LookupAndStore(int apPackOwner, int apRef, unsigned char* abRefInsideTriggerOut);
};

class PkgLocNode
{
  public:
    PkgLocChild* Resolve();
};

class PkgLocChild
{
  public:
    int Kind();
};

class PkgLocSink
{
  public:
    unsigned char Store(int apRef, PkgLocNode* apTrigger);
};

PkgLocSink* PkgLocFind(PkgLocNode* p);

// @fnv 0x0067f220
// pdb: PackageLocation::IsRefAtLocationTrigger | PackageLocation.obj | retail, call graph (shared callers), high
// pdb params: apPackOwner, apRef, abRefInsideTriggerOut
// pdb locals: plistener, ptrigger
bool FUN_0067f220_cls::LookupAndStore(int apPackOwner, int apRef, unsigned char* abRefInsideTriggerOut)
{
    *abRefInsideTriggerOut = 0;
    PkgLocNode* p = Lookup(apPackOwner);
    if (p == 0)
    {
        return false;
    }
    if (p->Resolve()->Kind() != 0x15)
    {
        return false;
    }
    PkgLocSink* pSink = PkgLocFind(p);
    if (pSink == 0)
    {
        return true;
    }
    *abRefInsideTriggerOut = pSink->Store(apRef, p);
    return true;
}

#include "../../include/TESSaveLoadGame.h"
extern TESSaveLoadGame* DAT_011de45c;
struct FUN_0067fa00_shell : public LocationDescriptor
{
    void SaveTargetReference();
};
// Serializes a form ID and object reference through the save/load game, writing the object's form ID for cases 0-4 and the pointer for case 5.
// @fnv 0x0067fa00
// src: GUESS 0x0067fa00 (confidence: medium)
// pdb: PackageLocation::SaveGame | PackageLocation.obj | retail, object-file run extension, low
// pdb locals: iformID
void FUN_0067fa00_shell::SaveTargetReference()
{
    DAT_011de45c->TESSaveLoadGame_SaveGameDataOLD(this, 1);
    DAT_011de45c->TESSaveLoadGame_SaveGameDataOLD(&iRad, 4);
    switch (eLocType)
    {
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4: {
        unsigned int id = 0;
        if (pObject)
            id = pObject->GetFormID();
        DAT_011de45c->TESSaveLoadGame_SaveNumericID((int)&id, 4);
        break;
    }
    case 5:
        DAT_011de45c->TESSaveLoadGame_SaveGameDataOLD(&pObject, 4);
        break;
    }
}

int __cdecl TESForm_GetFormByNumericID(int value);

struct FUN_0067fb20_holder
{
    signed char status;
    unsigned char flags1;
    unsigned short flags2;
    unsigned int metadata;
    int storedValue;
    void UpdateStoredValue();
};

// For status 0-4, replaces storedValue with the result of FUN_004839c0(storedValue); other statuses do nothing.
// @fnv 0x0067fb20
// pdb: PackageLocation::InitLoadGame | PackageLocation.obj | retail, object-file run extension, low
// pdb locals: iformID
void FUN_0067fb20_holder::UpdateStoredValue()
{
    switch (status)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4: {
        int originalValue = storedValue;
        storedValue = TESForm_GetFormByNumericID(originalValue);
        break;
    }
    default:
        break;
    }
}

class LocationDescriptor;
class TESForm;
struct LocationInputStream
{
    void FUN_00865e50(void* destination, unsigned int byteCount, int flags);
    void FUN_00865df0(TESForm* form, int flags);
};
struct FUN_0067fb60_shell : LocationDescriptor
{
    void PackageLocation_SaveGame(LocationInputStream* apSaveGameBuffer);
};
// Reads a one-byte type tag and a 4-byte value from the stream, then loads pObject as a form reference or a raw 4-byte value depending on the tag.
// @fnv 0x0067fb60
// src: XBOX_PDB 0x0067fb60
// pdb: PackageLocation::SaveGame | PackageLocation.obj | retail, call graph (shared callers), high
// pdb locals: iformID
void FUN_0067fb60_shell::PackageLocation_SaveGame(LocationInputStream* apSaveGameBuffer)
{
    apSaveGameBuffer->FUN_00865e50(&eLocType, 1, 0);
    apSaveGameBuffer->FUN_00865e50(&iRad, 4, 0);
    switch (eLocType)
    {
    case -1:
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        apSaveGameBuffer->FUN_00865df0(pObject, 0);
        break;
    case 5:
        apSaveGameBuffer->FUN_00865e50(&pObject, 4, 0);
        break;
    }
}

class PackageLoadStream
{
  public:
    void ReadBytes(void* apData, unsigned int auiSize);
    void ReadPtr(void* apData);
};

class FUN_0067fbe0_cls : public LocationDescriptor
{
  public:
    void PackageLocation_LoadGame_0067fbe0(PackageLoadStream* apLoadGameBuffer);
};

// Reads a type byte and a 4-byte value from the stream, then reads pObject as a pointer for types 0-4 or as 4 raw bytes for type 5.
// @fnv 0x0067fbe0
// src: XBOX_PDB 0x0067fbe0
// pdb: PackageLocation::LoadGame | PackageLocation.obj | retail, call graph (shared callers), high
// pdb locals: iformID
void FUN_0067fbe0_cls::PackageLocation_LoadGame_0067fbe0(PackageLoadStream* apLoadGameBuffer)
{
    apLoadGameBuffer->ReadBytes(&eLocType, 1);
    apLoadGameBuffer->ReadBytes(&iRad, 4);
    switch (eLocType)
    {
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        apLoadGameBuffer->ReadPtr(&pObject);
        break;
    case 5:
        apLoadGameBuffer->ReadBytes(&pObject, 4);
        break;
    }
}

class FUN_0067f990_C
{
  public:
    char m_eRequestType;

    unsigned short PackageLocation_GetSaveSize();
};

// Returns 5 plus 4 when m_eRequestType is between 0 and 5 inclusive.
// @fnv 0x0067f990
// src: COMMUNITY 0x0067f990
unsigned short FUN_0067f990_C::PackageLocation_GetSaveSize()
{
    unsigned short usSize = 0;
    usSize += 1;
    usSize += 4;
    switch (m_eRequestType)
    {
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        usSize += 4;
        break;
    case 5:
        usSize += 4;
        break;
    }
    return usSize;
}

namespace fnv_0067faa0 {
struct FileQ
{
    void TESSaveLoadGame_LoadGameDataOLD(void* apData, int aiSize);
    void FUN_00857aa0(void* p, int n);
};
extern FileQ* DAT_011de45c;
struct OwnQ
{
    char cType;
    char pad[3];
    int iVal;
    int iData;
    void PackageLocation_LoadGame();
};
// @fnv 0x0067faa0
// src: XBOX_PDB 0x0067faa0
// pdb: PackageLocation::LoadGame | PackageLocation.obj | retail, object-file run extension, low
// pdb locals: iformID
void OwnQ::PackageLocation_LoadGame()
{
    int iTmp;
    DAT_011de45c->TESSaveLoadGame_LoadGameDataOLD(this, 1);
    DAT_011de45c->TESSaveLoadGame_LoadGameDataOLD(&iVal, 4);
    switch (cType)
    {
        break;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        DAT_011de45c->FUN_00857aa0(&iTmp, 4);
        iData = iTmp;
        break;
    case 5:
        DAT_011de45c->TESSaveLoadGame_LoadGameDataOLD(&iData, 4);
        break;
    }
}
}  // namespace fnv_0067faa0

TESForm* TESForm_GetFormByNumericID(unsigned int formID);

struct FUN_0067fc50_shell : LocationDescriptor
{
    void PackageLocation_InitLoadGame(int apLoadGameBuffer);
};

// For PLT_REFERENCE, PLT_CELL, PLT_CURRLOC, PLT_EDITORLOC, or PLT_OBJECT, resolves non-null pObject through FUN_004839c0.
// @fnv 0x0067fc50
// src: XBOX_RETAIL_PDB 0x0067fc50
// pdb: PackageLocation::InitLoadGame | PackageLocation.obj | retail, object-file run extension, high
// pdb locals: iformID
void FUN_0067fc50_shell::PackageLocation_InitLoadGame(int apLoadGameBuffer)
{
    switch (eLocType)
    {
    case PLT_REFERENCE:
    case PLT_CELL:
    case PLT_CURRLOC:
    case PLT_EDITORLOC:
    case PLT_OBJECT:
        pObject = pObject ? TESForm_GetFormByNumericID(reinterpret_cast<unsigned int>(pObject)) : 0;
    }
}
