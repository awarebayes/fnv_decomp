// fallout_shared/tesimagespacemodifier.cpp: TESImageSpaceModifier, NiColorA, ImageSpaceModifierEffectData, NiColorInterpolator and more (37 functions)

#include "layout_support.h"
#include "../../include/ImageSpaceManager.h"
#include "../../include/ImageSpaceModifierEffectData.h"
#include "../../include/TESImageSpaceModifier.h"
#include "../../include/TESSound.h"
#include "../../include/NiColorData.h"
#include "../../include/NiPoint3.h"

#include <malloc.h>
void* __cdecl operator new(unsigned int);
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete(void*);
void __cdecl operator delete[](void*);
extern "C" void* __cdecl memset(void*, int, unsigned int);
template <int N> struct EndianSizeTag
{
};
void BSEndian(unsigned int*, EndianSizeTag<4>);
class TimedAnimationKey
{
  public:
    float m_fTime;
};
class TimedFloatKey : public TimedAnimationKey
{
  public:
    float m_fValue;
    TimedFloatKey();
    ~TimedFloatKey();
    void TESRegionPoint_SetX(float);
    void FUN_004f5d90(float);
};
class ColorAnimationKey
{
  public:
    float m_fTime;
    NiColorA m_kRgbaColor;
};
class AlphaColor : public ColorAnimationKey
{
  public:
    AlphaColor();
    ~AlphaColor();
    void TESRegionPoint_SetX(float);
    void NiColorKey_SetColor(const NiColorA&);
};
class TESImageSpaceColorDataPointer
{
  public:
    NiColorData*& operator=(NiColorData*);
};
FNV_STATIC_ASSERT(sizeof(TESImageSpaceModifier::ImageSpaceModifierEffectData) == 0xf4);
FNV_STATIC_ASSERT(offsetof(TESImageSpaceModifier, m_akFloatInterpolators) == 0x114);
FNV_STATIC_ASSERT(offsetof(TESImageSpaceModifier, pEffectEntryTable) == 0x654);
extern float DAT_0118aba0[2];
extern float DAT_0118aba8[2];

extern float DAT_011c96fc;
extern const float DAT_01018f98;
void BSClamp_float(float&, float, float);
extern TESImageSpaceModifier DAT_011c9750;
int __cdecl Warning(const char*, ...);
// Loads an IMAD record: reads the header, EDID/OBND/DNAM chunks and per-chunk arrays of entries, converting endianness and filling the linked lists.
// @fnv 0x0052d510
// pdb: TESImageSpaceModifier::Load | TESImageSpaceModifier.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, bdataread, formid, pMessage, uisize, pendianData, i, itempint, iindex, iop
bool TESImageSpaceModifier::Load(TESFile* apFile)
{
    bool loadedData;
    unsigned char form = apFile->GetTESForm();
    if (form != ENUM_FORM_ID_IMAD_ID)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    loadedData = false;
    while (int chunk = apFile->GetChunkID())
    {
        switch (chunk)
        {
        case CHUNK_ID_EDID_ID: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 512);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case CHUNK_ID_OBND_ID:
            readOBNDSubRecord(apFile);
            break;
        case CHUNK_ID_DNAM_ID:
            apFile->GetChunkData(&Data, sizeof(Data));
            if (apFile->IsEndianConversionRequired())
                Data.Endian();
            loadedData = true;
            break;
        case CHUNK_ID_BNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountSecond;
            pBlurFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pBlurFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pBlurFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            BlurInterpolator.SetAnimationKeys(pBlurFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_VNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountThird;
            pDoubleFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pDoubleFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pDoubleFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            DoubleInterpolator.SetAnimationKeys(pDoubleFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_RNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountFourth;
            pRadialBlurStrengthFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pRadialBlurStrengthFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pRadialBlurStrengthFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            RadialBlurStrengthInterpolator.SetAnimationKeys(pRadialBlurStrengthFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_SNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountFifth;
            pRadialBlurRampupFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pRadialBlurRampupFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pRadialBlurRampupFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            RadialBlurRampupInterpolator.SetAnimationKeys(pRadialBlurRampupFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_UNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountSixth;
            pRadialBlurStartFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pRadialBlurStartFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pRadialBlurStartFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            RadialBlurStartInterpolator.SetAnimationKeys(pRadialBlurStartFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_NAM1_ID: {
            unsigned int count = Data.m_uiEffectEntryCountTenth;
            pRadialBlurRampDownFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pRadialBlurRampDownFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pRadialBlurRampDownFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            RadialBlurRampDownInterpolator.SetAnimationKeys(pRadialBlurRampDownFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_NAM2_ID: {
            unsigned int count = Data.m_uiEffectEntryCountEleventh;
            pRadialBlurDownStartFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pRadialBlurDownStartFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pRadialBlurDownStartFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            RadialBlurDownStartInterpolator.SetAnimationKeys(pRadialBlurDownStartFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_TNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountFirst;
            pTintColorKey = new AlphaColor[count];
            apFile->GetChunkData(pTintColorKey, count * sizeof(AlphaColor));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pTintColorKey;
                for (unsigned int i = 0; i < count * sizeof(AlphaColor) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            TintColorInterpolator.ReplaceKeys(pTintColorKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_NAM3_ID: {
            unsigned int count = Data.m_uiEffectEntryCountTwelfth;
            pFadeColorKey = new AlphaColor[count];
            apFile->GetChunkData(pFadeColorKey, count * sizeof(AlphaColor));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pFadeColorKey;
                for (unsigned int i = 0; i < count * sizeof(AlphaColor) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            FadeColorInterpolator.ReplaceKeys(pFadeColorKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_WNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountSeventh;
            pDepthOfFieldStrengthFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pDepthOfFieldStrengthFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pDepthOfFieldStrengthFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            DepthOfFieldStrengthInterpolator.SetAnimationKeys(pDepthOfFieldStrengthFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_XNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountEighth;
            pDepthOfFieldDistanceFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pDepthOfFieldDistanceFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pDepthOfFieldDistanceFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            DepthOfFieldDistanceInterpolator.SetAnimationKeys(pDepthOfFieldDistanceFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_YNAM_ID: {
            unsigned int count = Data.m_uiEffectEntryCountNinth;
            pDepthOfFieldRangeFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pDepthOfFieldRangeFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pDepthOfFieldRangeFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            DepthOfFieldRangeInterpolator.SetAnimationKeys(pDepthOfFieldRangeFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_NAM4_ID: {
            unsigned int count = Data.m_uiEffectEntryCountThirteenth;
            pMotionBlurStrengthFloatKey = new TimedFloatKey[count];
            apFile->GetChunkData(pMotionBlurStrengthFloatKey, count * sizeof(TimedFloatKey));
            if (apFile->IsEndianConversionRequired())
            {
                unsigned int* words = (unsigned int*)pMotionBlurStrengthFloatKey;
                for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                    BSEndian(&words[i], EndianSizeTag<4>());
            }
            MotionBlurStrengthInterpolator.SetAnimationKeys(pMotionBlurStrengthFloatKey, count, NiAnimationKey::LINKEY);
            break;
        }
        case CHUNK_ID_RDSD_ID: {
            unsigned int sound = 0;
            apFile->GetChunkData(&sound);
            pIntroSound = (TESSound*)sound;
            break;
        }
        case CHUNK_ID_RDSI_ID: {
            unsigned int sound = 0;
            apFile->GetChunkData(&sound);
            pOutroSound = (TESSound*)sound;
            break;
        }
        default:
            if (chunk >= 0x44414900 && chunk <= 0x444149ff)
            {
                unsigned int count;
                int column;
                int row = chunk & 0x3f;
                column = (chunk & 0xc0) >> 6;
                count = Data.m_auiKeyframeCountsByTypeAndChannel[row][column];
                pEffectEntryTable[row][column] = new TimedFloatKey[count];
                apFile->GetChunkData(pEffectEntryTable[row][column], count * sizeof(TimedFloatKey));
                if (apFile->IsEndianConversionRequired())
                {
                    unsigned int* words = (unsigned int*)pEffectEntryTable[row][column];
                    for (unsigned int i = 0; i < count * sizeof(TimedFloatKey) / sizeof(unsigned int); ++i)
                        BSEndian(&words[i], EndianSizeTag<4>());
                }
                m_akFloatInterpolators[row][column].SetAnimationKeys(pEffectEntryTable[row][column], count,
                                                                     NiAnimationKey::LINKEY);
            }
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x0052bd40
// pdb: TESImageSpaceModifier::InitializeData | TESImageSpaceModifier.obj | retail, vtable slot, high
// pdb locals: j, k, i
void TESImageSpaceModifier::ResetFormData()
{
    int i, j, kKey;
    memset(&Data, 0, sizeof(Data));
    Data.m_bOverride = false;
    Data.fDuration = 1.0f;
    Data.m_bUseRadialBlur = false;
    Data.m_kRadialBlurCenter = NiPoint2(0.5f, 0.5f);
    Data.m_bUseEffectEntries = false;
    Data.m_ucEffectMode = 0;
    for (i = 0; i < 21; ++i)
        for (j = 0; j < 2; ++j)
        {
            Data.m_auiKeyframeCountsByTypeAndChannel[i][j] = 2;
            pEffectEntryTable[i][j] = new TimedFloatKey[2];
            for (kKey = 0; kKey < 2; ++kKey)
            {
                pEffectEntryTable[i][j][kKey].TESRegionPoint_SetX(DAT_0118aba0[kKey]);
                pEffectEntryTable[i][j][kKey].FUN_004f5d90(DAT_0118aba8[j]);
            }
            m_akFloatInterpolators[i][j].SetAnimationKeys(
                pEffectEntryTable[i][j], Data.m_auiKeyframeCountsByTypeAndChannel[i][j], NiAnimationKey::LINKEY);
        }
    Data.m_uiEffectEntryCountFirst = 2;
    pTintColorKey = new AlphaColor[2];
    for (i = 0; i < 2; ++i)
    {
        pTintColorKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pTintColorKey[i].NiColorKey_SetColor(NiColorA(1.0f, 1.0f, 1.0f, 0.0f));
    }
    TintColorInterpolator.ReplaceKeys(pTintColorKey, Data.m_uiEffectEntryCountFirst,
                                             NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountTwelfth = 2;
    pFadeColorKey = new AlphaColor[2];
    for (i = 0; i < 2; ++i)
    {
        pFadeColorKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pFadeColorKey[i].NiColorKey_SetColor(NiColorA(1.0f, 1.0f, 1.0f, 0.0f));
    }
    FadeColorInterpolator.ReplaceKeys(pFadeColorKey, Data.m_uiEffectEntryCountTwelfth,
                                             NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountSecond = 2;
    pBlurFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pBlurFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pBlurFloatKey[i].FUN_004f5d90(0.0f);
    }
    BlurInterpolator.SetAnimationKeys(pBlurFloatKey, Data.m_uiEffectEntryCountSecond,
                                             NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountThird = 2;
    pDoubleFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pDoubleFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pDoubleFloatKey[i].FUN_004f5d90(0.0f);
    }
    DoubleInterpolator.SetAnimationKeys(pDoubleFloatKey, Data.m_uiEffectEntryCountThird, NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountFourth = 2;
    pRadialBlurStrengthFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pRadialBlurStrengthFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pRadialBlurStrengthFloatKey[i].FUN_004f5d90(0.0f);
    }
    RadialBlurStrengthInterpolator.SetAnimationKeys(pRadialBlurStrengthFloatKey, Data.m_uiEffectEntryCountFourth,
                                              NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountFifth = 2;
    pRadialBlurRampupFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pRadialBlurRampupFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pRadialBlurRampupFloatKey[i].FUN_004f5d90(0.0f);
    }
    RadialBlurRampupInterpolator.SetAnimationKeys(pRadialBlurRampupFloatKey, Data.m_uiEffectEntryCountFifth,
                                             NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountSixth = 2;
    pRadialBlurStartFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pRadialBlurStartFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pRadialBlurStartFloatKey[i].FUN_004f5d90(0.0f);
    }
    RadialBlurStartInterpolator.SetAnimationKeys(pRadialBlurStartFloatKey, Data.m_uiEffectEntryCountSixth,
                                                NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountTenth = 2;
    pRadialBlurRampDownFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pRadialBlurRampDownFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pRadialBlurRampDownFloatKey[i].FUN_004f5d90(0.0f);
    }
    RadialBlurRampDownInterpolator.SetAnimationKeys(pRadialBlurRampDownFloatKey, Data.m_uiEffectEntryCountTenth,
                                               NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountEleventh = 2;
    pRadialBlurDownStartFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pRadialBlurDownStartFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pRadialBlurDownStartFloatKey[i].FUN_004f5d90(0.0f);
    }
    RadialBlurDownStartInterpolator.SetAnimationKeys(pRadialBlurDownStartFloatKey, Data.m_uiEffectEntryCountEleventh,
                                              NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountSeventh = 2;
    pDepthOfFieldStrengthFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pDepthOfFieldStrengthFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pDepthOfFieldStrengthFloatKey[i].FUN_004f5d90(0.0f);
    }
    DepthOfFieldStrengthInterpolator.SetAnimationKeys(pDepthOfFieldStrengthFloatKey, Data.m_uiEffectEntryCountSeventh,
                                              NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountEighth = 2;
    pDepthOfFieldDistanceFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pDepthOfFieldDistanceFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pDepthOfFieldDistanceFloatKey[i].FUN_004f5d90(0.0f);
    }
    DepthOfFieldDistanceInterpolator.SetAnimationKeys(pDepthOfFieldDistanceFloatKey, Data.m_uiEffectEntryCountEighth,
                                             NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountNinth = 2;
    pDepthOfFieldRangeFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pDepthOfFieldRangeFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pDepthOfFieldRangeFloatKey[i].FUN_004f5d90(0.0f);
    }
    DepthOfFieldRangeInterpolator.SetAnimationKeys(pDepthOfFieldRangeFloatKey, Data.m_uiEffectEntryCountNinth,
                                               NiAnimationKey::LINKEY);
    Data.m_uiEffectEntryCountThirteenth = 2;
    pMotionBlurStrengthFloatKey = new TimedFloatKey[2];
    for (i = 0; i < 2; ++i)
    {
        pMotionBlurStrengthFloatKey[i].TESRegionPoint_SetX(DAT_0118aba0[i]);
        pMotionBlurStrengthFloatKey[i].FUN_004f5d90(0.0f);
    }
    MotionBlurStrengthInterpolator.SetAnimationKeys(pMotionBlurStrengthFloatKey, Data.m_uiEffectEntryCountThirteenth,
                                               NiAnimationKey::LINKEY);
    pIntroSound = 0;
    pOutroSound = 0;
}

// @fnv 0x00531720
// pdb: TESImageSpaceModifier::Apply | TESImageSpaceModifier.obj | retail, upstream name already matches, high
// pdb params: afStartTime, afStrength, apTarget
// pdb locals: pmanager, fmbstr, fradialblurrampdown, fradialblurstart, ftime, fradialblurrampup, fdofrange, fdoubleval, fradialblurstr, fdofdistance, fradialblurdownstart, fval, fdofstrength, colval, fblurval, i, j
void TESImageSpaceModifier::Apply(float afStartTime, float afStrength, NiAVObject* apTarget)
{
    float effectTime;
    if (Data.m_bOverride)
        effectTime = (DAT_011c96fc - afStartTime) / Data.fDuration;
    else
        effectTime = 0.0f;
    BSClamp_float(effectTime, 0.0f, 1.0f);
    ImageSpaceManager* pManager = ImageSpaceManager::BSShaderManager_GetImageSpaceManager();
    float pfParameterValues[2];
    for (int i = 0; i < 21; ++i)
    {
        for (int j = 0; j < 2; ++j)
            m_akFloatInterpolators[i][j].UpdateFloat(effectTime, 0, pfParameterValues[j]);
        if (i >= 0 && i <= 13)
            pManager->ApplyParamModifier_00b8cb30(1, i, pfParameterValues, afStrength);
        if (i >= 14 && i <= 16)
            pManager->ApplyParamModifier_00b8cb30(0, i - 14, pfParameterValues, afStrength);
        if (i >= 17 && i <= 20)
            pManager->ApplyParamModifier_00b8cb30(3, i - 17, pfParameterValues, afStrength);
    }
    NiColorA kCurrentColor(0.0f, 0.0f, 0.0f, 0.0f);
    TintColorInterpolator.UpdateColorA(effectTime, 0, kCurrentColor);
    kCurrentColor.a *= afStrength;
    BSClamp_float(kCurrentColor.a, 0.0f, 1.0f);
    pManager->FUN_00b8b540(kCurrentColor);
    FadeColorInterpolator.UpdateColorA(effectTime, 0, kCurrentColor);
    kCurrentColor.a *= afStrength;
    BSClamp_float(kCurrentColor.a, 0.0f, 1.0f);
    pManager->FUN_00b8b610(kCurrentColor);
    float interpolatedBlur;
    BlurInterpolator.UpdateFloat(effectTime, 0, interpolatedBlur);
    float localDouble;
    DoubleInterpolator.UpdateFloat(effectTime, 0, localDouble);
    float interpolatedRadialBlurStrength;
    RadialBlurStrengthInterpolator.UpdateFloat(effectTime, 0, interpolatedRadialBlurStrength);
    float fRadialBlurRampupResult;
    RadialBlurRampupInterpolator.UpdateFloat(effectTime, 0, fRadialBlurRampupResult);
    float radialBlurStartOutput;
    RadialBlurStartInterpolator.UpdateFloat(effectTime, 0, radialBlurStartOutput);
    float fRadialBlurRampDownSample;
    RadialBlurRampDownInterpolator.UpdateFloat(effectTime, 0, fRadialBlurRampDownSample);
    float radialBlurDownStartParameter;
    RadialBlurDownStartInterpolator.UpdateFloat(effectTime, 0, radialBlurDownStartParameter);
    float sampledDepthOfFieldStrength;
    DepthOfFieldStrengthInterpolator.UpdateFloat(effectTime, 0, sampledDepthOfFieldStrength);
    float fDepthOfFieldDistanceOutput;
    DepthOfFieldDistanceInterpolator.UpdateFloat(effectTime, 0, fDepthOfFieldDistanceOutput);
    float fCurrentDepthOfFieldRange;
    DepthOfFieldRangeInterpolator.UpdateFloat(effectTime, 0, fCurrentDepthOfFieldRange);
    float motionBlurSample;
    MotionBlurStrengthInterpolator.UpdateFloat(effectTime, 0, motionBlurSample);
    interpolatedBlur *= afStrength;
    BSClamp_float(interpolatedBlur, 0.0f, DAT_01018f98);
    pManager->RaiseHighestBlurRadius(interpolatedBlur);
    pManager->ApplyDoubleVisionParamModifier_00b8afb0(localDouble * afStrength);
    if (Data.m_bUseRadialBlur && apTarget)
    {
        apTarget->UpdateWorldBound();
        pManager->ApplyRadialBlurParamModifier(interpolatedRadialBlurStrength * afStrength, fRadialBlurRampupResult,
                               radialBlurStartOutput, fRadialBlurRampDownSample, radialBlurDownStartParameter,
                               apTarget->NiAVObject_GetWorldBound());
    }
    else if (Data.m_bUseRadialBlur)
        pManager->SetRadialBlurParameters(interpolatedRadialBlurStrength * afStrength, fRadialBlurRampupResult,
                                          radialBlurStartOutput, fRadialBlurRampDownSample,
                                          radialBlurDownStartParameter, NiPoint2(0.5f, 0.5f));
    else
        pManager->SetRadialBlurParameters(interpolatedRadialBlurStrength * afStrength, fRadialBlurRampupResult,
                                          radialBlurStartOutput, fRadialBlurRampDownSample,
                                          radialBlurDownStartParameter, Data.m_kRadialBlurCenter);
    if (Data.m_bUseEffectEntries && apTarget)
    {
        apTarget->UpdateWorldBound();
        pManager->SetDistanceEffect(sampledDepthOfFieldStrength * afStrength, Data.m_ucEffectMode, 0.0f,
                                    apTarget->NiAVObject_GetWorldBound());
    }
    else if (Data.m_bUseEffectEntries)
        pManager->SetDepthOfFieldParameters(sampledDepthOfFieldStrength * afStrength, Data.m_ucEffectMode, 0.0f, 0.0f,
                                            1.0f);
    else
        pManager->SetDepthOfFieldParameters(sampledDepthOfFieldStrength * afStrength, Data.m_ucEffectMode, 0.0f,
                                            fDepthOfFieldDistanceOutput, fCurrentDepthOfFieldRange);
    pManager->RaiseBlurRadius(motionBlurSample);
}

// @fnv 0x00531d20
// pdb: TESImageSpaceModifier::ApplyWeather | TESImageSpaceModifier.obj | debug, object-file run extension, low
// pdb params: afStartTime, afStrength
// pdb locals: pmanager, fmbstr, fradialblurrampdown, fradialblurstart, ftime, fradialblurrampup, fdofrange, fdoubleval, fradialblurstr, fdofdistance, fradialblurdownstart, fval, fdofstrength, colval, fblurval, i, j
void TESImageSpaceModifier::EvaluateInterpolators(float startTime, float strength)
{
    float effectTime;
    if (Data.m_bOverride)
        effectTime = (DAT_011c96fc - startTime) / Data.fDuration;
    else
        effectTime = 0.0f;
    ImageSpaceManager* pManager = ImageSpaceManager::BSShaderManager_GetImageSpaceManager();
    float pfParameterValues[2];
    for (int i = 0; i < 21; ++i)
    {
        for (int j = 0; j < 2; ++j)
            m_akFloatInterpolators[i][j].UpdateFloat(effectTime, 0, pfParameterValues[j]);
        if (i >= 0 && i <= 13)
            pManager->AccumulateComponentValues(1, i, pfParameterValues, strength);
        if (i >= 14 && i <= 16)
            pManager->AccumulateComponentValues(0, i - 14, pfParameterValues, strength);
        if (i >= 17 && i <= 20)
            pManager->AccumulateComponentValues(3, i - 17, pfParameterValues, strength);
    }
    NiColorA kCurrentColor(0.0f, 0.0f, 0.0f, 0.0f);
    TintColorInterpolator.UpdateColorA(effectTime, 0, kCurrentColor);
    kCurrentColor *= strength;
    pManager->FUN_00b8b5c0(kCurrentColor);
    FadeColorInterpolator.UpdateColorA(effectTime, 0, kCurrentColor);
    kCurrentColor *= strength;
    pManager->FUN_00b8b690(kCurrentColor);
    float interpolatedBlur;
    BlurInterpolator.UpdateFloat(effectTime, 0, interpolatedBlur);
    float localDouble;
    DoubleInterpolator.UpdateFloat(effectTime, 0, localDouble);
    float interpolatedRadialBlurStrength;
    RadialBlurStrengthInterpolator.UpdateFloat(effectTime, 0, interpolatedRadialBlurStrength);
    float fRadialBlurRampupResult;
    RadialBlurRampupInterpolator.UpdateFloat(effectTime, 0, fRadialBlurRampupResult);
    float radialBlurStartOutput;
    RadialBlurStartInterpolator.UpdateFloat(effectTime, 0, radialBlurStartOutput);
    float fRadialBlurRampDownSample;
    RadialBlurRampDownInterpolator.UpdateFloat(effectTime, 0, fRadialBlurRampDownSample);
    float radialBlurDownStartParameter;
    RadialBlurDownStartInterpolator.UpdateFloat(effectTime, 0, radialBlurDownStartParameter);
    float sampledDepthOfFieldStrength;
    DepthOfFieldStrengthInterpolator.UpdateFloat(effectTime, 0, sampledDepthOfFieldStrength);
    float fDepthOfFieldDistanceOutput;
    DepthOfFieldDistanceInterpolator.UpdateFloat(effectTime, 0, fDepthOfFieldDistanceOutput);
    float fCurrentDepthOfFieldRange;
    DepthOfFieldRangeInterpolator.UpdateFloat(effectTime, 0, fCurrentDepthOfFieldRange);
    float motionBlurSample;
    MotionBlurStrengthInterpolator.UpdateFloat(effectTime, 0, motionBlurSample);
    pManager->ApplyBlurRadiusParamModifierWeather_00b8af40(interpolatedBlur * strength);
    pManager->AddDoubleVision(localDouble * strength);
    pManager->FUN_00b8b6e0(interpolatedRadialBlurStrength * strength, fRadialBlurRampupResult * strength,
                           radialBlurStartOutput * strength, fRadialBlurRampDownSample * strength,
                           radialBlurDownStartParameter * strength, Data.m_kRadialBlurCenter * strength);
    pManager->FUN_00b8b130(sampledDepthOfFieldStrength * strength, Data.m_ucEffectMode, 0.0f,
                           fDepthOfFieldDistanceOutput * strength, fCurrentDepthOfFieldRange * strength);
    pManager->ApplyMotionBlurStrengthParamModifierWeather_00b8af90(motionBlurSample * strength);
}

// Sets the form type to IMAD, calls the initializer, and if this is the global instance DAT_011c9750 calls the second initializer.
// @fnv 0x0052b8f0
// pdb: TESImageSpaceModifier::TESImageSpaceModifier | TESImageSpaceModifier.obj | retail, upstream name already matches, high
TESImageSpaceModifier::TESImageSpaceModifier()
    : BlurInterpolator(0), DoubleInterpolator(0),
      RadialBlurStrengthInterpolator(0), RadialBlurRampupInterpolator(0), RadialBlurStartInterpolator(0),
      RadialBlurRampDownInterpolator(0), RadialBlurDownStartInterpolator(0), DepthOfFieldStrengthInterpolator(0),
      DepthOfFieldDistanceInterpolator(0), DepthOfFieldRangeInterpolator(0), MotionBlurStrengthInterpolator(0)
{
    SetFormType(ENUM_FORM_ID_IMAD_ID);
    ResetFormData();
    if (this == &DAT_011c9750)
        NotifySaveLoadGameOfFormChange();
}
// @fnv 0x0052d260
// pdb: TESImageSpaceModifier::ClearData | TESImageSpaceModifier.obj | retail, upstream name already matches, high
// pdb locals: i, j
void TESImageSpaceModifier::ClearData()
{
    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 2; ++j)
            m_akFloatInterpolators[i][j].FUN_007fc2b0(0);
    TintColorInterpolator.SetColorData(0);
    FadeColorInterpolator.SetColorData(0);
    BlurInterpolator.FUN_007fc2b0(0);
    DoubleInterpolator.FUN_007fc2b0(0);
    RadialBlurStrengthInterpolator.FUN_007fc2b0(0);
    RadialBlurRampupInterpolator.FUN_007fc2b0(0);
    RadialBlurStartInterpolator.FUN_007fc2b0(0);
    RadialBlurRampDownInterpolator.FUN_007fc2b0(0);
    RadialBlurDownStartInterpolator.FUN_007fc2b0(0);
    DepthOfFieldStrengthInterpolator.FUN_007fc2b0(0);
    DepthOfFieldDistanceInterpolator.FUN_007fc2b0(0);
    DepthOfFieldRangeInterpolator.FUN_007fc2b0(0);
    MotionBlurStrengthInterpolator.FUN_007fc2b0(0);
    TESForm::ClearData();
    pIntroSound = 0;
    pOutroSound = 0;
}
// @fnv 0x0052d3d0
// pdb: TESImageSpaceModifier::InitItem | TESImageSpaceModifier.obj | retail, string-set bridge, high
// pdb locals: itempint
void TESImageSpaceModifier::InitItem()
{
    if (IsFormInitialized())
        return;
    unsigned int formID = (unsigned int)pIntroSound;
    if (formID)
    {
        RemapFormIDForFile(formID, GetFile(-1));
        pIntroSound = dynamic_cast<TESSound*>(TESForm::GetFormByID(formID));
        if (!pIntroSound)
            Warning("MASTERFILE: Could not find pIntroSound (%08X) for ImageSpaceModifier '%s' (%08X).", formID,
                    GetName(), GetFormID());
    }
    formID = (unsigned int)pOutroSound;
    if (formID)
    {
        RemapFormIDForFile(formID, GetFile(-1));
        pOutroSound = dynamic_cast<TESSound*>(TESForm::GetFormByID(formID));
        if (!pOutroSound)
            Warning("MASTERFILE: Could not find pOutroSound (%08X) for ImageSpaceModifier '%s' (%08X).", formID,
                    GetName(), GetFormID());
    }
    SetInitializationState(true);
}
// @fnv 0x005316c0
bool TESImageSpaceModifier::IsExpired(float startTime)
{
    if (!Data.m_bOverride)
        return false;
    if (startTime < -10000.0)
        return false;
    return DAT_011c96fc - startTime > Data.fDuration;
}
// @fnv 0x005330a0
// pdb: OBJ_LAND::Endian | TESLand.obj | debug, object-file run extension, low
void TESImageSpaceModifier::InitializeModifierData()
{
    InitializeComponent();
    Data.m_bOverride = true;
    pDoubleFloatKey->FUN_004f5d90(0.005f);
}

// NiColorA::operator*=(float) (community FalloutNV.pdb public symbol at this address).
// @fnv 0x005321d0
NiColorA& NiColorA::operator*=(float scale)
{
    r *= scale;
    g *= scale;
    b *= scale;
    a *= scale;
    return *this;
}
// @fnv 0x0052bad0
// pdb: TESImageSpaceModifier::ImageSpaceModifierData::ImageSpaceModifierData | TESImageSpaceModifier.obj | debug, object-file run extension, low
ImageSpaceModifierEffectData::ImageSpaceModifierEffectData()
{}
// @fnv 0x0052d3b0
// pdb: NiColorInterpolator::SetColorData | TESImageSpaceModifier.obj | debug, upstream name already matches, high
// pdb params: pkColorData
void NiColorInterpolator::SetColorData(NiColorData* pkColorData)
{
    reinterpret_cast<TESImageSpaceColorDataPointer&>(m_spColorData) = pkColorData;
}
// @fnv 0x00532fd0
NiPoint2 operator*(float scale, const NiPoint2& arPoint)
{
    return arPoint * scale;
}
// @fnv 0x00532ff0
TESImageSpaceModifier* TESImageSpaceModifier::QDefault()
{
    return &DAT_011c9750;
}

struct NiStream;
struct blob48
{
    char b[48];
};
struct blob64
{
    char b[64];
};

// Multiplies the point pt by the 3x3 matrix mat and stores the result in pOut, returning pOut.
// @fnv 0x004b3ae0
// src: GUESS 0x004b3ae0 (confidence: medium)
// pdb: operator* | GlobalFunc.obj | debug, call graph, medium
// pdb params: pt, mat
NiPoint3* NiPoint3_TransformByMatrix3(NiPoint3* pOut, const NiPoint3& pt, const float* mat)
{
    pOut->Set(pt.x * mat[0] + pt.y * mat[3] + pt.z * mat[6],
              pt.x * mat[1] + pt.y * mat[4] + pt.z * mat[7],
              pt.x * mat[2] + pt.y * mat[5] + pt.z * mat[8]);
    return pOut;
}

// @fnv 0x0052baf0
// pdb: NiFloatInterpolator::`default constructor closure' | TESImageSpaceModifier.obj | debug, object-file run extension, low
void __fastcall ConstructFloatInterpolator(NiFloatInterpolator* pThis)
{
    pThis->NiFloatInterpolator::NiFloatInterpolator(0);
}

#include "../../include/NiMemObject.h"
typedef char _sizecheck_NiMemObject[1];

struct Obj52 : NiMemObject
{
    void FUN_00a3cc80();
    void* FUN_0052cf90(unsigned int);
    static void operator delete(void*, unsigned int);
};
// Calls FUN_00a3cc80, deletes this with size 0x18 when flags has bit 0 set, and returns this.
// @fnv 0x0052cf90
// pdb: NiFloatData::`scalar deleting destructor' | TESImageSpaceModifier.obj | retail, vtable slot, high
void* Obj52::FUN_0052cf90(unsigned int flags)
{
    FUN_00a3cc80();
    if (flags & 1)
        Obj52::operator delete(this, 0x18);
    return this;
}

// Exposes this unit's deleting-destructor entry without duplicating NiColorData's layout.
struct ImageSpaceColorData : NiColorData
{
    void* Destroy(unsigned int flags);
};

// @fnv 0x0052d130
// pdb: NiColorData::`scalar deleting destructor' | TESImageSpaceModifier.obj | retail, vtable slot, high
void* ImageSpaceColorData::Destroy(unsigned int flags)
{
    NiColorData::~NiColorData();
    if (flags & 1)
        NiColorData::operator delete(this, sizeof(NiColorData));
    return this;
}

// Clamps the float pointed to by aVal to the range [aMin, aMax], writing the bound back when it is out of range.
// @fnv 0x005330e0
// src: XBOX_PDB 0x005330e0
// pdb: HGT_STRUCT::Endian | TESLand.obj | debug, object-file run extension, low
void __cdecl BSClamp_float(float* aVal, float aMin, float aMax)
{
    if (*aVal > aMax)
        *aVal = aMax;
    else if (*aVal < aMin)
        *aVal = aMin;
}

struct GlobalObjectO2_00a7b610;
extern GlobalObjectO2_00a7b610 NiFloatData_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011f3748 (old: g_011f3748)

struct FUN_0052cf80_shell
{
    GlobalObjectO2_00a7b610* GetGlobalObject();
};

// @fnv 0x0052cf80
GlobalObjectO2_00a7b610* FUN_0052cf80_shell::GetGlobalObject()
{
    return &NiFloatData_ms_RTTI;
}

extern NiRTTI NiColorData_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011f3750 (old: DAT_011f3750/g_011f3750)

// @fnv 0x0052d120
const NiRTTI* NiColorData::GetRTTI() const
{
    return &NiColorData_ms_RTTI;
}

struct ImageSpaceModifierDeleting : TESImageSpaceModifier
{
    void* __thiscall Destroy(unsigned int flags);
};

extern void* __cdecl _Deallocate(void*);

// Runs the destructor, frees this with _Deallocate when flags has bit 0 set, and returns this.
// @fnv 0x0052baa0
// pdb: TESImageSpaceModifier::`scalar deleting destructor' | TESImageSpaceModifier.obj | retail, vtable slot, high
void* __thiscall ImageSpaceModifierDeleting::Destroy(unsigned int flags)
{
    TESImageSpaceModifier::~TESImageSpaceModifier();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

struct FUN_00533060_shell
{
    void* __thiscall NiFloatData_GetAnim(void** a, void** b, char* c);
};
// @fnv 0x00533060
// src: COMMUNITY 0x00533060
void* __thiscall FUN_00533060_shell::NiFloatData_GetAnim(void** apFirst, void** apSecond, char* c)
{
    *apFirst = *(void**)((char*)this + 8);
    *apSecond = *(void**)((char*)this + 0x10);
    *c = *(char*)((char*)this + 0x14);
    return *(void**)((char*)this + 0xc);
}

namespace fnv_00533000 {
struct Flt
{
    operator unsigned int();
};
struct FUN_00533060_shell
{
    void* NiFloatData_GetAnim(void**, void**, char*);
};
struct FUN_00533000_shell
{
    char _pad[16];
    Flt m_kHandle;
    void* NiFloatInterpolator_GetKeys_0(void**, void**, char*);
};
// If the stored pointer is non-null, forwards to FUN_00533060; otherwise zeroes the three output pointers and returns null.
// @fnv 0x00533000
// src: COMMUNITY 0x00533000
void* FUN_00533000_shell::NiFloatInterpolator_GetKeys_0(void** a, void** b, char* c)
{
    if ((unsigned int)m_kHandle)
        return ((FUN_00533060_shell*)(unsigned int)m_kHandle)->NiFloatData_GetAnim(a, b, c);
    *a = 0;
    *b = 0;
    *c = 0;
    return 0;
}
}  // namespace fnv_00533000

// Destructor that calls TESImageSpaceModifier::ClearData on this object.
// @fnv 0x0052bbd0
// pdb: TESImageSpaceModifier::~TESImageSpaceModifier | TESImageSpaceModifier.obj | retail, upstream name already matches, high
TESImageSpaceModifier::~TESImageSpaceModifier()
{
    TESImageSpaceModifier::ClearData();
}

struct FUN_00532e40_s
{
    float x;
    float y;
    float z;
    float w;
    FUN_00532e40_s* AddDelta(FUN_00532e40_s* result, float* delta);
};

// @fnv 0x00532e40
FUN_00532e40_s* FUN_00532e40_s::AddDelta(FUN_00532e40_s* result, float* delta)
{
    FUN_00532e40_s temp = *this;
    temp.x += delta[0];
    temp.y += delta[1];
    temp.z += delta[2];
    temp.w += delta[3];
    *result = temp;
    return result;
}

struct FUN_00532ec0_v4
{
    float x;
    float y;
    float z;
    float w;
};
struct FUN_00532ec0
{
    FUN_00532ec0_v4* SUB(FUN_00532ec0_v4* result, const FUN_00532ec0_v4& sub);
};
// Subtracts the components of sub from this vector and stores the result in result, returning result.
// @fnv 0x00532ec0
FUN_00532ec0_v4* FUN_00532ec0::SUB(FUN_00532ec0_v4* result, const FUN_00532ec0_v4& sub)
{
    FUN_00532ec0_v4 tmp = *(FUN_00532ec0_v4*)this;
    tmp.x -= sub.x;
    tmp.y -= sub.y;
    tmp.z -= sub.z;
    tmp.w -= sub.w;
    *result = tmp;
    return result;
}

extern "C" void __fastcall TESImageSpaceModifier_FlushData(TESImageSpaceModifier*);

int __cdecl _memcmp(const void* left, const void* right, unsigned int count);
void* __cdecl FUN_00ec43fb(void* object, unsigned int type, void* data, const void* typeInfo, unsigned char option);
extern char _DAT_01183028;

// Looks up the TESImageSpaceModifier matching form, updates both objects, and returns false only if all compared fields and arrays are identical.
// @fnv 0x005310b0
// pdb: TESImageSpaceModifier::Compare | TESImageSpaceModifier.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pcomp, i, j
bool TESImageSpaceModifier::Compare(TESForm* form)
{
    TESImageSpaceModifier* imageSpaceModifier;
    int i;
    int j;

    imageSpaceModifier =
        static_cast<TESImageSpaceModifier*>(FUN_00ec43fb(form, 0, &_DAT_01183028, &typeid(TESImageSpaceModifier), 0));

    if (!imageSpaceModifier)
        return true;

    TESImageSpaceModifier_FlushData(this);
    TESImageSpaceModifier_FlushData(imageSpaceModifier);

    if (pIntroSound !=
        imageSpaceModifier->pIntroSound)
        return true;

    if (pOutroSound !=
        imageSpaceModifier->pOutroSound)
        return true;

    if (TESForm::Compare(imageSpaceModifier))
        return true;

    if (_memcmp(&Data, &imageSpaceModifier->Data, 0xf4))
        return true;

    if (_memcmp(pBlurFloatKey,
                imageSpaceModifier->pBlurFloatKey,
                Data.m_uiEffectEntryCountSecond * 8))
        return true;

    if (_memcmp(pDoubleFloatKey,
                imageSpaceModifier->pDoubleFloatKey,
                Data.m_uiEffectEntryCountThird * 8))
        return true;

    if (_memcmp(pTintColorKey,
                imageSpaceModifier->pTintColorKey,
                Data.m_uiEffectEntryCountFirst * 0x14))
        return true;

    if (_memcmp(pFadeColorKey,
                imageSpaceModifier->pFadeColorKey,
                Data.m_uiEffectEntryCountTwelfth * 0x14))
        return true;

    if (_memcmp(pRadialBlurStrengthFloatKey,
                imageSpaceModifier->pRadialBlurStrengthFloatKey,
                Data.m_uiEffectEntryCountFourth * 8))
        return true;

    if (_memcmp(pRadialBlurRampupFloatKey,
                imageSpaceModifier->pRadialBlurRampupFloatKey,
                Data.m_uiEffectEntryCountFifth * 8))
        return true;

    if (_memcmp(pRadialBlurStartFloatKey,
                imageSpaceModifier->pRadialBlurStartFloatKey,
                Data.m_uiEffectEntryCountSixth * 8))
        return true;

    if (_memcmp(pRadialBlurRampDownFloatKey,
                imageSpaceModifier->pRadialBlurRampDownFloatKey,
                Data.m_uiEffectEntryCountTenth * 8))
        return true;

    if (_memcmp(pRadialBlurDownStartFloatKey,
                imageSpaceModifier->pRadialBlurDownStartFloatKey,
                Data.m_uiEffectEntryCountEleventh * 8))
        return true;

    if (_memcmp(pDepthOfFieldStrengthFloatKey,
                imageSpaceModifier->pDepthOfFieldStrengthFloatKey,
                Data.m_uiEffectEntryCountSeventh * 8))
        return true;

    if (_memcmp(pDepthOfFieldDistanceFloatKey,
                imageSpaceModifier->pDepthOfFieldDistanceFloatKey,
                Data.m_uiEffectEntryCountEighth * 8))
        return true;

    if (_memcmp(pDepthOfFieldRangeFloatKey,
                imageSpaceModifier->pDepthOfFieldRangeFloatKey,
                Data.m_uiEffectEntryCountNinth * 8))
        return true;

    if (_memcmp(pMotionBlurStrengthFloatKey,
                imageSpaceModifier->pMotionBlurStrengthFloatKey,
                Data.m_uiEffectEntryCountThirteenth * 8))
        return true;

    for (i = 0; i < 0x15; ++i)
    {
        for (j = 0; j < 2; ++j)
        {
            if (_memcmp(pEffectEntryTable[i][j],
                        imageSpaceModifier->pEffectEntryTable[i][j],
                        Data.m_auiKeyframeCountsByTypeAndChannel[i][j] * 8))
                return true;
        }
    }

    return false;
}

// @fnv 0x00531490
// pdb: TESImageSpaceModifier::ImageSpaceModifierData::Endian | TESImageSpaceModifier.obj | retail, call graph (shared callers), medium
// pdb locals: j, i
void ImageSpaceModifierEffectData::Endian()
{
    int j;
    int i;

    BSEndian(reinterpret_cast<unsigned int*>(&fDuration), EndianSizeTag<4>());

    for (j = 0; j < 21; ++j)
    {
        for (i = 0; i < 2; ++i)
        {
            BSEndian(&m_auiKeyframeCountsByTypeAndChannel[j][i],
                     EndianSizeTag<4>());
        }
    }

    BSEndian(&m_uiEffectEntryCountSecond, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountThird, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountFirst, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountTwelfth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountFourth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountFifth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountSixth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountTenth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountEleventh, EndianSizeTag<4>());
    BSEndian(reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this) + 0xcc), EndianSizeTag<4>());
    BSEndian(reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(this) + 0xd0), EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountSeventh, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountEighth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountNinth, EndianSizeTag<4>());
    BSEndian(&m_uiEffectEntryCountThirteenth, EndianSizeTag<4>());
}

struct FUN_0045cec0
{
    ~FUN_0045cec0();
};

struct FUN_00a3ca30
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;

    ~FUN_00a3ca30();
};

struct FUN_0052bb10 : public FUN_00a3ca30
{
    FUN_0045cec0 unk10;

    ~FUN_0052bb10();
};

FUN_0045cec0::~FUN_0045cec0()
{}

FUN_00a3ca30::~FUN_00a3ca30()
{}

// @fnv 0x0052bb10
// pdb: NiFloatInterpolator::~NiFloatInterpolator | TESImageSpaceModifier.obj | retail, call graph, medium
FUN_0052bb10::~FUN_0052bb10()
{}

namespace Real {
struct InterpolatorBase
{
    unsigned int referenceCount;
    unsigned int flags;
    float lastTime;
    float currentTime;
    float startTime;
    float endTime;
    void* controller;
    ~InterpolatorBase();
};
struct ColorDataReference
{
    void* colorData;
    ~ColorDataReference();
};
struct FUN_0052bb70 : InterpolatorBase
{
    ColorDataReference colorData;
    ~FUN_0052bb70();
};
// Destructor for an interpolator that owns a ColorDataReference; the body is empty.
// @fnv 0x0052bb70
// pdb: NiColorInterpolator::~NiColorInterpolator | TESImageSpaceModifier.obj | retail, call graph, medium
FUN_0052bb70::~FUN_0052bb70()
{}
}

struct SpawnedObject
{
    void Method(float* pOut, float f);
};

// @fnv 0x00532f40
// src: GUESS 0x00532f40 (confidence: low)
float* __cdecl Object_ProcessAndReturnValue_00532f40(float* pOut, float f, SpawnedObject* pObj)
{
    pObj->Method(pOut, f);
    return pOut;
}

struct FUN_00532f60_vec4
{
    float x;
    float y;
    float z;
    float w;
};

class FUN_00532f60_cls
{
  public:
    float m_uiX;
    float m_uiY;
    float m_uiZ;
    float m_w;
    FUN_00532f60_vec4* ScaleVector4(FUN_00532f60_vec4* out, float scale);
};

// Multiplies the four floats of this object by scale and writes the result to out, returning out.
// @fnv 0x00532f60
FUN_00532f60_vec4* FUN_00532f60_cls::ScaleVector4(FUN_00532f60_vec4* out, float scale)
{
    FUN_00532f60_vec4 vecScaled = *(FUN_00532f60_vec4*)this;
    vecScaled.x *= scale;
    vecScaled.y *= scale;
    vecScaled.z *= scale;
    vecScaled.w *= scale;
    *out = vecScaled;
    return out;
}

// Caches the key counts and arrays from the shared interpolators.
// @fnv 0x0052ed90
// src: XBOX_RETAIL_PDB 0x0052ed90
// pdb: TESImageSpaceModifier::FlushData | TESImageSpaceModifier.obj | retail, call graph (shared callers), high
// pdb locals: i, j
extern "C" void __fastcall TESImageSpaceModifier_FlushData(TESImageSpaceModifier* modifier)
{
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            modifier->Data.m_auiKeyframeCountsByTypeAndChannel[i][j] = modifier->m_akFloatInterpolators[i][j].GetColorKeyCount(0);
            modifier->pEffectEntryTable[i][j] = (TimedFloatKey*)modifier->m_akFloatInterpolators[i][j].GetChannelKeys(0);
        }
    }
    modifier->Data.m_uiEffectEntryCountSecond = modifier->BlurInterpolator.GetColorKeyCount(0);
    modifier->pBlurFloatKey = (TimedFloatKey*)modifier->BlurInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountThird = modifier->DoubleInterpolator.GetColorKeyCount(0);
    modifier->pDoubleFloatKey = (TimedFloatKey*)modifier->DoubleInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountFirst = modifier->TintColorInterpolator.GetColorKeyCount(0);
    modifier->pTintColorKey = (AlphaColor*)modifier->TintColorInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountTwelfth = modifier->FadeColorInterpolator.GetColorKeyCount(0);
    modifier->pFadeColorKey = (AlphaColor*)modifier->FadeColorInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountFourth = modifier->RadialBlurStrengthInterpolator.GetColorKeyCount(0);
    modifier->pRadialBlurStrengthFloatKey = (TimedFloatKey*)modifier->RadialBlurStrengthInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountFifth = modifier->RadialBlurRampupInterpolator.GetColorKeyCount(0);
    modifier->pRadialBlurRampupFloatKey = (TimedFloatKey*)modifier->RadialBlurRampupInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountSixth = modifier->RadialBlurStartInterpolator.GetColorKeyCount(0);
    modifier->pRadialBlurStartFloatKey = (TimedFloatKey*)modifier->RadialBlurStartInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountTenth = modifier->RadialBlurRampDownInterpolator.GetColorKeyCount(0);
    modifier->pRadialBlurRampDownFloatKey = (TimedFloatKey*)modifier->RadialBlurRampDownInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountEleventh = modifier->RadialBlurDownStartInterpolator.GetColorKeyCount(0);
    modifier->pRadialBlurDownStartFloatKey = (TimedFloatKey*)modifier->RadialBlurDownStartInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountSeventh = modifier->DepthOfFieldStrengthInterpolator.GetColorKeyCount(0);
    modifier->pDepthOfFieldStrengthFloatKey = (TimedFloatKey*)modifier->DepthOfFieldStrengthInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountEighth = modifier->DepthOfFieldDistanceInterpolator.GetColorKeyCount(0);
    modifier->pDepthOfFieldDistanceFloatKey = (TimedFloatKey*)modifier->DepthOfFieldDistanceInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountNinth = modifier->DepthOfFieldRangeInterpolator.GetColorKeyCount(0);
    modifier->pDepthOfFieldRangeFloatKey = (TimedFloatKey*)modifier->DepthOfFieldRangeInterpolator.GetChannelKeys(0);
    modifier->Data.m_uiEffectEntryCountThirteenth = modifier->MotionBlurStrengthInterpolator.GetColorKeyCount(0);
    modifier->pMotionBlurStrengthFloatKey = (TimedFloatKey*)modifier->MotionBlurStrengthInterpolator.GetChannelKeys(0);
}

bool TESForm_QEndianSwapOnSave();
void AppendChunk(unsigned int, void*, unsigned int);
void TESForm_AddChunk_00485910(unsigned int, unsigned int);
void* __memcpy(void*, const void*, unsigned int);
// Serializes the image space modifier: writes a header, then each data array (endian-swapped when needed) with its tag, plus intro/outro sound form IDs.
// @fnv 0x0052f250
// pdb: TESImageSpaceModifier::Save | TESImageSpaceModifier.obj | retail, upstream name already matches, high
// pdb locals: itypesizeM, pendiandataM, iendiansizeM, iM, i, j, chunkid, itempint
void TESImageSpaceModifier::Save()
{
    TESForm::BeginFormRecord();
    TESImageSpaceModifier_FlushData(this);
    if (TESForm_QEndianSwapOnSave())
        Data.Endian();
    AppendChunk(0x4d414e44, &Data, 0xf4);
    if (TESForm_QEndianSwapOnSave())
        Data.Endian();
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountSecond / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pBlurFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e42, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountThird / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pDoubleFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e56, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 20;
        unsigned int count = size * Data.m_uiEffectEntryCountFirst / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pTintColorKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e54, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 20;
        unsigned int count = size * Data.m_uiEffectEntryCountTwelfth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pFadeColorKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x334d414e, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountFourth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pRadialBlurStrengthFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e52, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountFifth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pRadialBlurRampupFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e53, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountSixth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pRadialBlurStartFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e55, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountTenth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pRadialBlurRampDownFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x314d414e, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountEleventh / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pRadialBlurDownStartFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x324d414e, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountSeventh / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pDepthOfFieldStrengthFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e57, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountEighth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pDepthOfFieldDistanceFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e58, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountNinth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pDepthOfFieldRangeFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x4d414e59, dataTmp, count * 4);
        delete[] dataTmp;
    }
    {
        unsigned int size = 8;
        unsigned int count = size * Data.m_uiEffectEntryCountThirteenth / 4;
        unsigned int* dataTmp = new unsigned int[count];
        __memcpy(dataTmp, pMotionBlurStrengthFloatKey, count * 4);
        if (TESForm_QEndianSwapOnSave())
            for (unsigned int idx = 0; idx < count; idx++)
                BSEndian(dataTmp + idx, EndianSizeTag<4>());
        AppendChunk(0x344d414e, dataTmp, count * 4);
        delete[] dataTmp;
    }
    for (int i = 0; i < 21; i++)
        for (int j = 0; j < 2; j++)
        {
            unsigned int tag = 0x44414900 | i | (j << 6);
            unsigned int size = 8;
            unsigned int count2 = size * Data.m_auiKeyframeCountsByTypeAndChannel[i][j] / 4;
            unsigned int* dataTmp = new unsigned int[count2];
            __memcpy(dataTmp, pEffectEntryTable[i][j], count2 * 4);
            if (TESForm_QEndianSwapOnSave())
                for (unsigned int idx = 0; idx < count2; idx++)
                    BSEndian(dataTmp + idx, EndianSizeTag<4>());
            AppendChunk(tag, dataTmp, count2 * 4);
            delete[] dataTmp;
        }
    if (pIntroSound)
    {
        unsigned int intro = pIntroSound->GetFormID();
        TESForm_AddChunk_00485910(0x44534452, intro);
    }
    if (pOutroSound)
    {
        unsigned int outro = pOutroSound->GetFormID();
        TESForm_AddChunk_00485910(0x49534452, outro);
    }
    TESForm::FinishFormRecord();
}

extern char _DAT_011862b8;
extern "C" void* __cdecl FUN_00401460(void*, const void*, unsigned int);
struct PassPtr
{
    TESImageSpaceModifier* value;
};
extern "C" void __fastcall TESForm_CopyAllComponents(TESImageSpaceModifier*, PassPtr);
// Looks up a TESImageSpaceModifier by form, clears this object's data, then copies its sounds, Data struct and all list arrays into newly allocated storage.
// @fnv 0x00530160
// pdb: TESImageSpaceModifier::Copy | TESImageSpaceModifier.obj | retail, upstream name already matches, high
// pdb params: apCopy
// pdb locals: j, pcopy, uisize, i
void TESImageSpaceModifier::Copy(TESForm* form)
{
    unsigned int count;
    TESImageSpaceModifier* source = (TESImageSpaceModifier*)FUN_00ec43fb(form, 0, &_DAT_01183028, &_DAT_011862b8, 0);
    if (!source)
        return;
    TESImageSpaceModifier_FlushData(source);
    TESForm_CopyAllComponents(this, *(PassPtr*)&source);
    ClearData();
    pIntroSound = source->pIntroSound;
    pOutroSound = source->pOutroSound;
    FUN_00401460(&Data, &source->Data, sizeof(Data));
    count = Data.m_uiEffectEntryCountSecond;
    pBlurFloatKey = new TimedFloatKey[count];
    FUN_00401460(pBlurFloatKey, source->pBlurFloatKey, count * sizeof(TimedFloatKey));
    BlurInterpolator.SetAnimationKeys(pBlurFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountThird;
    pDoubleFloatKey = new TimedFloatKey[count];
    FUN_00401460(pDoubleFloatKey, source->pDoubleFloatKey, count * sizeof(TimedFloatKey));
    DoubleInterpolator.SetAnimationKeys(pDoubleFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountFirst;
    pTintColorKey = new AlphaColor[count];
    FUN_00401460(pTintColorKey, source->pTintColorKey, count * sizeof(AlphaColor));
    TintColorInterpolator.ReplaceKeys(pTintColorKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountTwelfth;
    pFadeColorKey = new AlphaColor[count];
    FUN_00401460(pFadeColorKey, source->pFadeColorKey, count * sizeof(AlphaColor));
    FadeColorInterpolator.ReplaceKeys(pFadeColorKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountFourth;
    pRadialBlurStrengthFloatKey = new TimedFloatKey[count];
    FUN_00401460(pRadialBlurStrengthFloatKey, source->pRadialBlurStrengthFloatKey, count * sizeof(TimedFloatKey));
    RadialBlurStrengthInterpolator.SetAnimationKeys(pRadialBlurStrengthFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountFifth;
    pRadialBlurRampupFloatKey = new TimedFloatKey[count];
    FUN_00401460(pRadialBlurRampupFloatKey, source->pRadialBlurRampupFloatKey, count * sizeof(TimedFloatKey));
    RadialBlurRampupInterpolator.SetAnimationKeys(pRadialBlurRampupFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountSixth;
    pRadialBlurStartFloatKey = new TimedFloatKey[count];
    FUN_00401460(pRadialBlurStartFloatKey, source->pRadialBlurStartFloatKey, count * sizeof(TimedFloatKey));
    RadialBlurStartInterpolator.SetAnimationKeys(pRadialBlurStartFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountTenth;
    pRadialBlurRampDownFloatKey = new TimedFloatKey[count];
    FUN_00401460(pRadialBlurRampDownFloatKey, source->pRadialBlurRampDownFloatKey, count * sizeof(TimedFloatKey));
    RadialBlurRampDownInterpolator.SetAnimationKeys(pRadialBlurRampDownFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountEleventh;
    pRadialBlurDownStartFloatKey = new TimedFloatKey[count];
    FUN_00401460(pRadialBlurDownStartFloatKey, source->pRadialBlurDownStartFloatKey, count * sizeof(TimedFloatKey));
    RadialBlurDownStartInterpolator.SetAnimationKeys(pRadialBlurDownStartFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountSeventh;
    pDepthOfFieldStrengthFloatKey = new TimedFloatKey[count];
    FUN_00401460(pDepthOfFieldStrengthFloatKey, source->pDepthOfFieldStrengthFloatKey, count * sizeof(TimedFloatKey));
    DepthOfFieldStrengthInterpolator.SetAnimationKeys(pDepthOfFieldStrengthFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountEighth;
    pDepthOfFieldDistanceFloatKey = new TimedFloatKey[count];
    FUN_00401460(pDepthOfFieldDistanceFloatKey, source->pDepthOfFieldDistanceFloatKey, count * sizeof(TimedFloatKey));
    DepthOfFieldDistanceInterpolator.SetAnimationKeys(pDepthOfFieldDistanceFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountNinth;
    pDepthOfFieldRangeFloatKey = new TimedFloatKey[count];
    FUN_00401460(pDepthOfFieldRangeFloatKey, source->pDepthOfFieldRangeFloatKey, count * sizeof(TimedFloatKey));
    DepthOfFieldRangeInterpolator.SetAnimationKeys(pDepthOfFieldRangeFloatKey, count, NiAnimationKey::LINKEY);
    count = Data.m_uiEffectEntryCountThirteenth;
    pMotionBlurStrengthFloatKey = new TimedFloatKey[count];
    FUN_00401460(pMotionBlurStrengthFloatKey, source->pMotionBlurStrengthFloatKey, count * sizeof(TimedFloatKey));
    MotionBlurStrengthInterpolator.SetAnimationKeys(pMotionBlurStrengthFloatKey, count, NiAnimationKey::LINKEY);
    {
        int j;
        for (int i = 0; i < 21; ++i)
        {
            for (j = 0; j < 2; ++j)
            {
                count = Data.m_auiKeyframeCountsByTypeAndChannel[i][j];
                pEffectEntryTable[i][j] = new TimedFloatKey[count];
                FUN_00401460(pEffectEntryTable[i][j], source->pEffectEntryTable[i][j], count * sizeof(TimedFloatKey));
                m_akFloatInterpolators[i][j].SetAnimationKeys(pEffectEntryTable[i][j], count, NiAnimationKey::LINKEY);
            }
        }
    }
}

struct BlendColor
{
    float r, g, b, a;
    BlendColor(float, float, float, float);
    BlendColor FUN_00532ec0(const BlendColor&) const;
    BlendColor AddDelta(const BlendColor&) const;
};
BlendColor Object_ProcessAndReturnValue_00532f40(float, const BlendColor&);
struct BlendPoint
{
    float x, y;
    BlendPoint();
    BlendPoint(float, float);
    BlendPoint Subtract(const BlendPoint&) const;
    BlendPoint AddPointComponents(const BlendPoint&) const;
};
BlendPoint FUN_00532fd0(float, const BlendPoint&);
// Interpolates two TESImageSpaceModifiers by blend and strength and pushes the blended values into the ImageSpaceManager.
// @fnv 0x00532220
void TESImageSpaceModifier::BlendModifiers(float blend, TESImageSpaceModifier* pPreviousModifier,
                              TESImageSpaceModifier* pNextModifier, float previousStrength, float nextStrength,
                              NiAVObject* previousTarget, NiAVObject* nextTarget)
{
    if (!pPreviousModifier)
        pPreviousModifier = TESImageSpaceModifier::QDefault();
    if (!pNextModifier)
        pNextModifier = TESImageSpaceModifier::QDefault();
    float time = 0.0f;
    ImageSpaceManager* manager4 = ImageSpaceManager::BSShaderManager_GetImageSpaceManager();
    float strength_15 = previousStrength + blend * (nextStrength - previousStrength);
    float values[2];
    for (int i = 0; i < 21; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            float first, second6;
            pPreviousModifier->m_akFloatInterpolators[i][j].UpdateFloat(time, 0, first);
            pNextModifier->m_akFloatInterpolators[i][j].UpdateFloat(time, 0, second6);
            values[j] = first + blend * (second6 - first);
        }
        if (i >= 0 && i <= 13)
            manager4->ApplyParamModifier_00b8cb30(1, i, values, strength_15);
        if (i >= 14 && i <= 16)
            manager4->ApplyParamModifier_00b8cb30(0, i - 14, values, strength_15);
        if (i >= 17 && i <= 20)
            manager4->ApplyParamModifier_00b8cb30(3, i - 17, values, strength_15);
    }
    BlendColor color(0, 0, 0, 0), aFirstColor(0, 0, 0, 0), secondColor(0, 0, 0, 0);
    pPreviousModifier->TintColorInterpolator.UpdateColorA(time, 0, (NiColorA&)aFirstColor);
    pNextModifier->TintColorInterpolator.UpdateColorA(time, 0, (NiColorA&)secondColor);
    color = aFirstColor.AddDelta(Object_ProcessAndReturnValue_00532f40(blend, secondColor.FUN_00532ec0(aFirstColor)));
    color.a *= strength_15;
    BSClamp_float(color.a, 0.0f, 1.0f);
    manager4->FUN_00b8b540((NiColorA&)color);
    pPreviousModifier->FadeColorInterpolator.UpdateColorA(time, 0, (NiColorA&)aFirstColor);
    pNextModifier->FadeColorInterpolator.UpdateColorA(time, 0, (NiColorA&)secondColor);
    color = aFirstColor.AddDelta(Object_ProcessAndReturnValue_00532f40(blend, secondColor.FUN_00532ec0(aFirstColor)));
    color.a *= strength_15;
    BSClamp_float(color.a, 0.0f, 1.0f);
    manager4->FUN_00b8b610((NiColorA&)color);
    float firstValue, secondValue3;
    pPreviousModifier->BlurInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->BlurInterpolator.UpdateFloat(time, 0, secondValue3);
    float blur2 = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->DoubleInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->DoubleInterpolator.UpdateFloat(time, 0, secondValue3);
    float doubleVision = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->RadialBlurStrengthInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->RadialBlurStrengthInterpolator.UpdateFloat(time, 0, secondValue3);
    float kRadialStrength = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->RadialBlurRampupInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->RadialBlurRampupInterpolator.UpdateFloat(time, 0, secondValue3);
    float radialRampUp_19 = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->RadialBlurStartInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->RadialBlurStartInterpolator.UpdateFloat(time, 0, secondValue3);
    float radialStart_8 = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->RadialBlurRampDownInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->RadialBlurRampDownInterpolator.UpdateFloat(time, 0, secondValue3);
    float radialRampDown_15 = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->RadialBlurDownStartInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->RadialBlurDownStartInterpolator.UpdateFloat(time, 0, secondValue3);
    float radialDownStart_3 = firstValue + blend * (secondValue3 - firstValue);
    pPreviousModifier->DepthOfFieldStrengthInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->DepthOfFieldStrengthInterpolator.UpdateFloat(time, 0, secondValue3);
    float depthStrength_4 = firstValue + blend * (secondValue3 - firstValue);
    float firstDepthDistance_, secondDepthDistance, firstDepthRange_2, secondDepthRange_12;
    pPreviousModifier->DepthOfFieldDistanceInterpolator.UpdateFloat(time, 0, firstDepthDistance_);
    pNextModifier->DepthOfFieldDistanceInterpolator.UpdateFloat(time, 0, secondDepthDistance);
    pPreviousModifier->DepthOfFieldRangeInterpolator.UpdateFloat(time, 0, firstDepthRange_2);
    pNextModifier->DepthOfFieldRangeInterpolator.UpdateFloat(time, 0, secondDepthRange_12);
    pPreviousModifier->MotionBlurStrengthInterpolator.UpdateFloat(time, 0, firstValue);
    pNextModifier->MotionBlurStrengthInterpolator.UpdateFloat(time, 0, secondValue3);
    float motion_24 = firstValue + blend * (secondValue3 - firstValue);
    blur2 *= strength_15;
    BSClamp_float(blur2, 0.0f, 7.0f);
    manager4->RaiseHighestBlurRadius(blur2);
    manager4->ApplyDoubleVisionParamModifier_00b8afb0(doubleVision * strength_15);
    BlendPoint aFirstCenter(0.5f, 0.5f), secondCenter5(0.5f, 0.5f), center7;
    if (pPreviousModifier->Data.m_bUseRadialBlur && previousTarget)
    {
        previousTarget->UpdateWorldBound();
        manager4->GetRadialBlurCenterFromWorldBound((void*)&previousTarget->NiAVObject_GetWorldBound(), (NiPoint2*)&aFirstCenter);
    }
    if (pNextModifier->Data.m_bUseRadialBlur && nextTarget)
    {
        previousTarget->UpdateWorldBound();
        manager4->GetRadialBlurCenterFromWorldBound((void*)&nextTarget->NiAVObject_GetWorldBound(), (NiPoint2*)&secondCenter5);
    }
    if (!pPreviousModifier->Data.m_bUseRadialBlur)
        aFirstCenter = (const BlendPoint&)pPreviousModifier->Data.m_kRadialBlurCenter;
    if (!pNextModifier->Data.m_bUseRadialBlur)
        secondCenter5 = (const BlendPoint&)pNextModifier->Data.m_kRadialBlurCenter;
    center7 = aFirstCenter.AddPointComponents(FUN_00532fd0(blend, secondCenter5.Subtract(aFirstCenter)));
    manager4->SetRadialBlurParameters(kRadialStrength * strength_15, radialRampUp_19, radialStart_8, radialRampDown_15,
                                      radialDownStart_3, (const NiPoint2&)center7);
    BlendPoint firstDepthB(0, 0), secondDepth(0, 0), depth5;
    if (pPreviousModifier->Data.m_bUseEffectEntries && previousTarget)
    {
        previousTarget->UpdateWorldBound();
        manager4->GetDepthOfFieldParamsFromWorldBound((void*)&previousTarget->NiAVObject_GetWorldBound(), (NiPoint2*)&firstDepthB);
    }
    if (pNextModifier->Data.m_bUseEffectEntries && nextTarget)
    {
        nextTarget->UpdateWorldBound();
        manager4->GetDepthOfFieldParamsFromWorldBound((void*)&nextTarget->NiAVObject_GetWorldBound(), (NiPoint2*)&secondDepth);
    }
    if (!pPreviousModifier->Data.m_bUseEffectEntries)
        firstDepthB = BlendPoint(firstDepthDistance_, firstDepthRange_2);
    if (!pNextModifier->Data.m_bUseEffectEntries)
        secondDepth = BlendPoint(secondDepthDistance, secondDepthRange_12);
    depth5 = firstDepthB.AddPointComponents(FUN_00532fd0(blend, secondDepth.Subtract(firstDepthB)));
    manager4->SetDepthOfFieldParameters(depthStrength_4 * strength_15,
                                        blend < 0.5 ? pPreviousModifier->Data.m_ucEffectMode
                                                    : pNextModifier->Data.m_ucEffectMode,
                                        0.0f, depth5.x, depth5.y);
    manager4->RaiseBlurRadius(motion_24);
}
