#pragma once

#include "TESProduceForm.h"
#include "TESObjectACTI.h"

// pdb: class TESFlora, size 0x9c, bases: TESProduceForm, TESObjectACTI, 0 members
class TESFlora : public TESProduceForm, public TESObjectACTI
{
public:
    TESFlora();

    void Copy_00508990(TESForm* apCopy);
};

typedef char TESFlora_size_check[1];
