
// extradatalist_ctx_variant.cpp: context variant of src/fallout_shared/extradatalist.cpp: its stripped skeleton, as these functions were checked
// against, for verified matches whose own declarations clash with the unit's current types.
// Use the shared extra-data base while retaining the packet's support templates.
#include "../../include/BSExtraData.h"
#include "../../include/BaseExtraList.h"
#include "../../include/layout_support.h"
#include "ExtraContainerChanges.h"
#include "InventoryChanges.h"
#include "game_constants.h"

// These support types are already supplied by the packet's generated headers.
#pragma include_alias("NiPoint3.h", "../../include/NiPoint3.h")
#pragma include_alias("NiAVObject.h", "../../include/layout_support.h")
#pragma include_alias("MagicTarget.h", "../../include/layout_support.h")
#pragma include_alias("TESForm.h", "../../include/TESForm.h")
#pragma include_alias("BaseFormComponent.h", "../../include/BaseFormComponent.h")
#pragma include_alias("TESFullName.h", "../../include/TESFullName.h")
#pragma include_alias("TESTexture.h", "../../include/TESTexture.h")
#include "../../include/BGSPrimitive.h"
#include "../../include/ExtraAction.h"
#include "../../include/ExtraActivateRef.h"
#include "../../include/ExtraAmmo.h"
#include "../../include/ExtraAudioBuoyMarker.h"
#include "../../include/ExtraAudioMarker.h"
#include "../../include/ExtraCell3D.h"
#include "../../include/ExtraCharge.h"
#include "../../include/ExtraCollisionData.h"
#include "../../include/ExtraCount.h"
#include "../../include/ExtraDecalRefs.h"
#include "../../include/ExtraDistantData.h"
#include "../../include/ExtraEditorRefMoveData.h"
#include "../../include/ExtraEmittanceSource.h"
#include "../../include/ExtraEnableStateParent.h"
#include "../../include/ExtraEncounterZone.h"
#include "../../include/ExtraFactionChanges.h"
#include "../../include/ExtraFollower.h"
#include "../../include/ExtraFollowerSwimBreadcrumbs.h"
#include "../../include/ExtraGlobal.h"
#include "../../include/ExtraHealth.h"
#include "../../include/ExtraHealthPerc.h"
#include "../../include/ExtraLevCreaModifier.h"
#include "../../include/ExtraLeveledItem.h"
#include "../../include/ExtraLinkedRef.h"
#include "../../include/ExtraLitWaterRefs.h"
#include "../../include/ExtraLock.h"
#include "../../include/ExtraMapMarker.h"
#include "../../include/ExtraMerchantContainer.h"
#include "../../include/ExtraMultiBound.h"
#include "../../include/ExtraMultiBoundRef.h"
#include "../../include/ExtraOcclusionPlaneRefData.h"
#include "../../include/ExtraOriginalReference.h"
#include "../../include/ExtraOwnership.h"
#include "../../include/ExtraPackage.h"
#include "../../include/ExtraPackageStartLocation.h"
#include "../../include/ExtraPatrolRefData.h"
#include "../../include/ExtraPoison.h"
#include "../../include/ExtraPortal.h"
#include "../../include/ExtraPortalRefData.h"
#include "../../include/ExtraPrimitive.h"
#include "../../include/ExtraRadiation.h"
#include "../../include/ExtraRadioData.h"
#include "../../include/ExtraRadius.h"
#include "../../include/ExtraRagDollData.h"
#include "../../include/ExtraRandomTeleportMarker.h"
#include "../../include/ExtraRank.h"
#include "../../include/ExtraReferencePointer.h"
#include "../../include/ExtraReflectedRefs.h"
#include "../../include/ExtraReflectorRefs.h"
#include "../../include/ExtraRefractionProperty.h"
#include "../../include/ExtraRoom.h"
#include "../../include/ExtraRoomRefData.h"
#include "../../include/ExtraRunOncePacks.h"
#include "../../include/ExtraSayTopicInfoOnceADay.h"
#include "../../include/ExtraScale.h"
#include "../../include/ExtraScript.h"
#include "../../include/ExtraSeed.h"
#include "../../include/ExtraSpecialRenderFlags.h"
#include "../../include/ExtraTeleport.h"
#include "../../include/ExtraTimeLeft.h"
#include "../../include/ExtraTresPassPackage.h"
#include "../../include/ExtraUses.h"
#include "../../include/ExtraWaterLightRefs.h"
#include "../../include/ExtraWeaponModFlags.h"
#include "../../include/ExtraXTarget.h"
#include "../../include/TESWorldSpace.h"
#pragma include_alias("NiAVObject.h", "NiAVObject.h")
// Keep the packet's inline support templates separate from the scene graph declarations.
#include "../../include/BSMultiBoundNode.h"
#pragma include_alias("NiPoint3.h", "NiPoint3.h")
#pragma include_alias("MagicTarget.h", "MagicTarget.h")
#pragma include_alias("TESForm.h", "TESForm.h")
#pragma include_alias("BaseFormComponent.h", "BaseFormComponent.h")
#pragma include_alias("TESFullName.h", "TESFullName.h")
#pragma include_alias("TESTexture.h", "TESTexture.h")

void* memcpy(void* apDest, const void* apSrc, size_t auiSize);

class BGSPortal;
class BGSAcousticSpace;
class BGSEncounterZone;
class BGSMusicType;
class TESClimate;
class TESImageSpace;
class TESGlobal;
class TESFaction;
class TESWaterForm;
class Script;
class ScriptLocals;
class TESTopicInfo;
class BSMultiBoundRoom;
class BSOcclusionShape;
class BSPortal;
class CanopyShadowMaskData;
class RagDollData;
class LevCreaModifierData;
class RadioData;

int Warning(const char* format, ...);

class NiRefCounted
{
  public:
    void IncRefCount();
    void DecRefCount();
};

class NiCell3DNode : public NiRefCounted
{
};
class NiCanopyTexture : public NiRefCounted
{
};
class BSRoomNode : public NiRefCounted
{
};
class BSOcclusionShapeNode : public NiRefCounted
{
};
class BSPortalNode : public NiRefCounted
{
};

// The original out-of-line byte accessor (0x004f1540) takes its object in ECX.
// Keep that call while the shared BSExtraData header has no GetType declaration.
unsigned char __fastcall GetExtraDataType(BSExtraData* apExtra);

struct LockExtraData
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    LockExtraData();
};

struct TeleportableReference
{
    int unk00, unk04, unk08, unk0C, unk10, unk14, unk18, unk1C;
    TeleportableReference();
    void Copy(TeleportableReference* apSource);
};

// pdb: class MapMarkerData, size 0x14, 5 members
struct MapMarkerData
{
    int LocationName, unk04, unk08, cFlags, pReputation; // src: XBOX_RETAIL_PDB LocationName +0x0 (old: unk00); cFlags +0xc (old: unk0C); pReputation +0x10 (old: unk10)
    MapMarkerData();
    void Copy(MapMarkerData* apCopy);
};

struct MediaLocation
{
    int unk00, unk04, unk08, unk0C, unk10, unk14, unk18, unk1C, unk20, unk24, unk28, unk2C, unk30;
    MediaLocation();
    void AudioMarkerData_Copy(MediaLocation* apCopy);
};

struct TwoIntegerValues
{
    int unk00, unk04;
    TwoIntegerValues();
    void Copy(TwoIntegerValues* apSource);
};

// pdb: class ImpactSwap, size 0x15c, 2 members
struct ImpactSwap
{
    int pRemappedData, unk004, unk008, unk00C, unk010, unk014, unk018, unk01C, unk020, unk024, unk028, unk02C, pAltFootstepMaterialNames, unk034, // src: XBOX_RETAIL_PDB pRemappedData +0x0 (old: unk000); pAltFootstepMaterialNames +0x30 (old: unk030)
        unk038, unk03C, unk040, unk044, unk048, unk04C, unk050, unk054, unk058, unk05C, unk060, unk064, unk068, unk06C,
        unk070, unk074, unk078, unk07C, unk080, unk084, unk088, unk08C, unk090, unk094, unk098, unk09C, unk0A0, unk0A4,
        unk0A8, unk0AC, unk0B0, unk0B4, unk0B8, unk0BC, unk0C0, unk0C4, unk0C8, unk0CC, unk0D0, unk0D4, unk0D8, unk0DC,
        unk0E0, unk0E4, unk0E8, unk0EC, unk0F0, unk0F4, unk0F8, unk0FC, unk100, unk104, unk108, unk10C, unk110, unk114,
        unk118, unk11C, unk120, unk124, unk128, unk12C, unk130, unk134, unk138, unk13C, unk140, unk144, unk148, unk14C,
        unk150, unk154, unk158;
    ImpactSwap();
    void Copy(ImpactSwap* apSource);
};

struct Point3Offset
{
    int unk00, unk04, unk08;
    Point3Offset();
    void Copy(Point3Offset* apSource);
};

// pdb: class PatrolRefData, size 0x14, 2 members
struct PatrolRefData
{
    int fTimeAtRef, OnPatrol, unk08, unk0C, unk10; // src: XBOX_RETAIL_PDB fTimeAtRef +0x0 (old: unk00); OnPatrol +0x4 (old: unk04)
    PatrolRefData();
    void Copy(PatrolRefData* apSource);
};

struct PatrolReferenceData
{
    TESObjectREFR* m_pPatrolReferences[4];
    PatrolReferenceData();
};

struct PortalRefData
{
    TESObjectREFR* pRooms[2];
};

struct RoomRefData
{
    int unk00;
    int unk04;
    BSSimpleList<TESObjectREFR*> kLinkedRooms;
    unsigned char bIsMaster;
};

// pdb: class CollisionData, size 0x4, 1 members
struct CollisionData
{
    unsigned int GetCollisionLayer() const;
    void SetCollisionLayer(unsigned int auiLayer);
    unsigned int eLayer; // src: XBOX_RETAIL_PDB +0x0 (old: uiCollisionLayer)
    CollisionData();
};

struct PackageReference
{
    TESPackage* pPackage;
    unsigned char bPickResultFound;
};

struct FactionRankData
{
    TESFaction* pFaction;
    char m_ucRank;
};

struct CanopyShadowMaskData
{
    int unk00;
    int unk04;
};


class ExtraHotkey : public BSExtraData
{
  public:
    unsigned char cKey;
};
class ExtraHeadTrackTarget : public BSExtraData
{
  public:
    TESObjectREFR* pTarget;
};
class ExtraWaterType : public BSExtraData
{
  public:
    TESWaterForm* m_pWaterForm;
};
class ExtraMusicType : public BSExtraData
{
  public:
    BGSMusicType* pMusic;
};
class ExtraAcousticSpace : public BSExtraData
{
  public:
    BGSAcousticSpace* m_pPathingCell;
};
class ExtraCanopyShadowMask : public BSExtraData
{
  public:
    int iMask;
    NiPointer<NiCanopyTexture> spTexture;
    int unk14;
    int unk18;
};
class ExtraClimate : public BSExtraData
{
  public:
    TESClimate* pClimateData;
};
class ExtraImageSpace : public BSExtraData
{
  public:
    TESImageSpace* m_pImageSpace;
};
class ExtraImpactSwap : public BSExtraData
{
  public:
    ImpactSwap* pSwap;
};
class ExtraMultiBoundMarkerData : public BSExtraData
{
  public:
    Point3Offset* pData;
};
class ExtraOcclusionShape : public BSExtraData
{
  public:
    NiPointer<BSOcclusionShapeNode> pShape;
};

// The shared render-flags class has no default constructor declaration.
// Supply only the construction entry point while inheriting its checked layout.
class ConstructibleExtraSpecialRenderFlags : public ExtraSpecialRenderFlags
{
  public:
    ConstructibleExtraSpecialRenderFlags();
};

#include "../../include/ExtraDataList.h"

// @fnv 0x004103d0
// src: XBOX_PDB 0x004103d0
// pdb: ExtraDataList::AddExtraCopy | ExtraDataList.obj | retail, string-set bridge, high
// pdb params: apCopy
// pdb locals: plist, pextra, pcopyextra, pcopy, pLock, pTel, pmapdata, paudiodata, pintcell, pworld, pactor, prect, pswap, pRun, cday, pPack, pdata, pcopydata, pext, pfac, pprimitive, pdataCopy, pcopyData, i ...
void ExtraDataList::AddExtraCopy(BSExtraData* apCopy)
{
    if (!apCopy || !AttachExtraData(apCopy))
        return;

    BSSimpleList<PackageReference*>* pIter = 0;
    switch (GetExtraDataType(apCopy))
    {
    case 0x4C: {
        ExtraEditorRefMoveData* pMoveData = (ExtraEditorRefMoveData*)GetExtraData(0x4C);
        if (!pMoveData)
        {
            pMoveData = new ExtraEditorRefMoveData(0);
            AddExtra(pMoveData);
        }
        ExtraEditorRefMoveData* pSource = (ExtraEditorRefMoveData*)apCopy;
        pMoveData->oldLocation = pSource->oldLocation;
        pMoveData->realAngle = pSource->realAngle;
        pMoveData->realLocation = pSource->realLocation;
        break;
    }
    case 0x0E:
        GetLocalData(((ExtraAction*)apCopy)->eAction);
        break;
    case 0x21:
        SetOwnership((TESForm*)((ExtraOwnership*)apCopy)->pOwner);
        break;
    case 0x22:
        SetGlobal(((ExtraGlobal*)apCopy)->pGlobalForm);
        break;
    case 0x23:
        SetRank(((ExtraRank*)apCopy)->m_iRank);
        break;
    case 0x24:
        SetCount(((ExtraCount*)apCopy)->iCount);
        break;
    case 0x6E: {
        ExtraAmmo* pAmmo = (ExtraAmmo*)apCopy;
        SetAmmoExtra(reinterpret_cast<TESForm*>(pAmmo->pAmmo), pAmmo->iCount);
        break;
    }
    case 0x16:
        SetEquippedState(true, false);
        break;
    case 0x4A:
        SetHotKey(((ExtraHotkey*)apCopy)->cKey);
        break;
    case 0x17:
        SetEquippedState(true, true);
        break;
    case 0x31:
        SetSeedIndex(((ExtraSeed*)apCopy)->seed);
        break;
    case 0x2F:
        SetLeveledItem(((ExtraLeveledItem*)apCopy)->iIndex);
        SetLevelItemExtraFlag(((ExtraLeveledItem*)apCopy)->m_bIsLeveledItem);
        break;
    case 0x25:
        SetHealth((((ExtraHealth*)apCopy)->fHealthPercent));
        break;
    case 0x7A:
        SetHealthPerc((((ExtraHealthPerc*)apCopy)->m_fHealthPercentage));
        break;
    case 0x26:
        SetUses(((ExtraUses*)apCopy)->m_ucUses);
        break;
    case 0x27:
        SetTimeLeft((((ExtraTimeLeft*)apCopy)->fTime));
        break;
    case 0x28:
        SetCharge((((ExtraCharge*)apCopy)->m_fCharge));
        break;
    case 0x0D:
        SetScript(((ExtraScript*)apCopy)->pScript);
        AssignScriptLocals(((ExtraScript*)apCopy)->m_pLocals);
        break;
    case 0x30:
        SetScale(((ExtraScale*)apCopy)->fScale);
        break;
    case 0x2A: {
        LockExtraData* pLock = new LockExtraData;
        memcpy(pLock, ((ExtraLock*)apCopy)->pLock, sizeof(LockExtraData));
        SetLockData(pLock);
        break;
    }
    case 0x68:
        SetRadioData((int*)((ExtraRadioData*)apCopy)->unk0C);
        break;
    case 0x2B: {
        TeleportableReference* pTeleport = new TeleportableReference;
        pTeleport->Copy(((ExtraTeleport*)apCopy)->data);
        SetTeleportData(pTeleport);
        break;
    }
    case 0x2C: {
        MapMarkerData* pTeleportMarker = new MapMarkerData;
        pTeleportMarker->Copy(((ExtraMapMarker*)apCopy)->data);
        SetMapMarkerPtr(pTeleportMarker);
        break;
    }
    case 0x90: {
        MediaLocation* pAudio = new MediaLocation;
        pAudio->AudioMarkerData_Copy(((ExtraAudioMarker*)apCopy)->pAudioData);
        SetAudioMarkerPtr(pAudio);
        break;
    }
    case 0x91: {
        TwoIntegerValues* pBuoy = new TwoIntegerValues;
        pBuoy->Copy((TwoIntegerValues*)((ExtraAudioBuoyMarker*)apCopy)->unk0C);
        SetAudioBuoyMarkerPtr(pBuoy);
        break;
    }
    case 0x18: {
        TESWorldSpace* pWorld = dynamic_cast<TESWorldSpace*>((TESForm*)((ExtraPackageStartLocation*)apCopy)->loc);
        TESObjectCELL* m_pOwningCell = dynamic_cast<TESObjectCELL*>((TESForm*)((ExtraPackageStartLocation*)apCopy)->loc);
        RelocateTo(pWorld, m_pOwningCell, (NiPoint3*)((ExtraPackageStartLocation*)apCopy)->pos,
                   *(float*)&((ExtraPackageStartLocation*)apCopy)->unk1C);
        break;
    }
    case 0x14:
        SetRagDollDataFromCopy(((ExtraRagDollData*)apCopy)->m_pRagDollData);
        break;
    case 0x13:
        SetWeaponModFlags(&((ExtraDistantData*)apCopy)->kDistantData);
        break;
    case 0x1D: {
        BSSimpleList<Actor*>* pFollowerIter = ((ExtraFollower*)apCopy)->list;
        while (pFollowerIter && pFollowerIter->GetItem())
        {
            Actor* pFollower = pFollowerIter->GetItem();
            SetCombatOpponent(pFollower);
            pFollowerIter = pFollowerIter->GetNext();
        }
        break;
    }
    case 0x46:
        SetHeadTrackTargetExtra(((ExtraHeadTrackTarget*)apCopy)->pTarget);
        break;
    case 0x3F:
        SetPoison((TESForm*)((ExtraPoison*)apCopy)->poison);
        break;
    case 0x02:
        SetCell3D(((ExtraCell3D*)apCopy)->spCellNode);
        break;
    case 0x03:
        SetWaterType(((ExtraWaterType*)apCopy)->m_pWaterForm);
        break;
    case 0x07:
        SetMusicType(((ExtraMusicType*)apCopy)->pMusic);
        break;
    case 0x81:
        SetAcousticSpace(((ExtraAcousticSpace*)apCopy)->m_pPathingCell);
        break;
    case 0x0A: {
        CanopyShadowMaskData* pMaskData;
        SetCanopyShadowMaskData(((ExtraCanopyShadowMask*)apCopy)->iMask, ((ExtraCanopyShadowMask*)apCopy)->spTexture,
                                &pMaskData);
        pMaskData->unk04 = ((ExtraCanopyShadowMask*)apCopy)->unk18;
        pMaskData->unk00 = ((ExtraCanopyShadowMask*)apCopy)->unk14;
        break;
    }
    case 0x08:
        SetClimate(((ExtraClimate*)apCopy)->pClimateData);
        break;
    case 0x59:
        SetImageSpace(((ExtraImageSpace*)apCopy)->m_pImageSpace);
        break;
    case 0x8C: {
        ImpactSwap* pSwap = new ImpactSwap;
        pSwap->Copy(((ExtraImpactSwap*)apCopy)->pSwap);
        SetImpactSwapData(pSwap);
        break;
    }
    case 0x1C:
        SetActivateRef(((ExtraReferencePointer*)apCopy)->pRef);
        break;
    // The third package byte follows unk19 in the shared class's tail padding.
    case 0x19:
        SetPackageOverride((TESPackage*)((ExtraPackage*)apCopy)->package, ((ExtraPackage*)apCopy)->unk10,
                           (int)((ExtraPackage*)apCopy)->form14, (unsigned char)((ExtraPackage*)apCopy)->bActionComplete,
                           (unsigned char)((ExtraPackage*)apCopy)->bActivated, ((unsigned char*)&((ExtraPackage*)apCopy)->bActivated)[1]);
        break;
    case 0x1A:
        SetActivePackage((TESPackage*)((ExtraTresPassPackage*)apCopy)->package);
        break;
    case 0x1B:
        pIter = (BSSimpleList<PackageReference*>*)((ExtraRunOncePacks*)apCopy)->list;
        while (pIter)
        {
            PackageReference* pRunOnce = pIter->GetItem();
            if (pRunOnce)
            {
                TESPackage* pPackage = pRunOnce->pPackage;
                unsigned char bPickResultFound = pRunOnce->bPickResultFound;
                SetPackageDoneState(pPackage, bPickResultFound);
            }
            pIter = pIter->GetNext();
        }
        break;
    case 0x20:
        SetOriginalReferenceExtra(((ExtraOriginalReference*)apCopy)->m_pWorldReferenceInfo);
        break;
    case 0x37:
        SetEnableStateParentRef(((ExtraEnableStateParent*)apCopy)->pParent);
        SetEnableStateParentFlags(((ExtraEnableStateParent*)apCopy)->cFlags);
        break;
    case 0x3B:
        SetRandomTeleportMarker(((ExtraRandomTeleportMarker*)apCopy)->pTeleportMarker);
        break;
    case 0x51:
        SetLinkedRef(((ExtraLinkedRef*)apCopy)->m_pLinkedReference);
        break;
    case 0x53: {
        ExtraActivateRef* m_pFourthReference = (ExtraActivateRef*)GetExtraData(0x53);
        if (!m_pFourthReference)
        {
            m_pFourthReference = new ExtraActivateRef;
            AddExtra(m_pFourthReference);
        }
        m_pFourthReference->Copy(reinterpret_cast<BSExtraDataEx*>(apCopy));
        break;
    }
    case 0x57: {
        ExtraDecalRefs* pDecalRefs = (ExtraDecalRefs*)GetExtraData(0x57);
        if (!pDecalRefs)
        {
            pDecalRefs = new ExtraDecalRefs;
            AddExtra(pDecalRefs);
        }
        pDecalRefs->Copy(reinterpret_cast<BSExtraDataEx*>(apCopy));
        break;
    }
    case 0x65: {
        ExtraReflectedRefs* pReflectedRefs = (ExtraReflectedRefs*)GetExtraData(0x65);
        if (!pReflectedRefs)
        {
            pReflectedRefs = new ExtraReflectedRefs;
            AddExtra(pReflectedRefs);
        }
        pReflectedRefs->Copy(apCopy);
        break;
    }
    case 0x66: {
        ExtraReflectorRefs* pReflectorRefs = (ExtraReflectorRefs*)GetExtraData(0x66);
        if (!pReflectorRefs)
        {
            pReflectorRefs = new ExtraReflectorRefs;
            AddExtra(pReflectorRefs);
        }
        pReflectorRefs->Copy(apCopy);
        break;
    }
    case 0x85: {
        ExtraLitWaterRefs* pLitWaterRefs = (ExtraLitWaterRefs*)GetExtraData(0x85);
        if (!pLitWaterRefs)
        {
            pLitWaterRefs = new ExtraLitWaterRefs;
            AddExtra(pLitWaterRefs);
        }
        pLitWaterRefs->Copy(reinterpret_cast<BSExtraDataEx*>(apCopy));
        break;
    }
    case 0x84: {
        ExtraWaterLightRefs* pWaterLightRefs = (ExtraWaterLightRefs*)GetExtraData(0x84);
        if (!pWaterLightRefs)
        {
            pWaterLightRefs = new ExtraWaterLightRefs;
            AddExtra(pWaterLightRefs);
        }
        pWaterLightRefs->Copy(reinterpret_cast<BSExtraDataEx*>(apCopy));
        break;
    }
    case 0x3C:
        SetMerchantContainer(((ExtraMerchantContainer*)apCopy)->pContainer);
        break;
    case 0x44:
        SetXTarget(((ExtraXTarget*)apCopy)->pTarget);
        break;
    // These shared classes reserve the payload as alignment padding after BSExtraData.
    case 0x74:
        SetEncounterZone(*(BGSEncounterZone**)((char*)(ExtraEncounterZone*)apCopy + sizeof(BSExtraData)));
        break;
    case 0x67:
        SetEmittanceSource(((ExtraEmittanceSource*)apCopy)->pSource);
        break;
    case 0x63:
        SetMultiBoundRef(((ExtraMultiBoundRef*)apCopy)->m_pReference);
        break;
    case 0x62: {
        Point3Offset* pNewData = 0;
        Point3Offset* pSrcData = ((ExtraMultiBoundMarkerData*)apCopy)->pData;
        if (pSrcData)
        {
            pNewData = new Point3Offset;
            pNewData->Copy(pSrcData);
        }
        SetExtraRecord(pNewData);
        break;
    }
    case 0x1E:
        SetLevCreaModifier(((ExtraLevCreaModifier*)apCopy)->m_iModifier);
        break;
    case 0x48:
        SetRefractionAmount(true, ((ExtraRefractionProperty*)apCopy)->fRefractionPower);
        break;
    case 0x3E:
        SetCannotWear(true);
        break;
    case 0x55:
        break;
    case 0x5C:
        SetRadius(((ExtraRadius*)apCopy)->fRadius);
        break;
    // ExtraRadiation likewise stores its float in the shared class's tail padding.
    case 0x5D:
        SetRadiation(*(float*)((char*)(ExtraRadiation*)apCopy + sizeof(BSExtraData)));
        break;
    case 0x5E: {
        UpdateExtraDataListExtension();
        ExtraFactionChanges* pChanges = GetFactionChangesExtra();
        BSSimpleList<FactionRankData*>* pRankIter = (BSSimpleList<FactionRankData*>*)((ExtraFactionChanges*)apCopy)->m_pFactionChanges;
        while (pRankIter && pRankIter->GetItem())
        {
            FactionRankData* pRank = pRankIter->GetItem();
            if (pRank)
                pChanges->AddToFaction(pRank->pFaction, pRank->m_ucRank);
            pRankIter = pRankIter->GetNext();
        }
        break;
    }
    case 0x5A:
        SetNavMeshPortal(apCopy);
        break;
    case 0x6B: {
        BGSPrimitive* pSrcPrimitive = ((ExtraPrimitive*)apCopy)->m_pPrimitive;
        BGSPrimitive* pNewPrimitive = BGSPrimitive::Copy(pSrcPrimitive);
        AddPrimitive(pNewPrimitive);
        break;
    }
    case 0x61:
        SetMultiBound(((ExtraMultiBound*)apCopy)->spBound);
        break;
    case 0x79:
        SetRoomNode(((ExtraRoom*)apCopy)->spRoom);
        break;
    case 0x6F: {
        PatrolRefData* pPatrol = new PatrolRefData;
        pPatrol->Copy(((ExtraPatrolRefData*)apCopy)->pPatrolData);
        SetPatrolRefData(pPatrol);
        break;
    }
    case 0x71:
        SetOcclusionPlane(((ExtraOcclusionShape*)apCopy)->pShape);
        break;
    case 0x76: {
        PatrolReferenceData* pPlaneData = new PatrolReferenceData;
        ExtraOcclusionPlaneRefData* pOcclusionExtra = (ExtraOcclusionPlaneRefData*)apCopy;
        PatrolReferenceData* pSrcLinkedRefs = pOcclusionExtra->pData;
        for (int i = 0; i < 4; i++)
            pPlaneData->m_pPatrolReferences[i] = pSrcLinkedRefs->m_pPatrolReferences[i];
        SetRoomRefData(pPlaneData);
        break;
    }
    case 0x77: {
        ExtraPortalRefData* pPortalRefExtra = (ExtraPortalRefData*)apCopy;
        PortalRefData* pPortalRefData = (PortalRefData*)pPortalRefExtra->pData;
        for (int j = 0; j < 2; j++)
            SetRoomLinkedRoomRef(j, pPortalRefData->pRooms[j]);
        break;
    }
    case 0x7B: {
        ExtraRoomRefData* pRefExtra = (ExtraRoomRefData*)apCopy;
        RoomRefData* pRoomRefData = pRefExtra->pData;
        BSSimpleList<TESObjectREFR*>* pRoomLinkIter = &pRoomRefData->kLinkedRooms;
        while (pRoomLinkIter && pRoomLinkIter->GetItem())
        {
            AddRoomLinkedRoomRef(pRoomLinkIter->GetItem());
            pRoomLinkIter = pRoomLinkIter->GetNext();
        }
        SetRoomIsMaster(pRoomRefData->bIsMaster != 0);
        break;
    }
    case 0x72: {
        CollisionData* pCollision = new CollisionData;
        pCollision->SetCollisionLayer(((ExtraCollisionData*)apCopy)->pCollisionData->GetCollisionLayer());
        SetCollisionData(pCollision);
        break;
    }
    case 0x73: {
        BSSimpleList<TESTopicInfo*>* pInfoIter = (BSSimpleList<TESTopicInfo*>*)((ExtraSayTopicInfoOnceADay*)apCopy)->pList;
        while (pInfoIter && pInfoIter->GetItem())
        {
            SetExtraSayTopicInfoOnceADayExtra(pInfoIter->GetItem());
            pInfoIter->GetNext();
        }
        break;
    }
    case 0x78: {
        BSPortalNode* pPortalNode = apCopy ? ((ExtraPortal*)apCopy)->spPortal : NiPointer<BSPortalNode>(0);
        SetPortalNode(pPortalNode);
        break;
    }
    case 0x80:
        SetIgnoredBySandbox(true);
        break;
    case 0x8B: {
        ExtraFollowerSwimBreadcrumbs* pDstBreadcrumbs = GetOrCreateExtraFollowerSwimBreadcrumbs();
        ExtraFollowerSwimBreadcrumbs* pSrcBreadcrumbs = (ExtraFollowerSwimBreadcrumbs*)apCopy;
        pDstBreadcrumbs->LeaderLocation = pSrcBreadcrumbs->LeaderLocation;
        pDstBreadcrumbs->LeaderNavMeshID = pSrcBreadcrumbs->LeaderNavMeshID;
        pDstBreadcrumbs->eLeaderState = pSrcBreadcrumbs->eLeaderState;
        pDstBreadcrumbs->CrumbList.RemoveAll();
        for (BSSimpleList<FollowerSwimBreadcrumb*>* pCrumbIter = &pSrcBreadcrumbs->CrumbList; pCrumbIter;
             pCrumbIter = pCrumbIter->GetNext())
        {
            if (pCrumbIter->GetItem())
                pDstBreadcrumbs->CrumbList.AddTail(pCrumbIter->GetItem());
        }
        break;
    }
    case 0x8D:
        if (((ExtraWeaponModFlags*)apCopy)->IsModActive(1))
            RemoveExtraByType(1);
        if (((ExtraWeaponModFlags*)apCopy)->IsModActive(2))
            RemoveExtraByType(2);
        if (((ExtraWeaponModFlags*)apCopy)->IsModActive(4))
            RemoveExtraByType(4);
        break;
    case 0x92: {
        ExtraSpecialRenderFlags* pRenderFlags = (ExtraSpecialRenderFlags*)GetExtraData(0x92);
        if (!pRenderFlags)
        {
            pRenderFlags = new ConstructibleExtraSpecialRenderFlags;
            AddExtra((BSExtraData*)pRenderFlags);
        }
        pRenderFlags->Copy_00438330(apCopy);
        break;
    }
    default:
        Warning("AI: No Copy function available for Extra Data type %i.", GetExtraDataType(apCopy));
        break;
    }
}
