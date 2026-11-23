// gamebryo2.2/corelibs/nimain/bstexturepalette_decls.h: declares FileNameHash, ArchiveManager, TextureFileEntrySetter and more
#ifndef FNV_BSTEXTUREPALETTE_DECLS_H
#define FNV_BSTEXTUREPALETTE_DECLS_H

#include "layout_support.h"
#include "../../../../include/ARCHIVE_TYPE_INDEX.h"

#include "../../../../include/NiTexture.h"
#include "../../../../include/MemoryManager.h"
#include "../../../../include/RecursiveSpinLock.h"

class BSFileEntry;

class FileNameHash
{
  public:
    unsigned char cLast, cSecondToLast, cLength, cFirst;
    unsigned int uiCRC;
    static void __cdecl CreateDirectoryAndFileHashes(const char*, FileNameHash&, FileNameHash&);
};
FNV_STATIC_ASSERT(sizeof(FileNameHash) == 8);

// Archive type one is the texture archive group.
// pdb: class ArchiveManager, size 0x1, 0 members
class ArchiveManager
{
  public:
    static BSFileEntry* __cdecl FindFileEntry(ARCHIVE_TYPE_INDEX, FileNameHash&, FileNameHash&, const char*);
};

class TextureFileEntrySetter
{
  public:
    static BSFileEntry* __cdecl FindEntryForTexture(const NiTexture*);
    static void __cdecl SetTexture(NiTexture*, const BSFileEntry*);
};
bool __cdecl CleanPath(const char*, char*, unsigned int);

// Shared BSTexMapItem.h is not available yet; retain one definition for the 0x10-byte cache node.
// pdb: structure BSTexMapItem, size 0x10, 5 members
class BSTexMapItem
{
  public:
    NiTexture* spTexture;
    BSFileEntry* pFileEntry; // src: XBOX_RETAIL_PDB +0x4 (old: m_pFileEntry)
    BSTexMapItem* pNext;
    unsigned short sPriority; // src: XBOX_RETAIL_PDB +0xc (old: m_usTextureSizeLevel)
    short sNameBaseOffset; // src: XBOX_RETAIL_PDB +0xe (old: m_sTextureCacheValue)


    BSTexMapItem(NiTexture* tex, BSTexMapItem* next)
    {
        spTexture = tex;
        InterlockedIncrement((volatile long*)&tex->m_uiRefCount);
        pFileEntry = 0;
        pNext = next;
        sPriority = 0;
        sNameBaseOffset = 0;
    }
};

FNV_STATIC_ASSERT(sizeof(BSTexMapItem) == 16);
class TextureCacheMap
{
  public:
    BSTexMapItem* pElement[713];
    unsigned int iCount;
    BSTexMapItem* FindOrInsertTexture(NiTexture*);
};
FNV_STATIC_ASSERT(sizeof(TextureCacheMap) == 0xb28);
class BSFileEntryMap
{
  public:
    void SetAt(const BSFileEntry*, BSTexMapItem*);
};
extern RecursiveSpinLock g_spinLock;
extern TextureCacheMap* BSTexturePalette_pTexMapA; // src: XBOX_RETAIL_PDB 0x011f4464 (old: DAT_011f4464)
extern BSFileEntryMap* DAT_011f4468;
unsigned int __cdecl GetTextureSizeLevel(NiTexture*);
#endif
