// include/havok/hkpSerializedAgentNnEntry.h - generated from the Havok 7.1 reflection data in the PC exe (gen_havok_headers.py, 2026-10-17).
// Reflected members at their exact offsets; unreflected bytes are padding. Size 0x140.
#ifndef FNV_HAVOK_HKPSERIALIZEDAGENTNNENTRY_H
#define FNV_HAVOK_HKPSERIALIZEDAGENTNNENTRY_H

#include "hkBase.h"
#include "hkReferencedObject.h"
#include "hkpSerializedTrack1nInfo.h"
class hkContactPoint;
class hkpEntity;
#pragma pack(push, 1)
// pdb: structure hkpSerializedAgentNnEntry, size 0x140, bases: hkReferencedObject, 14 members
class hkpSerializedAgentNnEntry : public hkReferencedObject
{
  public:
    enum SerializedAgentType
    {
        INVALID_AGENT_TYPE = 0,
        BOX_BOX_AGENT3 = 1,
        CAPSULE_TRIANGLE_AGENT3 = 2,
        PRED_GSK_AGENT3 = 3,
        PRED_GSK_CYLINDER_AGENT3 = 4,
        CONVEX_LIST_AGENT3 = 5,
        LIST_AGENT3 = 6,
        BV_TREE_AGENT3 = 7,
        COLLECTION_COLLECTION_AGENT3 = 8,
        COLLECTION_AGENT3 = 9
    };
    hkpEntity* m_bodyA;     // +0x8 // src: XBOX_RETAIL_PDB +0x8 (old: m_pBodyA)
    hkpEntity* m_bodyB;     // +0xC // src: XBOX_RETAIL_PDB +0xc (old: m_pBodyB)
    hkUlong m_bodyAId;       // +0x10 // src: XBOX_RETAIL_PDB +0x10 (old: m_ulData0)
    hkUlong m_bodyBId;       // +0x14 // src: XBOX_RETAIL_PDB +0x14 (old: m_ulData1)
    hkBool m_useEntityIds;     // +0x18 // src: XBOX_RETAIL_PDB +0x18 (old: m_bStateFlag)
    hkInt8 m_agentType;  // +0x19 (SerializedAgentType) // src: XBOX_RETAIL_PDB +0x19 (old: m_ucNnTrackType)
    char pad1A[0x36];
    hkArray<hkUint8> m_propertiesStream;               // +0x50 // src: XBOX_RETAIL_PDB +0x50 (old: m_akAgentData)
    hkArray<hkContactPoint> m_contactPoints;    // +0x5C // src: XBOX_RETAIL_PDB +0x5c (old: m_akContactPoints)
    hkArray<hkUint8> m_cpIdMgr;  // +0x68 // src: XBOX_RETAIL_PDB +0x68 (old: m_akContactPointProperties)
    hkUint8 m_nnEntryData[160];             // +0x74 // src: XBOX_RETAIL_PDB +0x74 (old: m_akAgentDataBuffer)
    hkpSerializedTrack1nInfo m_trackInfo;        // +0x114 // src: XBOX_RETAIL_PDB +0x114 (old: m_kTrackInfo)
    hkUint8 m_endianCheckBuffer[4];                 // +0x12C // src: XBOX_RETAIL_PDB +0x12c (old: m_akTrailingBytes)
    hkUint32 m_version;                           // +0x130
    char pad134[0xC];
};
#pragma pack(pop)
typedef char fnv_hk_size_hkpSerializedAgentNnEntry[1];
#endif
