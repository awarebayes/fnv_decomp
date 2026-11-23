// Distinct original implementations share reconstructed identifiers.
// Keep each matched body under its own address-qualified name.
#define DestroyAndFree DestroyAndFree_fnv_005fc8d0

#include "../../include/BSStringT.h"
// fallout_shared/tesfaction.cpp: faction records, reactions, and rank data.

// Use shared dependencies instead of the legacy definitions pulled in by tesfaction_decls.h.
#define FNV_BASEFORMCOMPONENT_H
#define FNV_TESTEXTURE_H
#define FNV_TESFILE_H
#include "layout_support.h"
#include "../../include/NiFile.h"
#include "../../include/TESFile.h"
#include "../../include/TESForm.h"
#include "../../include/TESTexture.h"
#include "../../include/BGSLoadFormBuffer.h"
#include "../../include/BGSLoadGameBuffer.h"
#include "../../include/FactionSaveData.h"
#include "../../include/TESFaction.h"
#include "tesfaction_decls.h"
#include "../../include/BGSAcousticSpace.h"
#include "../../include/BGSSaveFormBuffer.h"
#include "../../include/SkyObject.h"
#include "../../include/TESReputation.h"
#include "../../include/TESTexture1024.h"
#include "../../include/TESTopicInfo.h"
#include "../../include/NiFormArray.h"
#include "../../include/SL.h"

// Additional entry points are kept here until the shared declarations expose them.
// All object layout comes from TESFaction.
struct FactionAccess : TESFaction
{
    int TESFaction_GetFactionReaction(TESFaction* faction);
    int AddRank();
    void* DestroyAndFree(unsigned int flags);
    unsigned short GetFactionSaveSize(unsigned int flags);
    void SaveLegacyGame(unsigned int flags);
    void LoadLegacyGame(unsigned int flags, unsigned int currentFlags);
    void SetFactionFlag(int flag, bool value);
    void TESFaction_SetFactionReaction(TESForm* pFaction, int reaction);
    void TESFaction_ModFactionReaction(int pFaction, int reaction);
    void ResolveReactions(unsigned int flags, unsigned int currentFlags);
    void ModifyReputation(int count, bool modifyReputation);
    void CopyFaction(TESForm* form);
    void SaveFactionGame(BGSSaveFormBuffer* apSaveGameBuffer);
};

extern "C" void* __cdecl memset(void*, int, size_t);
extern TESDataHandler* g_dataHandler;

// @fnv 0x005fc840
// pdb: TESFaction::ClearData | TESFaction.obj | debug, upstream name already matches, high
void TESFaction::ClearData()
{
    ClearRankDataList();
}

// @fnv 0x005fc800
// pdb: TESFaction::InitializeData | TESFaction.obj | debug, object-file gap fill, medium
void TESFaction::ResetFormData()
{
    iMajorCrime = 0;
    iMinorCrime = 0;
    pReputation = 0;
    memset(&data, 0, sizeof(data));
}

// Reverts the form from the load buffer, then initializes the reaction's load-game state from the same buffer.
// @fnv 0x005fd7d0
// pdb: TESFaction::InitLoadGame | TESFaction.obj | retail, upstream name already matches, high
// pdb params: aiFlags, aiOldFlags
void TESFaction::InitLoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::Revert(apLoadGameBuffer);
    TESReactionForm::InitLoadGame(apLoadGameBuffer);
}

// @fnv 0x005fd800
// pdb: TESFaction::Revert | TESFaction.obj | retail, upstream name already matches, high
// pdb params: apLoadGameBuffer
void TESFaction::Revert(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::Revert(apLoadGameBuffer);
    if (apLoadGameBuffer->GetOldChangeFlags(0x80000000).operator bool())
    {
        iMajorCrime = 0;
        iMinorCrime = 0;
    }
}

// @fnv 0x005fc860
// src: XBOX_DEBUG_PDB 0x005fc860
// pdb: TESFaction::ClearRankDataList | TESFaction.obj | debug, call graph (shared callers), high
// pdb locals: rankData
void TESFaction::ClearRankDataList()
{
    FactionRankPresentation* rankData;
    while (!GetRankDataList()->IsEmpty())
    {
        rankData = GetRankDataList()->GetItem();
        delete rankData;
        GetRankDataList()->RemoveHead();
    }
}

// @fnv 0x005fcf80
// pdb: TESFaction::InitItem | TESFaction.obj | retail, upstream name already matches, high
// pdb locals: itempint
void TESFaction::InitItem()
{
    if (static_cast<const TESForm*>(this)->IsFormInitialized())
        return;

    TESReactionForm::InitItem(this);
    SetInitializationState(true);
    if (pReputation != 0)
    {
        unsigned int formID = 0;
        formID = reinterpret_cast<unsigned int>(pReputation);
        TESForm::RemapFormIDForFile(formID, GetFile(-1));
        pReputation = reinterpret_cast<TESDataHandlerAccess*>(g_dataHandler)->GetReputation(formID);
    }
}

// @fnv 0x005fd730
// pdb: TESFaction::LoadGame | TESFaction.obj | retail, vtable slot, high
// pdb params: aiFlags, aiCurrentFlags
void TESFaction::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::LoadGame(apLoadGameBuffer);
    TESReactionForm::LoadGame(apLoadGameBuffer);
    if (apLoadGameBuffer->GetChangeFlags(2u).operator bool())
        apLoadGameBuffer->ReadTypedData(static_cast<void*>(&data), 4u);
    if (apLoadGameBuffer->GetChangeFlags(0x80000000).operator bool())
    {
        apLoadGameBuffer->ReadTypedData(static_cast<void*>(&iMajorCrime), 4u);
        apLoadGameBuffer->ReadTypedData(static_cast<void*>(&iMinorCrime), 4u);
    }
}

extern unsigned int _data_011c16bc;
struct SB005fc900
{
    char p[8];
    ~SB005fc900();
};
struct TT005fc900
{
    virtual ~TT005fc900();
    char p[0xc];
};
struct FUN_005fc900 : public SB005fc900
{
    SB005fc900 s;
    TT005fc900 t;
    ~FUN_005fc900();
};
// Destructor for FUN_005fc900 that destroys its embedded SB005fc900 and TT005fc900 members.
// @fnv 0x005fc900
// pdb: TESFaction::AddMinorCrime | TESFaction.obj | retail, object-file run extension, low
// pdb params: aiCount, bModRep
FUN_005fc900::~FUN_005fc900()
{}


// @fnv 0x005fd3b0
// src: XBOX_DEBUG_PDB 0x005fd3b0
// pdb: TESFaction::GetFactionReaction | TESFaction.obj | debug, call graph (shared callers), high
// pdb params: pFaction
int FactionAccess::TESFaction_GetFactionReaction(TESFaction* pFaction)
{
    return TESReactionForm::GetReaction(pFaction);
}

// Forwards f and r to the object at this+0x24 via FUN_0048c220, then calls v12(4).
// @fnv 0x005fd430
// pdb: TESFaction::SetFactionFightReaction | TESFaction.obj | retail, upstream name already matches, high
// pdb params: apFaction, aiFightReaction
void TESFaction::SetFactionFightReaction(TESFaction* apFaction, _FightReaction::Type aiFightReaction)
{
    TESReactionForm::SetFightReaction(apFaction, aiFightReaction);
    MarkAsModified(4);
}


// Counts the entries in the rank data list, appends a new rank data node to the tail (or head if empty), and returns the previous count.
// @fnv 0x005fd460
// pdb: TESFaction::AddRankData | TESFaction.obj | debug, object-file run extension, low
// pdb locals: plist, iindex, pdata, plastlist
int FactionAccess::AddRank()
{
    int iElementIndex = 0;
    FactionRankPresentation* pdata = new FactionRankPresentation;
    BSSimpleList<FactionRankPresentation*>* pCurrentElementNext = GetRankDataList();
    BSSimpleList<FactionRankPresentation*>* plastlist = 0;
    while (pCurrentElementNext && !pCurrentElementNext->IsEmpty())
    {
        ++iElementIndex;
        plastlist = pCurrentElementNext;
        pCurrentElementNext = pCurrentElementNext->GetNext();
    }
    if (plastlist)
    {
        plastlist->AddTail(pdata);
    }
    else
    {
        GetRankDataList()->AddHead(pdata);
    }
    return iElementIndex;
}

void __cdecl Deallocate(void*);
namespace p07 {
struct DtorStandIn
{
    void FUN_0083dec0();
};
struct DelStandIn
{
    void* DestroyAndFree(unsigned int flags);
};
// Calls the destructor on this, then frees this when flags has bit 0 set, and returns this.
// @fnv 0x005fc8d0
// pdb: RANK_DATA::`scalar deleting destructor' | TESFaction.obj | debug, object-file run extension, low
void* DelStandIn::DestroyAndFree(unsigned int flags)
{
    ((DtorStandIn*)this)->FUN_0083dec0();
    if (flags & 1)
        Deallocate(this);
    return this;
}
}

// @fnv 0x005fc680
// pdb: TESFaction::TESFaction | TESFaction.obj | retail, upstream name already matches, high
TESFaction::TESFaction()
{
    ResetFormData();
    SetFormType(static_cast<ENUM_FORM_ID>(8));
}

namespace fnv_005fc730 {
extern "C" void* __cdecl Deallocate(void*);
}

// @fnv 0x005fc730
// pdb: TESFaction::`scalar deleting destructor' | TESFaction.obj | retail, vtable slot, high
void* FactionAccess::DestroyAndFree(unsigned int flags)
{
    TESFaction::~TESFaction();
    if (flags & 1)
        fnv_005fc730::Deallocate(this);
    return this;
}

// Returns the sum of the save sizes of the object and its sub-object at offset 0x24, plus 4 when aiFlags has bit 2 set.
// @fnv 0x005fd550
// pdb: TESFaction::GetSaveSize | TESFaction.obj | retail, vtable slot, high
// pdb params: aiFlags
// pdb locals: isize
unsigned short FactionAccess::GetFactionSaveSize(unsigned int aiFlags)
{
    unsigned short isize = 0;
    isize += static_cast<unsigned short>(TESForm::GetSaveSize(aiFlags));
    isize += TESReactionForm::GetSaveSize(aiFlags);
    if (aiFlags & 2)
        isize += 4;
    return isize;
}


struct FUN_005ff520_shell
{
    unsigned int TESIdleForm_GetIndexForChild(unsigned int apChild);
};

// @fnv 0x005ff520
// src: COMMUNITY 0x005ff520
// pdb: TESIdleForm::GetIndexForChild | TESIdleForm.obj | debug, upstream name already matches, high
// pdb params: apChild
// pdb locals: iretval
unsigned int FUN_005ff520_shell::TESIdleForm_GetIndexForChild(unsigned int apChild)
{
    unsigned int index = 0xffffffff;
    if (*reinterpret_cast<NiFormArray**>(reinterpret_cast<char*>(this) + 0x40))
    {
        index = (*reinterpret_cast<NiFormArray**>(reinterpret_cast<char*>(this) + 0x40))
                    ->GetIndexOfForm(reinterpret_cast<TESForm*>(apChild));
    }
    return index;
}

template <int N> struct EndianSizeTag
{
};
void __cdecl BSEndian(int*, EndianSizeTag<2>);
struct Class005a6b10
{
    int m0;
    int m1;
    int m2;
    void TESRegionSound_Endian();
};
// Byte-swaps the three int fields m0, m1 and m2 via BSEndian.
// @fnv 0x005a6b10
// src: XBOX_PDB 0x005a6b10
// pdb: TESRegionSound::Endian | TESRegionDataSound.obj | debug, upstream name already matches, high
void Class005a6b10::TESRegionSound_Endian()
{
    BSEndian(&m0, EndianSizeTag<2>());
    BSEndian(&m1, EndianSizeTag<2>());
    BSEndian(&m2, EndianSizeTag<2>());
}

// Legacy form serialization entry points missing from the shared TESForm API.
struct FormLegacyAccess : TESForm
{
    void SaveLegacyGame(unsigned int flags);
    void LoadLegacyGame(unsigned int aiFlags, unsigned int aiCurrentFlags);
    void WriteSaveData(unsigned int data, int size);
    void ReadLoadData(void* data, unsigned int size);
};
struct ReactionLegacyAccess : TESReactionForm
{
    void WriteReactionList(unsigned int aiFlags);
    void LoadReactions(unsigned int aiFlags, unsigned int aiCurrentFlags);
};
// @fnv 0x005fd5c0
// pdb: TESFaction::SaveGame | TESFaction.obj | retail, vtable slot, high
// pdb params: aiFlags
void FactionAccess::SaveLegacyGame(unsigned int aiFlags)
{
    ((FormLegacyAccess*)this)->SaveLegacyGame(aiFlags);
    ((ReactionLegacyAccess*)((char*)this + 0x24))->WriteReactionList(aiFlags);
    if (aiFlags & 2)
        ((FormLegacyAccess*)this)->WriteSaveData((unsigned int)((char*)this + 0x34), 4);
}

// @fnv 0x005fd610
// pdb: TESFaction::LoadGame | TESFaction.obj | retail, vtable slot, high
// pdb params: aiFlags, aiCurrentFlags
void FactionAccess::LoadLegacyGame(unsigned int aiFlags, unsigned int aiCurrentFlags)
{
    ((FormLegacyAccess*)this)->LoadLegacyGame(aiFlags, aiCurrentFlags);
    ((ReactionLegacyAccess*)((char*)this + 0x24))->LoadReactions(aiFlags, aiCurrentFlags);
    if (aiFlags & 2)
        ((FormLegacyAccess*)this)->ReadLoadData((void*)((char*)this + 0x34), 4);
}

// Sets or clears the given bit in m_flags based on value, then calls v18(2).
// @fnv 0x005fc970
// pdb: TESFaction::AddMajorCrime | TESFaction.obj | retail, object-file run extension, low
// pdb note: debug build links it to TESFaction::SetFlag
// pdb params: aiCount, bModRep
void FactionAccess::SetFactionFlag(int flag, bool value)
{
    value ? data.iFlags |= flag : data.iFlags &= ~flag;
    MarkAsModified(2);
}

// @fnv 0x005fd3d0
// src: XBOX_RETAIL_PDB 0x005fd3d0
// pdb: TESFaction::SetFactionReaction | TESFaction.obj | retail, object-file run extension, high
// pdb params: pFaction, reaction
void FactionAccess::TESFaction_SetFactionReaction(struct TESForm* pFaction, int reaction)
{
    TESReactionForm::SetReactionValue(pFaction, reaction);
    MarkAsModified(4);
}

// Calls the helper's FUN_0048c0b0 with a and b, then invokes virtual v18 with 4.
// @fnv 0x005fd400
// src: XBOX_RETAIL_PDB 0x005fd400
// pdb: TESFaction::ModFactionReaction | TESFaction.obj | retail, object-file run extension, high
// pdb params: pFaction, reaction
void FactionAccess::TESFaction_ModFactionReaction(int pFaction, int reaction)
{
    TESReactionForm::ModReaction(reinterpret_cast<TESForm*>(pFaction), reaction);
    MarkAsModified(4);
}

struct ReactionResolver
{
    void ResolveGroupReactionForms(unsigned int, unsigned int);
};
// @fnv 0x005fd660
// pdb: TESFaction::InitLoadGame | TESFaction.obj | debug, object-file gap fill, medium
// pdb params: aiFlags, aiOldFlags
void FactionAccess::ResolveReactions(unsigned int auiArg1, unsigned int auiArg2)
{
    reinterpret_cast<SkyObject*>(this)->Update(auiArg1, auiArg2);
    ((ReactionResolver*)((char*)this + 0x24))->ResolveGroupReactionForms(auiArg1, auiArg2);
}

struct Unk38
{
    void TESReputation_CommitCrime(int);
};
// @fnv 0x005fda00
// pdb: TESFaction::AddMinorCrime | TESFaction.obj | debug, object-file run extension, low
// pdb params: aiCount, bModRep
void FactionAccess::ModifyReputation(int aiCount, bool bModRep)
{
    iMinorCrime += aiCount;
    if (pReputation != 0 && bModRep)
        reinterpret_cast<Unk38*>(pReputation)->TESReputation_CommitCrime(0);
    MarkAsModified(0x80000000);
}

void* __memcpy(void*, const void*, unsigned int);

struct Flt
{
    operator unsigned int();
};


// Copies faction data from apCopy: base copy, rotation, noon sound, then deep-copies each rank entry into this faction's rank list.
// @fnv 0x005fd210
// pdb: TESFaction::Copy | TESFaction.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pCopyRankList, faction, pRankList, pdata
void FactionAccess::CopyFaction(TESForm* apCopy)
{
    BSSimpleList<FactionRankPresentation*>* pDestRankList;
    {
        TESFaction* faction = dynamic_cast<TESFaction*>(apCopy);
        if (!faction)
            return;
        TESForm::CopyFrom(apCopy);
        __memcpy((char*)this + 0x34, &((NiAVObject*)faction)->GetRotate(), 4);
        ((BGSAcousticSpace*)this)->SetNoonSound(reinterpret_cast<TESSound*>(((TESTopicInfo*)faction)->GetConversationData()));
        ClearRankDataList();
        pDestRankList = GetRankDataList();
        {
            BSSimpleList<FactionRankPresentation*>* pSourceRankList = faction->GetRankDataList();
            while (pSourceRankList && !pSourceRankList->IsEmpty())
            {
                FactionRankPresentation* pdata = new FactionRankPresentation;
                ((::BSStringT<char>*)&pdata->m_kMaleRankName)
                    ->Set((const char*)(unsigned int)*(Flt*)&pSourceRankList->GetItem()->m_kMaleRankName, 0);
                ((::BSStringT<char>*)&pdata->m_kFemaleRankName)
                    ->Set((const char*)(unsigned int)*(Flt*)&pSourceRankList->GetItem()->m_kFemaleRankName, 0);
                pdata->m_kRankTexture.CopyFromBase(&pSourceRankList->GetItem()->m_kRankTexture);
                pDestRankList->AddTail(pdata);
                if (pDestRankList->GetNext())
                    pDestRankList = (BSSimpleList<FactionRankPresentation*>*)pDestRankList->GetNext();
                pSourceRankList = (BSSimpleList<FactionRankPresentation*>*)pSourceRankList->GetNext();
            }
        }
    }
}

// @fnv 0x005fd690
// pdb: TESFaction::SaveGame | TESFaction.obj | retail, vtable slot, high
// pdb params: aiFlags
void FactionAccess::SaveFactionGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    ((BGSSaveFormBuffer*)this)->SaveIfChanged(apSaveGameBuffer);
    ((BGSSaveFormBuffer*)((char*)this + 0x24))->FUN_0048c9d0(apSaveGameBuffer);
    if (apSaveGameBuffer->GetChangeFlags(2u).operator bool())
        apSaveGameBuffer->SaveData((char*)this + 0x34, 4, 0);
    if (apSaveGameBuffer->GetChangeFlags(0x80000000).operator bool())
    {
        apSaveGameBuffer->SaveData((char*)this + 0x44, 4, 0);
        apSaveGameBuffer->SaveData((char*)this + 0x48, 4, 0);
    }
}

namespace fnv_005fd850 {
int __fastcall TESDataHandler_GetFactions(int);
int __fastcall FNV_GetFormType(TESForm*);
void FNV_Warning(const char*, ...);
struct Node_006815c0
{
    void** _ref_006815c0();
};
struct TgtOps
{
    void* GetPosition();
};
struct _FactionSub
{
    int FUN_0048c1b0(TESFaction*);
};
struct Reaction_5fd850
{
    TESFaction* pFaction;
    int iMod;
    int eReaction;
};
// Iterates all factions and their reactions; for friend/ally reactions not reciprocated, warns and forces the other faction to be a friend.
// @fnv 0x005fd850
// pdb: TESFaction::CheckFactionReactions | TESFaction.obj | debug, object-file run extension, low
// pdb locals: pfactionList, pfaction, preactionList, preaction, pnewFaction, eReact
void ValidateFactionReactions()
{
    BSSimpleList<TESFaction*>* pIter = (BSSimpleList<TESFaction*>*)TESDataHandler_GetFactions((int)g_dataHandler);
    while (pIter)
    {
        TESFaction* pFaction = (TESFaction*)*((Node_006815c0*)pIter)->_ref_006815c0();
        if (pFaction)
        {
            BSSimpleList<Reaction_5fd850*>* pRIter =
                (BSSimpleList<Reaction_5fd850*>*)((TgtOps*)((char*)pFaction + 0x24))->GetPosition();
            while (pRIter)
            {
                Reaction_5fd850* pReaction = (Reaction_5fd850*)*((Node_006815c0*)pRIter)->_ref_006815c0();
                if (pReaction && pReaction->pFaction && FNV_GetFormType((TESForm*)pReaction->pFaction) == 8 &&
                    (pReaction->eReaction == 3 || pReaction->eReaction == 2))
                {
                    TESFaction* pOther = pReaction->pFaction;
                    int iFactionReaction = ((_FactionSub*)((char*)pOther + 0x24))->FUN_0048c1b0(pFaction);
                    if (iFactionReaction != 2 && iFactionReaction != 3)
                    {
                        FNV_Warning(
                            "DEFAULT: (Faction Reaction Error) Faction '%s' (%08X) is a Friend or Ally of Faction '%s' (%08X), but Faction '%s' (%08X) is not a Friend or Ally of Faction '%s' (%08X). Faction '%s'(%08X) will be forced to be a Friend of Faction '%s' (%08X) ",
                            pFaction->GetName(), pFaction->GetFormID(), pOther->GetName(), pOther->GetFormID(),
                            pOther->GetName(), pOther->GetFormID(), pFaction->GetName(), pFaction->GetFormID(),
                            pOther->GetName(), pFaction->GetName(), pFaction->GetName(), pFaction->GetFormID());
                        pOther->SetFactionFightReaction(pFaction, _FightReaction::FRIEND);
                    }
                }
                pRIter = pRIter->GetNext();
            }
        }
        pIter = pIter->GetNext();
    }
}
}  // namespace fnv_005fd850

// @fnv 0x005fc760
// pdb: TESFaction::~TESFaction | TESFaction.obj | retail, upstream name already matches, high
TESFaction::~TESFaction()
{
    TESFaction::ClearData();
}

struct RankTitle : BSStringT<char>
{
    unsigned int GetLength();
    operator const char*() const;
};
struct RankInsignia : TESTexture
{
    unsigned int GetModelLength();
    const char* GetTextureResourceName();
};
struct FullNameSaver : TESFullName
{
    void SaveName();
};
struct ReactionSaver : TESReactionForm
{
    void SaveReactions();
};
bool IsSaveEndianSwap();
void SaveChunkValue(unsigned int chunk, unsigned int value);
void SaveChunkData(unsigned int aeChunkID, const void* apData, unsigned int aiSize);

struct TESFactionSaveShell : TESFaction
{
    void BeginSaveForm();
    void EndSaveForm();
    void SaveChunk(void* data, int aiSize);
    void SaveFactionData();
};

// @fnv 0x005fc9d0
// pdb: TESFaction::Save | TESFaction.obj | retail, vtable slot, high
// pdb locals: iRank, pRankList, pdata, itempint
void TESFactionSaveShell::SaveFactionData()
{
    BeginSaveForm();
    reinterpret_cast<FullNameSaver*>(reinterpret_cast<char*>(this) + 0x18)->SaveName();
    reinterpret_cast<ReactionSaver*>(reinterpret_cast<char*>(this) + 0x24)->SaveReactions();
    if (IsSaveEndianSwap())
        data.Endian();
    SaveChunk(&data, 4);
    if (IsSaveEndianSwap())
        data.Endian();
    int iRankIndex = 0;
    BSSimpleList<FactionRankPresentation*>* pNode = GetRankDataList();
    while (pNode && !pNode->IsEmpty())
    {
        FactionRankPresentation* pRank = pNode->GetItem();
        SaveChunkValue(0x4d414e52, iRankIndex);
        if (static_cast<RankTitle&>(pRank->m_kMaleRankName).GetLength())
            SaveChunkData(0x4d414e4d, (const char*)static_cast<RankTitle&>(pRank->m_kMaleRankName),
                          static_cast<RankTitle&>(pRank->m_kMaleRankName).GetLength() + 1);
        if (static_cast<RankTitle&>(pRank->m_kFemaleRankName).GetLength())
            SaveChunkData(0x4d414e46, (const char*)static_cast<RankTitle&>(pRank->m_kFemaleRankName),
                          static_cast<RankTitle&>(pRank->m_kFemaleRankName).GetLength() + 1);
        if (static_cast<RankInsignia&>(pRank->m_kRankTexture).GetModelLength())
            SaveChunkData(0x4d414e49, static_cast<RankInsignia&>(pRank->m_kRankTexture).GetTextureResourceName(),
                          static_cast<RankInsignia&>(pRank->m_kRankTexture).GetModelLength() + 1);
        iRankIndex++;
        pNode = pNode->GetNext();
    }
    if (pReputation)
    {
        unsigned int uiRepID = pReputation->GetSaveFormID();
        SaveChunkValue(0x31494d57, uiRepID);
    }
    EndSaveForm();
}


class FactionLoader : public TESFaction
{
  public:
    bool LoadFactionRecord(TESFile* apFile);
};

// Reads a faction record's chunks (editor ID, bounds, full name, reactions, flags, ranks, reputation) from file until no chunks remain and returns true.
// @fnv 0x005fcb70
// pdb: TESFaction::Load | TESFaction.obj | retail, vtable slot, high
// pdb params: apFile
// pdb locals: pRankData, pMessage, pdata, dataOld, pTemp, itempint
bool FactionLoader::LoadFactionRecord(TESFile* apFile)
{
    FactionRankPresentation* rankData = NULL;
    LoadRecordHeader(apFile);
    do
    {
        switch ((int)apFile->GetChunkID())
        {
        case CHUNK_ID_EDID_ID: {
            char* pEditorId = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(pEditorId, 0x200);
            SetEditorID_AtRuntime(pEditorId);
            break;
        }
        case CHUNK_ID_OBND_ID:
            readOBNDSubRecord(apFile);
            break;
        case CHUNK_ID_FULL_ID:
            TESFullNameLoader::LoadFullNameChunk(this, apFile);
            break;
        case CHUNK_ID_XNAM_ID: {
            ReactionInfo* reaction = new ReactionInfo;
            apFile->GetChunkData(reaction, 0xC);
            if (apFile->IsEndianConversionRequired())
            {
                reaction->Endian();
            }
            ((TESReactionFormLoader*)((char*)this + 0x24))->GetReactionList()->AddTail(reaction);
            break;
        }
        case 0x41544144: {  // DATA
            if (reinterpret_cast<TESFileFormVersion*>(apFile)->GetFormVersion() < 9)
            {
                char byteData;
                TESForm::ReadTypedData(apFile, &byteData, 1);
                data.iFlags = byteData;
            }
            else
            {
                TESForm::ReadTypedData(apFile, &data, 4);
                if (apFile->IsEndianConversionRequired())
                {
                    data.Endian();
                }
            }
            break;
            break;
        }
        case 0x4D414E52: {  // RNAM
            rankData = ::new FactionRankPresentation;
            GetRankDataList()->AddTail(rankData);
            break;
        }
        case 0x4D414E4D: {  // MNAM
            if (rankData)
            {
                char* pMaleRank = (char*)_alloca(apFile->GetChunkSize());
                apFile->GetChunkData(pMaleRank, 0);
                rankData->m_kMaleRankName.Set(pMaleRank, 0);
            }
            break;
        }
        case CHUNK_ID_FNAM_ID: {
            if (rankData)
            {
                char* pFemaleRank = (char*)_alloca(apFile->GetChunkSize());
                apFile->GetChunkData(pFemaleRank, 0);
                rankData->m_kFemaleRankName.Set(pFemaleRank, 0);
            }
            break;
        }
        case 0x4D414E49: {  // INAM
            if (rankData)
            {
                char* pInsignia = (char*)_alloca(apFile->GetChunkSize());
                apFile->GetChunkData(pInsignia, 0);
                reinterpret_cast<TESTexture1024*>(&rankData->m_kRankTexture)->SetTexture(pInsignia);
            }
            break;
        }
        case 0x31494D57: {  // WMI1
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            if (formID)
            {
                pReputation = (TESReputation*)formID;
            }
            break;
        }
        default:
            break;
        }
    } while (apFile->NextChunk());
    return true;
}

#include <string.h>
struct FactionCompareAccess : TESFaction
{
    bool TESForm_CompareAllComponents(TESForm*);
    const FactionSaveData* NiAVObject_GetRotate_006a9540();
    TESReputation* GetField38();
    BSSimpleList<FactionRankPresentation*>* ComputeValue_005a8080();
    bool IsDifferentFrom(TESForm* apCompare);
};

struct RankStringAccess : BSStringT<char>
{
    unsigned int BSStringT_char_Length() const;
    const char* Modelloader_GetReferencedThis() const;
    int FUN_00408a80(const char*, bool) const;
};

// Returns true if this faction differs from other in save data, reputation, or rank presentation lists; false if identical.
// @fnv 0x005fd000
// pdb: TESFaction::Compare | TESFaction.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pComp, pCompRankList, pRankList, pdata, pcompdata
bool FactionCompareAccess::IsDifferentFrom(TESForm* apCompare)
{
    FactionCompareAccess* faction2 = static_cast<FactionCompareAccess*>(dynamic_cast<TESFaction*>(apCompare));
    if (!faction2)
        return true;
    if (TESForm_CompareAllComponents(faction2))
        return true;
    if (memcmp(&data, faction2->NiAVObject_GetRotate_006a9540(), 4))
        return true;
    if (pReputation != faction2->GetField38())
        return true;

    BSSimpleList<FactionRankPresentation*>* node1 = ComputeValue_005a8080();
    BSSimpleList<FactionRankPresentation*>* node2 = faction2->ComputeValue_005a8080();
    while (node1 && !node1->IsEmpty())
    {
        if (!node2 || !node2->GetItem())
            return true;
        FactionRankPresentation* current2 = node1->GetItem();
        FactionRankPresentation* current = node2->GetItem();
        if (static_cast<RankStringAccess&>(current2->m_kMaleRankName).BSStringT_char_Length() ||
            static_cast<RankStringAccess&>(current->m_kMaleRankName).BSStringT_char_Length())
        {
            if (static_cast<RankStringAccess&>(current2->m_kMaleRankName).BSStringT_char_Length() !=
                static_cast<RankStringAccess&>(current->m_kMaleRankName).BSStringT_char_Length())
                return true;
            else if (static_cast<RankStringAccess&>(current2->m_kMaleRankName)
                         .FUN_00408a80(static_cast<RankStringAccess&>(current->m_kMaleRankName).Modelloader_GetReferencedThis(), false))
                return true;
        }
        if (static_cast<RankStringAccess&>(current2->m_kFemaleRankName).BSStringT_char_Length() ||
            static_cast<RankStringAccess&>(current->m_kFemaleRankName).BSStringT_char_Length())
        {
            if (static_cast<RankStringAccess&>(current2->m_kFemaleRankName).BSStringT_char_Length() !=
                static_cast<RankStringAccess&>(current->m_kFemaleRankName).BSStringT_char_Length())
                return true;
            else if (static_cast<RankStringAccess&>(current2->m_kFemaleRankName)
                         .FUN_00408a80(static_cast<RankStringAccess&>(current->m_kFemaleRankName).Modelloader_GetReferencedThis(),
                                       false))
                return true;
        }
        if (current2->m_kRankTexture.CompareWithBase(&current->m_kRankTexture))
            return true;
        node1 = node1->GetNext();
        node2 = node2->GetNext();
    }
    if (node2 && !node2->IsEmpty())
        return true;
    return false;
}
