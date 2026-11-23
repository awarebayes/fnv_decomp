#pragma once

#include "hkReferencedObject.h"
#include "hkStringPtr.h"
#include "hkaAnimation.h"
#include "hkaBone.h"

class hkaBone;
class BoneName;
struct hkQsTransform;
class hkLocalFrame;

template <class T>
struct HavokArray;

// Havok 7.1 hkaSkeleton: RTTI hkaSkeleton : hkReferencedObject, vftable 0x010c4dec (dtor 0x00c7e970);
// 0x48 bytes (BuildRigFromNodes 0x00ca21c0 allocates 0x48).
// pdb: class hkaSkeleton, size 0x48, bases: hkReferencedObject, 6 members
class hkaSkeleton : public hkReferencedObject
{
public:
    struct LocalFrameOnBone
    {
        hkLocalFrame* m_localFrame;  // hkRefPtr<hkLocalFrame> // src: XBOX_RETAIL_PDB +0x0 (old: m_pLocalFrame)
        int m_boneIndex; // src: XBOX_RETAIL_PDB +0x4 (old: m_iBoneIndex)
    };

    // Havok's hkArray() is m_data(HK_NULL), m_size(0), m_capacityAndFlags(DONT_DEALLOCATE_FLAG) for each array.
    // The shared hkArray has no constructor, so units that construct a skeleton define this (__forceinline)
    // with their own array type.
    hkaSkeleton();
    virtual ~hkaSkeleton() throw();

    static void* __cdecl operator new(size_t n);
    static void __cdecl operator delete(void* p);

    HavokArray<short>& getParents();
    hkArray<hkQsTransform>& getPose();
    HavokArray<BoneName>& getBones();

    hkStringPtr m_name;                          // +0x08
    hkArray<short> m_parentIndices;             // +0x0c // src: XBOX_RETAIL_PDB +0xc (old: m_kParentIndices)
    hkArray<hkaBone> m_bones;                   // +0x18 // src: XBOX_RETAIL_PDB +0x18 (old: m_kBones)
    hkArray<hkQsTransform> m_referencePose;     // +0x24 // src: XBOX_RETAIL_PDB +0x24 (old: m_kReferencePose)
    hkArray<hkStringPtr> m_floatSlots;          // +0x30 // src: XBOX_RETAIL_PDB +0x30 (old: m_kFloatSlots)
    hkArray<LocalFrameOnBone> m_localFrames;    // +0x3c // src: XBOX_RETAIL_PDB +0x3c (old: m_kLocalFrames)
};

typedef char hkaSkeleton_size_check[1];
