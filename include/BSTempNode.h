#pragma once

#include "NiNode.h"

// pdb: class BSTempNode, size 0xe0 (here 0xb0), bases: NiNode, 1 members
class BSTempNode : public NiNode
{
public:
    BSTempNode(float afTime);
    void BSTempNodeManager_RemoveExpired(float afTime);
    void UpdateDownwardPass(NiUpdateData& UpdateData, unsigned int uFlags);
    void UpdateSelectedDownwardPass(NiUpdateData& UpdateData, unsigned int uFlags);
    void UpdateRigidDownwardPass(NiUpdateData& UpdateData, unsigned int uFlags);
    static void operator delete(void*, size_t);

    float UpdateTime(float value);
    NiAVObject* GetChild(unsigned int index);
    void SetTime(float value);
    bool Expired() const;

    union
    {
        float time;
        float fTime;
    };
};
