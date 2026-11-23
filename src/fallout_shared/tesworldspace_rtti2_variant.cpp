// src/fallout_shared/tesworldspace_rtti2_variant.cpp - world-space comparison.

#include "../../include/BGSAcousticSpace.h"
#include "../../include/EffectSetting.h"
#include "../../include/TESDataHandler.h"
#include "../../include/TESFile.h"
#include "../../include/TESForm.h"
#include "../../include/TESFullName.h"
#include "../../include/TESObjectACTI.h"
#include "../../include/TESObjectCELL.h"
#include "../../include/TESObjectREFR.h"
#include "../../include/TESRegion.h"
#include "../../include/TESWorldSpace.h"
#include "../../include/TESImageSpace.h"

extern "C" int __cdecl memcmp(const void*, const void*, size_t);

// Method-only extensions for declarations not yet exposed by the shared headers.
struct WorldImageSpaceComparison : TESImageSpace
{
    bool ImpactSwap_Compare(TESImageSpace*);
};

struct WorldSpaceComparison : TESWorldSpace
{
    void* ImageSpace();
    void* Water();
    float Height();
    void* Map();
    bool TESWorldSpace_Compare(TESForm*);
};
// @fnv 0x00585250
// src: XBOX_PDB 0x00585250
// pdb: TESWorldSpace::Compare | TESWorldSpace.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pComp
bool WorldSpaceComparison::TESWorldSpace_Compare(TESForm* apCompare)
{
    WorldSpaceComparison* other = (WorldSpaceComparison*)dynamic_cast< ::TESWorldSpace*>(apCompare);
    if (!other)
        return true;
    if (TESForm::Compare(other))
        return true;
    if (cFlags != other->cFlags)
        return true;
    if (sParentUseFlags != other->sParentUseFlags)
        return true;
    if (pParentWorld != other->pParentWorld)
        return true;
    if (pEncounterZone != other->pEncounterZone)
        return true;
    if (GetClimate() != other->GetClimate())
        return true;
    if (ImageSpace() != other->ImageSpace())
        return true;
    if (pImpactSwap && other->pImpactSwap)
    {
        if (((WorldImageSpaceComparison*)pImpactSwap)->ImpactSwap_Compare(other->pImpactSwap))
            return true;
    }
    else if (pImpactSwap || other->pImpactSwap)
        return true;
    if (Water() != other->Water())
        return true;
    if (GetLODWaterType() != other->GetLODWaterType())
        return true;
    if (Height() != other->Height())
        return true;
    if (fDefaultLandHeight != other->fDefaultLandHeight || fDefaultWaterHeight != other->fDefaultWaterHeight)
        return true;
    if (memcmp(&WorldMapData, &other->WorldMapData, 16))
        return true;
    if (memcmp(WorldMapData + 0x10, other->WorldMapData + 0x10, 12))
        return true;
    if (Map() != other->Map())
        return true;
    if (CanopyShadowTexture.CompareWithBase((BaseFormComponent*)&other->CanopyShadowTexture))
        return true;
    if (WaterNoiseTexture.CompareWithBase((BaseFormComponent*)&other->WaterNoiseTexture))
        return true;
    return false;
}
