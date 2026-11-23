
// src/havok/physics/dynamics/entity/hkpentity.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/dynamics/entity/hkpentity.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../../include/hkpEntity.h"
#include "../../../../../include/hkpWorld.h"
#include "../../../../../include/hkpSimulationIsland.h"
#include "../../../../../include/hkpShape.h"
#include "../../../../../include/hkpShapeContainer.h"
#include "../../../../../include/hkpMaterial.h"
#include "../../../../../include/hkpContactListener.h"
#include "../../../../../include/hkArray.h"
#include "../../../../../include/hkBool.h"
#include "../../../../../include/hkSmallArray.h"
#include "../../../../../include/hkpCollisionDispatcher.h"
#include "../../../../../include/hkpConstraintInstance.h"
#include "../../../../../include/hkMemoryAllocator.h"
#include "../../../../../include/hkMemoryRouter.h"
#include "../../../../../include/FinishingFlag.h"
#include "../../../../../include/hkBaseObject.h"
#include "../../../../../include/hkReferencedObject.h"
#include "../../../../../include/hkStatisticsCollector.h"
#include "../../../../../include/hkStreamWriter.h"
#include "../../../../../include/hkStringPtr.h"
#include "../../../../../include/hkpBoxMotion.h"
#include "../../../../../include/hkpMotion.h"
#include "../../../../../include/hkpThinBoxMotion.h"
#include "../../../../../include/hkpWorldObject.h"
#include "../../../../../include/hkpMaxSizeMotion.h"
#include "../../../../../include/hkpSphereMotion.h"
#include "../../../../../include/hkpEntityExtendedListeners.h"

inline void* operator new(size_t, void* p)
{
    return p;
}

// Preserve the inline construction used by this unit's finishing constructors.
inline hkpMaterial::hkpMaterial(FinishingFlag) {}

inline hkMemoryAllocator& hkContainerHeapAllocator::get(const void*)
{
    return hkMemoryRouter::getInstance().heap();
}

template <typename T> __forceinline void releaseArrayStorage(hkArrayBase<T>& array, int elementSize)
{
    hkMemoryAllocator& alloc = hkMemoryRouter::getInstance().heap();
    array.m_iElementCount = 0;
    if ((array.m_iCapacityState & 0x80000000) == 0)
        alloc.bufFree(array.m_data, (array.m_iCapacityState & 0x3fffffff) * elementSize);
    array.m_data = 0;
    array.m_iCapacityState = 0x80000000;
}

template <typename T, typename Allocator> __forceinline hkArray<T, Allocator>::~hkArray()
{
    releaseArrayStorage(*this, sizeof(T));
}

template <typename T> __forceinline void hkArrayBase<T>::GrowArrayCapacity(hkMemoryAllocator& kHeap, int iWanted)
{
    const int iHave = getCapacity();
    if (iHave < iWanted)
    {
        int iTwice = 2 * iHave;
        int iGrowTo = (iWanted < iTwice) ? iTwice : iWanted;
        hkArrayUtil::_reserve(kHeap, this, iGrowTo, sizeof(T));
    }
}

template <typename T> __forceinline void hkArrayBase<T>::_setSize(hkMemoryAllocator& alloc, int n)
{
    GrowArrayCapacity(alloc, n);
    m_iElementCount = n;
}
inline hkQuaternion::hkQuaternion() {}
inline hkTransform::hkTransform() {}
inline QuantizedFloat8::QuantizedFloat8() {}
inline hkMotionState::hkMotionState() {}
template <typename E, typename S> inline hkEnum<E, S>::hkEnum() {}
inline hkpKeyframedRigidMotion::hkpKeyframedRigidMotion() : hkpMotion(FinishingFlag(0))
{
    m_savedMotion = 0;
}
static inline void setGravityHalf(hkHalf& half, float value)
{
    int bits = reinterpret_cast<const int*>(&value)[0];
    half.m_value = static_cast<short>(bits >> 16);
}
inline hkpMotion::hkpMotion(FinishingFlag flag) : hkReferencedObject(flag)
{
    if (flag.m_bFinishing)
        setGravityHalf(m_gravityFactor, 1.0f);
}

// @fnv 0x00c9c380
// src: HAVOK_LIB 0x00c9c380
// pdb: hkpRigidBody::getCollisionFilterInfo | bhkCharacterController.obj | debug, call graph, medium
__declspec(noinline) int hkpEntity::getNumConstraints() throw()
{
    return m_constraintsSlave.m_iElementCount + m_constraintsMaster.m_iElementCount;
}

// Returns the entry at index from the first array if index is below the count at offset 0xb0, otherwise from the second array.
// @fnv 0x00c9c3c0
int hkpEntity::GetEntryAt(int index) throw()
{
    if (index < m_constraintsMaster.m_iElementCount)
    {
        int offset = index * 3;
        offset <<= 4;
        return *reinterpret_cast<int*>(static_cast<char*>(m_constraintsMaster.m_data) + offset);
    }
    else
    {
        int offset = index - m_constraintsMaster.m_iElementCount;
        return reinterpret_cast<int*>(m_constraintsSlave.m_data)[offset];
    }
}

struct FUN_00c9c1a0_Value
{
    int m_00;
    int m_04;
    int m_08;
};

struct FUN_00c9c1a0_Item
{
    FUN_00c9c1a0_Value* m_00;
    int m_04;
};

// Scans the m_60 item array for an entry whose m_04 equals value + 0x10 and returns that entry's m_00->m_08, or 0 if none matches.
// @fnv 0x00c9c1a0
int __fastcall findCollisionEntryValue(hkpEntity* self, int, int value)
{
    int index = 0;
    if (self->m_kCollidable.m_collisionEntries.m_iElementCount > 0)
    {
        int key = value + 0x10;
        FUN_00c9c1a0_Item* item = reinterpret_cast<FUN_00c9c1a0_Item*>(self->m_kCollidable.m_collisionEntries.m_data);
        do
        {
            if (item->m_04 == key)
                return item->m_00->m_08;
            ++index;
            ++item;
        } while (index < self->m_kCollidable.m_collisionEntries.m_iElementCount);
    }
    return 0;
}

struct FUN_00c9d490_Allocator
{
    virtual void m_00() = 0;
    virtual void m_04() = 0;
    virtual void m_08(void* object, unsigned short type) = 0;
};

struct FUN_00c9d490_Router
{
    void* m_00;
    void* m_04;
    void* m_08;
    void* m_0c;
    FUN_00c9d490_Allocator* m_10;
};

extern FUN_00c9d490_Router* __cdecl _D1A0E0GetMemoryRouter();

namespace fnv_00c9cf60 { void __fastcall destroyEntity(hkpEntity* self); }

// @fnv 0x00c9d490
// pdb: hkpEntity::`scalar deleting destructor' | hkpEntity.obj | retail, vtable slot, high
void* __fastcall deleteEntity(hkpEntity* self, int, int flags)
{
    fnv_00c9cf60::destroyEntity(self);

    if (flags & 1)
    {
        FUN_00c9d490_Router* router = _D1A0E0GetMemoryRouter();
        FUN_00c9d490_Allocator* allocator = router->m_10;
        allocator->m_08(self, self->m_memSizeAndFlags);
    }

    return self;
}

// @fnv 0x00c9c020
const ResizableArray<ConstraintRuntimeInfo>& hkpEntity::getConstraintMastersImpl() const
{
    return reinterpret_cast<const ResizableArray<ConstraintRuntimeInfo>&>(m_constraintsMaster);
}

// @fnv 0x00c9c030
char* __fastcall getConstraintMasters(hkpEntity* self)
{
    return reinterpret_cast<char*>(&self->m_constraintsMaster);
}

// @fnv 0x00c9c3f0
char* __fastcall getConstraintSlaves(hkpEntity* self)
{
    return reinterpret_cast<char*>(&self->m_constraintsSlave);
}

void __cdecl hkpEntityAabbUtil_entityBatchRecalcAabb(int collisionInput, int* entityBatch, int numEntities);

// If m_08 and m_10 are set, calls FUN_00d1a330 with m_08->m_78, a pointer to this, and 1.
// @fnv 0x00c9bfa0
void __fastcall synchronizeEntity(hkpEntity* self)
{
    if (self->m_world && self->m_kCollidable.m_pCollisionShape)
    {
        hkpEntity* entity = self;
        hkpEntityAabbUtil_entityBatchRecalcAabb(reinterpret_cast<int*>(self->m_world)[0x78 / 4], (int*)&entity, 1);
    }
}

struct hkWorldOperationBaseC9C3
{
    unsigned char m_type;
};

struct hkWorldOperationC9C3 : public hkWorldOperationBaseC9C3
{
    hkpEntity* m_pEntity;

    hkWorldOperationC9C3()
    {
        m_type = 0x21;
    }
};

// @fnv 0x00c9c300
void __fastcall activateEntity(hkpEntity* self, int)
{
    if (static_cast<hkpWorld*>(self->m_world) && static_cast<hkpWorld*>(self->m_world)->m_criticalOperationsLockCount)
    {
        hkWorldOperationC9C3 kOp;
        kOp.m_pEntity = self;
        static_cast<hkpWorld*>(self->m_world)->hkpWorld_queueOperation(&kOp);
        return;
    }
    self->activate_00c9c1d0();
}

struct WorldOp
{
    unsigned char m_type;
    void* m_pEntity;
};

void __fastcall deactivateEntityImmediately(hkpEntity* self);

// @fnv 0x00c9c340
void __fastcall deactivateEntity(hkpEntity* self)
{
    hkpWorld* world = static_cast<hkpWorld*>(self->m_world);
    if (world && world->m_criticalOperationsLockCount)
    {
        WorldOp op;
        op.m_pEntity = self;
        op.m_type = 0x22;
        world->hkpWorld_queueOperation(&op);
        return;
    }
    deactivateEntityImmediately(self);
}

// Returns the entry at index from the inline array when index is below m_b0, otherwise from the m_b4 array offset by m_b0.
// @fnv 0x00c9c390
// pdb: __restgprlr | crtgpr.obj | debug, call graph, medium
int __fastcall getEntityConstraint(hkpEntity* self, int, int index)
{
    if (index < self->m_constraintsMaster.m_iElementCount)
        return ((int*)self->m_constraintsMaster.m_data)[(index + index * 2) * 4];
    return reinterpret_cast<int*>(self->m_constraintsSlave.m_data)[index - self->m_constraintsMaster.m_iElementCount];
}

class GlobalObjectO2_00d472a0;
extern GlobalObjectO2_00d472a0 g_012688c8;
extern GlobalObjectO2_00d472a0 g_01268808;

void __cdecl hkStatisticsCollectorUtil_addClassContents(const void* obj, const GlobalObjectO2_00d472a0* cls,
                                   hkStreamWriter* collector);

// Declaration-only extension: the shared statistics interface lacks this hkStreamWriter overload.
class hkpEntityStats : public hkpEntity
{
  public:
    void WriteToStream(hkStreamWriter* collector, const GlobalObjectO2_00d472a0* cls) const;
    void CollectStats(hkStreamWriter* collector, const hkClass* cls) const;
};

// Reports the entity's constraint, collision, listener and motion data to the writer, including flagged arrays when its serialization mode requests them.
// @fnv 0x00c9c400
// pdb: hkpEntity::calcContentStatistics | hkpEntity.obj | retail, string-set bridge, high
// pdb params: collector, cls
void hkpEntityStats::CollectStats(hkStreamWriter* collector, const hkClass* cls) const
{
    collector->WriteArray(3, "constraintsSlave", m_constraintsSlave.m_data, m_constraintsSlave.m_iElementCount * 4,
                              reinterpret_cast<void*>((m_constraintsSlave.m_iCapacityState & 0x3fffffff) * 4));
    hkStatisticsCollectorUtil_addClassContents(this, &g_012688c8, collector);
    if ((m_contactListeners.m_iCapacityState & 0x8000) == 0 || reinterpret_cast<const char*>(collector)[0x10])
    {
        collector->WriteArray(3, "CollisionListnr", m_contactListeners.m_data, m_contactListeners.m_iElementCount * 4,
                                  reinterpret_cast<void*>((m_contactListeners.m_iCapacityState & 0x3fff) * 4));
    }
    if (m_extendedListeners)
    {
        const hkSmallArray<hkpEntityListener*>& entityListeners = m_extendedListeners->m_entityListeners;
        if ((entityListeners.m_capacityAndFlags & 0x8000) == 0 || reinterpret_cast<const char*>(collector)[0x10])
        {
            collector->WriteArray(3, "EntityListeners", entityListeners.m_data, entityListeners.m_size * 4,
                                      reinterpret_cast<void*>((entityListeners.m_capacityAndFlags & 0x3fff) * 4));
        }
        const hkSmallArray<hkpEntityActivationListener*>& activationListeners =
            m_extendedListeners->m_activationListeners;
        if ((activationListeners.m_capacityAndFlags & 0x8000) == 0 || reinterpret_cast<const char*>(collector)[0x10])
        {
            collector->WriteArray(3, "ActivatonListners", activationListeners.m_data,
                                      activationListeners.m_size * 4,
                                      reinterpret_cast<void*>((activationListeners.m_capacityAndFlags & 0x3fff) * 4));
        }
    }
    collector->WriteField("SavedMotion", m_motion.m_savedMotion, 0);
    WriteToStream(collector, &g_01268808);
}

void __cdecl hkSmallArrayUtil__reserveMore(void* array, int sizeElem);

inline int findEmptyContactSlot(const hkpEntitySmallArraySerializeOverrideType& slots)
{
    for (int i = 0; i < slots.m_iElementCount; ++i)
        if (static_cast<void**>(slots.m_data)[i] == 0)
            return i;
    return -1;
}

inline void appendContactSlot(hkpEntitySmallArraySerializeOverrideType& slots, void* const& value)
{
    unsigned short capacity = static_cast<unsigned short>(slots.m_iCapacityState & 0x3fff);
    if (slots.m_iElementCount == capacity)
        hkSmallArrayUtil__reserveMore(&slots, sizeof(void*));
    static_cast<void**>(slots.m_data)[slots.m_iElementCount++] = value;
}

// Finds the first empty contact-listener slot or appends one.
// @fnv 0x00c9c540
// src: XBOX_RETAIL_PDB 0x00c9c540
// pdb: hkpEntity::addContactListener | hkpEntity.obj | retail, object-file gap fill, high
// pdb params: cl
// pdb locals: assertBuf, ostr
void hkpEntity::addContactListener(void* cl)
{
    int i = findEmptyContactSlot(m_contactListeners);
    if (i >= 0)
        static_cast<void**>(m_contactListeners.m_data)[i] = cl;
    else
        appendContactSlot(m_contactListeners, cl);
}

// A constraint-master record is 0x30 bytes; the approved headers leave this entity array as void storage.
struct ConstraintMasterRecord
{
    hkpConstraintInstance* m_00;
    int m_04;
    int m_08;
    int m_0c;
    int m_10;
    int m_14;
    int m_18;
    int m_1c;
    unsigned short m_20;
    unsigned short m_22;
    int m_24;
    int m_28;
    int m_2c;
};

void hkpConstraintInstance_SortConstraintInstances(void* a1, int a2, int a3, void* a4);
// @fnv 0x00c9c860
// src: XBOX_RETAIL_PDB 0x00c9c860
// pdb: hkpEntity::sortConstraintsSlavesDeterministically | hkpEntity.obj | retail, call graph, high
// pdb locals: isWarningEnabled
void __fastcall hkpEntity_sortConstraintsSlavesDeterministically(hkpEntity* self, int)
{
    if (*reinterpret_cast<const hkUint8*>(&self->m_motion.m_type) != 5)
    {
        return;
    }
    if (self->m_constraintsSlave.m_iElementCount > 1)
    {
        hkpConstraintInstance_SortConstraintInstances(self->m_constraintsSlave.m_data, 0, self->m_constraintsSlave.m_iElementCount - 1, self);
    }
    for (int i = 0; i < self->m_constraintsSlave.m_iElementCount; ++i)
    {
        hkpConstraintInstance* node = self->m_constraintsSlave.m_data[i];
        hkpEntity* owner =
            reinterpret_cast<hkpEntity*>(reinterpret_cast<unsigned int>(node->m_entities[1]) ^ reinterpret_cast<unsigned int>(node->m_entities[0]) ^ reinterpret_cast<unsigned int>(self));
        for (int k = 0; k < owner->m_constraintsMaster.m_iElementCount; ++k)
        {
            if (static_cast<ConstraintMasterRecord*>(owner->m_constraintsMaster.m_data)[k].m_00 == node)
            {
                static_cast<ConstraintMasterRecord*>(owner->m_constraintsMaster.m_data)[k].m_22 = (unsigned short)i;
                break;
            }
        }
    }
}

extern const int DAT_010c6c64;
extern const int DAT_010c6f7c;

// The shared material has a finishing constructor; build the ordinary defaults through its copy interface.
inline hkpMaterial makeDefaultMaterial()
{
    hkpMaterial material(FinishingFlag(0));
    material.m_friction = 0.5f;
    material.m_restitution = 0.4f;
    return material;
}

// The shared callback is plain storage; this helper supplies the entity constructor's callback defaults.
struct DefaultSpuCollisionCallback : public hkpEntitySpuCollisionCallback
{
    DefaultSpuCollisionCallback()
    {
        m_pCallbackData = 0;
        m_pRayCursor = 0;
        m_uiCallbackType = hkpEntity::SPU_SEND_CONTACT_POINT_ADDED_OR_PROCESS;
        m_bCallbackEnabled = 1;
    }
};

// The shared entity exposes the local-frame reference as a raw pointer; initialize it during member construction.
struct EmptyLocalFrameReference
{
    hkLocalFrame* m_pRetainedObject;
    EmptyLocalFrameReference() : m_pRetainedObject(0) {}
};

// This helper is only the 0x8c-byte world-object base, with the entity vtable installed before member construction.
class __declspec(novtable) EntityShapeConstructionBase : public hkpWorldObject
{
  public:
    EntityShapeConstructionBase(const hkpShape* pkShape) throw() : hkpWorldObject(pkShape, hkpWorldObject::BROAD_PHASE_ENTITY)
    {
        *(const int**)this = &DAT_010c6f7c;
    }
};

// hkpEntity has no shared shape constructor; retain only this constructor layout until one is approved.
class __declspec(novtable) FUN_00c9cce0 : public EntityShapeConstructionBase
{
  public:
    hkpMaterial m_iMaterialIndex;
    void* m_pEntityUserData;
    hkReal m_fBulletSpread;
    void* m_pShape;
    hkUint32 m_uiFilterInfo;
    hkUint16 m_usStorageIndex;
    hkUint16 m_usNextProcessCallbackDelay;
    hkSmallArray<void*> m_eSmallArraySerializeOverrideType;
    hkArray<hkpConstraintInstance*> m_kSlaveConstraints;
    hkArray<hkUint8> m_pByteBuffer;
    void* m_pSimulationIsland;
    hkInt8 m_iAutoRemoveLevel;
    hkUint8 m_ucNumShapeKeysInContactPointProperties;
    hkUint8 m_ucResponseModifierFlags;
    hkUint32 m_uiUniqueId;
    DefaultSpuCollisionCallback m_kSpuCollisionCallback;
    hkpMaxSizeMotion m_pMotionState;
    hkSmallArray<void*> m_kContactListeners;
    hkSmallArray<void*> m_ppActions;
    EmptyLocalFrameReference m_pLocalFrame;
    hkpEntityExtendedListeners* m_pExtendedListeners;

    FUN_00c9cce0(const hkpShape* shape) throw();
};

// Constructs an hkpEntity from a shape, initialising its material, motion, arrays and listener pointers to defaults.
// @fnv 0x00c9cce0
// pdb: hkpEntity::hkpEntity | hkpEntity.obj | retail, call graph, high
// pdb params: shape
FUN_00c9cce0::FUN_00c9cce0(const hkpShape* shape) throw()
    : EntityShapeConstructionBase(shape), m_iMaterialIndex(makeDefaultMaterial())
{
    m_usStorageIndex = 0xffff;
    m_fBulletSpread = 1.0f;
    m_pSimulationIsland = 0;
    m_pExtendedListeners = 0;
    m_pEntityUserData = 0;
    m_uiFilterInfo = 0;
    m_ucNumShapeKeysInContactPointProperties = 0;
    m_pShape = 0;
    m_uiUniqueId = 0xffffffff;
}

template <typename Array> inline void initializeEntitySmallArray(Array& array)
{
    array.m_data = 0;
    array.m_iElementCount = 0;
    array.m_iCapacityState = 0x8000;
}

template <typename T> inline void initializeEntityArray(hkArray<T>& array)
{
    array.m_data = 0;
    array.m_iElementCount = 0;
    array.m_iCapacityState = 0x80000000;
}

extern int DAT_010c59f4;
extern int DAT_010c6bec;

// @fnv 0x00c9d100
hkpKeyframedRigidMotion* __fastcall finishKeyframedMotion(hkpKeyframedRigidMotion* self, int, int enabled)
{
    *reinterpret_cast<int* volatile*>(self) = &DAT_010c59f4;
    if (enabled != 0)
    {
        *reinterpret_cast<volatile float*>(&enabled) = 1.0f;
        self->m_gravityFactor.m_value = static_cast<short>(enabled >> 16);
    }
    *reinterpret_cast<int* volatile*>(self) = &DAT_010c6bec;
    return self;
}

struct FnvCpu_FUN_00c9d160_CpuAllocator
{
    virtual void slot00();
    virtual void slot04();
    virtual void Invoke(void* block, unsigned int size);
};
struct FnvCpu_FUN_00c9d160_CpuAllocatorHolder
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0c;
    FnvCpu_FUN_00c9d160_CpuAllocator* allocator;
};
FnvCpu_FUN_00c9d160_CpuAllocatorHolder* __cdecl hkMemoryRouter_getInstance();
// @fnv 0x00c9d160
// pdb: hkpKeyframedRigidMotion::`scalar deleting destructor' | hkpEntity.obj | retail, vtable slot, high
// pdb params: this
hkpMotion* __fastcall deleteMotion(hkpMotion* self, int, unsigned int flags)
{
    self->hkpMotion::~hkpMotion();
    if (flags & 1)
    {
        FnvCpu_FUN_00c9d160_CpuAllocatorHolder* holder = hkMemoryRouter_getInstance();
        holder->allocator->Invoke(self, self->m_memSizeAndFlags);
    }
    return self;
}

extern int DAT_010c59f4;
extern int DAT_010c6d54;
// @fnv 0x00c9d190
hkpSphereMotion* __fastcall finishSphereMotion(hkpSphereMotion* self, int, int flag)
{
    *reinterpret_cast<int* volatile*>(self) = &DAT_010c59f4;
    if (flag)
    {
        float one = 1.0f;
        self->m_gravityFactor.m_value = static_cast<short>(*reinterpret_cast<int*>(&one) >> 16);
    }
    *reinterpret_cast<int* volatile*>(self) = &DAT_010c6d54;
    return self;
}

// Finishing helpers retain the game's scalar flag ABI and vtable write order.
// @fnv 0x00c9d1c0
hkpBoxMotion* __fastcall finishBoxMotion(hkpBoxMotion* self, int, int flag)
{
    extern const int DAT_010c6dc4;
    *reinterpret_cast<const int**>(self) = &DAT_010c59f4;
    if (flag)
    {
        *reinterpret_cast<float*>(&flag) = 1.0f;
        self->m_gravityFactor.m_value = static_cast<short>(flag >> 16);
    }
    *reinterpret_cast<const int* volatile*>(self) = &DAT_010c6dc4;
    return self;
}

// The shared small array does not yet declare its deallocation destructor.
// Keep that operation here using the shared storage and allocator definitions.
template <typename T> inline void deallocateListenerArray(T* data, int count)
{
    hkMemoryRouter::getInstance().heap().blockFree(data, count * sizeof(T));
}

template <typename T> inline void releaseListenerArray(hkSmallArray<T>& array)
{
    if ((array.m_capacityAndFlags & 0x8000) == 0)
        deallocateListenerArray(array.m_data, array.m_capacityAndFlags & 0x3fff);
}

// @fnv 0x00c9d340
// pdb: hkpLinkedCollidable::~hkpLinkedCollidable | hkpEntity.obj | retail, object-file run extension, low
void __fastcall destroyExtendedListeners(hkpEntityExtendedListeners* listeners)
{
    releaseListenerArray(listeners->m_entityListeners);
    releaseListenerArray(listeners->m_activationListeners);
}

namespace fnv_00c9d460 {
struct FnvCpu_FUN_00c9d460_CpuAllocator
{
    virtual void slot00();
    virtual void slot04();
    virtual void Invoke(void* block, unsigned int size);
};
struct FnvCpu_FUN_00c9d460_CpuAllocatorHolder
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0c;
    FnvCpu_FUN_00c9d460_CpuAllocator* allocator;
};
FnvCpu_FUN_00c9d460_CpuAllocatorHolder* __cdecl hkMemoryRouter_getInstance();
struct FnvCpu_FUN_00c9d460_CpuObject
{
    unsigned int unk00;
    unsigned short size;
    void FUN_00c9d3b0();
    FnvCpu_FUN_00c9d460_CpuObject* FUN_00c9d460(unsigned int flags);
};
// @fnv 0x00c9d460
// pdb: hkpWorldObject::`scalar deleting destructor' | hkpEntity.obj | retail, vtable slot, high
FnvCpu_FUN_00c9d460_CpuObject* FnvCpu_FUN_00c9d460_CpuObject::FUN_00c9d460(unsigned int flags)
{
    FUN_00c9d3b0();
    if (flags & 1)
    {
        FnvCpu_FUN_00c9d460_CpuAllocatorHolder* holder = hkMemoryRouter_getInstance();
        holder->allocator->Invoke(this, size);
    }
    return this;
}
}  // namespace fnv_00c9d460

namespace fnv_00d1f780 {
class FUN_00d08130
{
  public:
    virtual void v00();
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
    int unkF8;
    int unkFC;
    int unk100;
    int unk104;
    int unk108;
    int unk10C;
    int unk110;
    int unk114;
    int unk118;
    int unk11C;
    int unk120;
    int unk124;
    int unk128;
    int unk12C;
    int unk130;
    int unk134;
    int unk138;
    int unk13C;
    int unk140;
    int unk144;
    int unk148;
    int unk14C;
    int unk150;
    int unk154;
    int unk158;
    int unk15C;
    ~FUN_00d08130() throw();
};

class FUN_00d1f780 : public FUN_00d08130
{
  public:
    virtual ~FUN_00d1f780() throw();

    hkArray<void*> m_ppData;
};

// Destructor that releases the array member by freeing its heap buffer and resetting it.
// @fnv 0x00d1f780
// pdb: hkpSimpleShapePhantom::~hkpSimpleShapePhantom | hkpSimpleShapePhantom.obj | retail, object-file gap fill, high
FUN_00d1f780::~FUN_00d1f780() throw()
{
}
}  // namespace fnv_00d1f780

struct S00c92a80_Obj
{
    virtual void v0();
    virtual void v4();
    virtual void v8();
    virtual void vc();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c(int a, int b, void* c, void* d, int n);
};

struct __declspec(align(16)) S00c92a80_Range
{
    float lo;
    float hi;
    float range;
    float inv;
    S00c92a80_Range(float b, float a)
    {
        lo = a;
        hi = b;
        range = b - a;
        inv = range == 0.0f ? 0.0f : 1.0f / range;
    }
};

struct S00c92a80_cls
{
    int unk00;
    int unk04;
    S00c92a80_Obj* obj;
    void hkpWorld_findInitialContactPoints(int entities, int numEntities);
};

// Reads a float from obj, builds a S00c92a80_Range, and calls obj->v1c with a0, a1, this, the range, and 1.
// @fnv 0x00c92a80
// src: HAVOK_LIB 0x00c92a80
// pdb: hkpWorld::findInitialContactPoints | hkpWorld.obj | retail, upstream name already matches, high
// pdb params: entities, numEntities
// pdb locals: stepInfo
void S00c92a80_cls::hkpWorld_findInitialContactPoints(int entities, int numEntities)
{
    float b = *(float*)((char*)obj + 0x18);
    float a = *(float*)((char*)obj + 0x18);
    S00c92a80_Range r(b, a);
    obj->v1c(entities, numEntities, this, &r, 1);
}

// @fnv 0x00c9bfd0
// src: HAVOK_LIB 0x00c9bfd0
static __declspec(noinline) unsigned char* hkEntity_isActive(hkpEntity* self, unsigned char* out)
{
    hkpSimulationIsland* p = self->m_simulationIsland;
    if (!p)
        *(unsigned char*)out = 0;
    else
        *(unsigned char*)out = (p->flags26 >> 2) & 3;
    return out;
}

extern "C" unsigned char* c9bfd0_out_val(void);
void c9bfd0_caller(hkpEntity* self)
{
    (void)hkEntity_isActive(self, c9bfd0_out_val());
}

// Writes a 2-bit value extracted from byte 0x26 of m_cc into out, or 0 if m_cc is null.
// @fnv 0x00c9bff0
// src: HAVOK_LIB 0x00c9bff0
// pdb: bhkRigidBody::IsActive | TESObjectREFR.obj | retail, call graph, medium
// pdb locals: pb
unsigned char* hkpEntity::isActive_00c9bff0(unsigned char* out)
{
    if (m_simulationIsland == 0)
    {
        *out = 0;
        return out;
    }
    *out = (((unsigned char*)m_simulationIsland)[0x26] >> 2) & 3;
    return out;
}

class hkBoundingVolumeC9C0
{
  public:
    int m_dirty;
    int unk04, unk08, unk0C;
    int m_pChildShapeAabbs;
    int unk14, unk18, unk1C, unk20;
    int m_data;

    void FreeStorage();
    void FUN_00d01ae0(int iNumChildren);
    void Reset(int iNumChildren)
    {
        FUN_00d01ae0(iNumChildren);
        m_dirty = 1;
        m_pChildShapeAabbs = 0;
    }
};

// Frees the current bounding volume storage, then resets it from pkShape's container when the shape flags indicate a container shape.
// @fnv 0x00c9c040
// pdb: hkpEntity::setCachedShapeData | hkpEntity.obj | debug, call graph, medium
// pdb params: world, shape
void __fastcall updateEntityBoundingVolume(hkpEntity* self, int, const hkpWorld* pkWorld, const hkpShape* pkShape)
{
    const hkUint8& motionType = *reinterpret_cast<const hkUint8*>(&self->m_motion.m_type);
    if (reinterpret_cast<hkBoundingVolumeC9C0*>(&self->m_kCollidable.m_boundingVolumeData)->m_data)
        reinterpret_cast<hkBoundingVolumeC9C0*>(&self->m_kCollidable.m_boundingVolumeData)->FreeStorage();
    if (!pkWorld || !pkShape)
        return;

    int iShapeFlags = static_cast<hkpCollisionDispatcher*>(pkWorld->m_collisionDispatcher)->m_auiAlternateTypeFlags[*reinterpret_cast<const hkUint32*>(&pkShape->m_type)];
    bool m_bIsOptional = false;
    const hkpShapeContainer* pkContainer;
    if (iShapeFlags & 0x80)
    {
        pkContainer = pkShape->GetShapeContainer();
    }
    else if (iShapeFlags & 0x100)
    {
        pkContainer = pkShape->GetShapeContainer();
        m_bIsOptional = true;
    }
    else
    {
        return;
    }

    if ((motionType == 5 || motionType == 4) && m_bIsOptional)
        return;

    reinterpret_cast<hkBoundingVolumeC9C0*>(&self->m_kCollidable.m_boundingVolumeData)->Reset(pkContainer->GetNumChildShapes());
    if (motionType == 5)
        synchronizeEntity(self);
}

// Finds the given listener in m_kListeners and sets its slot to 0.
// @fnv 0x00c9c0f0
void hkpEntity::RemoveEntityListener(void* value)
{
    hkSmallArray<hkpEntityActivationListener*>& listeners = m_extendedListeners->m_activationListeners;
    hkpEntityActivationListener* listener = static_cast<hkpEntityActivationListener*>(value);
    int i = listeners.indexOf(listener);
    listeners.m_data[i] = 0;
}

// Sets the entry at the index found by indexOf to 0 in m_entries.
// @fnv 0x00c9c140
// pdb: hkpEntity::removeContactListener | hkpEntity.obj | debug, call graph, medium
// pdb params: cl
// pdb locals: assertBuf, ostr
void hkpEntity::RemoveContactListener(void* entry)
{
    hkSmallArray<void*>& entries = reinterpret_cast<hkSmallArray<void*>&>(m_contactListeners);
    entries.m_data[entries.indexOf(entry)] = 0;
}

void __cdecl hkpWorldOperationUtil_markIslandActive(hkpWorld* world, hkpSimulationIsland* island);

// Deactivation flag setup is not declared by the shared motion interface.

// @fnv 0x00c9c1d0
// src: HAVOK_LIB 0x00c9c1d0
void hkpEntity::activate_00c9c1d0()
{
    hkBool active(char(0));
    if (m_simulationIsland)
        active = hkBool(static_cast<char>(m_simulationIsland->f26.f26mid));
    if (!active && *reinterpret_cast<const hkUint8*>(&m_motion.m_type) != 5 && m_world)
    {
        m_motion.m_deactivationNumInactiveFrames[0] = 0;
        m_motion.m_deactivationNumInactiveFrames[1] = 0;
        hkpWorldOperationUtil_markIslandActive(static_cast<hkpWorld*>(m_world), m_simulationIsland);
        // Solver-info deactivation settings are not yet named in the shared world header.
        m_motion.SetFlagBits(reinterpret_cast<const hkUint8*>(m_world)[0x305],
                                   reinterpret_cast<const hkUint8*>(m_world)[0x306],
                                   reinterpret_cast<const hkUint8*>(m_world)[0x307]);
    }
}

// Ensures m_pExtendedListeners is allocated, then nulls the entry for pvListener in m_pExtendedListeners->m_entityListeners.
// @fnv 0x00c9c670
// src: HAVOK_LIB 0x00c9c670
// pdb: hkpEntity::removeEntityListener | hkpEntity.obj | debug, call graph, medium
// pdb params: el
// pdb locals: assertBuf, ostr
void hkpEntity::removeEntityListener_00c9c670(void* pvListener)
{
    if (!m_extendedListeners)
        m_extendedListeners = new hkpEntityExtendedListeners;
    hkSmallArray<hkpEntityListener*>& kArray = m_extendedListeners->m_entityListeners;
    hkpEntityListener* listener = static_cast<hkpEntityListener*>(pvListener);
    kArray[kArray.indexOf(listener)] = 0;
}

// @fnv 0x00c9c7c0
// src: XBOX_RETAIL_PDB 0x00c9c7c0
// pdb: hkpEntity::getAllConstraints | hkpEntity.obj | retail, object-file gap fill, high
// pdb params: constraints
void __fastcall hkpEntity_getAllConstraints(hkpEntity* self, int, hkArray<hkpConstraintInstance*>& constraints)
{
    constraints.setSize(self->getNumConstraints());
    int k = 0;
    for (int i = 0; i < self->m_constraintsMaster.m_iElementCount; i++)
        // The approved headers expose this array as void storage; hkConstraintInternal is 0x30 bytes.
        constraints[k++] = *reinterpret_cast<hkpConstraintInstance**>(
            static_cast<char*>(self->m_constraintsMaster.m_data) + i * 0x30);
    for (int i = 0; i < self->m_constraintsSlave.m_iElementCount; i++)
        constraints[k++] = self->m_constraintsSlave.m_data[i];
}

extern int DAT_010c59f4;
extern const int DAT_010c6c64;
extern const int DAT_010c6f7c;
inline void setHalfC9CE(hkHalf& kHalf, const float& f)
{
    int t = ((const int*)&f)[0];
    kHalf.m_value = short(t >> 16);
}
// The shared sphere interface has no finishing constructor.
class SphereMotionFinish : public hkpSphereMotion
{
  public:
    explicit SphereMotionFinish(int flag) throw();
};
class FUN_00c9d100
{
  public:
    FUN_00c9d100(int flag) throw();
};
class FUN_00c9d130
{
  public:
    FUN_00c9d130(int flag) throw();
};
class FUN_00c9d220
{
  public:
    FUN_00c9d220(int flag) throw();
};

// The approved world-object constructor still names an incomplete, unit-specific flag type.
// Its 0x00cd2c50 ABI carries the same single integer as the shared FinishingFlag.
hkpWorldObject* __fastcall hkpWorldObject_ctor(hkpWorldObject* self, int, FinishingFlag flag);

inline void finishMaxSizeMotion(hkpMaxSizeMotion& motion, int flag)
{
    *reinterpret_cast<int**>(&motion) = &DAT_010c59f4;
    if (flag)
        setHalfC9CE(motion.m_gravityFactor, 1.0f);
    *reinterpret_cast<const int**>(&motion) = &DAT_010c6c64;
}

// Constructs the base and m_pMotionState from flag, then if flag is set re-constructs m_pMotionState as the motion class matching its m_type.
// @fnv 0x00c9ce00
// src: XBOX_RETAIL_PDB 0x00c9ce00
// pdb: hkpEntity::hkpEntity | hkpEntity.obj | retail, call graph (shared callers), high
// pdb params: shape
hkpEntity* __fastcall hkpEntity_ctor(hkpEntity* self, int unused, int flag)
{
    hkpWorldObject_ctor(self, unused, FinishingFlag(*(volatile int*)&flag));
    *reinterpret_cast<const int**>(self) = &DAT_010c6f7c;
    initializeEntitySmallArray(self->m_constraintsMaster);
    initializeEntityArray(self->m_constraintsSlave);
    initializeEntityArray(self->m_constraintRuntime);
    finishMaxSizeMotion(self->m_motion, flag);
    initializeEntitySmallArray(self->m_contactListeners);
    initializeEntitySmallArray(self->m_actions);
    if (flag)
    {
        switch (*reinterpret_cast<const hkUint8*>(&self->m_motion.m_type))
        {
        case hkpMotion::MOTION_SPHERE_INERTIA:
            ::new (&self->m_motion) SphereMotionFinish(flag);
            break;
        case hkpMotion::MOTION_BOX_INERTIA:
            ::new (&self->m_motion) hkpBoxMotion(FinishingFlag(flag));
            break;
        case hkpMotion::MOTION_KEYFRAMED:
            ::new (&self->m_motion) FUN_00c9d100(flag);
            break;
        case hkpMotion::MOTION_FIXED:
            ::new (&self->m_motion) FUN_00c9d130(flag);
            break;
        case hkpMotion::MOTION_THIN_BOX_INERTIA:
            ::new (&self->m_motion) hkpThinBoxMotion(FinishingFlag(flag));
            break;
        case hkpMotion::MOTION_CHARACTER:
            ::new (&self->m_motion) FUN_00c9d220(flag);
            break;
        }
    }
    return self;
}

// @fnv 0x00c9d130
hkpMotion* __fastcall finishFixedMotion(hkpMotion* self, int, int flag)
{
    extern const int DAT_010c6cdc;
    *reinterpret_cast<const int**>(self) = &DAT_010c59f4;
    if (flag)
    {
        *reinterpret_cast<float*>(&flag) = 1.0f;
        self->m_gravityFactor.m_value = static_cast<short>(flag >> 16);
    }
    *reinterpret_cast<const int* volatile*>(self) = &DAT_010c6cdc;
    return self;
}

namespace fnv_00c9d220 {
struct MotionCtorBase : public hkpMotion
{
    virtual ~MotionCtorBase();
    MotionCtorBase(int flag) : hkpMotion(FinishingFlag(0))
    {
        if (flag)
        {
            *reinterpret_cast<float*>(&flag) = 1.0f;
            volatile short& gravityFactor = m_gravityFactor.m_value;
            gravityFactor = static_cast<short>(flag >> 16);
        }
    }
};
struct FUN_00c9d220 : public MotionCtorBase
{
    virtual ~FUN_00c9d220();
    FUN_00c9d220(int flag);
};
// Constructs a motion object by delegating to MotionCtorBase with the given flag.
// @fnv 0x00c9d220
FUN_00c9d220::FUN_00c9d220(int flag) : MotionCtorBase(flag)
{}
}  // namespace fnv_00c9d220

// 0x00c9d250 sorts constraint UIDs (+0x34); partners are entities (+0x14/+0x18), not agent entries.
struct hkEntryLessD250
{
    unsigned int m_key;

    hkpEntity* getPartner(const hkpConstraintInstance* pkEntry) const
    {
        return reinterpret_cast<hkpEntity*>(reinterpret_cast<unsigned int>(pkEntry->m_entities[1]) ^
                                                          reinterpret_cast<unsigned int>(pkEntry->m_entities[0]) ^
                                                          m_key);
    }

    bool operator()(const hkpConstraintInstance* pkA, const hkpConstraintInstance* pkB) const
    {
        if (pkA->m_uid < pkB->m_uid)
        {
            return true;
        }
        if (pkA->m_uid > pkB->m_uid)
        {
            return false;
        }
        return getPartner(pkA)->m_uid < getPartner(pkB)->m_uid;
    }
};

// Recursively sorts the hkpConstraintInstance array in place using the quicksort algorithm with the hkEntryLessD250 comparator.
// @fnv 0x00c9d250
// src: GUESS 0x00c9d250 (confidence: medium)
void __cdecl hkpConstraintInstance_SortConstraintInstances(hkpConstraintInstance** pArr, int aiLow, int aiHigh, hkEntryLessD250 cmpLess)
{
    int i, iRight;
    hkpConstraintInstance* x;

begin:
    i = aiLow;
    iRight = aiHigh;
    x = pArr[(aiLow + aiHigh) >> 1];

    do
    {
        while (cmpLess(pArr[i], x))
            i++;
        while (cmpLess(x, pArr[iRight]))
            iRight--;

        if (iRight >= i)
        {
            if (iRight != i)
            {
                __declspec(align(16)) hkpConstraintInstance* t = pArr[iRight];
                pArr[iRight] = pArr[i];
                pArr[i] = t;
            }
            i++;
            iRight--;
        }
    } while (i <= iRight);

    if (aiLow < iRight)
        hkpConstraintInstance_SortConstraintInstances(pArr, aiLow, iRight, cmpLess);
    if (i < aiHigh)
    {
        aiLow = i;
        goto begin;
    }
}

struct FUN_00c9c700_Allocator
{
    virtual void slot00();
    virtual void* slot04(int size);
};

// Initializes the internal array if needed, then inserts value into the first empty slot or appends it, growing capacity when full.
// @fnv 0x00c9c700
// src: XBOX_RETAIL_PDB 0x00c9c700
// pdb: hkpEntity::addEntityActivationListener | hkpEntity.obj | retail, object-file gap fill, high
// pdb params: el
// pdb locals: assertBuf, ostr
void __fastcall hkpEntity_addEntityActivationListener(hkpEntity* self, int, hkpEntityActivationListener* value)
{
    if (self->m_extendedListeners == 0)
    {
        FnvCpu_FUN_00c9d160_CpuAllocatorHolder* holder = hkMemoryRouter_getInstance();
        FUN_00c9c700_Allocator* allocator = (FUN_00c9c700_Allocator*)holder->allocator;
        hkpEntityExtendedListeners* array = (hkpEntityExtendedListeners*)allocator->slot04(0x10);

        if (array != 0)
        {
            array->m_activationListeners.m_data = 0;
            array->m_activationListeners.m_size = 0;
            array->m_activationListeners.m_capacityAndFlags = 0x8000;
            array->m_entityListeners.m_data = 0;
            array->m_entityListeners.m_size = 0;
            array->m_entityListeners.m_capacityAndFlags = 0x8000;
        }
        else
        {
            array = 0;
        }

        self->m_extendedListeners = array;
    }

    hkpEntityExtendedListeners* array = self->m_extendedListeners;
    int index = 0;

    if (0 < array->m_activationListeners.m_size)
    {
        hkpEntityActivationListener** data = array->m_activationListeners.m_data;
        hkpEntityActivationListener** p = data;
        do
        {
            if (*p == 0)
            {
                if (index >= 0)
                {
                    data[index] = value;
                    return;
                }
                break;
            }
            ++index;
            ++p;
        } while (index < self->m_extendedListeners->m_activationListeners.m_size);
    }

    if (array->m_activationListeners.m_size == (array->m_activationListeners.m_capacityAndFlags & 0x3fff))
        hkSmallArrayUtil__reserveMore(array, 4);

    array->m_activationListeners.m_data[array->m_activationListeners.m_size] = value;
    ++array->m_activationListeners.m_size;
}

extern float DAT_0104da48;
void __cdecl PropagateStateToChildren(hkpWorld* world, hkpSimulationIsland* currentIsland);
void __cdecl hkpWorldOperationUtil_markIslandInactive(hkpWorld* world, hkpSimulationIsland* island);
// If the island is active: runs PropagateStateToChildren when split-check or sparse is set, sets each entity's reference position w to FLT_MAX, then calls AddItemToContainer.
// @fnv 0x00c9c240
void __fastcall deactivateEntityImmediately(hkpEntity* self)
{
    hkpSimulationIsland* island = (hkpSimulationIsland*)self->m_simulationIsland;
    if (island)
    {
        unsigned char act = island->f26.f26mid;
        unsigned char* pact = &act;
        if (*pact)
        {
            if ((island->flags25 & 3) || ((island->flags25 >> 2) & 3))
            {
                hkpWorld* world = (hkpWorld*)self->m_world;
                int minIslandSize = world->m_minDesiredIslandSize;
                world->m_minDesiredIslandSize = 0;
                ((hkpSimulationIsland*)self->m_simulationIsland)->flags25 &= ~0x0c;
                ((hkpSimulationIsland*)self->m_simulationIsland)->flags25 = (((hkpSimulationIsland*)self->m_simulationIsland)->flags25 & ~3) | 1;
                PropagateStateToChildren((hkpWorld*)self->m_world,
                                         (hkpSimulationIsland*)self->m_simulationIsland);
                ((hkpWorld*)self->m_world)->m_minDesiredIslandSize = minIslandSize;
            }
            island = (hkpSimulationIsland*)self->m_simulationIsland;
            int i = 0;
            if (island->entities.count > 0)
            {
                hkpEntity** entities = island->entities.data;
                do
                {
                    hkpEntity* entity = *entities++;
                    entity->m_motion.m_deactivationRefPosition[0].w = 3.40282e+38f;
                    entity->m_motion.m_deactivationRefPosition[1].w = 3.40282e+38f;
                } while (++i < island->entities.count);
            }
            hkpWorldOperationUtil_markIslandInactive((hkpWorld*)self->m_world, island);
        }
    }
}

// @fnv 0x00c9d1f0
hkpThinBoxMotion* __fastcall finishThinBoxMotion(hkpThinBoxMotion* self, int, int flag)
{
    extern const int DAT_010c6e34;
    *reinterpret_cast<const int**>(self) = &DAT_010c59f4;
    if (flag)
    {
        *reinterpret_cast<float*>(&flag) = 1.0f;
        self->m_gravityFactor.m_value = static_cast<short>(flag >> 16);
    }
    *reinterpret_cast<const int* volatile*>(self) = &DAT_010c6e34;
    return self;
}

namespace fnv_00c9c8f0 {
struct FnvMem
{
    virtual void f00() = 0;
    virtual void f04() = 0;
    virtual void freeArray(void*, unsigned int) = 0;
    virtual void f0c() = 0;
    virtual void freeBlock(void*, unsigned int) = 0;
};
struct FnvRouter
{
    void* a;
    void* b;
    void* c;
    void* d;
    FnvMem* heap;
};
extern FnvRouter* __cdecl hkMemoryRouter_getInstance();

struct FnvRef : hkReferencedObject, hkpContactListener
{
    virtual ~FnvRef();
};

template <typename Array> __forceinline void clearSmallArray(Array& array, int elementSize)
{
    if (!(array.m_iCapacityState & 0x8000))
    {
        unsigned c = array.m_iCapacityState & 0x3fff;
        void* d = array.m_data;
        FnvRouter* router = hkMemoryRouter_getInstance();
        FnvMem* mem = router->heap;
        mem->freeArray(d, c * elementSize);
    }
    array.m_iElementCount = 0;
    array.m_data = 0;
    array.m_iCapacityState = (array.m_iCapacityState & 0xc000) | 0x8000;
}

__forceinline bool clearContactArrayIfEmpty(hkpEntitySmallArraySerializeOverrideType& array)
{
    for (int i = 0; i < array.m_iElementCount; ++i)
        if (static_cast<void**>(array.m_data)[i])
            return true;
    clearSmallArray(array, 4);
    return false;
}

template <typename T> __forceinline void releaseExtendedArray(hkSmallArray<T>& array)
{
    if (!(array.m_capacityAndFlags & 0x8000))
    {
        unsigned c = array.m_capacityAndFlags & 0x3fff;
        void* d = array.m_data;
        FnvRouter* router = hkMemoryRouter_getInstance();
        FnvMem* mem = router->heap;
        mem->freeArray(d, c * 4);
    }
}

template <typename T> __forceinline void clearExtendedArrayIfEmpty(hkSmallArray<T>& array, bool& used)
{
    for (int i = 0; i < array.m_size; ++i)
        if (array.m_data[i])
        {
            used = true;
            return;
        }
    releaseExtendedArray(array);
    array.m_size = 0;
    array.m_data = 0;
    array.m_capacityAndFlags = (array.m_capacityAndFlags & 0xc000) | 0x8000;
}

__forceinline void freeExtendedListeners(hkpEntityExtendedListeners* listeners)
{
    releaseExtendedArray(listeners->m_entityListeners);
    releaseExtendedArray(listeners->m_activationListeners);
    FnvRouter* router = hkMemoryRouter_getInstance();
    FnvMem* mem = router->heap;
    mem->freeArray(listeners, 16);
}

// Deletes p98 after FUN_00c9c140, then frees storage of p60, master, slave, runtime, contact, actions and extended when each is empty.
// @fnv 0x00c9c8f0
// pdb: hkpEntity::deallocateInternalArrays | hkpEntity.obj | retail, vtable slot, high
// pdb locals: assertBuf, ostr
void __fastcall rebuildEntity(hkpEntity* self)
{
    FnvRef* old = static_cast<FnvRef*>(self->m_limitContactImpulseUtil);
    if (old)
    {
        self->RemoveContactListener(static_cast<hkpContactListener*>(old));
        delete old;
        self->m_limitContactImpulseUtil = 0;
    }
    if (self->m_kCollidable.m_collisionEntries.m_iElementCount == 0)
    {
        FnvRouter* router = hkMemoryRouter_getInstance();
        FnvMem* mem = router->heap;
        int f = self->m_kCollidable.m_collisionEntries.m_iCapacityState;
        self->m_kCollidable.m_collisionEntries.m_iElementCount = 0;
        if (!(f & 0x80000000))
            mem->freeBlock(self->m_kCollidable.m_collisionEntries.m_data, (f & 0x3fffffff) * 8);
        self->m_kCollidable.m_collisionEntries.m_data = 0;
        self->m_kCollidable.m_collisionEntries.m_iCapacityState = 0x80000000;
    }
    if (self->m_constraintsMaster.m_iElementCount == 0)
        clearSmallArray(self->m_constraintsMaster, 48);
    if (self->m_constraintsSlave.m_iElementCount == 0)
    {
        FnvRouter* router = hkMemoryRouter_getInstance();
        FnvMem* mem = router->heap;
        int f = self->m_constraintsSlave.m_iCapacityState;
        self->m_constraintsSlave.m_iElementCount = 0;
        if (!(f & 0x80000000))
            mem->freeBlock(self->m_constraintsSlave.m_data, (f & 0x3fffffff) * 4);
        self->m_constraintsSlave.m_data = 0;
        self->m_constraintsSlave.m_iCapacityState = 0x80000000;
    }
    if (self->m_constraintRuntime.m_iElementCount == 0)
    {
        FnvRouter* router = hkMemoryRouter_getInstance();
        FnvMem* mem = router->heap;
        int f = self->m_constraintRuntime.m_iCapacityState;
        self->m_constraintRuntime.m_iElementCount = 0;
        if (!(f & 0x80000000))
            mem->freeBlock(self->m_constraintRuntime.m_data, f & 0x3fffffff);
        self->m_constraintRuntime.m_data = 0;
        self->m_constraintRuntime.m_iCapacityState = 0x80000000;
    }
    clearContactArrayIfEmpty(self->m_contactListeners);
    bool usedContact = false;
    hkpEntityExtendedListeners* pExtended = self->m_extendedListeners;
    if (pExtended)
    {
        clearExtendedArrayIfEmpty(pExtended->m_activationListeners, usedContact);
    }
    bool usedAction = false;
    pExtended = self->m_extendedListeners;
    if (pExtended)
    {
        clearExtendedArrayIfEmpty(pExtended->m_entityListeners, usedAction);
    }
    if (!usedContact && !usedAction)
    {
        pExtended = self->m_extendedListeners;
        if (pExtended)
            freeExtendedListeners(pExtended);
        self->m_extendedListeners = 0;
    }
    if (self->m_actions.m_iElementCount == 0)
        clearSmallArray(self->m_actions, 4);
}
}  // namespace fnv_00c9c8f0

namespace fnv_00c9cf60 {
// hkSmallArray has no shared destructor yet; release the shared serialized storage explicitly.
template <typename Array> inline void releaseEntitySmallArray(Array& array, int elementSize)
{
    if ((array.m_iCapacityState & 0x8000) == 0)
    {
        int capacity = array.m_iCapacityState & 0x3fff;
        void* data = array.m_data;
        hkMemoryRouter::getInstance().heap().blockFree(data, capacity * elementSize);
    }
}

struct LimitUtilC9CF : public hkReferencedObject, public hkpContactListener
{
    virtual ~LimitUtilC9CF() throw();
};

void __cdecl hkpEntityCallbackUtil_fireEntityDeleted(void* entity);

// Destructor: if m_pEntityUserData is set, passes it to FUN_00c9c140 and deletes it; calls DeleteCallbacks(this); deletes m_pExtendedListeners if set.
// @fnv 0x00c9cf60
// pdb: hkpEntity::~hkpEntity | hkpEntity.obj | retail, object-file run extension, low
// pdb locals: assertBuf, ostr
void __fastcall destroyEntity(hkpEntity* self)
{
    extern const int DAT_010c6f7c;
    *reinterpret_cast<const int**>(self) = &DAT_010c6f7c;
    LimitUtilC9CF* pkUtil = static_cast<LimitUtilC9CF*>(self->m_limitContactImpulseUtil);
    if (pkUtil)
    {
        self->RemoveContactListener(static_cast<hkpContactListener*>(pkUtil));
        delete pkUtil;
        self->m_limitContactImpulseUtil = 0;
    }
    hkpEntityCallbackUtil_fireEntityDeleted(self);
    hkpEntityExtendedListeners* listeners = self->m_extendedListeners;
    if (listeners)
    {
        destroyExtendedListeners(listeners);
        hkpEntityExtendedListeners::operator delete(listeners);
    }
    hkReferencedObject* localFrame = reinterpret_cast<hkReferencedObject*>(self->m_localFrame);
    if (localFrame)
        localFrame->removeReference();
    self->m_localFrame = 0;
    releaseEntitySmallArray(self->m_actions, 4);
    releaseEntitySmallArray(self->m_contactListeners, 4);
    self->m_motion.hkpMotion::~hkpMotion();
    releaseArrayStorage(self->m_constraintRuntime, 1);
    releaseArrayStorage(self->m_constraintsSlave, 4);
    // hkConstraintInternal has 0x30-byte records; no approved shared definition exists yet.
    releaseEntitySmallArray(self->m_constraintsMaster, 0x30);
    self->hkpWorldObject::~hkpWorldObject();
}
}  // namespace fnv_00c9cf60

// Keep the original destruction sequence explicit: properties, name, collision entries, then the base object.
// @fnv 0x00c9d3b0
// pdb: hkpWorldObject::~hkpWorldObject | hkpEntity.obj | retail, call graph (shared callers), high
// pdb locals: assertBuf, ostr
void __fastcall destroyWorldObject(hkpWorldObject* self, int)
{
    extern const int DAT_010c6f64;
    *reinterpret_cast<const int**>(self) = &DAT_010c6f64;
    if (self->m_kCollidable.m_pCollisionShape)
        const_cast<hkpShape*>(self->m_kCollidable.m_pCollisionShape)->removeReference();
    if (self->m_aiData)
        static_cast<hkReferencedObject*>(self->m_aiData)->removeReference();
    releaseArrayStorage(self->m_properties, 16);
    self->m_name.~hkStringPtr();
    releaseArrayStorage(self->m_kCollidable.m_collisionEntries, 8);
    self->hkBaseObject::~hkBaseObject();
}
