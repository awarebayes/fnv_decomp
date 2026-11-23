// fallout_shared/packagetarget.cpp: PackageTargetRecord, LocationDescriptor, Script (21 functions)

#include "../../include/layout_support.h"
#include "Script.h"
#include "../../include/BGSCameraPath.h"
#include "packagetarget_decls.h"
#include "packagetarget_types_PackageLocation.h"
#include "../../include/PackageTargetRecord.h"
#include "../../include/BGSSaveFormBuffer.h"
#include "../../include/BGSLoadFormBuffer.h"
#include "../../include/TESSaveLoadGame.h"

// @fnv 0x00519b00
int PackageTargetRecord::GetTargetKind()
{
    return (PACK_TARG_TYPE)eTargType;
}

// @fnv 0x006800b0
void PackageTargetRecord::SetTargetKind(PACK_TARG_TYPE aeTargetType)
{
    if (eTargType != aeTargetType)
    {
        eTargType = (unsigned char)aeTargetType;
        switch (eTargType)
        {
        case PACK_TARG_TYPE_PTT_REFERENCE:
            m_pWorldReferenceInfo = 0;
            break;
        case PACK_TARG_TYPE_PTT_OBJECT:
            m_pWorldReferenceInfo = 0;
            break;
        case PACK_TARG_TYPE_PTT_TYPE:
            m_pWorldReferenceInfo = 0;
            break;
        }
    }
}

// Returns the target reference if the target type is PACK_TARG_TYPE_PTT_REFERENCE, otherwise returns null.
// @fnv 0x00680020
// pdb: PackageTarget::GetTargReference | PackageTarget.obj | debug, call graph (shared callers), high
TESObjectREFR* PackageTargetRecord::GetReferenceTarget()
{
    if (GetTargetKind() == PACK_TARG_TYPE_PTT_REFERENCE)
        return m_pWorldReferenceInfo;
    else
        return 0;
}

// If the package target type is PACK_TARG_TYPE_PTT_REFERENCE, stores the given reference as the target.
// @fnv 0x00680110
// pdb: PackageTarget::SetTargReference | PackageTarget.obj | debug, call graph (shared callers), high
// pdb params: apRef
void PackageTargetRecord::SetTargetObjectReference(TESObjectREFR* apRef)
{
    if (GetTargetKind() == PACK_TARG_TYPE_PTT_REFERENCE)
        m_pWorldReferenceInfo = apRef;
}

// Returns pObject when the target type is PACK_TARG_TYPE_PTT_OBJECT, otherwise returns null.
// @fnv 0x00680050
// pdb: PackageTarget::GetTargObject | PackageTarget.obj | debug, call graph, high
TESForm* PackageTargetRecord::GetTargetForm()
{
    if (GetTargetKind() == PACK_TARG_TYPE_PTT_OBJECT)
        return pObject;
    else
        return 0;
}

// @fnv 0x00680080
// pdb: PackageTarget::GetTargObjectType | PackageTarget.obj | debug, call graph, high
int PackageTargetRecord::GetTargetObjectType()
{
    if (GetTargetKind() == PACK_TARG_TYPE_PTT_TYPE)
        return eObjType;
    else
        return (PACKAGE_OBJECT_TYPE)0;
}

// Copies location data from source into this object when copy is set or source's location type is valid, dispatching on the location type.
// @fnv 0x0067f4d0
// pdb: PackageLocation::Copy | PackageLocation.obj | debug, call graph, medium
// pdb params: apCopy, abNoneOK
void LocationDescriptor::Copy(LocationDescriptor* source, bool copy)
{
    if (source != 0)
    {
        if (copy || source->GetLocType() != PLT_NONE)
        {
            SetLocationType(source->GetLocType());
            switch (m_ucLocationType)
            {
            case 0:
                SetRadius(source->PackageLocation_GetRadius());
                PackageLocation_SetLocReference(source->GetLocationReference());
                break;
            case 1:
                PackageLocation_SetLocCell(source->GetLocationCellReference());
                break;
            case 4:
                SetRadius(source->PackageLocation_GetRadius());
                SetValue((TESForm*)source->FUN_0067f430());
                break;
            case 5:
                SetRadius(source->PackageLocation_GetRadius());
                PackageLocation_SetLocObjectType(source->GetObjectType());
                break;
            case 2:
            case 3:
            case 7:
                SetRadius(source->PackageLocation_GetRadius());
                break;
            case 6:
                SetRadius(source->PackageLocation_GetRadius());
                break;
            }
        }
    }
}

// @fnv 0x00671d10
char* Script::GetData()
{
    return m_data;
}

// Resolves the stored form ID against the owner's file and looks up the package target reference or object, warning if it is missing.
// @fnv 0x006803b0
// pdb: PackageTarget::InitItem | PackageTarget.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: tempint
void PackageTargetRecord::InitItem(TESForm* apOwner)
{
    unsigned int formID = (unsigned int)pTargetForm;
    TESForm::RemapFormIDForFile(formID, apOwner->GetFile(-1));
    switch (eTargType)
    {
    case 0:
        m_pWorldReferenceInfo = dynamic_cast<TESObjectREFR*>(GetFormByID(formID));
        if (!m_pWorldReferenceInfo)
        {
            if (apOwner)
            {
                if (((BGSCameraPath*)apOwner)->HasNameForWarning())
                    Warning("MASTERFILE: Unable to find Package Target Reference (%08X) on owner object \"%s\".",
                            formID, apOwner->GetName());
                else
                    Warning("MASTERFILE: Unable to find Package Target Reference (%08X) on owner object (%08X).",
                            formID, apOwner->GetFormID());
            }
            else
                Warning("MASTERFILE: Unable to find Package Target Reference (%08X) on unknown owner.", formID);
        }
        break;
    case 1:
        pObject = GetFormByID(formID);
        if (!pObject)
        {
            if (apOwner)
            {
                if (((BGSCameraPath*)apOwner)->HasNameForWarning())
                    Warning("MASTERFILE: Unable to find Package Target Object (%08X) on owner object \"%s\".", formID,
                            apOwner->GetName());
                else
                    Warning("MASTERFILE: Unable to find Package Target Object (%08X) on owner object (%08X).", formID,
                            apOwner->GetFormID());
            }
            else
                Warning("MASTERFILE: Unable to find Package Target Object (%08X) on unknown owner.", formID);
        }
        break;
    }
}

// Initializes an object-type target with zeroed target data, value and radius.
// @fnv 0x0067ff70
PackageTargetRecord::PackageTargetRecord()
{
    eTargType = 2;
    pTargetForm = 0;
    iValue = 0;
    fAcquireRadius = 0.0f;
}

// The shared Copy declaration returns void; this entry returns the target pointer.
// A pointer aggregate stays on the stack under __fastcall, preserving the original thiscall ABI.
struct PackageTargetCopySource
{
    PackageTargetRecord* record;
};

// @fnv 0x0067ffa0
// pdb: PackageTarget::PackageTarget | PackageTarget.obj | debug, object-file run extension, low
// pdb params: apPTFile
PackageTargetRecord* __fastcall CopyPackageTargetRecord(PackageTargetRecord* target, PackageTargetCopySource source)
{
    target->SetTargetKind(source.record->eTargType);
    target->SetTargValue(source.record->iValue);
    target->SetTargAcquireRadius(source.record->fAcquireRadius);
    switch (target->eTargType)
    {
    case 0:
    case 1:
        target->pTargetForm = source.record->pTargetForm;
        break;
    case 2:
        target->pTargetForm = source.record->pTargetForm;
        break;
    }
    return target;
}

// If the package target type is PTT_OBJECT, stores apObj in pObject.
// @fnv 0x00680140
// pdb: PackageTarget::SetTargObject | PackageTarget.obj | debug, call graph, high
// pdb params: apObj
void PackageTargetRecord::SetTargObject(TESForm* apObj)
{
    if (GetTargetKind() == PACK_TARG_TYPE_PTT_OBJECT)
        pObject = apObj;
}

// Returns true if apCompare is null or any of the compared values differ; returns false when all compared values match.
// @fnv 0x00680220
// pdb: PackageTarget::Compare | PackageTarget.obj | debug, call graph (shared callers), high
// pdb params: apCompare
bool PackageTargetRecord::Compare(PackageTargetRecord* apCompare)
{
    if (apCompare == 0)
        return true;
    if (this->GetTargetKind() != apCompare->GetTargetKind())
        return true;
    if (this->GetReferenceTarget() != apCompare->GetReferenceTarget())
        return true;
    if (this->GetTargetForm() != apCompare->GetTargetForm())
        return true;
    if (this->GetTargetObjectType() != apCompare->GetTargetObjectType())
        return true;
    if (this->GetTargValue() != apCompare->GetTargValue())
        return true;
    if (this->GetTargAcquireRadius() != apCompare->GetTargAcquireRadius())
        return true;
    return false;
}

struct ChunkPayload16
{
    void OBJ_WEAP_VATS_SPECIAL_Endian();
};
struct FormValueEntry
{
    unsigned char m_uiTargetType;
    unsigned int iFormID;
    int iValue;
    float m_fTargetValue;
};
extern "C" void* __cdecl memset(void* dest, int value, size_t count);
extern "C" bool __cdecl TESForm_QEndianSwapOnSave();
extern "C" void __cdecl AppendChunk(int id, FormValueEntry* data, unsigned int size);

// @fnv 0x006802f0
// src: XBOX_PDB 0x006802f0
// pdb: PackageTarget::Save | PackageTarget.obj | retail, upstream name already matches, high
// pdb params: aeChunk
// pdb locals: ptfile
void PackageTargetRecord::PackageTarget_Save(int aeChunk)
{
    FormValueEntry record;
    memset(&record, 0, 16);
    record.iValue = this->iValue;
    record.m_fTargetValue = this->fAcquireRadius;
    record.m_uiTargetType = this->eTargType;
    switch (this->eTargType)
    {
    case 2:
        record.iFormID = this->GetTargetObjectType();
        break;
    case 0:
    case 1:
        if (this->pObject)
            record.iFormID = this->pObject->GetFormID();
        break;
    }
    if (TESForm_QEndianSwapOnSave())
        ((ChunkPayload16*)&record)->OBJ_WEAP_VATS_SPECIAL_Endian();
    AppendChunk(aeChunk, &record, 16);
    if (TESForm_QEndianSwapOnSave())
        ((ChunkPayload16*)&record)->OBJ_WEAP_VATS_SPECIAL_Endian();
}

// @fnv 0x00680520
unsigned short PackageTargetRecord::GetSaveSize()
{
    unsigned short usSize = 0;
    usSize += 1;
    usSize += 4;
    usSize += 4;
    switch (eTargType)
    {
    case 3:
        break;
    case 0:
    case 1:
        usSize += 4;
        break;
    case 2:
        usSize += 4;
        break;
    default:;
    }
    return usSize;
}

// Writes the target type, value and radius, then the type-specific target.
// @fnv 0x00680720
// pdb: PackageTarget::SaveGame | PackageTarget.obj | retail, call graph (shared callers), high
// pdb locals: iformID
void PackageTargetRecord::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    apSaveGameBuffer->SaveData(this, 1, 0);
    apSaveGameBuffer->SaveData(&iValue, 4, 0);
    apSaveGameBuffer->SaveData(&fAcquireRadius, 4, 0);
    switch (eTargType)
    {
        break;
    case 0:
    case 1:
        apSaveGameBuffer->SerializeFormData(pTargetForm, 0);
        break;
    case 2:
        apSaveGameBuffer->SaveData(&eObjType, 4, 0);
        break;
    }
}

// @fnv 0x006807b0
// pdb: PackageTarget::LoadGame | PackageTarget.obj | retail, call graph (shared callers), high
// pdb locals: iformID
void PackageTargetRecord::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    apLoadGameBuffer->LoadData(this, 1);
    apLoadGameBuffer->LoadData(&iValue, 4);
    apLoadGameBuffer->LoadData(&fAcquireRadius, 4);
    switch (eTargType)
    {
        break;
    case 0:
    case 1:
        apLoadGameBuffer->FUN_008648e0(&pTargetForm);
        break;
    case 2:
        apLoadGameBuffer->LoadData(&pTargetForm, 4);
        break;
    }
}

// @fnv 0x00680830
// pdb: PackageTarget::InitLoadGame | PackageTarget.obj | retail, object-file run extension, high
// pdb locals: iformID
void PackageTargetRecord::InitLoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    switch (eTargType)
    {
    case PACK_TARG_TYPE_PTT_REFERENCE:
    case PACK_TARG_TYPE_PTT_OBJECT:
        pTargetForm = pTargetForm ? TESForm::GetFormByID((unsigned int)pTargetForm) : 0;
        break;
    }
}

extern TESSaveLoadGame* g_TESSaveLoadGame;

// Legacy save/load entry points have no declarations in the shared class.
// __fastcall passes the explicit target in ECX, matching their original no-argument thiscall ABI.
// @fnv 0x00680590
// pdb: PackageTarget::SaveGame | PackageTarget.obj | debug, object-file run extension, low
// pdb locals: iformID
void __fastcall SavePackageTargetRecordOLD(PackageTargetRecord* target)
{
    g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&target->eTargType, 1);
    g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&target->iValue, 4);
    g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&target->fAcquireRadius, 4);

    switch (target->eTargType)
    {
        break;
    case 0:
    case 1: {
        unsigned int uiResult = 0;
        if (target->pTargetForm)
        {
            uiResult = target->pTargetForm->GetFormID();
        }
        g_TESSaveLoadGame->TESSaveLoadGame_SaveNumericID((int)&uiResult, 4);
    }
    break;
    case 2:
        g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&target->eObjType, 4);
        break;
    }
}

TESForm* TESForm_GetFormByNumericID(unsigned int formID);

// For reference or object target types, replaces pTargetForm with the TESForm resolved from its formID via FUN_004839c0.
// @fnv 0x006806e0
// pdb: PackageTarget::InitLoadGame | PackageTarget.obj | retail, object-file run extension, low
// pdb locals: iformID
void __fastcall ResolvePackageTargetForm(PackageTargetRecord* target)
{
    switch (target->eTargType)
    {
    case PACK_TARG_TYPE_PTT_REFERENCE:
    case PACK_TARG_TYPE_PTT_OBJECT:
        unsigned int formID = reinterpret_cast<unsigned int>(target->pTargetForm);
        target->pTargetForm = TESForm_GetFormByNumericID(formID);
    }
}

struct SaveLoadStub
{
    void TESSaveLoadGame_LoadGameDataOLD(void* apData, int aiSize);
    void FUN_00857aa0(void* data, int size);
};
extern SaveLoadStub* g_saveLoad_011de45c;
// @fnv 0x00680640
// pdb: PackageTarget::LoadGame | PackageTarget.obj | debug, object-file run extension, low
// pdb locals: iformID
void __fastcall LoadPackageTargetRecordOLD(PackageTargetRecord* target)
{
    g_saveLoad_011de45c->TESSaveLoadGame_LoadGameDataOLD(target, 1);
    g_saveLoad_011de45c->TESSaveLoadGame_LoadGameDataOLD(&target->iValue, 4);
    g_saveLoad_011de45c->TESSaveLoadGame_LoadGameDataOLD(&target->fAcquireRadius, 4);
    switch (target->eTargType)
    {
        break;
    case 0:
    case 1: {
        int value;
        g_saveLoad_011de45c->FUN_00857aa0(&value, 4);
        target->eObjType = (PACKAGE_OBJECT_TYPE)value;
    }
    break;
    case 2:
        g_saveLoad_011de45c->TESSaveLoadGame_LoadGameDataOLD(&target->eObjType, 4);
        break;
    }
}
