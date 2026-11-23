#include "../../../include/SettingT.h"
// fallout/magic/limbconditioneffect.cpp: LimbConditionEffect, ValueModifierEffect (11 functions)

#include "../../../include/ActorValue.h"

#include "../../../include/LimbConditionEffect.h"
#include "../../../include/BGSLoadFormBuffer.h"
#include "../../../include/BGSSaveFormBuffer.h"
#include "../../../include/GameSettingCollection.h"
#include "../../../include/TESSaveLoadGame.h"

bool g_bLimbConditionEffectInitialized;

// @fnv 0x005b9210
bool GetLimbConditionEffectInitialized()
{
    return g_bLimbConditionEffectInitialized;
}

// @fnv 0x0080f2b0
// pdb: LimbConditionEffect::GetSaveSize | LimbConditionEffect.obj | debug, upstream name already matches, high
// pdb params: apActor
// pdb locals: isize
unsigned short __thiscall LimbConditionEffect::GetSaveSize(Actor* apActor)
{
    unsigned short isize = 0;
    isize += ValueModifierEffect::FUN_00802d90(apActor);
    isize += 4;
    return isize;
}

extern int DAT_0107a3fc;
extern "C" void ConstructValueModifierEffect();

// Constructs the value-modifier base, installs the game vtable and initializes the limb actor value.
// @fnv 0x0080f060
// src: EXE 0x0080f060
// pdb: LimbConditionEffect::~LimbConditionEffect | LimbConditionEffect.obj | debug, object-file run extension, low
LimbConditionEffect* LimbConditionEffect::LimbConditionEffect_0080f060(void* a, void* b, void* c)
{
    typedef ValueModifierEffect* (__thiscall* BaseConstructor)(ValueModifierEffect*, void*, void*, void*);
    ((BaseConstructor)&ConstructValueModifierEffect)(this, a, b, c);
    *(void**)this = (void*)&DAT_0107a3fc;
    eTargetBodyPart = -1;
    return this;
}

extern TESSaveLoadGame* g_TESSaveLoadGame;
// @fnv 0x0080f2f0
// pdb: ValueAndConditionsEffect::LoadGame | ValueAndConditionsEffect.obj | retail, vtable slot, high
// pdb params: apActor
void LimbConditionEffect::LoadGame(Actor* apActor)
{
    ValueModifierEffect::FUN_00802dd0(apActor);
    g_TESSaveLoadGame->ReadTypedData(&eTargetBodyPart, 4);
}

// @fnv 0x0080f320
// pdb: ValueAndConditionsEffect::SaveGame | ValueAndConditionsEffect.obj | retail, vtable slot, high
// pdb params: apActor
void LimbConditionEffect::SaveGame(BGSSaveFormBuffer* apSaveGameBuffer)
{
    ActiveEffect::SaveGame(apSaveGameBuffer);
    apSaveGameBuffer->SaveData(&eTargetBodyPart, 4, 0);
}

// Calls ActiveEffect::LoadGame, then for buffer version 8 or higher reads 4 bytes into the field at offset 0x4c.
// @fnv 0x0080f350
// pdb: ValueAndConditionsEffect::LoadGame | ValueAndConditionsEffect.obj | retail, vtable slot, high
// pdb params: apActor
void __thiscall LimbConditionEffect::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    ActiveEffect::LoadGame(apLoadGameBuffer);
    if (apLoadGameBuffer->GetVersion() >= 8)
    {
        apLoadGameBuffer->ReadTypedData(&eTargetBodyPart, 4u);
    }
}

// Copies the base value-modifier data from other, then, if other is a LimbConditionEffect, copies m_eModifiedActorValue into it.
// @fnv 0x0080f150
// pdb: LimbConditionEffect::CopyData | LimbConditionEffect.obj | retail, upstream name already matches, high
// pdb params: apDestination
// pdb locals: pvme
void __thiscall LimbConditionEffect::CopyData(ActiveEffect* apDestination)
{
    ValueModifierEffect::CopyData(apDestination);
    LimbConditionEffect* target = dynamic_cast<LimbConditionEffect*>(apDestination);
    if (target)
        target->eTargetBodyPart = eTargetBodyPart;
}

namespace fnv_0082ba30 {
extern TESSaveLoadGame* g_TESSaveLoadGame;
}  // namespace fnv_0082ba30

// @fnv 0x0082ba30
// src: XBOX_PDB 0x0082ba30
// pdb: ValueAndConditionsEffect::SaveGame | ValueAndConditionsEffect.obj | retail, vtable slot, high
// pdb params: apActor
void ValueModifierEffect::LimbConditionEffect_SaveGame(Actor* apActor)
{
    FUN_00802db0(apActor);
    fnv_0082ba30::g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD((int*)((char*)this + 0x4C), 4);
}

using ::SettingT;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting1213;
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting1214;
// @fnv 0x0080f220
// pdb: LimbConditionEffect::ModifyActorValue | LimbConditionEffect.obj | retail, vtable slot, high
// pdb params: apActor, afModifier, aeOverrideActorValue
// pdb locals: falteredMagnitude, i
void __thiscall LimbConditionEffect::ApplyActorValueModifier(Actor* apActor, float afModifier,
                                                           ActorValue::Index aeOverrideActorValue)
{
    if (eTargetBodyPart != ActorValue::ActorValue__Index_VALUE_NONE)
    {
        float fScaledMagnitude = afModifier * *originalRuntimeFloatSetting1213.Float();
        ValueModifierEffect::ApplyActorValueModifier(apActor, afModifier, (ActorValue::Index)eTargetBodyPart);
    }
    else
    {
        float fScaledMagnitude = afModifier * *originalRuntimeFloatSetting1214.Float();
        for (int i = ActorValue::ActorValue__Index_PERCEPTION_CONDITION; i < 32; ++i)
            ValueModifierEffect::ApplyActorValueModifier(apActor, fScaledMagnitude, (ActorValue::Index)i);
    }
}

// Allocates and constructs a limb-condition effect from three arguments.
// @fnv 0x0080f1a0
// src: COMMUNITY 0x0080f1a0
// pdb: LimbConditionEffect::Instantiate | LimbConditionEffect.obj | debug, object-file gap fill, medium
// pdb params: apCaster, apSpell, apEffect
LimbConditionEffect* LimbConditionEffect_Create(void* ap1, void* ap2, void* ap3)
{
    return new LimbConditionEffect(ap1, ap2, ap3);
}

void* __cdecl operator new(size_t);
extern int _data_011c16bc;

// @fnv 0x0080f0a0
// pdb: LimbConditionEffect::Copy | LimbConditionEffect.obj | retail, upstream name already matches, high
// pdb locals: presult
ActiveEffect* LimbConditionEffect::Copy()
{
    ActiveEffect* presult = new LimbConditionEffect(m_pEffectInstance, pSpell, pEffect);

    CopyData(presult);

    return presult;
}
