// fallout_shared/tesobjectimod.cpp: TESObjectIMOD (14 functions)

#include "../../include/BGSDestructibleObjectForm.h"
#include "../../include/BGSPickupPutdownSounds.h"
#include "../../include/TESBoundObject.h"
#include "../../include/TESForm.h"
#include "../../include/BGSMessageIcon.h"
#include "../../include/TESFullName.h"
#include "../../include/TESIcon.h"
#include "../../include/TESTexture.h"
#include "../../include/TESModel.h"
#include "../../include/TESModelTextureSwap.h"
#include "../../include/TESScriptableForm.h"
#include "../../include/TESValueForm.h"
#include "../../include/TESWeightForm.h"
#include "../../include/TESDescription.h"
#include "../../include/TESObjectIMOD.h"

extern "C" void* __cdecl _alloca(size_t);
#pragma intrinsic(_alloca)

#include "../../include/ENUM_FORM_ID.h"

// Access to TESForm serialization entry points absent from its shared header.
// This adds declarations only; all layout and virtual slots come from TESForm.
class TESFormSerialization : public TESForm
{
  public:
    unsigned short GetSaveSizeLegacy(unsigned int flags);
    void SaveHeader();
};

class TESModelRecordWriter : public TESModel
{
  public:
    void ProcessAndFreeData(unsigned int, unsigned int, unsigned int);
};

class PickupPutdownSoundRecordWriter : public BGSPickupPutdownSounds
{
  public:
    void BGSPickupPutdownSounds_Save();
};

// @fnv 0x00519fc0
// pdb: TESObjectIMOD::InitItem | TESObjectIMOD.obj | retail, upstream name already matches, high
void TESObjectIMOD::InitItem()
{
    if (IsFormInitialized())
        return;
    TESScriptableForm::InitItem(this);
    BGSDestructibleObjectForm::InitItem(this);
    TESModelTextureSwap::InitItem(this);
    BGSPickupPutdownSounds::InitItem(this);
    SetInitializationState(true);
}

// Loads a TESObjectIMOD record: reads the header, then dispatches each chunk to its loader until no chunks remain, returning true.
// @fnv 0x0051a030
// pdb: TESObjectIMOD::Load | TESObjectIMOD.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, pMessage, isize, pdata, tempint, itempint
bool TESObjectIMOD::Load(TESFile* apFile)
{
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    int chunkID;
    while ((chunkID = (int)apFile->GetChunkID()) != 0)
    {
        switch (chunkID)
        {
        case 0x44494445: {  // EDID
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:  // OBND
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:  // FULL
            TESFullName::LoadFullNameChunk(this, apFile);
            break;
        case 0x4c444f4d:  // MODL
        case 0x44444f4d:  // MODD
        case 0x54444f4d:  // MODT
        case 0x44534f4d:  // MOSD
            TESModel::LoadModelData(this, apFile);
            break;
        case 0x53444f4d: {  // MODS
            struct TextureSwapChunk
            {
                unsigned int size;
                void* data;
            } swaps;
            swaps.size = apFile->GetChunkSize();
            swaps.data = _alloca(swaps.size);
            apFile->GetChunkData(swaps.data, swaps.size);
            UnpackTextureSwapChunkData(swaps.data, swaps.size, apFile->GetFormVersion());
            break;
        }
        case 0x4e4f4349:  // ICON
            TESTexture::ReadTextureData(this, apFile);
            break;
        case 0x4f43494d:  // MICO
            BGSMessageIcon::LoadMessageIconChunk(this, apFile);
            break;
        case 0x49524353: {  // SCRI
            unsigned int scriptID = 0;
            apFile->GetChunkData(&scriptID);
            SetScript(reinterpret_cast<Script*>(scriptID));
            TESScriptableForm::InitItem(this);
            break;
        }
        case 0x43534544:  // DESC
            TESDescription::ReadDescriptionFromFile(this, apFile);
            break;
        case 0x54534544:  // DEST
        case 0x44545344:  // DSTD
            BGSDestructibleObjectForm::LoadDestructibleData(this, apFile);
            break;
        case 0x4d414e59: {  // YNAM
            unsigned int soundID = 0;
            apFile->GetChunkData(&soundID);
            SetPickupSound(reinterpret_cast<TESSound*>(soundID));
            break;
        }
        case 0x4d414e5a: {  // ZNAM
            unsigned int soundID = 0;
            apFile->GetChunkData(&soundID);
            SetPutdownSound(reinterpret_cast<TESSound*>(soundID));
            break;
        }
        case 0x41544144:  // DATA
            ReadTypedData(apFile, 0, 0);
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x00519cc0
// pdb: TESObjectIMOD::TESObjectIMOD | TESObjectIMOD.obj | retail, upstream name already matches, high
TESObjectIMOD::TESObjectIMOD()
{
    SetFormType(ENUM_FORM_ID_IMOD_ID);
    SetInitializationState(true);
}

// @fnv 0x0051a5d0
// pdb: TESObjectIMOD::LoadGame | TESObjectIMOD.obj | retail, vtable slot, high
// pdb params: aiFlags, aiCurrentFlags
void TESObjectIMOD::LoadGameLegacy(unsigned int aiFlags, unsigned int aiCurrentFlags)
{
    TESForm::LoadGameLegacy(aiFlags, aiCurrentFlags);
    TESValueForm::LoadGame_0048ea10(aiFlags, aiCurrentFlags);
}

// @fnv 0x0051a630
// pdb: TESObjectIMOD::LoadGame | TESObjectIMOD.obj | retail, vtable slot, high
// pdb params: aiFlags, aiCurrentFlags
void TESObjectIMOD::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::LoadGame(apLoadGameBuffer);
    TESValueForm::LoadGame(apLoadGameBuffer);
}

extern "C" void __cdecl _Deallocate(void*);

// @fnv 0x00519e40
// pdb: TESObjectIMOD::`scalar deleting destructor' | TESObjectIMOD.obj | retail, vtable slot, high
void* TESObjectIMOD::Destroy(unsigned int flags)
{
    TESObjectIMOD::~TESObjectIMOD();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

// @fnv 0x0051a550
// pdb: TESObjectIMOD::GetSaveSize | TESObjectIMOD.obj | retail, vtable slot, high
// pdb params: aiFlags
// pdb locals: isize
unsigned short TESObjectIMOD::GetSaveSizeLegacy(unsigned int aiFlags)
{
    unsigned short isize = 0;
    isize += reinterpret_cast<TESFormSerialization*>(this)->GetSaveSizeLegacy(aiFlags);
    isize += TESValueForm::GetFlaggedDataSize(aiFlags);
    return isize;
}

// @fnv 0x0051a5a0
// pdb: TESObjectIMOD::SaveGame | TESObjectIMOD.obj | retail, vtable slot, high
// pdb params: aiFlags
void TESObjectIMOD::SaveGameLegacy(unsigned int aiFlags)
{
    TESForm::SaveGameLegacy(aiFlags);
    TESValueForm::SaveGame_0048e9e0(aiFlags);
}

// @fnv 0x0051a600
// pdb: TESObjectIMOD::SaveGame | TESObjectIMOD.obj | retail, vtable slot, high
// pdb params: aiFlags
void TESObjectIMOD::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    TESForm::SaveGame(apSaveGameBuffer);
    TESValueForm::SaveGameBGS_0048ea40(apSaveGameBuffer);
}

// @fnv 0x0051a420
// pdb: TESObjectIMOD::Save | TESObjectIMOD.obj | retail, vtable slot, high
void TESObjectIMOD::SaveRecord()
{
    TESForm::BeginFormRecord();
    TESFullName::Save_00487010();
    reinterpret_cast<TESModelRecordWriter*>(reinterpret_cast<char*>(this) + 0x3c)->ProcessAndFreeData(0x4c444f4d, 0x54444f4d, 0x53444f4d);
    TESIcon::Save(0x4e4f4349);
    BGSMessageIcon::Save(0x4f43494d);
    TESScriptableForm::Save();
    TESDescription::Save();
    BGSDestructibleObjectForm::Save();
    reinterpret_cast<PickupPutdownSoundRecordWriter*>(reinterpret_cast<char*>(this) + 0xa4)->BGSPickupPutdownSounds_Save();
    reinterpret_cast<TESFormSerialization*>(this)->SaveHeader();
    TESForm::FinishFormRecord();
}

// @fnv 0x0051a4c0
// pdb: TESObjectIMOD::Copy | TESObjectIMOD.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pmisc
void TESObjectIMOD::CopyFrom(const TESForm* apCopy)
{
    const TESObjectIMOD* pmisc = dynamic_cast<const TESObjectIMOD*>(apCopy);
    if (!pmisc)
        return;
    TESForm::CopyFrom(apCopy);
}

// @fnv 0x0051a500
// pdb: TESObjectIMOD::Compare | TESObjectIMOD.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pcomp
bool TESObjectIMOD::Compare(TESForm* apCompare)
{
    TESObjectIMOD* pcomp;
    pcomp = dynamic_cast<TESObjectIMOD*>(apCompare);
    if (!pcomp)
        return true;
    if (TESForm::Compare(pcomp))
        return true;
    return false;
}

// @fnv 0x00519e70
// pdb: TESObjectIMOD::~TESObjectIMOD | TESObjectIMOD.obj | retail, upstream name already matches, high
TESObjectIMOD::~TESObjectIMOD()
{
    Shutdown();
}

struct FUN_0051a840 : TESBoundObject,
                      TESFullName,
                      TESModelTextureSwap,
                      TESIcon,
                      TESScriptableForm,
                      TESValueForm,
                      TESWeightForm,
                      BGSDestructibleObjectForm,
                      BGSMessageIcon,
                      BGSPickupPutdownSounds
{
    virtual ~FUN_0051a840();
    void FUN_00483710();
};
// @fnv 0x0051a840
// pdb: TESObjectMISC::~TESObjectMISC | TESObjectMISC.obj | retail, call graph (shared callers), high
FUN_0051a840::~FUN_0051a840()
{
    FUN_00483710();
}
