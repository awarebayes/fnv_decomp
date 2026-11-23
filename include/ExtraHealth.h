#pragma once

#include "BSExtraData.h"

// pdb: class ExtraHealth, size 0x10, bases: BSExtraData, 1 members
class ExtraHealth : public BSExtraData
{
public:
    bool Compare(BSExtraData* extra) override;

    ExtraHealth(float health);
    ExtraHealth(void* apCaster, void* apSpell, void* apEffect);

    static void* __cdecl operator new(size_t size);
    static void operator delete(void* pointer);

    float fHealthPercent;
};
