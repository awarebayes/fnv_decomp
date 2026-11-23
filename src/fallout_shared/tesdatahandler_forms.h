// Unit form utilities, container support, and layout checks.
#ifndef FNV_TESDATAHANDLER_FORMS_H
#define FNV_TESDATAHANDLER_FORMS_H

void* __cdecl operator new(size_t);
void __cdecl operator delete(void*);
class NiNode;
#include "../../include/ENUM_FORM_ID.h"

class TESFormInitFlag
{
  public:
    static TESForm* LookupFormByEditorID(const char*);
    static TESForm* GetFormByID(unsigned int);
};
class TESFormList
{
  public:
    TESObject* AddToObjectList(TESObject*);
    TESObject* GetFirst();
};

typedef char check_TESObjectACTI_size[1];
typedef char check_BGSTalkingActivator_size[1];
typedef char check_BGSTerminal_size[1];
typedef char check_TESObjectARMO_size[1];
typedef char check_TESObjectARMA_size[1];
typedef char check_TESObjectBOOK_size[1];
typedef char check_TESObjectCLOT_size[1];
typedef char check_TESObjectCONT_size[1];
typedef char check_TESObjectDOOR_size[1];
typedef char check_IngredientItem_size[1];
typedef char check_TESObjectLIGH_size[1];
typedef char check_TESObjectMISC_size[1];
typedef char check_TESObjectIMOD_size[1];
typedef char check_BGSConstructibleObject_size[1];
typedef char check_TESKey_size[1];
typedef char check_TESObjectSTAT_size[1];
typedef char check_BGSStaticCollection_size[1];
typedef char check_BGSPlaceableWater_size[1];
typedef char check_TESGrass_size[1];
typedef char check_TESIdleForm_size[1];
typedef char check_BGSCameraPath_size[1];
typedef char check_BGSIdleMarker_size[1];
typedef char check_TESPackage_size[1];
typedef char check_TESObjectTREE_size[1];
typedef char check_TESFurniture_size[1];
typedef char check_TESObjectWEAP_size[1];
typedef char check_TESAmmo_size[1];
class ActorBaseData
{
  public:
    bool FUN_00461560();
    virtual void vfn_0();
    virtual void vfn_4();
    virtual void vfn_8();
    virtual void vfn_c();
    virtual void vfn_10(bool);
};
typedef char check_TESNPC_size[1];
typedef char check_TESCreature_size[1];
typedef char check_TESLevCreature_size[1];
typedef char check_TESLevCharacter_size[1];
typedef char check_SpellItem_size[1];
typedef char check_EnchantmentItem_size[1];
typedef char check_AlchemyItem_size[1];
typedef char check_TESLevItem_size[1];
typedef char check_TESObjectREFR_size[1];
typedef char check_BGSNote_size[1];
typedef char check_BGSMenuIcon_size[1];
typedef char check_BGSListForm_size[1];
typedef char check_BGSHeadPart_size[1];
typedef char check_TESHair_size[1];
typedef char check_TESEyes_size[1];
typedef char check_TESRace_size[1];
typedef char check_BGSEncounterZone_size[1];
typedef char check_TESClass_size[1];
typedef char check_TESFaction_size[1];
typedef char check_TESReputation_size[1];
typedef char check_TESChallenge_size[1];
typedef char check_TESSound_size[1];
typedef char check_BGSAcousticSpace_size[1];
typedef char check_TESGlobal_size[1];
typedef char check_BGSVoiceType_size[1];
typedef char check_BGSImpactData_size[1];
typedef char check_BGSImpactDataSet_size[1];
typedef char check_EffectSetting_size[1];
typedef char check_TESClimate_size[1];
typedef char check_TESImageSpace_size[1];
typedef char check_TESImageSpaceModifier_size[1];
typedef char check_TESWeather_size[1];
typedef char check_TESWorldSpace_size[1];
typedef char check_TESObjectCELL_size[1];
typedef char check_TESSkill_size[1];
typedef char check_TESLandTexture_size[1];
typedef char check_BGSCameraShot_size[1];
typedef char check_TESQuest_size[1];
typedef char check_TESRegion_size[1];
typedef char check_Script_size[1];
typedef char check_TESCombatStyle_size[1];
typedef char check_TESLoadScreen_size[1];
typedef char check_TESWaterForm_size[1];
typedef char check_TESLevSpell_size[1];
typedef char check_TESObjectANIO_size[1];
typedef char check_TESObjectLAND_size[1];
typedef char check_TESEffectShader_size[1];
typedef char check_BGSProjectile_size[1];
typedef char check_BGSExplosion_size[1];
typedef char check_BGSDebris_size[1];
typedef char check_BGSTextureSet_size[1];
typedef char check_NavMesh_size[1];
typedef char check_BGSPerk_size[1];
typedef char check_BGSBodyPartData_size[1];
typedef char check_BGSAddonNode_size[1];
typedef char check_ActorValueInfo_size[1];
typedef char check_BGSRadiationStage_size[1];
typedef char check_BGSDehydrationStage_size[1];
typedef char check_BGSHungerStage_size[1];
class BGSSleepDeprivationStage : public TESForm
{
  public:
    BGSSleepDeprivationStage();
    unsigned char formData[0x8];
};
typedef char check_BGSSleepDeprivationStage_size[1];
typedef char check_BGSMessage_size[1];
typedef char check_BGSRagdoll_size[1];
typedef char check_BGSLightingTemplate_size[1];
typedef char check_BGSMusicType_size[1];
class BGSRecipe : public TESForm
{
  public:
    BGSRecipe();
    unsigned char formData[0x44];
};
typedef char check_BGSRecipe_size[1];
class BGSRecipeCategory : public TESForm
{
  public:
    BGSRecipeCategory();
    unsigned char formData[0x10];
};
typedef char check_BGSRecipeCategory_size[1];
typedef char check_TESAmmoEffect_size[1];
typedef char check_TESCasinoChips_size[1];
typedef char check_TESLoadScreenType_size[1];
typedef char check_TESCasino_size[1];
typedef char check_MediaSet_size[1];
typedef char check_MediaLocationController_size[1];
typedef char check_TESCaravanCard_size[1];
typedef char check_TESCaravanMoney_size[1];
typedef char check_TESCaravanDeck_size[1];
typedef char check_BGSMovableStatic_size[1];
typedef char check_TESFlora_size[1];
const char* GetStringForByte(unsigned char);
int Warning(const char*, ...);
#include "../../include/NiTMallocInterface.h"


extern TES* g_TES;
void EffectSettingCollection_AddEffect(EffectSetting*);
#endif
