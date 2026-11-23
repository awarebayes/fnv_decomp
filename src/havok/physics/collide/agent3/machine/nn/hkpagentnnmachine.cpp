
// src/havok/physics/collide/agent3/machine/nn/hkpagentnnmachine.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/collide/agent3/machine/nn/hkpagentnnmachine.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

// Keep the unit's original intrinsic declarations; the SDK SIMD union changes /GS classification.
// Exe 0x00cfe8a0 FUN_00cfe8a0: retain this LocalM128 view; the CRT type does not preserve this unit.
// VC9 requires the __m128 compatibility typedef for its native SIMD ABI (renamed tags fail C2719).
typedef union __declspec(intrin_type) __declspec(align(16)) {
    float m128_f32[4];
    unsigned __int64 m128_u64[2];
} __m128, LocalM128;
extern "C" LocalM128 _mm_setzero_ps(void);
#define FNV_TRANSFORM_NO_SSE
#include "../../../../../../../include/Transform.h"
#undef FNV_TRANSFORM_NO_SSE
#include "../../../../../../../include/hkMemoryAllocator.h"
#include "../../../../../../../include/hkpCollisionDispatcher.h"
#include "../../../../../../../include/hkpLinkedCollidable.h"
#include "../../../../../../../include/hkpAgentNnTrack.h"

// The shared collidable stores its +0x50 collision array as bytes; its records are CollisionKeyEntry.

// Havok's TLS router at 0x00c85750 exposes heap at +0x10; no include/hkMemoryRouter.h exists yet.
#include "../../../../../../../include/hkMemoryRouter.h"
#include "../../../../../../../include/hkpAgentNnEntry.h"

// Searches the smaller node's entry list for an entry whose key equals the other node and returns its value, else 0.
// @fnv 0x00cfdd70
void* __cdecl FindEntryValue(hkpLinkedCollidable* apFirst, hkpLinkedCollidable* apSecond)
{
    hkpLinkedCollidable* pSmaller;
    hkpLinkedCollidable* pOther;

    if (apFirst->m_collisionEntries.m_iElementCount <= apSecond->m_collisionEntries.m_iElementCount)
    {
        pSmaller = apFirst;
        pOther = apSecond;
    }
    else
    {
        pSmaller = apSecond;
        pOther = apFirst;
    }

    int iEntryCount = pSmaller->m_collisionEntries.m_iElementCount;
    for (int i = 0; i < iEntryCount; i++)
    {
        if (reinterpret_cast<const hkpLinkedCollidable::CollisionKeyEntry*>(pSmaller->m_collisionEntries.m_data)[i].m_pCollidable == pOther)
            return reinterpret_cast<const hkpLinkedCollidable::CollisionKeyEntry*>(pSmaller->m_collisionEntries.m_data)[i].m_pAgentEntry;
    }
    return 0;
}

// @fnv 0x00cfddc0
// src: HAVOK_LIB 0x00cfddc0
void _hkAgentNnMachine_AssertTrackValidity(hkpAgentNnTrack* pArg)
{
    int i = 0;
    if (pArg->m_sectors.m_iElementCount > i)
    {
        hkpAgentNnSector** aRecords = pArg->m_sectors.m_data;
        volatile int* pCount = &pArg->m_sectors.m_iElementCount;  // re-read the record count
        int iCount = *pCount;
        do
        {
            unsigned char* pPos = (unsigned char*)*aRecords++;
            i++;
            unsigned char* pLimit;
            if (i == iCount)
                pLimit = (unsigned char*)(pArg->m_bytesUsedInLastSector + (int)pPos);
            else
                pLimit = pPos + 0x200;
            while (pPos < pLimit)
                pPos += pPos[3];
        } while (i < iCount);
    }
}

// GetNodePairValue (0x00cfdf60, 184 bytes)
//
// Two "grid" objects are looked up in a 32-column byte table that is held by a
// context object:
//
//   * ppA / ppB point at objects whose +0x0c member is a row/column index,
//   * pCtx->unk00 is the base of the table, pCtx->unk14 selects one of two
//     parallel tables (offset 0x12b0 when it is set, 0xeb0 when it is clear),
//   * pOutVal receives the table byte of the (transposed) second lookup,
//   * pOutFlag receives 1 when the table entry describes a usable value.

struct FnvNode
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
};

struct FnvCtx
{
    unsigned char* unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
};

// @fnv 0x00cfdf60
// pdb: hkAgentNnMachine_IsEntryOnTrack | hkpAgentNnMachine.obj | debug, object-file run extension, low
// pdb params: track, entry
void GetNodePairValue(FnvNode** ppA, FnvNode** ppB, FnvCtx* pCtx, int* pOutVal, int* pOutFlag)
{
    FnvNode* pNodeA = *ppA;
    FnvNode* pNodeB = *ppB;
    FnvCtx* pCtxLocal = pCtx;
    unsigned char* pucBase = pCtxLocal->unk00;
    int iIndexA = pNodeA->unk0c;
    int iIndexB = pNodeB->unk0c;

    int iIndex = iIndexA * 32 + iIndexB;
    int iValue = pCtx->unk14 ? (int)pucBase[0x12b0 + iIndex] : (int)pucBase[0xeb0 + iIndex];

    int bOk = (*(int*)(pucBase + 0x16f0 + iValue * 80) == 2);
    if (iValue == 1)
    {
        int nQ = pucBase[(iIndexA + 13) * 32 + iIndexB];
        int nR = nQ + 0x7c;
        nR *= 5;
        bOk = (*(volatile unsigned char*)&pucBase[nR * 4] != 0);
    }

    int iResult = iValue;
    *pOutFlag = bOk;
    if (bOk)
    {
        int iIndexSwapped = iIndexB * 32 + iIndexA;
        iResult = pCtx->unk14 ? (int)pucBase[0x12b0 + iIndexSwapped] : (int)pucBase[0xeb0 + iIndexSwapped];
    }
    *pOutVal = iResult;
}

extern "C" hkMemoryRouter& __cdecl hkMemoryRouter_getInstance();
void __cdecl ResizeAudioStream(void* mem, void* array, int sizeElem);

static __forceinline void ConstructSectorPointers(hkpAgentNnSector** p, int n, hkpAgentNnSector* const& e)
{
    for (int i = 0; i < n; i++)
        p[i] = e;
}

template <> __forceinline void hkArrayBase<hkpAgentNnSector*>::_pushBack(hkMemoryAllocator& alloc, hkpAgentNnSector* const& e)
{
    if (m_iElementCount == getCapacity())
        ResizeAudioStream(&alloc, this, sizeof(hkpAgentNnSector*));
    ConstructSectorPointers(m_data + m_iElementCount, 1, e);
    m_iElementCount++;
}

// Returns a pointer into the pool's current block at the pool offset, advancing it by 0x80, or allocates a new 0x200-byte block when the offset reaches 0x200.
// @fnv 0x00cfe820
// src: XBOX_RETAIL_PDB 0x00cfe820
// pdb: hkAgentNnMachine_AllocateEntry | hkpAgentNnMachine.obj | retail, call graph (shared callers), high
// pdb params: track, forcePpuEntry
// pdb locals: sector
static __declspec(noinline) char* hkAgentNnMachine_AllocateEntry(hkpAgentNnTrack* pool)
{
    if (pool->m_bytesUsedInLastSector < 0x200)
    {
        char* p = (char*)pool->m_sectors.m_data[pool->m_sectors.m_iElementCount - 1] + pool->m_bytesUsedInLastSector;
        pool->m_bytesUsedInLastSector += 0x80;
        return p;
    }
    char* block = static_cast<char*>(hkMemoryRouter_getInstance().heap().blockAlloc(0x200));
    pool->m_sectors.pushBack((hkpAgentNnSector*)block);
    pool->m_bytesUsedInLastSector = 0x80;
    return block;
}

// Caller so the file-static helper is emitted with its register calling convention.
char* CFE820Allocate(hkpAgentNnTrack* pool)
{
    char* a = hkAgentNnMachine_AllocateEntry(pool);
    char* b = hkAgentNnMachine_AllocateEntry(pool);
    return a < b ? a : b;
}

// FUN_00cfeba0 - pick a per-shape handler out of a table and run it.
//
// The shape object carries a type byte and an index byte.  Depending on the
// type the function either jumps straight to a per-index callback or first
// evaluates two transform chains (FUN_00cfe1d0) and then re-dispatches with a
// masked type byte.

typedef void(__cdecl* FUN_00cfeba0_Fn)(void*, void*, void*, void*, void*, void*, void*);

struct FUN_00cfeba0_Obj
{
    unsigned char m_type;         // +0x00 shape type
    unsigned char m_uiEdgeIndex;  // +0x01 handler index
    int unk04;
    int unk08;  // +0x08
    int unk0c;
    void* m_p10;  // +0x10
    void* m_p14;  // +0x14
    int unk18;
    int unk1c;
    int unk20;  // +0x20
    int unk24;
    int unk28;
    int unk2c;
    int unk30;  // +0x30
};

struct FUN_00cfeba0_Result
{
    void* unk00;
    int unk04;
    void* unk08;
    void* unk0c;
};

struct __declspec(align(16)) FUN_00cfeba0_Node
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
    int unk68;
    int unk6c;
    int unk70;
    int unk74;
    int unk78;
    int unk7c;
    int unk80;
    int unk84;
    int unk88;
    int unk8c;
    int unk90;
    int unk94;
    int unk98;
    int unk9c;
    int unka0;
    int unka4;
    int unka8;
    int unkac;
};

void* __cdecl hkAgentMachine_processTransformedShapes(void* cdBody, void* newCdBodies, void* newMotionStates, unsigned int numSlots, void* cdBodyHasTransformFlag);

// @fnv 0x00cfeba0
// src: HAVOK_LIB 0x00cfeba0
// pdb: hkAgentNnMachine_UpdateShapeCollectionFilter | hkpAgentNnMachine.obj | retail, call graph, high
// pdb params: entry, input, constraintOwner
// pdb locals: modBodyB, modMotionB, modBodyA, modMotionA, dummyCdBodyHasTransformFlag, assertBuf, ostr
void __cdecl _hkAgentNnMachine_UpdateShapeCollectionFilter(FUN_00cfeba0_Obj* entry, void** input, void* constraintOwner)
{
    unsigned char flag;
    void* p = 0;
    FUN_00cfeba0_Result results[8];
    FUN_00cfeba0_Node elems[8];
    int type = entry->m_type;
    void* h1 = entry->m_p10;
    void* h2 = entry->m_p14;

    for (;;)
    {
        switch (type - 2)
        {
        case 0:
        case 4:
            p = (char*)entry + 0x20;
            break;
        case 2:
            p = (char*)entry + 0x30;
            break;
        case 8:
        case 10:
        case 12:
            h1 = hkAgentMachine_processTransformedShapes(h1, &results[0], &elems[0], 4, &flag);
            h2 = hkAgentMachine_processTransformedShapes(h2, &results[4], &elems[4], 4, &flag);
            type = (unsigned int)((unsigned char)type) & (unsigned int)0xf7;
            continue;
        default:
            return;
        }

        {
            FUN_00cfeba0_Fn fn = *(FUN_00cfeba0_Fn*)((char*)(*input) + 0x16c8 + entry->m_uiEdgeIndex * 80);
            if (fn)
            {
                fn(entry, p, h1, h2, input, (void*)entry->unk08, constraintOwner);
            }
        }
        return;
    }
}

namespace fnv_00cfeca0 {
extern "C" void _ReadWriteBarrier();
struct Record
{
    unsigned word00;
    unsigned word04;
    unsigned word08;
    unsigned word0c;
    unsigned word10;
    unsigned word14;
    unsigned word18;
    unsigned word1c;
    unsigned word20;
    unsigned word24;
    unsigned word28;
    unsigned word2c;
    unsigned word30;
    unsigned word34;
    unsigned word38;
    unsigned word3c;
    unsigned word40;
    unsigned word44;
    unsigned word48;
    unsigned word4c;
    unsigned word50;
    unsigned word54;
    unsigned word58;
    unsigned word5c;
    unsigned word60;
    unsigned word64;
    unsigned word68;
    unsigned word6c;
    unsigned word70;
    unsigned word74;
    unsigned word78;
    unsigned word7c;
};
struct DataBufferInfo
{
    Record* record;
    unsigned other;
};
extern "C" hkMemoryRouter& __cdecl hkMemoryRouter_getInstance();
void ResizeAudioStream(void*, void*, int);
struct SlabArray
{
    char** data;
    int count;
    unsigned capacityFlags;
    void Append(char* block)
    {
        hkMemoryRouter& router = hkMemoryRouter_getInstance();
        if (count == (capacityFlags & 0x3fffffffu))
            ::ResizeAudioStream(&router.heap(), this, 4);
        data[count] = block;
        ++count;
    }
};
struct State
{
    unsigned used;
    SlabArray slabs;
};
__declspec(noinline) static Record* hkAgentNnMachine_AllocateEntry(State* state) // src: XBOX_RETAIL_PDB 0x00cfe820
{
    if (state->used < 512u)
    {
        char* result = state->slabs.data[state->slabs.count - 1] + state->used;
        state->used += 128;
        return reinterpret_cast<Record*>(result);
    }
    hkMemoryRouter& router = hkMemoryRouter_getInstance();
    hkMemoryAllocator* allocator = &router.heap();
    char* block = static_cast<char*>(allocator->blockAlloc(512));
    state->slabs.Append(block);
    state->used = 128;
    return reinterpret_cast<Record*>(block);
}
// Allocates a Record from state, copies all 32 words from source, then registers the new record in two tables indexed by the halves of word04.
// @fnv 0x00cfeca0
// src: XBOX_RETAIL_PDB 0x00cfeca0
// pdb: hkAgentNnMachine_CopyAndRelinkAgentEntry | hkpAgentNnMachine.obj | retail, call graph, high
// pdb params: destTrack, entry
Record* hkAgentNnMachine_CopyAndRelinkAgentEntry(State* destTrack, const Record* entry)
{
    Record* record = hkAgentNnMachine_AllocateEntry(destTrack);
    record->word00 = entry->word00;
    record->word04 = entry->word04;
    record->word08 = entry->word08;
    record->word0c = entry->word0c;
    record->word10 = entry->word10;
    record->word14 = entry->word14;
    record->word18 = entry->word18;
    record->word1c = entry->word1c;
    record->word20 = entry->word20;
    record->word24 = entry->word24;
    record->word28 = entry->word28;
    record->word2c = entry->word2c;
    record->word30 = entry->word30;
    record->word34 = entry->word34;
    record->word38 = entry->word38;
    record->word3c = entry->word3c;
    record->word40 = entry->word40;
    record->word44 = entry->word44;
    record->word48 = entry->word48;
    record->word4c = entry->word4c;
    record->word50 = entry->word50;
    record->word54 = entry->word54;
    record->word58 = entry->word58;
    record->word5c = entry->word5c;
    record->word60 = entry->word60;
    record->word64 = entry->word64;
    record->word68 = entry->word68;
    record->word6c = entry->word6c;
    record->word70 = entry->word70;
    record->word74 = entry->word74;
    record->word78 = entry->word78;
    record->word7c = entry->word7c;
    _ReadWriteBarrier();
    unsigned short indexA = static_cast<unsigned short>(record->word04);
    unsigned short indexB = static_cast<unsigned short>(record->word04 >> 16);
    DataBufferInfo* tableA = *reinterpret_cast<DataBufferInfo**>(record->word10 + 0x50);
    tableA[indexA].record = record;
    DataBufferInfo* tableB = *reinterpret_cast<DataBufferInfo**>(record->word14 + 0x50);
    tableB[indexB].record = record;
    return record;
}
}  // namespace fnv_00cfeca0

struct ItemStack
{
    int m_base;
    int* m_pItems;
    int m_uiItemCount;
};

void __cdecl hkAgentNnMachine_DestroyAgent(ItemStack* s, int value, int dispatcher, int b);

// @fnv 0x00cfed90
// src: HAVOK_LIB 0x00cfed90
// pdb: hkAgentNnMachine_DestroyTrack | hkpAgentNnMachine.obj | retail, upstream name already matches, high
// pdb params: track, dispatcher, constraintOwner
void __cdecl hkAgentNnMachine_DestroyTrack(ItemStack* track, int dispatcher, int constraintOwner)
{
    if (track->m_uiItemCount != 0)
    {
        do
        {
            int iIndex = *(volatile int*)&track->m_uiItemCount;
            int iItemValue = track->m_pItems[iIndex - 1];
            hkAgentNnMachine_DestroyAgent(track, iItemValue + track->m_base - 0x80, dispatcher, constraintOwner);
        } while (track->m_uiItemCount != 0);
    }
}

extern const float DAT_01012054;

struct FUN_00cfdc90_Obj
{
    unsigned char type;
    unsigned char slot;
    short unk02;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    float unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
};

typedef void(__cdecl* FUN_00cfdc90_Fn)(FUN_00cfdc90_Obj* self, void* p, float a, float b, void* ctx);

// Looks up a per-slot callback from ctx and invokes it with obj, a payload pointer, x and y; for types 4 and 12 it also updates obj->unk18 from x/y.
// @fnv 0x00cfdc90
// pdb: hkAgentNnMachine_WarpTimeInAgent | hkpAgentNnMachine.obj | debug, object-file run extension, low
// pdb params: entry, oldTime, newTime, input
// pdb locals: assertBuf, ostr
void __cdecl DispatchSlotCallback(FUN_00cfdc90_Obj* obj, float x, float y, void* ctx)
{
    switch (obj->type - 2)
    {
    case 0:
    case 4:
    case 8:
    case 12: {
        FUN_00cfdc90_Fn fn = *(FUN_00cfdc90_Fn*)(*(char**)ctx + obj->slot * 80 + 0x16d4);
        if (fn != 0)
        {
            fn(obj, &obj->unk20, x, y, ctx);
        }
    }
    break;
    case 2:
    case 10: {
        void* p = &obj->unk30;
        float prev = obj->unk18;
        if (prev == x)
        {
            obj->unk18 = y;
        }
        else
        {
            obj->unk18 = DAT_01012054;
        }
        FUN_00cfdc90_Fn fn = *(FUN_00cfdc90_Fn*)(*(char**)ctx + obj->slot * 80 + 0x16d4);
        if (fn != 0)
        {
            fn(obj, p, x, y, ctx);
        }
    }
    break;
    }
}

__forceinline hkAgent3::DestroyFunc hkpCollisionDispatcher::getAgent3DestroyFunc(int agentType)
{
    return m_kAgent3FunctionTable[agentType].m_pAgent3DestroyFuncs;
}

// Agent header +0x00..0x1f, used by 0x00cfea30; no include/hkpAgentNnEntry.h exists yet.

template <int N> static __forceinline void memCpyOneAlignedCFEA(void* dst, const void* src)
{
    int i = 0;
    do
    {
        ((unsigned int*)dst)[i] = ((const unsigned int*)src)[i];
    } while (++i < N / 4);
}

static __forceinline int hkNextPowerOf2CFEA(unsigned int in)
{
    in -= 1;
    in |= in >> 16;
    in |= in >> 8;
    in |= in >> 4;
    in |= in >> 2;
    in |= in >> 1;
    return in + 1;
}

void __cdecl hkArrayUtil__reduce(void* mem, void* array, int sizeElem, void* inplaceMem, int newCapacity);

template <> __forceinline void hkArrayBase<hkpLinkedCollidable::CollisionKeyEntry>::removeAt(int index)
{
    m_iElementCount--;
    if (m_iElementCount != index)
        memCpyOneAlignedCFEA<sizeof(hkpLinkedCollidable::CollisionKeyEntry)>(m_data + index, m_data + m_iElementCount);
}

template <> __forceinline void hkArrayBase<hkpLinkedCollidable::CollisionKeyEntry>::_optimizeCapacity(hkMemoryAllocator& alloc, int numFreeElemsLeft, int)
{
    int totalSize = hkNextPowerOf2CFEA(m_iElementCount + numFreeElemsLeft);
    if (totalSize < getCapacity())
        hkArrayUtil__reduce(&alloc, this, sizeof(hkpLinkedCollidable::CollisionKeyEntry), 0, totalSize);
}

void __cdecl _hkAgentNnMachine_InternalDeallocateEntry(void* track, hkpAgentNnEntry* entry);

// @fnv 0x00cfea30
// src: XBOX_PDB 0x00cfea30
// pdb: hkAgentNnMachine_DestroyAgent | hkpAgentNnMachine.obj | retail, upstream name already matches, high
// pdb params: track, entry, dispatcher, constraintOwner
// pdb locals: assertBuf, ostr
void __cdecl hkAgentNnMachine_DestroyAgent(void* track, hkpAgentNnEntry* entry, hkpCollisionDispatcher* dispatcher,
                          void* constraintOwner)
{
    switch (entry->m_ucAgentType)
    {
    case 2:
    case 6:
    case 10:
    case 14: {
        hkAgent3::DestroyFunc pDestroyFunc = dispatcher->getAgent3DestroyFunc(entry->m_ucNnTrackType);
        pDestroyFunc((FourByteState*)entry, (hkpAgentData*)((char*)entry + 0x20),
                     (hkpContactMgr*)entry->m_contactMgr, *(hkpConstraintOwner*)constraintOwner, dispatcher);
        break;
    }
    case 4:
    case 12: {
        hkAgent3::DestroyFunc pDestroyFunc = dispatcher->getAgent3DestroyFunc(entry->m_ucNnTrackType);
        pDestroyFunc((FourByteState*)entry, (hkpAgentData*)((char*)entry + 0x30),
                     (hkpContactMgr*)entry->m_contactMgr, *(hkpConstraintOwner*)constraintOwner, dispatcher);
        break;
    }
    default:
        break;
    }

    for (int i = 0; i < 2; i++)
    {
        hkpLinkedCollidable* collidable = entry->m_collidable[i];
        int index = entry->m_agentIndexOnCollidable[i];
        hkArray<hkpLinkedCollidable::CollisionKeyEntry>& entries =
            reinterpret_cast<hkArray<hkpLinkedCollidable::CollisionKeyEntry>&>(collidable->m_collisionEntries);
        entries.removeAt(index);
        if (index < collidable->m_collisionEntries.m_iElementCount)
        {
            hkpAgentNnEntry* pMovedEntry = (hkpAgentNnEntry*)entries.m_data[index].m_pAgentEntry;
            pMovedEntry->m_agentIndexOnCollidable[pMovedEntry->m_collidable[0] != collidable] = (unsigned short)index;
        }
        entries._optimizeCapacity(hkMemoryRouter_getInstance().heap(), 4);
    }

    _hkAgentNnMachine_InternalDeallocateEntry(track, entry);
}

static __forceinline int hkNextPowerOf2CFF2(unsigned int in)
{
    in -= 1;
    in |= in >> 16;
    in |= in >> 8;
    in |= in >> 4;
    in |= in >> 2;
    in |= in >> 1;
    return in + 1;
}

void __cdecl hkArrayUtil__reduce(void* mem, void* array, int sizeElem, void* inplaceMem, int newCapacity);

template <> __forceinline void hkArrayBase<hkpAgentNnSector*>::popBack(int iNumElemsToRemove)
{
    m_iElementCount -= iNumElemsToRemove;
}

template <> __forceinline void hkInplaceArray<hkpAgentNnSector*, 1>::optimizeCapacity(int numFreeElemsLeft, int)
{
    if ((m_iCapacityState & 0x80000000) == 0)
    {
        int newCapacity = hkNextPowerOf2CFF2(m_iElementCount + numFreeElemsLeft);
        if (newCapacity < 1)
        {
            hkArrayUtil__reduce(&hkMemoryRouter_getInstance().heap(), this, sizeof(hkpAgentNnSector*), m_storage, 1);
        }
        else if (newCapacity < getCapacity())
        {
            hkArrayUtil__reduce(&hkMemoryRouter_getInstance().heap(), this, sizeof(hkpAgentNnSector*), 0, newCapacity);
        }
    }
}

void __cdecl hkAgentNnMachine_CopyAndRelinkAgentEntry(hkpAgentNnTrack* destTrack, hkpAgentNnEntry* entry);

// Clones records from the end of appendee into track while track's last sector is not full, then inserts appendee's remaining sectors into track's sector array.
// @fnv 0x00cff290
// src: HAVOK_LIB 0x00cff290
// pdb: hkAgentNnMachine_AppendTrack | hkpAgentNnMachine.obj | retail, call graph, high
// pdb params: track, appendee
void __cdecl _hkAgentNnMachine_AppendTrack(hkpAgentNnTrack* track, hkpAgentNnTrack* appendee)
{
    while (track->m_bytesUsedInLastSector != 0x200)
    {
        if (appendee->m_sectors.GetLogicalSize() == 0)
        {
            break;
        }
        hkpAgentNnEntry* entry =
            (hkpAgentNnEntry*)((char*)appendee->m_sectors.back() + appendee->m_bytesUsedInLastSector - 0x80);
        hkAgentNnMachine_CopyAndRelinkAgentEntry(track, entry);
        appendee->m_bytesUsedInLastSector = appendee->m_bytesUsedInLastSector - 0x80;
        if (appendee->m_bytesUsedInLastSector == 0)
        {
            hkpAgentNnSector* sector = appendee->m_sectors.back();
            appendee->m_sectors.popBack();
            hkMemoryRouter_getInstance().heap().blockFree(sector, 0x200);
            appendee->m_bytesUsedInLastSector = 0x200;
        }
        appendee->m_sectors.optimizeCapacity(4);
    }

    if (appendee->m_sectors.GetLogicalSize())
    {
        hkpAgentNnSector** sectors = appendee->m_sectors.m_data;
        track->m_sectors._insertAt(hkMemoryRouter_getInstance().heap(), track->m_sectors.GetLogicalSize(), sectors,
                                 appendee->m_sectors.GetLogicalSize());
        track->m_bytesUsedInLastSector = appendee->m_bytesUsedInLastSector;
        appendee->m_sectors.m_iElementCount = 0;
        appendee->m_bytesUsedInLastSector = 0x200;
    }
}

extern "C" void _ReadWriteBarrier(void);
struct S_cfe020_Entry
{
    int unk00;
    union
    {
        int unk04;
        struct
        {
            unsigned short idxA;
            unsigned short idxB;
        };
    };
    int unk08;
    int unk0c;
    hkpLinkedCollidable* ownerA;
    hkpLinkedCollidable* ownerB;
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
    int unk68;
    int unk6c;
    int unk70;
    int unk74;
    int unk78;
    volatile int unk7c;
    __forceinline S_cfe020_Entry& operator=(const S_cfe020_Entry& o)
    {
        unk00 = o.unk00;
        unk04 = o.unk04;
        unk08 = o.unk08;
        unk0c = o.unk0c;
        ownerA = o.ownerA;
        ownerB = o.ownerB;
        unk18 = o.unk18;
        unk1c = o.unk1c;
        unk20 = o.unk20;
        unk24 = o.unk24;
        unk28 = o.unk28;
        unk2c = o.unk2c;
        unk30 = o.unk30;
        unk34 = o.unk34;
        unk38 = o.unk38;
        unk3c = o.unk3c;
        unk40 = o.unk40;
        unk44 = o.unk44;
        unk48 = o.unk48;
        unk4c = o.unk4c;
        unk50 = o.unk50;
        unk54 = o.unk54;
        unk58 = o.unk58;
        unk5c = o.unk5c;
        unk60 = o.unk60;
        unk64 = o.unk64;
        unk68 = o.unk68;
        unk6c = o.unk6c;
        unk70 = o.unk70;
        unk74 = o.unk74;
        unk78 = o.unk78;
        unk7c = o.unk7c;
        return *this;
    }
};

void __cdecl hkArrayUtil__reduce(hkMemoryAllocator* mem, void* arr, int sizeElem, void* inplaceMem, int n);

// Pops an S_cfe020_Entry from the pool's stack, copies it to dest, updates the owner slot pointers, and releases the block when the pool empties.
// @fnv 0x00cfe020
// src: HAVOK_LIB 0x00cfe020
// pdb: hkAgentNnMachine_InternalDeallocateEntry | hkpAgentNnMachine.obj | retail, call graph, high
// pdb params: track, entry
// pdb locals: assertBuf, ostr
void _hkAgentNnMachine_InternalDeallocateEntry(hkpAgentNnTrack* track, S_cfe020_Entry* entry)
{
    S_cfe020_Entry* src = (S_cfe020_Entry*)((char*)track->m_sectors.back() + track->m_bytesUsedInLastSector - 0x80);
    if (entry != src)
    {
        *entry = *src;
        _ReadWriteBarrier();
        unsigned short a = entry->idxA;
        unsigned short b = entry->idxB;
        reinterpret_cast<hkpLinkedCollidable::CollisionKeyEntry*>(entry->ownerA->m_collisionEntries.m_data)[a].m_pAgentEntry = (NarrowPhaseAgentEntry*)entry;
        reinterpret_cast<hkpLinkedCollidable::CollisionKeyEntry*>(entry->ownerB->m_collisionEntries.m_data)[b].m_pAgentEntry = (NarrowPhaseAgentEntry*)entry;
    }
    track->m_bytesUsedInLastSector -= 0x80;
    if (track->m_bytesUsedInLastSector == 0)
    {
        hkpAgentNnSector* block = track->m_sectors.back();
        track->m_sectors.popBack();
        ((hkMemoryAllocator*)&hkMemoryRouter_getInstance().heap())->blockFree(block, 0x200);
        track->m_bytesUsedInLastSector = 0x200;
    }
    track->m_sectors.optimizeCapacity(4);
}

__forceinline hkMemoryAllocator& hkContainerHeapAllocator::get(const void*)
{
    return hkMemoryRouter_getInstance().heap();
}

template <> __forceinline hkpLinkedCollidable::CollisionKeyEntry&
hkArrayBase<hkpLinkedCollidable::CollisionKeyEntry>::_expandOne(hkMemoryAllocator& alloc)
{
    if (m_iElementCount == getCapacity())
        ResizeAudioStream(&alloc, this, sizeof(hkpLinkedCollidable::CollisionKeyEntry));
    hkpLinkedCollidable::CollisionKeyEntry* ret = &m_data[m_iElementCount];
    m_iElementCount++;
    return *ret;
}

struct CollidableBody
{
    int unk00;
    int unk04;
    const Transform* m_kMotionTransform;
};
struct CollisionAgentEntry;
struct NnAgentInputLocalForm
{ // local form of hkpAgent3Input: 0x00cfe8a0 needs the CRT-free LocalM128/Transform ABI; shared hkTransform includes xmmintrin.h and conflicts with _mm_setzero_ps.
    hkPadSpu<const CollidableBody*> m_pBodyA;
    hkPadSpu<const CollidableBody*> m_pBodyB;
    hkPadSpu<const CollidableBody*> m_pAdditionalCollidableBody;
    hkPadSpu<void*> m_pRayCastInput;
    hkPadSpu<void*> m_pContactManager;
    __declspec(align(16)) Transform m_kTransform;
    NnAgentInputLocalForm() : m_pAdditionalCollidableBody(0)
    {}
};
typedef void(__cdecl* AgentCreateFunc)(NnAgentInputLocalForm* input, CollisionAgentEntry* entry, void* subEntry);
struct QualityEntry
{
    AgentCreateFunc m_pCreateFunc;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int m_flag28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
};
struct Dispatcher
{
    char* m_tableBase;
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
    void* m_currentAgentEntry;
};
struct CollisionAgentEntry
{
    unsigned char m_flags;
    unsigned char m_iAutoRemoveLevel;
    unsigned char unk02;
    unsigned char m_iElementCount;
    unsigned short m_agentIndexA;
    unsigned short m_agentIndexB;
    void* m_pCurrentSimulationIsland;
    char m_ucNnTrackType;
    char unk0d;
    char unk0e;
    char unk0f;
    hkpLinkedCollidable* m_nodeA;
    hkpLinkedCollidable* m_nodeB;
    float m_fTimeOfImpact;
    int unk1c;
    LocalM128 m_vSeparatingNormal;
};
// Allocates a CollisionAgentEntry from pool, links nodeA/nodeB, builds NnAgentInputLocalForm, and calls the quality-level create function to initialize the agent.
// @fnv 0x00cfe8a0
// src: XBOX_PDB 0x00cfe8a0
// pdb: hkAgentNnMachine_CreateAgent | hkpAgentNnMachine.obj | retail, upstream name already matches, high
// pdb params: track, collA, firstNonTransformBodyA, collB, firstNonTransformBodyB, cdBodyHasTransformFlag, agentType, input, mgr
// pdb locals: in3, assertBuf, ostr
CollisionAgentEntry* __cdecl hkAgentNnMachine_CreateAgent(hkpAgentNnTrack* track, hkpLinkedCollidable* collA, CollidableBody* firstNonTransformBodyA,
                                          hkpLinkedCollidable* collB, CollidableBody* firstNonTransformBodyB, unsigned char cdBodyHasTransformFlag,
                                          int qualityType, Dispatcher* dispatcher, void* mgr)
{

    char* initialTable = dispatcher->m_tableBase;
    char* pAlloc = hkAgentNnMachine_AllocateEntry(track);
    CollisionAgentEntry* entry = (CollisionAgentEntry*)pAlloc;
    entry->m_nodeA = collA;
    entry->m_pCurrentSimulationIsland = mgr;
    entry->m_nodeB = collB;
    entry->m_agentIndexA = (unsigned short)collA->m_collisionEntries.m_iElementCount;
    entry->m_agentIndexB = (unsigned short)collB->m_collisionEntries.m_iElementCount;
    hkpLinkedCollidable::CollisionKeyEntry* linkA =
        &reinterpret_cast<hkArray<hkpLinkedCollidable::CollisionKeyEntry>&>(collA->m_collisionEntries).expandOne();
    hkpLinkedCollidable::CollisionKeyEntry* linkB =
        &reinterpret_cast<hkArray<hkpLinkedCollidable::CollisionKeyEntry>&>(collB->m_collisionEntries).expandOne();
    {
        NnAgentInputLocalForm input;
        linkA->m_pAgentEntry = (NarrowPhaseAgentEntry*)entry;
        linkB->m_pAgentEntry = (NarrowPhaseAgentEntry*)entry;
        linkA->m_pCollidable = collB;
        linkB->m_pCollidable = collA;
        input.m_pBodyA = firstNonTransformBodyA;
        input.m_pBodyB = firstNonTransformBodyB;
        input.m_pContactManager = entry->m_pCurrentSimulationIsland;
        input.m_pRayCastInput = dispatcher;
        input.m_kTransform.hkTransform_setMulInverseMul(*firstNonTransformBodyA->m_kMotionTransform, *firstNonTransformBodyB->m_kMotionTransform);
        char* tableBase = dispatcher->m_tableBase;
        char agentType = tableBase[0x1bb0 + collA->m_broadPhaseHandle.m_objectQualityType * 10 +
                                   collB->m_broadPhaseHandle.m_objectQualityType];
        entry->m_ucNnTrackType = agentType;
        dispatcher->m_currentAgentEntry = dispatcher->m_tableBase + 0x1c20 + ((int)(signed char)agentType << 6);
        QualityEntry* quality = (QualityEntry*)(initialTable + 0x16b0 + qualityType * 80);
        entry->m_iAutoRemoveLevel = (unsigned char)qualityType;
        void* subEntry;
        if (quality->m_flag28 != 0)
        {
            entry->m_flags = cdBodyHasTransformFlag | 4;
            entry->m_fTimeOfImpact = -1.0f;
            entry->m_vSeparatingNormal = _mm_setzero_ps();
            subEntry = (char*)entry + 0x30;
        }
        else
        {
            entry->m_flags = cdBodyHasTransformFlag | 2;
            subEntry = (char*)entry + 0x20;
        }
        ((QualityEntry*)(initialTable + 0x16b0 + (unsigned char)qualityType * 80))
            ->m_pCreateFunc(&input, entry, subEntry);
    }
    entry->m_iElementCount = 0x80;
    return entry;
}

__inline LocalM128 quadZero()
{
    return _mm_setzero_ps();
}
__inline LocalM128 quadInit()
{
    return quadZero();
}
struct SweptPose
{
    LocalM128 center0, center1, rotation0, rotation1, localCenter;
};
struct MotionPose
{
    __declspec(align(16)) Transform pose;
    SweptPose swept;
    LocalM128 angle;
    float radius, linearDamping, angularDamping, timeFactor;
    const SweptPose& getSweptTransform() const
    {
        return swept;
    }
};
class PairBody
{
  public:
    const void* shape;
    unsigned key;
    const void* motion;
    const PairBody* parent;
    PairBody()
    {}
    PairBody(const PairBody* p, const Transform* t)
    {
        parent = p;
        motion = t;
    }
    static void* operator new(size_t, void* p)
    {
        return p;
    }
    static void operator delete(void*, void*)
    {}
    const MotionPose* GetMotionState() const
    {
        return static_cast<const MotionPose*>(motion);
    }
    const void* getShape() const
    {
        return shape;
    }
    unsigned getShapeKey() const
    {
        return key;
    }
    void SetCollisionShapeAndKey(const void* s, unsigned k)
    {
        shape = s;
        key = k;
    }
};
struct StepTimes
{
    hkPadSpu<float> start, end, delta, invDelta;
};
struct PairContext
{
    void* dispatcher;
    int weld;
    float tolerance;
    int unk0c, unk10, unk14, unk18, unk1c;
    int unk20, unk24, unk28, unk2c;
    int unk30, unk34, unk38, unk3c;
    int unk40, unk44, unk48, unk4c;
    StepTimes step;
};
struct PairInput
{
    hkPadSpu<const PairBody*> bodyA, bodyB, overrideA;
    hkPadSpu<const PairContext*> context;
    hkPadSpu<void*> manager;
    __declspec(align(16)) Transform relative;
};
struct PairTemporary
{
    PairBody a, b;
    __declspec(align(16)) Transform ta, tb;
};
void __cdecl hkSweptTransformUtil_lerp2(const SweptPose&, float, Transform&);

// Computes swept transforms for both bodies, copies shapes and keys into temp bodies, and sets out fields from in and temp.
// @fnv 0x00cfde10
// src: HAVOK_LIB 0x00cfde10
// pdb: hkAgentNnMachine_initInputAtTime | hkpAgentNnMachine.obj | retail, upstream name already matches, high
// pdb params: in, temp, out
static __declspec(noinline) void hkAgentNnMachine_initInputAtTime(PairInput& out, PairInput& in, PairTemporary& temp)
{
    hkSweptTransformUtil_lerp2(in.bodyA->GetMotionState()->getSweptTransform(), in.context->step.start, temp.ta);
    hkSweptTransformUtil_lerp2(in.bodyB->GetMotionState()->getSweptTransform(), in.context->step.start, temp.tb);
    out.bodyA = &temp.a;
    out.bodyB = &temp.b;
    out.manager = in.manager;
    out.context = in.context;
    temp.a.SetCollisionShapeAndKey(in.bodyA->getShape(), in.bodyA->getShapeKey());
    temp.b.SetCollisionShapeAndKey(in.bodyB->getShape(), in.bodyB->getShapeKey());
    new (&temp.a) PairBody(in.bodyA, &temp.ta);
    new (&temp.b) PairBody(in.bodyB, &temp.tb);
    out.relative.hkTransform_setMulInverseMul(temp.ta, temp.tb);
}
PairInput* volatile pairIn;
PairTemporary* volatile pairTemp;
PairInput* volatile pairOut;
void pairCaller()
{
    hkAgentNnMachine_initInputAtTime(*pairOut, *pairIn, *pairTemp);
}
