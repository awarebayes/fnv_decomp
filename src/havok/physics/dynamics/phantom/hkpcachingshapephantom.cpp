
// src/havok/physics/dynamics/phantom/hkpcachingshapephantom.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/dynamics/phantom/hkpcachingshapephantom.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include <string.h>
#include "../../../../../include/hkArray.h"
#include "../../../../../include/hkBool.h"
#include "../../../../../include/hkBoolResult.h"
#include "../../../../../include/hkpCollidable.h"
#include "../../../../../include/hkpWorldObject.h"
#include "../../../../../include/hkClass.h"
#include "../../../../../include/hkMemoryAllocator.h"
#include "../../../../../include/hkMemoryRouter.h"
#include "../../../../../include/hkStatisticsCollector.h"
#include "../../../../../include/hkVector4.h"
#include "../../../../../include/hkTransform.h"
#include "../../../../../include/hkAabb.h"
#include "../../../../../include/hkpShape.h"
#include "../../../../../include/hkpWorld.h"
#include "../../../../../include/hkpShapePhantom.h"
#include "../../../../../include/hkpCdBodyPairCollector.h"
#include "../../../../../include/hkpCollisionAgent.h"
#include "../../../../../include/hkpCollisionDispatcher.h"
#include "../../../../../include/hkpCollisionInput.h"

typedef unsigned short hkUint16;
typedef short hkInt16;
typedef unsigned __int64 hkUint64;

inline const hkpShape* hkpCollidable::getShape() const { return m_pCollisionShape; }
__forceinline unsigned int hkpCollidable::GetFilterInfoForKey() const { return m_broadPhaseHandle.m_collisionFilterInfo; }

inline hkpShapeType hkpShape::getType() const
{
    return *reinterpret_cast<const hkpShapeType*>(&m_type);
}

void __cdecl SortEntriesByCollisionKey(void*, int, int, int);

// @fnv 0x00d4c910
// src: COMMUNITY 0x00d4c910
int hkpCachingShapePhantom_getType()
{
    return 2;
}

struct GskAgentInput;
struct WorldOperationEntry;

// The shared agent header does not yet declare its collision dispatch slots.
// Keep the calls here without introducing another definition of the agent.
__forceinline void ProcessAgent(hkpCollisionAgent* agent, const hkpCdBody& bodyA, const hkpCdBody& bodyB,
                               const GskAgentInput& input, WorldOperationEntry& output)
{
    typedef void (__thiscall* Dispatch)(hkpCollisionAgent*, const hkpCdBody&, const hkpCdBody&,
                                       const GskAgentInput&, WorldOperationEntry&);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(agent))[4])(agent, bodyA, bodyB, input, output);
}

struct CollisionAgentPair
{
    hkpCollisionAgent* m_pCollisionAgent;
    hkpCollidable* m_kCollidable;
};

struct LinearCastParameters;
class hkpCdPointCollector;

// include/ has no hkpCachingShapePhantom.h yet; one real definition serves every function in this unit.
// pdb: class hkpCachingShapePhantom, size 0x170, bases: hkpShapePhantom, 2 members
class hkpCachingShapePhantom : public hkpShapePhantom
{
public:
    hkArray<CollisionAgentPair> m_collisionDetails; // src: XBOX_RETAIL_PDB +0x160 (old: m_kCollisionDetails)
    hkBool m_orderDirty; // src: XBOX_RETAIL_PDB +0x16c (old: m_bOrderDirty)

    static void* operator new(unsigned int uiSize) throw();
    static void operator delete(void* p) throw();
    hkpCachingShapePhantom(const hkpShape* shape, const hkTransform& transform, hkUint32 collisionFilterInfo) throw();
    void ensureDeterministicOrder();
    void DispatchCollisionAgents(WorldOperationEntry& collector, GskAgentInput* input);
    void GetPenetrations(hkpCdBodyPairCollector& collector, const hkpCollisionInput* input);
    hkBool ContainsCollidable(const hkpCollidable* collidable) const;
    void AddToCollector(hkStatisticsCollector* collector, const hkClass* cls) const throw();
    hkpWorldObject* clone_00d4d0d0() const throw();
    void removeOverlappingCollidable(hkpCollidable* collidable);
    // The shared hkBoolResult preserves NotifyAll's nontrivial byte-copy ABI.
    void hkpPhantom_fireCollidableRemoved(hkpCollidable* collidable, hkBoolResult collidableWasAdded);
    int hkpPhantom_fireCollidableAdded(hkpCollidable* collidable) throw();
    void addOverlappingCollidable(hkpCollidable* collidable) throw();
    void QueryCollisionDetails();
    void hkpShapePhantom_deallocateInternalArrays();
    void deallocateInternalArrays();
    void hkpPhantom_updateBroadPhase(const hkAabb& aabb);
    void setPositionAndLinearCast(const hkVector4& position, const LinearCastParameters& input,
                    hkpCdPointCollector& collector, hkpCdPointCollector* startCollector);
    void CopyFrom(const hkTransform& transform, const LinearCastParameters& input,
                  hkpCdPointCollector& collector, hkpCdPointCollector* startCollector);
    void ReleaseCollisionAgents();
    hkpCachingShapePhantom* DeleteStorage(unsigned int flags);
};

void WritePhantomLists();
__forceinline void WritePhantomLists(const hkpCachingShapePhantom* object, hkStatisticsCollector* collector, const hkClass* cls)
{
    typedef void (__thiscall* Collect)(const hkpCachingShapePhantom*, hkStatisticsCollector*, const hkClass*);
    reinterpret_cast<Collect>(static_cast<void (*)()>(&WritePhantomLists))(object, collector, cls);
}

// @fnv 0x00d4d090
// src: HAVOK_SRC 0x00d4d090
// pdb: hkpCachingShapePhantom::ensureDeterministicOrder | hkpCachingShapePhantom.obj | retail, upstream name already matches, high
void hkpCachingShapePhantom::ensureDeterministicOrder()
{
    if (m_orderDirty)
    {
        int size = m_collisionDetails.m_iElementCount;

        union
        {
            unsigned char byteValue;
            unsigned int value;
        } seed;

        seed.byteValue = 0;

        if (size > 1)
            SortEntriesByCollisionKey(m_collisionDetails.m_data, 0, size - 1, seed.value);

        m_orderDirty = 0;
    }
}

// @fnv 0x00d4c920
// pdb: hkpCachingShapePhantom::getClosestPoints | hkpCachingShapePhantom.obj | retail, vtable slot, high
// pdb params: collector, input
// pdb locals: assertBuf, ostr
void hkpCachingShapePhantom::DispatchCollisionAgents(WorldOperationEntry& collector, GskAgentInput* input)
{
    if (!input)
    {
        input = (GskAgentInput*)((hkpWorld*)this->m_world)->m_collisionInput;
    }
    for (int i = this->m_collisionDetails.m_iElementCount - 1; i >= 0; --i)
    {
        CollisionAgentPair* detail = &this->m_collisionDetails.m_data[i];
        ProcessAgent(detail->m_pCollisionAgent, *(hkpCdBody*)&this->m_kCollidable, *(hkpCdBody*)detail->m_kCollidable, *input, collector);
    }
}

__forceinline void GetAgentPenetrations(hkpCollisionAgent* agent, const hkpCdBody& bodyA, const hkpCdBody& bodyB,
                                   const hkpCollisionInput& input, hkpCdBodyPairCollector& collector)
{
    typedef void (__thiscall* Dispatch)(hkpCollisionAgent*, const hkpCdBody&, const hkpCdBody&,
                                       const hkpCollisionInput&, hkpCdBodyPairCollector&);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(agent))[3])(agent, bodyA, bodyB, input, collector);
}

// The collector's early-out byte follows its vtable pointer; the shared header
// currently declares only its methods.
__forceinline const hkBool& earlyOutHit(const hkpCdBodyPairCollector& collector)
{
    return *reinterpret_cast<const hkBool*>(reinterpret_cast<const char*>(&collector) + sizeof(void*));
}

// Iterates the cached collision agent/collidable pairs and calls getPenetrations on each, stopping early if the collector signals an early-out hit.
// @fnv 0x00d4c980
// pdb: hkpCachingShapePhantom::getPenetrations | hkpCachingShapePhantom.obj | retail, vtable slot, high
// pdb params: collector, input
// pdb locals: assertBuf, ostr
void hkpCachingShapePhantom::GetPenetrations(hkpCdBodyPairCollector& collector,
                                                     const hkpCollisionInput* input)
{
    if (!input)
    {
        input = (const hkpCollisionInput*)((hkpWorld*)m_world)->m_collisionInput;
    }

    for (int i = m_collisionDetails.m_iElementCount - 1; i >= 0; i--)
    {
        CollisionAgentPair* detail = &m_collisionDetails.m_data[i];
        GetAgentPenetrations(detail->m_pCollisionAgent, *(hkpCdBody*)&m_kCollidable, *(hkpCdBody*)detail->m_kCollidable, *input, collector);
        if (earlyOutHit(collector))
        {
            break;
        }
    }
}

// Searches the collision detail array for an entry whose collidable pointer equals pkCollidable and returns true if found, otherwise false.
// @fnv 0x00d4c9e0
// pdb: hkpCachingShapePhantom::isOverlappingCollidableAdded | hkpCachingShapePhantom.obj | debug, vtable slot, medium
// pdb params: collidable
hkBool hkpCachingShapePhantom::ContainsCollidable(const hkpCollidable* pkCollidable) const
{
    for (int i = 0; i < m_collisionDetails.GetLogicalSize(); i++)
    {
        if (m_collisionDetails[i].m_kCollidable == pkCollidable)
            return true;
    }
    return false;
}

extern const hkClass DAT_0126902c;

int __cdecl hkStatisticsCollector_getAllocatedSize(const void* p, int size) throw();

// The collector header lacks the one-argument beginObject and this unit's virtual slot ordering.
__forceinline void BeginAgentCollection(hkStatisticsCollector* collector, const char* name)
{
    typedef void (__thiscall* Dispatch)(hkStatisticsCollector*, const char*);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(collector))[6])(collector, name);
}
__forceinline void EndAgentCollection(hkStatisticsCollector* collector)
{
    typedef void (__thiscall* Dispatch)(hkStatisticsCollector*);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(collector))[7])(collector);
}
__forceinline void CollectChildAgent(hkStatisticsCollector* collector, const char* name, const void* object, const void* cls)
{
    typedef void (__thiscall* Dispatch)(hkStatisticsCollector*, const char*, const void*, const void*);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(collector))[3])(collector, name, object, cls);
}
template <class T> __forceinline void CollectAgentArray(hkStatisticsCollector* collector, const char* name, const hkArray<T>& array)
{
    if ((array.m_iCapacityState & 0x80000000) == 0 || collector->m_includeUnmanagedArrayMemory)
    {
        int allocated = hkStatisticsCollector_getAllocatedSize(array.m_data, array.getCapacity() * sizeof(T));
        typedef void (__thiscall* Dispatch)(hkStatisticsCollector*, int, const char*, const void*, int, int);
        reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(collector))[5])(
            collector, 3, name, array.m_data, array.GetLogicalSize() * sizeof(T), allocated);
    }
}

// Adds this phantom's base data and its AgentPtr array to the collector, then reports each collision agent as a child object under "agents".
// @fnv 0x00d4cd60
// pdb: hkpCachingShapePhantom::calcContentStatistics | hkpCachingShapePhantom.obj | retail, string-set bridge, high
// pdb params: collector, cls
void hkpCachingShapePhantom::AddToCollector(hkStatisticsCollector* collector,
                                                const hkClass* cls) const throw()
{
    WritePhantomLists(this, collector, &DAT_0126902c);
    CollectAgentArray(collector, "AgentPtr", m_collisionDetails);
    BeginAgentCollection(collector, "agents");
    for (int i = 0; i < m_collisionDetails.GetLogicalSize(); i++)
        CollectChildAgent(collector, 0, m_collisionDetails.m_data[i].m_pCollisionAgent, 0);
    EndAgentCollection(collector);
}

extern "C" hkMemoryRouter& __cdecl hkMemoryRouter_getInstance();

__forceinline hkMemoryAllocator& hkContainerHeapAllocator::get(const void*) { return hkMemoryRouter_getInstance().heap(); }

template <typename T> T* hkMemoryAllocator::_bufAlloc(int& requested)
{
    int bytes = requested * sizeof(T);
    void* p = bufAlloc(bytes);
    requested = bytes / (int)sizeof(T);
    return (T*)p;
}
template <typename T> void hkMemoryAllocator::_bufFree(void* p, int count)
{
    bufFree(p, count * sizeof(T));
}
template <typename T, typename A> __forceinline hkArray<T, A>& hkArray<T, A>::operator=(const hkArray& other)
{
    hkMemoryAllocator& alloc = hkMemoryRouter_getInstance().heap();
    if (getCapacity() < other.GetLogicalSize())
    {
        if ((m_iCapacityState & 0x80000000) == 0)
            alloc._bufFree<T>(m_data, getCapacity());
        int n = other.GetLogicalSize();
        m_data = alloc._bufAlloc<T>(n);
        m_iCapacityState = n;
    }
    m_iElementCount = other.GetLogicalSize();
    hkArrayBase<T>::copy(m_data, other.m_data, m_iElementCount);
    return *this;
}
template <typename T> __forceinline void hkArrayBase<T>::_clearAndDeallocate(hkMemoryAllocator& alloc)
{
    m_iElementCount = 0;
    if ((m_iCapacityState & 0x80000000) == 0)
        alloc.bufFree(m_data, (m_iCapacityState & 0x3fffffff) * sizeof(T));
    m_data = 0;
    m_iCapacityState = 0x80000000;
}

// hkpWorldObject.h lacks CopyEntriesFrom; preserve its thiscall through a local call adapter.
void CopyWorldObjectEntries();
__forceinline void CopyWorldObjectEntries(hkpWorldObject* object, const hkpWorldObject* other)
{
    typedef void (__thiscall* Copy)(hkpWorldObject*, const hkpWorldObject*);
    reinterpret_cast<Copy>(static_cast<void (*)()>(&CopyWorldObjectEntries))(object, other);
}

__forceinline void* hkpCachingShapePhantom::operator new(unsigned int uiSize) throw()
{
    hkpWorldObject* p = (hkpWorldObject*)hkMemoryRouter_getInstance().heap().blockAlloc(uiSize);
    p->m_memSizeAndFlags = (hkUint16)uiSize;
    return p;
}

typedef char fnv_check_d4d0d0[1];

// @fnv 0x00d4d0d0
// src: HAVOK_SRC 0x00d4d0d0
// pdb: hkpCachingShapePhantom::clone | hkpCachingShapePhantom.obj | retail, vtable slot, high
hkpWorldObject* hkpCachingShapePhantom::clone_00d4d0d0() const throw()
{
    const hkpCollidable* pkCollidable = &m_kCollidable;
    hkpCachingShapePhantom* pkPhantom =
        new hkpCachingShapePhantom(pkCollidable->getShape(), m_motionState.m_transform, pkCollidable->GetFilterInfoForKey());
    pkPhantom->m_overlapListeners = m_overlapListeners;
    pkPhantom->m_phantomListeners = m_phantomListeners;
    CopyWorldObjectEntries(pkPhantom, this);
    return pkPhantom;
}

struct CollisionTarget
{
    int unknown00;
    int unknown04;
    int unknown08;
    int unknown0c;
    int unknown10;
    unsigned int collisionKey;
};
struct CollisionKeyEntry
{
    int shapeKey;
    CollisionTarget* target;
};
// Recursively quicksorts entries by their target's collisionKey, partitioning around a pivot and swapping out-of-order elements.
// @fnv 0x00d4d250
// pdb: hkAlgorithm::quickSortRecursive<hkpCachingShapePhantom::hkpCollisionDetail,hkpCachingShapePhantom::OrderByUid> | hkpCachingShapePhantom.obj | retail, object-file gap fill, high
// pdb params: pArr, d, h, cmpLess
// pdb locals: str
void __cdecl SortEntriesByCollisionKey(CollisionKeyEntry* pArr, int d, int right, int cmpLess)
{
    int i = d;
    int iRight = right;
    __declspec(align(16)) CollisionKeyEntry pivotEntry = pArr[(i + iRight) >> 1];
    do
    {
        while (pArr[i].target->collisionKey < pivotEntry.target->collisionKey)
            ++i;
        while (pivotEntry.target->collisionKey < pArr[iRight].target->collisionKey)
            --iRight;
        if (iRight < i)
            break;
        if (iRight != i)
        {
            CollisionKeyEntry tempEntry = pArr[iRight];
            pArr[iRight] = pArr[i];
            pArr[i] = tempEntry;
        }
        --iRight;
        ++i;
    } while (i <= iRight);
    if (d < iRight)
        SortEntriesByCollisionKey(pArr, d, iRight, cmpLess);
    if (i < right)
        SortEntriesByCollisionKey(pArr, i, right, cmpLess);
}

// @fnv 0x00d4d310
// pdb: hkpCachingShapePhantom::`scalar deleting destructor' | hkpCachingShapePhantom.obj | retail, vtable slot, high
hkpCachingShapePhantom* hkpCachingShapePhantom::DeleteStorage(unsigned int flags)
{
    ReleaseCollisionAgents();
    if (flags & 1)
        hkMemoryRouter_getInstance().heap().blockFree(this, m_memSizeAndFlags);
    return this;
}

// Copies the rotation columns, then casts from the transform's translation.
// @fnv 0x00d4cc50
// pdb: hkpCachingShapePhantom::setTransformAndLinearCast | hkpCachingShapePhantom.obj | debug, vtable slot, medium
// pdb params: transform, input, castCollector, startCollector
// pdb locals: assertBuf, ostr
void hkpCachingShapePhantom::CopyFrom(const hkTransform& transform, const LinearCastParameters& input,
                                    hkpCdPointCollector& collector, hkpCdPointCollector* startCollector)
{
    m_motionState.m_transform.m_kColumn0 = transform.m_kColumn0;
    m_motionState.m_transform.m_kColumn1 = transform.m_kColumn1;
    m_motionState.m_transform.m_kColumn2 = transform.m_kColumn2;
    setPositionAndLinearCast(*reinterpret_cast<const hkVector4*>(&transform.m_vTranslation), input, collector, startCollector);
}

extern int DAT_010ccebc;

// Stack constraint owner: cleanup slot 7 receives the object with vtable 0x010ccebc.
struct PhantomConstraintOwner
{
    int* m_vtbl;
    unsigned short m_usMemSizeAndFlags;
    unsigned short m_sReferenceCount;
    int unk08, unk0C, unk10, unk14;
};

__forceinline void cleanupCollisionAgent(hkpCollisionAgent* agent, PhantomConstraintOwner* owner)
{
    typedef void (__thiscall* Dispatch)(hkpCollisionAgent*, PhantomConstraintOwner*);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(agent))[7])(agent, owner);
}

inline void CopyFourByteWords(void* dst, const void* src, int nbytes)
{
    for (int i = 0; i < nbytes; i += 4)
        *(unsigned int*)((char*)dst + i) = *(const unsigned int*)((const char*)src + i);
}

template <typename T> __forceinline void hkArrayBase<T>::removeAt(int index)
{
    m_iElementCount--;
    if (m_iElementCount != index)
        CopyFourByteWords(m_data + index, m_data + m_iElementCount, sizeof(T));
}

// Removes a cached collidable, notifies overlap listeners, and marks the ordering dirty.
// @fnv 0x00d4cca0
// src: HAVOK_LIB 0x00d4cca0
// pdb: hkpCachingShapePhantom::removeOverlappingCollidable | hkpCachingShapePhantom.obj | retail, upstream name already matches, high
// pdb params: collidable
// pdb locals: constraintOwner
void hkpCachingShapePhantom::removeOverlappingCollidable(hkpCollidable* collidable)
{
    PhantomConstraintOwner visitor;
    visitor.m_sReferenceCount = 1;
    visitor.m_vtbl = &DAT_010ccebc;
    if (!collidable->m_pCollisionShape)
        return;
    for (int i = m_collisionDetails.GetLogicalSize() - 1; i >= 0; i--)
    {
        if (m_collisionDetails[i].m_kCollidable == collidable)
        {
            hkpPhantom_fireCollidableRemoved(collidable, hkBoolResult(true));
            if (m_collisionDetails[i].m_pCollisionAgent)
                cleanupCollisionAgent(m_collisionDetails[i].m_pCollisionAgent, &visitor);
            m_collisionDetails.removeAt(i);
            m_orderDirty = 1;
            return;
        }
    }
    hkpPhantom_fireCollidableRemoved(collidable, hkBoolResult(false));
}

void ResizeAudioStream(void* mem, void* array, int sizeElem) throw();

template <typename T> __forceinline T& hkArrayBase<T>::_expandOne(hkMemoryAllocator& alloc)
{
    if (m_iElementCount == getCapacity())
        ResizeAudioStream(&alloc, this, sizeof(T));
    return m_data[m_iElementCount++];
}

// The game selects the PSI quality (index 1), at dispatcher offset 0x1c60.
// The old one-int quality stand-in incorrectly described that address as index 0.
__forceinline hkpCollisionQualityInfo* hkpCollisionDispatcher::getCollisionQualityInfo(CollisionQualityIndex index)
{
    return &m_pCollisionQualityInfo[index];
}

// Preserve this unit's inline 0x50-byte collision-input copy.
inline hkpCollisionInput::hkpCollisionInput(const hkpCollisionInput& other)
{
    memcpy(this, &other, sizeof(*this));
}

// pdb: structure hkpProcessCollisionInput, size 0x80, bases: hkpCollisionInput, 7 members
struct hkpProcessCollisionInput : public hkpCollisionInput
{
    hkReal m_stepInfo; // src: XBOX_RETAIL_PDB +0x50 (old: m_uiStartTime)
    hkReal m_fEndTime;
    hkReal m_fTimeStep;
    hkReal m_fInverseDeltaTime;
    hkpCollisionQualityInfo* m_collisionQualityInfo; // src: XBOX_RETAIL_PDB +0x60 (old: m_pCollisionQualityInfo)
    void* m_spareAgentSector; // src: XBOX_RETAIL_PDB +0x64 (old: m_pAgent1nSector)
    void* m_dynamicsInfo; // src: XBOX_RETAIL_PDB +0x68 (old: m_pCollisionFilter)
    hkBool m_enableDeprecatedWelding; // src: XBOX_RETAIL_PDB +0x6c (old: m_bEnableDeprecatedWelding)
    hkBool m_allowToSkipConfirmedCallbacks; // src: XBOX_RETAIL_PDB +0x6d (old: m_bAllowToSkipConfirmedCallbacks)
    void* m_config; // src: XBOX_RETAIL_PDB +0x70 (old: m_pConfiguration)

    __forceinline static void* operator new(size_t, void* storage) { return storage; }

    __forceinline hkpProcessCollisionInput(const hkpProcessCollisionInput& kOther) throw() : hkpCollisionInput(kOther)
    {
        m_spareAgentSector = 0;
        FUN_00cfa7d0(kOther);
    }
    ~hkpProcessCollisionInput() throw()
    {
        if (m_spareAgentSector)
            hkMemoryRouter_getInstance().heap().blockFree(m_spareAgentSector, 0x200);
    }
    void FUN_00cfa7d0(const hkpProcessCollisionInput& kOther);
};

// The shared dispatcher exposes the tables but has no agent-creation convenience method.
__forceinline hkpCollisionAgent* CreateCachedCollisionAgent(const hkpCollisionDispatcher* dispatcher,
    const hkpCdBody& kBodyA, const hkpCdBody& kBodyB, const hkpProcessCollisionInput& kInput, hkpContactMgr* pvMgr)
{
    int iTypeA = kBodyA.m_shape->getType();
    int iTypeB = kBodyB.m_shape->getType();
    const hkUint8* pTypes = kInput.m_createPredictiveAgents
        ? &dispatcher->m_agent3TypeMap[0][0] : &dispatcher->m_agent2TypeMap[0][0];
    int iIdx = pTypes[iTypeA * 32 + iTypeB];
    hkpCollisionDispatcher::CreateFunc pfnCreate = dispatcher->m_agent2Funcs[iIdx].m_pCreateFunc;
    return pfnCreate(kBodyA, kBodyB, reinterpret_cast<const CollisionAgentContext&>(kInput), pvMgr);
}

// Use pointer storage for the local input: the original function has no GS cookie.
// @fnv 0x00d4cf10
// src: HAVOK_SRC 0x00d4cf10
// pdb: hkpCachingShapePhantom::addOverlappingCollidable | hkpCachingShapePhantom.obj | retail, upstream name already matches, high
// pdb params: collidable
// pdb locals: input
void hkpCachingShapePhantom::addOverlappingCollidable(hkpCollidable* collidable) throw()
{
    if (collidable->m_pCollisionShape != 0 && hkpPhantom_fireCollidableAdded(collidable) == 0)
    {
        CollisionAgentPair& kDetail = m_collisionDetails.expandOne();
        __declspec(align(16)) void* inputStorage[sizeof(hkpProcessCollisionInput) / sizeof(void*)];
        hkpProcessCollisionInput& kInput = *new (inputStorage) hkpProcessCollisionInput(
            *(hkpProcessCollisionInput*)((hkpWorld*)m_world)->m_collisionInput);
        kInput.m_createPredictiveAgents = false;
        kInput.m_collisionQualityInfo = kInput.m_dispatcher->getCollisionQualityInfo(static_cast<hkpCollisionDispatcher::CollisionQualityIndex>(
            hkpCollisionDispatcher::COLLISION_QUALITY_PSI));
        kDetail.m_pCollisionAgent =
            CreateCachedCollisionAgent(kInput.m_dispatcher, *(hkpCdBody*)&m_kCollidable, *(hkpCdBody*)collidable, kInput, 0);
        kDetail.m_kCollidable = collidable;
        m_orderDirty = true;
        kInput.~hkpProcessCollisionInput();
    }
}

__forceinline void queryCollisionAgent(hkpCollisionAgent* agent, void* a, hkpCollidable* b, int* c, void* d)
{
    typedef void (__thiscall* Dispatch)(hkpCollisionAgent*, void*, hkpCollidable*, int*, void*);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(agent))[8])(agent, a, b, c, d);
}

__declspec(align(8)) struct QueryDC4A30
{
    int m_pOwningWorld;
    hkUint16 m_unk04;
    hkUint16 m_type;
    hkUint64 m_unk08;
    hkUint64 m_unk10;
};

// Copies 0x50 bytes from the pointer at (this->8)+0x78 into a local buffer, then calls v20 on each collision detail entry with it and a type-1 query.
// @fnv 0x00d4ca30
// pdb: hkpCachingShapePhantom::updateShapeCollectionFilter | hkpCachingShapePhantom.obj | retail, vtable slot, high
// pdb locals: input, constraintOwner, assertBuf, ostr
void hkpCachingShapePhantom::QueryCollisionDetails()
{
    QueryDC4A30 query;
    __declspec(align(16)) int buffer[20];

    char* src = (char*)((hkpWorld*)m_world)->m_collisionInput;
    memcpy(buffer, src, 0x50);

    query.m_type = 1;
    query.m_pOwningWorld = (int)&DAT_010ccebc;

    for (int i = 0; i < m_collisionDetails.m_iElementCount; ++i)
    {
        CollisionAgentPair* pair = m_collisionDetails.m_data + i;
        queryCollisionAgent(pair->m_pCollisionAgent, &m_kCollidable, pair->m_kCollidable, buffer, &query);
    }
}

// Releases m_kCollisionDetails's buffer when its element count is 0, then calls FUN_00c9e820.
// @fnv 0x00d4d030
// src: HAVOK_LIB 0x00d4d030
// pdb: hkpCachingShapePhantom::deallocateInternalArrays | hkpCachingShapePhantom.obj | retail, upstream name already matches, high
// pdb locals: assertBuf, ostr
void hkpCachingShapePhantom::deallocateInternalArrays()
{
    if (m_collisionDetails.m_iElementCount == 0)
        m_collisionDetails.clearAndDeallocate();
    hkpShapePhantom_deallocateInternalArrays();
}

namespace CastSimd {
// Exe 0x00d4cab0 FUN_00d4cab0: retain this LocalM128 view; the CRT type does not preserve this unit.
// VC9 requires the __m128 compatibility typedef for its native SIMD ABI (renamed tags fail C2719).
typedef union __declspec(intrin_type) __declspec(align(16)) {
    float f[4];
    unsigned __int64 u[2];
} __m128, LocalM128;
extern "C"
{
    LocalM128 _mm_sub_ps(LocalM128, LocalM128);
    LocalM128 _mm_add_ps(LocalM128, LocalM128);
    LocalM128 _mm_mul_ps(LocalM128, LocalM128);
    LocalM128 _mm_min_ps(LocalM128, LocalM128);
    LocalM128 _mm_max_ps(LocalM128, LocalM128);
    LocalM128 _mm_setzero_ps(void);
    LocalM128 _mm_shuffle_ps(LocalM128, LocalM128, unsigned int);
    LocalM128 _mm_add_ss(LocalM128, LocalM128);
    LocalM128 _mm_sqrt_ss(LocalM128);
    float _mm_cvtss_f32(LocalM128);
}
}

extern float DAT_0104da4c;

struct GskAgentInput : public hkpCollisionInput
{
    int m_pad50;
    int m_pad54;
    int m_pad58;
    int m_pad5c;
    int m_pad60;
    int m_pad64;
    int m_pad68;
    int m_pad6c;
    void* m_pConfiguration;
};

struct ShapeSweepParameters : public hkpCollisionInput
{
    hkVector4 m_path;
    hkReal m_fMaxExtraPenetration;
    hkReal m_cachedDistances;
    void* m_pConfiguration;

    __forceinline void set(const GskAgentInput& input)
    {
        m_dispatcher.m_storage = input.m_dispatcher.m_storage;
        m_weldClosestPoints.m_storage = input.m_weldClosestPoints.m_storage;
        *reinterpret_cast<int*>(&m_tolerance.m_storage) = *reinterpret_cast<const int*>(&input.m_tolerance.m_storage);
        m_filter.m_storage = input.m_filter.m_storage;
        m_convexListFilter.m_storage = input.m_convexListFilter.m_storage;
        m_createPredictiveAgents = input.m_createPredictiveAgents;
        *(CastSimd::LocalM128*)&m_aabb32Info.m_kAabbMin = *(CastSimd::LocalM128*)&input.m_aabb32Info.m_kAabbMin;
        *(CastSimd::LocalM128*)&m_aabb32Info.m_kAabbMaximum = *(CastSimd::LocalM128*)&input.m_aabb32Info.m_kAabbMaximum;
        *(CastSimd::LocalM128*)&m_aabb32Info.m_kAabbScale = *(CastSimd::LocalM128*)&input.m_aabb32Info.m_kAabbScale;
        m_pConfiguration = input.m_pConfiguration;
    }
};

struct LinearCastParameters
{
    hkVector4 m_vTo;
    hkReal m_fMaxExtraPenetrationAgainstFixed;
    hkReal m_fMaxExtraPenetration;
};

__forceinline void linearCast(hkpCollisionAgent* agent, const hkpCdBody& a, const hkpCdBody& b,
                              const ShapeSweepParameters& input, hkpCdPointCollector& castCollector,
                              hkpCdPointCollector* startCollector)
{
    typedef void (__thiscall* Dispatch)(hkpCollisionAgent*, const hkpCdBody&, const hkpCdBody&,
                                       const ShapeSweepParameters&, hkpCdPointCollector&, hkpCdPointCollector*);
    reinterpret_cast<Dispatch>((*reinterpret_cast<void***>(agent))[5])(agent, a, b, input, castCollector, startCollector);
}

// Performs a linear cast against collidables by computing an expanded AABB and invoking linearCast on each collision agent pair.
// @fnv 0x00d4cab0
// src: HAVOK_SRC 0x00d4cab0
// pdb: hkpCachingShapePhantom::setPositionAndLinearCast | hkpCachingShapePhantom.obj | retail, upstream name already matches, high
// pdb params: position, input, castCollector, startCollector
// pdb locals: lcInput, aabb, assertBuf, ostr
void hkpCachingShapePhantom::setPositionAndLinearCast(const hkVector4& position, const LinearCastParameters& input,
                                               hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector)
{
    *(CastSimd::LocalM128*)&m_motionState.m_transform.m_vTranslation = *(CastSimd::LocalM128*)&position;

    // These outputs are filled below without calling the out-of-line vector constructors.
    __declspec(align(16)) void* aabbStorage[sizeof(hkAabb) / sizeof(void*)];
    hkAabb& aabb = *reinterpret_cast<hkAabb*>(aabbStorage);
    hkReal tol =
        (hkReal)(((GskAgentInput*)((hkpWorld*)m_world)->m_collisionInput)->m_tolerance * 0.5) + input.m_fMaxExtraPenetration;
    m_kCollidable.m_pCollisionShape->CalculateAabb(m_motionState.m_transform, tol, aabb);

    CastSimd::LocalM128 path = CastSimd::_mm_sub_ps(*(CastSimd::LocalM128*)&input.m_vTo, *(CastSimd::LocalM128*)&position);

    CastSimd::LocalM128 zero = CastSimd::_mm_setzero_ps();
    CastSimd::LocalM128 minVal = CastSimd::_mm_min_ps(zero, path);
    CastSimd::LocalM128 maxVal = CastSimd::_mm_max_ps(zero, path);

    CastSimd::LocalM128 auiLowerBounds = *(CastSimd::LocalM128*)&aabb.m_kMinimumCorner;
    CastSimd::LocalM128 auiUpperBounds = *(CastSimd::LocalM128*)&aabb.m_kMaximumCorner;

    auiLowerBounds = CastSimd::_mm_add_ps(minVal, auiLowerBounds);
    auiUpperBounds = CastSimd::_mm_add_ps(maxVal, auiUpperBounds);

    *(CastSimd::LocalM128*)&aabb.m_kMinimumCorner = auiLowerBounds;
    *(CastSimd::LocalM128*)&aabb.m_kMaximumCorner = auiUpperBounds;

    hkpPhantom_updateBroadPhase(aabb);

    // The shared collision input has no default constructor; initialize the sweep storage here.
    __declspec(align(16)) void* linearInputStorage[sizeof(ShapeSweepParameters) / sizeof(void*)];
    ShapeSweepParameters& linearInput = *reinterpret_cast<ShapeSweepParameters*>(linearInputStorage);
    linearInput.m_fMaxExtraPenetration = DAT_0104da4c;
    linearInput.m_weldClosestPoints.m_storage = 0;
    linearInput.set(*(GskAgentInput*)((hkpWorld*)m_world)->m_collisionInput);

    *(CastSimd::LocalM128*)&linearInput.m_path = path;

    CastSimd::LocalM128 sq = CastSimd::_mm_mul_ps(path, path);
    CastSimd::LocalM128 sh1 = CastSimd::_mm_shuffle_ps(sq, sq, 0x55);
    CastSimd::LocalM128 sh2 = CastSimd::_mm_shuffle_ps(sq, sq, 0xAA);
    CastSimd::LocalM128 add1 = CastSimd::_mm_add_ss(sh1, sq);
    CastSimd::LocalM128 add2 = CastSimd::_mm_add_ss(sh2, add1);
    CastSimd::LocalM128 len = CastSimd::_mm_sqrt_ss(add2);

    linearInput.m_cachedDistances = CastSimd::_mm_cvtss_f32(len);

    linearInput.m_tolerance = input.m_fMaxExtraPenetration;
    linearInput.m_fMaxExtraPenetration = input.m_fMaxExtraPenetrationAgainstFixed;

    for (int i = m_collisionDetails.m_iElementCount - 1; i >= 0; --i)
    {
        CollisionAgentPair* detail = &((CollisionAgentPair*)m_collisionDetails.m_data)[i];
        linearCast(detail->m_pCollisionAgent, *(hkpCdBody*)&m_kCollidable, *(hkpCdBody*)detail->m_kCollidable, linearInput, castCollector,
                         startCollector);
    }
}
