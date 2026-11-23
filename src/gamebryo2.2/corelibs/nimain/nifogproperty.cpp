
// src/gamebryo2.2/corelibs/nimain/nifogproperty.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nimain/nifogproperty.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiColor.h"
#include "../../../../include/NiFogProperty.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiPlane.h"
#include "../../../../include/NiProperty.h"

extern NiFogProperty* DAT_011f5f3c;
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(long volatile*);
// Releases the global fog property and clears the pointer.
// @fnv 0x00a9a120
// src: COMMUNITY 0x00a9a120
// pdb: NiFogProperty::_SDMShutdown | NiFogProperty.obj | retail, upstream name already matches, high
void NiFogProperty__SDMShutdown()
{
    if (DAT_011f5f3c)
    {
        NiFogProperty* p = DAT_011f5f3c;
        if (!InterlockedDecrement(reinterpret_cast<volatile long*>(&DAT_011f5f3c->m_uiRefCount)))
            p->DeleteThis();
        DAT_011f5f3c = 0;
    }
}

extern NiColor DAT_011f4998;
extern int DAT_011f499c;
extern int DAT_011f49a0;
inline NiColor::NiColor(float red, float green, float blue) : r(red), g(green), b(blue)
{}

// @fnv 0x00a99ed0
// pdb: NiFogProperty::NiFogProperty | BSFogProperty.obj | retail, upstream name already matches, high
NiFogProperty::NiFogProperty() : m_kColor(0.0f, 0.0f, 0.0f)
{
    m_uFlags = 0;
    m_fDepth = 1.0f;
    m_kColor = DAT_011f4998;
}

enum FogFunction
{
    FOG_Z_LINEAR = 0,
    FOG_RANGE_SQ = 1,
    FOG_VERTEX_ALPHA = 2
};

extern "C" unsigned int __cdecl strlen(const char*);
void* __cdecl NiMalloc(unsigned int);
char* __cdecl NiSprintf(char*, unsigned int, const char*, ...);

// Allocates a string of strlen(pcPrefix)+20 bytes and formats pcPrefix with the name of the given fog function.
// @fnv 0x00a99e50
// src: XBOX_RETAIL_PDB 0x00a99e50
// pdb: NiFogProperty::GetViewerString | NiFogProperty.obj | retail, string-set bridge, high
// pdb params: pcPrefix, eFunc
// pdb locals: pcString, uiLen
char* NiFogProperty_GetViewerString(const char* pcPrefix, FogFunction eFunc)
{
    char* pcString;
    unsigned int uiLen = strlen(pcPrefix);

    pcString = static_cast<char*>(NiMalloc(uiLen + 20));

    switch (eFunc)
    {
    case FOG_Z_LINEAR:
        NiSprintf(pcString, uiLen + 20, "%s = FOG_Z_LINEAR", pcPrefix);
        break;

    case FOG_RANGE_SQ:
        NiSprintf(pcString, uiLen + 20, "%s = FOG_RANGE_SQ", pcPrefix);
        break;

    case FOG_VERTEX_ALPHA:
        NiSprintf(pcString, uiLen + 20, "%s = FOG_VERTEX_ALPHA", pcPrefix);
        break;
    }

    return pcString;
}

// The shared NiFogProperty does not yet declare these routines.
// These helpers carry no object layout; all state uses the shared class.
struct FogPropertyRoutines
{
    bool GetFog() const
    {
        return (reinterpret_cast<const NiFogProperty*>(this)->m_uFlags & 1) != 0;
    }
    FogFunction GetFogFunction() const
    {
        return (FogFunction)((reinterpret_cast<const NiFogProperty*>(this)->m_uFlags >> 1) & 3);
    }
    void* Clone(int arg);
    bool IsEqual(NiFogProperty* other);
    void LoadPropertyData(NiStream* kStream);
    void GetViewerStrings(struct ViewerStrings* pkStrings);
    void SaveBinary(NiStream& stream);
};

struct PropertyCloneRoutines
{
    void NiProperty_CopyMembers(void* prop, int arg);
};

void* __cdecl Object_DispatchCommandFour(unsigned int);
void __stdcall CreateVertexBufferSet(void*, int);

void* __cdecl operator new(size_t, void* p)
{
    return p;
}

// @fnv 0x00a99f60
void* FogPropertyRoutines::Clone(int arg)
{
    NiFogProperty* self = reinterpret_cast<NiFogProperty*>(this);
    if (self->m_spControllers != 0)
    {
        void* raw = Object_DispatchCommandFour(0x2c);
        NiFogProperty* esi;
        if (raw)
        {
            esi = new (raw) NiFogProperty();
        }
        else
        {
            esi = 0;
        }
        ((PropertyCloneRoutines*)this)->NiProperty_CopyMembers(esi, arg);
        unsigned short cx = self->m_uFlags;
        esi->m_uFlags = cx;
        esi->m_fDepth = self->m_fDepth;
        esi->m_kColor = self->m_kColor;
        return esi;
    }
    else
    {
        CreateVertexBufferSet(this, arg);
        return this;
    }
}

void* __cdecl Object_DispatchCommandFour(unsigned int bytes);

// This factory inlines construction in the original, while Clone calls the constructor.
extern void* NiFogPropertyVTable[];
__forceinline void ConstructFogPropertyInPlace(NiFogProperty* property)
{
    property->NiProperty::NiProperty();
    *reinterpret_cast<void**>(property) = NiFogPropertyVTable;
    property->m_kColor.NiColor::NiColor(0.0f, 0.0f, 0.0f);
    property->m_uFlags = 0;
    property->m_fDepth = 1.0f;
    property->m_kColor = DAT_011f4998;
}

// Allocates memory for a NiFogProperty and constructs it in place, returning the new object or null on allocation failure.
// @fnv 0x00a99fd0
NiFogProperty* __cdecl CreateNiFogProperty()
{
    void* memory = Object_DispatchCommandFour(sizeof(NiFogProperty));
    if (memory != 0)
    {
        NiFogProperty* property = static_cast<NiFogProperty*>(memory);
        ConstructFogPropertyInPlace(property);
        return property;
    }
    return 0;
}

struct EqBase
{
    bool __thiscall NiSequenceStreamHelper_IsEqual(NiObject* pObject);
};

// @fnv 0x00a9a030
bool __thiscall FogPropertyRoutines::IsEqual(NiFogProperty* other)
{
    NiFogProperty* self = reinterpret_cast<NiFogProperty*>(this);
    EqBase* base = reinterpret_cast<EqBase*>(this);

    if (base->NiSequenceStreamHelper_IsEqual(reinterpret_cast<NiObject*>(other)))
    {
        volatile unsigned char* ownFlags = reinterpret_cast<volatile unsigned char*>(&self->m_uFlags);
        unsigned char* otherFlags = reinterpret_cast<unsigned char*>(&other->m_uFlags);

        if (((*ownFlags ^ *otherFlags) & 1) == 0)
        {
            if (self->m_fDepth == other->m_fDepth)
            {
                if (((*ownFlags ^ *otherFlags) & 6) == 0)
                {
                    NiPlane* color = reinterpret_cast<NiPlane*>(&self->m_kColor);
                    const NiPlane* otherColor = reinterpret_cast<const NiPlane*>(&other->m_kColor);

                    if (!color->FUN_00439090(otherColor))
                        return true;
                }
            }
        }
    }

    return false;
}

struct PropertyFlagWord
{
    unsigned short value;
    void Merge(unsigned bits)
    {
        unsigned short retained = value;
        retained &= 0xfff9;
        retained |= (unsigned short)(bits * 2);
        value = retained;
    }
};
void __stdcall FUN_00a5d7f0(NiStream*);
// Loads property flags and extra data from the stream based on version, then loads the sub-object at offset 0x20.
// @fnv 0x00a9a150
// pdb: NiFogProperty::LoadBinary | NiFogProperty.obj | retail, object-file gap fill, high
// pdb params: kStream
// pdb locals: bFog, eFunc
void FogPropertyRoutines::LoadPropertyData(NiStream* kStream)
{
    NiFogProperty* self = reinterpret_cast<NiFogProperty*>(this);
    NiStream* original = kStream;
    FUN_00a5d7f0(original);
    if (original->m_uiNifFileVersion < 0x0a000102)
    {
        unsigned raw = original->m_usNiPropertyFlags;
        self->m_uFlags = (unsigned short)raw;
        if (original->m_uiNifFileVersion < 0x05000016)
        {
            unsigned char bit = (unsigned char)raw;
            bit >>= 1;
            bit &= 1;
            raw >>= 3;
            raw &= 3;
            self->m_uFlags = 0;
            if (bit)
                self->m_uFlags = 1;
            else
                self->m_uFlags = 0;
            reinterpret_cast<PropertyFlagWord*>(&self->m_uFlags)->Merge(raw);
        }
        else
        {
            self->m_uFlags = (unsigned short)(raw & 7);
        }
    }
    else
    {
        NiBinaryStream* pReader = original->m_pkIstr;
        NiBinaryStream::NIBINARYSTREAM_READFN pReadFn = pReader->m_pfnRead;
        unsigned& amount = *(unsigned*)&kStream;
        amount = 2;
        int count = pReadFn(pReader, &self->m_uFlags, 2, &amount, 1);
        pReader->m_uiAbsoluteCurrentPos += count;
    }
    NiBinaryStream* pReader = original->m_pkIstr;
    NiBinaryStream::NIBINARYSTREAM_READFN pReadFn = pReader->m_pfnRead;
    unsigned& amount = *(unsigned*)&kStream;
    amount = 4;
    int count = pReadFn(pReader, reinterpret_cast<unsigned*>(&self->m_fDepth), 4, &amount, 1);
    pReader->m_uiAbsoluteCurrentPos += count;
    self->m_kColor.LoadBinary(*original);
}

struct ViewerStrings
{
    void* vtbl;
    char** m_pBase;
    unsigned short m_usMaxSize;
    unsigned short m_usSize;
    unsigned short m_usESize;
    unsigned short m_usGrowBy;

    void SetSize(unsigned int uiMaxSize);
    void SetAt(unsigned int uiIndex, char*& element);
    unsigned int Add(char* const& element)
    {
        unsigned int uiIndex = m_usSize;
        if (uiIndex >= m_usMaxSize)
            SetSize(uiIndex + m_usGrowBy);
        SetAt(uiIndex, (char*&)element);
        return uiIndex;
    }
};

struct RTTIStub
{
    const char* m_pcName;
};
extern RTTIStub g_fogRTTI;

char* MakeViewerString(const char* pcText);
char* MakeViewerString(const char* pcPrefix, bool bValue);
char* MakeViewerString(const char* pcPrefix, float fValue);

struct PropertyBase
{
    void GetViewerStringsBase(ViewerStrings* apViewerStrings);
};

// @fnv 0x00a9a2a0
// pdb: NiFogProperty::GetViewerStrings | NiFogProperty.obj | retail, string-set bridge, high
// pdb params: pkStrings
void FogPropertyRoutines::GetViewerStrings(ViewerStrings* pkStrings)
{
    NiFogProperty* self = reinterpret_cast<NiFogProperty*>(this);
    reinterpret_cast<PropertyBase*>(this)->GetViewerStringsBase(pkStrings);
    pkStrings->Add(MakeViewerString(g_fogRTTI.m_pcName));
    pkStrings->Add(MakeViewerString("Enable", GetFog()));
    pkStrings->Add(MakeViewerString("Depth", self->m_fDepth));
    pkStrings->Add(NiFogProperty_GetViewerString("Function", GetFogFunction()));
    pkStrings->Add(self->m_kColor.GetViewerString("Color"));
}

inline unsigned int NiBinaryStream::BinaryWrite(const void* pvBuffer, unsigned int uiTotalBytes,
                                                 unsigned int* puiComponentSizes, unsigned int uiNumComponents)
{
    unsigned int uiBytesWritten = m_pfnWrite(this, pvBuffer, uiTotalBytes, puiComponentSizes, uiNumComponents);
    m_uiAbsoluteCurrentPos += uiBytesWritten;
    return uiBytesWritten;
}

template <class T> inline void NiBinaryStreamSave(NiBinaryStream& os, T* pValue, unsigned int uiNumEls)
{
    unsigned int uiSize = sizeof(T);
    os.BinaryWrite(pValue, sizeof(T) * uiNumEls, &uiSize, 1);
}

template <class T> inline void WriteBinaryValue(NiStream& kStream, T& value)
{
    NiBinaryStreamSave(*kStream.m_pkOstr, &value, 1);
}

class PropertySaveRoutines
{
  public:
    void NiSequenceStreamHelper_SaveBinary(NiStream& kStream);
};

// Saves the base property, flags, depth and RGB tint to the output stream.
// @fnv 0x00a9a230
// pdb: NiFogProperty::SaveBinary | NiFogProperty.obj | retail, object-file gap fill, high
// pdb params: kStream
void FogPropertyRoutines::SaveBinary(NiStream& kStream)
{
    NiFogProperty* self = reinterpret_cast<NiFogProperty*>(this);
    reinterpret_cast<PropertySaveRoutines*>(this)->NiSequenceStreamHelper_SaveBinary(kStream);
    WriteBinaryValue(kStream, self->m_uFlags);
    WriteBinaryValue(kStream, self->m_fDepth);
    self->m_kColor.SaveBinary(kStream);
}

extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long volatile*);
inline void NiRefObject::IncRefCount()
{
    InterlockedIncrement(reinterpret_cast<volatile long*>(&m_uiRefCount));
}

inline void NiRefObject::DecRefCount()
{
    if (InterlockedDecrement(reinterpret_cast<volatile long*>(&m_uiRefCount)) == 0)
        DeleteThis();
}

template <class T> struct PtrRef
{
    T*& m_pObject;
    PtrRef(T*& r) : m_pObject(r)
    {}
    void operator=(T* pObject)
    {
        if (m_pObject != pObject)
        {
            if (m_pObject)
                m_pObject->NiRefObject::DecRefCount();
            m_pObject = pObject;
            if (m_pObject)
                m_pObject->NiRefObject::IncRefCount();
        }
    }
};
// Allocates a NiFogProperty and assigns it to DAT_011f5f3c through a reference-counting smart pointer.
// @fnv 0x00a9a090
// src: COMMUNITY 0x00a9a090
// pdb: NiFogProperty::_SDMInit | NiFogProperty.obj | retail, upstream name already matches, high
void NiFogProperty__SDMInit()
{
    PtrRef<NiFogProperty> r(DAT_011f5f3c);
    r = new (Object_DispatchCommandFour(sizeof(NiFogProperty))) NiFogProperty;
}
