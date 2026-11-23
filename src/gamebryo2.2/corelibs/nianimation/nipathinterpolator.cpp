#include "../../../../include/NiPathInterpolator.h"
#include "../../../../include/NiKeyBasedInterpolator.h"
#include "../../../../include/NiFloatData.h"
#include "../../../../include/NiObject.h"
#include "../../../../include/NiPosData.h"
#include "../../../../include/NiRefObject.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiQuaternion.h"
#include "../../../../include/NiMatrix3.h"
#include "../../../../include/NiPointer.h"
#include "../../../../include/NiQuatTransform.h"
#include "../../../../include/StrArr.h"

// src/gamebryo2.2/corelibs/nianimation/nipathinterpolator.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nianimation/nipathinterpolator.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern "C" void* g_011f3f68;

// @fnv 0x00a3dab0
// src: COMMUNITY 0x00a3dab0
void* __cdecl NiPosData_GetRTTI()
{
    return &g_011f3f68;
}

// Returns the pointer stored at offset 8 of the object at this+0x18 for idx 0 or this+0x1c for idx 1, or null if absent.
// @fnv 0x00a3dae0
// src: GAMEBRYO_LIB 0x00a3dae0
// pdb: NiPathInterpolator::GetKeyCount | NiPathInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
void* __thiscall NiPathInterpolator::GetKeyCount(short usChannel)
{
    if (usChannel == 0)
    {
        void* p = m_spPathData;
        if (p)
            return *(void**)((char*)p + 8);
    }
    else if (usChannel == 1)
    {
        void* p = m_spPctData;
        if (p)
            return *(void**)((char*)p + 8);
    }
    return 0;
}

// Returns the int at offset 0x10 of the pointer stored at this+0x18 for arg 0 or this+0x1c for arg 1, or 0 if that pointer is null or arg is other.
// @fnv 0x00a3db10
int __thiscall NiPathInterpolator::GetValueByIndex(short aiIndex)
{
    if (aiIndex == 0)
    {
        int* p = (int*)m_spPathData;
        if (p)
            return *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(p) + 0x10);
    }
    else if (aiIndex == 1)
    {
        int* p = (int*)m_spPctData;
        if (p)
            return *reinterpret_cast<int*>(reinterpret_cast<unsigned char*>(p) + 0x10);
    }
    return 0;
}

// Returns the int at offset 0xC of the object stored at this+0x18 when arg is 0, or at this+0x1C when arg is 1; otherwise returns 0.
// @fnv 0x00a3db40
// src: GAMEBRYO_LIB 0x00a3db40
// pdb: NiPathInterpolator::GetKeyArray | NiPathInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
// pdb locals: uiNumKeys, pkKeys, ucSize, eType
int __thiscall NiPathInterpolator::GetKeyArray(short usChannel)
{
    if (usChannel == 0)
    {
        int* p = (int*)m_spPathData;
        if (p)
            return *(p + 3);
    }
    else if (usChannel == 1)
    {
        int* p = (int*)m_spPctData;
        if (p)
            return *(p + 3);
    }
    return 0;
}

// Returns byte at offset 0x14 of the object stored at this+0x18 for arg 0 or this+0x1c for arg 1, otherwise 0.
// @fnv 0x00a3db70
// src: GAMEBRYO_LIB 0x00a3db70
// pdb: NiPathInterpolator::GetKeyStride | NiPathInterpolator.obj | debug, upstream name already matches, high
// pdb params: usChannel
// pdb locals: uiNumKeys, ucSize, eType
char __thiscall NiPathInterpolator::GetKeyStride(unsigned short usChannel)
{
    if (usChannel == 0)
    {
        void* p = m_spPathData;
        if (p)
            return *reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned char*>(p) + 0x14);
    }
    else if (usChannel == 1)
    {
        void* p = m_spPctData;
        if (p)
            return *reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned char*>(p) + 0x14);
    }
    return 0;
}

extern "C" void* g_011f3e48;

// @fnv 0x00a3dde0
const NiRTTI* NiPathInterpolator::GetRTTI() const
{
    return (const NiRTTI*)&g_011f3e48;
}

// NiFloatData.h lacks the time-range method; this bridge declares only that call.
struct FloatDataHelper
{
    void __thiscall FUN_00a3cc40(float m_fStartTime, float m_fMaximumDistance);
};

// Forwards the two float arguments to the FloatDataHelper stored at offset 0x1c, if that pointer is non-null.
// @fnv 0x00a3dc50
// src: XBOX_PDB 0x00a3dc50
void __thiscall NiPathInterpolator::GuaranteeTimeRange_00a3dc50(float m_fStartTime, float m_fMaximumDistance)
{
    FloatDataHelper* p = (FloatDataHelper*)m_spPctData;
    if (p)
    {
        p->FUN_00a3cc40(m_fStartTime, m_fMaximumDistance);
    }
}

// Maps channel 0 to 1, channel 1 to 0, and any other channel to 6.
// @fnv 0x00a3da80
// pdb: NiAnimationKey::Copy | NiAnimationKey.obj | debug, object-file run extension, low
// pdb params: __formal, __formal
int NiPathInterpolator::GetChannelIndex(unsigned short usChannel)
{
    if (usChannel == 0)
        return 1;
    return usChannel == 1 ? 0 : 6;
}

extern "C" void __cdecl FUN_00a48fb0();
extern "C" void __cdecl _delete(void*, unsigned int);

struct FUN_00a3dac0_shell
{
    void* __thiscall DestroyIfOdd(short idx);
};

// Calls FUN_00a48fb0, then deletes this with size 24 when idx has bit 0 set, and returns this.
// @fnv 0x00a3dac0
void* __thiscall FUN_00a3dac0_shell::DestroyIfOdd(short idx)
{
    FUN_00a48fb0();
    if (idx & 1)
        _delete(this, 24);
    return this;
}

extern void* DAT_011f3ca8[];
// Reads a type index, element size and base pointer from this+0x18, then calls the DAT_011f3ca8 entry for that type with two element pointers and pData.
// @fnv 0x00a3ddf0
// src: GAMEBRYO_LIB 0x00a3ddf0
// pdb: NiPathInterpolator::GenPathInterp | NiPathInterpolator.obj | retail, upstream name already matches, high
// pdb params: iI1, iI2, fNormTime
// pdb locals: kResult, uiNumPathKeys, pkPathKeys, pfnInterp, ePathType, ucSize
void* NiPathInterpolator::GenPathInterp(void* pData, int aiIndexA, int aiIndexB, float fTime)
{
    char* pcElementBase = 0;
    int iType;
    unsigned char ucSize;
    void* pkObj = m_spPathData;
    if (pkObj)
    {
        iType = *(int*)((char*)pkObj + 0x10);
        ucSize = *(unsigned char*)((char*)pkObj + 0x14);
        pcElementBase = *(char**)((char*)pkObj + 0xC);
    }
    else
    {
        iType = 0;
        ucSize = 0;
    }
    ((void(__cdecl*)(float, void const*, void const*, void*))DAT_011f3ca8[iType])(
        fTime, pcElementBase + ucSize * aiIndexA, pcElementBase + ucSize * aiIndexB, pData);
    return pData;
}

// @fnv 0x00a3e980 is the compiler-generated scalar deleting destructor.
// pdb: NiPathInterpolator::`scalar deleting destructor' | NiPathInterpolator.obj | retail, object-file gap fill, high

struct FUN_00a3cbf0_shell
{
    void __thiscall FUN_00a3cbf0(StrArr*);
};
char* __cdecl _FUN_00a7d330(const char*);
extern const char* _g_011f3e48;

// Appends the string returned by _FUN_00a7d330(_g_011f3e48) to pkStrings, growing the array if the size limit is reached.
// @fnv 0x00a3f4d0
void __thiscall NiPathInterpolator::AppendString(StrArr* pkStrings)
{
    ((FUN_00a3cbf0_shell*)this)->FUN_00a3cbf0(pkStrings);
    char* pcString = _FUN_00a7d330(_g_011f3e48);
    unsigned int uiIndex = pkStrings->m_usSize;
    if (uiIndex >= pkStrings->m_usMaxSize)
        pkStrings->SetSize(uiIndex + pkStrings->m_usGrowBy);
    pkStrings->SetAt(uiIndex, pcString);
}

class CloningProcess
{
  public:
    void* m_pCloneMap;
};

// Calls ProcessClone on this object and on the path and percentage objects if they are set.
// @fnv 0x00a3dc80
// src: XBOX_PDB 0x00a3dc80
// pdb: NiPathInterpolator::SetPathData | NiPathInterpolator.obj | debug, object-file run extension, low
// pdb params: pPathData
void __thiscall NiPathInterpolator::ProcessClone_00a3dc80(CloningProcess& kCloning)
{
    ((NiObject*)this)->NiObject::ProcessClone(kCloning);
    NiObject* pkPath = m_spPathData;
    if (pkPath)
        pkPath->ProcessClone(kCloning);
    NiObject* pkPct = (NiObject*)m_spPctData;
    if (pkPct)
        pkPct->ProcessClone(kCloning);
}

struct HC1Base
{
    bool __thiscall BSPackedAdditionalGeometryData_RegisterStreamables(void*);
};

// @fnv 0x00a3dcc0
// src: XBOX_PDB 0x00a3dcc0
// pdb: NiPathInterpolator::SetPctData | NiPathInterpolator.obj | debug, object-file run extension, low
// pdb params: pPctData
bool NiPathInterpolator::RegisterStreamables_00a3dcc0(void* pkStream)
{
    if (!((HC1Base*)this)->BSPackedAdditionalGeometryData_RegisterStreamables(pkStream))
        return false;
    NiObject* pPath = m_spPathData;
    if (pPath)
        pPath->RegisterStreamables(*(NiStream*)pkStream);
    NiObject* pPct = (NiObject*)m_spPctData;
    if (pPct)
        pPct->RegisterStreamables(*(NiStream*)pkStream);
    return true;
}

inline void* operator new(size_t, void* p) { return p; }

// @fnv 0x00a3f4b0
void* __cdecl CreateA51BF0Base()
{
    void* p = NiObject::operator new(0x5c);
    if (p)
        return ::new (p) NiPathInterpolator(0, 0);
    return 0;
}

extern float DAT_01097170;
extern float DAT_01015f5c;
extern double DAT_010231b0;
extern double DAT_010241b0;

// NiKeyBasedInterpolator's key-access overloads are not shared yet.
struct KeyDataAccess
{
    float* FUN_00a3ca80(int index, int mode);
    float* FUN_00a3ca80(int index);
};

// @fnv 0x00a3dba0
// pdb: NiPathInterpolator::ReplacePathKeys | NiPathInterpolator.obj | debug, object-file run extension, low
// pdb params: pkKeys, uiNumKeys, eType
// pdb locals: pfnCF, ucSize
void __thiscall NiPathInterpolator::GetRange(float* pfMin, float* pfMax)
{
    *pfMin = DAT_01097170;
    *pfMax = DAT_01015f5c;

    unsigned int count = ((unsigned int (__thiscall*)(NiPathInterpolator*, unsigned int))(*(void***)this)[56])(this, 1);

    if (count > 0)
    {
        float* first = ((KeyDataAccess*)this)->FUN_00a3ca80(0, 1);
        unsigned int index = ((unsigned int (__thiscall*)(NiPathInterpolator*, unsigned int, unsigned int))(*(void***)this)[56])(this, 1, 1) - 1;
        float* last = ((KeyDataAccess*)this)->FUN_00a3ca80(index);

        if (first != 0 && last != 0)
        {
            if (*first < *pfMin)
                *pfMin = *first;

            if (*last > *pfMax)
                *pfMax = *last;
        }
    }

    if ((double)*pfMin == DAT_010231b0 && (double)*pfMax == DAT_010241b0)
    {
        *pfMin = 0.0f;
        *pfMax = 0.0f;
    }
}

__forceinline NiBinaryStream* NiStream::GetBinaryStream()
{
    return *(NiBinaryStream**)((char*)this + 0x24c);
}
__forceinline WriteStream* NiStream::GetWriter()
{
    return *(WriteStream**)((char*)this + 0x250);
}

struct Stream
{
    unsigned int flags, bytesRead;
    int(__cdecl* readProc)(Stream*, void*, unsigned int, unsigned int*, int);
    __forceinline int Read(void* dst, unsigned int count)
    {
        unsigned int actual = count;
        return readProc(this, dst, count, &actual, 1);
    }
};
void __stdcall NiKeyBasedInterpolator_LoadBinary(NiStream*);

// Reads channel, value38, value28, value2c and channel30 from the source's stream, accumulating bytesRead, then calls FUN_00a645b0 twice.
// @fnv 0x00a3ed90
// pdb: NiPathInterpolator::LoadBinary | NiPathInterpolator.obj | retail, object-file run extension, low
// pdb params: kStream
void NiPathInterpolator::ReadFromStream(NiStream* source)
{
    NiKeyBasedInterpolator_LoadBinary(source);
    Stream* stream = (Stream*)source->GetBinaryStream();
    stream->bytesRead += stream->Read(&m_uFlags, 2);
    unsigned int value;
    stream = (Stream*)source->GetBinaryStream();
    stream->bytesRead += stream->Read(&value, 4);
    m_eBankDir = value;
    stream = (Stream*)source->GetBinaryStream();
    stream->bytesRead += stream->Read(&m_fMaxBankAngle, 4);
    stream = (Stream*)source->GetBinaryStream();
    stream->bytesRead += stream->Read(&m_fSmoothing, 4);
    stream = (Stream*)source->GetBinaryStream();
    stream->bytesRead += stream->Read(&m_sFollowAxis, 2);
    source->ResetStreamState();
    source->ResetStreamState();
}

struct FNV_WriteStream;
typedef int(__cdecl* FNV_WriteFn)(FNV_WriteStream* stream, void* data, int size, int* written, int flag);

struct FNV_WriteStream
{
    int unk00;
    int m_uiItemCount;
    void* unk08;
    FNV_WriteFn m_pfn;
};

struct StreamBase { void NiKeyBasedInterpolator_LinkObject(NiStream*); void NiKeyBasedInterpolator_LinkObject(NiStream&); };

// The single write the record layout performs: hand the payload and its size
// to the stream's callback and accumulate the byte count it returns.
static __forceinline void FNV_WriteField(FNV_WriteStream* pWriter, void* pData, int nSize)
{
    int n = nSize;
    int r = pWriter->m_pfn(pWriter, pData, n, &n, 1);
    pWriter->m_uiItemCount += r;
}

// Writes unk0c, unk38, unk28, unk2c and unk30 to the stream's writer, then calls f12 with unk18 and unk1c after FUN_00a95ca0.
// @fnv 0x00a3ef70
// pdb: NiPathInterpolator::SaveBinary | NiPathInterpolator.obj | retail, object-file run extension, low
// pdb params: kStream
void NiPathInterpolator::WriteToStream(NiStream* pStream)
{
    ((StreamBase*)this)->NiKeyBasedInterpolator_LinkObject(pStream);
    FNV_WriteField((FNV_WriteStream*)pStream->GetWriter(), &m_uFlags, 2);
    FNV_WriteField((FNV_WriteStream*)pStream->GetWriter(), &m_eBankDir, 4);
    FNV_WriteField((FNV_WriteStream*)pStream->GetWriter(), &m_fMaxBankAngle, 4);
    FNV_WriteField((FNV_WriteStream*)pStream->GetWriter(), &m_fSmoothing, 4);
    FNV_WriteField((FNV_WriteStream*)pStream->GetWriter(), &m_sFollowAxis, 2);
    pStream->SaveLinkID(reinterpret_cast<const NiObject*>(m_spPathData));
    pStream->SaveLinkID(reinterpret_cast<const NiObject*>(m_spPctData));
}

__declspec(dllimport) long __stdcall InterlockedIncrement(long* p);
__declspec(dllimport) long __stdcall InterlockedDecrement(long* p);
extern "C" void* __cdecl memcpy(void*, const void*, size_t);
void* __cdecl NiMalloc(unsigned int);

__forceinline void NiRefObject::IncRefCount()
{
    InterlockedIncrement((long*)&m_uiRefCount);
}
__forceinline void NiRefObject::DecRefCount()
{
    if (InterlockedDecrement((long*)&m_uiRefCount) == 0)
        DeleteThis();
}

template <class T> __forceinline NiPointer<T>::operator T*() const
{
    return m_pObject;
}
template <class T> __forceinline T* NiPointer<T>::operator->() const
{
    return m_pObject;
}
template <class T> __forceinline NiPointer<T>& NiPointer<T>::operator=(T* pObject)
{
    if (m_pObject != pObject)
    {
        if (m_pObject != 0)
            m_pObject->DecRefCount();
        m_pObject = pObject;
        if (m_pObject != 0)
            m_pObject->IncRefCount();
    }
    return *this;
}

struct NiInterpolatorBase
{
    void CopyMemberData(NiInterpolatorBase* pDest, CloningProcess& kCloning);
};

// Preserve the ownership operations of the pointer fields represented as raw pointers in the shared layout.
template <class T> static __forceinline NiPointer<NiPosData>& NiPathPointer(T*& value)
{
    return (NiPointer<NiPosData>&)value;
}
template <class T> static __forceinline void IncrementPathPointer(T* value) throw()
{
    if (value)
        value->IncRefCount();
}
template <class T, class U> static __forceinline void AssignPathPointer(T*& slot, U* value)
{
    if (slot != value)
    {
        if (slot) ((NiObject*)slot)->DecRefCount();
        slot = (T*)value;
        if (slot) ((NiObject*)slot)->IncRefCount();
    }
}
inline void NiPathInterpolator::SetBit(bool value, unsigned short mask)
{
    if (value) m_uFlags |= mask;
    else m_uFlags &= ~mask;
}
inline bool NiPathInterpolator::GetBit(unsigned short mask) const { return (m_uFlags & mask) != 0; }
inline unsigned short NiPathInterpolator::GetField(unsigned short mask, unsigned short pos) const { return (m_uFlags & mask) >> pos; }
inline bool NiPathInterpolator::GetConstVelocity() const { return (GetField(0x10, 4) & 1) != 0; }
inline bool NiPathInterpolator::GetCVDataNeedsUpdate() const { return GetBit(1); }
inline void NiPathInterpolator::SetCVDataNeedsUpdate(bool value) { SetBit(value, 1); }
inline bool NiPathInterpolator::GetFollow() const { return (bool)GetField(0x20, 5); }
inline bool NiPathInterpolator::TimeHasChanged(float time) const { return m_fLastTime != time; }
inline void NiPathInterpolator::SetConstVelocity(bool b)
{
    SetBit(b, 0x10);
    if (b && GetBit(1)) { m_fTotalLength = CalcTotalLength(); SetBit(false, 1); }
}
inline PositionKeyInterpolator* NiPathInterpolator::GetPathKeys(unsigned int& count, unsigned int& type, unsigned char& stride) const
{
    NiPosData* data = m_spPathData;
    if (data) { count = data->m_uiNumKeys; type = data->m_eType; stride = data->m_ucKeySize; return (PositionKeyInterpolator*)data->m_pkKeys; }
    count = 0; type = 0; stride = 0; return 0;
}

// @fnv 0x00a3f2f0
// src: XBOX_PDB 0x00a3f2f0
// pdb: NiPathInterpolator::CopyMembers | NiPathInterpolator.obj | retail, object-file run extension, low
// pdb params: pkDest, kCloning
// pdb locals: uiNumPathKeys, ePathType, ucSize, uiDestSize
void NiPathInterpolator::CopyMembers_00a3f2f0(NiPathInterpolator* pDest, CloningProcess& kCloning)
{
    ((NiInterpolatorBase*)this)->CopyMemberData((NiInterpolatorBase*)pDest, kCloning);
    pDest->m_uFlags = m_uFlags;
    if (NiPathPointer(m_spPathData))
        NiPathPointer(pDest->m_spPathData) = (NiPosData*)NiPathPointer(m_spPathData)->MakeClone(kCloning);
    if (NiPathPointer(m_spPctData))
        NiPathPointer(pDest->m_spPctData) = (NiPosData*)NiPathPointer(m_spPctData)->MakeClone(kCloning);
    pDest->SetBit(GetBit(4), 4);
    pDest->SetBit(GetBit(8), 8);
    pDest->m_eBankDir = m_eBankDir;
    pDest->SetConstVelocity(GetBit(0x10));
    pDest->SetBit(GetBit(0x20), 0x20);
    pDest->m_fMaxBankAngle = m_fMaxBankAngle;
    pDest->m_fSmoothing = m_fSmoothing;
    pDest->m_sFollowAxis = m_sFollowAxis;
    pDest->SetBit(GetBit(0x40), 0x40);
    pDest->SetBit(GetBit(2), 2);
    if (m_pfPartialLength)
    {
        unsigned int uiNumKeys = m_spPathData ? m_spPathData->m_uiNumKeys : 0;
        unsigned int uiSize = uiNumKeys * sizeof(float);
        pDest->m_pfPartialLength = (float*)NiMalloc(uiSize);
        memcpy(pDest->m_pfPartialLength, m_pfPartialLength, uiSize);
    }
    pDest->m_fTotalLength = m_fTotalLength;
}

// @fnv 0x00a3f520
NiObject* NiPathInterpolator::MakeClone(CloningProcess& kCloning)
{
    void* memory = NiObject::operator new(sizeof(NiPathInterpolator));
    NiPathInterpolator* pkClone = memory ? ::new (memory) NiPathInterpolator(0, 0) : 0;
    CopyMemberData(pkClone, kCloning);
    return pkClone;
}

struct MyPosKey
{
    float m_fTime;
};

static __forceinline MyPosKey* GetKey(MyPosKey* pkKeys, unsigned int uiIndex, unsigned char ucKeySize)
{
    return (MyPosKey*)((const char*)pkKeys + uiIndex * ucKeySize);
}

// @fnv 0x00a3ec30
// pdb: NiPathInterpolator::GetLoc | NiPathInterpolator.obj | retail, object-file run extension, low
// pdb params: fTime, uiI1, uiI2, fNormTime
// pdb locals: uiNumPathKeys, pkPathKeys, bFound, bFirstLoop, ui2, ePathType, ucSize, ui1
void NiPathInterpolator::FindKeyPair(float fTime, unsigned int& ruiKey0, unsigned int& ruiKey1, float& rfNormTime)
{
    if (GetBit(0x10))
    {
        GetPercentLoc(fTime, ruiKey0, ruiKey1, rfNormTime);
        unk10 = ruiKey0;
        return;
    }

    NiPosData* pkPathData = (NiPosData*)m_spPathData;
    unsigned int uiNumKeys;
    unsigned char ucKeySize;
    MyPosKey* pkKeys = 0;
    if (pkPathData)
    {
        uiNumKeys = pkPathData->m_uiNumKeys;
        ucKeySize = pkPathData->m_ucKeySize;
        pkKeys = (MyPosKey*)pkPathData->m_pkKeys;
    }
    else
    {
        uiNumKeys = 0;
        ucKeySize = 0;
    }

    if (fTime <= pkKeys->m_fTime)
    {
        ruiKey0 = 0;
        ruiKey1 = 1;
        rfNormTime = 0.0f;
        return;
    }

    MyPosKey* pkLastKey = GetKey(pkKeys, uiNumKeys - 1, ucKeySize);
    if (fTime >= pkLastKey->m_fTime)
    {
        ruiKey0 = uiNumKeys - 2;
        ruiKey1 = uiNumKeys - 1;
        rfNormTime = 1.0f;
        return;
    }

    unsigned int uiKey = unk10;
    unsigned int uiNextKey = 0;
    bool m_bIsBest = true;
    while (uiKey < uiNumKeys - 1)
    {
        if (fTime == GetKey(pkKeys, uiKey, ucKeySize)->m_fTime)
        {
            unk10 = uiKey;
            uiNextKey = uiKey + 1;
            break;
        }

        MyPosKey* pkKey = GetKey(pkKeys, uiKey, ucKeySize);
        if (pkKey->m_fTime < fTime && GetKey(pkKeys, uiKey + 1, ucKeySize)->m_fTime > fTime)
        {
            unk10 = uiKey;
            uiNextKey = uiKey + 1;
            break;
        }

        if (uiKey == uiNumKeys - 2 && m_bIsBest)
        {
            uiKey = 0;
            m_bIsBest = false;
        }
        else
        {
            uiKey++;
        }
    }

    MyPosKey* pkKey0 = GetKey(pkKeys, uiKey, ucKeySize);
    MyPosKey* pkKey1 = GetKey(pkKeys, uiNextKey, ucKeySize);
    rfNormTime = (fTime - pkKey0->m_fTime) / (pkKey1->m_fTime - pkKey0->m_fTime);
    ruiKey0 = uiKey;
    ruiKey1 = uiNextKey;
}

static const float kNegMax = -3.402823466e+38f;
extern const NiPoint3 DAT_011a9424;
extern NiQuaternion DAT_011f4208;

// @fnv 0x00a3f060
// pdb: NiPathInterpolator::NiPathInterpolator | NiPathInterpolator.obj | retail, upstream name already matches, high
// pdb params: pkPathData, pkPctData
__declspec(noinline) NiPathInterpolator::NiPathInterpolator(NiPosData* pkPathData, NiFloatData* pkPctData)
    : m_spPathData(pkPathData),
      m_spPctData((IncrementPathPointer(m_spPathData), pkPctData)),
      m_kRotation((IncrementPathPointer((NiFloatData*)m_spPctData), DAT_011f4208)),
      m_kPosition()
{
    memcpy(&m_kPosition, &DAT_011a9424, sizeof(m_kPosition));
    m_fMaxBankAngle = 0.0f;
    m_fSmoothing = 0.0f;
    m_fPathLength = 0.0f;
    m_fTotalLength = -1.0f;
    m_fLast = kNegMax;
    m_uiPathKeyCount = 0;
    m_uiPercentKeyCount = 0;
    m_eBankDir = 1;
    m_sFollowAxis = 0;
    m_pfPartialLength = 0;
    m_uFlags = 3;
}

#include <math.h>
extern double DAT_01032980;

// Normalizes fTime to [0,1], locates the path key segment containing the target distance, and refines the local parameter via Newton iteration.
// @fnv 0x00a3ea60
// src: GAMEBRYO_LIB 0x00a3ea60
// pdb: NiPathInterpolator::GetPercentLoc | NiPathInterpolator.obj | retail, upstream name already matches, high
// pdb params: fPercent, uiI1, uiI2, fNormTime
// pdb locals: uiNumPathKeys, iMaxIterations, fDist, fTime, ui0, ePathType, ucSize, fLength, ui1, i, fDiff, fTolerance
void NiPathInterpolator::GetPercentLoc(float fPercent, unsigned int& uiI1, unsigned int& uiI2, float& fNormTime)
{
    unsigned int uiNumKeys = m_spPathData ? m_spPathData->m_uiNumKeys : 0;
    if (fPercent == 0.0f)
    {
        uiI1 = 0;
        uiI2 = 1;
        fNormTime = 0.0f;
        return;
    }
    if (fPercent < 0.0f)
        fPercent = (float)fmod(fPercent, 1.0f) + 1.0f;
    if (fPercent == 1.0f)
    {
        uiI1 = uiNumKeys - 2;
        uiI2 = uiNumKeys - 1;
        fNormTime = 1.0f;
        return;
    }
    if (fPercent > 1.0f)
        fPercent = (float)fmod(fPercent, 1.0f);

    float fTargetDistance = fPercent * m_fTotalLength;
    float fAccumulatedDistance = 0.0f;
    float fLocalTime = 0.0f;
    unsigned int i1 = 0, i2 = 1;
    for (; i1 < uiNumKeys - 1; i1++, i2++)
    {
        if (fTargetDistance <= m_pfPartialLength[i2])
        {
            fAccumulatedDistance = fTargetDistance - m_pfPartialLength[i1];
            fLocalTime = fAccumulatedDistance / (m_pfPartialLength[i2] - m_pfPartialLength[i1]);
            break;
        }
    }
    for (int it = 0; it < 32; it++)
    {
        float fError = NiPathInterpolator_PathLength(i1, i2, fLocalTime) - fAccumulatedDistance;
        if ((float)fabs(fError) <= DAT_01032980)
            break;
        fLocalTime = (float)(fLocalTime - fError / PathSpeed(i1, i2, fLocalTime));
    }
    uiI1 = i1;
    uiI2 = i2;
    fNormTime = fLocalTime;
}

struct AnimInfo
{
    unsigned int count;
    int type;
    MyPosKey* keys;
};
static __forceinline AnimInfo ComputePositionAtTime(const NiPosData* data)
{
    AnimInfo info;
    info.count = data->m_uiNumKeys;
    info.type = data->m_eType;
    info.keys = (MyPosKey*)data->m_pkKeys;
    return info;
}
typedef float (*CurvatureFunction)(MyPosKey*, unsigned int);
extern CurvatureFunction DAT_011f3d38[];

// Reads path data from a stream, updates constant-velocity state, and computes curvature from the path object's position keys.
// @fnv 0x00a3ee80
// src: GAMEBRYO_LIB 0x00a3ee80
// pdb: NiPathInterpolator::LinkObject | NiPathInterpolator.obj | retail, object-file run extension, low
// pdb params: kStream
// pdb locals: uiNumPathKeys, pkPathKeys, ePathType, ucSize, pfnCF
void NiPathInterpolator::LinkObject_00a3ee80(NiStream& stream)
{
    ((StreamBase*)this)->NiKeyBasedInterpolator_LinkObject(stream);
    AssignPathPointer(m_spPathData, (NiPosData*)stream.GetLoadedObject());
    AssignPathPointer(m_spPctData, stream.GetLoadedObject());
    SetCVDataNeedsUpdate(true);
    if (GetConstVelocity() && GetCVDataNeedsUpdate())
    {
        m_fTotalLength = GetAccumulatedValue();
        SetCVDataNeedsUpdate(false);
    }
    if (m_spPathData)
    {
        AnimInfo info = ComputePositionAtTime((NiPosData*)(NiPosData*)m_spPathData);
        unsigned int count = info.count;
        int type = info.type;
        MyPosKey* keys = info.keys;
        if (count > 0)
        {
            CurvatureFunction curvature = DAT_011f3d38[type];
            *(float*)&unk34 = curvature(keys, count);
        }
    }
}

static const float kInvalidValue = -3.4028234663852886e+38F;
inline bool PointEquals(const NiPoint3& a, const NiPoint3& b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline void NiQuatTransform::GetTransformScale(void* kTranslate)
{
    *reinterpret_cast<NiPoint3*>(&m_kTranslate) = *static_cast<const NiPoint3*>(kTranslate);
}

__forceinline void NiQuatTransform::SetLocalRotation(void* value)
{
    memcpy(&m_kRotate, value, sizeof(NiQuaternion));
}

inline void NiQuatTransform::MakeInvalid()
{
    m_fScale = qy = m_kTranslate = kInvalidValue;
}

float InterpolateKey(float fTime, void* pkKeys, int eType, unsigned int uiNumKeys, unsigned int& uiLastIdx,
                     unsigned char ucSize);

// The shared declaration lacks the original no-alias call annotation.
struct PathKeySearch
{
    __declspec(noalias) void FindKeyPair(float, int&, int&, float&);
};

// Computes the path position and optional rotation at fTime, writes them into kValue, and returns whether the transform was updated.
// @fnv 0x00a3f120
// pdb: NiPathInterpolator::Update | NiPathInterpolator.obj | retail, object-file run extension, low
// pdb params: fTime, pkInterpTarget, kValue
// pdb locals: fNormTime, uiNumPathKeys, pkPathKeys, uiNumPctKeys, ui2, pkPctKeys, fPct, ePctType, ePathType, ucPctSize, ucPathSize, ui1, kRefFrame
bool NiPathInterpolator::UpdateTransform(float fTime, NiObjectNET* pkTarget, NiQuatTransform& arValue)

{
    NiQuatTransform& kValue = arValue;
    if (!TimeHasChanged(fTime))
    {
        if (PointEquals(m_kPosition, DAT_011a9424))
        {
            kValue.MakeInvalid();
            return false;
        }
        kValue.GetTransformScale(&m_kPosition);
        if (GetFollow())
            kValue.SetLocalRotation(&m_kRotation);
        return true;
    }

    unsigned int uiNumKeys;
    int eType;
    unsigned char ucSize;
    void* pkKeys = 0;
    if (m_spPctData)
        {
        uiNumKeys = ((NiFloatData*)m_spPctData)->m_uiNumKeys;
        eType = ((NiFloatData*)m_spPctData)->m_eType;
        ucSize = ((NiFloatData*)m_spPctData)->m_ucKeySize;
        pkKeys = ((NiFloatData*)m_spPctData)->m_pkKeys;
    }
    if (!pkKeys)
        return false;
    void* pkPathKeys = 0;
    if (m_spPathData)
        pkPathKeys = m_spPathData->m_pkKeys;
    if (!pkPathKeys)
        return false;
    float fPct = InterpolateKey(fTime, pkKeys, eType, uiNumKeys, m_uiPercentKeyCount, ucSize);

    int iI1, iI2;
    ((PathKeySearch*)this)->FindKeyPair(fPct, iI1, iI2, fPct);

    if (GetFollow())
    {
        NiMatrix3 kRot;
        CalcRefFrame(iI1, iI2, fPct, kRot);
        m_kRotation.FromRotationMatrix(kRot);
    }

    m_kPosition = GenPathInterp(iI1, iI2, fPct);

    kValue.GetTransformScale(&m_kPosition);
    if (GetFollow())
        kValue.SetLocalRotation(&m_kRotation);

    m_fZValue = fTime;
    return true;
}

#include <string.h>
template <class T> static __forceinline NiPointer<NiObject>& EqualityPointer(T*& value)
{
    return (NiPointer<NiObject>&)value;
}

// Compares base equality, path objects and interpolator, path lengths, total length, point count, flags, and timing state.
// @fnv 0x00a3e7b0
// src: XBOX_PDB 0x00a3e7b0
// pdb: NiPathInterpolator::IsEqual | NiPathInterpolator.obj | retail, object-file gap fill, high
// pdb params: pkObject
// pdb locals: pkDest, uiNumPathKeys, uiCount, ePathType, ucPathSize
bool NiPathInterpolator::IsEqual_00a3e7b0(NiObject* pkObject)
{
    if (!BaseIsEqual(pkObject))
        return false;

    NiPathInterpolator* pkPath = (NiPathInterpolator*)pkObject;

    if ((EqualityPointer(m_spPathData) && !EqualityPointer(pkPath->m_spPathData)) || (!EqualityPointer(m_spPathData) && EqualityPointer(pkPath->m_spPathData)) ||
        (EqualityPointer(m_spPathData) && !EqualityPointer(m_spPathData)->IsEqual(EqualityPointer(pkPath->m_spPathData))))
        return false;

    if ((EqualityPointer(m_spPctData) && !EqualityPointer(pkPath->m_spPctData)) || (!EqualityPointer(m_spPctData) && EqualityPointer(pkPath->m_spPctData)) ||
        (EqualityPointer(m_spPctData) && !EqualityPointer(m_spPctData)->IsEqual(EqualityPointer(pkPath->m_spPctData))))
        return false;

    int uiNumKeys;
    int eType;
    char ucSize;
    float* pfPartialLength = m_pfPartialLength;
    if ((m_pfPartialLength && !pkPath->m_pfPartialLength) || (!m_pfPartialLength && pkPath->m_pfPartialLength) ||
        (m_pfPartialLength && (GetPathKeys(uiNumKeys, eType, ucSize),
                             memcmp(pfPartialLength, pkPath->m_pfPartialLength, uiNumKeys * sizeof(float)) != 0)))
        return false;

    if (m_fTotalLength != pkPath->m_fTotalLength || GetBit(1) != pkPath->GetBit(1) ||
        GetBit(2) != pkPath->GetBit(2) || m_eBankDir != pkPath->m_eBankDir ||
        GetBit(4) != pkPath->GetBit(4) || GetBit(8) != pkPath->GetBit(8) || GetBit(16) != pkPath->GetBit(16) ||
        GetBit(32) != pkPath->GetBit(32) || m_fMaxBankAngle != pkPath->m_fMaxBankAngle ||
        m_fSmoothing != pkPath->m_fSmoothing || m_sFollowAxis != pkPath->m_sFollowAxis ||
        GetBit(64) != pkPath->GetBit(64))
        return false;

    return true;
}

inline void NiMatrix3::SetCol(unsigned int column, const NiPoint3& point)
{
    (&m_pEntry[0][0])[column] = point.x;
    (&m_pEntry[1][0])[column] = point.y;
    (&m_pEntry[2][0])[column] = point.z;
}

inline void NiMatrix3::GetCol(unsigned int uiCol, NiPoint3& col) const
{
    col.x = (&m_pEntry[0][0])[uiCol];
    col.y = (&m_pEntry[1][0])[uiCol];
    col.z = (&m_pEntry[2][0])[uiCol];
}

#include <math.h>
namespace fnv_00a3de50 {
struct FV3
{
    float x, y, z;
    FV3()
    {}
    FV3(float a, float b, float c) : x(a), y(b), z(c)
    {}
    FV3 operator-() const
    {
        FV3 v;
        v.x = -x;
        v.y = -y;
        v.z = -z;
        return v;
    }
    void __thiscall Unitize_004a0c10();
    FV3 __thiscall FUN_004b3800(const FV3&) const;
    FV3 __thiscall FUN_00439e90(const FV3&) const;
};
struct PathBasis
{
    float x, y, z;
    FV3 __thiscall FUN_0053d1a0(const FV3&) const;
};
struct PathBlend
{
    FV3* __thiscall FUN_00439e90(FV3*, const FV3*);
};
extern char DAT_011a9448;
extern "C" void* __cdecl memcpy(void*, const void*, size_t);
extern PathBasis DAT_011a9484;
extern float DAT_011f3e40;
extern unsigned int DAT_011f3e44;
extern float DAT_01097304;
extern unsigned int g_pathBankFactorInitialized;
extern float g_pathBankFactor;
extern "C" double __cdecl atan(double);
extern void __cdecl ComputeFrenetFrame(float, MyPosKey*, MyPosKey*, unsigned int, FV3*, FV3*, FV3*, float*);
extern FV3 __cdecl FUN_004a3760(float, const FV3&);

extern "C" double __cdecl sqrt(double);
extern double DAT_01017cf8;
__declspec(noinline) FV3 PathBasis::FUN_0053d1a0(const FV3& pt) const
{
    FV3 cross(y * pt.z - z * pt.y, z * pt.x - x * pt.z, x * pt.y - y * pt.x);
    float length = float(sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z));
    if (length > DAT_01017cf8)
    {
        float inv = 1.0f / length;
        return FV3(cross.x * inv, cross.y * inv, cross.z * inv);
    }
    return FV3(0.0f, 0.0f, 0.0f);
}

// Computes the path basis matrix at a given time by interpolating position, tangent, and curvature, applying m_fLastPercent, axis, and bank adjustments.
// @fnv 0x00a3de50
}  // namespace fnv_00a3de50
using namespace fnv_00a3de50;

// src: XBOX_RETAIL_PDB 0x00a3de50
// pdb: NiPathInterpolator::CalcRefFrame | NiPathInterpolator.obj | retail, object-file run extension, high
// pdb params: ui1, ui2, fNormTime, kRefFrame
// pdb locals: uiNumPathKeys, pkPathKeys, kTmp2, kTm, kNormal, ePathType, kBinormal, ucSize, kTmp, fCurvature, kTangent, fA1, kNormal1, fA0, kTmpTan, fCurvature1, ui2a, ui1a, right, up, kTemp, kBankMat, fBankAmt
void __thiscall NiPathInterpolator::CalcRefFrame(unsigned int ui1, unsigned int ui2, float fNormTime, NiMatrix3* kRefFrame)
{
    NiMatrix3* out = kRefFrame;
    NiMatrix3 transform;
    ::memcpy(&transform, &DAT_011a9448, 36);
    unsigned int count, type;
    unsigned char stride;
    MyPosKey* pKeys = (MyPosKey*)GetPathKeys(count, type, stride);
    if (count < 2)
    {
        *out = transform;
        return;
    }
    {
        FV3 vPosition, vTangent, normal;
        float curvature, otherCurvature;
        if (GetBit(4))
        {
            ComputeFrenetFrame(fNormTime, GetKey(pKeys, ui1, stride), GetKey(pKeys, ui2, stride), type,
                               &vPosition, &vTangent, &normal, &curvature);
            if (m_fSmoothing != 0.0f)
            {
                float firstWeight, secondWeight;
                unsigned int index;
                if (fNormTime >= 1.0f - m_fSmoothing)
                {
                    if (!GetBit(2) && ui2 == count - 1)
                        index = 0;
                    else
                        index = ui2;
                    if (index < count - 1)
                    {
                        FV3 vOtherPos, vOtherTangent;
                        firstWeight = (m_fSmoothing + (1.0f - fNormTime)) / (2.0f * m_fSmoothing);
                        secondWeight = 1.0f - firstWeight;
                        ComputeFrenetFrame(1.0f - m_fSmoothing, GetKey(pKeys, ui1, stride),
                                           GetKey(pKeys, ui2, stride), type, &vOtherPos, &vTangent, &normal,
                                           &curvature);
                        ComputeFrenetFrame(m_fSmoothing, GetKey(pKeys, index, stride), GetKey(pKeys, index + 1, stride),
                                           type, &vOtherPos, &vOtherTangent, &normal, &otherCurvature);
                        vTangent =
                            FUN_004a3760(firstWeight, vTangent).FUN_00439e90(FUN_004a3760(secondWeight, vOtherTangent));
                        vTangent.Unitize_004a0c10();
                        curvature = firstWeight * curvature + secondWeight * otherCurvature;
                        normal = vPosition.FUN_004b3800(vTangent);
                    }
                }
                else if (fNormTime <= m_fSmoothing)
                {
                    if (!GetBit(2) && ui1 == 0)
                        index = count - 1;
                    else
                        index = ui1;
                    if (index >= 1)
                    {
                        FV3 vOtherPos, vOtherTangent;
                        secondWeight = (m_fSmoothing - fNormTime) / (2.0f * m_fSmoothing);
                        firstWeight = 1.0f - secondWeight;
                        ComputeFrenetFrame(m_fSmoothing, GetKey(pKeys, ui1, stride),
                                           GetKey(pKeys, ui2, stride), type, &vOtherPos, &vTangent, &normal,
                                           &curvature);
                        ComputeFrenetFrame(1.0f - m_fSmoothing, GetKey(pKeys, index - 1, stride),
                                           GetKey(pKeys, index, stride), type, &vOtherPos, &vOtherTangent, &normal,
                                           &otherCurvature);
                        vTangent =
                            FUN_004a3760(firstWeight, vTangent).FUN_00439e90(FUN_004a3760(secondWeight, vOtherTangent));
                        vTangent.Unitize_004a0c10();
                        curvature = firstWeight * curvature + secondWeight * otherCurvature;
                        normal = vPosition.FUN_004b3800(vTangent);
                    }
                }
            }
            transform.SetCol(0, (const NiPoint3&)vPosition);
            transform.SetCol(1, (const NiPoint3&)-vTangent);
            transform.SetCol(2, (const NiPoint3&)-normal);
        }
        else
        {
            ComputeFrenetFrame(fNormTime, GetKey(pKeys, ui1, stride), GetKey(pKeys, ui2, stride), type,
                               &vPosition, &vTangent, &normal, &curvature);
            transform.SetCol(0, (const NiPoint3&)vPosition);
            FV3 vBasis = DAT_011a9484.FUN_0053d1a0(vPosition);
            transform.SetCol(1, (const NiPoint3&)vBasis);
            FV3 vBinormal;
            float fCrossPosY = vPosition.y;
            float fCrossPosX = vPosition.x;
            float fCrossBasisX = vBasis.x;
            vBinormal.x = fCrossPosY * vBasis.z - vPosition.z * vBasis.y;
            vBinormal.y = fCrossBasisX * vPosition.z - fCrossPosX * vBasis.z;
            vBinormal.z = vBasis.y * fCrossPosX - fCrossBasisX * fCrossPosY;
            transform.SetCol(2, (const NiPoint3&)vBinormal);
        }
        if (GetBit(64))
        {
            FV3 vCol;
            static_cast<const NiMatrix3&>(transform).GetCol(0, (NiPoint3&)vCol);
            transform.SetCol(0, (const NiPoint3&)-vCol);
            static_cast<const NiMatrix3&>(transform).GetCol(1, (NiPoint3&)vCol);
            transform.SetCol(1, (const NiPoint3&)-vCol);
        }
        FV3 vOld;
        switch (m_sFollowAxis)
        {
        case 1: {
            FV3 vCol;
            static_cast<const NiMatrix3&>(transform).GetCol(0, (NiPoint3&)vOld);
            static_cast<const NiMatrix3&>(transform).GetCol(1, (NiPoint3&)vCol);
            transform.SetCol(0, (const NiPoint3&)-vCol);
            transform.SetCol(1, (const NiPoint3&)vOld);
            break;
        }
        case 2: {
            FV3 vCol;
            static_cast<const NiMatrix3&>(transform).GetCol(0, (NiPoint3&)vOld);
            static_cast<const NiMatrix3&>(transform).GetCol(2, (NiPoint3&)vCol);
            transform.SetCol(0, (const NiPoint3&)-vCol);
            transform.SetCol(2, (const NiPoint3&)vOld);
            break;
        }
        }
        if (GetBit(8))
        {
            if (!(g_pathBankFactorInitialized & 1))
            {
                g_pathBankFactorInitialized |= 1;
                g_pathBankFactor = DAT_01097304;
            }
            otherCurvature = float(m_eBankDir) * m_fMaxBankAngle;
            if (curvature < m_fPathLength)
            {
                float ratio = curvature / m_fPathLength;
                otherCurvature *= g_pathBankFactor * float(atan(ratio));
            }
            NiMatrix3 rotation;
            rotation.MakeRotation(otherCurvature, vPosition.x, vPosition.y, vPosition.z);
            transform = static_cast<const NiMatrix3&>(rotation).operator*(transform);
        }
    }
    *out = transform;
}
