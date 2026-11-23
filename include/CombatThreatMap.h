#pragma once

#include <stddef.h>
#include "NiTMap.h"
#include "CombatTargetTimestamp.h"
#include "NiAVObject.h"
#include "NiPoint3.h"

class CombatTargetMap;
class CombatThreat;
class BGSLoadGameBuffer;
class BGSSaveFormBuffer;

class __declspec(novtable) CombatThreatMap : public NiTMap<unsigned int, CombatThreat*>
{
public:
    class ReadWriteLock
    {
    public:
        volatile unsigned int m_uiLockState;
        unsigned int m_uiWriterThreadId;
    };

    class ThreatPair
    {
    public:
        unsigned int data[2];

        ThreatPair();
    };

    ReadWriteLock Lock;
    unsigned int unk18[2];
    CombatTargetTimestamp stamp;
    ThreatPair pair1;
    ThreatPair pair2;
    unsigned int mode;
    NiPointer<NiAVObject> object;

    CombatThreatMap();
    virtual ~CombatThreatMap();

    void InitThreatObjects();
    void ApplyLoadBuffer(BGSLoadGameBuffer* buffer);
    void ClearAndDestroy(int);
    void ResetThreatObjects(int);
    void ProcessAllProjectiles(BGSLoadGameBuffer* buffer);
    void ProcessAllProjectiles(int aiDeltaTime);
    float EvaluateThreats(NiPoint3 pos, int arg, float delay, float a, CombatTargetTimestamp since, float b);
    void SaveGame(BGSSaveFormBuffer* apSaveGameBuffer);
};

typedef char CombatThreatMap_size_check[1];
typedef char CombatThreatMap_lock_offset_check[1];
typedef char CombatThreatMap_stamp_offset_check[1];
typedef char CombatThreatMap_pair1_offset_check[1];
typedef char CombatThreatMap_pair2_offset_check[1];
typedef char CombatThreatMap_mode_offset_check[1];
typedef char CombatThreatMap_object_offset_check[1];
