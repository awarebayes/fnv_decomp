// fallout/magic/scripteffect.cpp: ScriptEffect (14 functions)

#include "../../../include/Actor.h"
#include "../../../include/ScriptEffect.h"
#include "../../../include/BGSSaveFormBuffer.h"
#include "../../../include/ScriptLocalVariable.h"
#include "../../../include/ScriptLocals.h"
#include "../../../include/TESSaveLoadGame.h"
#include "../../../include/TESForm.h"
#include "../../../include/ActiveEffect.h"
#include "../../../include/Script.h"
#include "../../../include/BGSLoadFormBuffer.h"

// @fnv 0x00827bd0
// pdb: ScriptEffect::Update | ScriptEffect.obj | retail, upstream name already matches, high
// pdb params: afSecondsElapsed
void ScriptEffect::Update(float afSecondsElapsed)
{
    if (pScript)
    {
        pScript->RunScriptEffectUpdate(FUN_0059bb30()->GetMagicTargetReference(), pEffectLocals, afSecondsElapsed);
    }
}
// Loads the script effect's saved state: reads a has-locals flag, then loads script locals into the cached ScriptLocals or a temporary one.
// @fnv 0x00827e60
// pdb: ScriptEffect::LoadGame | ScriptEffect.obj | retail, vtable slot, high
// pdb params: apActor
// pdb locals: isize
void ScriptEffect::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    ActiveEffect::LoadGame(apLoadGameBuffer);
    unsigned char hasLocals = 0;
    apLoadGameBuffer->ReadTypedData(&hasLocals, 1);
    if (hasLocals)
    {
        if (!pEffectLocals)
            pEffectLocals = pScript->GetOrCreateScriptLocals();
        if (pEffectLocals)
            pEffectLocals->LoadGame(apLoadGameBuffer);
        else
        {
            ScriptLocals locals;
            locals.LoadGame(apLoadGameBuffer);
        }
    }
}

// Reverts the form, then resets the cached script locals when present.
// @fnv 0x00827de0
// pdb: ScriptEffect::Revert | ScriptEffect.obj | retail, upstream name already matches, high
// pdb params: apActor
void ScriptEffect::Revert(Actor* apActor)
{
    ActiveEffect::Revert(apActor);
    if (pEffectLocals)
        pEffectLocals->LoadGame();
}

void* __cdecl operator new(size_t);

struct SCRIPT_LOCAL_LIST
{
    ScriptLocalVariable* item;
    SCRIPT_LOCAL_LIST* next;
    bool FUN_008256d0();
    ScriptLocalVariable*& FUN_006815c0();
    SCRIPT_LOCAL_LIST* FUN_00726070();
};

struct ScriptEventListStub
{
    int a;
    int b;
};

// @fnv 0x00827930
// pdb: ScriptEffect::CopyData | ScriptEffect.obj | retail, upstream name already matches, high
// pdb params: apDestination
// pdb locals: pse, plist
void ScriptEffect::CopyData(ActiveEffect* apDestination)
{
    ActiveEffect::CopyData(apDestination);
    ScriptEffect* pse = dynamic_cast<ScriptEffect*>(apDestination);
    if (pse)
    {
        pse->pScript = pScript;
        if (pScript && pEffectLocals)
        {
            pse->pEffectLocals = pScript->GetOrCreateScriptLocals();
            pse->pEffectLocals->CopyActionListFrom(pEffectLocals);
            SCRIPT_LOCAL_LIST* pLocalEntry = (SCRIPT_LOCAL_LIST*)pEffectLocals->GetLocalList();
            while (pLocalEntry && !pLocalEntry->FUN_008256d0())
            {
                pse->pEffectLocals->SetVariable(pLocalEntry->FUN_006815c0()->uiID, pLocalEntry->FUN_006815c0()->fValue);
                pLocalEntry = pLocalEntry->FUN_00726070();
            }
            if (pEffectLocals->m_pScriptEffectData)
                pse->pEffectLocals->m_pScriptEffectData = reinterpret_cast<ScriptDefinition*>(new ScriptEventListStub);
        }
    }
}

// Deletes the cached script locals before destroying the active-effect base.
// @fnv 0x008277f0
// pdb: ScriptEffect::~ScriptEffect | ScriptEffect.obj | debug, upstream name already matches, high
ScriptEffect::~ScriptEffect()
{
    if (pEffectLocals)
        delete pEffectLocals;
}

// Allocates and constructs a ScriptEffect from the caster, magic item and effect item.
// @fnv 0x00827a40
// src: XBOX_RETAIL_PDB 0x00827a40
// pdb: ScriptEffect::Instantiate | ScriptEffect.obj | retail, object-file run extension, high
// pdb params: apCaster, apSpell, apEffect
ScriptEffect* ScriptEffect_Instantiate(MagicCaster* apCaster, MagicItem* apSpell, EffectItem* apEffect)
{
    return new ScriptEffect(apCaster, apSpell, apEffect);
}

extern TESSaveLoadGame* g_TESSaveLoadGame;
// @fnv 0x00827c20
// pdb: ScriptEffect::GetSaveSize | ScriptEffect.obj | retail, upstream name already matches, high
// pdb params: apActor
// pdb locals: isize
unsigned short ScriptEffect::GetSaveSize(Actor* apActor)
{
    unsigned short usSaveSize = 0;
    usSaveSize += ActiveEffect::GetSaveSize(apActor);
    if (g_TESSaveLoadGame->FUN_008df040() >= 0x2a)
        usSaveSize += 2;
    if (pEffectLocals)
        usSaveSize += pEffectLocals->GetSaveSize();
    return usSaveSize;
}

extern unsigned int _data_011c16bc;

// @fnv 0x00827880
// pdb: ScriptEffect::Copy | ScriptEffect.obj | retail, upstream name already matches, high
// pdb locals: presult
ActiveEffect* ScriptEffect::Copy()
{
    ScriptEffect* pEffect = new ScriptEffect(m_pEffectInstance, pSpell, this->pEffect);

    CopyData(pEffect);

    return pEffect;
}

struct ScriptExtended
{
    void RunScriptEffectStart(TESObjectREFR*);
};
struct TargetContext
{
    virtual void unused();
    virtual TESObjectREFR* GetActor(ScriptLocals*);
};
// If pScript is set, gets its ScriptLocals and passes the actor from the target context to ScriptExtended::RunScriptOnTarget.
// @fnv 0x00827ac0
// pdb: ScriptEffect::Start | ScriptEffect.obj | retail, upstream name already matches, high
void ScriptEffect::Start()
{
    if (pScript != 0)
    {
        pEffectLocals = pScript->GetOrCreateScriptLocals();
        ((ScriptExtended*)pScript)->RunScriptEffectStart(((TargetContext*)FUN_0059bb30())->GetActor(pEffectLocals));
    }
}

// The shared no-buffer overload names retain their legacy symbol bindings.
void __fastcall WriteScriptLocals(ScriptLocals* locals);

// @fnv 0x00827ca0
// pdb: ScriptEffect::SaveGame | ScriptEffect.obj | retail, vtable slot, high
// pdb params: apActor
// pdb locals: isize
void ScriptEffect::SaveGame(Actor* apActor)
{
    ActiveEffect::SaveGame(apActor);
    unsigned short count = 0;
    if (g_TESSaveLoadGame->FUN_008df040() >= 0x2a)
    {
        if (pEffectLocals)
        {
            count = pEffectLocals->GetSaveSize();
        }
        g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&count, 2u);
    }
    if (g_TESSaveLoadGame->FUN_008df040() < 0x2a)
    {
        count = 1;
    }
    if (count)
    {
        WriteScriptLocals(pEffectLocals);
    }
}

// @fnv 0x00827d30
// pdb: ScriptEffect::LoadGame | ScriptEffect.obj | retail, vtable slot, high
// pdb params: apActor
// pdb locals: isize
void ScriptEffect::LoadGame(Actor* apActor)
{
    ActiveEffect::LoadGame(apActor);
    unsigned short flag = 0;
    if (g_TESSaveLoadGame->FUN_008df040() >= 42)
    {
        g_TESSaveLoadGame->TESSaveLoadGame_LoadGameDataOLD(&flag, 2);
    }
    if (g_TESSaveLoadGame->FUN_008df040() < 42)
    {
        flag = 1;
    }
    if (flag)
    {
        if (pEffectLocals == 0)
        {
            pEffectLocals = pScript->GetOrCreateScriptLocals();
        }
        if (pEffectLocals != 0)
        {
            pEffectLocals->SaveGame();
        }
        else
        {
            g_TESSaveLoadGame->FUN_00857bd0(flag);
        }
    }
}

// Saves the base effect and the cached locals when present.
// @fnv 0x00827e10
// pdb: ScriptEffect::SaveGame | ScriptEffect.obj | retail, vtable slot, high
// pdb params: apActor
// pdb locals: isize
void ScriptEffect::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    ActiveEffect::SaveGame(apSaveGameBuffer);

    bool bValue = pEffectLocals;
    apSaveGameBuffer->FUN_00865e50(&bValue, 1, 0);

    if (bValue)
        pEffectLocals->SaveGame(apSaveGameBuffer);
}

struct Cell827b
{
    virtual void v00();
    virtual void* v01(ScriptLocals* apLocals);
};
struct FUN_005ac380_t
{
    void Script_RunScriptEffectFinish(void*);
};
// @fnv 0x00827b20
// pdb: ScriptEffect::Finish | ScriptEffect.obj | retail, upstream name already matches, high
void ScriptEffect::Finish()
{
    if (pScript && (!g_TESSaveLoadGame->FUN_0047c850() ||
                    g_TESSaveLoadGame->FUN_0047c850()))
    {
        ((FUN_005ac380_t*)pScript)->Script_RunScriptEffectFinish(((Cell827b*)FUN_0059bb30())->v01(pEffectLocals));
    }
    if (pEffectLocals)
    {
        delete pEffectLocals;
        pEffectLocals = 0;
    }
}
