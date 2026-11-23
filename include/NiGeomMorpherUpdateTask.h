#pragma once

#include "NiTask.h"
#include <stddef.h>

class MorphController;
class FakeGeo;

// pdb: class NiGeomMorpherUpdateTask, size 0x14, bases: NiTask, 2 members
class NiGeomMorpherUpdateTask : public NiTask
{
public:
    NiGeomMorpherUpdateTask();
    virtual ~NiGeomMorpherUpdateTask();
    virtual const NiRTTI* GetRTTI() const;
    virtual void vf35();
    virtual bool vf37();

    void FUN_00a56bb0();
    static void* operator new[](size_t size, size_t count);

    MorphController* m_spMorphController; // src: XBOX_RETAIL_PDB +0xc (old: m_pkMorphController)
    FakeGeo* m_spTarget; // src: XBOX_RETAIL_PDB +0x10 (old: m_spReferencedObject)
};
