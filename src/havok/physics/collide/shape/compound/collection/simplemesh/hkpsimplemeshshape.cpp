#include <xmmintrin.h>
typedef ::__m128 LocalM128;

// src/havok/physics/collide/shape/compound/collection/simplemesh/hkpsimplemeshshape.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/collide/shape/compound/collection/simplemesh/hkpsimplemeshshape.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

typedef ::__m128 LocalM128;

#include "../../../../../../../../include/hkVector4.h"
#include "../../../../../../../../include/hkBool.h"
#include "../../../../../../../../include/hkArray.h"
#include "../../../../../../../../include/hkBaseObject.h"
#include "../../../../../../../../include/hkReferencedObject.h"
#include "../../../../../../../../include/hkpShape.h"
#include "../../../../../../../../include/hkpShapeContainer.h"
#include "../../../../../../../../include/hkMemoryAllocator.h"
#include "../../../../../../../../include/hkpSphereRepShape.h"
#include "../../../../../../../../include/hkpConvexShape.h"
#include "../../../../../../../../include/hkpTriangleShape.h"
#include "../../../../../../../../include/hkpShapeCollection.h"
#include "../../../../../../../../include/hkStatisticsCollector.h"





inline hkpShapeContainer::~hkpShapeContainer()
{}

// include/ has no hkMemoryRouter header yet; the heap pointer is at offset 0x10.
#include "../../../../../../../../include/hkMemoryRouter.h"

extern "C" hkMemoryRouter& __cdecl hkMemoryRouter_getInstance() throw();

inline void hkReferencedObject::operator delete(void* p)
{
    hkReferencedObject* object = static_cast<hkReferencedObject*>(p);
    hkMemoryRouter_getInstance().heap().blockFree(object, object->m_memSizeAndFlags);
}

inline hkMemoryAllocator& hkContainerHeapAllocator::get(const void*)
{
    return hkMemoryRouter_getInstance().heap();
}

template <class T, class Allocator> inline hkArray<T, Allocator>::~hkArray() throw()
{
    hkMemoryAllocator& kAlloc = hkContainerHeapAllocator().get(this);
    m_iElementCount = 0;
    if ((m_iCapacityState & 0x80000000) == 0)
        kAlloc.bufFree(m_data, (m_iCapacityState & 0x3fffffff) * sizeof(T));
    m_data = 0;
    m_iCapacityState = 0x80000000;
}

using ::_mm_setzero_ps;

inline void* operator new(size_t, void* p)
{
    return p;
}

inline void hkVector4::setZero4()
{
    m_vComponents = _mm_setzero_ps();
}

template <class ENUM, class STORAGE> inline hkEnum<ENUM, STORAGE>::hkEnum()
{}

inline hkpShape::hkpShape(hkpShapeType type) throw() : m_userData(0)
{
    // hkEnum stores this shape tag as hkUint32; initialize it without an extra converting constructor.
    *reinterpret_cast<hkUint32*>(&m_type) = type;
}

inline hkpConvexShape::hkpConvexShape(int type, float radius) throw()
    : hkpShape(static_cast<hkpShapeType>(type)), m_radius(radius)
{}

inline hkpTriangleShape::hkpTriangleShape(float fRadius, unsigned short usWeldingInfo, int eWeldingType) throw()
    : hkpConvexShape(3, fRadius)
{
    m_weldingInfo = usWeldingInfo;
    *reinterpret_cast<hkUint8*>(&m_weldingType) = static_cast<hkUint8>(eWeldingType);
    m_extrusion.setZero4();
    m_isExtruded = 0;
}

// include/ has no hkpSimpleMeshShape header yet; keep its definition here.
// pdb: class hkpSimpleMeshShape, size 0x44, bases: hkpShapeCollection, 5 members
class hkpSimpleMeshShape : public hkpShapeCollection
{
public:
    struct Triangle
    {
        int m_a, m_b, m_c; // src: XBOX_RETAIL_PDB m_c +0x8 (old: m_iThirdValue); m_a +0x0 (old: m_kPointA); m_b +0x4 (old: m_kPointB)
        unsigned short m_weldingInfo; // src: XBOX_RETAIL_PDB +0xc (old: m_kWeldingInfo)
    };

    hkpSimpleMeshShape();
    virtual hkpShapeKey GetFirstKey() const;
    virtual hkpShapeKey GetNextShapeKey(hkpShapeKey oldKey) const;
    virtual const hkpShape* GetShapeForKey(hkpShapeKey key, hkpShapeBuffer& buffer) const;
    virtual void calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;
    // hkpShapeCollection has no declaration for its statistics implementation at 0x00d213e0.
    void CollectShapeStatistics(hkStatisticsCollector* collector, const hkClass* cls) const;
    void setTriangleWeldingInfo(int index, unsigned short value);
    void setWeldingType(unsigned char type);

    hkArray<hkVector4> m_vertices; // src: XBOX_RETAIL_PDB +0x18 (old: m_kVertices)
    hkArray<Triangle> m_triangles; // src: XBOX_RETAIL_PDB +0x24 (old: m_uiTriangleCount)
    hkArray<unsigned char> m_materialIndices; // src: XBOX_RETAIL_PDB +0x30 (old: m_kMaterialIndices8)
    float m_radius; // src: XBOX_RETAIL_PDB +0x3c (old: m_fSphereRadius)
    unsigned char m_weldingType; // src: XBOX_RETAIL_PDB +0x40 (old: m_eWeldingType)
};

// Its implicit destructor and deleting-destructor thunk cover 0x00dfcfd0 and 0x00dfcfc0/0x00dfd080.
hkpSimpleMeshShape::hkpSimpleMeshShape()
{}

// @fnv 0x00e3c240
// pdb: hkpSimpleMeshShape::setWeldingInfo | hkpSimpleMeshShape.obj | debug, vtable slot, medium
// pdb params: key, weldingInfo
// pdb locals: assertBuf, ostr
void hkpSimpleMeshShape::setTriangleWeldingInfo(int index, unsigned short value)
{
    m_triangles[index].m_weldingInfo = value;
}

// @fnv 0x00e3c260
void hkpSimpleMeshShape::setWeldingType(unsigned char type)
{
    m_weldingType = type;
}

// Placement-constructs a triangle shape from the vertex indices in m_uiTriangleCount[uiKey] and returns it as a hkpShape pointer.
// @fnv 0x00e3c430
// pdb: hkpSimpleMeshShape::getChildShape | hkpSimpleMeshShape.obj | retail, vtable slot, high
// pdb params: key, buffer
// pdb locals: assertBuf, ostr
const hkpShape* hkpSimpleMeshShape::GetShapeForKey(hkpShapeKey key, hkpShapeBuffer& buffer) const
{
    hkpTriangleShape* pkTriangle =
        ::new (&buffer) hkpTriangleShape(m_radius, m_triangles[key].m_weldingInfo, m_weldingType);
    pkTriangle->setVertex(0, m_vertices[m_triangles[key].m_a]);
    pkTriangle->setVertex(1, m_vertices[m_triangles[key].m_b]);
    pkTriangle->setVertex(2, m_vertices[m_triangles[key].m_c]);
    return pkTriangle;
}

extern float DAT_011b2e90;
hkBool __cdecl IsDegenerateTriangle(const hkVector4&, const hkVector4&, const hkVector4&, float);
// These arrays are at +8/+20 from the container this pointer, unlike hkGeometry at +0/+12.
// Returns the index of the first triangle in m_uiTriangleCount that is not degenerate, or -1 if all are degenerate.
// @fnv 0x00e3c340
// pdb: hkpSimpleMeshShape::getFirstKey | hkpSimpleMeshShape.obj | retail, vtable slot, high
hkpShapeKey hkpSimpleMeshShape::GetFirstKey() const
{
    for (int i = 0; i < m_triangles.GetLogicalSize(); i++)
    {
        if (!IsDegenerateTriangle(m_vertices[m_triangles[i].m_a], m_vertices[m_triangles[i].m_b],
                                  m_vertices[m_triangles[i].m_c], DAT_011b2e90))
            return i;
    }
    return -1;
}

int __cdecl hkStatisticsCollector_getAllocatedSize(const void* p, int size);
extern char DAT_01269198;

template <class T> inline void CollectArray(hkStatisticsCollector* collector, const char* name, int type, const hkArray<T>& a)
{
    if ((a.m_iCapacityState & 0x80000000) == 0 || collector->m_includeUnmanagedArrayMemory)
    {
        const T* data = a.m_data;
        int allocated = hkStatisticsCollector_getAllocatedSize(data, a.getCapacity() * (int)sizeof(T));
        collector->AddMemoryChunk(type, name, a.m_data, a.m_iElementCount * (int)sizeof(T), allocated);
    }
}

// Reports the mesh arrays, then delegates to the collection statistics implementation.
// @fnv 0x00e3c4e0
// pdb: hkpSimpleMeshShape::calcContentStatistics | hkpSimpleMeshShape.obj | retail, string-set bridge, high
// pdb params: collector, cls
void hkpSimpleMeshShape::calcContentStatistics(hkStatisticsCollector* collector, const hkClass* cls) const
{
    CollectArray(collector, "Vertices", 3, m_vertices);
    CollectArray(collector, "Triangles", 3, m_triangles);
    CollectArray(collector, "MaterialIds", 3, m_materialIndices);
    CollectShapeStatistics(collector, reinterpret_cast<const hkClass*>(&DAT_01269198));
}

// @fnv 0x00e3c3b0
// pdb: hkpSimpleMeshShape::getNextKey | hkpSimpleMeshShape.obj | retail, vtable slot, high
// pdb params: oldKey
hkpShapeKey hkpSimpleMeshShape::GetNextShapeKey(hkpShapeKey oldKey) const
{
    for (int i = static_cast<int>(oldKey) + 1; i < m_triangles.GetLogicalSize(); ++i)
    {
        if (!IsDegenerateTriangle(m_vertices[m_triangles[i].m_a], m_vertices[m_triangles[i].m_b],
                                  m_vertices[m_triangles[i].m_c], DAT_011b2e90))
            return i;
    }
    return 0xffffffffu;
}
