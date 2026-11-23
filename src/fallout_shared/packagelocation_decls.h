// fallout_shared/packagelocation_decls.h: declares LocationDescriptor
#ifndef FNV_PACKAGELOCATION_DECLS_H
#define FNV_PACKAGELOCATION_DECLS_H


class TESForm;

#include "../../include/PACK_LOC_TYPE.h"

// LocationDescriptor's nonvirtual, 12-byte layout is shared by the PC accessors
class LocationDescriptor
{
  public:
    char eLocType; // src: XBOX_RETAIL_PDB +0x0 (old: m_ucLocationType)
    unsigned int iRad; // src: XBOX_RETAIL_PDB +0x4 (old: m_pLocationData)
    union
    {
        TESForm* pObject;
        TESObjectREFR* pRef;
        TESObjectCELL* pCell;
        TESForm* m_pLocationObject;
    };

    TESObjectREFR* GetLocationReference();
    TESObjectCELL* GetLocationCellReference();
    PACK_LOC_TYPE GetLocType() const;
    void PackageLocation_SetLocCell(TESObjectCELL* cell);
    void SetValue(TESForm* object);
    // Keep this distinct, community-confirmed callee address explicit.
    void PackageLocation_SetLocReference(TESObjectREFR* reference);
    void InitItem(TESForm* apOwner);
    TESObjectREFR* ResolveLocationReference(MobileObject* apActor);
};

typedef char PackageLocation_size_check[1];
#endif
