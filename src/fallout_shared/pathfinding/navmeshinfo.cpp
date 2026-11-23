#include "../../../include/NavMeshInfo.h"
// fallout_shared/pathfinding/navmeshinfo.cpp: NavMeshInfo, NavMeshInfoSaveShell, NavInfo (15 functions)

#include "../../../include/NavMeshPtr.h"
#include "../../../include/NiPoint3.h"
#include "../../../include/TES.h"
#include "../../../include/TESFile.h"
#include "../../../include/TESForm.h"
#include "../../../include/NavMeshBounds.h"

class NavMeshInfo;
class TESObjectREFR;
class NavMeshLookupView_00693d70;
class TESObjectWEAP;
class SandboxProcedureData;
class InventoryStackRecord;

int __cdecl Warning(const char* format, ...);

template <int N> struct EndianSizeTag
{
};
void __cdecl BSEndian(unsigned int* value, EndianSizeTag<4> tag);
extern "C" void* __cdecl memcpy(void* destination, const void* source, size_t size);

// 0x00693d70 is the NavMeshInfoMap lookup (map at +0x1c), not the static Pathing utility.
class NavMeshLookupView_00693d70
{
  public:
    NavMeshInfo* FindReferenceByFormId();
};
extern TES* g_TES;
class TESObjectWEAP;

#include "../../../include/NiTNewInterface.h"
#include "../../../include/NiTArray.h"

#include "../../../include/BSSimpleArray.h"




// The constructor and destructor store the BSSimpleArray<NavMeshInfo *,1024> vftable 0x0106b1a4.
// @fnv 0x0069af10
// pdb: BSSimpleArray<NavMeshInfo *,1024>::~BSSimpleArray<NavMeshInfo *,1024> | NavMesh_Util.obj | debug, call graph, high
template <> BSSimpleArray<NavMeshInfo*, 1024>::~BSSimpleArray()
{
    ((BSSimpleArray<SandboxProcedureData, 1024>*)this)->Clear(true);
}

// @fnv 0x006b5490
// src: XBOX_DEBUG_PDB 0x006b5490
// pdb: NavMeshInfo::InitDoorPointers | NavMeshInfo.obj | debug, unique string literal, high
// pdb locals: icount, i, doorFormID
void NavMeshInfo::InitDoorPointers()
{
    unsigned int ItemUpdateArray = ((BSSimpleArray<NavMeshInfo*, 1024>*)&ConnectedDoors)->GetSize();
    for (unsigned int i = 0; i < ItemUpdateArray; ++i)
    {
        TESObjectREFR* door =
            ((NiTArray<TESObjectREFR*, NiTNewInterface<TESObjectREFR*> >*)&ConnectedDoors)->GetAt(i);
        ((NiTArray<TESObjectREFR*, NiTNewInterface<TESObjectREFR*> >*)&ConnectedDoors)->GetAt(i) =
            (TESObjectREFR*)TESForm::GetFormByID((unsigned int)door);
        if (!((NiTArray<TESObjectREFR*, NiTNewInterface<TESObjectREFR*> >*)&ConnectedDoors)->GetAt(i))
        {
            Warning("NavMeshInfo %08x, Could not find Door %08x referenced by NavMeshInfo", NavMeshID,
                    (unsigned int)door);
            ((BSSimpleArray<InventoryStackRecord*, 1024>*)&ConnectedDoors)->Remove(i, true);
            --ItemUpdateArray;
            --i;
        }
    }
}

// @fnv 0x006b51a0
// src: XBOX_DEBUG_PDB 0x006b51a0
// pdb: NavMeshInfo::SetFromPortalChunk | NavMeshInfo.obj | debug, call graph, high
// pdb params: apChunkBuffer, apFile, aiFileVersionNumber
// pdb locals: icount, i, infoID
void NavMeshInfo::SetFromPortalChunk(char* apChunkBuffer, TESFile* apFile, int aiFileVersionNumber)
{
    unsigned int count = 0;
    memcpy(&count, apChunkBuffer, 4);
    if (apFile->IsEndianConversionRequired())
        BSEndian(&count, EndianSizeTag<4>());
    ((BSSimpleArray<TESObjectWEAP*, 1024>*)&AdjacentNavMeshes)->SetSize(count, true);
    apChunkBuffer += 4;
    for (unsigned int adjacentIndex = 0; adjacentIndex < count; ++adjacentIndex)
    {
        unsigned int adjacentID = *(unsigned int*)apChunkBuffer;
        if (apFile->IsEndianConversionRequired())
            BSEndian(&adjacentID, EndianSizeTag<4>());
        TESForm::RemapFormIDForFile(adjacentID, apFile);
        if (adjacentID)
        {
            ((BSSimpleArray<NavMeshInfo*, 1024>*)&AdjacentNavMeshes)->operator[](adjacentIndex) =
                reinterpret_cast<NavMeshLookupView_00693d70*>(g_TES->GetActiveCell(adjacentID))->FindReferenceByFormId();
        }
        else
        {
            ((BSSimpleArray<NavMeshInfo*, 1024>*)&AdjacentNavMeshes)->operator[](adjacentIndex) = 0;
        }
        apChunkBuffer += 4;
    }

    if (aiFileVersionNumber >= 11)
    {
        count = 0;
        memcpy(&count, apChunkBuffer, 4);
        if (apFile->IsEndianConversionRequired())
            BSEndian(&count, EndianSizeTag<4>());
        ((BSSimpleArray<TESObjectWEAP*, 1024>*)&PreferredAdjacentNavMeshes)->SetSize(count, true);
        apChunkBuffer += 4;
        for (unsigned int preferredIndex = 0; preferredIndex < count; ++preferredIndex)
        {
            unsigned int preferredID = *(unsigned int*)apChunkBuffer;
            if (apFile->IsEndianConversionRequired())
                BSEndian(&preferredID, EndianSizeTag<4>());
            TESForm::RemapFormIDForFile(preferredID, apFile);
            if (preferredID)
            {
                ((BSSimpleArray<NavMeshInfo*, 1024>*)&PreferredAdjacentNavMeshes)->operator[](preferredIndex) =
                    reinterpret_cast<NavMeshLookupView_00693d70*>(g_TES->GetActiveCell(preferredID))->FindReferenceByFormId();
            }
            else
            {
                ((BSSimpleArray<NavMeshInfo*, 1024>*)&PreferredAdjacentNavMeshes)->operator[](preferredIndex) = 0;
            }
            apChunkBuffer += 4;
        }
    }

    memcpy(&count, apChunkBuffer, 4);
    if (apFile->IsEndianConversionRequired())
        BSEndian(&count, EndianSizeTag<4>());
    ((BSSimpleArray<TESObjectWEAP*, 1024>*)&ConnectedDoors)->SetSize(count, true);
    apChunkBuffer += 4;
    for (unsigned int doorIndex = 0; doorIndex < count; ++doorIndex)
    {
        unsigned int doorID = *(unsigned int*)apChunkBuffer;
        if (apFile->IsEndianConversionRequired())
            BSEndian(&doorID, EndianSizeTag<4>());
        TESForm::RemapFormIDForFile(doorID, apFile);
        ((BSSimpleArray<NavMeshInfo*, 1024>*)&ConnectedDoors)->operator[](doorIndex) = (NavMeshInfo*)doorID;
        apChunkBuffer += 4;
    }
}

// @fnv 0x006b5ab0
// pdb: NavMeshInfo::CopyTo | NavMeshInfo.obj | retail, upstream name already matches, high
// pdb params: apDestInfo
void NavMeshInfo::CopyTo(NavMeshInfo* apDestInfo)
{
    apDestInfo->NavMeshID = NavMeshID;
    apDestInfo->ParentSpaceID = ParentSpaceID;
    apDestInfo->uiFlags = uiFlags;
    apDestInfo->iCellKey = iCellKey;
    apDestInfo->ApproxLocation = ApproxLocation;
    apDestInfo->pParentSpace = pParentSpace;
    apDestInfo->pNavMesh = pNavMesh;
    ((BSSimpleArray<unsigned int, 1024>*)&AdjacentNavMeshes)
        ->CopyTo((BSSimpleArray<unsigned int, 1024>*)&apDestInfo->AdjacentNavMeshes);
    ((BSSimpleArray<unsigned int, 1024>*)&PreferredAdjacentNavMeshes)
        ->CopyTo((BSSimpleArray<unsigned int, 1024>*)&apDestInfo->PreferredAdjacentNavMeshes);
    ((BSSimpleArray<unsigned int, 1024>*)&ConnectedDoors)
        ->CopyTo((BSSimpleArray<unsigned int, 1024>*)&apDestInfo->ConnectedDoors);
    if (pBounds)
    {
        if (!apDestInfo->pBounds)
            apDestInfo->pBounds = new NavMeshBounds;
        pBounds->CopyTo(apDestInfo->pBounds);
    }
    else
    {
        if (apDestInfo->pBounds)
        {
            delete apDestInfo->pBounds;
        }
        apDestInfo->pBounds = 0;
    }
}

// Initializes the array's vtable and calls FUN_006b3eb0 with 0 and false.
// Default constructor for BSSimpleArray<NavMeshInfo*, 1024> that sets the vtable and calls FUN_006b3eb0 with 0 and false.
// @fnv 0x0069aee0
// pdb: BSSimpleArray<NavMeshInfo *,1024>::BSSimpleArray<NavMeshInfo *,1024> | NavMesh_Util.obj | debug, call graph, high
template <> BSSimpleArray<NavMeshInfo*, 1024>::BSSimpleArray()
{
    BSSimpleArray_float_1024_InitialSetup(0, false);
}

// Deletes this NavMeshInfo object by calling delete on itself.
// @fnv 0x006b6390
// pdb: NavMeshInfo::~NavMeshInfo | NavMeshInfo.obj | retail, call graph (shared callers), low
// pdb note: debug build links it to NavMeshInfo::`scalar deleting destructor'
inline void NavMeshInfo::Delete()
{
    delete this;
}

// Instantiate the compiler's deleting destructor through the native deletion member.
void (NavMeshInfo::*navmeshinfo_delete)() = &NavMeshInfo::Delete;

// Exe 0x0069c460: local storage view; GetFlags binds the folded DWORD-at-+8 accessor 0x0044ddc0.
struct NavmeshinfoOffset08View
{
    unsigned int GetFlags();
};

struct FUN_0069c460_shell
{
    int NavMeshBounds_GetBoundsChunkSize();
};

// Computes a size by summing fixed offsets with flag counts from the ArrSI objects at offsets 0x28 and 0x18 scaled by 0xc and 6.
// @fnv 0x0069c460
// src: XBOX_PDB 0x0069c460
// pdb: NavMeshBounds::GetBoundsChunkSize | NavMeshBounds.obj | debug, upstream name already matches, high
// pdb locals: isize
int FUN_0069c460_shell::NavMeshBounds_GetBoundsChunkSize()
{
    int result = 0;
    result += 0x18;
    result += 2;
    result += 2;
    result += ((NavmeshinfoOffset08View*)((char*)this + 0x28))->GetFlags() * 0xc;
    result += ((NavmeshinfoOffset08View*)((char*)this + 0x18))->GetFlags() * 6;
    return result;
}

namespace fnv_006b4e10 {
// Exe 0x006b4e10: local storage view; GetFlags binds the folded DWORD-at-+8 accessor 0x0044ddc0.
struct NavmeshinfoOffset08View
{
    unsigned int GetFlags();
};

class FUN_006b4e10_shell
{
  public:
    unsigned int NavMeshInfo_GetMeshDataSize();
};

// @fnv 0x006b4e10
// src: COMMUNITY 0x006b4e10
// pdb: NavMeshInfo::GetPortalChunkSize | NavMeshInfo.obj | debug, object-file gap fill, medium
// pdb locals: isize
unsigned int FUN_006b4e10_shell::NavMeshInfo_GetMeshDataSize()
{
    unsigned int v = 0;
    v += 4;
    v += ((NavmeshinfoOffset08View*)((char*)this + 0x24))->GetFlags() * 4;
    v += 4;
    v += ((NavmeshinfoOffset08View*)((char*)this + 0x34))->GetFlags() * 4;
    v += 4;
    v += ((NavmeshinfoOffset08View*)((char*)this + 0x44))->GetFlags() * 4;
    return v;
}
}  // namespace fnv_006b4e10

namespace fnv_006b5a90 {
// Exe 0x006b5a90: local storage view; GetFlags binds the folded DWORD-at-+8 accessor 0x0044ddc0.
struct NavmeshinfoOffset08View
{
    unsigned int GetFlags();
};

struct FUN_006b5a90_shell
{
    unsigned int NavMesh_GetNumDoorPortals();
};

// @fnv 0x006b5a90
// src: COMMUNITY 0x006b5a90
unsigned int FUN_006b5a90_shell::NavMesh_GetNumDoorPortals()
{
    return ((NavmeshinfoOffset08View*)((char*)this + 0x58))->GetFlags();
}
}  // namespace fnv_006b5a90

struct FUN_006b4850_t : public NavMeshBounds
{
    void FUN_0069bda0();
    void* NavMeshBounds_Destroy(unsigned int flags);
};
void __cdecl _Deallocate(void*);
// Calls FUN_0069bda0, then deallocates this if flags has bit 1 set, and returns this.
// @fnv 0x006b4850
// src: COMMUNITY 0x006b4850
// pdb: NavMeshBounds::~NavMeshBounds | NavMeshBounds.obj | retail, call graph (shared callers), low
// pdb note: debug build links it to NavMeshBounds::`scalar deleting destructor'
void* FUN_006b4850_t::NavMeshBounds_Destroy(unsigned int flags)
{
    FUN_0069bda0();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

bool TESForm_QEndianSwapOnSave();
struct NavMeshInfoHelper
{
    bool AcquireNavMesh(NavMeshPtr& ptr);
};
struct Arr
{
    unsigned int* GetAt(int index);
};

struct NavMeshInfoSaveShell : NavMeshInfo
{
    void SaveToBuffer(char* apData);
};

// @fnv 0x006b4e80
// pdb: NavMeshInfo::FillPortalChunk | NavMeshInfo.obj | debug, object-file gap fill, medium
// pdb params: apChunkBuffer
// pdb locals: itempint, isize, pinfo, pdoor, i, spmesh
void NavMeshInfoSaveShell::SaveToBuffer(char* apData)
{
    TESObjectREFR* pRefr;
    {
        NavMeshInfo* pInfo;
        {
            unsigned int ItemUpdateArray;
            {
                unsigned int uiTemp;

                uiTemp = ((NavmeshinfoOffset08View*)&AdjacentNavMeshes)->GetFlags();
                if (TESForm_QEndianSwapOnSave())
                    BSEndian(&uiTemp, EndianSizeTag<4>());
                *(unsigned int*)apData = uiTemp;
                apData += 4;
                ItemUpdateArray = ((NavmeshinfoOffset08View*)&AdjacentNavMeshes)->GetFlags();
                pInfo = 0;
                for (unsigned int i = 0; i < ItemUpdateArray; i++)
                {
                    pInfo = (NavMeshInfo*)*((Arr*)&AdjacentNavMeshes)->GetAt(i);
                    NavMeshPtr navMeshPtr;
                    if (((NavMeshInfoHelper*)pInfo)->AcquireNavMesh(navMeshPtr))
                    {
                        uiTemp = ((TESForm*)(unsigned int)navMeshPtr)->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
                        if (TESForm_QEndianSwapOnSave())
                            BSEndian(&uiTemp, EndianSizeTag<4>());
                        *(unsigned int*)apData = uiTemp;
                    }
                    apData += 4;
                }

                uiTemp = ((NavmeshinfoOffset08View*)&PreferredAdjacentNavMeshes)->GetFlags();
                if (TESForm_QEndianSwapOnSave())
                    BSEndian(&uiTemp, EndianSizeTag<4>());
                *(unsigned int*)apData = uiTemp;
                apData += 4;
                ItemUpdateArray = ((NavmeshinfoOffset08View*)&PreferredAdjacentNavMeshes)->GetFlags();
                pInfo = 0;
                for (unsigned int j = 0; j < ItemUpdateArray; j++)
                {
                    pInfo = (NavMeshInfo*)*((Arr*)&PreferredAdjacentNavMeshes)->GetAt(j);
                    NavMeshPtr navMeshPtr;
                    if (((NavMeshInfoHelper*)pInfo)->AcquireNavMesh(navMeshPtr))
                    {
                        uiTemp = ((TESForm*)(unsigned int)navMeshPtr)->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
                        if (TESForm_QEndianSwapOnSave())
                            BSEndian(&uiTemp, EndianSizeTag<4>());
                        *(unsigned int*)apData = uiTemp;
                    }
                    apData += 4;
                }

                uiTemp = ((NavmeshinfoOffset08View*)&ConnectedDoors)->GetFlags();
                if (TESForm_QEndianSwapOnSave())
                    BSEndian(&uiTemp, EndianSizeTag<4>());
                *(unsigned int*)apData = uiTemp;
                apData += 4;
                ItemUpdateArray = ((NavmeshinfoOffset08View*)&ConnectedDoors)->GetFlags();
                pRefr = 0;
                for (unsigned int k = 0; k < ItemUpdateArray; k++)
                {
                    pRefr = (TESObjectREFR*)*((Arr*)&ConnectedDoors)->GetAt(k);
                    if (pRefr)
                    {
                        uiTemp = ((TESForm*)pRefr)->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
                        if (TESForm_QEndianSwapOnSave())
                            BSEndian(&uiTemp, EndianSizeTag<4>());
                        *(unsigned int*)apData = uiTemp;
                    }
                    else
                    {
                        *(unsigned int*)apData = 0;
                    }
                    apData += 4;
                }
            }
        }
    }
}

struct FUN_0069c4c0_shell
{
    void WriteToBuffer(char* buffer);
};

void* __cdecl __memcpy(void* destination, const void* source, unsigned int size);

struct FUN_006b48f0_shell : public NavMeshInfo
{
    void WriteToChunkBuffer(char* apChunkBuffer);
};

// Serializes uiFlags, form IDs, a NiPoint3, optional bounds data and a float into apChunkBuffer, byte-swapping each value when FUN_00401500 is true.
// @fnv 0x006b48f0
// pdb: NavMeshInfo::FillInfoChunk | NavMeshInfo.obj | debug, object-file gap fill, medium
// pdb params: apChunkBuffer
// pdb locals: itempint, ftempFloat, temppoint
void FUN_006b48f0_shell::WriteToChunkBuffer(char* apChunkBuffer)
{
    unsigned int itempint;
    float ftempFloat;
    // Raw storage avoids a default-constructor call in this byte-preserving serializer.
    float temppoint[sizeof(NiPoint3) / sizeof(float)];

    itempint = this->uiFlags;
    if (TESForm_QEndianSwapOnSave())
        BSEndian(&itempint, EndianSizeTag<4>());
    __memcpy(apChunkBuffer, &itempint, 4);
    apChunkBuffer += 4;

    if (this->pNavMesh)
    {
        if (reinterpret_cast<TESForm*>(this->pNavMesh)->GetHavokWorldObjectCountInSceneGraph_0084e3a0() != this->NavMeshID)
            this->NavMeshID = reinterpret_cast<TESForm*>(this->pNavMesh)->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    }

    itempint = this->NavMeshID;
    if (TESForm_QEndianSwapOnSave())
        BSEndian(&itempint, EndianSizeTag<4>());
    __memcpy(apChunkBuffer, &itempint, 4);
    apChunkBuffer += 4;

    this->ParentSpaceID = this->pParentSpace->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    itempint = this->ParentSpaceID;
    if (TESForm_QEndianSwapOnSave())
        BSEndian(&itempint, EndianSizeTag<4>());
    __memcpy(apChunkBuffer, &itempint, 4);
    apChunkBuffer += 4;

    itempint = this->iCellKey;
    if (TESForm_QEndianSwapOnSave())
        BSEndian(&itempint, EndianSizeTag<4>());
    __memcpy(apChunkBuffer, &itempint, 4);
    apChunkBuffer += 4;

    *reinterpret_cast<NiPoint3*>(temppoint) = this->ApproxLocation;
    if (TESForm_QEndianSwapOnSave())
    {
        BSEndian(reinterpret_cast<unsigned int*>(&temppoint[0]), EndianSizeTag<4>());
        BSEndian(reinterpret_cast<unsigned int*>(&temppoint[1]), EndianSizeTag<4>());
        BSEndian(reinterpret_cast<unsigned int*>(&temppoint[2]), EndianSizeTag<4>());
    }
    __memcpy(apChunkBuffer, &temppoint, 12);
    apChunkBuffer += 12;

    if (this->pBounds)
    {
        reinterpret_cast<FUN_0069c4c0_shell*>(this->pBounds)->WriteToBuffer(apChunkBuffer);
        apChunkBuffer += reinterpret_cast<FUN_0069c460_shell*>(this->pBounds)->NavMeshBounds_GetBoundsChunkSize();
    }

    ftempFloat = this->fPreferredPercent;
    if (TESForm_QEndianSwapOnSave())
        BSEndian(reinterpret_cast<unsigned int*>(&ftempFloat), EndianSizeTag<4>());
    __memcpy(apChunkBuffer, &ftempFloat, 4);
    apChunkBuffer += 4;
}

struct FUN_006b4b50_shell : public NavMeshInfo
{
    void LoadNavMeshInfo(char*, TESFile*, int);
};

void* __cdecl __memcpy(void* destination, const void* source, unsigned int size);

struct FUN_0069c8f0_shell
{
    void NavMeshBounds_SetFromBoundsChunk(char* apChunkBuffer, TESFile* file, int aiFileVersionNumber);
};

// @fnv 0x006b4b50
// pdb: NavMeshInfo::SetFromInfoChunk | NavMeshInfo.obj | debug, object-file gap fill, medium
// pdb params: apChunkBuffer, apFile, aiFileVersionNumber
void FUN_006b4b50_shell::LoadNavMeshInfo(char* apChunkBuffer, TESFile* apFile, int aiFileVersionNumber)
{
    __memcpy(&this->uiFlags, apChunkBuffer, 4);
    if (apFile->GetChunkData())
    {
        BSEndian(&this->uiFlags, EndianSizeTag<4>());
    }
    apChunkBuffer += 4;

    __memcpy(&this->NavMeshID, apChunkBuffer, 4);
    if (apFile->GetChunkData())
    {
        BSEndian(&this->NavMeshID, EndianSizeTag<4>());
    }
    apChunkBuffer += 4;

    TESForm::RemapFormIDForFile(this->NavMeshID, apFile);

    __memcpy(&this->ParentSpaceID, apChunkBuffer, 4);
    if (apFile->GetChunkData())
    {
        BSEndian(&this->ParentSpaceID, EndianSizeTag<4>());
    }
    apChunkBuffer += 4;

    TESForm::RemapFormIDForFile(this->ParentSpaceID, apFile);

    __memcpy(reinterpret_cast<unsigned int*>(&this->iCellKey), apChunkBuffer, 4);
    if (apFile->GetChunkData())
    {
        BSEndian(reinterpret_cast<unsigned int*>(&this->iCellKey), EndianSizeTag<4>());
    }
    apChunkBuffer += 4;

    __memcpy(&this->ApproxLocation, apChunkBuffer, 12);
    if (apFile->GetChunkData())
    {
        BSEndian(reinterpret_cast<unsigned int*>(&this->ApproxLocation.x), EndianSizeTag<4>());
        BSEndian(reinterpret_cast<unsigned int*>(&this->ApproxLocation.y), EndianSizeTag<4>());
        BSEndian(reinterpret_cast<unsigned int*>(&this->ApproxLocation.z), EndianSizeTag<4>());
    }
    apChunkBuffer += 12;

    if ((this->uiFlags & 0x20) != 0)
    {
        if (!this->pBounds)
            this->pBounds = new NavMeshBounds;

        ((FUN_0069c8f0_shell*)this->pBounds)->NavMeshBounds_SetFromBoundsChunk(apChunkBuffer, apFile, aiFileVersionNumber);
        apChunkBuffer += ((FUN_0069c460_shell*)this->pBounds)->NavMeshBounds_GetBoundsChunkSize();
    }

    if (aiFileVersionNumber >= 10)
    {
        __memcpy(reinterpret_cast<unsigned int*>(&this->fPreferredPercent), apChunkBuffer, 4);
        if (apFile->GetChunkData())
        {
            BSEndian(reinterpret_cast<unsigned int*>(&this->fPreferredPercent), EndianSizeTag<4>());
        }
        apChunkBuffer += 4;
    }
}

// FUN_0069bcf0 - default constructor of a non-polymorphic class.
// Disassembly facts:
//   - __thiscall, no stack args, returns this (mov eax,[ebp-0x10] epilogue)
//   - no vtable store -> non-polymorphic
//   - ctor-call on this+0x00 and this+0x0C with 3 float args each
//     (globals D_01015f5c x3 then D_01016970 x3)
//   - ctor-call on this+0x18 (0x69cd10), no args
//   - EH scope state 0 set right after, covering only the call on
//     this+0x28 (0x49f1c0) -> member at +0x18 has a non-trivial dtor
//     (unwind funclet destroys it), member at +0x28 ctor is last.

extern float D_01015f5c;
extern float D_01016970;

struct FUN_0069cd10
{
    void* unk00;
    void* unk04;
    void* unk08;
    void* unk0C;
    FUN_0069cd10();
    ~FUN_0069cd10();
};

struct FUN_0049f1c0
{
    void* unk00;
    FUN_0049f1c0();
};

class FUN_0069bcf0
{
  public:
    NiPoint3 unk00;
    NiPoint3 unk0C;
    FUN_0069cd10 unk18;
    FUN_0049f1c0 unk28;

    FUN_0069bcf0();
};

// @fnv 0x0069bcf0
// pdb: NavMeshBounds::NavMeshBounds | NavMeshBounds.obj | debug, call graph, medium
FUN_0069bcf0::FUN_0069bcf0() : unk00(D_01015f5c, D_01015f5c, D_01015f5c), unk0C(D_01016970, D_01016970, D_01016970)
{}

struct MeshCell
{
    int FUN_0054ddd0();
    int FUN_00544c60();
    int FUN_00544c30();
};
struct MeshTriangle
{
    bool NavMeshTriangle_HasEdgeExtraInfo();
    bool HasFlag40();
    bool IsEdgeFlagSet(int);
    unsigned short GetEdgeExtraInfo_0068f2c0(int);
};
struct NavInfo;
struct NavInfoPointer
{
    NavInfo* pointer;
    bool NavMeshTriangleEdgePortal_IsValid();
};
struct MeshLink
{
    int unk00;
    NavInfoPointer info;
};
struct MeshSource
{
    TESForm* GetDoorForm();
    unsigned int NavMesh_GetTriangleCount();
    MeshTriangle* FUN_005582f0(unsigned short);
    MeshLink* GetEdge(unsigned short);
    unsigned int NavMesh_GetNumDoorPortals();
    NavInfo* const& FUN_0068f6b0(unsigned short);
    MeshCell* FUN_0059bb30();
    unsigned int GetArrFlags();
    const NiPoint3& GetVertex_0068f0a0(unsigned short);
    bool FUN_00440d80();
};
struct MeshArray
{
    int unk00;
    NavInfo** buffer;
    unsigned int size, capacity;
    void FUN_008454f0(bool);
    unsigned int FUN_0044ddc0();
    NavInfo*& FUN_00877a30(unsigned int);
    void BSSimpleArray_Actor_1024_RemoveFast(unsigned int, bool);
    void FUN_007cb2e0(NavInfo* const&);
};
struct NavInfo
{
    unsigned int id, parentId, flags;
    int cellKey;
    NiPoint3 point;
    TESForm* parent;
    float preferred;
    MeshArray adjacent, preferredAdjacent, doors;
    void* mesh;
    NavMeshBounds* bounds;
    bool FUN_00690860(NavMeshPtr&);
    void FUN_00693ef0(bool);
    void BaseProcess_SetLastTimeProcessed(float);
    void FUN_004534f0(bool);
    void NavMeshInfo_CreateFromNavMesh(MeshSource*);
};
extern NiPoint3 DAT_011f426c;
int PackShortPair(int, int);
// Resets this NavInfo and rebuilds its id, parent, adjacency, doors, cell key, point and preferred ratio from the given mesh source.
// @fnv 0x006b5560
// src: XBOX_RETAIL_PDB 0x006b5560
// pdb: NavMeshInfo::CreateFromNavMesh | NavMeshInfo.obj | retail, call graph, high
// pdb params: apNavMesh
// pdb locals: isize, iverts, itrisize, i, btriPreferred, ptri, e, pextraInfo, spotherMesh, baddPortal, iotherSize, k, ipreferredTris
void NavInfo::NavMeshInfo_CreateFromNavMesh(MeshSource* apNavMesh)
{
    id = reinterpret_cast<TESForm*>(apNavMesh)->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    parent = apNavMesh->GetDoorForm();
    parentId = apNavMesh->GetDoorForm()->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    adjacent.FUN_008454f0(true);
    preferredAdjacent.FUN_008454f0(true);
    doors.FUN_008454f0(true);
    flags = 0;
    if (bounds)
    {
        delete bounds;
        bounds = 0;
    }
    if (reinterpret_cast<TESForm*>(apNavMesh)->FUN_00440da0())
        FUN_00693ef0(true);
    {
        unsigned int count = apNavMesh->NavMesh_GetTriangleCount();
        for (unsigned int i = 0; i < count; ++i)
        {
            MeshTriangle* triangle = apNavMesh->FUN_005582f0((unsigned short)i);
            if (!triangle->NavMeshTriangle_HasEdgeExtraInfo())
                continue;
            bool isPreferred = triangle->HasFlag40();
            for (int edge = 0; edge < 3; ++edge)
            {
                if (!triangle->IsEdgeFlagSet(edge))
                    continue;
                MeshLink* link = apNavMesh->GetEdge(triangle->GetEdgeExtraInfo_0068f2c0(edge));
                if (link->info.NavMeshTriangleEdgePortal_IsValid())
                {
                    bool add3 = true;
                    NavMeshPtr reference;
                    if (link->info.pointer->FUN_00690860(reference))
                    {
                        if (reinterpret_cast<MeshSource*>(reference.Modelloader_GetReferencedThis())->FUN_00440d80())
                            add3 = false;
                        else if (isPreferred)
                        {
                            unsigned int size = adjacent.FUN_0044ddc0();
                            for (unsigned int j = 0; j < size; ++j)
                                if (adjacent.FUN_00877a30(j) == link->info.pointer)
                                {
                                    adjacent.BSSimpleArray_Actor_1024_RemoveFast(j, true);
                                    break;
                                }
                            size = preferredAdjacent.FUN_0044ddc0();
                            for (unsigned int k = 0; k < size; ++k)
                                if (preferredAdjacent.FUN_00877a30(k) == link->info.pointer)
                                {
                                    add3 = false;
                                    break;
                                }
                        }
                        else
                        {
                            unsigned int sizeOther = adjacent.FUN_0044ddc0();
                            for (unsigned int m = 0; m < sizeOther; ++m)
                                if (adjacent.FUN_00877a30(m) == link->info.pointer)
                                {
                                    add3 = false;
                                    break;
                                }
                        }
                    }
                    else
                        add3 = false;
                    if (add3)
                    {
                        if (isPreferred)
                            preferredAdjacent.FUN_007cb2e0(link->info.pointer);
                        else
                            adjacent.FUN_007cb2e0(link->info.pointer);
                    }
                }
            }
        }
        if (count == 0)
            flags |= 8;
        else
            flags &= ~8;
        count = apNavMesh->NavMesh_GetNumDoorPortals();
        for (unsigned int d = 0; d < count; ++d)
            doors.FUN_007cb2e0(apNavMesh->FUN_0068f6b0((unsigned short)d));
    }
    if (apNavMesh->FUN_0059bb30()->FUN_0054ddd0())
        cellKey = PackShortPair(apNavMesh->FUN_0059bb30()->FUN_00544c30(), apNavMesh->FUN_0059bb30()->FUN_00544c60());
    else
        cellKey = 0;
    point = DAT_011f426c;
    unsigned int vertices = apNavMesh->GetArrFlags();
    if (vertices > 0)
    {
        for (unsigned int v = 0; v < vertices; ++v)
            point.ComputeValue_0063c8a0(apNavMesh->GetVertex_0068f0a0((unsigned short)v));
        point.ScaleByInverse((float)vertices);
    }
    unsigned int triangles = apNavMesh->NavMesh_GetTriangleCount();
    if (triangles)
    {
        unsigned int preferredCount = 0;
        for (unsigned int t = 0; t < triangles; ++t)
        {
            MeshTriangle* triangle2 = apNavMesh->FUN_005582f0((unsigned short)t);
            if (triangle2->HasFlag40())
                ++preferredCount;
        }
        BaseProcess_SetLastTimeProcessed((float)preferredCount / (float)triangles);
    }
    FUN_004534f0(true);
}
