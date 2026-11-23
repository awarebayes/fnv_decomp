#include "../../../../include/SettingT.h"
#include "../../../../include/CombatProcedureSearch.h"
#include "../../../../include/BGSSaveFormBuffer.h"
#include "../../../../include/BGSLoadFormBuffer.h"
#include "../../../../include/NavMesh.h"
#include "../../../../include/Actor.h"
#include "../../../../include/CombatProcedure.h"
#include "../../../../include/EffectSetting.h"
#include "../../../../include/NavMeshPtr.h"
#include "../../../../include/PathingLocation.h"
#include "../../../../include/PathingRequestLOS.h"
#include "../../../../include/TESObjectREFR.h"
#include "../../../../include/SETTING_VALUE.h"
#include "../../../../include/TESForm.h"
#include "../../../../include/TimeoutTimer.h"
#include "../../../../include/NiPoint3.h"

// Only an opaque reference is needed; the primary template cannot dereference void.
template <> class NiPointer<void>;

// src/fallout/ai/combat/combatproceduresearch.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout/ai/combat/combatproceduresearch.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

// @fnv 0x009d8c00
int CombatProcedureSearch::GetType()
{
    return SEARCH;
}


// Calls FUN_009d8c40, frees this with operator delete when flags has bit 0 set, and returns this.
// @fnv 0x009d8c10
// pdb: CombatProcedureSearch::`scalar deleting destructor' | CombatProcedureSearch.obj | debug, vtable slot, medium
void* CombatProcedureSearch::Destroy(unsigned int flags)
{
    this->CombatProcedureSearch::~CombatProcedureSearch();
    if (flags & 1)
        operator delete(this);
    return this;
}

struct FUN_009d8d00_shell
{
    void SetFieldEC(int param);
};

// @fnv 0x009d8d00
void FUN_009d8d00_shell::SetFieldEC(int aiValue)
{
    *(int*)((char*)this + 0xec) = aiValue;
}

// Clears the stored reference when it equals apRef.
// @fnv 0x009d9fa0
void CombatProcedureSearch::ClearRefIfMatches(TESObjectREFR* apRef, TESBoundObject* apObject, int aiCount,
                                           bool abActivatedScriptOnly)
{
    if (pSearchDoor == apRef)
        pSearchDoor = 0;
}

// @fnv 0x009d8c40
// pdb: CombatProcedureSearch::~CombatProcedureSearch | CombatProcedureSearch.obj | debug, upstream name already matches, high
CombatProcedureSearch::~CombatProcedureSearch()
{
}

struct AvoidSphere
{
    float GetRadius() const;
};
struct Flt
{
    operator unsigned int() const;
};
struct FUN_009da570_shell
{
    float GetAvoidRadius() const;
};
// Returns the radius of the AvoidSphere referenced at offset 0xd4, or -1.0f when that pointer is null.
// @fnv 0x009da570
// pdb: CombatGroup::GetSearchPercent | CombatProcedureSearch.obj | debug, object-file run extension, low
float FUN_009da570_shell::GetAvoidRadius() const
{
    return reinterpret_cast<Flt*>(reinterpret_cast<char*>(const_cast<FUN_009da570_shell*>(this)) + 0xd4)
                   ->
                   operator unsigned int()
               ? reinterpret_cast<AvoidSphere*>(
                     reinterpret_cast<Flt*>(reinterpret_cast<char*>(const_cast<FUN_009da570_shell*>(this)) + 0xd4)
                         ->
                         operator unsigned int())
                     ->GetRadius()
               : -1.0f;
}

// Saves the combat procedure search state to the buffer: base data, a 4-byte field at 0x10, sub-objects at 0x2c/0x1c/0x14/0x24, and a pointer at 0x3c.
// @fnv 0x009da5c0
// pdb: CombatProcedureSearch::SaveGame | CombatProcedureSearch.obj | retail, upstream name already matches, high
// pdb params: apSaveGameBuffer
void CombatProcedureSearch::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    CombatProcedure::SaveGame(apSaveGameBuffer);
    apSaveGameBuffer->SaveData(&eState, 4, 0);
    InvestigateLocation.BGSWorldLocation_SaveGame(apSaveGameBuffer);
    LookingTimer.SaveGame(apSaveGameBuffer);
    CheckDestinationTimer.SaveGame(apSaveGameBuffer);
    CheckSearchDoorTimer.SaveGame(apSaveGameBuffer);
    apSaveGameBuffer->SerializeFormData(pSearchDoor, 0);
}

// Loads the base CombatProcedure state, then reads a 4-byte field and loads four sub-objects and a bool from the buffer.
// @fnv 0x009da640
// pdb: CombatProcedureSearch::LoadGame | CombatProcedureSearch.obj | retail, upstream name already matches, high
// pdb params: apLoadGameBuffer
void CombatProcedureSearch::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    CombatProcedure::LoadGame(apLoadGameBuffer);
    apLoadGameBuffer->ReadTypedData(static_cast<void*>(&eState), 4u);
    InvestigateLocation.BGSWorldLocation_LoadGame(apLoadGameBuffer);
    LookingTimer.LoadGame(apLoadGameBuffer);
    CheckDestinationTimer.LoadGame(apLoadGameBuffer);
    CheckSearchDoorTimer.LoadGame(apLoadGameBuffer);
    apLoadGameBuffer->ReadFormIdentifier(&pSearchDoor);
}

// @fnv 0x009da6c0
// pdb: CombatProcedureSearch::InitLoadGame | CombatProcedureSearch.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
void __thiscall CombatProcedureSearch::RevertAndResolveRef(BGSLoadFormBuffer* apLoadGameBuffer)
{
    ((TESForm*)this)->TESForm::Revert((unsigned int)apLoadGameBuffer);
    pSearchDoor =
        pSearchDoor ? dynamic_cast<TESObjectREFR*>(TESForm::GetFormByID((unsigned int)pSearchDoor)) : 0;
}

struct FUN_0098c780_shell
{
    void CombatGroup_UnreserveSearchDoor(int);
};
struct FUN_009d8d20_Class
{
    int GetFieldEC();
};
// @fnv 0x009d8c60
// pdb: CombatProcedureSearch::Shutdown | CombatProcedureSearch.obj | retail, vtable slot, high
// pdb locals: pcombatGroup, pattacker
void __thiscall CombatProcedureSearch::UpdateEngageTarget()
{
    Actor* pattacker = CombatProcedure_GetAttacker();
    EffectSetting* pcombatGroup = ((const EffectSetting**)((char*)this + 4))[0]->GetCastSound();
    if (pcombatGroup && ((FUN_009d8d20_Class*)pcombatGroup)->GetFieldEC() == (int)pattacker)
        ((FUN_009d8d00_shell*)pcombatGroup)->SetFieldEC(0);
    pattacker->StopMoving();
    ClearMoveMode();
    if (eState == 5 || eState == 4)
        pattacker->FadeIn_008bcd20();
    if (eState == 2 && pcombatGroup && pSearchDoor)
        ((FUN_0098c780_shell*)pcombatGroup)->CombatGroup_UnreserveSearchDoor((int)pSearchDoor);
}

// Initializes the base, search state, timers, location, and stored reference.
// @fnv 0x009d8b60
// pdb: CombatProcedureSearch::CombatProcedureSearch | CombatProcedureSearch.obj | debug, upstream name already matches, high
CombatProcedureSearch::CombatProcedureSearch() : eState(0)
{
    InvestigateLocation.Init();
    pSearchDoor = 0;
}

struct FUN_009da550_val
{
    int v;
};

struct FUN_009da550_cls
{
    int unk00;
    int unk01;
    int unk02;
    int unk03;
    int unk04;
    int unk05;
    int unk06;
    int unk07;
    int unk08;
    int unk09;
    int unk10;
    int unk11;
    int unk12;
    int unk13;
    int unk14;
    int unk15;
    int unk16;
    int unk17;
    int unk18;
    int unk19;
    int unk20;
    int unk21;
    int unk22;
    int unk23;
    int unk24;
    int unk25;
    int unk26;
    int unk27;
    int unk28;
    int unk29;
    int unk30;
    int unk31;
    int unk32;
    int unk33;
    int unk34;
    int unk35;
    int unk36;
    int unk37;
    int unk38;
    int unk39;
    int unk40;
    int unk41;
    int unk42;
    int unk43;
    int unk44;
    int unk45;
    int unk46;
    int unk47;
    int unk48;
    int unk49;
    int unk50;
    int unk51;
    int unk52;
    int unk53;
    int unk54;
    int unk55;
    int unk56;
    int unk57;
    FUN_009da550_val m_iE8;
    FUN_009da550_val* __thiscall GetValue(FUN_009da550_val* apOut);
};

// @fnv 0x009da550
FUN_009da550_val* FUN_009da550_cls::GetValue(FUN_009da550_val* apOut)
{
    *apOut = m_iE8;
    return apOut;
}

namespace SearchDiagnostics {
// TESObjectREFR.h lacks the containing-space helper used by these diagnostics.
struct Reference : TESObjectREFR
{
    void* TESObjectREFR_GetSpace();
};
typedef CombatProcedureSearch::Location Location;
struct Radius
{
    int value;
    double GetClampedDifference();
};
// The diagnostics use double-returning timer helpers absent from TimeoutTimer.h.
struct Range : TimeoutTimer
{
    double CombatTimer_GetTargetTime();
    double FUN_00435e00();
    double AStarSearch_EvaluateValue_006a7f50();
};
struct Context
{
    Range* CombatGroup_GetLastSearchAreaUpdateTimer();
    Location CombatGroup_GetSearchCenter();
    float GetAvoidRadius();
    Radius GetValue();
    double GetMoveSpeed_009442a0();
    int GetFieldD0();
};
struct Owner
{
    Context* FUN_004fb070();
};
extern const char DAT_01090b68[], DAT_01092078[], DAT_0109205c[], DAT_0108cfe4[], DAT_0109204c[], DAT_0109202c[],
    DAT_01092020[], DAT_0109200c[], DAT_01091ff0[], DAT_01091fe0[], DAT_01091fd8[], DAT_01091fb0[], DAT_01091f88[],
    DAT_01091f60[], DAT_01091f40[];
extern const double DAT_01012060, DAT_01017a40;
}

struct Output
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void Print(int, int, const char*, ...);
};

// @fnv 0x009d9fd0
// pdb: CombatProcedureSearch::DisplayDebugText | CombatProcedureSearch.obj | retail, string-set bridge, high
// pdb params: apDebugText, aeLocation, aeJustify
// pdb locals: pgroup, fsearchPercent, pattacker
void CombatProcedureSearch::PrintDebugInfo(Output* apDebugText, int aeLocation, int aeJustify)
{
    using namespace SearchDiagnostics;
    Reference* actor = reinterpret_cast<Reference*>(CombatProcedure_GetAttacker());
    SearchDiagnostics::Context* context = reinterpret_cast<SearchDiagnostics::Owner*>(pCombatController)->FUN_004fb070();
    switch (eState)
    {
    case 0:
        apDebugText->Print(aeLocation, aeJustify, DAT_01090b68);
        break;
    case 1:
        apDebugText->Print(aeLocation, aeJustify, DAT_01092078);
        apDebugText->Print(aeLocation, aeJustify, DAT_0109205c,
                      reinterpret_cast<const NiPoint3*>(InvestigateLocation.FUN_006815c0())->x,
                      reinterpret_cast<const NiPoint3*>(InvestigateLocation.FUN_006815c0())->y,
                      reinterpret_cast<const NiPoint3*>(InvestigateLocation.FUN_006815c0())->z);
        apDebugText->Print(aeLocation, aeJustify, DAT_0108cfe4, reinterpret_cast<TESObjectREFR*>(InvestigateLocation.GetHavokWorldObjectCountInSceneGraph_0084e3a0())->GetName());
        break;
    case 2:
        apDebugText->Print(aeLocation, aeJustify, DAT_0109204c);
        if (pSearchDoor)
            apDebugText->Print(aeLocation, aeJustify, DAT_0109202c, pSearchDoor->GetName(),
                          static_cast<const TESObjectREFR*>(pSearchDoor)->GetPos()->x,
                          static_cast<const TESObjectREFR*>(pSearchDoor)->GetPos()->y,
                          static_cast<const TESObjectREFR*>(pSearchDoor)->GetPos()->z);
        break;
    case 3:
        apDebugText->Print(aeLocation, aeJustify, DAT_01092020);
        break;
    case 4:
        apDebugText->Print(aeLocation, aeJustify, DAT_0109200c);
        apDebugText->Print(aeLocation, aeJustify, DAT_01091ff0,
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).FUN_00435e00(),
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).CombatTimer_GetTargetTime());
        break;
    case 5:
        apDebugText->Print(aeLocation, aeJustify, DAT_01091fe0);
        apDebugText->Print(aeLocation, aeJustify, DAT_01091ff0,
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).FUN_00435e00(),
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).CombatTimer_GetTargetTime());
        break;
    case 6:
        apDebugText->Print(aeLocation, aeJustify, DAT_01091fd8);
        apDebugText->Print(aeLocation, aeJustify, DAT_01091ff0,
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).FUN_00435e00(),
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).CombatTimer_GetTargetTime());
        break;
    case 7:
        apDebugText->Print(aeLocation, aeJustify, DAT_01091fb0);
        apDebugText->Print(aeLocation, aeJustify, DAT_01091ff0,
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).FUN_00435e00(),
                      reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).CombatTimer_GetTargetTime());
        if (context->CombatGroup_GetLastSearchAreaUpdateTimer()->AStarSearch_EvaluateValue_006a7f50() < reinterpret_cast<SearchDiagnostics::Range&>(LookingTimer).AStarSearch_EvaluateValue_006a7f50() &&
            context->CombatGroup_GetSearchCenter().GetHavokWorldObjectCountInSceneGraph_0084e3a0() != actor->TESObjectREFR_GetSpace())
            apDebugText->Print(aeLocation, aeJustify, DAT_01091f88, context->CombatGroup_GetLastSearchAreaUpdateTimer()->FUN_00435e00(),
                          context->CombatGroup_GetLastSearchAreaUpdateTimer()->CombatTimer_GetTargetTime());
        break;
    case 8:
        apDebugText->Print(aeLocation, aeJustify, DAT_01091f60);
        apDebugText->Print(aeLocation, aeJustify, DAT_0109205c,
                      reinterpret_cast<const NiPoint3*>(InvestigateLocation.FUN_006815c0())->x,
                      reinterpret_cast<const NiPoint3*>(InvestigateLocation.FUN_006815c0())->y,
                      reinterpret_cast<const NiPoint3*>(InvestigateLocation.FUN_006815c0())->z);
        apDebugText->Print(aeLocation, aeJustify, DAT_0108cfe4, reinterpret_cast<TESObjectREFR*>(InvestigateLocation.GetHavokWorldObjectCountInSceneGraph_0084e3a0())->GetName());
        break;
    }
    float confidence = context->GetAvoidRadius();
    if (confidence >= DAT_01012060)
        apDebugText->Print(aeLocation, aeJustify, DAT_01091f40, context->GetFieldD0(), context->GetMoveSpeed_009442a0(), confidence * DAT_01017a40,
                      context->GetValue().GetClampedDifference());
}

float WrapAngle(float);

struct GameSettingCollection;

using ::SettingT;

extern SettingT<GameSettingCollection> originalRuntimeFloatSetting461;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting462;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting464;
extern SettingT<GameSettingCollection> g_011cecc4;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting465;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting460;
extern SettingT<GameSettingCollection> g_011cfad4;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting463;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting457;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting459;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting458;

extern const float DAT_010162c0;
extern const double DAT_0101db88;
extern const float DAT_0101e704;
extern const double DAT_0106cd60;
extern const float DAT_010231a0;
extern const float DAT_01017718;
extern const double DAT_01012060;
extern const double DAT_01012070;

// TimeoutTimer.h lacks the double-returning elapsed-time helper.
struct SearchTimer : TimeoutTimer
{
    double AStarSearch_EvaluateValue_006a7f50();
};

struct Location16
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    void* FUN_006815c0();
    void* GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    double BGSWorldLocation_GetDistanceSquared(const Location16*);
};

struct Obj0049f1c0;

struct BSPortalGraphSub
{
    bool PathingLOSGridMap_SetLocationSeen(void*, int);
    bool PathingLOSGridMap_IsLocationSeen(void*);
};

struct TileSub
{
    bool CombatState_CheckMovement_009a02a0(Location16*, int, int);
    void* GetLODMultType();
    float CombatState_GetLocationTargetDistance();
    void SetNodeBlockAndTimestamp(const Location16&);
};

struct S00568fa0
{
    int TESObjectREFR_GetLinkedDoorTeleportPosition();
};

struct SearchRef : S00568fa0
{
    void* GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    const NiPoint3& FUN_006815c0();
    float CombatSearchLocation_GetScore();
    NiPoint3* TESObjectREFR_GetLinkedDoorTeleportRotation();
    void* TESObjectREFR_GetSpace();
    void* FUN_00568e50();
    Location16 TESObjectREFR_GetWorldLocation();
    void* DoorTeleportData_GetTeleportSpace();
    void* FUN_00717e50();
    void FUN_006dd6f0(int);
};

struct RangeSub
{
    double AStarSearch_EvaluateValue_006a7f50();
};

struct ContextSub
{
    BSPortalGraphSub* GetPortalGraph_009d9f20();
    bool CombatGroup_IsSearching();
    SearchRef* CombatGroup_FindSearchLocation(void*, int, int);
    SearchRef* FindNearestCombatNode(void*, float, int);
    void CombatGroup_ReserveSearchDoor(void*);
    Location16 CombatGroup_GetSearchCenter();
    void* GetFieldEC();
    bool CheckOtherEntriesForTarget(void*, Obj0049f1c0*);
    void SetFieldEC(void*);
    void CombatGroup_InvestigatedSearchLocation(Location16*);
    void CombatGroup_FailedToInvestigateSearchDoor(void*);
    void CombatGroup_InvestigatedSearchDoor(void*);
    void MiddleHighProcess_SetWantWeaponDrawn(int);
    RangeSub* CombatGroup_GetLastSearchAreaUpdateTimer();
};

struct OwnerSub
{
    ContextSub* FUN_004fb070();
    TileSub* CombatController_GetCombatState();
    int Projectile_GetSecondaryItemChange();
};

bool FUN_006d7490(PathingLocation*, void*, int, int);

struct PathingRequestHelper
{
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    int unk60;
    int unk64;
    int unk68;
    int unk6c;
    int unk70;
    int unk74;
    int unk78;
    int unk7c;
    int unk80;
    int unk84;
    int unk88;
    int unk8c;
    int unk90;
    int unk94;
    int unk98;
    int unk9c;
    int unka0;
    int unka4;
    int unka8;
    int unkac;

    PathingRequestHelper();
    virtual ~PathingRequestHelper();

    void SetUseSoft(bool);
    void SetTargetAngle(float);
    void SetDestination(PathingLocation*);
};

// NavMesh.h does not yet declare the triangle search classification helper.
// Keep just that method here, using the shared NavMesh layout.
struct SearchNavMesh : NavMesh
{
    unsigned char NavMesh_GetNeighborCount(unsigned short);
};

struct Obj0049f1c0
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    Obj0049f1c0();
    ~Obj0049f1c0();
};

struct ActorBaseSub
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual bool v10();
};

struct ActorBaseImpl
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    ActorBaseSub sub;
};

// Missing Actor method signatures, with no additional state or virtual slots.
struct ActorSub : Actor
{
    void BuildPathingRequest(void*, const void*, void*, float, int);
    Location16 TESObjectREFR_GetWorldLocation();
    bool Actor_IsPathValid();
    SearchRef* Actor_GetFailedPathDestination();
    void Actor_FadeOut();
    double Actor_GetFadePercent();
    bool Actor_GetCurrentPathfindingGoal_008b3860(void*);
    bool Actor_SetPathfindingFlee(void*);
    void TESObjectREFR_SetAngleOnReferenceZ(float);
};

// @fnv 0x009d8d40
// pdb: CombatProcedureSearch::Update | CombatProcedureSearch.obj | retail, unique string literal, high
// pdb locals: pcombatGroup, ptarget, pcombatState, eprocessLevel, psearchLOSGridMap, pattacker, psearchLocation, request, pdoor, teleportRotation, searchCenter, attackerWorldLoc, psearchingMember, losRequest, plocationTarget, ignoreLocations, bforceComplete, investigatePathingLocation, pteleportData, pfailedPathDestination, searchDestination, fsquaredDistance, vec, spmesh ...
void CombatProcedureSearch::UpdateSearchState()
{
    ActorSub* pActor2;
    BSPortalGraphSub* m_pPortalConnectivityGraphInfo;
    int hasTarget_2;
    TileSub* pTile2;
    void* pUnk7;
    ContextSub* pContext;

    pActor2 = static_cast<ActorSub*>(this->CombatProcedure_GetAttacker());
    pUnk7 = this->GetTarget_00996e00();
    hasTarget_2 = pActor2->FUN_00931850();
    pContext = reinterpret_cast<OwnerSub*>(this->pCombatController)->FUN_004fb070();
    m_pPortalConnectivityGraphInfo = pContext->GetPortalGraph_009d9f20();
    pTile2 = reinterpret_cast<OwnerSub*>(this->pCombatController)->CombatController_GetCombatState();

    switch (this->eState)
    {
    case 0:
        if (!hasTarget_2 && reinterpret_cast<OwnerSub*>(this->pCombatController)->Projectile_GetSecondaryItemChange() == 4)
        {
            this->eState = 7;
            this->LookingTimer.Start(DAT_010162c0);
            break;
        }
        this->pSearchDoor = 0;
        if (pContext->CombatGroup_IsSearching())
        {
            SearchRef* pObj28 = pContext->CombatGroup_FindSearchLocation(pActor2, 0, 0);
            if (pObj28 && pObj28->CombatSearchLocation_GetScore() > DAT_0101db88)
            {
                PathingRequestHelper req;
                pActor2->BuildPathingRequest(&req, &pObj28->FUN_006815c0(), pObj28->GetHavokWorldObjectCountInSceneGraph_0084e3a0(), DAT_0101e704, 0);
                req.SetUseSoft(false);
                if (!pActor2->SetPathingRequestAvoidNodes(&req))
                {
                    this->CombatProcedure_SetFailed("Cannot setup path to investigate point");
                    break;
                }
                this->eState = 1;
                this->CheckDestinationTimer.Start(originalRuntimeFloatSetting461.Float()[0]);
                this->InvestigateLocation = *(CombatProcedureSearch::Location*)pObj28;
            }
            else
            {
                SearchRef* pObjE0 =
                    pContext->FindNearestCombatNode(pActor2,
                                                    (hasTarget_2 == 0) ? originalRuntimeFloatSetting462.Float()[0]
                                                                       : originalRuntimeFloatSetting464.Float()[0],
                                                    1);
                if (pObjE0)
                {
                    PathingRequestHelper req;
                    pActor2->BuildPathingRequest(&req, (void*)pObjE0->TESObjectREFR_GetLinkedDoorTeleportPosition(), pObjE0->TESObjectREFR_GetSpace(), 0.0f, 0);
                    NiPoint3 rot = *pObjE0->TESObjectREFR_GetLinkedDoorTeleportRotation();
                    req.SetTargetAngle(WrapAngle(rot.z - 3.1415927410125732));
                    if (!pActor2->SetPathingRequestAvoidNodes(&req))
                    {
                        this->CombatProcedure_SetFailed("Cannot setup path to search door");
                        break;
                    }
                    this->eState = 2;
                    this->pSearchDoor = reinterpret_cast<TESObjectREFR*>(pObjE0);
                    pContext->CombatGroup_ReserveSearchDoor(pObjE0);
                }
                else
                {
                    Location16 loc1ac = pContext->CombatGroup_GetSearchCenter();
                    if (m_pPortalConnectivityGraphInfo == 0 || loc1ac.GetHavokWorldObjectCountInSceneGraph_0084e3a0() != pActor2->TESObjectREFR_GetSpace())
                    {
                        Location16 loc1bc = pActor2->TESObjectREFR_GetWorldLocation();
                        if (loc1bc.BGSWorldLocation_GetDistanceSquared(&loc1ac) > DAT_0106cd60)
                        {
                            PathingRequestHelper req;
                            if (pTile2->CombatState_CheckMovement_009a02a0(&loc1ac, 1, 0))
                            {
                                pActor2->BuildPathingRequest(&req, loc1ac.FUN_006815c0(), loc1ac.GetHavokWorldObjectCountInSceneGraph_0084e3a0(),
                                                             DAT_0101e704, 0);
                                if (!pActor2->SetPathingRequestAvoidNodes(&req))
                                {
                                    this->CombatProcedure_SetFailed("Cannot setup path to search center");
                                    break;
                                }
                                this->InvestigateLocation = reinterpret_cast<CombatProcedureSearch::Location&>(loc1ac);
                                this->eState = 8;
                            }
                        }
                        else
                        {
                            this->eState = 6;
                            this->LookingTimer.Start(1.0f);
                        }
                    }
                    else
                    {
                        void* p360 = pContext->GetFieldEC();
                        if (p360 && p360 != pActor2)
                        {
                            break;
                        }
                        PathingRequestLOS losReq;
                        losReq.PathingRequest_SetOrigin(pActor2);
                        if (pActor2->IsCreature())
                        {
                            losReq.SetCantOpenDoors(((ActorBaseImpl*)pActor2->GetActorBase())->sub.v10());
                        }
                        losReq.PathingRequest_SetNoDestination();
                        losReq.AssignAtOffsetB0(reinterpret_cast<const NiPointer<void>&>(
                            static_cast<const NiPointer<BSPortalGraphSub>&>(m_pPortalConnectivityGraphInfo)));
                        SearchRef* pSearch = (SearchRef*)pTile2->GetLODMultType();
                        if (pSearch && (*(unsigned char*)pSearch == 1 || *(unsigned char*)pSearch == 2 ||
                                        *(unsigned char*)pSearch == 3))
                        {
                            losReq.SetSearchCenter(*(NiPoint3*)&((SearchRef*)((char*)pSearch + 8))->FUN_006815c0(),
                                                pTile2->CombatState_GetLocationTargetDistance());
                        }
                        if (pObj28 && pObj28->GetHavokWorldObjectCountInSceneGraph_0084e3a0() == pActor2->TESObjectREFR_GetSpace())
                        {
                            losReq.SetDestination(&PathingLocation(reinterpret_cast<const NiPoint3&>(pObj28->FUN_006815c0()), (void*)pActor2));
                        }
                        losReq.PathingRequestLOS_SetIgnoreLocationRadius(DAT_010231a0);
                        losReq.SetByteAtOffset9B(0);
                        Obj0049f1c0 objLoc;
                        if (pContext->CheckOtherEntriesForTarget(pActor2, &objLoc))
                        {
                            losReq.PathingRequestLOS_SetIgnoreLocations(&objLoc);
                        }
                        if (!pActor2->Actor_SetPathfindingFlee(&losReq))
                        {
                            this->CombatProcedure_SetFailed("Cannot setup LOS search path");
                            break;
                        }
                        pContext->SetFieldEC(pActor2);
                        this->CheckDestinationTimer.Start(g_011cecc4.Float()[0]);
                        this->eState = 3;
                        this->CheckSearchDoorTimer.Start(originalRuntimeFloatSetting465.Float()[0]);
                    }
                }
            }
            this->SetMoveMode_00996e20(1);
        }
        break;

    case 1:
    case 8:
        if (pActor2->DidPathingFail())
        {
            if (!pActor2->Actor_IsPathValid())
            {
                pTile2->SetNodeBlockAndTimestamp(reinterpret_cast<Location16&>(this->InvestigateLocation));
            }
            this->CombatProcedure_SetFailed(pActor2->Actor_IsPathValid() ? "Pathing failed while moving to destination"
                                                       : "Path Building failed to destination");
            break;
        }
        {
            bool bFlag361 = false;
            if (this->eState == 1 && this->CheckDestinationTimer.Expired())
            {
                Location16 loc374 = pActor2->TESObjectREFR_GetWorldLocation();
                if (loc374.BGSWorldLocation_GetDistanceSquared(reinterpret_cast<Location16*>(&this->InvestigateLocation)) <
                    originalRuntimeFloatSetting460.Float()[0] * originalRuntimeFloatSetting460.Float()[0])
                {
                    PathingLocation loc39c(reinterpret_cast<Location16*>(&this->InvestigateLocation));
                    if (FUN_006d7490(&loc39c, loc374.FUN_006815c0(), 0, 0))
                    {
                        bFlag361 = true;
                    }
                }
                this->CheckDestinationTimer.Start(g_011cfad4.Float()[0]);
            }
            if (bFlag361 || pActor2->FUN_008b3bb0())
            {
                if (bFlag361)
                {
                    pActor2->StopMoving();
                }
                pContext->CombatGroup_InvestigatedSearchLocation(reinterpret_cast<Location16*>(&this->InvestigateLocation));
                this->eState = 6;
                this->LookingTimer.Start(1.0f);
            }
        }
        break;

    case 2:
        if (pActor2->DidPathingFail())
        {
            if (!pActor2->Actor_IsPathValid() && this->pSearchDoor)
            {
                pTile2->SetNodeBlockAndTimestamp(reinterpret_cast<SearchRef*>(this->pSearchDoor)->TESObjectREFR_GetWorldLocation());
                pContext->CombatGroup_FailedToInvestigateSearchDoor(this->pSearchDoor);
            }
            this->CombatProcedure_SetFailed(pActor2->Actor_IsPathValid() ? "Pathing failed while moving to search door"
                                                       : "Path Building failed to search door");
            break;
        }
        if (pActor2->FUN_008b3bb0())
        {
            if (this->pSearchDoor)
            {
                pContext->CombatGroup_InvestigatedSearchDoor(this->pSearchDoor);
                if (!hasTarget_2)
                {
                    pActor2->Actor_FadeOut();
                    this->eState = 4;
                    this->LookingTimer.Start(DAT_01017718);
                }
                else
                {
                    PathingRequestHelper req;
                    SearchRef* p3a0 = (SearchRef*)reinterpret_cast<SearchRef*>(this->pSearchDoor)->FUN_00568e50();
                    if (!p3a0)
                    {
                        break;
                    }
                    pActor2->BuildPathingRequest(&req, p3a0->FUN_00717e50(), p3a0->DoorTeleportData_GetTeleportSpace(), 0.0f, 0);
                    if (!pActor2->SetPathingRequestAvoidNodes(&req))
                    {
                        this->CombatProcedure_SetFailed("Cannot setup path through search door");
                        break;
                    }
                    this->eState = 2;
                }
                this->pSearchDoor = 0;
            }
            else
            {
                this->eState = 6;
                this->LookingTimer.Start(1.0f);
            }
        }
        break;

    case 3: {
        bool bFlag455;
        if (pActor2->DidPathingFail())
        {
            if (!pActor2->Actor_IsPathValid())
            {
                SearchRef* p45c = pActor2->Actor_GetFailedPathDestination();
                if (p45c && m_pPortalConnectivityGraphInfo)
                {
                    p45c->FUN_006dd6f0(0);
                    m_pPortalConnectivityGraphInfo->PathingLOSGridMap_SetLocationSeen(p45c, 1);
                    pContext->MiddleHighProcess_SetWantWeaponDrawn(1);
                }
            }
            this->CombatProcedure_SetFailed(pActor2->Actor_IsPathValid() ? "Pathing failed while searching"
                                                       : "Path Building failed to search");
            break;
        }
        if (this->CheckSearchDoorTimer.Expired() && !hasTarget_2)
        {
            this->CheckSearchDoorTimer.Start(originalRuntimeFloatSetting465.Float()[0]);
            SearchRef* pObj460 = pContext->FindNearestCombatNode(pActor2, originalRuntimeFloatSetting463.Float()[0], 1);
            if (pObj460)
            {
                PathingRequestHelper req;
                pActor2->BuildPathingRequest(&req, (void*)pObj460->TESObjectREFR_GetLinkedDoorTeleportPosition(), pObj460->TESObjectREFR_GetSpace(), 0.0f, 0);
                if (pActor2->SetPathingRequestAvoidNodes(&req))
                {
                    this->eState = 2;
                    this->pSearchDoor = reinterpret_cast<TESObjectREFR*>(pObj460);
                    pContext->CombatGroup_ReserveSearchDoor(pObj460);
                    break;
                }
            }
        }
        {
            bFlag455 = false;
            if (m_pPortalConnectivityGraphInfo && this->CheckDestinationTimer.Expired())
            {
                PathingLocation loc53c;
                if (pActor2->Actor_GetCurrentPathfindingGoal_008b3860(&loc53c))
                {
                    NiPoint3 vDiff = *reinterpret_cast<const NiPoint3*>(static_cast<const TESObjectREFR*>(pActor2)->GetPos()) - loc53c.FUN_005c3420<NiPoint3>();
                    float dist = vDiff.FUN_004a7290();
                    if (dist < originalRuntimeFloatSetting457.Float()[0] * originalRuntimeFloatSetting457.Float()[0])
                    {
                        loc53c.FUN_006dd6f0(0);
                        if (loc53c.FUN_006dd6a0() && m_pPortalConnectivityGraphInfo->PathingLOSGridMap_IsLocationSeen(&loc53c))
                        {
                            bFlag455 = true;
                            NavMeshPtr navMesh;
                            if (loc53c.FUN_006dd610(&navMesh))
                            {
                                char cRes = reinterpret_cast<SearchNavMesh*>(navMesh.operator->())
                                                ->NavMesh_GetNeighborCount(loc53c.GetShortValueAtOffset24());
                                if ((cRes == 1 && dist > originalRuntimeFloatSetting459.Float()[0] *
                                                             originalRuntimeFloatSetting459.Float()[0]) ||
                                    (cRes == 2 && dist > originalRuntimeFloatSetting458.Float()[0] *
                                                             originalRuntimeFloatSetting458.Float()[0]))
                                {
                                    bFlag455 = false;
                                }
                            }
                            if (bFlag455)
                            {
                                pActor2->StopMoving();
                            }
                        }
                    }
                }
                this->CheckDestinationTimer.Start(g_011cecc4.Float()[0]);
            }
            if (bFlag455 || pActor2->FUN_008b3bb0())
            {
                this->eState = 6;
                this->LookingTimer.Start(1.0f);
            }
        }
        break;
    }

    case 4:
        if (pActor2->Actor_GetFadePercent() <= DAT_01012060)
        {
            if (this->pSearchDoor)
            {
                NiPoint3 rot = *reinterpret_cast<SearchRef*>(this->pSearchDoor)->TESObjectREFR_GetLinkedDoorTeleportRotation();
                pActor2->TESObjectREFR_SetAngleOnReferenceZ(WrapAngle(rot.z));
            }
            this->eState = 5;
        }
        break;

    case 5:
        if (this->LookingTimer.Expired())
        {
            if (pActor2->Actor_GetFadePercent() < DAT_01012070)
            {
                pActor2->FadeIn_008bcd20();
            }
            this->eState = 0;
        }
        break;

    case 6:
        if (this->LookingTimer.Expired())
        {
            this->eState = 0;
        }
        break;

    case 7:
        if (pContext->CombatGroup_GetLastSearchAreaUpdateTimer()->AStarSearch_EvaluateValue_006a7f50() < reinterpret_cast<SearchTimer&>(this->LookingTimer).AStarSearch_EvaluateValue_006a7f50())
        {
            if (pContext->CombatGroup_GetSearchCenter().GetHavokWorldObjectCountInSceneGraph_0084e3a0() != pActor2->TESObjectREFR_GetSpace())
            {
                break;
            }
        }
        if (this->LookingTimer.Expired())
        {
            this->eState = 0;
        }
        break;
    }
}
