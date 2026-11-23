// Distinct original implementations share reconstructed identifiers.
// Keep each matched body under its own address-qualified name.
#define SaveBinary SaveBinary_fnv_00c302a0

#include "../../../../include/NiPSysData.h"
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiPSysFieldModifier.h"
#include "../../../../include/NiPSysGravityFieldModifier.h"
#include "../../../../include/NiColor.h"
#include "../../../../include/NiObject.h"
#include "../../../../include/BSFadeNode.h"
#include "../../../../include/NiTransform.h"


inline unsigned int NiBinaryStream::BinaryWrite(const void* p, unsigned int bytes, unsigned int* sizes, unsigned int count)
{
    unsigned int written = (*(WriteFn*)((char*)this + 0xC))(this, p, bytes, sizes, count);
    *(unsigned int*)((char*)this + 4) += written;
    return written;
}
inline unsigned int NiBinaryStream::BinaryRead(void* p, unsigned int bytes, unsigned int* sizes, unsigned int count)
{
    unsigned int read = (*(ReadFn*)((char*)this + 8))(this, p, bytes, sizes, count);
    *(unsigned int*)((char*)this + 4) += read;
    return read;
}
inline NiBinaryStream* GetAirBinaryStream(NiStream& stream) { return *(NiBinaryStream**)((char*)&stream + 0x250); }
inline NiBinaryStream& NiStream::Istr() { return **(NiBinaryStream**)((char*)this + 0x24C); }

// src/gamebryo2.2/corelibs/niparticle/nipsysairfieldmodifier.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/niparticle/nipsysairfieldmodifier.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern "C" void* g_012027a8;

// @fnv 0x00c2f590
// src: COMMUNITY 0x00c2f590
void* __cdecl NiPSysAirFieldModifier_GetRTTI()
{
    return &g_012027a8;
}

void __cdecl FUN_00c31750(void);
void __cdecl __Dummy_00b6fef0(void* p, unsigned int flags);

// pdb: class NiPSysAirFieldModifier, size 0x68 (here 0x1), bases: NiPSysFieldModifier, 10 members
struct NiPSysAirFieldModifier
{
    void* __thiscall Destroy(unsigned int flags);
};

// Calls FUN_00c31750, then if flags has bit 1 set calls __Dummy_00b6fef0 on this with 0x68, and returns this.
// @fnv 0x00c2f5a0
void* __thiscall NiPSysAirFieldModifier::Destroy(unsigned int flags)
{
    void* p = this;
    FUN_00c31750();
    if (flags & 1)
        __Dummy_00b6fef0(p, 0x68);
    return p;
}

struct CloningProcess
{
};

struct AirTmp : NiPSysFieldModifier
{
    AirTmp* __thiscall FUN_00c2f5c0();
    void __thiscall NiPSysAirFieldModifier_SetDirection(void*);
};
// @fnv 0x00c2fad0
// pdb: NiPSysAirFieldModifier::CreateClone | NiPSysAirFieldModifier.obj | retail, object-file run extension, low
// pdb params: kCloning
// pdb locals: pkObject
NiObject* __thiscall NiPSysFieldModifier::Clone(CloningProcess& kCloning)
{
    AirTmp* pkObject;
    void* pMemory = BSFadeNode::operator new(0x68);
    if (pMemory)
        pkObject = ((AirTmp*)pMemory)->FUN_00c2f5c0();
    else
        pkObject = 0;
    CopyMemberData(pkObject, kCloning);
    pkObject->NiPSysAirFieldModifier_SetDirection((char*)this + 0x40);
    return (NiObject*)pkObject;
}

// @fnv 0x00c2f630
// pdb: NiPSysAirFieldModifier::CreateObject | NiPSysAirFieldModifier.obj | retail, object-file gap fill, low
// pdb locals: pkObject
void* __cdecl CreateAirTmp()
{
    AirTmp* p = (AirTmp*)BSFadeNode::operator new(0x68);
    if (p)
        return p->FUN_00c2f5c0();
    return 0;
}

extern float DAT_011f426c;
extern float DAT_011f4270;
extern float DAT_011f4274;
extern float DAT_011a946c;
extern float DAT_011a9470;
extern float DAT_011a9474;
struct TmpVec3
{
    int x;
    int y;
    int z;
    void __thiscall Unitize_004a0c10();
};
// Copies three ints from param into this+0x40 and this+0x4c, calls FUN_004a0c10, and replaces the vector with DAT_011a946c/70/74 if it equals DAT_011f426c/70/74.
// @fnv 0x00c2f500
// src: XBOX_PDB 0x00c2f500
// pdb: NiPSysAirFieldModifier::SetDirection | NiPSysAirFieldModifier.obj | debug, upstream name already matches, high
// pdb params: kDirection
void AirTmp::NiPSysAirFieldModifier_SetDirection(void* kDirection)
{
    int* p = (int*)kDirection;
    int* a = (int*)((char*)this + 0x40);
    a[0] = p[0];
    a[1] = p[1];
    a[2] = p[2];
    TmpVec3* v = (TmpVec3*)((char*)this + 0x4c);
    v->x = p[0];
    v->y = p[1];
    v->z = p[2];
    v->Unitize_004a0c10();
    if (*(float*)&v->x == DAT_011f426c && *(float*)&v->y == DAT_011f4270 && *(float*)&v->z == DAT_011f4274)
    {
        v->x = *(int*)&DAT_011a946c;
        v->y = *(int*)&DAT_011a9470;
        v->z = *(int*)&DAT_011a9474;
    }
}

struct SubObject40
{
    int unk00;
    bool FUN_00439090(void* other);
};

struct FUN_00c2fb10_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    SubObject40 m_40;

    bool FUN_00c32060(void* other);
    bool NiPSysAirFieldModifier_IsEqual(void* pkObject);
};

// @fnv 0x00c2fb10
// src: XBOX_PDB 0x00c2fb10
// pdb: NiPSysAirFieldModifier::IsEqual | NiPSysAirFieldModifier.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkDest
bool FUN_00c2fb10_cls::NiPSysAirFieldModifier_IsEqual(void* pkObject)
{
    FUN_00c2fb10_cls* p = (FUN_00c2fb10_cls*)pkObject;
    if (FUN_00c32060(pkObject) == false)
    {
        return false;
    }
    if (p->m_40.FUN_00439090(&m_40))
        return false;
    return true;
}





template <class T> inline void SerializeValue(NiBinaryStream* pkStr, const T& value)
{
    unsigned int uiSize = sizeof(T);
    pkStr->BinaryWrite(&value, sizeof(T), &uiSize, 1);
}






// Saves the base field modifier, a color, m_f58, m_f5C, the three bools m_b60-m_b62 and m_f64 to the stream.
// @fnv 0x00c302a0
// pdb: NiPSysAirFieldModifier::SaveBinary | NiPSysAirFieldModifier.obj | retail, object-file gap fill, high
// pdb params: kStream
// pdb locals: kBool
void NiPSysGravityFieldModifier::SaveBinary(NiStream& kStream)
{
    NiPSysFieldModifier::SaveBinary(kStream);
    // This entry serializes the air modifier layout, independently of the gravity header layout.
    reinterpret_cast<NiColor*>((char*)this + 0x40)->SaveBinary(kStream);
    SerializeValue(GetAirBinaryStream(kStream), *reinterpret_cast<float*>((char*)this + 0x58));
    SerializeValue(GetAirBinaryStream(kStream), *reinterpret_cast<float*>((char*)this + 0x5C));
    unsigned char ucBool = *reinterpret_cast<bool*>((char*)this + 0x60);
    SerializeValue(GetAirBinaryStream(kStream), ucBool);
    ucBool = *reinterpret_cast<bool*>((char*)this + 0x61);
    SerializeValue(GetAirBinaryStream(kStream), ucBool);
    ucBool = *reinterpret_cast<bool*>((char*)this + 0x62);
    SerializeValue(GetAirBinaryStream(kStream), ucBool);
    SerializeValue(GetAirBinaryStream(kStream), *reinterpret_cast<float*>((char*)this + 0x64));
}

class BC2F5C0Point3
{
  public:
    BC2F5C0Point3()
    {}
    BC2F5C0Point3(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ)
    {}

    BC2F5C0Point3 operator-() const
    {
        return BC2F5C0Point3(-x, -y, -z);
    }

    float x;
    float y;
    float z;
};

extern const BC2F5C0Point3 g_kC2F5C0UnitZ;
extern const BC2F5C0Point3 g_kC2F5C0Zero;

class BC2F5C0Modifier
{
  public:
    BC2F5C0Modifier() throw();
    virtual ~BC2F5C0Modifier();

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
};

class FUN_00c2f5c0_cls : public BC2F5C0Modifier
{
  public:
    FUN_00c2f5c0_cls() throw();
    virtual ~FUN_00c2f5c0_cls();

    void SetDirection(const BC2F5C0Point3& kDirection);

    BC2F5C0Point3 m_kPathOffset;
    float m_fFirstLightParameter;
};

// Initialises the modifier: sets direction to -unit Z, resets the point to zero, and sets the float to FLT_MAX.
// @fnv 0x00c2f5c0
// pdb: NiPSysAirFieldModifier::NiPSysAirFieldModifier | NiPSysAirFieldModifier.obj | retail, call graph, low
// pdb params: pcName, pkFieldObj, fMagnitude, fAttenuation, bUseMaxDistance, fMaxDistance, kDirection, fAirFriction, fInheritVelocity, bInheritRotation, bComponentOnly, bEnableSpread, fSpread
FUN_00c2f5c0_cls::FUN_00c2f5c0_cls() throw()
{
    SetDirection(-g_kC2F5C0UnitZ);
    m_kPathOffset = g_kC2F5C0Zero;
    m_fFirstLightParameter = 3.402823466e+38f;
}

extern "C" void* g_012027a8;

struct ViewerStrings
{
    int unk00;
    int unk04;
    unsigned short m_08;
    unsigned short m_0a;
    unsigned short m_0c;
    unsigned short m_0e;
};

struct ViewerSub
{
    int unk00;
    int unk04;
    int unk08;

    void* GetViewerString_00a59af0(const char* pPrefix);
};

void* __cdecl NiViewerStrings_NiGetViewerString_Title(void* g);
void* __cdecl NiGetViewerString_00a7d550(const char* name, float value);
void* __cdecl NiGetViewerString(const char* pcPrefix, bool bValue);

struct FUN_00c303c0_cls
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
    ViewerSub m_40;
    ViewerSub m_4c;
    float m_58;
    float m_5c;
    bool m_60;
    bool m_61;
    bool m_62;
    float m_64;

    void FUN_00c322b0(ViewerStrings* s);
    void FUN_0096ad30(unsigned int newSize);
    void FUN_0096ae90(unsigned int index, void** value);
    void BuildViewerStrings(ViewerStrings* pkStrings);
};

static void AddViewerString(ViewerStrings* s, void* value)
{
    FUN_00c303c0_cls* arr = reinterpret_cast<FUN_00c303c0_cls*>(s);
    unsigned int count = s->m_0a;
    if (count >= s->m_08)
    {
        arr->FUN_0096ad30(count + s->m_0e);
    }
    arr->FUN_0096ae90(count, &value);
}

// @fnv 0x00c303c0
// pdb: NiPSysAirFieldModifier::GetViewerStrings | NiPSysAirFieldModifier.obj | retail, string-set bridge, high
// pdb params: pkStrings
void FUN_00c303c0_cls::BuildViewerStrings(ViewerStrings* pkStrings)
{
    FUN_00c322b0(pkStrings);
    void* v = NiViewerStrings_NiGetViewerString_Title(g_012027a8);
    AddViewerString(pkStrings, v);
    v = m_40.GetViewerString_00a59af0("Direction");
    AddViewerString(pkStrings, v);
    v = m_4c.GetViewerString_00a59af0("Unit Direction");
    AddViewerString(pkStrings, v);
    v = NiGetViewerString_00a7d550("AirFriction", m_58);
    AddViewerString(pkStrings, v);
    v = NiGetViewerString_00a7d550("Inherit Velocity", m_5c);
    AddViewerString(pkStrings, v);
    v = NiGetViewerString("Inherit Rotation", m_60);
    AddViewerString(pkStrings, v);
    v = NiGetViewerString("Component Only", m_61);
    AddViewerString(pkStrings, v);
    v = NiGetViewerString("Enable Spread", m_62);
    AddViewerString(pkStrings, v);
    v = NiGetViewerString_00a7d550("Spread", m_64);
    AddViewerString(pkStrings, v);
}

struct AirFieldModifier
{
    void __thiscall FUN_00c2e520(NiStream& kStream);
    void __thiscall NiPSysAirFieldModifier_LinkObject(NiStream& kStream);
};

// @fnv 0x00c2f4e0
// src: XBOX_PDB 0x00c2f4e0
// pdb: NiPSysAirFieldModifier::LinkObject | NiPSysAirFieldModifier.obj | debug, upstream name already matches, high
// pdb params: kStream
void __thiscall AirFieldModifier::NiPSysAirFieldModifier_LinkObject(NiStream& kStream)
{
    FUN_00c2e520(kStream);
}

struct FUN_00c61f60_class
{
    unsigned char __thiscall NiPSysAgeDeathModifier_RegisterStreamables(int param);
    unsigned char __thiscall NiPSysAirFieldModifier_RegisterStreamables(int kStream);
};

// @fnv 0x00c2f4f0
// src: XBOX_PDB 0x00c2f4f0
// pdb: NiPSysAirFieldModifier::RegisterStreamables | NiPSysAirFieldModifier.obj | debug, upstream name already matches, high
// pdb params: kStream
unsigned char __thiscall FUN_00c61f60_class::NiPSysAirFieldModifier_RegisterStreamables(int kStream)
{
    return NiPSysAgeDeathModifier_RegisterStreamables(kStream);
}

inline NiPoint3::NiPoint3(float fX, float fY, float fZ) : x(fX), y(fY), z(fZ)
{}

inline NiPoint3 NiPoint3::operator*(float fScalar) const
{
    return NiPoint3(x * fScalar, y * fScalar, z * fScalar);
}

inline NiPoint3 operator*(float fScalar, const NiPoint3& pt)
{
    return NiPoint3(fScalar * pt.x, fScalar * pt.y, fScalar * pt.z);
}

inline NiPoint3 NiPoint3::operator-(const NiPoint3& pt) const
{
    return NiPoint3(x - pt.x, y - pt.y, z - pt.z);
}

inline NiPoint3 NiPoint3::operator+(const NiPoint3& pt) const
{
    return NiPoint3(x + pt.x, y + pt.y, z + pt.z);
}

// Preserve the original floating-point evaluation of the squared distance.
inline float AirSquaredLength(const float& x, const float& y, const float& z)
{
    const double xx = (double)x * x;
    const double yy = (double)y * y;
    const double zz = (double)z * z;
    return (float)(xx + yy + zz);
}


// Self-copy retains uninitialized matrix storage.
inline NiTransform::NiTransform() : m_Rotate(m_Rotate)
{}

struct TargetObj
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    int unk60;
    int unk64;
    NiTransform m_kWorldSpaceTransform;

    NiTransform GetWorldTransform() const
    {
        return m_kWorldSpaceTransform;
    }
};

struct Point3ParameterData
{
    NiPoint3 m_kValue;
    int unk0c;
    int unk10;
    float m_fZValue;
    float GetLastUpdate() const
    {
        return m_fZValue;
    }
    int unk18;
};

inline NiPoint3* NiPSysData::GetPositions()
{
    return m_pkVertex;
}

inline unsigned short NiPSysData::GetShaderParticleCount() const
{
    return m_usActive;
}

inline Point3ParameterData* NiPSysData::GetParticleInfo()
{
    return m_pkParticleInfo;
}

struct Cls_00c2f650
{
    void* vtbl;
    int unk04;
    int unk08;
    int unk0c;
    TargetObj* m_pkTarget;
    int unk14;
    TargetObj* m_pkFieldObject;
    float m_fInfluence;
    float m_fFieldAttenuation;
    bool m_bEnableMaxDistance;
    char unk25;
    char unk26;
    char unk27;
    float m_fDistanceLimit;
    float m_fMaxDistanceSquared;
    float GetMaxDistanceSquared() const
    {
        return m_fMaxDistanceSquared;
    }
    NiPoint3 m_kTransformedFieldPosition;
    float m_fLastTime;
    NiPoint3 m_kLightVector;
    NiPoint3 m_kParticleVelocityDirection;
    float m_fParticleAirFriction;
    float GetAirFriction() const
    {
        return m_fParticleAirFriction;
    }
    float m_fInheritedVelocity;
    bool m_bFlagAtOffset60;
    char unk61;
    char unk62;
    char unk63;
    float unk64;

    void UpdateParticleAirFriction(float fTime, NiPSysData* pkData);
};

inline void* operator new(size_t, void* pv)
{
    return pv;
}

// For particles within the squared distance limit, adds an air-friction-scaled field velocity over elapsed time when relative velocity points against it.
// @fnv 0x00c2f650
// pdb: NiPSysAirFieldModifier::UpdateNoAttenuationNoInheritNoSpread | NiPSysAirFieldModifier.obj | retail, object-file run extension, low
// pdb params: fTime, pkData
// pdb locals: kInvPSys, kPosition, kDirection, kPSys, kAirToPSys, kAirFieldVelocity, kAir, us, pkCurrentParticle, fDeltaT, kParticlePosition, kParticleOffset
void Cls_00c2f650::UpdateParticleAirFriction(float fTime, NiPSysData* pkData)
{
    if (m_fInfluence == 0.0f)
        return;

    NiTransform kInvPSys;
    // Assignment retains the original trivial transform copy.
    NiTransform kField;
    kField = m_pkFieldObject->m_kWorldSpaceTransform;
    NiTransform kFieldToPSys;
    kFieldToPSys = m_pkTarget->m_kWorldSpaceTransform;
    kFieldToPSys.Invert(kInvPSys);
    new (&kFieldToPSys) NiTransform(kInvPSys * kField);

    m_kTransformedFieldPosition = kFieldToPSys.m_Translate;
    m_fLastTime = fTime;

    NiPoint3 kWorldPosition;
    if (m_bFlagAtOffset60)
    {
        kWorldPosition = static_cast<const NiMatrix3&>(kFieldToPSys.m_Rotate) * m_kParticleVelocityDirection;
    }
    else
    {
        kWorldPosition = m_kParticleVelocityDirection;
    }

    NiPoint3 kAirVel;
    kAirVel = m_fInfluence * kWorldPosition;

    for (unsigned short i = 0; i < pkData->GetShaderParticleCount(); ++i)
    {
        Point3ParameterData* pkInfo = &pkData->GetParticleInfo()[i];
        float fMoveElapsedTime = fTime - pkInfo->GetLastUpdate();
        if (fMoveElapsedTime != 0.0f)
        {
            NiPoint3 kPos = pkData->GetPositions()[i];
            kWorldPosition = kPos - kFieldToPSys.m_Translate;
            float fDistSqr = AirSquaredLength(kWorldPosition.x, kWorldPosition.y, kWorldPosition.z);
            if (fDistSqr <= GetMaxDistanceSquared())
            {
                float fDot;
                NiPoint3 kRelVel;
                kRelVel = pkInfo->m_kValue - kAirVel;
                fDot = kRelVel.Dot(kAirVel);
                if (fDot < 0.0f)
                {
                    NiPoint3 kForce = kAirVel * GetAirFriction();
                    pkInfo->m_kValue += kForce * fMoveElapsedTime;
                }
            }
        }
    }
}


template <class T> inline unsigned S00c30170_Load(NiBinaryStream& r, T* p, unsigned n)
{
    unsigned uiSize = sizeof(T);
    return r.BinaryRead(p, n * sizeof(T), &uiSize, 1);
}
template <class T> inline void ReadBinaryValue(NiStream* s, T& v)
{
    S00c30170_Load(s->Istr(), &v, 1);
}
struct S00c30170_Pt
{
    float x, y, z;
    void __thiscall FUN_00a59be0(NiStream*);
};
struct S00c30170_Obj
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    S00c30170_Pt m_40;
    int unk4c;
    int unk50;
    int unk54;
    float m_58;
    float m_5c;
    bool m_60;
    bool m_61;
    bool m_62;
    float m_64;
    void __thiscall LoadFromStream(NiStream*);
    void __thiscall NiPSysAirFieldModifier_LoadBinary(NiStream* kStream);
};
// Calls LoadFromStream, reads m_40, applies FUN_00c2f500 to it, then reads m_58, m_5c, m_60, m_61, m_62, and m_64.
// @fnv 0x00c30170
// src: XBOX_PDB 0x00c30170
// pdb: NiPSysAirFieldModifier::LoadBinary | NiPSysAirFieldModifier.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: kBool
void __thiscall S00c30170_Obj::NiPSysAirFieldModifier_LoadBinary(NiStream* kStream)
{
    LoadFromStream(kStream);
    m_40.FUN_00a59be0(kStream);
    ((AirTmp*)this)->NiPSysAirFieldModifier_SetDirection(&m_40);
    ReadBinaryValue(kStream, m_58);
    ReadBinaryValue(kStream, m_5c);
    unsigned char b;
    ReadBinaryValue(kStream, b);
    m_60 = b != 0;
    ReadBinaryValue(kStream, b);
    m_61 = b != 0;
    ReadBinaryValue(kStream, b);
    m_62 = b != 0;
    ReadBinaryValue(kStream, m_64);
}

struct S00c2f8d0_Mod
{
    void* vtbl;
    int unk04;
    int unk08;
    int unk0c;
    TargetObj* m_pkTarget;
    int unk14;
    TargetObj* m_pkFieldObject;
    float m_fInfluence;
    float m_fFieldAttenuation;
    bool m_bEnableMaxDistance;
    char unk25;
    char unk26;
    char unk27;
    float m_fDistanceLimit;
    float m_fMaxDistanceSquared;
    NiPoint3 m_kTransformedFieldPosition;
    float m_fLastTime;
    NiPoint3 m_kLightVector;
    NiPoint3 m_kParticleVelocityDirection;
    float m_fParticleAirFriction;
    float GetAirFriction() const
    {
        return m_fParticleAirFriction;
    }
    float m_fInheritedVelocity;
    bool m_bFlagAtOffset60;
    char unk61;
    char unk62;
    char unk63;
    float unk64;

    void UpdateAirFriction(float fTime, NiPSysData* pkData);
};

// When influence is nonzero, transforms the field direction and adjusts particle velocities using air friction and the time delta.
// @fnv 0x00c2f8d0
// pdb: NiPSysAirFieldModifier::UpdateNoAttenuationNoInheritNoSpreadNoMaxDistance | NiPSysAirFieldModifier.obj | retail, object-file run extension, low
// pdb params: fTime, pkData
// pdb locals: kInvPSys, kPosition, kDirection, kPSys, kAirToPSys, kAirFieldVelocity, kAir, us, pkCurrentParticle, fDeltaT
void S00c2f8d0_Mod::UpdateAirFriction(float fTime, NiPSysData* pkData)
{
    if (m_fInfluence == 0.0f)
        return;

    NiTransform kFieldXform;
    kFieldXform = m_pkFieldObject->m_kWorldSpaceTransform;
    NiTransform kInvSys;
    {
        NiTransform kSysXform;
        kSysXform = m_pkTarget->m_kWorldSpaceTransform;
        kSysXform.Invert(kInvSys);
    }
    {
        NiTransform kFieldToSys = kInvSys * kFieldXform;

        m_kTransformedFieldPosition = kFieldToSys.m_Translate;
        m_fLastTime = fTime;

        NiPoint3 kWorldPosition;
        if (m_bFlagAtOffset60)
            kWorldPosition = static_cast<const NiMatrix3&>(kFieldToSys.m_Rotate) * m_kParticleVelocityDirection;
        else
            kWorldPosition = m_kParticleVelocityDirection;

        NiPoint3 kAirVel = kWorldPosition * m_fInfluence;

        for (unsigned short i = 0; i < pkData->m_usActive; i++)
        {
            Point3ParameterData* pkInfo = &pkData->m_pkParticleInfo[i];
            float fDeltaT = fTime - pkInfo->GetLastUpdate();
            if (fDeltaT != 0.0f)
            {
                NiPoint3 kDiff = pkInfo->m_kValue - kAirVel;
                if (kDiff.Dot(kAirVel) < 0.0f)
                {
                    pkInfo->m_kValue += GetAirFriction() * kAirVel * fDeltaT;
                }
            }
        }
    }
}

#include <math.h>
struct AirPoint3
{
    float x, y, z;
    AirPoint3()
    {}
    AirPoint3(float a, float b, float c) : x(a), y(b), z(c)
    {}
    bool FUN_00439090(const AirPoint3&) const;
    float FUN_00457910();
    void Unitize_004a0c10();
    AirPoint3 ProcessAndReturnValue_0053d280(float) const;
    AirPoint3 operator+(const AirPoint3& v) const
    {
        return AirPoint3(x + v.x, y + v.y, z + v.z);
    }
    AirPoint3 operator-(const AirPoint3& v) const
    {
        return AirPoint3(x - v.x, y - v.y, z - v.z);
    }
    AirPoint3 operator*(float s) const
    {
        return AirPoint3(x * s, y * s, z * s);
    }
    AirPoint3& operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    AirPoint3& operator+=(const AirPoint3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    float Dot(const AirPoint3& v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }
    float SquaredLength() const
    {
        return x * x + y * y + z * z;
    }
};
inline AirPoint3 operator*(float s, const AirPoint3& v)
{
    return AirPoint3(s * v.x, s * v.y, s * v.z);
}
inline const AirPoint3& AirVector(const NiPoint3& point)
{
    return reinterpret_cast<const AirPoint3&>(point);
}

struct AirObject
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c, unk20, unk24;
    int unk28, unk2c, unk30, unk34, unk38, unk3c, unk40, unk44, unk48, unk4c;
    int unk50, unk54, unk58, unk5c, unk60, unk64;
    NiTransform world;
};
struct AirParticle
{
    AirPoint3 velocity;
    float unk0c, unk10, creationTime, unk18;
};

extern AirPoint3 NiPoint3_ZERO;
extern AirPoint3 DAT_011af220;
extern float DAT_011af218;
extern float DAT_011af21c;
extern float DAT_011f4ae0[];
struct AirModifier
{
    int unk00, unk04, unk08, unk0c;
    AirObject* owner;
    int unk14;
    AirObject* source;
    float magnitude;
    float attenuation;
    bool useMaxDistance;
    float maxDistance;
    float maxDistanceSquared;
    AirPoint3 previousPosition;
    float previousTime;
    int unk40, unk44, unk48;
    AirPoint3 direction;
    float airFriction;
    float inheritVelocity;
    bool inheritRotation;
    bool unk61;
    bool enableSpread;
    float spread;
    void UpdateParticleAirFriction(float, NiPSysData*);
    void UpdateAirFriction(float, NiPSysData*);
    void NiPSysAirFieldModifier_Update(float, NiPSysData*);
};
// Updates particle velocities using the field direction and magnitude, with optional spread, distance limiting, attenuation, and inherited velocity.
// @fnv 0x00c2fb40
// src: XBOX_PDB 0x00c2fb40
// pdb: NiPSysAirFieldModifier::Update | NiPSysAirFieldModifier.obj | retail, upstream name already matches, high
// pdb params: fTime, pkData
// pdb locals: fCosSpread, fAirFieldVelocityLength, kUnitAirFieldVelocity, kInvPSys, kPosition, kDirection, kPSys, kAirToPSys, kAirFieldVelocity, kAir, us, pkCurrentParticle, fDeltaT, kParticlePosition, fDistanceSqr, kParticleOffset, kPartVel, kAirVelocity, fAttenuationWithTime
void AirModifier::NiPSysAirFieldModifier_Update(float fTime, NiPSysData* pkData)
{
    if (pkData->m_usActive == 0 || !source)
        return;
    if (attenuation == 0.0f && inheritVelocity == 0.0f && !enableSpread)
    {
        if (useMaxDistance)
            UpdateParticleAirFriction(fTime, pkData);
        else
            UpdateAirFriction(fTime, pkData);
        return;
    }
    NiTransform sourceWorld;
    sourceWorld = source->world;
    NiTransform ownerWorld;
    ownerWorld = owner->world;
    NiTransform inverseTransform;
    ownerWorld.FUN_004b4880(&inverseTransform);
    inverseTransform.FUN_0062c250(&ownerWorld, sourceWorld);
    AirPoint3 fieldDirection;
    if (inheritRotation)
        fieldDirection = AirVector(ownerWorld.m_Rotate.operator*(reinterpret_cast<const NiPoint3&>(direction)));
    else
        fieldDirection = direction;
    AirPoint3 unitVelocity = NiPoint3_ZERO;
    if (previousPosition.FUN_00439090(DAT_011af220) && previousTime != 3.4028234663852886e38 && previousTime != fTime)
    {
        unitVelocity = ((AirVector(ownerWorld.m_Translate) - previousPosition) * inheritVelocity).ProcessAndReturnValue_0053d280(fTime - previousTime);
    }
    previousTime = fTime;
    previousPosition = AirVector(ownerWorld.m_Translate);
    unitVelocity += fieldDirection * magnitude;
    if (unitVelocity.SquaredLength() == 0.0f)
        return;
    {
        AirPoint3 fieldVelocity = unitVelocity;
        float speed = fieldVelocity.FUN_00457910();
        float fMinDot = 0.0f;
        if (enableSpread)
        {
            float angle = spread * DAT_011af218;
            fMinDot = DAT_011f4ae0[((int)(angle * (512.0 / DAT_011af21c))) & 511];
        }
        for (unsigned short i = 0; i < pkData->m_usActive; ++i)
        {
            AirParticle& particle = reinterpret_cast<AirParticle*>(pkData->m_pkParticleInfo)[i];
            float delta = fTime - particle.creationTime;
            if (delta == 0.0f)
                continue;
            AirPoint3* positions = reinterpret_cast<AirPoint3*>(pkData->m_pkVertex);
            AirPoint3 position = positions[i];
            AirPoint3 offset = position - AirVector(ownerWorld.m_Translate);
            float distanceSquared = offset.SquaredLength();
            bool limited = useMaxDistance;
            if (limited && !(distanceSquared <= maxDistanceSquared))
                continue;
            AirPoint3 velocity = unitVelocity;
            if (enableSpread)
            {
                velocity = offset;
                velocity.Unitize_004a0c10();
                float dot = fieldVelocity.Dot(velocity);
                if (dot < fMinDot)
                    continue;
                velocity *= speed;
            }
            AirPoint3 currentVelocity = particle.velocity;
            (void)currentVelocity.y;
            (void)currentVelocity.z;
            AirPoint3 difference = currentVelocity - velocity;
            float dot = difference.Dot(velocity);
            if (!(dot < 0.0f))
                continue;
            float factor = delta;
            if (limited && attenuation != 0.0f)
            {
                float distance = (float)sqrt(distanceSquared);
                float normalized = 1.0f - distance / maxDistance;
                float falloff = (float)pow(normalized, attenuation);
                factor = falloff * delta;
            }
            currentVelocity += (airFriction * velocity) * factor;
            particle.velocity = currentVelocity;
        }
    }
}
