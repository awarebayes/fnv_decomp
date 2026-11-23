#pragma once

#include "AStarSearch.h"
#include "BSSimpleArray.h"
#include "PathingLocation.h"

class ParentSpaceNode;
class TeleportLink;
class TESWorldSpace;
class TESObjectCELL;
class TESObjectREFR;
class PathingActorData;

// pdb: class TeleportDoorSearch, size 0x20b8, bases: AStarSearch<ParentSpaceNode, 8 members
class TeleportDoorSearch : public AStarSearch<ParentSpaceNode, TeleportLink>
{
public:
    virtual ~TeleportDoorSearch();
    virtual bool CollectConnections(const AStarNode<ParentSpaceNode, TeleportLink>*,
        BSSimpleArray<AStarNode<ParentSpaceNode, TeleportLink>*, 1024>*);
    virtual bool HasReachedGoal(const AStarNode<ParentSpaceNode, TeleportLink>*);

    int InsertOrUpdateNode(void*, float, AStarNode<ParentSpaceNode, TeleportLink>*&);

    char prefix[((0x205c) > sizeof(void*)) ? ((0x205c) - sizeof(void*)) : 1];
    TESWorldSpace* startWorld;
    int pad;
    PathingLocation StartLocation; // src: XBOX_RETAIL_PDB +0x2064 (old: location)
    bool GoalNode; // src: XBOX_RETAIL_PDB +0x208c (old: exterior)
    TESWorldSpace* world;
    TESObjectCELL* cell;
    int Restrictions; // src: XBOX_RETAIL_PDB +0x2098 (old: restriction)
    float fMaxDist; // src: XBOX_RETAIL_PDB +0x209c (old: maxCost)
    const PathingActorData* pLockData; // src: XBOX_RETAIL_PDB +0x20a0 (old: lockData)
    BSSimpleArray<TESObjectREFR*, 1024> DoorList; // src: XBOX_RETAIL_PDB +0x20a4 (old: doors)
    int DoorLockDisposition; // src: XBOX_RETAIL_PDB +0x20b4 (old: disposition)
};
