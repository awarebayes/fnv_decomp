// include/havok/hkpVehicleDefaultEngine.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x30.
#ifndef FNV_HAVOK_HKPVEHICLEDEFAULTENGINE_H
#define FNV_HAVOK_HKPVEHICLEDEFAULTENGINE_H

#include "hkBase.h"
#include "hkpVehicleEngine.h"
#pragma pack(push, 1)
// pdb: class hkpVehicleDefaultEngine, size 0x30, bases: hkpVehicleEngine, 10 members
class hkpVehicleDefaultEngine : public hkpVehicleEngine
{
  public:
    hkReal m_minRPM;                    // +0x8 // src: XBOX_RETAIL_PDB +0x8 (old: m_fMinRPM)
    hkReal m_optRPM;                    // +0xC // src: XBOX_RETAIL_PDB +0xc (old: m_fOptRPM)
    hkReal m_maxRPM;                    // +0x10 // src: XBOX_RETAIL_PDB +0x10 (old: m_fMaxRPM)
    hkReal m_maxTorque;                 // +0x14 // src: XBOX_RETAIL_PDB +0x14 (old: m_fMaxTorque)
    hkReal m_torqueFactorAtMinRPM;      // +0x18 // src: XBOX_RETAIL_PDB +0x18 (old: m_fTorqueFactorAtMinRPM)
    hkReal m_torqueFactorAtMaxRPM;      // +0x1C // src: XBOX_RETAIL_PDB +0x1c (old: m_fTorqueFactorAtMaxRPM)
    hkReal m_resistanceFactorAtMinRPM;  // +0x20 // src: XBOX_RETAIL_PDB +0x20 (old: m_fResistanceFactorAtMinRPM)
    hkReal m_resistanceFactorAtOptRPM;  // +0x24 // src: XBOX_RETAIL_PDB +0x24 (old: m_fResistanceFactorAtOptRPM)
    hkReal m_resistanceFactorAtMaxRPM;  // +0x28 // src: XBOX_RETAIL_PDB +0x28 (old: m_fResistanceFactorAtMaxRPM)
    hkReal m_clutchSlipRPM;             // +0x2C // src: XBOX_RETAIL_PDB +0x2c (old: m_fClutchSlipRPM)
};
#pragma pack(pop)
typedef char fnv_hk_size_hkpVehicleDefaultEngine[1];
#endif
