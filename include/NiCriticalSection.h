#pragma once

#include <stddef.h>

// pdb: class NiCriticalSection, size 0x80, 1 members
class NiCriticalSection
{
public:
    unsigned char m_kCriticalSection[24]; // Opaque Win32 CRITICAL_SECTION storage.
    unsigned char unk18[4];
    unsigned long m_ulThreadOwner;
    unsigned int m_uiLockCount;
    unsigned char layoutPadding24[92];

    void Lock(const char* pcFile = 0);
    void Unlock();
    void SleepWrapper(int iMS);
};

typedef char NiCriticalSectionSizeCheck[1];
typedef char NiCriticalSectionOwnerOffsetCheck[1];
typedef char NiCriticalSectionCountOffsetCheck[1];
