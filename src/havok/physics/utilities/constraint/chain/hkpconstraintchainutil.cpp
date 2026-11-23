#include "../../../../../../include/hkArray.h"

// src/havok/physics/utilities/constraint/chain/hkpconstraintchainutil.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/physics/utilities/constraint/chain/hkpconstraintchainutil.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../../../include/hkMemoryAllocator.h"
#include "../../../../../../include/hkMemoryRouter.h"

struct CallTarget_00e5c370
{
    int InsertAndDelete(int, int, int, int, int);
};
struct SearchObject
{
    int ProcessAndReturnValue_00e5c420(unsigned int arg0, unsigned int arg1, unsigned int arg2, unsigned int arg3);
};
// @fnv 0x00e5c420
// src: GUESS 0x00e5c420 (confidence: low)
int SearchObject::ProcessAndReturnValue_00e5c420(unsigned int auiParam0, unsigned int auiParam1, unsigned int auiParam2,
                               unsigned int auiParam3)
{
    unsigned int callResult0 =
        (unsigned int)((CallTarget_00e5c370*)this)
            ->InsertAndDelete((int)auiParam0, (int)auiParam1, (int)0u, (int)auiParam2, (int)auiParam3);
    return (int)callResult0;
}

namespace fnv_00e5c440 {
struct CallTarget_00e5c420
{
    int ProcessAndReturnValue_00e5c420(int, int, int, int);
};
struct SearchObject
{
    int ProcessAndReturnValue_00e5c440(unsigned int arg0, unsigned int arg1, unsigned int arg2);
};
// @fnv 0x00e5c440
// src: GUESS 0x00e5c440 (confidence: low)
int SearchObject::ProcessAndReturnValue_00e5c440(unsigned int arg0, unsigned int arg1, unsigned int arg2)
{
    unsigned int callResult0 =
        (unsigned int)((CallTarget_00e5c420*)this)->ProcessAndReturnValue_00e5c420((int)arg0, (int)arg1, (int)arg2, (int)1u);
    return (int)callResult0;
}
}  // namespace fnv_00e5c440

hkMemoryRouter* __cdecl hkMemoryRouter_getInstance() throw();

struct DispatchTarget
{
    void CallWithFlag(void* aValue, int aZero, void* aOut);
};

struct OutObject
{
    void FUN_00c90510();
};

struct FUN_00e5c460_cls
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
    DispatchTarget m_38;

    void hkpConstraintChainInstance_insertEntityAtFront(OutObject* entity);
};

// Gets an hkMemoryRouter, passes its allocator to m_38.CallWithFlag, then calls FUN_00c90510 on a1.
// @fnv 0x00e5c460
// src: HAVOK_LIB 0x00e5c460
// pdb: hkpConstraintChainInstance::insertEntityAtFront | hkpConstraintChainUtil.obj | retail, upstream name already matches, high
// pdb params: entity
// pdb locals: assertBuf, ostr
void FUN_00e5c460_cls::hkpConstraintChainInstance_insertEntityAtFront(OutObject* entity)
{
    hkMemoryRouter* pHolder = hkMemoryRouter_getInstance();
    void* pValue = pHolder->m_heap;
    m_38.CallWithFlag(pValue, 0, &entity);
    entity->FUN_00c90510();
}

#include <xmmintrin.h>
__declspec(align(16)) struct XfOut_cls
{
    unsigned int m_00;
    unsigned int m_04;
    unsigned int m_08;
    unsigned int m_0c;
};

struct FUN_00c7f690_cls
{
    void hkVector4_setTransformedInversePos(const __m128* m, const __m128* v);
};

struct XfSub_cls
{
    void addConstraintInfoInBodySpace(XfOut_cls* pivotInA, XfOut_cls* pivotInB);
};

struct XfScene_cls
{
    int unk00;
    int unk04;
    int unk08;
    XfSub_cls* m_0c;
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
    char** m_38;
    int m_3c;
    void addEntity(XfScene_cls* entity);
};

static char* XfSelectRec(char* rec)
{
    if (rec[0x18] == 1)
    {
        return rec + (signed char)rec[0x10];
    }
    return 0;
}

// @fnv 0x00e5b3a0
// src: HAVOK_LIB 0x00e5b3a0
// pdb: hkpConstraintChainUtil::addConstraintToBallSocketChain | hkpConstraintChainUtil.obj | retail, upstream name already matches, high
// pdb params: instance, entityToAppend, pivotWs
// pdb locals: info, assertBuf, ostr
void hkpConstraintChainUtil_addConstraintToBallSocketChain(XfScene_cls* instance, XfScene_cls* entityToAppend, const __m128* pivotWs)
{
    XfSub_cls* sub = instance->m_0c;
    char* p1 = XfSelectRec(instance->m_38[instance->m_3c - 1] + 0x10);
    char* p2 = XfSelectRec((char*)entityToAppend + 0x10);
    XfOut_cls outs[2];
    ((FUN_00c7f690_cls*)&outs[0])->hkVector4_setTransformedInversePos((const __m128*)(p1 + 0xf0), pivotWs);
    ((FUN_00c7f690_cls*)&outs[1])->hkVector4_setTransformedInversePos((const __m128*)(p2 + 0xf0), pivotWs);
    sub->addConstraintInfoInBodySpace(&outs[0], &outs[1]);
    instance->addEntity(entityToAppend);
}

namespace fnv_00e5b440 {
struct __declspec(align(16)) FUN_00c7f690_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    void hkVector4_setTransformedInversePos(const __m128* m, const __m128* v);
};

struct __declspec(align(16)) RelativeOrientationData
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int argument4;
    int argument5;
    int argument6;
    void setMulInverse(char* q0, char* q1);
};

struct Callee
{
    int unk00;
    void addConstraintInfoInBodySpace(void* pivotInA, void* pivotInB, void* aTc, int xMotor, int yMotor, int zMotor);
};

struct Owner
{
    int unk00;
    int unk04;
    int unk08;
    Callee* m_0c;
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
    void** m_38;
    int m_3c;
    void addEntity(char* entity);
};

// Computes relative orientation between two objects and applies it via a Callee, then adds a reference to a2.
// @fnv 0x00e5b440
// src: XBOX_RETAIL_PDB 0x00e5b440
// pdb: hkpConstraintChainUtil::addConstraintToPoweredChain | hkpConstraintChainUtil.obj | retail, object-file gap fill, high
// pdb params: instance, entityToAppend, pivotWs, motor0, motor1, motor2
// pdb locals: info, assertBuf, ostr
void hkpConstraintChainUtil_addConstraintToPoweredChain(Owner* instance, char* entityToAppend, const __m128* pivotWs, int motor0, int motor1, int motor2)
{
    Callee* pCallee = instance->m_0c;
    char* e1;
    char* e2;
    {
        char* p = (char*)instance->m_38[instance->m_3c - 1] + 0x10;
        e1 = (*(unsigned char*)(p + 0x18) == 1) ? p + (signed char)p[0x10] : 0;
    }
    {
        char* p = entityToAppend + 0x10;
        e2 = (*(unsigned char*)(p + 0x18) == 1) ? p + (signed char)p[0x10] : 0;
    }
    // The two transformed points form one adjacent pair consumed by the callback.
    struct LocalPoints
    {
        FUN_00c7f690_cls first;
        FUN_00c7f690_cls second;
    } points;
    points.first.hkVector4_setTransformedInversePos((const __m128*)(e1 + 0xf0), pivotWs);
    points.second.hkVector4_setTransformedInversePos((const __m128*)(e2 + 0xf0), pivotWs);
    // Helper writes the quaternion at offset 0; original also stores arguments at 0x20/24/28.
    RelativeOrientationData relativeOrientation;
    relativeOrientation.setMulInverse(e2 + 0x160, e1 + 0x160);
    relativeOrientation.argument4 = motor0;
    relativeOrientation.argument5 = motor1;
    relativeOrientation.argument6 = motor2;
    pCallee->addConstraintInfoInBodySpace(&points.first, &points.second, (void*)&relativeOrientation, motor0, motor1, motor2);
    instance->addEntity(entityToAppend);
}
}  // namespace fnv_00e5b440

// FUN_00e5b8f0: __cdecl, 317 bytes
// Two 16-byte keys are filled by FUN_00e5ad40; the sorted-list tail is compared against
// the owner's keys; the matching branch hands a 32-byte pair to a sub-object or holder.

struct __declspec(align(16)) Tmp16
{
    unsigned int m_00;
    unsigned int m_04;
    unsigned int m_08;
    unsigned int m_0c;
};

struct __declspec(align(16)) Pair32
{
    Tmp16 lo;
    Tmp16 hi;
};

static void CopyTmp16(Tmp16* dst, const Tmp16* src)
{
    _mm_store_ps(reinterpret_cast<float*>(dst), _mm_load_ps(reinterpret_cast<const float*>(src)));
}

struct KeyOwnerA
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0c;
    unsigned int unk10;
    unsigned int m_14;
    unsigned int m_18;
};

struct Sub18
{
    void ProcessAndReturnValue_00e5c440(hkMemoryAllocator* a, int b, Pair32* c);
};

struct OwnerC
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0c;
    unsigned int unk10;
    unsigned int unk14;
    Sub18 m_18;
    void addConstraintInfoInBodySpace(Tmp16* pivotInA, Tmp16* pivotInB);
};

struct ListB
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0C;
    unsigned int unk10;
    unsigned int unk14;
    unsigned int unk18;
    unsigned int unk1C;
    unsigned int unk20;
    unsigned int unk24;
    unsigned int unk28;
    unsigned int unk2C;
    unsigned int unk30;
    unsigned int unk34;
    unsigned int* m_38;
    unsigned int m_3c;
    void addEntity(unsigned int entity);
    void hkpConstraintChainInstance_insertEntityAtFront(unsigned int entity);
};

int hkpConstraintUtils_getConstraintPivots(KeyOwnerA* a, Tmp16* pivotInAOut, Tmp16* pivotInBOut);

// Returns 1 if FUN_00e5ad40 is nonzero or no key matches; else matches a1's m_14/m_18 to a2's list ends, calls a3 helpers, updates a2 and returns 0.
// @fnv 0x00e5b8f0
// src: XBOX_PDB 0x00e5b8f0
// pdb: hkpConstraintChainUtil::addConstraintToChain | hkpConstraintChainUtil.obj | retail, upstream name already matches, high
// pdb params: newChildInstance, chainInstance, chainData
// pdb locals: pivotB, pivotA, assertBuf, ostr, info
int hkpConstraintChainUtil_addConstraintToChain(KeyOwnerA* newChildInstance, ListB* chainInstance, OwnerC* chainData, ...)
{
    Pair32 keyPair;

    if (hkpConstraintUtils_getConstraintPivots(newChildInstance, &keyPair.lo, &keyPair.hi) != 0)
    {
        return 1;
    }
    if (chainInstance->m_3c == 0)
    {
        chainInstance->addEntity(newChildInstance->m_14);
    }
    unsigned int uiLastKey = chainInstance->m_38[chainInstance->m_3c - 1];
    if (uiLastKey == newChildInstance->m_14)
    {
        chainData->addConstraintInfoInBodySpace(&keyPair.lo, &keyPair.hi);
        chainInstance->addEntity(newChildInstance->m_18);
        return 0;
    }
    if (uiLastKey == newChildInstance->m_18)
    {
        chainData->addConstraintInfoInBodySpace(&keyPair.hi, &keyPair.lo);
        chainInstance->addEntity(newChildInstance->m_14);
        return 0;
    }
    if (chainInstance->m_38[0] == newChildInstance->m_14)
    {
        Pair32 p;
        CopyTmp16(&p.lo, &keyPair.hi);
        CopyTmp16(&p.hi, &keyPair.lo);
        hkMemoryRouter* h = hkMemoryRouter_getInstance();
        chainData->m_18.ProcessAndReturnValue_00e5c440(reinterpret_cast<hkMemoryAllocator*>(h->m_heap), 0, &p);
        chainInstance->hkpConstraintChainInstance_insertEntityAtFront(newChildInstance->m_18);
        return 0;
    }
    if (chainInstance->m_38[0] == newChildInstance->m_18)
    {
        Pair32 p;
        CopyTmp16(&p.lo, &keyPair.lo);
        CopyTmp16(&p.hi, &keyPair.hi);
        hkMemoryRouter* h = hkMemoryRouter_getInstance();
        chainData->m_18.ProcessAndReturnValue_00e5c440(reinterpret_cast<hkMemoryAllocator*>(h->m_heap), 0, &p);
        chainInstance->hkpConstraintChainInstance_insertEntityAtFront(newChildInstance->m_14);
        return 0;
    }
    return 1;
}

inline void* operator new(size_t, void* p)
{
    return p;
}
inline void operator delete(void*, void*)
{}

struct S_00e5c370_Alloc;
struct __declspec(align(16)) S_00e5c370_Vec
{
    float x, y, z, w;
};
struct S_00e5c370_Elem
{
    S_00e5c370_Vec a;
    S_00e5c370_Vec b;
    S_00e5c370_Elem(const S_00e5c370_Elem& o)
    {
        reinterpret_cast<__m128&>(a) = reinterpret_cast<const __m128&>(o.a);
        reinterpret_cast<__m128&>(b) = reinterpret_cast<const __m128&>(o.b);
    }
};
void __cdecl hkArrayUtil__reserve(S_00e5c370_Alloc& mem, void* array, int numElem, int sizeElem);
void __cdecl hkMemUtil_memMove(void* dst, const void* src, int nbytes);

template <class T>
static __forceinline void S_00e5c370_ArrayBase_GrowArrayCapacity(hkArrayBase<T>& self, S_00e5c370_Alloc& a, int n);

template <class T>
static __forceinline void S_00e5c370_ArrayBase_GrowArrayCapacity(hkArrayBase<T>& self, S_00e5c370_Alloc& a, int n)
{
        const int cap = self.getCapacity();
        if (cap < n)
        {
            int cap2 = 2 * cap;
            int newSize = (n < cap2) ? cap2 : n;
            hkArrayUtil__reserve(a, (&self), newSize, sizeof(T));
        }
    }



template <>
void hkArrayBase<S_00e5c370_Elem>::_spliceInto(hkMemoryAllocator& alloc, int index, int numdel, const S_00e5c370_Elem* apElements, int numtoinsert)
{
        int newsize = m_iElementCount + numtoinsert - numdel;
        int numtomove = m_iElementCount - index - numdel;
        if (newsize > getCapacity())
            S_00e5c370_ArrayBase_GrowArrayCapacity(*this, reinterpret_cast<S_00e5c370_Alloc&>(alloc), newsize);
        hkMemUtil_memMove(m_data + index + numtoinsert, m_data + index + numdel, numtomove * sizeof(S_00e5c370_Elem));
        S_00e5c370_Elem* dst = m_data + index;
        for (int i = 0; i < numtoinsert; i++)
            new (dst + i) S_00e5c370_Elem(apElements[i]);
        m_iElementCount = newsize;
    }
