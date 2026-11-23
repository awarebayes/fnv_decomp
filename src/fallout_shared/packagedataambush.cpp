
// src/fallout_shared/packagedataambush.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout shared/packagedataambush.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../include/TESAmbushPackageData.h"

void __cdecl _Deallocate(void*);
// Destroys the package data, then frees this with _Deallocate when flags has bit 1 set, and returns this.
// @fnv 0x0067ad90
// pdb: TESAmbushPackageData::`scalar deleting destructor' | PackageDataAmbush.obj | retail, vtable slot, high
void* __thiscall TESAmbushPackageData::Destroy(unsigned int flags)
{
    this->TESAmbushPackageData::~TESAmbushPackageData();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

void __cdecl TESForm_AddChunk(int);
struct LocationDescriptor
{
    void __thiscall Save(int);
};
// @fnv 0x0067afc0
// pdb: TESAmbushPackageData::Save | PackageDataAmbush.obj | retail, vtable slot, high
void __thiscall TESAmbushPackageData::SaveWithTag()
{
    TESForm_AddChunk(0x4d414b50);
    if (pAmbushLocation)
        reinterpret_cast<LocationDescriptor*>(pAmbushLocation)->Save(0x32444c50);
}

class Sub_0067adc0
{
  public:
    unsigned int m_00;
    int Release(unsigned int flags);
};

// Destructor releases the location by calling Release(1) if it is non-null.
// @fnv 0x0067adc0
// pdb: TESAmbushPackageData::~TESAmbushPackageData | PackageDataAmbush.obj | debug, upstream name already matches, high
TESAmbushPackageData::~TESAmbushPackageData()
{
    Sub_0067adc0* copy;
    Sub_0067adc0* tmp;
    copy = reinterpret_cast<Sub_0067adc0*>(pAmbushLocation);
    tmp = copy;
    tmp != 0 ? tmp->Release(1) : 0;
}

extern int DAT_0119bdec;
extern int DAT_0119bdc8;
struct PackageRecord
{
    int unk00;
    void* location04;
};
PackageRecord* __cdecl FUN_00ec43fb(int id, int flags, const int* name, const int* type, int option);
struct AmbushLocation
{
    int unk00, unk04, unk08;
    AmbushLocation();
    void* FUN_00670b30(int release);
    void FUN_0067f4d0(void* source, int flags);
};
// Looks up a package record by id; if found, releases the current location and creates a new AmbushLocation from the record's location.
// @fnv 0x0067ae50
// pdb: TESAmbushPackageData::Copy | PackageDataAmbush.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pcopy
void TESAmbushPackageData::SetLocationFromPackage(int apCopy)
{
    PackageRecord* record = FUN_00ec43fb(apCopy, 0, &DAT_0119bdec, &DAT_0119bdc8, 0);
    if (record)
    {
        if (pAmbushLocation)
        {
            AmbushLocation* pAllocatedPath = reinterpret_cast<AmbushLocation*>(pAmbushLocation);
            AmbushLocation* pOldLocation = pAllocatedPath;
            pOldLocation ? pOldLocation->FUN_00670b30(1) : 0;
            pAmbushLocation = 0;
        }
        if (record->location04)
        {
            pAmbushLocation = reinterpret_cast<AF50Location*>(new AmbushLocation());
            reinterpret_cast<AmbushLocation*>(pAmbushLocation)->FUN_0067f4d0(record->location04, 0);
        }
    }
}

// TESAmbushPackageData: report whether another package's data differs from ours (location only).

class AF50Location
{
  public:
    bool PackageLocation_Compare(AF50Location* apCompare);
};

// Casts pkOtherData to TESAmbushPackageData and returns true if the cast fails or the two m_pkLocation values differ, otherwise false.
// @fnv 0x0067af50
// src: XBOX_PDB 0x0067af50
// pdb: TESAmbushPackageData::Compare | PackageDataAmbush.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp
bool TESAmbushPackageData::Compare_0067af50(TESPackageData* apCompare)
{
    TESAmbushPackageData* pkOther = dynamic_cast<TESAmbushPackageData*>(apCompare);
    if (!pkOther)
        return true;

    if (pAmbushLocation)
    {
        if (pAmbushLocation->PackageLocation_Compare(pkOther->pAmbushLocation))
            return true;
    }
    else if (pkOther->pAmbushLocation)
    {
        return true;
    }
    return false;
}
