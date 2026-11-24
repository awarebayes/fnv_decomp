// fallout_shared/packagedatauseweapon.cpp: TESUseWeaponPackageData, UseWeaponActorPackageData and helpers (16 functions)

#include "../../include/TESUseWeaponPackageData.h"
#include "../../include/UseWeaponActorPackageData.h"
#include "../../include/ExtraDataList.h"
#include "../../include/TESObjectREFR.h"
#include "../../include/TESObjectWEAP.h"
#include "../../include/TESPackageData.h"
#include "../../include/PackageTargetRecord.h"
#include "packagedatauseweapon_decls.h"
#include "../../include/BGSSaveFormBuffer.h"
#include "../../include/BGSLoadFormBuffer.h"

// @fnv 0x0067d1c0
// pdb: TESUseWeaponPackageData::InitItem | PackageDataUseWeapon.obj | retail, upstream name already matches, high
// pdb params: apOwner
// pdb locals: ppackLoc, itempint, binit, ppack, plocRef, pboundObj, pextra, pattackTarget
void TESUseWeaponPackageData::InitItem(TESForm* apOwner)
{
    LocationDescriptor* package = reinterpret_cast<LocationDescriptor*>(apOwner);
    bool initializeTarget;
    {
        LocationDescriptor* location = reinterpret_cast<LocationDescriptor*>(package->SetLocRef());
        initializeTarget = true;
        if (!m_pUseTarget && location && location->GetLocationType() == 0)
        {
            TESObjectREFR* reference = location->GetLocationReference();
            if (reference)
            {
                TESBoundObject* object = reference->GetObjectReference();
                if (object == DAT_011ca244 || object == DAT_011ca248)
                {
                    ExtraDataList* extra = reference->GetExtraData();
                    if (extra)
                    {
                        TESObjectREFR* target = extra->GetTargetObjectReference();
                        if (target)
                        {
                            m_pUseTarget = new PackageTargetRecord;
                            m_pUseTarget->SetTargetKind(PACK_TARG_TYPE_PTT_REFERENCE);
                            m_pUseTarget->SetTargetObjectReference(target);
                            initializeTarget = false;
                        }
                    }
                }
            }
        }
        if (m_pUseTarget && initializeTarget)
            m_pUseTarget->InitItem(apOwner);
        unsigned int weaponID = reinterpret_cast<unsigned int>(pWeapon);
        pWeapon = dynamic_cast<TESObjectWEAP*>(TESForm::GetFormByID(weaponID));
    }
}

// Clears the stored reference if it equals the passed reference.
// @fnv 0x0067d9e0
void UseWeaponActorPackageData::SetReferenceTarget(TESObjectREFR* reference)
{
    m_pUseTarget = m_pUseTarget == reference ? 0 : m_pUseTarget;
}

void __cdecl Deallocate(void*);
// Runs the destructor on this, then frees this via Deallocate when flags has bit 0 set, and returns this.
// @fnv 0x0067cb40
// pdb: TESUseWeaponPackageData::`scalar deleting destructor' | PackageDataUseWeapon.obj | retail, vtable slot, high
void* TESUseWeaponPackageData::DestroyIfFlagged(unsigned int flags)
{
    this->TESUseWeaponPackageData::~TESUseWeaponPackageData();
    if (flags & 1)
        Deallocate(this);
    return this;
}

struct FUN_00670b30
{
    ~FUN_00670b30();
};

// Deletes the target and location objects and clears both pointers.
// @fnv 0x0067ccd0
// pdb: TESUseWeaponPackageData::~TESUseWeaponPackageData | PackageDataUseWeapon.obj | retail, upstream name already matches, high
TESUseWeaponPackageData::~TESUseWeaponPackageData()
{
    delete m_pUseTarget;
    m_pUseTarget = 0;
    delete reinterpret_cast<FUN_00670b30*>(pTargetLocation);
    pTargetLocation = 0;
}

struct FUN_0067fc50_impl
{
    void __thiscall PackageLocation_InitLoadGame(BGSLoadFormBuffer*);
};

// Resolves the optional target and location using the load game buffer.
// @fnv 0x0067d8b0
// pdb: TESUseWeaponPackageData::InitLoadGame | PackageDataUseWeapon.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
void TESUseWeaponPackageData::LoadForm(BGSLoadFormBuffer* apLoadGameBuffer)
{
    if (m_pUseTarget)
        m_pUseTarget->InitLoadGame(apLoadGameBuffer);
    if (pTargetLocation)
        ((FUN_0067fc50_impl*)pTargetLocation)->PackageLocation_InitLoadGame(apLoadGameBuffer);
}

// @fnv 0x0067da20
void UseWeaponActorPackageData::InitLoadGame(BGSLoadFormBuffer* arg)
{
    m_pUseTarget = 0;
}

struct FUN_0067f600_shell
{
    bool PackageLocation_Compare(void* apCompare);
};

void* FUN_00ec43fb(void* object, unsigned int flags, char* typeInfo1, char* typeInfo2, int reserved);
float __cdecl fabsf_00408860(float value);

extern char DAT_0119bdec;
extern char DAT_0119bf00;

// Compares this use-weapon package data against apCompare field by field, returning true when any field differs or the cast fails.
// @fnv 0x0067cfc0
// src: XBOX_PDB 0x0067cfc0
// pdb: TESUseWeaponPackageData::Compare | PackageDataUseWeapon.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp
bool TESUseWeaponPackageData::Compare_0067cfc0(TESPackageData* apCompare)
{
    TESUseWeaponPackageData* pOtherData =
        static_cast<TESUseWeaponPackageData*>(FUN_00ec43fb(apCompare, 0, &DAT_0119bdec, &DAT_0119bf00, 0));

    if (!pOtherData)
        return true;

    if (this->pWeapon != pOtherData->pWeapon)
        return true;

    if (this->m_pUseTarget)
    {
        if (this->m_pUseTarget->Compare(pOtherData->m_pUseTarget))
            return true;
    }
    else if (pOtherData->m_pUseTarget)
    {
        return true;
    }

    if (this->pTargetLocation)
    {
        if (reinterpret_cast<FUN_0067f600_shell*>(this->pTargetLocation)
                ->PackageLocation_Compare(static_cast<void*>(pOtherData->pTargetLocation)))
            return true;
    }
    else if (pOtherData->pTargetLocation)
    {
        return true;
    }

    if (this->bAlwaysHit != pOtherData->bAlwaysHit)
        return true;

    if (this->bDoNoDamage != pOtherData->bDoNoDamage)
        return true;

    if (this->bCrouch != pOtherData->bCrouch)
        return true;

    if (this->bHoldFire != pOtherData->bHoldFire)
        return true;

    if (this->bVolleyFire != pOtherData->bVolleyFire)
        return true;

    if (this->bRepeatFire != pOtherData->bRepeatFire)
        return true;

    if (this->sBurstCount != pOtherData->sBurstCount)
        return true;

    if (this->sVolleyShotsMin != pOtherData->sVolleyShotsMin)
        return true;

    if (this->sVolleyShotsMax != pOtherData->sVolleyShotsMax)
        return true;

    if (fabsf_00408860(this->fVolleyWaitMin - pOtherData->fVolleyWaitMin) > 0.01)
        return true;

    if (fabsf_00408860(this->fVolleyWaitMax - pOtherData->fVolleyWaitMax) > 0.01)
        return true;

    return false;
}

void* memset(void*, int, size_t);
bool __cdecl TESForm_QEndianSwapOnSave();
void __cdecl AppendChunk(unsigned int, void*, unsigned int);

struct PackageLocationSave
{
    void Save(int);
};

struct FlaggedValueData
{
    bool m_bFlag1;
    bool m_bFlag2;
    bool m_bFlag3;
    bool m_bFlag4;
    bool m_bFlag5;
    bool m_bFlag6;
    unsigned short m_usValue1;
    unsigned short m_usValue2;
    unsigned short m_usValue3;
    float m_fValue1;
    float m_fValue2;
    unsigned int m_uiIntegerValue;

    void PACK_USE_WEAPON_DATA_PKW2_Endian();
};

// @fnv 0x0067d350
// src: XBOX_PDB 0x0067d350
// pdb: TESUseWeaponPackageData::Save | PackageDataUseWeapon.obj | retail, vtable slot, high
// pdb locals: diastruct
void TESUseWeaponPackageData::Save_0067d350()
{
    FlaggedValueData saveData;

    memset(&saveData, 0, 0x18);

    saveData.m_bFlag1 = this->bAlwaysHit;
    saveData.m_bFlag2 = this->bDoNoDamage;
    saveData.m_bFlag3 = this->bCrouch;
    saveData.m_bFlag4 = this->bHoldFire;
    saveData.m_bFlag5 = this->bVolleyFire;
    saveData.m_bFlag6 = this->bRepeatFire;
    saveData.m_usValue1 = this->sBurstCount;
    saveData.m_usValue2 = this->sVolleyShotsMin;
    saveData.m_usValue3 = this->sVolleyShotsMax;
    saveData.m_fValue1 = this->fVolleyWaitMin;
    saveData.m_fValue2 = this->fVolleyWaitMax;

    saveData.m_uiIntegerValue = this->pWeapon ? this->pWeapon->GetFormID() : 0;

    if (TESForm_QEndianSwapOnSave())
        saveData.PACK_USE_WEAPON_DATA_PKW2_Endian();

    AppendChunk(0x33574b50, &saveData, 0x18);

    if (TESForm_QEndianSwapOnSave())
        saveData.PACK_USE_WEAPON_DATA_PKW2_Endian();

    if (this->m_pUseTarget)
        this->m_pUseTarget->PackageTarget_Save(0x32445450);

    if (this->pTargetLocation)
        reinterpret_cast<PackageLocationSave*>(this->pTargetLocation)->Save(0x32444c50);
}

// Initializes the parameter bytes and clears the target, location and weapon pointers.
// @fnv 0x0067cc20
// pdb: TESUseWeaponPackageData::TESUseWeaponPackageData | PackageDataUseWeapon.obj | debug, call graph, high
// pdb params: eDialData
TESUseWeaponPackageData::TESUseWeaponPackageData(Color kColor, Info kInfo, Range kRange, float fStart, float m_fEndValue)
{
    m_pUseTarget = 0;
    pTargetLocation = 0;
    pWeapon = 0;
    // Preserve the original parameter bytes without converting them to bool.
    reinterpret_cast<unsigned char*>(&bAlwaysHit)[0] = kColor.r;
    reinterpret_cast<unsigned char*>(&bAlwaysHit)[1] = kColor.g;
    reinterpret_cast<unsigned char*>(&bAlwaysHit)[2] = kColor.b;
    reinterpret_cast<unsigned char*>(&bAlwaysHit)[3] = kColor.a;
    reinterpret_cast<unsigned char*>(&bAlwaysHit)[4] = kInfo.m_ucType;
    bRepeatFire = kInfo.m_usElementCount <= 0;
    sBurstCount = kInfo.m_usElementCount;
    sVolleyShotsMin = kRange.m_usMin;
    sVolleyShotsMax = kRange.m_usMax;
    fVolleyWaitMin = fStart;
    fVolleyWaitMax = m_fEndValue;
}

// Copy the optional extra data from another object: create ours on demand, or drop it when the source is null.


// @fnv 0x0067d470
// pdb: TESUseWeaponPackageData::Copy | PackageDataUseWeapon.obj | retail, upstream name already matches, high
// pdb params: apCopy
// pdb locals: pcopy
void TESUseWeaponPackageData::Copy(TESPackageData* apCopy)
{
    if (apCopy)
    {
        if (!m_pUseTarget)
            m_pUseTarget = new PackageTargetRecord;
        m_pUseTarget->PackageTarget_Copy(reinterpret_cast<PackageTargetRecord*>(apCopy));
    }
    else
    {
        delete m_pUseTarget;
        m_pUseTarget = 0;
    }
}

TESForm* __cdecl TESForm_GetFormByNumericID(unsigned int);
struct PackageLocationObj
{
    unsigned int type;
    unsigned int location;
    unsigned int radius;
    PackageLocationObj();
    void PackageLocation_LoadGame_0067fbe0(BGSLoadFormBuffer*);
};
// Reads the use-weapon package data from the buffer: fixed-size fields, a weapon form, and optional target and location objects.
// @fnv 0x0067d6b0
// pdb: TESUseWeaponPackageData::LoadGame | PackageDataUseWeapon.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
// pdb locals: bhasLocation, bhasTarget
void TESUseWeaponPackageData::LoadUseWeaponData(BGSLoadFormBuffer* apLoadGameBuffer)
{
    apLoadGameBuffer->LoadData(&bAlwaysHit, 1);
    apLoadGameBuffer->LoadData(&bDoNoDamage, 1);
    apLoadGameBuffer->LoadData(&bCrouch, 1);
    apLoadGameBuffer->LoadData(&bHoldFire, 1);
    apLoadGameBuffer->LoadData(&bVolleyFire, 1);
    apLoadGameBuffer->LoadData(&bRepeatFire, 1);
    apLoadGameBuffer->LoadData(&sBurstCount, 2);
    apLoadGameBuffer->LoadData(&sVolleyShotsMin, 2);
    apLoadGameBuffer->LoadData(&sVolleyShotsMax, 2);
    apLoadGameBuffer->LoadData(&fVolleyWaitMin, 4);
    apLoadGameBuffer->LoadData(&fVolleyWaitMax, 4);
    pWeapon = dynamic_cast<TESObjectWEAP*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));
    bool hasTarget;
    bool hasLocation = false;
    apLoadGameBuffer->LoadData(&hasLocation, 1);
    if (hasLocation)
    {
        m_pUseTarget = new PackageTargetRecord;
        m_pUseTarget->LoadGame(apLoadGameBuffer);
    }
    hasTarget = false;
    apLoadGameBuffer->LoadData(&hasTarget, 1);
    if (hasTarget)
    {
        pTargetLocation = reinterpret_cast<LocationDescriptor*>(new PackageLocationObj);
        reinterpret_cast<PackageLocationObj*>(pTargetLocation)->PackageLocation_LoadGame_0067fbe0(apLoadGameBuffer);
    }
}

// @fnv 0x0067d9c0
// pdb: UseWeaponActorPackageData::~UseWeaponActorPackageData | PackageDataUseWeapon.obj | debug, upstream name already matches, high
UseWeaponActorPackageData::~UseWeaponActorPackageData()
{
}

void* FUN_00ec43fb(void* object, unsigned int flags, char* typeInfo1, char* typeInfo2, int reserved);
extern char DAT_0119bdec;
extern char DAT_0119bf00;

struct TargetLocation_Stub
{
    int unk00;
    int unk04;
    int unk08;
    TargetLocation_Stub();
    ~TargetLocation_Stub();
    void CopyFrom(LocationDescriptor*, bool);
};

// Copies a TESUseWeaponPackageData obtained from apSource into this, deep-copying its two sub-objects and copying the remaining fields.
// @fnv 0x0067cd90
// src: XBOX_PDB 0x0067cd90
void TESUseWeaponPackageData::Copy_0067cd90(TESPackageData* apSource)
{
    TESUseWeaponPackageData* target =
        (TESUseWeaponPackageData*)FUN_00ec43fb(apSource, 0, &DAT_0119bdec, &DAT_0119bf00, 0);
    if (!target)
    {
        return;
    }

    delete this->m_pUseTarget;
    this->m_pUseTarget = 0;

    if (target->m_pUseTarget)
    {
        this->m_pUseTarget = new PackageTargetRecord();
        this->m_pUseTarget->PackageTarget_Copy(target->m_pUseTarget);
    }

    this->pWeapon = target->pWeapon;

    delete (TargetLocation_Stub*)this->pTargetLocation;
    this->pTargetLocation = 0;

    if (target->pTargetLocation)
    {
        this->pTargetLocation = (LocationDescriptor*)new TargetLocation_Stub();
        ((TargetLocation_Stub*)this->pTargetLocation)->CopyFrom(target->pTargetLocation, false);
    }

    this->bAlwaysHit = target->bAlwaysHit;
    this->bDoNoDamage = target->bDoNoDamage;
    this->bCrouch = target->bCrouch;
    this->bHoldFire = target->bHoldFire;
    this->bVolleyFire = target->bVolleyFire;
    this->bRepeatFire = target->bRepeatFire;
    this->sBurstCount = target->sBurstCount;
    this->sVolleyShotsMin = target->sVolleyShotsMin;
    this->sVolleyShotsMax = target->sVolleyShotsMax;
    this->fVolleyWaitMin = target->fVolleyWaitMin;
    this->fVolleyWaitMax = target->fVolleyWaitMax;
}

struct LocationSave
{
    void PackageLocation_SaveGame(BGSSaveFormBuffer*);
};
// @fnv 0x0067d550
// src: XBOX_PDB 0x0067d550
// pdb: TESUseWeaponPackageData::SaveGame | PackageDataUseWeapon.obj | retail, vtable slot, high
// pdb params: apSaveGameBuffer
// pdb locals: bhasLocation, bhasTarget
void TESUseWeaponPackageData::SaveGame_0067d550(BGSSaveFormBuffer* apSaveGameBuffer)
{
    apSaveGameBuffer->SaveData(&bAlwaysHit, 1, 0);
    apSaveGameBuffer->SaveData(&bDoNoDamage, 1, 0);
    apSaveGameBuffer->SaveData(&bCrouch, 1, 0);
    apSaveGameBuffer->SaveData(&bHoldFire, 1, 0);
    apSaveGameBuffer->SaveData(&bVolleyFire, 1, 0);
    apSaveGameBuffer->SaveData(&bRepeatFire, 1, 0);
    apSaveGameBuffer->SaveData(&sBurstCount, 2, 0);
    apSaveGameBuffer->SaveData(&sVolleyShotsMin, 2, 0);
    apSaveGameBuffer->SaveData(&sVolleyShotsMax, 2, 0);
    apSaveGameBuffer->SaveData(&fVolleyWaitMin, 4, 0);
    apSaveGameBuffer->SaveData(&fVolleyWaitMax, 4, 0);
    apSaveGameBuffer->FUN_00865df0(pWeapon, 0);
    bool p = m_pUseTarget;
    apSaveGameBuffer->SaveData(&p, 1, 0);
    if (p)
        m_pUseTarget->SaveGame(apSaveGameBuffer);
    bool bHasLocation = pTargetLocation;
    apSaveGameBuffer->SaveData(&bHasLocation, 1, 0);
    if (bHasLocation)
        ((LocationSave*)pTargetLocation)->PackageLocation_SaveGame(apSaveGameBuffer);
}

// Copies the raw parameter fields, clears target/location, and stores the weapon value.
// @fnv 0x0067cb70
// pdb: TESUseWeaponPackageData::TESUseWeaponPackageData | PackageDataUseWeapon.obj | debug, object-file gap fill, medium
// pdb params: eDialData
TESUseWeaponPackageData::TESUseWeaponPackageData(Parameters kData, int iParam)
{
    m_pUseTarget = 0;
    pTargetLocation = 0;
    pWeapon = reinterpret_cast<TESObjectWEAP*>(iParam);
    // The serialized parameters retain their raw byte values.
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_c00 = kData.m_c00;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_c01 = kData.m_c01;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_c02 = kData.m_c02;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_c03 = kData.m_c03;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_c04 = kData.m_c04;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_c05 = kData.m_c05;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_s06 = kData.m_s06;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_s08 = kData.m_s08;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_s0A = kData.m_s0A;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_f0C = kData.m_f0C;
    reinterpret_cast<Parameters*>(&bAlwaysHit)->m_f10 = kData.m_f10;
}
