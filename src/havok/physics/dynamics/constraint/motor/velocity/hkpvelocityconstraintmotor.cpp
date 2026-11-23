
// src/havok/physics/dynamics/constraint/motor/velocity/hkpvelocityconstraintmotor.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/dynamics/constraint/motor/velocity/hkpvelocityconstraintmotor.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../../../../include/hkMemoryRouter.h"
#include "../../../../../../../include/hkReferencedObject.h"
#include "../../../../../../../include/hkpConstraintMotor.h"
#include "../../../../../../../include/hkpLimitedForceConstraintMotor.h"

// This unit calls the non-throwing chunk-allocation interface at vtable slot 1.
struct MotorChunkAllocator
{
    virtual void v00();
    virtual void* allocateChunk(int nbytes) throw();
};

__forceinline MotorChunkAllocator& GetMotorHeap(hkMemoryRouter& router)
{
    return *reinterpret_cast<MotorChunkAllocator*>(router.m_heap);
}

inline void* operator new(size_t size, void* p)
{
    return p;
}

// HK_DECLARE_CLASS_ALLOCATOR's operator new, against this unit's view of the memory router.
__forceinline void* __cdecl hkReferencedObject::operator new(std::size_t nbytes)
{
    hkReferencedObject* b = static_cast<hkReferencedObject*>(GetMotorHeap(hkMemoryRouter::getInstance()).allocateChunk(static_cast<int>(nbytes)));
    b->m_memSizeAndFlags = static_cast<unsigned short>(nbytes);
    return b;
}
// No include/hkpVelocityConstraintMotor.h exists yet; the reflected layout is 0x20 bytes.
// pdb: class hkpVelocityConstraintMotor, size 0x20, bases: hkpLimitedForceConstraintMotor, 3 members
class hkpVelocityConstraintMotor : public hkpLimitedForceConstraintMotor
{
  public:
    virtual void v0();
    float m_tau; // src: XBOX_RETAIL_PDB +0x14 (old: m_fTau)
    float m_velocityTarget; // src: XBOX_RETAIL_PDB +0x18 (old: m_fVelocityTarget)
    char m_useVelocityTargetFromConstraintTargets; // src: XBOX_RETAIL_PDB +0x1c (old: m_bUseVelocityTarget)
    char pad1D[3];

    __forceinline hkpVelocityConstraintMotor(const hkpVelocityConstraintMotor& o) throw()
        : hkpLimitedForceConstraintMotor(o), m_tau(o.m_tau), m_velocityTarget(o.m_velocityTarget),
          m_useVelocityTargetFromConstraintTargets(o.m_useVelocityTargetFromConstraintTargets)
    {}

    hkpVelocityConstraintMotor* clone_00d861c0() const;
};

// Allocates a chunk from the thread memory router and constructs a copy of this motor in it.
// @fnv 0x00d861c0
// src: HAVOK_LIB 0x00d861c0
// pdb: hkpVelocityConstraintMotor::clone | hkpVelocityConstraintMotor.obj | retail, vtable slot, high
hkpVelocityConstraintMotor* hkpVelocityConstraintMotor::clone_00d861c0() const
{
    return new hkpVelocityConstraintMotor(*this);
}
