// fallout_shared/tescaravan.cpp: TESCaravanCard, TESCaravanMoney, TESCaravanDeck and related operations (32 functions)

#include "layout_support.h"
#include "../../include/BGSImpactData.h"
#include "../../include/BGSMessageIcon.h"
#include "../../include/BSThread.h"
#include "../../include/TESBoundObject.h"
#include "../../include/TESBoundObject__BOUND_DATA.h"
#include "../../include/TESCaravanCard.h"
#include "../../include/TESCaravanMoney.h"
#include "../../include/TESModelTextureSwap.h"
#include "../../include/TESObject.h"

#include "../../include/BGSPickupPutdownSounds.h"
#include "../../include/CaravanDeckData.h"
#include "../../include/TESFullName.h"
#include "../../include/TESScriptableForm.h"
#include "../../include/TESValueForm.h"
#include "../../include/UnsignedValuePair.h"
#include "../../include/TESCaravanDeck.h"

// These extensions only declare operations missing from the shared headers.
struct CaravanCardAccess : TESCaravanCard
{
    TESTexture* GetFrontTexture();
    UnsignedValuePair* GetCardData();
};

// Retained locally: the shared TESCaravanDeck lacks the accessors and comparison
// used at 0x0059bb30, 0x004610d0 and 0x0059be50; this is the real deck layout.

FNV_STATIC_ASSERT(sizeof(TESValueForm) == 8);
FNV_STATIC_ASSERT(offsetof(TESValueForm, iValue) == 4);

FNV_STATIC_ASSERT(sizeof(TESScriptableForm) == 0xC);
FNV_STATIC_ASSERT(offsetof(TESScriptableForm, m_pFormScript) == 0x4);
FNV_STATIC_ASSERT(offsetof(TESScriptableForm, m_bHasScript) == 0x8);

FNV_STATIC_ASSERT(sizeof(BGSPickupPutdownSounds) == 12);
FNV_STATIC_ASSERT(offsetof(BGSPickupPutdownSounds, m_pPickupSound) == 4);
FNV_STATIC_ASSERT(offsetof(BGSPickupPutdownSounds, m_pPutdownSound) == 8);

FNV_STATIC_ASSERT(sizeof(UnsignedValuePair) == 8);
FNV_STATIC_ASSERT(offsetof(UnsignedValuePair, m_uiFirstValue) == 0);
FNV_STATIC_ASSERT(offsetof(UnsignedValuePair, m_uiSecondValue) == 4);

FNV_STATIC_ASSERT(sizeof(CaravanDeckData) == 4);
FNV_STATIC_ASSERT(offsetof(CaravanDeckData, m_iValue) == 0);

FNV_STATIC_ASSERT(sizeof(TESCaravanDeck) == 44);
FNV_STATIC_ASSERT(offsetof(TESCaravanDeck, m_pCardListHeadCur) == 36);
FNV_STATIC_ASSERT(offsetof(TESCaravanDeck, data) == 40);

class TESFileFormVersion
{
  public:
    unsigned short GetFormVersion();
};
class TESTextureLoader
{
  public:
    static void ReadTextureData(TESTexture*, TESFile*);
};

class RegionDataEntry
{
  public:
    void Endian();
};

extern "C" void* memset(void* dst, int val, size_t size);

// @fnv 0x0059a370
// pdb: TESCaravanCard::TESCaravanCard | TESCaravan.obj | retail, upstream name already matches, high
TESCaravanCard::TESCaravanCard()
{
    SetFormType(static_cast<ENUM_FORM_ID>(0x73));
    ResetFormData();
}

// @fnv 0x0059a670
// pdb: TESCaravanCard::InitializeData | TESCaravan.obj | retail, vtable slot, high
void TESCaravanCard::ResetFormData()
{
    faceTexture.Init();
    backTexture.Init();
    memset(&data, 0, sizeof(data));
}

// @fnv 0x0059a6c0
// pdb: TESCaravanCard::InitItem | TESCaravan.obj | retail, upstream name already matches, high
void TESCaravanCard::InitItem()
{
    if (IsFormInitialized())
        return;
    TESModelTextureSwap::InitItem(this);
    TESScriptableForm::InitItem(this);
    BGSPickupPutdownSounds::InitItem(this);
    SetInitializationState(true);
}

// Initializes both entries of the array by calling Init on each.
// @fnv 0x0059b120
// pdb: TESCaravanMoney::InitializeData | TESCaravan.obj | retail, vtable slot, high
void TESCaravanMoney::ResetFormData()
{
    anteModels[0].Init();
    anteModels[1].Init();
}

// @fnv 0x0059b160
// pdb: TESCaravanMoney::InitItem | TESCaravan.obj | retail, upstream name already matches, high
void TESCaravanMoney::InitItem()
{
    if (IsFormInitialized())
        return;
    TESModelTextureSwap::InitItem(this);
    BGSPickupPutdownSounds::InitItem(this);
    SetInitializationState(true);
}

// @fnv 0x0059b880
// pdb: TESCaravanDeck::TESCaravanDeck | TESCaravan.obj | retail, upstream name already matches, high
TESCaravanDeck::TESCaravanDeck()
{
    SetFormType(static_cast<ENUM_FORM_ID>(0x75));
    ResetFormData();
}

// @fnv 0x0059b9f0
// pdb: TESCaravanDeck::InitializeData | TESCaravan.obj | retail, vtable slot, high
void TESCaravanDeck::ResetFormData()
{
    m_pCardListHeadCur = new BSSimpleList<TESCaravanCard*>();
    data.m_iValue = 0;
}

// Reads a TESCaravanDeck record from file: validates form type 0x75, loads header, then processes EDID, OBND, FULL, DATA and CARD chunks.
// @fnv 0x0059bb50
// pdb: TESCaravanDeck::Load | TESCaravan.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, formid, pMessage, pForm, pCard, cardID
bool TESCaravanDeck::Load(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x75)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    int chunkID;
    while ((chunkID = apFile->GetChunkID()) != 0)
    {
        switch (chunkID)
        {
        case 0x44494445: {
            char* editorID = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:
            TESFullNameLoader::LoadFullNameChunk(static_cast<TESFullName*>(this), apFile);
            break;
        case 0x41544144:
            reinterpret_cast<BGSImpactData*>(this)->ReadTypedData(apFile, &data, 4);
            break;
        case 0x44524143: {
            unsigned int formID;
            apFile->GetChunkData(&formID);
            RemapFormIDForFile(formID, apFile);
            TESForm* form = TESForm::GetFormByID(formID);
            TESCaravanCard* card = static_cast<TESCaravanCard*>(form);
            if (!m_pCardListHeadCur || m_pCardListHeadCur->IsEmpty())
                m_pCardListHeadCur->SetItem(card);
            else
                m_pCardListHeadCur->AddHead(card);
            break;
        }
        }
        if (chunkID == 0x41544144 && apFile->IsEndianConversionRequired())
            reinterpret_cast<RegionDataEntry*>(&data)->Endian();
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x0059b290
// pdb: TESCaravanMoney::Load | TESCaravan.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, formid, pMessage, isize, pdata, itempint, pmessage
bool TESCaravanMoney::Load(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x74)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    int chunkID;
    while ((chunkID = apFile->GetChunkID()) != 0)
    {
        switch (chunkID)
        {
        case 0x44494445: {
            char* editorID = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:
            TESFullNameLoader::LoadFullNameChunk(this, apFile);
            break;
        case 0x4c444f4d:
        case 0x44444f4d:
        case 0x44534f4d:
        case 0x54444f4d:
            TESModel::LoadModelData(this, apFile);
            break;
        case 0x53444f4d: {
            char* textureSwapData;
            unsigned int chunkSize = apFile->GetChunkSize();
            textureSwapData = static_cast<char*>(_alloca(chunkSize));
            apFile->GetChunkData(textureSwapData, chunkSize);
            UnpackTextureSwapChunkData(textureSwapData, chunkSize,
                                 reinterpret_cast<TESFileFormVersion*>(apFile)->GetFormVersion());
            break;
        }
        case 0x4e4f4349:
            TESTextureLoader::ReadTextureData(static_cast<TESIcon*>(this), apFile);
            break;
        case 0x4f43494d:
            BGSMessageIcon::LoadMessageIconChunk(this, apFile);
            break;
        case 0x4d414e59: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            SetPickupSound(reinterpret_cast<TESSound*>(id));
            break;
        }
        case 0x4d414e5a: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            SetPutdownSound(reinterpret_cast<TESSound*>(id));
            break;
        }
        case 0x41544144:
            reinterpret_cast<BGSImpactData*>(this)->ReadTypedData(apFile, 0, 0);
            break;
        case 0x32444f4d: {
            char* model = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(model, 0);
            anteModels[0].SetModelFilePath(model);
            TESModel::LoadTextureSwapData(&anteModels[0], apFile);
            break;
        }
        case 0x33444f4d: {
            char* model = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(model, 0);
            anteModels[1].SetModelFilePath(model);
            TESModel::LoadTextureSwapData(&anteModels[1], apFile);
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x0059a800
// pdb: TESCaravanCard::Load | TESCaravan.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, int1loaded, formid, pMessage, isize, pdata, tempint, itempint
bool TESCaravanCard::Load(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    bool m_bHeaderParsed = false;
    if (formType != 0x73)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    int chunkID;
    while ((chunkID = apFile->GetChunkID()) != 0)
    {
        switch (chunkID)
        {
        case 0x44494445: {
            char* editorID = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:
            TESFullNameLoader::LoadFullNameChunk(this, apFile);
            break;
        case 0x4c444f4d:
        case 0x44444f4d:
        case 0x44534f4d:
        case 0x54444f4d:
            TESModel::LoadModelData(this, apFile);
            break;
        case 0x53444f4d: {
            char* textureSwapData;
            unsigned int chunkSize = apFile->GetChunkSize();
            textureSwapData = static_cast<char*>(_alloca(chunkSize));
            apFile->GetChunkData(textureSwapData, chunkSize);
            UnpackTextureSwapChunkData(textureSwapData, chunkSize,
                                 reinterpret_cast<TESFileFormVersion*>(apFile)->GetFormVersion());
            break;
        }
        case 0x4e4f4349:
            TESTextureLoader::ReadTextureData(static_cast<TESIcon*>(this), apFile);
            break;
        case 0x4f43494d:
            BGSMessageIcon::LoadMessageIconChunk(this, apFile);
            break;
        case 0x49524353: {
            unsigned int scriptID = 0;
            apFile->GetChunkData(&scriptID);
            SetScript(reinterpret_cast<Script*>(scriptID));
            TESScriptableForm::InitItem(this);
            break;
        }
        case 0x4d414e59: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            SetPickupSound(reinterpret_cast<TESSound*>(id));
            break;
        }
        case 0x4d414e5a: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            SetPutdownSound(reinterpret_cast<TESSound*>(id));
            break;
        }
        case 0x41544144:
            reinterpret_cast<BGSImpactData*>(this)->ReadTypedData(apFile, 0, 0);
            break;
        case 0x30305854:
            TESTextureLoader::ReadTextureData(&faceTexture, apFile);
            break;
        case 0x31305854:
            TESTextureLoader::ReadTextureData(&backTexture, apFile);
            break;
        case 0x56544e49:
            if (!m_bHeaderParsed)
            {
                apFile->GetChunkData(&data.m_uiSecondValue);
                m_bHeaderParsed = true;
            }
            else
            {
                apFile->GetChunkData(&data.m_uiFirstValue);
            }
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x00503650
// src: XBOX_PDB 0x00503650
TESTexture* TESCaravanCard::GetBackTexture()
{
    return &backTexture;
}

struct CaravanCardLoadGame : public TESCaravanCard
{
    void LoadGame(void* arg);
};

// @fnv 0x0059adf0
// pdb: TESCaravanCard::LoadGame | TESCaravan.obj | debug, object-file run extension, low
// pdb params: apLoadGameBuffer
void CaravanCardLoadGame::LoadGame(void* arg)
{
    TESForm::LoadGame(static_cast<BGSLoadFormBuffer*>(arg));
}

// @fnv 0x0059b270
TESModelTextureSwap* TESCaravanMoney::GetModelTextureSwap(int index)
{
    return &anteModels[index];
}

struct List_009e03e0
{
    void RemoveAll();
};

struct FUN_0059b9c0_shell
{
    char pad1[0x24];
    List_009e03e0* m_pCardListHeadCur;
    unsigned int data;
    void ClearListAndData();
};

// Calls RemoveAll on the list and zeroes the 4-byte data field.
// @fnv 0x0059b9c0
// pdb: TESCaravanDeck::ClearData | TESCaravan.obj | retail, vtable slot, high
void FUN_0059b9c0_shell::ClearListAndData()
{
    m_pCardListHeadCur->RemoveAll();
    memset(&data, 0, 4);
}

struct FUN_0048e370_shell
{
    void TESTexture_Save(int);
};
struct FUN_0047b560_shell
{
    void BGSPickupPutdownSounds_Save(void);
};
struct FUN_004855a0
{
    void M();
};
struct FUN_00485680
{
    void M();
};
struct S_00487010
{
    void Save_00487010();
};
struct FUN_00485e00_shell
{
    void TESForm_SaveData();
};

// @fnv 0x0059b1b0
// pdb: TESCaravanMoney::Save | TESCaravan.obj | retail, vtable slot, high
void TESCaravanMoney::SaveForm()
{
    TESForm::BeginFormRecord();
    ((S_00487010*)((char*)this + 0x30))->Save_00487010();
    reinterpret_cast<TESModelTextureSwap*>(reinterpret_cast<char*>(this) + sizeof(TESBoundObject) + sizeof(TESFullName))->ProcessAndFreeData(0x4c444f4d, 0x54444f4d, 0x53444f4d);
    ((FUN_0048e370_shell*)((char*)this + 0x5c))->TESTexture_Save(0x4e4f4349);
    BGSMessageIcon::Save(0x4f43494d);
    ((FUN_0047b560_shell*)((char*)this + 0x80))->BGSPickupPutdownSounds_Save();
    ((FUN_00485e00_shell*)this)->TESForm_SaveData();
    ((TESModelTextureSwap*)GetModelTextureSwap(0))->ProcessAndFreeData(0x32444f4d, 0x54444f4d, 0x53444f4d);
    ((TESModelTextureSwap*)GetModelTextureSwap(1))->ProcessAndFreeData(0x33444f4d, 0x54444f4d, 0x53444f4d);
    TESForm::FinishFormRecord();
}

void _Deallocate(void*);
struct CaravanCardRelease : TESCaravanCard
{
    void* Release(unsigned int flags);
};
// Destroy the card and deallocate it when the low bit of flags is set.
// @fnv 0x0059a4c0
// pdb: TESCaravanCard::`scalar deleting destructor' | TESCaravan.obj | retail, vtable slot, high
void* CaravanCardRelease::Release(unsigned int flags)
{
    TESCaravanCard::~TESCaravanCard();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

struct FUN_0048e370_obj
{
    void TESTexture_Save(unsigned int);
};
struct S_0048cca0
{
    void Save_0048cca0();
};
struct FUN_0047b560_holder
{
    void BGSPickupPutdownSounds_Save();
};
extern "C" unsigned int _FUN_00485910(unsigned int, unsigned int);

struct CaravanCardWriter : TESCaravanCard
{
    void SaveForm();
};

// @fnv 0x0059a720
// pdb: TESCaravanCard::Save | TESCaravan.obj | retail, vtable slot, high
void CaravanCardWriter::SaveForm()
{
    TESForm::BeginFormRecord();
    ((S_00487010*)((char*)this + 0x30))->Save_00487010();
    reinterpret_cast<TESModelTextureSwap*>(reinterpret_cast<char*>(this) + sizeof(TESBoundObject) + sizeof(TESFullName))->ProcessAndFreeData(0x4c444f4d, 0x54444f4d, 0x53444f4d);
    ((FUN_0048e370_obj*)((char*)this + 0x5c))->TESTexture_Save(0x4e4f4349);
    BGSMessageIcon::Save(0x4f43494d);
    ((S_0048cca0*)((char*)this + 0x80))->Save_0048cca0();
    ((FUN_0047b560_holder*)((char*)this + 0x8c))->BGSPickupPutdownSounds_Save();
    ((FUN_0048e370_obj*)static_cast<CaravanCardAccess*>(static_cast<TESCaravanCard*>(this))->GetFrontTexture())->TESTexture_Save(0x30305854);
    ((FUN_0048e370_obj*)GetBackTexture())->TESTexture_Save(0x31305854);
    _FUN_00485910(0x56544e49, data.m_uiSecondValue);
    _FUN_00485910(0x56544e49, data.m_uiFirstValue);
    ((FUN_00485e00_shell*)this)->TESForm_SaveData();
    TESForm::FinishFormRecord();
}

struct CaravanCardSaveGame : TESCaravanCard
{
    void ForwardToBase(unsigned int flags);
};
// @fnv 0x0059add0
// pdb: TESCaravanCard::SaveGame | TESCaravan.obj | debug, object-file run extension, low
// pdb params: apSaveGameBuffer
void CaravanCardSaveGame::ForwardToBase(unsigned int a)
{
    TESForm::SaveGame(reinterpret_cast<BGSSaveFormBuffer*>(a));
}

void _Deallocate(void*);
// Destroy the money form and deallocate it when the low bit of flags is set.
// @fnv 0x0059af90
// pdb: TESCaravanMoney::`scalar deleting destructor' | TESCaravan.obj | retail, vtable slot, high
void* TESCaravanMoney::Release(unsigned int flags)
{
    TESCaravanMoney::~TESCaravanMoney();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

// @fnv 0x0059b0e0
// pdb: TESCaravanMoney::ClearData | TESCaravan.obj | retail, upstream name already matches, high
void TESCaravanMoney::ClearData()
{
    anteModels[0].Free();
    anteModels[1].Free();
}

struct FUN_0059b910_shell
{
    void FUN_0059b940();
    void* FUN_0059b910(unsigned int);
};
void _Deallocate(void*);
// Calls FUN_0059b940, then frees this object with _Deallocate when bit 0 of flags is set, and returns this.
// @fnv 0x0059b910
// pdb: TESCaravanDeck::`scalar deleting destructor' | TESCaravan.obj | retail, vtable slot, high
void* FUN_0059b910_shell::FUN_0059b910(unsigned int flags)
{
    FUN_0059b940();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

namespace fnv_0059ba80 {
struct FUN_004855a0
{
    void M();
};
struct S_00487010
{
    void Save_00487010();
};
extern "C" void __cdecl TESForm_AddChunk_00485910(int, unsigned int);
extern "C" bool __cdecl TESForm_QEndianSwapOnSave();
struct BipedModelForm
{
    void Endian();
};
struct FUN_00485e00_shell
{
    void TESForm___SaveData(int, int);
};
struct FUN_00485680
{
    void M();
};

struct FUN_0059ba80_shell : public FUN_004855a0
{
    BSSimpleList<TESCaravanCard*>* m_pCardListHeadCur;
    BipedModelForm data;
    void TESForm___SaveData(int, int);
    void M();
    void TESCaravanDeck_SaveAlt();
};

// @fnv 0x0059ba80
// src: COMMUNITY 0x0059ba80
// pdb: TESCaravanDeck::Save | TESCaravan.obj | retail, vtable slot, high
// pdb locals: pDeck
void FUN_0059ba80_shell::TESCaravanDeck_SaveAlt()
{
    M();
    ((S_00487010*)((char*)this + 0x18))->Save_00487010();
    BSSimpleList<TESCaravanCard*>* current = (BSSimpleList<TESCaravanCard*>*)((BSThread*)this)->GetThreadID();
    while (current && current->GetItem())
    {
        TESForm_AddChunk_00485910(0x44524143, current->GetItem()->GetFormID());
        current = current->GetNext();
    }
    if (TESForm_QEndianSwapOnSave())
        ((BipedModelForm*)((char*)this + 0x28))->Endian();
    TESForm___SaveData((int)((char*)this + 0x28), 4);
    if (TESForm_QEndianSwapOnSave())
        ((BipedModelForm*)((char*)this + 0x28))->Endian();
    ((FUN_00485680*)this)->M();
}
}  // namespace fnv_0059ba80

// ClearData is not yet declared by the shared caravan-card header.
struct CaravanCardCleanup : TESCaravanCard
{
    void ClearData();
};

// @fnv 0x0059a620
// pdb: TESCaravanCard::ClearData | TESCaravan.obj | retail, vtable slot, high
void CaravanCardCleanup::ClearData()
{
    faceTexture.Free();
    backTexture.Free();
    memset(&data, 0, sizeof(data));
}

// @fnv 0x0059afc0
// pdb: TESCaravanMoney::~TESCaravanMoney | TESCaravan.obj | retail, upstream name already matches, high
TESCaravanMoney::~TESCaravanMoney()
{
    TESCaravanMoney::ClearData();
}

namespace fnv_0059b680 {
extern "C"
{
    extern char DAT_0118c5e4[];
    extern char DAT_01183028[];
}

void* __cdecl FUN_00ec43fb(unsigned int, unsigned int, unsigned int, unsigned int, int);

struct FUN_00401170_shell
{
    void* FUN_00401170();
};

struct CaravanMoneyCopy : TESCaravanMoney
{
    void TESForm_CopyAllComponents(void* source);
    void FUN_004f15a0(void* type);
    void CopyFrom(int apCopy);
};

// @fnv 0x0059b680
// pdb: TESCaravanMoney::Copy | TESCaravan.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pCopy
void CaravanMoneyCopy::CopyFrom(int apCopy)
{
    void* p = FUN_00ec43fb(apCopy, 0, (unsigned int)DAT_01183028, (unsigned int)DAT_0118c5e4, 0);
    if (p == 0)
    {
    }
    else
    {
        TESForm_CopyAllComponents(p);
        anteModels[0].SetModelFilePath(static_cast<TESCaravanMoney*>(p)->GetModelTextureSwap(0)->GetModelFilePath());
        anteModels[1].SetModelFilePath(static_cast<TESCaravanMoney*>(p)->GetModelTextureSwap(1)->GetModelFilePath());
        FUN_004f15a0(((FUN_00401170_shell*)apCopy)->FUN_00401170());
    }
}
}  // namespace fnv_0059b680

struct BaseDtor_b5117
{
    virtual void unk00();
    ~BaseDtor_b5117();
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
};

struct SecondDtor_b5117
{
    virtual void unk00();
    ~SecondDtor_b5117();
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
};

struct FUN_0059b940 : BaseDtor_b5117, SecondDtor_b5117
{
    virtual void unk08();
    void unk0C();
    ~FUN_0059b940();
};

// @fnv 0x0059b940
// pdb: TESCaravanDeck::~TESCaravanDeck | TESCaravan.obj | debug, object-file gap fill, medium
FUN_0059b940::~FUN_0059b940()
{
    unk0C();
}

extern const char DAT_01183028[];
extern const char DAT_0118643c[];

struct Itc
{
    void** FUN_006815c0();
    Itc* FUN_00726070();
};

struct DeckData
{
    int x;
};

struct L1c
{
    Itc* FUN_0059bb30();
    DeckData* GetExtraDataList();
    unsigned int FUN_00401170();
};

struct P24c
{
    bool FUN_008256d0();
    void FUN_0063f7b0();
    void FUN_00726c60(void*& v);
    void FUN_005ae3d0(void*& v);
};

struct FUN_0059bd50_cls
{
    void TESForm_CopyAllComponents(L1c* rec);
    void FUN_004f15a0(unsigned int v);
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0C;
    unsigned int unk10;
    unsigned int unk14;
    unsigned int unk18;
    unsigned int unk1C;
    unsigned int unk20;
    P24c* p24;
    DeckData f28;
    void LoadDeckData(void* apCopy);
};

L1c* __cdecl FUN_00ec43fb(void* arg, int a2, const char* a3, const char* a4, int a5);

// @fnv 0x0059bd50
// pdb: TESCaravanDeck::Copy | TESCaravan.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: cardList, pCopy, cpyCard
void FUN_0059bd50_cls::LoadDeckData(void* apCopy)
{
    L1c* rec = FUN_00ec43fb(apCopy, 0, DAT_01183028, DAT_0118643c, 0);
    if (rec == 0)
    {
    }
    else
    {
        TESForm_CopyAllComponents(rec);
        while (p24 != 0 && !p24->FUN_008256d0())
            p24->FUN_0063f7b0();
        Itc* it = rec->FUN_0059bb30();
        while (it != 0 && *it->FUN_006815c0() != 0)
        {
            void* v = *it->FUN_006815c0();
            if (p24 == 0 || p24->FUN_008256d0())
                p24->FUN_00726c60(v);
            else
                p24->FUN_005ae3d0(v);
            it = it->FUN_00726070();
        }
        f28 = *rec->GetExtraDataList();
        FUN_004f15a0(rec->FUN_00401170());
    }
}

// @fnv 0x0059a4f0
// pdb: TESCaravanCard::~TESCaravanCard | TESCaravan.obj | retail, upstream name already matches, high
TESCaravanCard::~TESCaravanCard()
{
    static_cast<CaravanCardCleanup*>(this)->CaravanCardCleanup::ClearData();
}

namespace fnv_0059ac20 {
extern char DAT_0118c5c4, DAT_01183028;
void* FUN_00ec43fb(void*, int, void*, void*, int);
class StrB
{
  public:
    const char* TESModel_GetModel();
};
class StrSet
{
  public:
    void FUN_00489100(const char*);
};
class FormA
{
  public:
    int FUN_00401170();
};
struct CaravanCardCopy : TESCaravanCard
{
    void TESForm_CopyAllComponents(CaravanCardAccess* source);
    void FUN_004f15a0(int type);
    void CopyFrom(FormA* apCopy);
};
// @fnv 0x0059ac20
// pdb: TESCaravanCard::Copy | TESCaravan.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pCopy
void CaravanCardCopy::CopyFrom(FormA* apCopy)
{
    CaravanCardAccess* pSrc = (CaravanCardAccess*)FUN_00ec43fb(apCopy, 0, &DAT_01183028, &DAT_0118c5c4, 0);
    if (!pSrc)
        return;
    TESForm_CopyAllComponents(pSrc);
    reinterpret_cast<StrSet*>(&faceTexture)->FUN_00489100(reinterpret_cast<StrB*>(pSrc->GetFrontTexture())->TESModel_GetModel());
    reinterpret_cast<StrSet*>(&backTexture)->FUN_00489100(reinterpret_cast<StrB*>(pSrc->GetBackTexture())->TESModel_GetModel());
    data = *pSrc->GetCardData();
    FUN_004f15a0(apCopy->FUN_00401170());
}
}  // namespace fnv_0059ac20

namespace fnv_0059acd0 {
extern "C" void* __cdecl FNV_DynamicCast(void* pObj, int vfDelta, void* pSrcType, void* pTargetType, int isRef);
extern "C" void* DAT_0118c5c4;
extern "C" void* DAT_01183028;
// memcmp comes from <string.h> (read through include/TESFile.h)

struct StrHelper_00408da0
{
    const char* TESModel_GetModel();
};

int BSstrcmp(const char*, const char*);

struct CaravanCardCompare : TESCaravanCard
{
    bool TESForm_CompareAllComponents(CaravanCardAccess* apCompare);
    bool IsDifferentFromCard(void* apCompare);
};

// Returns true if the cast fails or either compared string or the 8-byte data differs.
// @fnv 0x0059acd0
// pdb: TESCaravanCard::Compare | TESCaravan.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pComp
bool CaravanCardCompare::IsDifferentFromCard(void* apCompare)
{
    CaravanCardAccess* card = (CaravanCardAccess*)FNV_DynamicCast(apCompare, 0, &DAT_01183028, &DAT_0118c5c4, 0);
    if (!card)
        return true;
    if (this->TESForm_CompareAllComponents(card))
        return true;
    if (BSstrcmp(reinterpret_cast<StrHelper_00408da0*>(&faceTexture)->TESModel_GetModel(), reinterpret_cast<StrHelper_00408da0*>(card->GetFrontTexture())->TESModel_GetModel()))
        return true;
    if (BSstrcmp(reinterpret_cast<StrHelper_00408da0*>(&backTexture)->TESModel_GetModel(), reinterpret_cast<StrHelper_00408da0*>(card->GetBackTexture())->TESModel_GetModel()))
        return true;
    if (memcmp(&data, card->GetCardData(), 8))
        return true;
    return false;
}
}  // namespace fnv_0059acd0

#include <string.h>

// Returns true if the cast fails, FUN_00485270 returns true, card counts differ, a card from the other deck is absent here, or CaravanDeckData differs.
// @fnv 0x0059be50
// pdb: TESCaravanDeck::Compare | TESCaravan.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pComp, cardList
bool TESCaravanDeck::Compare(TESForm* apCompare)
{
    TESCaravanDeck* pDeck2 = dynamic_cast<TESCaravanDeck*>(apCompare);
    if (!pDeck2)
        return true;
    if (TESForm::Compare(pDeck2))
        return true;
    if (GetCardList()->ItemsInList() != pDeck2->GetCardList()->ItemsInList())
        return true;
    BSSimpleList<TESCaravanCard*>* pNode = pDeck2->GetCardList();
    while (pNode && pNode->GetItem())
    {
        if (!GetCardList()->IsInList(pNode->GetItem()))
            return true;
        pNode = pNode->GetNext();
    }
    if (memcmp(&data, pDeck2->GetDeckData(), sizeof(CaravanDeckData)))
        return true;
    return false;
}

int __cdecl BSstrcmp(const char*, const char*);

// @fnv 0x0059b740
// pdb: TESCaravanMoney::Compare | TESCaravan.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pComp
bool TESCaravanMoney::Compare(TESForm* apCompare)
{
    TESCaravanMoney* compared = dynamic_cast<TESCaravanMoney*>(apCompare);

    if (!compared)
        return true;

    if (TESForm::Compare(compared))
        return true;

    if (BSstrcmp(anteModels[0].GetModelFilePath(),
                     compared->GetModelTextureSwap(0)->GetModelFilePath()))
        return true;

    if (BSstrcmp(anteModels[1].GetModelFilePath(),
                     compared->GetModelTextureSwap(1)->GetModelFilePath()))
        return true;

    return false;
}
