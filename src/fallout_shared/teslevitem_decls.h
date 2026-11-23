// fallout_shared/teslevitem_decls.h: declares SettingT_GameSettingCollection_, ContainerItemExtra, LeveledObject
#ifndef TESLEVITEM_DECLS_H
#define TESLEVITEM_DECLS_H

#include "../../include/SETTING_VALUE.h"
#include "../../include/SettingT_GameSettingCollection_.h"
class GlobalObject2_0040c150 : public SettingT_GameSettingCollection_
{
};
extern GlobalObject2_0040c150 g_011c8f00;
#include "layout_support.h"
#include "../../include/TESBoundObject.h"
#include "../../include/TESLeveledList.h"
// pdb: class ContainerItemExtra, size 0xc, 5 members
class ContainerItemExtra
{
  public:
    unsigned int pOwnerForm[3]; // src: XBOX_RETAIL_PDB +0x0 (old: data)
    static void* operator new(size_t);
    static void operator delete(void*);
    ContainerItemExtra();
    bool Load(TESFile*);
};
class LeveledObject
{
  public:
    unsigned short level;
    unsigned short pad;
    TESForm* form;
    unsigned short count;
    unsigned short pad2;
    void LEVELED_OBJECT_FILE_Endian();
};
extern LeveledObject* DAT_011c8f1c;
extern "C" void* __cdecl memset(void*, int, unsigned int);
extern "C" void* __cdecl _alloca(unsigned int);
#endif
