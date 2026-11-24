// fallout_shared/tesattributes.cpp: TESAttributes (11 functions)

#include "../../include/BaseFormComponent.h"
#include "../../include/TESAttributes.h"

// @fnv 0x0047ff70
// pdb: TESAttributes::TESAttributes | TESAttributes.obj | debug, upstream name already matches, high
TESAttributes::TESAttributes()
{
    InitializeComponent();
}

// Sets all seven entries of cAttribute to 5.
// @fnv 0x0047ffc0
void TESAttributes::InitializeComponent()
{
    for (int i = 0; i < 7; ++i)
    {
        cAttribute[i] = 5;
    }
}

void* __cdecl memcpy(void*, const void*, size_t);
unsigned int __fastcall FUN_009d2510(void*);

// Copies 7 bytes from apData into the object at offset 4, then calls FUN_009d2510 on this and returns its result.
// @fnv 0x00480130
// pdb: TESAttributes::Load | TESAttributes.obj | debug, upstream name already matches, high
// pdb params: apData
unsigned int TESAttributes::Load(void* apData)
{
    memcpy(reinterpret_cast<char*>(this) + 4, apData, 7);
    return FUN_009d2510(this);
}

// @fnv 0x00480160
unsigned char TESAttributes::GetAttributeValue(int aeAttribute)
{
    return reinterpret_cast<unsigned char*>(this)[aeAttribute - 1];
}

extern "C" void* _memcpy(void* dst, const void* src, unsigned int n);
unsigned int __fastcall FUN_009d2510(void* p);

// Copies 7 bytes from this+4 into apData, then returns the result of FUN_009d2510(this).
// @fnv 0x00480100
// pdb: TESAttributes::Save | TESAttributes.obj | debug, upstream name already matches, high
// pdb params: apData
unsigned int __thiscall TESAttributes::Save(void* apData)
{
    _memcpy(apData, reinterpret_cast<char*>(this) + 4, 7);
    return FUN_009d2510(this);
}

// @fnv 0x004801e0
unsigned short TESAttributes::GetSaveSize(unsigned int param)
{
    unsigned short result = 0;
    if (param & 0)
    {
        result = result + 7;
    }
    return result;
}

#include "../../include/TESSaveLoadGameGlobals.h"


// @fnv 0x00480240
// pdb: TESAttributes::LoadGame | TESAttributes.obj | debug, call graph, high
// pdb params: aiFlags, aiCurrentFlags
void __thiscall TESAttributes::LoadGame(unsigned int aiFlags, unsigned int aiCurrentFlags)
{
    if (aiFlags & 0)
    {
        g_TESSaveLoadGame->ReadTypedData((void*)((char*)this + 4), 7);
    }
}

#include "../../include/ActorValue.h"

struct AVH
{
    static ActorValue::Index GetActorValueForSectionIndex(ActorValue::Section, unsigned char);
};

// Casts component to TESAttributes and copies its seven attribute values into cAttribute.
// @fnv 0x00480000
// pdb: TESAttributes::CopyComponent | TESAttributes.obj | retail, vtable slot, high
// pdb params: apCopyFC
// pdb locals: pCopy, n
void TESAttributes::BaseFormRecord(BaseFormComponent* apCopyFC)
{
    TESAttributes* attributes = dynamic_cast<TESAttributes*>(apCopyFC);
    if (!attributes)
        return;
    for (int i = 0; i < 7; ++i)
        cAttribute[i] = attributes->GetAttributeValue((int)AVH::GetActorValueForSectionIndex(ActorValue::ATTRIBUTE, i));
}

namespace fnv_00480070 {
struct AVH
{
    static ActorValue::Index GetActorValueForSectionIndex(int, unsigned char);
};
}  // namespace fnv_00480070
// Casts component to TESAttributes and returns false only if all seven attribute values match this object's cAttribute entries; returns true otherwise.
// @fnv 0x00480070
// pdb: TESAttributes::CompareComponent | TESAttributes.obj | retail, vtable slot, high
// pdb params: apCompareFC
// pdb locals: pCompare, n
bool TESAttributes::IsEqualToBase(BaseFormComponent* apCompareFC)
{
    TESAttributes* attributes = dynamic_cast<TESAttributes*>(apCompareFC);
    if (!attributes)
        return true;
    for (int i = 0; i < 7; ++i)
    {
        if (cAttribute[i] !=
            attributes->GetAttributeValue(fnv_00480070::AVH::GetActorValueForSectionIndex(0, i)))
            return true;
    }
    return false;
}
// @fnv 0x00480210
// pdb: TESAttributes::SaveGame | TESAttributes.obj | debug, call graph, high
// pdb params: aiFlags
void TESAttributes::SaveGame(unsigned int aiFlags)
{
    if (aiFlags & 0)
    {
        g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD((void*)((char*)this + 4), 7);
    }
}

extern void* PTR_PTR_01186c74;
extern void* PTR_PTR_01183028;
extern "C" void* FUN_00ec43fb(void* self, int a, void* b, void* c, int d);
// Writes value at byte offset offset - 1, then when flag is nonzero calls FUN_00ec43fb and a method on its result.
// @fnv 0x00480180
// pdb: TESAttributes::SetAttributeValue | TESAttributes.obj | retail, upstream name already matches, high
// pdb params: aeAttribute, acValue, abMarkChange
// pdb locals: pform
void TESAttributes::SetAttributeValue(int aeAttribute, unsigned char acValue, bool abMarkChange)
{
    ((char*)this)[aeAttribute - 1] = acValue;
    if (abMarkChange != 0)
    {
        void* p = FUN_00ec43fb(this, 0, &PTR_PTR_01186c74, &PTR_PTR_01183028, 0);
        if (p != 0)
        {
            (*(void(__thiscall**)(void*, int))(*(char**)p + 0x48))(p, 4);
        }
    }
}
