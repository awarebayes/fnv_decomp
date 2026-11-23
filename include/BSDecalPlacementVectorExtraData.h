#pragma once

#include "NiIntegerExtraData.h"

// pdb: class BSDecalPlacementVectorExtraData, size 0x40, bases: NiIntegerExtraData, 3 members
class BSDecalPlacementVectorExtraData : public NiIntegerExtraData
{
  public:
    struct ExtraList
    {
        void* head;
        void* tail;
        unsigned int size;

        ExtraList();
        ~ExtraList();
        void ReleaseStorage_00c64710();
    };

    BSDecalPlacementVectorExtraData(int iValue);
    virtual ~BSDecalPlacementVectorExtraData();
    void LinkToObject(void* apObject);
    // NiMemObject's operator new (0x00aa13e0), redeclared: blk_00c60000_od_ob1_variant's 0x00c63c30 inlines its body
    // for this class only, so that unit defines it inline here.
    static void* operator new(size_t size);

    unsigned short usGroupCount; // src: XBOX_RETAIL_PDB +0x10 (old: count)
    unsigned short unk12;
    void* pDecalPlacementGroups[8]; // src: XBOX_RETAIL_PDB +0x14 (old: groups)
    ExtraList skinnedObjects; // src: XBOX_RETAIL_PDB +0x34 (old: list)
};
