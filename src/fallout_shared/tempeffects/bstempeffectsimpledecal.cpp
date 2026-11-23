// fallout_shared/tempeffects/bstempeffectsimpledecal.cpp (27 functions)

#include "../../../include/AutoMemContext.h"
#include "../../../include/BSTempEffect.h"
#include "../../../include/MemoryManager.h"
#include "../../../include/QueuedFile.h"
#include "../../../include/NiPlane.h"
#include "../../../include/NiPoint3.h"
// In-class bodies of the former Point stand-in.
__forceinline NiPoint3::NiPoint3(float px, float py, float pz) : x(px), y(py), z(pz)
{}
inline NiPoint3 NiPoint3::operator-() const
{
    return NiPoint3(-x, -y, -z);
}
__forceinline NiPoint3 NiPoint3::operator*(float scalar) const
{
    return NiPoint3(scalar * x, scalar * y, scalar * z);
}
__forceinline NiPoint3 NiPoint3::operator/(float scalar) const
{
    float inverse = 1.0f / scalar;
    return NiPoint3(x * inverse, y * inverse, z * inverse);
}
inline bool NiPoint3::operator==(const NiPoint3& other) const
{
    return other.x == x && other.y == y && other.z == z;
}
#include "../../../include/NiPoint3.h"
#include "../../../include/NiPoint3.h"
#include "../../../include/Setting.h"
#include "../../../include/BSTempEffectSimpleDecal.h"
#include "../../../include/NiShadeProperty.h"
#include "../../../include/NiColorA.h"
#include "../../../include/NiPoint3.h"
#include "../../../include/NiFixedString.h"
#include "../../../include/NiUpdateData.h"
#include "../../../include/BSSimpleArray.h"
#include "../../../include/NiMatrix3.h"
#include "../../../include/NiAlphaProperty.h"
#include "../../../include/BSShaderNoLightingProperty.h"

// Memory context IDs used by the decal preload routine.
#include "../../../include/MEM_CONTEXT.h"

// Original PC source filename label passed to the preload memory context;
// contents and target are verified against the engine data section.
const char kBSTempEffectSimpleDecalFileLabel[] =
    "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\TempEffects\\BSTempEffectSimpleDecal.cpp";

// Source line of the preload context in the original PC file (0x11f).
enum
{
    kPreloadDecalDataSourceLine = 0x11f
};

NiPointer<QueuedFile> BSTempEffectSimpleDecal::ms_pPreloadDecalDataFile(0);

// @fnv 0x0068b3e0
// src: XBOX_RETAIL_PDB 0x0068b3e0
// pdb: BSTempEffectSimpleDecal::PreloadDecalData | BSTempEffectSimpleDecal.obj | retail, object-file gap fill, high
// pdb locals: tempMemContext
void BSTempEffectSimpleDecal::PreloadDecalData()
{
    if (ms_pPreloadDecalDataFile)
        return;

    AutoMemContext context(MEM_CONTEXT_MEM_CONTEXT_EFFECTS_OVERHEAD, true, kBSTempEffectSimpleDecalFileLabel,
                           kPreloadDecalDataSourceLine);
    ms_pPreloadDecalDataFile = new QueuedFile(5);
}

// Returns the cross product of this NiPoint3 with point, computing each component from the other two axes.
// @fnv 0x004b3800
// pdb: NiPoint3::Cross | GlobalFunc.obj | debug, upstream name already matches, high
// pdb params: pt
NiPoint3 NiPoint3::Cross(const NiPoint3& pt) const
{
    return NiPoint3(y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x);
}

// The shared material declaration lacks the change-notification method at 0x004bc480.
struct DecalStub
{
    char pad[0x3c];
    float v;
    void NiMaterialProperty_SetChanged(int);
    void NiMaterialProperty_SetAlpha(float c);
};
// Stores c into v and calls FUN_004bc480 with 1.
// @fnv 0x0068c9f0
// src: XBOX_PDB 0x0068c9f0
// pdb: NiMaterialProperty::SetAlpha | BSTempEffectSimpleDecal.obj | debug, upstream name already matches, high
// pdb params: c
void DecalStub::NiMaterialProperty_SetAlpha(float c)
{
    v = c;
    NiMaterialProperty_SetChanged(1);
}

// @fnv 0x0068c890
bool NiShadeProperty::GetBit(unsigned short aFlag)
{
    return (m_uFlags & aFlag) != 0;
}

struct FUN_0068c8b0_shell
{
    char _pad[0xe8];
    float fE8;
    void SetFieldE8(float value);
};

// @fnv 0x0068c8b0
void FUN_0068c8b0_shell::SetFieldE8(float value)
{
    fE8 = value;
}

extern NiFixedString* DAT_011c622c;

// @fnv 0x0068c8d0
// src: COMMUNITY 0x0068c8d0
NiFixedString* __cdecl FixedStrings_GetPermanentDecal()
{
    return DAT_011c622c;
}

extern NiRTTI g_011d6bf0;

// @fnv 0x0068acb0
const NiRTTI* BSTempEffectSimpleDecal::GetRTTI() const
{
    return &g_011d6bf0;
}

// @fnv 0x0068c870
// pdb: NiShadeProperty::GetSmooth | NiShadeProperty.obj | debug, upstream name already matches, high
bool NiShadeProperty::GetSmooth()
{
    return GetBit(1);
}

void __cdecl _delete(void*, unsigned int);
struct S_0068b080
{
    void FUN_0068b080();
};
struct S_0068ace0
{
    void* FUN_0068ace0(unsigned int);
};
// Calls FUN_0068b080 on this, then frees this with _delete when flags has bit 0 set, and returns this.
// @fnv 0x0068ace0
// pdb: BSTempEffectSimpleDecal::`scalar deleting destructor' | BSTempEffectSimpleDecal.obj | debug, object-file gap fill, medium
void* S_0068ace0::FUN_0068ace0(unsigned int flags)
{
    ((S_0068b080*)this)->FUN_0068b080();
    if (flags & 1)
        _delete(this, 0x144);
    return this;
}

// Returns the larger of the two arguments, comparing the float against the double and returning a double.
// @fnv 0x0068b050
// src: COMMUNITY 0x0068b050
double BSMAX_0068b050(double adValue, float afValue)
{
    double dMax;
    if (afValue < adValue)
        dMax = adValue;
    else
        dMax = afValue;
    return dMax;
}

struct Flt
{
    operator unsigned int();
};
struct DecalHolder
{
    bool ProcessGeometry(unsigned int);
    void FUN_00682810();
    void BSTempEffectSimpleDecal_Initialize();
};
// @fnv 0x0068b1e0
// src: COMMUNITY 0x0068b1e0
// pdb: BSTempEffectSimpleDecal::Initialize | BSTempEffectSimpleDecal.obj | retail, upstream name already matches, high
void DecalHolder::BSTempEffectSimpleDecal_Initialize()
{
    if (!ProcessGeometry(*(Flt*)((char*)this + 0x10C)))
        return;
    FUN_00682810();
}

// @fnv 0x0068acc0
// pdb: BSTempEffectSimpleDecal::Get3D | BSTempEffectSimpleDecal.obj | debug, upstream name already matches, high
NiAVObject* BSTempEffectSimpleDecal::Get3D()
{
    return sp3D;
}

// Clips the input polygon through six planes at offsets 0x64..0xb4, ping-ponging between temp and output buffers, and returns the surviving vertex count.
// @fnv 0x0068d660
// pdb: BSTempEffectSimpleDecal::ClipPolygon | BSTempEffectSimpleDecal.obj | debug, object-file run extension, low
// pdb params: aiVertexCount, apInIndex, apInVertex, apInNormal, apOutIndex, apOutVertex, apOutNormal
// pdb locals: tempNormal, tempIndex, tempVertex, count
int BSTempEffectSimpleDecal::ClipPolygonByPlanes(int count, float* apInVerts, NiPoint3* apInB, NiPoint3* apInC,
                                            float* apOutB, NiPoint3* apOutPos, NiPoint3* apOutC)
{
    NiPoint3 tempPos[9];
    NiPoint3 tempC[9];
    float n[9];
    int tempB = ClipPolygonAgainstPlane(&pClippingPlanes[0], count, (NiPoint3*)apInVerts, (float*)apInB, apInC, n,
                                   tempPos, tempC);
    if (tempB)
    {
        tempB = ClipPolygonAgainstPlane(&pClippingPlanes[1], tempB, (NiPoint3*)n, (float*)tempPos, tempC, apOutB,
                                   apOutPos, apOutC);
        if (tempB)
        {
            tempB = ClipPolygonAgainstPlane(&pClippingPlanes[2], tempB, (NiPoint3*)apOutB, (float*)apOutPos, apOutC, n,
                                       tempPos, tempC);
            if (tempB)
            {
                tempB = ClipPolygonAgainstPlane(&pClippingPlanes[3], tempB, (NiPoint3*)n, (float*)tempPos, tempC, apOutB,
                                           apOutPos, apOutC);
                if (tempB)
                {
                    tempB = ClipPolygonAgainstPlane(&pClippingPlanes[5], tempB, (NiPoint3*)apOutB, (float*)apOutPos,
                                               apOutC, n, tempPos, tempC);
                    if (tempB)
                    {
                        tempB = ClipPolygonAgainstPlane(&pClippingPlanes[4], tempB, (NiPoint3*)n, (float*)tempPos, tempC,
                                                   apOutB, apOutPos, apOutC);
                    }
                }
            }
        }
    }
    return tempB;
}

// @fnv 0x0068b4a0
// src: XBOX_PDB 0x0068b4a0
// pdb: BSTempEffectSimpleDecal::UnloadDecalData | BSTempEffectSimpleDecal.obj | retail, upstream name already matches, high
void BSTempEffectSimpleDecal::UnloadDecalData()
{
    ms_pPreloadDecalDataFile = 0;
}

struct DecalPoint
{
    float x, y, z;
    double DotProduct(const DecalPoint&);
    double FUN_00457990();
};

struct DecalPointRecord
{
    DecalPoint position;
    short index;
    short flags;
    DecalPointRecord();
    ~DecalPointRecord()
    {}
};

// pdb: structure ScrapHeapBuffer, size 0x8 (here 0x1), 2 members
struct ScrapHeapBuffer
{
    void Deallocate(void* pointer);
};

void NiFree(void* pointer);

// If the flag at offset 0x2d is clear, deallocates the six pointers at 0xcc-0xf4 and nulls them, then deletes each element of the array at 0xf8 and clears it.
// @fnv 0x0068b220
// src: COMMUNITY 0x0068b220
// pdb: BSTempEffectSimpleDecal::CleanUpMemory | BSTempEffectSimpleDecal.obj | retail, upstream name already matches, high
// pdb locals: pScrapHeap, i
void BSTempEffectSimpleDecal::CleanUpMemory()
{
    ScrapHeapBuffer* pAllocator;

    if (!bValidDecal)
    {
        pAllocator = MemoryManager::Instance().GetGeneralAllocator();

        if (pTempVertexArray)
            pAllocator->Deallocate(pTempVertexArray);

        if (pTempNormalArray)
            pAllocator->Deallocate(pTempNormalArray);

        if (pTempTexcoordArray)
            pAllocator->Deallocate(pTempTexcoordArray);

        if (pTempColorArray)
            pAllocator->Deallocate(pTempColorArray);

        if (pTempIndexArray)
            pAllocator->Deallocate(pTempIndexArray);

        if (pOldToNewIndexArray)
            NiFree(pOldToNewIndexArray);

        pTempVertexArray = 0;
        pTempNormalArray = 0;
        pTempTexcoordArray = 0;
        pTempColorArray = 0;
        pTempIndexArray = 0;
        pOldToNewIndexArray = 0;
    }

    {
        int i;

        for (i = 0; i < reinterpret_cast<BSSimpleArray<DecalPointRecord*>*>(pNewIndexMap)->GetSize(); ++i)
        {
            delete reinterpret_cast<DecalPointRecord*>(
                reinterpret_cast<BSSimpleArray<DecalPointRecord*>*>(pNewIndexMap)->GetAt(i));

            reinterpret_cast<BSSimpleArray<DecalPointRecord*>*>(pNewIndexMap)->GetAt(i) = 0;
        }
    }

    reinterpret_cast<BSSimpleArray<DecalPointRecord*>*>(pNewIndexMap)->Clear(true);
}

extern Setting g_011c57fc;
extern int DAT_0119c298;
extern float DAT_0119c29c;

float cos(float);
double BSMAX_0068b050(double param_1, float param_2);

// Retain the placement layout used by 0x0068ad20: include/DecalData.h
// defines a different 36-byte record, without pCell (+0x48), kColor (+0x60),
// or the placement flags through +0x79 required by this constructor.
struct SimpleDecalParamsView
{ // distinct from DecalData: ctor 0x0068ad20 consumes this 0x7c-byte placement packet, with cell +0x48 and color +0x60, rather than the 0x24-byte texture record.
    NiPoint3 akStart;     // 00
    NiPoint3 kDirection;  // 0c
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    NiNode* pNode;  // 28
    int unk2C;
    int unk30;  // 30
    int unk34;
    float f38;
    float f3C;
    float f40;
    float f44;
    TESObjectCELL* pCell;  // 48
    float f4C;
    int unk50;
    float f54;
    float f58;
    int unk5C;
    NiPoint3 kColor;  // 60
    int unk6C;
    char b70;
    char b71;
    char b72;
    char b73;
    char b74;
    char b75;
    char b76;
    char b77;
    char b78;
    char b79;
};

// The shared decal stores raw bytes at 0xf8 rather than a BSSimpleArray member.
// Keep this construction view until its automatic member lifetime is supported.
class FUN_0068ad20 : public BSTempEffect
{
  public:
    FUN_0068ad20(int apParent, float afLifetime, SimpleDecalParamsView* arDecalData);
    virtual ~FUN_0068ad20();

    char b18;
    char b19;
    char b1A;
    int unk1C;
    NiPointer<NiNode> spNode20;
    int unk24;
    NiPointer<NiNode> spNode28;
    char b2C;
    char b2D;
    char b2E;
    int unk30;
    NiPoint3 kPt34;
    NiPoint3 kPt40;
    NiPoint3 kPt4C;
    NiPoint3 kPt58;
    NiPlane g_kFrustumPlanes[6];
    int unkC4;
    int unkC8;
    int unkCC;
    int unkD0;
    int unkD4;
    int unkD8;
    int unkDC;
    int unkE0;
    int unkE4;
    int unkE8;
    int unkEC;
    int unkF0;
    int unkF4;
    BSSimpleArray<DecalPointRecord*> kList;
    int unk108;
    NiPointer<NiNode> spNode10C;
    int unk110;
    float f114;
    float f118;
    float f11C;
    float f120;
    char b124;
    char b125;
    float f128;
    char b12C;
    char b12D;
    char b12E;
    float f130;
    NiPoint3 kPt134;
    char b140;
    char b141;
};

// @fnv 0x0068ad20
// pdb: BSTempEffectSimpleDecal::BSTempEffectSimpleDecal | BSTempEffectSimpleDecal.obj | debug, call graph, high
// pdb params: apParent, afLifetime, arDecalData
FUN_0068ad20::FUN_0068ad20(int apParent, float afLifetime, SimpleDecalParamsView* arDecalData)
    : BSTempEffect(arDecalData->pCell, afLifetime), spNode20(0), spNode28(0), spNode10C(0),
      kPt134(0.0f, 0.0f, 0.0f)
{
    b18 = 0;
    b2D = 0;
    b19 = 0;
    b1A = 0;
    unk30 = arDecalData->unk30;
    if (DAT_0119c298 == -1)
    {
        DAT_0119c298 = *g_011c57fc.Int();
    }
    unk108 = apParent;
    spNode10C = arDecalData->pNode;
    spNode28 = 0;
    b2C = arDecalData->b71;
    b2E = arDecalData->b78;
    unkC4 = 0;
    unkC8 = 0;
    kPt34 = arDecalData->akStart;
    kPt40 = arDecalData->kDirection;
    f118 = arDecalData->f38;
    f11C = arDecalData->f3C;
    f120 = arDecalData->f40;
    f114 = arDecalData->f44;
    b124 = arDecalData->b70;
    unk1C = 0;
    spNode20 = 0;
    DAT_0119c29c = cos((float)(arDecalData->f58 * 0.01745329238474369));
    DAT_0119c29c = BSMAX_0068b050(0.01, DAT_0119c29c);
    b125 = arDecalData->b73;
    f128 = arDecalData->f4C;
    b12C = arDecalData->b76;
    b12D = arDecalData->b74;
    b12E = arDecalData->b75;
    f130 = arDecalData->f54;
    kPt134 = arDecalData->kColor;
    b140 = arDecalData->b77;
    b141 = arDecalData->b79;
}

extern const double DAT_01012060;
extern const double DAT_01012070;
void* __cdecl NiMaterialProperty_GetType();

struct DecalEffect
{
    float FUN_00598040();
    void NiMaterialProperty_SetAlpha(float c);
};
struct DecalTarget
{
    void ProcessState_004ee560(void* effect);
};
struct DecalState
{
    DecalEffect* FUN_00a59d30(void* texture);
    DecalTarget* NiAVObject_GetParent();
};
struct DecalRef
{
    DecalState* Modelloader_GetReferencedThis();
};
struct DecalHost
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
    DecalRef decalRef;
    bool FUN_006828a0(float alpha);
    double ActorPackage_GetPackageStartTime();
    double FUN_00488d50();
    bool UpdateDecalFade(float alpha);
};
// @fnv 0x0068c8e0
// pdb: BSTempEffectSimpleDecal::Update | BSTempEffectSimpleDecal.obj | debug, object-file run extension, low
// pdb params: afTimeDelta
// pdb locals: bkeep, ftimer, pnode, pProperty, pMaterial, falpha
bool DecalHost::UpdateDecalFade(float alpha)
{
    bool changed = FUN_006828a0(alpha);
    DecalTarget* pDecalTarget;
    if (!changed && decalRef.Modelloader_GetReferencedThis())
    {
        float elapsed = (float)(ActorPackage_GetPackageStartTime() - FUN_00488d50());
        float intensity;
        if (elapsed >= DAT_01012060 && elapsed <= DAT_01012070)
        {
            intensity = (float)((1.0 - elapsed) / DAT_01012070);
            DecalEffect* pDecalEffect = decalRef.Modelloader_GetReferencedThis()->FUN_00a59d30(NiMaterialProperty_GetType());
            DecalEffect* pEffect = pDecalEffect;
            if (pEffect && pEffect->FUN_00598040() != intensity)
                pEffect->NiMaterialProperty_SetAlpha(intensity);
            return true;
        }
        pDecalTarget = decalRef.Modelloader_GetReferencedThis()->NiAVObject_GetParent();
        if (pDecalTarget)
            pDecalTarget->ProcessState_004ee560(this);
    }
    return changed;
}

// The shared NiTriShape lacks the single geometry-data constructor.
class B68CA10Object
{
  public:
    static void* operator new(size_t uiSize);
    static void operator delete(void* pvMem, size_t);

    B68CA10Object(unsigned int uiArg);
    virtual ~B68CA10Object();

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
    int unkC0;
};

// Allocates and constructs a B68CA10Object with the given argument, returning the new object.
// @fnv 0x0068ca10
// pdb: BSTempEffectSimpleDecal::CreateTriShape | BSTempEffectSimpleDecal.obj | debug, object-file run extension, low
// pdb params: data
B68CA10Object* __cdecl CreateB68CA10Object(unsigned int uiArg)
{
    return new B68CA10Object(uiArg);
}

// NiTriShapeData declares this constructor throw(), which removes the original
// exception cleanup from 0x0068ca90. Keep the throwing construction view.
class B68CA90Geometry
{
  public:
    static void* operator new(size_t uiSize);
    static void operator delete(void* pvMem, size_t);

    B68CA90Geometry(unsigned short usVertices, void* pvPositions, void* pvNormals, void* pvColors, void* pvTexture,
                    int iTextureSets, int iDataFlags, unsigned short usTriangles, void* pvTriList);

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
};

// Builds geometry from the decal's secondary vertex and triangle buffers.
// @fnv 0x0068ca90
// src: COMMUNITY 0x0068ca90
// pdb: BSTempEffectSimpleDecal::CreateTriData | BSTempEffectSimpleDecal.obj | debug, object-file run extension, low
// pdb locals: data
int BSTempEffectSimpleDecal::CreateTriData_0068ca90()
{
    B68CA90Geometry* pkGeometry = new B68CA90Geometry((unsigned short)iDecalVertexCount, pFinalVertexArray, pFinalNormalArray, pFinalColorArray, pFinalTexcoordArray,
                                                      1, 0, (unsigned short)iDecalTriangleCount, pFinalIndexArray);
    return (int)pkGeometry;
}

struct DecalTextureObject
{
    ~DecalTextureObject();
    bool BSOcclusionQuery_GetResult(unsigned int* value, int flags);
};

// Calls FUN_00c4edf0 on unk1c with unk24; on success deletes unk1c, nulls it and returns true, otherwise returns false.
// @fnv 0x0068de30
// pdb: BSTempEffectSimpleDecal::CheckOcclusionQueryResults | BSTempEffectSimpleDecal.obj | debug, object-file gap fill, medium
bool BSTempEffectSimpleDecal::ReleaseTextureObject()
{
    if (!reinterpret_cast<DecalTextureObject*>(pOcclusionQuery)->BSOcclusionQuery_GetResult(&iCurrentOcclusionResult, 0))
    {
        return false;
    }
    else
    {
        delete reinterpret_cast<DecalTextureObject*>(pOcclusionQuery);
        pOcclusionQuery = 0;
        return true;
    }
}

struct DecalNode;

void __cdecl ShadowSceneNode_RemoveLightsFromGeom(void* pObject);

struct DecalOwner
{
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v0A();
    virtual void v0B();
    virtual void v0C();
    virtual void v0D();
    virtual void v0E();
    virtual void v0F();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v1A();
    virtual void v1B();
    virtual void v1C();
    virtual void v1D();
    virtual void v1E();
    virtual void v1F();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v2A();
    virtual void v2B();
    virtual void v2C();
    virtual void v2D();
    virtual void v2E();
    virtual void v2F();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void DetachDecal(DecalNode* pNode);
};

struct DecalNode
{
    DecalOwner* GetOwner();
};

extern int DAT_011c57ec;

// The array at 0xf8 must be destroyed between the target and scene pointers.
// Its lifetime is not represented by the shared decal's raw byte buffer.
class FUN_0068b080 : public BSTempEffect
{
  public:
    virtual ~FUN_0068b080();
    void CleanUpMemory();

    char b18;
    char b19;
    char b1A;
    int unk1C;
    NiPointer<DecalNode> spNode20;
    int unk24;
    NiPointer<DecalNode> spNode28;
    bool b2C;
    bool b2D;
    char b2E;
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
    int unkC0;
    int unkC4;
    int unkC8;
    int unkCC;
    int unkD0;
    int unkD4;
    int unkD8;
    int unkDC;
    int unkE0;
    int unkE4;
    int unkE8;
    int unkEC;
    int unkF0;
    int unkF4;
    BSSimpleArray<DecalPointRecord*> kList;
    int unk108;
    NiPointer<DecalNode> spNode10C;
};

// Releases the decal nodes, detaches the node from its owner, calls FUN_0068b220, and decrements DAT_011c57ec when b2D is set.
// @fnv 0x0068b080
// pdb: BSTempEffectSimpleDecal::~BSTempEffectSimpleDecal | BSTempEffectSimpleDecal.obj | debug, call graph, medium
// pdb locals: ptrigeom
FUN_0068b080::~FUN_0068b080()
{
    spNode10C = 0;
    unk108 = 0;
    DecalNode* pNode;
    if (!b2C && (DecalNode*)spNode28)
    {
        pNode = spNode28;
        ShadowSceneNode_RemoveLightsFromGeom(pNode);
        if (((DecalNode*)spNode28)->GetOwner())
        {
            ((DecalNode*)spNode28)->GetOwner()->DetachDecal(spNode28);
        }
        spNode28 = 0;
    }
    CleanUpMemory();
    if (b2D)
    {
        DAT_011c57ec--;
    }
}

struct Graph;
struct Graph
{
    virtual void slot000();
    virtual void slot004();
    virtual void slot008();
    virtual void slot00C();
    virtual void slot010();
    virtual void slot014();
    virtual void slot018();
    virtual void slot01C();
    virtual void slot020();
    virtual void slot024();
    virtual void slot028();
    virtual void slot02C();
    virtual void slot030();
    virtual void slot034();
    virtual void slot038();
    virtual void slot03C();
    virtual void slot040();
    virtual void slot044();
    virtual void slot048();
    virtual void slot04C();
    virtual void slot050();
    virtual void slot054();
    virtual void slot058();
    virtual void slot05C();
    virtual void slot060();
    virtual void slot064();
    virtual void slot068();
    virtual void slot06C();
    virtual void slot070();
    virtual void slot074();
    virtual void slot078();
    virtual void slot07C();
    virtual void slot080();
    virtual void slot084();
    virtual void slot088();
    virtual void slot08C();
    virtual void texture(int, NiPointer<Graph>*);
    virtual void slot094();
    virtual Graph* geometry(int);
    virtual void slot09C();
    virtual void slot0A0();
    virtual void slot0A4();
    virtual void slot0A8();
    virtual void slot0AC();
    virtual void slot0B0();
    virtual void slot0B4();
    virtual void slot0B8();
    virtual void slot0BC();
    virtual void slot0C0();
    virtual void slot0C4();
    virtual void slot0C8();
    virtual void slot0CC();
    virtual void slot0D0();
    virtual void slot0D4();
    virtual void slot0D8();
    virtual void slot0DC();
    virtual void slot0E0();
    virtual void slot0E4();
    virtual void slot0E8();
    virtual void slot0EC();
    virtual void slot0F0();
    virtual int hasFlag(int, int);
    virtual void slot0F8();
    virtual void slot0FC();
    virtual void slot100();
    virtual void attach(BSTempEffectSimpleDecal*, int);
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    int unk024;
    int unk028;
    int unk02C;
    int unk030;
    int unk034;
    int unk038;
    int unk03C;
    int unk040;
    int unk044;
    int unk048;
    int unk04C;
    int unk050;
    int unk054;
    int unk058;
    int unk05C;
    int unk060;
    int unk064;
    int unk068;
    int unk06C;
    int unk070;
    int unk074;
    int unk078;
    int unk07C;
    int unk080;
    int unk084;
    int unk088;
    int unk08C;
    int unk090;
    int unk094;
    int unk098;
    int unk09C;
    int unk0A0;
    int unk0A4;
    int unk0A8;
    Graph* list;
    void AttachProperty(void*);
    Graph* FUN_0050d100();
    Graph* AnimatingText_GetFullText();
    bool checkFlag();
    NiColorA* GetNextProc();
    unsigned short NiGeometry_GetVertexCount();
    Graph* NiAVObject_GetParent();
    unsigned int FUN_0043b480();
    Graph* FUN_0043b4a0(unsigned int);
    void* GetTranslate_0043c490();
    void SetWorldPosition(void*);
    void* rotation();
    void SetBodyOrientation(void*);
    float scale();
    void setScale(float);
    void refresh();
    void update(NiUpdateData&);
    void SetParentObject(void*);
    void addHead(const NiPointer<Graph>&);
};
struct Material
{
    int unk000;
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    int unk024;
    int unk028;
    int unk02C;
    int unk030;
    int unk034;
    int unk038;
    int unk03C;
    int unk040;
    int unk044;
    int unk048;
    static void* operator new(size_t);
    static void operator delete(void*, size_t);
    Material();
    void setAlpha(float);
};

struct Stencil
{
    int unk000;
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    static void* operator new(size_t);
    static void operator delete(void*, size_t);
    Stencil();
    void mode(int);
};
struct Shader
{
    virtual void slot000();
    virtual void slot004();
    virtual void slot008();
    virtual void slot00C();
    virtual void slot010();
    virtual void slot014();
    virtual void slot018();
    virtual void slot01C();
    virtual void slot020();
    virtual void slot024();
    virtual void slot028();
    virtual void slot02C();
    virtual void slot030();
    virtual void slot034();
    virtual void slot038();
    virtual void slot03C();
    virtual void slot040();
    virtual void slot044();
    virtual void slot048();
    virtual void slot04C();
    virtual void slot050();
    virtual void slot054();
    virtual void slot058();
    virtual void slot05C();
    virtual void slot060();
    virtual void slot064();
    virtual void slot068();
    virtual void slot06C();
    virtual void slot070();
    virtual void slot074();
    virtual void slot078();
    virtual void slot07C();
    virtual void slot080();
    virtual void slot084();
    virtual void slot088();
    virtual void slot08C();
    virtual void slot090();
    virtual void slot094();
    virtual void slot098();
    virtual void slot09C();
    virtual void slot0A0();
    virtual void slot0A4();
    virtual void slot0A8();
    virtual void slot0AC();
    virtual void slot0B0();
    virtual void slot0B4();
    virtual void slot0B8();
    virtual void slot0BC();
    virtual void slot0C0();
    virtual void slot0C4();
    virtual void slot0C8();
    virtual void slot0CC();
    virtual void slot0D0();
    virtual void slot0D4();
    virtual void slot0D8();
    virtual void slot0DC();
    virtual void slot0E0();
    virtual void slot0E4();
    virtual void slot0E8();
    virtual void slot0EC();
    virtual void slot0F0();
    virtual int hasFlag(int, int);
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    int unk024;
    int unk028;
    int unk02C;
    int unk030;
    int unk034;
    int unk038;
    int unk03C;
    int unk040;
    int unk044;
    int unk048;
    int unk04C;
    int unk050;
    int unk054;
    int unk058;
    int unk05C;
    int unk060;
    int unk064;
    int unk068;
    int unk06C;
    int unk070;
    int unk074;
    int unk078;
    int unk07C;
    int unk080;
    int unk084;
    int unk088;
    int unk08C;
    int unk090;
    int unk094;
    int unk098;
    int unk09C;
    int unk0A0;
    int unk0A4;
    int unk0A8;
    int unk0AC;
    int unk0B0;
    int unk0B4;
    int unk0B8;
    int unk0BC;
    int unk0C0;
    int unk0C4;
    int unk0C8;
    int unk0CC;
    int unk0D0;
    int unk0D4;
    int unk0D8;
    int unk0DC;
    int unk0E0;
    int unk0E4;
    int unk0E8;
    int unk0EC;
    int unk0F0;
    int unk0F4;
    int unk0F8;
    int unk0FC;
    int unk100;
    int unk104;
    static void* operator new(size_t);
    static void operator delete(void*, size_t);
    Shader();
    void category(int);
    void flag(int, bool);
    void setTexture(int, void*);
    void intensity(float);
    void opacity(float);
};
struct Attachment
{
    int unk000;
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    int unk024;
    int unk028;
    int unk02C;
    int unk030;
    int unk034;
    int unk038;
    int unk03C;
    int unk040;
    int unk044;
    int unk048;
    int unk04C;
    int unk050;
    int unk054;
    int unk058;
    int unk05C;
    int unk060;
    int unk064;
    int unk068;
    int unk06C;
    int unk070;
    int unk074;
    int unk078;
    int unk07C;
    int unk080;
    int unk084;
    int unk088;
    int unk08C;
    int unk090;
    int unk094;
    int unk098;
    int unk09C;
    int unk0A0;
    int unk0A4;
    int unk0A8;
    int unk0AC;
    int unk0B0;
    static void* operator new(size_t);
    static void operator delete(void*, size_t);
    Attachment(Graph*, int);
};
struct TextureAccessor
{
    void* getTexture();
};
struct TextureOwner
{
    int unk000;
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    int unk024;
    int unk028;
    int unk02C;
    Graph texturePart;
};
struct EffectList
{
    void addTail(const NiPointer<Graph>&);
    NiPointer<Graph>& head();
    void removeHead();
};
struct Manager
{
    void remove(BSTempEffectSimpleDecal*);
};
extern int DAT_011c57f4, DAT_011d6a20, DAT_0119c298;
extern float DAT_011c3c08;
extern EffectList g_011d6a30;
extern Manager DAT_011e0e80;
extern Setting g_011c5904;
extern int g_011c7d34;
NiFixedString* __cdecl FixedStrings_GetPermanentDecal();
void* __cdecl FixedStrings_GetDecal();
void __cdecl finalize(Graph*, int, int);
void __cdecl prepare(Graph*);
void __cdecl BSShaderManager_SetObjectTypeRecurse(Graph*, int);
bool __cdecl matches(void*, Graph*);
void __cdecl warning(const char*, ...);
Graph* __cdecl scene(int);
struct SceneOps
{
    void add(Graph*);
};

// @fnv 0x0068be90
// src: XBOX_DEBUG_PDB 0x0068be90
// pdb: BSTempEffectSimpleDecal::FinalizeGeometry | BSTempEffectSimpleDecal.obj | debug, unique string literal, high
// pdb locals: bForceSolidColor, UpdateData, palphaProp, plightingProp, pmatprop, pnode, pdecalnode, bfoundnode, pstencilprop, balpha, ppropertyState, pgeom, pnolightingProp, spTempTexture, spdata, pcolors, i, pchild
bool BSTempEffectSimpleDecal::FinalizeGeometry()
{
    bool disabled = false;
    if ((Graph*)(NiAVObject*)sp3D)
    {
        Material* l = new Material;
        l->setAlpha(fSpecular);
        ((Graph*)(NiAVObject*)sp3D)->AttachProperty(l);
        NiAlphaProperty* e = new NiAlphaProperty;
        e->NiAlphaProperty_SetAlphaBlending(bUseAlphaBlending);
        e->SetFlag512(bUseAlphaTesting);
        e->SetTestMode(4);
        e->NiAlphaProperty_SetTestRef(128);
        ((Graph*)(NiAVObject*)sp3D)->AttachProperty(e);
        if (bTwoSided)
        {
            Stencil* stencil = new Stencil;
            stencil->mode(3);
            ((Graph*)(NiAVObject*)sp3D)->AttachProperty(stencil);
        }
        Shader* c = 0;
        c = new Shader;
        c->category(29);
        c->flag(5, true);
        c->setTexture(0, ((TextureAccessor*)pTextureSet)->getTexture());
        if (c->hasFlag(1, 0) && !disabled)
        {
            if (c->hasFlag(3, 0))
            {
                c->flag(11, true);
                if (bUseParallaxOcclusion)
                {
                    c->flag(28, true);
                    c->intensity(fParallaxOcclusionScale);
                    c->opacity((float)(unsigned char)ucParallaxOcclusionMaxPasses);
                }
            }
            c->flag(27, true);
            Graph* eb = (Graph*)(NiAVObject*)spTargetNode;
            Graph* cf = eb->FUN_0050d100();
            bool q = cf->AnimatingText_GetFullText()->checkFlag();
            if (q)
                c->flag(53, true);
            ((Graph*)(NiAVObject*)sp3D)->AttachProperty(c);
            finalize((Graph*)(NiAVObject*)sp3D, 0, 0);
        }
        else
        {
            BSShaderNoLightingProperty* cf = new BSShaderNoLightingProperty;
            if (!disabled && pTextureSet && !(DAT_011c57f4 & 32))
            {
                NiPointer<Graph> texture(0);
                reinterpret_cast<TextureOwner*>(pTextureSet)->texturePart.texture(0, &texture);
                cf->BSShaderNoLightingProperty_SetBaseMap((NiTexture*)(Graph*)texture);
            }
            cf->SetFlag(27, true);
            Graph* f = (Graph*)(NiAVObject*)spTargetNode;
            Graph* eb = f->FUN_0050d100();
            bool q = eb->AnimatingText_GetFullText()->checkFlag();
            if (q)
                cf->SetFlag(53, true);
            ((Graph*)(NiAVObject*)sp3D)->AttachProperty(cf);
            if (DAT_011c57f4 & 32)
            {
                Graph* geom = (Graph*)(NiAVObject*)sp3D;
                NiColorA* colors = geom->GetNextProc();
                for (int index = 0; index < geom->NiGeometry_GetVertexCount(); index++)
                    colors[index] = NiColorA(1, 1, 0, 1);
            }
            finalize((Graph*)(NiAVObject*)sp3D, 0, 0);
        }
        if (bPermanent)
            ((Graph*)(NiAVObject*)sp3D)->SetParentObject(FixedStrings_GetPermanentDecal());
        else
            ((Graph*)(NiAVObject*)sp3D)->SetParentObject(FixedStrings_GetDecal());
        prepare((Graph*)(NiAVObject*)sp3D);
        BSShaderManager_SetObjectTypeRecurse((Graph*)(NiAVObject*)sp3D, 5);
        if (!bPermanent)
        {
            if (bForceFade)
                DAT_011e0e80.remove(this);
            else
            {
                g_011d6a30.addTail((Graph*)this);
                DAT_011d6a20++;
                if (DAT_011d6a20 > DAT_0119c298)
                {
                    DAT_011e0e80.remove((BSTempEffectSimpleDecal*)(Graph*)g_011d6a30.head());
                    g_011d6a30.removeHead();
                    DAT_011d6a20--;
                    if (*g_011c5904.Bool())
                        warning("DECAL: Reached Max Decal Limit of %d! Removing oldest decal...", DAT_0119c298);
                }
            }
        }
        Graph* k = ((Graph*)(NiAVObject*)spTargetNode)->NiAVObject_GetParent();
        Graph* ba = 0;
        bool b = false;
        if (k)
        {
            for (unsigned int childIndex = 0; childIndex < k->FUN_0043b480(); childIndex++)
            {
                Graph* child = k->FUN_0043b4a0(childIndex);
                if (child && matches(&g_011c7d34, child))
                {
                    b = true;
                    ba = child;
                    break;
                }
            }
        }
        else
        {
            bFinished = true;
            return false;
        }
        if (!b)
            pParentNode = (BGSDecalNode*)new Attachment(((Graph*)(NiAVObject*)spTargetNode)->NiAVObject_GetParent(), 0);
        else
            pParentNode = (BGSDecalNode*)ba;
        reinterpret_cast<Graph*>(pParentNode)->attach(this, 1);
        ((Graph*)(NiAVObject*)sp3D)->SetWorldPosition(((Graph*)(NiAVObject*)spTargetNode)->GetTranslate_0043c490());
        ((Graph*)(NiAVObject*)sp3D)->SetBodyOrientation(((Graph*)(NiAVObject*)spTargetNode)->rotation());
        ((Graph*)(NiAVObject*)sp3D)->setScale(((Graph*)(NiAVObject*)spTargetNode)->scale());
        ((Graph*)(NiAVObject*)sp3D)->refresh();
        NiUpdateData j(DAT_011c3c08, false, false);
        ((Graph*)(NiAVObject*)sp3D)->update(j);
        ((SceneOps*)scene(0))->add(reinterpret_cast<Graph*>(this)->geometry(0));
        reinterpret_cast<Graph*>(pParentNode)->list->addHead((Graph*)this);
        CleanUpMemory();
        bFinalize = false;
        bValidDecal = true;
        bFinished = true;
        return true;
    }
    else
    {
        bFinished = true;
        return false;
    }
}

struct DecalGeometry
{
    unsigned short FUN_004a1ff0();
};
struct DecalGeometryRef
{
    DecalGeometry* object;
    DecalGeometry* Modelloader_GetReferencedThis();
};
struct DecalHeap
{
    void* ScrapHeap_Allocate(unsigned, unsigned);
};
void* _NiMalloc_00aa1070(unsigned);
void* ourMemset(void*, int, unsigned);
bool AreVectorsEqual(const DecalPoint*, const DecalPointRecord*, float);
extern unsigned DAT_010a2720;
extern float DAT_0119c29c;

// @fnv 0x0068cb60
// pdb: BSTempEffectSimpleDecal::ClipMeshWithIterators | BSTempEffectSimpleDecal.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSTempEffectSimpleDecal::AddPolygon
// pdb params: apTriStrips, apGeomData
// pdb locals: i2, pindexlist, pvert, newIndex, istride, piterpos, newVertex, i3, pstripdata, newNormal, i1, pnormal, tricount, iverts, normalIterator, vertexIterator, i, cross, v2, vec1, v1, vec2, v3, vert3 ...
bool BSTempEffectSimpleDecal::AddDecalGeometry(int count, int* paiIndices, DecalPoint* points, DecalPoint* vectors)
{
    long used2;
    used2 = iDecalVertexCount;
    if (used2 + count >= 512)
        return false;
    if (reinterpret_cast<DecalPoint*&>(pTempVertexArray) == 0)
    {
        DecalHeap* vertex_;
        vertex_ = reinterpret_cast<DecalHeap*>(MemoryManager::Instance().GetGeneralAllocator());
        reinterpret_cast<DecalPoint*&>(pTempVertexArray) = (DecalPoint*)vertex_->ScrapHeap_Allocate(6144, DAT_010a2720);
        reinterpret_cast<DecalPoint*&>(pTempNormalArray) = (DecalPoint*)vertex_->ScrapHeap_Allocate(6144, DAT_010a2720);
        pTempTexcoordArray = (NiPoint2*)vertex_->ScrapHeap_Allocate(4096, DAT_010a2720);
        reinterpret_cast<NiColorA*&>(pTempColorArray) = (NiColorA*)vertex_->ScrapHeap_Allocate(8192, DAT_010a2720);
        pTempIndexArray = (unsigned short*)vertex_->ScrapHeap_Allocate(6144, DAT_010a2720);
        DecalGeometry* aMesh = reinterpret_cast<DecalGeometryRef&>(spTargetNode).Modelloader_GetReferencedThis();
        int _numTriangles = aMesh->FUN_004a1ff0();
        pOldToNewIndexArray = (short*)_NiMalloc_00aa1070(_numTriangles * 3 * 2);
        ourMemset(pOldToNewIndexArray, -1, _numTriangles * 3 * 2);
    }
    float scale = 1.0f / (1.0f - DAT_0119c29c);
    for (int i = 0; i < count; ++i)
    {
        unsigned int mapped8 = paiIndices[i];
        if (mapped8 != -1)
        {
            if (pOldToNewIndexArray[mapped8] != -1)
            {
                paiIndices[i] = pOldToNewIndexArray[mapped8];
            }
            else
            {
                reinterpret_cast<DecalPoint*&>(pTempVertexArray)[used2] = points[i];
                pOldToNewIndexArray[mapped8] = (short)used2;
                reinterpret_cast<DecalPoint*&>(pTempNormalArray)[used2] = vectors[i];
                float a;
                DecalPoint normal = reinterpret_cast<DecalPoint*&>(pTempNormalArray)[used2];
                a = (reinterpret_cast<DecalPoint&>(pDecalNormal).DotProduct(normal) / normal.FUN_00457990() - DAT_0119c29c) * scale;
                reinterpret_cast<NiColorA*>(pTempColorArray)[used2] = NiColorA(fVertexColor.r, fVertexColor.g, fVertexColor.b, a > 0.0 ? a : 0.0f);
                paiIndices[i] = used2;
                used2++;
            }
        }
        else
        {
            for (unsigned jA = 0; jA < reinterpret_cast<BSSimpleArray<DecalPointRecord*>&>(pNewIndexMap).GetSize(); ++jA)
            {
                if (AreVectorsEqual(&points[i], reinterpret_cast<BSSimpleArray<DecalPointRecord*>&>(pNewIndexMap).GetAt(jA), 0.01f))
                {
                    mapped8 = reinterpret_cast<BSSimpleArray<DecalPointRecord*>&>(pNewIndexMap).GetAt(jA)->index;
                    break;
                }
            }
            if (mapped8 != -1)
            {
                paiIndices[i] = mapped8;
            }
            else
            {
                reinterpret_cast<DecalPoint*&>(pTempVertexArray)[used2] = points[i];
                DecalPointRecord* s = new DecalPointRecord;
                s->position = reinterpret_cast<DecalPoint*&>(pTempVertexArray)[used2];
                s->index = (short)used2;
                reinterpret_cast<BSSimpleArray<DecalPointRecord*>&>(pNewIndexMap).Add(s);
                reinterpret_cast<DecalPoint*&>(pTempNormalArray)[used2] = vectors[i];
                DecalPoint normal = reinterpret_cast<DecalPoint*&>(pTempNormalArray)[used2];
                float a = (reinterpret_cast<DecalPoint&>(pDecalNormal).DotProduct(normal) / normal.FUN_00457990() - DAT_0119c29c) * scale;
                reinterpret_cast<NiColorA*>(pTempColorArray)[used2] = NiColorA(fVertexColor.r, fVertexColor.g, fVertexColor.b, a > 0.0 ? a : 0.0f);
                paiIndices[i] = used2;
                used2++;
            }
        }
    }
    unsigned short* output;
    output = &pTempIndexArray[iDecalTriangleCount * 3];
    iDecalTriangleCount += count - 2;
    int outIndex8 = 0;
    for (int i = 2; i < count; ++i)
    {
        output[outIndex8] = (unsigned short)paiIndices[0];
        outIndex8 += 1;
        output[outIndex8] = (unsigned short)paiIndices[i - 1];
        ++outIndex8;
        output[outIndex8] = (unsigned short)paiIndices[i];
        outIndex8 += 1;
    }
    iDecalVertexCount = used2;
    return true;
}

struct UnsignedShortBuffer
{
    unsigned short* FUN_00a75f10();
};
struct FixedName
{
    operator const char*() const;
};
struct Geometry
{
    NiPoint3* FUN_0049ec60();
    NiPoint3* NiGeometry_GetNormals();
    UnsignedShortBuffer* NiGeometry_GetModelData();
    FixedName& FUN_00413f40();
    unsigned short FUN_004a1ff0();
};
void __cdecl Warning_005b5e40(const char*, ...);
extern float DAT_0119c29c;

// Iterates triangle strips from the geometry, skips degenerate triangles, and clips those whose face normal faces along direction past a threshold.
// @fnv 0x0068d230
// src: XBOX_RETAIL_PDB 0x0068d230
// pdb: BSTempEffectSimpleDecal::ClipMesh | BSTempEffectSimpleDecal.obj | retail, string-set bridge, high
// pdb params: apAVObj
// pdb locals: pvertices, ptristrips, pgeomdata, pnormals, i2, pindexlist, newIndex, newVertex, i3, pstripdata, newNormal, i1, tricount, i, cross, v2, vec1, v1, vec2, v3, count, binvalidnormal
void BSTempEffectSimpleDecal::ClipMesh(Geometry* apAVObj)
{
    Geometry* data = apAVObj;
    NiPoint3* normals = data->FUN_0049ec60();
    NiPoint3* geometry = data->NiGeometry_GetNormals();
    UnsignedShortBuffer* vertices = data->NiGeometry_GetModelData();
    if (normals && !geometry)
    {
        Warning_005b5e40("TEXTURES: Decal Parent '%s' has no normals!", (const char*)apAVObj->FUN_00413f40());
        return;
    }
    if (!vertices)
        return;
    {
        NiPoint3 third[9];
        NiPoint3 strip[9];
        unsigned short old;
        UnsignedShortBuffer* count = vertices;
        int first = data->FUN_004a1ff0();
        unsigned short* positions = count->FUN_00a75f10();
        unsigned second[9];
        unsigned short indices, bridge;
        for (int i = 0; i < first; i++)
        {
            if (positions[i] == positions[i + 1] || positions[i] == positions[i + 2] ||
                positions[i + 1] == positions[i + 2])
                continue;
            if (i % 2 == 0)
            {
                old = positions[i];
                indices = positions[i + 1];
                bridge = positions[i + 2];
            }
            else
            {
                old = positions[i + 1];
                indices = positions[i];
                bridge = positions[i + 2];
            }
            const NiPoint3& edge3 = normals[old];
            const NiPoint3& edge1 = normals[indices];
            const NiPoint3& a = normals[bridge];
            NiPoint3 edge2 = edge1 - edge3;
            NiPoint3 b = a - edge3;
            NiPoint3 c = edge2.Cross(b);
            if (reinterpret_cast<NiPoint3&>(pDecalNormal).Dot(c) > c.Length() * DAT_0119c29c)
            {
                second[0] = old;
                second[1] = indices;
                second[2] = bridge;
                third[0] = edge3;
                third[1] = edge1;
                third[2] = a;
                strip[0] = geometry[old];
                strip[1] = geometry[indices];
                strip[2] = geometry[bridge];
                int clipped = ClipPolygonByPlanes(3, second, third, strip, second, third, strip);
                if (clipped)
                {
                    if (!AddDecalGeometry(clipped, second, third, strip))
                        break;
                }
            }
        }
    }
}

namespace DecalClipping {
struct Plane
{
    int ClassifyPoint(const NiPoint3&);
    const NiPoint3& FUN_006815c0();
    double FUN_0084d030();
};
extern const double DAT_01012070, DAT_01069ff0, DAT_01012060;
extern const char DAT_01069f88[];
void Warning_005b5e40(const char*, ...);
// Clips a polygon against a plane, emitting intersection vertices where edges cross it and dropping vertices outside, and returns the output vertex count.
// @fnv 0x0068d820
// src: XBOX_DEBUG_PDB 0x0068d820
// pdb: BSTempEffectSimpleDecal::ClipPolygonAgainstPlane | BSTempEffectSimpleDecal.obj | debug, unique string literal, high
// pdb params: apPlane, aiVertexCount, apInIndex, apInVertex, apInNormal, apOutIndex, apOutVertex, apOutNormal
// pdb locals: negativeCount, count, negative, a, b, c, planenormal, t, n2, v2, n1, v1, dv
int BSTempEffectSimpleDecal_ClipPolygonAgainstPlane(Plane* apPlane, int aiVertexCount, const int* apInIndex, const NiPoint3* apInVertex, const NiPoint3* apInNormal,
                       int* apOutIndex, NiPoint3* apOutVertex, NiPoint3* apOutNormal)
{
    bool outside4[10];
    int rejected = 0;
    for (int i = 0; i < aiVertexCount; ++i)
    {
        if (apPlane->ClassifyPoint(apInVertex[i]) == 2)
        {
            outside4[i] = true;
            ++rejected;
        }
        else
            outside4[i] = false;
    }
    if (rejected == aiVertexCount)
        return 0;
    int output = 0;
    for (int current = 0; current < aiVertexCount; ++current)
    {
        int previous = current ? current - 1 : aiVertexCount - 1;
        if (outside4[current])
        {
            if (!outside4[previous])
            {
                NiPoint3 start4 = apInVertex[previous];
                NiPoint3 _end = apInVertex[current];
                NiPoint3 edgeTmp = start4.SubtractIntoVector(_end);
                NiPoint3 normal = apPlane->FUN_006815c0();
                float fraction = (normal.DotProduct(start4) + -apPlane->FUN_0084d030()) / normal.DotProduct(edgeTmp);
                if (fraction > DAT_01012070 && fraction <= DAT_01069ff0)
                    fraction = 1.0f;
                if (fraction < DAT_01012060)
                {
                    Warning_005b5e40(DAT_01069f88, fraction);
                    fraction = 0.0f;
                }
                else if (fraction > DAT_01012070)
                {
                    Warning_005b5e40(DAT_01069f88, fraction);
                    fraction = 1.0f;
                }
                apOutIndex[output] = -1;
                apOutVertex[output] = start4.ApplyScaled(1.0f - fraction).FUN_00439e90(_end.ApplyScaled(fraction));
                NiPoint3 _startAttr = apInNormal[previous];
                NiPoint3 endAttr = apInNormal[current];
                apOutNormal[output] =
                    endAttr.ApplyScaled(1.0f - fraction).FUN_00439e90(_startAttr.ApplyScaled(fraction));
                ++output;
            }
        }
        else
        {
            if (outside4[previous])
            {
                NiPoint3 start_ = apInVertex[current];
                NiPoint3 end2 = apInVertex[previous];
                NiPoint3 edge_ = start_.SubtractIntoVector(end2);
                NiPoint3 normal3 = apPlane->FUN_006815c0();
                float fraction2 = (normal3.DotProduct(start_) + -apPlane->FUN_0084d030()) / normal3.DotProduct(edge_);
                if (fraction2 < DAT_01012060)
                {
                    Warning_005b5e40(DAT_01069f88, fraction2);
                    fraction2 = 0.0f;
                }
                else if (fraction2 > DAT_01012070)
                {
                    Warning_005b5e40(DAT_01069f88, fraction2);
                    fraction2 = 1.0f;
                }
                apOutIndex[output] = -1;
                apOutVertex[output] = start_.ApplyScaled(1.0f - fraction2).FUN_00439e90(end2.ApplyScaled(fraction2));
                NiPoint3 startAttr6 = apInNormal[current];
                NiPoint3 endAttr4 = apInNormal[previous];
                apOutNormal[output] =
                    endAttr4.ApplyScaled(1.0f - fraction2).FUN_00439e90(startAttr6.ApplyScaled(fraction2));
                ++output;
            }
            apOutIndex[output] = apInIndex[current];
            apOutVertex[output] = apInVertex[current];
            apOutNormal[output] = apInNormal[current];
            ++output;
        }
    }
    return output;
}
}

extern int DAT_011f4a20;
extern int DAT_011f426c;
extern char str_01069efc;

struct M3
{
    float m[9];
    void FUN_006815c0();
    void FUN_004b45b0(M3*);
    NiPoint3 FUN_004b4500(NiPoint3*);
    void FUN_00476930(int, NiPoint3*);
};
struct V2
{
    float x;
    float y;
    V2* FUN_00452dc0(float, float);
};
struct H36
{
    float m[9];
    void FUN_006815c0();
    void FUN_004a0c90(float);
    M3 FUN_0043f8d0(M3*);
};
struct SubOut
{
    float x;
    float y;
    float z;
    NiPoint3* SubtractIntoVector(NiPoint3*, NiPoint3*);
};
struct PtSet
{
    NiPoint3* SetComponents(float, float, float);
};
struct Param
{
    NiPoint3* NiAVObject_GetWorldTranslate();
    float NiAVObject_GetWorldScale();
    M3* NiAVObject_GetRotate_006a9540();
    M3* NiAVObject_GetWorldRotate();
};

struct StrObj
{
    const char* FUN_0043b1b0();
};
struct MidObj
{
    StrObj* FUN_00413f40();
};
struct Obj10c
{
    int dummy;
    MidObj* Modelloader_GetReferencedThis();
};
struct Sing
{
    unsigned char b0;
    Sing* FUN_00408d60();
};

bool __cdecl NiObject_IsKindOf(const char*, Param*);
void __cdecl NiMatrix3_TransformVertices(M3*, const char*, int, NiPoint3*, NiPoint3*);
void __cdecl BSUtilities_GetRotationZToVector(M3*, NiPoint3);
int __cdecl CreateB68CA10Object(int);
void __cdecl Warning_005b5e40(const char*, const char*);
extern Sing DAT_011c5904;

struct FUN_00452dc0 : V2
{
    FUN_00452dc0(float, float);
};

// @fnv 0x0068b4c0
// src: XBOX_PDB 0x0068b4c0
// pdb: BSTempEffectSimpleDecal::ProcessGeometry | BSTempEffectSimpleDecal.obj | retail, upstream name already matches, high
// pdb params: apAVObj
// pdb locals: tangent, binormal, bresult, kinvobjrot, d, kmodelspaceorigin, kobjrot, kmodelspaceray, rot, kzrot, kpos, decalnormal, decalcenter, data, one_over_h, one_over_w, a, t, subtex, s, v
bool BSTempEffectSimpleDecal::ProcessGeometry(Param* apAVObj)
{
    // The original uses unconstructed vector storage; NiPoint3 default construction adds calls.
    float vValue40;
    int modeValue10;
    float uValue22;
    float diffValue4[sizeof(NiPoint3) / sizeof(float)];
    int iValue13;
    int tValue0;
    float inv11cValue22;
    float inv118Value2;
    M3 mecValue16;
    float dotValue29;
    float gValue23[sizeof(NiPoint3) / sizeof(float)];
    M3 m1Value12;
    float fValue3[sizeof(NiPoint3) / sizeof(float)];
    M3 m2Value40;
    H36 hValue8;
    float eValue2[sizeof(NiPoint3) / sizeof(float)];
    float d40Value6[sizeof(NiPoint3) / sizeof(float)];
    float c34Value43[sizeof(NiPoint3) / sizeof(float)];
    float aValue1[sizeof(NiPoint3) / sizeof(float)];
    float bValue0[sizeof(NiPoint3) / sizeof(float)];
    bool resultValue23;

    reinterpret_cast<NiPoint3&>(aValue1).FUN_006815c0();

    reinterpret_cast<NiPoint3&>(bValue0).FUN_006815c0();
    if (apAVObj && NiObject_IsKindOf((const char*)&DAT_011f4a20, apAVObj))
    {

        reinterpret_cast<NiPoint3&>(c34Value43) = reinterpret_cast<NiPoint3&>(pOrigin);

        reinterpret_cast<NiPoint3&>(d40Value6) = reinterpret_cast<NiPoint3&>(pVector);

        reinterpret_cast<NiPoint3&>(fValue3).FUN_006815c0();

        reinterpret_cast<NiPoint3&>(gValue23).FUN_006815c0();

        m1Value12.FUN_006815c0();
        if (bModelSpace)
            m1Value12 = *apAVObj->NiAVObject_GetRotate_006a9540();
        else
            m1Value12 = *apAVObj->NiAVObject_GetWorldRotate();

        m1Value12.FUN_004b45b0(&mecValue16);
        reinterpret_cast<NiPoint3&>(fValue3) = mecValue16.FUN_004b4500(&reinterpret_cast<NiPoint3&>(d40Value6));

        reinterpret_cast<NiPoint3&>(eValue2).FUN_006815c0();
        if (bModelSpace)
        {
            reinterpret_cast<NiPoint3&>(eValue2) = reinterpret_cast<NiPoint3&>(c34Value43);
        }
        else
        {
            reinterpret_cast<NiPoint3&>(eValue2) = reinterpret_cast<NiPoint3&>(c34Value43).SubtractIntoVector(*apAVObj->NiAVObject_GetWorldTranslate());
            reinterpret_cast<NiPoint3&>(eValue2) = reinterpret_cast<NiPoint3&>(eValue2).ApplyScaled(1.0f / apAVObj->NiAVObject_GetWorldScale());
        }
        NiMatrix3_TransformVertices(&mecValue16, (const char*)&DAT_011f426c, 1, &reinterpret_cast<NiPoint3&>(eValue2), &reinterpret_cast<NiPoint3&>(gValue23));
        fWidth *= 1.0f / apAVObj->NiAVObject_GetWorldScale();
        fHeight *= 1.0f / apAVObj->NiAVObject_GetWorldScale();
        fDepth *= 1.0f / apAVObj->NiAVObject_GetWorldScale();
        reinterpret_cast<NiPoint3&>(pDecalCenter) = reinterpret_cast<NiPoint3&>(gValue23);
        reinterpret_cast<NiPoint3&>(pDecalNormal) = reinterpret_cast<NiPoint3&>(fValue3);

        BSUtilities_GetRotationZToVector(&m2Value40, reinterpret_cast<NiPoint3&>(fValue3));

        hValue8.FUN_006815c0();
        hValue8.FUN_004a0c90(fRotation);
        m2Value40 = hValue8.FUN_0043f8d0(&m2Value40);
        m2Value40.FUN_00476930(0, &reinterpret_cast<NiPoint3&>(aValue1));
        m2Value40.FUN_00476930(1, &reinterpret_cast<NiPoint3&>(bValue0));

        dotValue29 = reinterpret_cast<NiPoint3&>(pDecalCenter).DotProduct(&reinterpret_cast<NiPoint3&>(aValue1));
        {
            pClippingPlanes[0] = NiPlane(NiPoint3(reinterpret_cast<NiPoint3&>(aValue1).x, reinterpret_cast<NiPoint3&>(aValue1).y, reinterpret_cast<NiPoint3&>(aValue1).z), -(fWidth * 0.5 - dotValue29));
        }
        {
            pClippingPlanes[1] = NiPlane(NiPoint3(-reinterpret_cast<NiPoint3&>(aValue1).x, -reinterpret_cast<NiPoint3&>(aValue1).y, -reinterpret_cast<NiPoint3&>(aValue1).z), -(fWidth * 0.5 + dotValue29));
        }
        dotValue29 = reinterpret_cast<NiPoint3&>(pDecalCenter).DotProduct(&reinterpret_cast<NiPoint3&>(bValue0));
        {
            pClippingPlanes[2] = NiPlane(NiPoint3(reinterpret_cast<NiPoint3&>(bValue0).x, reinterpret_cast<NiPoint3&>(bValue0).y, reinterpret_cast<NiPoint3&>(bValue0).z), -(fHeight * 0.5 - dotValue29));
        }
        {
            pClippingPlanes[3] = NiPlane(NiPoint3(-reinterpret_cast<NiPoint3&>(bValue0).x, -reinterpret_cast<NiPoint3&>(bValue0).y, -reinterpret_cast<NiPoint3&>(bValue0).z), -(fHeight * 0.5 + dotValue29));
        }
        dotValue29 = reinterpret_cast<NiPoint3&>(pDecalCenter).DotProduct(&reinterpret_cast<NiPoint3&>(pDecalNormal));
        {
            pClippingPlanes[4] = NiPlane(NiPoint3(-pDecalNormal.x, -pDecalNormal.y, -pDecalNormal.z), -(fDepth + dotValue29));
        }
        {
            pClippingPlanes[5] = NiPlane(NiPoint3(pDecalNormal.x, pDecalNormal.y, pDecalNormal.z), -(fDepth - dotValue29));
        }
        ClipMesh(apAVObj);
        if (iDecalTriangleCount == 0)
        {
            bFinished = 1;
            if (DAT_011c5904.FUN_00408d60()->b0)
            {
                Warning_005b5e40(&str_01069efc, reinterpret_cast<Obj10c&>(spTargetNode).Modelloader_GetReferencedThis()->FUN_00413f40()->FUN_0043b1b0());
            }
            resultValue23 = false;
        }
        else
        {
            inv118Value2 = 1.0f / fWidth;
            inv11cValue22 = 1.0f / fHeight;

            for (iValue13 = 0; iValue13 < iDecalVertexCount; iValue13++)
            {

                ((SubOut*)&pTempVertexArray[iValue13])->SubtractIntoVector(&reinterpret_cast<NiPoint3&>(diffValue4), &reinterpret_cast<NiPoint3&>(pDecalCenter));
                uValue22 = reinterpret_cast<NiPoint3&>(diffValue4).DotProduct(&reinterpret_cast<NiPoint3&>(aValue1)) * inv118Value2 + 0.5;
                vValue40 = reinterpret_cast<NiPoint3&>(diffValue4).DotProduct(&reinterpret_cast<NiPoint3&>(bValue0)) * inv11cValue22 + 0.5;
                modeValue10 = cSubTexture;
                uValue22 = uValue22 / 2.0;
                vValue40 = vValue40 / 2.0;
                switch (modeValue10)
                {
                case 0:
                    break;
                case 1:
                    uValue22 = uValue22 + 0.5;
                    break;
                case 2:
                    vValue40 = vValue40 + 0.5;
                    break;
                case 3:
                    uValue22 = uValue22 + 0.5;
                    vValue40 = vValue40 + 0.5;
                    break;
                default:
                    uValue22 = uValue22 * 2.0;
                    vValue40 = vValue40 * 2.0;
                    break;
                }
                if (bPermanent)
                {
                    reinterpret_cast<V2*>(pTempTexcoordArray)[iValue13] = FUN_00452dc0(1.0f - uValue22, vValue40);
                }
                else
                {
                    reinterpret_cast<V2*>(pTempTexcoordArray)[iValue13] = FUN_00452dc0(uValue22, vValue40);
                }
            }
            ResizeMeshBuffers();
            tValue0 = CreateTriData_0068ca90();
            sp3D = (NiAVObject*)CreateB68CA10Object(tValue0);
            if ((NiAVObject*)sp3D)
            {
                pFinalVertexArray = 0;
                pFinalNormalArray = 0;
                pFinalTexcoordArray = 0;
                pFinalColorArray = 0;
                pFinalIndexArray = 0;
            }
            bFinalize = 1;
            resultValue23 = true;
        }
    }
    else
    {
        bFinished = 1;
        resultValue23 = false;
    }
    return resultValue23;
}

NiMatrix3 xFromEuler(NiPoint3);
extern const NiPoint3 xg_Zero;

struct xNode;

struct xVObj
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void Refresh();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void AddNode(xNode*, bool);
};
struct xAV
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual struct xAV* GetObjectByName(const NiFixedString&);
    void SetName(const NiFixedString&);
    void Clone(NiPointer<NiAVObject>&);
    void GetTransformScale(const NiPoint3*);
    void SetLocalRotation(const NiMatrix3&);
    void SetScale(float);
    void AttachProperty(void*);
    void F68e740();
    void Update(const NiUpdateData&);
    xVObj* GetParentObj();
    const NiMatrix3& GetRotate();
    const NiMatrix3& GetWorldRotate();
    const NiPoint3* GetTranslate();
    float GetScale();
};

struct xNode : xAV
{
};
struct xGeom : xAV
{
    NiPoint3* GetVertices();
    NiColorA* GetColors();
    unsigned short GetVertexCount();
};

extern NiPointer<NiAVObject> xg_DecalBox;
struct xStream
{
    xStream();
    ~xStream();
    bool Load(const char*, bool);
    xAV* NiStream_GetObjectAt(unsigned int);
    char m_auiShapeKeys[0x5d0];
};
int __fastcall NiStream_GetObjectCount(xStream*);
struct xNiObj
{
    static void* operator new(size_t);
    static void operator delete(void*, size_t);
};

struct xDecalNode : xNiObj
{
    xDecalNode(float);
    int m_auiShapeKeys[0x2c];
};
struct xTES
{
    void AddDecal(xNode*, float);
};
extern xTES* xg_TES;
void x62bc10(xNode*);
bool xIsNear(float, float, float);
void xb57e30(xAV*, int, int);

// Loads and clones the debug decal box, scales its vertices, sets its color and rotation, attaches alpha and shader properties, and adds it to the scene.
// @fnv 0x0068dea0
// src: XBOX_RETAIL_PDB 0x0068dea0
// pdb: BSTempEffectSimpleDecal::CreateDebugBox | BSTempEffectSimpleDecal.obj | retail, unique string literal, high
// pdb params: arColor, abWireframe
// pdb locals: stream, kzvrot, pshaderprop, UpdateData, ptmp, spdata, pavobj, spobj, pverts, pcolors, kzrot, decalnormal, i, kinvobjrot, kobjrot, rot, palpha, pn
void BSTempEffectSimpleDecal::CreateDebugBox(NiColorA& arColor, bool abWireframe)
{
    if (!xg_DecalBox)
    {
        xStream qfTOqo;
        if (qfTOqo.Load("Data\\Meshes\\Decals\\DebugDecalBox.NIF", false))
        {
            if (NiStream_GetObjectCount(&qfTOqo) == 1)
                xg_DecalBox = (NiAVObject*)qfTOqo.NiStream_GetObjectAt(0);
            xg_DecalBox->SetName(NiFixedString("Debug Decal Box"));
        }
    }
    if (xg_DecalBox)
    {
        NiPointer<NiAVObject> qrzAir(0);
        reinterpret_cast<xAV*>(xg_DecalBox.operator->())->Clone(qrzAir);
        xNode* qbXcw = (xNode*)(NiAVObject*)qrzAir;
        xAV* qrKBG = qbXcw->GetObjectByName(NiFixedString("DebugDecalBox01:0"));
        xGeom* qZAdkzg = (xGeom*)qrKBG;
        x62bc10(qbXcw);
        NiPoint3* qGfbX = qZAdkzg->GetVertices();
        NiColorA* qArxcFu = qZAdkzg->GetColors();
        for (int qJDuOZ = 0; qJDuOZ < qZAdkzg->GetVertexCount(); qJDuOZ++)
        {
            qGfbX[qJDuOZ].x = qGfbX[qJDuOZ].x * fWidth;
            qGfbX[qJDuOZ].y = qGfbX[qJDuOZ].y * fHeight;
            qGfbX[qJDuOZ].z *= fDepth * 2.0f;
            qArxcFu[qJDuOZ] = arColor;
        }
        if (xIsNear(pDecalNormal.x, 0.0f, 0.001f))
            pDecalNormal.x = 0.0f;
        if (xIsNear(pDecalNormal.y, 0.0f, 0.001f))
            pDecalNormal.y = 0.0f;
        if (xIsNear(pDecalNormal.z, 0.0f, 0.001f))
            pDecalNormal.z = 0.0f;
        NiPoint3 qxQHW = pDecalNormal;
        if (qxQHW.HasEqualComponents(xg_Zero))
        {
            qxQHW = pVector;
            NiMatrix3 qpia;
            if (bModelSpace)
                qpia = reinterpret_cast<xAV*>(spTargetNode.operator->())->GetRotate();
            else
                qpia = reinterpret_cast<xAV*>(spTargetNode.operator->())->GetWorldRotate();
            NiMatrix3 qZWI = static_cast<const NiMatrix3&>(qpia).Inverse();
            qxQHW = qZWI * qxQHW;
        }
        NiMatrix3 qRqUtp = xFromEuler(qxQHW);
        NiMatrix3 qAhbO;
        qAhbO.MakeZRotation(-fRotation);
        if (bModelSpace)
        {
            qbXcw->GetTransformScale(reinterpret_cast<xAV*>(spTargetNode.operator->())->GetTranslate());
            qbXcw->SetLocalRotation(reinterpret_cast<xAV*>(spTargetNode.operator->())->GetRotate());
            qbXcw->SetScale(reinterpret_cast<xAV*>(spTargetNode.operator->())->GetScale());
            NiMatrix3 qXMUYT = qRqUtp * qAhbO;
            qrKBG->SetLocalRotation(qXMUYT);
            qrKBG->GetTransformScale(&pDecalCenter);
        }
        else
        {
            NiMatrix3 qbfQdhM = reinterpret_cast<xAV*>(spTargetNode.operator->())->GetWorldRotate() * qRqUtp * qAhbO;
            qrKBG->SetLocalRotation(qbfQdhM);
            qrKBG->GetTransformScale(&pOrigin);
        }
        qbXcw->F68e740();
        {
            NiAlphaProperty* qAXSCg = new NiAlphaProperty();
            qAXSCg->NiAlphaProperty_SetAlphaBlending(true);
            qbXcw->AttachProperty(qAXSCg);
        }
        BSShaderNoLightingProperty* qsTDDg = new BSShaderNoLightingProperty();
        if (abWireframe)
            qsTDDg->SetFlag(0x30, 1);
        qrKBG->AttachProperty(qsTDDg);
        xb57e30(qrKBG, 0, 0);
        if (bModelSpace)
        {
            xVObj* qBLsiu = (xVObj*)new xDecalNode(30.0f);
            qBLsiu->AddNode(qbXcw, true);
            reinterpret_cast<xAV*>(spTargetNode.operator->())->GetParentObj()->AddNode((xNode*)qBLsiu, true);
        }
        else
            xg_TES->AddDecal(qbXcw, 30.0f);
        qbXcw->GetParentObj()->Refresh();
        NiUpdateData qBCzzXA(0.0f, false, false);
        qbXcw->Update(qBCzzXA);
    }
}
