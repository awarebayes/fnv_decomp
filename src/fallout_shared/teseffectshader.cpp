// fallout_shared/teseffectshader.cpp: TESEffectShader, EffectShaderData and shader helpers (26 functions)

// Keep the generated support templates private; use checked scene and model types.
#define FNV_TESFILE_H  // the shared TESFile below replaces layout_support.h's stub
#include "../../include/NiFile.h"
#include "layout_support.h"
#define FNV_BASEFORMCOMPONENT_H
#include "../../include/NiPoint3.h"
#include "../../include/TESForm.h"
#include "../../include/BGSDebris.h"
#include "../../include/NiAVObject.h"
#include "../../include/NiNode.h"
#include "../../include/NiProperty.h"
#include "../../include/NiBound.h"
#include "../../include/TESModel.h"
#include "../../include/EffectShaderData.h"
#include "../../include/TESEffectShader.h"
#include "../teseffectshader.h"
#include "TESFile.h"
#include "../../include/TESFile.h"

// @fnv 0x00506510
// pdb: EffectShaderData::Initialize | TESEffectShader.obj | retail, upstream name already matches, high
void EffectShaderData::Initialize()
{
    cFlags = 0;
    eTextureBlendModeSource = static_cast<_D3DBLEND>(5);
    eTextureBlendOperation = static_cast<_D3DBLENDOP>(1);
    eTextureZTestFunction = static_cast<_NDLZCMPFUNC>(3);
    iFillColor = 0;
    fFillAlphaFadeInTime = 0.0f;
    fFillAlphaFullTime = 0.0f;
    fFillAlphaFadeOutTime = 0.0f;
    fFillAlphaPersistentPercent = 0.0f;
    fFillAlphaPulseAmplitude = 0.0f;
    fFillAlphaPulseFrequency = 1.0f;
    fFillTextureUAnimSpeed = 0.0f;
    fFillTextureVAnimSpeed = 0.0f;
    fEdgeExponentValue = 1.0f;
    iEdgeColor = 0;
    fEdgeAlphaFadeInTime = 0.0f;
    fEdgeAlphaFullTime = 0.0f;
    fEdgeAlphaFadeOutTime = 0.0f;
    fEdgeAlphaPersistentPercent = 0.0f;
    fEdgeAlphaPulseAmplitude = 0.0f;
    fEdgeAlphaPulseFrequency = 1.0f;
    eTextureBlendModeDest = static_cast<_D3DBLEND>(6);
    fFillAlphaFullPercent = 1.0f;
    fEdgeAlphaFullPercent = 1.0f;
    eParticleBlendModeSource = static_cast<_D3DBLEND>(5);
    eParticleBlendModeDest = static_cast<_D3DBLEND>(6);
    eParticleBlendOperation = static_cast<_D3DBLENDOP>(1);
    eParticleZTestFunction = static_cast<_NDLZCMPFUNC>(4);
    fParticleFadeInTime = 0.0f;
    fParticleFullTime = 0.0f;
    fParticleFadeOutTime = 0.0f;
    fParticleFull = 1.0f;
    fParticlePersistent = 1.0f;
    fParticleLifetime = 1.0f;
    fParticleLifeVar = 0.0f;
    fParticleNormalSpeed = 0.0f;
    fParticleNormalAcc = 0.0f;
    ParticleVelocity = NiPoint3(0.0f, 0.0f, 0.0f);
    ParticleAcceleration = NiPoint3(0.0f, 0.0f, 0.0f);
    fParticleScale1 = 1.0f;
    fParticleScale2 = 1.0f;
    fParticleScale1Time = 0.0f;
    fParticleScale2Time = 1.0f;
    iParticleColor1 = 16777215;
    iParticleColor2 = 16777215;
    iParticleColor3 = 16777215;
    fParticleColor1Alpha = 1.0f;
    fParticleColor2Alpha = 1.0f;
    fParticleColor3Alpha = 1.0f;
    fParticleColor1Time = 0.0f;
    fParticleColor2Time = 0.5f;
    fParticleColor3Time = 1.0f;
    fParticleNormalSpeedVar = 0.0f;
    fParticleRotate = 0.0f;
    fParticleRotateVar = 0.0f;
    fParticleRotateSpeed = 0.0f;
    fParticleRotateSpeedVar = 0.0f;
    fAlphaTestStartTime = 0.0f;
    fAlphaTestEndTime = 10.0f;
    fAlphaTestStartValue = 255.0f;
    fAlphaTestEndValue = 0.0f;
    fAlphaEdgeWidth = 0.0f;
    iAlphaEdgeColor = 16777215;
    fExplosionSpeed = 0.0f;
    iParticleTextureCountX = 1;
    iParticleTextureCountY = 1;
    fAddOnFadeInTime = 1.0f;
    fAddOnFadeOutTime = 1.0f;
    fAddOnScaleStart = 1.0f;
    fAddOnScaleEnd = 1.0f;
    fAddOnScaleInTime = 1.0f;
    fAddOnScaleOutTime = 1.0f;
}

// @fnv 0x00505db0
// pdb: TESEffectShader::InitializeData | TESEffectShader.obj | debug, object-file gap fill, medium
void TESEffectShader::ResetFormData()
{
    GetData()->Initialize();
}

// @fnv 0x00505dd0
// pdb: TESEffectShader::InitItem | TESEffectShader.obj | retail, upstream name already matches, high
// pdb locals: itempInt
void TESEffectShader::InitItem()
{
    if (IsFormInitialized())
        return;
    unsigned int formID = reinterpret_cast<unsigned int>(GetAddonModels());
    if (formID)
    {
        RemapFormIDForFile(formID, GetFile(-1));
        SetAddonModels(dynamic_cast<BGSDebris*>(GetFormByID(formID)));
    }
    SetInitializationState(true);
}

extern "C" void* __cdecl _alloca(size_t size);

// @fnv 0x00505e70
// pdb: TESEffectShader::Load | TESEffectShader.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: echunkid, formid, pMessage
bool TESEffectShader::Load(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x4f)
        return false;
    ClearData();
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    int iChunkID;
    while (iChunkID = apFile->GetChunkID())
    {
        switch (iChunkID)
        {
        case 0x44494445: {
            char* pChunkBuffer = static_cast<char*>(_alloca(apFile->GetChunkSize()));
            apFile->GetChunkData(pChunkBuffer, 0x200);
            SetEditorID_AtRuntime(pChunkBuffer);
        }
        break;
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4e4f4349:
            TESTextureLoader::ReadTextureData(&TextureShaderTexture, apFile);
            break;
        case 0x324f4349:
            TESTextureLoader::ReadTextureData(&ParticleShaderTexture, apFile);
            break;
        case 0x374d414e:
            TESTextureLoader::ReadTextureData(&BlockOutTexture, apFile);
            break;
        case 0x41544144:
            if (apFile->IsEndianConversionRequired())
                Data.Endian();
            ReadTypedData(apFile, &Data, sizeof(Data));
            if (apFile->IsEndianConversionRequired())
                Data.Endian();
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    if (Data.fParticleFull <= 1.0 && Data.fParticleFull > 0.0)
        Data.fParticleFull *= ParticleShader::GetShaderParticleCount();
    if (Data.fParticlePersistent <= 1.0 && Data.fParticlePersistent > 0.0)
        Data.fParticlePersistent *= ParticleShader::GetShaderParticleCount();
    return true;
}

// @fnv 0x00507040
// src: XBOX_PDB 0x00507040
// pdb: TESEffectShader::IsFlagSet | TESEffectShader.obj | debug, call graph (shared callers), high
// pdb params: aeFlag
bool TESEffectShader::IsFlagSet_00507040(unsigned int aeFlag)
{
    return *(unsigned char*)GetData() & aeFlag;
}

struct FUN_005050b0 : TESForm
{
    ~FUN_005050b0();
};
// @fnv 0x005050b0
// pdb: TESCombatStyle::~TESCombatStyle | TESCombatStyle.obj | debug, object-file gap fill, medium
FUN_005050b0::~FUN_005050b0()
{}

// @fnv 0x00506370
TESTexture* TESEffectShader::GetFieldAt14C()
{
    return reinterpret_cast<TESTexture*>(&TextureShaderTexture);
}

// @fnv 0x00506390
TESTexture* TESEffectShader::GetFieldAt158()
{
    return reinterpret_cast<TESTexture*>(&ParticleShaderTexture);
}

// @fnv 0x005063b0
TESTexture* TESEffectShader::GetFieldAt164()
{
    return reinterpret_cast<TESTexture*>(&BlockOutTexture);
}

extern "C" void __cdecl ParticleShader_Stop(NiAVObject*, ParticleShaderProperty*);

// @fnv 0x00507810
// src: COMMUNITY 0x00507810
// pdb: TESEffectShader::StopParticleShader | TESEffectShader.obj | debug, upstream name already matches, high
// pdb params: apTargetObject, apProp
void TESEffectShader::StopParticleShader(NiAVObject* apTargetObject, ParticleShaderProperty* apProp)
{
    ParticleShader_Stop(apTargetObject, apProp);
}

// @fnv 0x00507b00
float __fastcall GetEffectShaderScale(TESEffectShader* shader)
{
    return shader->Data.fEdgeExponentValue;
}

// @fnv 0x00507b20
float __fastcall GetEffectShaderCount(TESEffectShader* shader)
{
    return shader->Data.fAlphaTestStartValue;
}

struct Color;
// pdb: class ParticleShaderProperty, size 0x14c (here 0x1), bases: BSShaderProperty, 41 members
class ParticleShaderProperty
{
  public:
    void SetParticleTime(float);
    void ParticleShaderProperty_SetTexture0x118(void*);
    void SetWaterLODHeight(float);
    void ParticleShaderProperty_SetLifetimeVar(float);
    void SkyShaderProperty_SetBlendValue(float);
    void bhkCharacterControllerCinfo_SetAcrobatics(float);
    void FUN_006e29d0(float);
    void SetData(NiPoint3);
    void ParticleShaderProperty_SetAcceleration(NiPoint3);
    void LowProcess_SetDeathTime(float);
    void bhkCharacterControllerCinfo_SetScale(float);
    void PathingRequestClosePoint_SetMinDistance(float);
    void SetMaxDistance_006e5ee0(float);
    void FUN_00507780(float);
    void SetBoltSmallWidth(float);
    void BoltShaderProperty_SetSegmentVariance(float);
    void SetPoint(Color);
    void ParticleShaderProperty_SetColor2(Color);
    void ParticleShaderProperty_SetColor3(Color);
    void PathingRequestCoveredMove_SetAttackerHeight(float);
    void PathingRequestCoveredMove_SetAttackerSneakHeight(float);
    void PathingRequestCoveredMove_SetAttackerHealth(float);
    void SetFieldAtOffsetF4_005076e0(float);
    void NiCamera_SetMinNearPlaneDist(float);
    void NiCamera_SetMaxFarNearRatio(float);
    void SetFieldAtOffset100_00507720(float);
    void PathingRequestCoveredMove_SetCombatCoverSearchInfo(int);
    void FUN_00505e50(int);
    void ParticleShaderProperty_SetBlendOp(int);
    void ParticleShaderProperty_SetZTestFunc(int);
};
ParticleShaderProperty* __cdecl ParticleShader_Start(NiAVObject*, NiAVObject*);
// @fnv 0x005077c0
// pdb: TESEffectShader::StartParticleShader | TESEffectShader.obj | debug, object-file run extension, low
// pdb params: apRootObject, apTargetObject, apTex, afTime
// pdb locals: pProp
ParticleShaderProperty* TESEffectShader::CreateParticleShaderProperty(NiAVObject* apRootObject, NiAVObject* apTargetObject,
                                                        NiSourceTexture* apTex, float afTime)
{
    ParticleShaderProperty* pProp;
    if (apTex)
    {
        pProp = ParticleShader_Start(apRootObject, apTargetObject);
        CopySettingsTo(pProp, apTex);
        pProp->SetParticleTime(afTime);
        return pProp;
    }
    return 0;
}

// @fnv 0x00505d20
// pdb: TESEffectShader::~TESEffectShader | TESEffectShader.obj | debug, upstream name already matches, high
TESEffectShader::~TESEffectShader()
{}

void __cdecl _memcpy(void* dst, const void* src, unsigned int n);
// Copies a TESEffectShader's data, base component and three textures into this object after a dynamic_cast check.
// @fnv 0x005062a0
// pdb: TESEffectShader::Copy | TESEffectShader.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: pcopy
void TESEffectShader::CopyFrom(const TESForm* apCopy)
{
    TESEffectShader* pcopy = dynamic_cast<TESEffectShader*>(const_cast<TESForm*>(apCopy));
    if (!pcopy)
        return;
    TESForm::CopyFrom(pcopy);
    _memcpy(GetData(), pcopy->GetData(), 0x134);
    TextureShaderTexture.CopyFromBase((BaseFormComponent*)pcopy->GetFieldAt14C());
    ParticleShaderTexture.CopyFromBase((BaseFormComponent*)pcopy->GetFieldAt158());
    BlockOutTexture.CopyFromBase((BaseFormComponent*)pcopy->GetFieldAt164());
}

int __cdecl _memcmp(const void*, const void*, unsigned int);
// Compares this effect shader with apCompare: returns true if apCompare is not a TESEffectShader or any component or the shader data differs.
// @fnv 0x005063d0
// pdb: TESEffectShader::Compare | TESEffectShader.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pcomp
bool TESEffectShader::Compare(TESForm* apCompare)
{
    TESEffectShader* pOtherShader = dynamic_cast<TESEffectShader*>(apCompare);
    if (!pOtherShader)
        return true;
    if (TESForm::Compare(pOtherShader))
        return true;
    if (TextureShaderTexture.CompareWithBase((BaseFormComponent*)pOtherShader->GetFieldAt14C()))
        return true;
    if (ParticleShaderTexture.CompareWithBase(
            (BaseFormComponent*)pOtherShader->GetFieldAt158()))
        return true;
    if (BlockOutTexture.CompareWithBase((BaseFormComponent*)pOtherShader->GetFieldAt164()))
        return true;
    if (_memcmp(&Data, pOtherShader->GetData(), sizeof(EffectShaderData)))
        return true;
    return false;
}

struct FUN_0048e370_obj
{
    void TESTexture_Save(unsigned int);
};

void TESForm_AddChunkArray(unsigned int, void*, unsigned int);

bool _FUN_00401500();

// @fnv 0x005060f0
// pdb: TESEffectShader::Save | TESEffectShader.obj | retail, vtable slot, high
// pdb locals: psaveAddon
void TESEffectShader::SaveForm()
{
    TESForm::BeginFormRecord();

    if (reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x14c)->GetModelLength() == 0)
    {
        TESForm_AddChunkArray(0x4e4f4349, reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x14c)->GetFullName(),
                     reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x14c)->GetModelLength() + 1);
    }
    else
    {
        reinterpret_cast<FUN_0048e370_obj*>(reinterpret_cast<char*>(this) + 0x14c)->TESTexture_Save(0x4e4f4349);
    }

    if (reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x158)->GetModelLength() == 0)
    {
        TESForm_AddChunkArray(0x324f4349, reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x158)->GetFullName(),
                     reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x158)->GetModelLength() + 1);
    }
    else
    {
        reinterpret_cast<FUN_0048e370_obj*>(reinterpret_cast<char*>(this) + 0x158)->TESTexture_Save(0x324f4349);
    }

    if (reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x164)->GetModelLength() == 0)
    {
        TESForm_AddChunkArray(0x374d414e, reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x164)->GetFullName(),
                     reinterpret_cast<TESModel*>(reinterpret_cast<char*>(this) + 0x164)->GetModelLength() + 1);
    }
    else
    {
        reinterpret_cast<FUN_0048e370_obj*>(reinterpret_cast<char*>(this) + 0x164)->TESTexture_Save(0x374d414e);
    }

    BGSDebris* psaveAddon = Data.pAddonModels;

    Data.pAddonModels =
        Data.pAddonModels
            ? reinterpret_cast<BGSDebris*>(Data.pAddonModels->GetFormID())
            : 0;

    if (_FUN_00401500())
    {
        Data.Endian();
    }

    TESForm::TESForm___SaveData(&Data, sizeof(Data));

    if (_FUN_00401500())
    {
        Data.Endian();
    }

    Data.pAddonModels = psaveAddon;

    TESForm::FinishFormRecord();
}

// @fnv 0x005064d0
// pdb: EffectShaderData::EffectShaderData | TESEffectShader.obj | debug, upstream name already matches, high
EffectShaderData::EffectShaderData()
{
    Initialize();
}

struct FUN_005074e0_vec4
{
    float x;
    float y;
    float z;
    float w;
};

class FUN_005074e0_cls
{
  public:
    FUN_005074e0_vec4 m_v;
    FUN_005074e0_vec4* DivideByScalar(FUN_005074e0_vec4* out, float s);
};

// Copies m_v into a local vector, divides each of its four components by s, stores the result in out and returns out.
// @fnv 0x005074e0
FUN_005074e0_vec4* FUN_005074e0_cls::DivideByScalar(FUN_005074e0_vec4* out, float s)
{
    FUN_005074e0_vec4 vDivided = m_v;
    vDivided.x = vDivided.x / s;
    vDivided.y = vDivided.y / s;
    vDivided.z = vDivided.z / s;
    vDivided.w = vDivided.w / s;
    *out = vDivided;
    return out;
}

struct NiPropStub
{
    NiFixedString& GetName();
};

struct ShaderPropStub
{
    void ApplyTexEffect(void* apData);
};

int BSstrcmp(const char*, const char*);

// @fnv 0x00507b40
// src: XBOX_RETAIL_PDB 0x00507b40
// pdb: TESEffectShader::StartTextureShader | TESEffectShader.obj | retail, string-set bridge, high
// pdb params: apObject, apTexEffectData
// pdb locals: pbsprop, pprop, bapply, pmat, pnode, i
void TESEffectShader::StartTextureShader(NiAVObject* apObject, BSShaderPPLightingProperty::TextureEffectData* apTexEffectData)
{
    if (apObject)
    {
        if (apObject->IsGeometry())
        {
            NiProperty* pProp = apObject->GetProperty(3);
            ShaderPropStub* pShader;
            if ((pShader = (!pProp ? 0 : (pProp->FUN_00441110() >= 8 && pProp->FUN_00441110() <= 12))
                               ? (ShaderPropStub*)pProp
                               : 0) != 0)
            {
                bool bApply = true;
                if (IsFlagSet_00507040(0x20))
                {
                    NiPropStub* pNameProp = reinterpret_cast<NiPropStub*>(apObject->GetProperty(2));
                    if (!(const char*)pNameProp->GetName() || BSstrcmp(pNameProp->GetName(), "skin"))
                    {
                        bApply = false;
                    }
                }
                if (bApply)
                {
                    pShader->ApplyTexEffect(apTexEffectData);
                }
            }
        }
        else
        {
            NiNode* pNode = apObject->IsSceneNode();
            if (pNode)
            {
                for (unsigned int i = 0; i < pNode->FUN_0043b480(); i++)
                {
                    StartTextureShader(pNode->FUN_0043b4a0(i), apTexEffectData);
                }
            }
        }
    }
}

struct S_00507c90_Ptr;
struct S_00507c90_Item
{
    int FUN_00441110();
    S_00507c90_Ptr* FOPipboyManager_Get3rdPersonPipboyRoot();
    void AssignRefObj(S_00507c90_Ptr* apData);
};
struct S_00507c90_Cls
{
    void ClearItemRef(NiAVObject* obj, S_00507c90_Ptr* ref);
};
// Recursively walks obj's list, and for items whose value is 8..12 clears the reference if it equals ref.
// @fnv 0x00507c90
// pdb: TESEffectShader::StopTextureShader | TESEffectShader.obj | retail, object-file run extension, low
// pdb params: apObject, apTexEffectData
// pdb locals: pbsprop, pprop, pnode, i
void S_00507c90_Cls::ClearItemRef(NiAVObject* obj, S_00507c90_Ptr* ref)
{
    if (obj)
    {
        if (obj->IsGeometry())
        {
            S_00507c90_Item* t_ = reinterpret_cast<S_00507c90_Item*>(obj->GetProperty(3));
            S_00507c90_Item* s;
            if ((s = (!t_ ? 0 : (t_->FUN_00441110() >= 8 && t_->FUN_00441110() <= 12)) ? t_ : 0) != 0)
            {
                if (s->FOPipboyManager_Get3rdPersonPipboyRoot() == ref)
                    s->AssignRefObj(0);
            }
        }
        else
        {
            NiNode* list = obj->IsSceneNode();
            if (list)
            {
                for (unsigned int i2 = 0; i2 < (unsigned int)list->FUN_0043b480(); i2++)
                    ClearItemRef(list->FUN_0043b4a0(i2), ref);
            }
        }
    }
}

struct Color
{
    float r, g, b, a;
    Color(float, float, float, float);
    Color operator/(float);
};

// Copies the effect shader's Data fields into dest via its setters, converting three packed RGB values into normalized Colors with separate alpha values.
// @fnv 0x00507060
// pdb: TESEffectShader::SetupParticleShader | TESEffectShader.obj | debug, call graph, low
// pdb params: apProperty, apTex
// pdb locals: color3, color1, color2
void TESEffectShader::CopySettingsTo(ParticleShaderProperty* apDestination, NiSourceTexture* apTexture)
{
    apDestination->ParticleShaderProperty_SetTexture0x118(apTexture);
    apDestination->SetWaterLODHeight(Data.fParticleLifetime);
    apDestination->ParticleShaderProperty_SetLifetimeVar(Data.fParticleLifeVar);
    apDestination->SkyShaderProperty_SetBlendValue(Data.fParticleNormalSpeed);
    apDestination->bhkCharacterControllerCinfo_SetAcrobatics(Data.fParticleNormalAcc);
    apDestination->FUN_006e29d0(Data.fParticleNormalSpeedVar);
    apDestination->SetData(Data.ParticleVelocity);
    apDestination->ParticleShaderProperty_SetAcceleration(Data.ParticleAcceleration);
    apDestination->LowProcess_SetDeathTime(Data.fParticleRotate);
    apDestination->bhkCharacterControllerCinfo_SetScale(Data.fParticleRotateVar);
    apDestination->PathingRequestClosePoint_SetMinDistance(Data.fParticleRotateSpeed);
    apDestination->SetMaxDistance_006e5ee0(Data.fParticleRotateSpeedVar);
    apDestination->FUN_00507780(Data.iParticleTextureCountX);
    apDestination->SetBoltSmallWidth(Data.iParticleTextureCountY);
    apDestination->BoltShaderProperty_SetSegmentVariance(Data.fExplosionSpeed);
    Color color1 = Color((unsigned char)(Data.iParticleColor1 & 255),
                         (unsigned char)(((unsigned short)Data.iParticleColor1 >> 8) & 255),
                         (unsigned char)((Data.iParticleColor1 >> 16) & 255), 0.0f) /
                   255.0f;
    Color color2 = Color((unsigned char)(Data.iParticleColor2 & 255),
                         (unsigned char)(((unsigned short)Data.iParticleColor2 >> 8) & 255),
                         (unsigned char)((Data.iParticleColor2 >> 16) & 255), 0.0f) /
                   255.0f;
    Color color3 = Color((unsigned char)(Data.iParticleColor3 & 255),
                         (unsigned char)(((unsigned short)Data.iParticleColor3 >> 8) & 255),
                         (unsigned char)((Data.iParticleColor3 >> 16) & 255), 0.0f) /
                   255.0f;
    color1.a = Data.fParticleColor1Alpha;
    color2.a = Data.fParticleColor2Alpha;
    color3.a = Data.fParticleColor3Alpha;
    apDestination->SetPoint(color1);
    apDestination->ParticleShaderProperty_SetColor2(color2);
    apDestination->ParticleShaderProperty_SetColor3(color3);
    apDestination->PathingRequestCoveredMove_SetAttackerHeight(Data.fParticleColor1Time);
    apDestination->PathingRequestCoveredMove_SetAttackerSneakHeight(Data.fParticleColor2Time);
    apDestination->PathingRequestCoveredMove_SetAttackerHealth(Data.fParticleColor3Time);
    apDestination->SetFieldAtOffsetF4_005076e0(Data.fParticleScale1);
    apDestination->NiCamera_SetMinNearPlaneDist(Data.fParticleScale2);
    apDestination->NiCamera_SetMaxFarNearRatio(Data.fParticleScale1Time);
    apDestination->SetFieldAtOffset100_00507720(Data.fParticleScale2Time);
    apDestination->PathingRequestCoveredMove_SetCombatCoverSearchInfo(Data.eParticleBlendModeSource);
    apDestination->FUN_00505e50(Data.eParticleBlendModeDest);
    apDestination->ParticleShaderProperty_SetBlendOp(Data.eParticleBlendOperation);
    apDestination->ParticleShaderProperty_SetZTestFunc(Data.eParticleZTestFunction);
}

struct ColorValue
{
    float x, y, z, w;
    ColorValue(float, float, float, float);
    ColorValue Normalize(float);
};
struct RefValue
{
    void* value;
    RefValue& operator=(void*);
};
struct EffectData
{
    int unk00, unk04;
    RefValue root;
    int unk0c, unk10, unk14, unk18, unk1c, unk20, unk24, unk28;
    ColorValue fill, edge;
    float unk4c, unk50, scale, diameter;
    unsigned int unk5c, unk60, unk64, unk68;
    RefValue target;
    unsigned int unk70;
    static void* operator new(size_t);
    static void operator delete(void*, size_t);
    EffectData();
};
struct ShaderSource
{
    unsigned int Fill();
    unsigned int Edge();
    unsigned int Mode();
    unsigned int Value60();
    unsigned int Value64();
    unsigned int Value68();
};
// @fnv 0x00507830
// pdb: TESEffectShader::CreateTextureShaderData | TESEffectShader.obj | debug, object-file run extension, low
// pdb params: apObject, apTex, apBlockOutTex
// pdb locals: uedgeColor, ufillColor, pData
BSShaderPPLightingProperty::TextureEffectData* TESEffectShader::CreateEffectData(
    NiAVObject* scene, NiSourceTexture* root, NiSourceTexture* target)
{
    EffectData* data2 = new EffectData;
    unsigned int fill = reinterpret_cast<ShaderSource*>(this)->Fill();
    unsigned int edge = reinterpret_cast<ShaderSource*>(this)->Edge();
    data2->root = 0;
    data2->fill =
        ColorValue((float)(unsigned char)(fill & 255), (float)(unsigned char)((unsigned short)fill >> 8 & 255),
                   (float)(unsigned char)(fill >> 16 & 255), 0.0f)
            .Normalize(255.0f);
    data2->edge =
        ColorValue((float)(unsigned char)(edge & 255), (float)(unsigned char)((unsigned short)edge >> 8 & 255),
                   (float)(unsigned char)(edge >> 16 & 255), 0.0f)
            .Normalize(255.0f);
    data2->unk4c = 0.0f;
    data2->unk50 = 0.0f;
    data2->scale = GetEffectShaderScale(this);
    data2->diameter = scene->GetBound().FUN_0084d030() * 2.0f;
    data2->unk5c = reinterpret_cast<ShaderSource*>(this)->Mode();
    data2->unk60 = reinterpret_cast<ShaderSource*>(this)->Value60();
    data2->unk64 = reinterpret_cast<ShaderSource*>(this)->Value64();
    data2->unk68 = reinterpret_cast<ShaderSource*>(this)->Value68();
    data2->root = root;
    data2->target = target;
    data2->unk70 = (unsigned int)GetEffectShaderCount(this);
    if (IsFlagSet_00507040(16))
    {
        data2->edge.x += -1.0;
        data2->edge.y += -1.0;
        data2->edge.z += -1.0;
    }
    return reinterpret_cast<BSShaderPPLightingProperty::TextureEffectData*>(data2);
}

struct Color4
{
    float x, y, z, w;
};
struct ShaderState
{
    int unk00, unk04, unk08;
    Color4 fill, edge, baseFill, baseEdge;
    float u, v;
    int unk54, unk58, unk5c, unk60, unk64, unk68, unk6c;
    unsigned int alpha;
};
float __cdecl BSMAX_00404010(float, float);
float __cdecl BSMIN(float, float);
float __cdecl LinearInterpolate(float, float, float, float, float);
struct ShaderUpdate
{
    float UpdateFill(float, float, float, unsigned char);
    float FillPulse(float);
    float UpdateEdge(float, float, float, unsigned char);
    float EdgePulse(float);
    float USpeed();
    float VSpeed();
    float EndTime();
    float StartTime();
    float EndAlpha();
    float StartAlpha();
    void UpdateShaderState(ShaderState*, float, float, float, unsigned char);
};
// Advances a ShaderState's fill/edge alpha, UV offsets and alpha from delta, duration, time and mode.
// @fnv 0x00507e30
void ShaderUpdate::UpdateShaderState(ShaderState* state, float delta, float duration, float time, unsigned char mode)
{
    if (state)
    {
        state->baseFill.w = UpdateFill(state->baseFill.w, delta, duration, mode);
        state->fill = state->baseFill;
        state->fill.w = FillPulse(time) * state->fill.w + state->fill.w;
        state->fill.w = BSMIN(1.0f, BSMAX_00404010(0.0f, state->fill.w));
        state->baseEdge.w = UpdateEdge(state->baseEdge.w, delta, duration, mode);
        state->edge = state->baseEdge;
        state->edge.w = EdgePulse(time) * state->edge.w + state->edge.w;
        state->edge.w = BSMIN(1.0f, BSMAX_00404010(0.0f, state->edge.w));
        state->u = USpeed() * delta + state->u;
        state->v = VSpeed() * delta + state->v;
        float boundedTime = BSMIN(StartTime(), BSMAX_00404010(EndTime(), time));
        state->alpha = (unsigned int)LinearInterpolate(StartAlpha(), EndAlpha(), EndTime(), StartTime(), boundedTime);
    }
}

struct SwapFlag
{
    bool value;
};
extern "C" void __cdecl BSEndian_00401080(void* field, bool reverseBytes);
// @fnv 0x00506850
// pdb: EffectShaderData::Endian | TESEffectShader.obj | retail, upstream name already matches, high
void EffectShaderData::Endian()
{
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eTextureBlendModeSource, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eTextureBlendOperation, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eTextureZTestFunction, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iFillColor, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaFadeInTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaFullTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaFadeOutTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaPersistentPercent, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaPulseAmplitude, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaPulseFrequency, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillTextureUAnimSpeed, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillTextureVAnimSpeed, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeExponentValue, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iEdgeColor, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaFadeInTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaFullTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaFadeOutTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaPersistentPercent, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaPulseAmplitude, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaPulseFrequency, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fFillAlphaFullPercent, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fEdgeAlphaFullPercent, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eTextureBlendModeDest, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eParticleBlendModeSource, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eParticleBlendOperation, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eParticleZTestFunction, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&eParticleBlendModeDest, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleFadeInTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleFullTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleFadeOutTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleFull, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticlePersistent, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleLifetime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleLifeVar, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleNormalSpeed, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleNormalAcc, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&ParticleVelocity.x, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&ParticleVelocity.y, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&ParticleVelocity.z, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&ParticleAcceleration.x, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&ParticleAcceleration.y, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&ParticleAcceleration.z, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleScale1, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleScale2, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleScale1Time, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleScale2Time, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iParticleColor1, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iParticleColor2, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iParticleColor3, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleColor1Alpha, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleColor2Alpha, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleColor3Alpha, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleColor1Time, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleColor2Time, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleColor3Time, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleNormalSpeedVar, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleRotate, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleRotateVar, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleRotateSpeed, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fParticleRotateSpeedVar, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAlphaTestStartTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAlphaTestEndTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAlphaTestStartValue, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAlphaTestEndValue, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAlphaEdgeWidth, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iAlphaEdgeColor, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fExplosionSpeed, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iParticleTextureCountX, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&iParticleTextureCountY, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAddOnFadeInTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAddOnFadeOutTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAddOnScaleStart, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAddOnScaleEnd, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAddOnScaleInTime, needsEndianSwap.value);
    }
    {
        SwapFlag needsEndianSwap = {};
        BSEndian_00401080(&fAddOnScaleOutTime, needsEndianSwap.value);
    }
}
