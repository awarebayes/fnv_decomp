// include/havok/hkxMaterialShaderSet.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x14.
#ifndef FNV_HAVOK_HKXMATERIALSHADERSET_H
#define FNV_HAVOK_HKXMATERIALSHADERSET_H

#include "hkBase.h"
#include "hkReferencedObject.h"
class hkxMaterialShader;
#pragma pack(push, 1)
// pdb: class hkxMaterialShaderSet, size 0x14, bases: hkReferencedObject, 1 members
class hkxMaterialShaderSet : public hkReferencedObject
{
  public:
    hkArray<hkxMaterialShader*> m_shaders;  // +0x8 // src: XBOX_RETAIL_PDB +0x8 (old: m_spPixelShaders)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkxMaterialShaderSet[1];
#endif
