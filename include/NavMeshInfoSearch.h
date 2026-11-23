#pragma once

#include <stddef.h>
#include "AStarSearch.h"
#include "NiPoint3.h"
#include "DoorAccessData.h"

class NavMeshInfo;
class TESObjectREFR;
class PathingActorData;

#include "Pathing.h"

// pdb: class NavMeshInfoSearch, size 0x209c, bases: AStarSearch<NavMeshInfo const *, 8 members
class NavMeshInfoSearch : public AStarSearch<NavMeshInfo const *, TESObjectREFR *>
{
public:
    NavMeshInfoSearch();
    virtual ~NavMeshInfoSearch();
    virtual bool CollectConnections(const AStarNodeType* apNode,
        BSSimpleArray<AStarNodeType*, 1024>* apRetNodes);
    virtual bool HasReachedGoal(const AStarNodeType* pNode);

    void* DestroyAndFree(unsigned int flags);
    bool SetupNavMeshSearch(PathingActorData* apActorLockData, void* aStartLocation,
        void* apStartMesh, void* aGoalLocation, void* apGoalMesh,
        void* apReturnNodes, void* apReturnLinks, int aeDoorLockDisposition);

    unsigned char unk4[0x2054];

    NiPoint3 m_kPathStartPosition;
    const NavMeshInfo* m_pStartNavMeshInfo;
    NiPoint3 m_kGoalPosition;
    const NavMeshInfo* m_pGoalNavMeshInfo;
    PathingActorData* m_pPathingActorData;
    Pathing::DOOR_LOCK_DISPOSITION m_eDoorLockDisposition;
    bool m_bUseGoalDistanceHeuristic;
    unsigned char unk2081[3];
    DoorAccessData m_kDoorAccessData;
};

typedef char NavMeshInfoSearch_size_check[1];
typedef char NavMeshInfoSearch_start_position_offset_check[1];
typedef char NavMeshInfoSearch_start_mesh_offset_check[1];
typedef char NavMeshInfoSearch_goal_position_offset_check[1];
typedef char NavMeshInfoSearch_goal_mesh_offset_check[1];
typedef char NavMeshInfoSearch_actor_data_offset_check[1];
typedef char NavMeshInfoSearch_lock_disposition_offset_check[1];
typedef char NavMeshInfoSearch_heuristic_offset_check[1];
typedef char NavMeshInfoSearch_door_access_offset_check[1];
