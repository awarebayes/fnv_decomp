// fallout_shared/tesclass.cpp: TESClass and class-data operations (15 functions)

#define FNV_TESFILE_H  // the shared TESFile below replaces layout_support.h's stub
#include "../../include/NiFile.h"
#include "layout_support.h"
#include "../../include/BGSLoadFormBuffer.h"
#include "../../include/BGSSaveFormBuffer.h"
#include "../../include/BGSImpactData.h"
#include "../../include/TESAttributes.h"
#include "../../include/TESClass.h"
#include "../../include/TESClassData.h"
#include "../../include/TESDescription.h"
#include "../../include/TESFullName.h"
#include "../../include/TESTexture.h"
#include "TESFile.h"
#include "../../include/TESFile.h"
#include "../../include/ActorValue.h"
namespace ActorValue {
bool IsSkill(Index);
}
class LegacyActorBaseData
{
  public:
    ActorValue::Index skills[3];
    char flags;
    int services;
    unsigned char trainingSkill, trainingLevel;
    void SwapEndianData();
};
extern "C" void* __cdecl memset(void*, int, size_t);

extern "C" void* __cdecl _alloca(size_t);
int __cdecl Warning(const char*, ...);
// TESFileFormInt_tesclass: local type (was include/TESFileFormInt.h, which had no evidence of one real class); its methods are 0x00472660 TESFile::GetTESForm
class TESFileFormInt_tesclass
{
public:
    int GetTESForm();
};
// Stores skill into data.m_aeTaggedSkills[index] and marks the class modified when skill is a skill or ActorValue__Index_VALUE_NONE.
// @fnv 0x005f6e80
// pdb: TESClass::SetTagSkill | TESClass.obj | debug, call graph, medium
// pdb params: aiIndex, aeSkill
void TESClass::SetClassSkillAtIndex(int index, ActorValue::Index skill)
{
    if (ActorValue::IsSkill(skill) || skill == ActorValue::ActorValue__Index_VALUE_NONE)
    {
        data.m_aeTaggedSkills[index] = skill;
        MarkAsModified(2);
    }
}
// Clears the 28-byte data block and sets all four actor value slots to ActorValue::ActorValue__Index_VALUE_NONE.
// @fnv 0x005f67c0
// pdb: TESClass::InitializeData | TESClass.obj | retail, vtable slot, high
// pdb locals: n
void TESClass::ResetFormData()
{
    memset(&data, 0, 28);
    for (int i = 0; i < 4; ++i)
        SetClassSkillAtIndex(i, ActorValue::ActorValue__Index_VALUE_NONE);
}
// Loads the base form data, then reads four skill values from the buffer when change flag 2 is set.
// @fnv 0x005f71c0
// pdb: TESClass::LoadGame | TESClass.obj | retail, vtable slot, high
// pdb locals: clength, pfullName, ptextureName
void TESClass::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::LoadGame(apLoadGameBuffer);
    if (apLoadGameBuffer->GetChangeFlags(2u).operator bool())
    {
        for (unsigned int i = 0; i < 4; ++i)
            apLoadGameBuffer->ReadTypedData(static_cast<void*>(&data.m_aeTaggedSkills[i]), 4u);
    }
}
// Reads a TESClass record from the file, handling EDID, OBND, FULL, DESC, ICON, DATA and ATTR chunks, then warns if the class data is invalid.
// @fnv 0x005f6930
// pdb: TESClass::Load | TESClass.obj | retail, string-set bridge, high
// pdb params: apFile
// pdb locals: chunkid, pMessage, tempData, i, cAttribData
bool TESClass::Load(TESFile* apFile)
{
    if (((TESFileFormInt_tesclass*)apFile)->GetTESForm() != 7)
        return false;
    LoadRecordHeader(apFile);
    int chunk;
    while (chunk = apFile->GetChunkID())
    {
        switch (chunk)
        {
        case 0x44494445: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 512);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:
            TESFullName::LoadFullNameChunk(this, apFile);
            break;
        case 0x43534544:
            TESDescription::ReadDescriptionFromFile(this, apFile);
            break;
        case 0x4e4f4349:
            TESTexture::ReadTextureData(this, apFile);
            break;
        case 0x41544144:
            if (apFile->GetChunkSize() == GetDataSizeAdjustment() + 28)
            {
                ((BGSImpactData*)this)->ReadTypedData(apFile, &data, 28);
                if (apFile->IsEndianConversionRequired())
                    data.CLASS_DATA_Endian();
            }
            else if (apFile->GetChunkSize() == 24 || apFile->GetChunkSize() == GetDataSizeAdjustment() + 24)
            {
                LegacyActorBaseData oldData;
                if (apFile->GetChunkSize() == GetDataSizeAdjustment() + 24)
                    ((BGSImpactData*)this)->ReadTypedData(apFile, &oldData, 24);
                else
                    apFile->GetChunkData(&oldData, 24);
                if (apFile->IsEndianConversionRequired())
                    oldData.SwapEndianData();
                data.m_ucClassFlags = oldData.flags;
                data.m_uiField08 = oldData.services;
                data.m_ucField0D = oldData.trainingLevel;
                data.m_ucField0C = oldData.trainingSkill;
                for (int i = 0; i < 3; ++i)
                    data.m_aeTaggedSkills[i] = oldData.skills[i];
            }
            else
            {
                apFile->GetChunkData(&data, 28);
                if (apFile->IsEndianConversionRequired())
                    data.CLASS_DATA_Endian();
            }
            break;
        case 0x52545441: {
            unsigned char attributes[7];
            apFile->GetChunkData(attributes, 7);
            TESAttributes::Load(attributes);
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    if (!CheckData())
        Warning("MASTERFILE: Class %s contains invalid data. Make sure all attributes and skills are unique.",
                GetName());
    return true;
}

// @fnv 0x005f6e20
// src: XBOX_DEBUG_PDB 0x005f6e20
// pdb: TESClass::CheckData | TESClass.obj | debug, call graph, high
// pdb locals: bGoodData
bool TESClass::CheckData()
{
    bool bIsSkill = true;
    if (!ActorValue::IsSkill(static_cast<ActorValue::Index>(GetTrainingSkill())))
    {
        SetTrainingSkill(static_cast<ActorValue::Index>(0x20));
        bIsSkill = false;
    }
    return bIsSkill;
}

template <int N> struct EndianSizeTag
{
};

void __cdecl BSEndian(int*, EndianSizeTag<4>);
void __cdecl BSEndian(int*, EndianSizeTag<3>);
void __cdecl BSEndian(float*, EndianSizeTag<3>);

// Byte-swaps the four 3-byte values at the start of this object and the 4-byte value at offset 0x14.
// @fnv 0x005f68d0
// src: XBOX_DEBUG_PDB 0x005f68d0
// pdb: CLASS_DATA::Endian | TESClass.obj | debug, call graph (shared callers), high
// pdb locals: n
void TESClassData::CLASS_DATA_Endian()
{
    BSEndian(&m_uiField08, EndianSizeTag<4>());
    for (int i = 0; i < 4; ++i)
        BSEndian(reinterpret_cast<int*>(&m_aeTaggedSkills[i]), EndianSizeTag<3>());
}

namespace fnv_005f6c90 {
template <int N> class EndianSizeTag
{
};

void __cdecl BSEndian(int*, EndianSizeTag<3>);

struct FUN_005f6c90_shell
{
    int m_data[4];
    void SwapEndianData();
};

// @fnv 0x005f6c90
// pdb: OLD_CLASS_DATA::Endian | TESClass.obj | debug, object-file gap fill, medium
// pdb locals: n
void FUN_005f6c90_shell::SwapEndianData()
{
    BSEndian(&m_data[4], EndianSizeTag<3>());
    for (int i = 0; i < 3; ++i)
    {
        BSEndian(&m_data[i], EndianSizeTag<3>());
    }
}
}  // namespace fnv_005f6c90

// @fnv 0x005f6dd0
int TESClass::GetTrainingSkill()
{
    int result = data.m_ucField0C + 0x20;
    return result;
}

// If index is a skill, stores index minus 0x20 into the byte at offset 0x5c of this object.
// @fnv 0x005f6df0
// pdb: TESClass::SetTrainingSkill | TESClass.obj | debug, upstream name already matches, high
// pdb params: aeSkill
void TESClass::SetTrainingSkill(ActorValue::Index aeSkill)
{
    if (ActorValue::IsSkill(aeSkill))
    {
        data.m_ucField0C = (unsigned char)(aeSkill - 0x20);
    }
}

// @fnv 0x005f7150
// pdb: TESClass::SaveGame | TESClass.obj | retail, vtable slot, high
// pdb locals: clength
void __thiscall TESClass::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    unsigned int i;
    TESForm::SaveGame(apSaveGameBuffer);
    if (((BGSLoadFormBuffer*)apSaveGameBuffer)->GetChangeFlags(2u).operator bool())
    {
        for (i = 0; i < 4; ++i)
        {
            apSaveGameBuffer->SaveData(&data.m_aeTaggedSkills[i], 4, 0);
        }
    }
}

extern "C"
{
    unsigned char __cdecl _FUN_00401500();
}

void __cdecl TESForm_AddChunkArray(unsigned int, void*, unsigned int);
void __cdecl _FUN_00485990(unsigned int, void*, unsigned int);

// @fnv 0x005f6810
// pdb: TESClass::Save | TESClass.obj | retail, vtable slot, high
// pdb locals: cAttribData
void __thiscall TESClass::SaveForm(void)
{
    char cAttribData[7];
    TESForm::BeginFormRecord();
    TESFullName::Save_00487010();
    TESDescription::Save_00482cb0();
    TESTexture::Save(0x4e4f4349);
    if (_FUN_00401500())
    {
        data.CLASS_DATA_Endian();
    }
    _FUN_00485990(0x41544144, &data, 0x1c);
    if (_FUN_00401500())
    {
        data.CLASS_DATA_Endian();
    }
    TESAttributes::Save(cAttribData);
    TESForm_AddChunkArray(0x52545441, cAttribData, 7);
    TESForm::FinishFormRecord();
}

void __cdecl _Deallocate(void*);
// Destroys the class, frees it when flags has bit 1 set, and returns its address.
// @fnv 0x005f66e0
// pdb: TESClass::`scalar deleting destructor' | TESClass.obj | retail, vtable slot, high
void* TESClass::DestroyIfFlagged(unsigned int flags)
{
    this->TESClass::~TESClass();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

void* __memcpy(void*, const void*, unsigned int);
// @fnv 0x005f6d70
// pdb: TESClass::Copy | TESClass.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: cl
void TESClass::CopyFromTESClass(TESForm* apCopy)
{
    TESClass* cl = dynamic_cast<TESClass*>(apCopy);
    if (!cl)
        return;
    TESForm_CopyAllComponents(apCopy);
    __memcpy(&data, cl->GetClassData(), 0x1c);
}

// Tears down the class component bases.
// @fnv 0x005f6710
// pdb: TESClass::~TESClass | TESClass.obj | retail, upstream name already matches, high
TESClass::~TESClass()
{}

extern "C" int __cdecl _memcmp(const void*, const void*, unsigned int);

// Returns true if the cast fails, FUN_00485270 returns true, or the two TESClassData byte ranges differ.
// @fnv 0x005f6cf0
// pdb: TESClass::Compare | TESClass.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pComp
bool TESClass::CompareClass(TESForm* apCompare)
{
    TESClass* pClass = dynamic_cast<TESClass*>(apCompare);
    if (!pClass)
        return true;
    if (TESForm_CompareAllComponents(pClass))
        return true;
    if (_memcmp(&data, pClass->GetClassData(), sizeof(TESClassData)))
        return true;
    return false;
}
