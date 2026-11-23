// fallout_shared/tesidlemanager_decls.h: local idle manager declarations
#ifndef TESIDLEMANAGER_DECLS_H
#define TESIDLEMANAGER_DECLS_H

#include "../../include/ANIM_GROUP_WEAPON_ENUM.h"

#include "../../include/BSSimpleList.h"

class IdleAnimationDirectoryEntry
{
    unsigned char unused[0x38];

  public:
    BSSimpleList<const char*>* m_ppFileListsByWeapon[12];
    IdleAnimationDirectoryEntry(const char*);
    void IDLE_ANIM_ROOT_AddRootFilenameList(BSSimpleList<const char*>* apList);
};

#include "../../include/ANIM_GROUP_MOVE_ENUM.h"

class BGSCameraPath;
template <class T> class NiTMallocInterface;

#include "../../include/NiTArray.h"

class BGSCameraPath;

class IdleReplayDelayEntry;

class TESObject;

class AnimData
{
  public:
    unsigned char unused[0x134];
    BSSimpleList<IdleReplayDelayEntry*> replayDelays;
};

// pdb: class TESIdleManager, size 0x28, 3 members
class TESIdleManager : public NiTMap<const char*, IdleAnimationDirectoryEntry*> // src: XBOX_RETAIL_PDB 0x00600170 (first retail link of 6 high-tier links to its methods; old: IdleAnimationManager)
{
    // The map occupies 0x10 bytes; the manager's loaded flag is at 0x14.
    unsigned char unused10[4];

  public:
    unsigned char bInitialized; // src: XBOX_RETAIL_PDB +0x14 (old: loadedIdles)
    unsigned char padLoadedIdles[3];
    NiTArray<BGSCameraPath*, NiTMallocInterface<BGSCameraPath*> > LoadedIdleArray; // src: XBOX_RETAIL_PDB +0x18 (old: idleForms)
    NiFormArray* GetLooseIdleArray(const char* directory);
    NiFormArray* GetIdleArray(TESForm* apForm);
    void InitializeIdleAnimations();
    NiFormArray* GetRootIdleArray(const char*);
    TESIdleForm* FindIdleFormForObject(TESObjectREFR*, TESObjectREFR*);
    IdleAnimationDirectoryEntry* AddRootIdleArray(const char*, BSSimpleList<const char*>*, bool);
    void InsertOrUpdate(const char*, IdleAnimationDirectoryEntry*);
    void ReleaseStorage_00600030();
    BSSimpleList<const char*>* FindIdleArrayEntry(const char* apIdleDir, ANIM_GROUP_WEAPON_ENUM aeWeaponGroup);
    static void GetIdleDirName(const char* apInDir, char* apOutDir);
    static TESObject* GetCurrentUsedItem();
    static bool GetUsedItemActivate();
};

extern "C" TESObject* fnv_tesidlemanager_used_item;
extern "C" bool fnv_tesidlemanager_used_item_activate;
extern "C" const char fnv_tesidlemanager_idle_anims[];
// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
class TaskQueueInterface
{
  public:
    static void CopyStringToBuffer(char*, unsigned int, const char*);
};
char* strrchr_0040ab30(char*, int);
char* FUN_004812f0(const char*, const char*);
void FUN_00406d50(char*, unsigned int, const char*);
char* strrchr(const char* string, int character);
namespace CRT {
extern "C" char* strrchr(const char* string, int character);
extern "C" int _stricmp(const char* left, const char* right);
}
extern "C" char* strstr(const char* string, const char* substring);
int strcat_s(char* destination, unsigned int size, const char* source);
extern "C" const char fnv_idle_1hm_fast_left[];
extern "C" const char fnv_idle_1hm_left[];
extern "C" const char fnv_idle_1hp_fast_left[];
extern "C" const char fnv_idle_1hp_left[];
template <class T> unsigned int BSStringLength(const T* string);

void* operator new(size_t);
// pdb: class ModelLoader, size 0x30 (here 0x1), 12 members
class ModelLoader
{
  public:
    BSSimpleList<const char*>* FindNIFFileListEntry(BSSimpleList<const char*>*, BSSimpleList<const char*>*);
    BSSimpleList<const char*>* GetModelFileList(const char*, bool, bool, int);
};
extern ModelLoader* g_modelLoader;
extern bool DAT_011cb71c;
#endif
