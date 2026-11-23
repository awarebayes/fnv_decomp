// fallout_shared/bgshunger.cpp: hunger and dehydration stages (11 functions)

#include "layout_support.h"
#include "../../include/BGSDehydrationStage.h"
#include "../../include/BGSHungerStage.h"
#include "../../include/HungerStageData.h"
#include "../../include/SpellItem.h"
#include <malloc.h>

void BGSHungerStage_AddStage(BGSHungerStage* apNewStage);

// @fnv 0x00401b40
// pdb: BGSHungerStage::ClearData | BGSHunger.obj | debug, upstream name already matches, high
void BGSHungerStage::ClearData()
{
    ClearStageData(this);
}

extern "C" unsigned int gHungerStageCount;
extern "C" BGSHungerStage** gHungerStages;

struct RemoveStageLocals
{
    BGSHungerStage** oldStages;  // ebp - 0x1c
    unsigned int source;         // ebp - 0x18
    unsigned int destination;    // ebp - 0x14
    unsigned int index;          // ebp - 0x10
    unsigned char padding0;
    unsigned char padding1;
    unsigned char padding2;
    bool found;                       // ebp - 0x09
    unsigned int newCount;            // ebp - 0x08
    BGSDehydrationStage** newStages;  // ebp - 0x04
};

// @fnv 0x00401f20
// pdb: BGSHungerStage::RemoveStage | BGSHunger.obj | retail, object-file run extension, low
// pdb params: apNewStage
// pdb locals: binArray, inewStagesCount, pnewStages, i, icurrentStage
void BGSHungerStage::ClearStageData(BGSHungerStage* stage)
{
    RemoveStageLocals locals;

    if (!gHungerStageCount)
        return;

    locals.found = false;
    locals.index = 0;
    for (; !locals.found && locals.index < gHungerStageCount; ++locals.index)
    {
        if (gHungerStages[locals.index] == stage)
            locals.found = true;
    }

    if (!locals.found)
        return;

    locals.newCount = gHungerStageCount - 1;
    locals.newStages = BGSDehydrationStage::AllocateStageArray(locals.newCount);
    if (locals.newStages)
    {
        locals.destination = 0;
        locals.source = 0;
        for (; locals.source < gHungerStageCount; ++locals.source)
        {
            if (gHungerStages[locals.source] != stage)
            {
                ((BGSHungerStage**)locals.newStages)[locals.destination] = gHungerStages[locals.source];
                ++locals.destination;
            }
        }
    }

    locals.oldStages = gHungerStages;
    ::operator delete(locals.oldStages);
    gHungerStageCount = locals.newCount;
    gHungerStages = (BGSHungerStage**)locals.newStages;
}

// Reads a hunger stage record from the TESFile: validates form type 0x77, loads the record header, then processes EDID, OBND and DATA chunks.
// @fnv 0x00401cd0
// pdb: BGSHungerStage::Load | BGSHunger.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: echunkid, formid, pMessage
bool BGSHungerStage::Load(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x77)
        return false;

    static_cast<TESForm*>(this)->ClearData();
    TESForm::LoadRecordHeader(apFile);
    TESForm::SetInitializationState(false);

    unsigned int chunk;
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch (chunk)
        {
        case 0x44494445: {
            char* buffer = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(buffer, 0x200);
            static_cast<TESForm*>(this)->SetEditorID_AtRuntime(buffer);
            break;
        }
        case 0x444e424f:
            static_cast<TESForm*>(this)->readOBNDSubRecord(apFile);
            break;
        case 0x41544144:
            reinterpret_cast<BGSDehydrationStage*>(this)->TESForm_LoadData(
                apFile, reinterpret_cast<BGSDehydrationStage*>(this)->FUN_00500940(8));
            break;
        }

        if (chunk == 0x41544144 && apFile->IsEndianConversionRequired())
            reinterpret_cast<DehydrationStageData*>(&data)->TwoValues_Endian();
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x00401b60
// pdb: BGSHungerStage::InitItem | BGSHunger.obj | retail, upstream name already matches, high
// pdb locals: itempInt
void BGSHungerStage::InitItem()
{
    if (TESForm::IsFormInitialized())
        return;

    unsigned int formID = reinterpret_cast<BGSDehydrationStage*>(this)->FUN_00441110();
    if (formID)
    {
        TESForm::RemapFormIDForFile(formID, TESForm::GetFile(-1));
        reinterpret_cast<BGSDehydrationStage*>(this)->FUN_0050f9a0(
            dynamic_cast<SpellItem*>(TESForm::GetFormByID(formID)));
    }

    TESForm::SetInitializationState(true);
    BGSHungerStage_AddStage(this);
}

struct AddStageLocals
{
    BGSHungerStage** oldStages;
    unsigned int index;
    unsigned char padding00;
    unsigned char padding01;
    unsigned char padding02;
    unsigned char newCount;
    unsigned int destination;
    unsigned char padding10;
    unsigned char padding11;
    unsigned char padding12;
    bool inserted;
    BGSHungerStage** newStages;
};

// @fnv 0x00401e20
// src: XBOX_RETAIL_PDB 0x00401e20
// pdb: BGSHungerStage::AddStage | BGSHunger.obj | retail, call graph, high
// pdb params: apNewStage
// pdb locals: cnewStagesCount, inewStageCounter, binserted, pnewStages, i
void BGSHungerStage_AddStage(BGSHungerStage* apNewStage)
{
    AddStageLocals locals;
    locals.newCount = (unsigned char)(gHungerStageCount + 1);
    locals.newStages = (BGSHungerStage**)BGSDehydrationStage::AllocateStageArray(locals.newCount);
    locals.destination = 0;
    locals.inserted = false;
    locals.index = 0;
    for (; locals.index < gHungerStageCount; ++locals.index)
    {
        if (apNewStage->FUN_00500940()->m_uiStageLevel > gHungerStages[locals.index]->FUN_00500940()->m_uiStageLevel &&
            !locals.inserted)
        {
            locals.newStages[locals.destination] = apNewStage;
            ++locals.destination;
            locals.inserted = true;
        }
        locals.newStages[locals.destination] = gHungerStages[locals.index];
        ++locals.destination;
    }
    if (!locals.inserted)
    {
        locals.newStages[locals.destination] = apNewStage;
        ++locals.destination;
    }
    locals.oldStages = gHungerStages;
    ::operator delete(locals.oldStages);
    gHungerStageCount = locals.newCount;
    gHungerStages = locals.newStages;
}

// pdb: class Calendar, size 0x1c (here 0x1), 7 members
class Calendar
{
  public:
    unsigned int GetDaysPassed();
};
BGSHungerStage* __cdecl GetHungerStage(unsigned int index);

// Returns the first hunger stage whose GetDaysPassed threshold is at or below level, or null if none match.
// @fnv 0x00402030
BGSHungerStage* __cdecl FindHungerStage(float level)
{
    BGSHungerStage* result = 0;
    for (unsigned int index = 0; index < gHungerStageCount && !result; ++index)
    {
        BGSHungerStage* stage = GetHungerStage(index);
        if (stage && level >= reinterpret_cast<Calendar*>(stage)->GetDaysPassed())
            result = stage;
    }
    return result;
}

// @fnv 0x00843170
void HungerStageData::Reinitialize()
{
    m_uiStageLevel = 0;
    m_pSpell = 0;
}

void __cdecl _Deallocate(void*);
// Calls FUN_00401ac0, then deallocates this when flags has bit 0 set, and returns this.
// @fnv 0x00401a90
// pdb: BGSHungerStage::`scalar deleting destructor' | BGSHunger.obj | retail, vtable slot, high
void* BGSHungerStage::FUN_00401a90(unsigned int flags)
{
    FUN_00401ac0();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

void* __cdecl memcpy(void* dest, const void* src, unsigned int size);
// @fnv 0x00401bf0
// pdb: BGSHungerStage::Copy | BGSHunger.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pcopy
void __thiscall BGSHungerStage::CopyFromForm(TESForm* apCopy)
{
    BGSHungerStage* copy = dynamic_cast<BGSHungerStage*>(apCopy);
    if (!copy)
        return;
    TESForm::CopyFrom(copy);
    memcpy(FUN_00500940(), copy->FUN_00500940(), 8);
}

bool _FUN_00401500();

// @fnv 0x00591e10
// pdb: BGSRadiationStage::Save | BGSRadiation.obj | retail, vtable slot, high
// pdb locals: psaveSickness
void BGSDehydrationStage::InitDehydrationStage()
{
    TESForm::BeginFormRecord();

    SpellItem* pSavedSpell = reinterpret_cast<SpellItem*>(FUN_00441110());

    FUN_0050f9a0(FUN_00441110() ? reinterpret_cast<SpellItem*>(reinterpret_cast<TESForm*>(FUN_00441110())->GetFormID())
                                : 0);

    if (_FUN_00401500())
    {
        data.TwoValues_Endian();
    }

    TESForm::TESForm___SaveData(&data, 8);

    if (_FUN_00401500())
    {
        data.TwoValues_Endian();
    }

    FUN_0050f9a0(pSavedSpell);

    TESForm::FinishFormRecord();
}

extern char DAT_0118307c;
extern char DAT_01183028;

void* __cdecl FUN_00ec43fb(void* a, unsigned int b, void* c, void* d, unsigned int e);
int __cdecl FUN_00ec4835(void* a, void* b, unsigned int c);

// @fnv 0x00401c50
// pdb: BGSHungerStage::Compare | BGSHunger.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pcompare
bool BGSHungerStage::Compare(TESForm* apCompare)
{
    BGSHungerStage* r = (BGSHungerStage*)FUN_00ec43fb(apCompare, 0, &DAT_0118307c, &DAT_01183028, 0);
    if (r == 0)
    {
        return true;
    }
    if (TESForm::Compare(r))
    {
        return true;
    }
    if (FUN_00ec4835(this->FUN_00500940(), r->FUN_00500940(), 8) != 0)
    {
        return true;
    }
    return false;
}
