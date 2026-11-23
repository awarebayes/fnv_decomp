#include "../include/SettingT.h"
// bgsencounterzone.cpp: BGSEncounterZone and TESObjectREFR (24 functions)

#include <stddef.h>
#include "../include/SETTING_VALUE.h"
#define FNV_JOIN_IMPL(a, b) a##b
#define FNV_JOIN(a, b) FNV_JOIN_IMPL(a, b)
#define FNV_STATIC_ASSERT(condition) typedef char FNV_JOIN(fnv_static_assert_, __LINE__)[1]
#include "../include/BGSEncounterZone.h"
#include "../include/CellDetachData.h"
#include "../include/EncounterZoneData.h"
#include "../include/MapEntryCount.h"
#include "../include/TESDataHandler.h"
#include "../include/TESFile.h"
#include "../include/TESForm.h"
#include "../include/TESObjectCELL.h"
#include "../include/BGSSaveFormBuffer.h"
#include "../include/BGSLoadFormBuffer.h"
#include "../include/PlayerCharacter.h"
#include "../include/TESObjectREFR.h"
#include "../include/FormChangeMask.h"
#include "../include/TESObject.h"
#include "../include/TESWorldSpace.h"

extern "C" void* __cdecl memset(void*, int, unsigned int);

// Sets the form type to 0x61 and then runs the constructor's initialization routine.
// @fnv 0x00525c40
// pdb: BGSEncounterZone::BGSEncounterZone | BGSEncounterZone.obj | retail, upstream name already matches, high
BGSEncounterZone::BGSEncounterZone()
{
    SetFormType((ENUM_FORM_ID)0x61);
    ResetFormData();
}

// Clears the Data and GameData members by zeroing their memory.
// @fnv 0x00525d00
// pdb: BGSEncounterZone::InitializeData | BGSEncounterZone.obj | retail, vtable slot, high
void BGSEncounterZone::ResetFormData()
{
    memset(&Data, 0, sizeof(Data));
    memset(&GameData, 0, sizeof(GameData));
}

#include <malloc.h>
#include "../include/ExtraDataList.h"
void* __cdecl operator new(size_t);
extern TESDataHandler* g_dataHandler;
int __cdecl Warning(const char*, ...);

// Creates the NoZoneZone BGSEncounterZone once, sets its ref ID and editor ID, and adds it to the data handler.
// @fnv 0x00526340
// src: XBOX_RETAIL_PDB 0x00526340
// pdb: BGSEncounterZone::CreateNoZone | BGSEncounterZone.obj | retail, string-set bridge, high
void BGSEncounterZone::CreateNoZone()
{
    if (!ms_pDefaultEncounterZone)
    {
        ms_pDefaultEncounterZone = new BGSEncounterZone;
        ms_pDefaultEncounterZone->SetRefID(0x1e, true);
        ms_pDefaultEncounterZone->SetEditorID_AtRuntime("NoZoneZone");
        g_dataHandler->AddFormToDataHandler(ms_pDefaultEncounterZone);
    }
}

// Reads an encounter zone record from file: checks form type 0x61, loads the header, then handles EDID, OBND and DATA chunks.
// @fnv 0x00525d40
// pdb: BGSEncounterZone::Load | BGSEncounterZone.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, formid, pMessage
bool BGSEncounterZone::Load(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x61)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    unsigned int chunk;
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch (chunk)
        {
        case 0x44494445: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x41544144:
            apFile->GetChunkData(&Data, sizeof(Data));
            if (apFile->GetChunkData())
                Data.Endian();
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// Resolves the encounter zone's owner form ID to a TESForm pointer, warns if it is missing, then marks the zone initialized.
// @fnv 0x00525f00
// pdb: BGSEncounterZone::InitItem | BGSEncounterZone.obj | retail, string-set bridge, high
// pdb locals: itempint, pform
void BGSEncounterZone::InitItem()
{
    if (static_cast<const TESForm*>(this)->GetInitialized())
        return;
    if (Data.m_pOwnerForm)
    {
        struct OwnerResolution
        {
            unsigned int formID;
            TESForm* owner;
        } ownerResolution;
        ownerResolution.formID = (unsigned int)Data.m_pOwnerForm;
        RemapFormIDForFile(ownerResolution.formID, GetFile(-1));
        ownerResolution.owner = GetFormByID(ownerResolution.formID);
        if (!ownerResolution.owner)
            Warning("Unable to find owner form (%08X) on encounter zone '%s' (%08X).", ownerResolution.formID,
                    GetName(), GetFormID());
        Data.m_pOwnerForm = ownerResolution.owner;
    }
    SetInitializationState(true);
}

// Returns the encounter zone from the extra data list, falling back to the parent cell's and then the worldspace's encounter zone.
// @fnv 0x00567d20
// pdb: TESObjectREFR::GetEncounterZone | TESObjectREFR.obj | retail, call graph (shared callers), high
// pdb locals: pretzone
BGSEncounterZone* TESObjectREFR::FindEncounterZone()
{
    BGSEncounterZone* zone = GetExtraData()->FindEncounterZone();
    if (!zone)
    {
        if (GetParentCell())
            zone = GetParentCell()->FindEncounterZone();
        if (!zone)
        {
            if (GetParentWorldSpace())
                zone = GetParentWorldSpace()->FindEncounterZone();
        }
    }
    return zone;
}

extern PlayerCharacter* g_thePlayer;
class GameSettingCollection;
using ::SettingT;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting36;

// @fnv 0x00526190
// pdb: BGSEncounterZone::GetCalcedZoneLevel | BGSEncounterZone.obj | debug, call graph, medium
// pdb locals: slevel
unsigned short BGSEncounterZone::GetCalculatedLevel()
{
    unsigned short level = g_thePlayer->GetLevel();
    if (level < GetByteAt1D())
    {
        if (!QMatchPCBelowMin())
            level = GetByteAt1D();
    }
    else
    {
        level = (unsigned short)(GetByteAt1D() + (level - GetByteAt1D()) * originalRuntimeFloatSetting36.Float()[0]);
    }
    return level;
}

// @fnv 0x00526250
bool BGSEncounterZone::QMatchPCBelowMin()
{
    return (bool)(Data.cFlags & 2);
}

// The compiler-generated scalar deleting destructor supplies 0x00525cb0.
// @fnv 0x00525ce0
// pdb: BGSEncounterZone::~BGSEncounterZone | BGSEncounterZone.obj | debug, upstream name already matches, high
BGSEncounterZone::~BGSEncounterZone()
{}

bool __cdecl TESForm_QEndianSwapOnSave();
void __cdecl AppendChunk(unsigned int, void*, unsigned int);
// @fnv 0x00525e70
// pdb: BGSEncounterZone::Save | BGSEncounterZone.obj | retail, upstream name already matches, high
// pdb locals: powner
void BGSEncounterZone::Save()
{
    TESForm* powner;
    TESForm::BeginFormRecord();
    powner = Data.m_pOwnerForm;
    if (powner)
    {
        Data.m_pOwnerForm = reinterpret_cast<TESForm*>(powner->GetFormID());
    }
    if (TESForm_QEndianSwapOnSave())
        Data.Endian();
    AppendChunk(0x41544144, &Data, 8);
    if (TESForm_QEndianSwapOnSave())
        Data.Endian();
    Data.m_pOwnerForm = powner;
    TESForm::FinishFormRecord();
}

// @fnv 0x00526070
// src: COMMUNITY 0x00526070
// pdb: BGSEncounterZone::SetDetachTime | BGSEncounterZone.obj | debug, call graph, high
// pdb params: aiVal
void BGSEncounterZone::SetDetachTime_00526070(unsigned int aiVal)
{
    GameData.m_uiDetachTime = aiVal;
    MarkAsModified(0x80000000);
}

unsigned int __cdecl TESObjectCELL_GetHoursToRespawnCell();
// @fnv 0x005260a0
// src: COMMUNITY 0x005260a0
// pdb: BGSEncounterZone::SetAttachTime | BGSEncounterZone.obj | debug, upstream name already matches, high
// pdb params: aiVal
void BGSEncounterZone::SetAttachTime(unsigned int aiVal)
{
    GameData.m_uiFirstValueAfterDetachTime = aiVal;
    if (GameData.m_uiDetachTime != 0)
    {
        if (GameData.m_uiFirstValueAfterDetachTime - GameData.m_uiDetachTime > TESObjectCELL_GetHoursToRespawnCell())
            GameData.DetachTimeRecord = aiVal;
    }
    MarkAsModified(0x80000000);
}

namespace fnv_00526100 {
struct GlobalObject2_0040c150
{
    int* Int();
};

GlobalObject2_0040c150 g_011ca160;

// @fnv 0x00526100
// src: XBOX_PDB 0x00526100
// pdb: TESObjectCELL::GetHoursToRespawnCell | BGSEncounterZone.obj | debug, upstream name already matches, high
int __cdecl TESObjectCELL_GetHoursToRespawnCell()
{
    return *g_011ca160.Int();
}
}  // namespace fnv_00526100

// Returns false if the value at offset 0x28 is zero, otherwise returns whether other's FUN_00546af0() result is below that value.
// @fnv 0x00526120
// src: COMMUNITY 0x00526120
// pdb: BGSEncounterZone::ShouldCellRecalc | BGSEncounterZone.obj | retail, upstream name already matches, high
// pdb params: apCell
// pdb locals: icelldetach
bool BGSEncounterZone::ShouldCellRecalc(TESObjectCELL* apCell)
{
    if (GameData.DetachTimeRecord == 0)
        return false;
    unsigned int v = apCell->GetDetachTime();
    if (v < GameData.DetachTimeRecord)
        return true;
    return false;
}

// @fnv 0x00526160
// src: XBOX_PDB 0x00526160
// pdb: BGSEncounterZone::SetZoneLevel | BGSEncounterZone.obj | debug, upstream name already matches, high
// pdb params: aiVal
void BGSEncounterZone::SetZoneLevel(unsigned short aiVal)
{
    GameData.m_usDetachRecordValue = aiVal;
    MarkAsModified(0x80000000);
}

// @fnv 0x00526270
// src: XBOX_PDB 0x00526270
// pdb: BGSEncounterZone::DetermineLevel | BGSEncounterZone.obj | retail, upstream name already matches, high
void BGSEncounterZone::DetermineLevel()
{
    if (GameData.m_usDetachRecordValue == 0)
        SetZoneLevel(GetCalculatedLevel());
}

// pdb: class Calendar, size 0x1c (here 0x1), 7 members
struct Calendar
{
    unsigned int GetGameHoursPassed();
};
extern Calendar g_calendar;
unsigned int __cdecl TESObjectCELL_GetHoursToRespawnCell();
// @fnv 0x005262a0
// src: XBOX_PDB 0x005262a0
// pdb: BGSEncounterZone::ShouldReset | BGSEncounterZone.obj | retail, upstream name already matches, high
// pdb locals: breset, idetachtime, itime
bool __fastcall BGSEncounterZone_ShouldReset(BGSEncounterZone* apZone)
{
    bool result = true;
    if (apZone != BGSEncounterZone::ms_pDefaultEncounterZone && apZone->GameData.m_usDetachRecordValue)
    {
        if (apZone->QNeverReset())
        {
            result = false;
        }
        else
        {
            unsigned int next = reinterpret_cast<unsigned int>(reinterpret_cast<TESObject*>(apZone)->GetNext());
            if (next)
            {
                unsigned int now = g_calendar.GetGameHoursPassed();
                if (now - next <= TESObjectCELL_GetHoursToRespawnCell())
                    result = false;
            }
            else
            {
                result = false;
            }
        }
    }
    return result;
}

// @fnv 0x00526320
// src: XBOX_PDB 0x00526320
bool BGSEncounterZone::QNeverReset()
{
    return (Data.cFlags & 1) ? true : false;
}

// @fnv 0x00526410
// pdb: BGSEncounterZone::SaveGame | BGSEncounterZone.obj | retail, upstream name already matches, high
// pdb params: apSaveGameBuffer
// pdb locals: ctempFlags
void BGSEncounterZone::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    unsigned char ctempFlags;
    TESForm::SaveGame(apSaveGameBuffer);
    if (apSaveGameBuffer->GetChangeFlags(2).operator bool())
    {
        ctempFlags = Data.cFlags & 1;
        apSaveGameBuffer->FUN_00865e50(&ctempFlags, 1, 0);
    }
    if (apSaveGameBuffer->GetChangeFlags(0x80000000).operator bool())
    {
        apSaveGameBuffer->FUN_00865e50(&GameData, 0x10, 0);
    }
}

// @fnv 0x00526570
// pdb: BGSEncounterZone::Revert | BGSEncounterZone.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
void BGSEncounterZone::RevertAndClearIfChanged(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::Revert(reinterpret_cast<unsigned int>(apLoadGameBuffer));
    if (apLoadGameBuffer->GetOldChangeFlags(0x80000000).operator bool())
        memset(&GameData, 0, 0x10);
}

// Loads the encounter zone from the buffer, reading a flag byte and, for version 8+, a 16-byte field at offset 0x20.
// @fnv 0x005264a0
// src: XBOX_PDB 0x005264a0
// pdb: BGSEncounterZone::LoadGame | BGSEncounterZone.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
// pdb locals: ctempFlags, ptempData
void BGSEncounterZone::LoadGame_005264a0(BGSLoadFormBuffer* apLoadGameBuffer)
{
    unsigned char ctempFlags;

    TESForm::LoadGame(apLoadGameBuffer);

    if (apLoadGameBuffer->GetChangeFlags(2u).operator bool())
    {
        ctempFlags = 0;
        apLoadGameBuffer->ReadTypedData(&ctempFlags, 1u);
        Data.cFlags = (Data.cFlags & ~1) | (Data.cFlags & 1);
    }

    if (apLoadGameBuffer->GetChangeFlags(0x80000000).operator bool())
    {
        if (apLoadGameBuffer->GetVersion() >= 8)
        {
            apLoadGameBuffer->ReadTypedData(&GameData, 0x10u);
        }
        else
        {
            char acTempData[8];
            apLoadGameBuffer->ReadTypedData(acTempData, 8u);
        }
    }
}

extern "C" int __cdecl memcmp(const void*, const void*, unsigned int);
// Returns true when other is not a BGSEncounterZone, the base comparison returns true, or the Data bytes differ.
// @fnv 0x00526000
// src: XBOX_PDB 0x00526000
// pdb: BGSEncounterZone::Compare | BGSEncounterZone.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp
bool BGSEncounterZone::Compare_00526000(TESForm* apCompare)
{
    BGSEncounterZone* zone = dynamic_cast<BGSEncounterZone*>(apCompare);
    if (!zone)
        return true;
    if (TESForm::Compare(zone))
        return true;
    if (memcmp(&Data, &zone->Data, sizeof(Data)))
        return true;
    return false;
}

void* __cdecl _memcpy(void*, const void*, unsigned int);

// Copies the base form and encounter-zone data from another BGSEncounterZone.
// @fnv 0x00525fa0
// src: XBOX_PDB 0x00525fa0
// pdb: BGSEncounterZone::Copy | BGSEncounterZone.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pcopy
void BGSEncounterZone::Copy_00525fa0(TESForm* apCopy)
{
    BGSEncounterZone* other = dynamic_cast<BGSEncounterZone*>(apCopy);
    if (!other)
        return;
    TESForm::CopyFrom(other);
    _memcpy(&Data, &other->Data, sizeof(Data));
}
