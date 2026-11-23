// gamebryo2.2/corelibs/nimain/niextradata.cpp: NiExtraData and NiAccumulator (13 functions)

#include "../../../../include/NiAccumulator.h"
#include "../../../../include/NiBinaryExtraData.h"
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiExtraData.h"
#include "../../../../include/NiFixedString.h"
#include "../../../../include/NiGlobalStringTable.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/StrArr.h"

// Unit-specific stream declarations and inline string operations.

extern void* kVtbl_NiExtraData[];
inline NiFixedString::operator const char*() const
{
    return m_kHandle;
}
inline unsigned int NiBinaryStream::ReadRaw(void* out, unsigned int n, unsigned int* size)
{
    return (*reinterpret_cast<ReadFunction*>(reinterpret_cast<char*>(this) + 8))(this, out, n, size, 1);
}
inline unsigned int NiBinaryStream::Read(void* out, unsigned int n)
{
    unsigned int size = 4;
    unsigned int amount = ReadRaw(out, n, &size);
    *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(this) + 4) += amount;
    return amount;
}
// Accessors for the stream's opaque fields retain the original layout.
inline unsigned int NiStream::GetFileVersion() const
{
    return *reinterpret_cast<const unsigned int*>(reinterpret_cast<const char*>(this) + 0xd8);
}
inline NiBinaryStream* NiStream::GetBinaryStream()
{
    return *reinterpret_cast<NiBinaryStream**>(reinterpret_cast<char*>(this) + 0x24c);
}

void* _NiMalloc(unsigned int);
template <class T> void ReadBinaryValue(NiStream&, T*, unsigned int);
extern const char g_011c7820_arg1[];

inline bool IsExactKindOf(const void* type, NiObject* object)
{
    if (object)
        return object->GetRTTI() == type;
    return false;
}

inline void ReadCount(NiStream& stream, unsigned int& count)
{
    stream.GetBinaryStream()->Read(&count, 4);
}

// @fnv 0x00aa0fe0
void NiAccumulator::ClearCamera()
{
    m_pkCamera = 0;
}
// @fnv 0x00a7b3b0
// pdb: NiExtraData::LinkObject | NiExtraData.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiExtraData::LinkObject(NiStream& kStream)
{
    if (kStream.GetFileVersion() < 0x0500000b)
        m_kName.m_kHandle = reinterpret_cast<char*>(kStream.GetLoadedObject());
}
// Constructs a NiExtraData: sets the vtable, then assigns name from src with interlocked refcount updates on the string handle.
// @fnv 0x00a7b440
// pdb: NiExtraData::NiExtraData | NiExtraData.obj | retail, object-file gap fill, medium
// pdb note: debug build links it to BSBound::BSBound
NiExtraData::NiExtraData(const NiFixedString& aName)
{
    const NiFixedString& src = aName;
    char* empty = m_kName.m_kHandle;
    char* srcHandle = src.m_kHandle;
    if (empty != srcHandle)
    {
        if (srcHandle != NiGlobalStringTable::ms_pcEmptyString)
            InterlockedIncrement((long*)((char*)srcHandle - 8));
        if (m_kName.m_kHandle != NiGlobalStringTable::ms_pcEmptyString)
            InterlockedDecrement((long*)((char*)m_kName.m_kHandle - 8));
        m_kName.m_kHandle = srcHandle;
    }
}
// Loads NiExtraData from a stream: reads a count and byte buffer for old versions, otherwise reads the name, registering a NiBinaryExtraData object.
// @fnv 0x00a7b4a0
// pdb: NiExtraData::LoadBinary | NiExtraData.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: uiSize, pcChunk, pkBinaryExtraData
void NiExtraData::LoadBinary(NiStream& kStream)
{
    NiObject::LoadBinary(kStream);
    if (kStream.GetFileVersion() < 0x0500000b)
    {
        kStream.ResetStreamState();
        unsigned int count;
        ReadCount(kStream, count);
        if (count > 0 && IsExactKindOf(g_011c7820_arg1, this))
        {
            unsigned char* buffer = (unsigned char*)_NiMalloc(count);
            ReadBinaryValue(kStream, buffer, count);
            NiBinaryExtraData* extra = new NiBinaryExtraData(count, buffer);
            if (extra)
                kStream.ChangeObject(extra);
        }
    }
    else if (kStream.GetFileVersion() < 0x14010001)
        kStream.ReadFixedString(m_kName);
    else
        kStream.ReadStringTableEntry(m_kName);
}

// The shared header does not declare this override; preserve its thiscall ABI.
// @fnv 0x00a7b290
// pdb: NiExtraData::RegisterStreamables | NiExtraData.obj | retail, vtable slot, high
// pdb params: kStream
bool __fastcall RegisterExtraDataStreamables(NiExtraData* self, void*, NiStream& kStream)
{
    if (!self->NiObject::RegisterStreamables(kStream))
        return false;
    // RegisterFixedString (0x00a66480) occupies slot 8; retain the original byte-to-bool conversion.
    typedef unsigned char (__thiscall* RegisterStringFunction)(NiStream*, const NiFixedString&);
    return reinterpret_cast<RegisterStringFunction>((*reinterpret_cast<void***>(&kStream))[8])(
        &kStream, self->m_kName);
}

// @fnv 0x00a7b330
// src: XBOX_PDB 0x00a7b330
// pdb: NiExtraData::CopyMembers | NiExtraData.obj | retail, call graph, low
// pdb note: debug build links it to NiVertWeightsExtraData::CopyMembers
// pdb params: pDest, kCloning
void NiExtraData::CopyMembers_00a7b330(void* destination, CloningProcess* kCloning)
{
    NiExtraData* pkDest = static_cast<NiExtraData*>(destination);
    NiObject::CreateVertexBufferSet(pkDest, *kCloning);
    char* src = m_kName.m_kHandle;
    if (pkDest->m_kName.m_kHandle != src)
    {
        if (src != NiGlobalStringTable::ms_pcEmptyString)
            InterlockedIncrement((long*)(src - 8));
        char* dst = pkDest->m_kName.m_kHandle;
        if (dst != NiGlobalStringTable::ms_pcEmptyString)
            InterlockedDecrement((long*)(dst - 8));
        pkDest->m_kName.m_kHandle = src;
    }
}

inline void* __cdecl operator new(size_t, void* p)
{
    return p;
}
// Allocates a 12-byte NiExtraData, constructs it, sets its vtable and copies a stored int into offset 8, returning the object or null.
// @fnv 0x00a7b380
// src: EXE 0x00a7b380
// pdb: NiExtraData::CreateObject | NiExtraData.obj | retail, upstream name already matches, high
// pdb locals: pkObject
NiExtraData* NiExtraData_CreateObject()
{
    NiExtraData* p = (NiExtraData*)NiExtraData::operator new(sizeof(NiExtraData));
    if (p)
    {
        ::new (p) NiObject;
        *(void**)p = (void*)kVtbl_NiExtraData;
        p->m_kName.m_kHandle = NiGlobalStringTable::ms_pcEmptyString;
        return p;
    }
    return 0;
}

// @fnv 0x00a7b2c0
// pdb: NiExtraData::SaveBinary | NiExtraData.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiExtraData::SaveBinary(NiStream& kStream)
{
    // The empty base implementation is folded with LinkObject in the original.
    NiObject::LinkObject(kStream);
    kStream.SerializeString(reinterpret_cast<const char*>(&m_kName));
}

// Restores the NiExtraData vtable, releases the reference-counted object at offset 8 if not the empty sentinel, then calls the NiRefObject destructor.
// @fnv 0x00a7b300
// src: GUESS 0x00a7b300 (confidence: medium)
// pdb: NiExtraData::~NiExtraData | NiExtraData.obj | debug, call graph, medium
void NiExtraData::DestroyFields_00a7b300()
{
    *reinterpret_cast<void* volatile*>(this) = kVtbl_NiExtraData;

    void* pRef = *reinterpret_cast<char* const volatile*>(&m_kName.m_kHandle);

    if (pRef != NiGlobalStringTable::ms_pcEmptyString)
        InterlockedDecrement(reinterpret_cast<long*>(reinterpret_cast<unsigned char*>(pRef) - 8));

    NiRefObject::~NiRefObject();
}

// @fnv 0x00a7b2e0
// pdb: NiExtraData::NiExtraData | NiExtraData.obj | retail, call graph, medium
__fastcall NiExtraData::NiExtraData()
{
}

// The shared header does not declare this comparison override.
// @fnv 0x00a7b3d0
// pdb: NiExtraData::IsEqual | NiExtraData.obj | retail, vtable slot, high
// pdb params: pObject
// pdb locals: pkExtra
unsigned char __fastcall IsExtraDataEqual(NiExtraData* self, void*, NiExtraData* object)
{
    if (!self->NiObject::IsSameAs(object))
        return 0;

    return self->m_kName.m_kHandle == object->m_kName.m_kHandle;
}

extern "C" void _ReadWriteBarrier(void);
void __cdecl FUN_00aa1460(void* ptr, unsigned int size);

// The compiler-generated deleting destructor is represented explicitly here.
// @fnv 0x00a7b400
// pdb: NiExtraData::`scalar deleting destructor' | NiExtraData.obj | retail, object-file gap fill, high
NiExtraData* __fastcall DestroyExtraData(NiExtraData* self, void*, unsigned char acFlags)
{
    *reinterpret_cast<void* volatile*>(self) = kVtbl_NiExtraData;
    _ReadWriteBarrier();
    if (self->m_kName.m_kHandle != NiGlobalStringTable::ms_pcEmptyString)
    {
        InterlockedDecrement((long*)((char*)self->m_kName.m_kHandle - 8));
    }
    self->NiObject::FUN_00a5d3d0();
    if (acFlags & 1)
    {
        FUN_00aa1460(self, sizeof(NiExtraData));
    }
    return self;
}

inline void AppendExtraDataString(StrArr* strings, char* value)
{
    unsigned int index = strings->m_usSize;
    if (index >= strings->m_usMaxSize)
        strings->SetSize(index + strings->m_usGrowBy);
    strings->SetAt(index, value);
}
extern const char* const g_01270908_arg1;
extern const char DAT_01097790[];
extern "C" char* NiViewerStrings_NiGetViewerString_Title(const char*);
char* NiGetViewerString_00a7d5e0(const char*, const char*);
// The shared header does not declare the StrArr viewer-string overload.
// @fnv 0x00a7b580
// pdb: NiExtraData::GetViewerStrings | NiExtraData.obj | retail, string-set bridge, high
// pdb params: pkStrings
void __fastcall GetExtraDataViewerStrings(NiExtraData* self, void*, StrArr* strings)
{
    AppendExtraDataString(strings, NiViewerStrings_NiGetViewerString_Title(g_01270908_arg1));
    AppendExtraDataString(strings, NiGetViewerString_00a7d5e0(DAT_01097790, static_cast<const char*>(self->m_kName)));
}
