// include/havok/hkxMesh.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x20.
#ifndef FNV_HAVOK_HKXMESH_H
#define FNV_HAVOK_HKXMESH_H

#include "hkBase.h"
#include "hkReferencedObject.h"
class hkxMeshSection;
class hkxMeshUserChannelInfo;
#pragma pack(push, 1)
// pdb: class hkxMesh, size 0x20, bases: hkReferencedObject, 2 members
class hkxMesh : public hkReferencedObject
{
  public:
    hkArray<hkxMeshSection*> m_sections;                   // +0x8
    hkArray<hkxMeshUserChannelInfo*> m_userChannelInfos;  // +0x14 // src: XBOX_RETAIL_PDB +0x14 (old: m_kUserChannelInfos)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkxMesh[1];
#endif
