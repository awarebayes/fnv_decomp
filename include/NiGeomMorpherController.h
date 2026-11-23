#pragma once

#include "NiInterpController.h"
#include "NiMorphData.h"
#include "NiPointer.h"

// pdb: class NiGeomMorpherController, size 0x44, bases: NiInterpController, 7 members
class NiGeomMorpherController : public NiInterpController
{
public:
    struct InterpItem
    {
        NiPointer<NiInterpolator> m_spInterpolator; // src: XBOX_RETAIL_PDB +0x0 (old: m_spControllerInterpolator)
        float m_fWeight;

        __declspec(nothrow) InterpItem();
        __declspec(nothrow) ~InterpItem();
        static void* operator new[](size_t size) throw();
        static void operator delete[](void* p) throw();
    };

    NiGeomMorpherController(NiMorphData* pkData = 0) throw()
        : m_spMorphData(pkData), m_pkInterpItems(0)
    {
        m_uFlags = 0;
        m_bNeedPreDisplay = false;
        m_bIgnoreBaseTarget = false;
        m_bAlwaysUpdate = false;
        m_bBoundCalculated = false;
    }
    virtual ~NiGeomMorpherController() throw();
    virtual NiObject* MakeClone(CloningProcess& kCloning);

    virtual unsigned short GetTargetCount() const;
    virtual unsigned short GetTargetIndex(const char* pcID) const;
    virtual unsigned short GetTargetIndex(const NiFixedString& kID) const;

    virtual void vf58();
    virtual void vf59();

    unsigned int GetNumTargets() const;
    void CopyMemberData(NiGeomMorpherController* pkClone, CloningProcess& kCloning);
    void SetMorphData(NiMorphData* pkData, bool bCalculateMorphBound);
    void AllocateInterpArray();

    unsigned short m_uFlags; // src: XBOX_RETAIL_PDB +0x34 (old: m_uiObjectFlags)
    unsigned char m_aucPad[2];
    NiPointer<NiMorphData> m_spMorphData; // src: XBOX_RETAIL_PDB +0x38 (old: m_spMorphTargetData)
    InterpItem* m_pkInterpItems; // src: XBOX_RETAIL_PDB +0x3c (old: m_pInterpolatorItems)
    bool m_bNeedPreDisplay; // src: XBOX_RETAIL_PDB +0x40 (old: m_bUpdateEnabled)
    bool m_bIgnoreBaseTarget; // src: XBOX_RETAIL_PDB +0x41 (old: m_bUpdateNormals)
    bool m_bAlwaysUpdate; // src: XBOX_RETAIL_PDB +0x42 (old: m_bNeedsUpdate)
    bool m_bBoundCalculated; // src: XBOX_RETAIL_PDB +0x43 (old: m_bUpdateBounds)
};

