// include/havok/hkpSetLocalTranslationsConstraintAtom.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x30.
#ifndef FNV_HAVOK_HKPSETLOCALTRANSLATIONSCONSTRAINTATOM_H
#define FNV_HAVOK_HKPSETLOCALTRANSLATIONSCONSTRAINTATOM_H

#include "hkBase.h"
#include "hkpConstraintAtom.h"
#pragma pack(push, 1)
// pdb: structure hkpSetLocalTranslationsConstraintAtom, size 0x30, bases: hkpConstraintAtom, 2 members
class hkpSetLocalTranslationsConstraintAtom : public hkpConstraintAtom
{
  public:
    char pad2[0xE];
    hkVector4 m_translationA;  // +0x10 // src: XBOX_RETAIL_PDB +0x10 (old: m_kTranslationA)
    hkVector4 m_translationB;  // +0x20 // src: XBOX_RETAIL_PDB +0x20 (old: m_kTranslationB)
};
#pragma pack(pop)
typedef char
    fnv_hk_size_hkpSetLocalTranslationsConstraintAtom[1];
#endif
