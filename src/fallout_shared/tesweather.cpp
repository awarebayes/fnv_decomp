// fallout_shared/tesweather.cpp: TESWeather, TESWeather__WeatherSoundList, TESWeather__WeatherSound, WeatherSave and more (13 functions)

#include "layout_support.h"
#include "../../include/FileEntryList.h"
#include "../../include/TESModel.h"
#include "../../include/TESTexture1024.h"
#include "../../include/TESWeather__WeatherSoundList.h"
#include "../../include/TESWeather.h"
#include "tesweather_decls.h"

// Helpers not yet declared by the shared image-space component.
class WeatherImageSpaceFunctions : public TESImageSpaceModifiableCountForm<6>
{
  public:
    void TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(unsigned int index, unsigned int formID);
    void TESImageSpaceModifiableCountForm_6_Save();
};

// @fnv 0x005807f0
// pdb: TESWeather::TESWeather | TESWeather.obj | retail, upstream name already matches, high
TESWeather::TESWeather()
{
    SetFormType(static_cast<ENUM_FORM_ID>(0x35));
    ResetFormData();
}

// @fnv 0x00581b40
// pdb: TESWeather::InitializeData | TESWeather.obj | retail, vtable slot, high
void TESWeather::ResetFormData()
{
    iNumCloudLayers = 3;
    memset(uiColorData, 0, sizeof(uiColorData));
    memset(ucWeatherData, 0, sizeof(ucWeatherData));
    memset(fFogData, 0, sizeof(fFogData));
    fFogData[4] = 1.0F;
    fFogData[5] = 1.0F;
    memset(ucCloudLayerSpeed, 0, sizeof(ucCloudLayerSpeed));
    memset(uiCloudColorData, 0, sizeof(uiCloudColorData));
}

// @fnv 0x00581b00
// pdb: TESWeather::InitItem | TESWeather.obj | retail, upstream name already matches, high
void TESWeather::InitItem()
{
    if (IsFormInitialized())
        return;
    TESImageSpaceModifiableCountForm<6>::InitItem(this);
    SetInitializationState(true);
}

// @fnv 0x00581be0
// pdb: TESWeather::WeatherSoundList::FlushList | TESWeather.obj | retail, call graph, low
// pdb note: debug build links it to TESWeather::ClearData
// pdb locals: pCurrentNode
void TESWeather::ClearData()
{
    soundList.ClearWeatherSounds();
}

// @fnv 0x00581c00
// pdb: TESWeather::MakeDefault | TESWeather.obj | retail, string-set bridge, high
void TESWeather::MakeDefault()
{
    iNumCloudLayers = 3;
    cloudTexture[0].SetTexture("Sky\\WastelandCloudCloudyUpper01.dds");
    cloudTexture[1].SetTexture("Sky\\WastelandCloudHorizon01.dds");
    cloudTexture[2].SetTexture("Sky\\WastelandCloudCloudyLower01.dds");
    ucCloudLayerSpeed[0] = 0x33;
    uiCloudColorData[0][0] = 0x5B4B46;
    uiCloudColorData[0][1] = 0x9B9B9B;
    uiCloudColorData[0][2] = 0x6C5953;
    uiCloudColorData[0][3] = 0x1C140F;
    ucWeatherData[0] = 0x33;
    ucWeatherData[3] = 0xFF;
    ucWeatherData[4] = 0xFF;
    ucWeatherData[5] = 0xFF;
    ucWeatherData[6] = 0x00;
    ucWeatherData[7] = 0xFF;
    ucWeatherData[8] = 0x00;
    ucWeatherData[9] = 0xFF;
    ucWeatherData[10] = 0x32;
    ucWeatherData[11] = 0x00;
    fFogData[2] = 27852.8F;
    fFogData[0] = 27852.8F;
    fFogData[3] = 163840.0F;
    fFogData[1] = 163840.0F;
    fFogData[5] = 1.0F;
    fFogData[4] = 1.0F;
    uiColorData[7][0] = 0x846C66;
    uiColorData[0][0] = 0x846C66;
    uiColorData[7][1] = 0x71533E;
    uiColorData[0][1] = 0x71533E;
    uiColorData[7][4] = 0x71533E;
    uiColorData[0][4] = 0x71533E;
    uiColorData[7][2] = 0x846D66;
    uiColorData[0][2] = 0x846D66;
    uiColorData[7][3] = 0x100A05;
    uiColorData[0][3] = 0x100A05;
    uiColorData[7][5] = 0x100A05;
    uiColorData[0][5] = 0x100A05;
    uiColorData[8][0] = 0x5F6DB6;
    uiColorData[1][0] = 0x5F6DB6;
    uiColorData[8][1] = 0xD19A83;
    uiColorData[1][1] = 0xD19A83;
    uiColorData[8][4] = 0xD19A83;
    uiColorData[1][4] = 0xD19A83;
    uiColorData[8][2] = 0x5782A8;
    uiColorData[1][2] = 0x5782A8;
    uiColorData[8][3] = 0x261B0F;
    uiColorData[1][3] = 0x261B0F;
    uiColorData[8][5] = 0x261B0F;
    uiColorData[1][5] = 0x261B0F;
    uiColorData[3][0] = 0x252212;
    uiColorData[3][1] = 0x857065;
    uiColorData[3][4] = 0x857065;
    uiColorData[3][2] = 0x35221A;
    uiColorData[3][3] = 0x281C15;
    uiColorData[3][5] = 0x281C15;
    uiColorData[4][0] = 0x79A5FF;
    uiColorData[4][1] = 0xCAEDFF;
    uiColorData[4][4] = 0xCAEDFF;
    uiColorData[4][2] = 0x499AE2;
    uiColorData[4][3] = 0x764D34;
    uiColorData[4][5] = 0x764D34;
    uiColorData[5][0] = 0x779FF2;
    uiColorData[5][1] = 0xBED9DE;
    uiColorData[5][4] = 0xBED9DE;
    uiColorData[5][2] = 0x9DBDFF;
    uiColorData[5][3] = 0x000000;
    uiColorData[5][5] = 0x000000;
}

// Deletes each TESWeather__WeatherSound in the list, then clears the list with RemoveAll.
// @fnv 0x00581f50
// pdb: TESWeather::WeatherSoundList::FlushList | TESWeather.obj | debug, object-file run extension, low
// pdb locals: pCurrentNode
void TESWeather__WeatherSoundList::ClearWeatherSounds()
{
    BSSimpleList<TESWeather__WeatherSound*>* iter = this;
    while (iter && iter->GetItem())
    {
        TESWeather__WeatherSound* sound = iter->GetItem();
        ::operator delete(sound);
        iter = iter->GetNext();
    }
    RemoveAll();
}
// @fnv 0x00580990
// pdb: TESWeather::Load | TESWeather.obj | retail, vtable slot, high
// pdb params: apFile
// pdb locals: chunkid, formid, pMessage, uitemp, uiCloudColorDataTemp, i, j, uiColorDataTemp, idataCount, pWeatherSound
bool TESWeather::LoadForm(TESFile* apFile)
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x35)
        return false;
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    unsigned int uiChunkID;
    while (uiChunkID = apFile->GetChunkID())
    {
        switch (uiChunkID)
        {
        case 0x44494445:  // EDID
        {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 512);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:  // OBND
            readOBNDSubRecord(apFile);
            break;
        case 0x44414900: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(0, formID);
            break;
        }
        case 0x44414901:
        case 0x4d414e4d:  // MNAM
        {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(1, formID);
            break;
        }
        case 0x44414902: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(2, formID);
            break;
        }
        case 0x44414903: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(3, formID);
            break;
        }
        case 0x44414904: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(4, formID);
            break;
        }
        case 0x44414905: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(5, formID);
            break;
        }
        case 0x44414906: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_SetFormImageSpaceModifying(6, formID);
            break;
        }
        case 0x4d414e43:  // CNAM
            TESTexture::ReadTextureData(&cloudTexture[1], apFile);
            break;
        case 0x4d414e44:  // DNAM
            TESTexture::ReadTextureData(&cloudTexture[0], apFile);
            break;
        case 0x4d414e41:  // ANAM
            TESTexture::ReadTextureData(&cloudTexture[2], apFile);
            break;
        case 0x4d414e42:  // BNAM
            TESTexture::ReadTextureData(&cloudTexture[3], apFile);
            break;
        case 0x4d414e45:  // ENAM (legacy fifth texture)
            TESTexture::ReadTextureData(&cloudTexture[4], apFile);
            break;
            // Retained empty legacy branches present in the original loader.
            break;
            break;
            break;
        case 0x4d414e4f:  // ONAM
            apFile->GetChunkData(ucCloudLayerSpeed, sizeof(ucCloudLayerSpeed));
            break;
        case 0x4d414e50:  // PNAM
            if (apFile->GetChunkSize() == sizeof(uiCloudColorData))
                apFile->GetChunkData(uiCloudColorData, sizeof(uiCloudColorData));
            else if (apFile->GetChunkSize() == 64)
            {
                unsigned int oldCloudColors[4][4];
                apFile->GetChunkData(oldCloudColors, sizeof(oldCloudColors));
                for (int layer = 0; layer < 4; ++layer)
                    for (int time = 0; time < 4; ++time)
                        uiCloudColorData[layer][time] = oldCloudColors[layer][time];
            }
            if (apFile->IsEndianConversionRequired())
                for (int layer = 0; layer < 4; ++layer)
                    for (int time = 0; time < 6; ++time)
                        BSEndian(&uiCloudColorData[layer][time], EndianSizeTag<4>());
            break;
        case 0x4c444f4d:  // MODL
            TESModel::LoadModelData(&precipNIF, apFile);
            break;
        case 0x304d414e:  // NAM0
            if (apFile->GetChunkSize() == sizeof(uiColorData))
                apFile->GetChunkData(uiColorData, sizeof(uiColorData));
            else if (apFile->GetChunkSize() == 160)
            {
                unsigned int oldColors[10][4];
                apFile->GetChunkData(oldColors, sizeof(oldColors));
                for (int color = 0; color < 10; ++color)
                    for (int time = 0; time < 4; ++time)
                        uiColorData[color][time] = oldColors[color][time];
            }
            if (apFile->IsEndianConversionRequired())
                for (int color = 0; color < 10; ++color)
                    for (int time = 0; time < 6; ++time)
                        BSEndian(&uiColorData[color][time], EndianSizeTag<4>());
            break;
        case 0x41544144:  // DATA
            if (apFile->GetChunkSize() == sizeof(ucWeatherData))
                apFile->GetChunkData(ucWeatherData, sizeof(ucWeatherData));
            else if (apFile->GetChunkSize() == 12)
            {
                apFile->GetChunkData(ucWeatherData, 12);
                memset(&ucWeatherData[12], 255, 3);
            }
            break;
        case 0x4d414e46:  // FNAM
        {
            int count = 6;
            if (apFile->GetChunkSize() != sizeof(fFogData))
                count -= 2;
            apFile->GetChunkData(fFogData, sizeof(fFogData));
            if (apFile->IsEndianConversionRequired())
                for (int i = 0; i < count; ++i)
                    BSEndian(&fFogData[i], EndianSizeTag<4>());
            break;
        }
        case 0x4d414e48:  // HNAM
            apFile->GetChunkData(fHDRData, sizeof(fHDRData));
            if (apFile->IsEndianConversionRequired())
                for (int i = 0; i < 14; ++i)
                    BSEndian(&fHDRData[i], EndianSizeTag<4>());
            break;
        case 0x4d414e49:  // INAM
            apFile->GetChunkData(pImageSpaceParameterMods, sizeof(pImageSpaceParameterMods));
            if (apFile->IsEndianConversionRequired())
                for (int i = 0; i < 2; ++i)
                    pImageSpaceParameterMods[i].Endian();
            break;
        case 0x4d414e4c:  // LNAM
            apFile->GetChunkData(&iNumCloudLayers, sizeof(iNumCloudLayers));
            if (apFile->IsEndianConversionRequired())
                BSEndian(&iNumCloudLayers, EndianSizeTag<4>());
            break;
        case 0x4d414e53:  // SNAM
        {
            TESWeather__WeatherSound* sound = new TESWeather__WeatherSound;
            apFile->GetChunkData(sound, sizeof(TESWeather__WeatherSound));
            if (apFile->IsEndianConversionRequired())
                sound->TwoValues_Endian();
            RemapFormIDForFile(sound->formID, apFile);
            soundList.AddTail(sound);
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// @fnv 0x005813b0
// pdb: TESWeather::WeatherSound::WeatherSound | TESWeather.obj | debug, object-file run extension, low
TESWeather__WeatherSound::TESWeather__WeatherSound()
{
    memset(this, 0, sizeof(*this));
}

// @fnv 0x005808e0
// pdb: TESWeather::~TESWeather | TESWeather.obj | retail, upstream name already matches, high
TESWeather::~TESWeather()
{
    ClearData();
}

void TESForm_AddChunkArray(unsigned int, void*, unsigned int);
void TESForm_AddChunkArray_00485710(unsigned int, unsigned int*, unsigned int);
void TESForm_AddChunk_00485910(unsigned int, int);
void AppendChunk(unsigned int, void*, unsigned int);
bool TESForm_QEndianSwapOnSave();
class WeatherSave : public TESWeather
{
  public:
    void TESWeather_Save();
};
// @fnv 0x005813d0
// src: XBOX_PDB 0x005813d0
// pdb: TESWeather::Save | TESWeather.obj | retail, upstream name already matches, high
// pdb locals: pitextureID, i, psl
void WeatherSave::TESWeather_Save()
{
    TESForm::BeginFormRecord();
    reinterpret_cast<WeatherImageSpaceFunctions*>(reinterpret_cast<char*>(this) + sizeof(TESForm))->TESImageSpaceModifiableCountForm_6_Save();
    unsigned int auiChunkIds[8] = {0x4d414e44, 0x4d414e43, 0x4d414e41, 0x4d414e42,
                                   0x4d414e45, 0x4d414e47, 0x4d414e4a, 0x4d414e4b};
    for (int i = 0; i < iNumCloudLayers; ++i)
        cloudTexture[i].Save(auiChunkIds[i]);
    precipNIF.WriteToBuffer(0x4c444f4d, 0x54444f4d);
    TESForm_AddChunk_00485910(0x4d414e4c, iNumCloudLayers);
    TESForm_AddChunkArray(0x4d414e4f, ucCloudLayerSpeed, 4);
    TESForm_AddChunkArray_00485710(0x4d414e50, &uiCloudColorData[0][0], 24);
    TESForm_AddChunkArray_00485710(0x304d414e, &uiColorData[0][0], 60);
    TESForm_AddChunkArray_00485710(0x4d414e46, reinterpret_cast<unsigned int*>(fFogData), 6);
    if (TESForm_QEndianSwapOnSave())
    {
        for (unsigned int j = 0; j < 2; ++j)
            pImageSpaceParameterMods[j].Endian();
    }
    AppendChunk(0x4d414e49, pImageSpaceParameterMods, 304);
    if (TESForm_QEndianSwapOnSave())
    {
        for (unsigned int k = 0; k < 2; ++k)
            pImageSpaceParameterMods[k].Endian();
    }
    TESForm_AddChunkArray(0x41544144, ucWeatherData, 15);
    for (BSSimpleList<TESWeather__WeatherSound*>* node = &soundList; node && node->GetItem(); node = node->GetNext())
    {
        if (TESForm_QEndianSwapOnSave())
            node->GetItem()->TwoValues_Endian();
        AppendChunk(0x4d414e53, node->GetItem(), 8);
        if (TESForm_QEndianSwapOnSave())
            node->GetItem()->TwoValues_Endian();
    }
    TESForm::FinishFormRecord();
}

extern "C" int memcmp(const void* buf1, const void* buf2, size_t count);

class TESRegionData;

#include "../../include/BSSimpleList.h"

class FUN_005e79a0_t
{
  public:
    bool TESForm_CompareAllComponents(TESForm* apCompare);
};

class WeatherEq : public TESWeather
{
  public:
    bool IsDifferentFrom(TESForm* apCompare);
};

// @fnv 0x00581880
// pdb: TESWeather::Compare | TESWeather.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp, pThisSL, pCompSL, i
bool WeatherEq::IsDifferentFrom(TESForm* apCompare)
{
    TESWeather* other = dynamic_cast<TESWeather*>(apCompare);
    if (!other)
        return 1;
    BSSimpleList<TESRegionData*>* otherList3 = (BSSimpleList<TESRegionData*>*)&other->soundList;
    BSSimpleList<TESRegionData*>* thisList2 = (BSSimpleList<TESRegionData*>*)&this->soundList;
    if (otherList3->ItemsInList() == thisList2->ItemsInList())
    {
        while (otherList3 != 0 && thisList2 != 0 && otherList3->GetItem() != 0 && thisList2->GetItem() != 0)
        {
            if (memcmp(otherList3->GetItem(), thisList2->GetItem(), 8) != 0)
                return 1;
            otherList3 = otherList3->GetNext();
            thisList2 = thisList2->GetNext();
        }
    }
    else
    {
        return 1;
    }
    if (this->iNumCloudLayers == other->iNumCloudLayers)
    {
        for (int i3 = 0; i3 < this->iNumCloudLayers; ++i3)
        {
            if (cloudTexture[i3].CompareWithBase(&other->cloudTexture[i3]))
                return 1;
        }
    }
    else
    {
        return 1;
    }
    return ((FUN_005e79a0_t*)this)->TESForm_CompareAllComponents(other) || precipNIF.CompareWithBase(static_cast<BaseFormComponent*>(&other->precipNIF)) ||
           memcmp(this->ucWeatherData, other->ucWeatherData, 15) != 0 ||
           memcmp(this->uiColorData, other->uiColorData, 240) != 0 ||
           memcmp(this->fFogData, other->fFogData, 24) != 0 ||
           memcmp(this->ucCloudLayerSpeed, other->ucCloudLayerSpeed, 4) != 0 ||
           memcmp(this->uiCloudColorData, other->uiCloudColorData, 96) != 0 ||
           memcmp(this->pImageSpaceParameterMods, other->pImageSpaceParameterMods, 304) != 0;
}

#include <string.h>
struct FUN_004b0680
{
    unsigned int formID;
    unsigned int type;
    FUN_004b0680(unsigned int formID, unsigned int type);
};

class WeatherCopy : public TESWeather
{
  public:
    void TESForm_CopyAllComponents(TESForm* source);
    void TESWeather_Copy(TESForm* apCopy);
};

// Copies weather data from a source TESWeather into this WeatherCopy, including cloud layers, model, flags, colors, fog, image space modifiers, and sound list.
// @fnv 0x00581630
// src: XBOX_PDB 0x00581630
// pdb: TESWeather::Copy | TESWeather.obj | retail, upstream name already matches, high
// pdb params: apCopy
// pdb locals: pcopy, i, psl
void WeatherCopy::TESWeather_Copy(TESForm* apCopy)
{
    TESWeather* weather = dynamic_cast<TESWeather*>(apCopy);
    if (!weather)
        return;
    TESForm_CopyAllComponents(weather);
    iNumCloudLayers = weather->iNumCloudLayers;
    for (int i = 0; i < iNumCloudLayers; ++i)
        cloudTexture[i].CopyFromBase(&weather->cloudTexture[i]);
    precipNIF.CopyFromBase(static_cast<BaseFormComponent*>(&weather->precipNIF));
    memcpy(ucCloudLayerSpeed, weather->ucCloudLayerSpeed, 4);
    memcpy(uiCloudColorData, weather->uiCloudColorData, 96);
    memcpy(ucWeatherData, weather->ucWeatherData, 15);
    memcpy(uiColorData, weather->uiColorData, 240);
    memcpy(fFogData, weather->fFogData, 24);
    memcpy(pImageSpaceParameterMods, weather->pImageSpaceParameterMods, 304);
    soundList.RemoveAll();
    for (BSSimpleList<TESWeather__WeatherSound*>* node = &weather->soundList; node && node->GetItem();
         node = node->GetNext())
    {
        TESWeather__WeatherSound* sound = reinterpret_cast<TESWeather__WeatherSound*>(
            new FUN_004b0680(node->GetItem()->formID, node->GetItem()->type));
        soundList.AddTail(sound);
    }
}
