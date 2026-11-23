// include/havok/hkpVehicleDefaultVelocityDamper.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x14.
#ifndef FNV_HAVOK_HKPVEHICLEDEFAULTVELOCITYDAMPER_H
#define FNV_HAVOK_HKPVEHICLEDEFAULTVELOCITYDAMPER_H

#include "hkBase.h"
#include "hkpVehicleVelocityDamper.h"
#pragma pack(push, 1)
// pdb: class hkpVehicleDefaultVelocityDamper, size 0x14, bases: hkpVehicleVelocityDamper, 3 members
class hkpVehicleDefaultVelocityDamper : public hkpVehicleVelocityDamper
{
  public:
    hkReal m_normalSpinDamping;           // +0x8 // src: XBOX_RETAIL_PDB +0x8 (old: m_fSpinDamping)
    hkReal m_collisionSpinDamping;  // +0xC // src: XBOX_RETAIL_PDB +0xc (old: m_fCollisionSpinDamping)
    hkReal m_collisionThreshold;    // +0x10 // src: XBOX_RETAIL_PDB +0x10 (old: m_fCollisionThreshold)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkpVehicleDefaultVelocityDamper[1];
#endif
