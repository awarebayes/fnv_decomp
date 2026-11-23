#pragma once

#include "ValueModifierEffect.h"

class Actor;

// pdb: class InvisibilityEffect, size 0x4c, bases: ValueModifierEffect, 0 members
class InvisibilityEffect : public ValueModifierEffect
{
public:
    InvisibilityEffect(void*, void*, void*);
    static void* operator new(size_t uiSize);
    static void operator delete(void* pvMem);
    InvisibilityEffect* CreateEffect();
    void __thiscall ValueModifierEffect_FinishInitLoadGame(Actor* actor);
    void __thiscall ApplyInvisibility(Actor* apActor);
    void RevertInvisibility(Actor* apActor);
};
