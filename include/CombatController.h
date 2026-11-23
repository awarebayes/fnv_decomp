#pragma once

#include "TESPackage.h"
#include "NiAVObject.h"
#include <stddef.h>

class ActorCombatGroup;
class AnimIdle;
class Arg0Class;
class Arg1Class;
class Arg2Class;
class BGSSaveFormBuffer;
class BSThread;
class CombatActor;
class CombatLocations;
class CombatOptions;
class CombatPackage;
class CombatPlan;
class CombatProcedure;
class CombatState;
class PathingRequest;
class TESBoundObject;

// pdb: class CombatController, size 0x188, bases: TESPackage, 43 members
class CombatController : public TESPackage
{
public:
    class ProcedureArray
    {
    public:
        ProcedureArray();
        virtual ~ProcedureArray();
        CombatProcedure** data;
        unsigned int size;
        unsigned int capacity;

        unsigned int GetSize();
        CombatProcedure*& GetAt(int index);
        unsigned int Add(CombatProcedure* const& procedure);
        void Clear(bool freeMemory);
        void UpdateEntryValue(unsigned int index, unsigned int count);
    };

    struct CombatTimer
    {
        float x;
        float y;

        CombatTimer();
        void FUN_008d7f40(float duration);
        bool FUN_008d7f80();
    };

    struct Struct14A
    {
        int a, b, c, d, e;
    };

    struct Struct14B
    {
        int a, b, c, d, e;
    };

    struct Struct02
    {
        char a, b;
    };

    struct Struct3C
    {
        int s00, s04, s08, s0C, s10, s14, s18, s1C;
        int s20, s24, s28, s2C, s30, s34, s38;
    };

    // TESPackage occupies bytes 0x00 through 0x7F.
    union
    {
        ActorCombatGroup* m_pCombatData;                 // 0x80
        ActorCombatGroup* unk80;
        char pad[4];
    };
    union
    {
        CombatProcedure* m_pkCurrentProcedure;           // 0x84
        CombatProcedure* pProc84;
        CombatProcedure* unk84;
    };
    union
    {
        CombatProcedure* m_pkQueuedProcedure;            // 0x88
        CombatProcedure* pProc88;
        CombatProcedure* unk88;
    };
    ProcedureArray unk8C;                               // 0x8C
    union
    {
        CombatState* m_pModifier;                        // 0x9C
        CombatState* unk9C;
        CombatState* field_9c;
        CombatState* combat;
    };
    union { CombatPlan* m_pCombatPlan; CombatPlan* unkA0; }; // 0xA0
    int pCombatPlannerData;                                          // 0xA4 // src: XBOX_RETAIL_PDB +0xa4 (old: unkA4)
    union { BSThread* m_pCombatThread; BSThread* unkA8; }; // 0xA8
    int pCurrentGoal;                                          // 0xAC // src: XBOX_RETAIL_PDB +0xac (old: unkAC)
    union { int unkB0; int ib0; };                       // 0xB0
    CombatTimer unkB4;                                  // 0xB4
    union
    {
        Actor* m_pkAttacker;                             // 0xBC
        Actor* actor;
        Actor* pActor;
    };
    union { Actor* pTarget; Actor* field_c0; Actor* unkC0; }; // 0xC0
    unsigned char bForceCombat;                                // 0xC4 // src: XBOX_RETAIL_PDB +0xc4 (old: byteC4)
    union { unsigned char byteC5; char cc5; };            // 0xC5
    union { unsigned char byteC6; char cc6; };            // 0xC6
    union { unsigned char byteC7; char cc7; };            // 0xC7
    union
    {
        unsigned int m_uiCombatState;                    // 0xC8
        unsigned int unkC8;
        int ic8;
    };
    float floatCC;                                      // 0xCC
    float fAttackerSneakHeight;                                      // 0xD0 // src: XBOX_RETAIL_PDB +0xd0 (old: floatD0)
    bool bAttackerCanSneak;                                        // 0xD4 // src: XBOX_RETAIL_PDB +0xd4 (old: byteD4)
    bool bAttackerCanMove;                                        // 0xD5 // src: XBOX_RETAIL_PDB +0xd5 (old: byteD5)
    char unkD6[2];                                      // 0xD6
    union { float floatD8; int id8; };                   // 0xD8
    union { float floatDC; int idc; };                   // 0xDC
    float fCombatPosition;                                      // 0xE0 // src: XBOX_RETAIL_PDB +0xe0 (old: floatE0)
    union { unsigned char byteE4; char ce4; bool equipmentChanged; };            // 0xE4
    char unkE5[3];                                      // 0xE5
    NiPointer<AnimIdle> unkE8;                           // 0xE8
    NiPointer<AnimIdle> spCellDebugGeometry;                           // 0xEC // src: XBOX_RETAIL_PDB +0xec (old: unkEC)
    int pCurrentWeapon;                                          // 0xF0 // src: XBOX_RETAIL_PDB +0xf0 (old: unkF0)
    int pCurrentCombatStyle;                                          // 0xF4 // src: XBOX_RETAIL_PDB +0xf4 (old: unkF4)
    unsigned char bDestructing;                                // 0xF8 // src: XBOX_RETAIL_PDB +0xf8 (old: byteF8)
    unsigned char bDeterminedPowerAttackDeltas;                                // 0xF9 // src: XBOX_RETAIL_PDB +0xf9 (old: byteF9)
    char unkFA[2];                                      // 0xFA
    Struct14A unkFC;                                    // 0xFC
    Struct14B fPowerAttackHitTimes;                                   // 0x110 // src: XBOX_RETAIL_PDB +0x110 (old: unk110)
    unsigned char bHasForwardAttacks;                               // 0x124 // src: XBOX_RETAIL_PDB +0x124 (old: byte124)
    union { unsigned char byte125; bool byte_125; char c125; }; // 0x125
    union { unsigned char byte126; bool byte_126; char c126; }; // 0x126
    char unk127;                                        // 0x127
    NiPoint3 unk128[2];                                  // 0x128
    union
    {
        unsigned char m_aucStateByIndex[2];              // 0x140
        Struct02 unk140;
    };
    union { unsigned char byte142; char c142; };          // 0x142
    char unk143;                                        // 0x143
    union { int unk144; int i144; };                     // 0x144
    union { float unk148; float m_fCombatModifier; float lastUpdate; int i148; }; // 0x148
    Struct3C unk14C;                                    // 0x14C

    CombatController(Actor* apAttacker, ActorCombatGroup* apCombatGroup, bool apCurrentPackage);
    CombatController(void* apAttacker, ActorCombatGroup* apCombatGroup, void* apCurrentPackage);
    CombatController(Actor* apAttacker, int apCombatGroup, TESPackage* apCurrentPackage);
    void* operator new(size_t size);

    // Overrides of primary vtable slots 4, 21, 23, 25 and 33.
    virtual ~CombatController();
    virtual void SaveGame(BGSSaveFormBuffer* arg);
    virtual void LoadGame(BGSLoadFormBuffer* apLoadGameBuffer);
    virtual void Unk_19(void* arg);
    virtual void Unk_21(BGSLoadFormBuffer* arg);

    CombatActor* GetFieldBC();
    bool GetFieldC5();
    void SetByteAtOffsetC5(bool value);
    void SetFieldC7(unsigned char value);
    bool GetByteAtOffsetC7();
    void ClearCombatPlan();
    void UpdateCurrentCombatStyle();
    void DeterminePowerAttackDeltas();
    void ValidateCurrentWeapon();
    void ActivatedReference(TESObjectREFR* apRef, TESBoundObject* apObject, int aiCount,
                           bool abActivatedScriptOnly);
    void UpdateCombatEquipment(int first, int second);
    ActorCombatGroup* GetCombatGroup();
    void __thiscall ResetCombatState();
    char GetModifierKey(void);
    void StartCalmed();
    void UpdateMoveTarget();
    void UpdatePackageData(TESPackage* apCurrentPackage);
    bool GetLoading();
    bool SetActive(Actor* actor);

    CombatActor* CombatController_GetTarget();
    CombatPackage* FUN_004f9bf0();
    ActorCombatGroup* FUN_004fb070();
    CombatLocations* FUN_005e3fc0();
    bool TESPackage_GetContinueDuringCombat();
    void* GetCurrentWeapon();
    void bhkCharacterController_SetQuickSimulate(bool abTF);
    void bhkCharacterController_SetFarAway(bool abTF);
    void FUN_008d0600();
    int PlayerCharacter_AttemptYield();
    void* FUN_0097d740(unsigned int flags);
    void FUN_0097d770();
    void FirstUpdate();
    void CalculateAttackerInfo();
    void Update_0097da50();
    void __thiscall ForcePlanUpdate();
    void UpdateCurrentWeapon();
    void StopCombat_0097f4d0(int apTarget);
    bool StopCombat_0097f4d0(void* apTarget);
    void Disarmed_0097f6d0(int apOpponent);
    void FiredProjectile(int apProjectile);
    void ResetEmbeddedWeaponSwitchTimer(void* arg, bool value);
    void AddTarget_0097f930(Actor* apTarget, int iPriority, bool abHighPriority,
                      float afDamageDone, float afDamageTaken);
    void RemoveTarget_0097f9c0(void* actor);
    void RemoveTarget_0097f9c0(Actor* actor);
    CombatOptions* GetCurrentCombatStyle();
    void SetActionProcedure(CombatProcedure* apActionProcedure);
    void SetMovementProcedure(CombatProcedure* apMovementProcedure);
    void ClearProcedures();
    void AddBackgroundProcedure(CombatProcedure* apBackgroundProcedure);
    void ClearBackgroundProcedures();
    bool GetProcedureFailed();
    bool GetProcedureFinished();
    void SetTarget_00980830(void* apTarget);
    void ClearReferencePointers(int apRef);
    bool TESPackage_GetPretendInCombat();
    void FinishedPackageTask();
    bool ShouldStopCombat();
    void __thiscall SetMoveMode(int aiCombatMoveMode);
    void CombatController_SetActualMoveMode(unsigned int aiMoveMode);
    unsigned int GetLookAtTargetIndex(CombatProcedure* apCombatProcedure);
    int __thiscall GetLookAtTargetIndex(void* arg);
    int __thiscall GetLookAtTargetIndex(void* arg, const NiPoint3& point, unsigned char index);
    void __thiscall SetLookAtTarget(int arg);
    void __thiscall SetLookAtTarget(unsigned char acIndex, const NiPoint3& arLookAtTarget,
                                unsigned char acPriority);
    void __thiscall ClearLookAtTarget_00981720(unsigned char index);
    bool CombatController_CombatPlanContainsAction(int arg);
    bool IsFleeing_00981990();
    void SaveGame_009819f0(BGSSaveFormBuffer* buffer);
    void LoadGame_00981d00(BGSLoadFormBuffer* apLoadGameBuffer);
    void InitLoadGame_00982080(BGSLoadFormBuffer* apLoadGameBuffer);
    void FinishLoadGame_00982310(BGSLoadFormBuffer* buffer);
    void CombatProcedure_SaveGame_(CombatProcedure** procedure, BGSSaveFormBuffer* buffer);
    void FUN_00984f60(unsigned int flags);
    bool CombatController_GetAttackerCanSneak();
    bool FindBestAdvanceLocation(Arg0Class* a0, Arg1Class* a1, Arg2Class* a2, int a3,
                      const NiPoint3& a4, float a5, void* a6);
    void BuildCoveredMoveRequest(PathingRequest* request);
};

typedef char CombatControllerSizeCheck[1];
typedef char CombatControllerGroupOffsetCheck[1];
typedef char CombatControllerProcedureOffsetCheck[1];
typedef char CombatControllerStateOffsetCheck[1];
typedef char CombatControllerTimerOffsetCheck[1];
typedef char CombatControllerActorOffsetCheck[1];
typedef char CombatControllerFloatOffsetCheck[1];
typedef char CombatControllerIdleOffsetCheck[1];
typedef char CombatControllerRecordOffsetCheck[1];
typedef char CombatControllerPointsOffsetCheck[1];
typedef char CombatControllerIndicesOffsetCheck[1];
typedef char CombatControllerTailOffsetCheck[1];
