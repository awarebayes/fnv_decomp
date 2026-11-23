#pragma once

#include "ActiveEffect.h"

// pdb: class DemoralizeEffect, size 0x4c, bases: ActiveEffect, 1 members
class DemoralizeEffect : public ActiveEffect
{
public:
    DemoralizeEffect(MagicCaster*, MagicItem*, EffectItem*);

    ActiveEffect* Copy() override;
    unsigned short GetSaveSize(Actor*) override;
    void SaveGame(Actor*) override;
    bool IsEffectValid(MagicTarget*) override;
    void Start() override;
    void Finish() override;

    MagicCaster* FUN_0045cd60();
    static void* operator new(size_t);
    static void operator delete(void*);

    unsigned char bTurned; // src: XBOX_RETAIL_PDB +0x48 (old: m_bDemoralized)
};
