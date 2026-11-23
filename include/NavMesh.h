#pragma once

#include "TESForm.h"
#include "TESChildCell.h"
#include "NiRefObject.h"
#include "NavMeshGrid.h"
#include "NiTMap.h"
#include "BSSimpleArray.h"

struct EdgeT;
class AvoidSphere;
class b9_big;
class P3;
template <class T> class NiPointer;

class EdgeExtraInfo;
class FindTriangleForLocationFilter;
class NavMeshClosedDoorInfo;
class NavMeshEdgeData;
class NavMeshEdgeExtra;
class NavMeshEdgeRef;
class NavMeshEdgeOut;
class NavMeshEdgeVertexPair;
class NavMeshInfo;
class NavMeshPOVData;
class NavEdgeHit;
class NavMeshTriList;
class NavMeshPtr;
class NavMeshStaticAvoidNode;
class NavMeshTriangle;
class NavMeshTriangleDoorPortal;
class NavMeshVertex;
class ObstacleData;
class ObstacleUndoData;
class OutArr;
class ParentChildLinks;
class TESObjectCELL;

// pdb: class NavMesh, size 0x108, bases: TESForm, TESChildCell, NiRefObject, 15 members
class NavMesh : public TESForm, public TESChildCell, public NiRefObject
{
public:
    NavMesh();
    virtual ~NavMesh();
    virtual void InitItem(void);
    virtual void SetAltered(bool);                  // TESForm override, 0x00694090
    virtual TESObjectCELL* GetSaveParentCell();     // TESChildCell override, 0x0068ef50

    EdgeT* GetEdge_0068efd0(EdgeT* out, unsigned short tri, int idx);
    // 0x006911f0: delegates to the NavMeshGrid at +0xa8 and returns the triangle list of grid cell iIndex
    void* NavMesh_GetTrianglesForGridCell(int iIndex);
    TESForm* GetDoorForm();
    void* DestroyNavMesh(unsigned int flags);
    void __thiscall FUN_0068ed70();
    // The explicit base cleanup entry takes the NiRefObject subobject as this.
    void __thiscall Animation_ResetVtableAndFinishScriptRunLock();

    void ForceSetFormID(unsigned int aiID);
    void CopyNavMeshDataTo(NavMeshPtr&);
    void SetDeletionState(bool);
    void InitializeNavMeshData();
    void RebuildTriangleList();
    unsigned int CollectTriangleCells(unsigned short tri, OutArr& out);
    void BuildPortalPOVs();
    void ComputeBounds(NiPoint3& apRetMin, NiPoint3& apRetMax);
    bool CheckNavMesh(int);
    void GetTriangleVertexPositions(unsigned short, unsigned int*, unsigned int*, unsigned int*);
    void AddAvoidNode(void* source, unsigned short triangle, float radius, float cost);
    TESObjectCELL* GetParentCell();
    NavMeshTriangle* GetTriangle(unsigned short);
    NiPoint3 GetNavMeshData(unsigned short);
    NavMeshStaticAvoidNode* NavMeshPtr_ctor();
    EdgeExtraInfo* GetEdgeInfo(unsigned short, int);
    NavMeshEdgeVertexPair GetEdgeVertices(unsigned short, int);
    void GetEdgeVertices(unsigned short, int, NiPoint3&, NiPoint3&);
    bool GetStaticAvoidNodesForTriangle(unsigned short, BSSimpleArray<NavMeshStaticAvoidNode*, 1024>&);
    NiPoint3 GetPlaneNormal(unsigned short);
    bool AddDoorObstacle(unsigned int, ObstacleData*);
    NavMeshClosedDoorInfo* GetClosedDoorInfo(unsigned short);
    bool FindHeightOnTriangle(unsigned short, const NiPoint3&, float&, float, float);
    bool GetObstaclePOVsForTriangle(const NiPointer<ObstacleData>&, ParentChildLinks&);
    bool RemoveObstacleAndUpdateLinks(const NiPointer<ObstacleData>&, ParentChildLinks&);
    bool RemoveDoorObstacle(unsigned int);
    NavMeshEdgeExtra* GetEdge(unsigned short);
    bool FindAdjacentTriangle(unsigned short, int, NavMesh*&, unsigned short&, int&);
    bool FindAdjacentEdge(unsigned short, int, NavMesh**, unsigned short*, int*);
    bool FindAdjacentEdge(unsigned short, int, NavMesh**, int, int);
    bool GetMatchingEdge_1(const NavMeshEdgeRef*, NavMeshEdgeOut*);
    bool GetMatchingEdge(unsigned short, int, NiPointer<NavMesh>&, int, int);
    unsigned short FindTriangleForLocation(const NiPoint3&, FindTriangleForLocationFilter*);
    int FindIntersectingEdgeFast(unsigned short, int, const NiPoint3&, const NiPoint3&);
    bool MatchNavMesh(const NavMeshEdgeData&, NavMeshEdgeData&);
    unsigned int GetCurrentMoveModeType();
    bool ComputeTriangleZForLocation(unsigned short, const NiPoint3&, float, float, float&);
    static void operator delete(void*, size_t);
    static void Deallocate(void*);

    bool IsLocationOnNavMesh(const NiPoint3&, FindTriangleForLocationFilter*);
    bool TestPointOnTriangle(unsigned short, const NiPoint3&, float&, FindTriangleForLocationFilter*);
    bool TestPointOnTriangle(unsigned short, const NiPoint3*, float*, int);
    int CollectTrianglePOVs(unsigned short, BSSimpleArray<unsigned short, 1024>*);
    bool ComputeTriangleZForLocation(unsigned short, const NiPoint3&, float, float, float*);
    bool ComputeNavMeshZForLocation(const NiPoint3&, float, float, float*);
    int FindIntersectingEdgeFast(unsigned short, int, const NiPoint3*, const NiPoint3*);
    NavEdgeHit FindInterectingEdge2(unsigned short, int, const NiPoint3&, const NiPoint3&);
    NiPoint3 AdjustPointToNavMesh(const NiPoint3&, float);
    EdgeExtraInfo* GetOrCreateEdgeInfo(unsigned short, int);
    unsigned short AddEdgeInfo(const EdgeExtraInfo&);
    void SwapTriangleEdges(unsigned short);
    void UpdateTriangleExtraInfo(unsigned short);
    bool CheckNeighborCells();
    bool AddTrianglesToGrid(unsigned short, unsigned short);
    bool LinkMatchingEdges();
    unsigned int ComputeTriangleHash(NavMeshInfo*);
    bool CollectTrianglesForDoor(unsigned int, BSSimpleArray<unsigned int, 1024>*);
    bool UndoToPosition(int, int);
    bool UndoToPosition(unsigned int, void*);

    bool GetBounds_006969e0(NiPoint3*, NiPoint3*);
    unsigned short FindTriangleForLocation_00696a50(const NiPoint3*, int);
    bool FindClosestTriangleForLocation(const NiPoint3&, unsigned short*, bool*);
    bool FindClosestOpenEdgeForLocation(const NiPoint3&, float, unsigned short*, int*, bool);
    void GetCircumCenter2D(unsigned short, NiPoint3*);
    bool FUN_00697730(unsigned short, const NiPoint3&, float&, float, float);
    bool FindRandomPointWithinRadius(const NiPoint3&, float, NiPoint3*);
    void GetOverlappedTriangleList(const NiPointer<ObstacleData>&, const NiPoint3&, NavMeshTriList*);
    bool RemoveObstacle(int, int);
    void ClearAllObstacles();
    unsigned int AddObstacleUndoData(ObstacleData*);
    bool Navmesh_RemoveClosedDoor(unsigned int);
    unsigned int AppendTriangleVerticesInList(unsigned short, BSSimpleArray<unsigned short, 1024>*);
    void LookupPOVValueWithInitialization(unsigned short, unsigned int);
    bool AddTrianglesToXYGridCells(unsigned short, unsigned short);
    void CreateGrid();
    bool FUN_006dcee0(void*, FindTriangleForLocationFilter*);

    const NavMeshInfo* GetNavMeshInfo_0068efb0();
    unsigned int NavMesh_GetTriangleCount();
    const NiPoint3& GetVertex_0068f0a0(unsigned short);
    NavMeshEdgeVertexPair NavMesh_GetEdgeVertices_0(unsigned short, int);
    NiPoint3 GetCircumCenter(unsigned short);
    void GetOverlappedTriangleList(const NiPointer<ObstacleData>&, const NiPoint3&, BSSimpleArray<unsigned short, 1024>*);
    unsigned int GetAvoidNodeCount();
    AvoidSphere& NavMesh_GetAvoidNode(unsigned int);
    NiPoint3 FUN_006971e0(unsigned short);
    void GetEdgeVertices_0068f0c0(unsigned short, int, NiPoint3&, NiPoint3&);
    bool GetEdgeVertices_0068f0c0(unsigned short, int, NiPoint3*, NiPoint3*);
    void GetEdgeVertices_0068f0c0(unsigned short, int, P3&, P3&);
    NavMeshTriangle* FUN_005582f0(unsigned short);
    void NiPointer_NavMesh_NiPointer_NavMesh_0042fd70(int);
    EdgeExtraInfo* GetEdgeExtraInfo(unsigned short, int);
    NiPoint3 FUN_00558220(unsigned short);
    void FUN_00558220(NiPoint3*, unsigned short);
    unsigned int* FUN_00699af0(unsigned short);
    void FindAdjacentEdge(unsigned short, int, NavMesh*&, unsigned short&, int&);
    bool FUN_00460340();
    bool FUN_00440d80();
    void UpdateNavMeshInfo();
    void PathingRequestCoveredMove_SetCombatCoverLocation(NavMeshInfo*);
    void PathingRequestCoveredMove_SetCombatCoverLocation(b9_big*);
    void ForwardToStub(int);
    int FindEntryIndex(int);
    int FindEntryIndex(void*);
    int GetUndoDataIndexForObstacle(int, unsigned int*);
    unsigned int GetHavokWorldObjectCountInSceneGraph_0084e3a0();
    NavMesh* NavMeshPtr_NavMeshPtr_00464ff0(int);
    bool GetObstacleForTriangle(unsigned short);

    TESObjectCELL* pParentCell; // 0x24 // src: XBOX_RETAIL_PDB +0x24 (old: m_pOwningCell)
    BSSimpleArray<NavMeshVertex, 1024> Vertices; // 0x28
    BSSimpleArray<NavMeshTriangle, 1024> Triangles; // 0x38
    BSSimpleArray<EdgeExtraInfo, 1024> ExtraEdgeInfo; // 0x48 // src: XBOX_RETAIL_PDB +0x48 (old: m_kEdgeExtraInfo)
    BSSimpleArray<NavMeshTriangleDoorPortal, 1024> DoorPortals; // 0x58 // src: XBOX_RETAIL_PDB +0x58 (old: m_kDoorPortals)
    BSSimpleArray<NavMeshClosedDoorInfo, 1024> ClosedDoors; // 0x68 // src: XBOX_RETAIL_PDB +0x68 (old: m_kClosedDoorInfo)
    BSSimpleArray<unsigned short, 1024> CoverArray; // 0x78 // src: XBOX_RETAIL_PDB +0x78 (old: m_kIndexList)
    NiTMap<unsigned short, NavMeshPOVData> POVs; // 0x88 // src: XBOX_RETAIL_PDB +0x88 (old: POVPointerArray)
    BSSimpleArray<unsigned short, 1024> ClosestPOVs; // 0x98 // src: XBOX_RETAIL_PDB +0x98 (old: m_kAdditionalIndexList)
    NavMeshGrid MeshGrid; // 0xA8 // src: XBOX_RETAIL_PDB +0xa8 (old: m_kGrid)
    BSSimpleArray<NiPointer<ObstacleUndoData>, 1024> Obstacles; // 0xD0 // src: XBOX_RETAIL_PDB +0xd0 (old: m_kObstacleUndoData)
    NiTMap<unsigned short, NiPointer<ObstacleData> >* pTriangleToObstacleMap; // 0xE0
    BSSimpleArray<unsigned short, 1024> ObstaclePOVs; // 0xE4 // src: XBOX_RETAIL_PDB +0xe4 (old: m_kAdditionalData)
    BSSimpleArray<NavMeshStaticAvoidNode, 1024> StaticAvoidNodes; // 0xF4 // src: XBOX_RETAIL_PDB +0xf4 (old: m_kStaticAvoidNodes)
    NavMeshInfo* pNavMeshInfo; // 0x104 // src: XBOX_RETAIL_PDB +0x104 (old: m_pNavigationMesh)
};
