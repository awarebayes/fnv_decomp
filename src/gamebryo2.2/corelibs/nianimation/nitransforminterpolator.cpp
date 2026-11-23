// gamebryo2.2/corelibs/nianimation/nitransforminterpolator.cpp: NiTransformInterpolator (26 functions)

#include "../../../../include/NiObject.h"
#include "../../../../include/NiPoint3.h"
#include "../../../../include/NiPointer.h"
#include "../../../../include/NiQuatTransform.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiTransformInterpolator.h"
#include "../../../../include/BSFadeNode.h"
#include "../../../../include/BSRotAccumTransfInterpolator.h"
#include "../../../../include/StrArr.h"
#include "../../../../include/NiTransformData.h"
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(volatile long*);
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(volatile long*);

inline void NiRefObject::IncRefCount()
{
    InterlockedIncrement((volatile long*)&m_uiRefCount);
}
inline void NiRefObject::DecRefCount()
{
    if (InterlockedDecrement((volatile long*)&m_uiRefCount) == 0)
        DeleteThis();
}

extern NiPoint3 DAT_011a8400;
extern NiQuaternion DAT_011f3704;
extern float DAT_01096b8c;
struct TransformWords
{
    unsigned int words[8];
};
static inline void CopyTransformRotation(NiQuaternion& destination, const NiQuaternion& source)
{
    *(unsigned int*)&destination.m_fW = *(const unsigned int*)&source.m_fW;
    *(unsigned int*)&destination.m_fX = *(const unsigned int*)&source.m_fX;
    *(unsigned int*)&destination.m_fY = *(const unsigned int*)&source.m_fY;
    *(unsigned int*)&destination.m_fZ = *(const unsigned int*)&source.m_fZ;
}
inline NiQuatTransform::NiQuatTransform()
{
    *(NiPoint3*)&m_kTranslate = DAT_011a8400;
    CopyTransformRotation(*(NiQuaternion*)&m_kRotate, DAT_011f3704);
    m_fScale = DAT_01096b8c;
}
inline NiQuatTransform& NiQuatTransform::operator=(const NiQuatTransform& rhs)
{
    *(TransformWords*)this = *(const TransformWords*)&rhs;
    return *this;
}

class PositionKeyInterpolator : public NiAnimationKey
{
  public:
    static NiPoint3 InterpolatePosition(float, PositionKeyInterpolator*, NiAnimationKey::KeyType, unsigned int,
                                        unsigned int&, unsigned char);
};
class TimedRotationKey : public NiAnimationKey
{
  public:
    static NiQuaternion InterpolatePosition(float, TimedRotationKey*, NiAnimationKey::KeyType, unsigned int,
                                            unsigned int&, unsigned char);
};
class TimedAnimationKey : public NiAnimationKey
{
  public:
    static float InterpolatePosition(float, TimedAnimationKey*, NiAnimationKey::KeyType, unsigned int, unsigned int&,
                                     unsigned char);
};

template <> inline NiPointer<NiTransformData>::NiPointer(NiTransformData* object) : m_pObject(object)
{
    if (m_pObject)
        InterlockedIncrement((volatile long*)((char*)m_pObject + 4));
}
template <> inline NiPointer<NiTransformData>::~NiPointer()
{
    if (m_pObject != 0)
        m_pObject->DecRefCount();
}
template <> inline NiPointer<NiTransformData>& NiPointer<NiTransformData>::operator=(NiTransformData* object)
{
    if (m_pObject != object)
    {
        if (m_pObject != 0)
            m_pObject->DecRefCount();
        m_pObject = object;
        if (m_pObject != 0)
            m_pObject->IncRefCount();
    }
    return *this;
}

// Key-array convenience methods are absent from the shared interpolator header.
struct TransformInterpolatorKeys : NiTransformInterpolator
{
    PositionKeyInterpolator* FUN_00a3f990(unsigned int&, NiAnimationKey::KeyType&, unsigned char&);
    TimedRotationKey* NiTransformInterpolator_GetRotData(unsigned int&, NiAnimationKey::KeyType&, unsigned char&);
    TimedAnimationKey* NiTransformInterpolator_GetScaleData(unsigned int&, NiAnimationKey::KeyType&, unsigned char&);
};

// @fnv 0x00a3fae0
// pdb: NiTransformInterpolator::NiTransformInterpolator | NiTransformInterpolator.obj | retail, call graph, medium
// pdb params: pkData
NiTransformInterpolator::NiTransformInterpolator(NiQuatTransform kTransform) throw()
    : m_kTransformValue(kTransform), m_spData(0)
{
    m_usLastTransIdx = 0;
    m_usLastRotIdx = 0;
    m_usLastScaleIdx = 0;
    bPose = false;
}

static inline bool TransformIsValid(const NiQuatTransform& value)
{
    return value.m_fScale != -3.4028234663852886e38 || value.qy != -3.4028234663852886e38 ||
           value.m_kTranslate != -3.4028234663852886e38;
}
static inline void SetTransformPosition(NiQuatTransform& transform, const NiPoint3& position)
{
    *(unsigned int*)&transform.m_kTranslate = *(const unsigned int*)&position.x;
    *(unsigned int*)&transform.y = *(const unsigned int*)&position.y;
    *(unsigned int*)&transform.z = *(const unsigned int*)&position.z;
}
// @fnv 0x00a3fdb0
// pdb: NiTransformInterpolator::Update | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: fTime, pkInterpTarget, kValue
// pdb locals: uiNumKeys, pkTransKeys, eScaleType, uiTempLast, eRotType, ucSize, pkRotKeys, pkScaleKeys, eTransType
bool NiTransformInterpolator::Update(float fTime, NiObjectNET* pkInterpTarget, NiQuatTransform& kValue)
{
    if (bPose)
    {
        m_fLastTime = fTime;
        kValue = m_kTransformValue;
        return true;
    }
    if (m_fLastTime == fTime)
    {
        kValue = m_kTransformValue;
        if (!TransformIsValid(m_kTransformValue))
            return false;
        return true;
    }
    unsigned int count;
    NiAnimationKey::KeyType eKeyType;
    unsigned char size;
    unsigned int last;
    PositionKeyInterpolator* pTranslateKeys = ((TransformInterpolatorKeys*)this)->FUN_00a3f990(count, eKeyType, size);
    if (count > 0)
    {
        last = m_usLastTransIdx;
        SetTransformPosition(m_kTransformValue,
            PositionKeyInterpolator::InterpolatePosition(fTime, pTranslateKeys, eKeyType, count, last, size));
        m_usLastTransIdx = (unsigned short)last;
    }
    TimedRotationKey* pRotateKeys = ((TransformInterpolatorKeys*)this)->NiTransformInterpolator_GetRotData(count, eKeyType, size);
    if (count > 0)
    {
        last = m_usLastRotIdx;
        CopyTransformRotation(*(NiQuaternion*)&m_kTransformValue.m_kRotate,
            TimedRotationKey::InterpolatePosition(fTime, pRotateKeys, eKeyType, count, last, size));
        m_usLastRotIdx = (unsigned short)last;
    }
    TimedAnimationKey* pScaleKeys = ((TransformInterpolatorKeys*)this)->NiTransformInterpolator_GetScaleData(count, eKeyType, size);
    if (count > 0)
    {
        last = m_usLastScaleIdx;
        m_kTransformValue.m_fScale =
            TimedAnimationKey::InterpolatePosition(fTime, pScaleKeys, eKeyType, count, last, size);
        m_usLastScaleIdx = (unsigned short)last;
    }
    kValue = m_kTransformValue;
    if (!TransformIsValid(m_kTransformValue))
        return false;
    m_fLastTime = fTime;
    return true;
}

void __fastcall UpdateState_00a3ca30(void* pThis);
void __cdecl FUN_00aa1460(void* p, unsigned int s);

// @fnv 0x00a40410
// pdb: NiTransformInterpolator::`scalar deleting destructor' | NiTransformInterpolator.obj | retail, object-file gap fill, high
void* __fastcall FUN_00a40410(NiTransformInterpolator* pThis, int, unsigned int flags)
{
    NiRefObject* pkData = *(NiRefObject**)((char*)pThis + 0x2c);
    if (pkData)
        pkData->DecRefCount();
    UpdateState_00a3ca30(pThis);
    if (flags & 1)
        FUN_00aa1460(pThis, 0x48);
    return pThis;
}

enum KeyContent
{
    KEY_CONTENT_NONE = 0,
    KEY_CONTENT_QUATERNION = 1,
    KEY_CONTENT_POSITION = 2,
    KEY_CONTENT_ROTATION = 3,
    KEY_CONTENT_SCALE = 4,
    KEY_CONTENT_NORMAL = 5,
    KEY_CONTENT_TANGENT = 6
};

// Maps a channel index to a KeyContent: 0 quaternion, 1 position, 2 none, otherwise tangent.
// @fnv 0x00a3f570
// src: XBOX_PDB 0x00a3f570
// pdb: NiTransformInterpolator::GetKeyContent | NiTransformInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
KeyContent __stdcall NiTransformInterpolator_GetKeyContent(unsigned short usChannel)
{
    if (usChannel == 0)
        return KEY_CONTENT_QUATERNION;
    if (usChannel == 1)
        return KEY_CONTENT_POSITION;
    if (usChannel == 2)
        return KEY_CONTENT_NONE;
    return KEY_CONTENT_TANGENT;
}

// Channel key types (data offsets 0x10-0x18) are not declared by the shared interpolator header.
struct TransformChannelTypeAccess : NiTransformInterpolator
{
    void* GetChannelType(short channel);
};

// Returns the pointer stored at offset 0x14, 0x10 or 0x18 of the object at this+0x2c depending on a (0, 1, 2), or null.
// @fnv 0x00a3f600
void* TransformChannelTypeAccess::GetChannelType(short aiIndex)
{
    if (aiIndex == 0)
    {
        void* p = *(void**)((char*)this + 0x2c);
        if (p)
            return *(void**)((char*)p + 0x14);
    }
    else if (aiIndex == 1)
    {
        void* p = *(void**)((char*)this + 0x2c);
        if (p)
            return *(void**)((char*)p + 0x10);
    }
    else if (aiIndex == 2)
    {
        void* p = *(void**)((char*)this + 0x2c);
        if (p)
            return *(void**)((char*)p + 0x18);
    }
    return 0;
}

// Returns a field from the object at offset 0x2c based on arg (0, 1, or 2), or 0 if that pointer is null or arg is other.
// @fnv 0x00a3f650
NiAnimationKey* NiTransformInterpolator::GetChannelKeys(unsigned short aiIndex) const
{
    if (aiIndex == 0)
    {
        void* p = *(void**)((char*)this + 0x2c);
        if (p)
            return *(NiAnimationKey**)((char*)p + 0x24);
    }
    else if (aiIndex == 1)
    {
        void* p = *(void**)((char*)this + 0x2c);
        if (p)
            return *(NiAnimationKey**)((char*)p + 0x20);
    }
    else if (aiIndex == 2)
    {
        void* p = *(void**)((char*)this + 0x2c);
        if (p)
            return *(NiAnimationKey**)((char*)p + 0x28);
    }
    return 0;
}

// Reads fields at offsets 0xa, 0x14, 0x1d and 0x24 from the object at this+0x2c into the out parameters and returns the last; zeroes them if that pointer is null.
// @fnv 0x00a3f990
// pdb: NiTransformInterpolator::GetPosData | NiTransformInterpolator.obj | debug, object-file run extension, low
// pdb params: uiNumKeys, eType, ucSize
// Pointer-based position-key accessor used by 0x00a3f990; the shared header lacks this overload.
struct PositionChannelAccess : NiTransformInterpolator
{
    int GetPositionKeys(unsigned int*, int*, unsigned char*);
};
int PositionChannelAccess::GetPositionKeys(unsigned int* puiValue, int* piValue, unsigned char* pucValue)
{
    void* p = m_spData.m_pObject;
    if (p)
    {
        *puiValue = *(unsigned short*)((char*)p + 0xa);
        *piValue = *(int*)((char*)p + 0x14);
        *pucValue = *(unsigned char*)((char*)p + 0x1d);
        return *(int*)((char*)p + 0x24);
    }
    *puiValue = 0;
    *piValue = 0;
    *pucValue = 0;
    return 0;
}

extern "C" const char* g_011f3ee0_arg1;

// @fnv 0x00a3fad0
const NiRTTI* NiTransformInterpolator::GetRTTI() const
{
    return (const NiRTTI*)&g_011f3ee0_arg1;
}

// Time-range operations not yet declared by the shared header.
struct NiTransformDataTimeRange : NiTransformData
{
    void NiTransformData_GetSequenceData(float, float, NiTransformData**);
    void GuaranteeTimeRange(float, float);
};
// The shared class has no transform-data setter; this adapter adds only its declaration.
struct TransformDataAssignment : NiTransformInterpolator
{
    void SetTransformData(NiTransformData* pkData);
};
__declspec(dllimport) long __stdcall InterlockedDecrement(volatile long*);

// @fnv 0x00a3fd10
// pdb: NiTransformInterpolator::GetSequenceInterpolator | NiTransformInterpolator.obj | debug, object-file run extension, low
// pdb params: fStartTime, fEndTime
// pdb locals: pkSeqInterp, spNewData
NiInterpolator* NiTransformInterpolator::CloneForTimeRange(float m_fStartTime, float m_fMaximumDistance)
{
    NiTransformInterpolator* pkSeqInterp =
        (NiTransformInterpolator*)NiInterpolator::CloneForTimeRange(m_fStartTime, m_fMaximumDistance);
    if (pkSeqInterp && *(void**)((char*)this + 0x2c))
    {
        NiTransformData* spNewData = 0;
        ((NiTransformDataTimeRange*)m_spData.m_pObject)->NiTransformData_GetSequenceData(m_fStartTime, m_fMaximumDistance, &spNewData);
        ((TransformDataAssignment*)pkSeqInterp)->SetTransformData(spNewData);
        *(unsigned short*)((char*)pkSeqInterp + 0x30) = 0;
        *(unsigned short*)((char*)pkSeqInterp + 0x32) = 0;
        *(unsigned short*)((char*)pkSeqInterp + 0x34) = 0;
        if (spNewData)
        {
            spNewData->DecRefCount();
        }
    }
    return (NiInterpolator*)pkSeqInterp;
}

// Returns the unsigned short at offset 0xa, 0x8 or 0xc of the data pointer at this+0x2c for channel 0, 1 or 2, else 0.
// @fnv 0x00a3f5b0
unsigned int NiTransformInterpolator::GetColorKeyCount(unsigned short usChannel) const
{
    if (usChannel == 0)
    {
        char* pkData = *(char**)((char*)this + 0x2c);
        if (pkData)
            return *(unsigned short*)(pkData + 0xa);
    }
    else if (usChannel == 1)
    {
        char* pkData = *(char**)((char*)this + 0x2c);
        if (pkData)
            return *(unsigned short*)(pkData + 0x8);
    }
    else if (usChannel == 2)
    {
        char* pkData = *(char**)((char*)this + 0x2c);
        if (pkData)
            return *(unsigned short*)(pkData + 0xc);
    }
    return 0;
}

// Returns the bool flag at offset 0x1d, 0x1c or 0x1e of the data pointer at this+0x2c for channel 0, 1 or 2; false otherwise.
// @fnv 0x00a3f6a0
unsigned char NiTransformInterpolator::GetColorKeySize(unsigned short usChannel) const
{
    if (usChannel == 0)
    {
        char* pkData = *(char**)((char*)this + 0x2c);
        if (pkData)
            return *(unsigned char*)(pkData + 0x1d);
    }
    else if (usChannel == 1)
    {
        char* pkData = *(char**)((char*)this + 0x2c);
        if (pkData)
            return *(unsigned char*)(pkData + 0x1c);
    }
    else if (usChannel == 2)
    {
        char* pkData = *(char**)((char*)this + 0x2c);
        if (pkData)
            return *(unsigned char*)(pkData + 0x1e);
    }
    return false;
}

// @fnv 0x00a3f6f0
void NiTransformInterpolator::SetActiveTimeRange(float afArg1, float afArg2)
{
    NiTransformDataTimeRange* pkData = (NiTransformDataTimeRange*)m_spData.m_pObject;
    if (pkData)
        pkData->GuaranteeTimeRange(afArg1, afArg2);
}

// Returns the scale key array from the interpolator's transform data, filling in key count, interpolation type and key size, or zeroes if no data exists.
// @fnv 0x00a3f9e0
// src: COMMUNITY 0x00a3f9e0
// pdb: NiTransformInterpolator::GetScaleData | NiTransformInterpolator.obj | debug, upstream name already matches, high
// pdb params: uiNumKeys, eType, ucSize
TimedAnimationKey* TransformInterpolatorKeys::NiTransformInterpolator_GetScaleData(unsigned int& uiNumKeys,
                                                         NiAnimationKey::KeyType& eType,
                                                         unsigned char& ucSize)
{
    NiTransformData* pkData = *(NiTransformData**)&m_spData;
    if (pkData)
    {
        uiNumKeys = pkData->m_usNumScaleKeys;
        eType = (NiAnimationKey::KeyType)pkData->m_eScaleType;
        ucSize = pkData->m_ucScaleSize;
        return pkData->m_pkScaleKeyArray;
    }
    uiNumKeys = 0;
    eType = NiAnimationKey::NOINTERP;
    ucSize = 0;
    return 0;
}

struct CloningProcess
{
    void __dflt_ctor_closure();
    void* __vecDelDtor(unsigned int);
    void* m_pCloneMap;
    void* m_pProcessMap;
    int m_eCloneCopyMode;
    char m_bExactType;
    char _pad_d[3];
};
typedef char _sizecheck_NiCloningProcess[1];
inline void* operator new(unsigned int, void* pkMem)
{
    return pkMem;
}
// Allocates a new NiTransformInterpolator and copies this interpolator's data into it via FUN_00a40360, returning it as a NiObject.
// @fnv 0x00a40460
// pdb: NiTransformInterpolator::CreateClone | NiTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: kCloning
// pdb locals: pkObject
NiObject* NiTransformInterpolator::MakeClone(CloningProcess& kCloning)
{
    NiTransformInterpolator* pkObject = ::new (BSFadeNode::operator new(sizeof(NiTransformInterpolator))) NiTransformInterpolator((NiTransformData*)0);
    CopyMembers(pkObject, kCloning);
    return (NiObject*)pkObject;
}

extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long volatile*);
extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long volatile*);
// Replaces the held NiTransformData pointer with pkData, adjusting reference counts, and resets the three counters to zero.
// @fnv 0x00a3f930
// pdb: NiTransformInterpolator::SetTransformData | NiTransformInterpolator.obj | debug, object-file run extension, low
// pdb params: pkData
void TransformDataAssignment::SetTransformData(NiTransformData* pkData)
{
    if (m_spData.m_pObject != pkData)
    {
        if (m_spData.m_pObject)
            m_spData.m_pObject->DecRefCount();
        m_spData.m_pObject = pkData;
        if (pkData)
            pkData->IncRefCount();
    }
    m_usLastRotIdx = 0;
    m_usLastScaleIdx = 0;
    m_usLastTransIdx = 0;
}

// @fnv 0x00a3fa30
// pdb: NiTransformInterpolator::NiTransformInterpolator | NiTransformInterpolator.obj | retail, call graph, low
// pdb note: debug build links it to NiPointer<NiTransformData>::operator=
// pdb params: pkData
NiTransformInterpolator::NiTransformInterpolator(NiTransformData* pkData) : m_spData(pkData)
{
    m_kTransformValue.m_kTranslate = -3.402823466e+38F;
    m_kTransformValue.qy = -3.402823466e+38F;
    m_kTransformValue.m_fScale = -3.402823466e+38F;
    m_usLastTransIdx = 0;
    m_usLastRotIdx = 0;
    m_usLastScaleIdx = 0;
    bPose = false;
}

namespace fnv_00a3fb20 {
struct TransformAnimationData : public TimedRotationKey
{
    static int RegisterObjectLoader();
    static void RegisterSupportedFunctions(int, int);
    static NiAnimationKey* CreateObjectFromStream(NiStream*, unsigned int);
    void LoadBinary(NiStream*);
    static void SaveToStream(NiStream*, NiAnimationKey*, unsigned int);
    static void SaveBinary(NiStream*, NiAnimationKey*);
    static void Copy(NiAnimationKey*, void*);
    static NiAnimationKey* CreateArray(unsigned int);
    static void DeleteArray(NiAnimationKey*);
    static bool Insert(float, NiAnimationKey**, unsigned int*);
    static float ComputeMaxCurvature(NiAnimationKey*, unsigned int);
    static void Interpolate(float, void*, void*, void*);
    static void InterpolateD1(float, void*, void*, void*);
    static void InterpolateD2(float, void*, void*, void*);
    static bool Equal(void*, void*);
    static void FillDerivedVals(NiAnimationKey*, unsigned int, unsigned char);
    void Initialize(TimedAnimationKey*, unsigned int, int, TimedAnimationKey*, unsigned int, int, TimedAnimationKey*,
                    unsigned int, int);
    void CleanUp();
    void SetKeyData(unsigned char, TimedAnimationKey*);
    void SetNumKeys(unsigned char, unsigned int);
    void SetType(unsigned char, int);
    void SetAnimationKeys(unsigned char, unsigned int, TimedAnimationKey*);
    TimedAnimationKey* GetKeys(unsigned char);
    unsigned char GetKeySize(unsigned char);
    unsigned int GetNumKeys(unsigned char);
    int GetType(unsigned char);
    void* __vecDelDtor(unsigned int);
    char _pad_4[16];
    char m_uiNumKeys[12];
    char m_eRequestType[12];
    char m_ucKeyTypes[3];
    char _pad_2f[1];
    char m_ppKeyData[12];
    char m_uiLastKeyIndex[12];
};
typedef char _sizecheck_NiEulerRotKey[1];
extern float DAT_01097308;
extern float _DAT_01015f5c;
struct EulerK
{
    char pad[0x14];
    unsigned int m_uiNumKeys[3];
    int m_eRequestType[3];
    unsigned char m_ucKeyTypes[3];
    char pad2f;
    char* m_ppKeyData[3];
};
static inline char* TI_GetPos(NiTransformData* d, unsigned int& n, int& t, unsigned char& s)
{
    char* pk = 0;
    if (d)
    {
        n = d->m_usNumPosKeys;
        t = d->m_ePosType;
        s = d->m_ucPosSize;
        pk = (char*)d->m_pkPositionKeys;
    }
    else
    {
        n = 0;
    }
    return pk;
}
static inline char* TI_GetRot(NiTransformData* d, unsigned int& n, int& t, unsigned char& s)
{
    char* pk = 0;
    if (d)
    {
        n = d->m_usNumRotKeys;
        t = d->m_eRotationKeyType;
        s = d->m_ucRotSize;
        pk = (char*)d->m_pkRotationKeys;
    }
    else
    {
        n = 0;
    }
    return pk;
}
static inline char* TI_GetScale(NiTransformData* d, unsigned int& n, int& t, unsigned char& s)
{
    char* pk = 0;
    if (d)
    {
        n = d->m_usNumScaleKeys;
        t = d->m_eScaleType;
        s = d->m_ucScaleSize;
        pk = (char*)d->m_pkScaleKeyArray;
    }
    else
    {
        n = 0;
    }
    return pk;
}
static inline float TI_Time(char* keys, unsigned int i, unsigned char s)
{
    return *(float*)(keys + i * s);
}

}  // namespace fnv_00a3fb20

// Computes the first and last key times across the position, rotation and scale key arrays, defaulting both to 0 when no keys exist.
// @fnv 0x00a3fb20
void NiTransformInterpolator::GetInterpolatorTimeRange(float& begin, float& end) const
{
    using namespace fnv_00a3fb20;
    float* fBeginKeyTime = &begin;
    float* fEndKeyTime = &end;
    *fBeginKeyTime = DAT_01097308;
    *fEndKeyTime = _DAT_01015f5c;
    bool bHasKeys = false;

    unsigned int uiNumKeys;
    int eTransType;
    unsigned char ucSize;
    char* pkTransKeys = TI_GetPos(*(NiTransformData**)((char*)this + 0x2c), uiNumKeys, eTransType, ucSize);
    if (uiNumKeys > 0)
    {
        float fKeyTime = TI_Time(pkTransKeys, 0, ucSize);
        if (fKeyTime < *fBeginKeyTime)
            *fBeginKeyTime = fKeyTime;
        fKeyTime = TI_Time(pkTransKeys, uiNumKeys - 1, ucSize);
        if (fKeyTime > *fEndKeyTime)
            *fEndKeyTime = fKeyTime;
        bHasKeys = true;
    }

    int eRotType;
    char* pkRotKeys = TI_GetRot(*(NiTransformData**)((char*)this + 0x2c), uiNumKeys, eRotType, ucSize);
    if (uiNumKeys > 0)
    {
        if (eRotType == 4)
        {
            EulerK* pkEulerKey = (EulerK*)pkRotKeys;
            for (unsigned char ucAxis = 0; ucAxis < 3; ucAxis++)
            {
                unsigned int uiNumFloatKeys = pkEulerKey->m_uiNumKeys[ucAxis];
                if (uiNumFloatKeys > 0)
                {
                    char* pkFloatKeys = pkEulerKey->m_ppKeyData[ucAxis];
                    unsigned char ucFloatKeySize = pkEulerKey->m_ucKeyTypes[ucAxis];
                    float fKeyTime = TI_Time(pkFloatKeys, 0, ucFloatKeySize);
                    if (fKeyTime < *fBeginKeyTime)
                        *fBeginKeyTime = fKeyTime;
                    fKeyTime = TI_Time(pkFloatKeys, uiNumFloatKeys - 1, ucFloatKeySize);
                    if (fKeyTime > *fEndKeyTime)
                        *fEndKeyTime = fKeyTime;
                    bHasKeys = true;
                }
            }
        }
        else
        {
            float fKeyTime = TI_Time(pkRotKeys, 0, ucSize);
            if (fKeyTime < *fBeginKeyTime)
                *fBeginKeyTime = fKeyTime;
            fKeyTime = TI_Time(pkRotKeys, uiNumKeys - 1, ucSize);
            if (fKeyTime > *fEndKeyTime)
                *fEndKeyTime = fKeyTime;
            bHasKeys = true;
        }
    }

    int eScaleType;
    char* pkScaleKeys = TI_GetScale(*(NiTransformData**)((char*)this + 0x2c), uiNumKeys, eScaleType, ucSize);
    if (uiNumKeys > 0)
    {
        float fKeyTime = TI_Time(pkScaleKeys, 0, ucSize);
        if (fKeyTime < *fBeginKeyTime)
            *fBeginKeyTime = fKeyTime;
        fKeyTime = TI_Time(pkScaleKeys, uiNumKeys - 1, ucSize);
        if (fKeyTime > *fEndKeyTime)
            *fEndKeyTime = fKeyTime;
        bHasKeys = true;
    }

    if (!bHasKeys)
    {
        *fBeginKeyTime = 0.0f;
        *fEndKeyTime = 0.0f;
    }
}

namespace fnv_00a404b0 {
// Viewer helpers at 0x00a3cbf0 and 0x00a38cb0 are absent from the shared class declarations.
struct InterpolatorViewerBase : NiKeyBasedInterpolator
{
    void GetBaseViewerStrings(StrArr*);
};
struct TransformViewerFields : NiQuatTransform
{
    void GetTransformViewerStrings(StrArr*);
};
extern char* _FUN_00a7d330(char*);
extern char* g_011f3ee0_arg1;
}  // namespace fnv_00a404b0

// @fnv 0x00a404b0
// pdb: NiTransformInterpolator::GetViewerStrings | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void NiTransformInterpolator::GetViewerStrings(NiViewerStringsArray* pkStrings)
{
    using namespace fnv_00a404b0;
    ((InterpolatorViewerBase*)this)->GetBaseViewerStrings((StrArr*)pkStrings);
    char* pString = _FUN_00a7d330(fnv_00a404b0::g_011f3ee0_arg1);
    unsigned int count = ((StrArr*)pkStrings)->m_usSize;
    unsigned int size = ((StrArr*)pkStrings)->m_usMaxSize;
    if (count >= size)
        ((StrArr*)pkStrings)->SetSize(((StrArr*)pkStrings)->m_usGrowBy + count);
    ((StrArr*)pkStrings)->SetAt(count, pString);
    ((TransformViewerFields*)&m_kTransformValue)->GetTransformViewerStrings((StrArr*)pkStrings);
    void* pData = *(void**)((char*)this + 0x2c);
    if (pData)
        ((void(__thiscall*)(void*, StrArr*))(*(void***)pData)[24])(pData, (StrArr*)pkStrings);
}


extern double DAT_010241b0;

// The shared header lacks the channel pose query; reuse its layout and virtual key-count method.
struct TransformChannelPoseQuery : NiTransformInterpolator
{
    bool IsChannelPosed(unsigned short mode);
};

// For modes 0-2, rejects when m_2c is set and v56(mode) > 0, otherwise compares the mode's float field against DAT_010241b0.
// @fnv 0x00a3f720
// pdb: NiTransformInterpolator::GetChannelPosed | NiTransformInterpolator.obj | retail, object-file run extension, low
// pdb params: usChannel
bool TransformChannelPoseQuery::IsChannelPosed(unsigned short mode)
{
    double value;
    switch (mode)
    {
    case 0:
        if (m_spData.m_pObject != 0 && GetColorKeyCount(mode) > 0)
        {
            return false;
        }
        value = m_kTransformValue.m_kTranslate;
        break;
    case 1:
        if (m_spData.m_pObject != 0 && GetColorKeyCount(mode) > 0)
        {
            return false;
        }
        value = m_kTransformValue.qy;
        break;
    case 2:
        if (m_spData.m_pObject != 0 && GetColorKeyCount(mode) > 0)
        {
            return false;
        }
        value = m_kTransformValue.m_fScale;
        break;
    default:
        return false;
    }
    return DAT_010241b0 != value;
}


// The base registration stub at 0x00aa0ad0 is not declared by the shared base class.
struct InterpolatorRegistrationBase : NiKeyBasedInterpolator
{
    bool RegisterBase(NiStream& stream);
};

// Calls FUN_00aa0ad0(arg); if it succeeds, invokes v21(arg) on unk2c when non-null and returns true, otherwise returns false.
// @fnv 0x00a3f870
// pdb: NiTransformInterpolator::RegisterStreamables | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: kStream
bool NiTransformInterpolator::RegisterStreamables(NiStream& kStream)
{
    if (!((InterpolatorRegistrationBase*)this)->RegisterBase(kStream))
        return false;

    NiTransformData* data = m_spData.m_pObject;
    if (data)
        data->RegisterStreamables(kStream);

    return true;
}

// Equality at 0x00a391c0 takes two NiQuatTransforms; its declaration is absent from the shared header.
struct TransformComparison : NiQuatTransform
{
    bool Equals(const NiQuatTransform& kOther) const;
};
template <> inline NiPointer<NiTransformData>::operator NiTransformData*() const
{
    return m_pObject;
}
template <> inline NiTransformData* NiPointer<NiTransformData>::operator->() const
{
    return m_pObject;
}

// The inherited NiObject::IsEqual is a different function; this unit calls the base stub at 0x00c85800.
struct InterpolatorComparisonBase : NiKeyBasedInterpolator
{
    bool BaseIsEqual(NiObject* pkObject) const;
};

// @fnv 0x00a3f8d0
// pdb: NiTransformInterpolator::IsEqual | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkDest
bool NiTransformInterpolator::IsEqual(NiObject* pkObject) const
{
    if (!((const InterpolatorComparisonBase*)this)->BaseIsEqual(pkObject))
        return false;
    NiTransformInterpolator* pkOther = (NiTransformInterpolator*)pkObject;
    if (!((const TransformComparison*)&m_kTransformValue)->Equals(pkOther->m_kTransformValue))
        return false;
    if ((m_spData && !pkOther->m_spData) || (!m_spData && pkOther->m_spData))
        return false;
    if (m_spData && !m_spData->IsEqual(pkOther->m_spData))
        return false;
    return true;
}

// Transform serialization at 0x00a38b60 is absent from NiQuatTransform's shared declarations.
struct TransformBinaryWriter : NiQuatTransform
{
    void SaveTransform(NiStream& stream);
};

// @fnv 0x00a3f8a0
// pdb: NiTransformInterpolator::SaveBinary | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiTransformInterpolator::SaveBinary(NiStream& kStream)
{
    NiObject::SaveBinary(kStream);
    ((TransformBinaryWriter*)&m_kTransformValue)->SaveTransform(kStream);
    kStream.SaveLinkID(m_spData.m_pObject);
}

#include <string.h>

void __stdcall NiKeyBasedInterpolator_CopyMembers(NiTransformInterpolator* pkDest, CloningProcess* kCloning);

template <> inline NiPointer<NiTransformData>& NiPointer<NiTransformData>::operator=(const NiPointer& rhs)
{
    if (m_pObject != rhs.m_pObject)
    {
        if (m_pObject != 0)
            m_pObject->DecRefCount();
        m_pObject = rhs.m_pObject;
        if (m_pObject != 0)
            m_pObject->IncRefCount();
    }
    return *this;
}

// Calls FUN_00aa0ab0, then copies 32 bytes at offset 0x0c plus fields 0x38-0x40, the ref-counted pointer at 0x2c and the shorts at 0x30-0x34 into pkDest.
// @fnv 0x00a40360
// src: XBOX_RETAIL_PDB 0x00a40360
// pdb: NiTransformInterpolator::CopyMembers | NiTransformInterpolator.obj | retail, object-file gap fill, high
// pdb params: pkDest, kCloning
void NiTransformInterpolator::CopyMembers(NiTransformInterpolator* pkDest, CloningProcess& kCloning)
{
    NiKeyBasedInterpolator_CopyMembers(pkDest, &kCloning);

    memcpy(&pkDest->m_kTransformValue, &m_kTransformValue, 32);

    *(unsigned int*)&pkDest->m_origTranslate.x = *(unsigned int*)&m_origTranslate.x;
    *(unsigned int*)&pkDest->m_origTranslate.y = *(unsigned int*)&m_origTranslate.y;
    *(unsigned int*)&pkDest->m_origTranslate.z = *(unsigned int*)&m_origTranslate.z;
    pkDest->m_spData = m_spData;
    pkDest->m_usLastTransIdx = m_usLastTransIdx;
    pkDest->m_usLastRotIdx = m_usLastRotIdx;
    pkDest->m_usLastScaleIdx = m_usLastScaleIdx;
}

namespace fnv_00a403f0 {
void* __cdecl Object_DispatchCommandFour(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* address)
{
    return address;
}
// @fnv 0x00a403f0
// src: EXE 0x00a403f0
// pdb: NiTransformInterpolator::CreateObject | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb locals: pkObject
NiTransformInterpolator* __cdecl NiTransformInterpolator_CreateObject()
{
    void* storage = Object_DispatchCommandFour(sizeof(NiTransformInterpolator));
    if (storage)
        return ::new (storage) NiTransformInterpolator(static_cast<NiTransformData*>(0));
    return 0;
}
}  // namespace fnv_00a403f0

inline bool NiQuatTransform::IsTranslateValid()
{
    return m_kTranslate != -3.4028234663852886e+38;
}
inline bool NiQuatTransform::IsRotateValid()
{
    return qy != -3.4028234663852886e+38;
}
inline bool NiQuatTransform::IsScaleValid()
{
    return m_fScale != -3.4028234663852886e+38;
}
inline bool NiQuatTransform::IsMarkedInvalid()
{
    return !IsScaleValid() && !IsRotateValid() && !IsTranslateValid();
}

// Deserializes the default transform, data object, and pose flag after loading the base class.
// @fnv 0x00a3f7b0
// pdb: NiTransformInterpolator::LoadBinary | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiTransformInterpolator::LoadBinary(NiStream& kStream)
{
    NiKeyBasedInterpolator::LoadBinary(kStream);
    m_kTransformValue.LoadBinary(kStream);
    m_origTranslate = *(NiPoint3*)&m_kTransformValue.m_kTranslate;
    m_spData = (NiTransformData*)kStream.DeserializeObject();
    bPose = !m_spData.m_pObject && !m_kTransformValue.IsMarkedInvalid();
}

__declspec(noinline) bool NiPoint3::operator!=(const NiPoint3& p)
{
    return !(x == p.x && y == p.y && z == p.z);
}

struct S00a3ffd0_PosKey : NiAnimationKey
{
    NiPoint3 m_kPositionValue;
    S00a3ffd0_PosKey* GetKeyPointerByIndex(unsigned int ui, unsigned char ucSize)
    {
        return (S00a3ffd0_PosKey*)((char*)this + ui * ucSize);
    }
};
struct S00a3ffd0_RotKey : NiAnimationKey
{
    NiQuaternion m_kRotationValue;
    S00a3ffd0_RotKey* GetKeyPointerByIndex(unsigned int ui, unsigned char ucSize)
    {
        return (S00a3ffd0_RotKey*)((char*)this + ui * ucSize);
    }
};
struct S00a3ffd0_FloatKey : NiAnimationKey
{
    float m_fValue;
    S00a3ffd0_FloatKey* GetKeyPointerByIndex(unsigned int ui, unsigned char ucSize)
    {
        return (S00a3ffd0_FloatKey*)((char*)this + ui * ucSize);
    }
};
inline bool S00a3ffd0_QEq(const NiQuaternion& a, const NiQuaternion& q)
{
    return a.m_fW == q.m_fW && a.m_fX == q.m_fX && a.m_fY == q.m_fY &&
           a.m_fZ == q.m_fZ;
}
static inline S00a3ffd0_PosKey* GetTransformPositionKeys(const NiTransformData* data, unsigned int& n, int& t, unsigned char& s)
{
    n = data->m_usNumPosKeys;
    t = data->m_ePosType;
    s = data->m_ucPosSize;
    return (S00a3ffd0_PosKey*)data->m_pkPositionKeys;
}
static inline S00a3ffd0_RotKey* GetTransformRotationKeys(const NiTransformData* data, unsigned int& n, int& t, unsigned char& s)
{
    n = data->m_usNumRotKeys;
    t = data->m_eRotationKeyType;
    s = data->m_ucRotSize;
    return (S00a3ffd0_RotKey*)data->m_pkRotationKeys;
}
static inline S00a3ffd0_FloatKey* GetTransformScaleKeys(const NiTransformData* data, unsigned int& n, int& t, unsigned char& s)
{
    n = data->m_usNumScaleKeys;
    t = data->m_eScaleType;
    s = data->m_ucScaleSize;
    return (S00a3ffd0_FloatKey*)data->m_pkScaleKeyArray;
}
// Evaluates position, rotation, and scale keys to determine if they are constant, updating m_kDefaultTransform and clearing m_spTransformData if all are absent.
// @fnv 0x00a3ffd0
// pdb: NiTransformInterpolator::Collapse | NiTransformInterpolator.obj | retail, upstream name already matches, high
// pdb locals: uiNumKeys, pkPosKeys, kCollapsedTransform, ucSize, pkRotKeys, pkScaleKeys, eType, kValue, bCollapse, ui, fValue
void NiTransformInterpolator::Collapse()
{
    if (!m_spData.m_pObject)
        return;
    NiQuatTransform kTransform;
    unsigned int uiNumKeys;
    int eType;
    unsigned char ucSize;
    S00a3ffd0_PosKey* pkPosKeys = GetTransformPositionKeys(m_spData.m_pObject, uiNumKeys, eType, ucSize);
    if (uiNumKeys == 0)
    {
        m_spData.m_pObject->ReplacePosAnim(0, 0, NiAnimationKey::NOINTERP);
        if (m_kTransformValue.m_kTranslate != -3.4028234663852886e38)
            (*(NiPoint3*)&kTransform.m_kTranslate) = (*(NiPoint3*)&m_kTransformValue.m_kTranslate);
    }
    else
    {
        NiPoint3 kValue = pkPosKeys->GetKeyPointerByIndex(0, ucSize)->m_kPositionValue;
        bool positionConstant = false;
        if (uiNumKeys == 1)
        {
            positionConstant = true;
        }
        else if (eType == 1 || eType == 5)
        {
            positionConstant = true;
            for (unsigned int ui = 1; positionConstant && ui < uiNumKeys; ui++)
            {
                if (pkPosKeys->GetKeyPointerByIndex(ui, ucSize)->m_kPositionValue != kValue)
                    positionConstant = false;
            }
        }
        if (positionConstant)
        {
            m_spData.m_pObject->ReplacePosAnim(0, 0, NiAnimationKey::NOINTERP);
            (*(NiPoint3*)&kTransform.m_kTranslate) = kValue;
        }
        else if (m_kTransformValue.m_kTranslate != -3.4028234663852886e38)
            (*(NiPoint3*)&kTransform.m_kTranslate) = (*(NiPoint3*)&m_kTransformValue.m_kTranslate);
    }
    S00a3ffd0_RotKey* pkRotKeys = GetTransformRotationKeys(m_spData.m_pObject, uiNumKeys, eType, ucSize);
    if (uiNumKeys == 0)
    {
        m_spData.m_pObject->ReplaceRotAnim((TimedRotationKey*)uiNumKeys, uiNumKeys, (NiAnimationKey::KeyType)uiNumKeys);
        if (m_kTransformValue.qy != -3.4028234663852886e38)
            CopyTransformRotation(*(NiQuaternion*)&kTransform.m_kRotate, *(NiQuaternion*)&m_kTransformValue.m_kRotate);
    }
    else
    {
        NiQuaternion kValue = pkRotKeys->GetKeyPointerByIndex(0, ucSize)->m_kRotationValue;
        bool rotationConstant;
        try
        {
            if (uiNumKeys == 1 && eType != 4)
                goto constant52;
            if (eType != 1 && eType != 5)
                goto fallback52;
        }
        catch (...)
        {
            throw;
        }
        rotationConstant = true;
        for (unsigned int ui = 1; rotationConstant && ui < uiNumKeys; ui++)
        {
            if (!S00a3ffd0_QEq(pkRotKeys->GetKeyPointerByIndex(ui, ucSize)->m_kRotationValue, kValue))
                rotationConstant = false;
        }
    done52:;
        if (rotationConstant)
        {
        constant52:
            m_spData.m_pObject->ReplaceRotAnim(0, 0, NiAnimationKey::NOINTERP);
            CopyTransformRotation(*(NiQuaternion*)&kTransform.m_kRotate, kValue);
        }
        else
        {
        fallback52:
            if (m_kTransformValue.qy != -3.4028234663852886e38)
                CopyTransformRotation(*(NiQuaternion*)&kTransform.m_kRotate, *(NiQuaternion*)&m_kTransformValue.m_kRotate);
        }
    }
    S00a3ffd0_FloatKey* pkScaleKeys = GetTransformScaleKeys(m_spData.m_pObject, uiNumKeys, eType, ucSize);
    if (uiNumKeys == 0)
    {
        m_spData.m_pObject->ReplaceScaleAnim(0, 0, NiAnimationKey::NOINTERP);
        if (m_kTransformValue.m_fScale != -3.4028234663852886e38)
            kTransform.m_fScale = m_kTransformValue.m_fScale;
    }
    else
    {
        float kValue = pkScaleKeys->GetKeyPointerByIndex(0, ucSize)->m_fValue;
        bool scaleConstant = false;
        if (uiNumKeys == 1)
        {
            scaleConstant = true;
        }
        else if (eType == 1 || eType == 5)
        {
            scaleConstant = true;
            for (unsigned int ui = 1; scaleConstant && ui < uiNumKeys; ui++)
            {
                if (pkScaleKeys->GetKeyPointerByIndex(ui, ucSize)->m_fValue != kValue)
                    scaleConstant = false;
            }
        }
        if (scaleConstant)
        {
            m_spData.m_pObject->ReplaceScaleAnim(0, 0, NiAnimationKey::NOINTERP);
            kTransform.m_fScale = kValue;
        }
        else if (m_kTransformValue.m_fScale != -3.4028234663852886e38)
            kTransform.m_fScale = m_kTransformValue.m_fScale;
    }
    if (!m_spData.m_pObject->m_usNumPosKeys && !m_spData.m_pObject->m_usNumRotKeys && !m_spData.m_pObject->m_usNumScaleKeys)
        m_spData = 0;
    m_kTransformValue = kTransform;
}
