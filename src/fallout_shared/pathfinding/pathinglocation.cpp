#include <new>
// fallout_shared/pathfinding/pathinglocation.cpp: PathingLocation and pathfinding helpers (34 functions)

#include "pathinglocation_decls.h"
#include "../../../include/BGSSaveFormBuffer.h"

// @fnv 0x004ff7e0
PathingLocation::~PathingLocation()
{
}

// @fnv 0x006dce10
// pdb: PathingLocation::PathingLocation | PathingLocation.obj | debug, call graph, high
// pdb params: aLocation, apRef
PathingLocation::PathingLocation(const NiPoint3& aLocation, TESObjectREFR* apRef)
{
    SetLocationData(aLocation, reinterpret_cast<TESObjectCELL*>(reinterpret_cast<LowProcess*>(apRef)->LowProcess::GetTarget()),
                    apRef->GetParentWorldSpace());
}

static const int kClearTriangleDataValue1 = 65535;

// @fnv 0x006ddbe0
void PathingLocation::ResetLocation()
{
    usTriangle = kClearTriangleDataValue1;
}

// Copies Location, the Id fields, pCell and uiFlags from other into this PathingLocation and returns *this.
// @fnv 0x006dcce0
// pdb: PathingLocation::QValid | PathingSearchRayCast.obj | debug, call graph, low
PathingLocation& PathingLocation::operator=(const PathingLocation& other)
{
    Location = other.Location;
    pNavMesh = other.pNavMesh;
    pNavMeshes = other.pNavMeshes;
    pCell = other.pCell;
    pWorldSpace = other.pWorldSpace;
    iCellKey = other.iCellKey;
    usTriangle = other.usTriangle;
    uiFlags = other.uiFlags;
    return *this;
}

// @fnv 0x006dd4f0
// pdb: PathingLocation::GetCell | PathingLocation.obj | retail, call graph, medium
// pdb locals: pcell
TESObjectCELL* PathingLocation::GetContainingCell()
{
    if (pCell)
        return pCell;
    else if (pWorldSpace)
    {
        TESObjectCELL* cell = pWorldSpace->GetCellFromKey(iCellKey);
        if (cell)
            return cell;
    }
    return 0;
}

// @fnv 0x006dd610
// pdb: PathingLocation::GetNavMesh | PathingLocation.obj | debug, call graph, high
// pdb params: aRetNavMesh
bool PathingLocation::ResolveNavMesh(NavMeshPtr& aRetNavMesh)
{
    if (pNavMesh)
        return const_cast<NavMeshInfo*>(pNavMesh)->ResolveNavMesh(aRetNavMesh);
    else
        return false;
}

// If a NavMeshInfo is set and its triangle index is valid, checks it against navMesh, writes the triangle index to triangle and returns true.
// @fnv 0x006dd640
// pdb: PathingLocation::GetNavMeshAndTriangle | PathingLocation.obj | debug, call graph, high
// pdb params: aRetNavMesh, aRetTriangle
bool PathingLocation::FindNavMeshTriangle(NavMeshPtr& aRetNavMesh, unsigned short& aRetTriangle)
{
    if (pNavMesh)
    {
        if (usTriangle == 0xFFFF)
            return false;
        if (!const_cast<NavMeshInfo*>(pNavMesh)->ResolveNavMesh(aRetNavMesh))
            return false;
        aRetTriangle = usTriangle;
        return true;
    }
    else
        return false;
}

// @fnv 0x006dd540
// pdb: PathingLocation::GetBestNavMeshInfo | PathingLocation.obj | debug, call graph (shared callers), medium
void* PathingLocation::GetAssociatedNavMesh()
{
    if (pNavMesh)
        return const_cast<NavMeshInfo*>(pNavMesh);
    else if (!pNavMeshes)
        return 0;
    else if (IsPathingLocationFlagSet())
        return (void*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(0);
    else
        return 0;
}

// Placement-constructs count PathingLocation objects in the items array.
// @fnv 0x006d00a0
// pdb: BSSimpleArray<PathingLocation,1024>::_ConstructItems | PathBuilder.obj | debug, call graph (shared callers), high
// pdb params: apFirstAddr, aiCount
// pdb locals: i
void PathingLocationArray::ConstructItems(PathingLocation* apFirstAddr, unsigned int aiCount)
{
    for (unsigned int i = 0; i < aiCount; ++i)
        new (apFirstAddr + i) PathingLocation;
}

// Reads a PathingLocation from a load-game buffer: location, navmesh id, cell/worldspace, flags and optional mesh data.
// @fnv 0x006df010
// pdb: PathingLocation::LoadGame | PathingLocation.obj | retail, upstream name already matches, high
// pdb params: apLoadGameBuffer
// pdb locals: inavmeshID, bhadNavMeshesArray
void PathingLocation::LoadGame(BGSLoadGameBuffer* apLoadGameBuffer)
{
    apLoadGameBuffer->ReadDataBlock(&Location, 12);
    unsigned int navMeshID = 0;
    ((BGSLoadFormBuffer*)apLoadGameBuffer)->ReadFormIdentifier(&navMeshID);
    pNavMesh = g_TES->GetActiveCell(navMeshID)->FindReferenceByFormId();
    pCell = dynamic_cast<TESObjectCELL*>(TESForm::GetFormByID(apLoadGameBuffer->ReadFormIdentifier()));
    pWorldSpace = dynamic_cast<TESWorldSpace*>(TESForm::GetFormByID(apLoadGameBuffer->ReadFormIdentifier()));
    apLoadGameBuffer->ReadDataBlock(&iCellKey, 4);
    apLoadGameBuffer->ReadDataBlock(&usTriangle, 2);
    usTriangle = kClearTriangleDataValue1;
    apLoadGameBuffer->ReadDataBlock(&uiFlags, 1);
    bool hasMeshes = false;
    apLoadGameBuffer->ReadDataBlock(&hasMeshes, 1);
    if (hasMeshes)
    {
        if (pWorldSpace)
            pNavMeshes = ForwardToPathing(pWorldSpace, iCellKey);
        else if (pCell)
            pNavMeshes = Pathing_GetPotentialNavMeshInfoForLocation(pCell);
    }
}

// Checks whether the location is reachable via its nav mesh or mesh array, pruning invalid meshes and updating reachability state.
// @fnv 0x006de250
// pdb: PathingLocation::ResolveNavMeshInfoOnly | PathingLocation.obj | debug, call graph (shared callers), high
// pdb locals: allMeshes, i, pbounds, pmesh, spmesh
bool PathingLocation::HasPathingLocation()
{
    if (PathingLocation_QUnused())
        return false;
    if (!pNavMesh && !pNavMeshes)
        return false;
    else if (pNavMesh)
    {
        if (pNavMesh->NavMeshInfo_GetDisabled())
            return false;
        else
            return true;
    }
    else
    {
        PathBuilderDetailedPathInfos meshes;
        ((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->CopyTo((BSSimpleArray<unsigned int, 1024>*)&meshes);
        for (unsigned int i = 0; i < ((BSSimpleArray<SandboxProcedureData, 1024>*)&meshes)->GetSize(); ++i)
        {
            struct
            {
                NavMeshBounds* bounds;
                NavMeshInfo* info;
            } meshEntry;
            meshEntry.info = (NavMeshInfo*)((NiFormArray*)&meshes)->FUN_00877a30(i);
            meshEntry.bounds = meshEntry.info->GetBounds();
            if (meshEntry.bounds)
            {
                if (meshEntry.bounds->ContainsPoint(Location))
                {
                    ((BSSimpleArray<unsigned short, 1024>*)&meshes)->Clear(false);
                    ((BSSimpleArray<NavMeshInfo*, 1024>*)&meshes)->Add(meshEntry.info);
                    break;
                }
                else
                {
                    ((BSSimpleArray<TESLoadScreen*, 1024>*)&meshes)->UpdateEntryValue(i, 1);
                    --i;
                }
            }
            else
            {
                NavMeshPtr mesh;
                if (meshEntry.info->ResolveNavMesh(mesh) && static_cast<const TESForm*>(mesh.Modelloader_GetReferencedThis())->IsDeleted())
                {
                    ((BSSimpleArray<TESLoadScreen*, 1024>*)&meshes)->UpdateEntryValue(i, 1);
                    --i;
                }
            }
        }
        if (((BSSimpleArray<SandboxProcedureData, 1024>*)&meshes)->GetSize() == 1)
        {
            pNavMesh = meshes.GetFirstForm();
            pNavMeshes = 0;
            SetAllMeshesReachable(true);
            return true;
        }
        else
        {
            SetAllMeshesReachable(GetAllMeshesReachable());
            return IsPathingLocationFlagSet();
        }
    }
}

// @fnv 0x006dd6f0
// pdb: PathingLocation::ResolveNavMeshInfo | PathingLocation.obj | retail, call graph (shared callers), high
// pdb params: apFilter
// pdb locals: spmesh, bret, prevLoc, i, usfoundTri, imeshWithoutBounds, fBestDistanceSqr, ibestMesh, pmesh, pbounds, delta, fdistSqr
bool PathingLocation::FindContainingTriangle(FindTriangleForLocationFilter* apFilter)
{
    if (PathingLocation_QUnused())
        return false;
    if (!pNavMesh && !pNavMeshes)
        return false;
    else if (pNavMesh)
    {
        if (pNavMesh->NavMeshInfo_GetDisabled())
            return false;
        if (usTriangle == 0xffff)
        {
            NavMeshPtr mesh;
            if (const_cast<NavMeshInfo*>(pNavMesh)->ResolveNavMesh(mesh))
                usTriangle =
                    mesh.Modelloader_GetReferencedThis()->FindTriangleForLocation(Location, apFilter);
        }
        return true;
    }
    else if (GetContainingCell() && GetContainingCell()->IsOutdoorCell())
    {
        for (unsigned int i = 0; i < ((BSSimpleArray<SandboxProcedureData, 1024>*)pNavMeshes)->GetSize(); ++i)
        {
            NavMeshPtr mesh;
            if (((NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(static_cast<int>(i)))
                    ->ResolveNavMesh(mesh) &&
                !mesh.Modelloader_GetReferencedThis()->GetDisabled())
            {
                unsigned short triangle =
                    mesh.Modelloader_GetReferencedThis()->FindTriangleForLocation(Location, apFilter);
                if (triangle != 0xffff)
                {
                    pNavMesh =
                        (NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(static_cast<int>(i));
                    pNavMeshes = 0;
                    usTriangle = triangle;
                    SetAllMeshesReachable(true);
                    return true;
                }
            }
        }
        NiPoint3 savedLocation = Location;
        bool resolved = IsPathingLocationValid();
        usTriangle = kClearTriangleDataValue1;
        Location = savedLocation;
        if (resolved)
            SetAllMeshesReachable(true);
        else
            SetAllMeshesReachable(IsPathingLocationFlagSet());
        return resolved;
    }
    else
    {
        for (unsigned int i = 0; i < ((BSSimpleArray<SandboxProcedureData, 1024>*)pNavMeshes)->GetSize(); ++i)
        {
            const NavMeshInfo* info = (NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(static_cast<int>(i));
            if (!info->NavMeshInfo_GetDisabled())
            {
                NavMeshBounds* bounds = const_cast<NavMeshInfo*>(info)->GetBounds();
                if (bounds && bounds->ContainsPoint(Location))
                {
                    pNavMesh =
                        (NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(static_cast<int>(i));
                    pNavMeshes = 0;
                    NavMeshPtr mesh;
                    if (const_cast<NavMeshInfo*>(pNavMesh)->ResolveNavMesh(mesh))
                        usTriangle = mesh.Modelloader_GetReferencedThis()
                                             ->FindTriangleForLocation(Location, apFilter);
                    SetAllMeshesReachable(true);
                    return true;
                }
            }
        }
        struct
        {
            int noBoundsIndex;
            float minimumDistance;
            int closestIndex;
        } nearest;
        nearest.noBoundsIndex = -1;
        nearest.minimumDistance = 3.402823466e+38F;
        nearest.closestIndex = -1;
        for (unsigned int i = 0; i < ((BSSimpleArray<SandboxProcedureData, 1024>*)pNavMeshes)->GetSize(); ++i)
        {
            const NavMeshInfo* info = (NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(static_cast<int>(i));
            if (!info->NavMeshInfo_GetDisabled())
            {
                NavMeshBounds* bounds = const_cast<NavMeshInfo*>(info)->GetBounds();
                if (bounds)
                {
                    NiPoint3 delta = bounds->DistanceFromPointToBounds(Location);
                    float distance = delta.FUN_004a7290();
                    if (distance < nearest.minimumDistance)
                    {
                        nearest.minimumDistance = distance;
                        nearest.closestIndex = i;
                    }
                }
                else
                    nearest.noBoundsIndex = i;
            }
        }
        if (nearest.minimumDistance < DAT_0106cd60)
        {
            pNavMesh =
                (NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(nearest.closestIndex);
            pNavMeshes = 0;
            usTriangle = kClearTriangleDataValue1;
            SetAllMeshesReachable(true);
            return true;
        }
        else
        {
            pNavMesh =
                (NavMeshInfo*)((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->GetAt(nearest.noBoundsIndex);
            pNavMeshes = 0;
            SetAllMeshesReachable(GetAllMeshesReachable());
            return IsPathingLocationFlagSet();
        }
    }
}

// Clears bit 0 of the byte at offset 0x26 and sets it to the low bit of aiVal.
// @fnv 0x006dcfe0
void PathingLocation::SetAllMeshesReachable(int aiVal)
{
    uiFlags &= ~1;
    uiFlags |= aiVal;
}

// @fnv 0x006dd5a0
int __fastcall IsPathingLocationFlagSet(PathingLocation* pThis)
{
    return pThis->uiFlags & 1;
}

// @fnv 0x006df160
// pdb: BSSimpleArray<PathSmoother::PathEdge *,1024>::GetFirst | PathSmoother.obj | debug, call graph, medium
TESForm*& __fastcall GetFirstForm(NiFormArray* pThis)
{
    return pThis->FUN_00877a30(0);
}

// @fnv 0x006dcc40
// pdb: PathingLocation::PathingLocation | PathingLocation.obj | debug, call graph, high
// pdb params: aLoc
PathingLocation::PathingLocation(const PathingLocation& aLoc)
{
    Location = aLoc.Location;
    pNavMesh = aLoc.pNavMesh;
    pNavMeshes = aLoc.pNavMeshes;
    pCell = aLoc.pCell;
    pWorldSpace = aLoc.pWorldSpace;
    iCellKey = aLoc.iCellKey;
    usTriangle = aLoc.usTriangle;
    uiFlags = aLoc.uiFlags;
}

// @fnv 0x006dcf20
// pdb: PathingLocation::PathingLocation | PathingLocation.obj | retail, call graph (shared callers), high
PathingLocation::PathingLocation(const NiPoint3& aLocation, const NavMeshInfo* apNavMeshInfo)
{
    Location = aLocation;
    uiFlags = 0;
    pNavMesh = apNavMeshInfo;
    pNavMeshes = 0;
    pWorldSpace = const_cast<NavMeshInfo*>(apNavMeshInfo)->GetParentWorldSpace();
    if (pWorldSpace)
    {
        pCell = 0;
        iCellKey = apNavMeshInfo->iCellKey;
    }
    else
    {
        pCell = const_cast<NavMeshInfo*>(apNavMeshInfo)->GetInteriorCell();
        iCellKey = 0xdeaddead;
    }
    SetAllMeshesReachable(1);
    usTriangle = kClearTriangleDataValue1;
}

// @fnv 0x006dce60
// pdb: PathingLocation::PathingLocation | PathingLocation.obj | retail, object-file gap fill, high
PathingLocation::PathingLocation(NiPoint3 const& aLocation, TESObjectCELL* apCell)
{
    SetLocationData(aLocation, apCell, 0);
}

// @fnv 0x006dd4d0
// pdb: PathingLocation::SetTriangleIndex | PathingLocation.obj | debug, upstream name already matches, high
// pdb params: aiTriIndex
void PathingLocation::SetTriangleIndex(unsigned short aiTriIndex)
{
    usTriangle = aiTriIndex;
}

// Appends the navmesh info, or copies the navmesh info array, into the output array.
// @fnv 0x006dd5c0
bool PathingLocation::IsOnNavMesh(void* array)
{
    if (pNavMesh)
    {
        static_cast<BSSimpleArray<void*, 1024>*>(array)->Add(reinterpret_cast<void*&>(const_cast<NavMeshInfo*&>(pNavMesh)));
        return true;
    }
    else
    {
        if (((BSSimpleArray<unsigned int, 1024>*)pNavMeshes))
        {
            ((BSSimpleArray<unsigned int, 1024>*)pNavMeshes)->CopyTo((BSSimpleArray<unsigned int, 1024>*)static_cast<BSSimpleArray<void*, 1024>*>(array));
            return true;
        }
        else
        {
            return false;
        }
    }
}

// Initializes a pathing location from a point, using the hint location's navmesh triangle if found, otherwise setting up from the point and cell/worldspace.
// @fnv 0x006dd0d0
// pdb: PathingLocation::PathingLocation | PathingLocation.obj | retail, object-file run extension, high
PathingLocation::PathingLocation(const NiPoint3& aLocation, const PathingLocation& aHintLoc)
{
    Location = aLocation;
    uiFlags = 0;
    usTriangle = kClearTriangleDataValue1;
    NavMeshPtr pFoundNavMesh;
    if (const_cast<PathingLocation&>(aHintLoc).ResolveNavMesh(pFoundNavMesh))
    {
        usTriangle =
            pFoundNavMesh.Modelloader_GetReferencedThis()->FindTriangleForLocation(aLocation, 0);
        if (usTriangle != kClearTriangleDataValue1)
        {
            pNavMesh = aHintLoc.pNavMesh;
            pNavMeshes = 0;
            pCell = aHintLoc.pCell;
            pWorldSpace = aHintLoc.pWorldSpace;
            iCellKey = aHintLoc.iCellKey;
            return;
        }
    }
    SetLocationData(aLocation, const_cast<PathingLocation&>(aHintLoc).GetContainingCell(),
                          const_cast<PathingLocation&>(aHintLoc).GetParentWorldspace());
}

// @fnv 0x006dd3e0
// pdb: PathingLocation::SetNavMeshInfo | PathingLocation.obj | retail, upstream name already matches, high
// pdb params: apNavMeshInfo
void PathingLocation::SetNavMeshInfo(void* apNavMeshInfo)
{
    pNavMeshes = 0;
    pNavMesh = static_cast<const NavMeshInfo*>(apNavMeshInfo);
    pWorldSpace = static_cast<NavMeshInfo*>(apNavMeshInfo)->GetParentWorldSpace();
    if (pWorldSpace)
    {
        pCell = 0;
        iCellKey = static_cast<const NavMeshInfo*>(apNavMeshInfo)->iCellKey;
    }
    else
    {
        pCell = static_cast<NavMeshInfo*>(apNavMeshInfo)->GetInteriorCell();
        iCellKey = 0xdeaddead;
    }
    SetAllMeshesReachable(true);
    usTriangle = 65535;
}

// @fnv 0x006dd460
void PathingLocation::SetLocationSimple(void* point)
{
    Location = (*static_cast<const NiPoint3*>(point));
    usTriangle = 65535;
    if (pWorldSpace)
    {
        int key = TESWorldSpace::GetKeyForWorldCoord(Location);
        if (key != iCellKey)
            SetLocationData((*static_cast<const NiPoint3*>(point)), 0, pWorldSpace);
    }
}

// @fnv 0x006def40
// pdb: PathingLocation::SaveGame | PathingLocation.obj | retail, upstream name already matches, high
// pdb params: apSaveGameBuffer
// pdb locals: bhadNavMeshesArray
void PathingLocation::SaveGame(BGSSaveGameBuffer* apSaveGameBuffer)
{
    bool bhadNavMeshesArray;

    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SaveData(&Location, 12, 0);
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->FUN_00865db0(pNavMesh ? const_cast<NavMeshInfo*>(pNavMesh)->GetNavMeshID() : 0, 0);
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SerializeFormData(pCell, 0);
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SerializeFormData(pWorldSpace, 0);
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SaveData(&iCellKey, 4, 0);
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SaveData(&usTriangle, 2, 0);
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SaveData(&uiFlags, 1, 0);

    bhadNavMeshesArray = pNavMeshes;
    reinterpret_cast<BGSSaveFormBuffer*>(apSaveGameBuffer)->SaveData(&bhadNavMeshesArray, 1, 0);
}

unsigned int TESWorldSpace_GetKeyForWorldCoord(const NiPoint3&);
unsigned int PackShortPair(int, int);
const BSSimpleArray<const NavMeshInfo*, 1024>* ForwardToPathing(TESWorldSpace*, unsigned int);
const BSSimpleArray<const NavMeshInfo*, 1024>* Pathing_GetPotentialNavMeshInfoForLocation(TESObjectCELL*);

// Stores the position, resolves the nav-mesh array from the world or cell, marks all meshes reachable when exactly one mesh exists, and sets flags.
// @fnv 0x006dd280
// pdb: PathingLocation::SetupData | PathingLocation.obj | retail, call graph (shared callers), high
// pdb params: aLocation, apCell, apWorldSpace
void PathingLocation::SetLocationData(const NiPoint3& aLocation, TESObjectCELL* apCell, TESWorldSpace* apWorldSpace)
{
    Location = aLocation;
    uiFlags = 0;
    if (apWorldSpace)
    {
        pWorldSpace = apWorldSpace;
        iCellKey = TESWorldSpace_GetKeyForWorldCoord(Location);
        pCell = 0;
        pNavMeshes = ForwardToPathing(pWorldSpace, iCellKey);
    }
    else
    {
        pNavMeshes = Pathing_GetPotentialNavMeshInfoForLocation(apCell);
        if (apCell && !apCell->IsInterior())
        {
            pWorldSpace = apCell->GetParentWorldSpace();
            iCellKey = PackShortPair(apCell->GetCellGridX(), apCell->GetCellGridY());
            pCell = 0;
        }
        else
        {
            pCell = apCell;
            pWorldSpace = 0;
            iCellKey = 0xDEADDEAD;
        }
    }
    if (pNavMeshes && const_cast<BSSimpleArray<const NavMeshInfo*, 1024>*>(pNavMeshes)->GetSize() == 1)
    {
        SetAllMeshesReachable(true);
        pNavMesh = reinterpret_cast<const NavMeshInfo*>(
            const_cast<BSSimpleArray<const NavMeshInfo*, 1024>*>(pNavMeshes)->GetAt(0));
        pNavMeshes = 0;
    }
    else
    {
        SetAllMeshesReachable(false);
        pNavMesh = 0;
    }
    usTriangle = 0xFFFF;
}

// @fnv 0x006deeb0
// pdb: PathingLocation::IsCloseTo | PathingLocation.obj | debug, upstream name already matches, high
// pdb params: aOtherLoc, afTolerance
bool PathingLocation::IsCloseTo(void* aOtherLoc, float afTolerance)
{
    if (static_cast<const PathingLocation*>(aOtherLoc)->pWorldSpace != pWorldSpace)
    {
        return false;
    }
    if (pWorldSpace != 0)
    {
        if (static_cast<const PathingLocation*>(aOtherLoc)->iCellKey != iCellKey)
        {
            return false;
        }
    }
    else
    {
        if (static_cast<const PathingLocation*>(aOtherLoc)->pCell != pCell)
        {
            return false;
        }
    }
    if (static_cast<const PathingLocation*>(aOtherLoc)->Location.Sub(Location).FUN_004a7290() > afTolerance * afTolerance)
    {
        return false;
    }
    return true;
}

struct PathingLocationOwner
{
    void PathSmoother_AddPathingNode_0(PathingLocation* location);
    void PathSmoother_AddPathingNode(const NiPoint3& position, TESObjectREFR* reference);
};
// Constructs a pathing location from a position and reference, then passes it to the owner's location setter.
// @fnv 0x006ed730
// src: COMMUNITY 0x006ed730
// pdb: PathSmoother::AddPathingNode | PathSmoother.obj | debug, object-file gap fill, medium
// pdb params: aLocation, apInfo
// pdb locals: location
void PathingLocationOwner::PathSmoother_AddPathingNode(const NiPoint3& position, TESObjectREFR* reference)
{
    PathingLocation location(position, reinterpret_cast<const NavMeshInfo*>(reference));
    PathSmoother_AddPathingNode_0(&location);
}

struct FUN_006f4da0
{
    unsigned int flags;
    PathingLocation path;
    int context;
    FUN_006f4da0(int, int);
};

// Initialises the path from source, stores contextValue, sets flags to 6 and adds bit 1 when the path check for 0 succeeds.
// @fnv 0x006f4da0
// pdb: VirtualPathingNode::VirtualPathingNode | VirtualPathingNode.obj | debug, object-file run extension, low
// pdb params: aLocation, apDoorRef
FUN_006f4da0::FUN_006f4da0(int source, int contextValue) : path()
{
    path = *reinterpret_cast<const PathingLocation*>(source);
    context = contextValue;
    flags = 6;
    if (path.FUN_006dd6f0(0))
        flags |= 1;
}

struct MeshTriangle
{
    unsigned short v0;
    unsigned short v1;
    unsigned short v2;
};
class MeshTriangleArray
{
  public:
    unsigned int FUN_0044ddc0();                 // GetSize
    MeshTriangle* FUN_0069ccf0(unsigned int i);  // GetAt
};
class MeshVertexArray
{
  public:
    bool NiTListBase_DFALL_NiTListItem_BSAnimGroupSequence_const_BSAnimGroupSequence_const_IsEmpty();                      // IsEmpty
    NiPoint3* Object_GetIndexedValue_006a1440(unsigned int i);  // GetAt
};
bool __cdecl NavMeshMath_IsPointInsideTriangle(const NiPoint3* pkA, const NiPoint3* pkB, const NiPoint3* pkC, const NiPoint3* pkPoint,
                          float* pfHeight, float fAbove, float fBelow);

class MeshBounds
{
  public:
    NiPoint3 m_kMin;           // +0x00
    NiPoint3 m_kMax;           // +0x0c
    MeshTriangleArray m_kTris;  // +0x18
    int unk1c[3];
    MeshVertexArray m_kVerts;  // +0x28

    bool ContainsPoint(const NiPoint3* pkPoint);
};

// @fnv 0x0069bea0
// pdb: NavMeshBounds::IsPointInBounds | NavMeshBounds.obj | debug, object-file gap fill, medium
// pdb params: aPoint
// pdb locals: zdiff, maxzdelta, i, TriVert3, TriVert1, TriVert2
bool MeshBounds::ContainsPoint(const NiPoint3* pkPoint)
{
    if (pkPoint->x >= m_kMin.x && pkPoint->x <= m_kMax.x && pkPoint->y >= m_kMin.y && pkPoint->y <= m_kMax.y &&
        m_kMin.z - 180.0 <= pkPoint->z && m_kMax.z + 180.0 >= pkPoint->z)
    {
        if (!m_kVerts.NiTListBase_DFALL_NiTListItem_BSAnimGroupSequence_const_BSAnimGroupSequence_const_IsEmpty())
        {
            float m_fGlyphExtent = m_kMax.z - m_kMin.z;
            float fOut;
            for (unsigned int i = 0; i < m_kTris.FUN_0044ddc0(); i++)
            {
                NiPoint3 kA = *m_kVerts.Object_GetIndexedValue_006a1440(m_kTris.FUN_0069ccf0(i)->v0);
                NiPoint3 kB = *m_kVerts.Object_GetIndexedValue_006a1440(m_kTris.FUN_0069ccf0(i)->v1);
                NiPoint3 kC = *m_kVerts.Object_GetIndexedValue_006a1440(m_kTris.FUN_0069ccf0(i)->v2);
                if (NavMeshMath_IsPointInsideTriangle(&kA, &kB, &kC, pkPoint, &fOut, m_fGlyphExtent, m_fGlyphExtent))
                    return true;
            }
        }
        else
        {
            return true;
        }
    }
    return false;
}

extern const char DAT_0106cdec[];
extern const char DAT_0106cdd8[];
int __cdecl BSsprintf(char*, unsigned int, const char*, ...);
int __cdecl FUN_00406d50(char*, unsigned int, const char*);
int __cdecl BSstrcmp(const char*, const char*);

// NavMeshInfo parent-space accessor at 0x006b7770; the shared header has no declaration.
TESForm* __fastcall GetNavMeshParentSpace(NavMeshInfo*);
struct DebugMeshes
{
    unsigned int FUN_0044ddc0();
    NavMeshInfo*& BSSimpleArray_NiAVObject_1024_GetAt(unsigned int);
};

// Writes a formatted description of the location's cell, world, mesh or mesh list and coordinates into out up to capacity.
// @fnv 0x006de790
// src: XBOX_RETAIL_PDB 0x006de790
// pdb: PathingLocation::PrintDebugText | PathingLocation.obj | retail, unique string literal, high
// pdb params: apBuffer, aiMaxSize
// pdb locals: pcell, celly, cellx, navmeshinfo, pspace, navmeshBuffer, i, navmeshId
void PathingLocation::PrintDebugText(char* apBuffer, int aiMaxSize)
{
    if (!pNavMesh || (((DebugMeshes*)pNavMeshes) && !((DebugMeshes*)pNavMeshes)->FUN_0044ddc0()))
    {
        if (!pWorldSpace)
        {
            BSsprintf(apBuffer, aiMaxSize, "Cell %s, %.2f %.2f %.2f", pCell ? pCell->GetName() : "NONE", Location.x,
                         Location.y, Location.z);
        }
        else
        {
            int cellX2 = (int)Location.x >> 12;
            int y8 = (int)Location.y >> 12;
            TESForm* found8 = pWorldSpace->GetCellFromWorldCoord(Location);
            if (found8)
                BSsprintf(apBuffer, aiMaxSize, "Cell %s[%d,%d] %s,  %.2f %.2f %.2f", pWorldSpace->GetName(), cellX2, y8,
                             found8->GetName(), Location.x, Location.y, Location.z);
            else
                BSsprintf(apBuffer, aiMaxSize, "Cell %s[%d,%d], %.2f %.2f %.2f", pWorldSpace->GetName(), cellX2, y8,
                             Location.x, Location.y, Location.z);
        }
    }
    else
    {
        char meshName2[256];
        if (pNavMesh)
        {
            if (usTriangle != 65535)
                BSsprintf(apBuffer, aiMaxSize, DAT_0106cdec, meshName2, usTriangle, Location.x, Location.y, Location.z);
            else
                BSsprintf(apBuffer, aiMaxSize, DAT_0106cdd8, meshName2, Location.x, Location.y, Location.z);
        }
        else
        {
            char list7[256];
            BSsprintf(list7, aiMaxSize, "[");
            {
                unsigned int i3;
                for (i3 = 0; i3 < ((DebugMeshes*)pNavMeshes)->FUN_0044ddc0(); i3++)
                {
                    char entry2[64];
                    BSsprintf(entry2, 64, "%08x", ((DebugMeshes*)pNavMeshes)->BSSimpleArray_NiAVObject_1024_GetAt(i3)->GetNavMeshID());
                    FUN_00406d50(list7, 256, entry2);
                    if (i3 < ((DebugMeshes*)pNavMeshes)->FUN_0044ddc0() - 1)
                        FUN_00406d50(list7, 256, ", ");
                }
                FUN_00406d50(list7, 256, "]");
                TESWorldSpace* listWorld4 = ((DebugMeshes*)pNavMeshes)->BSSimpleArray_NiAVObject_1024_GetAt(0)->GetParentWorldSpace();
                TESForm* listCell3 = GetNavMeshParentSpace(((DebugMeshes*)pNavMeshes)->BSSimpleArray_NiAVObject_1024_GetAt(0));
                if (!listWorld4)
                {
                    BSsprintf(apBuffer, aiMaxSize, "%s Cell %s, %.2f %.2f %.2f", list7, listCell3->GetName(),
                                 Location.x, Location.y, Location.z);
                }
                else
                {
                    int gx = (int)Location.x >> 12;
                    int gy2 = (int)Location.y >> 12;
                    if (listCell3 && BSstrcmp(listCell3->GetName(), ""))
                        BSsprintf(apBuffer, aiMaxSize, "%s Cell %s[%d, %d](%s), %.2f %.2f %.2f", list7,
                                     listWorld4->GetName(), gx, gy2, listCell3->GetName(), Location.x, Location.y,
                                     Location.z);
                    else
                        BSsprintf(apBuffer, aiMaxSize, "%s Cell %s[%d, %d], %.2f %.2f %.2f", list7,
                                     listWorld4->GetName(), gx, gy2, Location.x, Location.y, Location.z);
                }
            }
        }
    }
}

bool __cdecl PathingLocationTestPoint(PathingLocation* aStartLocation, NiPoint3* aGoalLocation, int apReturnIntersectionLocation, int apFilter);

extern double _gModifierZero;

// @fnv 0x006ded70
// src: XBOX_DEBUG_PDB 0x006ded70
// pdb: PathingLocation::IsValidLocation | PathingLocation.obj | debug, call graph, high
// pdb params: afRadius
// pdb locals: ypoint1, xpoint1, ypoint2, xpoint2, point
bool PathingLocation::IsValidLocation(float afRadius)
{
    FUN_006dd6f0(0);
    if (!FUN_006dd6a0())
        return false;
    if (afRadius <= _gModifierZero)
        return true;
    NiPoint3 kLoc4 = FUN_005c3420();
    NiPoint3 kMinX2 = kLoc4;
    kMinX2.x -= afRadius;
    if (!PathingLocationTestPoint(this, &kMinX2, 0, 0))
        return false;
    NiPoint3 kMaxX7 = kLoc4;
    kMaxX7.x += afRadius;
    if (!PathingLocationTestPoint(this, &kMaxX7, 0, 0))
        return false;
    NiPoint3 kMinY = kLoc4;
    kMinY.y -= afRadius;
    if (!PathingLocationTestPoint(this, &kMinY, 0, 0))
        return false;
    NiPoint3 kMaxY6 = kLoc4;
    kMaxY6.y += afRadius;
    if (!PathingLocationTestPoint(this, &kMaxY6, 0, 0))
        return false;
    return true;
}

class NodeArr_cls
{
  public:
    void* unk00;
    NavMeshInfo** pBuffer;
    unsigned int iSize;
    unsigned int SimpleArrayStorage;
    unsigned int GetCount();
    NavMeshInfo*& GetAt(unsigned int index);
    int Find(NavMeshInfo** item, unsigned int start, int (*pred)());
};

class Owner_cls : public NodeArr_cls
{
  public:
    bool Check();
    NavMeshInfo** GetNodeRef(unsigned int index);
};

class Snap_cls : public NodeArr_cls
{
  public:
    Snap_cls();
    ~Snap_cls();
    void Add(NavMeshInfo** item);
};

int Actor_AreReferencedValuesEqual();

// @fnv 0x006de490
// pdb: PathingLocation::AreAllMeshesReachable | PathingLocation.obj | debug, object-file gap fill, medium
// pdb locals: reachableMeshes, bfoundMoreMeshes, isize, ipreferredSize, j, potherInfo, i, pinfo
bool PathingLocation::GetAllMeshesReachable()
{
    Snap_cls list;
    NavMeshInfo* itemB;
    NavMeshInfo* fCur;
    NavMeshInfo* item2;
    unsigned int aJ;
    unsigned int b;
    unsigned int iG;
    unsigned int j6;
    unsigned int k;
    bool changed2;
    int unk2C;

    if (pNavMesh != 0)
    {
        return true;
    }
    else
    {
        if (((Owner_cls*)pNavMeshes) != 0 && !((Owner_cls*)pNavMeshes)->Check())
        {
            list.Add(((Owner_cls*)pNavMeshes)->GetNodeRef(0));
            aJ = (*((Owner_cls*)pNavMeshes)->GetNodeRef(0))->AdjacentNavMeshes.GetSize();
            b = (*((Owner_cls*)pNavMeshes)->GetNodeRef(0))->PreferredAdjacentNavMeshes.GetSize();
            for (iG = 0; iG < aJ + b; iG++)
            {
                if (iG < aJ)
                {
                    itemB = (*((Owner_cls*)pNavMeshes)->GetNodeRef(0))->AdjacentNavMeshes.GetAt(iG);
                }
                else
                {
                    itemB = (*((Owner_cls*)pNavMeshes)->GetNodeRef(0))->PreferredAdjacentNavMeshes.GetAt(iG - aJ);
                }
                if (((Owner_cls*)pNavMeshes)->Find(&itemB, 0, Actor_AreReferencedValuesEqual) != -1)
                {
                    list.Add(&itemB);
                }
            }
            if (list.GetCount() == ((Owner_cls*)pNavMeshes)->GetCount())
            {
                return true;
            }
            do
            {
                changed2 = false;
                for (k = 1; k < list.GetCount(); k++)
                {
                    fCur = list.GetAt(k);
                    aJ = fCur->AdjacentNavMeshes.GetSize();
                    b = fCur->PreferredAdjacentNavMeshes.GetSize();
                    for (j6 = 0; j6 < fCur->AdjacentNavMeshes.GetSize(); j6++)
                    {
                        if (j6 < aJ)
                        {
                            item2 = fCur->AdjacentNavMeshes.GetAt(j6);
                        }
                        else
                        {
                            item2 = fCur->PreferredAdjacentNavMeshes.GetAt(j6 - aJ);
                        }
                        if (((Owner_cls*)pNavMeshes)->Find(&item2, 0, Actor_AreReferencedValuesEqual) != -1)
                        {
                            if (list.Find(&item2, 0, Actor_AreReferencedValuesEqual) == -1)
                            {
                                list.Add(&item2);
                                changed2 = true;
                            }
                        }
                    }
                }
            } while (changed2);
            if (list.GetCount() == ((Owner_cls*)pNavMeshes)->GetCount())
            {
                return true;
            }
        }
    }
    return false;
}

struct NavEdgeHit
{
    int m_iEdgeNumber;
    int unk04;
    NiPoint3 kPoint;
};

class NLMeshInfoArray
{
  public:
    unsigned int FUN_0044ddc0();                      // GetSize
    NavMeshInfo*& BSSimpleArray_NiAVObject_1024_GetAt(unsigned int uiIndex);  // GetAt
};

extern TES* DAT_011dea10;
extern const float DAT_01016970;  // FLT_MAX

// @fnv 0x006ddc00
// pdb: PathingLocation::ResolveToClosestNavmeshAndTriangle | PathingLocation.obj | retail, call graph (shared callers), high
// pdb locals: bpointInside, spmesh, res, center, isize, fminDistToPointSqr, usbestTri, ibestNavMesh, bunderWater, bbestTriWithinZRange, i, usthisNavMeshClosestTriIndex, pinfo, delta, fdistToPointSqr, fzdist, bthisTriInZRange
bool PathingLocation::IsPathingLocationValid()
{
    SetLocationData(Location, pCell, pWorldSpace);
    if (!pNavMesh && !pNavMeshes)
        return false;
    if (pNavMesh && usTriangle != kClearTriangleDataValue1)
        return true;

    if (pNavMesh)
    {
        bool bInside = false;
        NavMeshPtr kMeshPtr;
        if (const_cast<NavMeshInfo*>(pNavMesh)->ResolveNavMesh(kMeshPtr))
        {
            if (kMeshPtr.Modelloader_GetReferencedThis()->FindClosestTriangleForLocation(Location, &usTriangle, &bInside))
            {
                if (!bInside)
                {
                    NiPoint3 kTriCenter = kMeshPtr.Modelloader_GetReferencedThis()->FUN_00558220(usTriangle);
                    NavEdgeHit kResult = kMeshPtr.Modelloader_GetReferencedThis()
                                            ->FindInterectingEdge2(usTriangle, -1, kTriCenter, Location);
                    if (kResult.m_iEdgeNumber)
                        Location = kResult.kPoint.ApplyScaled(0.9f).FUN_00439e90(kTriCenter.ApplyScaled(0.1f));
                }
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
    {
        float fNearest = DAT_01016970;
        int iClosest = -1;
        unsigned short usBestTri = kClearTriangleDataValue1;
        bool bValidFound = false;
        unsigned int uiNumMeshes = ((NLMeshInfoArray*)pNavMeshes)->FUN_0044ddc0();
        bool bClose = false;
        if (DAT_011dea10->GetCellHeightAtPosition(reinterpret_cast<NiPoint3*>(&Location), reinterpret_cast<int>(pCell)) + 25.0 > Location.z)
            bClose = true;
        for (unsigned int i = 0; i < uiNumMeshes; i++)
        {
            unsigned short usTri = kClearTriangleDataValue1;
            bool bInTri = false;
            NavMeshPtr pFoundNavMesh;
            NavMeshInfo* pInfo = ((NLMeshInfoArray*)pNavMeshes)->BSSimpleArray_NiAVObject_1024_GetAt(i);
            if (!pInfo->NavMeshInfo_GetDisabled() && pInfo->ResolveNavMesh(pFoundNavMesh) &&
                !pFoundNavMesh.Modelloader_GetReferencedThis()->FindClosestTriangleForLocation(Location, &usTri, &bInTri))
                continue;
            if (bInTri)
            {
                pNavMesh = ((NLMeshInfoArray*)pNavMeshes)->BSSimpleArray_NiAVObject_1024_GetAt(i);
                pNavMeshes = 0;
                usTriangle = usTri;
                return true;
            }
            else if (pFoundNavMesh.AnimatingText_GetFullText())
            {
                NiPoint3 kToCenter =
                    Location.SubtractIntoVector(pFoundNavMesh.Modelloader_GetReferencedThis()->FUN_00558220(usTri));
                float fDist = kToCenter.GetSquaredXYLength();
                float m_fGlyphExtent = kToCenter.z;
                bool bGood = false;
                if ((bClose || m_fGlyphExtent < 180.0) && m_fGlyphExtent > -64.0)
                    bGood = true;
                if (bValidFound)
                {
                    if (bGood && fDist < fNearest)
                    {
                        fNearest = fDist;
                        usBestTri = usTri;
                        iClosest = i;
                    }
                }
                else if (bGood)
                {
                    fNearest = fDist;
                    usBestTri = usTri;
                    iClosest = i;
                    bValidFound = true;
                }
                else if (fDist < fNearest)
                {
                    fNearest = fDist;
                    usBestTri = usTri;
                    iClosest = i;
                }
            }
        }
        if (iClosest != -1)
        {
            pNavMesh = ((NLMeshInfoArray*)pNavMeshes)->BSSimpleArray_NiAVObject_1024_GetAt(iClosest);
            pNavMeshes = 0;
            usTriangle = usBestTri;
            NavMeshPtr spBest;
            if (const_cast<NavMeshInfo*>(pNavMesh)->ResolveNavMesh(spBest))
            {
                NiPoint3 kTriCenter = spBest.Modelloader_GetReferencedThis()->FUN_00558220(usBestTri);
                NavEdgeHit kResult = spBest.Modelloader_GetReferencedThis()
                                        ->FindInterectingEdge2(usBestTri, -1, kTriCenter, Location);
                if (kResult.m_iEdgeNumber)
                    Location = kTriCenter.ApplyScaled(0.1f).FUN_00439e90(kResult.kPoint.ApplyScaled(0.9f));
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
}

class PathingRequest;
class PathingSolution;

#include "../../../include/PathBuilder.h"

// Creates a PathBuilder from pRequest and m_pOutputSolution, calls BuildPath, and returns its result.
// @fnv 0x006d0b10
// src: XBOX_PDB 0x006d0b10
// pdb: Pathing::Pathfind | Pathfind.obj | retail, upstream name already matches, high
// pdb params: apRequest, apSolution
// pdb locals: bret, builder
bool __cdecl Pathing_Pathfind(PathingRequest* apRequest, PathingSolution* apSolution)
{
    PathBuilder kBuilder(apRequest, apSolution);
    bool bResult = kBuilder.BuildPath();
    return bResult;
}
