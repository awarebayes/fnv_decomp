// gamebryo2.2/corelibs/nimain/nigeometrydata.cpp: NiGeometryData, NiGeometryDataRecovered, NiGeometryDataCmp, GeomData and more (36 functions)

#include "nigeometrydata_decls.h"
#include "../../../../include/NiObjectGroup.h"
#include "../../../../include/NiRenderer.h"
#include "../../../../include/MemoryManager.h"
#include "../../../../include/MemoryHeap.h"
#include "../../../../include/StrArr.h"
#include "../../../../include/NiPoint2.h"
#include "../../../../include/NiColorA.h"
#include "../../../../include/NiPoint3.h"
#include "../../../../include/NiBound.h"
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiStream.h"

inline NiBound::NiBound()
{}


// If additional geometry data is absent or not packed, updates the consistency bits of m_usDirtyFlags to eConsistency.
// @fnv 0x00a67050
// pdb: NiGeometryData::SetConsistency | NiGeometryData.obj | retail, call graph, high
// pdb params: eConsistency
void NiGeometryData::SetDataConsistency(Consistency eConsistency)
{
    if (m_spAdditionalGeomData.m_pObject != NULL && m_spAdditionalGeomData.m_pObject->IsPacked())
    {
        return;
    }
    m_usDirtyFlags = (m_usDirtyFlags & ~CONSISTENCY_MASK) | eConsistency;
}

// ORs the low 12 bits of uiDirtyFlags into m_usDirtyFlags unless the additional geometry data object exists and is packed.
// @fnv 0x00a67090
// pdb: NiGeometryData::MarkAsChanged | NiGeometryData.obj | retail, call graph, medium
// pdb params: usFlags
void NiGeometryData::SetDirtyFlags(unsigned int uiDirtyFlags)
{
    if (m_spAdditionalGeomData.m_pObject != NULL && m_spAdditionalGeomData.m_pObject->IsPacked())
    {
        return;
    }
    m_usDirtyFlags |= (uiDirtyFlags & 0xFFF);
}

// Registers the object and its additional geometry data with the stream, returning false if either registration fails.
// @fnv 0x00a66f50
// pdb: NiGeometryData::RegisterStreamables | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: kStream
bool NiGeometryData::RegisterStreamables(NiStream& kStream)
{
    if (!NiObject::RegisterStreamables(kStream))
        return false;
    NiAdditionalGeometryData* pkAdditional = m_spAdditionalGeomData.m_pObject;
    if (pkAdditional)
    {
        if (!pkAdditional->RegisterStreamables(kStream))
            return false;
    }
    return true;
}

// @fnv 0x00a677e0
// pdb: NiGeometryData::LinkObject | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiGeometryData::LinkObject(NiStream& kStream)
{
    NiObject::LinkObject(kStream);

    if (kStream.GetFileVersion() >= 0x0A030007)
    {
        NiAdditionalGeometryData* pkAdditional = (NiAdditionalGeometryData*)kStream.GetLoadedObject();
        if (m_spAdditionalGeomData.m_pObject != pkAdditional)
        {
            if (m_spAdditionalGeomData.m_pObject)
                m_spAdditionalGeomData.m_pObject->DecRefCount();
            m_spAdditionalGeomData.m_pObject = pkAdditional;
            if (m_spAdditionalGeomData.m_pObject)
                m_spAdditionalGeomData.m_pObject->IncRefCount();
        }
    }
}

// @fnv 0x00a67360
// pdb: NiGeometryData::LockPackedVertexData | NiGeometryData.obj | retail, object-file gap fill, high
// pdb params: bWriteAccess
// pdb locals: pkAGD, uiBlockSize
bool NiGeometryData::IsGeometryDataStreamable(bool bWriteAccess)
{
    if (m_bVBLocked)
        return false;
    NiAdditionalGeometryData* pkAdditional = m_spAdditionalGeomData.m_pObject;
    if (pkAdditional && pkAdditional->IsPacked())
    {
        if (bWriteAccess)
            pkAdditional->NiAdditionalGeometryData_UnlockDataBlock(0);
        else
        {
            unsigned int uiSize = 0;
            pkAdditional->NiAdditionalGeometryData_LockDataBlock(0, &uiSize);
        }
    }
    m_bVBLockWrite = bWriteAccess;
    m_bVBLocked = true;
    return true;
}

#pragma optimize("", off)
// @fnv 0x004410f0
void NiGeometryData::SetCompressFlags(unsigned char ucCompressFlags)
{
    m_ucCompressFlags = ucCompressFlags;
}
#pragma optimize("", on)

struct FUN_00a673d0_shell : public NiGeometryData
{
    void NiGeometryData_UnlockPackedVertexData();
};

// @fnv 0x00a673d0
// src: COMMUNITY 0x00a673d0
// pdb: NiGeometryData::UnlockPackedVertexData | NiGeometryData.obj | retail, upstream name already matches, high
// pdb locals: pkAGD
void FUN_00a673d0_shell::NiGeometryData_UnlockPackedVertexData()
{
    if (*reinterpret_cast<bool*>(reinterpret_cast<char*>(this) + 0x3a))
    {
        NiAdditionalGeometryData* pkAGD =
            *reinterpret_cast<NiAdditionalGeometryData**>(reinterpret_cast<char*>(this) + 0x30);
        if (pkAGD && pkAGD->IsPacked() && !*reinterpret_cast<bool*>(reinterpret_cast<char*>(this) + 0x3b))
        {
            pkAGD->NiAdditionalGeometryData_UnlockDataBlock(0);
        }
        *reinterpret_cast<bool*>(reinterpret_cast<char*>(this) + 0x3a) = false;
    }
}

extern NiObjectGroup* DAT_011f4880;
// @fnv 0x00a669c0
// src: COMMUNITY 0x00a669c0
NiObjectGroup* __cdecl NiGeometryData_GetGroup()
{
    return DAT_011f4880;
}

extern NiObjectGroup* DAT_011f4880;
// @fnv 0x00a669d0
// src: XBOX_PDB 0x00a669d0
void __stdcall NiGeometryData_SetGroup(NiObjectGroup* pkGroup)
{
    DAT_011f4880 = pkGroup;
}

extern MemoryManager g_memoryManager;

// These operations must remain visible to this unit's original inline callers.
inline bool NiColorA::operator==(const NiColorA& c) const
{
    return (r == c.r && g == c.g && b == c.b && a == c.a);
}

__declspec(noinline) inline bool NiColorA::operator!=(const NiColorA& c) const
{
    return !(*this == c);
}

inline void* NiPoint2::operator new[](size_t uiSize)
{
    return g_memoryManager.Allocate(uiSize);
}

inline void* operator new[](size_t uiSize, MemoryManager& memoryManager)
{
    return memoryManager.Allocate(uiSize);
}

struct FUN_00a67000_shell
{
    char pad[0x24];
    void* m_pvBuf;
    void FreeBuffer();
};
// @fnv 0x00a67000
void FUN_00a67000_shell::FreeBuffer()
{
    if (m_pvBuf)
    {
        g_memoryManager.Deallocate(m_pvBuf);
        m_pvBuf = 0;
    }
}

struct FUN_00a67020_shell
{
    char pad[0x10];
    NiBound m_kBound;
    void NiGeometryData_SetBound(NiBound* kBound);
};
// @fnv 0x00a67020
// src: XBOX_PDB 0x00a67020
// pdb: NiGeometryData::ClearNormals | NiGeometryData.obj | retail, object-file run extension, low
void FUN_00a67020_shell::NiGeometryData_SetBound(NiBound* kBound)
{
    if (kBound)
    {
        float fRadius = m_kBound.m_fBoundRadius;
        kBound->m_kCenter = m_kBound.m_kCenter;
        kBound->m_fBoundRadius = fRadius;
    }
}

extern const char* const g_011f5b88_arg1;
// @fnv 0x00a67bf0
// src: COMMUNITY 0x00a67bf0
const char* const* __cdecl NiGeometryData_GetRTTI()
{
    return &g_011f5b88_arg1;
}

struct NiAdd
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
    virtual bool v35();
};
struct _Call_a5d3a0
{
    _Call_a5d3a0();
    virtual void bvf();
    int m_rc;
};
struct SP
{
    NiAdd* m_p;
    SP& operator=(NiAdd* p)
    {
        m_p = p;
        return *this;
    }
    NiAdd* operator->() const
    {
        return m_p;
    }
    operator NiAdd*() const
    {
        return m_p;
    }
};
struct FUN_00a67c00 : _Call_a5d3a0
{
    unsigned short m_usVertices;
    unsigned short m_usGeometryFlags;
    unsigned short m_usTrianglePoints;
    unsigned short m_usDirtyFlags;
    char m_kBound[16];
    void* m_pkVertex;
    void* m_pkNormal;
    void* m_pkColor;
    void* m_pkTexture;
    SP m_spAdd;
    void* m_pkGeometryRendererData;
    unsigned char m_ucKeepFlags;
    unsigned char m_ucCompressFlags;
    bool m_bIsDynamic;
    bool m_bBoundDirty;
    bool m_bVerticesDirty;
    FUN_00a67c00();
};
// @fnv 0x00a67c00
// pdb: NiGeometryData::NiGeometryData | NiGeometryData.obj | retail, call graph, high
// pdb params: usVertices, pkVertex, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod
// pdb locals: usActiveVertices
FUN_00a67c00::FUN_00a67c00()
{
    m_usVertices = 0;
    m_usTrianglePoints = 0;
    m_usDirtyFlags = 0;
    m_pkVertex = 0;
    m_pkNormal = 0;
    m_pkColor = 0;
    m_pkTexture = 0;
    m_spAdd = 0;
    m_pkGeometryRendererData = 0;
    m_ucKeepFlags = 0;
    m_ucCompressFlags = 0;
    m_bIsDynamic = false;
    m_bBoundDirty = false;
    m_bVerticesDirty = true;
    if (!m_spAdd || !m_spAdd->v35())
        m_usDirtyFlags &= 0x8fff;
}

void __cdecl __Dummy_00b6fef0(void*, unsigned int);
struct FUN_00a68800_shell
{
    void NiGeometryData_dtor();
    void* FUN_00a68800(unsigned int flags);
};
// @fnv 0x00a68800
// pdb: NiGeometryData::`scalar deleting destructor' | NiGeometryData.obj | retail, object-file gap fill, high
void* FUN_00a68800_shell::FUN_00a68800(unsigned int flags)
{
    NiGeometryData_dtor();
    if (flags & 1)
        __Dummy_00b6fef0(this, 0x40);
    return this;
}

struct FUN_00a725d0_shell
{
    char pad[8];
    unsigned int m_uiRefCount;
    char pad2[0x14];
    void*** m_ppChildren;
    char pad3[2];
    unsigned short m_usElementCount;
    void NiAdditionalGeometryData_UnlockDataBlock(unsigned int idx);
};
struct VObj
{
    virtual void f0();
    virtual void f1();
    virtual void f2();
    virtual void f3(int, int);
};
struct FUN_00a725d0_shell2
{
    char pad[8];
    int m_uiValueCount;
    char pad2[0x14];
    VObj** m_aiIndexedValues;
    char pad3[2];
    unsigned short m_us;
    void NiAdditionalGeometryData_UnlockDataBlock(unsigned int idx);
};
// @fnv 0x00a725d0
// src: COMMUNITY 0x00a725d0
// pdb: NiAdditionalGeometryData::UnlockDataBlock | NiAdditionalGeometryData.obj | retail, upstream name already matches, high
// pdb params: uiIndex
// pdb locals: pkBlock
void FUN_00a725d0_shell2::NiAdditionalGeometryData_UnlockDataBlock(unsigned int uiIndex)
{
    if (uiIndex < m_us)
    {
        VObj* p = m_aiIndexedValues[uiIndex];
        if (p)
        {
            p->f3(0, 0);
            ++m_uiValueCount;
        }
    }
}

struct NiGeometryData_shell
{
    void* m_ppVirtualMethodTable;
    unsigned short m_usVertices;
    unsigned short m_usTrianglePoints;
    NiPoint3* m_pkVertex;
    NiPoint3* m_pkNormal;
    NiColorA* m_pkColor;
    NiPoint2* m_pkTexture;
    void NiGeometryData_ReplaceData(unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor,
                      NiPoint2* pkTexture, unsigned short usNumTextureSets, unsigned short eNBTMethod);
};

// Stores vertex count, vertex/normal/color/texture pointers, and packs texture-set count and NBT method into the flags field.
// @fnv 0x00a66bb0
// src: XBOX_PDB 0x00a66bb0
// pdb: NiGeometryData::ReplaceData | NiGeometryData.obj | debug, upstream name already matches, high
// pdb params: usVertices, pkVertex, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod
void NiGeometryData_shell::NiGeometryData_ReplaceData(unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal,
                                        NiColorA* pkColor, NiPoint2* pkTexture, unsigned short usNumTextureSets,
                                        unsigned short eNBTMethod)
{
    *(unsigned short*)((char*)this + 8) = usVertices;
    *(NiPoint3**)((char*)this + 0x20) = pkVertex;
    *(NiPoint3**)((char*)this + 0x24) = pkNormal;
    *(NiColorA**)((char*)this + 0x28) = pkColor;
    *(NiPoint2**)((char*)this + 0x2c) = pkTexture;
    *(unsigned short*)((char*)this + 0xc) =
        (*(unsigned short*)((char*)this + 0xc) & 0xFFE) | ((usNumTextureSets & 1)) | eNBTMethod;
}

void __cdecl _delete(void* p, unsigned int size);
inline void NiObjectGroup::DecRefCount()
{
    if (--m_uiRefCount == 0)
    {
        ForwardCall_00a8a670();
        _delete(this, 0x10);
    }
}

namespace fnv_00a67c70 {
struct FUN_00a67c70_shell : public NiGeometryData
{
    void NiGeometryData_dtor();
};

extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long volatile* lpAddend);

extern MemoryManager g_memoryManager;
extern NiObjectGroup* DAT_011f4880;

struct RefStub_a67c70
{
    virtual void v0();
    virtual void DeleteThis();
    unsigned int m_uiRefCount;
};
inline void ReleaseRef_a67c70(RefStub_a67c70* p)
{
    if (InterlockedDecrement((long volatile*)&p->m_uiRefCount) == 0)
        p->DeleteThis();
}

struct FUN_00a67c00
{
    virtual __forceinline ~FUN_00a67c00()
    {
        char* self = (char*)this;
        if (NiRenderer::ms_pRenderer)
            NiRenderer::ms_pRenderer->PurgeGeometryData((NiGeometryData*)this);
        RefStub_a67c70*& spAdditional = *(RefStub_a67c70**)(self + 0x30);
        if (spAdditional != 0)
        {
            ReleaseRef_a67c70(spAdditional);
            spAdditional = 0;
        }
        if (DAT_011f4880)
        {
            if (*(void**)(self + 0x20))
                DAT_011f4880->DecRefCount();
            if (*(void**)(self + 0x24))
                DAT_011f4880->DecRefCount();
            if (*(void**)(self + 0x28))
                DAT_011f4880->DecRefCount();
            if (*(void**)(self + 0x2c))
                DAT_011f4880->DecRefCount();
        }
        else
        {
            g_memoryManager.Deallocate(*(void**)(self + 0x20));
            g_memoryManager.Deallocate(*(void**)(self + 0x24));
            g_memoryManager.Deallocate(*(void**)(self + 0x28));
            g_memoryManager.Deallocate(*(void**)(self + 0x2c));
        }
        if (spAdditional)
            ReleaseRef_a67c70(spAdditional);
        ((NiRefObject*)this)->NiRefObject::~NiRefObject();
    }
};

// Destructor wrapper that invokes the base geometry-data destructor on this object.
// @fnv 0x00a67c70
// src: XBOX_RETAIL_PDB 0x00a67c70
// pdb: NiGeometryData::~NiGeometryData | NiGeometryData.obj | retail, object-file gap fill, high
void FUN_00a67c70_shell::NiGeometryData_dtor()
{
    ((FUN_00a67c00*)this)->FUN_00a67c00::~FUN_00a67c00();
}
}  // namespace fnv_00a67c70

struct NiBoundProxy
{
    void ComputeBoundingSphere(unsigned int vertexCount, NiPoint3* vertices);
};
struct NiGeometryDataRecovered : NiGeometryData
{
    void NiGeometryData_SetData(unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor,
                      NiPoint2* pkTexture, unsigned short usNumTextureSets, unsigned short eNBTMethod);
};
// @fnv 0x00a66b30
// src: COMMUNITY 0x00a66b30
// pdb: NiGeometryData::SetData | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: usVertices, pkVertex, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod
// pdb locals: usActiveVertices
void NiGeometryDataRecovered::NiGeometryData_SetData(unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal,
                                           NiColorA* pkColor, NiPoint2* pkTexture, unsigned short usNumTextureSets,
                                           unsigned short eNBTMethod)
{
    m_usVertices = usVertices;
    m_pkVertex = pkVertex;
    m_pkNormal = pkNormal;
    unsigned short actualCount = GetVertexCount();  // virtual slot 36
    if (actualCount != 0 && m_pkVertex != 0)
        reinterpret_cast<NiBoundProxy*>(&m_kBound)->ComputeBoundingSphere(actualCount, m_pkVertex);
    reinterpret_cast<NiColorA*&>(m_pkColor) = pkColor;
    m_pkTexture = pkTexture;
    m_usDataFlags = (m_usDataFlags & 0x0ffe) | (usNumTextureSets & 1) | eNBTMethod;
}

static __declspec(noinline) int CompareFloats(const float* pA, const float* pB, unsigned int ItemUpdateArray)
{
    for (unsigned int i = 0; i < ItemUpdateArray; i++)
    {
        if (pA[i] < pB[i])
            return -1;
        if (pA[i] > pB[i])
            return 1;
    }
    return 0;
}

struct NiGeometryDataCmp
{
    void* m_ppVirtualMethodTable;
    int unk04;
    unsigned short m_usVertices;
    unsigned short m_usGeometryFlags;
    unsigned short m_usTrianglePoints;
    unsigned short m_usDirtyFlags;
    float m_kBound0;
    float m_kBound1;
    float m_kBound2;
    float m_kBound3;
    float* m_pkVertex;
    float* m_pkNormal;
    float* m_pkColor;
    float* m_pkTexture;
    int NiGeometryData_VertexCompare(unsigned short v0, unsigned short v1, unsigned int mask);
};

// Compares vertex, normal, color and texture data of two vertices selected by usA/usB according to uiFlags, returning -1, 1 or 0.
// @fnv 0x00a66db0
// src: XBOX_RETAIL_PDB 0x00a66db0
// pdb: NiGeometryData::VertexCompare | NiGeometryData.obj | retail, call graph, high
// pdb params: v0, v1, mask
// pdb locals: pkNormal, pkVertex, pkTexture, pkColor, iCmp, usOffset, usVertices, usTextureSets, usSet
int NiGeometryDataCmp::NiGeometryData_VertexCompare(unsigned short v0, unsigned short v1, unsigned int mask)
{
    float* pkVertex;
    pkVertex = m_pkVertex;
    if (mask & 1)
    {
        const float* pB;
        pB = pkVertex + v1 * 3;
        const float* pA = pkVertex + v0 * 3;
        for (unsigned int i = 0; i < 3; i++)
        {
            if (pA[i] < pB[i])
                return -1;
            if (pA[i] > pB[i])
                return 1;
        }
    }
    float* pkNormal = m_pkNormal;
    if (pkNormal && (mask & 2))
    {
        int iCompareResult;
        iCompareResult = CompareFloats(pkNormal + v0 * 3, pkNormal + v1 * 3, 3);
        if (iCompareResult)
            return iCompareResult;
    }
    float* pkColor = m_pkColor;
    if (pkColor && (mask & 4))
    {
        int iCompareResult = CompareFloats(pkColor + v0 * 4, pkColor + v1 * 4, 4);
        if (iCompareResult)
            return iCompareResult;
    }
    float* pkTexture;
    pkTexture = m_pkTexture;
    if (pkTexture && (mask & 8))
    {
        unsigned int uiStride = m_usVertices;
        unsigned short usBaseIndex = 0;
        unsigned short usTextureSets;
        usTextureSets = m_usTrianglePoints & 1;
        for (unsigned short usSet = 0; usSet < usTextureSets; usSet++)
        {
            const float* pB = pkTexture + (v1 + usBaseIndex) * 2;
            const float* pA = pkTexture + (v0 + usBaseIndex) * 2;
            for (unsigned int i = 0; i < 2; i++)
            {
                if (pA[i] < pB[i])
                    return -1;
                if (pA[i] > pB[i])
                    return 1;
            }
            usBaseIndex += uiStride;
        }
    }
    return 0;
}

struct FUN_00a66f90_cls : public NiGeometryData
{
    unsigned int NiGeometryData_GetBlockAllocationSize();
    unsigned int FUN_00e7cd40_base();
};

// Computes the total geometry data size from vertex, normal, color and texture buffers and adds the base size.
// @fnv 0x00a66f90
// src: XBOX_PDB 0x00a66f90
// pdb: NiGeometryData::GetBlockAllocationSize | NiGeometryData.obj | retail, upstream name already matches, high
// pdb locals: uiSize
unsigned int FUN_00a66f90_cls::NiGeometryData_GetBlockAllocationSize()
{
    unsigned int total;

    if (m_pkVertex != 0)
    {
        total = m_usVertices * 3;
        total += total;
        total += total;
    }
    else
    {
        total = 0;
    }
    if (m_pkNormal != 0)
    {
        unsigned int uiNormalSize = (m_usDataFlags & 0xf000) != 0;
        uiNormalSize = uiNormalSize * 2 + 1;
        total += uiNormalSize * m_usVertices * 3 * 4;
    }
    if (m_pkColor != 0)
    {
        total += m_usVertices << 4;
    }
    if (m_pkTexture != 0)
    {
        total += (m_usDataFlags & 1) * m_usVertices * 8;
    }
    return FUN_00e7cd40_base() + total;
}

void* _NiMalloc_00aa1070(unsigned int uiSize);

class FUN_00a68820_cls
{
  public:
    unsigned int unk00;
    unsigned int unk04;
    unsigned short m_08;

    void NiGeometryData_IndexQSort(unsigned short* pusIndex, unsigned int mask, unsigned int l, int r);
    void NiGeometryData_IndexSort(unsigned short** pusIndex0, unsigned short** pusIndex1, unsigned int mask);
};

// @fnv 0x00a68820
// src: XBOX_RETAIL_PDB 0x00a68820
// pdb: NiGeometryData::IndexSort | NiGeometryData.obj | retail, object-file gap fill, high
// pdb params: pusIndex0, pusIndex1, mask
// pdb locals: i, usVertices
void FUN_00a68820_cls::NiGeometryData_IndexSort(unsigned short** pusIndex0, unsigned short** pusIndex1,
                                          unsigned int mask)
{
    unsigned short m_usPassCountLocal = m_08;
    unsigned int uiBytes = (unsigned int)m_usPassCountLocal * 2;
    *pusIndex0 = (unsigned short*)_NiMalloc_00aa1070(uiBytes);
    *pusIndex1 = (unsigned short*)_NiMalloc_00aa1070(uiBytes);

    for (unsigned short i = 0; i < m_usPassCountLocal; i++)
    {
        (*pusIndex0)[i] = i;
    }

    NiGeometryData_IndexQSort(*pusIndex0, mask, 0, (int)m_usPassCountLocal - 1);

    for (unsigned short j = 0; j < m_usPassCountLocal; j++)
    {
        (*pusIndex1)[(*pusIndex0)[j]] = j;
    }
}


extern char* DAT_011f4884;
char* __cdecl NiViewerStrings_NiGetViewerString_Title(char* pName);
char* __cdecl NiGetViewerString_00a7d490(const char* pcPrefix, unsigned short usValue);
char* __cdecl NiGetViewerString_00a7d5a0(const char* pcPrefix, void* pvValue);
char* __cdecl NiGetViewerString_00a7d4d0(const char* pcPrefix, int iValue);
char* __cdecl NiGetViewerString_00a7d410(const char* pcPrefix, unsigned char ucValue);

struct BoundVal
{
    float m_fCenterX;
    float m_fCenterY;
    float m_fCenterZ;
    float m_fBoundRadius;
    char* GetViewerString_00a7fb20(const char* pName);
};

struct AddObj
{
    virtual void reserved00();
    virtual void reserved01();
    virtual void reserved02();
    virtual void reserved03();
    virtual void reserved04();
    virtual void reserved05();
    virtual void reserved06();
    virtual void reserved07();
    virtual void reserved08();
    virtual void reserved09();
    virtual void VirtualFunction10();
    virtual void ReplaceImpactData();
    virtual void reserved12();
    virtual void VirtualFunction13();
    virtual void reserved14();
    virtual void reserved15();
    virtual void reserved16();
    virtual void reserved17();
    virtual void reserved18();
    virtual void reserved19();
    virtual void reserved20();
    virtual void reserved21();
    virtual void VirtualFunction22();
    virtual void VirtualFunction23();
    virtual void RegisterFieldArray(StrArr* arr);
};

static __forceinline void AppendValue(StrArr* arr, char*& pValue)
{
    unsigned int uiSize = arr->m_usSize;
    if (uiSize >= arr->m_usMaxSize)
        arr->SetSize(uiSize + arr->m_usGrowBy);
    arr->SetAt(uiSize, pValue);
}

struct Base380
{
    void AppendCameraPathEntries(StrArr* pStrings);
};

struct GeomData : Base380
{
    void* m_ppVirtualMethodTable;
    int m_iRefCount;
    unsigned short m_usVertices;
    unsigned short m_usGeometryFlags;
    unsigned short m_usTrianglePoints;
    unsigned short m_usDirtyFlags;
    BoundVal m_kBound;
    void* m_pkVertex;
    void* m_pkNormal;
    void* m_pkColor;
    void* m_pkTexture;
    AddObj* m_spAdd;
    void* m_pkGeometryRendererData;
    unsigned char m_ucKeepFlags;
    unsigned char m_ucCompressFlags;
    bool m_bIsDynamic;
    bool m_bBoundDirty;
    bool m_bVerticesDirty;
    void RegisterFields(StrArr* pkStrings);
};

// Appends this geometry's vertex, normal, color, texture, bound and flag fields to arr, then registers the additional-geometry object or a null entry.
// @fnv 0x00a688b0
// pdb: NiGeometryData::GetViewerStrings | NiGeometryData.obj | retail, string-set bridge, high
// pdb params: pkStrings
void GeomData::RegisterFields(StrArr* pkStrings)
{
    AppendCameraPathEntries(pkStrings);
    char* pValue;
    pValue = NiViewerStrings_NiGetViewerString_Title(DAT_011f4884);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d490("m_usVertices", m_usVertices);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d5a0("m_pkVertex", m_pkVertex);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d5a0("m_pkNormal", m_pkNormal);
    AppendValue(pkStrings, pValue);
    int iNBTMethod = m_usTrianglePoints & 0xf000;
    pValue = NiGetViewerString_00a7d4d0("NormalBinormalTangent", iNBTMethod);
    AppendValue(pkStrings, pValue);
    pValue = m_kBound.GetViewerString_00a7fb20("m_kBound");
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d5a0("m_pkColor", m_pkColor);
    AppendValue(pkStrings, pValue);
    unsigned short usNumTextureSets = (unsigned short)(m_usTrianglePoints & 1);
    pValue = NiGetViewerString_00a7d490("Num Texture Sets", usNumTextureSets);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d5a0("m_pkTexture", m_pkTexture);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d490("m_usDirtyFlags", m_usDirtyFlags);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d410("m_ucKeepFlags", m_ucKeepFlags);
    AppendValue(pkStrings, pValue);
    pValue = NiGetViewerString_00a7d410("m_ucCompressFlags", m_ucCompressFlags);
    AppendValue(pkStrings, pValue);
    AddObj* pAddObj = m_spAdd;
    if (pAddObj)
    {
        pAddObj->RegisterFieldArray(pkStrings);
    }
    else
    {
        pValue = NiGetViewerString_00a7d4d0("m_spAdditionalGeomData", 0);
        AppendValue(pkStrings, pValue);
    }
}

// NiGeometryData construction uses a nonthrowing constructor contract and real NiPointer initialization.
extern unsigned short NiGeometryData_ms_usNextID; // src: XBOX_RETAIL_PDB 0x011a9b14 (old: DAT_011a9b14)
struct Base302
{
    virtual ~Base302();
    unsigned int references;
    Base302();
};
struct FUN_00a67b20 : Base302
{
    virtual ~FUN_00a67b20();
    unsigned short vertices, id, dataFlags, dirtyFlags;
    NiBound bound;
    NiPoint3* vertex;
    NiPoint3* normal;
    NiColorA* color;
    NiPoint2* texture;
    NiGeomInlinePointer<NiAdditionalGeometryData> additional;
    void* buffer;
    unsigned char keepFlags, compressFlags;
    bool locked, writeLocked, saveVertices;
    FUN_00a67b20(unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor, NiPoint2* pkTexture, unsigned short usNumTextureSets,
                 unsigned short eNBTMethod) throw();
};
// Initialises vertex/normal/color/texture pointers, sets data flags from textureSets and nbt, computes the bound, assigns a new id, and clears dirty flags.
// @fnv 0x00a67b20
// pdb: NiGeometryData::NiGeometryData | NiGeometryData.obj | retail, call graph (shared callers), high
// pdb params: usVertices, pkVertex, pkNormal, pkColor, pkTexture, usNumTextureSets, eNBTMethod
// pdb locals: usActiveVertices
FUN_00a67b20::FUN_00a67b20(unsigned short usVertices, NiPoint3* pkVertex, NiPoint3* pkNormal, NiColorA* pkColor,
                           NiPoint2* pkTexture, unsigned short usNumTextureSets, unsigned short eNBTMethod) throw()
    : vertices(usVertices), dataFlags(0), dirtyFlags(0), vertex(pkVertex), normal(pkNormal), color(pkColor),
      texture(pkTexture), additional(0), buffer(0), keepFlags(0), compressFlags(0), locked(false), writeLocked(false),
      saveVertices(true)
{
    dataFlags = (dataFlags & 0x0ffe) | (usNumTextureSets & 1) | eNBTMethod;
    if (vertices > 0 && vertex)
        bound.ComputeBoundingSphere(vertices, vertex);
    id = NiGeometryData_ms_usNextID++;
    if (!additional || !additional->IsPacked())
        dirtyFlags &= 0x8fff;
}

struct GeomView
{
    void* vptr;
    unsigned int refs;
    unsigned short usVertices;
    unsigned short NiGeometryData_GetActiveVertexCount() const;
};
// @fnv 0x00a669e0
// src: XBOX_PDB 0x00a669e0
unsigned short GeomView::NiGeometryData_GetActiveVertexCount() const
{
    return usVertices;
}


void __cdecl FUN_00ec61c0(void* dst, int value, unsigned int size);

struct NormalVec
{
    float x;
    float y;
    float z;
    NormalVec()
    {}
    static void* operator new[](size_t bytes)
    {
        return g_memoryManager.Allocate(bytes);
    }
};

struct GeomNormals : public NiGeometryData
{
    void EnsureNormals(bool bZero);
};

// @fnv 0x00a669f0
// pdb: NiGeometryData::CreateNormals | NiGeometryData.obj | retail, object-file run extension, low
// pdb params: bInitialize
// pdb locals: uiFactor
void GeomNormals::EnsureNormals(bool bZero)
{
    unsigned int uiMul = 1;
    if ((m_usDataFlags & 0xf000) != 0)
    {
        uiMul = 3;
    }
    if (m_pkNormal == 0)
    {
        m_pkNormal = (NiPoint3*)new NormalVec[m_usVertices * uiMul];
    }
    if (bZero)
    {
        FUN_00ec61c0(m_pkNormal, 0, m_usVertices * uiMul * 12);
    }
}

struct FUN_00a67720_res
{
    unsigned int m_uiA;
    unsigned int m_uiB;
    unsigned char m_ucC;
};

struct FUN_00a67720_obj
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
    virtual bool v35();
    unsigned int m_i04, m_i08, m_i0c, m_i10, m_i14, m_i18, m_i1c, m_i20, m_i24, m_i28;
    unsigned int m_uiKey2c;
    void NiAdditionalGeometryData_GetDataStream(unsigned int uiStreamIndex, unsigned int* pucData, unsigned int* uiType, unsigned int* usCount, unsigned int* uiTotalSize,
                      unsigned int* uiUnitSize, unsigned int* uiStride);
};

class FUN_00a67720_cls
{
  public:
    unsigned int m_i00, m_i04, m_i08, m_i0c, m_i10, m_i14, m_i18, m_i1c, m_i20, m_i24, m_i28;
    unsigned int m_uiKey2c;
    FUN_00a67720_obj* m_pEntryObject;
    unsigned int m_i34;
    unsigned short m_us38;
    bool m_bFlag3a;
    void NiGeometryData_GetTextureSetIterator(unsigned int usSet, FUN_00a67720_res* arIterator);
};

static unsigned int Mode(bool f0, unsigned int key)
{
    return (f0 + f0 + 2) + ((key >> 1) & 1);
}

// @fnv 0x00a67720
// src: XBOX_RETAIL_PDB 0x00a67720
// pdb: NiGeometryData::GetTextureSetIterator | NiGeometryData.obj | retail, object-file gap fill, high
// pdb params: usSet, arIterator
// pdb locals: pucData, pkAGD, bcompressed, uiStride, usCount, uiType, pkPackedAGD, uiStream, bResult, uiTotalSize, uiUnitSize, uiIndex
void FUN_00a67720_cls::NiGeometryData_GetTextureSetIterator(unsigned int usSet, FUN_00a67720_res* arIterator)
{
    unsigned int a, b, c, u, v, w;
    if (!m_bFlag3a)
    {
        return;
    }
    b = 0;
    c = 0;
    a = 0;
    FUN_00a67720_obj* obj = m_pEntryObject;
    if (obj != 0 && obj->v35())
    {
        unsigned int key = obj->m_uiKey2c;
        unsigned int bit1 = (key >> 1) & 1;
        unsigned int uiMode = ((key & 1) != 0) ? 4 : 2;
        u = 0;
        obj->NiAdditionalGeometryData_GetDataStream(uiMode + bit1, &a, &w, &c, &v, &u, &b);
    }
    unsigned int x, y;
    if (a == 0)
    {
        x = m_uiKey2c;
        y = 8;
    }
    else
    {
        x = a;
        y = b;
    }
    arIterator->m_uiA = x;
    arIterator->m_uiB = y;
    arIterator->m_ucC = 0;
}

struct Owner;

typedef int(__cdecl* CallableFn)(struct Callable*, unsigned int, unsigned int, unsigned int*, unsigned int);

struct Callable
{
    int unk000;
    int m_nCount;
    int unk008;
    CallableFn m_pfn;

    int BinaryWrite(const void* pv, unsigned int bytes, unsigned int uiSize)
    {
        int res = m_pfn(this, (unsigned int)pv, bytes, &uiSize, 1);
        m_nCount += res;
        return res;
    }
};

struct Owner
{
    int unk000;
    int unk004;
    int unk008;
    int unk00c;
    int unk010;
    int unk014;
    int unk018;
    int unk01c;
    int unk020;
    int unk024;
    int unk028;
    int unk02c;
    int unk030;
    int unk034;
    int unk038;
    int unk03c;
    int unk040;
    int unk044;
    int unk048;
    int unk04c;
    int unk050;
    int unk054;
    int unk058;
    int unk05c;
    int unk060;
    int unk064;
    int unk068;
    int unk06c;
    int unk070;
    int unk074;
    int unk078;
    int unk07c;
    int unk080;
    int unk084;
    int unk088;
    int unk08c;
    int unk090;
    int unk094;
    int unk098;
    int unk09c;
    int unk0a0;
    int unk0a4;
    int unk0a8;
    int unk0ac;
    int unk0b0;
    int unk0b4;
    int unk0b8;
    int unk0bc;
    int unk0c0;
    int unk0c4;
    int unk0c8;
    int unk0cc;
    int unk0d0;
    int unk0d4;
    int unk0d8;
    int unk0dc;
    int unk0e0;
    int unk0e4;
    int unk0e8;
    int unk0ec;
    int unk0f0;
    int unk0f4;
    int unk0f8;
    int unk0fc;
    int unk100;
    int unk104;
    int unk108;
    int unk10c;
    int unk110;
    int unk114;
    int unk118;
    int unk11c;
    int unk120;
    int unk124;
    int unk128;
    int unk12c;
    int unk130;
    int unk134;
    int unk138;
    int unk13c;
    int unk140;
    int unk144;
    int unk148;
    int unk14c;
    int unk150;
    int unk154;
    int unk158;
    int unk15c;
    int unk160;
    int unk164;
    int unk168;
    int unk16c;
    int unk170;
    int unk174;
    int unk178;
    int unk17c;
    int unk180;
    int unk184;
    int unk188;
    int unk18c;
    int unk190;
    int unk194;
    int unk198;
    int unk19c;
    int unk1a0;
    int unk1a4;
    int unk1a8;
    int unk1ac;
    int unk1b0;
    int unk1b4;
    int unk1b8;
    int unk1bc;
    int unk1c0;
    int unk1c4;
    int unk1c8;
    int unk1cc;
    int unk1d0;
    int unk1d4;
    int unk1d8;
    int unk1dc;
    int unk1e0;
    int unk1e4;
    int unk1e8;
    int unk1ec;
    int unk1f0;
    int unk1f4;
    int unk1f8;
    int unk1fc;
    int unk200;
    int unk204;
    int unk208;
    int unk20c;
    int unk210;
    int unk214;
    int unk218;
    int unk21c;
    int unk220;
    int unk224;
    int unk228;
    int unk22c;
    int unk230;
    int unk234;
    int unk238;
    int unk23c;
    int unk240;
    int unk244;
    int unk248;
    int unk24c;
    Callable* m_callable;
};

// Doubles a3 three times (multiplying by 8) and writes a2 with that size via the owner's callable BinaryWrite.
// @fnv 0x00a67ae0
// src: COMMUNITY 0x00a67ae0
// pdb: NiPoint2::SaveBinary | NiGeometryData.obj | retail, object-file gap fill, high
// pdb params: stream, pkValues, uiNumValues
void __cdecl NiPoint2_SaveBinary(Owner* stream, unsigned int pkValues, unsigned int uiNumValues)
{
    Callable* pkCallable = stream->m_callable;
    for (unsigned int ui = 0; ui < 3; ++ui)
        uiNumValues += uiNumValues;
    pkCallable->BinaryWrite((void*)pkValues, uiNumValues, 4);
}

namespace fnv_00a66a60 {
extern "C" void* __cdecl memset(void* pvDst, int iValue, unsigned int uiSize);

extern MemoryManager g_memoryManager;



class GeometryDataA66A
{
  public:
    int unk00, unk04;
    unsigned short m_usVertices;
    unsigned short m_usGeometryFlags;
    unsigned short m_usTrianglePoints;
    unsigned short m_usDirtyFlags;
    int unk10, unk14, unk18, unk1C;
    NiPoint3* m_pkVertex;
    NiPoint3* m_pkNormal;

    unsigned int GetNormalBinormalTangentMethod() const
    {
        return m_usTrianglePoints & 0xf000;
    }
    void SetNormalBinormalTangentMethod(unsigned int eMethod)
    {
        m_usTrianglePoints = (unsigned short)((m_usTrianglePoints & 0xfff) | eMethod);
    }

    void NiGeometryData_StoreNormalBinormalTangent(unsigned int eNBTMethod);
};

// Sets the normal binormal tangent method, reallocating m_pkNormal to hold one or three NiPoint3 per vertex depending on eMethod.
// @fnv 0x00a66a60
// src: COMMUNITY 0x00a66a60
// pdb: NiGeometryData::StoreNormalBinormalTangent | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: eNBTMethod
// pdb locals: pkNormal, uiFactor, uiIndex
void GeometryDataA66A::NiGeometryData_StoreNormalBinormalTangent(unsigned int eNBTMethod)
{
    if (GetNormalBinormalTangentMethod() == eNBTMethod)
        return;
    SetNormalBinormalTangentMethod(eNBTMethod);
    if (!m_pkNormal)
        return;

    unsigned int uiMultiplier = 1;
    if (eNBTMethod)
        uiMultiplier = 3;
    NiPoint3* pkNewNormals = new (g_memoryManager) NiPoint3[m_usVertices * uiMultiplier];
    for (unsigned int i = 0; i < m_usVertices; i++)
        pkNewNormals[i] = m_pkNormal[i];
    if (eNBTMethod)
    {
        memset(&pkNewNormals[m_usVertices], 0, sizeof(NiPoint3));
    }
    g_memoryManager.DeallocateAlias(m_pkNormal);
    m_pkNormal = pkNewNormals;
}
}  // namespace fnv_00a66a60

struct NiRenderObj30
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
    virtual bool v35();
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0c;
    unsigned int unk10;
    unsigned int unk14;
    unsigned int unk18;
    unsigned int unk1c;
    unsigned int unk20;
    unsigned int unk24;
    unsigned int unk28;
    unsigned int m_uiFlags2c;
    void NiAdditionalGeometryData_GetDataStream(unsigned int uiStreamIndex, unsigned int* pucData, unsigned int* uiType, unsigned int* usCount, unsigned int* uiTotalSize,
                      unsigned int* uiUnitSize, unsigned int* uiStride);
};

struct BoundOut
{
    unsigned int m_uiPtr;
    unsigned int m_uiCount;
    unsigned char m_ucFlag;
};

struct RenderOwner
{
    char unk00[0x24];
    unsigned int m_uiField24;
    unsigned int unk28;
    unsigned int unk2c;
    NiRenderObj30* m_pObj30;
    unsigned int unk34;
    unsigned char unk38;
    unsigned char unk39;
    bool m_bFlag3a;
    void NiGeometryData_GetNormalsIterator(BoundOut* arIterator);
};

// When m_bFlag3a is set, fills pOut from m_pObj30 or falls back to m_uiField24 and count 0xc.
// @fnv 0x00a675b0
// src: XBOX_RETAIL_PDB 0x00a675b0
// pdb: NiGeometryData::GetNormalsIterator | NiGeometryData.obj | retail, object-file gap fill, high
// pdb params: arIterator
// pdb locals: pucData, pkAGD, bcompressed, uiStride, usCount, uiType, pkPackedAGD, uiStream, bResult, uiTotalSize, uiUnitSize, uiIndex
void RenderOwner::NiGeometryData_GetNormalsIterator(BoundOut* arIterator)
{
    if (m_bFlag3a)
    {
        unsigned int uiA = 0;
        unsigned int uiD = 0;
        unsigned int uiC = 0;
        unsigned int uiE;
        unsigned int pObj;
        NiRenderObj30* pObj30 = m_pObj30;
        if (pObj30 != 0 && pObj30->v35())
        {
            unsigned int uiFlags = (pObj30->m_uiFlags2c & 1) * 2 | 1;
            unsigned int uiB = 0;
            pObj30->NiAdditionalGeometryData_GetDataStream(uiFlags, &uiC, &pObj, &uiD, &uiE, &uiB, &uiA);
        }
        unsigned int uiPtr;
        unsigned int ItemUpdateArray;
        if (uiC == 0)
        {
            uiPtr = m_uiField24;
            ItemUpdateArray = 0xc;
        }
        else
        {
            uiPtr = uiC;
            ItemUpdateArray = uiA;
        }
        arIterator->m_uiPtr = uiPtr;
        arIterator->m_uiCount = ItemUpdateArray;
        arIterator->m_ucFlag = 0;
    }
}

class FUN_00a67dd0_cls
{
  public:
    unsigned int NiGeometryData_ChoosePivot(unsigned short* pusIndex, unsigned int mask, int l, int r);
    int NiGeometryData_VertexCompare(unsigned int v0, unsigned int v1, unsigned int mask);
    void NiGeometryData_IndexQSort(unsigned short* pusIndex, unsigned int mask, int l, int r);
};

// Reorders the unsigned-short entries in pArr between iLo and iHi using pivot selection and CompareVertices.
// @fnv 0x00a67dd0
// src: XBOX_PDB 0x00a67dd0
// pdb: NiGeometryData::IndexQSort | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: pusIndex, mask, l, r
// pdb locals: j, iPivot, i, usTmp
void FUN_00a67dd0_cls::NiGeometryData_IndexQSort(unsigned short* pusIndex, unsigned int mask, int l, int r)
{
    if (r <= l)
        return;

    unsigned short* pBase = pusIndex;
    for (;;)
    {
        int j = r + 1;
        int i = l - 1;
        unsigned int pivot = NiGeometryData_ChoosePivot(pBase, mask, l, r);
        for (;;)
        {
            do
            {
                --j;
            } while (NiGeometryData_VertexCompare(pivot, pBase[j], mask) < 0);
            do
            {
                ++i;
            } while (NiGeometryData_VertexCompare(pBase[i], pivot, mask) < 0);
            if (i >= j)
                break;
            unsigned short t = pBase[i];
            pBase[i] = pBase[j];
            pBase[j] = t;
        }
        if (j == r)
        {
            r = j - 1;
        }
        else
        {
            NiGeometryData_IndexQSort(pBase, mask, l, j);
            l = j + 1;
        }
        if (r <= l)
            return;
    }
}

// Gamebryo's inline NiBinaryStream::BinaryWrite (NiBinaryStream.inl).
__forceinline unsigned int NiBinaryStream::BinaryWrite(const void* pvBuffer, unsigned int uiTotalBytes,
                                                       unsigned int* puiComponentSizes, unsigned int uiNumComponents)
{
    unsigned int uiBytesWritten = m_pfnWrite(this, pvBuffer, uiTotalBytes, puiComponentSizes, uiNumComponents);
    m_uiAbsoluteCurrentPos += uiBytesWritten;
    return uiBytesWritten;
}

// The stream NiStream writes through while saving (Gamebryo m_pkOstr, +0x250).
__forceinline NiBinaryStream* NiStream::GetBinaryStream()
{
    return *reinterpret_cast<NiBinaryStream**>(reinterpret_cast<char*>(this) + 0x250);
}

namespace fnv_00a68520 {

// The saving-side NiStream with the group-ID lookup this unit calls (0x00a634f0).
struct SaveStream : NiStream
{
    unsigned int NiStream_GetIDFromGroup(NiObjectGroup* pkGroup);
};

void NiPoint2_SaveBinary(SaveStream& stream, NiPoint2* pkValues, unsigned int uiNumValues);

// The bounding volume's saving entry point (declared on the real class).
struct NiBoundSave : NiBound
{
    void SaveBinary(SaveStream& kStream);
};

template <class T> FNV_FORCEINLINE void WriteBinaryArray(NiBinaryStream& kStream, T* pValue, unsigned int uiNumEls = 1)
{
    unsigned int uiSize = sizeof(T);
    kStream.BinaryWrite(pValue, uiNumEls * uiSize, &uiSize, 1);
}

// Colour elements are one whole 16-byte NiColorA each, so the byte count is
// formed straight from the vertex count.
template <class T>
FNV_FORCEINLINE void NiBinaryStreamSaveBytes(NiBinaryStream& kStream, T* pValue, unsigned int uiBytes)
{
    unsigned int uiSize = sizeof(float);
    kStream.BinaryWrite(pValue, uiBytes, &uiSize, 1);
}

template <class T> FNV_FORCEINLINE void SerializeValue(SaveStream& kStream, const T& value)
{
    WriteBinaryArray(*kStream.GetBinaryStream(), &value, 1);
}

extern NiObjectGroup* DAT_011f4880;

struct NiGeometryDataSave : NiGeometryData
{
    void NiGeometryData_SaveBinary(SaveStream& kStream);
};

// Serializes NiGeometryData vertex, normal, color, texture and bound data to kStream, writing arrays only when m_bVerticesDirty is set.
// @fnv 0x00a68520
// src: XBOX_PDB 0x00a68520
// pdb: NiGeometryData::SaveBinary | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: uiID, uiNumberOfNormals, bHasColor, bHasVertex, bHasNormal, uiQuantity
void NiGeometryDataSave::NiGeometryData_SaveBinary(SaveStream& kStream)
{
    NiObject::SaveBinary(kStream);

    unsigned int uiObjectGroup = kStream.NiStream_GetIDFromGroup(DAT_011f4880);
    SerializeValue(kStream, uiObjectGroup);

    SerializeValue(kStream, m_usVertices);
    SerializeValue(kStream, m_ucKeepFlags);
    SerializeValue(kStream, m_ucCompressFlags);

    bool bHasVertexData = (m_pkVertex != 0);
    SerializeValue(kStream, bHasVertexData);
    // The vertex blob is three floats per vertex; the byte count is formed
    // at the call site (element count times float size), as in the original.
    if (bHasVertexData && m_bSaveVertexData)
        NiBinaryStreamSaveBytes(*kStream.GetBinaryStream(), (float*)m_pkVertex, m_usVertices * 3 * sizeof(float));

    if (m_pkTexture == 0)
        m_usDataFlags &= (unsigned short)~NiGeometryData::TEXTURE_SET_MASK;
    SerializeValue(kStream, m_usDataFlags);

    unsigned int uiNormalEls = m_usVertices;
    if (m_usDataFlags & NiGeometryData::NBT_METHOD_MASK)
        uiNormalEls = m_usVertices * 3;

    bool bHasNormalData = (m_pkNormal != 0);
    SerializeValue(kStream, bHasNormalData);
    if (bHasNormalData && m_bSaveVertexData)
        WriteBinaryArray(*kStream.GetBinaryStream(), (float*)m_pkNormal, uiNormalEls * 3);

    (*(NiBoundSave*)&m_kBound).SaveBinary(kStream);

    bool bHasColorData = (m_pkColor != 0);
    SerializeValue(kStream, bHasColorData);
    if (bHasColorData && m_bSaveVertexData)
        NiBinaryStreamSaveBytes(*kStream.GetBinaryStream(), (float*)m_pkColor, m_usVertices * sizeof(NiColorA));

    unsigned int uiTextureCount = m_usVertices * (m_usDataFlags & NiGeometryData::TEXTURE_SET_MASK);
    if (uiTextureCount && m_pkTexture && m_bSaveVertexData)
        NiPoint2_SaveBinary(kStream, m_pkTexture, uiTextureCount);

    SerializeValue(kStream, (unsigned short)(m_usDirtyFlags & 0x7000));

    kStream.SaveLinkID(reinterpret_cast<const NiObject*>(m_spAdditionalGeomData.m_pObject));
}
}  // namespace fnv_00a68520

class SlotOverride
{
  public:
    virtual void v00();
    virtual bool HasSlot(int iSlot);  // +0x04
};

class SlotOwner
{
  public:
    int unk00;
    int unk04;
    int unk08;
    unsigned short m_uiLevel : 1;  // +0x0c
    unsigned short m_uiFlagBits : 11;
    unsigned short m_uiHighBits : 4;
    unsigned short unk0e;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    void* m_pSlot0;    // +0x20
    void* m_pSlot3;    // +0x24
    void* m_pSlot4;    // +0x28
    void* m_pLeveled;  // +0x2c
    int unk30;
    SlotOverride* m_pOverride;  // +0x34

    bool NiGeometryData_ContainsVertexData(int eParameter);
};

// Returns true if the slot is occupied, checking m_pOverride first, then the specific slot pointer or m_pLeveled with m_uiLevel thresholds.
// @fnv 0x00a66c00
// src: XBOX_PDB 0x00a66c00
// pdb: NiGeometryData::ContainsVertexData | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: eParameter
bool SlotOwner::NiGeometryData_ContainsVertexData(int eParameter)
{
    if (m_pOverride && m_pOverride->HasSlot(eParameter))
        return true;
    switch (eParameter)
    {
    case 0:
        return m_pSlot0 != 0;
    case 3:
        return m_pSlot3 != 0;
    case 4:
        return m_pSlot4 != 0;
    case 5:
        return m_pLeveled && m_uiLevel >= 1;
    case 6:
        return m_pLeveled && m_uiLevel >= 2;
    case 7:
        return m_pLeveled && m_uiLevel >= 3;
    case 8:
        return m_pLeveled && m_uiLevel >= 4;
    case 9:
        return m_pLeveled && m_uiLevel >= 5;
    case 10:
        return m_pLeveled && m_uiLevel >= 6;
    case 11:
        return m_pLeveled && m_uiLevel >= 7;
    case 12:
        return m_pLeveled && m_uiLevel >= 8;
    case 13:
    case 14:
        return m_pSlot3 && m_uiHighBits;
    }
    return false;
}

struct FUN_00a67660_obj
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
    virtual bool v35();
    void NiAdditionalGeometryData_GetDataStream(unsigned int uiStreamIndex, unsigned int* pucData, unsigned int* uiType, unsigned int* usCount, unsigned int* uiTotalSize,
                      unsigned int* uiUnitSize, unsigned int* uiStride);
    int unk04, unk08, unk0C, unk10, unk14, unk18, unk1C, unk20, unk24, unk28;
    unsigned int m_uiValue1;
};

struct StreamOut_a676
{
    unsigned int m_uiData;
    unsigned int m_uiStride;
    bool m_bFlag;
};

struct FUN_00a67660_cls
{
    int unk00, unk04, unk08, unk0C, unk10, unk14, unk18, unk1C, unk20, unk24;
    unsigned int m_uiDefault;
    int unk2C;
    FUN_00a67660_obj* m_pEntryObject;
    int unk34;
    unsigned short unk38;
    unsigned char m_bEnableState;
    void NiGeometryData_GetColorsIterator(StreamOut_a676* arIterator);
};

// When enabled, it may return early; otherwise it writes data and stride from the entry object or defaults.
// @fnv 0x00a67660
// src: XBOX_RETAIL_PDB 0x00a67660 (was COMMUNITY)
// pdb: NiGeometryData::GetColorsIterator | NiGeometryData.obj | retail, object-file gap fill, high
// pdb params: arIterator
// pdb locals: pucData, pkAGD, bcompressed, uiStride, usCount, uiType, pkPackedAGD, uiStream, bResult, uiTotalSize, uiUnitSize, uiIndex
void FUN_00a67660_cls::NiGeometryData_GetColorsIterator(StreamOut_a676* arIterator)
{
    if (!m_bEnableState)
        return;
    bool bFlag = false;
    unsigned int uiStride = 0;
    unsigned int uiW = 0;
    unsigned int uiData = 0;
    FUN_00a67660_obj* obj = m_pEntryObject;
    if (obj && obj->v35())
    {
        unsigned int fl = obj->m_uiValue1;
        if (!(fl & 2))
            return;
        unsigned int uiZ = 0;
        unsigned int uiU;
        unsigned int uiV;
        obj->NiAdditionalGeometryData_GetDataStream((fl & 1) ? 4 : 2, &uiData, &uiV, &uiW, &uiU, &uiZ, &uiStride);
        if (fl & 0x20)
            bFlag = true;
    }
    if (!uiData)
    {
        uiData = m_uiDefault;
        uiStride = 0x10;
    }
    arIterator->m_uiData = uiData;
    arIterator->m_uiStride = uiStride;
    arIterator->m_bFlag = bFlag;
}

struct Point3Equality : NiPoint3
{
    bool operator==(const NiPoint3& pt) const
    {
        return (x == pt.x && y == pt.y && z == pt.z);
    }
    __declspec(noinline) bool FUN_00439090(const NiPoint3& pt) const
    {
        return !(*this == pt);
    }
};

struct BoundEquality : NiBound
{
    bool operator==(const NiBound& kBound) const
    {
        return (static_cast<const Point3Equality&>(m_kCenter) == kBound.m_kCenter &&
                m_fBoundRadius == kBound.m_fBoundRadius);
    }
// src: GUESS 0x00a926b0 (confidence: high)
    __declspec(noinline) bool IsDifferent(const NiBound& kBound) const
    {
        return !(*this == kBound);
    }
};

struct GeometryEquality : NiGeometryData
{
    bool NiGeometryData_IsEqual(NiObject* pkObject);
};

// Compares this NiGeometryData against pkObject, returning false if vertex, normal, bound, color, texture, or additional data differ.
// @fnv 0x00a67840
// src: XBOX_PDB 0x00a67840
// pdb: NiGeometryData::IsEqual | NiGeometryData.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkData, uiIndex, uiCount, uiQuantity
bool GeometryEquality::NiGeometryData_IsEqual(NiObject* pkObject)
{
    if (!NiObject::IsEqual(pkObject))
        return false;

    NiGeometryData* pkData = static_cast<NiGeometryData*>(pkObject);

    if (m_usVertices != pkData->m_usVertices)
        return false;

    if ((m_pkVertex && !pkData->m_pkVertex) || (!m_pkVertex && pkData->m_pkVertex))
        return false;

    if (m_ucKeepFlags != pkData->m_ucKeepFlags)
        return false;

    if (m_ucCompressFlags != pkData->m_ucCompressFlags)
        return false;

    unsigned int i;

    if (m_pkVertex)
    {
        for (i = 0; i < m_usVertices; i++)
        {
            if (static_cast<Point3Equality*>(&m_pkVertex[i])->FUN_00439090(pkData->m_pkVertex[i]))
                return false;
        }
    }

    if ((m_pkNormal && !pkData->m_pkNormal) || (!m_pkNormal && pkData->m_pkNormal))
        return false;

    if (m_pkNormal)
    {
        if ((m_usDataFlags & NBT_METHOD_MASK) != (pkData->m_usDataFlags & NBT_METHOD_MASK))
            return false;

        unsigned int ItemUpdateArray =
            ((m_usDataFlags & NBT_METHOD_MASK) == NBT_METHOD_NONE) ? m_usVertices : 3 * m_usVertices;
        for (i = 0; i < ItemUpdateArray; i++)
        {
            if (static_cast<Point3Equality*>(&m_pkNormal[i])->FUN_00439090(pkData->m_pkNormal[i]))
                return false;
        }
    }

    if (static_cast<BoundEquality*>(&m_kBound)->IsDifferent(pkData->m_kBound))
        return false;

    if ((m_pkColor && !pkData->m_pkColor) || (!m_pkTexture && pkData->m_pkTexture))
        return false;

    if (m_pkColor)
    {
        for (i = 0; i < m_usVertices; i++)
        {
            if (reinterpret_cast<const NiColorA*>(m_pkColor)[i] !=
                reinterpret_cast<const NiColorA*>(pkData->m_pkColor)[i])
                return false;
        }
    }

    if ((m_pkTexture && !pkData->m_pkTexture) || (!m_pkTexture && pkData->m_pkTexture))
        return false;

    if (m_pkTexture)
    {
        if ((m_usDataFlags & TEXTURE_SET_MASK) != (pkData->m_usDataFlags & TEXTURE_SET_MASK))
            return false;

        unsigned int ItemUpdateArray = (m_usDataFlags & TEXTURE_SET_MASK) * m_usVertices;
        for (i = 0; i < ItemUpdateArray; i++)
        {
            if (m_pkTexture[i].ForwardCall_0051c330(pkData->m_pkTexture[i]))
                return false;
        }
    }

#define FNV_GEOM_SP(p) reinterpret_cast<const NiGeomInlinePointer<NiAdditionalGeometryData>&>(p)
    if ((FNV_GEOM_SP(m_spAdditionalGeomData) && !FNV_GEOM_SP(pkData->m_spAdditionalGeomData)) ||
        (!FNV_GEOM_SP(m_spAdditionalGeomData) && FNV_GEOM_SP(pkData->m_spAdditionalGeomData)))
        return false;

    if (FNV_GEOM_SP(m_spAdditionalGeomData) && FNV_GEOM_SP(pkData->m_spAdditionalGeomData) &&
        // the virtual IsEqual is const; the shared NiObject also declares a non-virtual non-const overload
        !static_cast<const NiAdditionalGeometryData*>(FNV_GEOM_SP(m_spAdditionalGeomData))
             ->IsEqual(FNV_GEOM_SP(pkData->m_spAdditionalGeomData)))
#undef FNV_GEOM_SP
        return false;

    return true;
}

namespace fnv_00a67ea0 {
struct CountEA0
{
    int value;
    CountEA0(int v) : value(v)
    {}
};
struct ElementEA0
{
    int value;
    ElementEA0(int v) : value(v)
    {}
};

extern MemoryManager g_memoryManager;
extern void* DAT_011f4880;
extern char UNK_0109c040;
extern char UNK_004a7800;





struct BuffEA0
{
    void* m_pField0;
    int m_iField4;
    void* m_pCurrent;
    int m_iArrayLength;
};

class ObjEA0
{
  public:
    virtual ~ObjEA0();
    virtual const void* GetRTTI() const;
    virtual ObjEA0* MakeClone(void* pCloning);
    virtual void CopyMemberData(ObjEA0* pkDest, void* pCloning);
    virtual void PostLinkObject(NiStream& kStream);
    virtual bool StreamCanSkip();
    virtual bool GetStreamableRTTIName(char* acName, unsigned int uiMaxSize) const;
    virtual unsigned int GetBlockAllocationSize() const;
    virtual void* GetGroup() const;
    virtual void SetGroup(void* pkGroup);
    virtual void AddViewerStrings(void* pkViewerStrings);
    virtual void SetName(unsigned int uiName);
    virtual bool AddExtraData(ObjEA0* pkExtra);
    virtual ObjEA0* GetExtraData() const;
    virtual bool RemoveExtraDataByName(ObjEA0* pkExtra);
    virtual void UpdateModelBound();
    virtual bool ResizeExtraData(unsigned short usSize);
    virtual void GetNumChildren();
    virtual void ReadGroup(NiStream& kStream);
    virtual void LinkObject(NiStream& kStream);
    virtual bool RegisterStreamables(NiStream& kStream);
    virtual void SaveBinary(NiStream& kStream);
    virtual bool IsEqual(ObjEA0* pkObject);
    virtual void GetViewerStrings(void* pkViewerStrings);
    virtual void InsertExtraData(ObjEA0* pkExtra);
    virtual void ProcessClone(void* pCloning);
    unsigned int m_uiRefCount;
};

inline int ReadGeometryBuffer(NiBinaryStream* reader, void* data, int bytes, int size)
{
    int result = reader->m_pfnRead(reader, data, bytes, reinterpret_cast<unsigned int*>(&size), 1);
    reader->m_uiAbsoluteCurrentPos += result;
    return result;
}

__forceinline void ReadEA0(NiStream& kStream, void* pvDest, int iTotal, int iElem)
{
    NiBinaryStream* pkReader = kStream.m_pkIstr;
    ReadGeometryBuffer(pkReader, pvDest, iTotal, iElem);
}

__forceinline int ReadWrapped(NiBinaryStream* reader, void* data, CountEA0 bytes, ElementEA0 elem)
{
    unsigned int result = reader->m_pfnRead(reader, data, bytes.value,
                                           reinterpret_cast<unsigned int*>(&elem.value), 1);
    reader->m_uiAbsoluteCurrentPos += result;
    return result;
}

__forceinline void ReadNormalEA0(NiStream& stream, void* data, int count)
{
    NiBinaryStream* reader = stream.m_pkIstr;
    ReadWrapped(reader, data, count * 12, 4);
}

__forceinline void ReadNiPoint2(NiStream& stream, void* pkValues, unsigned int uiNumValues)
{
    NiBinaryStream* reader = stream.m_pkIstr;
    for (unsigned int i = 0; i < 3; ++i)
        uiNumValues += uiNumValues;
    ReadGeometryBuffer(reader, pkValues, uiNumValues, 4);
}

void* FUN_00ec61c0(void* pvDest, int iValue, unsigned int uiSize);
typedef void*(__thiscall* CtorStubEA0)(void*);
extern "C" void __stdcall _vector_constructor_iterator(void* apvArray, unsigned int uiElemSize, int iCount,
                                                       CtorStubEA0 pfnCtor);
void FUN_00a67aa0(NiStream& kStream, void* pkValues, int uiNumValues);
void Object_DoNothing(const char* pcFmt, const char* pcName, int iKBytes);

class GeomEA0 : public ObjEA0
{
  public:
    virtual void* GetBoundObject();
    virtual void* GetVertexArray();
    virtual void* GetNormalArray();
    virtual void* GetColorArray();
    virtual void* GetTextureArray();
    virtual void* GetBufferObject();
    virtual BuffEA0* GetBuffData();

    unsigned short m_usVertices;
    unsigned short m_usGeometryFlags;
    unsigned short m_usTrianglePoints;
    unsigned short m_usDirtyFlags;
    NiBound m_kBound;
    NiPoint3* m_pkVertex;
    NiPoint3* m_pkNormal;
    NiColorA* m_pkColor;
    NiPoint2* m_pkTexture;
    void* m_spAdditionalGeomData;
    void* m_pkGeometryRendererData;
    unsigned char m_ucKeepFlags;
    unsigned char m_ucCompressFlags;
    bool m_bIsDynamic;
    bool m_bBoundDirty;
    bool m_bVerticesDirty;

    void ReadBinary(NiStream& kStream);
};

// Reads geometry arrays and bound data from the stream, using buffer storage when available and otherwise allocating arrays.
// @fnv 0x00a67ea0
// pdb: NiGeometryData::LoadBinary | NiGeometryData.obj | retail, string-set bridge, high
// pdb params: kStream
// pdb locals: uiNumberOfNormals, bHasColor, usoldTextureSetCount, bHasVertex, bHasNormal, uiID, usNumTextureSets, uiQuantity, pignoredPointsA, uiextraQuantity
void GeomEA0::ReadBinary(NiStream& kStream)
{
    void* pvCur;
    ObjEA0::ReadGroup(kStream);
    if (kStream.m_uiNifFileVersion >= 0x0a010072)
    {
        int pkBuff;
        ReadEA0(kStream, &pkBuff, 4, 4);
        DAT_011f4880 = kStream.GetObjectGroup(pkBuff);
    }
    ReadEA0(kStream, &m_usVertices, 2, 2);
    if (kStream.m_uiNifFileVersion >= 0x0a000110)
    {
        ReadEA0(kStream, &m_ucKeepFlags, 1, 1);
        ReadEA0(kStream, &m_ucCompressFlags, 1, 1);
    }
    bool r;
    if (kStream.m_uiNifFileVersion < 0x04010000)
    {
        ReadEA0(kStream, &m_pkVertex, 4, 4);
        r = m_pkVertex != 0;
    }
    else
    {
        ReadEA0(kStream, &r, 1, 1);
    }
    if (r)
    {
        if ((pvCur = GetBuffData()) != 0)
        {
            int iTotal = 12 * m_usVertices;
            BuffEA0* tmp = GetBuffData();
            pvCur = tmp->m_pCurrent;
            tmp->m_pCurrent = (char*)pvCur + iTotal;
            tmp->m_iArrayLength++;
            m_pkVertex = (NiPoint3*)pvCur;
        }
        else
        {
            m_pkVertex = new (g_memoryManager) NiPoint3[m_usVertices];
        }
        if (kStream.m_BSStreamHeader.uiVersion >= 0x13 && !m_bVerticesDirty)
            fnv_00a67ea0::FUN_00ec61c0(m_pkVertex, 0, m_usVertices * 12);
        else
            ReadEA0(kStream, m_pkVertex, m_usVertices * 12, 4);
    }
    int iNormalCount = m_usVertices;
    if (kStream.m_uiNifFileVersion >= 0x0a000002)
    {
        ReadEA0(kStream, &m_usTrianglePoints, 2, 2);
        if (m_usTrianglePoints & 0xf000)
            iNormalCount = iNormalCount * 3;
    }
    bool uiTexCount;
    if (kStream.m_uiNifFileVersion < 0x04010000)
    {
        ReadEA0(kStream, &m_pkNormal, 4, 4);
        uiTexCount = m_pkNormal != 0;
    }
    else
    {
        ReadEA0(kStream, &uiTexCount, 1, 1);
    }
    if (uiTexCount)
    {
        if ((pvCur = GetBuffData()) != 0)
        {
            BuffEA0* tmp = GetBuffData();
            pvCur = tmp->m_pCurrent;
            unsigned int iTotal;
            iTotal = iNormalCount * 12;
            tmp->m_pCurrent = (char*)pvCur + iTotal;
            tmp->m_iArrayLength++;
            m_pkNormal = (NiPoint3*)pvCur;
        }
        else
        {
            m_pkNormal = new (g_memoryManager) NiPoint3[iNormalCount];
        }
        if (kStream.m_BSStreamHeader.uiVersion < 0x13 || m_bVerticesDirty)
            ReadNormalEA0(kStream, m_pkNormal, iNormalCount);
    }
    m_kBound.LoadBinary(&kStream);
    bool bHasColor;
    if (kStream.m_uiNifFileVersion < 0x04010000)
    {
        ReadEA0(kStream, &m_pkColor, 4, 4);
        bHasColor = m_pkColor != 0;
    }
    else
    {
        ReadEA0(kStream, &bHasColor, 1, 1);
    }
    if (bHasColor)
    {
        if ((pvCur = GetBuffData()) != 0)
        {
            int iTotal = m_usVertices * 16;
            BuffEA0* tmp = GetBuffData();
            pvCur = tmp->m_pCurrent;
            tmp->m_pCurrent = (char*)pvCur + iTotal;
            tmp->m_iArrayLength++;
            m_pkColor = (NiColorA*)pvCur;
        }
        else
        {
            unsigned int uiColorCount = m_usVertices;
            // Allocate raw colour storage before the original nonthrowing constructor iterator.
            NiColorA* pkColors = (NiColorA*)new (g_memoryManager) unsigned char[uiColorCount][sizeof(NiColorA)];
            if (pkColors)
                _vector_constructor_iterator(pkColors, 16, uiColorCount, (CtorStubEA0)&UNK_004a7800);
            else
                pkColors = 0;
            m_pkColor = pkColors;
        }
        if (kStream.m_BSStreamHeader.uiVersion < 0x13 || m_bVerticesDirty)
            ReadEA0(kStream, m_pkColor, m_usVertices * 16, 4);
    }
    if (kStream.m_uiNifFileVersion < 0x0500000a)
    {
        unsigned short usTempFlags;
        ReadEA0(kStream, &usTempFlags, 2, 2);
        unsigned short bHasVertex = (usTempFlags ^ m_usTrianglePoints) & 1;
        m_usTrianglePoints ^= bHasVertex;
    }
    else if (kStream.m_uiNifFileVersion < 0x0a000002)
    {
        ReadEA0(kStream, &m_usTrianglePoints, 2, 2);
    }
    if (kStream.m_uiNifFileVersion < 0x04010000)
        ReadEA0(kStream, &m_pkTexture, 4, 4);
    unsigned int uiTextureFlags;
    if (kStream.m_BSStreamHeader.uiVersion < 0x1b)
    {
        uiTextureFlags = m_usTrianglePoints & 0x1f;
        if ((unsigned short)uiTextureFlags > 1)
            m_usTrianglePoints |= 1;
    }
    else
    {
        uiTextureFlags = 0;
    }
    if ((unsigned short)(m_usTrianglePoints & 1) > 0)
    {
        unsigned int bHasNormal = (m_usTrianglePoints & 1) * m_usVertices;
        if ((pvCur = GetBuffData()) != 0)
        {
            BuffEA0* tmp = GetBuffData();
            int iTotal = bHasNormal * 8;
            pvCur = tmp->m_pCurrent;
            ++tmp->m_iArrayLength;
            tmp->m_pCurrent = (char*)pvCur + iTotal;
            m_pkTexture = (NiPoint2*)pvCur;
        }
        else
        {
            m_pkTexture = new NiPoint2[bHasNormal];
        }
        if (kStream.m_BSStreamHeader.uiVersion < 0x13 || m_bVerticesDirty)
        {
            ReadNiPoint2(kStream, m_pkTexture, bHasNormal);
            if ((unsigned short)uiTextureFlags > 1)
            {
                unsigned int uiExtraCount = ((unsigned short)uiTextureFlags - 1) * m_usVertices;
                NiPoint2* pkTemp = new NiPoint2[uiExtraCount];
                FUN_00a67aa0(kStream, pkTemp, uiExtraCount);
                g_memoryManager.Deallocate(pkTemp);
                Object_DoNothing(&UNK_0109c040, kStream.m_acFileName, (uiExtraCount * 8) >> 10);
            }
        }
    }
    if (kStream.m_uiNifFileVersion >= 0x0500000a)
        ReadEA0(kStream, &m_usDirtyFlags, 2, 2);
    if (kStream.m_uiNifFileVersion >= 0x0a030007)
        kStream.ReadLinkID();
}
}  // namespace fnv_00a67ea0
