#include "../../../../../include/hkArray.h"
#include "../../../../../include/hkMemoryAllocator.h"
#include "../../../../../include/hkMemoryRouter.h"



// src/havok/common/serialize/util/hknativepackfileutils.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/projects/fallout/code/havok/source/common/serialize/util/hknativepackfileutils.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

extern "C" void __cdecl ProcessElements(int, int, int);

struct FUN_00dc0ed0_class
{
    void ProcessElements_Thunk(int a, int b);
};

// @fnv 0x00dc0ed0
// src: GUESS 0x00dc0ed0 (confidence: medium)
void FUN_00dc0ed0_class::ProcessElements_Thunk(int aiFirst, int aiSecond)
{
    ProcessElements(*(int*)((char*)this + 8), aiSecond, aiFirst);
}

void __cdecl hkNativePackfileUtils_unload(void*, void*);

struct FUN_00dc0ef0_class
{
    void* m_00;
    void* m_04;
    void* m_08;
    void* m_0c;
    int m_10;
    int m_14;
    int m_18;
    void UpdateState_00dc0ef0();
};

// If m_14 is set, calls FUN_00dc0de0 on m_08 and m_0c, then clears m_14 and m_18.
// @fnv 0x00dc0ef0
// src: GUESS 0x00dc0ef0 (confidence: low)
// pdb: `anonymous namespace'::hkNativeResource::callDestructors | hkNativePackfileUtils.obj | retail, vtable slot, high
void FUN_00dc0ef0_class::UpdateState_00dc0ef0()
{
    if (m_14)
    {
        hkNativePackfileUtils_unload(m_08, m_0c);
        m_14 = 0;
        m_18 = 0;
    }
}

struct FUN_00dc08f0_Element
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int dataOffset;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    void IsRendererActive(void* data, int arg);
    void SecondPass(void* data, int arg);
};

struct FUN_00dc08f0_Header
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    FUN_00dc08f0_Element* elements;
    int count;
};

// @fnv 0x00dc08f0
// pdb: hkNativePackfileUtils::getImportsExports | hkNativePackfileUtils.obj | retail, object-file run extension, low
// pdb params: loadedBuffer, expOut, impOut
extern "C" void __cdecl ProcessElements(int header, int first, int second)
{
    FUN_00dc08f0_Header* pHeader = (FUN_00dc08f0_Header*)header;
    for (int i = 0; i < pHeader->count; i++)
    {
        FUN_00dc08f0_Element* pElement = &pHeader->elements[i];
        void* data = (char*)pHeader + pElement->dataOffset;
        pElement->IsRendererActive(data, first);
        pElement->SecondPass(data, second);
    }
}

hkMemoryRouter* hkMemoryRouter_getInstance();

inline hkMemoryAllocator& GetHeap(hkMemoryRouter* holder)
{
    return *reinterpret_cast<hkMemoryAllocator*>(holder->m_heap);
}

inline hkMemoryRouter& GetMonitorStream() throw()
{
    return *hkMemoryRouter_getInstance();
}

struct S_00dc0de0_Value
{
    int unk00;
    int unk04;
    void (*m_callback)(unsigned int key);
};

struct S_00dc0de0_Pair
{
    unsigned int key;
    S_00dc0de0_Value* val;
};

struct S_00dc0de0_Map
{
    S_00dc0de0_Pair* m_kPrimitiveElement;
    int m_iCapacityFlags;
    int m_iPairCount;

    int GetFirstIterator() const
    {
        int i;
        for (i = 0; i <= m_iPairCount; i++)
            if (m_kPrimitiveElement[i].key != (unsigned int)-1)
                break;
        return i;
    }
    int getNext(int it) const
    {
        int i;
        for (i = it + 1; i <= m_iPairCount; i++)
            if (m_kPrimitiveElement[i].key != (unsigned int)-1)
                break;
        return i;
    }
    bool isValid(int it) const
    {
        return it <= m_iPairCount;
    }
    unsigned int GetIteratorKey(int it) const
    {
        return m_kPrimitiveElement[it].key;
    }
    S_00dc0de0_Value* getValue(int it) const
    {
        return m_kPrimitiveElement[it].val;
    }
    void ClearAndRelease(hkMemoryAllocator& alloc);
    void Object_DoNothing();
};

struct S_00dc0de0_Elem
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
};

typedef hkArrayBase<S_00dc0de0_Elem> S_00dc0de0_Array;
template<> __forceinline void hkArrayBase<S_00dc0de0_Elem>::_clearAndDeallocate(hkMemoryAllocator& sharedAlloc)
{
hkMemoryAllocator& alloc = reinterpret_cast<hkMemoryAllocator&>(sharedAlloc);

        m_iElementCount = 0;
        if ((m_iCapacityState & 0x80000000) == 0)
            alloc.bufFree(m_data, (int)((m_iCapacityState & 0x3fffffff) * sizeof(S_00dc0de0_Elem)));
        m_data = 0;
        m_iCapacityState = 0x80000000;
    
}


struct S_00dc0de0
{
    int m_00;
    int unk04;
    S_00dc0de0_Map m_map;
    S_00dc0de0_Array m_ppData;
};

// @fnv 0x00dc0de0
// src: XBOX_PDB 0x00dc0de0
// pdb: hkNativePackfileUtils::unload | hkNativePackfileUtils.obj | retail, upstream name already matches, high
// pdb params: buffer, bufferSize
// pdb locals: assertBuf, ostr
void hkNativePackfileUtils_unload(S_00dc0de0* s)
{
    s->m_00 = 0;
    S_00dc0de0_Map& map = s->m_map;
    for (int it = map.GetFirstIterator(); map.isValid(it); it = map.getNext(it))
    {
        S_00dc0de0_Value* pValue = map.getValue(it);
        if (pValue->m_callback)
            pValue->m_callback(map.GetIteratorKey(it));
    }
    s->m_ppData._clearAndDeallocate(GetHeap(hkMemoryRouter_getInstance()));
    map.ClearAndRelease(GetHeap(hkMemoryRouter_getInstance()));
    map.Object_DoNothing();
}

extern char DAT_010d6ab4[];

// @fnv 0x00dc0eb0
// src: GUESS 0x00dc0eb0 (confidence: low)
void* __cdecl Hknativepackfileutils_GetDAT010d6ab4()
{
    return DAT_010d6ab4;
}

struct FUN_00dc0ec0_cls
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int GetUnk14_00dc0ec0(int first, int second);
};
// @fnv 0x00dc0ec0
// src: GUESS 0x00dc0ec0 (confidence: low)
int FUN_00dc0ec0_cls::GetUnk14_00dc0ec0(int first, int second)
{
    return unk14;
}

extern "C" int __cdecl hkPackfile_ComputeNativeStorageSize(void*, void*) throw();
extern "C" void __cdecl hkArrayUtil__reserve(void*, void*, int, int) throw();
extern "C" void* __cdecl UnpackPackfile(void*, void*, void*, int, void*) throw();
extern "C" void* DAT_010d6a94;

struct S_00dc0cd0_Map
{
    void* LookupHashEntry(void* key, int defVal) throw();
};

struct S_00dc0cd0_Base0
{
    int unk00;
    int unk04;
};

struct S_00dc0cd0_Data : public S_00dc0cd0_Base0, public S_00dc0cd0_Map
{
};

struct S_00dc0cd0_MemberArray
{
    void* m_data;
    int m_iElementCount;
    int m_iCapacityState;

    __forceinline void init() throw()
    {
        m_data = 0;
        m_iElementCount = 0;
        m_iCapacityState = 0x80000000;
    }

    void SwapThreeInts(void* other) throw();
};

struct S_00dc0cd0_ArrayLocalForm
{ // local form of hkArray: 0x00dc0cd0 needs the original inline/out-of-line helper split; shared replacement changes bytes.
    S_00dc0cd0_Data* m_data;
    int m_iElementCount;
    int m_iCapacityState;

    S_00dc0cd0_ArrayLocalForm() throw()
    {
        m_data = 0;
        m_iElementCount = 0;
        m_iCapacityState = 0x80000000;
    }

    void GrowArrayCapacity(hkMemoryAllocator& alloc, int n) throw()
    {
        int cap = m_iCapacityState & 0x3fffffff;
        if (cap < n)
        {
            int newCap = 2 * cap;
            if (n >= newCap)
                newCap = n;
            hkArrayUtil__reserve(&alloc, this, newCap, 1);
        }
    }

    void _setSize(hkMemoryAllocator& alloc, int n) throw()
    {
        if ((m_iCapacityState & 0x3fffffff) < n)
        {
            GrowArrayCapacity(alloc, n);
        }
        m_iElementCount = n;
    }

    void SetSizeAndGrow(int n) throw()
    {
        _setSize(GetHeap(&GetMonitorStream()), n);
    }

    ~S_00dc0cd0_ArrayLocalForm() throw()
    {
        hkMemoryAllocator& alloc = GetHeap(&GetMonitorStream());
        m_iElementCount = 0;
        int cap = m_iCapacityState;
        if ((cap & 0x80000000) == 0)
        {
            alloc.bufFree(m_data, (unsigned int)(cap & 0x3fffffff));
        }
    }
};

struct S_00dc0cd0_Resource
{
    void* m_00;
    short m_04;
    short m_06;
    S_00dc0cd0_MemberArray m_ppData;
    void* m_14;
    void* m_18;
};

// @fnv 0x00dc0cd0
// src: XBOX_RETAIL_PDB 0x00dc0cd0
// pdb: hkNativePackfileUtils::load | hkNativePackfileUtils.obj | retail, call graph (shared callers), high
// pdb params: packfileData, packfileSize, outBuffer, outBufferSize, userRegistry
// pdb locals: inSections, assertBuf, ostr
void* __cdecl hkNativePackfileUtils_load(void* packfileData, void* dataSize, void* userRegistry) throw()
{
    S_00dc0cd0_ArrayLocalForm tempArray;
    int needed = hkPackfile_ComputeNativeStorageSize(packfileData, dataSize);
    tempArray.SetSizeAndGrow(needed);
    void* pKey = UnpackPackfile(packfileData, dataSize, tempArray.m_data, needed, userRegistry);
    S_00dc0cd0_Map* map = (S_00dc0cd0_Map*)((char*)tempArray.m_data + 8);
    void* pFound = 0;
    void** ppValue = (void**)map->LookupHashEntry(pKey, 0);
    if (ppValue)
    {
        pFound = *ppValue;
    }
    S_00dc0cd0_Resource* pResource =
        (S_00dc0cd0_Resource*)GetHeap(&GetMonitorStream()).blockAlloc(0x1c);
    pResource->m_04 = 0x1c;
    pResource->m_06 = 1;
    pResource->m_00 = &DAT_010d6a94;
    pResource->m_ppData.init();
    pResource->m_14 = pKey;
    pResource->m_18 = pFound;
    pResource->m_ppData.SwapThreeInts(&tempArray);
    return pResource;
}

class BSAudio;
namespace fnv_00dc0f20 {
extern int DAT_010d6a94;
extern int DAT_0102e388;

void __cdecl hkNativePackfileUtils_unload(void*, void*);


struct FUN_00dc0f20_cls
{
    int m_vptr;
    int unk04;
    void* m_08;
    void* m_0c;
    unsigned int m_10;
    int m_14;
    int m_18;

    void anonymous_namespace_hkNativeResource_dtor();
};

// @fnv 0x00dc0f20
// src: XBOX_RETAIL_PDB 0x00dc0f20
// pdb: `anonymous namespace'::hkNativeResource::~hkNativeResource | hkNativePackfileUtils.obj | retail, object-file gap fill, high
void FUN_00dc0f20_cls::anonymous_namespace_hkNativeResource_dtor()
{
    m_vptr = (int)&DAT_010d6a94;
    if (m_14 != 0)
    {
        hkNativePackfileUtils_unload(m_08, m_0c);
        m_14 = 0;
        m_18 = 0;
    }
    hkMemoryRouter* pAudioHolder = hkMemoryRouter_getInstance();
    hkMemoryAllocator* pAllocator = reinterpret_cast<hkMemoryAllocator*>(pAudioHolder->m_heap);
    m_0c = 0;
    unsigned int uCapBits = m_10;
    if ((uCapBits & 0x80000000u) == 0)
    {
        pAllocator->bufFree(m_08, uCapBits & 0x3fffffffu);
    }
    m_08 = 0;
    m_10 = 0x80000000u;
    *(volatile int*)&m_vptr = (int)&DAT_0102e388;
}
}  // namespace fnv_00dc0f20

struct StructureLayoutRules
{
    unsigned char m_ucBytesInPointer;
    unsigned char m_bLittleEndian;
    unsigned char m_bReusePaddingOptimization;
    unsigned char m_bEmptyBaseClassOptimization;
};

extern StructureLayoutRules DAT_011b7b88;

void __cdecl hkString_memSet(void* p, int a, int b);
void* __cdecl hkVersionUtil_getCurrentVersion();
int __cdecl hkString_strCmp(const void* a, const void* b);

// pdb: class hkPackfileHeader, size 0x40, 11 members
struct hkPackfileHeader
{
    unsigned int m_magic; // src: XBOX_RETAIL_PDB +0x0 (old: m_magic0)
    unsigned int m_magic1;
    int m_userTag; // src: XBOX_RETAIL_PDB +0x8 (old: m_uiUserTag)
    int m_fileVersion; // src: XBOX_RETAIL_PDB +0xc (old: m_iFileVersion)
    unsigned char m_layoutRules; // src: XBOX_RETAIL_PDB +0x10 (old: m_layoutRules_0)
    unsigned char m_layoutRules_1;
    unsigned char m_layoutRules_2;
    unsigned char m_layoutRules_3;
    int m_numSections; // src: XBOX_RETAIL_PDB +0x14 (old: m_iNumSections)
    int m_contentsSectionIndex; // src: XBOX_RETAIL_PDB +0x18 (old: m_iContentsSectionIndex)
    int m_contentsSectionOffset; // src: XBOX_RETAIL_PDB +0x1c (old: m_iContentsSectionOffset)
    int m_contentsClassNameSectionIndex; // src: XBOX_RETAIL_PDB +0x20 (old: m_iContentsClassNameSectionIndex)
    int m_contentsClassNameSectionOffset; // src: XBOX_RETAIL_PDB +0x24 (old: m_iContentsClassNameSectionOffset)
    unsigned char m_contentsVersion; // src: XBOX_RETAIL_PDB +0x28 (old: m_contentsVersion_0)
    unsigned char unk29;
    unsigned char unk2a;
    unsigned char unk2b;
    unsigned int unk2c;
    unsigned int unk30;
    unsigned int unk34;
    int m_pad; // src: XBOX_RETAIL_PDB +0x38 (old: m_flags)
    int unk3c;

    hkPackfileHeader()
    {
        hkString_memSet(this, -1, sizeof(*this));
        m_magic = 0x57e0e057;
        m_magic1 = 0x10c0c010;
        m_contentsVersion = 0;
    }
};

// @fnv 0x00dc0640
// src: XBOX_RETAIL_PDB 0x00dc0640
// pdb: hkNativePackfileUtils::validatePackfileHeader | hkNativePackfileUtils.obj | retail, string-set bridge, high
// pdb params: packfileData, errOut
// pdb locals: magic
int __cdecl hkNativePackfileUtils_validatePackfileHeader(const hkPackfileHeader* packfileData, const char** errOut)
{
    if (!packfileData)
    {
        if (errOut)
            *errOut = "Pointer is null";
        return 1;
    }

    hkPackfileHeader defaultHeader;
    if (packfileData->m_magic != defaultHeader.m_magic)
        goto bad_magic;
    if (packfileData->m_magic1 != defaultHeader.m_magic1)
    {
    bad_magic:
        if (errOut)
            *errOut = "Missing packfile magic header. Is this from a binary file?";
        return 1;
    }

    const StructureLayoutRules& hostRules = DAT_011b7b88;
    if (packfileData->m_layoutRules != hostRules.m_ucBytesInPointer)
    {
        if (errOut)
            *errOut = "Trying to process a binary file with a different pointer size than this platform.";
        return 1;
    }
    if (packfileData->m_layoutRules_1 != hostRules.m_bLittleEndian)
    {
        if (errOut)
            *errOut = "Trying to process a binary file with a different endian than this platform.";
        return 1;
    }
    if (packfileData->m_layoutRules_2 != DAT_011b7b88.m_bReusePaddingOptimization)
    {
        if (errOut)
            *errOut = "Trying to process a binary file with a different padding optimization than this platform.";
        return 1;
    }
    if (packfileData->m_layoutRules_3 != DAT_011b7b88.m_bEmptyBaseClassOptimization)
    {
        if (errOut)
            *errOut =
                "Trying to process a binary file with a different empty base class optimization than this platform.";
        return 1;
    }
    if (((unsigned int)packfileData & 3) != 0)
    {
        if (errOut)
            *errOut = "Packfile data source needs to be 4 byte aligned";
        return 1;
    }
    if (packfileData->m_contentsVersion == 0xff)
    {
        if (errOut)
            *errOut = "Packfile file format is too old";
        return 1;
    }
    if (hkString_strCmp(&packfileData->m_contentsVersion, hkVersionUtil_getCurrentVersion()) != 0)
    {
        if (errOut)
            *errOut = "Packfile contents are not up to date";
        return 1;
    }

    return 0;
}

struct hkPackfileSectionHeaderN
{
    char m_acSectionTag[19];
    char m_cNullByte;
    int m_iAbsoluteDataStart;
    int m_iLocalFixupsOffset;
    int m_iGlobalFixupsOffset;
    int m_iVirtualFixupsOffset;
    int m_iExportsOffset;
    int m_iImportsOffset;
    int m_iEndOffset;
};

void __cdecl hkMemCpyN(void* dst, const void* src, int n);

inline void* operator new(size_t, void* p) throw()
{
    return p;
}

typedef hkArrayBase<hkPackfileSectionHeaderN> hkSectionArrayN;

struct hkTypeInfoRegistryN
{
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual const void* RegisterLoadedObject(void* obj, const char* className);
};

struct hkBuiltinTypeRegistryN
{
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual hkTypeInfoRegistryN* GetTypeInfoRegistry();
};

extern hkBuiltinTypeRegistryN* g_pkBuiltinTypeRegistryN;

struct hkLoadedObjectTrackerN
{
    int m_00;
    int m_04;
    int m_08;

    hkLoadedObjectTrackerN(void* buffer, int bufferSize) throw();
    void add(void* allocator, void* obj, const void* typeInfo) throw();
};

struct hkNativeHeaderN
{
    unsigned int m_kMagic;
    int m_iContentsVersion;
    hkLoadedObjectTrackerN m_pTracker;
    hkSectionArrayN m_sections;

    void* getSectionPointer(int sectionIndex, int offset)
    {
        return m_sections[sectionIndex].m_iLocalFixupsOffset != 0
                   ? (char*)this + m_sections[sectionIndex].m_iAbsoluteDataStart + offset
                   : 0;
    }
};

// Copies section data, applies local and global fixups, registers objects listed by virtual fixups, and returns the contents pointer.
// @fnv 0x00dc0940
// pdb: hkNativePackfileUtils::load | hkNativePackfileUtils.obj | retail, object-file run extension, low
// pdb params: packfileData, packfileSize, outBuffer, outBufferSize, userRegistry
// pdb locals: inSections, assertBuf, ostr
extern "C" void* __cdecl UnpackPackfile(void* packfileData, void* dataSize, void* outBuffer, int outBufferSize,
                                        void* userRegistry) throw()
{
    int iFixupIndex;
    const hkPackfileHeader* header = (const hkPackfileHeader*)packfileData;
    const hkPackfileSectionHeaderN* sectionHeaders =
        header->m_numSections > 0 ? (const hkPackfileSectionHeaderN*)(header + 1) : 0;
    hkNativeHeaderN* out = (hkNativeHeaderN*)outBuffer;

    out->m_kMagic = 0xd5109142;
    new (&out->m_sections)
        hkSectionArrayN((hkPackfileSectionHeaderN*)(out + 1), header->m_numSections, header->m_numSections);

    int dataOffset = sizeof(hkNativeHeaderN) + header->m_numSections * sizeof(hkPackfileSectionHeaderN);
    for (int i = 0; i < header->m_numSections; i++)
    {
        hkPackfileSectionHeaderN& dst = out->m_sections[i];
        const hkPackfileSectionHeaderN& src = sectionHeaders[i];

        hkMemCpyN(dst.m_acSectionTag, src.m_acSectionTag, sizeof(src.m_acSectionTag));
        dst.m_cNullByte = src.m_cNullByte;
        dst.m_iAbsoluteDataStart = dataOffset;
        dst.m_iLocalFixupsOffset = src.m_iLocalFixupsOffset;
        dst.m_iGlobalFixupsOffset = src.m_iLocalFixupsOffset;
        dst.m_iVirtualFixupsOffset = src.m_iLocalFixupsOffset;
        dst.m_iExportsOffset = src.m_iLocalFixupsOffset;
        dst.m_iImportsOffset = src.m_iImportsOffset - src.m_iExportsOffset + src.m_iLocalFixupsOffset;
        dst.m_iEndOffset = src.m_iEndOffset - src.m_iExportsOffset + src.m_iLocalFixupsOffset;

        const char* srcData = (const char*)packfileData + src.m_iAbsoluteDataStart;
        char* dstData = (char*)outBuffer + dataOffset;
        hkMemCpyN(dstData, srcData, src.m_iLocalFixupsOffset);
        hkMemCpyN(dstData + dst.m_iExportsOffset, srcData + src.m_iExportsOffset,
                  src.m_iImportsOffset - src.m_iExportsOffset);
        hkMemCpyN(dstData + dst.m_iImportsOffset, srcData + src.m_iImportsOffset,
                  src.m_iEndOffset - src.m_iImportsOffset);

        if (i == header->m_contentsSectionIndex)
            out->m_iContentsVersion = dataOffset + header->m_contentsSectionOffset;

        const int* localFixups = (const int*)(srcData + src.m_iLocalFixupsOffset);
        for (iFixupIndex = 0; iFixupIndex < (src.m_iGlobalFixupsOffset - src.m_iLocalFixupsOffset) / 4;
             iFixupIndex += 2)
        {
            int srcOffset = localFixups[iFixupIndex];
            if (srcOffset != -1)
                *(void**)(dstData + srcOffset) = dstData + localFixups[iFixupIndex + 1];
        }
        dataOffset += dst.m_iEndOffset;
    }

    for (int i = 0; i < header->m_numSections; i++)
    {
        const hkPackfileSectionHeaderN& src = sectionHeaders[i];
        char* dstData = (char*)outBuffer + out->m_sections[i].m_iAbsoluteDataStart;
        const int* globalFixups =
            (const int*)((const char*)packfileData + src.m_iAbsoluteDataStart + src.m_iGlobalFixupsOffset);
        for (iFixupIndex = 0; iFixupIndex < (src.m_iVirtualFixupsOffset - src.m_iGlobalFixupsOffset) / 4;
             iFixupIndex += 3)
        {
            int srcOffset = globalFixups[iFixupIndex];
            if (srcOffset != -1)
            {
                void* target = out->getSectionPointer(globalFixups[iFixupIndex + 1], globalFixups[iFixupIndex + 2]);
                *(void**)(dstData + srcOffset) = target;
            }
        }
    }

    new (&out->m_pTracker) hkLoadedObjectTrackerN((char*)outBuffer + dataOffset, outBufferSize - dataOffset);

    hkTypeInfoRegistryN* registry =
        userRegistry ? (hkTypeInfoRegistryN*)userRegistry : g_pkBuiltinTypeRegistryN->GetTypeInfoRegistry();

    for (int i = 0; i < header->m_numSections; i++)
    {
        char* dstData = (char*)outBuffer + out->m_sections[i].m_iAbsoluteDataStart;
        const hkPackfileSectionHeaderN& src = sectionHeaders[i];
        const int* virtualFixups =
            (const int*)((const char*)packfileData + src.m_iAbsoluteDataStart + src.m_iVirtualFixupsOffset);
        for (iFixupIndex = 0; iFixupIndex < (src.m_iExportsOffset - src.m_iVirtualFixupsOffset) / 4; iFixupIndex += 3)
        {
            int objOffset = virtualFixups[iFixupIndex];
            if (objOffset != -1)
            {
                const char* className =
                    (const char*)out->getSectionPointer(virtualFixups[iFixupIndex + 1], virtualFixups[iFixupIndex + 2]);
                void* obj = dstData + objOffset;
                const void* typeInfo = registry->RegisterLoadedObject(obj, className);
                if (typeInfo)
                {
                    hkMemoryAllocator& heap = GetHeap(hkMemoryRouter_getInstance());
                    out->m_pTracker.add(&heap, obj, typeInfo);
                }
            }
        }
    }
    return (char*)outBuffer + out->m_iContentsVersion;
}

struct Entry_dc07b0
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
};

struct Table_dc07b0
{
    Entry_dc07b0* entries;
};

// @fnv 0x00dc07b0
// src: GUESS 0x00dc07b0 (confidence: low)
static __declspec(noinline) int Hknativepackfileutils_EvaluateValue_00dc07b0(Table_dc07b0* apTable, int aiIndex, int a, int b)
{
    if (apTable->entries[aiIndex].unk18 != 0)
    {
        return apTable->entries[aiIndex].unk14 + a + b;
    }
    return 0;
}

volatile int g_idx_dc07b0;
volatile int g_a_dc07b0;
volatile int g_b_dc07b0;
Table_dc07b0* volatile g_t_dc07b0;
volatile int g_out_dc07b0;

void CallTarget_dc07b0()
{
    g_out_dc07b0 = Hknativepackfileutils_EvaluateValue_00dc07b0(g_t_dc07b0, g_idx_dc07b0, g_a_dc07b0, g_b_dc07b0);
}

extern "C" int __cdecl Hkmap_ProcessEntries_00cb17f0(int);

// pdb: class hkPackfileSectionHeader, size 0x30, 9 members
struct hkPackfileSectionHeader
{
    char m_sectionTag[19]; // src: XBOX_RETAIL_PDB +0x0 (old: m_acSectionTag)
    char m_nullByte; // src: XBOX_RETAIL_PDB +0x13 (old: m_cNullByte)
    int m_absoluteDataStart; // src: XBOX_RETAIL_PDB +0x14 (old: m_iAbsoluteDataStart)
    int m_localFixupsOffset; // src: XBOX_RETAIL_PDB +0x18 (old: m_iLocalFixupsOffset)
    int m_globalFixupsOffset; // src: XBOX_RETAIL_PDB +0x1c (old: m_iGlobalFixupsOffset)
    int m_virtualFixupsOffset; // src: XBOX_RETAIL_PDB +0x20 (old: m_iVirtualFixupsOffset)
    int m_exportsOffset; // src: XBOX_RETAIL_PDB +0x24 (old: m_iExportsOffset)
    int m_importsOffset; // src: XBOX_RETAIL_PDB +0x28 (old: m_iImportsOffset)
    int m_endOffset; // src: XBOX_RETAIL_PDB +0x2c (old: m_iEndOffset)

    int getDataSize() const
    {
        return m_localFixupsOffset;
    }
    int getVirtualFixupsSize() const
    {
        return m_exportsOffset - m_virtualFixupsOffset;
    }
    int getExportsSize() const
    {
        return m_importsOffset - m_exportsOffset;
    }
    int getImportsSize() const
    {
        return m_endOffset - m_importsOffset;
    }
};

// @fnv 0x00dc07d0
// src: GUESS 0x00dc07d0 (confidence: medium)
// pdb: hkNativePackfileUtils::getRequiredBufferSize | hkNativePackfileUtils.obj | retail, object-file run extension, low
// pdb params: packfileData, packfileSize
// pdb locals: assertBuf, ostr
extern "C" int __cdecl hkPackfile_ComputeNativeStorageSize(void* packfileData, void* packfileSize) throw()
{
    const hkPackfileHeader* header = (const hkPackfileHeader*)packfileData;
    int dataSize = 0;
    int numVirtualFixups = 0;
    const hkPackfileSectionHeader* sections =
        header->m_numSections > 0 ? (const hkPackfileSectionHeader*)(header + 1) : 0;
    for (int i = 0; i < header->m_numSections; i++)
    {
        numVirtualFixups += sections[i].getVirtualFixupsSize() / (3 * (int)sizeof(int));
        dataSize += sections[i].getDataSize() + sections[i].getExportsSize() + sections[i].getImportsSize();
    }
    int mapSize = Hkmap_ProcessEntries_00cb17f0(numVirtualFixups);
    return 0x20 + header->m_numSections * (int)sizeof(hkPackfileSectionHeader) + dataSize + mapSize;
}

namespace fnv_00dc0f80 {
hkMemoryRouter* hkMemoryRouter_getInstance();
struct FUN_00dc0f80
{
    unsigned short allocationSize;
    FUN_00dc0f80();
    virtual ~FUN_00dc0f80();
    void operator delete(void* ptr)
    {
        hkMemoryRouter* manager = hkMemoryRouter_getInstance();
        reinterpret_cast<hkMemoryAllocator*>(manager->m_heap)->blockFree(ptr, static_cast<FUN_00dc0f80*>(ptr)->allocationSize);
    }
};
// @fnv 0x00dc0f80
// pdb: `anonymous namespace'::hkNativeResource::`scalar deleting destructor' | hkNativePackfileUtils.obj | retail, vtable slot, high
FUN_00dc0f80::FUN_00dc0f80()
{}
}  // namespace fnv_00dc0f80
