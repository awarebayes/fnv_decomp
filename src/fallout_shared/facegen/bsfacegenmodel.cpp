#include "../../../include/BSStringT.h"
// fallout_shared/facegen/bsfacegenmodel.cpp: BSFaceGenModel, FaceGenTriData, DelStandIn, FileSizeInfo and more (43 functions)

#include "../../../include/AutoMemHelper.h"
#include "../../../include/BGSQuestObjective.h"
#include "../../../include/BSFaceGenBaseMorphExtraData.h"
#include "../../../include/BSFaceGenModelExtraData.h"
#include "../../../include/BSFadeNode.h"
#include "../../../include/NiFixedString.h"
#include "../../../include/NiColorA.h"
#include "../../../include/GO.h"
#include "../../../include/NiGeometryData.h"
#include "../../../include/NiTriBasedGeom.h"
#include "../../../include/TES.h"

#include "../../../include/NP.h"
#include "../../../include/NiGeometry.h"
#include "../../../include/NiNode.h"
#include "../../../include/StrMem.h"
#include "../../../include/AutoMemContext.h"
#include "../../../include/BSFaceGenManager.h"
#include "../../../include/BSFaceGenModel.h"
#include "../../../include/BSFaceGenMorphData.h"
#include "../../../include/BSFaceGenMorphDataHair.h"
#include "../../../include/BSFaceGenMorphDataHead.h"
#include "../../../include/NiUpdateData.h"

// Unit-specific declarations; shared Ni types come from NiGeometry.h.
void* __cdecl operator new(size_t);
void __cdecl operator delete(void*);

void* __cdecl operator new[](size_t);
void __cdecl operator delete[](void*);
#include "../../../include/MEM_CONTEXT.h"
#include "../../../include/HAIR_PART.h"
template <class T> class FaceGenStringT
{
  public:
    T* pString;
    unsigned short size, capacity;
    const T* String();
    unsigned int Length();
    bool Set(const T*, unsigned int);
};
template <class T> class FaceGenIterator
{
  public:
    T* current;
    void* container;
    T& operator*() const;
    FaceGenIterator operator++(int);
};
template <class T> class FaceGenVector
{
  public:
    void* proxy;
    T* first;
    T* last;
    T* end;
    unsigned int allocator;
    unsigned int state;
    unsigned int size() const;
    T& operator[](unsigned int);
    FaceGenIterator<T> begin();
};
template <class T> class ThreeComponentVector
{
  public:
    T x, y, z;
};
class FaceGenMorphTarget
{
  public:
    float scale;
    FaceGenVector<ThreeComponentVector<short> > kVertexPositionLocal;
};
class FaceGenMorphData
{
  public:
    unsigned int state;
    FaceGenVector<FaceGenMorphTarget> modes[2];
    FaceGenMorphData(const char*);
};
class FaceGenCoordinate
{
  public:
    unsigned int data[8];
    FaceGenIterator<float> operator()(unsigned int, int);
};
class GeometryBufferView
{
  public:
    void* data;
    unsigned int stride;
    unsigned int count;
    GeometryBufferView();
    unsigned int* FUN_00717e50();
    void* Modelloader_GetReferencedThis();
    void NiGeometryDataIteratorNiPoint3_Get(unsigned int, NiPoint3&);
    void FUN_0065b390(unsigned int, const NiPoint3&);
};



class FaceGenTriData
{
  public:
    unsigned int data[60];
    FaceGenTriData();
    ~FaceGenTriData();
    unsigned int FffTriFileDataS_numModelVtx();
    unsigned int std_vector_FR2Vect3TC_float_std_allocator_FR2Vect3TC_float_size();
    NiPoint3& GetCheckedElement(unsigned int);
};
class FaceGenGeometryModel
{
  public:
    FaceGenStringT<char> m_kEGMFileName;
    FaceGenMorphData* m_pModelData;
    NiPointer<NiNode> m_spModelRoot;
    NiPointer<NiTriBasedGeom> m_pRenderedNodeEntry;
    NiPoint3* pAdditionalVertices;
    unsigned int m_uiAdditionalVertexCount;
    NiPointer<BSFaceGenMorphData> m_spFaceMorphData;
    int BackgroundTask;
    FaceGenGeometryModel();
};
struct FaceGenTextureFile;
class BSFaceGenTexture
{
  public:
    FaceGenStringT<char> EGTFileName;
    FaceGenTextureFile* m_pTextureData;
    BSFaceGenTexture();
};
void BSFaceGenManager_LockFaceGenAccess();
void BSFaceGenManager_UnlockFaceGenAccess();
bool BSFaceGenManager_LoadEGMData(BSFaceGenModel*);
BSFaceGenBaseMorphExtraData* GetExtraDataIfPresent(NiTriBasedGeom*);
class CloningProcess
{
  public:
    unsigned int state[7];
    CloningProcess(float);
    ~CloningProcess();
};
// pdb: class ModelLoader, size 0x30 (here 0x1), 12 members
class ModelLoader
{
  public:
    NiNode* LoadFile(const char*, int, bool, bool, bool, bool);
};
class NarrowString
{
  public:
    unsigned int text[7];
    NarrowString(const char*);
    ~NarrowString();
};
extern ModelLoader* g_modelLoader;
extern TES* g_TES;
extern const char* DAT_0119b734[];
int __cdecl BSstricmp(const char*, const char*);
int Warning(const char*, ...);
int NiTexturingProperty_GetType();
bool fffReadTriFile(const NarrowString&, FaceGenTriData*);
FaceGenTextureFile* BSFaceGenModel_EGTData_Create(const char*);
NiAVObject* BSFaceGenManager_GetFaceGenModelExtraData(NiAVObject*);

// @fnv 0x00659ce0
// pdb: BSFaceGenModel::LoadModelMesh | BSFaceGenModel.obj | retail, upstream name already matches, high
// pdb params: apEGMFilename, apNIFFilename, apTRIFilename, aeHairPart, abIsAccessory
// pdb locals: tempMemContext, pn, iobjCount, pobjTemp, iobj, cloneProc, updateData, pmodelData, ivert
bool BSFaceGenModel::LoadModelMesh(const char* apEGMFilename, const char* apNIFFilename, const char* apTRIFilename, HAIR_PART aeHairPart,
                                   bool abIsAccessory)
{
    if (pMeshData || !apEGMFilename)
        return true;
    AutoMemContext context(MEM_CONTEXT_MEM_CONTEXT_FACEGEN_EGM, true,
                           "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\FaceGen\\BSFaceGenModel.cpp", 286);
    pMeshData = new FaceGenGeometryModel;
    pMeshData->m_kEGMFileName.Set(apEGMFilename, 0);
    if (apNIFFilename)
    {
        unsigned int count;
        NiAVObject* i;
        unsigned int child;
        NiNode* root = g_modelLoader->LoadFile(apNIFFilename + 7, 0, true, false, true, true);
        if (root)
        {
            CloningProcess cloning(1.0f);
            pMeshData->m_spModelRoot = g_TES->CreateDeepCopySameTextures(root, cloning);
            if (pMeshData->m_spModelRoot)
            {
                child = pMeshData->m_spModelRoot->FUN_0043b480();
                for (count = 0; count < child; count++)
                {
                    i = pMeshData->m_spModelRoot->FUN_0043b4a0(count);
                    if (i)
                    {
                        switch (aeHairPart)
                        {
                        case HAIR_PART_HAIR_PART_NORMAL:
                            if (child == 1)
                            {
                                ((NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry) = i->IsTriBasedGeometry();
                            }
                            else
                            {
                            case HAIR_PART_HAIR_PART_HAT:
                                if (!BSstricmp(i->GetName(), DAT_0119b734[aeHairPart]))
                                    ((NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry) = i->IsTriBasedGeometry();
                            }
                            break;
                        default:
                            ((NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry) = i->IsTriBasedGeometry();
                            break;
                        }
                    }
                    if (((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator NiAVObject*())
                        break;
                }
                if (((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator NiAVObject*())
                {
                    if (!abIsAccessory)
                        ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                            ->RemoveProperty(NiTexturingProperty_GetType());
                    ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                        ->RemoveAllControllers();
                    ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                        ->UpdateModelBound();
                    if (((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                            ->GetSkinInstance())
                        ((BGSQuestObjective*)((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry)
                                                  .
                                                  operator->())
                             ->GetSkinInstance())
                            ->SetOwnerQuest(
                                (TESQuest*)(NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry)
                                    .
                                    operator NiAVObject*());
                    ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                        ->SetSortObject(true);
                }
                root->UpdateSceneObject();
                NiUpdateData update(0.0f, false, false);
                root->Update(update);
            }
        }
        else
        {
            Warning("MODELS: Failed to load '%s' in BSFaceGenModel::LoadModelMesh.", apNIFFilename);
            return false;
        }
    }
    if (apTRIFilename)
    {
        FaceGenTriData* i = new FaceGenTriData;
        unsigned int tri;
        if (fffReadTriFile(apTRIFilename, i) &&
            ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())->GetVertexCount() ==
                i->FffTriFileDataS_numModelVtx())
        {
            if (aeHairPart != HAIR_PART_HAIR_PART_NONE)
                ((NiPointer<NiAVObject>&)pMeshData->m_spFaceMorphData) =
                    (NiAVObject*)new BSFaceGenMorphDataHair(i);
            else
            {
                ((NiPointer<NiAVObject>&)pMeshData->m_spFaceMorphData) =
                    (NiAVObject*)new BSFaceGenMorphDataHead(i);
                if (i->std_vector_FR2Vect3TC_float_std_allocator_FR2Vect3TC_float_size() >
                    ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                        ->GetVertexCount())
                {
                    pMeshData->m_uiAdditionalVertexCount =
                        i->std_vector_FR2Vect3TC_float_std_allocator_FR2Vect3TC_float_size() -
                        ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())
                            ->GetVertexCount();
                    pMeshData->pAdditionalVertices = new NiPoint3[pMeshData->m_uiAdditionalVertexCount];
                    for (tri = 0; tri < pMeshData->m_uiAdditionalVertexCount; tri++)
                    {
                        pMeshData->pAdditionalVertices[tri] = NiPoint3(
                            i->GetCheckedElement(i->FffTriFileDataS_numModelVtx() + tri).x, i->GetCheckedElement(i->FffTriFileDataS_numModelVtx() + tri).y,
                            i->GetCheckedElement(i->FffTriFileDataS_numModelVtx() + tri).z);
                    }
                }
            }
        }
        delete i;
    }
    if (pMeshData && ((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator NiAVObject*())
        ((NiTriBasedGeom*)((const NiPointer<NiAVObject>&)pMeshData->m_pRenderedNodeEntry).operator->())->SetDataConsistency(false);
    return true;
}

// @fnv 0x0065c2c0
// pdb: BSFaceGenModel::LoadEGTData | BSFaceGenModel.obj | retail, call graph, high
// pdb locals: tempMemContext
bool BSFaceGenModel::LoadFaceGenTexture()
{
    AutoMemContext context(MEM_CONTEXT_MEM_CONTEXT_FACEGEN_EGT, true,
                           "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\FaceGen\\BSFaceGenModel.cpp", 1336);
    if (!pTextureData)
        return false;
    if (!pTextureData->m_pTextureData)
    {
        if (!pTextureData->EGTFileName.Length())
            return false;
        pTextureData->m_pTextureData = BSFaceGenModel_EGTData_Create(pTextureData->EGTFileName.String());
    }
    return true;
}

// @fnv 0x0065a760
// src: XBOX_RETAIL_PDB 0x0065a760
// pdb: BSFaceGenModel::LoadModelTexture | BSFaceGenModel.obj | retail, call graph, high
// pdb params: apEGTFilename
// pdb locals: tempMemContext
bool BSFaceGenModel::LoadModelTexture(const char* apEGTFilename)
{
    if (pTextureData || !apEGTFilename)
        return true;
    AutoMemContext context(MEM_CONTEXT_MEM_CONTEXT_FACEGEN_EGT, true,
                           "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\FaceGen\\BSFaceGenModel.cpp", 448);
    pTextureData = new BSFaceGenTexture;
    pTextureData->EGTFileName.Set(apEGTFilename, 0);
    return true;
}

// Resolves the given object via GetGeomExtraData and returns the result of its vfn_94 virtual call, or null if resolution fails.
// @fnv 0x006530e0
// pdb: BSFaceGenManager::GetFaceGenModel | BSFaceGenManager.obj | debug, call graph, high
// pdb params: apGeom
// pdb locals: pextra
NiAVObject* BSFaceGenModel::GetFaceGenModelNode(NiAVObject* apGeom)
{
    NiAVObject* result = BSFaceGenManager_GetFaceGenModelExtraData(apGeom);
    // Preserve the resolver's untyped virtual call at slot 0x94.
    typedef NiAVObject* (__thiscall *ResolveNode)(NiAVObject*);
    return result ? reinterpret_cast<ResolveNode>((*reinterpret_cast<void***>(result))[0x94 / 4])(result) : 0;
}

void* __fastcall FUN_00659670(void* pThis, int, const char* apEGTFilename);

struct Dummy_00659780
{
    char pad[0x34];
    Dummy_00659780(const char* apEGTFilename)
    {
        FUN_00659670(this, 0, apEGTFilename);
    }
};

// Allocates a Dummy_00659780 object, constructing it from apEGTFilename, and returns it as a pointer.
// @fnv 0x00659780
// src: COMMUNITY 0x00659780
// pdb: EGTData::Create | BSFaceGenModel.obj | debug, call graph, high
// pdb params: apEGTFilename
FaceGenTextureFile* __cdecl BSFaceGenModel_EGTData_Create(const char* apEGTFilename)
{
    return (FaceGenTextureFile*)new Dummy_00659780(apEGTFilename);
}

void __fastcall FUN_00659b50(void*);

#include "../../../include/NiMemObject.h"

struct FUN_00659b20_thiscall
{
    void* FUN_00659b20(unsigned int flags);
};

// Calls FUN_00659b50 on this, frees this with NiMemObject::operator delete when flags has bit 1 set, and returns this.
// @fnv 0x00659b20
// pdb: BSFaceGenModel::`scalar deleting destructor' | BSFaceGenModel.obj | retail, vtable slot, high
void* FUN_00659b20_thiscall::FUN_00659b20(unsigned int flags)
{
    FUN_00659b50(this);
    if (flags & 1)
        NiMemObject::operator delete(this, 0x10);
    return this;
}

struct FUN_00659c70_shell : public BSFaceGenModel
{
    bool BSFaceGenModel_LoadModel(char*, char*, char*, char*, int, bool);
};

bool __cdecl BSFaceGenManager_GetUseFaceGenHeads(void);

// @fnv 0x00659c70
// src: COMMUNITY 0x00659c70
// pdb: BSFaceGenModel::LoadModel | BSFaceGenModel.obj | retail, upstream name already matches, high
// pdb params: apEGMFilename, apNIFFilename, apTRIFilename, apEGTFilename, aeHairPart, abIsAccessory
// pdb locals: breturn
bool FUN_00659c70_shell::BSFaceGenModel_LoadModel(char* apEGMFilename, char* apNIFFilename, char* apTRIFilename,
                                      char* apEGTFilename, int aeHairPart, bool abIsAccessory)
{
    bool breturn = false;
    if (!BSFaceGenManager_GetUseFaceGenHeads())
        return false;
    breturn |=
        LoadModelMesh(apEGMFilename, apNIFFilename, apTRIFilename, static_cast<HAIR_PART>(aeHairPart), abIsAccessory);
    breturn |= LoadModelTexture(apEGTFilename);
    return breturn;
}

struct StatisticalEntry;
class StatisticalArray
{
  public:
    unsigned int EvaluateValue_0065cc30();
    StatisticalEntry* GetElementChecked_00662890(unsigned int);
};

// @fnv 0x0065a5a0
// src: XBOX_DEBUG_PDB 0x0065a5a0
// pdb: FffTriFileDataS::numModelVtx | BSFaceGenModel.obj | debug, call graph, high
// pdb locals: numVtx, xx
unsigned int FaceGenTriData::FffTriFileDataS_numModelVtx()
{
    unsigned int uiMinValue = std_vector_FR2Vect3TC_float_std_allocator_FR2Vect3TC_float_size();
    for (unsigned int uiIndex = 0; uiIndex < ((StatisticalArray*)((char*)this + 0xd8))->EvaluateValue_0065cc30(); ++uiIndex)
    {
        if (*(unsigned int*)((char*)((StatisticalArray*)((char*)this + 0xd8))->GetElementChecked_00662890(uiIndex) + 0x1c) <
            uiMinValue)
        {
            uiMinValue =
                *(unsigned int*)((char*)((StatisticalArray*)((char*)this + 0xd8))->GetElementChecked_00662890(uiIndex) + 0x1c);
        }
    }
    return uiMinValue;
}

struct FourComponentFloatVector;
struct NiAdditionalGeometryData;
struct NiBound;
struct NiCullingProcess;
struct NiObject;
struct NiPoint2;
struct NiPropertyState;
struct NiRenderer;
struct NiShader;
struct NiStream;


// Forwards eConsistency to the model data consistency setter.
// @fnv 0x0065a620
// pdb: NiGeometry::SetConsistency | BSFaceGenModel.obj | debug, object-file gap fill, medium
// pdb params: eConsistency
void NiGeometry::SetDataConsistency(int eConsistency)
{
    m_spModelData->SetDataConsistency(eConsistency);
}

struct S_0065c6a0
{
    int a[6];
    ~S_0065c6a0();
};
struct S_0065c7c0
{
    int a[6];
    ~S_0065c7c0();
};
struct S_0065c890
{
    int a[6];
    ~S_0065c890();
};
struct S_0065c960
{
    int a[6];
    ~S_0065c960();
};
struct S_0065ca30
{
    int a[6];
    ~S_0065ca30();
};
struct S_0065cb00
{
    int a[6];
    ~S_0065cb00();
};
struct S_0065cbd0
{
    int a[6];
    ~S_0065cbd0();
};

void __fastcall FUN_0065c6a0(void*);
void __fastcall FUN_0065c7c0(void*);
void __fastcall FUN_0065c890(void*);
void __fastcall FUN_0065c960(void*);
void __fastcall FUN_0065ca30(void*);
void __fastcall FUN_0065cb00(void*);
void __fastcall FUN_0065cbd0(void*);

struct FUN_0065a680
{
    S_0065c6a0 m0;
    S_0065c6a0 m1;
    S_0065c7c0 m2;
    S_0065c890 m3;
    S_0065c6a0 m4;
    S_0065c7c0 m5;
    S_0065c960 m6;
    S_0065ca30 m7;
    S_0065cb00 m8;
    S_0065cbd0 m9;
    ~FUN_0065a680();
};

// @fnv 0x0065a680
// pdb: BSFaceGenModelMeshData::BSFaceGenModelMeshData | BSFaceGenModel.obj | retail, object-file run extension, low
// pdb note: debug build links it to FffTriFileDataS::~FffTriFileDataS
FUN_0065a680::~FUN_0065a680()
{}

S_0065c6a0::~S_0065c6a0()
{
    FUN_0065c6a0(this);
}
S_0065c7c0::~S_0065c7c0()
{
    FUN_0065c7c0(this);
}
S_0065c890::~S_0065c890()
{
    FUN_0065c890(this);
}
S_0065c960::~S_0065c960()
{
    FUN_0065c960(this);
}
S_0065ca30::~S_0065ca30()
{
    FUN_0065ca30(this);
}
S_0065cb00::~S_0065cb00()
{
    FUN_0065cb00(this);
}
S_0065cbd0::~S_0065cbd0()
{
    FUN_0065cbd0(this);
}

extern NiFixedString* DAT_011d5b50;

// Allocates the interned "FMD" name and stores it in DAT_011d5b50.
// @fnv 0x0065c570
// src: XBOX_RETAIL_PDB 0x0065c570 (was COMMUNITY)
// pdb: BSFaceGenModelExtraData::InitSDM | BSFaceGenModel.obj | retail, call graph, high
void __cdecl BSFaceGenModelExtraData_InitSDM()
{
    DAT_011d5b50 = ::new NiFixedString("FMD");
}

struct _Elem12
{
    int a, b, c;
};

struct _Vec12
{
    int pad0, pad1, pad2;
    _Elem12* _Myfirst;
    _Elem12* _Mylast;
};

// @fnv 0x0065c700
// src: COMMUNITY 0x0065c700
// pdb: std::vector<FR2Vect3TC<float>,std::allocator<FR2Vect3TC<float> > >::~vector<FR2Vect3TC<float>,std::allocator<FR2Vect3TC<float> > > | BSFaceGenModel.obj | debug, object-file gap fill, medium
unsigned int __fastcall std_vector_FR2Vect3TC_float_std_allocator_FR2Vect3TC_float_size(const _Vec12* pVec)
{
    return pVec->_Mylast - pVec->_Myfirst;
}

void __cdecl FUN_00ec7c56();

// Bounds-checks _Pos against FUN_0065c700() and returns the NiPoint3 element at that index from the array at data[3].
// @fnv 0x0065c720
// src: GUESS 0x0065c720 (confidence: medium)
// pdb: std::vector<FR2Vect3TC<float>,std::allocator<FR2Vect3TC<float> > >::operator[] | BSFaceGenModel.obj | debug, call graph, medium
// pdb params: _Pos
NiPoint3& FaceGenTriData::GetCheckedElement(unsigned int uiIndex)
{
    if (uiIndex >= std_vector_FR2Vect3TC_float_std_allocator_FR2Vect3TC_float_size())
    {
        FUN_00ec7c56();
    }
    do
    {
    } while (0);
    return reinterpret_cast<NiPoint3*>(data[3])[uiIndex];
}

extern unsigned int _data_011c16bc;
struct FUN_0065c630
{
    char p[0x18];
    FUN_0065c630();
    ~FUN_0065c630();
};
struct FUN_0065c750
{
    char p[0x18];
    FUN_0065c750();
    ~FUN_0065c750();
};
struct FUN_0065c820
{
    char p[0x18];
    FUN_0065c820();
    ~FUN_0065c820();
};
struct FUN_0065c8f0
{
    char p[0x18];
    FUN_0065c8f0();
    ~FUN_0065c8f0();
};
struct FUN_0065c9c0
{
    char p[0x18];
    FUN_0065c9c0();
    ~FUN_0065c9c0();
};
struct FUN_0065ca90
{
    char p[0x18];
    FUN_0065ca90();
    ~FUN_0065ca90();
};
struct FUN_0065cb60
{
    char p[0x18];
    FUN_0065cb60();
};
struct FUN_0065a4c0
{
    FUN_0065c630 m0;
    FUN_0065c630 m1;
    FUN_0065c750 m2;
    FUN_0065c820 m3;
    FUN_0065c630 m4;
    FUN_0065c750 m5;
    FUN_0065c8f0 m6;
    FUN_0065c9c0 m7;
    FUN_0065ca90 m8;
    FUN_0065cb60 m9;
    FUN_0065a4c0();
};
// Default constructor for FUN_0065a4c0 that initializes its member subobjects.
// @fnv 0x0065a4c0
// pdb: FffTriFileDataS::FffTriFileDataS | BSFaceGenModel.obj | debug, call graph, high
FUN_0065a4c0::FUN_0065a4c0()
{}

struct FUN_0065a910_shell : public BSFaceGenModel
{
    bool BSFaceGenModel_ApplyCoordinateToNewMesh(void*, void*);
};

struct FUN_0065bbc0_h
{
    bool CloneTriBasedGeom(void*);
};
struct FUN_00559450_h
{
    NiTriBasedGeom* Modelloader_GetReferencedThis();
};

// @fnv 0x0065a910
// src: COMMUNITY 0x0065a910
// pdb: BSFaceGenModel::ApplyCoordinateToNewMesh | BSFaceGenModel.obj | retail, upstream name already matches, high
// pdb params: apCoord, aspReturnMesh
bool FUN_0065a910_shell::BSFaceGenModel_ApplyCoordinateToNewMesh(void* apCoord, void* aspReturnMesh)
{
    if (!apCoord)
        return false;
    if (!((FUN_0065bbc0_h*)this)->CloneTriBasedGeom(aspReturnMesh))
        return false;
    if (!ApplyFaceGenMorphs((FaceGenCoordinate*)apCoord, ((FUN_00559450_h*)aspReturnMesh)->Modelloader_GetReferencedThis(), true))
    {
        *(NP*)aspReturnMesh = (void*)0;
        return false;
    }
    return true;
}

struct EBase
{
    virtual void Unk00();
    ~EBase();
};
struct FUN_00659c10
{
    ~FUN_00659c10();
};
struct FUN_00659c40
{
    ~FUN_00659c40();
};
struct FUN_00659b50 : EBase
{
    int pad;
    FUN_00659c10* m_pGeometryModel;
    FUN_00659c40* m_kTextureSlots;
    virtual ~FUN_00659b50();
};
// @fnv 0x00659b50
// pdb: BSFaceGenModel::~BSFaceGenModel | BSFaceGenModel.obj | debug, call graph, medium
FUN_00659b50::~FUN_00659b50()
{
    if (m_pGeometryModel)
        delete m_pGeometryModel;
    if (m_kTextureSlots)
        delete m_kTextureSlots;
}

void __cdecl Deallocate(void*);
namespace p07 {
struct DtorStandIn
{
    void FUN_0083dec0();
};
struct DelStandIn
{
    void* FUN_0065a650(unsigned int flags);
};
// Calls the destructor on this, then frees this via Deallocate when flags has bit 1 set, and returns this.
// @fnv 0x0065a650
// pdb: FffTriFileDataS::`scalar deleting destructor' | BSFaceGenModel.obj | debug, object-file gap fill, medium
void* DelStandIn::FUN_0065a650(unsigned int flags)
{
    ((DtorStandIn*)this)->FUN_0083dec0();
    if (flags & 1)
        Deallocate(this);
    return this;
}
}

// @fnv 0x00657bc0
// src: GUESS 0x00657bc0 (confidence: low)
int __fastcall Object_GetPointerSpanCount_00657bc0(int param_1)
{
    return (*(int*)(param_1 + 0x10) - *(int*)(param_1 + 0xc)) / 6;
}

struct GlobalObject_00a7b610
{
};
extern GlobalObject_00a7b610 g_011d5b58;

struct FUN_0065bee0_shell
{
    GlobalObject_00a7b610* __thiscall GetGlobalObject();
};

// @fnv 0x0065bee0
// pdb: BSFaceGenModelExtraData::GetModel | BSFaceGenModel.obj | debug, object-file gap fill, medium
GlobalObject_00a7b610* __thiscall FUN_0065bee0_shell::GetGlobalObject()
{
    return &g_011d5b58;
}

struct FUN_0065cca0
{
    unsigned int uiNumMorphs;
    unsigned int m_uiScaleCount;
    float* pScales;
    void* pFirst;
    void* pLast;
    void* pEnd;
    FUN_0065cca0();
    ~FUN_0065cca0();
};
void __cdecl fffReadEgmFile(const NarrowString& fname, void* basisKey, void* symm, void* asym);
struct FUN_00659560
{
    unsigned int m_uiScaleCount;
    FUN_0065cca0 kMorphs[2];
    FUN_00659560(const char* apcPath);
};

// Constructs a name from apcPath and passes it with this and both kMorphs entries to FUN_00aafc80.
// @fnv 0x00659560
FUN_00659560::FUN_00659560(const char* apcPath)
{
    NarrowString kName(apcPath);
    fffReadEgmFile(kName, this, &kMorphs[0], &kMorphs[1]);
}

struct FUN_00659800
{
    StrMem m_kEGMFileName;
    FaceGenMorphData* m_pModelData;
    GO m_spModelRoot;
    GO m_pRenderedNodeEntry;
    NiPoint3* pAdditionalVertices;
    unsigned int m_uiAdditionalVertexCount;
    GO m_spFaceMorphData;
    int BackgroundTask;
    FUN_00659800();
};

// @fnv 0x00659800
// pdb: BSFaceGenModelMeshData::BSFaceGenModelMeshData | BSFaceGenModel.obj | debug, call graph, high
FUN_00659800::FUN_00659800() : m_spModelRoot(0), m_pRenderedNodeEntry(0), m_spFaceMorphData(0)
{
    m_pModelData = 0;
    m_pRenderedNodeEntry = (void*)0;
    m_spModelRoot = (void*)0;
    m_spFaceMorphData = (void*)0;
    pAdditionalVertices = 0;
    m_uiAdditionalVertexCount = 0;
    BackgroundTask = 0;
}

namespace fnv_006598d0 {


struct FUN_006599d0
{
    ~FUN_006599d0();
};
struct FUN_006598d0
{
    BSStringT<char> m_kEGMFileName;
    FUN_006599d0* m_pModelData;
    GO m_spModelRoot;
    GO m_pRenderedNodeEntry;
    NiPoint3* pAdditionalVertices;
    unsigned int m_uiAdditionalVertexCount;
    GO m_spFaceMorphData;
    int BackgroundTask;
    ~FUN_006598d0();
};

// @fnv 0x006598d0
FUN_006598d0::~FUN_006598d0()
{
    m_pRenderedNodeEntry = (void*)0;
    m_spModelRoot = (void*)0;
    m_spFaceMorphData = (void*)0;
    if (pAdditionalVertices)
        delete pAdditionalVertices;
    if (m_pModelData)
        delete m_pModelData;
}
}  // namespace fnv_006598d0


struct FaceGenTextureFile
{
    ~FaceGenTextureFile();
};
struct FUN_00659a20
{
    BSStringT<char> strName;
    FaceGenTextureFile* m_pTextureData;
    ~FUN_00659a20();
};

// @fnv 0x00659a20
FUN_00659a20::~FUN_00659a20()
{
    if (m_pTextureData)
        delete m_pTextureData;
}

int __cdecl strcpy_s(char*, unsigned int, const char*);
int __cdecl strcat_s(char*, unsigned int, const char*);
char* __cdecl strrchr_0040ab30(char*, int);
struct FUN_0065a840_t : public BSFaceGenModel
{
    const char* GetFloatValue();
    void BSFaceGenModel_ForceLoadModelTexture();
};

// @fnv 0x0065a840
// src: COMMUNITY 0x0065a840
// pdb: BSFaceGenModel::ForceLoadModelTexture | BSFaceGenModel.obj | retail, string-set bridge, high
// pdb locals: pegmfile, pext, pegtfile
void FUN_0065a840_t::BSFaceGenModel_ForceLoadModelTexture()
{
    if (!pTextureData)
    {
        const char* pegmfile = GetFloatValue();
        if (pegmfile)
        {
            char pegtfile[260];
            strcpy_s(pegtfile, 260, pegmfile);
            char* pExtension = strrchr_0040ab30(pegtfile, '.');
            if (pExtension)
            {
                *pExtension = 0;
                strcat_s(pegtfile, 260, ".egt");
                LoadModelTexture(pegtfile);
                LoadFaceGenTexture();
            }
        }
    }
}

namespace fnv_0065bf80 {
struct FUN_0065bf80_shell : public BSFaceGenModel
{
    bool EnsureMorphData();
};

struct S7010
{
    unsigned int Length();
};
int __fastcall Modelloader_GetReferencedThis(int*);
struct FUN_0065cca0
{
    unsigned int uiNumMorphs;
    unsigned int m_uiScaleCount;
    float* pScales;
    void* pFirst;
    void* pLast;
    void* pEnd;
    FUN_0065cca0();
};
struct FUN_00659560
{
    unsigned int m_uiScaleCount;
    FUN_0065cca0 kMorphs[2];
    FUN_00659560(const char*);
};

// @fnv 0x0065bf80
// pdb: BSFaceGenModel::CreateNewMesh | BSFaceGenModel.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSFaceGenModel::LoadEGMData
// pdb params: aspReturnMesh
// pdb locals: spskinPartition, spTempObject, pmorphBase, spdata, pskin, spTempSkinObject
bool FUN_0065bf80_shell::EnsureMorphData()
{
    AutoMemHelper tempMemContext(
        0x38, true, "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\FaceGen\\BSFaceGenModel.cpp", 0x4f0);
    if (!pMeshData)
        return false;
    if (!*(FUN_00659560**)((char*)pMeshData + 8))
    {
        if (!((S7010*)pMeshData)->Length())
            return false;
        *(FUN_00659560**)((char*)pMeshData + 8) =
            new FUN_00659560((const char*)Modelloader_GetReferencedThis((int*)pMeshData));
    }
    return true;
}
}  // namespace fnv_0065bf80

int __fastcall Object_GetPointerSpanCount_00657bc0(FaceGenVector<ThreeComponentVector<short> >* pDeltas);
struct FUN_0065c110_t : public BSFaceGenModel
{
    unsigned int GetEGMSize();
};

// Sums the size of the EGM data: base struct plus per-morph list and delta sizes for both morph lists, returning the total.
// @fnv 0x0065c110
// pdb: BSFaceGenModel::GetEGMDataSize | BSFaceGenModel.obj | debug, call graph (shared callers), high
// pdb locals: cnt2, size, cnt
unsigned int FUN_0065c110_t::GetEGMSize()
{
    unsigned int uiSize = 0;
    unsigned int i;
    unsigned int j;
    if (pMeshData && pMeshData->m_pModelData)
    {
        uiSize = sizeof(FaceGenMorphData);
        for (i = 0; i < 2; i++)
        {
            uiSize += pMeshData->m_pModelData->modes[i].size() * 0x18;
            for (j = 0; j < pMeshData->m_pModelData->modes[i].size(); j++)
                uiSize += Object_GetPointerSpanCount_00657bc0(&pMeshData->m_pModelData->modes[i][j].kVertexPositionLocal) * 6;
        }
    }
    return uiSize;
}

struct FileSizeInfo
{
    enum ErrorCode
    {
        BSSF_OK = 0
    };
    enum AccessMode
    {
        AM_READ = 0
    };
    enum OpenMode
    {
        OM_NONE = 0
    };
    ErrorCode m_eErrorCode;
    AccessMode m_eAccessMode;
    OpenMode m_eOpenMode;
    bool bSynchronousNoBuffer;
    bool bIsOpen;
    void* hFile;
    FileSizeInfo(const char* apName, AccessMode aeAccessMode, OpenMode aeOpenMode, bool abAllowAsync);
    ~FileSizeInfo();
    ErrorCode Modelloader_GetReferencedThis();
    ErrorCode BSSystemFile_GetSize(unsigned __int64& auiSize);
    static ErrorCode GetFileSize(const char* apcPath, unsigned __int64& auiSize);
};

// Opens the file at apcPath for reading, queries its size into auiSize, and returns the error code; sets auiSize to 0 if opening fails.
// @fnv 0x0065c1f0
FileSizeInfo::ErrorCode FileSizeInfo::GetFileSize(const char* apcPath, unsigned __int64& auiSize)
{
    FileSizeInfo::ErrorCode eError;
    FileSizeInfo File(apcPath, AM_READ, OM_NONE, false);
    eError = File.Modelloader_GetReferencedThis();
    if (eError == BSSF_OK)
        eError = File.BSSystemFile_GetSize(auiSize);
    else
        auiSize = 0;
    return eError;
}

namespace fnv_0065c400 {

struct FaceGenTextureFile
{
    ~FaceGenTextureFile();
};
struct FUN_00659c40
{
    BSStringT<char> strName;
    FaceGenTextureFile* m_pTextureData;
    ~FUN_00659c40();
};
struct FUN_0065c400_t : public BSFaceGenModel
{
    void BSFaceGenModel_KillTextureData();
};

// @fnv 0x0065c400
// src: COMMUNITY 0x0065c400
// pdb: BSFaceGenModel::KillTextureData | BSFaceGenModel.obj | retail, upstream name already matches, high
void FUN_0065c400_t::BSFaceGenModel_KillTextureData()
{
    if (pTextureData)
    {
        delete ((FUN_00659c40*)pTextureData)->m_pTextureData;
        ((FUN_00659c40*)pTextureData)->m_pTextureData = 0;
        delete (FUN_00659c40*)pTextureData;
        pTextureData = 0;
    }
}
}  // namespace fnv_0065c400

struct FUN_00657c00_shell
{
    int GetCount();
};

// @fnv 0x00657c00
int FUN_00657c00_shell::GetCount()
{
    return (*(int*)((char*)this + 0x10) - *(int*)((char*)this + 0xc)) / 0x1c;
}

struct FUN_00659c10_shell
{
    void __thiscall FUN_006598d0();
    void* __thiscall FUN_00659c10(unsigned int flags);
};
void __cdecl _Deallocate(void*);

// Calls FUN_006598d0, then deallocates this when flags has bit 0 set, and returns this.
// @fnv 0x00659c10
// pdb: BSFaceGenModelMeshData::`scalar deleting destructor' | BSFaceGenModel.obj | debug, object-file gap fill, medium
void* __thiscall FUN_00659c10_shell::FUN_00659c10(unsigned int flags)
{
    FUN_006598d0();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

extern "C" void __cdecl _delete(void*, unsigned int);


// Calls FUN_0065bf20, frees this with _delete when flags has bit 0 set, and returns this.
// @fnv 0x0065bef0
// pdb: BSFaceGenModelExtraData::`scalar deleting destructor' | BSFaceGenModel.obj | debug, object-file gap fill, medium
void* BSFaceGenModelExtraData::FUN_0065bef0(unsigned int flags)
{
    this->FUN_0065bf20();
    if (flags & 1)
        _delete(this, 0x10);
    return this;
}

namespace fnv_00657c20 {
struct EGMMorphList
{
    char _pad[12];
    int _base;
    unsigned int GetCount();
    int GetMorphEntry(unsigned int);
};
unsigned int __cdecl FUN_00ec7c56(void);
// Bounds-checks idx against the morph count, then returns the address of the entry at idx (stride 0x1c) from _base.
// @fnv 0x00657c20
// pdb: std::vector<FffModeGS,std::allocator<FffModeGS> >::operator[] | BSFaceGenManager.obj | debug, call graph (shared callers), medium
// pdb params: _Pos
int EGMMorphList::GetMorphEntry(unsigned int idx)
{
    if (idx >= GetCount())
        FUN_00ec7c56();
    do
    {
    } while (0);
    return idx * 0x1c + _base;
}
}  // namespace fnv_00657c20

void __cdecl _Deallocate(void*);
struct Unk_006599d0
{
    void FUN_00659600();
    void* FUN_006599d0(unsigned int);
};
// Calls FUN_00659600, then deallocates this when flags has bit 0 set, and returns this.
// @fnv 0x006599d0
// pdb: EGMData::`scalar deleting destructor' | BSFaceGenModel.obj | debug, object-file run extension, low
void* Unk_006599d0::FUN_006599d0(unsigned int flags)
{
    FUN_00659600();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

namespace fnv_00659a00 {

struct FUN_00659a00 : StrMem
{
    int _8;
    FUN_00659a00();
};
// Initializes the _8 member to 0 after the base StrMem constructor runs.
// @fnv 0x00659a00
// pdb: BSFaceGenModelTextureData::BSFaceGenModelTextureData | BSFaceGenModel.obj | debug, call graph, medium
FUN_00659a00::FUN_00659a00()
{
    _8 = 0;
}
}  // namespace fnv_00659a00

extern NiFixedString* DAT_011d5b50;

// Calls FUN_004b9900(1) on DAT_011d5b50 if it is non-null, then sets DAT_011d5b50 to null.
// @fnv 0x0065c5f0
// src: GUESS 0x0065c5f0 (confidence: low)
// pdb: std::_Allocate<FR2Vect3TC<short> > | BSFaceGenModel.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSFaceGenModelExtraData::KillSDM
// pdb params: _Count, __formal
// pdb locals: _Ptr
void NiFixedString_UpdateState_0065c5f0()
{
    NiFixedString* pName;
    {
        NiFixedString* pInnerName = DAT_011d5b50;
        pName = pInnerName;
        {
            void* iRes;
            if (pName)
                iRes = pName->NiFixedString_ScalarDtor(1);
            else
                iRes = 0;
        }
    }
    DAT_011d5b50 = 0;
}

namespace fnv_0065c6a0 {
extern int _data_011c16bc;
struct FUN_0065cfb0
{
    ~FUN_0065cfb0();
};
struct FUN_0065c6a0 : FUN_0065cfb0
{
    void FUN_0065d050();
    ~FUN_0065c6a0();
};
// @fnv 0x0065c6a0
// pdb: std::vector<FR2Vect3TC<float>,std::allocator<FR2Vect3TC<float> > >::vector<FR2Vect3TC<float>,std::allocator<FR2Vect3TC<float> > > | BSFaceGenModel.obj | debug, object-file gap fill, medium
FUN_0065c6a0::~FUN_0065c6a0()
{
    FUN_0065d050();
}
}  // namespace fnv_0065c6a0

void __cdecl FormHeapFreeAndLog_00401030(void*);

struct Cls_659ab0
{
    void FUN_00659710();
    void* FUN_00659ab0(unsigned int flags);
};

// Calls FUN_00659710, then if bit 0 of flags is set calls FUN_00401030 on this, and returns this.
// @fnv 0x00659ab0
// pdb: EGTData::`scalar deleting destructor' | BSFaceGenModel.obj | debug, call graph, medium
void* Cls_659ab0::FUN_00659ab0(unsigned int flags)
{
    FUN_00659710();
    if (flags & 1)
        FormHeapFreeAndLog_00401030(this);
    return this;
}

// @fnv 0x0065bf20
// pdb: BSFaceGenModelExtraData::~BSFaceGenModelExtraData | BSFaceGenModel.obj | retail, upstream name already matches, high
BSFaceGenModelExtraData::~BSFaceGenModelExtraData()
{}

struct FUN_0065c090_Inner
{
    int unk00;
    int unk04;
    Unk_006599d0* unk08;
    int GetObjectReference();
};

struct FUN_0065c090_shell
{
    int unk00;
    int unk04;
    FUN_0065c090_Inner* unk08;
    bool UpdateState_0065c090();
};

// @fnv 0x0065c090
// src: GUESS 0x0065c090 (confidence: low)
// pdb: BSFaceGenModelExtraData::KillSDM | BSFaceGenModel.obj | retail, object-file run extension, low
// pdb note: debug build links it to BSFaceGenModel::UnloadEGMData
bool FUN_0065c090_shell::UpdateState_0065c090()
{
    if (unk08 && unk08->unk08)
    {
        if (!unk08->GetObjectReference())
        {
            Unk_006599d0* pCopy = unk08->unk08;
            Unk_006599d0* p = pCopy;
            p ? p->FUN_006599d0(1) : 0;
            unk08->unk08 = 0;
        }
        else
        {
            return false;
        }
    }
    return true;
}

extern int DAT_01065af4;
class FUN_004968b0
{
  public:
    void* m_vt;
    int m_iWordAt04;
    FUN_004968b0();
};
class FUN_00659ae0 : public FUN_004968b0
{
  public:
    int m_8;
    int m_iThirdValue;
    FUN_00659ae0();
};
// Initializes the vtable pointer to DAT_01065af4 and zeroes the two integer members.
// @fnv 0x00659ae0
// pdb: BSFaceGenModel::BSFaceGenModel | BSFaceGenModel.obj | debug, object-file gap fill, medium
FUN_00659ae0::FUN_00659ae0()
{
    m_vt = &DAT_01065af4;
    m_8 = 0;
    m_iThirdValue = 0;
}

struct FUN_0065a970_shell : BSFaceGenModel
{
    bool ApplyMorphs(FaceGenCoordinate* apCoord, NiTriBasedGeom* apMesh, bool abForceBaseMorph);
};

// Applies face-gen morph data to apGeometry vertices using apCoordinate weights, updating positions and additional geometry data in place.
// @fnv 0x0065a970
// pdb: BSFaceGenModel::ApplyCoordinateToExistingMesh | BSFaceGenModel.obj | retail, call graph (shared callers), high
// pdb params: apCoord, apMesh, abForceBaseMorph
// pdb locals: pskinInstance, pexDestPos, basisVect, pexDestBase, psrc, pdestPos, pextraBase, ibasisSize, ivbstride, pdestBase, fcoeff, brefresh, uipartitionCount, bdbuffered, ppartitions, iter, iverts, pmd, isymm, ibasis, pskinPartition, vcoeff, xbsrcVect, ppartition ...
bool FUN_0065a970_shell::ApplyMorphs(FaceGenCoordinate* apCoord, NiTriBasedGeom* apMesh, bool abForceBaseMorph)
{
    if (!pMeshData)
        return false;
    if (!apCoord)
        apCoord = reinterpret_cast<FaceGenCoordinate*>(BSFaceGenManager::GetFaceGenCoordinates());
    if (!apCoord)
        return false;

    BSFaceGenManager_LockFaceGenAccess();
    if (!BSFaceGenManager_LoadEGMData(this))
    {
        if (!pMeshData->m_pModelData)
        {
            if (!pMeshData->m_kEGMFileName.Length())
            {
                BSFaceGenManager_UnlockFaceGenAccess();
                return false;
            }
            pMeshData->m_pModelData = new FaceGenMorphData(pMeshData->m_kEGMFileName.String());
        }
        if (!pMeshData->m_pModelData)
        {
            BSFaceGenManager_UnlockFaceGenAccess();
            return false;
        }
    }

    unsigned int uiNumAdditionalVal = 0;
    NiPoint3* pAdditional = 0;
    NiPoint3* pVerticesCount = 0;
    NiPoint3* pThePositions = 0;
    BSFaceGenBaseMorphExtraData* pLastMorphExtra = GetExtraDataIfPresent(apMesh);
    NiPoint3* pExtraVerticesTmp = pLastMorphExtra ? pLastMorphExtra->GetVertices() : 0;
    NiPoint3* pExtra = 0;
    bool bPackedResult = false;
    GeometryBufferView kIteratorNum;
    if (apMesh)
    {
        NiGeometryData* pData = apMesh->GetModelData();
        if (pData && pData->HasAdditionalGeometryData(1))
        {
            pData->NiGeometryData_GetVerticesIterator(kIteratorNum);
            uiNumAdditionalVal = *kIteratorNum.FUN_00717e50();
            pVerticesCount = (NiPoint3*)kIteratorNum.Modelloader_GetReferencedThis();
        }
    }

    unsigned int uiNumVertices;
    if (pVerticesCount)
    {
        uiNumVertices = apMesh->GetVertexCount();
    }
    else
    {
        BSFaceGenManager_UnlockFaceGenAccess();
        return false;
    }

    unsigned int uiMorphCountVal;
    for (unsigned int uiMode = 0; uiMode < 2; uiMode++)
    {
        uiMorphCountVal = pMeshData->m_pModelData->modes[uiMode].size();
        for (unsigned int i = 0; i < uiMorphCountVal; i++)
        {
            if (pMeshData->m_pModelData->modes[uiMode][i].kVertexPositionLocal.size() < uiNumVertices)
                uiNumVertices = pMeshData->m_pModelData->modes[uiMode][i].kVertexPositionLocal.size();
        }
    }

    float fWeightCur = 0.0f;
    for (unsigned int uiMode = 0; uiMode < 2; uiMode++)
    {
        uiMorphCountVal = pMeshData->m_pModelData->modes[uiMode].size();
        for (unsigned int i = 0; i < uiMorphCountVal; i++)
        {
            fWeightCur = *apCoord[uiMode](i, 0) * pMeshData->m_pModelData->modes[uiMode][i].scale;
            if (fWeightCur == 0.0 && !abForceBaseMorph)
                continue;

            pThePositions = pVerticesCount;
            pExtra = pExtraVerticesTmp;
            FaceGenIterator<ThreeComponentVector<short> > kDelta;
            kDelta = pMeshData->m_pModelData->modes[uiMode][i].kVertexPositionLocal.begin();
            unsigned int j;
            for (j = 0; j + 3 < uiNumVertices; j += 4)
            {
                NiPoint3 kP0;
                NiPoint3 kP1New;
                NiPoint3 kP2Tmp;
                NiPoint3 kP3Tmp;
                if (pExtra)
                {
                    kP0 = pExtra[0];
                    kP1New = pExtra[1];
                    kP2Tmp = pExtra[2];
                    kP3Tmp = pExtra[3];
                }
                else
                {
                    kIteratorNum.NiGeometryDataIteratorNiPoint3_Get(j, kP0);
                    kIteratorNum.NiGeometryDataIteratorNiPoint3_Get(j + 1, kP1New);
                    kIteratorNum.NiGeometryDataIteratorNiPoint3_Get(j + 2, kP2Tmp);
                    kIteratorNum.NiGeometryDataIteratorNiPoint3_Get(j + 3, kP3Tmp);
                }
                ThreeComponentVector<short> kD0Tmp = *kDelta;
                kDelta++;
                ThreeComponentVector<short> kD1Cur = *kDelta;
                kDelta++;
                ThreeComponentVector<short> kD2 = *kDelta;
                kDelta++;
                ThreeComponentVector<short> kD3 = *kDelta;
                kDelta++;

                kP0.x += kD0Tmp.x * fWeightCur;
                kP0.y += kD0Tmp.y * fWeightCur;
                kP0.z += kD0Tmp.z * fWeightCur;
                kP1New.x += kD1Cur.x * fWeightCur;
                kP1New.y += kD1Cur.y * fWeightCur;
                kP1New.z += kD1Cur.z * fWeightCur;
                kP2Tmp.x += kD2.x * fWeightCur;
                kP2Tmp.y += kD2.y * fWeightCur;
                kP2Tmp.z += kD2.z * fWeightCur;
                kP3Tmp.x += kD3.x * fWeightCur;
                kP3Tmp.y += kD3.y * fWeightCur;
                kP3Tmp.z += kD3.z * fWeightCur;

                kIteratorNum.FUN_0065b390(j, kP0);
                kIteratorNum.FUN_0065b390(j + 1, kP1New);
                kIteratorNum.FUN_0065b390(j + 2, kP2Tmp);
                kIteratorNum.FUN_0065b390(j + 3, kP3Tmp);
                if (pExtra)
                {
                    pExtra[0] = kP0;
                    pExtra[1] = kP1New;
                    pExtra[2] = kP2Tmp;
                    pExtra[3] = kP3Tmp;
                    pExtra += 4;
                }
            }
            for (; j < uiNumVertices; j++, kDelta++)
            {
                NiPoint3 kVertexDelta;
                if (pExtra)
                    kVertexDelta = *pExtra;
                else
                    kIteratorNum.NiGeometryDataIteratorNiPoint3_Get(j, kVertexDelta);
                kVertexDelta.x += (*kDelta).x * fWeightCur;
                kVertexDelta.y += (*kDelta).y * fWeightCur;
                kVertexDelta.z += (*kDelta).z * fWeightCur;
                kIteratorNum.FUN_0065b390(j, kVertexDelta);
                if (pExtra)
                {
                    *pExtra = kVertexDelta;
                    pExtra++;
                }
            }
            if (pExtra)
            {
                unsigned int uiTotal = pLastMorphExtra->GetPointerByFlag(0);
                for (unsigned int k = uiNumVertices; k < uiTotal; k++, kDelta++)
                {
                    pExtra->x = (*kDelta).x * fWeightCur + pExtra->x;
                    pExtra->y = (*kDelta).y * fWeightCur + pExtra->y;
                    pExtra->z = (*kDelta).z * fWeightCur + pExtra->z;
                    pExtra++;
                }
            }
        }
    }

    if (apMesh->GetShaderFromMaterial() && apMesh->GetModelData()->GetConsistency_00457b80() != 0x4000)
        apMesh->GetModelData()->SetDirtyFlags(1);
    apMesh->GetModelData()->NiGeometryData_UnlockPackedVertexData();
    BSFaceGenManager_UnlockFaceGenAccess();
    return true;
}

namespace fnv_0065c630 {
struct Allocator65C630
{
    Allocator65C630();
    Allocator65C630(const Allocator65C630& kOther);
    int unk00;
};
struct Base65C630
{
    Base65C630(Allocator65C630 kAllocator);
    ~Base65C630();
    int unk00;
};
struct FUN_0065c630 : public Base65C630
{
    FUN_0065c630();
    void Init(int iCount);
};

// Default constructor that initializes the base with a default allocator and calls Init with zero.
// @fnv 0x0065c630
// pdb: std::_Allocate<FffModeGS> | BSFaceGenModel.obj | retail, object-file run extension, low
// pdb params: _Count, __formal
// pdb locals: _Ptr
FUN_0065c630::FUN_0065c630() : Base65C630(Allocator65C630())
{
    Init(0);
}
}  // namespace fnv_0065c630

namespace fnv_00659c40 {
extern "C" void __cdecl _Deallocate(void*);
struct FUN_00659a20
{
    ~FUN_00659a20();
};
struct FUN_00659c40_shell : FUN_00659a20
{
    void* FUN_00659c40(unsigned int flags);
};
// Deletes the object, then calls _Deallocate on it if the low bit of flags is set, and returns the original this pointer.
// @fnv 0x00659c40
// pdb: BSFaceGenModelTextureData::`scalar deleting destructor' | BSFaceGenModel.obj | debug, call graph (shared callers), high
void* FUN_00659c40_shell::FUN_00659c40(unsigned int flags)
{
    delete this;
    if (flags & 1)
        _Deallocate(this);
    return this;
}
}  // namespace fnv_00659c40
