#pragma once

#include "NiParticles.h"

struct StrArr2;

// pdb: class NiParticleMeshes, size 0x100 (here 0xcc), bases: NiParticles, 2 members
class NiParticleMeshes : public NiParticles
{
  public:
    NiParticleMeshes();
    virtual ~NiParticleMeshes();
    virtual const NiRTTI* GetRTTI() const;
    virtual NiObject* MakeClone(CloningProcess& process);
    virtual void GetViewerStrings(NiViewerStringsArray* strings);
    virtual void SetSelectiveUpdateFlags(bool& selectiveUpdate, bool transforms, bool& rigid);
    virtual void UpdateDownwardPass(NiUpdateData& UpdateData, unsigned int uFlags);
    virtual void UpdateSelectedDownwardPass(NiUpdateData& UpdateData, unsigned int uFlags);
    virtual void UpdateRigidDownwardPass(NiUpdateData& data, unsigned int flags);
    virtual void UpdatePropertiesDownwardPass(NiPropertyState* parentState);
    virtual void UpdateWorldData(NiUpdateData& UpdateData);
    virtual void PurgeRendererData(NiRenderer* renderer);
    virtual void RenderImmediate(NiRenderer* renderer);

    void Init();
    void* Delete(unsigned int flags);
    void UpdatePropertyState(NiPropertyState* parentState);
    void FUN_00a59f90(NiUpdateData* UpdateData, int uFlags);
    void UpdateDownwardPassIfSelective(NiUpdateData& data);
    void RenderTarget(NiRenderer* renderer);
    void NiParticleMeshes_CopyMembers(NiParticleMeshes* object, void* arg);
    NiParticleMeshes* CreateAndInitObj(void* arg);
    void NiParticles_GetViewerStrings(StrArr2* pStrings);
    void AppendString(StrArr2* strings);
    void Object_DoNothing();

    float m_fLastTime;
    bool m_bUpdate;
    char unkC9[3];

    static const void* vftable;
};

typedef char NiParticleMeshes_size_check[1];
typedef char NiParticleMeshes_last_time_offset_check[1];
typedef char NiParticleMeshes_update_offset_check[1];
