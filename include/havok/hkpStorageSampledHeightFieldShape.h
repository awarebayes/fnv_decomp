// include/havok/hkpStorageSampledHeightFieldShape.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x80.
#ifndef FNV_HAVOK_HKPSTORAGESAMPLEDHEIGHTFIELDSHAPE_H
#define FNV_HAVOK_HKPSTORAGESAMPLEDHEIGHTFIELDSHAPE_H

#include "hkBase.h"
#include "hkpSampledHeightFieldShape.h"
#pragma pack(push, 1)
// pdb: class hkpStorageSampledHeightFieldShape, size 0x80, bases: hkpSampledHeightFieldShape, 5 members
class hkpStorageSampledHeightFieldShape : public hkpSampledHeightFieldShape
{
  public:
    hkArray<hkReal> m_storage;  // +0x60 // src: XBOX_RETAIL_PDB +0x60 (old: m_kStorage)
    char fOffset[0x14]; // src: XBOX_RETAIL_PDB +0x6c (old: pad6C)
};
#pragma pack(pop)
typedef char
    fnv_hk_size_hkpStorageSampledHeightFieldShape[1];
#endif
