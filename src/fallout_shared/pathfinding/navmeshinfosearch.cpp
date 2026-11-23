#include "../../../include/AStarSearch.h"
#include "../../../include/NavMeshInfo.h"
// fallout_shared/pathfinding/navmeshinfosearch.cpp: NavMeshInfoSearch, NavMeshInfoMap (8 functions)

#include <stddef.h>
#define FNV_JOIN_IMPL(a, b) a##b
#define FNV_JOIN(a, b) FNV_JOIN_IMPL(a, b)
#define FNV_STATIC_ASSERT(condition) typedef char FNV_JOIN(fnv_static_assert_, __LINE__)[1]
#pragma pack(push, 4)
#include "../../../include/NavMeshInfoMap.h"

#include "../../../include/PathingActorData.h"
#include "../../../include/DoorAccessData.h"
#include "../../../include/ExtraDataList.h"
#include "../../../include/NiPoint3.h"
#include "../../../include/TES.h"
#include "../../../include/TESObjectDOOR.h"
#include "../../../include/TESObjectREFR.h"

// The approved AStarSearch header lacks the constructor and virtual interface
// used by this specialization. Retain only that interface locally; the shared
// NavMeshInfoSearch supplies the complete search storage.

#include "../../../include/NavMeshInfoSearch.h"

// PC NiPoint3 layout with the out-of-line arithmetic used by this search.
FNV_STATIC_ASSERT(sizeof(NiPoint3) == 12);
FNV_STATIC_ASSERT(offsetof(NiPoint3, x) == 0);
FNV_STATIC_ASSERT(offsetof(NiPoint3, y) == 4);
FNV_STATIC_ASSERT(offsetof(NiPoint3, z) == 8);


class NavMeshPortalData
{
  public:
    char extraDataPrefix[12];
    unsigned int m_uiNavMeshID;
};
class TeleportableReference
{
  public:
    TESObjectREFR* GetLinkedDoor();
    const NiPoint3& GetPosition();
};
extern TES* g_TES;
void __cdecl BSClamp_float(float&, float, float);
template <class NodeDataType, class LinkDataType> class AStarNode
{
  public:
    float m_fPathingCostCur;
    float m_fCost;
    NodeDataType ObjectResolver;
    float GetHeuristic() const;
    LinkDataType m_kOpenListLink;
    AStarNode* m_pParentNode;
    AStarNode** m_ppPreviousNodeLink;
    AStarNode* pListNode;
    unsigned int iKey;
    AStarNode* m_pNextHashNode;
};
template <class NodeDataType, class LinkDataType> class AStarOpenList
{
  public:
    float m_fValue;
    AStarNode<NodeDataType, LinkDataType>* m_ppNodes[20];
    AStarOpenList();
};
typedef AStarOpenList<const NavMeshInfo*, TESObjectREFR*> NavMeshInfoQueue;
FNV_STATIC_ASSERT(sizeof(NavMeshInfoQueue) == 0x54);
FNV_STATIC_ASSERT(offsetof(NavMeshInfoSearch, m_pGoalNavMeshInfo) == 0x2074);
extern "C" void* __cdecl memset(void*, int, size_t);

// The legacy destructor label is contradicted by the queue member initialization
// at AStarSearch construction: the receiver is this + 0x1ff4.
// Default-constructs the AStarOpenList, initializing m_fValue to -1.0f and zeroing the m_ppNodes array.
// @fnv 0x006b9470
// pdb: AStarQueue<CombatPlanNode *,CombatPlanLink *>::AStarQueue<CombatPlanNode *,CombatPlanLink *> | CombatController.obj | debug, call graph, medium
template <> AStarOpenList<const NavMeshInfo*, TESObjectREFR*>::AStarOpenList()
{
    m_fValue = -1.0f;
    memset(m_ppNodes, 0, sizeof(m_ppNodes));
}

// @fnv 0x006b8c20
bool NavMeshInfoSearch::HasReachedGoal(const AStarNodeType* pNode)
{
    return pNode->ObjectResolver == m_pGoalNavMeshInfo;
}

// Builds the A* connection list for pNode from its mesh's connections, preferred connections and door portals, computing costs and adding nodes to pConnections.
// @fnv 0x006b8490
// pdb: NavMeshInfoSearch::GetNodeConnections | NavMeshInfoSearch.obj | retail, vtable slot, high
// pdb params: apNode, apRetNodes
// pdb locals: fcostFactor, isize, fparentCost, ipreferredSize, pparentInfo, i, end, fcost, pnode, pportal, begin, pextraPortal, bcanOpenLock, bmustPick, potherDoor, pdoor, fdoorAdditionalCost, pinfo
bool NavMeshInfoSearch::CollectConnections(const AStarNodeType* apNode,
                                           BSSimpleArray<AStarNodeType*, 1024>* apRetNodes)
{
    float fInitialCost = apNode->m_fPathingCostCur;
    const NavMeshInfo* m_pNavigationMesh = apNode->ObjectResolver;
    float fCostMultiplier = 1.0f - m_pNavigationMesh->FUN_007df1f0() * 100.0;
    BSClamp_float(fCostMultiplier, 0.1f, 1.0f);
    unsigned int ItemUpdateArray = const_cast<NavMeshInfo*>(m_pNavigationMesh)->AdjacentNavMeshes.GetSize();
    for (unsigned int i = 0; i < ItemUpdateArray; ++i)
    {
        const NavMeshInfo* pNextMesh = const_cast<NavMeshInfo*>(m_pNavigationMesh)->AdjacentNavMeshes.GetAt(i);
        if (!pNextMesh)
            continue;
        NiPoint3 Start =
            m_pNavigationMesh == m_pStartNavMeshInfo ? m_kPathStartPosition : m_pNavigationMesh->ApproxLocation;
        NiPoint3 Goal = pNextMesh == m_pGoalNavMeshInfo ? m_kGoalPosition : pNextMesh->ApproxLocation;
        float fConnectionCost = (Goal - Start).Length() * fCostMultiplier * 3.0 + fInitialCost;
        AStarNodeType* pNewAStarNode;
        switch (CreateOrUpdateNode(reinterpret_cast<unsigned int>(pNextMesh), fConnectionCost, pNewAStarNode))
        {
        case ASTARNODE_CREATED:
            pNewAStarNode->ObjectResolver = pNextMesh;
            if (m_bUseGoalDistanceHeuristic)
                pNewAStarNode->m_fCost = (m_kGoalPosition - pNextMesh->ApproxLocation).Length() * 0.1f;
            else
                pNewAStarNode->m_fCost = 1.0f;
        case ASTARNODE_UPDATED:
            pNewAStarNode->m_kOpenListLink = 0;
            apRetNodes->Add(pNewAStarNode);
            break;
        }
    }
    unsigned int uiPreferredConnections = const_cast<NavMeshInfo*>(m_pNavigationMesh)->PreferredAdjacentNavMeshes.GetSize();
    for (unsigned int i = 0; i < uiPreferredConnections; ++i)
    {
        const NavMeshInfo* pNextMesh = const_cast<NavMeshInfo*>(m_pNavigationMesh)->PreferredAdjacentNavMeshes.GetAt(i);
        if (!pNextMesh)
            continue;
        NiPoint3 Start =
            m_pNavigationMesh == m_pStartNavMeshInfo ? m_kPathStartPosition : m_pNavigationMesh->ApproxLocation;
        NiPoint3 Goal = pNextMesh == m_pGoalNavMeshInfo ? m_kGoalPosition : pNextMesh->ApproxLocation;
        float fConnectionCost = (Goal - Start).Length() * fCostMultiplier + fInitialCost;
        AStarNodeType* pNewAStarNode;
        switch (CreateOrUpdateNode(reinterpret_cast<unsigned int>(pNextMesh), fConnectionCost, pNewAStarNode))
        {
        case ASTARNODE_CREATED:
            pNewAStarNode->ObjectResolver = pNextMesh;
            if (m_bUseGoalDistanceHeuristic)
                pNewAStarNode->m_fCost = (m_kGoalPosition - pNextMesh->ApproxLocation).Length() * 0.1f;
            else
                pNewAStarNode->m_fCost = 1.0f;
        case ASTARNODE_UPDATED:
            pNewAStarNode->m_kOpenListLink = 0;
            apRetNodes->Add(pNewAStarNode);
            break;
        }
    }
    if (m_pPathingActorData || m_eDoorLockDisposition == Pathing::IGNORE_LOCKS)
    {
        ItemUpdateArray = const_cast<NavMeshInfo*>(m_pNavigationMesh)->ConnectedDoors.GetSize();
        for (unsigned int i = 0; i < ItemUpdateArray; ++i)
        {
            TESObjectREFR* pPathNodeDoor = const_cast<NavMeshInfo*>(m_pNavigationMesh)->ConnectedDoors.GetAt(i);
            if (!pPathNodeDoor)
                continue;
            if (pPathNodeDoor->GetDisabled())
                continue;
            m_kDoorAccessData.SetDoorReference(pPathNodeDoor);
            bool bDoorLocked;
            bool bCanUnlockLockedDoor = m_kDoorAccessData.CanUnlockDoor(m_pPathingActorData, pPathNodeDoor, bDoorLocked);
            float fDoorCost = 0.0f;
            if (!bCanUnlockLockedDoor || bDoorLocked)
            {
                switch (m_eDoorLockDisposition)
                {
                case Pathing::IGNORE_LOCKS:
                    break;
                case Pathing::LOCKS_UNUSED:
                    fDoorCost = 409600.0f;
                    break;
                case Pathing::DEFAULT_LOCKS:
                    continue;
                }
            }
            if (reinterpret_cast<TESObjectDOOR*>(pPathNodeDoor->GetObjectReference())->IsTeleportDoor())
                fDoorCost = 409600.0f;
            if (m_eDoorLockDisposition == Pathing::LOCKS_UNUSED && pPathNodeDoor->GetIsHiddenDoor())
                continue;
            if (!pPathNodeDoor->Set3DObject())
                continue;
            TESObjectREFR* m_pLinkedDoorReference = pPathNodeDoor->Set3DObject()->GetLinkedDoor();
            if (!m_pLinkedDoorReference)
                continue;
            NavMeshPortalData* pExtraNavMeshPortal = m_pLinkedDoorReference->GetExtraData()->GetNavMeshPortal();
            if (pExtraNavMeshPortal)
            {
                const NavMeshInfo* pNextMesh =
                    g_TES->GetActiveCell(pExtraNavMeshPortal->m_uiNavMeshID)->FindReferenceByFormId();
                if (pNextMesh && !pNextMesh->NavMeshInfo_GetDisabled())
                {
                    NiPoint3 Start = m_pNavigationMesh == m_pStartNavMeshInfo ? m_kPathStartPosition
                                                                              : m_pNavigationMesh->ApproxLocation;
                    NiPoint3 Goal = pNextMesh == m_pGoalNavMeshInfo ? m_kGoalPosition : pNextMesh->ApproxLocation;
                    float fConnectionCost = (m_pLinkedDoorReference->Set3DObject()->GetPosition() - Start).Length() +
                                            (Goal - pPathNodeDoor->Set3DObject()->GetPosition()).Length();
                    fConnectionCost *= fCostMultiplier;
                    fConnectionCost *= 3.0;
                    fConnectionCost += fInitialCost;
                    fConnectionCost += fDoorCost;
                    AStarNodeType* pNewAStarNode;
                    switch (
                        CreateOrUpdateNode(reinterpret_cast<unsigned int>(pNextMesh), fConnectionCost, pNewAStarNode))
                    {
                    case ASTARNODE_CREATED:
                        pNewAStarNode->ObjectResolver = pNextMesh;
                        if (m_bUseGoalDistanceHeuristic)
                            pNewAStarNode->m_fCost = apNode->GetHeuristic() * 2;
                        else
                            pNewAStarNode->m_fCost = 1.0f;
                    case ASTARNODE_UPDATED:
                        pNewAStarNode->m_kOpenListLink = pPathNodeDoor;
                        apRetNodes->Add(pNewAStarNode);
                        break;
                    }
                }
            }
        }
    }
    return true;
}

// The shared types provide storage; only their missing out-of-line default initialization is declared here.
struct NiPoint3DefaultConstruction : public NiPoint3
{
    NiPoint3DefaultConstruction();
};
struct DoorAccessDataDefaultConstruction : public DoorAccessData
{
    DoorAccessDataDefaultConstruction();
};
// NiPoint3's shared default constructor is inline, and DoorAccessData has no default constructor.
// These constructor-only wrappers preserve the required out-of-line initialization calls.
struct NavMeshInfoSearchConstruction : public AStarSearch<const NavMeshInfo*, TESObjectREFR*>
{
    unsigned char m_kSearchStorage[0x2054];
    NiPoint3DefaultConstruction a;
    int i1;
    NiPoint3DefaultConstruction b;
    int i2;
    int i3;
    char pad2[8];
    DoorAccessDataDefaultConstruction c;
    NavMeshInfoSearchConstruction();
    virtual ~NavMeshInfoSearchConstruction();
};
FNV_STATIC_ASSERT(sizeof(NavMeshInfoSearchConstruction) == sizeof(NavMeshInfoSearch));
FNV_STATIC_ASSERT(offsetof(NavMeshInfoSearchConstruction, a) == offsetof(NavMeshInfoSearch, m_kPathStartPosition));
FNV_STATIC_ASSERT(offsetof(NavMeshInfoSearchConstruction, c) == offsetof(NavMeshInfoSearch, m_kDoorAccessData));
// The shared headers lack the out-of-line default constructors used here.
// Initializes the three integer members i1, i2 and i3 to zero.
// @fnv 0x006b8340
// pdb: NavMeshInfoSearch::NavMeshInfoSearch | NavMeshInfoSearch.obj | debug, call graph, high
NavMeshInfoSearchConstruction::NavMeshInfoSearchConstruction()
{
    i3 = 0;
    i1 = 0;
    i2 = 0;
}

// Destroys the shared door-access state and the A* search base.
// @fnv 0x006b8420
// pdb: NavMeshInfoSearch::~NavMeshInfoSearch | NavMeshInfoSearch.obj | retail, upstream name already matches, high
NavMeshInfoSearch::~NavMeshInfoSearch()
{}

extern "C" void __cdecl Deallocate(void*);

// Runs the base destructor, then frees this object with Deallocate when flags has bit 0 set, and returns this.
// @fnv 0x006b83f0
// pdb: NavMeshInfoSearch::`scalar deleting destructor' | NavMeshInfoSearch.obj | retail, vtable slot, high
void* NavMeshInfoSearch::DestroyAndFree(unsigned int flags)
{
    NavMeshInfoSearch::~NavMeshInfoSearch();
    if (flags & 1)
        Deallocate(this);
    return this;
}

// @fnv 0x006b6940
// pdb: NavMeshInfoMap::AddNavMeshInfo | NavMeshInfoMap.obj | retail, call graph, medium
// pdb params: apNavMeshInfo
void NavMeshInfoMap::RegisterNavMeshInfo(TeleportableReference* pInfo)
{
    InfoMap.SetAt((unsigned int)pInfo->GetLinkedDoor(), (NavMeshInfo*)pInfo);
    AddInfoToCellKeyMap((NavMeshInfo*)pInfo);
    SetAltered(true);
}

TESWorldSpace* __fastcall NavMeshInfo_GetWorldSpace(const NavMeshInfo*);
// @fnv 0x006b8c50
// pdb: NavMeshInfoSearch::Pathfind | NavMeshInfoSearch.obj | debug, call graph (shared callers), medium
// pdb params: apActorLockData, aStartLocation, apStartMesh, aGoalLocation, apGoalMesh, apReturnNodes, apReturnLinks, aeDoorLockDisposition
// pdb locals: fstartheuristic
bool NavMeshInfoSearch::SetupNavMeshSearch(PathingActorData* apActorLockData, void* aStartLocation, void* apStartMesh,
                                           void* aGoalLocation, void* apGoalMesh, void* apReturnNodes,
                                           void* apReturnLinks, int aeDoorLockDisposition)
{
    m_kPathStartPosition = *static_cast<const NiPoint3*>(aStartLocation);
    m_pStartNavMeshInfo = static_cast<const NavMeshInfo*>(apStartMesh);
    m_kGoalPosition = *static_cast<const NiPoint3*>(aGoalLocation);
    m_pGoalNavMeshInfo = static_cast<const NavMeshInfo*>(apGoalMesh);
    m_pPathingActorData = apActorLockData;
    m_eDoorLockDisposition = static_cast<Pathing::DOOR_LOCK_DISPOSITION>(aeDoorLockDisposition);
    m_bUseGoalDistanceHeuristic = false;
    if (NavMeshInfo_GetWorldSpace((const NavMeshInfo*)apStartMesh) &&
        NavMeshInfo_GetWorldSpace((const NavMeshInfo*)apStartMesh) == NavMeshInfo_GetWorldSpace((const NavMeshInfo*)apGoalMesh))
        m_bUseGoalDistanceHeuristic = true;
    float fstartheuristic = 3.402823466e+38F;
    if (m_bUseGoalDistanceHeuristic)
        fstartheuristic = m_kGoalPosition.operator-(m_pStartNavMeshInfo->ApproxLocation).Length() * (double)0.1f;
    else
        fstartheuristic = 1.0f;
    FindOrCreateSearchNode(reinterpret_cast<unsigned int>(apStartMesh),
                           reinterpret_cast<const NavMeshInfo* const&>(apStartMesh), fstartheuristic);
    if (!Search(reinterpret_cast<unsigned int>(apReturnNodes), reinterpret_cast<unsigned int>(apReturnLinks), true))
        return false;
    return true;
}
