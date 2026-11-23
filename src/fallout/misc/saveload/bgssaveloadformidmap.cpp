// fallout/misc/saveload/bgssaveloadformidmap.cpp: FormIDConverter (10 functions)

#include "FormIDConverter.h"

// @fnv 0x00846b60
// pdb: BGSSaveLoadFormIDMap::BGSSaveLoadFormIDMap | BGSSaveLoadFormIDMap.obj | retail, call graph, high
// pdb params: iBucketCount
BGSSaveLoadFormIDMap::BGSSaveLoadFormIDMap(unsigned int iBucketCount)
    : FormIDToIndexMap(iBucketCount), IndexToFormIDMap(iBucketCount)
{
    iCurrentIndex = 0;
    AddFormID(0);
}

// Destructor that clears the two member containers via RemoveAll.
// @fnv 0x00846be0
// pdb: BGSSaveLoadFormIDMap::~BGSSaveLoadFormIDMap | BGSSaveLoadFormIDMap.obj | debug, object-file gap fill, medium
BGSSaveLoadFormIDMap::~BGSSaveLoadFormIDMap()
{
    FormIDToIndexMap.RemoveAll();
    IndexToFormIDMap.RemoveAll();
}

// @fnv 0x00846c50
// pdb: BGSSaveLoadFormIDMap::Clear | BGSSaveLoadFormIDMap.obj | debug, object-file gap fill, medium
void BGSSaveLoadFormIDMap::Clear()
{
    FormIDToIndexMap.RemoveAll();
    IndexToFormIDMap.RemoveAll();
    iCurrentIndex = 0;
    AddFormID(0);
}

// @fnv 0x00846c90
// src: XBOX_RETAIL_PDB 0x00846c90
// pdb: BGSSaveLoadFormIDMap::AddFormID | BGSSaveLoadFormIDMap.obj | retail, call graph (shared callers), high
// pdb params: aiFormID
// pdb locals: iindex
unsigned int BGSSaveLoadFormIDMap::AddFormID(unsigned int aiFormID)
{
    if (g_dataHandler->FormIDExists(aiFormID))
        return aiFormID;

    unsigned int index = 0;
    if (FormIDToIndexMap.GetAt(aiFormID, index))
        return index;

    index = iCurrentIndex++;
    FormIDToIndexMap.SetAt(aiFormID, index);
    IndexToFormIDMap.SetAt(index, aiFormID);
    return index;
}

struct BSFile;
struct SaveLoadFile
{
    unsigned int Read(void*, unsigned int);
    unsigned int Write(void*, unsigned int);
    bool Open();
    unsigned int GetFilePosition();
    unsigned int GetFileSize();
    void SeekSet(int);
    void SeekCurrent(int);
    char* GetFileName();
    static SaveLoadFile* CreateFileA(char*, bool, unsigned int);
    void SetSaveFileName(char*);
    char* GetSaveFileName();
    BSFile* QFile();
    unsigned int GetSimulatedFileSize();
    void* __vecDelDtor(unsigned int);
    char m_cFileName[260];
    BSFile* pFile;
    unsigned int m_uiBufferSize;
    unsigned int m_uiFilePosition;
};
typedef char _sizecheck_BGSSaveLoadFile[1];
struct FUN_00846e70_shell : public BGSSaveLoadFormIDMap
{
    void BGSSaveLoadFormIDMap_Load(SaveLoadFile*);
};

struct FUN_00846d80_cls
{
    unsigned int RemapFormID(unsigned int);
};
void __cdecl ReadFileIntoBuffer(SaveLoadFile*, unsigned int*, unsigned int);
// Clears the map, reads a count and that many form IDs from the file, converts each and stores both directions in the two maps.
// @fnv 0x00846e70
// src: XBOX_RETAIL_PDB 0x00846e70
// pdb: BGSSaveLoadFormIDMap::Load | BGSSaveLoadFormIDMap.obj | retail, call graph, high
// pdb params: apFile
// pdb locals: icount, i, iconvertedFormID, iformID, itempIndex
void FUN_00846e70_shell::BGSSaveLoadFormIDMap_Load(SaveLoadFile* apFile)
{
    Clear();
    unsigned int icount = 0;
    ReadFileIntoBuffer(apFile, &icount, 4);
    for (unsigned int i = 0; i < icount; i++)
    {
        unsigned int iformID = 0;
        ReadFileIntoBuffer(apFile, &iformID, 4);
        unsigned int uiConvertedFormID = ((FUN_00846d80_cls*)this)->RemapFormID(iformID);
        if (uiConvertedFormID)
            FormIDToIndexMap.SetAt(uiConvertedFormID, iCurrentIndex);
        IndexToFormIDMap.SetAt(iCurrentIndex, uiConvertedFormID);
        iCurrentIndex++;
    }
}

struct FUN_00846de0_cls
{
    unsigned char __thiscall GetByteAtOffset(unsigned char param_1);
};

// @fnv 0x00846de0
unsigned char __thiscall FUN_00846de0_cls::GetByteAtOffset(unsigned char param_1)
{
    return ((unsigned char*)this)[param_1 + 0x44];
}

extern FUN_00846de0_cls* g_BGSSaveLoadGame;
// Remaps a form ID's mod index through g_BGSSaveLoadGame; returns the original ID if the index is 0xFF, or 0 if the remap fails.
// @fnv 0x00846d80
// pdb: BGSSaveLoadFormIDMap::ConvertFormID | BGSSaveLoadFormIDMap.obj | debug, object-file run extension, low
// pdb params: aiFormID
// pdb locals: cnewFileIndex, cfileIndex
unsigned int FUN_00846d80_cls::RemapFormID(unsigned int formID)
{
    unsigned char modIndex = (unsigned char)(formID >> 24);
    if (modIndex == 0xFF)
        return formID;
    {
        unsigned char newModIndex = g_BGSSaveLoadGame->GetByteAtOffset(modIndex);
        if (newModIndex != 0xFF)
            return (formID & 0xFFFFFF) | (newModIndex << 24);
        return 0;
    }
}

struct BGSSaveLoadFormIDMap;
struct FUN_00846d20_shell : public BGSSaveLoadFormIDMap
{
    unsigned int BGSSaveLoadFormIDMap_GetFormID(unsigned int aiFormIDIndex);
};
// @fnv 0x00846d20
// src: XBOX_PDB 0x00846d20
// pdb: BGSSaveLoadFormIDMap::GetFormID | BGSSaveLoadFormIDMap.obj | retail, upstream name already matches, high
// pdb params: aiFormIDIndex
// pdb locals: iformID
unsigned int FUN_00846d20_shell::BGSSaveLoadFormIDMap_GetFormID(unsigned int aiFormIDIndex)
{
    if (g_dataHandler->FormIDExists(aiFormIDIndex))
        return aiFormIDIndex;
    unsigned int result = 0;
    if (((NiTPointerMap<unsigned int, unsigned int>*)((char*)this + 0x10))->GetAt(aiFormIDIndex, result))
        return result;
    return 0;
}

struct SlotItem
{
    unsigned int m_uKey;
    void* m_pInventoryEntryData;
};

struct CleanupItem
{
    unsigned int FreeItem(unsigned int flag);
};

extern void __cdecl SlotFree(void* ptr);
extern void __cdecl TailClear(void* ptr);

class FUN_0044ee40_cls
{
  public:
    unsigned int unk00;
    void* m_pTailObj;
    SlotItem* m_pSlots;
    unsigned int GetFriendHitSlotCount();
    void ClearSlots();
};

// Frees each slot's item, frees the slot array, then clears the tail object.
// @fnv 0x0044ee40
void FUN_0044ee40_cls::ClearSlots()
{
    for (unsigned int u = 0; u < GetFriendHitSlotCount(); u++)
    {
        void* pItem = m_pSlots[u].m_pInventoryEntryData;
        void* RunFourthVirtual = pItem;
        RunFourthVirtual != 0 ? ((CleanupItem*)RunFourthVirtual)->FreeItem(1) : 0;
    }
    {
        SlotItem* a = m_pSlots;
        SlotFree(a);
    }
    TailClear(m_pTailObj);
}

class BSFile;
namespace fnv_00846e00 {
struct SaveLoadFile
{
    unsigned int Read(void*, unsigned int);
    unsigned int Write(void*, unsigned int);
    bool Open();
    unsigned int GetFilePosition();
    unsigned int GetFileSize();
    void SetFilePosition(int);
    void SeekCurrent(int);
    char* GetFileName();
    static SaveLoadFile* CreateFileA(char*, bool, unsigned int);
    void SetSaveFileName(char*);
    char* GetSaveFileName();
    BSFile* QFile();
    unsigned int GetSimulatedFileSize();
    void* __vecDelDtor(unsigned int);
    char m_cFileName[260];
    BSFile* pFile;
    unsigned int m_uiBufferSize;
    unsigned int m_uiFilePosition;
};
typedef char _sizecheck_BGSSaveLoadFile[1];

struct FUN_00846d20_shell : public BGSSaveLoadFormIDMap
{
    unsigned int BGSSaveLoadFormIDMap_GetFormID(unsigned int aiFormIDIndex);
};
void __cdecl ReadFileIntoBuffer(SaveLoadFile*, unsigned int*, unsigned int);

struct FUN_00846e00_cls : public BGSSaveLoadFormIDMap
{
    void BGSSaveLoadFormIDMap_Save(SaveLoadFile* apFile);
};

// Passes the count minus one and each converted index value to FUN_00845d20 with file and size 4.
// @fnv 0x00846e00
// src: COMMUNITY 0x00846e00
// pdb: BGSSaveLoadFormIDMap::Save | BGSSaveLoadFormIDMap.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: icount, i, iformID
void FUN_00846e00_cls::BGSSaveLoadFormIDMap_Save(SaveLoadFile* apFile)
{
    unsigned int count = this->iCurrentIndex - 1;
    ReadFileIntoBuffer(apFile, &count, 4);
    for (unsigned int i = 1; i < this->iCurrentIndex; i++)
    {
        unsigned int id = ((FUN_00846d20_shell*)this)->BGSSaveLoadFormIDMap_GetFormID(i);
        ReadFileIntoBuffer(apFile, &id, 4);
    }
}
}  // namespace fnv_00846e00
