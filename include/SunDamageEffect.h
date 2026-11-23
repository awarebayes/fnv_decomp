#pragma once

#include "ActiveEffect.h"

// pdb: class SunDamageEffect, size 0x50, bases: ActiveEffect, 3 members
class SunDamageEffect : public ActiveEffect
{
public:
    static void* operator new(size_t size);
    static void operator delete(void* p);

    SunDamageEffect(MagicCaster*, MagicItem*, EffectItem*);

    ActiveEffect* Copy() override;
    unsigned short GetSaveSize(Actor* apActor) override;
    void SaveGame(Actor* apActor) override;

    void LoadGame(Actor* apActor) override;
    void UpdateEffect(float afSecondsElapsed);
    bool IsCausingDamage();
    double GetDamageAmount();
    void UpdateHitEffect(bool abDamaging);
    int FUN_0055b980();

    float fGlowEffect; // src: XBOX_RETAIL_PDB +0x48 (old: fValue)
    bool bHitEffectApplied; // src: XBOX_RETAIL_PDB +0x4c (old: bFlag4C)
    bool bDoNotTakeDamageNextUpdate; // src: XBOX_RETAIL_PDB +0x4d (old: bFlag4D)
};
