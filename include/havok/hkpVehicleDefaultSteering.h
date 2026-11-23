// include/havok/hkpVehicleDefaultSteering.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x1C.
#ifndef FNV_HAVOK_HKPVEHICLEDEFAULTSTEERING_H
#define FNV_HAVOK_HKPVEHICLEDEFAULTSTEERING_H

#include "hkBase.h"
#include "hkpVehicleSteering.h"
#pragma pack(push, 1)
// pdb: class hkpVehicleDefaultSteering, size 0x1c, bases: hkpVehicleSteering, 3 members
class hkpVehicleDefaultSteering : public hkpVehicleSteering
{
  public:
    hkReal m_maxSteeringAngle;           // +0x8 // src: XBOX_RETAIL_PDB +0x8 (old: m_fMaxSteeringAngle)
    hkReal m_maxSpeedFullSteeringAngle;  // +0xC // src: XBOX_RETAIL_PDB +0xc (old: m_fMaxSpeedFullSteeringAngle)
    hkArray<hkBool> m_doesWheelSteer;    // +0x10 // src: XBOX_RETAIL_PDB +0x10 (old: m_kDoesWheelSteer)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkpVehicleDefaultSteering[1];
#endif
