// src/fallout/magic/valueandconditionseffect.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout/magic/valueandconditionseffect.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../include/ActorValue.h"
#include "../../../include/Setting.h"
#include "../../../include/TESSaveLoadGame.h"
#include "../../../include/ValueAndConditionsEffect.h"

extern TESSaveLoadGame* g_TESSaveLoadGame;

// @fnv 0x0082b780
// pdb: TurnUndeadEffect::LoadGame | TurnUndeadEffect.obj | retail, vtable slot, high
// pdb params: apActor
void ValueAndConditionsEffect::LoadGame(void* apActor)
{
    ActiveEffect::LoadGame(apActor);
    g_TESSaveLoadGame->ReadTypedData(&eWhichActorValue, 1);
}

// @fnv 0x0082b7b0
// pdb: ValueAndConditionsEffect::ValueAndConditionsEffect | ValueAndConditionsEffect.obj | debug, upstream name already matches, high
// pdb params: apCaster, apSpell, apEffect
ValueAndConditionsEffect::ValueAndConditionsEffect(void* apCaster, void* apSpell, void* apEffect) : ValueModifierEffect(apCaster, apSpell, apEffect)
{
    eTargetBodyPart = ActorValue::ActorValue__Index_VALUE_NONE;
}

extern Setting originalRuntimeFloatSetting1213;
extern Setting originalRuntimeFloatSetting1214;

// Applies the modifier to the stored actor value (or none) and, scaled by a runtime setting, to ActorValue__Index_VALUE_NONE or actor values 25-31.
// @fnv 0x0082b970
// pdb: ValueAndConditionsEffect::ModifyActorValue | ValueAndConditionsEffect.obj | retail, vtable slot, high
// pdb params: apActor, afModifier, aeOverrideActorValue
// pdb locals: falteredMagnitude, i
void ValueAndConditionsEffect::ApplyActorValueModifier(Actor* apActor, float afModifier, ActorValue::Index aeOverrideActorValue)
{
    if (eTargetBodyPart != ActorValue::ActorValue__Index_VALUE_NONE)
    {
        float fScaledMagnitude = afModifier * originalRuntimeFloatSetting1213.Float()[0];
        ValueModifierEffect::ApplyActorValueModifier(apActor, afModifier, eTargetBodyPart);
        ValueModifierEffect::ApplyActorValueModifier(apActor, fScaledMagnitude, ActorValue::ActorValue__Index_VALUE_NONE);
    }
    else
    {
        ValueModifierEffect::ApplyActorValueModifier(apActor, afModifier, ActorValue::ActorValue__Index_VALUE_NONE);
        float fScaledMagnitude = afModifier * originalRuntimeFloatSetting1214.Float()[0];
        for (int i = 25; i < 32; ++i)
            ValueModifierEffect::ApplyActorValueModifier(apActor, fScaledMagnitude, (ActorValue::Index)i);
    }
}

extern int DAT_011a13d0;
extern int DAT_011a0d1c;
void* __cdecl CreateSpawnedRecord(void* p, int a2, void* a3, void* a4, int a5);

// @fnv 0x0082b8a0
// pdb: ValueAndConditionsEffect::CopyData | ValueAndConditionsEffect.obj | retail, upstream name already matches, high
// pdb params: apDestination
// pdb locals: pvme
void ValueAndConditionsEffect::CopyData(ActiveEffect* apDestination)
{
    ValueModifierEffect::CopyData(apDestination);
    ValueAndConditionsEffect* copy = (ValueAndConditionsEffect*)CreateSpawnedRecord(apDestination, 0, &DAT_011a0d1c, &DAT_011a13d0, 0);
    if (copy != 0)
        copy->eTargetBodyPart = eTargetBodyPart;
}

void* __cdecl operator new(size_t size);

// @fnv 0x0082b8f0
// pdb: ValueAndConditionsEffect::Instantiate | ValueAndConditionsEffect.obj | debug, object-file gap fill, medium
// pdb params: apCaster, apSpell, apEffect
ValueAndConditionsEffect* CreateValueModifier(void* a, void* b, void* c)
{
    return new ValueAndConditionsEffect(a, b, c);
}

// @fnv 0x0082b7f0
// pdb: ValueAndConditionsEffect::Copy | ValueAndConditionsEffect.obj | retail, upstream name already matches, high
// pdb locals: presult
ActiveEffect* ValueAndConditionsEffect::Copy()
{
    ValueAndConditionsEffect* effect = new ValueAndConditionsEffect(m_pEffectInstance, pSpell, pEffect);
    CopyData(effect);
    return effect;
}
