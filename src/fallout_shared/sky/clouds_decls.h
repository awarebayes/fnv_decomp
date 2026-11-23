// Declarations private to clouds.cpp.
#ifndef FNV_CLOUDS_DECLS_H
#define FNV_CLOUDS_DECLS_H

class NiTexture;

#include "../../../include/BSStringT.h"
#include "../../../include/BSShaderManager.h"
#include "../../../include/NiPointer.h"
#include "../../../include/BSStream.h"
#include "../../../include/Clouds.h"
#include "../../../include/NiColor.h"
#include "../../../include/NiGeometry.h"
#include "../../../include/NiNode.h"
#include "../../../include/NiObjectNET.h"
#include "../../../include/NiProperty.h"
#include "../../../include/NiStream.h"
#include "../../../include/NiTimeController.h"
#include "../../../include/NiColorA.h"
#include "../../../include/SkyObject.h"
#include "../../../include/SkyShaderProperty.h"
#include "../../../include/SkyTextureManager.h"
#include "../../../include/TESTexture1024.h"
#include "../../../include/TESWeather.h"

class CloudsColorController : public NiTimeController
{
  public:
    unsigned char layoutPadding[0x60 - sizeof(NiTimeController)];
    NiColorA color;
};
#include "../../../include/SETTING_VALUE.h"
#include "../../../include/SettingT_GameSettingCollection_.h"
extern SettingT_GameSettingCollection_ DAT_011cca6c;
struct NiRTTI;
extern NiRTTI SkyShaderProperty_ms_RTTI; // src: XBOX_RETAIL_PDB 0x012005b8 (old: DAT_012005b8/g_012005b8)
SkyShaderProperty* NiObject_DynamicCast(const NiRTTI*, NiProperty*);
void AddToFloatArray(unsigned short, float);
// pdb: class Model, size 0x10, 4 members
class Model
{
  public:
    char* pFilename;
    int iRefCount, iManualRefCount; // src: XBOX_RETAIL_PDB iRefCount +0x4 (old: BackgroundTask); iManualRefCount +0x8 (old: m_iActualRefCount)
    NiPointer<NiNode> spObject3D; // src: XBOX_RETAIL_PDB +0xc (old: m_spNode)
    Model(const char*, BSStream*, bool, bool);
    ~Model();
    NiNode* GetObject3D();
};
typedef char clouds_stream_size[1];
typedef char clouds_model_size[1];
#endif
