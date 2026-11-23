// fallout_shared/bgsquestobjective.cpp: BGSQuestObjective and objective targets (15 functions)

#include "../../include/BGSQuestObjective.h"
#include "../../include/PlayerCharacter.h"
#include "../../include/StrMem.h"
#include "../../include/TESFile.h"
#include "../../include/TESQuest.h"

class RegionObjectEntry
{
  public:
    void InitItem(TESForm*);
};
class BGSPerkEntry;
extern const char g_emptyTopicInfoPrompt[];

// Operations not yet declared in the shared objective header. This view adds
// methods only; all objective data and lifetime use the shared definition.
struct QuestObjectiveLocalMethods : BGSQuestObjective
{
    void Clear();
    void BGSQuestObjective_Save();
    unsigned int GetDisplayTextLength();
    void BGSQuestObjective_SetDisplayText(const char* apDisplayText);
    void ClearTargets();
};

// If not yet initialized, stores quest and calls InitItem on each entry of the target list, then marks the objective initialized.
// @fnv 0x005ec210
// pdb: BGSQuestObjective::InitItem | BGSQuestObjective.obj | retail, upstream name already matches, high
// pdb params: apUser
// pdb locals: pTargetList
void BGSQuestObjective::InitItem(TESQuest* apUser)
{
    if (bInitialized)
        return;
    m_pOwnerQuest = apUser;
    BSSimpleList<RegionObjectEntry*>* targets = GetTargetList();
    while (targets && !reinterpret_cast<BSSimpleList<const char*>*>(targets)->IsEmpty())
    {
        reinterpret_cast<RegionObjectEntry*>(reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(targets)->GetItem())
            ->InitItem(reinterpret_cast<TESForm*>(apUser));
        targets = reinterpret_cast<BSSimpleList<RegionObjectEntry*>*>(
            reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(targets)->GetNext());
    }
    bInitialized = true;
}

// @fnv 0x005ec1b0
// pdb: BGSQuestObjective::InitializeData | BGSQuestObjective.obj | debug, object-file gap fill, medium
void BGSQuestObjective::Initialize()
{
    iIndex = 0;
    cDisplayText = g_emptyTopicInfoPrompt;
    bInitialized = false;
    eState = QOS_DORMANT;
    m_pOwnerQuest = 0;
}

// @fnv 0x005ec090
// pdb: BGSQuestObjective::BGSQuestObjective | BGSQuestObjective.obj | retail, upstream name already matches, high
BGSQuestObjective::BGSQuestObjective()
{
    Initialize();
}

// Returns the string stored at offset 8 of this objective, or g_emptyTopicInfoPrompt when that string is empty.
// @fnv 0x005ec330
// pdb: BGSQuestObjective::GetDisplayText | BGSQuestObjective.obj | debug, upstream name already matches, high
const char* BGSQuestObjective::GetDisplayText()
{
    return (reinterpret_cast<const StrMem*>(&cDisplayText)->Modelloader_GetReferencedThis() ? reinterpret_cast<const StrMem*>(&cDisplayText)->Modelloader_GetReferencedThis()
                                                          : g_emptyTopicInfoPrompt);
}

extern "C" void* __cdecl _alloca(size_t);
// Reads chunks from apFile: stores the QOBJ chunk's int in iIndex, and on the NNAM chunk passes its data (or null) to SetDisplayText, then returns.
// @fnv 0x005ec370
// pdb: BGSQuestObjective::Load | BGSQuestObjective.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, pMessage
void BGSQuestObjective::Load(TESFile* apFile)
{
    int chunkid;
    while ((chunkid = apFile->ReadCurrentForm()) != 0)
    {
        switch (chunkid)
        {
        case 0x4a424f51:
            apFile->GetChunkData(&iIndex);
            break;
        case 0x4d414e4e:
            if (apFile->GetChunkSize())
            {
                char* pMessage = (char*)_alloca(apFile->GetChunkSize());
                apFile->GetChunkData(pMessage, 0);
                static_cast<QuestObjectiveLocalMethods*>(this)->BGSQuestObjective_SetDisplayText(pMessage);
            }
            else
            {
                static_cast<QuestObjectiveLocalMethods*>(this)->BGSQuestObjective_SetDisplayText(0);
            }
            return;
        }
        if (!apFile->NextChunk())
            break;
    }
}

struct ListOps
{
    RegionObjectEntry** _ref_006815c0();
    ListOps* _ref_00726070();
};
struct TgtOps
{
    void* GetReference(bool);
    bool IsFlagSet();
    void* GetSourceFiles();
    void* GetPosition();
};
struct CondOps
{
    bool ContainsTrait(void*, int);
};
// Objective update methods are not declared by the shared player header yet.
struct PlayerObjectiveUpdates : PlayerCharacter
{
    void AddObjectiveTarget(void*, void*, int);
    void UpdateObjective(BGSQuestObjective*);
};
struct OutList
{
    void AddTail(RegionObjectEntry**);
};
extern PlayerCharacter* g_thePlayer;

// If the objective's object reference is 1, walks the target list and appends targets whose condition passes to apListToFill, calling the player op on each.
// @fnv 0x005ec500
// pdb: BGSQuestObjective::UpdateCurrentTargetList | BGSQuestObjective.obj | retail, call graph (shared callers), high
// pdb params: apListToFill
// pdb locals: pTarget, pMasterList
void BGSQuestObjective::AddMatchingTargetsToList(void* apListToFill)
{
    if (GetState() != QOS_DISPLAYED)
        return;
    ListOps* pMasterList = reinterpret_cast<ListOps*>(GetTargetList());
    RegionObjectEntry* pTarget = 0;
    while (pMasterList && *pMasterList->_ref_006815c0())
    {
        pTarget = *pMasterList->_ref_006815c0();
        pMasterList = pMasterList->_ref_00726070();
        if (((CondOps*)((TgtOps*)pTarget)->GetPosition())->ContainsTrait(((TgtOps*)pTarget)->GetReference(0), 0))
        {
            static_cast<PlayerObjectiveUpdates*>(g_thePlayer)->AddObjectiveTarget(((TgtOps*)pTarget)->GetReference(1), ((TgtOps*)pTarget)->GetSourceFiles(),
                                      ((TgtOps*)pTarget)->IsFlagSet());
            ((OutList*)apListToFill)->AddTail(&pTarget);
        }
    }
}

// @fnv 0x005ec310
// pdb: BGSQuestObjective::GetDisplayTextLength | BGSQuestObjective.obj | debug, object-file gap fill, medium
unsigned int QuestObjectiveLocalMethods::GetDisplayTextLength()
{
    return reinterpret_cast<StrMem*>(&cDisplayText)->BSStringT_char_Length();
}

struct StandIn
{
    void FUN_0060ffe0();
    void* FUN_005ec4d0(unsigned int);
};
void __cdecl _Deallocate(void*);
// Calls FUN_0060ffe0, then deallocates this when flags has bit 0 set, and returns this.
// @fnv 0x005ec4d0
// pdb: TESQuestTarget::`scalar deleting destructor' | BGSQuestObjective.obj | debug, object-file run extension, low
void* StandIn::FUN_005ec4d0(unsigned int flags)
{
    FUN_0060ffe0();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

// @fnv 0x005ec1f0
// pdb: BGSQuestObjective::ClearData | BGSQuestObjective.obj | debug, call graph, medium
void QuestObjectiveLocalMethods::Clear()
{
    ClearTargets();
}

// @fnv 0x005ec440
// src: XBOX_DEBUG_PDB 0x005ec440
// pdb: BGSQuestObjective::SetDisplayText | BGSQuestObjective.obj | debug, call graph, high
// pdb params: apDisplayText
void QuestObjectiveLocalMethods::BGSQuestObjective_SetDisplayText(const char* apDisplayText)
{
    reinterpret_cast<StrMem*>(&cDisplayText)->Set(apDisplayText, 0);
}

namespace fnv_005ec280 {
struct FUN_00485910_shell
{
    static void __cdecl TESForm_AddChunk_00485910(int, void*);
};
struct FUN_004856f0_shell
{
    static void __cdecl TESForm_AddChunkArray(int, const char*, unsigned int);
};
struct Date
{
    ~Date();
};
}  // namespace fnv_005ec280

// @fnv 0x005ec280
// src: XBOX_RETAIL_PDB 0x005ec280
// pdb: BGSQuestObjective::Save | BGSQuestObjective.obj | retail, object-file run extension, high
// pdb locals: pTargetList
void QuestObjectiveLocalMethods::BGSQuestObjective_Save()
{
    fnv_005ec280::FUN_00485910_shell::TESForm_AddChunk_00485910(0x4a424f51, ((BSSimpleList<RegionObjectEntry*>*)this)->GetNext());
    fnv_005ec280::FUN_004856f0_shell::TESForm_AddChunkArray(0x4d414e4e, GetDisplayText(),
                                     GetDisplayTextLength() + 1);
    BSSimpleList<RegionObjectEntry*>* list = GetTargetList();
    while (list && !list->IsEmpty())
    {
        ((fnv_005ec280::Date*)list->GetItem())->fnv_005ec280::Date::~Date();
        list = list->GetNext();
    }
}

namespace fnv_005ec5d0 {
// Objective notification flags are not declared by the shared quest header yet.
struct QuestObjectiveFlags : TESQuest
{
    bool TESQuest_GetDisplayedInHUD();
    void TESQuest_SetDisplayedInHUD(bool);
};

extern PlayerCharacter* _g_thePlayer;

void __cdecl HUDMainMenu_ShowQuestName(void*);
void __cdecl HUDMainMenu_ShowQuestObjective(char const*, int, int);

}  // namespace fnv_005ec5d0

// @fnv 0x005ec5d0
// pdb: BGSQuestObjective::SetState | BGSQuestObjective.obj | retail, upstream name already matches, high
// pdb params: aeNewState
// pdb locals: powner
void BGSQuestObjective::SetState(QUEST_OBJECTIVE_STATE aeNewState)
{
    using namespace fnv_005ec5d0;
    TESQuest* powner = TESMain_GetThread();

    powner->MarkAsModified(0x20000000);

    if (aeNewState == QOS_DISPLAYED || eState == QOS_DORMANT)
    {
        if (aeNewState == QOS_DISPLAYED)
        {
            if (!powner->GetCompleted())
            {
                powner->SetEnabled(true);
                if (!_g_thePlayer->PlayerCharacter_GetActiveQuest())
                {
                    _g_thePlayer->SetActiveQuest(powner);
                }
            }

            HUDMainMenu_ShowQuestObjective(GetDisplayText(), 0, 0);

            if (!static_cast<QuestObjectiveFlags*>(powner)->TESQuest_GetDisplayedInHUD())
            {
                HUDMainMenu_ShowQuestName(powner);
                static_cast<QuestObjectiveFlags*>(powner)->TESQuest_SetDisplayedInHUD(true);
            }
        }

        static_cast<PlayerObjectiveUpdates*>(_g_thePlayer)->UpdateObjective(this);
    }

    if (aeNewState == QOS_COMPLETED_DISPLAYED && eState == QOS_DISPLAYED && !powner->GetCompleted())
    {
        HUDMainMenu_ShowQuestObjective(GetDisplayText(), 1, 0);
    }

    eState = aeNewState;
}

// @fnv 0x005ec140
// pdb: BGSQuestObjective::~BGSQuestObjective | BGSQuestObjective.obj | debug, upstream name already matches, high
BGSQuestObjective::~BGSQuestObjective()
{
    static_cast<QuestObjectiveLocalMethods*>(this)->Clear();
}

struct FUN_005ec460_List
{
    bool FUN_008256d0();
    StandIn** FUN_006815c0();
    void FUN_0063f7b0();
};
// @fnv 0x005ec460
// pdb: BGSQuestObjective::ClearTargetList | BGSQuestObjective.obj | debug, object-file run extension, low
// pdb locals: pList
void QuestObjectiveLocalMethods::ClearTargets()
{
    FUN_005ec460_List* list = reinterpret_cast<FUN_005ec460_List*>(GetTargetList());
    while (list && !list->FUN_008256d0())
    {
        StandIn* target = *list->FUN_006815c0();
        StandIn* deletionTarget = target;
        deletionTarget ? deletionTarget->FUN_005ec4d0(1) : 0;
        list->FUN_0063f7b0();
    }
}
