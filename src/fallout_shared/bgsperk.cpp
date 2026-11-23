// fallout_shared/bgsperk.cpp: BGSEntryPointPerkEntry, BGSPerk, BGSAbilityPerkEntry, BGSQuestPerkEntry and more (56 functions)

#include "../../include/Actor.h"
#include "../../include/TESAIForm.h"
#include "../../include/TESQuest.h"
#include "../../include/BGSAbilityPerkEntry.h"
#include "../../include/BGSCameraPath.h"
#include "../../include/BGSEntryPointFunctionData.h"
#include "../../include/BGSEntryPointPerkEntry.h"
#include "../../include/BGSPerk.h"
#include "../../include/BGSQuestPerkEntry.h"
#include "../../include/BSSimpleList.h"
#include "../../include/Script.h"
#include "../../include/BGSPerkEntry.h"
#include "../../include/TESBoundObject.h"
#include "../../include/PerkData.h"
#include "../../include/PerkEntryConditionSet.h"
#include "../../include/PerkEntryPointData.h"
#include "../../include/SpellItem.h"
#include "../../include/TESConditionItem.h"
#include "../../include/TESDescription.h"
#include "../../include/TESFile.h"
#include "../../include/TESFullName.h"
#include "../../include/TESIcon.h"
#include "../../include/TESObjectREFR.h"
#include "../../include/TESTexture.h"

// Unit support declarations kept here to share TESForm and BSSimpleList with Script.
// Fallout shared perk entry declarations, reconstructed to the PC release layout.
#ifndef FNV_FALL_SHARED_BGSPERK_WORKER_H
#define FNV_FALL_SHARED_BGSPERK_WORKER_H

#include <stddef.h>

#define FNV_JOIN_IMPL(a, b) a##b
#define FNV_JOIN(a, b) FNV_JOIN_IMPL(a, b)
#define FNV_STATIC_ASSERT(condition) typedef char FNV_JOIN(fnv_static_assert_, __LINE__)[1]


#include "../../include/ENUM_FORM_ID.h"


class PerkEntryFunctionType
{
  public:
    virtual void Unk_00();
    virtual void Unk_01();
    virtual void Unk_02();
    virtual void Unk_03();
    virtual int GetFunctionType();
};

class RegionDataEntry
{
  public:
    void Endian();
};

#ifndef __GNUC__
FNV_STATIC_ASSERT(sizeof(BGSPerkEntry) == 0x8);
FNV_STATIC_ASSERT(sizeof(BGSEntryPointPerkEntry) == 0x14);
FNV_STATIC_ASSERT(offsetof(BGSEntryPointPerkEntry, Data) == 0x8);
FNV_STATIC_ASSERT(offsetof(BGSEntryPointPerkEntry, m_pFunctionData) == 0xC);
FNV_STATIC_ASSERT(offsetof(BGSEntryPointPerkEntry, m_pConditions) == 0x10);
#endif

#endif

// File-local operations missing from BGSPerk.h; storage is always a shared BGSPerk.
class EB900Entry;
struct PerkOperations
{
    void ClearConditions();
    void Save();
    void CopyFrom(int apCopy);
    void CopyFormComponents(int source);
    bool Compare(TESForm* apCompare);
    bool CompareFormComponents(TESForm* apCompare);
    void UpdateEntries(void* actor, unsigned char oldRank, unsigned char newRank, bool alternate);
    EB900Entry* FindEntryByLevel(unsigned char level);
};

BGSPerkEntry* CreatePerkEntry(unsigned char type);

void* __cdecl operator new[](size_t);
void __cdecl operator delete[](void*);
BGSEntryPointFunctionData* BGSEntryPointFunctionData_CreateFunctionDataOfType(unsigned char);
int Warning(const char*, ...);

FNV_STATIC_ASSERT(sizeof(TESObjectREFR) == 0x68);

template <int N> struct EndianSizeTag
{
};
void BSEndian(unsigned int*, EndianSizeTag<4>);

FNV_STATIC_ASSERT(sizeof(TESForm) == 0x18);
FNV_STATIC_ASSERT(sizeof(TESFullName) == 0x0c);
FNV_STATIC_ASSERT(sizeof(TESDescription) == 0x08);
FNV_STATIC_ASSERT(sizeof(TESIcon) == 0x0c);
FNV_STATIC_ASSERT(sizeof(BGSPerk) == 0x50);
FNV_STATIC_ASSERT(offsetof(BGSPerk, Data) == 0x38);
FNV_STATIC_ASSERT(offsetof(BGSPerk, m_kConditions) == 0x40);
FNV_STATIC_ASSERT(offsetof(BGSPerk, m_kEntryList) == 0x48);
FNV_STATIC_ASSERT(sizeof(BGSQuestPerkEntry) == 0x10);
FNV_STATIC_ASSERT(sizeof(BGSAbilityPerkEntry) == 0x0c);

int ComparePerkEntries(BGSPerkEntry*, BGSPerkEntry*);

extern unsigned int BGSAbilityPerkEntry_vtable[];
#include <malloc.h>
#include "../../include/PatrolActorPackageData.h"

// @fnv 0x005e9fb0
unsigned char BGSEntryPointPerkEntry::GetFunction()
{
    return Data.m_ucFunction;
}

// Calls the actor's virtual method at vtable index 0x129, passing this entry point and the apply flag.
// @fnv 0x005eaa50
// pdb: BGSEntryPointPerkEntry::ApplyPerkEntry | BGSPerk.obj | debug, vtable slot, medium
// pdb params: apActor, bAlt
void BGSEntryPointPerkEntry::ApplyEntryPointEffect(Actor* actor, bool apply)
{
    reinterpret_cast<void(__thiscall*)(Actor*, BGSEntryPointPerkEntry*, bool)>(
        (*reinterpret_cast<void***>(actor))[0x129])(actor, this, apply);
}

// @fnv 0x005ea330
// pdb: BGSEntryPointPerkEntry::InitItem | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apOwner
// pdb locals: i
void BGSEntryPointPerkEntry::InitItem(TESForm* apOwner)
{
    unsigned int i;

    for (i = 0; i < Data.m_ucConditionCount; ++i)
        m_pConditions[i].InitItem(apOwner);

    if (m_pFunctionData)
        m_pFunctionData->InitItem(apOwner);
}

// @fnv 0x005ead20
// pdb: BGSPerk::InitItem | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: plist, pentry
void BGSPerk::InitItem()
{
    struct EntryListLocals
    {
        BGSPerkEntry* entry;
        BSSimpleList<BGSPerkEntry*>* list;
    } locals;

    if (static_cast<const TESForm*>(this)->GetInitialized())
        return;

    m_kConditions.InitItem(reinterpret_cast<TESForm*>(this));
    locals.list = &m_kEntryList;
    while (locals.list)
    {
        locals.entry = locals.list->GetItem();
        if (locals.entry)
            locals.entry->InitItem(reinterpret_cast<TESForm*>(this));
        locals.list = locals.list->GetNext();
    }
    SetInitializationState(true);
}

// Iterates the perk entry list and runs each entry whose rank matches rank-1 and whose function type is 2.
// @fnv 0x005eb980
// pdb: BGSPerk::PerkEntryRankSortFunction | BGSPerk.obj | retail, object-file run extension, low
// pdb note: debug build links it to BGSPerk::AddPerkEntryPoints
// pdb params: apPerkEntry1, apPerkEntry2
void BGSPerk::ApplyPerkEntries(Actor* actor, unsigned char rank, bool alternate)
{
    struct EntryListLocals
    {
        BGSPerkEntry* entry;
        BSSimpleList<BGSPerkEntry*>* list;
    } locals;

    locals.list = &m_kEntryList;
    while (locals.list)
    {
        locals.entry = locals.list->GetItem();
        if (locals.entry && reinterpret_cast<PatrolActorPackageData*>(locals.entry)->IsPatrolling() + 1 == rank &&
            reinterpret_cast<PerkEntryFunctionType*>(locals.entry)->GetFunctionType() == 2)
        {
            locals.entry->ApplyEntryPointEffect(actor, alternate);
        }
        locals.list = locals.list->GetNext();
    }
}

// @fnv 0x005eada0
// pdb: BGSPerk::Load | BGSPerk.obj | retail, vtable slot, high
// pdb params: apFile
// pdb locals: chunkid, formid, pMessage, saveData, pperkEntry, isize, temp
bool BGSPerk::LoadForm(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x56)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    unsigned int chunk;
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch ((int)chunk)
        {
        case 'DIDE': {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 'DNBO':
            readOBNDSubRecord(apFile);
            break;
        case 'LLUF':
            TESFullName::LoadFullNameChunk(this, apFile);
            break;
        case 'CSED':
            TESDescription::ReadDescriptionFromFile(this, apFile);
            break;
        case 'NOCI':
            TESTexture::ReadTextureData(this, apFile);
            break;
        case 'ADTC':
            m_kConditions.Load(apFile);
            break;
        case 'EKRP': {
            struct
            {
                struct
                {
                    unsigned char type, rank, priority;
                } data;
                BGSPerkEntry* entry;
                unsigned int size;
            } prkeRecord;
            prkeRecord.size = apFile->GetChunkSize();
            if (prkeRecord.size != 3)
                return false;
            apFile->GetChunkData(&prkeRecord.data, prkeRecord.size);
            prkeRecord.entry = CreatePerkEntry(prkeRecord.data.type);
            if (!prkeRecord.entry)
                return false;
            prkeRecord.entry->LoadFromSaveData(&prkeRecord.data);
            if (!prkeRecord.entry->Load(apFile))
                return false;
            AppendPerkEntry(prkeRecord.entry);
            break;
        }
        case 'ATAD': {
            unsigned int size = apFile->GetChunkSize();
            if (size != 5)
            {
                if (size == 3)
                {
                    struct
                    {
                        bool trait;
                        unsigned char level;
                        unsigned char ranks;
                    } data;
                    data.trait = false;
                    data.level = 0;
                    data.ranks = 1;
                    apFile->GetChunkData(&data, 3);
                    Data.m_bTrait = data.trait;
                    Data.cLevel = data.level;
                    Data.m_ucRankCount = data.ranks;
                }
                else if (size == 4)
                {
                    struct
                    {
                        bool trait;
                        unsigned char level;
                        unsigned char ranks;
                        bool playable;
                    } data;
                    data.trait = false;
                    data.level = 0;
                    data.ranks = 1;
                    data.playable = true;
                    apFile->GetChunkData(&data, 4);
                    Data.m_bTrait = data.trait;
                    Data.cLevel = data.level;
                    Data.m_ucRankCount = data.ranks;
                    Data.bPlayable = data.playable;
                }
                else
                    return false;
            }
            else
                apFile->GetChunkData(&Data, 5);
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x005ea790
// pdb: BGSEntryPointPerkEntry::Load | BGSPerk.obj | retail, string-set bridge, high
// pdb params: apFile
// pdb locals: ccurrentConditionsIndex, chunkid, isize, cfunctionDataType
bool BGSEntryPointPerkEntry::Load(TESFile* apFile)
{
    unsigned int chunk;
    unsigned char condition = 0;
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch ((int)chunk)
        {
        case 'ATAD': {
            unsigned int size = apFile->GetChunkSize();
            if (size != 3)
                return false;
            apFile->GetChunkData(&Data, size);
            m_pConditions = new PerkEntryConditionSet[Data.m_ucConditionCount];
            break;
        }
        case 'CKRP':
            apFile->GetChunkData(&condition, 1);
            break;
        case 'ADTC':
            if (!m_pConditions)
                return false;
            m_pConditions[condition].Load(apFile);
            break;
        case 'TFPE': {
            unsigned char type = 0;
            unsigned int size = apFile->GetChunkSize();
            if (size != 1)
                return false;
            apFile->GetChunkData(&type, 0);
            m_pFunctionData = BGSEntryPointFunctionData_CreateFunctionDataOfType(type);
            if (!m_pFunctionData)
                return false;
            break;
        }
        case 'DFPE':
        case '2FPE':
        case 'RHCS':
        case 'ADCS':
        case 'XTCS':
        case 'ORCS':
        case 'VRCS':
            if (!m_pFunctionData)
            {
                Warning("MASTERFILE: Function data chunk is loading without function data existing on perk entry");
                return false;
            }
            m_pFunctionData->Load(apFile);
            break;
        case 'FKRP':
            return true;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x005ea060
// pdb: BGSEntryPointPerkEntry::CheckConditionFilters | BGSPerk.obj | retail, string-set bridge, high
// pdb params: aiNumFilterForms, apFilterForms
// pdb locals: i, pref, tempRef, pform, bignore
bool BGSEntryPointPerkEntry::CheckConditionFilters(unsigned int aiNumFilterForms, TESForm** apFilterForms)
{
    if (aiNumFilterForms != Data.m_ucConditionCount)
    {
        Warning(
            "FORMS: BGSEntryPointPerkEntry::CheckConditionFilters() was called with %i filter forms, but there are %i conditions",
            aiNumFilterForms, Data.m_ucConditionCount);
        return false;
    }
    unsigned int i;
    TESForm* form;
    for (i = 0; i < aiNumFilterForms; ++i)
    {
        form = apFilterForms[i];
        TESObjectREFR* reference = 0;
        TESObjectREFR temporary;
        temporary.NotifySaveLoadGameOfFormChange();
        if (form->IsActor_InEditor())
        {
            temporary.ClearReferenceState(static_cast<TESBoundObject*>(form));
            reference = &temporary;
        }
        else if (form->GetIsReference())
            reference = static_cast<TESObjectREFR*>(form);
        if (reference)
        {
            bool result;
            if (!m_pConditions[i].IsTrue(reference, 0, result, false))
                return false;
        }
        else
            Warning("DEFAULT: Bad Filter form %08X %s sent to BGSEntryPointPerkEntry::CheckConditionFilters.",
                    reference->GetFormID(), reference->GetName());
    }
    return true;
}

// @fnv 0x005eaab0
// pdb: BGSPerk::BGSPerk | BGSPerk.obj | retail, upstream name already matches, high
BGSPerk::BGSPerk()
{
    SetFormType(ENUM_FORM_ID_PERK_ID);
    ResetFormData();
}

// Resolves Data.pQuest by form ID and warns if the quest cannot be found, naming the owner object by name or form ID.
// @fnv 0x005e9880
// pdb: BGSQuestPerkEntry::InitItem | BGSPerk.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: itempint
void BGSQuestPerkEntry::InitItem(TESForm* apOwner)
{
    if (Data.pQuest)
    {
        unsigned int id = reinterpret_cast<unsigned int>(Data.pQuest);
        TESForm::RemapFormIDForFile(id, apOwner->GetFile(-1));
        Data.pQuest = dynamic_cast<TESQuest*>(TESForm::GetFormByID(id));
        if (!Data.pQuest)
        {
            if (reinterpret_cast<BGSCameraPath*>(apOwner)->HasNameForWarning())
                Warning("MASTERFILE: Unable to find quest (%08X) on owner object \"%s\".", id, apOwner->GetName());
            else
                Warning("MASTERFILE: Unable to find quest (%08X) on owner object (%08X).", id, apOwner->GetFormID());
        }
    }
}

// Resolves the stored ability form ID against the owner's file and warns if the resulting form is not a SpellItem.
// @fnv 0x005e9c00
// pdb: BGSAbilityPerkEntry::InitItem | BGSPerk.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: itempint
void BGSAbilityPerkEntry::InitItem(TESForm* apOwner)
{
    if (pAbility)
    {
        unsigned int id = reinterpret_cast<unsigned int>(pAbility);
        TESForm::RemapFormIDForFile(id, apOwner->GetFile(-1));
        pAbility = dynamic_cast<SpellItem*>(TESForm::GetFormByID(id));
        if (!pAbility)
        {
            if (reinterpret_cast<BGSCameraPath*>(apOwner)->HasNameForWarning())
                Warning("MASTERFILE: Unable to find ability (%08X) on owner object \"%s\".", id, apOwner->GetName());
            else
                Warning("MASTERFILE: Unable to find ability (%08X) on owner object (%08X).", id, apOwner->GetFormID());
        }
    }
}

// Reads the DATA chunk (4-byte spell id, endian-swapped if needed) into the perk entry and stops at the PKRF chunk.
// @fnv 0x005e9e20
// pdb: BGSAbilityPerkEntry::Load | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, isize, iabilityID
bool BGSAbilityPerkEntry::Load(TESFile* apFile)
{
    unsigned int chunk;
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch ((int)chunk)
        {
        case 'ATAD': {
            unsigned int size = apFile->GetChunkSize();
            if (size != 4)
                return false;
            unsigned int id = 0;
            apFile->GetChunkData(&id, 0);
            if (apFile->IsEndianConversionRequired())
                BSEndian(&id, EndianSizeTag<4>());
            pAbility = reinterpret_cast<SpellItem*>(id);
            break;
        }
        case 'FKRP':
            return true;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// Reads chunks from the file, loading the 8-byte DATA chunk into Data and converting endianness if required, until the FKRP chunk or end of chunks.
// @fnv 0x005e9b00
// pdb: BGSQuestPerkEntry::Load | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, isize
bool BGSQuestPerkEntry::Load(TESFile* apFile)
{
    unsigned int chunk;
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch ((int)chunk)
        {
        case 'ATAD': {
            unsigned int size = apFile->GetChunkSize();
            if (size != 8)
                return false;
            apFile->GetChunkData(&Data, size);
            if (apFile->IsEndianConversionRequired())
                reinterpret_cast<RegionDataEntry*>(&Data)->Endian();
            break;
        }
        case 'FKRP':
            return true;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x005eb460
// pdb: BGSPerk::PerkEntryRankSortFunction | BGSPerk.obj | debug, object-file run extension, low
// pdb params: apPerkEntry1, apPerkEntry2
int ComparePerkEntries(BGSPerkEntry* apEntryA, BGSPerkEntry* apEntryB)
{
    if (reinterpret_cast<PatrolActorPackageData*>(apEntryA)->IsPatrolling() <
        reinterpret_cast<PatrolActorPackageData*>(apEntryB)->IsPatrolling())
        return -1;
    else if (reinterpret_cast<PatrolActorPackageData*>(apEntryA)->IsPatrolling() >
             reinterpret_cast<PatrolActorPackageData*>(apEntryB)->IsPatrolling())
        return 1;
    else if (reinterpret_cast<TESAIForm*>(apEntryA)->_GetByte05() >
             reinterpret_cast<TESAIForm*>(apEntryB)->_GetByte05())
        return -1;
    else if (reinterpret_cast<TESAIForm*>(apEntryA)->_GetByte05() <
             reinterpret_cast<TESAIForm*>(apEntryB)->_GetByte05())
        return 1;
    else if (reinterpret_cast<PerkEntryFunctionType*>(apEntryA)->GetFunctionType() <
             reinterpret_cast<PerkEntryFunctionType*>(apEntryB)->GetFunctionType())
        return -1;
    else if (reinterpret_cast<PerkEntryFunctionType*>(apEntryA)->GetFunctionType() >
             reinterpret_cast<PerkEntryFunctionType*>(apEntryB)->GetFunctionType())
        return 1;
    return 0;
}

// Inserts entry into the perk entry list if it is non-null and not already present, using ComparePerkEntries for ordering.
// @fnv 0x005eb410
bool BGSPerk::AppendPerkEntry(BGSPerkEntry* entry)
{
    if (!entry || m_kEntryList.IsInList(entry))
        return false;
    m_kEntryList.Insert(entry, ComparePerkEntries);
    return true;
}

// @fnv 0x005e9bd0
BGSAbilityPerkEntry* BGSAbilityPerkEntry::Initialize()
{
    this->BGSPerkEntry::BGSPerkEntry();
    *reinterpret_cast<unsigned int**>(this) = BGSAbilityPerkEntry_vtable;
    pAbility = 0;
    return this;
}

// @fnv 0x005eb680
// src: XBOX_PDB 0x005eb680
unsigned int TESConditionItem::GetConditionFunction()
{
    return m_usFunctionCode;
}

struct RGBColor
{
    void Endian();
    unsigned char m_ucRedValue;
    unsigned char m_ucRank;
    unsigned char m_ucRegionPriority;
};
typedef char _sizecheck_PerkEntrySaveData[1];

// Copies the two bytes from the save data into the perk entry at offsets 4 and 5.
// @fnv 0x005e96b0
void BGSPerkEntry::LoadFromSaveData(const void* data)
{
    *((unsigned char*)this + 4) = static_cast<const RGBColor*>(data)->m_ucRank;
    *((unsigned char*)this + 5) = static_cast<const RGBColor*>(data)->m_ucRegionPriority;
}

// @fnv 0x005ec040
// pdb: BSSimpleList<BGSPerkEntry *>::~BSSimpleList<BGSPerkEntry *> | BGSPerk.obj | debug, object-file run extension, low
bool TESConditionItem::IsFlagSet()
{
    return (Data & 1) ? true : false;
}

// Extracts bits 5-7 of the first byte of this condition item and returns them as an integer.
// @fnv 0x005ec060
int TESConditionItem::GetComparisonCondition()
{
    int ival = Data & 0xe0;
    ival >>= 5;
    return ival;
}

// Copies the two BGSPerkEntry fields from apCopy into this entry.
// @fnv 0x005e95b0
void __thiscall BGSPerkEntry::Copy(BGSPerkEntry* apCopy)
{
    this->cRank = apCopy->cRank;
    this->cPriority = apCopy->cPriority;
}

// @fnv 0x005e95e0
bool BGSPerkEntry::Compare(BGSPerkEntry* apCompare)
{
    if (cRank != apCompare->cRank)
        return true;
    if (cPriority != apCompare->cPriority)
        return true;
    return false;
}

// @fnv 0x005eb7e0
// src: COMMUNITY 0x005eb7e0
void* __fastcall PlayerCharacter_GetTeammates(void* this_ptr)
{
    return reinterpret_cast<char*>(this_ptr) + 0x5FC;
}

// Allocates and returns a new perk entry object matching the given perk entry type, or null for unknown types.
// @fnv 0x005e96e0
// pdb: BGSPerkEntry::CreatePerkEntryOfType | BGSPerk.obj | debug, object-file run extension, low
// pdb params: aePerkEntryType
BGSPerkEntry* CreatePerkEntry(int aePerkEntryType)
{
    switch (aePerkEntryType)
    {
    case 0:
        return new BGSQuestPerkEntry;
    case 1:
        return new BGSAbilityPerkEntry;
    case 2:
        return new BGSEntryPointPerkEntry;
    }
    return 0;
}

struct PerkEntrySerialization
{
    void WriteHeader(int);
    void WriteEnd();
};
bool TESForm_QEndianSwapOnSave();
void AppendChunk(unsigned int, void*, unsigned int);

// @fnv 0x005e9a70
// pdb: BGSQuestPerkEntry::Save | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: ptempQuest
void BGSQuestPerkEntry::Save()
{
    TESQuest* ptempQuest;
    reinterpret_cast<PerkEntrySerialization*>(this)->WriteHeader(0);
    ptempQuest = Data.pQuest;
    if (Data.pQuest)
        Data.pQuest = (TESQuest*)Data.pQuest->GetFormID();
    if (TESForm_QEndianSwapOnSave())
        reinterpret_cast<RegionDataEntry*>(&Data)->Endian();
    AppendChunk(0x41544144, &Data, 8);
    if (TESForm_QEndianSwapOnSave())
        reinterpret_cast<RegionDataEntry*>(&Data)->Endian();
    Data.pQuest = ptempQuest;
    reinterpret_cast<PerkEntrySerialization*>(this)->WriteEnd();
}

// Deletes the condition list and function data, zeroes the entry point data bytes, and nulls both pointers.
// @fnv 0x005ea230
// pdb: BGSEntryPointPerkEntry::ClearData | BGSPerk.obj | retail, upstream name already matches, high
void BGSEntryPointPerkEntry::ClearData()
{
    delete[] m_pConditions;
    delete m_pFunctionData;
    Data.m_ucEntryPoint = 0;
    Data.m_ucFunction = 0;
    Data.m_ucConditionCount = 0;
    m_pConditions = 0;
    m_pFunctionData = 0;
}

// @fnv 0x005eb550
// src: COMMUNITY 0x005eb550
// pdb: BGSPerk::PerkEntryPrioritySortFunction | BGSPerk.obj | debug, upstream name already matches, high
// pdb params: apPerkEntry1, apPerkEntry2
int BGSPerk_PerkEntryPrioritySortFunction(BGSPerkEntry* apPerkEntry1, BGSPerkEntry* apPerkEntry2)
{
    if (reinterpret_cast<TESAIForm*>(apPerkEntry1)->_GetByte05() > reinterpret_cast<TESAIForm*>(apPerkEntry2)->_GetByte05())
        return -1;
    else if (reinterpret_cast<TESAIForm*>(apPerkEntry1)->_GetByte05() <
             reinterpret_cast<TESAIForm*>(apPerkEntry2)->_GetByte05())
        return 1;
    else if (reinterpret_cast<PatrolActorPackageData*>(apPerkEntry1)->IsPatrolling() <
             reinterpret_cast<PatrolActorPackageData*>(apPerkEntry2)->IsPatrolling())
        return -1;
    else if (reinterpret_cast<PatrolActorPackageData*>(apPerkEntry1)->IsPatrolling() >
             reinterpret_cast<PatrolActorPackageData*>(apPerkEntry2)->IsPatrolling())
        return 1;
    return 0;
}

class PlayerCharacter;
extern PlayerCharacter* g_thePlayer;
struct ConditionList_t
{
    bool FUN_008256d0() const;
    TESConditionItem* const* FUN_006815c0() const;
    const ConditionList_t* FUN_00726070() const;
};
struct Conditions_t
{
    const ConditionList_t* FUN_006815c0();
};
// If the flag at offset 0x3b is set, walks the condition list and evaluates each item of type 0x46 against the player, returning the last result.
// @fnv 0x005eb5e0
// pdb: BGSPerk::GetIsPlayable | BGSPerk.obj | retail, call graph (shared callers), medium
// pdb locals: bresult, plist, pitem, bignore
bool BGSPerk::CheckPlayerConditions()
{
    bool bresult;
    const ConditionList_t* pCurrentElementNext;
    bresult = *(reinterpret_cast<bool*>(this) + 0x3b);
    if (bresult)
    {
        pCurrentElementNext = reinterpret_cast<Conditions_t*>(reinterpret_cast<char*>(this) + 0x40)->FUN_006815c0();
        while (bresult && pCurrentElementNext && !pCurrentElementNext->FUN_008256d0())
        {
            TESConditionItem* pConditionItem = *pCurrentElementNext->FUN_006815c0();
            if (pConditionItem && pConditionItem->GetConditionFunction() == 0x46)
            {
                bool bignore;
                bresult = pConditionItem->IsTrue(reinterpret_cast<TESObjectREFR*>(g_thePlayer), 0, bignore);
            }
            pCurrentElementNext = pCurrentElementNext->FUN_00726070();
        }
    }
    return bresult;
}

struct FUN_00470440
{
    FUN_00470440(BGSPerkEntry* const& apItem);
    void FUN_005ae3d0(BGSPerkEntry* const& apItem);
    void BSSimpleList_ActiveEffect_Insert(BGSPerkEntry* apNewItem, int(__cdecl* apCompareFn)(BGSPerkEntry*, BGSPerkEntry*));
    BGSPerkEntry* m_item;
    FUN_00470440* m_pkNext;
};
// Inserts apNewItem into the linked list in sorted order using apCompareFn, filling an empty node or allocating a new node as needed.
// @fnv 0x007a7eb0
// src: XBOX_PDB 0x007a7eb0
// pdb: BSSimpleList<LoadFormHeader *>::Insert | TESSaveLoadGame.obj | retail, call graph (shared callers), low
// pdb note: debug build links it to BSSimpleList<MiscSaveStat *>::Insert
// pdb params: apNewItem, apCompareFn
// pdb locals: pkPrev, bdone, pkCur, pkNew
void FUN_00470440::BSSimpleList_ActiveEffect_Insert(BGSPerkEntry* apNewItem, int(__cdecl* apCompareFn)(BGSPerkEntry*, BGSPerkEntry*))
{
    FUN_00470440* pkPrev;
    bool bdone;
    FUN_00470440* pkCur;
    if (!apNewItem)
        return;
    pkCur = this;
    pkPrev = 0;
    bdone = false;
    while (pkCur && !bdone)
    {
        if (!pkCur->m_item)
        {
            pkCur->m_item = apNewItem;
            bdone = true;
        }
        else if (apCompareFn(apNewItem, pkCur->m_item) <= 0)
        {
            if (!pkPrev)
            {
                FUN_005ae3d0(apNewItem);
            }
            else
            {
                FUN_00470440* pkNew = new FUN_00470440(apNewItem);
                pkPrev->m_pkNext = pkNew;
                pkNew->m_pkNext = pkCur;
            }
            bdone = true;
        }
        else if (!pkCur->m_pkNext)
        {
            FUN_00470440* pkNew = new FUN_00470440(apNewItem);
            pkCur->m_pkNext = pkNew;
            bdone = true;
        }
        pkPrev = pkCur;
        pkCur = pkCur->m_pkNext;
    }
}

void __fastcall FUN_00483710(void* apData);
void _Deallocate(void*);

// @fnv 0x005e9560
// pdb: BGSPerkEntry::`scalar deleting destructor' | BGSPerk.obj | debug, vtable slot, medium
void* BGSPerkEntry::Destroy(unsigned int flags)
{
    BGSPerkEntry::~BGSPerkEntry();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

void WritePerkAbilityID(unsigned int, unsigned int);
// @fnv 0x005e9dd0
// pdb: BGSAbilityPerkEntry::Save | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: iabilityID
void BGSAbilityPerkEntry::Save()
{
    unsigned int iabilityID;
    reinterpret_cast<PerkEntrySerialization*>(this)->WriteHeader(1);
    if (pAbility)
    {
        iabilityID = pAbility->GetFormID();
        WritePerkAbilityID(0x41544144, iabilityID);
    }
    reinterpret_cast<PerkEntrySerialization*>(this)->WriteEnd();
}

void _Deallocate(void*);

// @fnv 0x005e9fd0
// pdb: BGSEntryPointPerkEntry::`scalar deleting destructor' | BGSPerk.obj | retail, vtable slot, high
void* BGSEntryPointPerkEntry::Destroy(unsigned int flags)
{
    BGSEntryPointPerkEntry::~BGSEntryPointPerkEntry();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

// @fnv 0x005ea000
// pdb: BGSEntryPointPerkEntry::~BGSEntryPointPerkEntry | BGSPerk.obj | debug, upstream name already matches, high
BGSEntryPointPerkEntry::~BGSEntryPointPerkEntry()
{
    BGSEntryPointPerkEntry::ClearData();
}

// Calls the actor virtual method at index 0x12A with this entry and bAlt.
// @fnv 0x005eaa80
// pdb: BGSEntryPointPerkEntry::RemovePerkEntry | BGSPerk.obj | debug, vtable slot, medium
// pdb params: apActor, bAlt
void BGSEntryPointPerkEntry::RemoveEntryPointEffect(Actor* apActor, bool bAlt)
{
    reinterpret_cast<void(__thiscall*)(Actor*, BGSEntryPointPerkEntry*, bool)>(
        (*reinterpret_cast<void***>(apActor))[0x12A])(apActor, this, bAlt);
}

void _Deallocate(void*);

// @fnv 0x005e9ba0
// pdb: BGSQuestPerkEntry::ApplyPerkEntry | BGSPerk.obj | debug, vtable slot, medium
// pdb params: apActor, bAlt
void BGSQuestPerkEntry::ApplyEntryPointEffect(Actor* apActor, bool bAlt)
{
    if (Data.pQuest)
        Data.pQuest->SetStageDone(Data.stage);
}

// @fnv 0x005e9ee0
// pdb: BGSAbilityPerkEntry::ApplyPerkEntry | BGSPerk.obj | retail, vtable slot, high
// pdb params: apActor, bAlt
void BGSAbilityPerkEntry::ApplyEntryPointEffect(Actor* apActor, bool bAlt)
{
    if (pAbility != 0)
    {
        reinterpret_cast<void(__thiscall*)(Actor*, void*)>(
            (*reinterpret_cast<void***>(apActor))[0xF8])(apActor, pAbility);
        apActor->ConfigureActorState(false, false);
    }
}

// @fnv 0x005e9f20
// pdb: BGSAbilityPerkEntry::RemovePerkEntry | BGSPerk.obj | retail, vtable slot, high
// pdb params: apActor, bAlt
void BGSAbilityPerkEntry::RemoveEntryPointEffect(Actor* apActor, bool bAlt)
{
    if (pAbility != 0)
    {
        reinterpret_cast<void(__thiscall*)(Actor*, void*)>(
            (*reinterpret_cast<void***>(apActor))[0xF9])(apActor, pAbility);
        apActor->ConfigureActorState(false, false);
    }
}

struct FUN_004855a0
{
    void M();
};

struct FUN_00485680
{
    void M();
};

bool TESForm_QEndianSwapOnSave();
void AppendChunk(unsigned int, void*, int);
// Calls M on several subobjects, clears and serializes the data at offset 0x38, then iterates the perk-entry list calling vfn_28 on each entry.
// @fnv 0x005eb0e0
// pdb: BGSPerk::Save | BGSPerk.obj | retail, vtable slot, high
// pdb locals: plist, pentry
void PerkOperations::Save()
{
    ((FUN_004855a0*)this)->M();
    ((TESFullName*)((char*)this + 0x18))->Save_00487010();
    ((TESDescription*)((char*)this + 0x24))->Save_00482cb0();
    ((TESTexture*)((char*)this + 0x2c))->Save(0x4e4f4349);
    ((PerkEntryConditionSet*)((char*)this + 0x40))->Save();
    if (TESForm_QEndianSwapOnSave())
        FUN_00483710((char*)this + 0x38);
    AppendChunk(0x41544144, (char*)this + 0x38, 5);
    if (TESForm_QEndianSwapOnSave())
        FUN_00483710((char*)this + 0x38);
    BSSimpleList<BGSPerkEntry*>* pCurrentElementNext = (BSSimpleList<BGSPerkEntry*>*)((char*)this + 0x48);
    while (pCurrentElementNext)
    {
        BGSPerkEntry* pPerkEntry = pCurrentElementNext->GetItem();
        if (pPerkEntry)
            pPerkEntry->Save();
        pCurrentElementNext = pCurrentElementNext->GetNext();
    }
    ((FUN_00485680*)this)->M();
}

// @fnv 0x005e9940
// pdb: BGSQuestPerkEntry::Duplicate | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: pduplicate
BGSPerkEntry* BGSQuestPerkEntry::Duplicate()
{
    BGSQuestPerkEntry* pduplicate = new BGSQuestPerkEntry;
    pduplicate->Data.pQuest = Data.pQuest;
    pduplicate->Data.stage = Data.stage;
    pduplicate->Copy(this);
    return pduplicate;
}

struct PerkConditionCopier
{
    void Copy(PerkEntryConditionSet*, PerkEntryConditionSet*);
};

// @fnv 0x005ea3a0
// pdb: BGSEntryPointPerkEntry::Duplicate | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: pduplicate, i
BGSPerkEntry* BGSEntryPointPerkEntry::Duplicate()
{
    BGSPerkEntry* pduplicate = (BGSPerkEntry*)(void*)new BGSEntryPointPerkEntry();

    ((BGSEntryPointPerkEntry*)pduplicate)->Data.m_ucEntryPoint = Data.m_ucEntryPoint;
    ((BGSEntryPointPerkEntry*)pduplicate)->Data.m_ucFunction = Data.m_ucFunction;
    ((BGSEntryPointPerkEntry*)pduplicate)->Data.m_ucConditionCount = Data.m_ucConditionCount;

    if (m_pFunctionData)
        ((BGSEntryPointPerkEntry*)pduplicate)->m_pFunctionData =
            m_pFunctionData->Duplicate();

    if (Data.m_ucConditionCount)
    {
        ((BGSEntryPointPerkEntry*)pduplicate)->m_pConditions = new PerkEntryConditionSet[Data.m_ucConditionCount];

        for (unsigned int i = 0; i < Data.m_ucConditionCount; i++)
        {
            reinterpret_cast<PerkConditionCopier*>(((BGSEntryPointPerkEntry*)pduplicate)->m_pConditions + i)
                ->Copy(m_pConditions + i, 0);
        }
    }

    pduplicate->Copy(this);
    pduplicate;
    return pduplicate;
}

extern unsigned int _data_011c16bc;

// @fnv 0x005eabb0
// pdb: BGSPerk::~BGSPerk | BGSPerk.obj | retail, upstream name already matches, high
BGSPerk::~BGSPerk()
{
    reinterpret_cast<PerkOperations*>(this)->ClearConditions();
}

struct FUN_005a4910_Class
{
    unsigned int GetFieldD68(void) const;
};

extern FUN_005a4910_Class* _g_thePlayer;
void* __fastcall PlayerCharacter_GetTeammates(void*);

// @fnv 0x005eb6a0
void PerkOperations::UpdateEntries(void* data, unsigned char value1, unsigned char value2, bool flag)
{
    BSSimpleList<PatrolActorPackageData>* list = reinterpret_cast<BSSimpleList<PatrolActorPackageData>*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList);

    while (list)
    {
        PatrolActorPackageData* item = *reinterpret_cast<PatrolActorPackageData**>(&list->GetItem());

        if (item)
        {
            if (!flag)
            {
                if (item->IsPatrolling() + 1 == value1)
                {
                    reinterpret_cast<BGSPerkEntry*>(item)->RemoveEntryPointEffect(reinterpret_cast<Actor*>(data), flag);
                }
                else if (item->IsPatrolling() + 1 == value2)
                {
                    reinterpret_cast<BGSPerkEntry*>(item)->ApplyEntryPointEffect(reinterpret_cast<Actor*>(data), flag);
                }
            }
            else
            {
                int count = static_cast<int>(_g_thePlayer->GetFieldD68());

                if (count > 0)
                {
                    BSSimpleList<PatrolActorPackageData>* m_kList =
                        reinterpret_cast<BSSimpleList<PatrolActorPackageData>*>(PlayerCharacter_GetTeammates(_g_thePlayer));

                    while (m_kList)
                    {
                        PatrolActorPackageData* playerItem =
                            *reinterpret_cast<PatrolActorPackageData**>(&m_kList->GetItem());

                        if (reinterpret_cast<PerkEntryFunctionType*>(item)->GetFunctionType() == 2)
                        {
                            reinterpret_cast<BGSPerkEntry*>(item)->ApplyEntryPointEffect(reinterpret_cast<Actor*>(_g_thePlayer), flag);
                        }
                        else
                        {
                            reinterpret_cast<BGSPerkEntry*>(item)->ApplyEntryPointEffect(reinterpret_cast<Actor*>(playerItem), flag);
                        }

                        m_kList = m_kList->GetNext();
                    }
                }
            }
        }

        list = list->GetNext();
    }
}

namespace perk_removal {
struct FUN_005a4910_Class
{
    unsigned int GetFieldD68() const;
};

extern FUN_005a4910_Class* _g_thePlayer;
void* __fastcall PlayerCharacter_GetTeammates(void*);

struct PerkOperations
{
    void RemoveEntries(BGSPerk* perk, bool flag);
};

// @fnv 0x005eb800
// pdb: BGSPerk::RemovePerk | BGSPerk.obj | debug, object-file run extension, low
// pdb params: apActor, bAlt
// pdb locals: plist, pperkEntry, iCount, pList, pActor
void PerkOperations::RemoveEntries(BGSPerk* perk, bool flag)
{
    BSSimpleList<BGSPerk*>* perkList = reinterpret_cast<BSSimpleList<BGSPerk*>*>(reinterpret_cast<char*>(this) + 0x48);

    while (perkList)
    {
        BGSPerk* currentPerk = perkList->GetItem();

        if (currentPerk)
        {
            if (!flag)
            {
                reinterpret_cast<BGSPerkEntry*>(currentPerk)->RemoveEntryPointEffect(reinterpret_cast<Actor*>(perk), flag);
            }
            else
            {
                int playerLevel = static_cast<int>(_g_thePlayer->GetFieldD68());

                if (playerLevel > 0)
                {
                    BSSimpleList<BGSPerk*>* playerPerkList =
                        reinterpret_cast<BSSimpleList<BGSPerk*>*>(PlayerCharacter_GetTeammates(_g_thePlayer));

                    while (playerPerkList)
                    {
                        BGSPerk* playerPerk = playerPerkList->GetItem();

                        if (reinterpret_cast<PerkEntryFunctionType*>(currentPerk)->GetFunctionType() == 2)
                        {
                            reinterpret_cast<BGSPerkEntry*>(currentPerk)->RemoveEntryPointEffect(reinterpret_cast<Actor*>(_g_thePlayer), flag);
                        }
                        else
                        {
                            reinterpret_cast<BGSPerkEntry*>(currentPerk)->RemoveEntryPointEffect(reinterpret_cast<Actor*>(playerPerk), flag);
                        }

                        playerPerkList = playerPerkList->GetNext();
                    }
                }
            }
        }

        perkList = perkList->GetNext();
    }
}
}  // namespace perk_removal

struct ConditionParameters
{
    unsigned short function;
    unsigned short padding;
    int value;
    unsigned int upperBound;
};

// The shared TESConditionItem API lacks the out-of-line accessor at 0x00413f40,
// which returns the record starting at m_usFunctionCode. Keep only that operation local.
struct ConditionItemParameters
{
    ConditionParameters* GetParameters();
};

struct PerkConditionLookup
{
    TESConditionItem* __thiscall GetItemByIndex(int index);
};

// @fnv 0x005eba00
// src: COMMUNITY 0x005eba00
// pdb: BGSPerk::IsPerkAttainable | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apActionRef
// pdb locals: pcurrCondition, iconditionIndex, bresult, freturnVal
bool BGSPerk::IsPerkAttainable(PlayerCharacter* apActionRef)
{
    TESConditionItem* pcurrCondition;
    int iconditionIndex;
    bool bresult;

    bresult = true;
    iconditionIndex = 0;

    pcurrCondition = reinterpret_cast<PerkConditionLookup*>(reinterpret_cast<unsigned char*>(this) + 0x40)
                         ->GetItemByIndex(iconditionIndex++);

    while (pcurrCondition != 0)
    {
        if (reinterpret_cast<ConditionItemParameters*>(pcurrCondition)->GetParameters()->function == 0x46)
        {
            double freturnVal;

            freturnVal = 0.0;
            Script::IsSexConditionFunction(reinterpret_cast<TESObjectREFR*>(apActionRef), reinterpret_cast<void*>(reinterpret_cast<ConditionItemParameters*>(pcurrCondition)->GetParameters()->value), 0,
                                           freturnVal);
            bresult = (freturnVal == 1.0);
        }

        pcurrCondition = reinterpret_cast<PerkConditionLookup*>(reinterpret_cast<unsigned char*>(this) + 0x40)
                             ->GetItemByIndex(iconditionIndex++);
    }

    return bresult;
}

// @fnv 0x005e9590
// pdb: BGSPerkEntry::~BGSPerkEntry | BGSPerk.obj | debug, upstream name already matches, high
BGSPerkEntry::~BGSPerkEntry()
{
    FUN_00483710(this);
}

extern const char DAT_01183028[];
extern const char DAT_011861dc[];

void* __cdecl FUN_00ec43fb(int, unsigned int, const char*, const char*, unsigned int);
void __cdecl FUN_00401460(void*, void*, unsigned int);

struct FUN_005eb1b0_node
{
    void*& FUN_006815c0();
    FUN_005eb1b0_node* FUN_00726070();
    void FUN_00905820(void* const& item);
    void* m_item;
    FUN_005eb1b0_node* m_pkNext;
};

// @fnv 0x005eb1b0
// pdb: BGSPerk::Copy | BGSPerk.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: plist, pperk, pperkEntry, pperkEntryCopy
void PerkOperations::CopyFrom(int apCopy)
{
    BGSPerk* _copy;
    FUN_005eb1b0_node* node2;
    BGSPerkEntry* entry2;
    void* result;

    reinterpret_cast<BGSPerk*>(this)->ClearData();
    _copy = (BGSPerk*)FUN_00ec43fb(apCopy, 0, DAT_01183028, DAT_011861dc, 0);
    if (_copy == 0)
    {
        return;
    }
    FUN_00401460(&reinterpret_cast<BGSPerk*>(this)->Data, &_copy->Data, 5);
    CopyFormComponents(apCopy);
    reinterpret_cast<PerkConditionCopier*>(&reinterpret_cast<BGSPerk*>(this)->m_kConditions)->Copy(&_copy->m_kConditions, 0);
    node2 = reinterpret_cast<FUN_005eb1b0_node*>(&_copy->m_kEntryList);
    while (node2 != 0)
    {
        entry2 = (BGSPerkEntry*)node2->FUN_006815c0();
        if (entry2 != 0)
        {
            result = entry2->Duplicate();
            reinterpret_cast<FUN_005eb1b0_node*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList)->FUN_00905820(result);
        }
        node2 = node2->FUN_00726070();
    }
}

template <class T> class PerkList_t
{
  public:
    T m_item;
    PerkList_t<T>* m_pkNext;
    int GetSize();
    T& GetItem();
    PerkList_t<T>* GetNext();
};

// Compares this perk's playable, id, level, hidden, full name, conditions and entry list against another BGSPerk, returning true if any differ.
// @fnv 0x005eb280
// pdb: BGSPerk::Compare | BGSPerk.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pperk, plist1, plist2, pperkEntry1, pperkEntry2
bool PerkOperations::Compare(TESForm* apCompare)
{
    BGSPerk* pOtherPerk = dynamic_cast<BGSPerk*>(apCompare);
    if (!pOtherPerk)
        return true;
    if (reinterpret_cast<BGSPerk*>(this)->Data.bPlayable != pOtherPerk->Data.bPlayable || reinterpret_cast<BGSPerk*>(this)->Data.m_bTrait != pOtherPerk->Data.m_bTrait ||
        reinterpret_cast<BGSPerk*>(this)->Data.cLevel != pOtherPerk->Data.cLevel || reinterpret_cast<BGSPerk*>(this)->Data.m_ucRankCount != pOtherPerk->Data.m_ucRankCount ||
        reinterpret_cast<BGSPerk*>(this)->Data.bHidden != pOtherPerk->Data.bHidden)
        return true;
    if (CompareFormComponents(pOtherPerk))
        return true;
    if (reinterpret_cast<BGSPerk*>(this)->m_kConditions.Compare(&pOtherPerk->m_kConditions))
        return true;
    if (reinterpret_cast<PerkList_t<BGSPerkEntry*>*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList)->GetSize() !=
        reinterpret_cast<PerkList_t<BGSPerkEntry*>*>(&pOtherPerk->m_kEntryList)->GetSize())
        return true;
    PerkList_t<BGSPerkEntry*>* pEntryIter = reinterpret_cast<PerkList_t<BGSPerkEntry*>*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList);
    PerkList_t<BGSPerkEntry*>* pOtherEntryIter = reinterpret_cast<PerkList_t<BGSPerkEntry*>*>(&pOtherPerk->m_kEntryList);
    while (pEntryIter)
    {
        BGSPerkEntry* pEntry = pEntryIter->GetItem();
        BGSPerkEntry* pOtherEntry = pOtherEntryIter->GetItem();
        if ((pEntry && !pOtherEntry) || (!pEntry && pOtherEntry))
            return true;
        if (pEntry && pOtherEntry && pEntry->Compare(pOtherEntry))
            return true;
        pEntryIter = pEntryIter->GetNext();
        pOtherEntryIter = pOtherEntryIter->GetNext();
    }
    return false;
}

// Find the first listed entry whose level is one below the requested level.

class EB900Entry
{
  public:
    unsigned char GetLevel();
};

class EB900Node
{
  public:
    EB900Entry** Item();
    EB900Node* Next();

    EB900Entry* m_item;
    EB900Node* m_pkNext;
};

// Walks the m_kEntries list and returns the first entry whose GetLevel()+1 equals ucLevel, or null if none matches.
// @fnv 0x005eb900
// pdb: BGSPerk::GetPerkEntryByRank | BGSPerk.obj | debug, object-file run extension, low
// pdb params: acRank
// pdb locals: preturnEntry, pcurrEntryObj
EB900Entry* PerkOperations::FindEntryByLevel(unsigned char ucLevel)
{
    EB900Entry* pkFound = 0;
    for (EB900Node* pkIter = reinterpret_cast<EB900Node*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList); !pkFound && pkIter && *pkIter->Item(); pkIter = pkIter->Next())
    {
        if ((*pkIter->Item())->GetLevel() + 1 == ucLevel)
            pkFound = *pkIter->Item();
    }
    return pkFound;
}

// Returns true if other is not a BGSQuestPerkEntry, differs by base comparison, or has a different quest or stage; otherwise false.
// @fnv 0x005e99f0
// pdb: BGSQuestPerkEntry::Compare | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pquestPerkEntry
bool BGSQuestPerkEntry::Compare(BGSPerkEntry* apCompare)
{
    BGSQuestPerkEntry* other_quest = dynamic_cast<BGSQuestPerkEntry*>(apCompare);
    if (!other_quest)
    {
        return true;
    }
    if (BGSPerkEntry::Compare(apCompare))
    {
        return true;
    }
    if (Data.pQuest != other_quest->Data.pQuest)
    {
        return true;
    }
    if (Data.stage != other_quest->Data.stage)
    {
        return true;
    }
    return false;
}

// @fnv 0x005e9cc0
// pdb: BGSAbilityPerkEntry::Duplicate | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: pduplicate
BGSPerkEntry* BGSAbilityPerkEntry::Duplicate()
{
    BGSAbilityPerkEntry* pNew = new BGSAbilityPerkEntry();
    pNew->pAbility = pAbility;
    pNew->Copy(this);
    return pNew;
}

// Compares this entry point perk entry against another BGSEntryPointPerkEntry, returning true if any data, function or condition differs.
// @fnv 0x005ea560
// pdb: BGSEntryPointPerkEntry::Compare | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pentryPointPerkEntry, i
bool BGSEntryPointPerkEntry::Compare(BGSPerkEntry* apCompare)
{
    BGSEntryPointPerkEntry* pOtherEntry = dynamic_cast<BGSEntryPointPerkEntry*>(apCompare);
    if (!pOtherEntry)
        return true;
    if (BGSPerkEntry::Compare(apCompare))
        return true;
    if (Data.m_ucEntryPoint != pOtherEntry->Data.m_ucEntryPoint)
        return true;
    if (Data.m_ucFunction != pOtherEntry->Data.m_ucFunction)
        return true;
    if (Data.m_ucConditionCount != pOtherEntry->Data.m_ucConditionCount)
        return true;
    if ((m_pFunctionData && !pOtherEntry->m_pFunctionData) || (!m_pFunctionData && pOtherEntry->m_pFunctionData))
        return true;
    if (m_pFunctionData && pOtherEntry->m_pFunctionData &&
        m_pFunctionData->Compare(pOtherEntry->m_pFunctionData))
        return true;
    for (unsigned int i = 0; i < Data.m_ucConditionCount; i++)
    {
        if (m_pConditions[i].Compare(&pOtherEntry->m_pConditions[i]))
            return true;
    }
    return false;
}

void __fastcall FUN_00483710(void* apData);
void TESForm_AddChunk_3(unsigned int, unsigned char);

// @fnv 0x005ea6b0
// pdb: BGSEntryPointPerkEntry::Save | BGSPerk.obj | retail, upstream name already matches, high
// pdb locals: i
void BGSEntryPointPerkEntry::Save()
{
    reinterpret_cast<PerkEntrySerialization*>(this)->WriteHeader(2);
    if (TESForm_QEndianSwapOnSave())
    {
        FUN_00483710(&Data);
    }
    AppendChunk(0x41544144, &Data, 3u);
    if (TESForm_QEndianSwapOnSave())
    {
        FUN_00483710(&Data);
    }
    for (unsigned char i = 0; i < Data.m_ucConditionCount; ++i)
    {
        if (reinterpret_cast<PerkConditionLookup*>(&m_pConditions[i])->GetItemByIndex(0))
        {
            TESForm_AddChunk_3(0x434b5250, i);
            m_pConditions[i].Save();
        }
    }
    if (m_pFunctionData)
    {
        m_pFunctionData->Save();
    }
    reinterpret_cast<PerkEntrySerialization*>(this)->WriteEnd();
}

struct VirtualListItem
{
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual ~VirtualListItem();
};
struct ListCleanup
{
    void FUN_00470470();
};
struct ConditionCleanup
{
    void FUN_00680fe0();
};
// Deletes each item returned by the condition list cursor, then calls cleanup routines on the list and m_condition.
// @fnv 0x005eaca0
// pdb: BGSPerk::ClearData | BGSPerk.obj | retail, vtable slot, high
// pdb locals: plist
void PerkOperations::ClearConditions()
{
    const ConditionList_t* cursor = reinterpret_cast<const ConditionList_t*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList);
    while (cursor != 0)
    {
        delete reinterpret_cast<VirtualListItem*>(*cursor->FUN_006815c0());
        cursor = cursor->FUN_00726070();
    }
    reinterpret_cast<ListCleanup*>(&reinterpret_cast<BGSPerk*>(this)->m_kEntryList)->FUN_00470470();
    reinterpret_cast<ConditionCleanup*>(&reinterpret_cast<BGSPerk*>(this)->m_kConditions)->FUN_00680fe0();
}

extern "C" int __cdecl sprintf_s(char*, size_t, const char*, ...);
int __cdecl AppendPerkDescription(char*, unsigned int, const char*);

struct LocalizedText
{
    const char* FUN_00403df0();
};
extern LocalizedText _data_011d4678;
extern LocalizedText _data_011d3b2c;
extern LocalizedText _data_011d324c;
extern LocalizedText _data_011d40e4;
extern const char _data_01047040[];
extern const char _data_01047038[];
extern const char _data_01047034[];
extern const char _data_01047028[];
extern const char _data_0104701c[];
extern const char _data_01047014[];
extern const char _data_0104700c[];
extern const char _data_01047008[];
extern const char _data_01046ffc[];
extern const char _data_01020770[];
extern const char _data_01012630[];
extern const char _data_01012648[];
extern const char _data_01019f08[];

struct FormNameData
{
    const char* TESModel_GetModel();
};
const char* __cdecl ActorValue_GetActorValueAbbreviation(int);

// Builds the perk description string from level, condition items and description data into output, bounded by outputSize.
// @fnv 0x005ebac0
// pdb: BGSPerk::BuildDescriptionString | BGSPerk.obj | retail, unique string literal, high
// pdb params: apOutText, aiOutSize
// pdb locals: pcurrConditon, bHasRequirement, bIgnoreComma, strtemp, iConditionIndex, pfuncData, eindex, iValueMod, cSymbol, iRank, pPerk
void BGSPerk::GetDescription(char* apOutText, unsigned int aiOutSize)
{
    char message[128];
    {
        bool first = false;
        TESConditionItem* kItem = reinterpret_cast<PerkConditionLookup*>(&m_kConditions)->GetItemByIndex(0);
        if (Data.cLevel > 1)
        {
            sprintf_s(apOutText, aiOutSize, _data_01047040, _data_011d4678.FUN_00403df0(), _data_011d3b2c.FUN_00403df0(),
                      Data.cLevel);
        }
        else
        {
            sprintf_s(apOutText, aiOutSize, _data_01047038, _data_011d4678.FUN_00403df0());
            first = true;
        }
        bool foundVal = false;
        int index = 0;
        for (; kItem != 0; index++)
        {
            if (kItem->GetConditionFunction() == 0x1ef)
            {
                ConditionParameters* node = reinterpret_cast<ConditionItemParameters*>(kItem)->GetParameters();
                int form = node->value;
                const char* value = _data_01020770;
                int offset = 0;
                switch (kItem->GetComparisonCondition())
                {
                case 0:
                case 1:
                    break;
                case 2:
                    offset = -1;
                    break;
                case 3:
                    break;
                case 4:
                    value = _data_01047034;
                    break;
                case 5:
                    value = _data_01047034;
                    offset = 1;
                    break;
                }
                if (form != 0x17 && ActorValue_GetActorValueAbbreviation(form))
                {
                    if (static_cast<int>(kItem->GetComparisonValue()) + offset == 0)
                    {
                        kItem = reinterpret_cast<PerkConditionLookup*>(&m_kConditions)->GetItemByIndex(++index);
                        continue;
                    }
                    foundVal = true;
                    if (index == 0 && !first)
                    {
                        AppendPerkDescription(apOutText, aiOutSize, _data_01012630);
                    }
                    if (kItem->IsFlagSet() &&
                        reinterpret_cast<PerkConditionLookup*>(&m_kConditions)->GetItemByIndex(index + 1))
                    {
                        sprintf_s(message, 128, _data_01047028, ActorValue_GetActorValueAbbreviation(form), value,
                                  (static_cast<int>(kItem->GetComparisonValue()) + offset), _data_011d324c.FUN_00403df0());
                    }
                    else if (reinterpret_cast<PerkConditionLookup*>(&m_kConditions)->GetItemByIndex(index + 1))
                    {
                        sprintf_s(message, 128, _data_0104701c, ActorValue_GetActorValueAbbreviation(form), value,
                                  (static_cast<int>(kItem->GetComparisonValue()) + offset));
                    }
                    else
                    {
                        sprintf_s(message, 128, _data_01047014, ActorValue_GetActorValueAbbreviation(form), value,
                                  (static_cast<int>(kItem->GetComparisonValue()) + offset));
                    }
                    AppendPerkDescription(apOutText, aiOutSize, message);
                }
            }
            else if (kItem->GetConditionFunction() == 0x1c1)
            {
                foundVal = true;
                if (index == 0 && !first)
                {
                    AppendPerkDescription(apOutText, aiOutSize, _data_01012630);
                }
                ConditionParameters* dataTmp = reinterpret_cast<ConditionItemParameters*>(kItem)->GetParameters();
                int formX = dataTmp->value;
                unsigned int countB = dataTmp->upperBound;
                if (formX && countB > 0)
                {
                    sprintf_s(message, 128, _data_0104700c,
                              reinterpret_cast<FormNameData*>((char*)formX + 0x18)->TESModel_GetModel(), countB);
                }
                else if (formX)
                {
                    sprintf_s(message, 128, _data_01019f08,
                              reinterpret_cast<FormNameData*>((char*)formX + 0x18)->TESModel_GetModel());
                }
                AppendPerkDescription(apOutText, aiOutSize, message);
            }
            else
            {
                foundVal = true;
                if (index == 0 && !first)
                {
                    AppendPerkDescription(apOutText, aiOutSize, _data_01012630);
                }
            }
            kItem = reinterpret_cast<PerkConditionLookup*>(&m_kConditions)->GetItemByIndex(++index);
        }
        if (!foundVal)
            AppendPerkDescription(apOutText, aiOutSize, _data_01047008);
        AppendPerkDescription(apOutText, aiOutSize, _data_01012648);
        sprintf_s(message, 128, _data_01046ffc, _data_011d40e4.FUN_00403df0(), Data.m_ucRankCount);
        AppendPerkDescription(apOutText, aiOutSize, message);
        AppendPerkDescription(apOutText, aiOutSize,
                              reinterpret_cast<TESDescription*>(reinterpret_cast<char*>(this) + 0x24)
                                  ->GetDescription(0, static_cast<CHUNK_ID>(0x43534544)));
    }
}

// Returns true if apCompare is not a BGSAbilityPerkEntry, the base comparison returns true, or the m_pSpell fields differ.
// @fnv 0x005e9d60
// pdb: BGSAbilityPerkEntry::Compare | BGSPerk.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pabilityPerkEntry
bool BGSAbilityPerkEntry::Compare(BGSPerkEntry* apCompare)
{
    BGSAbilityPerkEntry* pAbilityPerkEntry = dynamic_cast<BGSAbilityPerkEntry*>(apCompare);
    if (!pAbilityPerkEntry)
        return true;
    if (BGSPerkEntry::Compare(apCompare))
        return true;
    if (pAbility != pAbilityPerkEntry->pAbility)
        return true;
    return false;
}
