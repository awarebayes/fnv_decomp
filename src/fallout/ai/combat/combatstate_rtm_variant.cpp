// src/fallout/ai/combat/combatstate_rtm_variant.cpp - RTM variant unit created 2026-11-01 by rtm_route.py: community reworks of src/fallout/ai/combat/combatstate.cpp that match byte for byte only
// with VC9 RTM (cl 15.00.21022.08, the compiler the game was built with). Declarations copied from that unit's skeleton.

#include "combatstate_decls.h"
// CombatCoverPointData_combatstate_rtm_variant: local type (was include/CombatCoverPointData.h, which had no evidence of one real class); its methods are 0x005dc960 Main::GameOver, 0x0067fdb0 PackageSchedule::SetStartDayOfWeek, 0x009a2ea0 CombatCoverLocation::LoadGameBGS
class BGSSaveFormBuffer;
class BGSLoadFormBuffer;

class CombatCoverPointData_combatstate_rtm_variant
{
public:
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    int unk60;
    int unk64;
    int unk68;
    int unk6c;
    int unk70;
    int unk74;

    void Copy(void*);
    void SaveGame(BGSSaveFormBuffer*);
    void LoadGame(BGSLoadFormBuffer*);
    bool IsHideCover();
    bool IsCrouchCover();
    bool IsStandCover();
    bool IsLedgeCover();
    bool IsValidCover();
    unsigned char GetCoverHeight();
    bool IsReserved();
    void SetReserved(bool);
    bool IsDirty();
    void SetDirty(bool);
    void* __vecDelDtor(unsigned int);
};
#include "../../../../include/BGSExplosion.h"
#include "../../../../include/TESBoundObject.h"
#include "../../../../include/TESForm.h"
#include "../../../../include/TESObjectWEAP.h"

struct CombatAvoidanceSphere
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
};
TESForm* __cdecl TESForm_GetFormByNumericID(unsigned int);

struct CombatLoadBuffer
{

    virtual unsigned char Version();

    void LoadData(void*, unsigned int);

    unsigned int FUN_008648a0();

    unsigned int FUN_00864a60();

    void FUN_008648e0(void*);
};

struct LoadStamp4
{
    unsigned int value;
    void FUN_009a5ff0(CombatLoadBuffer*);
};

struct LoadStamp8
{
    unsigned int low, high;
    void FUN_009a5f20(CombatLoadBuffer*);
};

// The shared class has no constructor declaration; retain only this construction adapter.
struct LoadCover : CombatCoverPointData_combatstate_rtm_variant
{
    LoadCover();
};

struct LoadLocation : CombatAvoidanceSphere
{
    LoadLocation();
    void CombatLocation_LoadGame(CombatLoadBuffer*);
};

struct LoadInfo
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c;
    LoadInfo();
    void CombatState_LocationTarget_LoadGame(CombatLoadBuffer*);
};

struct LoadBoundPair
{
    int value;
    TESBoundObject* object;
    LoadBoundPair();
};

struct LoadArea
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c, unk20, unk24, unk28, unk2c, unk30, unk34, unk38;
    void CombatCoverSearchInfo_LoadGame(CombatLoadBuffer*);
};

struct LoadA
{
    void UnreachableCoverLocation_LoadGame(CombatLoadBuffer*);
};

struct LoadB
{
    void UnreachableLocation_LoadGame(CombatLoadBuffer*);
};

struct LoadWeaponArray
{
    int unk00;
    TESObjectWEAP** data;
    unsigned int count, capacity;
    void FUN_006f2290(unsigned int, bool);
    TESObjectWEAP** BSSimpleArray_NiAVObject_1024_GetAt(unsigned int);
};

struct LoadArrayA
{
    int unk00;
    LoadA* data;
    unsigned int count, capacity;
    void FUN_009a3480(unsigned int, bool);
    LoadA* BSSimpleArray_GroupMemberData_1024_GetAt(unsigned int);
};

struct LoadArrayB
{
    int unk00;
    LoadB* data;
    unsigned int count, capacity;
    void FUN_009a3610(unsigned int, bool);
    LoadB* BSSimpleArray_CombatMember_1024_GetAt(unsigned int);
};

void* __cdecl operator new(size_t);

void __cdecl operator delete(void*);

struct CombatLoadState
{

    unsigned char flags;

    unsigned char unk01;

    unsigned char unk02;

    unsigned char unk03;

    int state;

    int unk08;

    TESObjectWEAP* weapons[6];

    LoadWeaponArray otherWeapons;

    TESObjectWEAP* currentWeapon;

    float value38;

    float values3c[7];

    unsigned int unk058;

    unsigned int unk05c;

    unsigned int unk060;

    unsigned int unk064;

    unsigned int unk068;

    unsigned int unk06c;

    unsigned char flag70;

    unsigned char flag71;

    unsigned char flag72;

    unsigned char flag73;

    unsigned char flag74;

    unsigned char flag75;

    unsigned char flag76;

    unsigned char flag77;

    BGSExplosion* explosion;

    unsigned int unk07c;

    unsigned int unk080;

    float position[3];

    unsigned int unk090;

    LoadStamp4 stamp094;

    LoadStamp4 stamp098;

    LoadStamp4 stamp09c;

    LoadStamp4 stamp0a0;

    LoadStamp4 stamp0a4;

    LoadStamp8 stamp0a8;

    LoadStamp8 stamp0b0;

    LoadStamp8 stamp0b8;

    LoadStamp4 stamp0c0;

    LoadInfo* info;

    unsigned char flagc8;

    unsigned char flagc9;

    unsigned char unkca;

    unsigned char unkcb;

    unsigned int unk0cc;

    unsigned int unk0d0;

    unsigned int unk0d4;

    unsigned char flagd8;

    unsigned char unkd9;

    unsigned char unkda;

    unsigned char unkdb;

    unsigned int unk0dc;

    LoadStamp4 stamp0e0;

    CombatCoverPointData_combatstate_rtm_variant* cover;

    CombatCoverPointData_combatstate_rtm_variant* alternateCover;

    LoadArea area1;

    LoadArea area2;

    LoadArrayA itemsA;

    LoadArrayB itemsB;

    LoadStamp4 stamp184;

    unsigned char flag188;

    unsigned char unk189;

    unsigned char unk18a;

    unsigned char unk18b;

    LoadLocation* location;

    LoadBoundPair* pairs[5];

    TESBoundObject* objects[2];

    LoadStamp8 objectStamps[2];

    unsigned int unk1bc;

    unsigned int unk1c0;

    unsigned int unk1c4;

    unsigned char flag1c8;

    unsigned char unk1c9;

    unsigned char unk1ca;

    unsigned char unk1cb;

    LoadStamp8 stamp1cc;

    LoadStamp8 stamp1d4;

    LoadStamp8 stamp1dc;

    LoadStamp8 stamp1e4;

    LoadStamp8 stamp1ec;

    LoadStamp8 stamp1f4;

    LoadStamp8 stamp1fc;

    LoadStamp8 stamp204;

    LoadStamp8 stamp20c;

    LoadStamp8 stamp214;

    LoadStamp8 stamp21c;

    unsigned int unk224;

    unsigned int unk228;

    void CombatState_ReserveCoverLocation(CombatCoverPointData_combatstate_rtm_variant*);

    void CombatState_LoadGame(CombatLoadBuffer*);
};

// @fnv 0x009a2040
// src: XBOX_DEBUG_PDB 0x009a2040 (was COMMUNITY)
// pdb: CombatState::LoadGame | CombatState.obj | debug, call graph, high
// pdb params: apLoadGameBuffer
// pdb locals: ccombatItems, csavedData, i, isaveGameCount, pembeddedWeapon, unreachableCoverLocation, unreachableLocation
void CombatLoadState::CombatState_LoadGame(CombatLoadBuffer* apLoadGameBuffer)

{

    unsigned char presence;

    unsigned char pairPresence;

    apLoadGameBuffer->LoadData(&flags, 1);

    apLoadGameBuffer->LoadData(&state, 4);

    if (state == -1)
        state = 0;

    for (unsigned int i = 0; i < 6; i++)
        weapons[i] = dynamic_cast<TESObjectWEAP*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));

    {

        unsigned int weaponCount = apLoadGameBuffer->FUN_00864a60();

        otherWeapons.FUN_006f2290(weaponCount, true);

        for (unsigned int i = 0; i < weaponCount; i++)
        {

            TESObjectWEAP** item = otherWeapons.BSSimpleArray_NiAVObject_1024_GetAt(i);

            *item = dynamic_cast<TESObjectWEAP*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));
        }

        currentWeapon = dynamic_cast<TESObjectWEAP*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));

        apLoadGameBuffer->LoadData(&value38, 4);

        for (unsigned int i = 0; i < 7; i++)
            apLoadGameBuffer->LoadData(&values3c[i], 4);

        apLoadGameBuffer->LoadData(&unk058, 4);

        apLoadGameBuffer->LoadData(&unk05c, 4);

        apLoadGameBuffer->LoadData(&unk064, 4);

        apLoadGameBuffer->LoadData(&unk068, 4);

        apLoadGameBuffer->LoadData(&unk06c, 4);

        apLoadGameBuffer->LoadData(&flag70, 1);

        apLoadGameBuffer->LoadData(&flag71, 1);

        apLoadGameBuffer->LoadData(&flag72, 1);

        apLoadGameBuffer->LoadData(&flag73, 1);

        apLoadGameBuffer->LoadData(&flag74, 1);

        apLoadGameBuffer->LoadData(&flag75, 1);

        apLoadGameBuffer->LoadData(&flag76, 1);

        apLoadGameBuffer->LoadData(&flag77, 1);

        explosion = dynamic_cast<BGSExplosion*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));

        apLoadGameBuffer->LoadData(&unk07c, 4);

        apLoadGameBuffer->LoadData(&unk080, 4);

        apLoadGameBuffer->LoadData(&position, 12);

        apLoadGameBuffer->LoadData(&unk090, 4);

        apLoadGameBuffer->LoadData(&flagc8, 1);

        apLoadGameBuffer->LoadData(&flagc9, 1);

        apLoadGameBuffer->LoadData(&unk0cc, 4);

        apLoadGameBuffer->LoadData(&unk0d0, 4);

        apLoadGameBuffer->LoadData(&unk0d4, 4);

        apLoadGameBuffer->LoadData(&unk08, 4);

        apLoadGameBuffer->LoadData(&flagd8, 1);

        apLoadGameBuffer->FUN_008648e0(&unk0dc);

        stamp094.FUN_009a5ff0(apLoadGameBuffer);

        stamp098.FUN_009a5ff0(apLoadGameBuffer);

        stamp09c.FUN_009a5ff0(apLoadGameBuffer);

        stamp0a0.FUN_009a5ff0(apLoadGameBuffer);

        stamp0a4.FUN_009a5ff0(apLoadGameBuffer);

        stamp0a8.FUN_009a5f20(apLoadGameBuffer);

        stamp0b0.FUN_009a5f20(apLoadGameBuffer);

        stamp0c0.FUN_009a5ff0(apLoadGameBuffer);

        if (apLoadGameBuffer->Version() >= 20)
        {
            stamp0b8.FUN_009a5f20(apLoadGameBuffer);
            stamp0e0.FUN_009a5ff0(apLoadGameBuffer);
        }

        presence = 0;

        apLoadGameBuffer->LoadData(&presence, 1);

        if (presence & 1)
        {

            cover = new LoadCover;

            cover->LoadGame(reinterpret_cast<BGSLoadFormBuffer*>(apLoadGameBuffer));

            if (cover->IsReserved())
            {
                cover->SetReserved(false);
                CombatState_ReserveCoverLocation(cover);
            }
        }

        if (presence & 4)
        {

            alternateCover = new LoadCover;

            alternateCover->LoadGame(reinterpret_cast<BGSLoadFormBuffer*>(apLoadGameBuffer));

            if (alternateCover->IsReserved())
            {
                alternateCover->SetReserved(false);
                CombatState_ReserveCoverLocation(alternateCover);
            }
        }

        if (presence & 16)
        {
            location = new LoadLocation;
            location->CombatLocation_LoadGame(apLoadGameBuffer);
        }

        if (presence & 32)
        {
            info = new LoadInfo;
            info->CombatState_LocationTarget_LoadGame(apLoadGameBuffer);
        }

        if (presence & 2)
            area1.CombatCoverSearchInfo_LoadGame(apLoadGameBuffer);

        if (presence & 8)
            area2.CombatCoverSearchInfo_LoadGame(apLoadGameBuffer);

        {

            unsigned int countA = apLoadGameBuffer->FUN_00864a60();

            itemsA.FUN_009a3480(countA, true);

            for (unsigned int i = 0; i < countA; i++)
            {

                LoadA* item = itemsA.BSSimpleArray_GroupMemberData_1024_GetAt(i);

                item->UnreachableCoverLocation_LoadGame(apLoadGameBuffer);
            }

            {

                unsigned int countB = apLoadGameBuffer->FUN_00864a60();

                itemsB.FUN_009a3610(countB, true);

                for (unsigned int i = 0; i < countB; i++)
                {

                    LoadB* item = itemsB.BSSimpleArray_CombatMember_1024_GetAt(i);

                    item->UnreachableLocation_LoadGame(apLoadGameBuffer);
                }

                stamp184.FUN_009a5ff0(apLoadGameBuffer);

                apLoadGameBuffer->LoadData(&flag188, 1);

                pairPresence = 0;

                apLoadGameBuffer->LoadData(&pairPresence, 1);

                for (unsigned int i = 0; i < 5; i++)
                {

                    if (pairPresence & (1 << i))
                    {

                        pairs[i] = new LoadBoundPair;

                        apLoadGameBuffer->FUN_008648e0(pairs[i]);

                        pairs[i]->object = dynamic_cast<TESBoundObject*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));
                    }
                }

                for (unsigned int i = 0; i < 2; i++)
                {

                    objects[i] = dynamic_cast<TESBoundObject*>(TESForm_GetFormByNumericID(apLoadGameBuffer->FUN_008648a0()));

                    objectStamps[i].FUN_009a5f20(apLoadGameBuffer);
                }

                apLoadGameBuffer->LoadData(&unk1bc, 4);

                apLoadGameBuffer->FUN_008648e0(&unk1c0);

                apLoadGameBuffer->LoadData(&flag1c8, 1);

                if (apLoadGameBuffer->Version() >= 15)
                    apLoadGameBuffer->LoadData(&unk228, 4);

                stamp1cc.FUN_009a5f20(apLoadGameBuffer);

                stamp1d4.FUN_009a5f20(apLoadGameBuffer);

                stamp1dc.FUN_009a5f20(apLoadGameBuffer);

                stamp1e4.FUN_009a5f20(apLoadGameBuffer);

                stamp1ec.FUN_009a5f20(apLoadGameBuffer);

                stamp1f4.FUN_009a5f20(apLoadGameBuffer);

                stamp1fc.FUN_009a5f20(apLoadGameBuffer);

                stamp20c.FUN_009a5f20(apLoadGameBuffer);

                stamp204.FUN_009a5f20(apLoadGameBuffer);

                stamp214.FUN_009a5f20(apLoadGameBuffer);

                stamp21c.FUN_009a5f20(apLoadGameBuffer);
            }
        }
    }
}
