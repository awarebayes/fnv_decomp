
// src/gamebryo2.2/corelibs/nimain/nipalette.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nimain/nipalette.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/BSFadeNode.h"
#include "../../../../include/NiPalette.h"

extern "C" void* NiPalette_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011f5c08 (old: g_011f5c08)

// @fnv 0x00a8b070
// src: COMMUNITY 0x00a8b070
void* __cdecl NiPalette_GetRTTI()
{
    return &NiPalette_ms_RTTI;
}

extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void*);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void*);
extern NiPalette* DAT_011f5c00;
extern NiPalette* DAT_011f5c04;
extern int DAT_011f5c80;
// Under a critical section, removes this node from the doubly linked list, updating the head, tail and neighbouring links.
// @fnv 0x00a8b010
// pdb: NiPalette::RemovePaletteFromList | NiPalette.obj | retail, object-file run extension, low
void __fastcall UnlinkPalette(NiPalette* self)
{
    EnterCriticalSection(&DAT_011f5c80);
    if (DAT_011f5c00 == self)
        DAT_011f5c00 = reinterpret_cast<NiPalette*>(self->m_pkNext);
    if (DAT_011f5c04 == self)
        DAT_011f5c04 = reinterpret_cast<NiPalette*>(self->m_pkPrev);
    if (reinterpret_cast<NiPalette*>(self->m_pkPrev))
        reinterpret_cast<NiPalette*>(self->m_pkPrev)->m_pkNext = self->m_pkNext;
    if (reinterpret_cast<NiPalette*>(self->m_pkNext))
        reinterpret_cast<NiPalette*>(self->m_pkNext)->m_pkPrev = self->m_pkPrev;
    LeaveCriticalSection(&DAT_011f5c80);
}

extern int DAT_0109f3fc;
void __fastcall _Call_a5d3a0(void*);
void __fastcall NiPalette_AddPaletteToList(void*);
// @fnv 0x00a8b220
// src: EXE 0x00a8b220
// pdb: NiPalette::CreateObject | NiPalette.obj | retail, upstream name already matches, high
// pdb locals: pkObject
void* NiPalette_CreateObject()
{
    NiPalette* p = (NiPalette*)BSFadeNode::operator new(0x24);
    if (p)
    {
        _Call_a5d3a0(p);
        *(void**)p = (void*)&DAT_0109f3fc;
        *(unsigned char*)&p->m_bAlpha = 0;
        p->m_uiEntries = 0;
        p->m_pkPalette = 0;
        p->m_uiRevID = 1;
        p->m_pkRendererData = 0;
        NiPalette_AddPaletteToList(p);
        return p;
    }
    return 0;
}

// Exe 0x00a8b270: unit-local RendererData view; declarations and layout are preserved.
struct NipaletteRendererDataView
{
    virtual ~NipaletteRendererDataView();
};
struct MM
{
    void _DeallocateAlias(void*);
};
extern MM g_memoryManager;
extern int DAT_0109f3fc;
void _delete(void*, unsigned int);
// @fnv 0x00a8b270
// pdb: NiPalette::`scalar deleting destructor' | NiPalette.obj | retail, object-file gap fill, high
void* NiPalette::DestroyPalette(unsigned int auiFlags)
{
    *(void**)this = (void*)&DAT_0109f3fc;
    g_memoryManager._DeallocateAlias((void*)m_pkPalette);
    delete (NipaletteRendererDataView*)m_pkRendererData;
    UnlinkPalette(this);
    this->NiRefObject::~NiRefObject();
    if (auiFlags & 1)
        _delete(this, 0x24);
    return this;
}

template <class T> struct PaletteStringAllocator
{
};
template <class T, class A> struct PaletteStringArray
{
    void SetSize(unsigned int);
    void SetAt(unsigned int, T&);
};
void __stdcall AppendCameraPathEntries(PaletteStringArray<char*, PaletteStringAllocator<char*> >*);
char* __cdecl _FUN_00a7d330(void*);
// @fnv 0x00a8b460
// src: XBOX_PDB 0x00a8b460
// pdb: NiPalette::GetViewerStrings | NiPalette.obj | retail, upstream name already matches, high
// pdb params: pkStrings
void __stdcall NiPalette_GetViewerStrings(PaletteStringArray<char*, PaletteStringAllocator<char*> >* pkStrings)
{
    AppendCameraPathEntries(pkStrings);
    char* pszString = _FUN_00a7d330(NiPalette_ms_RTTI);
    unsigned int uiSize = *(unsigned short*)((char*)pkStrings + 10);
    unsigned int uiCapacity = *(unsigned short*)((char*)pkStrings + 8);
    if (uiSize >= uiCapacity)
    {
        unsigned int uiGrow = *(unsigned short*)((char*)pkStrings + 14);
        pkStrings->SetSize(uiGrow + uiSize);
    }
    pkStrings->SetAt(uiSize, pszString);
}


// @fnv 0x00a8af40
// pdb: NiPalette::IsEqual | NiPalette.obj | retail, object-file run extension, low
// pdb params: pkObject
// pdb locals: pkPalette
unsigned char __fastcall PaletteEquals(NiPalette* self, void*, NiPalette* apOther)
{
    if (self->NiObject::IsEqual(apOther) == 0)
    {
        return 0;
    }
    if (*(unsigned char*)&self->m_bAlpha != *(unsigned char*)&apOther->m_bAlpha)
    {
        return 0;
    }
    if (self->m_uiEntries != apOther->m_uiEntries)
    {
        return 0;
    }
    unsigned int uiByteCount = self->m_uiEntries;
    uiByteCount += uiByteCount;
    uiByteCount += uiByteCount;
    unsigned int n = uiByteCount != 0;
    unsigned char* pOtherData = (unsigned char*)apOther->m_pkPalette;
    unsigned char* pThisData = (unsigned char*)self->m_pkPalette;
    while (n >= 4)
    {
        if (*(unsigned int*)pThisData != *(unsigned int*)pOtherData)
        {
            return 0;
        }
        n -= 4;
        pOtherData += 4;
        pThisData += 4;
    }
    if (n != 0)
    {
        if (pOtherData[0] != pThisData[0])
            return 0;
        if (n > 1)
        {
            if (pOtherData[1] != pThisData[1])
                return 0;
            if (n > 2)
            {
                if (pOtherData[2] != pThisData[2])
                    return 0;
            }
        }
    }
    return 1;
}

// FUN_00a8b110: thiscall, 3 stack args (ret 0xc). Stores the byte and
// dword args, allocates a 4-byte element array of count arg2 through the
// global allocator, copies arg3 into it, then notifies the global listener.

struct A8b110Allocator
{
    void* Game_DoHeapAlloc(unsigned int size);  // allocates size bytes, callee pops
};
extern A8b110Allocator DAT_011f6238;

struct A8b110Elem
{
    static void* operator new[](size_t bytes)
    {
        return DAT_011f6238.Game_DoHeapAlloc(bytes);
    }
    int m_uiFilterValue;
};

struct A8b110Listener
{
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v0A();
    virtual void v0B();
    virtual void v0C();
    virtual void v0D();
    virtual void v0E();
    virtual void v0F();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v1A();
    virtual void v1B();
    virtual void v1C();
    virtual void v1D();
    virtual void v1E();
    virtual void v1F();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v2A();
    virtual void v2B();
    virtual void v2C();
    virtual void v2D();
    virtual void v2E();
    virtual void v2F();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v3A();
    virtual void v3B();
    virtual void v3C();
    virtual void v3D();
    virtual void v3E();
    virtual void v3F();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v4A();
    virtual void v4B();
    virtual void v4C();
    virtual void v4D();
    virtual void v4E();
    virtual void v4F();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54(void* p);
};
extern A8b110Listener* DAT_011f4748;
void __cdecl FUN_00ec44d0(void* dst, void* src, unsigned int size);


// Initializes the owner: stores the byte and count, allocates an element array, copies count*4 bytes from a3, and notifies DAT_011f4748.
// @fnv 0x00a8b110
// src: EXE 0x00a8b110
// pdb: NiPalette::NiPalette | NiPalette.obj | retail, call graph, high
void* __fastcall NiPalette_NiPalette_00a8b110(NiPalette* self, void*, unsigned char a1, unsigned int a2, void* pSrc)
{
    _Call_a5d3a0(self);
    *(unsigned char*)&self->m_bAlpha = a1;
    self->m_uiEntries = a2;
    *(void**)self = (void*)&DAT_0109f3fc;
    self->m_uiRevID = 1;
    self->m_pkPalette = (unsigned int)(new A8b110Elem[a2]);
    FUN_00ec44d0((void*)self->m_pkPalette, pSrc, self->m_uiEntries * 4);
    self->m_pkRendererData = 0;
    if (DAT_011f4748)
    {
        DAT_011f4748->v54(self);
    }
    NiPalette_AddPaletteToList(self);
    return self;
}

namespace fnv_00a8b2c0 {
struct BinaryReader
{
    void* unk00;
    unsigned int bytesRead;
    unsigned int(__cdecl* read)(BinaryReader*, void*, unsigned int, unsigned int*, unsigned int);
    void readBlock(void* output, unsigned int bytes, unsigned int unitValue)
    {
        unsigned int unit = unitValue;
        bytesRead += read(this, output, bytes, &unit, 1);
    }
};
struct DataSource
{
    BinaryReader* reader() const
    {
        return *reinterpret_cast<BinaryReader* const*>(reinterpret_cast<const char*>(this) + 0x24c);
    }
};
void __stdcall ReadGroup(DataSource*);
struct MMExtra
{
    void* _Allocate(unsigned int);
};
struct ReadState
{
    unsigned int elementCount;
};
struct MemoryBlock
{
    unsigned int value;
    MemoryBlock()
    {}
    static void* operator new[](size_t size)
    {
        return reinterpret_cast<MMExtra&>(g_memoryManager)._Allocate(size);
    }
};
struct Observer
{
    void** vtable;
};
extern Observer* DAT_011f4748;
// Reads an enabled flag and element count from the source's reader, reallocates values if the count changed, reads the values, then notifies the observer.
// @fnv 0x00a8b2c0
// pdb: NiPalette::LoadBinary | NiPalette.obj | retail, object-file gap fill, high
// pdb params: kStream
// pdb locals: bAlpha, uiEntries
void __fastcall ReadPaletteFromSource(NiPalette* self, void*, DataSource* source)
{
    ReadGroup(source);
    ReadState readState;

    unsigned char enabledByte;
    BinaryReader* reader = source->reader();
    reader->readBlock(&enabledByte, 1, 1);
    *(unsigned char*)&self->m_bAlpha = enabledByte != 0;

    reader = source->reader();
    reader->readBlock(&readState.elementCount, 4, 4);
    if (readState.elementCount != self->m_uiEntries)
    {
        g_memoryManager._DeallocateAlias((void*)self->m_pkPalette);
        self->m_uiEntries = readState.elementCount;
        self->m_pkPalette = (unsigned int)(new MemoryBlock[readState.elementCount]);
    }

    reader = source->reader();
    reader->readBlock((void*)self->m_pkPalette, self->m_uiEntries * sizeof(MemoryBlock), 1);
    Observer* observer = DAT_011f4748;
    if (observer)
    {
        typedef void(__thiscall * OnRead)(Observer*, NiPalette*);
        (reinterpret_cast<OnRead>(observer->vtable[0x150 / 4]))(observer, self);
    }
}
}  // namespace fnv_00a8b2c0

// NiObject-derived data SaveBinary: base data, a flag byte, the element count, then the raw element bytes.

class A8B3C0BinaryStream
{
  public:
    void* m_ppVirtualMethodTable;
    unsigned int m_uiStreamFlags;
    unsigned int(__cdecl* m_pfnRead)(A8B3C0BinaryStream*, void*, unsigned int, unsigned int*, unsigned int);
    unsigned int(__cdecl* m_pfnWrite)(A8B3C0BinaryStream*, const void*, unsigned int, unsigned int*, unsigned int);

    unsigned int BinaryWrite(const void* p, unsigned int n, unsigned int* sizes, unsigned int count)
    {
        unsigned int written = m_pfnWrite(this, p, n, sizes, count);
        m_uiStreamFlags += written;
        return written;
    }
    unsigned int Write(const void* p, unsigned int n, unsigned int uiComponentSize)
    {
        unsigned int size = uiComponentSize;
        return BinaryWrite(p, n, &size, 1);
    }
};

class A8B3C0Stream
{
  public:
    int unk000;
    int unk004;
    int unk008;
    int unk00C;
    int unk010;
    int unk014;
    int unk018;
    int unk01C;
    int unk020;
    int unk024;
    int unk028;
    int unk02C;
    int unk030;
    int unk034;
    int unk038;
    int unk03C;
    int unk040;
    int unk044;
    int unk048;
    int unk04C;
    int unk050;
    int unk054;
    int unk058;
    int unk05C;
    int unk060;
    int unk064;
    int unk068;
    int unk06C;
    int unk070;
    int unk074;
    int unk078;
    int unk07C;
    int unk080;
    int unk084;
    int unk088;
    int unk08C;
    int unk090;
    int unk094;
    int unk098;
    int unk09C;
    int unk0A0;
    int unk0A4;
    int unk0A8;
    int unk0AC;
    int unk0B0;
    int unk0B4;
    int unk0B8;
    int unk0BC;
    int unk0C0;
    int unk0C4;
    int unk0C8;
    int unk0CC;
    int unk0D0;
    int unk0D4;
    int unk0D8;
    int unk0DC;
    int unk0E0;
    int unk0E4;
    int unk0E8;
    int unk0EC;
    int unk0F0;
    int unk0F4;
    int unk0F8;
    int unk0FC;
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
    int unk160;
    int unk164;
    int unk168;
    int unk16C;
    int unk170;
    int unk174;
    int unk178;
    int unk17C;
    int unk180;
    int unk184;
    int unk188;
    int unk18C;
    int unk190;
    int unk194;
    int unk198;
    int unk19C;
    int unk1A0;
    int unk1A4;
    int unk1A8;
    int unk1AC;
    int unk1B0;
    int unk1B4;
    int unk1B8;
    int unk1BC;
    int unk1C0;
    int unk1C4;
    int unk1C8;
    int unk1CC;
    int unk1D0;
    int unk1D4;
    int unk1D8;
    int unk1DC;
    int unk1E0;
    int unk1E4;
    int unk1E8;
    int unk1EC;
    int unk1F0;
    int unk1F4;
    int unk1F8;
    int unk1FC;
    int unk200;
    int unk204;
    int unk208;
    int unk20C;
    int unk210;
    int unk214;
    int unk218;
    int unk21C;
    int unk220;
    int unk224;
    int unk228;
    int unk22C;
    int unk230;
    int unk234;
    int unk238;
    int unk23C;
    int unk240;
    int unk244;
    int unk248;
    int unk24C;
    A8B3C0BinaryStream* m_pBackingStream;
};

template <class T>
inline void A8B3C0BinaryStreamSave(A8B3C0BinaryStream& kOstr, const T* pValue, unsigned int uiNumEls = 1)
{
    unsigned int uiSize = sizeof(T);
    kOstr.BinaryWrite(pValue, uiNumEls * uiSize, &uiSize, 1);
}

template <class T> inline void A8B3C0SaveBinary(A8B3C0Stream& kStream, const T& value)
{
    A8B3C0BinaryStreamSave(*kStream.m_pBackingStream, &value);
}

template <class T> inline void A8B3C0SaveBinary(A8B3C0Stream& kStream, const T* pValues, unsigned int ItemUpdateArray)
{
    A8B3C0BinaryStreamSave(*kStream.m_pBackingStream, pValues, ItemUpdateArray);
}



// @fnv 0x00a8b3c0
// src: XBOX_PDB 0x00a8b3c0
// pdb: NiPalette::SaveBinary | NiPalette.obj | retail, upstream name already matches, high
// pdb params: kStream
void __fastcall NiPalette_SaveBinary(NiPalette* self, void*, A8B3C0Stream& kStream)
{
    self->NiObject::SaveBinary(reinterpret_cast<NiStream&>(kStream));
    A8B3C0SaveBinary<unsigned char>(kStream, *reinterpret_cast<const bool*>(&self->m_bAlpha));
    A8B3C0SaveBinary(kStream, self->m_uiEntries);
    A8B3C0SaveBinary(kStream, (unsigned char*)self->m_pkPalette, self->m_uiEntries * 4);
}

struct PaletteElement : A8b110Elem
{
    PaletteElement()
    {}
};

// Copies m_08, m_0C and a deep copy of the m_14 element array from src, then calls FUN_00a8afc0.
// @fnv 0x00a8b080
// src: EXE 0x00a8b080
// pdb: NiPalette::NiPalette | NiPalette.obj | retail, object-file run extension, low
void* __fastcall NiPalette_NiPalette(NiPalette* self, void*, const NiPalette& src)
{
    _Call_a5d3a0(self);
    *(void**)self = &DAT_0109f3fc;
    *(unsigned char*)&self->m_bAlpha = *(const unsigned char*)&src.m_bAlpha;
    self->m_uiEntries = src.m_uiEntries;
    self->m_uiRevID = 1;
    self->m_pkPalette = (unsigned int)(new PaletteElement[self->m_uiEntries]);
    FUN_00ec44d0((void*)self->m_pkPalette, (void*)src.m_pkPalette, src.m_uiEntries * 4);
    self->m_pkRendererData = 0;
    if (DAT_011f4748)
        DAT_011f4748->v54(self);
    NiPalette_AddPaletteToList(self);
    return self;
}

#include <string.h>
struct MMAllocInterface
{
    void* Game_DoHeapAlloc(unsigned int);
};
struct ArrayElement
{
    unsigned int value;
    static void* operator new[](size_t bytes)
    {
        return ((MMAllocInterface*)&g_memoryManager)->Game_DoHeapAlloc(bytes);
    }
};
// Copies the f08 scalar and the f14/f0C integer array from rhs, reallocating the array only when the element count differs.
// @fnv 0x00a8b190
// src: COMMUNITY 0x00a8b190
NiPalette* __fastcall NiPalette_Replace(NiPalette* self, void*, const NiPalette* rhs)
{
    if (self != rhs)
    {
        if (self->m_pkRendererData)
        {
            delete reinterpret_cast<NipaletteRendererDataView*>(self->m_pkRendererData);
            self->m_pkRendererData = 0;
        }
        *(unsigned char*)&self->m_bAlpha = *(const unsigned char*)&rhs->m_bAlpha;
        self->m_uiRevID = 1;
        if (self->m_uiEntries != rhs->m_uiEntries)
        {
            g_memoryManager._DeallocateAlias((void*)self->m_pkPalette);
            unsigned int count = (unsigned int)rhs->m_uiEntries;
            self->m_uiEntries = count;
            self->m_pkPalette = (unsigned int)(new ArrayElement[count]);
        }
        memcpy((void*)self->m_pkPalette, (void*)rhs->m_pkPalette, (unsigned int)rhs->m_uiEntries * sizeof(int));
    }
    return self;
}

// Appends the given node to the end of a doubly-linked list guarded by a critical section, updating the head and tail pointers.
// @fnv 0x00a8afc0
// src: COMMUNITY 0x00a8afc0
// pdb: NiPalette::AddPaletteToList | NiPalette.obj | retail, upstream name already matches, high
void __fastcall NiPalette_AddPaletteToList(void* apNode)
{
    NiPalette* node = static_cast<NiPalette*>(apNode);
    EnterCriticalSection(&DAT_011f5c80);
    if (DAT_011f5c00 == 0)
        DAT_011f5c00 = node;
    if (DAT_011f5c04 != 0)
    {
        DAT_011f5c04->m_pkNext = (unsigned int)node;
        node->m_pkPrev = (unsigned int)DAT_011f5c04;
    }
    else
    {
        node->m_pkPrev = 0;
    }
    DAT_011f5c04 = node;
    node->m_pkNext = 0;
    LeaveCriticalSection(&DAT_011f5c80);
}
