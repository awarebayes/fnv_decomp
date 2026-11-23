// src/fallout/magic/cureeffect.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout/magic/cureeffect.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../include/ActiveEffect.h"
#include "../../../include/BSThread.h"
#include "../../../include/CureEffect.h"
#include "../../../include/MagicTarget.h"

// Keep only the missing removal declarations here; the layout is shared.
class CureEffectTarget : public MagicTarget
{
public:
    void MagicTarget_Dispel(int);
    void RemoveEffectsByArchetype(int, int);
};

// The shared CureEffect header does not yet declare these constructors.
class CureEffectConstruction : public CureEffect
{
public:
    CureEffectConstruction(MagicCaster*, MagicItem*, EffectItem*, MagicSystem__SpellType);
    CureEffectConstruction(MagicCaster*, MagicItem*, EffectItem*, int);
};

// @fnv 0x0080d4b0
// pdb: SummonCreatureEffect::CheckTarget | SummonCreatureEffect.obj | retail, vtable slot, high
// pdb params: apTarget
// pdb locals: pActor
bool CureEffect::IsEffectValid(MagicTarget* apTarget)
{
    Actor* pActor = apTarget ? apTarget->GetOwnerActor() : 0;
    return pActor != 0;
}

// @fnv 0x0080c210
// pdb: CureEffect::InstantiateCureDisease | CureEffect.obj | retail, object-file run extension, low
// pdb note: debug build links it to CureEffect::Start
// pdb params: apCaster, apSpell, apEffect
void CureEffect::Start()
{
    if (eArchetypeID == -1)
        static_cast<CureEffectTarget*>(GetTarget())->MagicTarget_Dispel(eSpellType);
    else
        static_cast<CureEffectTarget*>(GetTarget())->RemoveEffectsByArchetype(eArchetypeID, 0);
}


// @fnv 0x0080c0f0
// pdb: CureEffect::Copy | CureEffect.obj | retail, upstream name already matches, high
// pdb locals: presult
ActiveEffect* CureEffect::Copy()
{
    ActiveEffect* pEffect = 0;

    if (eArchetypeID == -1)
    {
        pEffect = new CureEffectConstruction(m_pEffectInstance, pSpell, this->pEffect, (MagicSystem__SpellType)eSpellType);
    }
    else
    {
        pEffect = new CureEffectConstruction(m_pEffectInstance, pSpell, this->pEffect, eArchetypeID);
    }

    this->CopyData(pEffect);
    return pEffect;
}
