#include "../../../../include/NiTArray.h"
#include "../../../../include/MemoryManager.h"
#include "../../../../include/BSFadeNode.h"
#include "../../../../include/NiBlendAccumTransformInterpolator.h"
#include "../../../../include/NiQuatTransform.h"
#include <string.h>
#include <math.h>

extern NiPoint3 DAT_011a8400;
extern NiQuaternion DAT_011f3704;
extern float DAT_01096b8c;
extern float DAT_0109696c;
extern MemoryManager g_memoryManager;
// Preserve the original inline value copies and invalid-component tests.
inline NiQuatTransform& NiQuatTransform::operator=(const NiQuatTransform& other)
{
    memcpy(this, &other, sizeof(*this));
    return *this;
}
inline NiQuaternion& NiQuaternion::operator=(const NiQuaternion& other)
{
    memcpy(this, &other, sizeof(*this));
    return *this;
}
inline bool NiQuatTransform::IsMarkedInvalid() const
{
    return m_fScale == -3.4028234663852886e38 && qy == -3.4028234663852886e38 && m_kTranslate == -3.4028234663852886e38;
}
inline bool NiQuatTransform::IsMarkedInvalid()
{
    return static_cast<const NiQuatTransform&>(*this).IsMarkedInvalid();
}
struct AccumItem
{
    float time;
    NiQuatTransform value, accum;
    float rotation[9];
    AccumItem() throw();
    static void* operator new[](size_t size)
    {
        return g_memoryManager.Allocate(size);
    }
    AccumItem& operator=(const AccumItem& other)
    {
        memcpy(this, &other, sizeof(*this));
        return *this;
    }
};
// @fnv 0x00a397b0
// src: XBOX_RETAIL_PDB 0x00a397b0
// pdb: NiBlendAccumTransformInterpolator::StoreSingleValue | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: fTime, pkInterpTarget, kValue
// pdb locals: kLastInv
bool NiBlendAccumTransformInterpolator::StoreSingleValue(float fTime, NiObjectNET* pkInterpTarget,
                                                                   NiQuatTransform& kValue)
{
    if (m_uFlags & 1)
        fTime = m_fSingleTime;
    if (fTime == DAT_0109696c)
        return false;
    CalculateSingleValue(fTime, pkInterpTarget);
    if (m_bReset || m_kAccumulatedTransformValue.IsMarkedInvalid())
    {
        // The scratch transform is fully initialized below, without a default-constructor call.
        float inverseStorage[sizeof(NiQuatTransform) / sizeof(float)];
        NiQuatTransform& inverse = *reinterpret_cast<NiQuatTransform*>(inverseStorage);
        *reinterpret_cast<NiPoint3*>(&inverse.m_kTranslate) = DAT_011a8400;
        *reinterpret_cast<NiQuaternion*>(&inverse.m_kRotate) = DAT_011f3704;
        inverse.m_fScale = DAT_01096b8c;
        m_pkAccumArray[m_ucSingleIdx].value.TransformBy(inverse);
        if (m_kAccumulatedTransformValue.IsMarkedInvalid())
            m_kAccumulatedTransformValue = inverse;
        else
            m_kAccumulatedTransformValue = m_kAccumulatedTransformValue.CombineTransforms(inverse);
        m_bReset = false;
    }
    if (m_kAccumulatedTransformValue.IsMarkedInvalid())
        return false;
    kValue = m_kAccumulatedTransformValue.CombineTransforms(m_pkAccumArray[m_ucSingleIdx].value);
    return true;
}

struct Q
{
    float w, x, y, z;
    bool Object_HasEqualFloatComponents(const Q&) const;
};
// The shared transform has no equality declaration; retain the implementation as a free function.
// @fnv 0x00a391c0
// pdb: NiQuatTransform::operator!= | NiBlendAccumTransformInterpolator.obj | debug, call graph, medium
// pdb params: kTransform
bool __fastcall EqualQuatTransforms(const NiQuatTransform* self, int, const NiQuatTransform& other)
{
    return self->m_kTranslate == other.m_kTranslate && self->y == other.y && self->z == other.z &&
           reinterpret_cast<const Q*>(&self->m_kRotate)->Object_HasEqualFloatComponents(*reinterpret_cast<const Q*>(&other.m_kRotate)) &&
           self->m_fScale == other.m_fScale;
}

extern int DAT_01096784;
extern int DAT_011a8404, DAT_011a8408, DAT_011f3708, DAT_011f370c, DAT_011f3710;
void __fastcall NiBlendInterpolator_NiBlendInterpolator(void*);
// Initialize the base, vtable, accumulated transform and accumulation array.
// @fnv 0x00a39230
// pdb: NiBlendAccumTransformInterpolator::NiBlendAccumTransformInterpolator | NiBlendAccumTransformInterpolator.obj | retail, call graph, medium
// pdb note: debug build links it to NiQuatTransform::operator==
__declspec(noinline) NiBlendAccumTransformInterpolator* __fastcall ConstructAccumulatedTransform(NiBlendAccumTransformInterpolator* pObj)
{
    NiBlendInterpolator_NiBlendInterpolator(pObj);
    *reinterpret_cast<void**>(pObj) = &DAT_01096784;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.m_kTranslate) = *(int*)&DAT_011a8400;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.y) = DAT_011a8404;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.z) = DAT_011a8408;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.m_kRotate) = *(int*)&DAT_011f3704;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.qy) = DAT_011f3708;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.qz) = DAT_011f370c;
    *reinterpret_cast<int*>(&pObj->m_kAccumulatedTransformValue.qw) = DAT_011f3710;
    pObj->m_kAccumulatedTransformValue.m_fScale = DAT_01096b8c;
    pObj->m_pkAccumArray = 0;
    pObj->m_bReset = 0;
    return pObj;
}

void __fastcall DestroyInterpolatorArray(void*);
// @fnv 0x00a39290
// src: XBOX_RETAIL_PDB 0x00a39290
// pdb: NiBlendAccumTransformInterpolator::~NiBlendAccumTransformInterpolator | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
void __fastcall NiBlendAccumTransformInterpolator_dtor(NiBlendAccumTransformInterpolator* apObject, int unused)
{
    *reinterpret_cast<void**>(apObject) = &DAT_01096784;
    g_memoryManager.Deallocate(apObject->m_pkAccumArray);
    DestroyInterpolatorArray(apObject);
}

extern double _DAT_010241b0;
// @fnv 0x00a394b0
// pdb: NiBlendAccumTransformInterpolator::GetValue | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: kValue
bool NiBlendAccumTransformInterpolator::vf59(NiQuatTransform* kValue)
{
    const NiQuatTransform* pAcc = &m_kAccumulatedTransformValue;
    if (pAcc->m_fScale == _DAT_010241b0 && pAcc->qy == _DAT_010241b0 && pAcc->m_kTranslate == _DAT_010241b0)
        return false;
    if (m_ucInterpCount == 1)
    {
        *kValue = m_kAccumulatedTransformValue.CombineTransforms(m_pkAccumArray[m_ucSingleIdx].value);
        return true;
    }
    *kValue = *pAcc;
    return true;
}

namespace fnv_00a39700 {
// @fnv 0x00a39700
// src: EXE 0x00a39700
// pdb: NiBlendAccumTransformInterpolator::CreateObject | NiBlendAccumTransformInterpolator.obj | retail, upstream name already matches, high
// pdb locals: pkObject
void* __cdecl NiBlendAccumTransformInterpolator_CreateObject()
{
    void* p = BSFadeNode::operator new(sizeof(NiBlendAccumTransformInterpolator));
    if (p)
    {
        return reinterpret_cast<void* (__thiscall*)(NiBlendAccumTransformInterpolator*)>(&ConstructAccumulatedTransform)(
            static_cast<NiBlendAccumTransformInterpolator*>(p));
    }
    return 0;
}
}  // namespace fnv_00a39700

// Keep undeclared methods as free functions, using the original member-call ABI at call sites.
void __fastcall SaveBlendInterpolator(NiBlendInterpolator*, int, NiStream*);

// @fnv 0x00a39720
// pdb: NiBlendAccumTransformInterpolator::SaveBinary | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: kStream
// pdb locals: uc
void __fastcall SaveAccumulatedTransform(NiBlendAccumTransformInterpolator* self, int, NiStream* kStream)
{
    reinterpret_cast<void (__thiscall*)(NiBlendInterpolator*, NiStream*)>(&SaveBlendInterpolator)(self, kStream);
    unsigned char uc;
    for (uc = 0; uc < self->m_ucArraySize; uc++)
    {
        AccumItem* item = &self->m_pkAccumArray[uc];
        item->value.SaveBinary(kStream);
        item->accum.SaveBinary(kStream);
        reinterpret_cast<NiMatrix3*>(item->rotation)->SaveBinary(*kStream);
    }
    self->m_kAccumulatedTransformValue.SaveBinary(kStream);
}

// @fnv 0x00a39780
// pdb: NiBlendAccumTransformInterpolator::IsEqual | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: pkObject
// pdb locals: pkDest
bool __fastcall IsAccumulatedTransformEqual(NiBlendAccumTransformInterpolator* self, int, NiObject* pkObject)
{
    if (!self->NiBlendInterpolator::IsEqual(pkObject))
        return false;
    return reinterpret_cast<char (__thiscall*)(const NiQuatTransform*, const NiQuatTransform&)>(&EqualQuatTransforms)(
        &self->m_kAccumulatedTransformValue,
        reinterpret_cast<NiBlendAccumTransformInterpolator*>(pkObject)->m_kAccumulatedTransformValue);
}

// @fnv 0x00a39ec0
// pdb: NiBlendAccumTransformInterpolator::ReallocateArrays | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb locals: ucOldArraySize, bReturn, pkNewAccumArray, uc
bool __fastcall ResizeAccumulationItems(NiBlendAccumTransformInterpolator* self, int)
{
    unsigned char ucOldCount = self->m_ucArraySize;
    bool bResized = self->NiBlendInterpolator::GrowInterpolatorArray();
    if (bResized)
    {
        AccumItem* pkNewAccumArray = new AccumItem[self->m_ucArraySize];
        for (unsigned char uc = 0; uc < ucOldCount; uc++)
            pkNewAccumArray[uc] = self->m_pkAccumArray[uc];
        g_memoryManager.Deallocate(self->m_pkAccumArray);
        self->m_pkAccumArray = pkNewAccumArray;
    }
    return bResized;
}

// This base helper is not declared by the approved shared header yet.
void __fastcall CopyBlendInterpolatorMembers(NiBlendInterpolator*, int, void*, void*);

// @fnv 0x00a3a450
// src: XBOX_RETAIL_PDB 0x00a3a450
// pdb: NiBlendAccumTransformInterpolator::CopyMembers | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: pkDest, kCloning
// pdb locals: uc
void __fastcall NiBlendAccumTransformInterpolator_CopyMembers(NiBlendAccumTransformInterpolator* self, int,
                                      NiBlendAccumTransformInterpolator* dest, void* arg)
{
    reinterpret_cast<void (__thiscall*)(NiBlendInterpolator*, void*, void*)>(
        &CopyBlendInterpolatorMembers)(self, dest, arg);
    dest->m_pkAccumArray = new AccumItem[self->m_ucArraySize];
    for (unsigned char i = 0; i < self->m_ucArraySize; i++)
        dest->m_pkAccumArray[i] = self->m_pkAccumArray[i];
    dest->m_kAccumulatedTransformValue = self->m_kAccumulatedTransformValue;
}

// @fnv 0x00a3a4f0
// pdb: NiBlendAccumTransformInterpolator::LoadBinary | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: kStream
// pdb locals: uc
void __fastcall LoadAccumulatedTransform(NiBlendAccumTransformInterpolator* self, int, NiStream& stream)
{
    self->NiBlendInterpolator::LoadBinary(stream);
    self->m_pkAccumArray = new AccumItem[self->m_ucArraySize];
    for (unsigned char i = 0; i < self->m_ucArraySize; i++)
    {
        AccumItem* item = &self->m_pkAccumArray[i];
        item->value.LoadBinary(stream);
        item->accum.LoadBinary(stream);
        reinterpret_cast<NiMatrix3*>(item->rotation)->LoadBinary(stream);
    }
    self->m_kAccumulatedTransformValue.LoadBinary(stream);
}

struct InterpItem
{
    NiInterpolator* m_pkInterpolator;
    float m_fWeight;
    float m_fNormalizedWeight;
    int m_iPriority;
    float m_fEaseSpinner;
    float m_fUpdateTime;
};

void __fastcall NiBlendAccumTransformInterpolator_CalculateValue(NiBlendAccumTransformInterpolator*, int, unsigned char, float, NiObjectNET*);
bool __fastcall NiBlendAccumTransformInterpolator_BlendValues(NiBlendAccumTransformInterpolator*, int, float, NiObjectNET*, NiQuatTransform&);

// @fnv 0x00a3a580
// pdb: NiBlendAccumTransformInterpolator::Update | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: fTime, pkInterpTarget, kValue
// pdb locals: bReturnValue, uc, fUpdateTime
bool __fastcall UpdateAccumulationInterpolators(NiBlendAccumTransformInterpolator* self, int,
                                              float fTime, NiObjectNET* pkInterpTarget, NiQuatTransform& kValue)
{
    bool bResult = false;
    unsigned char uc;
    float fUpdateTime;
    if (self->m_ucInterpCount == 1)
    {
        bResult = self->StoreSingleValue(fTime, pkInterpTarget, kValue);
    }
    else if (self->m_ucInterpCount > 0)
    {
        self->NormalizeWeights();
        for (uc = 0; uc < self->m_ucArraySize; uc++)
        {
            fUpdateTime = fTime;
            InterpItem* pItem = &reinterpret_cast<InterpItem*>(self->m_pkInterpArray)[uc];
            if (pItem->m_pkInterpolator && pItem->m_fNormalizedWeight != 0.0f)
            {
                if (self->m_uFlags & 1)
                    fUpdateTime = pItem->m_fUpdateTime;
                if (fUpdateTime != DAT_0109696c)
                    reinterpret_cast<void (__thiscall*)(NiBlendAccumTransformInterpolator*, unsigned char, float, NiObjectNET*)>(
                        &NiBlendAccumTransformInterpolator_CalculateValue)(self, uc, fUpdateTime, pkInterpTarget);
            }
        }
        bResult = reinterpret_cast<bool (__thiscall*)(NiBlendAccumTransformInterpolator*, float, NiObjectNET*, NiQuatTransform&)>(
            &NiBlendAccumTransformInterpolator_BlendValues)(self, fTime, pkInterpTarget, kValue);
    }
    self->m_fLastTime = fTime;
    return bResult;
}

// @fnv 0x00a3a660
// pdb: NiBlendAccumTransformInterpolator::CreateClone | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: kCloning
// pdb locals: pkObject
NiBlendAccumTransformInterpolator* __fastcall CloneAccumulatedTransform(NiBlendAccumTransformInterpolator* self,
                                                                      int, void* process)
{
    void* storage = BSFadeNode::operator new(sizeof(NiBlendAccumTransformInterpolator));
    NiBlendAccumTransformInterpolator* result = storage
        ? reinterpret_cast<NiBlendAccumTransformInterpolator* (__thiscall*)(NiBlendAccumTransformInterpolator*)>(
            &ConstructAccumulatedTransform)(static_cast<NiBlendAccumTransformInterpolator*>(storage))
        : 0;
    reinterpret_cast<void (__thiscall*)(NiBlendAccumTransformInterpolator*, NiBlendAccumTransformInterpolator*, void*)>(
        &NiBlendAccumTransformInterpolator_CopyMembers)(self, result, process);
    return result;
}

namespace fnv_00a3a6b0 {

void __fastcall AppendQuatTransformStrings(NiQuatTransform*, int, NiTArray<char*, NiTMallocInterface<char*> >*);
void __fastcall AppendBlendInterpolatorStrings(NiBlendInterpolator*, int, NiTArray<char*, NiTMallocInterface<char*> >*);
extern int NiBlendAccumTransformInterpolator_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011f371c (old: g_011f371c)
char* NiViewerStrings_NiGetViewerString_Title(int);
// Append base strings, the class name and the accumulated transform strings.
// @fnv 0x00a3a6b0
// src: XBOX_PDB 0x00a3a6b0
// pdb: NiBlendAccumTransformInterpolator::GetViewerStrings | NiBlendAccumTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void __fastcall NiBlendAccumTransformInterpolator_GetViewerStrings(NiBlendAccumTransformInterpolator* self, int unused, NiTArray<char*, NiTMallocInterface<char*> >* strs)
{
    reinterpret_cast<void (__thiscall*)(NiBlendInterpolator*, NiTArray<char*, NiTMallocInterface<char*> >*)>(&AppendBlendInterpolatorStrings)(self, strs);
    char* s = NiViewerStrings_NiGetViewerString_Title(NiBlendAccumTransformInterpolator_ms_RTTI);
    unsigned int n = strs->m_usSize;
    if (n >= strs->m_usMaxSize)
        strs->SetSize(n + strs->m_usGrowBy);
    strs->SetAt(n, s);
    reinterpret_cast<void (__thiscall*)(NiQuatTransform*, NiTArray<char*, NiTMallocInterface<char*> >*)>(&AppendQuatTransformStrings)(
        &self->m_kAccumulatedTransformValue, strs);
}
}  // namespace fnv_00a3a6b0

void _fnv_CopyFilePath(char*, unsigned int, const char*, unsigned int);
// Copies a file path from the source buffer into the destination buffer using the given sizes and returns the destination pointer.
// @fnv 0x00a574b0
char* CopyFilePath(char* pcDestination, unsigned int uiDestinationSize, const char* pcSource, unsigned int uiSourceSize)
{
    _fnv_CopyFilePath(pcDestination, uiDestinationSize, pcSource, uiSourceSize);
    return pcDestination;
}

// Updates the interpolator at the current index with the given time and object, accumulating a cycle transform when time moves backwards.
// @fnv 0x00a39530
// src: XBOX_RETAIL_PDB 0x00a39530
// pdb: NiBlendAccumTransformInterpolator::CalculateSingleValue | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: fTime, pkInterpTarget
// pdb locals: kAccumItem, pkInterpolator, bSuccess, fEndTime, kInvBegin, kBeginTransform, fBeginTime, kEndTransform
void NiBlendAccumTransformInterpolator::CalculateSingleValue(float fTime, NiObjectNET* pkInterpTarget)
{
    NiInterpolator* interp = reinterpret_cast<InterpItem*>(m_pkInterpArray)[m_ucSingleIdx].m_pkInterpolator;
    AccumItem& item = m_pkAccumArray[m_ucSingleIdx];
    if (item.time != fTime)
    {
        if (item.time > fTime)
        {
            NiQuatTransform& cycle = item.accum;
            if (cycle.IsMarkedInvalid())
            {
                float begin, end;
                interp->GetInterpolatorTimeRange(begin, end);
                NiQuatTransform first;
                interp->UpdateTransform(begin, pkInterpTarget, first);
                NiQuatTransform last;
                interp->UpdateTransform(end, pkInterpTarget, last);
                NiQuatTransform inverse;
                first.TransformBy(inverse);
                cycle = last.CombineTransforms(inverse);
            }
            if (!m_kAccumulatedTransformValue.IsMarkedInvalid())
                m_kAccumulatedTransformValue = m_kAccumulatedTransformValue.CombineTransforms(cycle);
        }
        interp->UpdateTransform(fTime, pkInterpTarget, item.value);
    }
    item.time = fTime;
}

extern unsigned char DAT_01096968;
extern float g_011a9448[9];

__forceinline void InvalidateAccumulatedValue(AccumItem& item)
{
    item.accum.m_fScale = item.accum.qy = item.accum.m_kTranslate = item.value.m_fScale =
        item.value.qy = item.value.m_kTranslate = item.time = -3.4028234663852886e38f;
    memcpy(item.rotation, g_011a9448, sizeof(item.rotation));
}

// Invalidates the selected slot and, with two interpolators, may combine and invalidate the old slot.
// @fnv 0x00a392b0
// pdb: NiBlendAccumTransformInterpolator::AddInterpInfo | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: pkInterpolator, fWeight, cPriority, fEaseSpinner
// pdb locals: ucSingleIdx, ucIndex
unsigned char __fastcall AddAccumulationInterpolator(NiBlendAccumTransformInterpolator* self, int,
                                                      NiInterpolator* interpolator, float weight, unsigned char priority,
                                                      float ease)
{
    unsigned char oldIndex = self->m_ucSingleIdx;
    unsigned char newIndex = self->NiBlendInterpolator::AddInterpolatorItem(interpolator, weight, static_cast<char>(priority), ease);
    if (newIndex != DAT_01096968)
    {
        InvalidateAccumulatedValue(self->m_pkAccumArray[newIndex]);
        if (self->m_ucInterpCount == 1)
        {
            self->m_bReset = true;
        }
        else if (self->m_ucInterpCount == 2)
        {
            if (!self->m_kAccumulatedTransformValue.IsMarkedInvalid() && !self->m_pkAccumArray[oldIndex].value.IsMarkedInvalid())
            {
                self->m_kAccumulatedTransformValue =
                    self->m_kAccumulatedTransformValue.CombineTransforms(self->m_pkAccumArray[oldIndex].value);
            }
            InvalidateAccumulatedValue(self->m_pkAccumArray[oldIndex]);
        }
    }
    return newIndex;
}

struct MatrixAccum;
struct QQuatOps
{
    float w, x, y, z;
    QQuatOps MultiplyQuaternion(const QQuatOps&) const;
    void NiQuaternion_ToRotation(MatrixAccum*);
};
extern QQuatOps DAT_011a9ea4;
extern NiPoint3 DAT_011f426c;
QQuatOps* __cdecl FUN_00a6dc00(QQuatOps*, const QQuatOps&);
struct MatrixAccum
{
    float m00, m01, m02, m10, m11, m12, m20, m21, m22;
    NiPoint3 FUN_004b4500(const NiPoint3*);
};
__forceinline void SetTransformTranslationAndRotation(NiQuatTransform& value)
{
    *reinterpret_cast<NiPoint3*>(&value.m_kTranslate) = DAT_011f426c;
    *reinterpret_cast<QQuatOps*>(&value.m_kRotate) = DAT_011a9ea4;
}
// Updates the indexed state from the interpolator at time, computing transform deltas and rotation data when needed.
// @fnv 0x00a39f60
// src: XBOX_RETAIL_PDB 0x00a39f60
// pdb: NiBlendAccumTransformInterpolator::CalculateValue | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: ucIndex, fTime, pkInterpTarget
// pdb locals: kAccumItem, pkInterpolator, bSuccess, kAccumRot, kInvLast, kRefFrame, fEndTime, kInvBegin, kBeginTransform, fBeginTime, kNowTransform, kEndTransform, kAccumRotate
void __fastcall NiBlendAccumTransformInterpolator_CalculateValue(NiBlendAccumTransformInterpolator* self, int,
                                       unsigned char index, float time, NiObjectNET* target)
{
    NiInterpolator* interpolator = reinterpret_cast<InterpItem*>(self->m_pkInterpArray)[index].m_pkInterpolator;
    AccumItem* state = self->m_pkAccumArray + index;
    if (!interpolator || state->time == time)
    {
        SetTransformTranslationAndRotation(state->accum);
        state->accum.m_fScale = 1.0f;
        state->time = time;
        return;
    }
    if (self->m_bReset || state->value.IsInvalid())
    {
        interpolator->UpdateTransform(time, target, state->value);
        QQuatOps firstRotation = DAT_011a9ea4;
        if (self->m_kAccumulatedTransformValue.qy != -3.4028234663852886e38)
            firstRotation = *reinterpret_cast<QQuatOps*>(&self->m_kAccumulatedTransformValue.m_kRotate);
        QQuatOps secondRotation = DAT_011a9ea4;
        if (state->value.qy != -3.4028234663852886e38)
        {
            QQuatOps result;
            QQuatOps* __restrict source = FUN_00a6dc00(&result, *reinterpret_cast<QQuatOps*>(&state->value.m_kRotate));
            secondRotation = *source;
        }
        QQuatOps productRotation = firstRotation.MultiplyQuaternion(secondRotation);
        productRotation.NiQuaternion_ToRotation(reinterpret_cast<MatrixAccum*>(state->rotation));
    }
    if (time < state->time && !self->m_bReset)
    {
        float begin, end;
        interpolator->GetInterpolatorTimeRange(begin, end);
        NiQuatTransform last;
        interpolator->UpdateTransform(end, target, last);
        NiQuatTransform first;
        interpolator->UpdateTransform(begin, target, first);
        NiQuatTransform current;
        interpolator->UpdateTransform(time, target, current);
        NiQuatTransform inverse;
        state->value.Invert(&inverse);
        state->accum = inverse.CombineTransforms(static_cast<void*>(&last));
        if (state->accum.m_kTranslate != -3.4028234663852886e38)
            *reinterpret_cast<NiPoint3*>(&state->accum.m_kTranslate) = reinterpret_cast<MatrixAccum*>(state->rotation)->FUN_004b4500(reinterpret_cast<NiPoint3*>(&state->accum.m_kTranslate));
        {
            QQuatOps secondRotation = DAT_011a9ea4;
            if (self->m_kAccumulatedTransformValue.qy != -3.4028234663852886e38)
                secondRotation = *reinterpret_cast<QQuatOps*>(&self->m_kAccumulatedTransformValue.m_kRotate);
            QQuatOps firstRotation = DAT_011a9ea4;
            if (first.qy != -3.4028234663852886e38)
            {
                QQuatOps result;
                QQuatOps* __restrict source = FUN_00a6dc00(&result, *reinterpret_cast<QQuatOps*>(&first.m_kRotate));
                firstRotation = *source;
            }
            QQuatOps productRotation = secondRotation.MultiplyQuaternion(firstRotation);
            productRotation.NiQuaternion_ToRotation(reinterpret_cast<MatrixAccum*>(state->rotation));
        }
        first.Invert(&inverse);
        state->value = inverse.CombineTransforms(static_cast<void*>(&current));
        if (state->value.m_kTranslate != -3.4028234663852886e38)
            *reinterpret_cast<NiPoint3*>(&state->value.m_kTranslate) = reinterpret_cast<MatrixAccum*>(state->rotation)->FUN_004b4500(reinterpret_cast<NiPoint3*>(&state->value.m_kTranslate));
        state->accum = state->accum.CombineTransforms(static_cast<void*>(&state->value));
        state->value = current;
    }
    else
    {
        NiQuatTransform current;
        interpolator->UpdateTransform(time, target, current);
        NiQuatTransform inverse;
        state->value.Invert(&inverse);
        state->accum = inverse.CombineTransforms(static_cast<void*>(&current));
        if (state->accum.m_kTranslate != -3.4028234663852886e38)
            *reinterpret_cast<NiPoint3*>(&state->accum.m_kTranslate) = reinterpret_cast<MatrixAccum*>(state->rotation)->FUN_004b4500(reinterpret_cast<NiPoint3*>(&state->accum.m_kTranslate));
        state->value = current;
    }
    self->m_bReset = false;
    state->time = time;
}

struct Inner_00a6db50
{
    float w, x, y, z;
    void Set(float w, float x, float y, float z);
    Inner_00a6db50 Negate() const;
    Inner_00a6db50 ScaleFourComponents(float factor) const;
    Inner_00a6db50& operator+=(const Inner_00a6db50& other)
    {
        w = other.w + w;
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
};
struct PositionOps
{
    float x, y, z;
    PositionOps operator*(float f) const
    {
        PositionOps result;
        result.x = x * f;
        result.y = y * f;
        result.z = z * f;
        return result;
    }
    PositionOps& operator+=(const PositionOps& other)
    {
        x = x + other.x;
        y = y + other.y;
        z = z + other.z;
        return *this;
    }
};
// Combines available accumulated transform components, copies the result to output, and returns false if none are available or the result is invalid.
// @fnv 0x00a39960
// src: XBOX_RETAIL_PDB 0x00a39960
// pdb: NiBlendAccumTransformInterpolator::BlendValues | NiBlendAccumTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: fTime, pkInterpTarget, kValue
// pdb locals: kFinalTranslate, bTransChanged, kFinalRotate, fFinalScale, fTotalScaleWeight, bAccumTransformInvalid, bRotChanged, bFirstRotation, bScaleChanged, fTotalTransWeight, uc, kAccumItem, kItem, kTransform, kRotValue, fCos, kFinalTransform
bool __fastcall NiBlendAccumTransformInterpolator_BlendValues(NiBlendAccumTransformInterpolator* self, int, float time,
                                      NiObjectNET* target, NiQuatTransform& output)
{
    float positionWeight = 1.0f;
    float current = 1.0f;
    Inner_00a6db50 firstRotation;
    PositionOps kAccumulatedPosition = *reinterpret_cast<const PositionOps*>(&DAT_011f426c);
    firstRotation.Set(0.0f, 0.0f, 0.0f, 0.0f);
    bool rot = false;
    bool hasRotation = false;
    float z = 0.0f;
    bool item = false;
    bool j = self->m_kAccumulatedTransformValue.IsMarkedInvalid();
    if (j)
    {
        *reinterpret_cast<NiPoint3*>(&self->m_kAccumulatedTransformValue.m_kTranslate) = DAT_011f426c;
        NiQuaternion& positionSum = *reinterpret_cast<NiQuaternion*>(&self->m_kAccumulatedTransformValue.m_kRotate);
        positionSum = *reinterpret_cast<const NiQuaternion*>(&DAT_011a9ea4);
        self->m_kAccumulatedTransformValue.m_fScale = 1.0f;
    }
    bool bFirstRotation = true;
    for (unsigned char i = 0; i < self->m_ucArraySize; ++i)
    {
        InterpItem& kInterpItem = reinterpret_cast<InterpItem*>(self->m_pkInterpArray)[i];
        AccumItem& b = self->m_pkAccumArray[i];
        if (!kInterpItem.m_pkInterpolator || !(kInterpItem.m_fNormalizedWeight > 0.00001f))
            continue;
        // Select a complete transform without invoking the default constructor.
        float kSourceTransformStorage[sizeof(NiQuatTransform) / sizeof(float)];
        NiQuatTransform& kSourceTransform = *reinterpret_cast<NiQuatTransform*>(kSourceTransformStorage);
        const float& temp = kInterpItem.m_fNormalizedWeight;
        if (j)
            kSourceTransform = b.value;
        else
            kSourceTransform = b.accum;
        if (kSourceTransform.m_kTranslate != -3.4028234663852886e38)
        {
            rot = true;
            PositionOps kWeightedPosition = *reinterpret_cast<PositionOps*>(&kSourceTransform.m_kTranslate) * temp;
            kAccumulatedPosition.x = kAccumulatedPosition.x + kWeightedPosition.x;
            kAccumulatedPosition.y += kWeightedPosition.y;
            kAccumulatedPosition.z += kWeightedPosition.z;
        }
        else
        {
            positionWeight -= temp;
        }
        if (kSourceTransform.qy != -3.4028234663852886e38)
        {
            Inner_00a6db50 kSourceRotation;
            kSourceRotation = *reinterpret_cast<Inner_00a6db50*>(&kSourceTransform.m_kRotate);
            if (!bFirstRotation)
            {
                double x = kSourceRotation.x * firstRotation.x;
                float r = firstRotation.w * kSourceRotation.w + x + kSourceRotation.y * firstRotation.y +
                          kSourceRotation.z * firstRotation.z;
                if (r < 0.0f)
                    kSourceRotation = kSourceRotation.Negate();
            }
            else
            {
                bFirstRotation = false;
            }
            kSourceRotation = kSourceRotation.ScaleFourComponents(temp);
            firstRotation += kSourceRotation;
            hasRotation = true;
        }
        if (kSourceTransform.m_fScale != -3.4028234663852886e38)
        {
            item = true;
            z += temp * kSourceTransform.m_fScale;
        }
        else
        {
            current -= temp;
        }
    }
    if (rot || hasRotation || item)
    {
        float kIterStorage[sizeof(NiQuatTransform) / sizeof(float)];
        NiQuatTransform& kIter = *reinterpret_cast<NiQuatTransform*>(kIterStorage);
        kIter.m_fScale = DAT_01096b8c;
        *reinterpret_cast<NiPoint3*>(&kIter.m_kTranslate) = DAT_011a8400;
        *reinterpret_cast<NiQuaternion*>(&kIter.m_kRotate) = DAT_011f3704;
        if (rot)
        {
            const float fInvPositionWeight = 1.0f / positionWeight;
            kAccumulatedPosition.x = fInvPositionWeight * kAccumulatedPosition.x;
            const double inv = fInvPositionWeight;
            kAccumulatedPosition.y = inv * kAccumulatedPosition.y;
            kAccumulatedPosition.z = inv * kAccumulatedPosition.z;
            *reinterpret_cast<NiPoint3*>(&kIter.m_kTranslate) = *reinterpret_cast<NiPoint3*>(&kAccumulatedPosition);
        }
        if (hasRotation)
        {
            double scaleWeight = firstRotation.x;
            float fRotationLength =
                (float)sqrt((float)(firstRotation.w * firstRotation.w + scaleWeight * scaleWeight +
                                    firstRotation.y * firstRotation.y + firstRotation.z * firstRotation.z));
            const float invW = (float)(1.0f / fRotationLength);
            firstRotation.w = invW * firstRotation.w;
            const double inv = invW;
            firstRotation.x = inv * firstRotation.x;
            firstRotation.y = firstRotation.y * inv;
            firstRotation.z = firstRotation.z * inv;
            *reinterpret_cast<NiQuaternion*>(&kIter.m_kRotate) = *reinterpret_cast<NiQuaternion*>(&firstRotation);
        }
        if (item)
        {
            if (current > 0.00001f)
                kIter.m_fScale = (1.0f / current) * z;
        }
        self->m_kAccumulatedTransformValue = self->m_kAccumulatedTransformValue.CombineTransforms(static_cast<void*>(&kIter));
        if (self->m_kAccumulatedTransformValue.IsMarkedInvalid())
            return false;
        output = self->m_kAccumulatedTransformValue;
        return true;
    }
    return false;
}
