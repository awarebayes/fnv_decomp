
// src/havok/physics/collide/shape/deprecated/fastmesh/hkpfastmeshshape.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/collide/shape/deprecated/fastmesh/hkpfastmeshshape.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

// Exe 0x00e48cb0 GetShapeForKey: retain this LocalM128 view; the CRT type does not preserve this unit.
// VC9 requires the __m128 compatibility typedef for its native SIMD ABI (renamed tags fail C2719).
typedef union __declspec(intrin_type) __declspec(align(16)) {
    float m128_f32[4];
    unsigned __int64 m128_u64[2];
} __m128, LocalM128;

#include "../../../../../../../include/hkArray.h"
#include "../../../../../../../include/hkBaseObject.h"
#include "../../../../../../../include/hkReferencedObject.h"
#include "../../../../../../../include/hkVector4.h"
#include "../../../../../../../include/hkpShape.h"
#include "../../../../../../../include/hkpSphereRepShape.h"
#include "../../../../../../../include/hkpConvexShape.h"
#include "../../../../../../../include/hkpShapeContainer.h"
#include "../../../../../../../include/hkpShapeCollection.h"
#include "../../../../../../../include/hkpMeshShape.h"
#include "../../../../../../../include/hkpTriangleShape.h"

extern "C" LocalM128 _mm_setzero_ps();
extern "C" LocalM128 _mm_load_ps(const float*);

inline void* operator new(size_t, void* p)
{
    return p;
}



template <class ENUM, class STORAGE> inline hkEnum<ENUM, STORAGE>::hkEnum()
{}

inline hkpShape::hkpShape(hkpShapeType type) : m_userData(0)
{
    *reinterpret_cast<hkUint32*>(&m_type) = type;
}

inline hkpConvexShape::hkpConvexShape(int type, float radius)
    : hkpShape(static_cast<hkpShapeType>(type)), m_radius(radius)
{}

inline void hkVector4::setZero4()
{
    m_vComponents = _mm_setzero_ps();
}

// The triangle vtable at 0x010c749c is also bound by the simple-mesh unit.
inline hkpTriangleShape::hkpTriangleShape(float fRadius, unsigned short usWeldingInfo, int eWeldingType) throw()
    : hkpConvexShape(3, fRadius)
{
    m_weldingInfo = usWeldingInfo;
    *reinterpret_cast<hkUint8*>(&m_weldingType) = static_cast<hkUint8>(eWeldingType);
    m_extrusion.setZero4();
    m_isExtruded = 0;
}

// include/ has no subpart header; these fields match the reflected hkpMeshShapeSubpart (size 0x38).
class hkpMeshShapeSubpart
{
  public:
    const void* m_pMaterialIndexBase;
    int m_iTriangleIndexStride;
    int m_iStridedVertexCount;
    const void* m_pMaterialDataBase;
    signed char m_ucIndexStridingType;
    signed char m_iMaterialIndexType;
    char pad12[2];
    int m_iMaterialStriding;
    int m_bFlipAlternateTriangles;
    int m_iTriangleCount;
    const void* m_pVertexBase;
    int m_iVertexStriding;
    const void* m_pIndexBase;
    int m_iIndexStriding;
    int m_iNumTriangles;
    int m_iTriangleOffset;
};

// This unit's 0x00e48cb0 override uses subpart zero: hkpFastMeshShape, with the shared mesh layout.
// include/ has no hkpFastMeshShape header yet.
// pdb: class hkpFastMeshShape, size 0x60, bases: hkpMeshShape, 0 members
class hkpFastMeshShape : public hkpMeshShape
{
  public:
    virtual const hkpShape* GetShapeForKey(hkpShapeKey key, hkpShapeBuffer& buffer) const;
};

// Builds a hkpTriangleShape triangle in buffer from subpart 0 vertex data, scaling, and welding info for the given uiKey.
// @fnv 0x00e48cb0
// pdb: hkpFastMeshShape::getChildShape | hkpFastMeshShape.obj | retail, vtable slot, high
// pdb params: key, buffer
// pdb locals: assertBuf, ostr
const hkpShape* hkpFastMeshShape::GetShapeForKey(hkpShapeKey key, hkpShapeBuffer& buffer) const
{
    const hkpMeshShapeSubpart& kPart = m_subparts[0];
    const unsigned short* pusTriangle = reinterpret_cast<const unsigned short*>(
        static_cast<const char*>(kPart.m_pMaterialDataBase) + kPart.m_iMaterialStriding * key);
    int iFlip = kPart.m_bFlipAlternateTriangles & key;
    const hkVector4* pkA = reinterpret_cast<const hkVector4*>(
        static_cast<const char*>(kPart.m_pMaterialIndexBase) + pusTriangle[0] * kPart.m_iTriangleIndexStride);
    const hkVector4* pkB = reinterpret_cast<const hkVector4*>(
        static_cast<const char*>(kPart.m_pMaterialIndexBase) + pusTriangle[1 + iFlip] * kPart.m_iTriangleIndexStride);
    const hkVector4* pkC =
        reinterpret_cast<const hkVector4*>(static_cast<const char*>(kPart.m_pMaterialIndexBase) +
                                               pusTriangle[1 + (iFlip ^ 1)] * kPart.m_iTriangleIndexStride);
    unsigned short usWeldingInfo = (m_weldingInfo.m_iElementCount == 0) ? 0 : m_weldingInfo[kPart.m_iTriangleOffset + key];
    hkpTriangleShape* pkTriangle = ::new (&buffer) hkpTriangleShape(m_radius, usWeldingInfo, m_weldingType);
    pkTriangle->m_kVertexA.setMul4(*pkA, m_scaling);
    pkTriangle->m_kVertexB.m_vComponents =
        _mm_mul_ps(_mm_load_ps(pkB->m_vComponents.m128_f32), m_scaling.m_vComponents);
    pkTriangle->m_kVertexC.m_vComponents =
        _mm_mul_ps(_mm_load_ps(pkC->m_vComponents.m128_f32), m_scaling.m_vComponents);
    return pkTriangle;
}
