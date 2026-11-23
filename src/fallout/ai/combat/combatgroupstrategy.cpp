
// src/fallout/ai/combat/combatgroupstrategy.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout/ai/combat/combatgroupstrategy.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/Actor.h"
#include "../../../../include/NiPoint4.h"
#include "../../../../include/CombatController.h"
#include "../../../../include/ActorCombatGroup.h"
#include "../../../../include/CombatGroupStrategy.h"
#include "../../../../include/CombatGroupStrategyAmbush.h"

#include "../../../../include/CombatGroupStrategyAdvance.h"
#include "../../../../include/CombatGroupStrategyFallBack.h"
#include "../../../../include/CombatGroupStrategyFlank.h"

// Exe 0x00990890: local storage view; GetFlags binds the folded DWORD-at-+8 accessor 0x0044ddc0.
struct CombatgroupstrategyOffset08View
{
    unsigned int GetFlags();
};

// @fnv 0x00990890
// src: XBOX_PDB 0x00990890
// pdb: CombatGroup::GetMemberCount | TESConditionFunctions.obj | debug, upstream name already matches, high
unsigned int ActorCombatGroup::CombatGroup_GetMemberCount()
{
    return reinterpret_cast<CombatgroupstrategyOffset08View*>(reinterpret_cast<char*>(this) + 0x18)->GetFlags();
}

namespace fnv_00990a60 {
// Exe 0x00990a60: local storage view; GetFlags binds the folded DWORD-at-+8 accessor 0x0044ddc0.
struct CombatgroupstrategyOffset08View
{
    unsigned int GetFlags();
};

struct FUN_00990a60_shell
{
    unsigned int GetArrFlags();
};

// @fnv 0x00990a60
unsigned int FUN_00990a60_shell::GetArrFlags()
{
    return ((CombatgroupstrategyOffset08View*)(this + 0x28))->GetFlags();
}
}  // namespace fnv_00990a60

struct FUN_00844160_holder
{
    void __thiscall BSSimpleArray_GroupMemberData_1024_GetAt(int);
};

struct FUN_00990a80_shell
{
    void __thiscall CombatGroupData_GetMeleeMember(int aiIndex);
};

// @fnv 0x00990a80
// src: XBOX_DEBUG_PDB 0x00990a80
// pdb: CombatGroupData::GetMeleeMember | CombatGroupStrategy.obj | debug, call graph, high
// pdb params: aiIndex
void __thiscall FUN_00990a80_shell::CombatGroupData_GetMeleeMember(int aiIndex)
{
    ((FUN_00844160_holder*)((char*)this + 0x28))->BSSimpleArray_GroupMemberData_1024_GetAt(aiIndex);
}

struct FUN_00844160_shell
{
    void __thiscall BSSimpleArray_GroupMemberData_1024_GetAt(int state);
};

struct FUN_00990aa0_shell
{
    void __thiscall CombatGroupData_GetRangedMember(int aiIndex);
};

// src: XBOX_PDB 0x00844160
void __thiscall FUN_00844160_shell::BSSimpleArray_GroupMemberData_1024_GetAt(int state)
{}

// @fnv 0x00990aa0
// src: XBOX_PDB 0x00990aa0
// pdb: CombatGroupData::GetRangedMember | CombatGroupStrategy.obj | debug, upstream name already matches, high
// pdb params: aiIndex
void __thiscall FUN_00990aa0_shell::CombatGroupData_GetRangedMember(int aiIndex)
{
    ((FUN_00844160_shell*)((char*)this + 0x18))->BSSimpleArray_GroupMemberData_1024_GetAt(aiIndex);
}

struct MarkerStruct
{
};

// Iterates over every member of the group and calls SetElementValue with index i and value 2.
// @fnv 0x00990bd0
// pdb: CombatGroupStrategyFallBack::Execute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
// pdb locals: i
void CombatGroupStrategyFallBack::vf01(ActorCombatGroup* apGroup, MarkerStruct* apGroupData)
{
    unsigned int i = 0;
    for (; i < apGroup->GetCount(); i++)
    {
        apGroup->SetElementValue((int)i, 2);
    }
}

// Returns true if apGroup's lookup for index 3 returns a non-null value, otherwise false.
// @fnv 0x00990d30
// pdb: CombatGroupStrategyAmbush::Update | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup
bool CombatGroupStrategyAmbush::vf02(ActorCombatGroup* apGroup)
{
    if (apGroup->CombatGroup_FindMemberWithStrategyAssignment(3) == 0)
        return false;
    return true;
}

struct FUN_00990480_shell
{
    bool CombatState_ShouldUseMelee();
};

// Returns whether the float at offset 0x58 is greater than the float at offset 0x5c.
// @fnv 0x00990480
// src: COMMUNITY 0x00990480
bool FUN_00990480_shell::CombatState_ShouldUseMelee()
{
    return *(float*)((char*)this + 0x58) > *(float*)((char*)this + 0x5c);
}

struct Grp_Call2
{
    bool __thiscall CombatGroup_CheckMemberTargetIntersection();
};
struct Holder904c0
{
    Grp_Call2* m_pCombatData;
    int _pad;
    int m_iValue;
    unsigned char __thiscall CombatGroupData_GetMembersTargetsIntersecting();
};
// If the cached flag is unset, evaluates the held object's check and stores the result; returns whether the cached flag equals 1.
// @fnv 0x009904c0
// src: XBOX_PDB 0x009904c0
// pdb: CombatGroupData::GetMembersTargetsIntersecting | CombatGroupStrategy.obj | retail, upstream name already matches, high
unsigned char __thiscall Holder904c0::CombatGroupData_GetMembersTargetsIntersecting()
{
    if (m_iValue == -1)
        m_iValue = m_pCombatData->CombatGroup_CheckMemberTargetIntersection() != 0;
    return m_iValue == 1;
}

struct FUN_009905a0_shell
{
    void __thiscall CombatGroupData_ComputeMinMaxTargetDetection();
};
struct FUN_00990570_shell
{
    unsigned int __thiscall CombatGroupData_GetMemberMaximumTargetDetection();
};
// @fnv 0x00990570
// src: XBOX_PDB 0x00990570
// pdb: CombatGroupData::ComputeMinMaxTargetDetection | CombatGroupStrategy.obj | retail, call graph (shared callers), low
// pdb note: debug build links it to CombatGroupData::GetMemberMaximumTargetDetection
// pdb locals: imemberCount, i, pmember, pcombatController, itargetDetection
unsigned int __thiscall FUN_00990570_shell::CombatGroupData_GetMemberMaximumTargetDetection()
{
    if (*(unsigned int*)((char*)this + 0x10) == 0x80000000)
        ((FUN_009905a0_shell*)this)->CombatGroupData_ComputeMinMaxTargetDetection();
    return *(unsigned int*)((char*)this + 0x10);
}

extern const double DAT_01026998;
struct DataOps
{
    int __thiscall _ref_00726070();
    double __thiscall CombatGroupData_GetMemberHealthPercentage();
};
struct GroupOps
{
    bool __thiscall CombatGroup_CheckGroupStrategyChosenTimeStamp(int, float);
};
// @fnv 0x00990b70
// pdb: CombatGroupStrategyFallBack::CanExecute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
bool CombatGroupStrategyFallBack::CanFlank(MarkerStruct* apGroup, ActorCombatGroup* apGroupData)
{
    if (((DataOps*)apGroupData)->_ref_00726070() < 2)
        return false;
    if (!((GroupOps*)apGroup)->CombatGroup_CheckGroupStrategyChosenTimeStamp(0, 30.0f))
        return false;
    if (((DataOps*)apGroupData)->CombatGroupData_GetMemberHealthPercentage() > DAT_01026998)
        return false;
    return true;
}

// Calls FindByValue with 2 on the group and returns true if it succeeds, otherwise false.
// @fnv 0x00990c10
// pdb: CombatGroupStrategyFallBack::Update | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup
bool CombatGroupStrategyFallBack::vf02(ActorCombatGroup* apGroup)
{
    if (!apGroup->CombatGroup_FindMemberWithStrategyAssignment(2))
        return false;
    return true;
}

// @fnv 0x00990cb0
// pdb: CombatGroupStrategyAmbush::Execute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
// pdb locals: ifallBackCount, i
void __thiscall CombatGroupStrategyAmbush::vf01(ActorCombatGroup* apGroup, MarkerStruct* apGroupData)
{
    unsigned int i;
    unsigned int i2;
    unsigned int ifallBackCount4 = apGroup->GetCount() >> 1;
    for (i = 0; i < ifallBackCount4; ++i)
    {
        apGroup->SetElementValue(i, 2);
    }
    for (i2 = ifallBackCount4; i2 < apGroup->GetCount(); ++i2)
    {
        apGroup->SetElementValue(i2, 3);
    }
}

struct IntPair
{
    int _0;
    int _4;
};
struct FUN_00990aa0_helper
{
    IntPair* __thiscall CombatGroupData_GetRangedMember(int);
};
// Iterates the group data's members and calls SetElementValue on the group with each member's _4 value and 4.
// @fnv 0x00990e70
// pdb: CombatGroupStrategyAdvance::Execute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
// pdb locals: i, pmember
void __thiscall CombatGroupStrategyAdvance::vf01(ActorCombatGroup* apGroup, MarkerStruct* apGroupData)
{
    int i;
    IntPair* pmember;
    for (i = 0; i < (int)((ActorCombatGroup*)apGroupData)->GetCount(); i++)
    {
        pmember = ((FUN_00990aa0_helper*)apGroupData)->CombatGroupData_GetRangedMember(i);
        apGroup->SetElementValue(pmember->_4, 4);
    }
}

// Calls FindByValue with 4 on apGroup and returns true if the result is nonzero, false otherwise.
// @fnv 0x00990ec0
// pdb: CombatGroupStrategyAdvance::Update | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup
bool __thiscall CombatGroupStrategyAdvance::vf02(ActorCombatGroup* apGroup)
{
    if (apGroup->CombatGroup_FindMemberWithStrategyAssignment(4) == 0)
        return false;
    return true;
}

// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
struct TaskQueueInterface
{
    static void __cdecl CopyStringToBuffer(char* buffer, unsigned int size, const char* name);
};

char* __cdecl FUN_004812f0(const char* lhs, const char* rhs);
int __cdecl CombatGroupStrategies_GetStrategy(int index);
int __cdecl FUN_00eca7f4(int character);

extern const char* DAT_011a4ed0[4];

// Copies apName into a 260-byte buffer, lowercases it, then compares it against four names and returns the matching index's value or 0.
// @fnv 0x00990250
// src: XBOX_DEBUG_PDB 0x00990250 (was COMMUNITY)
// pdb: CombatGroupStrategy::GetCombatGroupStrategyByName | CombatGroupStrategy.obj | debug, call graph, high
// pdb params: apName
// pdb locals: psearchName, ptemp, i
int CombatGroupStrategy_GetCombatGroupStrategyByName(const char* apName)
{
    char psearchName[260];
    char* pChar;

    TaskQueueInterface::CopyStringToBuffer(psearchName, 0x104, apName);
    pChar = psearchName;
    while (*pChar)
    {
        *pChar = static_cast<char>(FUN_00eca7f4(*pChar));
        ++pChar;
    }

    for (unsigned int i = 0; i < 4; ++i)
    {
        if (FUN_004812f0(DAT_011a4ed0[i], psearchName) != 0)
        {
            return CombatGroupStrategies_GetStrategy(i);
        }
    }

    return 0;
}

struct FUN_00990a60_shell
{
    unsigned int __thiscall GetArrFlags();
};

struct FUN_00990a80_member
{
    IntPair* __thiscall CombatGroupData_GetMeleeMember(unsigned int aiIndex);
};

struct FUN_00990aa0_member
{
    IntPair* __thiscall CombatGroupData_GetRangedMember(unsigned int aiIndex);
};

struct FUN_00990640_shell
{
    void __thiscall SortMembersByTargetDistance();
};

// @fnv 0x00990930
// pdb: CombatGroupStrategyFlank::Execute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
// pdb locals: irangedMemberCount, itotalMemberCount, frangedPercentage, irangedFlankers, imeleeMemberCount, i, pmember
void CombatGroupStrategyFlank::vf01(ActorCombatGroup* apGroup, MarkerStruct* apGroupData)
{
    unsigned int irangedMemberCount;
    unsigned int itotalMemberCount;
    float frangedPercentage;
    unsigned int irangedFlankers;
    unsigned int imeleeMemberCount;

    imeleeMemberCount = reinterpret_cast<FUN_00990a60_shell*>(apGroupData)->GetArrFlags();

    {
        unsigned int i;
        IntPair* pmember;

        for (i = 0; i < imeleeMemberCount; ++i)
        {
            pmember = reinterpret_cast<FUN_00990a80_member*>(apGroupData)->CombatGroupData_GetMeleeMember(i);
            apGroup->SetElementValue(pmember->_4, 1);
        }
    }

    irangedMemberCount = reinterpret_cast<ActorCombatGroup*>(apGroupData)->GetCount();
    itotalMemberCount = reinterpret_cast<DataOps*>(apGroupData)->_ref_00726070();
    frangedPercentage = static_cast<float>(irangedMemberCount) / static_cast<float>(itotalMemberCount);
    irangedFlankers = 0;

    if (frangedPercentage > 0.5)
        irangedFlankers = static_cast<unsigned int>((frangedPercentage - 0.5) * irangedMemberCount);

    if (irangedFlankers > 0)
    {
        reinterpret_cast<FUN_00990640_shell*>(apGroupData)->SortMembersByTargetDistance();

        {
            unsigned int i;
            IntPair* pmember;

            for (i = irangedMemberCount - irangedFlankers; i < irangedMemberCount; ++i)
            {
                pmember = reinterpret_cast<FUN_00990aa0_member*>(apGroupData)->CombatGroupData_GetRangedMember(i);
                apGroup->SetElementValue(pmember->_4, 1);
            }
        }
    }
}

struct CombatMemberState
{
    bool GetByteAtOffset();
    bool CombatState_ShouldUseMelee();
};
struct MemberController
{
    CombatMemberState* CombatController_GetCombatState();
};

struct IndexedMember
{
    Actor* member;
    unsigned int index;
    IndexedMember(Actor*, unsigned int);
};
struct IndexedMemberArray
{
    void* data;
    unsigned int size, capacity, growth;
    IndexedMemberArray();
    ~IndexedMemberArray();
    void InsertAtEnd(const IndexedMember&);
};
struct FUN_00990320
{
    ActorCombatGroup* group;
    unsigned int activeCount;
    int intersectionCount;
    int minimum;
    int maximum;
    float strength;
    IndexedMemberArray normalMembers;
    IndexedMemberArray specialMembers;
    FUN_00990320(ActorCombatGroup*);
};
// @fnv 0x00990320
// pdb: CombatGroupData::CombatGroupData | CombatGroupStrategy.obj | debug, call graph, high
// pdb params: apCombatGroup
// pdb locals: imemberCount, i, pmember, pcombatState
FUN_00990320::FUN_00990320(ActorCombatGroup* apCombatGroup)
    : group(apCombatGroup), activeCount(0), intersectionCount(-1), minimum(0x7fffffff), maximum(0x80000000), strength(0.0f)
{
    unsigned int count = apCombatGroup->CombatGroup_GetMemberCount();
    for (unsigned int index = 0; index < count; ++index)
    {
        Actor* aActor = reinterpret_cast<Actor*>(apCombatGroup->GetIntAt(index));
        CombatMemberState* state = reinterpret_cast<MemberController*>(aActor->GetCombatController())->CombatController_GetCombatState();
        if (!state->GetByteAtOffset())
        {
            ++activeCount;
            if (state->CombatState_ShouldUseMelee())
                specialMembers.InsertAtEnd(IndexedMember(aActor, index));
            else
                normalMembers.InsertAtEnd(IndexedMember(aActor, index));
        }
    }
}

extern const float g_011dadbc_arg1;
struct ContNode;
ContNode* __fastcall FUN_00726070(ContNode* node);

// @fnv 0x009908b0
// pdb: CombatGroupStrategyFlank::CanExecute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
bool CombatGroupStrategyFlank::CanFlank(MarkerStruct* apGroupData, ActorCombatGroup* apGroup)
{
    if ((int)FUN_00726070((ContNode*)apGroup) < 3)
    {
        return false;
    }
    if ((int)apGroup->CombatGroup_GetMemberCount() < 1)
    {
        return false;
    }
    if (!reinterpret_cast<GroupOps*>(apGroupData)->CombatGroup_CheckGroupStrategyChosenTimeStamp(2, g_011dadbc_arg1))
    {
        return false;
    }
    if ((int)reinterpret_cast<FUN_00990570_shell*>(apGroup)->CombatGroupData_GetMemberMaximumTargetDetection() <= 0)
    {
        return false;
    }
    if (reinterpret_cast<Holder904c0*>(apGroup)->CombatGroupData_GetMembersTargetsIntersecting())
    {
        return false;
    }
    return true;
}

struct CombatGroupMember
{
    int TESObjectREFR_GetWorldLocation(NiPoint4*, float, int);
};
extern const float DAT_01050b98;
struct CombatGroupMethods
{
    int CombatGroup_GetMemberGroupStrategyAssignment(unsigned);
    CombatGroupMember* GetIntAt(unsigned);
    int FindRefInRadius(int);
};
// @fnv 0x00990ac0
// pdb: CombatGroupStrategyFlank::Update | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup
// pdb locals: bflanking, imemberCount, i, pflanker
bool CombatGroupStrategy::CheckGroup(ActorCombatGroup* apGroup)
{
    bool bFound = false;
    unsigned count = apGroup->GetCount();
    for (unsigned index = 0; index < count; ++index)
    {
        if (((CombatGroupMethods*)apGroup)->CombatGroup_GetMemberGroupStrategyAssignment(index) == 1)
        {
            CombatGroupMember* pData = ((CombatGroupMethods*)apGroup)->GetIntAt(index);
            NiPoint4 position;
            if (((CombatGroupMethods*)apGroup)->FindRefInRadius(pData->TESObjectREFR_GetWorldLocation(&position, DAT_01050b98, 0)))
                apGroup->SetElementValue(index, 0);
            else
                bFound = true;
        }
    }
    if (!bFound)
        return false;
    return true;
}

extern const float DAT_01034294;

struct FUN_00990c40_arg2
{
    int _ref_00726070();
    int CombatGroupData_GetMemberMaximumTargetDetection();
    unsigned char CombatGroupData_GetMembersTargetsIntersecting();
};

struct FUN_00990c40_arg1
{
    bool CombatGroup_CheckGroupStrategyChosenTimeStamp(int aiIndex, float afMinimumTimePassed);
};

// @fnv 0x00990c40
// pdb: CombatGroupStrategyAmbush::CanExecute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
bool CombatGroupStrategyAmbush::CanFlank(MarkerStruct* apGroup, ActorCombatGroup* apGroupData)
{
    if (((FUN_00990c40_arg2*)apGroupData)->_ref_00726070() < 2)
    {
        return false;
    }
    if (!((FUN_00990c40_arg1*)apGroup)->CombatGroup_CheckGroupStrategyChosenTimeStamp(3, DAT_01034294))
    {
        return false;
    }
    if (((FUN_00990c40_arg2*)apGroupData)->CombatGroupData_GetMemberMaximumTargetDetection() <= 0)
    {
        return false;
    }
    if (((FUN_00990c40_arg2*)apGroupData)->CombatGroupData_GetMembersTargetsIntersecting() != 0)
    {
        return false;
    }
    return true;
}

struct Vec990d60
{
    float x, y, z;
    Vec990d60 SubtractIntoVector(const Vec990d60*);
    double FUN_004a7290();
};
extern const double DAT_0101ece0;



struct Member990d60
{
    Actor* actor;
};
// Strategy member queries bind to 0x00990890/0x00990540/0x00990AA0, unlike include/Data.h.
struct StrategyMemberQueries
{
    int CombatGroup_GetMemberCount();
    int CombatGroupData_GetMemberMinimumTargetDetection();
    Member990d60* CombatGroupData_GetRangedMember(unsigned int);
};
extern const float DAT_01034294;
// @fnv 0x00990d60
// pdb: CombatGroupStrategyAdvance::CanExecute | CombatGroupStrategy.obj | retail, vtable slot, high
// pdb params: apGroup, apGroupData
// pdb locals: irangedMemberCount, i, pcombatController, ptarget, prangedMember, targetVector
bool CombatGroupStrategyAdvance::CanFlank(MarkerStruct* apGroup, ActorCombatGroup* apGroupData)
{
    if (((StrategyMemberQueries*)apGroupData)->CombatGroup_GetMemberCount() < 2)
        return false;
    if (!((GroupOps*)apGroup)->CombatGroup_CheckGroupStrategyChosenTimeStamp(1, DAT_01034294))
        return false;
    if (((StrategyMemberQueries*)apGroupData)->CombatGroupData_GetMemberMinimumTargetDetection() <= 0)
        return false;
    unsigned int count = ((StrategyMemberQueries*)apGroupData)->CombatGroup_GetMemberCount();
    for (unsigned int index = 0; index < count; ++index)
    {
        Member990d60* member9 = ((StrategyMemberQueries*)apGroupData)->CombatGroupData_GetRangedMember(index);
        CombatController* handle5 = member9->actor->GetCombatController();
        const TESObjectREFR* target8 = reinterpret_cast<const TESObjectREFR*>(handle5->CombatController_GetTarget());
        if (target8)
        {
            Vec990d60 delta = reinterpret_cast<Vec990d60*>(static_cast<const TESObjectREFR*>(member9->actor)->GetPos())->SubtractIntoVector(reinterpret_cast<Vec990d60*>(target8->GetPos()));
            if (delta.FUN_004a7290() < DAT_0101ece0)
                return false;
        }
    }
    return true;
}

extern "C" void* __cdecl _alloca(size_t);
extern const Vec990d60 DAT_011f426c;
struct Centroid990640
{
    float x, y, z;
    void ComputeValue_0063c8a0(const Vec990d60*);
    void ScaleByInverse(float);
};
struct MemberList990640
{
    Member990d60* BSSimpleArray_GroupMemberData_1024_GetAt(unsigned int);
    IntPair& GetElement(unsigned int);
};
struct Strategy990640
{
    ActorCombatGroup* group;
    unsigned int activeCount;
    int intersectionCount;
    int minimum;
    int maximum;
    float strength;
    MemberList990640 normalMembers;
    unsigned int CombatGroup_GetMemberCount();
    void SortMembersByTargetDistance();
};
// Computes each member's target value, averages them into a centroid, then selection-sorts members by distance to that centroid.
// @fnv 0x00990640
// pdb: CombatGroupData::SortRangedMembers | CombatGroupStrategy.obj | debug, object-file run extension, low
// pdb locals: irangedMemberCount, plocations, pfsquaredDistances, center, i, groupMemberData, icurrent, fminDistance, j, fdistance
void Strategy990640::SortMembersByTargetDistance()
{
    // i: member count; a: positions; p: centroid; h: distances.
    unsigned int i = CombatGroup_GetMemberCount();
    Vec990d60* a = (Vec990d60*)_alloca(i * sizeof(Vec990d60));
    Vec990d60 p = DAT_011f426c;
    for (unsigned int memberIndex = 0; memberIndex < i; ++memberIndex)
    {
        Member990d60* member = normalMembers.BSSimpleArray_GroupMemberData_1024_GetAt(memberIndex);
        a[memberIndex] = *reinterpret_cast<Vec990d60*>(static_cast<const TESObjectREFR*>(member->actor)->GetPos());
        ((Centroid990640*)&p)->ComputeValue_0063c8a0(&a[memberIndex]);
    }
    ((Centroid990640*)&p)->ScaleByInverse((float)i);
    float* h = (float*)_alloca(i * sizeof(float));
    for (unsigned int j = 0; j < i; ++j)
        h[j] = a[j].SubtractIntoVector(&p).FUN_004a7290();
    for (unsigned int k = 0; k < i - 1; ++k)
    {
        unsigned int nearest = k;
        float nearestDistance = h[k];
        for (unsigned int n = k + 1; n < i; ++n)
        {
            if (nearestDistance > h[n])
            {
                nearest = n;
                nearestDistance = h[n];
            }
        }
        if (k != nearest)
        {
            // Save the distance and member while exchanging the pair.
            float p = h[k];
            h[k] = h[nearest];
            h[nearest] = p;
            IntPair j = normalMembers.GetElement(k);
            normalMembers.GetElement(k) = normalMembers.GetElement(nearest);
            normalMembers.GetElement(nearest) = j;
        }
    }
}
