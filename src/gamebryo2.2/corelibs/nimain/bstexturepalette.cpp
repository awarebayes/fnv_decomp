// gamebryo2.2/corelibs/nimain/bstexturepalette.cpp: TextureFileEntrySetter, TexMap (6 functions)

#include "bstexturepalette_decls.h"

#pragma optimize("", off)

// Builds an archive key from the texture's path and hashes, then looks up the matching BSFileEntry in the textures archive.
// @fnv 0x00c5d560
// pdb: BSTexturePalette::FindEntryForTexture | BSMain.obj | debug, object-file run extension, low
// pdb params: apTex
// pdb locals: pentry, ppathA, dirHash, pname, fileHash
BSFileEntry* __cdecl TextureFileEntrySetter::FindEntryForTexture(const NiTexture* texture)
{
    struct ArchiveKey
    {
        FileNameHash directoryHash;
        const char* texturePath;
        FileNameHash fileHash;
    } archiveKey;
    char path[260];
    {
        struct TextureEntryResolution
        {
            const NiFixedString* name;
            const char* handle;
            BSFileEntry* entry;
        } lookupResult;
        lookupResult.name = &texture->GetPath();
        lookupResult.handle = lookupResult.name->m_kHandle;
        archiveKey.texturePath = lookupResult.handle;
        CleanPath(archiveKey.texturePath, path, sizeof(path));
        FileNameHash::CreateDirectoryAndFileHashes(path, archiveKey.directoryHash, archiveKey.fileHash);
        lookupResult.entry =
            ArchiveManager::FindFileEntry(ARCHIVE_TYPE_INDEX_TEXTURES, archiveKey.directoryHash, archiveKey.fileHash, path);
        return lookupResult.entry;
    }
}

#pragma optimize("", on)

// Locks the spin lock, looks up the texture's priority bucket, stores the priority on the item and registers the file entry if the item has none.
// @fnv 0x00a61c50
// pdb: BSTexturePalette::SetTexture | BSTexturePalette.obj | retail, call graph (shared callers), high
// pdb params: apTex, apEntry
// pdb locals: pitem
void __cdecl TextureFileEntrySetter::SetTexture(NiTexture* apTex, const BSFileEntry* apEntry)
{
    if (DAT_011f4468)
    {
        g_spinLock.Lock(0);
        unsigned int priority = GetTextureSizeLevel(apTex);
        BSTexMapItem* item = BSTexturePalette_pTexMapA[priority].FindOrInsertTexture(apTex);
        item->sPriority = (unsigned short)priority;
        if (!item->pFileEntry)
        {
            if (!apEntry)
                apEntry = FindEntryForTexture(apTex);
            if (apEntry)
                DAT_011f4468->SetAt(apEntry, item);
        }
        if (--g_spinLock.m_uiRecursionCount == 0)
            g_spinLock.m_uiOwningThreadID = 0;
    }
}

struct FUN_00a61670_stub
{
    void BSEntryMap_Remove(void* apEntry);
};
struct FUN_00a61890_stub
{
    void BSTexMap_Remove(void* apValue);
};
void* __cdecl BSTexturePalette_GetItemFor(const void* apTexture);
extern int DAT_011f4484;
// @fnv 0x00a61f30
// src: COMMUNITY 0x00a61f30
// pdb: BSTexturePalette::RemoveTexture | BSTexturePalette.obj | retail, call graph, high
// pdb params: apTexture
// pdb locals: bres, pitem
bool BSTexturePalette_RemoveTexture(const void* apTexture)
{
    bool bres = false;
    if (DAT_011f4468)
    {
        g_spinLock.Lock(0);
        BSTexMapItem* pitem = (BSTexMapItem*)BSTexturePalette_GetItemFor(apTexture);
        if (pitem)
        {
            if (pitem->pFileEntry)
                ((FUN_00a61670_stub*)DAT_011f4468)->BSEntryMap_Remove(pitem->pFileEntry);
            ((FUN_00a61890_stub*)((char*)BSTexturePalette_pTexMapA + pitem->sPriority * 0xb28))->BSTexMap_Remove(pitem);
            bres = true;
        }
        if (--DAT_011f4484 == 0)
            g_spinLock.m_uiOwningThreadID = 0;
    }
    return bres;
}

extern MemoryManager g_memoryManager;

unsigned int __cdecl GetPathHash(const char*, bool);
unsigned int __cdecl BSTexturePalette_GetContainerNumber(const char*);
bool __cdecl BSTexturePalette_ArePathsEqual(const char*, const char*);

inline void* x64_orphan_BSTexMapItem_new(size_t size)
{
    return g_memoryManager.Allocate(size);
}

struct FUN_00a617c0_shell : public TextureFileEntrySetter
{
    BSTexMapItem* FindOrInsertTexture(NiTexture* apTex);
};

// Looks up a texture by its path in a hash bucket, returning the matching item or inserting a new one and incrementing the count.
// @fnv 0x00a617c0
// pdb: BSTexMap::SetAt | BSTexturePalette.obj | retail, object-file run extension, low
// pdb params: apValue
// pdb locals: ioffset, pitem, iindex, pname
BSTexMapItem* FUN_00a617c0_shell::FindOrInsertTexture(NiTexture* apTex)
{
    const char* path = apTex->GetPath().m_kHandle;
    unsigned int offset = GetPathHash(path, true);
    path += offset;
    unsigned int hash = BSTexturePalette_GetContainerNumber(path);

    for (BSTexMapItem* item = reinterpret_cast<BSTexMapItem**>(this)[hash]; item; item = item->pNext)
    {
        const char* otherPath = item->spTexture->GetPath().m_kHandle +
                                item->sNameBaseOffset;
        if (BSTexturePalette_ArePathsEqual(path, otherPath))
            return item;
    }

    BSTexMapItem* pitem = new BSTexMapItem(apTex, reinterpret_cast<BSTexMapItem**>(this)[hash]);
    pitem->sNameBaseOffset = (short)offset;
    reinterpret_cast<BSTexMapItem**>(this)[hash] = reinterpret_cast<BSTexMapItem*>(pitem);
    (*reinterpret_cast<int*>((char*)this + 0xb24))++;
    return reinterpret_cast<BSTexMapItem*>(pitem);
}

struct S_00a61b90_Map : TextureCacheMap
{
    bool BSTexMap_GetAt(unsigned int aiIndex, const char* apName, NiTexture** arValue);
};

// Locks the spin lock, hashes the path suffix, releases any existing texture in ppTex, and probes up to 7 map slots for a matching texture.
// @fnv 0x00a61b90
// pdb: BSTexturePalette::GetTexture | BSTexturePalette.obj | retail, call graph, medium
// pdb params: apEntry, arTexture
bool __cdecl FindTextureByPath(const char* path, NiTexture** ppTex)
{
    bool found = false;
    if (DAT_011f4468 != 0)
    {
        g_spinLock.Lock(0);
        unsigned int offset = GetPathHash(path, true);
        const char* name = path + offset;
        unsigned int hash = BSTexturePalette_GetContainerNumber(path + offset);
        NiTexture* pOldTexture = *ppTex;
        if (pOldTexture)
        {
            if (InterlockedDecrement((volatile long*)&pOldTexture->m_uiRefCount) == 0)
                pOldTexture->DeleteThis();
            *ppTex = 0;
        }
        int iSlot = 7;
        do
        {
            if (iSlot-- == 0)
                break;
            found = static_cast<S_00a61b90_Map*>(BSTexturePalette_pTexMapA + iSlot)->BSTexMap_GetAt(hash, name, ppTex);
        } while (!found);
        if (--g_spinLock.m_uiRecursionCount == 0)
            g_spinLock.m_uiOwningThreadID = 0;
    }
    return found;
}

struct TexMapIter
{
    BSTexMapItem* pItem;
    unsigned int uiBucket;
    unsigned int uiMap;
};
struct TexPtr
{
    NiTexture* m_pObject;
};
struct TexEntryMap
{
    void BSEntryMap_Remove(BSFileEntry* apEntry);
};
struct TexMap
{
    BSTexMapItem* pElement[713];
    unsigned int iCount;
    bool BSTexMap_GetNext(TexMapIter& arIterator, TexPtr& arValue);
    bool First(TexMapIter& it)
    {
        it.uiBucket = 0;
        it.pItem = 0;
        if (iCount)
        {
            for (unsigned int i = 0; i < 713; ++i)
            {
                if (pElement[i])
                {
                    it.uiBucket = i;
                    it.pItem = pElement[i];
                    it.uiMap = it.pItem->sPriority;
                    break;
                }
            }
        }
        return it.pItem != 0;
    }
    void BSTexMap_Remove(BSTexMapItem* apValue);
};
extern unsigned char DAT_011f4461;
extern int DAT_011a95fc;
bool __cdecl BSTexturePalette_GetFirstPos(TexMapIter& arIterator);
// src: XBOX_RETAIL_PDB 0x00a619b0
__declspec(noinline) bool TexMap::BSTexMap_GetNext(TexMapIter& arIterator, TexPtr& arValue)
{
    if (arIterator.pItem)
    {
        NiTexture* p = arIterator.pItem->spTexture;
        if (arValue.m_pObject != p)
        {
            if (arValue.m_pObject && !InterlockedDecrement((volatile long*)&arValue.m_pObject->m_uiRefCount))
                arValue.m_pObject->DeleteThis();
            arValue.m_pObject = p;
            if (arValue.m_pObject)
                InterlockedIncrement((volatile long*)&arValue.m_pObject->m_uiRefCount);
        }
        arIterator.pItem = arIterator.pItem->pNext;
        if (!arIterator.pItem)
        {
            while (++arIterator.uiBucket < 713)
            {
                if (pElement[arIterator.uiBucket])
                {
                    arIterator.pItem = pElement[arIterator.uiBucket];
                    break;
                }
            }
        }
    }
    return arIterator.pItem != 0;
}

inline bool Next(TexMapIter& it, TexPtr& tex, bool cross = true)
{
    bool more = ((TexMap*)BSTexturePalette_pTexMapA)[it.uiMap].BSTexMap_GetNext(it, tex);
    if (!more && cross)
    {
        while (it.uiMap-- != 0)
        {
            more = ((TexMap*)BSTexturePalette_pTexMapA)[it.uiMap].First(it);
            if (more)
                break;
        }
    }
    return more;
}
// src: COMMUNITY 0x00a616e0
__declspec(noinline) bool __cdecl BSTexturePalette_GetFirstPos(TexMapIter& arIterator)
{
    unsigned int idx = 7;
    bool found;
    do
    {
        --idx;
        TexMap& m = ((TexMap*)BSTexturePalette_pTexMapA)[idx];
        arIterator.uiBucket = 0;
        arIterator.pItem = 0;
        if (m.iCount)
        {
            for (unsigned int b = 0; b < 713; ++b)
            {
                if (m.pElement[b])
                {
                    arIterator.uiBucket = b;
                    arIterator.pItem = m.pElement[b];
                    arIterator.uiMap = arIterator.pItem->sPriority;
                    break;
                }
            }
        }
        found = arIterator.pItem != 0;
    } while (!found && idx);
    return found;
}

// Iterates texture cache entries, removes entries whose object ref count is 2 up to a limit, then decrements the last object's ref count.
// @fnv 0x00a61cd0
// src: XBOX_PDB 0x00a61cd0
// pdb: BSTexturePalette::PurgeUnusedTextures | BSTexturePalette.obj | retail, upstream name already matches, high
// pdb locals: icurrentCount, iterator, bnext, sptex, pcurItem, icurPri
void __cdecl BSTexturePalette_PurgeUnusedTextures()
{
    if (DAT_011f4468)
    {
        g_spinLock.Lock(0);
        int removed = 0;
        DAT_011f4461 = 0;
        TexMapIter it;
        if (BSTexturePalette_GetFirstPos(it))
        {
            TexPtr tex = {0};
            bool more;
            NiTexture* last;
            do
            {
                unsigned int oldmap = it.uiMap;
                BSTexMapItem* olditem = it.pItem;
                more = Next(it, tex);
                last = tex.m_pObject;
                if (last && last->m_uiRefCount == 2)
                {
                    if (olditem->pFileEntry)
                        ((TexEntryMap*)DAT_011f4468)->BSEntryMap_Remove(olditem->pFileEntry);
                    ((TexMap*)BSTexturePalette_pTexMapA)[oldmap].BSTexMap_Remove(olditem);
                    ++removed;
                }
            } while (more && removed < DAT_011a95fc);
            if (last && InterlockedDecrement((volatile long*)&last->m_uiRefCount) == 0)
                last->DeleteThis();
        }
        g_spinLock.Unlock();
    }
}
