// include/havok/hkpVehicleDefaultBrake.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x18.
#ifndef FNV_HAVOK_HKPVEHICLEDEFAULTBRAKE_H
#define FNV_HAVOK_HKPVEHICLEDEFAULTBRAKE_H

#include "hkBase.h"
#include "hkpVehicleBrake.h"
class hkpVehicleDefaultBrakeWheelBrakingProperties;
#pragma pack(push, 1)
// pdb: class hkpVehicleDefaultBrake, size 0x18, bases: hkpVehicleBrake, 2 members
class hkpVehicleDefaultBrake : public hkpVehicleBrake
{
  public:
    hkArray<hkpVehicleDefaultBrakeWheelBrakingProperties> m_wheelBrakingProperties;  // +0x8 // src: XBOX_RETAIL_PDB +0x8 (old: m_kWheelBrakingProperties)
    hkReal m_wheelsMinTimeToBlock;                                                         // +0x14 // src: XBOX_RETAIL_PDB +0x14 (old: m_fWheelBlockTime)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkpVehicleDefaultBrake[1];
#endif
