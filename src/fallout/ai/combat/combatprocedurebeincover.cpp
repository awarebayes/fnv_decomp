#include "../../../../include/InterfaceManager.h"
#include "../../../../include/Actor.h"
#include "../../../../include/BaseFormComponent.h"
#include "../../../../include/BGSLoadFormBuffer.h"
#include "../../../../include/CombatProcedure.h"
#include "../../../../include/CombatProcedureBeInCover.h"
#include "../../../../include/CombatProcedureEngageTarget.h"
#include "../../../../include/TESCombatStyle.h"
#include "../../../../include/IDebugText.h"
#include "../../../../include/TESAttributes.h"

// src/fallout/ai/combat/combatprocedurebeincover.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout/ai/combat/combatprocedurebeincover.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

struct BGSTextureSet;
struct BipedAnim;
struct CHUNK;
struct FormRecordHeader;
struct TESFileGroup;
struct HWND__;
struct NiAVObject;
struct NiColor;
struct NiPoint3;
struct TESBoundObject;
struct TESFile;
struct TESForm;
struct TESFullName;
struct TESModel;
struct TESModelTextureSwap;
struct TESObject;
struct TESFormList;
struct TESObjectREFR;
struct TESTexture;
struct FileEntryList;
struct TESWaterForm;
struct DistantTreeTextureInfo;
struct blob8
{
    char b[8];
};
struct blob12
{
    char b[12];
};
typedef char _sizecheck_BaseFormComponent[1];
typedef char _sizecheck_TESAttributes[1];
// @fnv 0x009d2510
int CombatProcedureBeInCover::GetType()
{
    return 7;
}


extern "C" float __cdecl RandomFloat(float, float);

// Returns a random float between the two floats stored at offsets 0x20 and 0x24 of this object.
// @fnv 0x009d32e0
// pdb: TESCombatStyle::GetRandomCoverWaitTimer | CombatProcedureBeInCover.obj | debug, object-file run extension, low
float __fastcall GetCoverWaitTime(TESCombatStyle* apStyle)
{
    return RandomFloat(*(float*)&apStyle->m_kUnsignedIntArray16.data[2],
                       *(float*)&apStyle->m_kUnsignedIntArray16.data[3]);
}

float __cdecl RandomFloat(float, float);

// Returns a random float between the two float values stored at offsets 0x28 and 0x2c of this object.
// @fnv 0x009d3310
// pdb: TESCombatStyle::GetRandomCoverWaitToFireTimer | CombatProcedureBeInCover.obj | debug, object-file run extension, low
float __fastcall GetOutOfCoverWaitTime(TESCombatStyle* apStyle)
{
    return RandomFloat(*(float*)&apStyle->m_kUnsignedIntArray16.data[4],
                       *(float*)&apStyle->m_kUnsignedIntArray16.data[5]);
}

float __cdecl RandomFloat(float, float);

// Returns a random float between the combat style's two stored float fields.
// @fnv 0x009d3340
// pdb: TESCombatStyle::GetRandomCoverFireTimer | CombatProcedureBeInCover.obj | debug, object-file run extension, low
float __thiscall TESCombatStyle::GetRandomValue()
{
    return RandomFloat(*(float*)&m_kUnsignedIntArray16.data[6], *(float*)&m_kUnsignedIntArray16.data[7]);
}

// @fnv 0x009d2520: scalar deleting destructor emitted by the compiler.
// @fnv 0x009d2550
// pdb: CombatProcedureBeInCover::`scalar deleting destructor' | CombatProcedureBeInCover.obj | debug, vtable slot, medium
// pdb: CombatProcedureBeInCover::~CombatProcedureBeInCover | CombatProcedureBeInCover.obj | debug, upstream name already matches, high
CombatProcedureBeInCover::~CombatProcedureBeInCover()
{}

struct TESAmmoInitCalls
{
    Actor* GetFieldBC();
};
// @fnv 0x009d2570
// pdb: CombatProcedureHideFromTarget::Shutdown | CombatProcedureHideFromTarget.obj | retail, vtable slot, high
// pdb locals: pattacker
void CombatProcedureBeInCover::StopAttackerAndEngageTarget()
{
    Actor* actor = (((TESAmmoInitCalls*)pCombatController))->GetFieldBC();
    actor->StopMoving();
    ((CombatProcedureEngageTarget*)this)->ClearMoveMode();
    RunProcedure();
}

struct FUN_00980070_shell
{
    unsigned int GetCurrentCombatStyle();
};
struct FUN_008d7f40_shell
{
    void FUN_008d7f40(float);
};
// @fnv 0x009d33e0
// pdb: CombatProcedureBeInCover::FiredProjectile | CombatProcedureBeInCover.obj | retail, vtable slot, high
// pdb params: apProjectile
void CombatProcedureBeInCover::UpdateCombatProcedure(Projectile* apProjectile)
{
    switch (eState)
    {
    case 1:
        ((FUN_008d7f40_shell*)&WaitBehindCoverTimer)
            ->FUN_008d7f40(GetCoverWaitTime((TESCombatStyle*)((FUN_00980070_shell*)(int)pCombatController)->GetCurrentCombatStyle()));
        break;
    case 2:
        ((CombatProcedureEngageTarget*)this)->CombatProcedure_GetAttacker()->StopMoving();
        RunProcedure();
        eState = COVER_STATE_FIRING_OUT_OF_COVER;
        ((FUN_008d7f40_shell*)&FiringOutOfCoverTimer)
            ->FUN_008d7f40(
                ((TESCombatStyle*)((FUN_00980070_shell*)(int)pCombatController)->GetCurrentCombatStyle())->GetRandomValue());
        break;
    }
}

enum DEBUGTEXT_LOCATION
{
    DEBUGTEXT_LOCATION_0
};

enum DEBUGTEXT_JUSTIFY
{
    DEBUGTEXT_JUSTIFY_0
};

extern const char $SG2706[];
extern const char str_01091084[];
extern const char str_0109135c[];
extern const char str_01091380[];
extern const char str_010913a4[];
extern const char str_010913c8[];
extern const char str_010913ec[];
extern const char str_010913fc[];
extern const char str_01091414[];
extern const char str_01091420[];
extern const char str_01091434[];
extern const char str_01091458[];
extern const char str_01091474[];
extern const char str_0109148c[];
extern const char str_01091498[];
extern const char str_010914b0[];

// Prints debug text describing the current cover state and its timers, positions and target-blocked status via pDebugText.
// @fnv 0x009d3470
// pdb: CombatProcedureBeInCover::DisplayDebugText | CombatProcedureBeInCover.obj | retail, string-set bridge, high
// pdb params: apDebugText, aeLocation, aeJustify
void CombatProcedureBeInCover::DebugText(IDebugText* apDebugText, DEBUGTEXT_LOCATION aeLocation,
                                         DEBUGTEXT_JUSTIFY aeJustify)
{
    switch (eState)
    {
    case COVER_STATE_INITIALIZING:
        apDebugText->Print(aeLocation, aeJustify, $SG2706);
        break;

    case COVER_STATE_WAITING_BEHIND_COVER:
        apDebugText->Print(aeLocation, aeJustify, str_010914b0);
        apDebugText->Print(aeLocation, aeJustify, str_01091498, WaitBehindCoverTimer.GetElapsedTime(), WaitBehindCoverTimer.GetValue());
        break;

    case COVER_STATE_MOVING_OUT:
        apDebugText->Print(aeLocation, aeJustify, str_0109148c);
        break;

    case COVER_STATE_WAITING_OUT_OF_COVER:
        apDebugText->Print(aeLocation, aeJustify, str_01091474);
        apDebugText->Print(aeLocation, aeJustify, str_01091458, WaitingOutOfCoverTimer.GetElapsedTime(),
                          WaitingOutOfCoverTimer.GetValue());
        if (TargetBlockedTimer.CombatTimer_IsSet())
        {
            apDebugText->Print(aeLocation, aeJustify, str_01091434, TargetBlockedTimer.GetElapsedTime(),
                              TargetBlockedTimer.GetValue());
        }
        break;

    case COVER_STATE_FIRING_OUT_OF_COVER:
        apDebugText->Print(aeLocation, aeJustify, str_01091420);
        apDebugText->Print(aeLocation, aeJustify, str_01091084, FiringOutOfCoverTimer.GetElapsedTime(),
                          FiringOutOfCoverTimer.GetValue());
        break;

    case COVER_STATE_MOVING_IN:
        apDebugText->Print(aeLocation, aeJustify, str_01091414);
        break;

    case COVER_STATE_MOVING_IN_AND_ROTATE:
        apDebugText->Print(aeLocation, aeJustify, str_010913fc);
        break;

    case COVER_STATE_HOLDING_GROUND:
        apDebugText->Print(aeLocation, aeJustify, str_010913ec);
        apDebugText->Print(aeLocation, aeJustify, str_010913c8, HoldingGroundTimer.GetElapsedTime(),
                          HoldingGroundTimer.GetValue());
        break;
    }

    if (ReevaluateCoverTimer.CombatTimer_IsSet())
    {
        apDebugText->Print(aeLocation, aeJustify, str_010913a4, ReevaluateCoverTimer.GetElapsedTime(),
                          ReevaluateCoverTimer.GetValue());
    }

    apDebugText->Print(aeLocation, aeJustify, str_01091380, CurrentCoverPosition[0], CurrentCoverPosition[1], CurrentCoverPosition[2]);

    apDebugText->Print(aeLocation, aeJustify, str_0109135c, CurrentFirePosition[0], CurrentFirePosition[1],
                      CurrentFirePosition[2]);
}

struct CoverVec3
{
    float x;
    float y;
    float z;
    CoverVec3();
};

struct CoverTimer
{
    unsigned int unk00;
    unsigned int unk04;
    CoverTimer();
};

// The shared class stores these vectors and timers as raw storage.
// Invoke their constructors explicitly to preserve the original initialization calls.
// @fnv 0x009d2440
// pdb: CombatProcedureBeInCover::CombatProcedureBeInCover | CombatProcedureBeInCover.obj | debug, upstream name already matches, high
// pdb params: apCombatCoverLocation
CombatProcedureBeInCover::CombatProcedureBeInCover(void* apCombatCoverLocation) : eState(COVER_STATE_INITIALIZING), pCoverLocation(apCombatCoverLocation)
{
    ((CoverVec3*)CurrentCoverPosition)->CoverVec3::CoverVec3();
    ((CoverVec3*)CurrentFirePosition)->CoverVec3::CoverVec3();
    ((CoverVec3*)LookAtPosition)->CoverVec3::CoverVec3();
    ((CoverTimer*)&WaitBehindCoverTimer)->CoverTimer::CoverTimer();
    ((CoverTimer*)&WaitingOutOfCoverTimer)->CoverTimer::CoverTimer();
    ((CoverTimer*)&FiringOutOfCoverTimer)->CoverTimer::CoverTimer();
    ((CoverTimer*)&ReevaluateCoverTimer)->CoverTimer::CoverTimer();
    ((CoverTimer*)&HoldingGroundTimer)->CoverTimer::CoverTimer();
    ((CoverTimer*)&TargetBlockedTimer)->CoverTimer::CoverTimer();
}

struct FUN_00586150_shell
{
    void* CombatController_GetCombatState();
};

struct FUN_0097fb60_shell
{
    int GetFieldAtE8();
};

struct FUN_00865e50_shell
{
    void FUN_00865e50(void* p, int size, int flags);
};

struct FUN_009a5eb0_shell
{
    void FUN_009a5eb0(void* a1);
};

// @fnv 0x009d3800
// pdb: CombatProcedureBeInCover::SaveGame | CombatProcedureBeInCover.obj | retail, upstream name already matches, high
// pdb params: apSaveGameBuffer
// pdb locals: bsecondaryCoverLocation
void CombatProcedureBeInCover::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    unsigned char flag;

    CombatProcedure::SaveGame(apSaveGameBuffer);
    flag = ((FUN_0097fb60_shell*)((FUN_00586150_shell*)((void*)pCombatController))->CombatController_GetCombatState())->GetFieldAtE8() ==
           (int)pCoverLocation;
    ((FUN_00865e50_shell*)apSaveGameBuffer)->FUN_00865e50(&eState, 4, 0);
    ((FUN_00865e50_shell*)apSaveGameBuffer)->FUN_00865e50(&flag, 1, 0);
    ((FUN_00865e50_shell*)apSaveGameBuffer)->FUN_00865e50(CurrentCoverPosition, 0xc, 0);
    ((FUN_00865e50_shell*)apSaveGameBuffer)->FUN_00865e50(CurrentFirePosition, 0xc, 0);
    ((FUN_00865e50_shell*)apSaveGameBuffer)->FUN_00865e50(LookAtPosition, 0xc, 0);
    ((FUN_009a5eb0_shell*)&WaitBehindCoverTimer)->FUN_009a5eb0(apSaveGameBuffer);
    ((FUN_009a5eb0_shell*)&WaitingOutOfCoverTimer)->FUN_009a5eb0(apSaveGameBuffer);
    ((FUN_009a5eb0_shell*)&FiringOutOfCoverTimer)->FUN_009a5eb0(apSaveGameBuffer);
    ((FUN_009a5eb0_shell*)&ReevaluateCoverTimer)->FUN_009a5eb0(apSaveGameBuffer);
    ((FUN_009a5eb0_shell*)&HoldingGroundTimer)->FUN_009a5eb0(apSaveGameBuffer);
    ((FUN_009a5eb0_shell*)&TargetBlockedTimer)->FUN_009a5eb0(apSaveGameBuffer);
}

class Tile;


unsigned int __fastcall GetFieldValue(unsigned int* apData);
struct FUN_009a5f20_Timer
{
    char data[8];
    void FUN_009a5f20(BGSLoadFormBuffer* apLoadGameBuffer);
};
// Loads state, cover location, three 3-float vectors and six timers from the form buffer, resolving the cover location via the interface manager.
// @fnv 0x009d38f0
// pdb: CombatProcedureBeInCover::LoadGame | CombatProcedureBeInCover.obj | retail, upstream name already matches, high
// pdb params: apLoadGameBuffer
// pdb locals: bsecondaryCoverLocation
void CombatProcedureBeInCover::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    CombatProcedure::LoadGame(apLoadGameBuffer);
    bool bHasCover = false;
    apLoadGameBuffer->LoadData(&eState, 4u);
    apLoadGameBuffer->LoadData(&bHasCover, 1u);
    if (bHasCover)
        pCoverLocation = (void*)((FUN_0097fb60_shell*)(((InterfaceManager*)pCombatController))->CombatController_GetCombatState())->GetFieldAtE8();
    else
        pCoverLocation = (void*)GetFieldValue((unsigned int*)(((InterfaceManager*)pCombatController))->CombatController_GetCombatState());
    apLoadGameBuffer->LoadData(CurrentCoverPosition, 0xcu);
    apLoadGameBuffer->LoadData(CurrentFirePosition, 0xcu);
    apLoadGameBuffer->LoadData(LookAtPosition, 0xcu);
    ((FUN_009a5f20_Timer*)&WaitBehindCoverTimer)->FUN_009a5f20(apLoadGameBuffer);
    ((FUN_009a5f20_Timer*)&WaitingOutOfCoverTimer)->FUN_009a5f20(apLoadGameBuffer);
    ((FUN_009a5f20_Timer*)&FiringOutOfCoverTimer)->FUN_009a5f20(apLoadGameBuffer);
    ((FUN_009a5f20_Timer*)&ReevaluateCoverTimer)->FUN_009a5f20(apLoadGameBuffer);
    ((FUN_009a5f20_Timer*)&HoldingGroundTimer)->FUN_009a5f20(apLoadGameBuffer);
    ((FUN_009a5f20_Timer*)&TargetBlockedTimer)->FUN_009a5f20(apLoadGameBuffer);
}

struct CoverPoint
{
    float x, y, z;
    CoverPoint operator-(const CoverPoint&) const;
    float FUN_004a7290() const;
};
struct CoverClock
{
    int started;
    float duration;
    bool CombatTimer_IsSet();
    bool FUN_008d7f80();
    void FUN_008d7f40(float);
    float GetElapsedTime();
    float GetValue();
    float AStarSearch_EvaluateValue_006a7f50();
    void Reset();
};
struct CoverRequest
{
    virtual ~CoverRequest();
    CoverRequest();
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
};
struct CoverActor
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
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void v72();
    virtual void v73();
    virtual void v74();
    virtual void v75();
    virtual void v76();
    virtual void v77();
    virtual void v78();
    virtual void v79();
    virtual void v80();
    virtual void v81();
    virtual void v82();
    virtual void v83();
    virtual void v84();
    virtual void v85();
    virtual void v86();
    virtual void v87();
    virtual void v88();
    virtual void v89();
    virtual void v90();
    virtual void v91();
    virtual void v92();
    virtual void v93();
    virtual void v94();
    virtual void v95();
    virtual void v96();
    virtual void v97();
    virtual void v98();
    virtual void v99();
    virtual void v100();
    virtual CoverPoint GetAim();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual const CoverPoint& GetPosition();

    void StopMoving();
    float Actor_GetSneakHeight(bool);
    float GetActorHeight() const;
    void* GetWorld();
    void* GetContainingCell();
    void SetupPath(CoverRequest&, const CoverPoint&, void*, void*, float, int);
    bool RequestPath(CoverRequest*);
    bool IsAttacking();
    float AttackTime();
    void UpdateAttackState();
    bool PathFailed();
    bool PathBuiltFailed();
    bool PathFinished();
};
struct CombatCoverPoint
{
    int unk00, unk04;
    CoverPoint position;
    CoverPoint direction;
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
    bool occupied;
    bool CanFire();
    bool CanRotate();
    void SetFlag(int);
};
struct CoverStamp
{
    int value;
    float GetElapsedTime();
    float AStarSearch_EvaluateValue_006a7f50();
};
struct CoverState
{
    bool Validate(CombatCoverPoint*);
    bool IsGroup();
    bool CheckPath(const CoverPoint&, const CoverPoint&, CoverActor*, int);
    CoverStamp GetStamp();
    void End(int);
    bool IsBlocked();
};
struct CoverGroup
{
    bool HasLOS(CoverActor*);
};
struct CoverManager
{
    CoverState* CombatController_GetCombatState();
    CoverGroup* GetGroup();
    bool CombatController_GetAttackerCanSneak();
    TESCombatStyle* GetCurrentCombatStyle();
};
struct CoverSetting
{
    float* Float();
};
extern CoverSetting originalRuntimeFloatSetting378, originalRuntimeFloatSetting379;
void ComputePositions(CoverManager*, CombatCoverPoint*, CoverPoint&, CoverPoint&);
// Calls used by the cover state machine that are not declared on the shared class yet.
struct CoverProcedureCalls
{
    void ClearLook();
    void LookAt(const CoverPoint&, unsigned char);
};
static const char kCoverLOS[] = "Target has LOS on Cover position";
// Validates the cover location, reevaluates paths/LOS, and advances the cover procedure state machine through movement, firing and holding phases.
// @fnv 0x009d25b0
// pdb: CombatProcedureBeInCover::Update | CombatProcedureBeInCover.obj | retail, string-set bridge, high
// pdb locals: pcombatGroup, ptarget, pcombatState, bledgeCover, bcoverInvalid, pattacker, breevaluatedCover, bcanSneak, bcoverSafe, targetVec, targetLoc, originalTargetLoc, request, fheight, flastAttackTimeStamp, fcurrentAmmoClipPercent, fdistToCoverSquared, attackerLoc
void CombatProcedureBeInCover::UpdateCoverState()
{
    if (!(((CoverManager*)pCombatController))->CombatController_GetCombatState()->Validate(((CombatCoverPoint*)pCoverLocation)))
    {
        ((CombatProcedureEngageTarget*)this)->Fail("Cover Location no longer exists");
        return;
    }
    CoverActor* actor_ = (CoverActor*)((CombatProcedureEngageTarget*)this)->CombatProcedure_GetAttacker();
    CoverActor* targetTmp = (CoverActor*)((CombatProcedureEngageTarget*)this)->GetTarget();
    CoverState* combat7 = (((CoverManager*)pCombatController))->CombatController_GetCombatState();
    CoverGroup* group7 = (((CoverManager*)pCombatController))->GetGroup();
    bool evaluatedA = false;
    bool changedVal = false;
    bool canFire4 = ((CombatCoverPoint*)pCoverLocation)->CanFire();
    bool active8 = (((CoverManager*)pCombatController))->CombatController_GetAttackerCanSneak();
    if (!(*(CoverClock*)&ReevaluateCoverTimer).CombatTimer_IsSet())
        (*(CoverClock*)&ReevaluateCoverTimer).FUN_008d7f40(1.0f);
    else if ((*(CoverClock*)&ReevaluateCoverTimer).FUN_008d7f80())
    {
        (*(CoverClock*)&ReevaluateCoverTimer).FUN_008d7f40(1.0f);
        evaluatedA = true;
        if (eState != 0)
        {
            if (combat7->IsGroup())
            {
                bool reachable4 = combat7->CheckPath(((CombatCoverPoint*)pCoverLocation)->direction, (*(CoverPoint*)CurrentCoverPosition), targetTmp, 1);
                if (!reachable4 && eState != 7)
                {
                    actor_->StopMoving();
                    ((CoverProcedureCalls*)this)->ClearLook();
                    eState = COVER_STATE_HOLDING_GROUND;
                    (*(CoverClock*)&HoldingGroundTimer).FUN_008d7f40(2.0f);
                    changedVal = true;
                }
            }
            else
            {
                CoverPoint coverPos9 = targetTmp->GetPosition();
                CoverPoint targetPos = ((CombatCoverPoint*)pCoverLocation)->position;
                CoverPoint delta = coverPos9 - targetPos;
                if (delta.FUN_004a7290() > 589824.0 && group7->HasLOS(targetTmp))
                    ((CombatCoverPoint*)pCoverLocation)->SetFlag(1);
            }
        }
    }
    switch (eState)
    {
    case 0: {
        ((CombatCoverPoint*)pCoverLocation)->occupied = false;
        ComputePositions((((CoverManager*)pCombatController)), ((CombatCoverPoint*)pCoverLocation), (*(CoverPoint*)CurrentCoverPosition), (*(CoverPoint*)CurrentFirePosition));
        (*(CoverPoint*)LookAtPosition) = (*(CoverPoint*)CurrentFirePosition);
        float height = (((CoverManager*)pCombatController))->CombatController_GetAttackerCanSneak() ? actor_->Actor_GetSneakHeight(false) : actor_->GetActorHeight();
        (*(CoverPoint*)LookAtPosition).z = height * 0.75 + (*(CoverPoint*)LookAtPosition).z;
        CoverRequest request;
        actor_->SetupPath(request, (*(CoverPoint*)CurrentCoverPosition), actor_->GetContainingCell(), actor_->GetWorld(), 0.0f, 0);
        if (!actor_->RequestPath(&request))
        {
            ((CombatProcedureEngageTarget*)this)->Fail("Cannot setup path to Cover position");
            return;
        }
        eState = COVER_STATE_MOVING_IN_AND_ROTATE;
        if (((CombatCoverPoint*)pCoverLocation)->CanRotate())
            ((CoverProcedureCalls*)this)->LookAt((*(CoverPoint*)LookAtPosition), 3);
        ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(3);
        break;
    }
    case 1: {
        if (!actor_->IsAttacking())
        {
            if (combat7->GetStamp().GetElapsedTime() > 0.5 &&
                actor_->AttackTime() < *originalRuntimeFloatSetting378.Float())
            {
                actor_->UpdateAttackState();
            }
            else if ((*(CoverClock*)&WaitBehindCoverTimer).FUN_008d7f80())
            {
                CoverRequest request;
                actor_->SetupPath(request, (*(CoverPoint*)CurrentFirePosition), actor_->GetContainingCell(), actor_->GetWorld(), 0.0f, 0);
                if (!actor_->RequestPath(&request))
                {
                    ((CombatProcedureEngageTarget*)this)->Fail("Cannot setup path to Fire position");
                    return;
                }
                if (((CombatCoverPoint*)pCoverLocation)->CanRotate())
                    ((CoverProcedureCalls*)this)->ClearLook();
                ((CoverProcedureCalls*)this)->LookAt(targetTmp->GetAim(), 1);
                eState = COVER_STATE_MOVING_OUT;
                ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(canFire4 && active8 ? 2 : 1);
            }
        }
        break;
    }
    case 2: {
        if (actor_->PathFailed())
        {
            ((CombatProcedureEngageTarget*)this)->Fail(actor_->PathBuiltFailed() ? "Pathing failed while moving out" : "Path Building failed to move out");
            return;
        }
        if (actor_->PathFinished())
        {
            eState = COVER_STATE_WAITING_OUT_OF_COVER;
            ((CoverProcedureCalls*)this)->ClearLook();
            (*(CoverClock*)&WaitingOutOfCoverTimer).FUN_008d7f40(GetOutOfCoverWaitTime((((CoverManager*)pCombatController))->GetCurrentCombatStyle()));
        }
        break;
    }
    case 3: {
        CoverStamp stamp = combat7->GetStamp();
        if (stamp.AStarSearch_EvaluateValue_006a7f50() > (*(CoverClock*)&WaitBehindCoverTimer).AStarSearch_EvaluateValue_006a7f50())
        {
            eState = COVER_STATE_FIRING_OUT_OF_COVER;
            (*(CoverClock*)&FiringOutOfCoverTimer).FUN_008d7f40((((CoverManager*)pCombatController))->GetCurrentCombatStyle()->GetRandomValue());
            (*(CoverClock*)&TargetBlockedTimer).Reset();
        }
        else if ((*(CoverClock*)&WaitingOutOfCoverTimer).FUN_008d7f80())
        {
            combat7->End(0);
            ((CombatProcedureEngageTarget*)this)->Fail("Waited out of cover without firing");
            return;
        }
        else if ((*(CoverClock*)&WaitingOutOfCoverTimer).GetElapsedTime() > 2.0 && !combat7->IsGroup() && group7->HasLOS(targetTmp))
        {
            combat7->End(0);
            ((CombatProcedureEngageTarget*)this)->Fail("Waited out of cover and group member has LOS on target");
            return;
        }
        if (combat7->IsBlocked())
        {
            if (!(*(CoverClock*)&TargetBlockedTimer).CombatTimer_IsSet())
                (*(CoverClock*)&TargetBlockedTimer).FUN_008d7f40(2.0f);
            else if ((*(CoverClock*)&TargetBlockedTimer).FUN_008d7f80())
            {
                combat7->End(1);
                ((CombatProcedureEngageTarget*)this)->Fail("Waited out of cover but was blocked");
                return;
            }
        }
        break;
    }
    case 4: {
        float attackTime_ = actor_->AttackTime();
        if (attackTime_ < *originalRuntimeFloatSetting379.Float() || actor_->IsAttacking() || (*(CoverClock*)&FiringOutOfCoverTimer).FUN_008d7f80())
        {
            CoverRequest request;
            actor_->SetupPath(request, (*(CoverPoint*)CurrentCoverPosition), actor_->GetContainingCell(), actor_->GetWorld(), 0.0f, 0);
            if (!actor_->RequestPath(&request))
            {
                ((CombatProcedureEngageTarget*)this)->Fail("Cannot setup path to move back to Cover position");
                return;
            }
            ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(canFire4 && active8 ? 2 : 1);
            eState = COVER_STATE_MOVING_IN;
        }
        break;
    }
    case 5: {
        if (actor_->PathFailed())
        {
            ((CombatProcedureEngageTarget*)this)->Fail(actor_->PathBuiltFailed() ? "Pathing failed while moving in" : "Path Building failed to move in");
            return;
        }
        if (actor_->PathFinished())
        {
            eState = COVER_STATE_WAITING_BEHIND_COVER;
            ((CoverProcedureCalls*)this)->ClearLook();
            ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(2);
            (*(CoverClock*)&WaitBehindCoverTimer).FUN_008d7f40(GetCoverWaitTime((((CoverManager*)pCombatController))->GetCurrentCombatStyle()));
        }
        else
        {
            float dist9 = (actor_->GetPosition() - (*(CoverPoint*)CurrentCoverPosition)).FUN_004a7290();
            if (dist9 < 1024.0)
            {
                eState = COVER_STATE_MOVING_IN_AND_ROTATE;
                if (((CombatCoverPoint*)pCoverLocation)->CanRotate())
                    ((CoverProcedureCalls*)this)->LookAt((*(CoverPoint*)LookAtPosition), 3);
                ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(1);
            }
        }
        break;
    }
    case 6: {
        if (actor_->PathFailed())
        {
            ((CombatProcedureEngageTarget*)this)->Fail(actor_->PathBuiltFailed() ? "Pathing failed while moving in and rotating"
                                           : "Path Building failed to move in and rotate");
            return;
        }
        if (actor_->PathFinished())
        {
            eState = COVER_STATE_WAITING_BEHIND_COVER;
            ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(2);
            (*(CoverClock*)&WaitBehindCoverTimer).FUN_008d7f40(GetCoverWaitTime((((CoverManager*)pCombatController))->GetCurrentCombatStyle()));
        }
        break;
    }
    case 7: {
        if (evaluatedA && !changedVal)
        {
            CoverRequest request;
            actor_->SetupPath(request, (*(CoverPoint*)CurrentCoverPosition), actor_->GetContainingCell(), actor_->GetWorld(), 0.0f, 0);
            if (!actor_->RequestPath(&request))
            {
                ((CombatProcedureEngageTarget*)this)->Fail("Cannot setup path to move back to Cover position");
                return;
            }
            ((CombatProcedureEngageTarget*)this)->SetMoveMode_00996e20(canFire4 && active8 ? 2 : 1);
            eState = COVER_STATE_MOVING_IN;
        }
        else if ((*(CoverClock*)&HoldingGroundTimer).FUN_008d7f80())
        {
            combat7->End(0);
            ((CombatProcedureEngageTarget*)this)->Fail(kCoverLOS);
            return;
        }
        else if ((*(CoverClock*)&HoldingGroundTimer).GetElapsedTime() > (*(CoverClock*)&HoldingGroundTimer).GetValue() / 4.0)
        {
            CoverPoint delta = targetTmp->GetPosition();
            CoverPoint targetPos = actor_->GetPosition();
            CoverPoint actorPos6 = targetPos - delta;
            if (actorPos6.FUN_004a7290() < 262144.0)
            {
                combat7->End(0);
                ((CombatProcedureEngageTarget*)this)->Fail(kCoverLOS);
            }
        }
        break;
    }
    }
}
