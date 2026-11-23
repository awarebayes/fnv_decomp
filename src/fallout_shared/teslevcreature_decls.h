// fallout_shared/teslevcreature_decls.h: local leveled-entry declarations
#pragma once

#include <malloc.h>
#include <string.h>
#include "../../include/TESFile.h"
#include "../../include/TESForm.h"
#include "../../include/TESBoundObject.h"
#include "../../include/TESLeveledList.h"
#include "../../include/TESModel.h"
#include "../../include/TESModelTextureSwap.h"


// pdb: class ContainerItemExtra, size 0xc, 5 members
class ContainerItemExtra
{
  public:
    char pOwnerForm[12]; // src: XBOX_RETAIL_PDB +0x0 (old: data)
    ContainerItemExtra();
    bool Load(TESFile*);
};
class LeveledObject
{
  public:
    unsigned short level, pad;
    TESForm* form;
    unsigned short count, pad2;
    void LEVELED_OBJECT_FILE_Endian();
};
struct LeveledEntry
{
    unsigned int a, b;
    ContainerItemExtra* extra;
};
extern LeveledEntry* DAT_011c8eec;
