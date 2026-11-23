// include/havok/hkpSerializedTrack1nInfo.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x18.
#ifndef FNV_HAVOK_HKPSERIALIZEDTRACK1NINFO_H
#define FNV_HAVOK_HKPSERIALIZEDTRACK1NINFO_H

#include "hkBase.h"
class hkpAgent1nSector;
class hkpSerializedSubTrack1nInfo;
#pragma pack(push, 1)
// pdb: structure hkpSerializedTrack1nInfo, size 0x18, 2 members
class hkpSerializedTrack1nInfo
{
  public:
    hkArray<hkpAgent1nSector*> m_sectors;               // +0x0 // src: XBOX_RETAIL_PDB +0x0 (old: m_kSectors)
    hkArray<hkpSerializedSubTrack1nInfo*> m_subTracks;  // +0xC // src: XBOX_RETAIL_PDB +0xc (old: m_kSubTracks)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkpSerializedTrack1nInfo[1];
#endif
