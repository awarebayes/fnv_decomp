#pragma once

#include "hkStringPtr.h"
#include "hkBool.h"

// pdb: class hkaBone, size 0x8, 2 members
class hkaBone
{
public:
    hkStringPtr m_name;
    hkBool m_bLockTranslation;

    hkaBone();
    static void* operator new(size_t, void* p);
    static void operator delete(void*, void*);
};
