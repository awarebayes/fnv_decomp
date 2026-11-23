// fallout_shared/bgsvoicetype.cpp: BGSVoiceType (6 functions)

#include "../../include/BGSVoiceType.h"
#include "../../include/TESFile.h"

extern "C" void* __cdecl _alloca(size_t);
// Loads a BGSVoiceType record from a TESFile, handling EDID, OBND and DATA chunks, and returns false if the form type is not 0x5d.
// @fnv 0x005f01a0
// pdb: BGSVoiceType::Load | BGSVoiceType.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, pMessage
bool BGSVoiceType::Load(TESFile* apFile)
{
    // Preserve the original caller's full-register comparison of the return value.
    if ((apFile->*reinterpret_cast<int (__thiscall TESFile::*)()>(&TESFile::GetTESForm))() != 0x5d)
        return false;
    LoadRecordHeader(apFile);
    unsigned int chunk;
    char* buffer;
    while (chunk = apFile->GetChunkID())
    {
        switch (chunk)
        {
        case 0x44494445:
            buffer = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(buffer, 512);
            SetEditorID_AtRuntime(buffer);
            break;
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4d414e44:
            apFile->GetChunkData(&Data, 1);
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

bool __cdecl TESForm_QEndianSwapOnSave();
void __cdecl AppendChunk(unsigned int, void*, unsigned int);
// @fnv 0x005f0140
// src: XBOX_PDB 0x005f0140
// pdb: BGSVoiceType::Save | BGSVoiceType.obj | retail, vtable slot, high
void BGSVoiceType::Save_005f0140()
{
    BeginFormRecord();
    if (TESForm_QEndianSwapOnSave())
        reinterpret_cast<TESForm*>(&Data)->TESForm::ClearData();
    AppendChunk(0x4d414e44, &Data, 1);
    if (TESForm_QEndianSwapOnSave())
        reinterpret_cast<TESForm*>(&Data)->TESForm::ClearData();
    FinishFormRecord();
}

// @fnv 0x005f00d0
// pdb: BGSVoiceType::~BGSVoiceType | BGSVoiceType.obj | debug, upstream name already matches, high
BGSVoiceType::~BGSVoiceType()
{
    TESForm::ClearData();
}

extern "C" void* __cdecl FUN_00ec43fb(void* p, long vf, void* src, void* tgt, int isRef);
extern char DAT_01183028;
extern char DAT_01186318;
// If FUN_00ec43fb resolves a source from form, copies its Data member to this and calls FUN_004851b0(form).
// @fnv 0x005f02a0
// src: XBOX_PDB 0x005f02a0
// pdb: BGSVoiceType::Copy | BGSVoiceType.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pCopy
void BGSVoiceType::Copy_005f02a0(TESForm* apCopy)
{
    BGSVoiceType* src = (BGSVoiceType*)FUN_00ec43fb(apCopy, 0, &DAT_01183028, &DAT_01186318, 0);
    if (!src)
    {
        return;
    }
    Data = src->Data;
    TESForm_CopyAllComponents(apCopy);
}

// Returns true if form is not a BGSVoiceType, its Data differs, or FUN_00485270(voice) is true; otherwise false.
// @fnv 0x005f02f0
// src: XBOX_PDB 0x005f02f0
// pdb: BGSVoiceType::Compare | BGSVoiceType.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pComp
bool BGSVoiceType::Compare_005f02f0(TESForm* apCompare)
{
    BGSVoiceType* voice = dynamic_cast<BGSVoiceType*>(apCompare);
    if (!voice)
        return true;
    if (Data != voice->Data)
        return true;
    if (TESForm_CompareAllComponents(voice))
        return true;
    return false;
}
