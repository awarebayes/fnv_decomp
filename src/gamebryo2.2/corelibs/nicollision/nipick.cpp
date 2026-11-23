// gamebryo2.2/corelibs/nicollision/nipick.cpp: Results, NiPick (10 functions)
#include "NiObjectNET_inline.h"
#include "../../../../include/MemoryManager.h"
#include "../../../../include/NiTMallocInterface.h"
#define FNV64_NITARRAY_INLINE_BODIES
#include "../../../../include/NiTArray.h"
#include "../../../../include/NiNode.h"
#include "../../../../include/NiPick.h"
#include "../../../../include/NiColorA.h"

#include "nipick_decls.h"
#include "../../../../include/NiStream.h"

// @fnv 0x00e98b40
// src: XBOX_RETAIL_PDB 0x00e98b40
// pdb: NiPick::Results::SortResults | NiPick.obj | retail, call graph, high
// pdb locals: i, pRecord0, j, pRecord1
void NiPick::Results::SortResults()
{
    for (unsigned int i = 0; i < m_uiValidCount; ++i)
    {
        Record* record = GetAt(i);
        for (unsigned int j = i + 1; j < m_uiValidCount; ++j)
        {
            Record* other = GetAt(j);
            if (other->m_fDistance < record->m_fDistance)
            {
                SetAt(j, record);
                SetAt(i, other);
                record = other;
            }
        }
    }
}

// @fnv 0x00e98e20
// pdb: NiPick::PickObjects | NiPick.obj | retail, object-file run extension, low
// pdb params: kOrigin, kDir, bAppend
// pdb locals: bRet
bool NiPick::PerformPick(const NiPoint3& origin, const NiPoint3& direction, bool append)
{
    if (!append)
        resultGrow = 0;
    if (!NiPickIntersections::FindIntersections(origin, direction, *this, GetRoot()) || !resultGrow)
        return false;
    if (m_eSortType != NO_SORT && m_eIntersectType != BOUND_INTERSECT && resultGrow > 1)
    {
        if (m_ePickType == FIND_ALL)
            reinterpret_cast<Results*>(&m_pickResults)->SortResults();
        else
            reinterpret_cast<Results*>(&m_pickResults)->FindClosest();
    }
    return true;
}

// Destructor for NiPick that calls Clear on the object's member.
// @fnv 0x00e98fa0
// pdb: NiPick::~NiPick | NiPick.obj | retail, upstream name already matches, high
NiPick::~NiPick()
{
    reinterpret_cast<Results*>(&m_pickResults)->Clear();
    if (m_spRoot)
        static_cast<NiAVObject*>(m_spRoot)->NiObjectNET::DecRefCount();
}

// Releases each record's two referenced objects, deallocates the records, then frees the base array.
// @fnv 0x00e98e90
// pdb: NiPick::Results::~Results | NiPick.obj | retail, object-file run extension, low
// pdb locals: ui
void NiPick::Results::Clear() throw()
{
    m_ppVirtualMethodTable = DAT_010f16ec;
    for (unsigned int i = 0; i < m_usSize; ++i)
    {
        Record* record = m_pBase[i];
        if (record)
        {
            if (record->m_spProxyParent.m_pObject)
                record->m_spProxyParent.m_pObject->NiObjectNET::DecRefCount();
            if (record->m_spObject.m_pObject)
                record->m_spObject.m_pObject->NiObjectNET::DecRefCount();
            g_memoryManager.Deallocate(record);
        }
    }
    m_ppVirtualMethodTable = DAT_010f16dc;
    NiFree(m_pBase);
}

// Calls Clear, then frees this object through g_memoryManager when free is set, and returns this.
// @fnv 0x00e98fd0
// pdb: NiPick::ClearResultsArray | NiPick.obj | debug, object-file run extension, low
// pdb params: bDeallocateRecords
void* __fastcall DestroyPickResults(NiPick::Results* results, void*, bool free)
{
    results->Clear();
    if (free & 1)
        g_memoryManager.Deallocate(results);
    return results;
}

extern int DAT_0126fa60;
void __cdecl InitNiStreamCallbacks();
void __cdecl NiCollisionSDM_Shutdown();
void __cdecl NiStaticDataManager_AddLibrary(void*, void*);

struct FUN_00e99300_shell
{
    void* __thiscall NiCollisionSDM_NiCollisionSDM();
};

// Increments DAT_0126fa60 and, when the previous value was zero, registers InitNiStreamCallbacks and FUN_00e99260 via FUN_00aa2080; returns this.
// @fnv 0x00e99300
// src: COMMUNITY 0x00e99300
// pdb: NiCollisionSDM::NiCollisionSDM | NiCollisionSDM.obj | retail, upstream name already matches, high
void* __thiscall FUN_00e99300_shell::NiCollisionSDM_NiCollisionSDM()
{
    void* result = this;
    int iPreviousCount = DAT_0126fa60;
    DAT_0126fa60 = DAT_0126fa60 + 1;
    if (iPreviousCount == 0)
        NiStaticDataManager_AddLibrary((void*)InitNiStreamCallbacks, (void*)NiCollisionSDM_Shutdown);
    return result;
}

void* __cdecl _NiMalloc(unsigned int);
typedef void(__cdecl* PFN)();
extern const char g_0126fab4_arg0[];
NiObject* __cdecl Runtime_CreateNode();
void __cdecl NiRegisterBVStream();
int* __cdecl NiCollisionConvert_ConvertToSphereBV_Fast(float, int*);
struct Obj;
Obj* __cdecl NiCollisionConvert_ConvertToSphereBV_Safe(float, int*);
void __cdecl NiCollisionConvert_ConvertToBoxBV();
void __cdecl NiHalfSpaceBV_HalfSpaceSphereTestIntersect();
void __cdecl IsIntersectingFogPlane();
void __cdecl NiHalfSpaceBV_HalfSpaceCapsuleTestIntersect();
void __cdecl NiHalfSpaceBV_HalfSpaceHalfSpaceTestIntersect();
void __cdecl NiHalfSpaceBV_HalfSpaceTriTestIntersect();
void __cdecl NiHalfSpaceBV_HalfSpaceSphereFindIntersect();
void __cdecl NiHalfSpaceBV_HalfSpaceBoxFindIntersect();
void __cdecl NiHalfSpaceBV_HalfSpaceCapsuleFindIntersect();
void __cdecl IntersectPlanes();
void __cdecl NiHalfSpaceBV_HalfSpaceTriFindIntersect();
void __cdecl NiHalfSpaceBV_CreateFromStream();
struct D_00e9e980;
bool __cdecl NiUnionBV_UnionOtherTestIntersect(float, D_00e9e980*, void*, void*, void*);
void __cdecl NiUnionBV_UnionUnionTestIntersect();
void __cdecl NiUnionBV_UnionTriTestIntersect();
void __cdecl NiUnionBV_UnionOtherFindIntersect();
void __cdecl NiUnionBV_UnionUnionFindIntersect();
void __cdecl NiUnionBV_UnionTriFindIntersect();
void __cdecl NiUnionBV_CreateFromStream();
void __cdecl NiCapsuleBV_CapsuleSphereFindIntersect();
void __cdecl NiCapsuleBV_CapsuleCapsuleFindIntersect();
void __cdecl SweepSphereAgainstCapsule();
void __cdecl IsCapsuleSweptHit();
void __cdecl CapsuleTriangleIntersect();
void __cdecl NiCapsuleBV_CapsuleTriFindIntersect();
void __cdecl NiCapsuleBV_CreateFromStream();
void __cdecl NiBoxBV_BoxBoxTestIntersect();
void __cdecl NiBoxBV_BoxCapsuleTestIntersect();
void __cdecl NiBoxBV_BoxTriTestIntersect();
void __cdecl NiBoxBV_BoxSphereFindIntersect();
void __cdecl NiBoxBV_BoxCapsuleFindIntersect();
void __cdecl NiBoxBV_BoxTriFindIntersect();
void __cdecl IntersectsSphereAndBox();
void __cdecl NiBoxBV_BoxBoxFindIntersect();
void __cdecl CheckSweptSphereOverlap();
void __cdecl IntersectSpheres();
struct FUN_00eac170_Ret;
FUN_00eac170_Ret* __cdecl NiSphereBV_CreateFromStream(NiStream&);
void __cdecl SphereTriangleSweepTest();
void __cdecl SphereTriangleCollision();
void __cdecl ReadObjB0();
extern bool DAT_0126fa5c;
extern PFN DAT_0126fa7c;
extern PFN DAT_0126fa80;
extern PFN DAT_0126fa84;
extern PFN DAT_0126fa88;
extern PFN DAT_0126fa8c;
extern PFN DAT_0126fa90;
extern PFN* DAT_0126fa94;
extern PFN* NiBoundingVolume_ms_pfnFindFuncTable; // src: XBOX_RETAIL_PDB 0x0126fa98 (old: DAT_0126fa98)
extern PFN* NiBoundingVolume_ms_pfnTriTestFuncTable; // src: XBOX_RETAIL_PDB 0x0126fa9c (old: DAT_0126fa9c)
extern PFN* NiBoundingVolume_ms_pfnTriFindFuncTable; // src: XBOX_RETAIL_PDB 0x0126faa0 (old: DAT_0126faa0)
extern PFN DAT_0126faa4;
extern PFN DAT_0126faa8;
extern PFN DAT_0126faac;

// @fnv 0x00e98ff0
// pdb: NiCollisionSDM::Init | NiCollisionSDM.obj | retail, object-file run extension, low
// pdb locals: pFTF, uiSize, pFIF, pTIF, pTTF, uiSizeSqr
void __cdecl InitNiStreamCallbacks()
{
    if (DAT_0126fa5c)
        return;
    DAT_0126fa5c = true;
    NiStream::RegisterObjectLoader(g_0126fab4_arg0, Runtime_CreateNode);
    DAT_0126fa80 = NiRegisterBVStream;
    DAT_0126fa84 = NiCapsuleBV_CreateFromStream;
    DAT_0126fa90 = NiHalfSpaceBV_CreateFromStream;
    DAT_0126fa88 = ReadObjB0;
    DAT_0126fa7c = (PFN)NiSphereBV_CreateFromStream;
    DAT_0126fa8c = NiUnionBV_CreateFromStream;

    PFN* a = (PFN*)_NiMalloc(0x90);
    a[0] = CheckSweptSphereOverlap;
    a[1] = 0;
    a[2] = 0;
    a[3] = 0;
    a[4] = 0;
    a[5] = 0;
    a[6] = IntersectsSphereAndBox;
    a[7] = NiBoxBV_BoxBoxTestIntersect;
    a[8] = NiBoxBV_BoxCapsuleTestIntersect;
    a[9] = 0;
    a[10] = 0;
    a[11] = 0;
    a[12] = SweepSphereAgainstCapsule;
    a[13] = 0;
    a[14] = IsCapsuleSweptHit;
    a[15] = 0;
    a[16] = 0;
    a[17] = 0;
    a[18] = 0;
    a[19] = 0;
    a[20] = 0;
    a[21] = 0;
    a[22] = 0;
    a[23] = 0;
    a[24] = (PFN)NiUnionBV_UnionOtherTestIntersect;
    a[25] = (PFN)NiUnionBV_UnionOtherTestIntersect;
    a[26] = (PFN)NiUnionBV_UnionOtherTestIntersect;
    a[27] = 0;
    a[28] = NiUnionBV_UnionUnionTestIntersect;
    a[29] = (PFN)NiUnionBV_UnionOtherTestIntersect;
    a[30] = NiHalfSpaceBV_HalfSpaceSphereTestIntersect;
    a[31] = IsIntersectingFogPlane;
    a[32] = NiHalfSpaceBV_HalfSpaceCapsuleTestIntersect;
    a[33] = 0;
    a[34] = 0;
    a[35] = NiHalfSpaceBV_HalfSpaceHalfSpaceTestIntersect;

    PFN* b = (PFN*)_NiMalloc(0x90);
    b[0] = IntersectSpheres;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;
    b[4] = 0;
    b[5] = 0;
    b[6] = NiBoxBV_BoxSphereFindIntersect;
    b[7] = NiBoxBV_BoxBoxFindIntersect;
    b[8] = NiBoxBV_BoxCapsuleFindIntersect;
    b[9] = 0;
    b[10] = 0;
    b[11] = 0;
    b[12] = NiCapsuleBV_CapsuleSphereFindIntersect;
    b[13] = 0;
    b[14] = NiCapsuleBV_CapsuleCapsuleFindIntersect;
    b[15] = 0;
    b[16] = 0;
    b[17] = 0;
    b[18] = 0;
    b[19] = 0;
    b[20] = 0;
    b[21] = 0;
    b[22] = 0;
    b[23] = 0;
    b[24] = NiUnionBV_UnionOtherFindIntersect;
    b[25] = NiUnionBV_UnionOtherFindIntersect;
    b[26] = NiUnionBV_UnionOtherFindIntersect;
    b[27] = NiUnionBV_UnionOtherFindIntersect;
    b[28] = NiUnionBV_UnionUnionFindIntersect;
    b[29] = NiUnionBV_UnionOtherFindIntersect;
    b[30] = NiHalfSpaceBV_HalfSpaceSphereFindIntersect;
    b[31] = NiHalfSpaceBV_HalfSpaceBoxFindIntersect;
    b[32] = NiHalfSpaceBV_HalfSpaceCapsuleFindIntersect;
    b[33] = 0;
    b[34] = 0;
    b[35] = IntersectPlanes;

    PFN* pTableC = (PFN*)_NiMalloc(0x18);
    pTableC[0] = SphereTriangleSweepTest;
    pTableC[1] = NiBoxBV_BoxTriTestIntersect;
    pTableC[2] = CapsuleTriangleIntersect;
    pTableC[3] = 0;
    pTableC[4] = NiUnionBV_UnionTriTestIntersect;
    pTableC[5] = NiHalfSpaceBV_HalfSpaceTriTestIntersect;

    PFN* pTableD = (PFN*)_NiMalloc(0x18);
    pTableD[0] = SphereTriangleCollision;
    pTableD[1] = NiBoxBV_BoxTriFindIntersect;
    pTableD[2] = NiCapsuleBV_CapsuleTriFindIntersect;
    pTableD[3] = 0;
    pTableD[4] = NiUnionBV_UnionTriFindIntersect;
    pTableD[5] = NiHalfSpaceBV_HalfSpaceTriFindIntersect;

    DAT_0126fa94 = a;
    NiBoundingVolume_ms_pfnFindFuncTable = b;
    NiBoundingVolume_ms_pfnTriTestFuncTable = pTableC;
    NiBoundingVolume_ms_pfnTriFindFuncTable = pTableD;
    DAT_0126faa4 = (PFN)NiCollisionConvert_ConvertToSphereBV_Fast;
    DAT_0126faa8 = (PFN)NiCollisionConvert_ConvertToSphereBV_Safe;
    DAT_0126faac = NiCollisionConvert_ConvertToBoxBV;
}

struct NiStreamUnreg
{
    static void UnregisterObjectLoader(const char*);
};
// @fnv 0x00e99260
// src: COMMUNITY 0x00e99260
// pdb: NiCollisionSDM::Shutdown | NiCollisionSDM.obj | retail, upstream name already matches, high
void __cdecl NiCollisionSDM_Shutdown()
{
    if (DAT_0126fa5c)
    {
        DAT_0126fa5c = false;
        NiStreamUnreg::UnregisterObjectLoader(g_0126fab4_arg0);
        DAT_0126fa80 = 0;
        DAT_0126fa84 = 0;
        DAT_0126fa90 = 0;
        DAT_0126fa88 = 0;
        DAT_0126fa7c = 0;
        DAT_0126fa8c = 0;
        NiFree(DAT_0126fa94);
        NiFree(NiBoundingVolume_ms_pfnFindFuncTable);
        NiFree(NiBoundingVolume_ms_pfnTriTestFuncTable);
        NiFree(NiBoundingVolume_ms_pfnTriFindFuncTable);
        DAT_0126fa94 = 0;
        NiBoundingVolume_ms_pfnFindFuncTable = 0;
        NiBoundingVolume_ms_pfnTriTestFuncTable = 0;
        NiBoundingVolume_ms_pfnTriFindFuncTable = 0;
        DAT_0126faa4 = 0;
        DAT_0126faa8 = 0;
        DAT_0126faac = 0;
    }
}

// @fnv 0x00e98be0
// src: XBOX_RETAIL_PDB 0x00e98be0
// pdb: NiPick::Results::FindClosest | NiPick.obj | retail, call graph, high
// pdb locals: pRecordClosest, uiClosest, i, pRecordTest, bTestIsCloser
void NiPick::Results::FindClosest()
{
    if (m_uiValidCount)
    {
        Record* pFirst = m_pBase[0];
        Record* pLowestPriority = m_pBase[0];
        unsigned int uiLowestIndex = 0;
        for (unsigned int i = uiLowestIndex + 1; i < m_uiValidCount; i++)
        {
            if (m_pBase[i]->m_fDistance < pLowestPriority->m_fDistance)
            {
                pLowestPriority = m_pBase[i];
                uiLowestIndex = i;
            }
        }
        Record* pFirstElement = pFirst;
        typedef NiTArray<Record*, NiTMallocInterface<Record*> > RecordArray;
        RecordArray* array = reinterpret_cast<RecordArray*>(this);
        array->SetAt(uiLowestIndex, pFirstElement);
        array->SetAt(0, pLowestPriority);
        m_uiValidCount = 1;
    }
}

inline NiColorA::NiColorA(float fR, float fG, float fB, float fA) : r(fR), g(fG), b(fB), a(fA)
{
}

// Pick-record storage: 0x44 bytes, with SetObject at 0x00e98aa0 (nitarray.cpp), unlike the 0x10-byte ImageSpaceTexture.
class PooledPickRecord
{
  public:
    int m_unk00;
    int m_unk04;
    int unk08, unk0C, unk10, unk14, unk18, unk1C, unk20, unk24, unk28, unk2C;
    int unk30;
    NiColorA m_kDiffuseColor;

    PooledPickRecord(int iParam) : m_unk00(0), m_unk04(0)
    {
        SetObject(iParam);
    }
    void SetObject(int iParam);

    static void* operator new(size_t uiSize)
    {
        return g_memoryManager.Game_DoHeapAlloc(uiSize);
    }
    static void operator delete(void*, int, int);
};

class PickRecordPool
{
  public:
    virtual ~PickRecordPool();
    PooledPickRecord** m_pBase;
    unsigned short m_usMaxSize;
    unsigned short m_usSize;
    unsigned short m_usESize;
    unsigned short m_usGrowBy;
    unsigned int m_uiNext;

    void FUN_00a90180(unsigned int uiMaxSize);
    void FUN_00ba3aa0(unsigned int uiIndex, PooledPickRecord* const& pkElement);

    unsigned int Add(PooledPickRecord* pkElement)
    {
        unsigned int uiIndex = m_usSize;
        if (uiIndex >= m_usMaxSize)
            FUN_00a90180(uiIndex + m_usGrowBy);
        FUN_00ba3aa0(uiIndex, pkElement);
        return uiIndex;
    }

    PooledPickRecord* GetTexture(int pObject);
};

// Returns the next PooledPickRecord from the pool, reusing an existing slot or allocating a new one when the pool is exhausted.
// @fnv 0x00e99330
// pdb: NiPick::Results::AddNew | NiCollisionTraversals.obj | retail, call graph (shared callers), high
// pdb params: pObject
// pdb locals: pkRecord
PooledPickRecord* PickRecordPool::GetTexture(int pObject)
{
    if (m_uiNext < m_usSize)
    {
        PooledPickRecord* pkTexture = m_pBase[m_uiNext];
        pkTexture->SetObject(pObject);
        m_uiNext++;
        return pkTexture;
    }

    PooledPickRecord* pkTexture = new PooledPickRecord(pObject);
    Add(pkTexture);
    m_uiNext++;
    return pkTexture;
}
