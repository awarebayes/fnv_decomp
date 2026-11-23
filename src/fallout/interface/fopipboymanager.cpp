#include "../../../include/SettingT.h"
// fallout/interface/fopipboymanager.cpp: FOPipboyManager, FOPipboyManagerPC, SystemColorManager, PipboyInterface and more (29 functions)

#include "../../../include/ANIM_GROUP_SECTION.h"
#include "../../../include/ACTION_FLAGS.h"
#include "../../../include/FOPipboyManager.h"
#include "../../../include/StatsMenu.h"
#include "../../../include/NiNode.h"
#include "../../../include/ActorValueOwner.h"
#include "../../../include/NiDirectionalLight.h"
#include "../../../include/NiPointLight.h"
#include "../../../include/PlayerCharacter.h"
#include "../../../include/SettingT_GameSettingCollection_.h"
#include "../../../include/BSAnimGroupSequence.h"
#include "../../../include/BSAudio.h"
#include "../../../include/NiControllerSequence.h"
#include "../../../include/NiScreenElements.h"
#include "../../../include/TESAnimGroup.h"
#include "../../../include/AnimIdle.h"
#include "../../../include/GameTimerRecord.h"
#include "../../../include/NiColor.h"
#include "../../../include/NiColorA.h"
#include "../../../include/BSSoundHandle.h"

// Private PC declarations for this unit. Offsets and call signatures follow the PC disassembly.

#include <stddef.h>
#include "../../../include/BGSBodyPart.h"
#include "../../../include/BSShaderProperty.h"
#include "../../../include/NiProperty.h"
#include "../../../include/NiRTTI.h"
#include "../../../include/NiUpdateData.h"

// These shared owners still expose the old stand-in types in their method signatures.
// Declare only the typed calls needed here, retaining the shared owners' layouts.
class PipboyColorMethods : public FOPipboyManager
{
  public:
    void SetQuad(const NiColorA&);
    void FORenderedMenu_SetTintColor(const NiColorA&);
};
class PipboyAudioMethods : public BSAudio
{
  public:
    BSSoundHandle GetSoundHandleByEditorID(const char*, unsigned int);
};

// The shared light classes do not yet declare this color setter.
class PipboyLightMethods : public NiPointLight
{
  public:
    void NiLight_SetAmbientColor(const NiPoint3&);
};
// 0x006b0570 copies three floats at +0x2c here on the rendered Pipboy menu, not a path node.
class PipboyColorStoreView_006b0570
{
  public:
    void FORenderedMenu_SetScreenLightColor(const NiPoint3&);
};
unsigned int NiMaterialProperty_GetType();
unsigned int NiShadeProperty_GetType();
NiAVObject* NiDynamicCast(const NiRTTI*, NiAVObject*);
extern NiRTTI DAT_011f4428;
extern SettingT_GameSettingCollection_ DAT_011db33c, DAT_011db34c;
extern PlayerCharacter* g_thePlayer;
union INISettingValue
{
    bool b;
};
class INISettingCollection;
using ::SettingT;
extern SettingT<INISettingCollection> DAT_011db2cc;
extern SettingT_GameSettingCollection_ DAT_011db2dc, DAT_011db328, DAT_011db274;
extern NiRTTI DAT_011f4acc;
// Menu.h declares Interface for menu IDs; keep this unit's PC layout helper separate.
class PipboyInterface
{
  public:
    unsigned char interfaceState[0x174];
    FOPipboyManager* pPipboyManager;
    FOPipboyManager* GetPipboyManager();
    static PipboyInterface* InterfaceManager_GetSingleton();
    NiNode* GetRoot();
};
// pdb: class SystemColorManager, size 0x10 (here 0x1), 2 members
class SystemColorManager
{
  public:
    static SystemColorManager* GetSingleton();
    bool GetColor(unsigned int, NiColorA&);
};

typedef char FOPipboyManagerButtonsOffset[1];
typedef char FOPipboyManagerKnobsOffset[1];
typedef char FOPipboyManagerLightOffset[1];
typedef char InterfacePipboyManagerOffset[1];
typedef char NiUpdateDataSize[1];
typedef char NiMatrix3Size[1];
extern SettingT_GameSettingCollection_ DAT_011db514;

// @fnv 0x007f9070
// src: XBOX_RETAIL_PDB 0x007f9070
// pdb: FOPipboyManager::InitButtons | FOPipboyManager.obj | retail, string-set bridge, high
// pdb locals: pMesh, pGeometry
void FOPipboyManager::InitButtons()
{
    NiAVObject* button = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton01"));
    if (!button)
        button = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton01:0"));
    if (button)
    {
        NiNode* node;
        node = (NiNode*)NiDynamicCast(&DAT_011f4428,
                                      ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton01")));
        if (!node)
            node = (NiNode*)NiDynamicCast(
                &DAT_011f4428, ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton01:0")));
        m_cPipBoyButtonAvailability[0] = node->NiNode_GetAtChecked(0u);
        node = (NiNode*)NiDynamicCast(&DAT_011f4428,
                                      ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton02")));
        if (!node)
            node = (NiNode*)NiDynamicCast(
                &DAT_011f4428, ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton02:0")));
        m_cPipBoyButtonAvailability[1] = node->NiNode_GetAtChecked(0u);
        node = (NiNode*)NiDynamicCast(&DAT_011f4428,
                                      ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton03")));
        if (!node)
            node = (NiNode*)NiDynamicCast(
                &DAT_011f4428, ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("PipBoyButton03:0")));
        m_cPipBoyButtonAvailability[2] = node->NiNode_GetAtChecked(0u);
    }
    button = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("StatsGlow"));
    if (!button)
        button = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("StatsGlow:0"));
    if (button)
    {
        spButtonGlow[0] = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("StatsGlow"));
        if (!spButtonGlow[0])
            spButtonGlow[0] = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("StatsGlow:0"));
        spButtonGlow[1] = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("ItemsGlow"));
        if (!spButtonGlow[1])
            spButtonGlow[1] = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("ItemsGlow:0"));
        spButtonGlow[2] = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("DataGlow"));
        if (!spButtonGlow[2])
            spButtonGlow[2] = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("DataGlow:0"));
        SetDataGlowEnabled(false);
    }
}

// @fnv 0x007f99d0
// src: XBOX_RETAIL_PDB 0x007f99d0
// pdb: FOPipboyManager::InitKnobs | FOPipboyManager.obj | retail, string-set bridge, high
// pdb locals: pGeometry, ftabKnobIncrement, tmp, prn, i
void FOPipboyManager::InitKnobs()
{
    NiAVObject* pObject = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("ScrollKnob"));
    if (!pObject)
        pObject = ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("ScrollKnob:0"));
    if (pObject && pObject->IsSceneNode())
        m_pGaugeObjects[0] = (NiGeometry*)(pObject->IsSceneNode()->GetAt(0));
    m_pGaugeObjects[1] = (NiGeometry*)(((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("RadNeedle")));
    if (!m_pGaugeObjects[1])
        m_pGaugeObjects[1] = (NiGeometry*)(((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("RadNeedle:0")));
    m_pGaugeObjects[2] = (NiGeometry*)(((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("TabKnob")));
    if (!m_pGaugeObjects[2])
        m_pGaugeObjects[2] = (NiGeometry*)(((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("TabKnob:0")));
    fTabKnobPositions[0] = DAT_011db33c.Float()->f;
    fTabKnobPositions[4] = DAT_011db34c.Float()->f;
    float fStep = (fTabKnobPositions[4] - fTabKnobPositions[0]) / 4.0;
    for (unsigned int i = 1; i < 4; ++i)
        fTabKnobPositions[i] = fTabKnobPositions[i - 1] + fStep;
    fKnobPositions[2] = fTabKnobPositions[0];
    fKnobTargets[2] = fTabKnobPositions[0];
    fKnobRates[2] = 0.0f;
    fKnobTargets[1] = g_thePlayer ? 1.5707963267948966 - g_thePlayer->GetCurrentActorValue((ActorValue::Index)54) /
                                                                    StatsMenu::GetChartScale() * 3.1415927410125732
                                         : 0.0;
    fKnobPositions[1] = fKnobTargets[1];
    fKnobRates[1] = 0.0f;
    NiMatrix3 kLocalRotate;
    NiMatrix3 kParentRotate;
    if (m_pGaugeObjects[0])
    {
        kLocalRotate = ((NiAVObject*)m_pGaugeObjects[0])->GetRotate();
        kParentRotate = ((NiAVObject*)m_pGaugeObjects[0])->GetParent()->GetRotate();
        ((NiAVObject*)m_pGaugeObjects[0])->GetParent()->SetLocalRotation(kParentRotate * kLocalRotate);
        ((NiAVObject*)m_pGaugeObjects[0])->SetLocalRotation(fKnobPositions[0], 1.0f, 0.0f, 0.0f);
    }
    if (m_pGaugeObjects[1])
        ((NiAVObject*)m_pGaugeObjects[1])->SetLocalRotation(fKnobPositions[1], 0.0f, 0.0f, 1.0f);
    if (m_pGaugeObjects[2])
        ((NiAVObject*)m_pGaugeObjects[2])->SetLocalRotation(fKnobPositions[2], 0.0f, 0.0f, 1.0f);
}

// @fnv 0x007f8ba0
// pdb: FOPipboyManager::Initialize | FOPipboyManager.obj | retail, string-set bridge, high
// pdb locals: p3rdPersonScreenGeom, pPipboyScreen, newcolor, pPipboyNode, UpdateData
void FOPipboyManager::OnMenuOpen()
{
    NiNode* screenNode;
    if (DAT_011db2cc.Bool()[0] && !IsUnk28Set() && g_thePlayer && g_thePlayer->GetNode(true))
    {
        {
            struct Screens
            {
                NiAVObject* thirdPerson;
                NiAVObject* firstPerson;
            } screenObjects;
            screenObjects.thirdPerson = 0;
            screenObjects.firstPerson = 0;
            screenNode = 0;
            screenNode = (NiNode*)NiDynamicCast(
                &DAT_011f4428, ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("pipboyscreen")));
            if (!screenNode)
                screenNode = (NiNode*)NiDynamicCast(
                    &DAT_011f4428, ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("pipboyscreen:0")));
            FORenderedMenu::OnMenuOpen();
            SetTarget_00705fc0(PipboyInterface::InterfaceManager_GetSingleton()->GetRoot());
            if (screenNode)
            {
                screenObjects.firstPerson = screenNode->NiNode_GetAtChecked(0u);
                spPipboyRoot = screenNode->GetParent();
            }
            else
            {
                screenObjects.firstPerson = NiDynamicCast(
                    &DAT_011f4acc, ((const NiNode*)g_thePlayer->GetNode(true))->GetObjectByName(NiFixedString("pipboyscreen:0")));
                if (screenObjects.firstPerson)
                    spPipboyRoot = screenObjects.firstPerson->GetParent();
            }
            NiTexturingProperty_Map_SetTexture(g_thePlayer->GetNode(true));
            if (screenNode)
            {
                screenObjects.thirdPerson = NiDynamicCast(&DAT_011f4acc, screenNode->NiNode_GetAtChecked(0u));
                sp3rdPersonPipboyRoot = screenNode->GetParent();
                sp3rdPersonScreen = screenObjects.thirdPerson;
            }
            else if (screenObjects.firstPerson)
            {
                screenObjects.thirdPerson = screenObjects.firstPerson;
                sp3rdPersonPipboyRoot = screenObjects.firstPerson->GetParent();
                sp3rdPersonScreen = screenObjects.thirdPerson;
            }
            if (screenObjects.firstPerson && screenObjects.thirdPerson)
            {
                FORenderedMenu_SetupScreen(screenObjects.firstPerson);
                FORenderedMenu_SetupScreen(screenObjects.thirdPerson);
                FORenderedMenu_SetScreenGeometry(screenObjects.firstPerson);
            }
            InitButtons();
            InitKnobs();
            InitLight();
            if (spPipboyRoot)
            {
                NiUpdateData updateData(0.0f, false, false);
                spPipboyRoot->Update(updateData);
                spPipboyRoot->UpdateSceneObject();
            }
            if (sp3rdPersonPipboyRoot)
            {
                NiUpdateData updateData(0.0f, false, false);
                sp3rdPersonPipboyRoot->Update(updateData);
                sp3rdPersonPipboyRoot->UpdateSceneObject();
            }
        }
        NiColorA kColor(0.0f, 0.0f, 0.0f, 0.0f);
        if (SystemColorManager::GetSingleton()->GetColor(4, kColor))
        {
            ((PipboyColorMethods*)this)->SetQuad(kColor);
            ((PipboyColorMethods*)this)->FORenderedMenu_SetTintColor(kColor);
        }
        SetBlurIntensity(DAT_011db2dc.Float()->f);
        TESQuest_SetScriptProcessingDelay(DAT_011db328.Float()->f);
        SetScanlines(true, DAT_011db274.Float()->f);
        bhkCharacterControllerCinfo_SetAvoidBox(true);
        SetPipBoyObjectVisibility(false, false, true);
        SetPipBoyObjectVisibility(true, false, true);
    }
}

// @fnv 0x007fa310
// pdb: FOPipboyManager::ShowPipboyLightEffect | FOPipboyManager.obj | retail, call graph (shared callers), high
// pdb params: ab3rdPerson, abShow, abUpdateVariable
// pdb locals: proot, plightEffect, plightNode, peffect, pmp, psp
void FOPipboyManager::SetPipBoyObjectVisibility(bool ab3rdPerson, bool abShow, bool abUpdateVariable)
{
    NiNode* pRoot = (ab3rdPerson ? sp3rdPersonPipboyRoot : spPipboyRoot);
    if (pRoot)
    {
        NiAVObject* pEffect = ((const NiNode*)pRoot)->GetObjectByName(NiFixedString("PipboyLightEffect"));
        if (!pEffect)
            pEffect = ((const NiNode*)pRoot)->GetObjectByName(NiFixedString("PipboyLightEffect:0"));
        if (pEffect)
        {
            pEffect->FUN_00450f90(!abShow);
            if (abUpdateVariable)
            {
                if (ab3rdPerson)
                {
                    b1stPersonLightOn = abShow;
                }
                else
                {
                    b3rdPersonLightOn = abShow;
                    NiNode* pNode = pEffect->IsSceneNode();
                    if (pNode && pNode->NiNode_GetChildCount())
                    {
                        NiGeometry* m_pShaderGeometry = pNode->GetAt(0)->IsGeometry();
                        if (m_pShaderGeometry)
                        {
                            NiProperty* pProperty = m_pShaderGeometry->GetProperty(NiMaterialProperty_GetType());
                            if (pProperty)
                                pProperty->NiMaterialProperty_SetAlpha((float)abShow);
                            else
                            {
                                BSShaderProperty* pShaderProperty =
                                    (BSShaderProperty*)m_pShaderGeometry->GetProperty(NiShadeProperty_GetType());
                                pShaderProperty->SetAlpha((float)abShow);
                            }
                            m_pShaderGeometry->UpdateSceneObject();
                        }
                    }
                }
            }
        }
    }
    else if (abUpdateVariable)
    {
        if (ab3rdPerson)
            b1stPersonLightOn = abShow;
        else
            b3rdPersonLightOn = abShow;
    }
}

// The callers receive PipboyInterface::FUN_004b7210() and then access the Pipboy manager.
// @fnv 0x00704370
FOPipboyManager* PipboyInterface::GetPipboyManager()
{
    return pPipboyManager;
}

// @fnv 0x007f9840
// src: XBOX_RETAIL_PDB 0x007f9840
// pdb: FOPipboyManager::InitLight | FOPipboyManager.obj | retail, string-set bridge, high
// pdb locals: plight, newcolor, screenLightColor
void FOPipboyManager::InitLight()
{
    if (!CreateMenuLight("Omni01", 0))
        CreateMenuLight("Omni01:0", 0);
    NiPointLight* light = (NiPointLight*)((BGSBodyPart*)this)->GetPartName();
    struct PointStorage
    {
        float x, y, z;
    } rgb;
    if (light)
    {
        light->SetEffectColor(NiPoint3(0.0f, 0.0f, 0.0f));
        ((PipboyLightMethods*)light)->NiLight_SetAmbientColor(NiPoint3(0.0f, 0.0f, 0.0f));
        light->FUN_0050c630(0.0f);
        light->FUN_0050c660(0.0f);
        light->FUN_0050c690(0.0f);
        light->NiLight_SetDimmer(1.0f);
        light->FUN_0050dd50(NiPoint3(DAT_011db514.Float()->f, 0.0f, 0.0f));
        NiColorA color(0.0f, 0.0f, 0.0f, 0.0f);
        SystemColorManager::GetSingleton()->GetColor(4, color);
        ((NiPoint3*)&rgb)->NiPoint3::NiPoint3(color.r, color.g, color.b);
        ((PipboyColorStoreView_006b0570*)this)->FORenderedMenu_SetScreenLightColor((const NiPoint3&)rgb);
        SetIntensity(1.0f);
        SetScreenLightFlickerIntensity(1.0f);
    }
}
// @fnv 0x007f9050
// pdb: FORenderedMenu::SetScreenParent | FOPipboyManager.obj | debug, object-file run extension, low
// pdb params: apScreenParent
void FOPipboyManager::SetScanlines(bool enabled, float scale)
{
    bDoScanlines = enabled;
    fScanlineScale = scale;
}
// @fnv 0x007f9010
// src: XBOX_DEBUG_PDB 0x007f9010
// pdb: FORenderedMenu::SetScreenGeometry | FOPipboyManager.obj | debug, call graph, high
// pdb params: apScreenTriShape
void FOPipboyManager::FORenderedMenu_SetScreenGeometry(NiAVObject* apScreenTriShape)
{
    spScreenTriShape = (NiGeometry*)apScreenTriShape;
}
// @fnv 0x007f99b0
// pdb: FORenderedMenu::GetMenuLight | FOPipboyManager.obj | debug, object-file gap fill, medium
void FOPipboyManager::SetScreenLightFlickerIntensity(float value)
{
    fScreenLightFlickerIntensity = value;
}
// @fnv 0x007f9030
void FOPipboyManager::SetBlurIntensity(float value)
{
    fBlurIntensity = value;
}

struct RenderedMenuNodeMethods : public FORenderedMenu
{
    void NiTexturingProperty_Map_SetTexture(NiAVObject*);
};

// @fnv 0x004dc540
// src: XBOX_PDB 0x004dc540
// pdb: NiTexturingProperty::Map::SetTexture | TESScriptFunctions.obj | debug, upstream name already matches, high
// pdb params: pkTexture
void RenderedMenuNodeMethods::NiTexturingProperty_Map_SetTexture(NiAVObject* pkTexture)
{
    spScreenParent = (NiNode*)pkTexture;
}

struct NiStream;
typedef char _sizecheck_NiColor[1];
struct RenderedMenuColorMethods : public FORenderedMenu
{
    void FORenderedMenu_SetScreenLightColor(NiColor*);
};

// Copies the NiColor pointed to by the argument into the field at offset 0x2c of this object.
// @fnv 0x006b0570
// src: COMMUNITY 0x006b0570
void RenderedMenuColorMethods::FORenderedMenu_SetScreenLightColor(NiColor* apColor)
{
    ScreenLightColor = *apColor;
}

// Stores afTarget and afRate into the per-knob arrays at offsets 0x118 and 0x124 for index aiKnob.
// @fnv 0x007f86f0
void FOPipboyManager::SetKnobTargetAndRate(int aiKnob, float afTarget, float afRate)
{
    fKnobTargets[aiKnob] = afTarget;
    fKnobRates[aiKnob] = afRate;
}

struct RenderedMenuIntensityMethods : public FORenderedMenu
{
    void SetIntensity(float);
};

// @fnv 0x00965580
void RenderedMenuIntensityMethods::SetIntensity(float afIntensity)
{
    fScreenLightBaseIntensity = afIntensity;
}

void __cdecl Deallocate(void*);
namespace p07 {
struct DtorStandIn
{
    void FUN_0083dec0();
};
struct DelStandIn
{
    void* FUN_007f7cb0(unsigned int flags);
};
// Calls the destructor, frees this when flags has bit 0 set, and returns this.
// @fnv 0x007f7cb0
// pdb: FOPipboyManager::`scalar deleting destructor' | FOPipboyManager.obj | debug, object-file gap fill, medium
void* DelStandIn::FUN_007f7cb0(unsigned int flags)
{
    ((DtorStandIn*)this)->FUN_0083dec0();
    if (flags & 1)
        Deallocate(this);
    return this;
}
}

// The shared header does not yet declare the pipboy reset override.
struct PipboyResetMethods : public FOPipboyManager
{
    void FOPipboyManager_ClearModels();
};

// Clears the pipboy node pointers, per-index arrays and screen elements, then calls FUN_007fadf0.
// @fnv 0x007f7e00
// src: COMMUNITY 0x007f7e00
// pdb: FOPipboyManager::Shutdown | FOPipboyManager.obj | debug, object-file gap fill, medium
// pdb locals: i
void PipboyResetMethods::FOPipboyManager_ClearModels()
{
    spPipboyRoot = 0;
    sp3rdPersonPipboyRoot = 0;
    spPartialScreenGeometry = 0;
    for (int i = 0; i < 3; i++)
    {
        m_cPipBoyButtonAvailability[i] = 0;
        spButtonGlow[i] = 0;
        m_pGaugeObjects[i] = 0;
        fKnobPositions[i] = 0;
        fKnobTargets[i] = 0;
        fKnobRates[i] = 0;
    }
    sp3rdPersonScreen = 0;
    FORenderedMenu::ResetNifs();
}

namespace fnv_007f8610 {
// The shared header does not yet declare the relative knob adjustment.
struct PipboyKnobMethods : public FOPipboyManager
{
    enum Knobs {};
    void FOPipboyManager_MoveKnob(Knobs, float, float, bool);
};

// @fnv 0x007f8610
// src: XBOX_PDB 0x007f8610
// pdb: FOPipboyManager::MoveKnob | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb params: aiKnob, afAmount, afRate, abPlaySound
// pdb locals: psoundHand
void PipboyKnobMethods::FOPipboyManager_MoveKnob(Knobs aiKnob, float afAmount, float afRate, bool abPlaySound)
{
    if (abPlaySound && aiKnob == 0)
    {
        BSSoundHandle soundHandle;
        soundHandle = ((PipboyAudioMethods*)BSAudio::GetSingleton())->GetSoundHandleByEditorID("UIPipBoyScroll", 0x121);
        soundHandle.Play(false);
    }
    fKnobTargets[aiKnob] += afAmount;
    fKnobRates[aiKnob] = afRate;
}
}  // namespace fnv_007f8610

namespace fnv_007f8a80 {
float __cdecl RandomFloat0To1();

class FORenderedMenuPC : public FORenderedMenu
{
  public:
    void FUN_007fb250(float, float);
    void FUN_007fb2b0(float, float, float);
};

extern SettingT_GameSettingCollection_ g_011db3b0;
extern SettingT_GameSettingCollection_ g_011db3bc;
extern SettingT_GameSettingCollection_ g_011db3fc;
extern SettingT_GameSettingCollection_ g_011db414;
extern SettingT_GameSettingCollection_ g_011db43c;
extern SettingT_GameSettingCollection_ g_011db480;
extern SettingT_GameSettingCollection_ g_011db4b4;
extern SettingT_GameSettingCollection_ g_011db4e0;
extern SettingT_GameSettingCollection_ g_011db4ec;

// The shared header does not yet declare the burst update.
struct PipboyBurstMethods : public FOPipboyManager
{
    void FOPipboyManager_TriggerRandomEffect();
};

// @fnv 0x007f8a80
// src: XBOX_PDB 0x007f8a80
// pdb: FOPipboyManager::TriggerRandomEffect | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb locals: fchance
void PipboyBurstMethods::FOPipboyManager_TriggerRandomEffect()
{
    float fchance;

    fchance = RandomFloat0To1();
    if (fchance < g_011db4b4.Float()->f)
        ((FORenderedMenuPC*)PipboyInterface::InterfaceManager_GetSingleton()->GetPipboyManager())
            ->FUN_007fb250(g_011db4ec.Float()->f, g_011db3fc.Float()->f);
    if (fchance >= g_011db4b4.Float()->f && fchance < g_011db4b4.Float()->f + g_011db4e0.Float()->f)
        ((FORenderedMenuPC*)PipboyInterface::InterfaceManager_GetSingleton()->GetPipboyManager())
            ->FUN_007fb2b0(g_011db414.Float()->f, g_011db3bc.Float()->f, g_011db3b0.Float()->f);
    ((FORenderedMenuPC*)PipboyInterface::InterfaceManager_GetSingleton()->GetPipboyManager())
        ->TriggerBurst(g_011db480.Float()->f, g_011db43c.Float()->f, 0.5f);
}
}  // namespace fnv_007f8a80

class Actor;
namespace fnv_007fa220 {
class Animation
{
  public:
    NiControllerSequence* GetControllerSequence(int);
    void Update(Actor*, float, float);
    void UpdateMovement(Actor*);
    bool IsGroupQueued(unsigned short);
    BSAnimGroupSequence* PlayGroup(unsigned short, ACTION_FLAGS, int, ANIM_GROUP_SECTION);
    void Animation_SkipUpdate(int);
};
struct FUN_00424940_shell
{
    char InterfaceManager_GetMenuMode();
};
struct Twin_0084d030
{
    float GetStoredFloatValue();
};
extern Twin_0084d030 g_timeGlobal;
bool __cdecl IsConsoleActive();

// The shared header does not yet declare the animation update.
struct PipboyAnimationMethods : public FOPipboyManager
{
    void FOPipboyManager_UpdateIdleAnims();
};

// @fnv 0x007fa220
// src: XBOX_PDB 0x007fa220
// pdb: FOPipboyManager::UpdateIdleAnims | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb locals: pidle, panim
void PipboyAnimationMethods::FOPipboyManager_UpdateIdleAnims()
{
    BSAnimGroupSequence* pIdleSequence;
    Animation* panim;

    if (IsConsoleActive())
        return;
    if (*(int*)((char*)PipboyInterface::InterfaceManager_GetSingleton() + 0x4bc) != 3)
        return;
    if (((FUN_00424940_shell*)PipboyInterface::InterfaceManager_GetSingleton())->InterfaceManager_GetMenuMode() == 4)
        return;
    panim = (Animation*)((const ::PlayerCharacter*)g_thePlayer)->GetAnimation(true);
    pIdleSequence = (BSAnimGroupSequence*)panim->GetControllerSequence(0);
    if (pIdleSequence && pIdleSequence->GetAnimGroup()->GetType() != 1 && panim->IsGroupQueued(1))
        panim->PlayGroup(1, (ACTION_FLAGS)1, -1, (ANIM_GROUP_SECTION)-1);
    panim->Animation_SkipUpdate(0x17);
    panim->Update((Actor*)g_thePlayer, g_timeGlobal.GetStoredFloatValue(), -1.0f);
    panim->UpdateMovement((Actor*)g_thePlayer);
}
}  // namespace fnv_007fa220

namespace fnv_007fa810 {
struct ReferenceLightData
{
    NiPointer<NiPointLight> m_pMenuLight;
};

float __cdecl BSMAX_00404010(float, float);
float __cdecl BSMIN(float, float);
ReferenceLightData* __fastcall ProcessAndReturnValue_0057bd40(PlayerCharacter* apPlayer);
extern double _gModifierZero;
extern SettingT_GameSettingCollection_ g_011db308;

}  // namespace fnv_007fa810

// @fnv 0x007fa810
// pdb: FOPipboyManager::FadeLight | FOPipboyManager.obj | debug, object-file gap fill, medium
// pdb params: afTime
// pdb locals: plightref, ffade, fdelta
void FOPipboyManager::UpdatePipboyLightFade(float afTime)
{
    using namespace fnv_007fa810;
    ReferenceLightData* plightref;
    float ffade;
    float fdelta;

    fdelta = afTime - fLightEffectFadeStartTime;
    ffade = BSMIN(fdelta / g_011db308.Float()->f, 1.0f);
    ffade = BSMAX_00404010(0.0f, ffade);
    if (fLightEffectFadeTarget == 0.0)
        ffade = 1.0f - ffade;
    plightref = ProcessAndReturnValue_0057bd40(g_thePlayer);
    if (plightref && plightref->m_pMenuLight)
        ((NiPointLight*)plightref->m_pMenuLight)->NiLight_SetDimmer(ffade);
}

namespace fnv_007ff960 {

ShadowSceneNode* __cdecl FUN_00450b80(int);
void __cdecl _FNV_Warning(const char*);

extern SettingT_GameSettingCollection_ g_011db5f8;
extern SettingT_GameSettingCollection_ g_011db608;
extern SettingT_GameSettingCollection_ g_011db644;
extern SettingT_GameSettingCollection_ g_011db650;
extern SettingT_GameSettingCollection_ g_011db668;

class NiAVObjectPC : public NiAVObject
{
  public:
    void Update(NiUpdateData&);
};

struct TerminalLightMethods : public FOPipboyManager
{
    NiPointLight* GetLight();
    void FORenderedMenu_SetScreenLightColor(const NiColor&);
    void FORenderedTerminal_InitLight();
};

// @fnv 0x007ff960
// src: XBOX_RETAIL_PDB 0x007ff960
// pdb: FORenderedTerminal::InitLight | FORenderedTerminal.obj | retail, call graph, high
// pdb locals: plight, screenLightColor, foo
void TerminalLightMethods::FORenderedTerminal_InitLight()
{
    NiPointLight* plight;

    if (!CreateMenuLight("Omni01", FUN_00450b80(1)))
    {
        _FNV_Warning("MENUS: Failed to create terminal light.  Terminal will only receive ambient light.");
        return;
    }
    plight = GetLight();
    if (plight)
    {
        ((PipboyLightMethods*)plight)->NiLight_SetAmbientColor(NiPoint3(0, 0, 0));
        plight->FUN_0050c630(0);
        plight->FUN_0050c660(0);
        plight->FUN_0050c690(0);
        plight->NiLight_SetDimmer(1.0f);
        plight->FUN_0050dd50(NiPoint3(g_011db644.Float()->f, 0, 0));
        {
            NiColor screenLightColor(g_011db608.Float()->f, g_011db668.Float()->f, g_011db5f8.Float()->f);
            FORenderedMenu_SetScreenLightColor(screenLightColor);
        }
        SetIntensity(g_011db650.Float()->f);
        SetScreenLightFlickerIntensity(1.0f);
        NiUpdateData updateData(0, false, false);
        ((NiAVObjectPC*)plight)->Update(updateData);
    }
}
}  // namespace fnv_007ff960

extern GameTimerRecord g_timeGlobal;
// The shared header does not yet declare the pipboy update override.
struct PipboyUpdateMethods : public FOPipboyManager
{
    void FOPipboyManager_UpdateKnobs(float);
    void FOPipboyManager_UpdateIdleAnims();
    bool FOPipboyManager_OnIdle();
};
// @fnv 0x007f7ee0
// src: COMMUNITY 0x007f7ee0
// pdb: FOPipboyManager::OnIdle | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb locals: fappTime
bool PipboyUpdateMethods::FOPipboyManager_OnIdle()
{
    float fappTime;
    if (!FORenderedMenu::UpdateScreenEffect())
        return false;
    fappTime = (float)(unsigned int)((AnimIdle*)&g_timeGlobal)->GetSection();
    this->FOPipboyManager_UpdateKnobs(fappTime);
    this->FOPipboyManager_UpdateIdleAnims();
    return true;
}

void __cdecl FORenderedTerminal_PlayShutdownSound();
void* __cdecl InterfaceManager_GetSingleton();
struct Im7181
{
    void SetRenderedMenu(int);
};
namespace ol4_00985000 {
struct CGBase
{
    void SetProcedureType(int);
};
}
struct VTab
{
    virtual int VFn(int);
};
struct FUN_007ffd50_shell
{
    void FORenderedMenu_Close();
};
// Calls FUN_007ffe40, invokes VFn(1) on the parent if present, then sets procedure type 0 and calls SetRenderedMenu(0) on the FUN_004b7210 result.
// @fnv 0x007ffd50
// src: COMMUNITY 0x007ffd50
void FUN_007ffd50_shell::FORenderedMenu_Close()
{
    VTab* parent;
    FORenderedTerminal_PlayShutdownSound();
    {
        NiNode* obj = ((NiAVObject*)this)->GetParent();
        parent = (VTab*)obj;
    }
    parent ? parent->VFn(1) : 0;
    ((ol4_00985000::CGBase*)this)->SetProcedureType(0);
    ((Im7181*)InterfaceManager_GetSingleton())->SetRenderedMenu(0);
}

// FUN_007fa990: thiscall; calls on two module globals, three far virtual calls
struct G02d98
{
    void LoadQueuedPriority(int);
};
class G11dea3c
{
  public:
    struct Obj* FUN_008d8520();
};

struct Obj
{
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual void v010() = 0;
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual void v015() = 0;
    virtual void v016() = 0;
    virtual void v017() = 0;
    virtual void v018() = 0;
    virtual void v019() = 0;
    virtual void v020() = 0;
    virtual void v021() = 0;
    virtual void v022() = 0;
    virtual void v023() = 0;
    virtual void v024() = 0;
    virtual void v025() = 0;
    virtual void v026() = 0;
    virtual void v027() = 0;
    virtual void v028() = 0;
    virtual void v029() = 0;
    virtual void v030() = 0;
    virtual void v031() = 0;
    virtual void v032() = 0;
    virtual void v033() = 0;
    virtual void v034() = 0;
    virtual void v035() = 0;
    virtual void v036() = 0;
    virtual void v037() = 0;
    virtual void v038() = 0;
    virtual void v039() = 0;
    virtual void v040() = 0;
    virtual void v041() = 0;
    virtual void v042() = 0;
    virtual void v043() = 0;
    virtual void v044() = 0;
    virtual void v045() = 0;
    virtual void v046() = 0;
    virtual void v047() = 0;
    virtual void v048() = 0;
    virtual void v049() = 0;
    virtual void v050() = 0;
    virtual void v051() = 0;
    virtual void v052() = 0;
    virtual void v053() = 0;
    virtual void v054() = 0;
    virtual void v055() = 0;
    virtual void v056() = 0;
    virtual void v057() = 0;
    virtual void v058() = 0;
    virtual void v059() = 0;
    virtual void v060() = 0;
    virtual void v061() = 0;
    virtual void v062() = 0;
    virtual void v063() = 0;
    virtual void v064() = 0;
    virtual void v065() = 0;
    virtual void v066() = 0;
    virtual void v067() = 0;
    virtual void v068() = 0;
    virtual void v069() = 0;
    virtual void v070() = 0;
    virtual void v071() = 0;
    virtual void v072() = 0;
    virtual void v073() = 0;
    virtual void v074() = 0;
    virtual void v075() = 0;
    virtual void v076() = 0;
    virtual void v077() = 0;
    virtual void v078() = 0;
    virtual void v079() = 0;
    virtual void v080() = 0;
    virtual void v081() = 0;
    virtual void v082() = 0;
    virtual void v083() = 0;
    virtual void v084() = 0;
    virtual void v085() = 0;
    virtual void v086() = 0;
    virtual void v087() = 0;
    virtual void v088() = 0;
    virtual void v089() = 0;
    virtual void v090() = 0;
    virtual void v091() = 0;
    virtual void v092(G11dea3c* a) = 0;
    virtual void v093() = 0;
    virtual void v094() = 0;
    virtual void v095() = 0;
    virtual void v096() = 0;
    virtual void v097() = 0;
    virtual void v098() = 0;
    virtual void v099() = 0;
    virtual void v100() = 0;
    virtual void v101() = 0;
    virtual void v102() = 0;
    virtual void v103() = 0;
    virtual void v104() = 0;
    virtual void v105() = 0;
    virtual void v106() = 0;
    virtual void v107() = 0;
    virtual void v108() = 0;
    virtual void v109() = 0;
    virtual void v110() = 0;
    virtual void v111() = 0;
    virtual void v112() = 0;
    virtual void v113() = 0;
    virtual void v114() = 0;
    virtual void v115() = 0;
    virtual void v116() = 0;
    virtual void v117() = 0;
    virtual void v118() = 0;
    virtual void v119() = 0;
    virtual void v120() = 0;
    virtual void v121() = 0;
    virtual void v122() = 0;
    virtual void v123() = 0;
    virtual void v124() = 0;
    virtual void v125() = 0;
    virtual void v126() = 0;
    virtual void v127() = 0;
    virtual void v128() = 0;
    virtual void v129() = 0;
    virtual void v130() = 0;
    virtual void v131() = 0;
    virtual void v132() = 0;
    virtual void v133() = 0;
    virtual void v134() = 0;
    virtual void v135() = 0;
    virtual void v136() = 0;
    virtual void v137() = 0;
    virtual void v138() = 0;
    virtual void v139() = 0;
    virtual void v140() = 0;
    virtual void v141() = 0;
    virtual void v142() = 0;
    virtual void v143() = 0;
    virtual void v144() = 0;
    virtual void v145() = 0;
    virtual void v146() = 0;
    virtual void v147() = 0;
    virtual void v148() = 0;
    virtual void v149() = 0;
    virtual void v150() = 0;
    virtual void v151() = 0;
    virtual void v152() = 0;
    virtual void v153() = 0;
    virtual void v154() = 0;
    virtual void v155() = 0;
    virtual void v156() = 0;
    virtual void v157() = 0;
    virtual void v158() = 0;
    virtual void v159() = 0;
    virtual void v160() = 0;
    virtual void v161() = 0;
    virtual void v162() = 0;
    virtual void v163() = 0;
    virtual void v164() = 0;
    virtual void v165() = 0;
    virtual void v166() = 0;
    virtual void v167() = 0;
    virtual void v168() = 0;
    virtual void v169() = 0;
    virtual void v170() = 0;
    virtual void v171() = 0;
    virtual void v172() = 0;
    virtual void v173() = 0;
    virtual void v174() = 0;
    virtual void v175() = 0;
    virtual void v176() = 0;
    virtual void v177() = 0;
    virtual void v178() = 0;
    virtual void v179() = 0;
    virtual void v180() = 0;
    virtual void v181() = 0;
    virtual void v182() = 0;
    virtual void v183() = 0;
    virtual void v184() = 0;
    virtual void v185() = 0;
    virtual void v186() = 0;
    virtual void v187() = 0;
    virtual void v188() = 0;
    virtual void v189() = 0;
    virtual void v190() = 0;
    virtual void v191() = 0;
    virtual void v192() = 0;
    virtual void v193() = 0;
    virtual void v194() = 0;
    virtual void v195() = 0;
    virtual void v196() = 0;
    virtual void v197() = 0;
    virtual void v198() = 0;
    virtual void v199() = 0;
    virtual void v200() = 0;
    virtual void v201() = 0;
    virtual void v202() = 0;
    virtual void v203() = 0;
    virtual void v204() = 0;
    virtual void v205() = 0;
    virtual void v206() = 0;
    virtual void v207() = 0;
    virtual void v208() = 0;
    virtual void v209() = 0;
    virtual void v210() = 0;
    virtual void v211() = 0;
    virtual void v212() = 0;
    virtual void v213() = 0;
    virtual void v214() = 0;
    virtual void v215() = 0;
    virtual void v216() = 0;
    virtual void v217() = 0;
    virtual void v218() = 0;
    virtual void v219() = 0;
    virtual void v220() = 0;
    virtual void v221() = 0;
    virtual void v222() = 0;
    virtual void v223() = 0;
    virtual void v224() = 0;
    virtual void v225() = 0;
    virtual void v226() = 0;
    virtual void v227() = 0;
    virtual void v228() = 0;
    virtual void v229() = 0;
    virtual void v230() = 0;
    virtual void v231() = 0;
    virtual void v232() = 0;
    virtual void v233() = 0;
    virtual void v234() = 0;
    virtual void v235() = 0;
    virtual void v236() = 0;
    virtual void v237() = 0;
    virtual void v238() = 0;
    virtual void v239() = 0;
    virtual void v240() = 0;
    virtual void v241() = 0;
    virtual void v242() = 0;
    virtual void v243() = 0;
    virtual void v244() = 0;
    virtual void v245() = 0;
    virtual void v246() = 0;
    virtual void v247() = 0;
    virtual void v248() = 0;
    virtual void v249() = 0;
    virtual void v250() = 0;
    virtual void v251() = 0;
    virtual void v252() = 0;
    virtual void v253() = 0;
    virtual void v254() = 0;
    virtual void v255() = 0;
    virtual void v256() = 0;
    virtual void v257() = 0;
    virtual void v258() = 0;
    virtual void v259() = 0;
    virtual void v260() = 0;
    virtual void v261() = 0;
    virtual void v262() = 0;
    virtual void v263() = 0;
    virtual void v264() = 0;
    virtual void v265() = 0;
    virtual void v266() = 0;
    virtual void v267() = 0;
    virtual void v268() = 0;
    virtual void v269() = 0;
    virtual void v270() = 0;
    virtual void v271() = 0;
    virtual void v272() = 0;
    virtual void v273() = 0;
    virtual void v274() = 0;
    virtual void v275() = 0;
    virtual void v276() = 0;
    virtual void v277() = 0;
    virtual void v278() = 0;
    virtual void v279() = 0;
    virtual void v280() = 0;
    virtual void v281(G11dea3c* a) = 0;
    virtual void v282(int a) = 0;
};

extern unsigned int DAT_01202d98;
extern unsigned int DAT_011dea3c;

// The shared header does not yet declare this pipboy open helper.
class PipboyOpenMethods : public FOPipboyManager
{
  public:
    void FOPipboyManager_ResetPipBoyManager();
    void BSShaderAccumulator_SetRenderDecals(int a);
};

// Calls FUN_00c3dfa0(5) on DAT_01202d98, obtains an object from DAT_011dea3c, invokes v092, v282(1) and v281 on it, then FUN_007fadf0, OnMenuOpen and FUN_005db4c0(0).
// @fnv 0x007fa990
// src: XBOX_PDB 0x007fa990
// pdb: FOPipboyManager::ResetPipBoyManager | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb locals: pproc
void PipboyOpenMethods::FOPipboyManager_ResetPipBoyManager()
{
    reinterpret_cast<G02d98*>(DAT_01202d98)->LoadQueuedPriority(5);
    Obj* obj = reinterpret_cast<G11dea3c*>(DAT_011dea3c)->FUN_008d8520();
    obj->v092(reinterpret_cast<G11dea3c*>(DAT_011dea3c));
    obj->v282(1);
    obj->v281(reinterpret_cast<G11dea3c*>(DAT_011dea3c));
    ResetNifs();
    OnMenuOpen();
    BSShaderAccumulator_SetRenderDecals(0);
}

// @fnv 0x007f7a70
// pdb: FOPipboyManager::FOPipboyManager | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb locals: i
FOPipboyManager::FOPipboyManager()
    : spPipboyRoot(0), sp3rdPersonPipboyRoot(0), sp3rdPersonScreen(0), spPartialScreenGeometry(0)
{
    spPipboyRoot = 0;
    sp3rdPersonPipboyRoot = 0;
    spPartialScreenGeometry = 0;
    for (int index = 0; index < 3; ++index)
    {
        m_cPipBoyButtonAvailability[index] = 0;
        spButtonGlow[index] = 0;
        m_pGaugeObjects[index] = 0;
        fKnobPositions[index] = 0.0f;
        fKnobTargets[index] = 0.0f;
        fKnobRates[index] = 0.0f;
    }
    bScaledTexCoords = false;
    bPipboyUp = false;
    b1stPersonLightOn = false;
    b3rdPersonLightOn = false;
    Init(true);
    bLightEffectFading = false;
    sp3rdPersonScreen = 0;
}

bool FUN_00705ea0();
void Interface_HideMenus();

// Destructor that, if FUN_00705ea0() returns true, calls FUN_00703610().
// @fnv 0x007f7ce0
// pdb: FOPipboyManager::~FOPipboyManager | FOPipboyManager.obj | retail, upstream name already matches, high
FOPipboyManager::~FOPipboyManager()
{
    if (FUN_00705ea0())
        Interface_HideMenus();
}

namespace fnv_007f8320 {
extern float DAT_011db370;

class NiAVObjectHelper : public NiAVObject
{
  public:
    void FUN_006404b0(float, float, float, float);
};

// The shared header does not yet declare the knob animation update.
class PipboyKnobUpdateMethods : public FOPipboyManager
{
  public:
    void FOPipboyManager_UpdateKnobs(float afTime);
};

// @fnv 0x007f8320
// src: COMMUNITY 0x007f8320
// pdb: FOPipboyManager::UpdateKnobs | FOPipboyManager.obj | retail, upstream name already matches, high
// pdb params: afTime
// pdb locals: fdelta, bKnobChanged, i, UpdateData
void PipboyKnobUpdateMethods::FOPipboyManager_UpdateKnobs(float afTime)
{
    bool bUpdated[3];
    float fMoveElapsedTime = afTime - DAT_011db370;
    bUpdated[0] = false;
    bUpdated[1] = false;
    bUpdated[2] = false;

    for (int i = 0; i < 3; i++)
    {
        if (fKnobTargets[i] > fKnobPositions[i])
        {
            fKnobPositions[i] += fKnobRates[i] * fMoveElapsedTime;
            if (fKnobPositions[i] > fKnobTargets[i])
            {
                fKnobPositions[i] = fKnobTargets[i];
            }
            bUpdated[i] = true;
        }
        if (fKnobTargets[i] < fKnobPositions[i])
        {
            fKnobPositions[i] -= fKnobRates[i] * fMoveElapsedTime;
            if (fKnobPositions[i] < fKnobTargets[i])
            {
                fKnobPositions[i] = fKnobTargets[i];
            }
            bUpdated[i] = true;
        }
    }

    if (m_pGaugeObjects[0] && bUpdated[0])
    {
        ((NiAVObjectHelper*)(NiAVObject*)m_pGaugeObjects[0])->FUN_006404b0(fKnobPositions[0], 1.0f, 0.0f, 0.0f);
        NiUpdateData updateData(0.0f, false, false);
        ((NiAVObject*)m_pGaugeObjects[0])->Update(updateData);
    }

    if (m_pGaugeObjects[1] && bUpdated[1])
    {
        ((NiAVObjectHelper*)(NiAVObject*)m_pGaugeObjects[1])->FUN_006404b0(fKnobPositions[1], 0.0f, 0.0f, 1.0f);
        NiUpdateData updateData(0.0f, false, false);
        ((NiAVObject*)m_pGaugeObjects[1])->Update(updateData);
    }

    if (m_pGaugeObjects[2] && bUpdated[2])
    {
        ((NiAVObjectHelper*)(NiAVObject*)m_pGaugeObjects[2])->FUN_006404b0(fKnobPositions[2], 0.0f, 0.0f, 1.0f);
        NiUpdateData updateData(0.0f, false, false);
        ((NiAVObject*)m_pGaugeObjects[2])->Update(updateData);
    }

    DAT_011db370 = afTime;
}
}  // namespace fnv_007f8320

extern SettingT_GameSettingCollection_ DAT_011db2bc;
float __cdecl BSMIN(float, float);
float __cdecl BSMAX_00404010(float, float);
class FadeGeometry : public NiGeometry
{
  public:
    void UpdateProperties();
};

// @fnv 0x007fa540
// pdb: FOPipboyManager::UpdateLightEffect | FOPipboyManager.obj | debug, object-file gap fill, medium
// pdb params: afTime
// pdb locals: ffade, pLightEffect, fdelta, proot, plightNode, peffect, pmp, psp
void FOPipboyManager::UpdateFade(float now)
{
    if (bLightEffectFading)
    {
        UpdatePipboyLightFade(now);
        SetPipBoyObjectVisibility(false, true, false);
        NiNode* aTarget = spPipboyRoot;  // Root containing the light effect.
        float time = now - fLightEffectFadeStartTime;
        float result = BSMIN(time / DAT_011db2bc.Float()->f, 1.0f);
        result = BSMAX_00404010(0.0f, result);
        if (fLightEffectFadeTarget == 0.0)
            result = 1.0f - result;
        NiAVObject* effect = ((const NiNode*)aTarget)->GetObjectByName(NiFixedString("PipboyLightEffect"));
        if (!effect)
            effect = ((const NiNode*)aTarget)->GetObjectByName(NiFixedString("PipboyLightEffect:0"));
        NiNode* current = 0;
        if (effect)
            current = effect->IsSceneNode();
        if (current && current->NiNode_GetChildCount())
        {
            FadeGeometry* geometry = static_cast<FadeGeometry*>(current->GetAt(0)->IsGeometry());
            if (geometry)
            {
                NiProperty* property = geometry->GetProperty(NiMaterialProperty_GetType());
                if (property)
                    property->NiMaterialProperty_SetAlpha(result);
                else
                {
                    BSShaderProperty* shader = static_cast<BSShaderProperty*>(geometry->GetProperty(NiShadeProperty_GetType()));
                    shader->SetAlpha(result);
                }
                geometry->UpdateProperties();
            }
        }
        if (result == fLightEffectFadeTarget)
        {
            bLightEffectFading = false;
            if (fLightEffectFadeTarget == 0.0)
                SetPipBoyObjectVisibility(false, false, false);
        }
    }
}

namespace fnv_007fa7a0 {
class TimeGlobal
{
  public:
    unsigned int GetTimeStamp();
};

extern TimeGlobal g_timeGlobal;

}  // namespace fnv_007fa7a0

// @fnv 0x007fa7a0
// pdb: FOPipboyManager::FadeLightEffect | FOPipboyManager.obj | debug, call graph, high
// pdb params: abFadeOut
void FOPipboyManager::UpdatePipboyLight(int abFadeOut)
{
    using namespace fnv_007fa7a0;
    if (b1stPersonLightOn)
    {
        fLightEffectFadeStartTime = (float)fnv_007fa7a0::g_timeGlobal.GetTimeStamp();
        bLightEffectFading = true;
        if ((unsigned char)abFadeOut)
            fLightEffectFadeTarget = 0.0f;
        else
            fLightEffectFadeTarget = 1.0f;
    }
}

