#pragma once

#include "NiParticles.h"
#include "NiTransform.h"

class NiPSysData;
class NiPSysModifier;
class NiStream;
class NiUpdateData;
class StrArr;

// pdb: class NiParticleSystem, size 0x150 (here 0x110), bases: NiParticles, 6 members
class NiParticleSystem : public NiParticles
{
  public:
    NiParticleSystem();
    virtual ~NiParticleSystem();
    virtual const NiRTTI* GetRTTI() const;
    virtual NiObject* MakeClone(CloningProcess& process);
    virtual void LoadBinary(NiStream& kStream);
    virtual void LinkObject(NiStream& stream);
    virtual bool RegisterStreamables(NiStream& stream);
    virtual void SaveBinary(NiStream& stream);
    virtual bool IsEqual(NiObject* pkObject) const;
    virtual void GetViewerStrings(NiViewerStringsArray* strings);
    virtual void ProcessClone(CloningProcess& process);
    virtual void PostLinkObject(NiStream& stream);
    virtual void SetSelectiveUpdateFlags(bool& selectiveUpdate, bool transforms, bool& rigid);
    virtual void UpdateDownwardPass(NiUpdateData& data, unsigned int flags);
    virtual void UpdateSelectedDownwardPass(NiUpdateData& UpdateData, unsigned int uFlags);
    virtual void UpdateRigidDownwardPass(NiUpdateData& data, unsigned int flags);
    virtual void UpdateWorldData(NiUpdateData& UpdateData);
    virtual void UpdateWorldBound();
    virtual void UpdateSystem(float fTime);
    virtual void Update(float time);

    void NiLines_LinkObject(NiStream* kStream);
    void InitializeNewParticle(unsigned short usNewParticle);
    void InitializeNewParticle(unsigned int usNewParticle);
    void PerformSystemReset();
    void Do_UpdateSystem(float time);
    void ProcessClone_00c1b060(CloningProcess& process);
    void CopyMembers_00c1b6f0(NiParticleSystem* pkDest, CloningProcess& process);
    NiParticleSystem* NiParticleSystem_00c1b5c0();
    void CopyMemberData(NiParticleSystem* destination, CloningProcess& process);
    void InsertSorted(NiPSysModifier* modifier);
    void LoadModifiers(NiStream* stream);
    void FindModifier();
    NiPSysModifier* GetModifier(const NiFixedString& name);
    void UpdateParticleSystem(NiUpdateData& UpdateData, unsigned int uFlags);
    void WriteParticleSystemInfo(StrArr* pkStrings);

    union
    {
        bool m_bFlag;
        bool m_bC4;
        unsigned char m_c4;
    };
    unsigned char unkc5[3];
    union
    {
        NiTPointerList<NiPointer<NiPSysModifier> > m_kModifiers;
        NiTPointerList<NiPointer<NiPSysModifier> > m_kList;
    };
    union
    {
        float m_fLastTime;
        float m_fD4;
    };
    union
    {
        bool m_bHasAccumulation;
        bool m_bD8;
    };
    bool m_bD9;
    unsigned char unkda[2];
    NiTransform m_kUnmodifiedWorld;
};

typedef char NiParticleSystem_flag_offset_check[1];
typedef char NiParticleSystem_modifiers_offset_check[1];
typedef char NiParticleSystem_time_offset_check[1];
typedef char NiParticleSystem_reset_offset_check[1];
typedef char NiParticleSystem_bound_offset_check[1];
typedef char NiParticleSystem_transform_offset_check[1];
