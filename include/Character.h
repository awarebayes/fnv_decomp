#pragma once

#include "Actor.h"

class BipedAnim;
class BGSLoadFormBuffer;

// pdb: class Character, size 0x1c8, bases: Actor, 6 members
class Character : public Actor
{
public:
    BipedAnim* pBiped;
    float fArmorRating; // src: XBOX_RETAIL_PDB +0x1b8 (old: m_fFirstResetValue)
    float fArmorThreshold; // src: XBOX_RETAIL_PDB +0x1bc (old: m_fSecondValue)
    bool bTrespassing; // src: XBOX_RETAIL_PDB +0x1c0 (old: m_bTrespassing)
    bool bGuard; // src: XBOX_RETAIL_PDB +0x1c1 (old: m_bActorGuard)
    unsigned short layoutPadding1C2;
    float fequippedweight; // src: XBOX_RETAIL_PDB +0x1c4 (old: m_fThirdValue)

    virtual void Unk_137(void);
    virtual void Unk_138(void);

    Character(bool);
    virtual ~Character();
    void* operator new(size_t);

    MagicTarget* GetMagicTarget();
    void InitLoadGame(BGSLoadFormBuffer* buffer);
    void ApplyLoadBuffer(BGSLoadFormBuffer* buffer);
    void Revert(BGSLoadFormBuffer* buffer);
    virtual void LoadGame(BGSLoadFormBuffer* buffer);
    void* GetCurrentMusicMarker();
    bool GetActorGuard();
    void SetActorGuardState(bool guard);
    void Reset3D();
};
