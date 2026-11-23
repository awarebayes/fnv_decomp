// the shared NiTArray first: NiNode.h then takes it (its own copy is only for units without one)
#include "../../include/NiTArray.h"
// fallout_shared/bgsprimitive.cpp: BGSPrimitiveSphere, BGSPrimitiveBox, BSMultiBoundAABB, BSMultiBoundSphere and more (113 functions)

#include "../../include/BGSPrimitive.h"
#include "../../include/BGSPrimitiveBox.h"
#include "../../include/BGSPrimitivePlane.h"
#include "../../include/BGSPrimitiveSphere.h"
#include "../../include/BSFadeNode.h"
#include "../../include/BSMultiBoundAABB.h"
#include "../../include/BSMultiBoundOBB.h"
#include "../../include/BSMultiBoundSphere.h"
#include "../../include/BSOcclusionPlane.h"
#include "../../include/BSPortal.h"
#include "../../include/BSShaderNoLightingProperty.h"

#include "../../include/NiColorInterpolator.h"
// Both header revisions describe the same public stream type. Keep its class
// tag consistent with the shared NiObject and matrix declarations in this unit.
#pragma push_macro("struct")
#define struct class
#include "../../include/NiStream.h"
#pragma pop_macro("struct")

#include "../../include/NiBound.h"
#include "../../include/NiFixedString.h"
#include "../../include/NiGeometry.h"
#include "../../include/NiGeometryData.h"
#include "../../include/NiLines.h"
#include "../../include/NiAlphaProperty.h"
#include "../../include/NiUpdateData.h"
#include "../../include/NiColorA.h"
#include "../../include/bhkMultiSphereShape.h"
#include "../../include/NiPoint2.h"
#include "../../include/NiColor.h"
#include "../../include/NiObject.h"
#include "../../include/NiPlane.h"
#include "../../include/NiPoint3.h"
// In-class bodies of the former NiPoint3_ext stand-in.
inline NiPoint3::NiPoint3(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ)
{}
inline NiPoint3 NiPoint3::operator-(const NiPoint3& kRhs) const
{
    return NiPoint3(x - kRhs.x, y - kRhs.y, z - kRhs.z);
}
inline NiPoint3 NiPoint3::operator+(const NiPoint3& kRhs) const
{
    return NiPoint3(x + kRhs.x, y + kRhs.y, z + kRhs.z);
}
inline float NiPoint3::operator*(const NiPoint3& point) const
{
    return x * point.x + y * point.y + z * point.z;
}
inline NiPoint3 operator*(float scalar, const NiPoint3& point)
{
    return NiPoint3(scalar * point.x, scalar * point.y, scalar * point.z);
}
#include "../../include/NiPoint3.h"
#include "../../include/NiRefObject.h"
#include "../../include/NiMatrix3.h"
#include "../../include/NiPoint3.h"
#include "../../include/Vec4.h"
#include "../../include/BSMultiBoundShape.h"

class PrimitiveAABB : public BSMultiBoundAABB
{
public:
    PrimitiveAABB(const NiPoint3& center, const NiPoint3& extents);
    PrimitiveAABB* Initialize(const NiPoint3& center, const NiPoint3& extents);
};
extern void* gMultiBoundAABBVtable[];

// NiPoint3 is used for raw point copies to preserve the original bitwise
// assignments; NiPoint3 now has a user-declared assignment operator.
namespace std {
float abs(float);
}

#include "../../include/NiMemObject.h"

typedef char BGSPrimitive_size_check[1];
typedef char BGSPrimitiveBox_size_check[1];
typedef char BSFadeNode_size_check[1];

#include "../../include/GO.h"
#include "../../include/NiTMallocInterface.h"
#include "../../include/NiTPrimitiveArray.h"
#include "../../include/TESCombatStyle.h"
#include "../../include/NiFrustumPlanes.h"

// Takes the absolute value of each radius component, stores it, rebuilds the box and frame geometry vertices, and updates the model and world bounds.
// @fnv 0x004a6640
// pdb: BGSPrimitiveBox::SetRadii | BGSPrimitive.obj | debug, object-file run extension, low
// pdb params: aRadii
// pdb locals: UpdateData, pverts, pgeomdata, frameMult, i, fRad, kBound
void BGSPrimitiveBox::NormalizeRadii(NiPoint3& radii)
{
    radii[0] = std::abs(radii[0]);
    radii[1] = std::abs(radii[1]);
    radii[2] = std::abs(radii[2]);
    reinterpret_cast<NiPoint3&>(m_kRadii) = reinterpret_cast<const NiPoint3&>(radii);
    if (m_kGeometry)
    {
        struct GeometryVertices
        {
            NiPoint3* vertices;
            NiGeometryData* data;
        } geometry;
        geometry.data = m_kGeometry->GetModelData();
        geometry.vertices = geometry.data->GetVertices();
        reinterpret_cast<NiPoint3&>(geometry.vertices[0]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[1]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[2]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], -m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[3]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], -m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[4]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[5]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[6]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], -m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(geometry.vertices[7]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], -m_kRadii[1], -m_kRadii[2]));
        geometry.data->SetDirtyFlags(1);
    }
    if (spFrame)
    {
        NiGeometryData* pkData = spFrame->GetModelData();
        {
            struct FrameVertices
            {
                NiPoint3* vertices;
                NiPoint3 scale;
            } frame = {pkData->GetVertices(), reinterpret_cast<const NiPoint3&>(radii)};
            if (std::abs(frame.scale.x) < (double)0.01f)
                frame.scale.x = 0.01f;
            if (std::abs(frame.scale.y) < (double)0.01f)
                frame.scale.y = 0.01f;
            if (std::abs(frame.scale.z) < (double)0.01f)
                frame.scale.z = 0.01f;
            for (int i = 0; i < pkData->NiGeometryData::GetVertexCount(); i++)
            {
                frame.vertices[i].x = frame.vertices[i].x < 0.0 ? -1.0f : 1.0f;
                frame.vertices[i].y = frame.vertices[i].y < 0.0 ? -1.0f : 1.0f;
                frame.vertices[i].z = frame.vertices[i].z < 0.0 ? -1.0f : 1.0f;
                frame.vertices[i].x *= frame.scale.x;
                frame.vertices[i].y *= frame.scale.y;
                frame.vertices[i].z *= frame.scale.z;
                pkData->SetDirtyFlags(1);
            }
        }
    }
    NiUpdateData kUpdateData(0.0f, false, false);
    if (m_pController)
    {
        float radius = radii.x;
        if (radii.y > radii.x && radii.y > radii.z)
            radius = radii.y;
        if (radii.z > radii.y && radii.z > radii.x)
            radius = radii.z;
        NiBound bound;
        bound.SetCenterAndRadius(NiPoint3::ZERO, radius);
        m_kGeometry->SetModelBound(bound);
        static_cast<NiAVObject*>(m_pController)->Update(kUpdateData);
        m_kGeometry->UpdateSceneObject();
        m_pController->UpdateWorldBound();
    }
}

// @fnv 0x004a53e0
// pdb: BGSPrimitivePlane::SetColor | BGSPrimitive.obj | retail, upstream name already matches, high
// pdb params: aColor
// pdb locals: pcolors, pgeomdata
void BGSPrimitivePlane::SetColor(NiColorA& aColor)
{
    Color = aColor;
    if (m_kGeometry)
    {
        struct GeometryColors
        {
            NiColorA* colors;
            NiGeometryData* data;
        } geometry;
        geometry.data = m_kGeometry->GetModelData();
        geometry.colors = reinterpret_cast<NiColorA*>(geometry.data->GetColors());
        geometry.colors[0] = geometry.colors[1] = geometry.colors[2] = geometry.colors[3] = geometry.colors[4] =
            geometry.colors[5] = geometry.colors[6] = geometry.colors[7] = reinterpret_cast<const NiColorA&>(Color);
        geometry.data->SetDirtyFlags(4);
    }
    if (spFrame)
        spFrame->SetAppCulled(aColor.a < 0.001);
}

// Stores color, then if geometry exists, fills both geometry data color arrays with it and sets app-culled based on alpha.
// @fnv 0x004a64e0
// pdb: BGSPrimitiveBox::SetColor | BGSPrimitive.obj | retail, upstream name already matches, high
// pdb params: aColor
// pdb locals: i, pcolors, pgeomdata
void BGSPrimitiveBox::SetColor(NiColorA& aColor)
{
    Color = aColor;
    if (m_kGeometry)
    {
        struct GeometryColors
        {
            NiColorA* colors;
            NiGeometryData* data;
        } geometry;
        geometry.data = m_kGeometry->GetModelData();
        geometry.colors = reinterpret_cast<NiColorA*>(geometry.data->GetColors());
        int i;
        for (i = 0; i < geometry.data->NiGeometryData::GetVertexCount(); i++)
            geometry.colors[i] = reinterpret_cast<const NiColorA&>(Color);
        geometry.data->SetDirtyFlags(4);
        geometry.data = spFrame->GetModelData();
        geometry.colors = reinterpret_cast<NiColorA*>(geometry.data->GetColors());
        for (i = 0; i < geometry.data->NiGeometryData::GetVertexCount(); i++)
            geometry.colors[i] = reinterpret_cast<const NiColorA&>(Color);
        geometry.data->SetDirtyFlags(4);
        spFrame->SetAppCulled(aColor.a < 0.001);
    }
}

// Creates a BSFadeNode named "Box" and stores it in the primitive's node pointer if not already created.
// @fnv 0x004a63f0
// pdb: BGSPrimitiveBox::Build | BGSPrimitive.obj | retail, upstream name already matches, high
// pdb locals: frameMult, alpha
void BGSPrimitiveBox::Build()
{
    if (!m_pController)
    {
        m_pController = new BSFadeNode;
        static_cast<NiAVObject*>(m_pController)->SetName(NiFixedString("Box"));
        m_pController->UpdateSceneObject();
    }
}

// Initializes the primitive box: clears two fields and sets eType to 1.
// @fnv 0x004a6370
// pdb: BGSPrimitiveBox::BGSPrimitiveBox | BGSPrimitive.obj | retail, upstream name already matches, high
BGSPrimitiveBox::BGSPrimitiveBox()
{
    m_kGeometry = 0;
    m_pController = 0;
    eType = 1;
}

// @fnv 0x004a60f0
unsigned int bhkMultiSphereShape::GetInstanceSize()
{
    return 16;
}

// Compares this object's first four floats with those of p and returns true only if all four are equal.
// @fnv 0x004a4cc0
// pdb: NiColorInterpolator::GetKeys | TESImageSpaceModifier.obj | debug, call graph, medium
// pdb params: uiNumKeys, eType, ucSize
bool NiColorA::operator==(const NiColorA& apOther) const
{
    return r == apOther.r && g == apOther.g && b == apOther.b && a == apOther.a;
}

// @fnv 0x004a51d0
// pdb: NiBound::NiBound | BGSPrimitive.obj | debug, object-file run extension, low
void NiBound::SetCenterAndRadius(const NiPoint3& apCenter, float fRadius)
{
    reinterpret_cast<NiPoint3&>(m_kCenter) = reinterpret_cast<const NiPoint3&>(apCenter);
    m_fRadius = fRadius;
}

// Copies the center and radius from the given NiBound into this bound and returns this.
// @fnv 0x004a5250
// pdb: NiBound::operator= | BGSPrimitive.obj | debug, object-file gap fill, medium
// pdb params: aRhs
const NiBound& NiBound::operator=(const NiBound& aRhs)
{
    SetCenterAndRadius(aRhs.m_kCenter, aRhs.m_fRadius);
    return *this;
}

void __fastcall _ref_006815c0(void* pThis);

// @fnv 0x006240d0
// src: GUESS 0x006240d0 (confidence: low)
// pdb: BGSDistantTreeBlock::InstanceData::InstanceData | BGSDistantTreeBlock.obj | debug, call graph, medium
void* __fastcall BGSPrimitive_EvaluateValue_006240d0(void* pThis)
{
    _ref_006815c0(pThis);
    return pThis;
}


// @fnv 0x00c37ec0
// src: GUESS 0x00c37ec0 (confidence: low)
// pdb: BSMultiBoundAABB::BSMultiBoundAABB | BSMultiBoundAABB.obj | debug, call graph, medium
void* __fastcall Object_ProcessAndReturnValue_00c37ec0(void* this_ptr)
{
    static_cast<BSMultiBoundShape*>(this_ptr)->BSMultiBoundShape::BSMultiBoundShape();
    *(void***)this_ptr = gMultiBoundAABBVtable;
    return this_ptr;
}

// @fnv 0x004a6100
// src: GUESS 0x004a6100 (confidence: high)
int __fastcall Object_Return0x1e(int this_)
{
    return 0x1e;
}

// @fnv 0x004a7040
// pdb: BSMultiBoundAABB::QMultiBoundRadius | BGSPrimitive.obj | debug, vtable slot, medium
float BSMultiBoundAABB::GetPointLength()
{
    return static_cast<const NiPoint3&>(HalfExtents).Length();
}

extern int BSMultiBoundOBB_ms_RTTI; // src: XBOX_RETAIL_PDB 0x01202818 (old: g_01202818)

struct FUN_004a70f0_shell
{
    int* __thiscall BSMultiBoundOBB_GetRTTI();
};

// @fnv 0x004a70f0
// src: COMMUNITY 0x004a70f0
int* __thiscall FUN_004a70f0_shell::BSMultiBoundOBB_GetRTTI()
{
    return &BSMultiBoundOBB_ms_RTTI;
}

// @fnv 0x00c365e0
// src: COMMUNITY 0x00c365e0
int __fastcall BSMultiBoundOBB_GetType(int)
{
    return 2;
}

struct FUN_00c382a0_shell
{
    int BSMultiBoundAABB_GetType();
};

// @fnv 0x00c382a0
// src: COMMUNITY 0x00c382a0
int FUN_00c382a0_shell::BSMultiBoundAABB_GetType()
{
    return 1;
}

void* g_012030b8;

struct FUN_00c4f8d0_shell
{
    void* __thiscall GetGlobalPtr();
};

// @fnv 0x00c4f8d0
void* __thiscall FUN_00c4f8d0_shell::GetGlobalPtr()
{
    return &g_012030b8;
}

struct FUN_00c4fec0_shell
{
    int __thiscall BSMultiBoundSphere_GetShape();
};

// @fnv 0x00c4fec0
// src: COMMUNITY 0x00c4fec0
int __thiscall FUN_00c4fec0_shell::BSMultiBoundSphere_GetShape()
{
    return 3;
}

struct PrimitiveBoxContainment : public BGSPrimitiveBox
{
    bool IsPointInsideBox(NiPoint3& aPoint);
};

// Returns true when aPoint lies within the axis-aligned box given by the negated and positive extents stored at offset 0x18 of this.
// @fnv 0x004a6d90
bool PrimitiveBoxContainment::IsPointInsideBox(NiPoint3& aPoint)
{
    if (-m_kRadii.x > aPoint.x || -m_kRadii.y > aPoint.y ||
        -m_kRadii.z > aPoint.z || aPoint.x > m_kRadii.x ||
        aPoint.y > m_kRadii.y || aPoint.z > m_kRadii.z)
        return false;
    return true;
}

struct FUN_00c335d0 : NiRefObject
{
    static void* operator new(size_t uiSize);
    static void operator delete(void*, size_t);
    NiPoint3 kTargetCenter;
    NiPoint2 m_kPoint2;
    NiMatrix3 kRotationBasis;
    int m_uiShapeType;
    NiFrustumPlanes m_kFrustumPlanes;
    NiPoint3 m_kCornerPoints[4];
    NiPoint3 m_kReferencePoint;
    bool m_bIsInFrustum;
    bool m_bIsVisible;
    bool m_bCornerVisible[4];
    bool m_bDecalTextureCreated;
    FUN_00c335d0* m_apDecalObjects[4];
    FUN_00c335d0();
};
struct FUN_00c33730_shell
{
    void CopyStateTo(FUN_00c335d0* pkDest, CloningProcess& kCloning);
    FUN_00c335d0* BSOcclusionPlane_CreateClone(CloningProcess& arCloneData);
};

// @fnv 0x00c33730
// src: XBOX_PDB 0x00c33730
// pdb: BSOcclusionPlane::CreateClone | BSOcclusionPlane.obj | retail, upstream name already matches, high
// pdb params: arCloneData
// pdb locals: pobj
FUN_00c335d0* FUN_00c33730_shell::BSOcclusionPlane_CreateClone(CloningProcess& arCloneData)
{
    FUN_00c335d0* pkObject = new FUN_00c335d0;
    CopyStateTo(pkObject, arCloneData);
    return pkObject;
}

namespace fnv_00c36330 {

// The shared OBB has no clone/copy declarations yet.
struct MultiBoundOBBClone : public BSMultiBoundOBB
{
    void CopyTo(BSMultiBoundOBB* pkDest, CloningProcess& kCloning);
    BSMultiBoundOBB* Clone(CloningProcess& arCloneData);
};

// @fnv 0x00c36330
// pdb: BSMultiBoundOBB::CreateClone | BSMultiBoundOBB.obj | retail, vtable slot, high
// pdb params: arCloneData
// pdb locals: pobj
BSMultiBoundOBB* MultiBoundOBBClone::Clone(CloningProcess& arCloneData)
{
    BSMultiBoundOBB* pkObject = new BSMultiBoundOBB;
    CopyTo(pkObject, arCloneData);
    return pkObject;
}
}  // namespace fnv_00c36330

namespace fnv_00c37ee0 {

struct MultiBoundAABBCloneHelper
{
    void FUN_00c37f70(BSMultiBoundAABB* pkDest, CloningProcess& kCloning);
    BSMultiBoundAABB* BSMultiBoundAABB_CreateClone(CloningProcess& arCloneData);
};

// @fnv 0x00c37ee0
// src: XBOX_PDB 0x00c37ee0
// pdb: BSMultiBoundAABB::CreateClone | BSMultiBoundAABB.obj | retail, upstream name already matches, high
// pdb params: arCloneData
// pdb locals: pobj
BSMultiBoundAABB* MultiBoundAABBCloneHelper::BSMultiBoundAABB_CreateClone(CloningProcess& arCloneData)
{
    BSMultiBoundAABB* pkObject = new BSMultiBoundAABB;
    FUN_00c37f70(pkObject, arCloneData);
    return pkObject;
}
}  // namespace fnv_00c37ee0

// @fnv 0x00c4fb30
// src: XBOX_PDB 0x00c4fb30
// pdb: BSMultiBoundSphere::CreateClone | BSMultiBoundSphere.obj | debug, vtable slot, medium
// pdb params: arCloneData
// pdb locals: pobj
BSMultiBoundSphere* BSMultiBoundSphere::CreateClone_00c4fb30(CloningProcess& kCloning)
{
    BSMultiBoundSphere* pkObject = new BSMultiBoundSphere;
    CopyMembers_00c4fbc0(pkObject, kCloning);
    return pkObject;
}

extern char const* const g_012030b0_arg1;

struct FUN_004a60e0_shell
{
    char const* const* __thiscall BSOcclusionPlane_GetRTTI();
};

// @fnv 0x004a60e0
// src: COMMUNITY 0x004a60e0
char const* const* __thiscall FUN_004a60e0_shell::BSOcclusionPlane_GetRTTI()
{
    return &g_012030b0_arg1;
}

struct FUN_004a62c0_shell
{
    int* BSPortal_GetRTTI();
};

extern int g_012030b0;

// @fnv 0x004a62c0
// src: COMMUNITY 0x004a62c0
int* FUN_004a62c0_shell::BSPortal_GetRTTI()
{
    return &g_012030b0;
}

// @fnv 0x004a7030
// src: COMMUNITY 0x004a7030
int BSMultiBoundAABB::GetDebugGeomShapeSize()
{
    return 0x24;
}

// @fnv 0x004a71d0
// src: XBOX_PDB 0x004a71d0
// pdb: BGSPrimitiveSphere::SetRadii | BGSPrimitive.obj | debug, upstream name already matches, high
// pdb params: aRadii
void BGSPrimitiveSphere::SetRadii(NiPoint3& aRadii)
{
    SetRadius(aRadii.x);
}

// This destructor entry calls the base directly without installing a vtable.
struct PrimitiveSphereDestruction
{
    void Destroy();
};
// @fnv 0x004a7220
// pdb: BGSPrimitiveSphere::~BGSPrimitiveSphere | BGSPrimitive.obj | debug, object-file gap fill, medium
void PrimitiveSphereDestruction::Destroy()
{
    reinterpret_cast<BGSPrimitive*>(this)->BGSPrimitive::~BGSPrimitive();
}

// Calls DestroyBase, frees this with _delete when flags has bit 0 set, and returns this.
// @fnv 0x004a7bf0
// pdb: BSMultiBoundSphere::`scalar deleting destructor' | BGSPrimitive.obj | debug, vtable slot, medium
void* BSMultiBoundSphere::FUN_004a7bf0(unsigned int flags)
{
    DestroyBase();
    if (flags & 1)
        _delete(this, 28);
    return this;
}

struct FUN_00c38020_shell
{
    unsigned char __thiscall BSMultiBoundShape_RegisterStreamables(int arStream);
    unsigned char __thiscall BSMultiBoundAABB_RegisterStreamables(int arStream);
};

// Returns 0 if FUN_00c4fa30(a) is false, otherwise returns 1.
// @fnv 0x00c38020
// src: XBOX_PDB 0x00c38020
// pdb: BSMultiBoundAABB::RegisterStreamables | BSMultiBoundAABB.obj | retail, upstream name already matches, high
// pdb params: arStream
unsigned char __thiscall FUN_00c38020_shell::BSMultiBoundAABB_RegisterStreamables(int arStream)
{
    if (!BSMultiBoundShape_RegisterStreamables(arStream))
        return 0;
    return 1;
}

struct FUN_00c4f6c0_shell
{
    void BSPortalSharedNode_CopyMembers(int, int);
};

// @fnv 0x00c4f6c0
// src: XBOX_PDB 0x00c4f6c0
void FUN_00c4f6c0_shell::BSPortalSharedNode_CopyMembers(int, int)
{}

struct FUN_00c4f550_shell
{
    void FUN_00c4f550();
};
void __cdecl _delete(void* p, unsigned int size);

struct FUN_00c4f820_shell
{
    void* FUN_00c4f820(unsigned int flags);
};

// @fnv 0x00c4f820
// pdb: BSPortalSharedNode::`scalar deleting destructor' | BSPortal.obj | retail, vtable slot, high
void* FUN_00c4f820_shell::FUN_00c4f820(unsigned int flags)
{
    reinterpret_cast<FUN_00c4f550_shell*>(this)->FUN_00c4f550();
    if (flags & 1)
        _delete(this, 0xc4);
    return this;
}

struct FUN_00c4f9f0_shell : NiObject
{
    void BSMultiBoundShape_LoadBinary(NiStream& arStream);
};

// @fnv 0x00c4f9f0
// src: XBOX_PDB 0x00c4f9f0
// pdb: BSMultiBoundShape::LoadBinary | BSMultiBoundShape.obj | debug, upstream name already matches, high
// pdb params: arStream
void FUN_00c4f9f0_shell::BSMultiBoundShape_LoadBinary(NiStream& arStream)
{
    NiObject::LoadBinary(arStream);
}

namespace fnv_00c4fa10 {

struct FUN_00c4fa10_shell : NiObject
{
    void BSMultiBoundShape_LinkObject(NiStream& arStream);
};

// @fnv 0x00c4fa10
// src: XBOX_PDB 0x00c4fa10
// pdb: BSMultiBoundShape::LinkObject | BSMultiBoundShape.obj | debug, upstream name already matches, high
// pdb params: arStream
void FUN_00c4fa10_shell::BSMultiBoundShape_LinkObject(NiStream& arStream)
{
    NiObject::LinkObject(arStream);
}
}  // namespace fnv_00c4fa10

namespace fnv_00c4fa30 {

struct FUN_00c4fa30_shell : NiObject
{
    bool BSMultiBoundShape_RegisterStreamables(NiStream& arStream);
};

// Calls the base NiObject::RegisterStreamables and returns its result.
// @fnv 0x00c4fa30
// src: XBOX_PDB 0x00c4fa30
// pdb: BSMultiBoundShape::RegisterStreamables | BSMultiBoundShape.obj | retail, upstream name already matches, high
// pdb params: arStream
bool FUN_00c4fa30_shell::BSMultiBoundShape_RegisterStreamables(NiStream& arStream)
{
    if (!NiObject::RegisterStreamables(arStream))
        return false;
    return true;
}
}  // namespace fnv_00c4fa30

namespace fnv_00c4fa60 {

struct FUN_00c4fa60_shell : NiObject
{
    void BSMultiBoundShape_SaveBinary(NiStream& arStream);
};

// @fnv 0x00c4fa60
// src: XBOX_PDB 0x00c4fa60
// pdb: BSMultiBoundShape::SaveBinary | BSMultiBoundShape.obj | debug, upstream name already matches, high
// pdb params: arStream
void FUN_00c4fa60_shell::BSMultiBoundShape_SaveBinary(NiStream& arStream)
{
    NiObject::LinkObject(arStream);
}
}  // namespace fnv_00c4fa60

extern char const* const g_01202820_arg1;

struct FUN_00c4fb00_shell
{
    char const* const* BSMultiBoundShape_GetRTTI();
};

// @fnv 0x00c4fb00
// src: COMMUNITY 0x00c4fb00
char const* const* FUN_00c4fb00_shell::BSMultiBoundShape_GetRTTI()
{
    return &g_01202820_arg1;
}

// Returns the logical negation of color equality.
// @fnv 0x004a4c90
// pdb: NiColorA::operator!= | BGSPrimitive.obj | debug, call graph, medium
// pdb params: c
bool NiColorA::operator!=(const NiColorA& c) const
{
    return !operator==(c);
}

void __cdecl _delete(void*, unsigned int);

// @fnv 0x004a6110
// pdb: BSOcclusionPlane::`scalar deleting destructor' | BGSPrimitive.obj | retail, vtable slot, high
void* BSOcclusionPlane::FUN_004a6110(unsigned int flags)
{
    this->BSOcclusionPlane::~BSOcclusionPlane();
    if (flags & 1)
        _delete(this, 0xfc);
    return this;
}

void __cdecl _delete(void*, unsigned int);
// @fnv 0x004a62d0
// pdb: BSPortal::`scalar deleting destructor' | BGSPrimitive.obj | retail, vtable slot, high
void* BSPortal::Destroy(unsigned int flags)
{
    DestroyBase();
    if (flags & 1)
        _delete(this, 0x108);
    return this;
}

// BSPortal's destructor body (BSPortal::DestroyBase, called by the scalar deleting destructor 0x004a62d0): it destroys
// m_104 (0x0045cec0), then runs 0x004a7090 (BSOcclusionPlane's teardown, straight to ~NiRefObject 0x00a5d3d0); neither
// stores a vftable, which no destructor of the virtual shared classes reproduces, so these two views carry the shape.
struct BSOcclusionPlaneTeardown_004a7090
{
    ~BSOcclusionPlaneTeardown_004a7090();
    char _pad[0x104];
};
struct BSPortalTeardown_004a6310 : BSOcclusionPlaneTeardown_004a7090
{
    GO field_104;
    ~BSPortalTeardown_004a6310();
};
// @fnv 0x004a6310
// pdb: BSPortal::~BSPortal | BGSPrimitive.obj | debug, call graph, medium
BSPortalTeardown_004a6310::~BSPortalTeardown_004a6310()
{}

void __cdecl _delete(void*, unsigned int);

// Calls DestroyBase on this, optionally frees this with _delete when flags has bit 0 set, and returns this.
// @fnv 0x004a7060
// pdb: BSMultiBoundAABB::`scalar deleting destructor' | BGSPrimitive.obj | debug, vtable slot, medium
void* BSMultiBoundAABB::FUN_004a7060(unsigned int flags)
{
    reinterpret_cast<BSMultiBoundSphere*>(this)->DestroyBase();
    if (flags & 1)
        _delete(this, 0x24);
    return this;
}

void __cdecl _delete(void*, unsigned int);
// @fnv 0x004a7100
// pdb: BSMultiBoundOBB::`scalar deleting destructor' | BGSPrimitive.obj | retail, vtable slot, high
void* BSMultiBoundOBB::FUN_004a7100(unsigned int flags)
{
    this->DestroyBase_Thunk();
    if (flags & 1)
        _delete(this, 72);
    return this;
}

void _Deallocate(void*);
// Calls the sphere destructor and optionally deallocates the primitive.
// @fnv 0x004a71f0
void* BGSPrimitiveSphere::FUN_004a71f0(unsigned int flags)
{
    this->BGSPrimitiveSphere::~BGSPrimitiveSphere();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

// @fnv 0x004a7240
// src: XBOX_PDB 0x004a7240
// pdb: BGSPrimitiveSphere::IsInside | BGSPrimitive.obj | debug, upstream name already matches, high
// pdb params: aPoint
bool BGSPrimitiveSphere::IsInside(TESCombatStyle& aPoint)
{
    return (double)*(float*)((char*)this + 0x18) * *(float*)((char*)this + 0x18) > aPoint.UpdateCombatStyle();
}

extern int DAT_0101ea8c;
// Keep the original constructor entry's explicit base initialization: expressing
// this as a C++ constructor adds exception unwinding absent from this entry.
struct MultiBoundSphereInitialization : public BSMultiBoundSphere
{
    BSMultiBoundSphere* Initialize(const NiPoint3& point, float radius);
};

// Initializes the base, center and radius.
// @fnv 0x004a7b40
// pdb: BSMultiBoundSphere::BSMultiBoundSphere | BGSPrimitive.obj | debug, object-file run extension, low
// pdb params: arCenter, afRadius
BSMultiBoundSphere* MultiBoundSphereInitialization::Initialize(const NiPoint3& aPt, float aRadius)
{
    this->BSMultiBoundShape::BSMultiBoundShape();
    *(void**)this = (void*)&DAT_0101ea8c;
    Center = aPt;
    Radius = aRadius;
    return this;
}

namespace fnv_00c36400 {

struct MultiBoundOBBLoad : public BSMultiBoundOBB
{
    void LoadBase(NiStream&);
    void LoadBinary(NiStream&);
};
// Loads the object from the stream: calls FUN_00c37fc0(s), then loads the member via LoadBinary(s).
// @fnv 0x00c36400
// pdb: BSMultiBoundOBB::LoadBinary | BSMultiBoundOBB.obj | retail, vtable slot, high
// pdb params: arStream
void MultiBoundOBBLoad::LoadBinary(NiStream& arStream)
{
    LoadBase(arStream);
    reinterpret_cast<NiMatrix3&>(unk24).LoadBinary(arStream);
}
}  // namespace fnv_00c36400

namespace fnv_00c36450 {
struct FUN_00c38020_shell
{
    unsigned char BSMultiBoundAABB_RegisterStreamables(int);
};
struct FUN_00c36450_shell : FUN_00c38020_shell
{
    bool BSMultiBoundOBB_RegisterStreamables(int);
};
// @fnv 0x00c36450
// src: XBOX_PDB 0x00c36450
// pdb: BSMultiBoundOBB::RegisterStreamables | BSMultiBoundOBB.obj | retail, upstream name already matches, high
// pdb params: arStream
bool FUN_00c36450_shell::BSMultiBoundOBB_RegisterStreamables(int arStream)
{
    if (!BSMultiBoundAABB_RegisterStreamables(arStream))
        return false;
    return true;
}
}  // namespace fnv_00c36450

struct MultiBoundOBBSave : public BSMultiBoundOBB
{
    void SaveBase(NiStream*);
    void SaveBinary(NiStream*);
};
// @fnv 0x00c36480
// pdb: BSMultiBoundOBB::SaveBinary | BSMultiBoundOBB.obj | retail, vtable slot, high
// pdb params: arStream
void MultiBoundOBBSave::SaveBinary(NiStream* arStream)
{
    SaveBase(arStream);
    reinterpret_cast<NiMatrix3&>(unk24).SaveBinary(*arStream);
}

namespace fnv_00c37110 {
struct Pt
{
    float Length() const;
};
struct Vec12
{
    float x;
    float y;
    float z;
};
struct Out12
{
    Vec12 v;
    float l;
};
struct S37110
{
    char _pad[12];
    Vec12 v;
    Pt p;
    void GetVectorAndLength(Out12* a);
};
// Copies v into a->v and stores the length of p into a->l.
// @fnv 0x00c37110
// pdb: BSMultiBoundOBB::GetNiBound | BSMultiBoundOBB.obj | debug, vtable slot, medium
// pdb params: aBound
void S37110::GetVectorAndLength(Out12* apOut)
{
    int dummy;
    {
        float f = p.Length();
        apOut->v = v;
        apOut->l = f;
    }
}
}  // namespace fnv_00c37110

namespace fnv_00c37fc0 {

struct FUN_00c4f9f0_shell
{
    char _pad[12];
    void BSMultiBoundShape_LoadBinary(class NiStream&);
};
struct FUN_00c37fc0_shell : FUN_00c4f9f0_shell
{
    NiColor unk0C;
    NiColor unk18;
    void BSMultiBoundAABB_LoadBinary(class NiStream&);
};
// Calls the base loader, then reads two NiColor members from the stream.
// @fnv 0x00c37fc0
// src: XBOX_PDB 0x00c37fc0
// pdb: BSMultiBoundAABB::LoadBinary | BSMultiBoundAABB.obj | retail, upstream name already matches, high
// pdb params: arStream
void FUN_00c37fc0_shell::BSMultiBoundAABB_LoadBinary(class NiStream& arStream)
{
    FUN_00c4f9f0_shell::BSMultiBoundShape_LoadBinary(arStream);
    unk0C.LoadBinary(arStream);
    unk18.LoadBinary(arStream);
}
}  // namespace fnv_00c37fc0

struct FUN_00c38050_Holder
{
    char _pad[12];
    NiColor c1;
    NiColor c2;
    void BSMultiBoundShape_SaveBinary(NiStream&);
    void BSMultiBoundAABB_SaveBinary(NiStream&);
};
// @fnv 0x00c38050
// src: XBOX_PDB 0x00c38050
// pdb: BSMultiBoundAABB::SaveBinary | BSMultiBoundAABB.obj | retail, upstream name already matches, high
// pdb params: arStream
void FUN_00c38050_Holder::BSMultiBoundAABB_SaveBinary(NiStream& arStream)
{
    BSMultiBoundShape_SaveBinary(arStream);
    c1.SaveBinary(arStream);
    c2.SaveBinary(arStream);
}

// @fnv 0x00c389e0
// src: XBOX_PDB 0x00c389e0
// pdb: BSMultiBoundAABB::GetNiBound | BSMultiBoundAABB.obj | debug, upstream name already matches, high
// pdb params: aBound
void BSMultiBoundAABB::GetNiBound(Vec4* aBound)
{
    int pad;
    {
        float len = static_cast<const NiPoint3&>(HalfExtents).Length();
        *reinterpret_cast<NiPoint3*>(aBound) = reinterpret_cast<const NiPoint3&>(Center);
        aBound->w = len;
    }
}

namespace fnv_00c4f9b0 {

struct FUN_00c4f9b0_class : BSMultiBoundShape
{
    void CopyMembersStreamWithField(NiObject*, void*);
};
// Calls NiObject::CopyMembersStream on pDest with pArg, then copies m08 into pDest->m08.
// @fnv 0x00c4f9b0
void FUN_00c4f9b0_class::CopyMembersStreamWithField(NiObject* pDest, void* pArg)
{
    int tmp;
    NiObject::CopyMembersStream((unsigned int)pDest, pArg);
    tmp = m_uiShapeType;
    static_cast<BSMultiBoundShape*>(pDest)->m_uiShapeType = tmp;
}
}  // namespace fnv_00c4f9b0

struct TCEq
{
    bool IsSameAs(class NiObject*);
    bool BSMultiBoundShape_IsEqual(class NiObject*);
};
// @fnv 0x00c4fa80
// src: XBOX_PDB 0x00c4fa80
// pdb: BSMultiBoundShape::IsEqual | BSMultiBoundShape.obj | debug, upstream name already matches, high
// pdb params: apObject
// pdb locals: bres
bool TCEq::BSMultiBoundShape_IsEqual(class NiObject* apObject)
{
    bool result = IsSameAs(apObject);
    return result;
}

namespace fnv_00c4fad0 {

void __cdecl _delete(void*, unsigned int);
struct FUN_00c4fad0_host : NiRefObject
{
    void* FUN_00c4fad0(unsigned int);
};
// Destructs the NiRefObject base and, when flags has bit 0 set, frees this with _delete, returning this.
// @fnv 0x00c4fad0
// pdb: BSMultiBoundShape::`scalar deleting destructor' | BSMultiBoundShape.obj | retail, vtable slot, high
void* FUN_00c4fad0_host::FUN_00c4fad0(unsigned int flags)
{
    this->NiRefObject::~NiRefObject();
    if (flags & 1)
        fnv_00c4fad0::_delete(this, 12);
    return this;
}
}  // namespace fnv_00c4fad0

namespace fnv_00c4fc10 {

struct FUN_00c4f9f0_shell
{
    void BSMultiBoundShape_LoadBinary(NiStream&);
};
template <typename T> void ReadBinaryValue(NiStream&, T&);
struct MultiBoundSphereLoad : public BSMultiBoundSphere
{
    void BSMultiBoundSphere_LoadBinary(NiStream&);
};
// @fnv 0x00c4fc10
// src: XBOX_PDB 0x00c4fc10
// pdb: BSMultiBoundSphere::LoadBinary | BSMultiBoundSphere.obj | retail, upstream name already matches, high
// pdb params: arStream
void MultiBoundSphereLoad::BSMultiBoundSphere_LoadBinary(NiStream& arStream)
{
    ((FUN_00c4f9f0_shell*)this)->BSMultiBoundShape_LoadBinary(arStream);
    reinterpret_cast<NiColor&>(Center).LoadBinary(arStream);
    ReadBinaryValue<unsigned int>(arStream, reinterpret_cast<unsigned int&>(Radius));
}
}  // namespace fnv_00c4fc10

namespace fnv_00c4fc70 {
struct FUN_00c38020_shell
{
    unsigned char BSMultiBoundShape_RegisterStreamables(int);
    unsigned char BSMultiBoundSphere_RegisterStreamables(int);
};
// Returns 1 if FUN_00c4fa30 returns nonzero for a, otherwise returns 0.
// @fnv 0x00c4fc70
// src: XBOX_PDB 0x00c4fc70
// pdb: BSMultiBoundSphere::RegisterStreamables | BSMultiBoundSphere.obj | debug, upstream name already matches, high
// pdb params: arStream
unsigned char FUN_00c38020_shell::BSMultiBoundSphere_RegisterStreamables(int arStream)
{
    if (!BSMultiBoundShape_RegisterStreamables(arStream))
        return 0;
    return 1;
}
}  // namespace fnv_00c4fc70

namespace fnv_00c4fca0 {

struct Holder60
{
    void BSMultiBoundShape_SaveBinary(NiStream&);
};
void __cdecl CallAnimCallback(NiStream&, void*);
struct MultiBoundSphereSave : public BSMultiBoundSphere
{
    void SaveToStream(NiStream&);
};
// @fnv 0x00c4fca0
// pdb: BSMultiBoundSphere::BSMultiBoundSphere | BSMultiBoundSphere.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSMultiBoundSphere::SaveBinary
// pdb params: arCenter, afRadius
void MultiBoundSphereSave::SaveToStream(NiStream& s)
{
    ((Holder60*)this)->BSMultiBoundShape_SaveBinary(s);
    reinterpret_cast<NiColor&>(Center).SaveBinary(s);
    CallAnimCallback(s, &Radius);
}
}  // namespace fnv_00c4fca0

extern char const* const g_01202818_arg1;

struct FUN_004a7020_shell
{
    char const* const* __thiscall BSMultiBoundAABB_GetRTTI();
};

// @fnv 0x004a7020
// src: COMMUNITY 0x004a7020
char const* const* __thiscall FUN_004a7020_shell::BSMultiBoundAABB_GetRTTI()
{
    return &g_01202818_arg1;
}

// @fnv 0x004a7130
// src: GUESS 0x004a7130 (confidence: medium)
// pdb: BSMultiBoundOBB::~BSMultiBoundOBB | BGSPrimitive.obj | debug, call graph, medium
void BSMultiBoundSphere::DestroyBase_Thunk()
{
    DestroyBase();
}

// @fnv 0x004a7830
// src: XBOX_PDB 0x004a7830
// pdb: BGSPrimitiveBox::MakeMultiBoundShape | BGSPrimitive.obj | retail, object-file run extension, low
// pdb note: debug build links it to BGSPrimitiveSphere::SetColor
// pdb params: akAngle
// pdb locals: abOriented, mat, pshape
void BGSPrimitiveSphere::SetColor_004a7830(NiColorA& apTextColor)
{
    Color = apTextColor;
    if (m_kGeometry)
    {
        NiGeometryData* pgeomdata = m_kGeometry->GetModelData();
        NiColorA* pcolors = reinterpret_cast<NiColorA*>(pgeomdata->GetColors());
        int i2 = 0;
        for (; i2 < 112; ++i2)
            pcolors[i2] = reinterpret_cast<const NiColorA&>(Color);
        pgeomdata->SetDirtyFlags(4);
    }
}

extern "C" void* g_01203108;

struct FUN_004a7b90_shell
{
    void* __thiscall GetGlobalPointer();
};

// @fnv 0x004a7b90
void* __thiscall FUN_004a7b90_shell::GetGlobalPointer()
{
    return &g_01203108;
}

namespace fnv_004a7c20 {
struct FUN_004a7090
{
    ~FUN_004a7090();
};

struct FUN_004a7c20_shell : FUN_004a7090
{
    void DestroyBase();
};

// @fnv 0x004a7c20
void FUN_004a7c20_shell::DestroyBase()
{
    FUN_004a7090::~FUN_004a7090();
}
}  // namespace fnv_004a7c20

struct FUN_00c36430_shell
{
    void BSMultiBoundAABB_LinkObject(int);
    void BSMultiBoundOBB_LinkObject(int arStream);
};

// @fnv 0x00c36430
// src: XBOX_PDB 0x00c36430
// pdb: BSMultiBoundOBB::LinkObject | BSMultiBoundOBB.obj | debug, upstream name already matches, high
// pdb params: arStream
void FUN_00c36430_shell::BSMultiBoundOBB_LinkObject(int arStream)
{
    BSMultiBoundAABB_LinkObject(arStream);
}

struct FUN_00c38000_shell
{
    void BSMultiBoundShape_LinkObject(int);
    void BSMultiBoundAABB_LinkObject(int arStream);
};

// @fnv 0x00c38000
// src: XBOX_PDB 0x00c38000
// pdb: BSMultiBoundAABB::LinkObject | BSMultiBoundAABB.obj | debug, upstream name already matches, high
// pdb params: arStream
void FUN_00c38000_shell::BSMultiBoundAABB_LinkObject(int arStream)
{
    BSMultiBoundShape_LinkObject(arStream);
}

struct FUN_00c4f9a0_shell
{
    void* GetSelf(void* a);
};

// @fnv 0x00c4f9a0
void* FUN_00c4f9a0_shell::GetSelf(void* apUnused)
{
    return this;
}

struct FUN_00c4fab0_shell
{
    void __thiscall AppendCameraPathEntries(int);
    void __thiscall BSMultiBoundShape_GetViewerStrings(int apStrings);
};

// @fnv 0x00c4fab0
// src: XBOX_PDB 0x00c4fab0
// pdb: BSMultiBoundShape::GetViewerStrings | BSMultiBoundShape.obj | debug, upstream name already matches, high
// pdb params: apStrings
void __thiscall FUN_00c4fab0_shell::BSMultiBoundShape_GetViewerStrings(int apStrings)
{
    AppendCameraPathEntries(apStrings);
}

struct FUN_00c4fa10_shell
{
    void __thiscall BSMultiBoundShape_LinkObject(int);
    void __thiscall BSMultiBoundSphere_LinkObject(int arStream);
};
// @fnv 0x00c4fc50
// src: XBOX_PDB 0x00c4fc50
// pdb: BSMultiBoundSphere::LinkObject | BSMultiBoundSphere.obj | debug, upstream name already matches, high
// pdb params: arStream
void __thiscall FUN_00c4fa10_shell::BSMultiBoundSphere_LinkObject(int arStream)
{
    BSMultiBoundShape_LinkObject(arStream);
}

struct FUN_00c50370_shell
{
    void BSMultiBoundSphere_CreateDebugGeometry(int a, int b, int c, int d, int e, int f);
};

// @fnv 0x00c50370
// src: COMMUNITY 0x00c50370
void FUN_00c50370_shell::BSMultiBoundSphere_CreateDebugGeometry(int a, int b, int c, int d, int e, int f)
{}

extern float _DAT_010162c0;

void* __cdecl BGSPrimitivePlane_CreateObject(NiPoint2*, NiColorA*);
void* __cdecl BGSPrimitive_CreateBox(NiPoint3*, NiColorA*);
void* __cdecl BGSPrimitive_CreateSphere(float, NiColorA*);

// @fnv 0x004a4e90
// src: XBOX_RETAIL_PDB 0x004a4e90 (was COMMUNITY)
// pdb: BGSPrimitive::CreateAbstract | BGSPrimitive.obj | retail, object-file run extension, high
// pdb params: eType, aRadii, aColor
// pdb locals: extents
void* __cdecl BGSPrimitive_CreateAbstract(int eType, NiPoint3* aRadii, NiColorA* aColor)
{
    switch (eType)
    {
    case 3: {
        NiPoint2 extents = NiPoint2(aRadii->x, aRadii->z) * _DAT_010162c0;
        return BGSPrimitivePlane_CreateObject(&extents, aColor);
    }
    case 1: {
        NiPoint3 extents = *static_cast<const NiPoint3*>(aRadii) * _DAT_010162c0;
        return BGSPrimitive_CreateBox(&extents, aColor);
    }
    case 2:
        return BGSPrimitive_CreateSphere(aRadii->x, aColor);
    default:
        return 0;
    }
}

// The shared AABB has no two-point constructor overload. Keep its entry
// callable here while using the shared base and members for all storage.
// @fnv 0x004a6fc0
PrimitiveAABB* PrimitiveAABB::Initialize(const NiPoint3& aPoint1, const NiPoint3& aPoint2)
{
    this->BSMultiBoundShape::BSMultiBoundShape();
    *(void***)this = gMultiBoundAABBVtable;
    reinterpret_cast<NiPoint3&>(Center) = reinterpret_cast<const NiPoint3&>(aPoint1);
    reinterpret_cast<NiPoint3&>(HalfExtents) = reinterpret_cast<const NiPoint3&>(aPoint2);
    return this;
}

extern int DAT_0101e9a4;
struct Data9
{
    int v[9];
};
struct MultiBoundOBBInitialization : public BSMultiBoundOBB
{
    void InitializeBase(int a, int b);
    MultiBoundOBBInitialization* Initialize(int a, int b, const Data9& c);
};
// Calls FUN_004a6fc0 with a and b, sets the vtable pointer to DAT_0101e9a4, copies c into d, and returns this.
// @fnv 0x004a70b0
// pdb: BSMultiBoundShape::~BSMultiBoundShape | BGSPrimitive.obj | debug, object-file run extension, low
MultiBoundOBBInitialization* MultiBoundOBBInitialization::Initialize(int aiArg1, int aiArg2, const Data9& arData)
{
    InitializeBase(aiArg1, aiArg2);
    *(int*)this = (int)&DAT_0101e9a4;
    reinterpret_cast<Data9&>(unk24) = arData;
    return this;
}

struct MultiBoundOBBCopy : public BSMultiBoundOBB
{
    void CopyBase(MultiBoundOBBCopy*, int);
    void CopyTo(MultiBoundOBBCopy*, int);
};
// Calls FUN_00c37f70 on the passed Holder and then copies this object's fld into it.
// @fnv 0x00c363c0
// pdb: BSMultiBoundAABB::CopyMembers | BSMultiBoundAABB.obj | retail, call graph, low
// pdb note: debug build links it to BSMultiBoundOBB::CopyMembers
// pdb params: pDest, arCloneData
void MultiBoundOBBCopy::CopyTo(MultiBoundOBBCopy* apTarget, int b)
{
    CopyBase(apTarget, b);
    reinterpret_cast<Data9&>(apTarget->unk24) = reinterpret_cast<const Data9&>(unk24);
}

struct MultiBoundOBBEquality : public BSMultiBoundOBB
{
    bool EqualsBase(MultiBoundOBBEquality*);
    bool Equals(MultiBoundOBBEquality*);
};
// Calls FUN_00c38090 on other; if it returns true, also compares m with other's m via _Call_4d9ae0 and returns that result.
// @fnv 0x00c364b0
// pdb: BSMultiBoundOBB::IsEqual | BSMultiBoundOBB.obj | retail, vtable slot, high
// pdb params: apObject
// pdb locals: bres, pobj
bool MultiBoundOBBEquality::Equals(MultiBoundOBBEquality* apObject)
{
    bool ret = EqualsBase(apObject);
    if (ret)
    {
        MultiBoundOBBEquality* p = apObject;
        ret = reinterpret_cast<const NiMatrix3&>(unk24) == reinterpret_cast<const NiMatrix3&>(p->unk24);
    }
    return ret;
}

// Copy support missing from the shared declaration; all state is inherited.
struct MultiBoundAABBCopy : public BSMultiBoundAABB
{
    void CopyMembersStreamWithField(BSMultiBoundAABB* target, int process);
    void CopyTo(BSMultiBoundAABB* target, int process);
};

// @fnv 0x00c37f70
// pdb: BSMultiBoundAABB::CopyMembers | BSMultiBoundAABB.obj | debug, call graph, medium
// pdb params: pDest, arCloneData
void MultiBoundAABBCopy::CopyTo(BSMultiBoundAABB* apTarget, int b)
{
    CopyMembersStreamWithField(apTarget, b);
    apTarget->SetGridPosition(Center);
    reinterpret_cast<NiPoint3&>(apTarget->HalfExtents) = reinterpret_cast<const NiPoint3&>(HalfExtents);
}

// Copies the portal fields into the target portal.
// @fnv 0x00c4f420
void BSPortal::CopyFields(BSPortal* apTarget, int aiUnused)
{
    apTarget->m_fc = this->m_fc;
    apTarget->m_100 = this->m_100;
}

// Calls CopyMembersStreamWithField with a2, invokes Real on _m0C, then reads and rewrites _m18 with its own value.
// @fnv 0x00c4fbc0
// src: XBOX_PDB 0x00c4fbc0
// pdb: BSMultiBoundSphere::CreateClone | BSMultiBoundSphere.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSMultiBoundSphere::CopyMembers
// pdb params: arCloneData
// pdb locals: pobj
void BSMultiBoundSphere::CopyMembers_00c4fbc0(BSMultiBoundSphere* a1, CloningProcess& a2)
{
    reinterpret_cast<fnv_00c4f9b0::FUN_00c4f9b0_class*>(this)->CopyMembersStreamWithField(a1, &a2);
    a1->SetGridPosition(reinterpret_cast<const NiPoint3&>(Center));
    float f = Radius;
    a1->Radius = f;
}

struct MultiBoundSphereVector : public BSMultiBoundSphere
{
    void CopyTo(Vec4* p);
};
// Copies the stored vector and scalar into the given output structure.
// @fnv 0x00c50330
// pdb: BSMultiBoundSphere::GetNiBound | BSMultiBoundSphere.obj | debug, vtable slot, medium
// pdb params: aBound
void MultiBoundSphereVector::CopyTo(Vec4* pOut)
{
    float f = Radius;
    *reinterpret_cast<NiPoint3*>(pOut) = reinterpret_cast<const NiPoint3&>(Center);
    pOut->w = f;
}

// Allocates and constructs a sphere bound.
// @fnv 0x00c4fdb0
// src: EXE 0x00c4fdb0
// pdb: BSMultiBoundSphere::CreateObject | BSMultiBoundSphere.obj | retail, upstream name already matches, high
BSMultiBoundSphere* BSMultiBoundSphere_CreateObject()
{
    return new BSMultiBoundSphere;
}

// Allocates a sphere, applies color and radius, and sets its primitive type.
// @fnv 0x004a5100
// src: XBOX_PDB 0x004a5100
// pdb: BGSPrimitive::CreateSphere | BGSPrimitive.obj | retail, upstream name already matches, high
// pdb params: afRadius, aColor
// pdb locals: pprimitive
void* __cdecl BGSPrimitive_CreateSphere(float afRadius, NiColorA* aColor)
{
    BGSPrimitiveSphere* p = new BGSPrimitiveSphere;
    p->ApplyColor(reinterpret_cast<NiColorA&>(*aColor));
    p->SetRadius(afRadius);
    p->eType = 2;
    return p;
}

// @fnv 0x004a5300
// pdb: BGSPrimitivePlane::Build | BGSPrimitive.obj | retail, string-set bridge, high
// pdb locals: alpha
void BGSPrimitivePlane::Build()
{
    m_pController = new BSFadeNode;
    static_cast<NiAVObject*>(m_pController)->SetName(NiFixedString("Plane"));
    static_cast<NiAVObject*>(m_pController)->UpdateSceneObject();
}

namespace fnv_004a6150 {

struct FUN_004a6150_shell
{
    BSPortal* CreateBSPortal();
};

extern char DAT_011a9448;
extern int _data_011c16bc;

// Allocates a BSPortal using the zero point, a NiPoint2 built from the floats at this+0x18 and this+0x20, and DAT_011a9448, then returns it.
// @fnv 0x004a6150
// pdb: BGSPrimitivePlane::MakePortal | BGSPrimitive.obj | retail, vtable slot, high
BSPortal* FUN_004a6150_shell::CreateBSPortal()
{
    return new BSPortal(NiPoint3::ZERO,
                        &NiPoint2(*reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x18),
                                  *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0x20)),
                        &DAT_011a9448);
}
}  // namespace fnv_004a6150

namespace fnv_00c38220 {
struct FUN_00c4fab0_shell
{
    void BSMultiBoundShape_GetViewerStrings(int);
};

void* _FUN_00a7d330(char const*);

extern char const* const g_01202818_arg1;

struct FUN_00c38220_shell : public FUN_00c4fab0_shell
{
    void AddPropertyNames(NiTPrimitiveArray<unsigned int>*);
};

// Calls the base method, then appends the property id and the "Center" and "Half Extents" names to the array.
// @fnv 0x00c38220
// pdb: BSMultiBoundAABB::GetViewerStrings | BSMultiBoundAABB.obj | retail, string-set bridge, high
// pdb params: apStrings
void FUN_00c38220_shell::AddPropertyNames(NiTPrimitiveArray<unsigned int>* apStrings)
{
    char unused[0x20];
    char* denter;

    FUN_00c4fab0_shell::BSMultiBoundShape_GetViewerStrings(reinterpret_cast<int>(apStrings));

    char const* volatile property = g_01202818_arg1;
    void* propertyId = _FUN_00a7d330(property);
    apStrings->AddFirstEmpty(reinterpret_cast<unsigned int const&>(propertyId));

    denter =
        reinterpret_cast<NiPoint3*>(reinterpret_cast<char*>(this) + 0x0c)->GetViewerString_00a59af0("Center");
    apStrings->AddFirstEmpty(reinterpret_cast<unsigned int const&>(denter));

    char* halfExtents = reinterpret_cast<NiPoint3*>(reinterpret_cast<char*>(this) + 0x18)
                            ->GetViewerString_00a59af0("Half Extents");
    apStrings->AddFirstEmpty(reinterpret_cast<unsigned int const&>(halfExtents));
}
}  // namespace fnv_00c38220

namespace fnv_00c4f620 {
struct A51BF0Base
{
    static void* __cdecl operator new(size_t);
    static void operator delete(void*, size_t);
    virtual ~A51BF0Base();
};

struct FUN_00c4f460 : public A51BF0Base
{
    unsigned char pad[0xc0];

    FUN_00c4f460(int);
};

struct FUN_00c4f6c0_shell
{
    void BSPortalSharedNode_CopyMembers(int, int);
    FUN_00c4f460* BSPortalSharedNode_CreateClone(int);
};

// Allocates a FUN_00c4f460 using the int at this+0xc0, passes it and arg to FUN_00c4f6c0, and returns the new object.
// @fnv 0x00c4f620
// src: XBOX_PDB 0x00c4f620
// pdb: BSPortalSharedNode::CreateClone | BSPortal.obj | retail, upstream name already matches, high
// pdb params: arCloneData
// pdb locals: pobj
FUN_00c4f460* FUN_00c4f6c0_shell::BSPortalSharedNode_CreateClone(int arg)
{
    FUN_00c4f460* object = new FUN_00c4f460(*(int*)((char*)this + 0xc0));

    this->BSPortalSharedNode_CopyMembers((int)object, arg);

    return object;
}
}  // namespace fnv_00c4f620

// Initializes the primitive type and clears its geometry and controller pointers.
// @fnv 0x004a5280
// pdb: BGSPrimitivePlane::BGSPrimitivePlane | BGSPrimitive.obj | retail, upstream name already matches, high
BGSPrimitivePlane::BGSPrimitivePlane()
{
    m_kGeometry = 0;
    m_pController = 0;
    eType = 3;
}

// Takes the absolute X/Z of a point, zeroes Y, writes the 8 or 16 box vertices into the geometry data and updates the model bound and node.
// @fnv 0x004a55a0
// pdb: BGSPrimitivePlane::SetRadii | BGSPrimitive.obj | retail, vtable slot, high
// pdb params: aRadii
// pdb locals: UpdateData, pverts, pgeomdata, fRad, kBound
void BGSPrimitivePlane::NormalizeRadii(NiPoint3& aExtents)
{
    aExtents[0] = std::abs(aExtents[0]);
    aExtents[1] = 0.0f;
    aExtents[2] = std::abs(aExtents[2]);
    reinterpret_cast<NiPoint3&>(m_kRadii) = reinterpret_cast<const NiPoint3&>(aExtents);
    if (m_kGeometry)
    {
        NiGeometryData* dataB = m_kGeometry->GetModelData();
        NiPoint3* verts = dataB->GetVertices();
        reinterpret_cast<NiPoint3&>(verts[0]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[1]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[2]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], -m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[3]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], -m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[4]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[5]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[6]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], -m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(verts[7]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], -m_kRadii[1], -m_kRadii[2]));
        dataB->SetDirtyFlags(1);
    }
    if (spFrame)
    {
        NiGeometryData* data2 = spFrame->GetModelData();
        NiPoint3* aVerts2 = data2->GetVertices();
        reinterpret_cast<NiPoint3&>(aVerts2[0]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[1]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[2]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], -m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[3]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], -m_kRadii[1], m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[4]) = reinterpret_cast<const NiPoint3&>(aVerts2[0]);
        reinterpret_cast<NiPoint3&>(aVerts2[5]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[6]) = reinterpret_cast<const NiPoint3&>(NiPoint3(-m_kRadii[0], -m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[7]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], -m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[8]) = reinterpret_cast<const NiPoint3&>(NiPoint3(m_kRadii[0], m_kRadii[1], -m_kRadii[2]));
        reinterpret_cast<NiPoint3&>(aVerts2[9]) = reinterpret_cast<const NiPoint3&>(aVerts2[5]);
        reinterpret_cast<NiPoint3&>(aVerts2[10]) = reinterpret_cast<const NiPoint3&>(aVerts2[3]);
        reinterpret_cast<NiPoint3&>(aVerts2[11]) = reinterpret_cast<const NiPoint3&>(aVerts2[6]);
        reinterpret_cast<NiPoint3&>(aVerts2[12]) = reinterpret_cast<const NiPoint3&>(aVerts2[1]);
        reinterpret_cast<NiPoint3&>(aVerts2[13]) = reinterpret_cast<const NiPoint3&>(aVerts2[8]);
        reinterpret_cast<NiPoint3&>(aVerts2[14]) = reinterpret_cast<const NiPoint3&>(aVerts2[2]);
        reinterpret_cast<NiPoint3&>(aVerts2[15]) = reinterpret_cast<const NiPoint3&>(aVerts2[7]);
        data2->SetDirtyFlags(1);
    }
    NiUpdateData updateData3(0.0f, false, false);
    if (m_pController)
    {
        float fMax3 = aExtents.x;
        if (aExtents.y > aExtents.x && aExtents.y > aExtents.z)
            fMax3 = aExtents.y;
        if (aExtents.z > aExtents.y && aExtents.z > aExtents.x)
            fMax3 = aExtents.z;
        NiBound bound2;
        bound2.SetCenterAndRadius(NiPoint3::ZERO, fMax3);
        m_kGeometry->SetModelBound(bound2);
        static_cast<NiAVObject*>(m_pController)->Update(updateData3);
        m_pController->UpdateWorldData(updateData3);
        m_kGeometry->UpdateSceneObject();
        m_pController->UpdateWorldBound();
    }
}

struct BGSPrimBase
{
    BGSPrimBase();
    virtual ~BGSPrimBase();
    unsigned int unk04;
};

struct BGSPrimPair
{
    unsigned int m_00;
    unsigned int m_04;
};

struct BGSPrimMatrix
{
    unsigned int m_00[9];
};

struct BGSPrimSub
{
    BGSPrimSub();
    unsigned int unk00[25];
};

struct BGSPrimElem
{
    BGSPrimElem();
    unsigned int m_00;
    unsigned int m_04;
    unsigned int m_08;
};

class FUN_004a5ff0_cls : public BGSPrimBase
{
  public:
    NiPoint3 m_kCenter;
    BGSPrimPair m_pair;
    BGSPrimMatrix m_mat;
    unsigned int m_40;
    BGSPrimSub m_sub;
    BGSPrimElem m_aiIndexedValues[4];
    BGSPrimElem m_kPrimitiveElement;
    unsigned char m_e4;
    unsigned char m_e5;
    unsigned char unkE6[4];
    unsigned char m_ea;

    FUN_004a5ff0_cls(NiPoint3& a1, BGSPrimPair& a2, BGSPrimMatrix& a3);
    virtual ~FUN_004a5ff0_cls();
};

// Constructs the primitive from a center point, pair and matrix, and initializes m_e4, m_e5, m_ea to 1 and m_40 to 0.
// @fnv 0x004a5ff0
// pdb: BSOcclusionPlane::BSOcclusionPlane | BGSPrimitive.obj | debug, object-file run extension, low
// pdb params: arCenter, arHalfExtents, arRotation
FUN_004a5ff0_cls::FUN_004a5ff0_cls(NiPoint3& akCenter, BGSPrimPair& akPair, BGSPrimMatrix& akMatrix)
    : m_kCenter(reinterpret_cast<const NiPoint3&>(akCenter)), m_pair(akPair), m_mat(akMatrix)
{
    m_e4 = 1;
    m_e5 = 1;
    m_40 = 0;
    m_ea = 1;
}

// Initializes the base from the centre, extents and rotation, sets m_e4/m_e5 to 1, zeroes m_40, m_100, m_fc, and constructs m_104 with 0 then calls Ping(0).
// @fnv 0x004a61f0
// pdb: BSPortal::BSPortal | BGSPrimitive.obj | debug, object-file run extension, low
// pdb params: arCenter, arHalfExtents, arRotation
BSPortal::BSPortal(const NiPoint3& arCenter, NiPoint2* apExtents, char* apRotation)
    : BSOcclusionPlane(arCenter, apExtents, apRotation), m_104(0)
{
    m_e4 = 1;
    m_e5 = 1;
    m_40 = 0;
    m_100 = 0;
    m_fc = 0;
    m_104.Ping(0);
}

struct BoundMatrix3
{
    float m00;
    float m01;
    float m02;
    float m10;
    float m11;
    float m12;
    float m20;
    float m21;
    float m22;
    BoundMatrix3();
    void SetRotationFromEulerXYZ(float fXAngle, float fYAngle, float fZAngle);
    static const BoundMatrix3 IDENTITY;
};

struct BoundBase : public NiMemObject
{
};

struct BoundOBB : public BSMultiBoundOBB
{
    BoundOBB(const NiPoint3& akStart, const NiPoint3& kExtents, const BoundMatrix3& kRot);
    void SetRotation(const BoundMatrix3& kRot);
};

struct BoundOwner
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    NiPoint3 kExtents;
    BoundBase* CreateBound(NiPoint3* pAngles);
};

// Creates an OBB rotated by pAngles when any angle exceeds 0.01, otherwise creates an axis-aligned AABB, both from kExtents.
// @fnv 0x004a6e30
BoundBase* BoundOwner::CreateBound(NiPoint3* pAngles)
{
    bool bRotated = false;
    if (std::abs(pAngles->x) > (double)0.01f)
        bRotated = true;
    if (std::abs(pAngles->y) > (double)0.01f)
        bRotated = true;
    if (std::abs(pAngles->z) > (double)0.01f)
        bRotated = true;
    if (bRotated)
    {
        BoundOBB* pOBB = new BoundOBB(NiPoint3::ZERO, kExtents, BoundMatrix3::IDENTITY);
        {
            BoundMatrix3 kRot;
            kRot.SetRotationFromEulerXYZ(pAngles->x, pAngles->y, pAngles->z);
            pOBB->SetRotation(kRot);
        }
        return reinterpret_cast<BoundBase*>(pOBB);
    }
    else
    {
        return reinterpret_cast<BoundBase*>(new PrimitiveAABB(NiPoint3::ZERO, kExtents));
    }
}

struct FUN_004a7aa0_color
{
    float x, y, z;
    FUN_004a7aa0_color(float, float, float, float);
};

// The shared sphere lacks the bound-value constructor used by this allocation.
struct PrimitiveSphereBound : public BSMultiBoundSphere
{
    PrimitiveSphereBound(const FUN_004a7aa0_color& c);
};

struct FUN_004a7aa0_cls
{
    char _00[0x18];
    float m_f18;
    void* BGSPrimitiveSphere_MakeMultiBoundShape(int akAngle);
};

// Allocates a new object constructed from a color built with the class's m_f18 value and returns it.
// @fnv 0x004a7aa0
// src: XBOX_PDB 0x004a7aa0
// pdb: BGSPrimitiveSphere::MakeMultiBoundShape | BGSPrimitive.obj | retail, upstream name already matches, high
// pdb params: akAngle
void* FUN_004a7aa0_cls::BGSPrimitiveSphere_MakeMultiBoundShape(int akAngle)
{
    return new PrimitiveSphereBound(FUN_004a7aa0_color(0.0f, 0.0f, 0.0f, m_f18));
}

// Allocates and constructs a new OBB.
// @fnv 0x00c36500
// pdb: BSMultiBoundOBB::BSMultiBoundOBB | BSMultiBoundOBB.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSMultiBoundOBB::CreateObject
// pdb params: arCenter, arHalfExtents, arAxis
BSMultiBoundOBB* __cdecl CreateMultiBoundOBB()
{
    return new BSMultiBoundOBB;
}

extern "C" int _NiPoint3_ZERO;

class MultiBoundOBBPlanes : public BSMultiBoundOBB
{
  public:
    void BSMultiBoundOBB_GetPlanes(NiPlane* apPlanes);
};

// Builds three planes from the columns of m_kRotate, setting each plane normal to a matrix column and its constant to the dot product with m_kCenter.
// @fnv 0x00c37e00
// src: XBOX_DEBUG_PDB 0x00c37e00
// pdb: BSMultiBoundOBB::GetPlanes | BSMultiBoundOBB.obj | debug, call graph (shared callers), high
// pdb params: apPlanes
// pdb locals: i, kaxis
void MultiBoundOBBPlanes::BSMultiBoundOBB_GetPlanes(NiPlane* apPlanes)
{
    for (int i = 0; i < 3; i++)
    {
        NiPoint3 kColumn = reinterpret_cast<const NiPoint3&>(_NiPoint3_ZERO);
        const NiMatrix3& kMatrix = reinterpret_cast<const NiMatrix3&>(unk24);
        kColumn.x = reinterpret_cast<const float(*)[3]>(&kMatrix)[0][i];
        kColumn.y = reinterpret_cast<const float(*)[3]>(&kMatrix)[1][i];
        kColumn.z = reinterpret_cast<const float(*)[3]>(&kMatrix)[2][i];
        reinterpret_cast<NiPoint3&>(apPlanes[i]) = kColumn;
        float fConstant = kColumn.Dot(reinterpret_cast<const NiPoint3&>(Center));
        apPlanes[i].m_fConstant = fConstant;
    }
}

extern "C" void* __cdecl Object_DispatchCommandFour(unsigned int size);

// @fnv 0x00c381a0
// src: XBOX_RETAIL_PDB 0x00c381a0
// pdb: BSMultiBoundAABB::CreateObject | BSMultiBoundAABB.obj | retail, object-file run extension, high
BSMultiBoundAABB* __cdecl BSMultiBoundAABB_CreateObject()
{
    return new BSMultiBoundAABB();
}

struct Vec12
{
    float x;
    float y;
    float z;
};

struct ProbeElem16
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
};

struct ProbeSet
{
    ProbeElem16 elems[6];
    unsigned int mask;
};

struct FUN_0049da80_shell
{
    int __thiscall ClassifyPoint(Vec12* p);
};

struct FUN_00c38920_cls
{
    void __thiscall BSMultiBoundAABB_GetVertices(Vec12* apPoints);
    bool __thiscall BSMultiBoundAABB_CompletelyWithinFrustum(ProbeSet* aPlanes);
};

// Builds eight box points, then for each masked element tests all eight points and returns false if any test returns 2, otherwise true.
// @fnv 0x00c38920
// src: XBOX_PDB 0x00c38920
// pdb: BSMultiBoundAABB::CompletelyWithinFrustum | BSMultiBoundAABB.obj | retail, upstream name already matches, high
// pdb params: aPlanes
// pdb locals: kboxpoints, i, j
bool __thiscall FUN_00c38920_cls::BSMultiBoundAABB_CompletelyWithinFrustum(ProbeSet* aPlanes)
{
    Vec12 boxPts[8];
    int i;
    int j;
    {
        int n = 8;
        Vec12* p = boxPts;
        unsigned int scratch[90];
        int unkTail;
        while (--n >= 0)
        {
            p++;
        }
    }
    BSMultiBoundAABB_GetVertices(boxPts);
    for (i = 0; i < 6; i++)
    {
        if (bool(aPlanes->mask & (1u << i)))
        {
            for (j = 0; j < 8; j++)
            {
                if (((FUN_0049da80_shell*)&aPlanes->elems[i])->ClassifyPoint(&boxPts[j]) == 2)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

struct FUN_00c49850_cls
{
    void BSCompoundFrustum_PrethreadOpList();
};

struct FUN_00a59e00_cls
{
    void FUN_00a59e00(void* pObj);
};

struct FUN_00c4f6d0_sub
{
    int unk00;
    FUN_00a59e00_cls** unk04;
    unsigned short unk08;
    unsigned short unk0A;
};

struct FUN_00c4f6d0_arg
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    int unk80;
    int unk84;
    int unk88;
    int unk8C;
    int unk90;
    int unk94;
    int unk98;
    int unk9C;
    int unkA0;
    int unkA4;
    int unkA8;
    int unkAC;
    int unkB0;
    int unkB4;
    int unkB8;
    int unkBC;
    FUN_00c49850_cls* m_pThing;
};

struct FUN_00c4f6d0_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    int unk80;
    int unk84;
    int unk88;
    int unk8C;
    int unk90;
    int unk94;
    int unk98;
    FUN_00c4f6d0_sub m_sub;
    unsigned short m_uFlag;
    unsigned short unkAA;
    FUN_00c49850_cls* m_pEntryObject;

    void BSPortalSharedNode_OnVisible(FUN_00c4f6d0_arg* kCuller);
};

// @fnv 0x00c4f6d0
// src: XBOX_PDB 0x00c4f6d0
// pdb: BSPortalSharedNode::OnVisible | BSPortal.obj | retail, upstream name already matches, high
// pdb params: kCuller
// pdb locals: pprevcfrustum, isize, pkbsCuller, pchildren, i, pkChild
void FUN_00c4f6d0_cls::BSPortalSharedNode_OnVisible(FUN_00c4f6d0_arg* kCuller)
{
    FUN_00c4f6d0_sub* tgtC;
    FUN_00c4f6d0_arg* subB = 0;
    FUN_00c49850_cls* savA;
    FUN_00c49850_cls* pSwapE;
    unsigned int cntB;
    unsigned int idxD;
    FUN_00a59e00_cls** pSlotE;
    FUN_00a59e00_cls* pElemA;
    if (m_uFlag == 0)
    {
    }
    else
    {
        subB = kCuller;
        savA = subB->m_pThing;
        pSwapE = m_pEntryObject;
        subB->m_pThing = pSwapE;
        if (m_pEntryObject != 0)
        {
            m_pEntryObject->BSCompoundFrustum_PrethreadOpList();
        }
        tgtC = &this->m_sub;
        cntB = tgtC->unk0A;
        for (idxD = 0; idxD < cntB; idxD++)
        {
            pSlotE = &tgtC->unk04[idxD];
            pElemA = *pSlotE;
            if (pElemA != 0)
            {
                pElemA->FUN_00a59e00(kCuller);
            }
        }
        subB->m_pThing = savA;
    }
}

int __fastcall BSMultiBoundRoom_GetCullResult(int value);

struct FUN_00c4f7b0_scratch
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c, unk20, unk24;
};

// Returns whether either of the two fields m_fc or m_100 is nonzero and passes FUN_00c4f860 with result 1.
// @fnv 0x00c4f7b0
// pdb: BSPortalSharedNode::~BSPortalSharedNode | BSPortal.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSPortal::AnyRoomsVisible
int BSPortal::QueryLinkedObjects()
{
    FUN_00c4f7b0_scratch scratch;
    int result5 = 0;
    int v = m_fc;
    if (v != 0)
    {
        result5 = (BSMultiBoundRoom_GetCullResult(v) == 1);
    }
    v = m_100;
    if (v != 0)
    {
        result5 |= (BSMultiBoundRoom_GetCullResult(v) == 1);
    }
    return result5;
}

// @fnv 0x00c4f980
// pdb: BSMultiBoundShape::BSMultiBoundShape | BSMultiBoundShape.obj | retail, upstream name already matches, high
BSMultiBoundShape::BSMultiBoundShape()
{}

struct MultiBoundSphereEquality : public BSMultiBoundSphere
{
    bool BSMultiBoundShape_IsEqual(void* other);
    bool Equals(MultiBoundSphereEquality* apObject);
};

// Compares this object with another: first calls FUN_00c4fa80, then checks m_pos x/y/z and m_18 are equal, returning the combined result.
// @fnv 0x00c4fce0
// pdb: BSMultiBoundSphere::IsEqual | BSMultiBoundSphere.obj | retail, object-file run extension, high
// pdb params: apObject
// pdb locals: bres, pobj
bool MultiBoundSphereEquality::Equals(MultiBoundSphereEquality* apObject)
{
    bool r6 = BSMultiBoundShape_IsEqual(apObject);
    if (r6)
    {
        MultiBoundSphereEquality* o4 = apObject;
        const NiPoint3* pb = &o4->Center;
        const NiPoint3* pa = &Center;
        r6 = (pa->x == pb->x && pa->y == pb->y && pa->z == pb->z);
        float t = Radius;
        r6 = r6 & (t == o4->Radius);
    }
    return r6;
}

struct NodeArgB9733
{
    void FUN_0096a610(void** pp);
};

void* NiViewerStrings_NiGetViewerString_Title(void* g);
void* NiGetViewerString_00a7d550(const char* name, float value);

extern void* DAT_01203108;

struct MultiBoundSphereProperties : public BSMultiBoundSphere
{
    void Init(NodeArgB9733* apStrings);
    void InitNode(NodeArgB9733* apStrings);
};

// @fnv 0x00c4fe30
// pdb: BSMultiBoundSphere::GetViewerStrings | BSMultiBoundSphere.obj | retail, string-set bridge, high
// pdb params: apStrings
void MultiBoundSphereProperties::InitNode(NodeArgB9733* apStrings)
{
    Init(apStrings);
    unsigned int data[5];
    unsigned int unk[4];
    void* g = DAT_01203108;
    void* root = NiViewerStrings_NiGetViewerString_Title(g);
    apStrings->FUN_0096a610(&root);
    void* center7 = reinterpret_cast<NiPoint3*>(&Center)->GetViewerString_00a59af0("Center");
    apStrings->FUN_0096a610(&center7);
    float r = Radius;
    void* radius = NiGetViewerString_00a7d550("Radius", r);
    apStrings->FUN_0096a610(&radius);
}

struct Point2_construct_shell
{
    float x, y;
    Point2_construct_shell(float width, float height, const void* axis);
};
struct FUN_004a5ff0_shell : FUN_00c335d0
{
    FUN_004a5ff0_shell(const void* type, const Point2_construct_shell& extent);
};
struct PrimitivePlaneClone : BGSPrimitive
{
    FUN_004a5ff0_shell* CreatePrimitive();
};
extern int g_011a9448;
extern int g_011f426c;

// Allocates a new primitive of type g_011f426c using this primitive's x and z extents and returns it.
// @fnv 0x004a5f50
// pdb: BGSPrimitivePlane::MakeOcclusionPlane | BGSPrimitive.obj | retail, vtable slot, high
FUN_004a5ff0_shell* PrimitivePlaneClone::CreatePrimitive()
{
    return new FUN_004a5ff0_shell(&g_011f426c, Point2_construct_shell(m_kRadii.x, m_kRadii.z, &g_011a9448));
}

struct SphereVertex
{
    double x;
    double y;
    double z;
};
extern SphereVertex DAT_01187ac8[];

// @fnv 0x004a78e0
// pdb: BGSPrimitiveSphere::SetRadius | BGSPrimitive.obj | retail, upstream name already matches, high
// pdb params: fRadius
// pdb locals: UpdateData, pverts, pgeomdata, i, kBound
void BGSPrimitiveSphere::SetRadius(float fRadius)
{
    fRadius = std::abs(fRadius);
    if (m_kGeometry)
    {
        NiGeometryData* pkData5 = m_kGeometry->GetModelData();
        NiPoint3* pkVertices2 = pkData5->GetVertices();
        for (int i = 0; i < 0x70; i++)
        {
            pkVertices2[i].x = DAT_01187ac8[i].x;
            pkVertices2[i].y = DAT_01187ac8[i].y;
            pkVertices2[i].z = DAT_01187ac8[i].z;
            ((NiPoint3*)pkVertices2)[i] *= fRadius;
        }
        pkData5->SetDirtyFlags(1);
    }
    m_kRadii.x = m_kRadii.y = m_kRadii.z = fRadius;
    NiUpdateData kUpdateDataTmp(0.0f, false, false);
    if (m_pController)
    {
        NiBound kBound;
        kBound.SetCenterAndRadius(NiPoint3(0.0f, 0.0f, 0.0f), fRadius);
        m_kGeometry->SetModelBound(kBound);
        static_cast<NiAVObject*>(m_pController)->Update(kUpdateDataTmp);
        m_kGeometry->UpdateSceneObject();
        m_pController->UpdateWorldBound();
    }
}

struct SphereVertexPosition
{
    float x, y, z;
    SphereVertexPosition();
};

class PrimitiveSphereStrips : public NiGeometry
{
  public:
    static void operator delete(void*, size_t);
    PrimitiveSphereStrips(unsigned short usVertices, SphereVertexPosition* pkVertex, void* pkNormal, NiColorA* pkColor, void* pkTexture,
                  void* usNumTextureSets, void* eNBTMethod, unsigned short usTriangles, unsigned int usStrips, unsigned short* pusStripLengths,
                  unsigned short* pusStripLists);
};

extern const double gSphereCoordinates[112][3];
void __cdecl FUN_00b57e30(PrimitiveSphereStrips*, int, int);

// Builds the sphere's line geometry, shader and alpha properties and fade node on first call, otherwise updates radius and color, then updates the fade node.
// @fnv 0x004a72d0
// pdb: BGSPrimitiveSphere::Build | BGSPrimitive.obj | retail, unique string literal, high
// pdb locals: UpdateData, l, ptri, pshprop, pverts, pcolors, pstrip, alpha, i
void BGSPrimitiveSphere::CreateSphereGeometry()
{
    if (!m_kGeometry)
    {
        NiColorA* colors5 = new NiColorA[112];
        SphereVertexPosition* positions = new SphereVertexPosition[112];
        unsigned short* indices5 = new unsigned short[112];

        for (int i2 = 0; i2 < 112; ++i2)
        {
            positions[i2].x = (float)gSphereCoordinates[i2][0] * m_kRadii.x;
            positions[i2].y = (float)gSphereCoordinates[i2][1] * m_kRadii.x;
            positions[i2].z = (float)gSphereCoordinates[i2][2] * m_kRadii.x;
            colors5[i2] = reinterpret_cast<const NiColorA&>(Color);
            indices5[i2] = (unsigned short)i2;
        }

        unsigned short* lengths3 = new unsigned short[1];
        lengths3[0] = 112;

        PrimitiveSphereStrips* lines2 =
            new PrimitiveSphereStrips(112, positions, 0, colors5, 0, 0, 0, 110, 1, lengths3, indices5);
        BSShaderNoLightingProperty* prop2 = new BSShaderNoLightingProperty;
        prop2->SetShader(33);
        lines2->NiAVObject::AttachProperty(reinterpret_cast<NiObject*>(prop2));
        FUN_00b57e30(lines2, 0, 0);
        m_kGeometry = lines2;

        NiPointer<NiAlphaProperty> spAlphaProperty2 = new NiAlphaProperty;
        ((NiObjectNET*)(NiAlphaProperty*)spAlphaProperty2)->SetName(NiFixedString("primitive: alpha"));
        ((NiAlphaProperty*)spAlphaProperty2)->NiAlphaProperty_SetAlphaBlending(true);
        m_kGeometry->AttachProperty((NiAlphaProperty*)spAlphaProperty2);
        m_kGeometry->GetProperty(3u)->BSShaderProperty_SetFlag(32, false);

        m_pController = new BSFadeNode;
        m_pController->AttachChildInAvailableSlot(static_cast<NiAVObject*>((NiGeometry*)m_kGeometry), true);
        static_cast<NiAVObject*>(m_pController)->SetName(NiFixedString("Sphere"));
        m_pController->UpdateSceneObject();
        m_kGeometry->UpdateSceneObject();
    }
    else
    {
        SetRadius(m_kRadii[0]);
        ApplyColor(Color);
    }

    NiUpdateData updateData3(0.0f, false, false);
    static_cast<NiAVObject*>(m_pController)->Update(updateData3);
}

struct KVec
{
    unsigned int m_uiX;
    unsigned int m_uiY;
    unsigned int m_uiZ;
};

class KPlane
{
  public:
    NiPoint3 m_kPlaneNormal;
    float m_fPlaneConstant;
};

class KPlanes
{
  public:
    KPlane m_akPlanes[6];
    unsigned int m_uiActivePlaneMask;
};

struct KSphere
{
    KVec m_kCenter;
    float m_fBoundRadius;
    int Side(KPlane* kPlane);
};

class MultiBoundSphereFrustum : public BSMultiBoundSphere
{
  public:

    bool BSMultiBoundSphere_WithinFrustum(KPlanes* aPlanes);
};

// @fnv 0x00c50210
// src: XBOX_PDB 0x00c50210
// pdb: BSMultiBoundSphere::WithinFrustum | BSMultiBoundSphere.obj | debug, upstream name already matches, high
// pdb params: aPlanes
// pdb locals: kbound, i, iSide
bool MultiBoundSphereFrustum::BSMultiBoundSphere_WithinFrustum(KPlanes* aPlanes)
{
    int iPlaneIndex;
    float fRad = Radius;
    int result;
    KSphere kBound;
    unsigned int iLoop;
    kBound.m_kCenter = *(KVec*)&Center;
    kBound.m_fBoundRadius = fRad;
    for (iLoop = 0; iLoop < 6; ++iLoop)
    {
        if ((bool)((aPlanes->m_uiActivePlaneMask & (1u << iLoop)) != 0))
        {
            result = kBound.Side(&aPlanes->m_akPlanes[iLoop]);
            if (result == 2)
                return false;
        }
    }
    return true;
}

// Explicit expansion of the geometry accessors used during box mesh construction.
struct PointB
{
    float x, y, z;
    PointB* Difference(PointB* result, const PointB& origin) const;
};
struct ColorB
{
    float r, g, b, a;
};
extern ColorB DAT_011a9be0;
struct DataB
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c;
    PointB* vertices;
    int unk24;
    ColorB* colors;
    int unk2c, unk30, unk34, unk38, unk3c;
    bool* flags;
    unsigned short* GetIndices();
};
struct MeshB
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c;
    int unk20, unk24, unk28, unk2c, unk30, unk34, unk38, unk3c;
    int unk40, unk44, unk48, unk4c, unk50, unk54;
    PointB position;
    int unk64, unk68, unk6c, unk70, unk74, unk78, unk7c;
    int unk80, unk84, unk88, unk8c, unk90, unk94, unk98, unk9c;
    int unka0, unka4, unka8, unkac, unkb0, unkb4;
    DataB* data;
};
struct BoxB
{
    int unk00, unk04, unk08;
    PointB position;
    void FillCorners(PointB*);
    void BuildBoxGeometry(MeshB*, MeshB*, ColorB);
};
// Temporary working records used by the mesh builder. Their lifetime does not
// overlap external mesh ownership.
struct BoxBuildContext
{
    MeshB mesh;
    DataB data;
    PointB center, radii;
    ColorB edgeColor, faceColor;
    float selectionBrightness, selectionAlpha;
};
typedef char ContextSizeCheck[1];
struct CornerTraversalB
{
    int completed;
    int remaining;
};
struct BoundsPairB
{
    PointB center, radii;
};
struct TrianglePointsB
{
    PointB a, b, c;
};
// Fills the line and face meshes with the box's 8 corners: 12 edges and 12 triangles, setting vertices, colors, flags and indices.
// @fnv 0x00c37150
// pdb: BSMultiBoundOBB::MakeDebugGeom | BSMultiBoundOBB.obj | retail, vtable slot, high
// pdb params: apLines, apShape, akBaseColor
// pdb locals: kpoints, klinecolor, kpvert, kpconn, ktriconn, kconn, kcolor, kpcol, kpflag, i
void BoxB::BuildBoxGeometry(MeshB* apLines, MeshB* apShape, ColorB akBaseColor)
{
    // Locals are declared separately from their first assignment so that the
    // index tables and operation results have independent lifetimes.
    TrianglePointsB scratch3;
    DataB* faceIndexDataS;
    ColorB* pTemp;
    DataB* faceColorData4;
    PointB* faceVertexResultV;
    DataB* faceVertexDataJ;
    BoundsPairB scratch7;
    bool* lineFlagResult8;
    DataB* lineFlagData6;
    ColorB* lineColorResult2;
    DataB* _lineColorDataR;
    PointB* lineVertexResultR;
    DataB* lineVertexData7;
    float nScratch4;
    BoxBuildContext scratchC;
    int remainingE;
    PointB* cursorM2;
    PointB point, point5, pointB, pointC, pointR;
    unsigned short j4;
    int i8;
    PointB cornersInA[8];
    ColorB _white;
    PointB* _vertices;
    unsigned short* indices2;
    unsigned short triangles6[12][3];
    unsigned short edges9[12][2];
    ColorB color9;
    ColorB* colorsT;
    bool* flagsR;
    remainingE = 8;
    cursorM2 = cornersInA;
    while (--remainingE >= 0)
    {
        ++cursorM2;
    }
    FillCorners(cornersInA);
    edges9[0][0] = 0;
    edges9[0][1] = 1;
    edges9[1][0] = 0;
    edges9[1][1] = 2;
    edges9[2][0] = 1;
    edges9[2][1] = 3;
    edges9[3][0] = 3;
    edges9[3][1] = 2;
    edges9[4][0] = 4;
    edges9[4][1] = 5;
    edges9[5][0] = 4;
    edges9[5][1] = 6;
    edges9[6][0] = 5;
    edges9[6][1] = 7;
    edges9[7][0] = 7;
    edges9[7][1] = 6;
    edges9[8][0] = 0;
    edges9[8][1] = 4;
    edges9[9][0] = 1;
    edges9[9][1] = 5;
    edges9[10][0] = 2;
    edges9[10][1] = 6;
    edges9[11][0] = 3;
    edges9[11][1] = 7;
    triangles6[0][0] = 2;
    triangles6[0][1] = 3;
    triangles6[0][2] = 1;
    triangles6[1][0] = 2;
    triangles6[1][1] = 1;
    triangles6[1][2] = 0;
    triangles6[2][0] = 5;
    triangles6[2][1] = 7;
    triangles6[2][2] = 6;
    triangles6[3][0] = 5;
    triangles6[3][1] = 6;
    triangles6[3][2] = 4;
    triangles6[4][0] = 7;
    triangles6[4][1] = 2;
    triangles6[4][2] = 6;
    triangles6[5][0] = 7;
    triangles6[5][1] = 3;
    triangles6[5][2] = 2;
    triangles6[6][0] = 4;
    triangles6[6][1] = 0;
    triangles6[6][2] = 5;
    triangles6[7][0] = 5;
    triangles6[7][1] = 0;
    triangles6[7][2] = 1;
    triangles6[8][0] = 6;
    triangles6[8][1] = 0;
    triangles6[8][2] = 4;
    triangles6[9][0] = 6;
    triangles6[9][1] = 2;
    triangles6[9][2] = 0;
    triangles6[10][0] = 3;
    triangles6[10][1] = 7;
    triangles6[10][2] = 1;
    triangles6[11][0] = 7;
    triangles6[11][1] = 5;
    triangles6[11][2] = 1;
    color9 = akBaseColor;
    _white = DAT_011a9be0;
    lineVertexData7 = apLines->data;
    lineVertexResultR = lineVertexData7->vertices;
    _vertices = lineVertexResultR;
    _lineColorDataR = apLines->data;
    lineColorResult2 = _lineColorDataR->colors;
    colorsT = lineColorResult2;
    lineFlagData6 = apLines->data;
    lineFlagResult8 = lineFlagData6->flags;
    flagsR = lineFlagResult8;
    apLines->position = position;
    for (i8 = 0; i8 < 12; ++i8)
    {
        _vertices[i8 * 2] = *cornersInA[edges9[i8][0]].Difference(&pointR, position);
        colorsT[i8 * 2] = _white;
        _vertices[i8 * 2 + 1] = *cornersInA[edges9[i8][1]].Difference(&pointC, position);
        colorsT[i8 * 2 + 1] = _white;
        flagsR[i8 * 2] = true;
        flagsR[i8 * 2 + 1] = false;
    }
    faceVertexDataJ = apShape->data;
    faceVertexResultV = faceVertexDataJ->vertices;
    _vertices = faceVertexResultV;
    faceColorData4 = apShape->data;
    pTemp = faceColorData4->colors;
    colorsT = pTemp;
    faceIndexDataS = apShape->data;
    indices2 = faceIndexDataS->GetIndices();
    apShape->position = position;
    for (j4 = 0; j4 < 12; ++j4)
    {
        _vertices[j4 * 3] = *cornersInA[triangles6[j4][0]].Difference(&pointB, position);
        colorsT[j4 * 3] = color9;
        _vertices[j4 * 3 + 1] = *cornersInA[triangles6[j4][1]].Difference(&point5, position);
        colorsT[j4 * 3 + 1] = color9;
        _vertices[j4 * 3 + 2] = *cornersInA[triangles6[j4][2]].Difference(&point, position);
        colorsT[j4 * 3 + 2] = color9;
        indices2[j4 * 3] = j4 * 3;
        indices2[j4 * 3 + 1] = j4 * 3 + 1;
        indices2[j4 * 3 + 2] = j4 * 3 + 2;
    }
}

// Initializes the primitive's color, extents, geometry pointers and flags.
// @fnv 0x004a4ac0
// pdb: BGSPrimitive::BGSPrimitive | BGSPrimitive.obj | retail, upstream name already matches, high
BGSPrimitive::BGSPrimitive() : Color(0.0f, 0.0f, 0.0f, 0.0f), m_kGeometry(0), spFrame(0), m_pController(0)
{
    m_kGeometry = 0;
    m_pController = 0;
    spFrame = 0;
    iScaleHandlesEnabled = 7;
}

#include <math.h>
struct MultiBoundAABBSphereTest : public BSMultiBoundAABB
{
    int TestSphereBox(const NiBound& bound);
};
// Tests a sphere against the axis-aligned box Center/m_kOffset and returns 2 for inside, 1 for intersecting, or 0 for outside.
// @fnv 0x00c382b0
// pdb: BSMultiBoundAABB::CheckBound | BSMultiBoundAABB.obj | debug, vtable slot, medium
// pdb params: aTargetBound
// pdb locals: r, bintersect, fDx, bwithin, P, fDy, fDz
int MultiBoundAABBSphereTest::TestSphereBox(const NiBound& bound)
{
    NiPoint3 p = reinterpret_cast<const NiPoint3&>(bound.m_kCenter);
    float cf = bound.m_fRadius;
    bool s = true;
    bool eb = true;
    float f, q, b;
    float c, e, j;
    c = (p.x) - (Center.x);
    f = (double)(float)fabs((double)c);
    s &= f - (HalfExtents.x - cf) <= 0.0;
    eb &= f - (HalfExtents.x + cf) <= 0.0;
    if (s || eb)
    {
        e = (p.y) - (Center.y);
        q = (double)(float)fabs((double)e);
        if (s)
            s &= q - (HalfExtents.y - cf) <= 0.0;
        eb &= q - (HalfExtents.y + cf) <= 0.0;
        if (s || eb)
        {
            j = (p.z) - (Center.z);
            b = (double)(float)fabs((double)j);
            if (s)
                s &= b - (HalfExtents.z - cf) <= 0.0;
            eb &= b - (HalfExtents.z + cf) <= 0.0;
        }
    }
    return s ? 2 : (eb ? 1 : 0);
}

extern double DAT_01012060;

class MultiBoundSphereIntersection : public BSMultiBoundSphere
{
  public:
    int BSMultiBoundSphere_CheckBound(const NiBound& aTargetBound);
};

// Returns 2 when center distance is at most the radius difference plus DAT_01012060, 1 when at most the radius sum plus that value, otherwise 0.
// @fnv 0x00c4fed0
// src: XBOX_PDB 0x00c4fed0
// pdb: BSMultiBoundSphere::CheckBound | BSMultiBoundSphere.obj | retail, upstream name already matches, high
// pdb params: aTargetBound
// pdb locals: flen, kvect
int MultiBoundSphereIntersection::BSMultiBoundSphere_CheckBound(const NiBound& aTargetBound)
{
    NiPoint3 kDiff = reinterpret_cast<const NiPoint3&>(Center) - aTargetBound.m_kCenter;
    float fCenterDist = static_cast<const NiPoint3&>(kDiff).Length();
    float ba;
    float k;
    float l;
    float c;
    float j = aTargetBound.m_fRadius;
    if (fCenterDist - (Radius - j) <= DAT_01012060)
        return 2;
    else
    {
        float kTemp = aTargetBound.m_fRadius;
        if (fCenterDist - (Radius + kTemp) <= DAT_01012060)
            return 1;
    }
    return 0;
}

struct Plane
{
    float x, y, z, d;
};
struct Frustum
{
    Plane planes[6];
    unsigned int kComparisonMask;
};
// pdb: structure Sphere, size 0x10, 2 members
struct Sphere
{
    NiPoint3 Center; // src: XBOX_RETAIL_PDB +0x0 (old: center)
    float fRadius; // src: XBOX_RETAIL_PDB +0xc (old: radius)
    int ClassifySphereAgainstPlane(const Plane& kPlane);
};
struct MultiBoundSphereContainment : public BSMultiBoundSphere
{
    bool BSMultiBoundSphere_CompletelyWithinFrustum(const Frustum* aPlanes);
};
// Returns true when every plane selected by kComparisonMask classifies the sphere as 1; otherwise returns false.
// @fnv 0x00c502a0
// src: XBOX_PDB 0x00c502a0
// pdb: BSMultiBoundSphere::CompletelyWithinFrustum | BSMultiBoundSphere.obj | debug, upstream name already matches, high
// pdb params: aPlanes
// pdb locals: kbound, i, iSide
bool MultiBoundSphereContainment::BSMultiBoundSphere_CompletelyWithinFrustum(const Frustum* aPlanes)
{
    float radius = this->Radius;
    Sphere bound = {reinterpret_cast<const NiPoint3&>(Center), radius};
    int planeSide;
    unsigned int uiPlaneIndex;
    for (uiPlaneIndex = 0; uiPlaneIndex < 6; ++uiPlaneIndex)
    {
        if (bool(aPlanes->kComparisonMask & (1 << uiPlaneIndex)))
        {
            planeSide = bound.ClassifySphereAgainstPlane(aPlanes->planes[uiPlaneIndex]);
            if (planeSide != 1)
                return false;
        }
    }
    {
        int ReportDiagnostic;
    }
    return true;
}

// Initializes the sphere's geometry and controller pointers and primitive type.
// @fnv 0x004a7150
// pdb: BGSPrimitiveSphere::BGSPrimitiveSphere | BGSPrimitive.obj | retail, upstream name already matches, high
BGSPrimitiveSphere::BGSPrimitiveSphere()
{
    m_kGeometry = 0;
    m_pController = 0;
    eType = 2;
}

void* __fastcall Object_ProcessAndReturnValue_00c37ec0(void*);
extern int DAT_0101e9a4;
struct MultiBoundOBBConstructor
{
    int __thiscall SetVtableAndProcessState_00c36310();
};
// @fnv 0x00c36310
// src: GUESS 0x00c36310 (confidence: low)
// pdb: BSMultiBoundAABB::BSMultiBoundAABB | BSMultiBoundAABB.obj | retail, call graph, low
// pdb note: debug build links it to BSMultiBoundOBB::BSMultiBoundOBB
// pdb params: arCenter, arHalfExtents
int __thiscall MultiBoundOBBConstructor::SetVtableAndProcessState_00c36310()
{
    Object_ProcessAndReturnValue_00c37ec0(this);
    *(int**)this = &DAT_0101e9a4;
    return (int)this;
}

extern int DAT_0101ea8c;
// @fnv 0x00c4fb10
// src: EXE 0x00c4fb10
// pdb: BSMultiBoundSphere::BSMultiBoundSphere | BSMultiBoundSphere.obj | debug, call graph, medium
void* __fastcall BSMultiBoundSphere_BSMultiBoundSphere(void* apObj)
{
    static_cast<BSMultiBoundShape*>(apObj)->BSMultiBoundShape::BSMultiBoundShape();
    *(int*)apObj = (int)&DAT_0101ea8c;
    return apObj;
}
