#include "../../../../include/NiTArray.h"
// gamebryo2.2/corelibs/nimain/nidefaultavobjectpalette.cpp: NiDefaultAVObjectPalette, Map, PaletteOwner and more (18 functions)

#include "../../../../include/MemoryManager.h"
#include "../../../../include/BSStreamHeader.h"
#include "../../../../include/NiBinaryStream.h"
#include "../../../../include/NiTPointerAllocator_unsigned_int_.h"
#include "../../../../include/NiTMapBase.h"
#include "../../../../include/NiTransform.h"
#include "../../../../include/NiAVObject.h"
#include "../../../../include/NiAVObjectPalette.h"
#include "../../../../include/NiDefaultAVObjectPalette.h"
#include "nidefaultavobjectpalette_types_NiDefaultAVObjectPalette.h"
#include "../../../../include/NiStream.h"
#include "../../../../include/NiInterpolator.h"
#include "../../../../include/NiGlobalStringTable.h"
#include "nidefaultavobjectpalette_decls.h"

void AddObjectToPalette(NiAVObject*, NiDefaultAVObjectPalette*);

// The bare handle of an NiFixedString whose constructor and destructor these functions spell out inline (NiMain
// has no exception frames; a real NiFixedString local would add one to this /EHsc unit).
struct FixedStringHandle
{
    char* m_kHandle;
};

struct NiDefaultPaletteNodeChildren
{
    unsigned char m_data[0xA0];
    NiAVObject** m_children;
    unsigned short m_usMaxSize;
    unsigned short m_usArraySize;
};

// @fnv 0x00a6e870
// pdb: RecurseAndAddObjectsToPalette | NiDefaultAVObjectPalette.obj | retail, call graph (shared callers), high
// pdb params: apObj3D, apPalette
// pdb locals: pnode, i
void AddObjectToPalette(NiAVObject* apObj3D, NiDefaultAVObjectPalette* apPalette)
{
    if (!apObj3D)
        return;
    if (!apPalette)
        return;

    if (apObj3D->m_kName.m_kHandle)
        apPalette->AddNamedAVObject(apObj3D->m_kName, apObj3D);

    NiNode* node = apObj3D->IsSceneNode();
    if (node)
    {
        NiDefaultPaletteNodeChildren* children = (NiDefaultPaletteNodeChildren*)node;
        for (unsigned int i = 0; i < children->m_usArraySize; ++i)
            AddObjectToPalette(children->m_children[i], apPalette);
    }
}

// @fnv 0x00a6e960
// src: XBOX_RETAIL_PDB 0x00a6e960
// pdb: NiDefaultAVObjectPalette::ResetAndFillFromScenegraph | NiDefaultAVObjectPalette.obj | retail, call graph, high
void NiDefaultAVObjectPalette::ResetAndFillFromScenegraph()
{
    if (m_pkScene)
    {
        m_kObjectMap.RemoveAll();
        AddObjectToPalette(m_pkScene, this);
    }
}

// Loads the scene object and a count-prefixed list of name/object pairs from the stream, registering each pair in the palette.
// @fnv 0x00a6ea40
// pdb: NiDefaultAVObjectPalette::LoadBinary | NiDefaultAVObjectPalette.obj | retail, upstream name already matches, high
// pdb params: stream
// pdb locals: uiCount, pkObj, pcString
void NiDefaultAVObjectPalette::LoadBinary(NiStream& stream)
{
    ((NiInterpolator*)this)->NiInterpolator::LoadBinary(stream);
    m_pkScene = (NiAVObject*)stream.DeserializeObject();

    unsigned int count = 0;
    ReadBinaryValue(stream, count);

    if (count > 0)
    {
        do
        {
            --count;
            char* string = 0;
            stream.ReadStreamString(string);
            NiAVObject* object = (NiAVObject*)stream.DeserializeObject();
            if (string && object)
            {
                FixedStringHandle name;
                name.m_kHandle = NiGlobalStringTable::AddString(string);
                AddNamedAVObject(reinterpret_cast<NiFixedString&>(name), object);
                if (name.m_kHandle != EmptyFS.m_kHandle)
                {
                    InterlockedDecrement((volatile long*)(name.m_kHandle - 8));
                }
            }
            NiFree(string);
        } while (count > 0);
    }
}

extern MemoryManager g_memoryManager;

// @fnv 0x00a6ef50
// src: COMMUNITY 0x00a6ef50
void* __cdecl NiTFixedStringMap_NiGlobalConstantEntry_NewItem()
{
    void* p = g_memoryManager.Allocate(0xC);
    if (!p)
        p = 0;
    ((int*)p)[1] = 0;
    return p;
}

// Assigns the pointer from p2 into p1's slot with reference counting, and stores p3 alongside it.
// @fnv 0x00a6e550
// pdb: NiDefaultAVObjectPalette::CopyMembers | NiDefaultAVObjectPalette.obj | debug, object-file run extension, low
// pdb params: pDest, kCloning
void __stdcall AssignSharedPtr(void* apTarget, void* apSource, void* apValue)
{
    void* v = *(void**)apSource;
    if (*(void**)((char*)apTarget + 4) != v)
    {
        if (v != *(void**)&EmptyFS)
            InterlockedIncrement((long*)((char*)v - 8));
        void* old = *(void**)((char*)apTarget + 4);
        if (old != *(void**)&EmptyFS)
            InterlockedDecrement((long*)((char*)old - 8));
        *(void**)((char*)apTarget + 4) = v;
        *(void**)((char*)apTarget + 8) = apValue;
        return;
    }
    *(void**)((char*)apTarget + 8) = apValue;
}

extern int DAT_0109ca08;
extern int DAT_0109ca18;
struct FUN_00a6eb20_Holder
{
    void* __thiscall DestroyHolder(unsigned int);
};
// Resets the two embedded maps, frees the pointer at offset 8, and optionally deallocates the object when flag bit 0 is set.
// @fnv 0x00a6eb20
void* __thiscall FUN_00a6eb20_Holder::DestroyHolder(unsigned int auiFlags)
{
    PaletteObjectMapView* p = (PaletteObjectMapView*)this;
    *(void**)this = (void*)&DAT_0109ca18;
    p->RemoveAll();
    *(void**)this = (void*)&DAT_0109ca08;
    p->RemoveAll();
    NiFree(*(void**)((char*)this + 8));
    if (auiFlags & 1)
        g_memoryManager.Deallocate(this);
    return this;
}

struct NodeChildrenView_cls
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
    unsigned int unk38;
    unsigned int unk3C;
    unsigned int unk40;
    unsigned int unk44;
    unsigned int unk48;
    unsigned int unk4C;
    unsigned int unk50;
    unsigned int unk54;
    unsigned int unk58;
    unsigned int unk5C;
    unsigned int unk60;
    unsigned int unk64;
    unsigned int unk68;
    unsigned int unk6C;
    unsigned int unk70;
    unsigned int unk74;
    unsigned int unk78;
    unsigned int unk7C;
    unsigned int unk80;
    unsigned int unk84;
    unsigned int unk88;
    unsigned int unk8C;
    unsigned int unk90;
    unsigned int unk94;
    unsigned int unk98;
    unsigned int unk9C;
    NiAVObject** m_children;
    unsigned short m_usMaxSize;
    unsigned short m_usArraySize;
};

// @fnv 0x00a6e870
// pdb: RecurseAndAddObjectsToPalette | NiDefaultAVObjectPalette.obj | retail, call graph (shared callers), high
// pdb params: apObj3D, apPalette
// pdb locals: pnode, i
void __cdecl AddObjectToPaletteRecursive(NiAVObject* apObj3D, NiDefaultAVObjectPalette* apPalette)
{
    if (apObj3D == 0)
        return;
    if (apPalette == 0)
        return;
    if (apObj3D->m_kName.m_kHandle != 0)
        apPalette->AddNamedAVObject(apObj3D->m_kName, apObj3D);
    NodeChildrenView_cls* node = (NodeChildrenView_cls*)apObj3D->IsSceneNode();
    if (node == 0)
        return;
    unsigned int i = 0;
    if ((unsigned short)i < node->m_usArraySize)
    {
        do
        {
            AddObjectToPaletteRecursive(node->m_children[i], apPalette);
            i++;
        } while (i < node->m_usArraySize);
    }
}

// @fnv 0x00a6e8e0
// src: COMMUNITY 0x00a6e8e0
// pdb: RecurseAndRemoveObjectsFromPalette | NiDefaultAVObjectPalette.obj | retail, upstream name already matches, high
// pdb params: apObj3D, apPalette
// pdb locals: pnode, i
void __cdecl RecurseAndRemoveObjectsFromPalette(NiAVObject* apObj3D, NiDefaultAVObjectPalette* apPalette)
{
    if (apObj3D == 0)
        return;
    if (apPalette == 0)
        return;
    if (apObj3D->m_kName.m_kHandle != 0)
        apPalette->AddNamedAVObject(apObj3D->m_kName, 0);
    NiDefaultPaletteNodeChildren* children = (NiDefaultPaletteNodeChildren*)apObj3D->IsSceneNode();
    if (children == 0)
        return;
    for (unsigned int i = 0; i < children->m_usArraySize; ++i)
        RecurseAndRemoveObjectsFromPalette((NiAVObject*)children->m_children[i], apPalette);
}

extern "C" void* __cdecl Object_DispatchCommandFour(unsigned int size);
extern "C" void* __cdecl _NiMalloc_00aa1070(unsigned int size);
extern "C" void* __cdecl FUN_00ec61c0(void* buffer, int value, unsigned int size);
extern "C" unsigned char DAT_01096284;
extern "C" unsigned char DAT_01096114;
extern "C" unsigned char DAT_0109618c;
// The shared class has no initializing constructor declaration; this adapter adds initialization only.
struct PaletteObject : NiDefaultAVObjectPalette
{
    static __forceinline void* operator new(size_t size) throw()
    {
        return Object_DispatchCommandFour(size);
    }
    __forceinline PaletteObject() throw()
    {
        *(void**)this = &DAT_01096284;
        PaletteObjectMapView& hash = reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap);
        hash.m_ppVirtualMethodTable = (void**)&DAT_01096114;
        hash.m_uiHashSize = 37;
        hash.m_kItemCount.m_uiCount = 0;
        hash.m_ppkHashTable = (NiTMapItem<NiFixedString, NiAVObject*>**)_NiMalloc_00aa1070(148);
        FUN_00ec61c0(hash.m_ppkHashTable, 0, hash.m_uiHashSize * 4);
        hash.m_ppVirtualMethodTable = (void**)&DAT_0109618c;
        m_pkScene = 0;
    }
};
struct PaletteOwner
{
    void __thiscall NiCollisionObject_CopyMembers(PaletteObject* palette, void* argument);
    PaletteObject* __thiscall CreatePalette(void* argument);
};
// Allocates a PaletteObject, initializes it via FUN_00aa1000 with argument, and returns it.
// @fnv 0x00a6ef70
// pdb: NiDefaultAVObjectPalette::CreateClone | NiDefaultAVObjectPalette.obj | retail, object-file run extension, low
// pdb params: kCloning
// pdb locals: pkObject
PaletteObject* PaletteOwner::CreatePalette(void* argument)
{
    PaletteObject* palette = new PaletteObject;
    NiCollisionObject_CopyMembers(palette, argument);
    return palette;
}

struct BC1F550Point3
{
    float x;
    float y;
    float z;
};

extern const BC1F550Point3 g_kC1F550Zero;

class BC1F550Geometry;

template <class T> class BC1F550GeometryPointer
{
  public:
    T* m_pObject;
};

template <class T> class BC1F550NewInterface
{
  public:
    static T* Allocate(unsigned int auiCount);
};

template <class T> class BC1F550MallocInterface
{
  public:
    static T* Allocate(unsigned int auiCount);
};

template <class T, class TAlloc> inline NiTArray<T, TAlloc>::NiTArray(unsigned int uiMaxSize, unsigned int uiGrowBy)
{
        m_usMaxSize = (unsigned short)uiMaxSize;
        m_usGrowBy = (unsigned short)uiGrowBy;
        m_usSize = 0;
        m_usESize = 0;
        if (m_usMaxSize > 0)
            m_pBase = TAlloc::Allocate(m_usMaxSize);
        else
            m_pBase = 0;
    }

template <class T> class BC1F550ObjectArray : public NiTArray<T, BC1F550NewInterface<T> >
{
  public:
    BC1F550ObjectArray(unsigned int uiMaxSize = 0, unsigned int uiGrowBy = 1) throw()
        : NiTArray<T, BC1F550NewInterface<T> >(uiMaxSize, uiGrowBy)
    {}
    virtual ~BC1F550ObjectArray();
};

template <class T> class BC1F550PrimitiveArray : public NiTArray<T, BC1F550MallocInterface<T> >
{
  public:
    BC1F550PrimitiveArray(unsigned int uiMaxSize = 0, unsigned int uiGrowBy = 1) throw()
        : NiTArray<T, BC1F550MallocInterface<T> >(uiMaxSize, uiGrowBy)
    {}
    virtual ~BC1F550PrimitiveArray();
};

class BC1F550Emitter
{
  public:
    BC1F550Emitter() throw();
    virtual ~BC1F550Emitter();

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
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
};

class FUN_00c1f550_cls : public BC1F550Emitter
{
  public:
    FUN_00c1f550_cls() throw();
    virtual ~FUN_00c1f550_cls();

    BC1F550ObjectArray<BC1F550GeometryPointer<BC1F550Geometry> > m_kGeometryEmitters;
    BC1F550PrimitiveArray<unsigned int> m_kEmitterTypes;
    int m_eInitVelocityType;
    int m_eEmissionType;
    BC1F550Point3 m_kEmitterPoint;
};

// @fnv 0x00c1f550
// pdb: NiPSysMeshEmitter::NiPSysMeshEmitter | NiPSysMeshEmitter.obj | retail, call graph, high
// pdb params: pcName, pkGeomEmitter, eMeshEmissionType, eInitialVelocityType, kEmitAxis, fSpeed, fSpeedVar, fDeclination, fDeclinationVar, fPlanarAngle, fPlanarAngleVar, kInitialColor, fInitialRadius, fLifeSpan, fLifeSpanVar, fRadiusVar
FUN_00c1f550_cls::FUN_00c1f550_cls() throw()
    : m_kGeometryEmitters(1, 2), m_kEmitterTypes(0, 1), m_eInitVelocityType(0), m_eEmissionType(0),
      m_kEmitterPoint(g_kC1F550Zero)
{}

class CloningProcess
{
  public:
    NiTMapBase<NiTPointerAllocator<unsigned int>, NiObject*, NiObject*>* m_pCloneMap;
};

struct NiObjectCloneBase
{
    void ProcessClone(CloningProcess& kCloning);
};

inline void* FirstPos(const PaletteObjectMapView& m)
{
    for (unsigned int i = 0; i < m.m_uiHashSize; i++)
    {
        if (m.m_ppkHashTable[i])
            return m.m_ppkHashTable[i];
    }
    return 0;
}

// @fnv 0x00a6e5b0
// pdb: NiDefaultAVObjectPalette::ProcessClone | NiDefaultAVObjectPalette.obj | retail, upstream name already matches, high
// pdb params: kCloning
// pdb locals: bCloned, pkClone, pkDest, kIter, pkValue, key, pkClonedValue
void NiDefaultAVObjectPalette::ProcessClone(CloningProcess& kCloning)
{
    ((NiObjectCloneBase*)this)->ProcessClone(kCloning);

    FixedStringHandle kName;
    NiObject*& pkClone = reinterpret_cast<NiObject*&>(kName.m_kHandle);
    pkClone = 0;
    kCloning.m_pCloneMap->GetAt(this, pkClone);
    NiDefaultAVObjectPalette* pkDest = (NiDefaultAVObjectPalette*)pkClone;

    if (m_pkScene)
    {
        kCloning.m_pCloneMap->GetAt(m_pkScene, pkClone);
        pkDest->m_pkScene = (NiAVObject*)pkClone;
    }

    if (reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap).m_kItemCount.m_uiCount)
    {
        void* pos = FirstPos(reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap));
        while (pos)
        {
            kName.m_kHandle = EmptyFS.m_kHandle;
            NiAVObject* pkObj;
            m_kObjectMap.GetNext(pos, reinterpret_cast<NiFixedString&>(kName), pkObj);
            if (kName.m_kHandle && pkObj)
            {
                NiObject* pkObjClone;
                if (kCloning.m_pCloneMap->GetAt(pkObj, pkObjClone))
                    pkDest->AddNamedAVObject(reinterpret_cast<NiFixedString&>(kName), (NiAVObject*)pkObjClone);
            }
            if (kName.m_kHandle != EmptyFS.m_kHandle)
                InterlockedDecrement((volatile long*)(kName.m_kHandle - 8));
        }
    }
}

bool __stdcall BSAnimNote_RegisterStreamables(void*);

// @fnv 0x00a6e540
// src: XBOX_PDB 0x00a6e540
// pdb: NiDefaultAVObjectPalette::RegisterStreamables | NiDefaultAVObjectPalette.obj | debug, upstream name already matches, high
// pdb params: stream
bool __stdcall NiDefaultAVObjectPalette_RegisterStreamables(void* stream)
{
    return BSAnimNote_RegisterStreamables(stream);
}

extern char* DAT_0109b220;
extern "C" void(__stdcall* DAT_00fdf1c4)(void*);

struct PaletteMap
{
    void* unk00;
    int unk04;
    void* unk08;
    unsigned int m_uiItemCount;
    void* NiTMapIterator_GetFirstPos();
    void FUN_00a8f770(void** iterator, char** key, void** object);
};
// Keep the equality implementation here until the shared class declares its override and base call.
struct PaletteEquality : NiDefaultAVObjectPalette
{
    bool BaseEquals(PaletteEquality* other);
    bool Equals(PaletteEquality* pObject);
};
// @fnv 0x00a6e7b0
// pdb: NiDefaultAVObjectPalette::IsEqual | NiDefaultAVObjectPalette.obj | retail, object-file gap fill, high
// pdb params: pObject
// pdb locals: pkDest, kIter, pkValue, pkDestValue, key
bool PaletteEquality::Equals(PaletteEquality* pObject)
{
    if (!BaseEquals(pObject))
        return false;
    unsigned int num = reinterpret_cast<PaletteMap&>(m_kObjectMap).m_uiItemCount;
    if (reinterpret_cast<PaletteMap&>(pObject->m_kObjectMap).m_uiItemCount != num)
        return false;
    if (num)
    {
        void* iterator = reinterpret_cast<PaletteMap&>(m_kObjectMap).NiTMapIterator_GetFirstPos();
        if (iterator)
        {
            void(__stdcall * deallocate)(void*) = DAT_00fdf1c4;
            do
            {
                char* key = DAT_0109b220;
                void* object;
                reinterpret_cast<PaletteMap&>(m_kObjectMap).FUN_00a8f770(&iterator, &key, &object);
                if (!pObject->FindAVObject(reinterpret_cast<const NiFixedString&>(key)))
                {
                    if (key != DAT_0109b220)
                        deallocate(key - 8);
                    return false;
                }
                if (key != DAT_0109b220)
                    deallocate(key - 8);
            } while (iterator != 0);
        }
    }
    return true;
}

struct MapNoAlias : PaletteObjectMapView
{
    __declspec(noalias) void GetNext(void*&, NiFixedString&, NiAVObject*&) const;
};

// The shared class does not yet declare its SaveBinary override.
struct PaletteSaveShell : public NiDefaultAVObjectPalette
{
    void NiDefaultAVObjectPalette_SaveBinary(NiStream& stream);
    void NiInterpolator_LinkObject(NiStream& kStream);
};

template <class T> void WriteBinaryArray(NiBinaryStream& kOstr, const T* pValue, unsigned int uiNumValues)
{
    unsigned int uiComponentSize = sizeof(T);
    kOstr.m_uiAbsoluteCurrentPos += kOstr.m_pfnWrite(&kOstr, pValue, sizeof(T) * uiNumValues, &uiComponentSize, 1);
}

// @fnv 0x00a6e6c0
// src: XBOX_PDB 0x00a6e6c0
// pdb: NiDefaultAVObjectPalette::SaveBinary | NiDefaultAVObjectPalette.obj | retail, upstream name already matches, high
// pdb params: stream
// pdb locals: uiCount, kIter, pkValue, pcKey, key
void PaletteSaveShell::NiDefaultAVObjectPalette_SaveBinary(NiStream& stream)
{
    NiInterpolator_LinkObject(stream);

    stream.SaveLinkID(m_pkScene);

    unsigned int uiObjectCount = reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap).m_kItemCount.m_uiCount;
    NiBinaryStream* pkOstr = stream.m_pkOstr;
    WriteBinaryArray(*pkOstr, &uiObjectCount, 1);

    if (reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap).m_kItemCount.m_uiCount != 0)
    {
        void* pvPos = FirstPos(reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap));
        while (pvPos)
        {
            FixedStringHandle kKey;
            const char* pcKey;
            kKey.m_kHandle = EmptyFS.m_kHandle;
            NiAVObject* pkObject;
            ((MapNoAlias&)m_kObjectMap).GetNext(pvPos, reinterpret_cast<NiFixedString&>(kKey), pkObject);
            pcKey = kKey.m_kHandle;
            stream.SaveCString(pcKey);
            stream.SaveLinkID(pkObject);
            if (pcKey != EmptyFS.m_kHandle)
                InterlockedDecrement((volatile long*)(pcKey - 8));
        }
    }
}

void __cdecl CreateVertexBufferSet();
// @fnv 0x00aa1000
// src: XBOX_PDB 0x00aa1000
// pdb: NiCollisionObject::CopyMembers | NiCollisionObject.obj | debug, upstream name already matches, high
// pdb params: pDest, kCloning
void __cdecl NiCollisionObject_CopyMembers()
{
    CreateVertexBufferSet();
}

// Adapted from another contributor's unverified best attempt; prior model was not supplied.

extern MemoryManager g_viewerMemoryManager;
extern "C" int __cdecl sprintf(char*, const char*, ...);
char* NiViewerStrings_NiGetViewerString_Title(const char* pcName);
extern const char* NiDefaultAVObjectPalette_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011f4a04 (old: DAT_011f4a04/g_011f4a04/resetPaletteRTTI)
extern char* g_nullFixedStringHandle;
struct ViewerStrings
{
    void** m_ppVirtualMethodTable;
    char** m_pBase;
    unsigned short m_usMaxSize;
    unsigned short m_usSize;
    unsigned short m_usESize;
    unsigned short m_usGrowBy;
    void SetSize(unsigned int uiMaxSize) throw();
    void SetAt(unsigned int uiIndex, char*& element)
    {
        if (uiIndex >= m_usSize)
        {
            m_usSize = (unsigned short)(uiIndex + 1);
            if (element)
                m_usESize++;
        }
        else
        {
            if (element)
            {
                if (!m_pBase[uiIndex])
                    m_usESize++;
            }
            else
            {
                if (m_pBase[uiIndex])
                    m_usESize--;
            }
        }
        m_pBase[uiIndex] = element;
    }
    unsigned int Add(char*& element)
    {
        unsigned int uiIndex = m_usSize;
        if (uiIndex >= m_usMaxSize)
            SetSize(uiIndex + m_usGrowBy);
        SetAt(uiIndex, element);
        return uiIndex;
    }
};
struct PaletteBaseViewer
{
    void GetViewerStrings(ViewerStrings* pkStrings);
};
struct ViewerFixedString
{
    char* m_kHandle;
    ViewerFixedString() : m_kHandle(g_nullFixedStringHandle)
    {}
};
struct PaletteHashViewer
{
    void** m_ppVirtualMethodTable;
    unsigned int m_uiHashSize;
    void** m_ppkHashTable;
    unsigned int m_uiCount;
    void* GetFirstPos()
    {
        for (unsigned int i = 0; i < m_uiHashSize; i++)
        {
            if (m_ppkHashTable[i])
                return m_ppkHashTable[i];
        }
        return 0;
    }
    __declspec(noalias) void GetNext(void*& pos, ViewerFixedString& kName, NiAVObject*& pkObj) const throw();
};
// The shared class does not yet declare its viewer override.
struct NiDefaultAVObjectPaletteViewer : public NiDefaultAVObjectPalette
{
    void NiDefaultAVObjectPalette_GetViewerStrings(ViewerStrings* pStrings);
};
// Adds a formatted scene-name line and object-map keys to pkStrings, decrementing non-sentinel key handles.
// @fnv 0x00a6ed90
// src: XBOX_PDB 0x00a6ed90
// pdb: NiDefaultAVObjectPalette::GetViewerStrings | NiDefaultAVObjectPalette.obj | retail, string-set bridge, high
// pdb params: pStrings
// pdb locals: message, kIter, pkValue, key
void NiDefaultAVObjectPaletteViewer::NiDefaultAVObjectPalette_GetViewerStrings(ViewerStrings* pStrings)
{
    ((PaletteBaseViewer*)this)->GetViewerStrings(pStrings);
    char* pcRTTI = NiViewerStrings_NiGetViewerString_Title(NiDefaultAVObjectPalette_ms_RTTI);
    pStrings->Add(pcRTTI);
    char* pcScene = (char*)g_viewerMemoryManager.Allocate(0x60);
    sprintf(pcScene, "m_pkScene = %s", m_pkScene->m_kName.m_kHandle ? m_pkScene->m_kName.m_kHandle : "UNKNOWN");
    pStrings->Add(pcScene);
    if (reinterpret_cast<PaletteObjectMapView&>(m_kObjectMap).m_kItemCount.m_uiCount)
    {
        PaletteHashViewer* pkHash = (PaletteHashViewer*)&m_kObjectMap;
        void* pos = pkHash->GetFirstPos();
        while (pos)
        {
            ViewerFixedString kName;
            NiAVObject* pkObj;
            pkHash->GetNext(pos, kName, pkObj);
            char* pcName = (char*)g_viewerMemoryManager.Allocate(0x60);
            char* pcKey = kName.m_kHandle;
            sprintf(pcName, "%s", pcKey);
            pStrings->Add(pcName);
            if (pcKey != g_nullFixedStringHandle)
                InterlockedDecrement((long*)(pcKey - 8));
        }
    }
}

#include <string.h>
#include "NiRefObject_inline.h"
extern int DAT_0109ca08;
extern int DAT_0109ca18;
extern NiFixedString EmptyFS;

void* __cdecl NiMalloc(unsigned int);
void __cdecl _NiFree_00aa10f0(void*);
extern "C" void* __cdecl fnv_zlib_memset(void*, int, unsigned int);
// pdb: class Key, size 0x8 (here 0x4), 2 members
struct Key
{
    const char* text;
    __forceinline Key()
    {}
    __forceinline Key(const char* value);
    __forceinline void Release()
    {
        if (text != EmptyFS.m_kHandle)
            DAT_00fdf1c4((long*)text - 2);
    }
};
struct StringPool
{
    static char* __cdecl AddString(const char*);
};
__forceinline Key::Key(const char* value)
{
    text = StringPool::AddString(value);
}
struct Item
{
    Item* next;
    const char* key;
    NiAVObject* value;
};
// Exe 0x00a6eb60: unit-local Map view; declarations and layout are preserved.
struct NidefaultavobjectpaletteMapView
{
    void* vtable;
    unsigned int size;
    Item** buckets;
    unsigned int count;
    __forceinline NidefaultavobjectpaletteMapView()
    {
        vtable = &DAT_0109ca08;
        size = 37;
        count = 0;
        buckets = (Item**)NiMalloc(148);
        fnv_zlib_memset(buckets, 0, size * 4);
        vtable = &DAT_0109ca18;
    }
    void __thiscall FUN_00aa01f0();
    __forceinline void Destroy()
    {
        vtable = &DAT_0109ca18;
        FUN_00aa01f0();
        vtable = &DAT_0109ca08;
        FUN_00aa01f0();
        _NiFree_00aa10f0(buckets);
    }
    __forceinline void* First() const
    {
        for (unsigned int index = 0; index < size; ++index)
            if (buckets[index])
                return buckets[index];
        return 0;
    }
    void __thiscall FUN_00a8f770(void*&, Key&, NiAVObject*&);
    void __thiscall FUN_00a6e980(const Key&, const void*);
    void __thiscall RemoveKey(const Key&);
    __forceinline bool GetAt(const Key& key, NiAVObject*& value)
    {
        Item* item = buckets[(unsigned int)key.text % size];
        while (item)
        {
            if (key.text == item->key)
            {
                value = item->value;
                return true;
            }
            item = item->next;
        }
        return false;
    }
};

extern "C" void(__stdcall* DAT_00fdf1c0)(void*);
struct MapDispatch
{
    void* destroy;
    Item*(__thiscall* allocate)(NidefaultavobjectpaletteMapView*);
    void(__thiscall* release)(NidefaultavobjectpaletteMapView*, Item*);
};
__declspec(noinline) void __stdcall AssignEntry(Item* item, const Key& key, const void* value)
{
    const char* fresh = key.text;
    if (item->key != fresh)
    {
        if (fresh != EmptyFS.m_kHandle)
            DAT_00fdf1c0((long*)fresh - 2);
        if (item->key != EmptyFS.m_kHandle)
            DAT_00fdf1c4((long*)item->key - 2);
        item->key = fresh;
    }
    item->value = (NiAVObject*)value;
}
__declspec(noinline) void NidefaultavobjectpaletteMapView::FUN_00a6e980(const Key& key, const void* value)
{
    unsigned int hash = (unsigned int)key.text % size;
    Item* cursor = buckets[hash];
    while (cursor)
    {
        if (cursor->key == key.text)
        {
            AssignEntry(cursor, key, value);
            return;
        }
        cursor = cursor->next;
    }
    Item* added = ((MapDispatch*)vtable)->allocate(this);
    AssignEntry(added, key, value);
    added->next = buckets[hash];
    buckets[hash] = added;
    ++count;
}

__declspec(noinline) void NidefaultavobjectpaletteMapView::RemoveKey(const Key& key)
{
    unsigned int hash = (unsigned int)key.text % size;
    Item** link = buckets + hash;
    while (*link)
    {
        Item* item = *link;
        if (item->key == key.text)
        {
            *link = item->next;
            ((MapDispatch*)vtable)->release(this, item);
            --count;
            return;
        }
        link = &item->next;
    }
}

// Keep the undeclared post-link implementation on the shared palette layout.
struct Palette : NiDefaultAVObjectPalette
{
    void __thiscall CloneBase(void*);
    void __thiscall NiDefaultAVObjectPalette_LinkObject(void*);
};
// Clones the base object, then renames palette entries whose key differs from the object name by moving them in the map under the new key.
// @fnv 0x00a6eb60
// src: XBOX_PDB 0x00a6eb60
// pdb: NiDefaultAVObjectPalette::LinkObject | NiDefaultAVObjectPalette.obj | retail, upstream name already matches, high
// pdb params: stream
// pdb locals: swapThese, iter, obj, key, badKey, goodKey, gk
void Palette::NiDefaultAVObjectPalette_LinkObject(void* stream)
{
    CloneBase(stream);
    NidefaultavobjectpaletteMapView changes;
    stream = reinterpret_cast<NidefaultavobjectpaletteMapView&>(m_kObjectMap).First();
    while (stream)
    {
        Key key;
        key.text = EmptyFS.m_kHandle;
        NiAVObject* object;
        reinterpret_cast<NidefaultavobjectpaletteMapView&>(m_kObjectMap).FUN_00a8f770(stream, key, object);
        const char* pcKey;
        const char* name;
        if (key.text && object && (pcKey = key.text, (name = object->m_kName.m_kHandle) != 0) && strcmp(pcKey, name))
        {
            changes.FUN_00a6e980(key, name);
            if (pcKey != EmptyFS.m_kHandle)
                DAT_00fdf1c4((long*)pcKey - 2);
        }
        else if (key.text != EmptyFS.m_kHandle)
            DAT_00fdf1c4((long*)key.text - 2);
    }
    stream = changes.First();
    while (stream)
    {
        Key key;
        key.text = EmptyFS.m_kHandle;
        NiAVObject* name;
        NiAVObject* object = 0;
        changes.FUN_00a8f770(stream, key, name);
        if (key.text && name)
        {
            const char* pcKey = key.text;
            Key keyNew((const char*)name);
            Key keyOld;
            keyOld.text = pcKey;
            reinterpret_cast<NidefaultavobjectpaletteMapView&>(m_kObjectMap).GetAt(keyOld, object);
            const char* pcNewKey = keyNew.text;
            reinterpret_cast<NidefaultavobjectpaletteMapView&>(m_kObjectMap).FUN_00a6e980(keyNew, object);
            reinterpret_cast<NidefaultavobjectpaletteMapView&>(m_kObjectMap).RemoveKey(key);
            if (pcNewKey != EmptyFS.m_kHandle)
                DAT_00fdf1c4((long*)pcNewKey - 2);
            if (pcKey != EmptyFS.m_kHandle)
                DAT_00fdf1c4((long*)pcKey - 2);
        }
        else if (key.text != EmptyFS.m_kHandle)
            DAT_00fdf1c4((long*)key.text - 2);
    }
    changes.Destroy();
}
