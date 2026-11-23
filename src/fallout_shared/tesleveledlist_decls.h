// Remaining local declarations for tesleveledlist_ob1_variant.cpp.
#ifndef FNV_TESLEVELEDLIST_WORKER_DECLS_H
#define FNV_TESLEVELEDLIST_WORKER_DECLS_H

extern unsigned int DAT_010a2480;

// pdb: class ContainerItemExtra, size 0xc, 5 members
class ContainerItemExtra
{
  public:
    unsigned int pOwnerForm[3]; // src: XBOX_RETAIL_PDB +0x0 (old: data)
    ContainerItemExtra();
    void ContainerItemExtra_AdjustHealthPercent(float);
    void InitItem(TESForm*);
};
class LeveledObject;

class TESGlobalValueReader
{
  public:
    float GetValue();
};

class PerkEntryConditionSet;
class PlayerQuestData
{
  public:
    PerkEntryConditionSet* GetConditions();
};
class BGSCameraShot;
class ActorCause;

class MersenneTwister
{
  public:
    static MersenneTwister* GetRandomGenerator();
    unsigned int GetRandomUInt();
};
unsigned int DialogueRandom();

void* operator new(size_t) throw();

int Warning(const char*, ...);

#endif
