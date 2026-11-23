// fallout_shared/teswaterform.cpp: TESWaterForm, BGSPlaceableWater, WaterShaderSettings and helpers (27 functions)

#include "layout_support.h"
#include "../../include/BaseFormComponent.h"
#include "../../include/TESForm.h"
// Use the shared bases when the remaining unit-local headers include them.
#define FNV_BASEFORMCOMPONENT_H
#define FNV_TESFORM_H
#include "../../include/BGSLoadGameBuffer.h"
#include "../../include/BGSLoadFormBuffer.h"
#include "../../include/BGSPlaceableWater.h"
#include "../../include/FormChangeMask.h"
#include "../../include/TESModel.h"
#include "../../include/TESPackage.h"
#include "../../include/BGSImpactData.h"
#include "../../include/EffectSetting.h"
#include "../../include/SpellItem.h"
#include "../../include/TESAttackDamageForm.h"
#include "../../include/TESSound.h"
#include "../../include/TESWaterForm.h"
#include "teswaterform_decls.h"
#include "../../include/WaterShaderSettings.h"
#include "../../include/BGSSaveFormBuffer.h"
#include "../../include/NP.h"
#include "../../include/EffectItem.h"
#include "../../include/TESTexture.h"
#include "../../include/TESFullName.h"
#include "../../include/BSStringT.h"
#include "../../include/NiPointer.h"

// @fnv 0x0057ef10
// pdb: TESWaterForm::InitializeData | TESWaterForm.obj | retail, vtable slot, high
// pdb locals: i
void TESWaterForm::ResetFormData()
{
    cAlpha = 0x4B;
    cFlags = 0;
    pWaterSound = 0;
    cMaterialID.Set(0, 0);
    Sky_GetStarsColor()->Initialize();

    for (int i = 0; i < 3; ++i)
        pWaterWeatherControl[i] = 0;
}

// Initializes the object's fields with default constants and global values.
// @fnv 0x0057e9c0
void WaterShaderSettings::Initialize()
{
    SplatterSettings = g_splatterDefaultSecond;
    m_fSecondParameter = DAT_010249d8;
    m_fThirdParameter = 0.5f;
    m_fFourthSetting = 1.0f;
    SevenFloatParameterData = DAT_0101b268;
    SevenFloatParameters = 0.5f;
    SevenFloatData = DAT_010313d4;
    m_fEighthParameter = 0.0f;
    FiveFloatAndTwoUnsignedIntegerRecord = 0.0f;
    FourFloatAndThreeUnsignedIntegerRecord = 0.0f;
    WaterFogSettings = 1.0f;
    fUnderwaterFogDistNear = 0.0f;
    fUnderwaterFogDistFar = DAT_01013974;
    fUnderwaterFogAmount = 1.0f;
    FloatColorSettingsRecord = 0x00808000;
    ShaderColorParameterBlock = 0x00190000;
    ColorSettingsRecord = 0x00FFFFFF;
    SplatterColorSettings = g_splatterDefaultSecond;
    SplatterScalarSettings = DAT_01018180;
    SplatterFloatSettings = DAT_010231a4;
    SplatterParameterSet = DAT_010162c0;
    SplatterTimingSettings = 0.01f;
    SevenFloatRecordWithGlobals = DAT_010231a8;
    SevenFloatRecordWithStepAndGlobals = DAT_01018180;
    SevenFloatRecordWithStep = DAT_010231a4;
    SevenFloatFormatBlock = 10.0f;
    SevenFloatRecordWithUnitValue = DAT_010181bc;
    m_fWaterFogParameter1 = 0.0f;
    m_fValueBeforeWaterFogSettings = 0.0f;
    SevenFloatParameterSet = 1.0f;
    WaterFogParameterData = DAT_01013d84;
    m_fWaterParameter1 = 100.0f;
    m_fWaterParameter2 = 100.0f;
    m_fWaterParameter3 = 100.0f;
    WaterShaderProperties = DAT_0101b2d0;
    m_fShininess = 100.0f;
    WaterShaderParameters = 1.0f;
    MaterialParametersWithLeadingFloat = DAT_01022958;
    MaterialParameters = 1.0f;
}

// @fnv 0x0057f8b0
// pdb: TESWaterForm::InitItem | TESWaterForm.obj | retail, string-set bridge, high
// pdb locals: itempint, plist, pdamageRadiationEffect, prestoreHealthEffect, peffect, i
void TESWaterForm::InitItem()
{
    if (static_cast<const TESForm*>(this)->GetInitialized())
        return;
    unsigned int id = (unsigned int)pWaterSound;
    if (id)
    {
        RemapFormIDForFile(id, GetFile(-1));
        pWaterSound = dynamic_cast<TESSound*>(TESForm::GetFormByID(id));
        if (!pWaterSound)
            Warning("MASTERFILE: Could not find sound (%08X) for water type '%s' (%08X).", id, GetName(), GetFormID());
    }
    id = (unsigned int)pSpellItem;
    if (id)
    {
        RemapFormIDForFile(id, GetFile(-1));
        pSpellItem = dynamic_cast<SpellItem*>(TESForm::GetFormByID(id));
        if (!pSpellItem)
            Warning("MASTERFILE: Could not find sound (%08X) for spell item '%s' (%08X).", id, GetName(), GetFormID());
        if (pSpellItem && !pSpellItem->IsEmpty())
        {
            EffectItemList* node = pSpellItem;
            EffectItem* radiation = 0;
            EffectItem* health = 0;
            while (node && (!radiation || !health))
            {
                EffectItem* effect = node->GetItem();
                if (effect->GetAffectedActorValue() == 54 && effect->GetEffectSetting()->IsFlagSet(4))
                {
                    radiation = effect;
                    iRadiationDamage = radiation->GetMagnitude();
                }
                else if (effect->GetAffectedActorValue() == 16 && !effect->GetEffectSetting()->IsFlagSet(4))
                {
                    health = effect;
                    iHealthRestoration = health->GetMagnitude();
                }
                node = static_cast<EffectItemList*>(node->GetNext());
            }
        }
    }
    for (int i = 0; i < 3; ++i)
    {
        id = (unsigned int)pWaterWeatherControl[i];
        if (id)
        {
            RemapFormIDForFile(id, GetFile(-1));
            pWaterWeatherControl[i] = dynamic_cast<TESWaterForm*>(TESForm::GetFormByID(id));
            if (!pWaterWeatherControl[i])
                Warning("MASTERFILE: Could not find WaterForm (%08X) for water type '%s' (%08X).", id, GetName(),
                        GetFormID());
        }
    }
    SetInitializationState(true);
}

// Reads a TESWaterForm record from file: checks form type 78, loads the header, then processes chunks (EDID, OBND, FULL, DATA, DNAM, etc.) into the form's fields.
// @fnv 0x0057ef80
// pdb: TESWaterForm::Load | TESWaterForm.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, formid, pMessage, idatasize, itempint, pmessage, i
bool TESWaterForm::Load(TESFile* apFile)
{
    unsigned char ucFormType = apFile->GetTESForm();
    if (ucFormType != 78)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    unsigned int uiChunkID;
    while ((uiChunkID = apFile->GetChunkID()) != 0)
    {
        switch ((int)uiChunkID)
        {
        case 0x44494445: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 512);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:
            TESFullName::LoadFullNameChunk(this, apFile);
            break;
        case 0x41544144: {
            unsigned int uiAddedSize = apFile->GetChunkSize() - GetDataSizeAdjustment();
            if (!uiAddedSize)
            {
                reinterpret_cast<BGSImpactData*>(this)->ReadTypedData(apFile, 0, 0);
                if (apFile->IsEndianConversionRequired())
                    Data.WaterShaderData_Endian();
            }
            else
            {
                reinterpret_cast<BGSImpactData*>(this)->ReadTypedData(apFile, &Data, 196);
                if (apFile->IsEndianConversionRequired())
                    Data.WaterShaderData_Endian();
                Data.m_fWaterParameter1 = 100.0f;
                Data.m_fWaterParameter2 = 100.0f;
                Data.m_fWaterParameter3 = 100.0f;
                Data.m_fWaterParameter4 = 1.0f;
                Data.m_fWaterParameter5 = 0.5f;
                Data.m_fWaterParameter6 = 0.25f;
                SetAttackDamage(0);
            }
            break;
        }
        case 0x4d414e44:
            apFile->GetChunkData(&Data, 196);
            if (apFile->IsEndianConversionRequired())
                Data.WaterShaderData_Endian();
            break;
        case 0x4d414e4e:
            TESTexture::ReadTextureData(&NoiseTexture, apFile);
            break;
        case 0x4d414e41:
            apFile->GetChunkData(&cAlpha, 1);
            break;
        case 0x4d414e46:
            apFile->GetChunkData(&cFlags, 1);
            break;
        case 0x4d414e53: {
            unsigned int shader = 0;
            apFile->GetChunkData(&shader);
            reinterpret_cast<EffectSetting*>(this)->SetEnchantEffectShader((TESEffectShader*)shader);
            break;
        }
        case 0x4d414e58: {
            unsigned int spell = 0;
            apFile->GetChunkData(&spell);
            SetSpellItem((SpellItem*)spell);
            break;
        }
        case 0x4d414e4d: {
            char* material = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(material, 0);
            cMaterialID.Set(material, 0);
            break;
        }
        case 0x4d414e47:
            apFile->GetChunkData(pWaterWeatherControl, 12);
            if (apFile->IsEndianConversionRequired())
            {
                for (int i = 0; i < 3; ++i)
                    BSEndian((unsigned int*)&pWaterWeatherControl[i], EndianSizeTag<4>());
            }
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x0057eef0
// src: XBOX_PDB 0x0057eef0
WaterShaderSettings* TESWaterForm::Sky_GetStarsColor()
{
    return &Data;
}

// @fnv 0x0057f890
// src: XBOX_PDB 0x0057f890
void TESWaterForm::SetSpellItem_0057f890(SpellItem* apSpellItem)
{
    pSpellItem = apSpellItem;
}

// @fnv 0x0057fed0
unsigned int TESWaterForm::GetMember()
{
    return (unsigned int)pSpellItem;
}

// @fnv 0x0057fef0
// src: COMMUNITY 0x0057fef0
// pdb: TESWaterForm::SetMaterialName | TESWaterForm.obj | debug, upstream name already matches, high
// pdb params: apMaterial
void TESWaterForm::SetMaterialName(const char* apMaterial)
{
    cMaterialID.Set(apMaterial, 0);
}

// @fnv 0x00580060
// src: COMMUNITY 0x00580060
bool TESWaterForm::IsDangerous()
{
    if (this->cFlags & 1)
        return true;
    return false;
}

// @fnv 0x00580080
// src: COMMUNITY 0x00580080
// pdb: TESWaterForm::GetNoiseTextureNameLength | TESWaterForm.obj | debug, upstream name already matches, high
unsigned int TESWaterForm::GetNoiseTextureNameLength()
{
    return NoiseTexture.GetTextureNameLength();
}


// @fnv 0x005800a0
// src: COMMUNITY 0x005800a0
// pdb: TESWaterForm::GetNoiseTextureName | TESWaterForm.obj | debug, upstream name already matches, high
char const* TESWaterForm::GetNoiseTextureName() const
{
    return const_cast<TESTexture&>(NoiseTexture).GetTextureResourceName();
}

// @fnv 0x005800c0
void TESWaterForm::SetNP(void* apValue)
{
    *reinterpret_cast<NP*>(&spNoiseTexture) = apValue;
}

// @fnv 0x005806e0
// src: XBOX_PDB 0x005806e0
// pdb: TESWaterForm::SaveGame | TESWaterForm.obj | retail, vtable slot, high
// pdb params: apSaveGameBuffer
void TESWaterForm::SaveGame_005806e0(BGSSaveFormBuffer* apSaveGameBuffer)
{
    TESForm::SaveGame(apSaveGameBuffer);
    if (apSaveGameBuffer->GetChangeFlags(0x80000000).operator bool())
        apSaveGameBuffer->FUN_00865df0(pRemappedWaterForm, 0);
}

// @fnv 0x005807a0
// src: COMMUNITY 0x005807a0
// pdb: TESWaterForm::Revert | TESWaterForm.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
void TESWaterForm::Revert_005807a0(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::Revert((unsigned int)apLoadGameBuffer);
    if (static_cast<const BGSLoadFormBuffer*>(apLoadGameBuffer)->GetOldChangeFlags(0x80000000).operator bool())
        *(void**)((char*)this + 0x80) = 0;
}

// BGSPlaceableWater.h does not yet declare the helper at 0x005802f0.
struct FUN_005802f0_shell
{
    void BGSPlaceableWater_SetReflectsLODTrees(bool abValue);
};
// @fnv 0x005802f0
// src: COMMUNITY 0x005802f0
// pdb: BGSPlaceableWater::SetReflectsLODTrees | TESWaterForm.obj | debug, upstream name already matches, high
// pdb params: abValue
void FUN_005802f0_shell::BGSPlaceableWater_SetReflectsLODTrees(bool abValue)
{
    ((BGSPlaceableWater*)this)->BGSPlaceableWater_SetFlag(0x20, abValue);
}

// @fnv 0x00580730
// src: XBOX_PDB 0x00580730
// pdb: TESWaterForm::LoadGame | TESWaterForm.obj | retail, vtable slot, high
// pdb params: apLoadGameBuffer
void TESWaterForm::LoadGame_00580730(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::LoadGame(apLoadGameBuffer);
    if (apLoadGameBuffer->GetChangeFlags(0x80000000).operator bool())
        pRemappedWaterForm = dynamic_cast<TESWaterForm*>(TESForm::GetFormByID(apLoadGameBuffer->BGSLoadGameBuffer::ReadFormIdentifier()));
}

// Returns the stored string pointer if the field is nonzero, otherwise an empty string literal.
// @fnv 0x0057fda0
// src: COMMUNITY 0x0057fda0
// pdb: TESWaterForm::GetMaterialName | TESWaterForm.obj | debug, upstream name already matches, high
const char* TESWaterForm::GetMaterialName()
{
    return cMaterialID ? (const char*)cMaterialID : "";
}

extern double g_d100;
// @fnv 0x00580100
// src: COMMUNITY 0x00580100
float TESWaterForm::GetOpacity_00580100()
{
    return cAlpha / g_d100;
}

// TESForm.h does not yet declare the serialization helper at 0x00485e00.
struct FUN_00485e00_shell
{
    void TESForm_SaveData();
};
void TESForm_AddChunkArray(unsigned int aeChunkID, void* apData, unsigned int aiSize);
void TESForm_AddChunk_3(unsigned int aeChunkID, unsigned char aiData);
void TESForm_AddChunk_00485910(unsigned int id, unsigned int value);
void AppendChunk(unsigned int id, void* data, unsigned int size);
void TESForm_AddChunkArray_00485710(unsigned int aeChunkID, unsigned int* apData, unsigned int aiCount);
bool TESForm_QEndianSwapOnSave();

// Serializes the water form: writes name, model, flags, form IDs, and a 0xc4-byte Data block, swapping endianness when needed.
// @fnv 0x0057fbf0
// pdb: TESWaterForm::Save | TESWaterForm.obj | retail, vtable slot, high
// pdb locals: tmpid, itempint, i
void TESWaterForm::SaveWaterForm()
{
    unsigned int auiFormIds[3];
    TESForm::BeginFormRecord();
    TESFullName::Save_00487010();
    TESForm_AddChunkArray(0x4d414e4e, (void*)NoiseTexture.GetTextureResourceName(),
                 NoiseTexture.GetTextureNameLength() + 1);
    TESForm_AddChunk_3(0x4d414e41, cAlpha);
    TESForm_AddChunk_3(0x4d414e46, cFlags);
    TESForm_AddChunkArray(0x4d414e4d, (void*)GetMaterialName(), cMaterialID.Length() + 1);
    if (pWaterSound)
    {
        unsigned int itempint = pWaterSound->TESForm::GetFormID();
        TESForm_AddChunk_00485910(0x4d414e53, itempint);
    }
    if (pSpellItem)
    {
        unsigned int itempint = pSpellItem->GetFormID();
        TESForm_AddChunk_00485910(0x4d414e58, itempint);
    }
    ((FUN_00485e00_shell*)this)->TESForm_SaveData();
    if (TESForm_QEndianSwapOnSave())
        Data.WaterShaderData_Endian();
    AppendChunk(0x4d414e44, &Data, 0xc4);
    if (TESForm_QEndianSwapOnSave())
        Data.WaterShaderData_Endian();
    for (int i = 0; i < 3; i++)
    {
        if (pWaterWeatherControl[i])
            auiFormIds[i] = pWaterWeatherControl[i]->GetFormID();
        else
            auiFormIds[i] = 0;
    }
    TESForm_AddChunkArray_00485710(0x4d414e47, auiFormIds, 3);
    TESForm::FinishFormRecord();
}



extern int __cdecl _memcmp(const void*, const void*, unsigned int);

// @fnv 0x0057ff10
// src: XBOX_PDB 0x0057ff10
// pdb: TESWaterForm::Compare | TESWaterForm.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp
bool TESWaterForm::Compare_0057ff10(TESForm* apCompare)
{
    TESWaterForm* pcomp;

    pcomp = dynamic_cast<TESWaterForm*>(apCompare);
    if (pcomp == 0)
        return true;

    if (TESForm::Compare(pcomp))
        return true;

    if (*(unsigned char*)((char*)this + 0x71) != *(unsigned char*)((char*)pcomp + 0x71))
        return true;

    if (*(unsigned char*)((char*)this + 0x70) != *(unsigned char*)((char*)pcomp + 0x70))
        return true;

    if (_memcmp(Sky_GetStarsColor(), pcomp->Sky_GetStarsColor(), 0xc4) != 0)
        return true;

    if (NoiseTexture.CompareWithBase(&pcomp->NoiseTexture))
        return true;

    if (((TESPackage*)this)->GetRefCount() != ((TESPackage*)pcomp)->GetRefCount())
        return true;

    if (GetMaterialName() != pcomp->GetMaterialName())
        return true;

    if (GetMember() != pcomp->GetMember())
        return true;

    if (_memcmp((char*)this + 0x148, (char*)pcomp + 0x148, 0xc) != 0)
        return true;

    return false;
}

// BGSPlaceableWater.h lacks these three no-op helper declarations.
struct FUN_00580310_t
{
    void SetFlag400(bool);
};

struct FUN_005802d0_shell
{
    void BGSPlaceableWater_SetReflectsLODBuildings(bool);
};

struct FUN_00589f50_shell
{
    void SetSound(void*);
};

int _sprintf_s(char*, unsigned int, const char*, ...);

void FUN_00580310_t::SetFlag400(bool)
{}

// src: COMMUNITY 0x005802d0
void FUN_005802d0_shell::BGSPlaceableWater_SetReflectsLODBuildings(bool)
{}

void FUN_00589f50_shell::SetSound(void*)
{}

// Returns the BGSPlaceableWater stored at offset 0x180, creating and initialising it with editor ID "PlaceableAutoWater" if it is null.
// @fnv 0x00580130
// src: XBOX_PDB 0x00580130
// pdb: TESWaterForm::GetPlaceableAutoWater | TESWaterForm.obj | retail, string-set bridge, high
// pdb locals: ptempname
BGSPlaceableWater* TESWaterForm::GetPlaceableAutoWater()
{
    char ptempname[260];

    if (*reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180) == 0)
    {
        *reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180) = new BGSPlaceableWater;

        (*reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))
            ->NotifySaveLoadGameOfFormChange();

        (*reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))->BGSPlaceableWater_SetAutomaticWater(true);

        reinterpret_cast<FUN_00580310_t*>(*reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))
            ->SetFlag400(true);

        (*reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))->BGSPlaceableWater_SetReflectsLODLand(true);

        reinterpret_cast<FUN_005802d0_shell*>(
            *reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))
            ->BGSPlaceableWater_SetReflectsLODBuildings(true);

        reinterpret_cast<FUN_005802f0_shell*>(
            *reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))
            ->BGSPlaceableWater_SetReflectsLODTrees(true);

        _sprintf_s(ptempname, 0x104, "PlaceableAutoWater");

        (*reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))
            ->SetEditorID_AtRuntime(ptempname);

        reinterpret_cast<FUN_00589f50_shell*>(
            *reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180))
            ->SetSound(this);
    }

    return *reinterpret_cast<BGSPlaceableWater**>(reinterpret_cast<char*>(this) + 0x180);
}

// @fnv 0x00580330
// src: XBOX_PDB 0x00580330
// pdb: TESWaterForm::GetPlaceableLODWater | TESWaterForm.obj | retail, string-set bridge, high
// pdb locals: ptempname
BGSPlaceableWater* TESWaterForm::GetPlaceableLODWater()
{
    char ptempname[260];

    if (pPlaceableLODWater == 0)
    {
        pPlaceableLODWater = new BGSPlaceableWater;

        reinterpret_cast<TESForm*>(pPlaceableLODWater)->NotifySaveLoadGameOfFormChange();

        pPlaceableLODWater->SetFlag8000000(true);
        pPlaceableLODWater->BGSPlaceableWater_SetReflectionEnabled(true);
        pPlaceableLODWater->BGSPlaceableWater_SetRefractionEnabled(false);
        pPlaceableLODWater->SetValue(false);
        pPlaceableLODWater->BGSPlaceableWater_SetReflectsLand(false);
        pPlaceableLODWater->BGSPlaceableWater_SetReflectsLODLand(true);

        reinterpret_cast<FUN_005802d0_shell*>(pPlaceableLODWater)->BGSPlaceableWater_SetReflectsLODBuildings(true);

        reinterpret_cast<FUN_005802f0_shell*>(pPlaceableLODWater)->BGSPlaceableWater_SetReflectsLODTrees(false);

        pPlaceableLODWater->BGSPlaceableWater_SetReflectsSky(true);
        pPlaceableLODWater->SetFlag80(false);
        pPlaceableLODWater->SetFlag100(false);

        _sprintf_s(ptempname, 0x104, "PlaceableLODWater");

        reinterpret_cast<TESForm*>(pPlaceableLODWater)->SetEditorID_AtRuntime(ptempname);

        reinterpret_cast<FUN_00589f50_shell*>(pPlaceableLODWater)->SetSound(this);
    }

    return pPlaceableLODWater;
}

// Release the noise texture before destroying the other components.
// @fnv 0x0057ee10
// pdb: TESWaterForm::~TESWaterForm | TESWaterForm.obj | retail, upstream name already matches, high
TESWaterForm::~TESWaterForm()
{
    reinterpret_cast<NP*>(&spNoiseTexture)->operator=((void*)0);
}

// @fnv 0x0057eba0
// pdb: TESWaterForm::TESWaterForm | TESWaterForm.obj | retail, upstream name already matches, high
TESWaterForm::TESWaterForm()
    : spNoiseNormalMap(0), fTexScroll0(0.0f, 0.0f, 0.0f, 0.0f),
      fTexScroll1(0.0f, 0.0f, 0.0f, 0.0f), fTexScroll2(0.0f, 0.0f, 0.0f, 0.0f),
      spNoiseTexture(0)
{
    SetFormType((ENUM_FORM_ID)0x4e);
    ResetFormData();
    iCurrentTextureSelect01 = 0;
    iCurrentTextureSelect02 = 0;
    fFrequencyX = 9;
    fFrequencyY = 9;
    iOctaves = 4;
    fAmplitude = 0.5f;
    fLacunarity = 2.0f;
    fBias = 0.5f;
    fGain = 0.5f;
    pSpellItem = 0;
    bNeedUpdate = true;
    bResetNoiseTexture = false;
    pPlaceableAutoWater = 0;
    pPlaceableLODWater = 0;
    iRadiationDamage = 0;
    iHealthRestoration = 0;
    pRemappedWaterForm = 0;
}

struct ByteArg
{
    unsigned char c;
};
void BSEndian_00401080(void* p, ByteArg v);
// @fnv 0x0057f3a0
// src: XBOX_RETAIL_PDB 0x0057f3a0
// pdb: WaterShaderData::Endian | TESWaterForm.obj | retail, call graph (shared callers), high
void WaterShaderSettings::WaterShaderData_Endian()
{
    BSEndian_00401080((char*)this + 0x0, ByteArg());
    BSEndian_00401080((char*)this + 0x4, ByteArg());
    BSEndian_00401080((char*)this + 0x8, ByteArg());
    BSEndian_00401080((char*)this + 0xc, ByteArg());
    BSEndian_00401080((char*)this + 0x10, ByteArg());
    BSEndian_00401080((char*)this + 0x14, ByteArg());
    BSEndian_00401080((char*)this + 0x18, ByteArg());
    BSEndian_00401080((char*)this + 0x1c, ByteArg());
    BSEndian_00401080((char*)this + 0x20, ByteArg());
    BSEndian_00401080((char*)this + 0x24, ByteArg());
    BSEndian_00401080((char*)this + 0x28, ByteArg());
    BSEndian_00401080((char*)this + 0x2c, ByteArg());
    BSEndian_00401080((char*)this + 0x30, ByteArg());
    BSEndian_00401080((char*)this + 0x38, ByteArg());
    BSEndian_00401080((char*)this + 0x3c, ByteArg());
    BSEndian_00401080((char*)this + 0x40, ByteArg());
    BSEndian_00401080((char*)this + 0x44, ByteArg());
    BSEndian_00401080((char*)this + 0x48, ByteArg());
    BSEndian_00401080((char*)this + 0x4c, ByteArg());
    BSEndian_00401080((char*)this + 0x50, ByteArg());
    BSEndian_00401080((char*)this + 0x54, ByteArg());
    BSEndian_00401080((char*)this + 0x58, ByteArg());
    BSEndian_00401080((char*)this + 0x5c, ByteArg());
    BSEndian_00401080((char*)this + 0x60, ByteArg());
    BSEndian_00401080((char*)this + 0x64, ByteArg());
    BSEndian_00401080((char*)this + 0x68, ByteArg());
    BSEndian_00401080((char*)this + 0x6c, ByteArg());
    BSEndian_00401080((char*)this + 0x70, ByteArg());
    BSEndian_00401080((char*)this + 0x74, ByteArg());
    BSEndian_00401080((char*)this + 0x78, ByteArg());
    BSEndian_00401080((char*)this + 0x7c, ByteArg());
    BSEndian_00401080((char*)this + 0x80, ByteArg());
    BSEndian_00401080((char*)this + 0x84, ByteArg());
    BSEndian_00401080((char*)this + 0x88, ByteArg());
    BSEndian_00401080((char*)this + 0x8c, ByteArg());
    BSEndian_00401080((char*)this + 0x90, ByteArg());
    BSEndian_00401080((char*)this + 0x94, ByteArg());
    BSEndian_00401080((char*)this + 0x98, ByteArg());
    BSEndian_00401080((char*)this + 0x9c, ByteArg());
    BSEndian_00401080((char*)this + 0xa0, ByteArg());
    BSEndian_00401080((char*)this + 0xa4, ByteArg());
    BSEndian_00401080((char*)this + 0xa8, ByteArg());
    BSEndian_00401080((char*)this + 0xac, ByteArg());
    BSEndian_00401080((char*)this + 0xb0, ByteArg());
    BSEndian_00401080((char*)this + 0xb4, ByteArg());
    BSEndian_00401080((char*)this + 0xb8, ByteArg());
    BSEndian_00401080((char*)this + 0xbc, ByteArg());
    BSEndian_00401080((char*)this + 0xc0, ByteArg());
}

// @fnv 0x005800e0
// src: COMMUNITY 0x005800e0
// pdb: TESWaterForm::GetNoiseTexture | TESWaterForm.obj | debug, upstream name already matches, high
// pdb params: aspTexture
void TESWaterForm::GetNoiseTexture(NiPointer<NiTexture>& aspTexture)
{
    aspTexture = spNoiseTexture;
}

#include <string.h>

// Copies water effect settings from apCopy into this, including flags, opacity, shader, spell item, and noise map texture.
// @fnv 0x0057fde0
// src: XBOX_PDB 0x0057fde0
// pdb: TESWaterForm::Copy | TESWaterForm.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pcopy
void TESWaterForm::Copy_0057fde0(TESForm* apCopy)
{
    TESWaterForm* pcopy = dynamic_cast<TESWaterForm*>(apCopy);
    if (!pcopy)
        return;
    TESForm::CopyFrom(pcopy);
    this->cFlags = pcopy->cFlags;
    this->cAlpha = pcopy->cAlpha;
    memcpy(this->Sky_GetStarsColor(), pcopy->Sky_GetStarsColor(), 0xc4);
    reinterpret_cast<EffectSetting*>(this)->SetEnchantEffectShader(
        reinterpret_cast<TESEffectShader*>(reinterpret_cast<TESPackage*>(pcopy)->GetRefCount()));
    this->SetSpellItem(reinterpret_cast<SpellItem*>(pcopy->GetMember()));
    memcpy(reinterpret_cast<unsigned char*>(this) + 0x148, reinterpret_cast<unsigned char*>(pcopy) + 0x148, 0xc);
    this->NoiseTexture.CopyFromBase(
        reinterpret_cast<BaseFormComponent*>(reinterpret_cast<unsigned char*>(pcopy) + 0x64));
    SetMaterialName(pcopy->GetMaterialName());
}
