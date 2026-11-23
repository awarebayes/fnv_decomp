#pragma once

#include <stddef.h>
#include "NiPoint3.h"

class NiPlane;
class NiStream;
class NiTransform;
template <class T> class NiTPrimitiveArray;

// pdb: class NiBound, size 0x10, 3 members
class NiBound
{
  public:
    NiPoint3 m_kCenter;
    union
    {
        float m_fRadius;
        float m_fBoundRadius;
        int m_iRadius;
        float radius;
        unsigned int radiusBits;
    };

    NiBound();
    NiBound(const NiBound& bound);
    static void* operator new[](size_t size);
    const NiBound& operator=(const NiBound& bound);

    bool operator==(const NiBound& bound) const;
    bool IsDifferent(const NiBound& bound);
    bool __thiscall Object_HasEqualFloatComponents(const NiBound& bound);

    void SetGridPosition(float x, float y, float z);
    void SetGridPosition(void* position);
    NiPoint3& GetNavMeshData();
    const NiPoint3& GetNavMeshData() const;
    NiPoint3* FUN_006815c0();
    void SetRadius(float radius);
    float GetRadius();
    float GetRadius() const;
    float FUN_0084d030();
    float FUN_0084d030() const;
    void SetCenterAndRadius(void* center, float radius);
    void SetCenterAndRadius(const NiPoint3& center, float radius);
    void Set(const NiPoint3& center, float radius);
    void ComputeFromPoints(int count, void* points);
    void ComputeFromPoints(int count, const NiPoint3* points);
    void ComputeBoundingSphere(unsigned int vertexCount, const NiPoint3* vertices);
    void NiBound_Merge(const NiBound* pBound);
    void Update(void* data);
    void Update(void* data, void* transform);
    void Update(const NiBound& bound, const NiTransform& transform);
    void TransformPoint(const NiTransform& transform);
    void ComputeMinimalBound(void* bounds);
    void ComputeMinimalBound(NiTPrimitiveArray<const NiBound*>& bounds);
    void Merge(void* bound);
    void Merge(const NiBound* pBound);
    int WhichSide(void* kPlane);
    int WhichSide(const NiPlane& plane);

    static bool TestIntersect(float time, void* bound0, void* velocity0, void* bound1, void* velocity1);
    static bool TestIntersect(float time, const NiBound& bound0, const NiPoint3& velocity0,
                              const NiBound& bound1, const NiPoint3& velocity1);
    static bool FindIntersect(float time, void* bound0, void* velocity0, void* bound1, void* velocity1,
                              float* intersectTime, NiPoint3* intersectPoint, bool calculateNormals,
                              NiPoint3* normal0, NiPoint3* normal1);
    static bool FindIntersect(float time, const NiBound& bound0, const NiPoint3& velocity0,
                              const NiBound& bound1, const NiPoint3& velocity1,
                              float& intersectTime, NiPoint3& intersectPoint, bool calculateNormals,
                              NiPoint3& normal0, NiPoint3& normal1);

    void LoadBinary(NiStream* kStream);
    void LoadBinary(NiStream& kStream);
    void SaveBinary(NiStream* stream);
    void SaveBinary(NiStream& kStream);
    char* FormatToBuffer(char* buffer);
    char* FormatToBuffer(const char* prefix);
    char* GetViewerString_00a7fb20(const char* prefix);
};

typedef char NiBound_size_check[1];
typedef char NiBound_center_offset_check[1];
typedef char NiBound_radius_offset_check[1];
