
// NiFloatInterpolator constructor and binary loading for
// gamebryo2.2/corelibs/nianimation/nifloatinterpolator.cpp.
#include "nifloatinterpolator.h"
#include "../../../../include/NiFloatData.h"

// NOTE (matcher worker): the constructor below compiles byte-identically for
// its 57 bytes under /O2 /GS /EHsc /Gy except that MSVC 9 hoists the `fld`
// of the m_fFloatValue store above the argument load and pushes the vtable
// store below the matching `fstp`; no C++ spelling of the member-initializer
// list, assignment order or statement order tried here keeps MSVC 9 from
// emitting that pair in the original position (argument load, vtable store,
// fld/fstp). Its mark is intentionally left off because the canonical span of
// 0x00A3C340 (70 bytes, see task.json/covered_functions.json) also absorbs
// the seven INT3 bytes and the 6-byte GetRTTI at 0x00A3C380, which is absent
// from the function index, so no /Gy compilation can ever fill those 13
// bytes. Full evidence is in this unit's matcher checks.
NiFloatInterpolator::NiFloatInterpolator(NiFloatData* pkFloatData)
    : m_fFloatValue(-FLT_MAX), m_spFloatData(pkFloatData), m_uiLastIdx(0)
{}

// Loads the float value and a NiFloatData reference from the stream, releasing the old data and incrementing the new one's ref count.
// @fnv 0x00a3c800
// pdb: NiFloatInterpolator::LoadBinary | NiFloatInterpolator.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiFloatInterpolator::LoadBinary(NiStream& kStream)
{
    NiKeyBasedInterpolator::LoadBinary(kStream);

    ReadBinaryValue(kStream, m_fFloatValue);

    // FNV 2.2 NiPointer assignment order: release the old object before
    // retaining the new one (matches the original refcount sequence).
    NiFloatData* pkData = (NiFloatData*)kStream.DeserializeObject();
    if (m_spFloatData.m_pObject != pkData)
    {
        if (m_spFloatData.m_pObject)
            m_spFloatData.m_pObject->DecRefCount();
        m_spFloatData.m_pObject = pkData;
        if (m_spFloatData.m_pObject)
            m_spFloatData.m_pObject->IncRefCount();
    }
}

// @fnv 0x00a3c3c0
// src: COMMUNITY 0x00a3c3c0
// pdb: NiFloatInterpolator::GetKeyType | NiFloatInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
// pdb locals: uiNumKeys, ucSize, eType
int __fastcall NiFloatInterpolator_GetKeyType(void* thisPtr, int, unsigned short)
{
    void* pData = *(void**)((char*)thisPtr + 0x10);
    if (!pData)
        return 0;
    return *(int*)((char*)pData + 0x10);
}

// @fnv 0x00a3c3d0
// src: COMMUNITY 0x00a3c3d0
// pdb: NiFloatInterpolator::GetKeyStride | NiFloatInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
// pdb locals: uiNumKeys, ucSize, eType
unsigned char __fastcall NiFloatInterpolator_GetKeyStride(const void* self, int, unsigned short)
{
    const char* p = *(const char**)((const char*)self + 0x10);
    if (!p)
        return 0;
    return *(const unsigned char*)(p + 0x14);
}

struct GlobalObjectO2_00a7b610
{
    int pad;
};
extern GlobalObjectO2_00a7b610 g_011f3738;

// @fnv 0x00a3c380
// src: COMMUNITY 0x00a3c380
const GlobalObjectO2_00a7b610* __cdecl NiFloatInterpolator_GetRTTI()
{
    return &g_011f3738;
}

struct FloatDataHelper : NiFloatData
{
    void FUN_00a3cc40(float a, float b);
};
struct FloatInterpView
{
    char pad[0x10];
    FloatDataHelper* m_spFloatData;
};

// If m_spFloatData is set, forwards the two float arguments to its FUN_00a3cc40 method.
// @fnv 0x00a3c540
// src: XBOX_PDB 0x00a3c540
// pdb: NiFloatInterpolator::GuaranteeTimeRange | NiFloatInterpolator.obj | debug, upstream name already matches, high
// pdb params: fStartTime, fEndTime
void __fastcall NiFloatInterpolator_GuaranteeTimeRange(FloatInterpView* self, int edx, float m_fStartTime, float m_fMaximumDistance)
{
    if (self->m_spFloatData)
        self->m_spFloatData->FUN_00a3cc40(m_fStartTime, m_fMaximumDistance);
}

struct FUN_00a3c500_shell : NiFloatInterpolator
{
    void SetFloatValue(float value);
};

// Releases the NiFloatData pointer at offset 0x10 if set, then stores value at offset 0xc.
// @fnv 0x00a3c500
// pdb: NiFloatInterpolator::SetPoseValue | NiFloatInterpolator.obj | debug, object-file gap fill, medium
// pdb params: fPoseValue
void FUN_00a3c500_shell::SetFloatValue(float value)
{
    if (reinterpret_cast<NiPointer<NiFloatData>*>(reinterpret_cast<char*>(this) + 0x10)->m_pObject)
    {
        reinterpret_cast<NiPointer<NiFloatData>*>(reinterpret_cast<char*>(this) + 0x10)->m_pObject->DecRefCount();
        reinterpret_cast<NiPointer<NiFloatData>*>(reinterpret_cast<char*>(this) + 0x10)->m_pObject = 0;
    }
    *reinterpret_cast<float*>(reinterpret_cast<char*>(this) + 0xc) = value;
}

extern char DAT_01096f34;
extern float DAT_0109a918;

extern void(__stdcall* FUN_00fdf1c0)(int);

struct S_00a3c340_Class
{
    void* unk00;
    int unk04;
    int unk08;
    float unk0c;
    void* unk10;
    int unk14;

    void FUN_00a3ca10();
    S_00a3c340_Class* Initialize(int arg);
};

// @fnv 0x00a3c340
// pdb: NiFloatInterpolator::NiFloatInterpolator | NiFloatInterpolator.obj | retail, call graph, medium
// pdb params: pkFloatData
S_00a3c340_Class* S_00a3c340_Class::Initialize(int arg)
{
    FUN_00a3ca10();

    unk00 = &DAT_01096f34;
    unk0c = DAT_0109a918;
    unk10 = (void*)arg;

    if (arg != 0)
    {
        FUN_00fdf1c0(arg + 4);
    }

    unk14 = 0;
    return this;
}

// NiFloatInterpolator::NiFloatInterpolator(float)
class FUN_00a3c390 : public NiKeyBasedInterpolator
{
  public:
    FUN_00a3c390(float fPoseValue);
    virtual ~FUN_00a3c390();

    float m_fFloatValue;
    NiPointer<NiFloatData> m_spFloatData;
    unsigned int m_uiLastKeyIndex;
};

// Constructs a float interpolator, initialising the pose value from the argument and clearing the float data pointer and the extra field.
// @fnv 0x00a3c390
// pdb: NiFloatInterpolator::NiFloatInterpolator | NiFloatInterpolator.obj | retail, call graph, medium
// pdb params: pkFloatData
FUN_00a3c390::FUN_00a3c390(float fPoseValue) : m_fFloatValue(fPoseValue), m_spFloatData(0), m_uiLastKeyIndex(0)
{}

namespace fnv_00a3c3f0 {
extern const volatile float DAT_0109a918;

struct NiFloatInterpolatorShell_00a3c3f0 : NiFloatInterpolator
{
    unsigned char NiFloatInterpolator_GetChannelPosed(unsigned int usChannel) const;
};

// Returns 0 if m_spFloatData.m_pObject is set or m_fFloatValue equals DAT_0109a918, otherwise returns 1.
// @fnv 0x00a3c3f0
// src: GAMEBRYO_LIB 0x00a3c3f0
// pdb: NiFloatInterpolator::GetChannelPosed | NiFloatInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
unsigned char NiFloatInterpolatorShell_00a3c3f0::NiFloatInterpolator_GetChannelPosed(unsigned int usChannel) const
{
    if (m_spFloatData.m_pObject)
        return 0;
    if (m_fFloatValue == DAT_0109a918)
        return 0;
    return 1;
}
}  // namespace fnv_00a3c3f0

// Base equality operation absent from the shared key-based declaration.
struct KeyBasedInterpolatorEqualityOps : NiKeyBasedInterpolator
{
    unsigned char bhkCharacterProxy_IsEqual(const NiObject* other);
};
struct NiFloatInterpolatorEquality : NiFloatInterpolator
{
    bool NiFloatInterpolator_IsEqual(const NiObject* pkObject);
};
// @fnv 0x00a3c5e0
// src: XBOX_PDB 0x00a3c5e0
// pdb: NiFloatInterpolator::IsEqual | NiFloatInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkDest
bool NiFloatInterpolatorEquality::NiFloatInterpolator_IsEqual(const NiObject* pkObject)
{
    if (!reinterpret_cast<KeyBasedInterpolatorEqualityOps*>(this)->bhkCharacterProxy_IsEqual(pkObject))
        return false;
    const NiFloatInterpolator* other = static_cast<const NiFloatInterpolator*>(pkObject);
    if (m_fFloatValue != other->m_fFloatValue)
        return false;
    if ((m_spFloatData.m_pObject && !other->m_spFloatData.m_pObject) ||
        (!m_spFloatData.m_pObject && other->m_spFloatData.m_pObject))
        return false;
    if (m_spFloatData.m_pObject)
    {
        NiFloatData& data = *this->m_spFloatData.m_pObject;
        NiFloatData* otherData = other->m_spFloatData.m_pObject;
        if (!data.IsEqual(otherData))
            return false;
    }
    return true;
}

// Additional float-data operations absent from the shared declaration.
class FloatDataRangeOps : public NiFloatData
{
  public:
    void NiFloatData_GetSequenceData(float m_fStartTime, float m_fMaximumDistance, NiPointer<NiFloatData>& spNewData) const throw();
};

class A3C640Interpolator
{
  public:
    int unk00;
    int unk04;
    int unk08;
    float m_fFloatValue;
    FloatDataRangeOps* m_spFloatData;
    unsigned int m_uiLastKeyIndex;

    A3C640Interpolator* NiInterpolator_GetSequenceInterpolator(float fStartTime, float fEndTime) const throw();
    void SetFloatData(NiFloatData* pkFloatData) throw();

    A3C640Interpolator* NiFloatInterpolator_GetSequenceInterpolator(float fStartTime, float fEndTime) const;
};

// @fnv 0x00a3c640
// src: XBOX_PDB 0x00a3c640
// pdb: NiFloatInterpolator::GetSequenceInterpolator | NiFloatInterpolator.obj | retail, upstream name already matches, high
// pdb params: fStartTime, fEndTime
// pdb locals: pkSeqInterp, spNewFloatData
A3C640Interpolator* A3C640Interpolator::NiFloatInterpolator_GetSequenceInterpolator(float fStartTime, float fEndTime) const
{
    A3C640Interpolator* pkSeqInterp = NiInterpolator_GetSequenceInterpolator(fStartTime, fEndTime);
    if (m_spFloatData && pkSeqInterp)
    {
        NiPointer<NiFloatData> spNewFloatData;
        m_spFloatData->NiFloatData_GetSequenceData(fStartTime, fEndTime, spNewFloatData);
        pkSeqInterp->SetFloatData(spNewFloatData);
        pkSeqInterp->m_uiLastKeyIndex = 0;
    }
    return pkSeqInterp;
}

struct BA3C6D0FloatKey
{
    static float InterpolatePosition(float fTime, BA3C6D0FloatKey* pkKeys, unsigned int eType, unsigned int uiNumKeys,
                                     unsigned int& uiLastIdx, unsigned char ucSize);
};

FNV_FORCEINLINE BA3C6D0FloatKey* GetFloatKeys(const NiFloatData& data, unsigned int& uiNumKeys,
    unsigned int& eType, unsigned char& ucSize)
{
    uiNumKeys = data.m_uiNumKeys;
    eType = data.m_eType;
    ucSize = data.m_ucKeySize;
    return reinterpret_cast<BA3C6D0FloatKey*>(data.m_pkKeys);
}

static const float NI_INFINITY = 3.402823466e+38f;
static const float INVALID_FLOAT = -NI_INFINITY;

class BA3C6D0Interpolator
{
  public:
    bool TimeHasChanged(float fTime) const
    {
        return m_fLastTime != fTime;
    }
    bool EvaluateFloatAtTime(float fTime, void* pkInterpTarget, float& fValue);

    int unk00;
    int unk04;
    float m_fLastTime;
    float m_fFloatValue;
    NiFloatData* m_spFloatData;
    unsigned int m_uiLastKeyIndex;
};

// @fnv 0x00a3c6d0
// pdb: NiFloatInterpolator::Update | NiFloatInterpolator.obj | debug, object-file run extension, low
// pdb params: fTime, pkInterpTarget, fValue
// pdb locals: uiNumKeys, pkKeys, ucSize, eType
bool BA3C6D0Interpolator::EvaluateFloatAtTime(float fTime, void* pkInterpTarget, float& fValue)
{
    if (!TimeHasChanged(fTime))
    {
        fValue = m_fFloatValue;
        if (m_fFloatValue == INVALID_FLOAT)
            return false;
        return true;
    }

    if (m_spFloatData)
    {
        unsigned int uiNumKeys;
        unsigned int eType;
        unsigned char ucSize;
        BA3C6D0FloatKey* pkKeys = GetFloatKeys(*m_spFloatData, uiNumKeys, eType, ucSize);
        if (uiNumKeys > 0)
            m_fFloatValue =
                BA3C6D0FloatKey::InterpolatePosition(fTime, pkKeys, eType, uiNumKeys, m_uiLastKeyIndex, ucSize);
    }

    if (m_fFloatValue == INVALID_FLOAT)
        return false;

    fValue = m_fFloatValue;
    m_fLastTime = fTime;
    return true;
}

struct FUN_00a3c780_cls : NiFloatInterpolator
{
    void CopyBase(NiFloatInterpolator* dst, unsigned int uiArg);
    void NiFloatInterpolator_CopyMembers(NiFloatInterpolator* pkDest, unsigned int kCloning);
};

// @fnv 0x00a3c780
// src: GAMEBRYO_LIB 0x00a3c780
// pdb: NiFloatInterpolator::CopyMembers | NiFloatInterpolator.obj | debug, upstream name already matches, high
// pdb params: pkDest, kCloning
void FUN_00a3c780_cls::NiFloatInterpolator_CopyMembers(NiFloatInterpolator* pkDest, unsigned int kCloning)
{
    CopyBase(pkDest, kCloning);
    *(volatile float*)&pkDest->m_fFloatValue = m_fFloatValue;
    NiFloatData* pOld = *(NiFloatData* volatile*)&pkDest->m_spFloatData.m_pObject;
    if (pOld != m_spFloatData.m_pObject)
    {
        if (pOld)
        {
            pOld->DecRefCount();
        }
        NiFloatData* pNew = m_spFloatData.m_pObject;
        pkDest->m_spFloatData.m_pObject = pNew;
        if (pNew)
        {
            pNew->IncRefCount();
        }
    }
    pkDest->m_uiLastIdx = m_uiLastIdx;
}

// GetViewerStrings: base strings, then the class name, m_fFloatValue and m_spFloatData.

class C940StringArray
{
  public:
    virtual ~C940StringArray();
    void SetSize(unsigned int uiMaxSize);
    void SetAt(unsigned int uiIndex, char* const& pcElement);

    unsigned int SetAtGrow(unsigned int uiIndex, char* const& pcElement)
    {
        if (uiIndex >= m_usMaxSize)
            SetSize(uiIndex + m_usGrowBy);
        SetAt(uiIndex, pcElement);
        return uiIndex;
    }
    unsigned int Add(char* const& pcElement)
    {
        return SetAtGrow(m_usSize, pcElement);
    }

    char** m_pBase;
    unsigned short m_usMaxSize;
    unsigned short m_usSize;
    unsigned short m_usESize;
    unsigned short m_usGrowBy;
};

template <class T> class C940Pointer
{
  public:
    operator T*() const
    {
        return m_pObject;
    }

    T* m_pObject;
};


extern const char* g_pcC940ClassName;

char* __cdecl C940GetViewerString(const char* pcName);
char* __cdecl C940GetViewerString(const char* pcPrefix, float fValue);
char* __cdecl C940GetViewerString(const char* pcPrefix, void* pvValue);

class C940FloatInterpolator
{
  public:
    int unk00;
    int unk04;
    int unk08;
    float m_fFloatValue;
    C940Pointer<NiFloatData> m_spFloatData;

    void BaseGetViewerStrings(C940StringArray* pkStrings);
    void NiFloatInterpolator_GetViewerStrings(C940StringArray* pkStrings);
};

// Appends the base viewer strings plus the class name, m_fFloatValue and m_spFloatData entries to pkStrings.
// @fnv 0x00a3c940
// src: XBOX_PDB 0x00a3c940
// pdb: NiFloatInterpolator::GetViewerStrings | NiFloatInterpolator.obj | retail, string-set bridge, high
// pdb params: pkStrings
void C940FloatInterpolator::NiFloatInterpolator_GetViewerStrings(C940StringArray* pkStrings)
{
    BaseGetViewerStrings(pkStrings);
    pkStrings->Add(C940GetViewerString(g_pcC940ClassName));
    pkStrings->Add(C940GetViewerString("m_fFloatValue", m_fFloatValue));
    pkStrings->Add(C940GetViewerString("m_spFloatData", m_spFloatData));
}

template <class T> inline void WriteBinaryArray(NiBinaryStream& os, const T* pValue, unsigned int uiNumEls)
{
    unsigned int uiSize = sizeof(T);
    os.BinaryWrite(pValue, sizeof(T) * uiNumEls, &uiSize, 1);
}

template <class T> inline void SerializeValue(NiStream& kStream, const T& value)
{
    WriteBinaryArray(*kStream.m_pkOstr, &value, 1);
}

class NiFloatInterpolatorW : public NiFloatInterpolator
{
  public:
    void SaveToStream(NiStream& kStream);
};

// @fnv 0x00a3c880
// pdb: NiFloatInterpolator::SaveBinary | NiFloatInterpolator.obj | retail, object-file gap fill, high
// pdb params: kStream
void NiFloatInterpolatorW::SaveToStream(NiStream& kStream)
{
    NiObject::SaveBinary(kStream);
    SerializeValue(kStream, m_fFloatValue);
    kStream.SaveLinkID(m_spFloatData.m_pObject);
}

class TimedAnimationKey
{
  public:
    float m_fTime;
    float m_fValue;

    FNV_FORCEINLINE float GetValue() const
    {
        return m_fValue;
    }
    FNV_FORCEINLINE TimedAnimationKey* GetKeyPointerByIndex(unsigned int uiIndex, unsigned char ucKeySize)
    {
        return (TimedAnimationKey*)((char*)this + uiIndex * ucKeySize);
    }
};

FNV_FORCEINLINE TimedAnimationKey* GetFloatKeys(const NiFloatData& data, unsigned int& uiNumKeys,
    NiAnimationKey::KeyType& eType, unsigned char& ucSize)
{
    uiNumKeys = data.m_uiNumKeys;
    eType = data.m_eType;
    ucSize = data.m_ucKeySize;
    return data.m_pkKeys;
}

template <class T> FNV_FORCEINLINE void NiPointerAssign(NiPointer<T>& sp, T* pObject)
{
    if (sp.m_pObject != pObject)
    {
        if (pObject != 0)
            pObject->IncRefCount();
        if (sp.m_pObject)
            sp.m_pObject->DecRefCount();
        sp.m_pObject = pObject;
    }
}

extern float DAT_0109a918;

class FUN_00a3c420_cls : public NiFloatInterpolator
{
  public:
    void CollapseConstantKeys();
};

// When the key data is empty or collapsible, clears m_spFloatData and stores a float value in m_fFloatValue.
// @fnv 0x00a3c420
// pdb: NiFloatInterpolator::Collapse | NiFloatInterpolator.obj | debug, object-file gap fill, medium
// pdb locals: uiNumKeys, pkKeys, ucSize, eType, bCollapse, fValue, ui
void FUN_00a3c420_cls::CollapseConstantKeys()
{
    if (m_spFloatData.m_pObject)
    {
        unsigned int uiNumKeys;
        NiAnimationKey::KeyType eType;
        unsigned char ucSize;
        TimedAnimationKey* pkKeys =
            GetFloatKeys(*m_spFloatData.m_pObject, uiNumKeys, eType, ucSize);
        if (uiNumKeys == 0)
        {
            NiPointerAssign<NiFloatData>(m_spFloatData, 0);
            m_fFloatValue = DAT_0109a918;
            return;
        }

        float fValue = pkKeys->GetKeyPointerByIndex(0, ucSize)->GetValue();
        if (uiNumKeys != 1)
        {
            if (eType != NiAnimationKey::LINKEY && eType != NiAnimationKey::STEPKEY)
                return;
            bool bPosed = true;
            for (unsigned int ui = 1; bPosed && ui < uiNumKeys; ui++)
            {
                if (pkKeys->GetKeyPointerByIndex(ui, ucSize)->GetValue() != fValue)
                    bPosed = false;
            }
            if (!bPosed)
                return;
        }
        NiPointerAssign<NiFloatData>(m_spFloatData, 0);
        m_fFloatValue = fValue;
    }
}

void* __cdecl Object_DispatchCommandFour(unsigned int size);
extern float DAT_0109a918;
class A3C8D0_Object : public NiKeyBasedInterpolator
{
  public:
    float m_0C;
    int m_10;
    NiPointer<NiObject> m_14;

    static void* operator new(size_t size)
    {
        return Object_DispatchCommandFour(size);
    }
    A3C8D0_Object() : m_0C(DAT_0109a918), m_10(0)
    {}
    virtual void vf00();
    virtual unsigned short GetKeyChannelCount() const;
    virtual unsigned int GetColorKeyCount(unsigned short) const;
    virtual void vf57();
    virtual void vf58();
    virtual NiAnimationKey* GetChannelKeys(unsigned short) const;
    virtual unsigned char GetColorKeySize(unsigned short) const;
    virtual void vf61(unsigned short);
};
struct A3C8D0_Owner
{
    void NiFloatInterpolator_CopyMembers(A3C8D0_Object* pkDest, int kCloning);
    A3C8D0_Object* CreateObject(int kCloning);
};
// Allocates an A3C8D0_Object, calls FUN_00a3c780 with the object and arg, and returns the object.
// @fnv 0x00a3c8d0
// pdb: NiFloatInterpolator::CreateClone | NiFloatInterpolator.obj | retail, object-file gap fill, high
// pdb params: kCloning
// pdb locals: pkObject
A3C8D0_Object* A3C8D0_Owner::CreateObject(int kCloning)
{
    A3C8D0_Object* object = new A3C8D0_Object;
    NiFloatInterpolator_CopyMembers(object, kCloning);
    return object;
}
