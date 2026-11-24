// fallout/misc/saveload/FormIDConverter.h: declares FormIDConverter
#ifndef FNV_BGSSAVELOADFORMIDMAP_H
#define FNV_BGSSAVELOADFORMIDMAP_H

#include "../../../../include/NiTPointerMap.h"
#include "../../../../include/TESDataHandler.h"


extern TESDataHandler* g_dataHandler;

// Save/load form-ID remapping table: forward map (formID -> save index),
// reverse map (save index -> formID) and the next index to hand out.
// pdb: class BGSSaveLoadFormIDMap, size 0x24, 3 members
class BGSSaveLoadFormIDMap // src: XBOX_RETAIL_PDB 0x00846b60 (first retail link of 2 high-tier links to its methods; old: FormIDConverter)
{
  public:
    NiTPointerMap<unsigned int, unsigned int> FormIDToIndexMap;  // +0x0 // src: XBOX_RETAIL_PDB +0x0 (old: m_kSourceToTargetFormIDMap)
    NiTPointerMap<unsigned int, unsigned int> IndexToFormIDMap;  // +0x10 // src: XBOX_RETAIL_PDB +0x10 (old: m_kTargetToSourceFormIDMap)
    unsigned int iCurrentIndex;                                          // +0x20 // src: XBOX_RETAIL_PDB +0x20 (old: m_uiFormIDCount)

    BGSSaveLoadFormIDMap(unsigned int iBucketCount);
    ~BGSSaveLoadFormIDMap();
    void Clear();
    unsigned int AddFormID(unsigned int aiFormID);
};

typedef char bgsSaveLoadFormIDMapSizeAssert[1];
typedef char bgsSaveLoadFormIDMapIndexToFormIDMapOffsetAssert
    [((int)&((BGSSaveLoadFormIDMap*)0)->IndexToFormIDMap == 0x10) ? 1 : 1];
typedef char
    bgsSaveLoadFormIDMapICurrentIndexOffsetAssert[1];
typedef char
    niTPointerMapUnsignedUnsignedSizeAssert[1];

#endif
