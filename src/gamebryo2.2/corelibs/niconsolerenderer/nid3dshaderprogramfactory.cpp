#include "../../../../include/NiTListBase.h"
#include "../../../../include/NiTListItem.h"
// gamebryo2.2/corelibs/niconsolerenderer/nid3dshaderprogramfactory.cpp: NiD3DShaderProgramFactory, ProgFactoryDirs, ShaderEntryOwner (26 functions)

#include "../../../../include/NiD3DShaderProgramCreator.h"
#include "../../../../include/NiD3DPixelShader.h"
#include "../../../../include/NiD3DVertexShader.h"
#include "../../../../include/NiRefObject.h"
#include "../../../../include/NiD3DShaderProgramFactory.h"
#include "../../../../include/NiMemObject.h"
#include "../../../../include/NiTStringPointerMap.h"

class NiShader;

typedef NiTStringTemplateMap<NiTPointerMap<const char*, NiShader*>, NiShader*> CreatorMap;
extern CreatorMap* NiD3DShaderProgramFactory_ms_pkShaderCreators; // src: XBOX_RETAIL_PDB 0x0126fa14 (old: DAT_0126fa14)
extern "C" const char* __cdecl strchr(const char*, int);
extern "C" int __cdecl tolower(int);
struct DirectoryEntry
{
    DirectoryEntry* next;
    unsigned int unused;
    const char* value;
};
// @fnv 0x00e93000
const char* NiD3DShaderProgramFactory::GetNextName(void*& apPosition)
{
    DirectoryEntry* entry = static_cast<DirectoryEntry*>(apPosition);
    if (entry)
    {
        apPosition = entry->next;
        return entry->value;
    }
    return 0;
}
// @fnv 0x00e93e70
// pdb: NiD3DShaderProgramFactory::RegisterShaderCreator | NiD3DShaderProgramFactory.obj | retail, call graph (shared callers), medium
// pdb params: pcExt, pkCreator
// pdb locals: acFileExt, i
bool NiD3DShaderProgramFactory::RegisterCreator(const char* pcName, NiD3DShaderProgramCreator* creator)
{
    char lower[256];
    if (!pcName || strchr(pcName, '.'))
        return false;
    for (unsigned int i = 0; i < 256; ++i)
    {
        lower[i] = (char)tolower(pcName[i]);
        if (!pcName[i])
            break;
    }
    if (!NiD3DShaderProgramFactory_ms_pkShaderCreators)
        CreateFactoryInstance();
    NiD3DShaderProgramFactory_ms_pkShaderCreators->SetAt(lower, reinterpret_cast<NiShader*>(creator));
    return true;
}

struct Program;
extern Program* NiD3DShaderProgramFactory_ms_pkFactory; // src: XBOX_RETAIL_PDB 0x0126fa10 (old: DAT_0126fa10)
// @fnv 0x00e92eb0
// src: COMMUNITY 0x00e92eb0
Program* NiD3DShaderProgramFactory_GetInstance()
{
    return NiD3DShaderProgramFactory_ms_pkFactory;
}

struct Program
{
    virtual void Destroy(int);
};
extern Program* NiD3DShaderProgramFactory_ms_pkFactory; // src: XBOX_RETAIL_PDB 0x0126fa10 (old: DAT_0126fa10)
// @fnv 0x00e92ec0
// pdb: NiD3DShaderProgramFactory::Shutdown | NiD3DShaderProgramFactory.obj | retail, call graph, medium
void DestroyProgram()
{
    if (NiD3DShaderProgramFactory_ms_pkFactory)
        NiD3DShaderProgramFactory_ms_pkFactory->Destroy(1);
    NiD3DShaderProgramFactory_ms_pkFactory = 0;
}

struct Nd
{
    Nd* next;
    int b;
    int data;
};
struct Lst
{
    int a;
    int b;
    Nd* head;
};
// @fnv 0x00e92fe0
// src: GAMEBRYO_LIB 0x00e92fe0
int __fastcall NiD3D10ShaderProgramFactory_GetFirstProgramDirectory(Lst* l, int, Nd** apNext)
{
    Nd* n = l->head;
    *apNext = n;
    if (n)
    {
        *apNext = n->next;
        return n->data;
    }
    return 0;
}

// @fnv 0x00e93df0
// src: COMMUNITY 0x00e93df0
// pdb: NiD3DShaderProgramFactory::_SDMInit | NiD3DShaderProgramFactory.obj | debug, upstream name already matches, high
void NiD3DShaderProgramFactory__SDMInit()
{
    if (NiD3DShaderProgramFactory_ms_pkShaderCreators == 0)
        NiD3DShaderProgramFactory::CreateFactoryInstance();
}

extern "C" void __cdecl __Dummy_00b6fef0(void*, unsigned int);
void __cdecl UpdateFields_00e93e00();

struct FUN_00e93f30_shell
{
    void* __thiscall DestroyShaderProgramFactory(unsigned int flags);
};

// @fnv 0x00e93f30
void* __thiscall FUN_00e93f30_shell::DestroyShaderProgramFactory(unsigned int flags)
{
    NiD3DShaderProgramFactory* self = (NiD3DShaderProgramFactory*)this;
    UpdateFields_00e93e00();
    if (flags & 1)
    {
        __Dummy_00b6fef0(self, 0x1c);
    }
    return self;
}

struct ShaderBufferRecord;

// Local fallback: the shared factory lacks both cache fields (+0x14/+0x18) and lookups (0x00e93020/0x00e93060).
struct ShaderFactoryCacheView
{
    int m_unusedPrefix[5];
    NiTStringPointerMap<ShaderBufferRecord*>* m_pVertexShaderMap;
    NiTStringPointerMap<ShaderBufferRecord*>* m_pPixelShaderMap;
    ShaderBufferRecord* __thiscall NiD3DShaderProgramFactory_GetVertexShader(const char* pcShaderName);
    ShaderBufferRecord* __thiscall NiD3DShaderProgramFactory_GetPixelShader(const char* pcShaderName);
};

// @fnv 0x00e93060
// src: XBOX_RETAIL_PDB 0x00e93060
// pdb: NiD3DShaderProgramFactory::GetPixelShader | NiD3DShaderProgramFactory.obj | retail, call graph, high
// pdb params: pcShaderName
// pdb locals: pkPixelShader
ShaderBufferRecord* ShaderFactoryCacheView::NiD3DShaderProgramFactory_GetPixelShader(const char* pcShaderName)
{
    NiTStringPointerMap<ShaderBufferRecord*>* map = m_pPixelShaderMap;
    if (!map)
        return 0;
    ShaderBufferRecord* pkPixelShader = 0;
    if (map->GetAt(pcShaderName, pkPixelShader))
        return pkPixelShader;
    return 0;
}

struct P03MM
{
    void* __thiscall Allocate(unsigned int size);
};
extern P03MM g_memoryManager;
extern "C" void* __cdecl _NiMalloc(unsigned int size);
extern "C" void* __cdecl _fnv_zlib_memset(void* dst, int value, size_t size);
extern int DAT_010f0eb0;
extern int DAT_010f0f50;

// @fnv 0x00e93d80
// pdb: NiD3DShaderProgramFactory::CreateShaderCreatorMap | NiD3DShaderProgramFactory.obj | retail, object-file run extension, low
void CreateCreatorMap()
{
    int* p = (int*)g_memoryManager.Allocate(0x14);
    if (p)
    {
        p[0] = (int)&DAT_010f0eb0;
        p[1] = 0x25;
        p[3] = 0;
        int* data = (int*)_NiMalloc(0x94);
        p[2] = (int)data;
        _fnv_zlib_memset((void*)data, 0, 4 * p[1]);
        *(char*)(p + 4) = 1;
        p[0] = (int)&DAT_010f0f50;
        NiD3DShaderProgramFactory_ms_pkShaderCreators = (CreatorMap*)p;
    }
    else
    {
        NiD3DShaderProgramFactory_ms_pkShaderCreators = 0;
    }
}

namespace fnv_00e92ee0 {
// If a global flag is set and pkVertexShader is non-null, returns true when already recreated, otherwise asks its creator to recreate it.
// @fnv 0x00e92ee0
// src: XBOX_PDB 0x00e92ee0
// pdb: NiD3DShaderProgramFactory::RecreateVertexShader | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pkVertexShader
// pdb locals: pkCreator
bool __cdecl NiD3DShaderProgramFactory_RecreateVertexShader(NiD3DVertexShader* pkVertexShader)
{
    NiD3DShaderProgramCreator* pkCreator;
    if (NiD3DShaderProgramFactory_ms_pkFactory && pkVertexShader)
    {
        if (pkVertexShader->IsRecreated())
            return true;
        pkCreator = pkVertexShader->GetCreator();
        if (pkCreator)
            return pkCreator->Recreate(pkVertexShader);
    }
    return false;
}
}  // namespace fnv_00e92ee0

// @fnv 0x00e93020
// src: XBOX_RETAIL_PDB 0x00e93020
// pdb: NiD3DShaderProgramFactory::GetVertexShader | NiD3DShaderProgramFactory.obj | retail, call graph, high
// pdb params: pcShaderName
// pdb locals: pkVertexShader
ShaderBufferRecord* ShaderFactoryCacheView::NiD3DShaderProgramFactory_GetVertexShader(const char* pcShaderName)
{
    if (!m_pVertexShaderMap)
        return 0;
    ShaderBufferRecord* pkVertexShader = 0;
    return m_pVertexShaderMap->GetAt(pcShaderName, pkVertexShader) ? pkVertexShader : 0;
}

struct FUN_00e93820_shell
{
    int m_auiShapeKeys[6];
    NiTStringTemplateMap<NiTPointerMap<char const*, NiShader*>, NiShader*>* m_map;
    void RegisterShader(NiD3DPixelShader* p);
};
// If p and m_map are non-null, inserts p into m_map keyed by p->GetName().
// @fnv 0x00e93820
// pdb: NiD3DShaderProgramFactory::InsertPixelShaderIntoMap | NiD3DShaderProgramFactory.obj | retail, call graph, medium
// pdb params: pkShader
void FUN_00e93820_shell::RegisterShader(NiD3DPixelShader* apShader)
{
    if (apShader && m_map)
        m_map->SetAt(apShader->GetName(), (NiShader*)apShader);
}

struct Dealloc_00e93ab0
{
    void __thiscall _DeallocateAlias(void* apMem);
};
struct Owner_00e93ab0
{
    void __thiscall FUN_00e93540();
    void* __thiscall DestroyIfFlagged(unsigned int);
};
// @fnv 0x00e93ab0
// pdb: NiTStringPointerMap<NiD3DShaderProgramCreator *>::`scalar deleting destructor' | NiD3DShaderProgramFactory.obj | retail, object-file gap fill, high
// pdb params: this
void* __thiscall Owner_00e93ab0::DestroyIfFlagged(unsigned int flags)
{
    FUN_00e93540();
    if (flags & 1)
        ((Dealloc_00e93ab0*)&g_memoryManager)->_DeallocateAlias(this);
    return this;
}

namespace fnv_00e92f30 {
// Returns true if the pixel shader can be recreated or its creator successfully recreates it, otherwise false.
// @fnv 0x00e92f30
// src: XBOX_PDB 0x00e92f30
// pdb: NiD3DShaderProgramFactory::RecreatePixelShader | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pkPixelShader
// pdb locals: pkCreator
bool NiD3DShaderProgramFactory_RecreatePixelShader(NiD3DPixelShader* pkPixelShader)
{
    NiD3DShaderProgramCreator* pkCreator;
    if (NiD3DShaderProgramFactory_ms_pkFactory && pkPixelShader)
    {
        if (pkPixelShader->GetD3DPixelShader())
            return true;
        pkCreator = pkPixelShader->GetCreator();
        if (pkCreator)
            return pkCreator->Recreate(pkPixelShader);
    }
    return false;
}
}  // namespace fnv_00e92f30

struct ShaderArg
{
    virtual void vf00();
    virtual void vf01();
    virtual void vf02();
    virtual void vf03();
    virtual void vf04();
    virtual void vf05();
    virtual void vf06();
    virtual void vf07();
    virtual void vf08();
    virtual void vf09();
    virtual void vf10();
    virtual void vf11();
    virtual void vf12();
    virtual void vf13();
    virtual void vf14();
    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();
    virtual void vf19();
    virtual const char* GetKey();
};
struct FUN_00e937f0_shell
{
    int m_auiShapeKeys[5];
    NiTStringTemplateMap<NiTPointerMap<char const*, NiShader*>, NiShader*>* m_pTaskMap;
    void __thiscall AddShader(ShaderArg* pkShader);
};
// If pkShader and the map are non-null, inserts pkShader into the map keyed by pkShader->GetKey().
// @fnv 0x00e937f0
// pdb: NiD3DShaderProgramFactory::InsertVertexShaderIntoMap | NiD3DShaderProgramFactory.obj | retail, call graph, medium
// pdb params: pkShader
void __thiscall FUN_00e937f0_shell::AddShader(ShaderArg* pkShader)
{
    if (pkShader && m_pTaskMap)
        m_pTaskMap->SetAt(pkShader->GetKey(), (NiShader*)pkShader);
}

extern int DAT_0126fa18;
extern int DAT_0126fa1c;
extern int DAT_0126fa20;

// @fnv 0x00e92f70
// src: COMMUNITY 0x00e92f70
// pdb: NiD3DShaderProgramFactory::SetD3DRenderer | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pkD3DRenderer
void NiD3DShaderProgramFactory_SetD3DRenderer(int pkD3DRenderer)
{
    DAT_0126fa1c = pkD3DRenderer;
    if (pkD3DRenderer != 0)
    {
        int* piVar1 = *(int**)(pkD3DRenderer + 0x288);
        if (DAT_0126fa18 != 0)
        {
            (**(void(__stdcall**)(int))(*(int*)DAT_0126fa18 + 8))(DAT_0126fa18);
        }
        DAT_0126fa18 = (int)piVar1;
        if (piVar1 != 0)
        {
            (**(void(__stdcall**)(int*))(*(int*)piVar1 + 4))(piVar1);
        }
        DAT_0126fa20 = *(int*)(DAT_0126fa1c + 0x8b8);
        return;
    }
    DAT_0126fa20 = 0;
    if (DAT_0126fa18 != 0)
    {
        (**(void(__stdcall**)(int))(*(int*)DAT_0126fa18 + 8))(DAT_0126fa18);
    }
    DAT_0126fa18 = 0;
}

extern "C" int __cdecl strcmp(const char*, const char*);
extern "C" size_t __cdecl strlen(const char*);
int strcpy_s(char* dst, size_t size, const char* src);
void* NiMalloc(unsigned int size);
extern "C" int __cdecl FUN_00aa1360(const char* s1, const char* s2);
typedef NiTListItem<char* > DirNode;
struct NodePoolView
{
    unsigned int m_uiCount;
    void* AddHead();
};
struct DirList
{
    void* vtbl;
    DirNode* m_pkHead;
    DirNode* m_pkTail;
    NodePoolView m_kItemCount;
    char*& GetNext(DirNode*& pos)
    {
        char*& e = pos->m_element;
        pos = pos->m_pkNext;
        return e;
    }
    void AddHead(char* e)
    {
        DirNode* n = (DirNode*)m_kItemCount.AddHead();
        n->m_element = e;
        n->m_pkPrev = 0;
        n->m_pkNext = m_pkHead;
        if (m_pkHead)
            m_pkHead->m_pkPrev = n;
        else
            m_pkTail = n;
        m_kItemCount.m_uiCount++;
        m_pkHead = n;
    }
};
struct ProgFactoryDirs
{
    void* vtbl;
    DirList m_kDirs;
    void NiD3DShaderProgramFactory_AddProgramDirectory(const char* pacDirectory);
};
// Normalizes '/' to '\\' in pcDir, returns if it already exists in m_kDirs, otherwise allocates a copy and adds it to the head of the list.
// @fnv 0x00e93960
// src: XBOX_RETAIL_PDB 0x00e93960
// pdb: NiD3DShaderProgramFactory::AddProgramDirectory | NiD3DShaderProgramFactory.obj | retail, object-file gap fill, high
// pdb params: pacDirectory
// pdb locals: pacNewAdd, kIter, pacCheckName, acSourcePath, uiLen, ui
void ProgFactoryDirs::NiD3DShaderProgramFactory_AddProgramDirectory(const char* pacDirectory)
{
    if (!pacDirectory || strcmp(pacDirectory, "") == 0)
        return;
    char acDir[260];
    strcpy_s(acDir, 260, pacDirectory);
    for (unsigned int i = 0; i < strlen(acDir); i++)
    {
        if (acDir[i] == '/')
            acDir[i] = '\\';
    }
    DirNode* pos = m_kDirs.m_pkHead;
    while (pos)
    {
        char* e = m_kDirs.GetNext(pos);
        if (e && FUN_00aa1360(e, acDir) == 0)
            return;
    }
    unsigned int len = strlen(acDir) + 1;
    char* p = (char*)NiMalloc(len);
    strcpy_s(p, len, acDir);
    m_kDirs.AddHead(p);
}

namespace fnv_00e93bb0 {
extern "C" int __cdecl strcmp(const char*, const char*);
extern "C" size_t __cdecl strlen(const char*);
extern "C" int __cdecl strcpy_s(char*, size_t, const char*);
extern "C" int __cdecl NiStricmp(const char*, const char*);

struct BSBatchRenderer__GeometryGroup;

struct DirectoryEntryNode
{
    DirectoryEntryNode* next;
    DirectoryEntryNode* prev;
    const char* value;
};

struct BSBatchRenderer__AlphaList
{
    DirectoryEntryNode* head;
    DirectoryEntryNode* tail;
    unsigned int count;

    inline BSBatchRenderer__GeometryGroup* const& GetNext(DirectoryEntryNode*& pos) const
    {
        const char* const& item = pos->value;
        pos = pos->next;
        return (BSBatchRenderer__GeometryGroup* const&)item;
    }

    BSBatchRenderer__GeometryGroup* __thiscall Remove(BSBatchRenderer__GeometryGroup*&);
};

// Factory directory view: 0x00e93bb0 accesses the list at +0x08, absent from the shared factory.
struct ShaderFactoryDirectoryView
{
    int unk00;
    int unk04;
    BSBatchRenderer__AlphaList m_directories;

    void __thiscall NiD3DShaderProgramFactory_RemoveProgramDirectory(const char* pcDirectory);
};

// Copies pcDirectory into a path buffer, converts '/' to '\\', then removes the matching entry from m_directories if found.
// @fnv 0x00e93bb0
// src: XBOX_RETAIL_PDB 0x00e93bb0
// pdb: NiD3DShaderProgramFactory::RemoveProgramDirectory | NiD3DShaderProgramFactory.obj | retail, object-file gap fill, high
// pdb params: pcDirectory
// pdb locals: pcCheckName, kIter, acSourcePath, ui
void __thiscall ShaderFactoryDirectoryView::NiD3DShaderProgramFactory_RemoveProgramDirectory(const char* pcDirectory)
{
    if (!pcDirectory || strcmp(pcDirectory, "") == 0)
        return;

    char acPath[260];
    strcpy_s(acPath, 260, pcDirectory);

    for (unsigned int i = 0; i < strlen(acPath); i++)
    {
        if (acPath[i] == '/')
            acPath[i] = '\\';
    }

    DirectoryEntryNode* kPos = m_directories.head;
    while (kPos)
    {
        BSBatchRenderer__GeometryGroup* pcSearchDir = m_directories.GetNext(kPos);
        if (pcSearchDir && NiStricmp((const char*)pcSearchDir, acPath) == 0)
        {
            m_directories.Remove(pcSearchDir);
            break;
        }
    }
}
}  // namespace fnv_00e93bb0

extern int DAT_010f0f70;
extern int DAT_0101dce4;
extern int DAT_011f4400;
extern int(__stdcall* DAT_00fdf1c4)(void*);

struct ShaderProgramStandIn_00e93e00
{
    virtual void __thiscall vf00(int);
    void __thiscall NiTPointerMap_RemoveAll_00b601a0();
};

struct FUN_00e93cc0_Obj
{
    virtual void vf00();
    void __thiscall NiD3DShaderProgramFactory_RemoveAllProgramDirectories();
};

struct FUN_00b71300_Obj
{
    virtual void vf00();
    int unk04;
    void __thiscall FUN_00b71300();
};

struct FUN_00e93e00_Obj : public FUN_00e93cc0_Obj
{
    int unk04;
    FUN_00b71300_Obj unk08;
    int unk10;
    ShaderProgramStandIn_00e93e00* unk14;
    ShaderProgramStandIn_00e93e00* unk18;

    void __thiscall UpdateFields_00e93e00();
};

// @fnv 0x00e93e00
// src: GUESS 0x00e93e00 (confidence: low)
void FUN_00e93e00_Obj::UpdateFields_00e93e00()
{
    *reinterpret_cast<int*>(this) = reinterpret_cast<int>(&DAT_010f0f70);

    if (unk14 != 0)
        unk14->NiTPointerMap_RemoveAll_00b601a0();

    if (unk14 != 0)
        unk14->vf00(1);

    if (unk18 != 0)
        unk18->NiTPointerMap_RemoveAll_00b601a0();

    if (unk18 != 0)
        unk18->vf00(1);

    NiD3DShaderProgramFactory_RemoveAllProgramDirectories();
    unk08.FUN_00b71300();

    *reinterpret_cast<int*>(this) = reinterpret_cast<int>(&DAT_0101dce4);
    DAT_00fdf1c4(&DAT_011f4400);
}

// The creator map at DAT_0126fa14 (an NiTStringPointerMap) seen through the members 0x00e93260 uses.
class CreatorMapView
{
  public:
    void* m_pvVtbl;
    unsigned int m_uiHashSize;
    void** m_ppkHashTable;

    void* GetFirstPos() const
    {
        for (unsigned int i = 0; i < m_uiHashSize; i++)
        {
            if (m_ppkHashTable[i])
                return m_ppkHashTable[i];
        }
        return 0;
    }

    void AdvanceToNextEntry(void*& pvPos, const char*& pcKey, NiD3DShaderProgramCreator*& pkVal) const;
    bool RemoveAt(const char* pcKey);
};

// Removes pkCreator from the global CreatorMapView at DAT_0126fa14 if found, iterating entries and calling RemoveAt on a matching key.
// @fnv 0x00e93260
// src: COMMUNITY 0x00e93260
// pdb: NiD3DShaderProgramFactory::UnregisterShaderCreator | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pkCreator
// pdb locals: kIter, pkMapCreator, pcExt
void __cdecl NiD3DShaderProgramFactory_UnregisterShaderCreator(NiD3DShaderProgramCreator* pkCreator)
{
    if (NiD3DShaderProgramFactory_ms_pkShaderCreators == 0)
        return;

    void* pvIter = ((CreatorMapView*)NiD3DShaderProgramFactory_ms_pkShaderCreators)->GetFirstPos();
    while (pvIter)
    {
        const char* pcExt = 0;
        NiD3DShaderProgramCreator* pkTestCreator = 0;
        ((CreatorMapView*)NiD3DShaderProgramFactory_ms_pkShaderCreators)->AdvanceToNextEntry(pvIter, pcExt, pkTestCreator);
        if (pkCreator == pkTestCreator)
            ((CreatorMapView*)NiD3DShaderProgramFactory_ms_pkShaderCreators)->RemoveAt(pcExt);
    }
}

NiD3DShaderProgramCreator* __cdecl NiD3DShaderProgramFactory_GetShaderCreator(const char* pcLibName);

// Returns the cached entry for pcName, or creates a shader with pkLibrary, registers it, and returns it.
// @fnv 0x00e938e0
// src: GAMEBRYO_LIB 0x00e938e0
// pdb: NiD3DShaderProgramFactory::CreatePixelShaderFromBuffer | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pvBuffer, uiBufferSize, pcExt, pcShaderName, pcEntryPoint, pcShaderTarget, bRecoverable
// pdb locals: pkPixelShader, pkCreator
void* __cdecl NiD3DShaderProgramFactory_CreatePixelShaderFromBuffer(void* pvBuffer, void* uiBufferSize, const char* pcExt, const char* pcShaderName, void* pcEntryPoint,
                           void* pcShaderTarget, void* bRecoverable)
{
    if (!NiD3DShaderProgramFactory_ms_pkFactory)
        return 0;
    if (!pvBuffer || !uiBufferSize)
        return 0;
    ShaderBufferRecord* pkEntry =
        reinterpret_cast<ShaderFactoryCacheView*>(NiD3DShaderProgramFactory_ms_pkFactory)->NiD3DShaderProgramFactory_GetPixelShader(pcShaderName);
    if (pkEntry)
        return pkEntry;
    NiD3DShaderProgramCreator* pkLibrary = NiD3DShaderProgramFactory_GetShaderCreator(pcExt);
    if (!pkLibrary)
        return 0;
    // The shared creator header does not yet declare the pixel-shader creation slot.
    typedef NiD3DPixelShader* (__thiscall* CreatePixelShader)(NiD3DShaderProgramCreator*, void*, void*, const char*,
                                                            void*, void*, void*);
    NiD3DPixelShader* pkShader =
        (*reinterpret_cast<CreatePixelShader**>(pkLibrary))[6](pkLibrary, pvBuffer, uiBufferSize, pcShaderName, pcEntryPoint, pcShaderTarget, bRecoverable);
    reinterpret_cast<FUN_00e93820_shell*>(NiD3DShaderProgramFactory_ms_pkFactory)->RegisterShader(pkShader);
    return pkShader;
}

extern "C" __declspec(dllimport) long __stdcall InterlockedIncrement(long volatile* p);
extern "C" void* __cdecl MemSet_00e93ad0(void* dst, int value, unsigned int size);

inline NiRefObject::NiRefObject()
{
    m_uiRefCount = 0;
    InterlockedIncrement((long volatile*)&DAT_011f4400);
}

inline void* NiMemObject::operator new(size_t uiSize)
{
    return g_memoryManager.Allocate(uiSize);
}

template <class TVAL> class ShaderMapBase : public NiMemObject
{
  public:
    ShaderMapBase(unsigned int uiHashSize) throw()
    {
        m_uiHashSize = uiHashSize;
        m_uiCount = 0;
        unsigned int uiSize = sizeof(void*) * m_uiHashSize;
        m_ppkHashTable = (void**)NiMalloc(uiSize);
        MemSet_00e93ad0(m_ppkHashTable, 0, m_uiHashSize * sizeof(void*));
    }
    virtual ~ShaderMapBase();

    unsigned int m_uiHashSize;
    void** m_ppkHashTable;
    unsigned int m_uiCount;
};

template <class TVAL> class ShaderStringTemplateMap : public ShaderMapBase<TVAL>
{
  public:
    ShaderStringTemplateMap(unsigned int uiHashSize, bool bCopy) throw() : ShaderMapBase<TVAL>(uiHashSize)
    {
        m_bCopy = bCopy;
    }
    virtual ~ShaderStringTemplateMap();

    bool m_bCopy;
};

template <class TVAL> class ShaderStringMap : public ShaderStringTemplateMap<TVAL>
{
  public:
    ShaderStringMap(unsigned int uiHashSize, bool bCopy) throw() : ShaderStringTemplateMap<TVAL>(uiHashSize, bCopy)
    {}
    virtual ~ShaderStringMap();
};

struct DirAllocator_00e93ad0 { unsigned int m_uiCount; };

typedef NiTListItem<char*> DirListItem_00e93ad0;

// local form of NiTListBase: 0x00e93ad0 needs its inlined member constructor; shared-list trial moves the owner vftable store (exe relocation +0x1e).
class DirList_00e93ad0
{
  public:
    DirList_00e93ad0() throw()
    {
        m_kItemCount.m_uiCount = 0;
        m_pkHead = 0;
        m_pkTail = 0;
    }

    DirListItem_00e93ad0* m_pkHead;
    DirListItem_00e93ad0* m_pkTail;
    DirAllocator_00e93ad0 m_kItemCount;
};

class NiD3DVertexShaderEntry;
class NiD3DPixelShaderEntry;

class FUN_00e93ad0 : public NiRefObject
{
  public:
    FUN_00e93ad0() throw();
    virtual ~FUN_00e93ad0();

    DirList_00e93ad0 m_kDirectoryList;
    ShaderStringMap<NiD3DVertexShaderEntry*>* m_pVertexShaderMap;
    ShaderStringMap<NiD3DPixelShaderEntry*>* m_pShaderBufferMap;
};

// Allocates string maps for NiD3DVertexShaderEntry and NiD3DPixelShaderEntry pointers, each with hash size 59 and copying disabled.
// @fnv 0x00e93ad0
// pdb: NiD3DShaderProgramFactory::NiD3DShaderProgramFactory | NiD3DShaderProgramFactory.obj | retail, object-file gap fill, high
FUN_00e93ad0::FUN_00e93ad0() throw()
{
    m_pVertexShaderMap = new ShaderStringMap<NiD3DVertexShaderEntry*>(59, false);
    m_pShaderBufferMap = new ShaderStringMap<NiD3DPixelShaderEntry*>(59, false);
}

struct ShaderEntryNode
{
    ShaderEntryNode* next;
    ShaderEntryNode* previous;
    void* element;
};
struct ShaderEntryList
{
    ShaderEntryNode* head;
    ShaderEntryNode* tail;
    unsigned int count;
    void*& GetNext(ShaderEntryNode*& position)
    {
        void*& element = position->element;
        position = position->next;
        return element;
    }
    void __thiscall FUN_00b602d0();
    void __thiscall FUN_00e7d5b0();
};
struct ShaderEntryNodePool
{
    void* first;
    void __thiscall BSSpinLock_Lock(int count);
};
extern ShaderEntryNodePool DAT_011c5f80;
extern ShaderEntryNode* DAT_011c5f58;
extern unsigned int DAT_011c5f84;
extern "C" void __cdecl _NiFree_00aa10f0(void*);
struct ShaderEntryOwner
{
    int unknown00;
    int unknown04;
    ShaderEntryList entries;
    void NiD3DShaderProgramFactory_RemoveAllProgramDirectories();
};
// Iterates nonnull elements, searches for matching list nodes, removes matches, and passes each element to FUN_00aa10f0.
// @fnv 0x00e93cc0
// src: XBOX_PDB 0x00e93cc0
// pdb: NiD3DShaderProgramFactory::RemoveAllProgramDirectories | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb locals: pcCheckName, kIter
void ShaderEntryOwner::NiD3DShaderProgramFactory_RemoveAllProgramDirectories()
{
    ShaderEntryList* pEntries = &entries;
    ShaderEntryNode* pNode = pEntries->head;
    if (pNode)
    {
        do
        {
            void* element = pEntries->GetNext(pNode);
            if (!element)
                continue;
            ShaderEntryNode* head = pEntries->head;
            ShaderEntryNode* candidate;
            ShaderEntryNode* position = pEntries->head;
            if (!position)
                goto release;
            do
            {
                candidate = position;
                if (element == pEntries->GetNext(position))
                    goto found;
            } while (position);
            goto release;
        found:
            if (candidate)
            {
                if (candidate == head)
                    pEntries->FUN_00b602d0();
                else if (candidate == pEntries->tail)
                    pEntries->FUN_00e7d5b0();
                else
                {
                    ShaderEntryNode* pPrev = candidate->previous;
                    ShaderEntryNode* pNext = candidate->next;
                    if (pPrev)
                        pPrev->next = pNext;
                    if (pNext)
                        pNext->previous = pPrev;
                    candidate->element = 0;
                    DAT_011c5f80.BSSpinLock_Lock(0);
                    candidate->previous = 0;
                    candidate->next = DAT_011c5f58;
                    --DAT_011c5f84;
                    DAT_011c5f58 = candidate;
                    if (!DAT_011c5f84)
                        DAT_011c5f80.first = 0;
                    --pEntries->count;
                }
            }
        release:
            _NiFree_00aa10f0(element);
        } while (pNode);
    }
}

namespace fnv_00e93850 {

NiD3DShaderProgramCreator* NiD3DShaderProgramFactory_GetShaderCreator(const char* p2);

// Returns a cached ShaderArg for p3, or creates one via the NiD3DShaderProgramCreator for p2 and registers it with the factory.
// @fnv 0x00e93850
// src: XBOX_PDB 0x00e93850
// pdb: NiD3DShaderProgramFactory::CreateVertexShaderFromBuffer | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pvBuffer, uiBufferSize, pcExt, pcShaderName, pcEntryPoint, pcShaderTarget, hDecl, uiUsage, bSoftwareVP, bRecoverable
// pdb locals: pkVertexShader, pkCreator
ShaderArg* NiD3DShaderProgramFactory_CreateVertexShaderFromBuffer(const char* pvBuffer, const char* uiBufferSize, const char* pcExt, const char* pcShaderName,
                        unsigned int pcEntryPoint, unsigned int pcShaderTarget, unsigned int hDecl, unsigned int uiUsage,
                        unsigned int bSoftwareVP, unsigned int bRecoverable)
{
    if (!NiD3DShaderProgramFactory_ms_pkFactory)
        return 0;

    if (!pvBuffer || !uiBufferSize)
        return 0;

    ShaderArg* pkShader = (ShaderArg*)((ShaderFactoryCacheView*)NiD3DShaderProgramFactory_ms_pkFactory)->NiD3DShaderProgramFactory_GetVertexShader(pcShaderName);
    if (pkShader)
        return pkShader;

    NiD3DShaderProgramCreator* pkCreator = NiD3DShaderProgramFactory_GetShaderCreator(pcExt);
    if (!pkCreator)
        return (ShaderArg*)pkCreator;

    ShaderArg* pkNewShader =
        pkCreator->CreateShader(pvBuffer, uiBufferSize, pcShaderName, pcEntryPoint, pcShaderTarget, hDecl, uiUsage, bSoftwareVP, bRecoverable);
    ((FUN_00e937f0_shell*)NiD3DShaderProgramFactory_ms_pkFactory)->AddShader(pkNewShader);
    return pkNewShader;
}
}  // namespace fnv_00e93850

bool __cdecl NiFile_Access(const char* pcName, int eMode);
extern "C" int __cdecl FUN_00ec65a6(char* d, unsigned int n, const char* s);
int __cdecl NiShaderFactory_ReportError(int, bool, const char*, ...);
void __cdecl NiD3DUtility_GetSplitPath(const char* pcFilename, char* pcDrive, char* pcDir, char* pcFname, char* pcExt);
int __cdecl FUN_00a305b0(char* d, unsigned int n, const char* fmt, ...);
typedef NiTListItem<const char* > ShPF_Node;
struct ShPF_Factory
{
    void* vt;
    int m4;
    ShPF_Node* m_pkHead;
    ShPF_Node* GetHeadPos() const
    {
        return m_pkHead;
    }
    const char*& GetNext(ShPF_Node*& kPos) const
    {
        const char*& element = kPos->m_element;
        kPos = kPos->m_pkNext;
        return element;
    }
    const char* GetFirstDir(ShPF_Node*& kIter) const
    {
        kIter = GetHeadPos();
        if (kIter)
            return GetNext(kIter);
        return 0;
    }
    const char* GetNextDir(ShPF_Node*& kIter) const
    {
        if (kIter)
            return GetNext(kIter);
        return 0;
    }
};
inline ShPF_Factory* ShPF_Get()
{
    return (ShPF_Factory*)NiD3DShaderProgramFactory_ms_pkFactory;
}
// Resolves a shader program file path by checking registered directories and returns true if the file exists.
// @fnv 0x00e930a0
// src: XBOX_RETAIL_PDB 0x00e930a0
// pdb: NiD3DShaderProgramFactory::ResolveShaderFileName | NiD3DShaderProgramFactory.obj | retail, string-set bridge, high
// pdb params: pcOriginalName, pcTrueName, uiStringLen
// pdb locals: acFileExt, acDir, acFileName, pacProgDir, kIter, acDrive, bGood
bool NiD3DShaderProgramFactory_ResolveShaderFileName(const char* pcOriginalName, char* pcTrueName, unsigned int uiStringLen)
{
    if (NiFile_Access(pcOriginalName, 0))
    {
        FUN_00ec65a6(pcTrueName, uiStringLen, pcOriginalName);
        return true;
    }
    if (!ShPF_Get())
    {
        NiShaderFactory_ReportError(1, false, "No valid shader program factory\n");
        return false;
    }
    char acDrive[4];
    char acDir[260];
    char acFileName[260];
    char acExt[256];
    NiD3DUtility_GetSplitPath(pcOriginalName, acDrive, acDir, acFileName, acExt);
    ShPF_Node* kIter;
    const char* pcDir = ShPF_Get()->GetFirstDir(kIter);
    while (pcDir)
    {
        char valid = pcDir[0];
        if (valid)
        {
            if (pcDir[strlen(pcDir) - 1] != '/' && pcDir[strlen(pcDir) - 1] != '\\')
                FUN_00a305b0(pcTrueName, uiStringLen, "%s\\%s%s", pcDir, acFileName, acExt);
            else
                FUN_00a305b0(pcTrueName, uiStringLen, "%s%s%s", pcDir, acFileName, acExt);
        }
        else
        {
            NiShaderFactory_ReportError(1, false, "Invalid or no shader program directory\n");
        }
        if (valid)
        {
            if (NiFile_Access(pcTrueName, 0))
                return true;
            NiShaderFactory_ReportError(1, true, "Shader program file not found %s in directory %s\n", pcOriginalName, pcDir);
        }
        pcDir = ShPF_Get()->GetNextDir(kIter);
    }
    return false;
}

namespace fnv_00e932f0 {
struct FUN_00a53c30_Map
{
    bool __thiscall FUN_00a53c30(const char* key, void*& value);
};

void __cdecl NiD3DUtility_GetFileExtension(const char* source, char* pcExt, unsigned int uiMaxLen);
char __cdecl FUN_00ec67aa(int c);

// Looks up a value in DAT_0126fa14 using the lowercased extension, or the filename when no extension is extracted.
// @fnv 0x00e932f0
// src: XBOX_PDB 0x00e932f0
// pdb: NiD3DShaderProgramFactory::GetShaderCreator | NiD3DShaderProgramFactory.obj | retail, upstream name already matches, high
// pdb params: pcFilename
// pdb locals: acFileExt, pcSrc, i, pkCreator
void* __cdecl NiD3DShaderProgramFactory_GetShaderCreator(const char* pcFilename)
{
    char acFileExt[256];
    void* creator;
    const char* source;
    unsigned int index;

    if (NiD3DShaderProgramFactory_ms_pkShaderCreators == 0)
        return 0;

    if (pcFilename == 0 || *pcFilename == 0)
        return 0;

    NiD3DUtility_GetFileExtension(pcFilename, acFileExt, 0x100);
    source = acFileExt + 1;
    if (acFileExt[1] == 0)
        source = pcFilename;
    for (index = 0; index < 0x100; ++index)
    {
        char converted = FUN_00ec67aa(source[index]);
        acFileExt[index] = converted;

        if (converted == 0)
            break;
    }

    creator = 0;
    if (((FUN_00a53c30_Map*)NiD3DShaderProgramFactory_ms_pkShaderCreators)->FUN_00a53c30(acFileExt, creator))
        return creator;

    return 0;
}
}  // namespace fnv_00e932f0
