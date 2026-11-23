
// src/fallout/ai/combat/combatprocedure.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout/ai/combat/combatprocedure.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/Actor.h"
#include "../../../../include/ActorMover.h"
#include "../../../../include/BGSLoadFormBuffer.h"
#include "../../../../include/BGSSaveFormBuffer.h"
#include "../../../../include/CombatController.h"
#include "../../../../include/CombatProcedure.h"
#include "../../../../include/CombatProcedureHideFromTarget.h"
#include "../../../../include/CombatProcedureEngageTarget.h"
#include "../../../../include/CombatProcedureAttackGrenade.h"
#include "../../../../include/CombatProcedureAttackLow.h"
#include "../../../../include/CombatProcedureEvade.h"
#include "../../../../include/CombatProcedureAttackRanged.h"
#include "../../../../include/CombatProcedureSwitchWeapon.h"
#include "../../../../include/CombatProcedureMove.h"
#include "../../../../include/CombatProcedureActivateObject.h"
#include "../../../../include/CombatProcedureSearch.h"
#include "../../../../include/CombatProcedureUseCombatItem.h"
#include "../../../../include/NiPoint3.h"
#include "../../../../include/CombatProcedureAttackMelee.h"
#include "../../../../include/CombatProcedureBeInCover.h"

struct FUN_0097f450_shell
{
    void SetAttacker(unsigned char bAttacker);
};

// @fnv 0x0097f450
void FUN_0097f450_shell::SetAttacker(unsigned char bAttacker)
{
    *reinterpret_cast<unsigned char*>(this + 0x77) = bAttacker;
}

// Calls FUN_009ca6c0, deletes this when flags has bit 0 set, and returns this.
// @fnv 0x009ca6e0
// pdb: CombatProcedure::`scalar deleting destructor' | CombatProcedureActivateObject.obj | debug, vtable slot, medium
void* __thiscall CombatProcedure::DestroyIfFlagged(unsigned int flags)
{
    SetVirtualTable_009ca6c0();
    if (flags & 1)
        operator delete(this);
    return this;
}

struct FUN_009d1b10_shell
{
    char _pad[0x158];
    float m_fTarget;  // +0x158
    float GetTarget();
};

// @fnv 0x009d1b10
float FUN_009d1b10_shell::GetTarget()
{
    return m_fTarget;
}

struct FUN_009d4f30_shell
{
    void SetByteAtOffset188ToFF();
};

// @fnv 0x009d4f30
void FUN_009d4f30_shell::SetByteAtOffset188ToFF()
{
    *reinterpret_cast<unsigned char*>(reinterpret_cast<char*>(this) + 0x188) = 0xff;
}


// @fnv 0x00980070
// src: XBOX_PDB 0x00980070
CombatOptions* CombatController::GetCurrentCombatStyle()
{
    return reinterpret_cast<CombatOptions*>(pCurrentCombatStyle);
}


// The saved procedure status is at +0x08 in the shared base.
// @fnv 0x00996ed0
// pdb: CombatProcedure::SaveGame | CombatProcedure.obj | debug, vtable slot, medium
// pdb params: apSaveGameBuffer
void __thiscall CombatProcedure::SaveToBuffer(BGSSaveFormBuffer* apSaveGameBuffer)
{
    apSaveGameBuffer->SaveData(&eStatus, 4, 0);
}

// Passes the look-at point and priority to the combat controller, then applies the returned value via a second controller call.
// @fnv 0x00996e60
// pdb: CombatProcedure::SetLookAtTarget | CombatProcedure.obj | retail, call graph (shared callers), high
// pdb params: arLookAtPoint, acPriority
void CombatProcedure::SetLookAtPoint(const NiPoint3& arLookAtPoint, unsigned char acPriority)
{
    pCombatController->SetLookAtTarget(
        pCombatController->GetLookAtTargetIndex(
            this, arLookAtPoint, acPriority));
}

// Passes this procedure to the controller, then applies the returned look-at index.
// @fnv 0x00996ea0
// src: XBOX_PDB 0x00996ea0
// pdb: CombatProcedure::ClearLookAtTarget | CombatProcedure.obj | retail, call graph (shared callers), high
void __thiscall CombatProcedure::ClearLookAtTarget_00996ea0()
{
    pCombatController->ClearLookAtTarget_00981720(
        pCombatController->GetLookAtTargetIndex(
            static_cast<void*>(this)));
}

// @fnv 0x00996ef0
// src: XBOX_PDB 0x00996ef0
// pdb: CombatProcedure::LoadGame | CombatProcedure.obj | debug, vtable slot, medium
// pdb params: apLoadGameBuffer
void __thiscall CombatProcedure::LoadGame_00996ef0(BGSLoadFormBuffer* buffer)
{
    buffer->ReadTypedData(&eStatus, 4u);
}

// The melee unit calls 0x008b3a50 on an Actor; Actor::m_pActorMover is at +0x190,
// and mover slot +0x10 resets movement data. Actor.h has no wrapper declaration.
// @fnv 0x008b3a50
// pdb: Actor::ClearForcedMoveMode | Actor.obj | debug, call graph, medium
void __fastcall ResetActorMovement(Actor* actor)
{
    actor->pActorMover->ResetMovementData();
}


// @fnv 0x00996e00
// src: XBOX_PDB 0x00996e00
// pdb: CombatProcedure::GetTarget | CombatProcedure.obj | debug, call graph, medium
void* __thiscall CombatProcedure::GetTarget_00996e00()
{
    return pCombatController->CombatController_GetTarget();
}

// @fnv 0x00996e20
// src: XBOX_PDB 0x00996e20
// pdb: CombatProcedure::SetMoveMode | CombatProcedure.obj | debug, call graph, medium
// pdb params: aiCombatMoveMode
void __thiscall CombatProcedure::SetMoveMode_00996e20(int aiCombatMoveMode)
{
    pCombatController->SetMoveMode(aiCombatMoveMode);
}

// @fnv 0x00996e40
// src: XBOX_PDB 0x00996e40
// pdb: CombatProcedure::ClearMoveMode | CombatProcedure.obj | debug, upstream name already matches, high
void __thiscall CombatProcedure::ClearMoveMode()
{
    pCombatController->ResetCombatState();
}

void __cdecl Warning_005b5e40(int, const char*, int);
extern char DAT_0108ddac;
// Returns a new CombatProcedure subclass for kind values 0 through 12; otherwise calls FUN_005b5e40 and returns null.
// @fnv 0x00996f10
// src: XBOX_DEBUG_PDB 0x00996f10
// pdb: CombatProcedure::CreateProcedure | CombatProcedure.obj | debug, unique string literal, high
// pdb params: aiType
CombatProcedure* __cdecl CombatProcedure_CreateProcedure(int aiType)
{
    switch (aiType)
    {
    case 0:
        return new CombatProcedureAttackRanged();
    case 1:
        return new CombatProcedureAttackMelee();
    case 2:
        return new CombatProcedureAttackGrenade();
    case 3:
        return new CombatProcedureAttackLow();
    case 4:
        return new CombatProcedureEvade(0);
    case 5:
        return new CombatProcedureSwitchWeapon(0);
    case 6:
        return new CombatProcedureMove(0, 0.0f, 0);
    case 7:
        return new CombatProcedureBeInCover(0);
    case 8:
        return new CombatProcedureActivateObject(0, 0);
    case 9:
        return new CombatProcedureHideFromTarget(0, 0.0f, 0, 0);
    case 10:
        return new CombatProcedureSearch();
    case 11:
        return new CombatProcedureUseCombatItem(0);
    case 12:
        return new CombatProcedureEngageTarget();
    default:
        Warning_005b5e40(1, &DAT_0108ddac, aiType);
        return 0;
    }
}
