#include "../../../../include/NiRenderer.h"
#include "../../../../include/NiScreenTexture.h"
#include "../../../../include/NiColorA.h"
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiObject.h"
#include "../../../../include/NiRefObject.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiTexturingProperty.h"

inline NiColorA::NiColorA(float red, float green, float blue, float alpha)
    : r(red), g(green), b(blue), a(alpha)
{}

// The shared color header does not yet declare the stream methods. Keep their
// original member-call ABI through external entry points, without a color copy.
extern "C" void NiColorA_LoadBinary();
extern "C" void NiColorA_SaveBinary();

inline void* __cdecl operator new(size_t, void* memory) throw()
{
    return memory;
}

// src/gamebryo2.2/corelibs/nimain/niscreentexture.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nimain/niscreentexture.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern const void* g_011f5ee0;

// @fnv 0x00a955f0
const NiRTTI* NiScreenTexture::GetRTTI() const
{
    return reinterpret_cast<const NiRTTI*>(&g_011f5ee0);
}

// Calls NiObject::RegisterStreamables on the stream, then registers the texturing property's streamables; returns false if the base call fails.
// @fnv 0x00a954f0
// pdb: NiScreenTexture::RegisterStreamables | NiScreenTexture.obj | retail, upstream name already matches, high
// pdb params: kStream
bool NiScreenTexture::RegisterStreamables(NiStream& kStream)
{
    if (!NiObject::RegisterStreamables(kStream))
        return false;
    m_spTexProp->RegisterStreamables(kStream);
    return true;
}

extern int DAT_010a048c;
void __cdecl _NiFreeAlias(void*);
extern "C" __declspec(dllimport) long __stdcall InterlockedDecrement(volatile long*);
// @fnv 0x00a95590
// src: GUESS 0x00a95590 (confidence: medium)
// pdb: NiScreenTexture::~NiScreenTexture | NiScreenTexture.obj | retail, object-file run extension, low
void NiScreenTexture::ReleaseReference_00a95590()
{
    *reinterpret_cast<void**>(this) = (void*)&DAT_010a048c;
    NiRenderer* r = NiRenderer::ms_pRenderer;
    if (r)
    {
        // NiRenderer does not yet declare the cleanup virtual at slot 63.
        typedef void (__thiscall* ReleaseScreenTexture)(NiRenderer*, NiScreenTexture*);
        reinterpret_cast<ReleaseScreenTexture>((*reinterpret_cast<void***>(r))[63])(r, this);
    }
    NiRefObject* p = m_spTexProp;
    if (p)
    {
        if (InterlockedDecrement(reinterpret_cast<volatile long*>(&p->m_uiRefCount)) == 0)
            p->DeleteThis();
    }
    _NiFreeAlias(m_kScreenRects);
    ((NiRefObject*)this)->NiRefObject::~NiRefObject();
}

void __cdecl _delete(void*, unsigned int);
// Calls FUN_00a95590, then if flags has bit 1 set frees this with size 0x20, and returns this.
// @fnv 0x00a95740
// pdb: NiScreenTexture::`scalar deleting destructor' | NiScreenTexture.obj | retail, object-file gap fill, high
void* __thiscall NiScreenTexture::FUN_00a95740(unsigned int flags)
{
    ReleaseReference_00a95590();
    if (flags & 1)
        _delete(this, sizeof(NiScreenTexture));
    return this;
}

// Preserve the stream's inline transfer and position accounting.
inline unsigned int NiBinaryStream::Write(const void* pvBuffer, unsigned int uiBytes,
                                         unsigned int* puiComponentSizes, unsigned int uiNumComponents)
{
    unsigned int uiBytesWritten = (*reinterpret_cast<NIBINARYSTREAM_WRITEFN*>(reinterpret_cast<char*>(this) + 0x0C))(
        this, pvBuffer, uiBytes, puiComponentSizes, uiNumComponents);
    *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(this) + 4) += uiBytesWritten;
    return uiBytesWritten;
}

inline unsigned int NiBinaryStream::Read(void* buffer, unsigned int bytes,
                                        unsigned int* componentSizes, unsigned int components)
{
    unsigned int n = (*reinterpret_cast<NIBINARYSTREAM_READFN*>(reinterpret_cast<char*>(this) + 8))(
        this, buffer, bytes, componentSizes, components);
    *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(this) + 4) += n;
    return n;
}

template <class T> inline void WriteBinaryArray(NiBinaryStream& binstream, const T* pValue, unsigned int uiNumEls)
{
    unsigned int uiSize = sizeof(T);
    binstream.Write(pValue, uiNumEls * sizeof(T), &uiSize, uiNumEls);
}
template <class T> inline void SerializeValue(NiStream& kStream, const T& value)
{
    WriteBinaryArray(**reinterpret_cast<NiBinaryStream**>(reinterpret_cast<char*>(&kStream) + 0x250), &value, 1);
}
struct RendererDataRecord
{
    short m_sFirstValue;
    short m_sSecondValue;
    unsigned short m_usFirstValue;
    unsigned short m_usSecondValue;
    unsigned short m_usThirdValue;
    unsigned short m_usFourthValue;
    NiColorA m_kFogTint;
};
// Links the object to the stream, writes the rect count, then serializes each rect's shorts and color, and saves the link ID.
// @fnv 0x00a95a90
// pdb: NiScreenTexture::SaveBinary | NiScreenTexture.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: uiNumRects, i, kColor, kRect
void NiScreenTexture::SaveBinary(NiStream& kStream)
{
    NiObject::LinkObject(kStream);
    unsigned int uiNumRects = m_uiAllocatedCapacity;
    SerializeValue(kStream, uiNumRects);
    for (unsigned int i = 0; i < uiNumRects; i++)
    {
        const RendererDataRecord* pDataToSerialize = &m_kScreenRects[i];
        SerializeValue(kStream, pDataToSerialize->m_sFirstValue);
        SerializeValue(kStream, pDataToSerialize->m_sSecondValue);
        SerializeValue(kStream, pDataToSerialize->m_usFirstValue);
        SerializeValue(kStream, pDataToSerialize->m_usSecondValue);
        SerializeValue(kStream, pDataToSerialize->m_usThirdValue);
        SerializeValue(kStream, pDataToSerialize->m_usFourthValue);
        NiColorA kColor = pDataToSerialize->m_kFogTint;
        ((void (__thiscall*)(NiColorA*, NiStream&))NiColorA_SaveBinary)(&kColor, kStream);
    }
    kStream.SaveLinkID(reinterpret_cast<const NiObject*>(m_spTexProp));
}

void* __cdecl _NiMalloc_00aa1070(unsigned int bytes);
void __cdecl _NiFree_00aa10f0(void* memory);

struct ScreenRectArray
{
    RendererDataRecord* m_pBase;
    unsigned int m_uiElementCount;
    unsigned int m_uiAllocatedCapacity;
    void NiTSet_NiScreenTexture_ScreenRect_NiTMallocInterface_NiScreenTexture_ScreenRect_Realloc(unsigned int uiNewSize);
};

// @fnv 0x00a95520
// src: XBOX_RETAIL_PDB 0x00a95520
// pdb: NiTSet<NiScreenTexture::ScreenRect,NiTMallocInterface<NiScreenTexture::ScreenRect> >::Realloc | NiScreenTexture.obj | retail, call graph, high
// pdb params: uiNewSize
// pdb locals: pNewBase, i
void ScreenRectArray::NiTSet_NiScreenTexture_ScreenRect_NiTMallocInterface_NiScreenTexture_ScreenRect_Realloc(unsigned int uiNewSize)
{
    if (uiNewSize != m_uiElementCount)
    {
        RendererDataRecord* pNew;
        if (uiNewSize > 0)
        {
            pNew = static_cast<RendererDataRecord*>(_NiMalloc_00aa1070(uiNewSize * sizeof(RendererDataRecord)));
            for (unsigned int i = 0; i < m_uiAllocatedCapacity; ++i)
            {
                pNew[i] = m_pBase[i];
            }
        }
        else
        {
            pNew = 0;
        }
        _NiFree_00aa10f0(m_pBase);
        m_pBase = pNew;
        m_uiElementCount = uiNewSize;
    }
}

extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(volatile long*);
struct RectCopy
{
    short top, left;
    unsigned short width, height, texTop, texLeft;
    NiColorA color;
    RectCopy(const RectCopy& rhs)
        : top(rhs.top), left(rhs.left), width(rhs.width), height(rhs.height), texTop(rhs.texTop), texLeft(rhs.texLeft),
          color(rhs.color)
    {}
};
struct RectArray
{
    RectCopy* base;
    unsigned int capacity, used;
    void NiTSet_NiScreenTexture_ScreenRect_NiTMallocInterface_NiScreenTexture_ScreenRect_Realloc(unsigned int);
    void Add(const RectCopy& rect)
    {
        if (used == capacity)
            NiTSet_NiScreenTexture_ScreenRect_NiTMallocInterface_NiScreenTexture_ScreenRect_Realloc(capacity > 0 ? capacity * 2 : 1);
        base[used] = rect;
        ++used;
    }
};
// The shared class has no declaration for this nonvirtual copy helper.
struct ScreenCopy : NiScreenTexture
{
    void NiScreenTexture_CopyMembers(ScreenCopy* pkDest, void* kCloning);
};
// @fnv 0x00a95760
// src: XBOX_RETAIL_PDB 0x00a95760
// pdb: NiScreenTexture::CopyMembers | NiScreenTexture.obj | retail, object-file gap fill, high
// pdb params: pkDest, kCloning
// pdb locals: i, kNewRect, kRect
void ScreenCopy::NiScreenTexture_CopyMembers(ScreenCopy* pkDest, void* kCloning)
{
    NiObject::CreateVertexBufferSet(pkDest, *static_cast<CloningProcess*>(kCloning));
    RectArray& rects = *reinterpret_cast<RectArray*>(&m_kScreenRects);
    for (unsigned int i = 0; i < rects.used; i++)
    {
        RectCopy rect(rects.base[i]);
        reinterpret_cast<RectArray*>(&pkDest->m_kScreenRects)->Add(rect);
    }
    if (pkDest->m_spTexProp != m_spTexProp)
    {
        NiRefObject* pOldProp = pkDest->m_spTexProp;
        if (pOldProp && InterlockedDecrement(reinterpret_cast<volatile long*>(&pOldProp->m_uiRefCount)) == 0)
            pOldProp->DeleteThis();
        pkDest->m_spTexProp = m_spTexProp;
        if (pkDest->m_spTexProp)
            InterlockedIncrement(reinterpret_cast<volatile long*>(&pkDest->m_spTexProp->m_uiRefCount));
    }
    pkDest->m_usDirtyFlags = m_usDirtyFlags;
}

void* __cdecl Object_DispatchCommandFour(unsigned int size);

// Initialize the base explicitly; NiScreenTexture has no shared constructor declaration.
static inline void InitScreenTextureBase(NiScreenTexture* p) throw()
{
    ::new (p) NiObject;
    *reinterpret_cast<void**>(p) = &DAT_010a048c;
}

// @fnv 0x00a958b0
// src: EXE 0x00a958b0
// pdb: NiScreenTexture::CreateObject | NiScreenTexture.obj | retail, upstream name already matches, high
// pdb locals: pkObject
NiScreenTexture* __cdecl NiScreenTexture_CreateObject()
{
    NiScreenTexture* p = (NiScreenTexture*)Object_DispatchCommandFour(sizeof(NiScreenTexture));
    if (p != 0)
    {
        ::new (p) NiObject;
        *reinterpret_cast<void* volatile*>(p) = (void*)&DAT_010a048c;
        *(RendererDataRecord* volatile*)&p->m_kScreenRects = 0;
        *(volatile unsigned int*)&p->m_uiElementCount = 0;
        *(volatile unsigned int*)&p->m_uiAllocatedCapacity = 0;
        *(NiTexturingProperty* volatile*)&p->m_spTexProp = 0;
        *(void* volatile*)&p->m_pkBuffData = 0;
        p->m_usDirtyFlags = 0;
    }
    else
    {
        p = 0;
    }
    return p;
}

// Links the stream's texturing property, releasing the old reference and acquiring the new one.
// @fnv 0x00a95600
// pdb: NiScreenTexture::LinkObject | NiScreenTexture.obj | retail, upstream name already matches, high
// pdb params: kStream
void NiScreenTexture::LinkObject(NiStream& kStream)
{
    NiObject::LinkObject(kStream);

    NiTexturingProperty* pkProp = (NiTexturingProperty*)kStream.GetLoadedObject();
    if (m_spTexProp != pkProp)
    {
        NiTexturingProperty* old = m_spTexProp;
        if (old)
        {
            if (InterlockedDecrement(reinterpret_cast<volatile long*>(&old->m_uiRefCount)) == 0)
                old->DeleteThis();
        }
        m_spTexProp = pkProp;
        if (m_spTexProp)
            InterlockedIncrement(reinterpret_cast<volatile long*>(&m_spTexProp->m_uiRefCount));
    }
}



template <class T> inline void NiBinaryStreamLoad(NiBinaryStream& binstream, T* pValue, unsigned int uiNumEls = 1)
{
    unsigned int uiSize = sizeof(T);
    binstream.Read(pValue, uiNumEls * sizeof(T), &uiSize, uiNumEls);
}


template <class T> inline void ReadBinaryValue(NiStream& kStream, T& value)
{
    NiBinaryStreamLoad(**reinterpret_cast<NiBinaryStream**>(reinterpret_cast<char*>(&kStream) + 0x24C), &value, 1);
}

struct ReadScreenRect
{
    short pixTop;
    short pixLeft;
    unsigned short pixWidth;
    unsigned short pixHeight;
    unsigned short texTop;
    unsigned short texLeft;
    NiColorA color;
};
struct ReadScreenRectArray
{
    ReadScreenRect* buffer;
    unsigned int capacity;
    unsigned int used;
    void NiTSet_NiScreenTexture_ScreenRect_NiTMallocInterface_NiScreenTexture_ScreenRect_Realloc(unsigned int uiNewSize);
};
// Reads a count and that many screen rects from the stream, appending each to rects and growing the array as needed.
// @fnv 0x00a958f0
// pdb: NiScreenTexture::LoadBinary | NiScreenTexture.obj | retail, upstream name already matches, high
// pdb params: kStream
// pdb locals: uiNumRects, i, kRect
void NiScreenTexture::LoadBinary(NiStream& kStream)
{
    NiObject::LoadBinary(kStream);
    unsigned int count;
    ReadBinaryValue(kStream, count);
    for (unsigned int i = 0; i < count; ++i)
    {
        ReadScreenRectArray& arr = *reinterpret_cast<ReadScreenRectArray*>(&m_kScreenRects);
        ReadScreenRect rect;
        ReadBinaryValue(kStream, rect.pixTop);
        ReadBinaryValue(kStream, rect.pixLeft);
        ReadBinaryValue(kStream, rect.pixWidth);
        ReadBinaryValue(kStream, rect.pixHeight);
        ReadBinaryValue(kStream, rect.texTop);
        ReadBinaryValue(kStream, rect.texLeft);
        ((void (__thiscall*)(NiColorA*, NiStream&))NiColorA_LoadBinary)(&rect.color, kStream);
        if (arr.used == arr.capacity)
        {
            unsigned int newSize = arr.capacity > 0 ? arr.capacity * 2 : 1;
            arr.NiTSet_NiScreenTexture_ScreenRect_NiTMallocInterface_NiScreenTexture_ScreenRect_Realloc(newSize);
        }
        arr.buffer[arr.used] = rect;
        ++arr.used;
    }
    kStream.ReadLinkIDStub();
}

struct S_00a95660_Rect
{
    short a, b;
    unsigned short c, d, e, f;
    NiColorA col;
};
// Checks base equality and capacity, examines rectangle fields and colors, then returns the texturing-property comparison.
// @fnv 0x00a95660
// pdb: NiScreenTexture::IsEqual | NiScreenTexture.obj | retail, upstream name already matches, high
// pdb params: pkObject
// pdb locals: pkTexture, uiNumRects, i, kOtherRect, kRect
bool NiScreenTexture::IsEqual(NiObject* pkObject) const
{
    NiScreenTexture* apOther = static_cast<NiScreenTexture*>(pkObject);
    if (!const_cast<NiScreenTexture*>(this)->NiObject::IsSameAs(apOther))
        return false;
    unsigned int n = m_uiAllocatedCapacity;
    if (n != apOther->m_uiAllocatedCapacity)
        return false;
    for (unsigned int i = 0; i < n; i++)
    {
        S_00a95660_Rect& refSelf = reinterpret_cast<S_00a95660_Rect*>(m_kScreenRects)[i];
        S_00a95660_Rect& refOther = reinterpret_cast<S_00a95660_Rect*>(apOther->m_kScreenRects)[i];
        if (refSelf.a != refOther.a && refSelf.b != refOther.b && refSelf.c != refOther.c && refSelf.d != refOther.d &&
            refSelf.e != refOther.e && refSelf.f != refOther.f)
        {
            if (refSelf.col != refOther.col)
                return false;
        }
    }
    NiTexturingProperty* pSelfProp = m_spTexProp;
    NiTexturingProperty* pOtherProp = apOther->m_spTexProp;
    typedef unsigned char (__thiscall* CompareProperty)(NiTexturingProperty*, NiTexturingProperty*);
    return reinterpret_cast<CompareProperty>((*reinterpret_cast<void***>(pSelfProp))[23])(pSelfProp, pOtherProp);
}

__declspec(noinline) bool NiColorA::operator!=(const NiColorA& other) const
{
    return !(r == other.r && g == other.g && b == other.b && a == other.a);
}

static inline void InitRectArray(unsigned int* p) throw()
{
    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
}

static inline unsigned int* InitTexProp(unsigned int* p) throw()
{
    *p = 0;
    return p;
}

// Allocates a screen texture, copies the source into it, and returns the new object.
// @fnv 0x00a95c10
// pdb: NiScreenTexture::CreateClone | NiScreenTexture.obj | retail, object-file run extension, low
// pdb params: kCloning
// pdb locals: pkObject
NiObject* NiScreenTexture::MakeClone(CloningProcess& cloning)
{
    NiScreenTexture* object = static_cast<NiScreenTexture*>(Object_DispatchCommandFour(sizeof(NiScreenTexture)));
    if (object)
    {
        InitScreenTextureBase(object);
        InitRectArray(reinterpret_cast<unsigned int*>(&object->m_kScreenRects));
        InitTexProp(reinterpret_cast<unsigned int*>(&object->m_spTexProp));
        object->m_usDirtyFlags = 0;
        object->m_pkBuffData = 0;
    }
    else
    {
        object = 0;
    }
    reinterpret_cast<ScreenCopy*>(this)->NiScreenTexture_CopyMembers(reinterpret_cast<ScreenCopy*>(object), &cloning);
    return object;
}
