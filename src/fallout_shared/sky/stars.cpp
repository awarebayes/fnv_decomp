#include "../../../include/SettingT.h"
// fallout_shared/sky/stars.cpp: Stars (7 functions)

#include "../../../include/BSStringT.h"
#include "../../../include/SkyObject.h"
#include "../../../include/SkyShaderProperty.h"
#include "../../../include/Stars.h"
#include "../../../include/NiAVObject.h"
#include "../../../include/NiGeometry.h"
#include "../../../include/NiNode.h"
#include "../../../include/TESWaterForm.h"
#include "../../../include/BSAudio.h"
#include "../../../include/NiPoint3.h"
#include "../../../include/BSStream.h"
#include "../../../include/NiQuaternion.h"
#include "../../../include/NiMatrix3.h"
#include "../../../include/Sky.h"
#include "../../../include/SETTING_VALUE.h"
#include "../../../include/BGSTerrainManager.h"
// @fnv 0x0063fd90
// pdb: Stars::Initialize | Stars.obj | debug, upstream name already matches, high
// pdb params: apSkyRoot
void Stars::Initialize(NiNode* apSkyRoot)
{
    SkyObject::Initialize(apSkyRoot);
}


// @fnv 0x0043fa80
// src: XBOX_PDB 0x0043fa80
// pdb: NiPointer<NiGeometryData>::~NiPointer<NiGeometryData> | ModelLoader.obj | debug, call graph, medium
void NiAVObject::SetRotate_0043fa80(const NiMatrix3& kRot)
{
    m_kLocal.m_Rotate = kRot;
}

struct blob16
{
    char b[16];
};
typedef char _sizecheck_NiQuaternion[1];

// @fnv 0x004f0160
// src: COMMUNITY 0x004f0160
// pdb: NiAVObject::SetRotate | BSBipNode.obj | debug, object-file gap fill, medium
// pdb params: kQuat
void NiAVObject::NiNode_SetRotate(const NiQuaternion& rotation)
{
    const_cast<NiQuaternion&>(rotation).ToRotationMatrix(&m_kLocal.m_Rotate);
}

class GameSettingCollection;
namespace fnv_00640020 {
struct blob12
{
    char b[12];
};
struct blob16
{
    char b[16];
};
typedef char _sizecheck_NiPoint3[1];

using ::SettingT;
class GlobalObject_0040e0b0 : public SettingT<GameSettingCollection>
{
};
extern SettingT<GameSettingCollection> originalRuntimeFloatSetting79;
extern GlobalObject_0040e0b0 g_011ccdc4;
extern GlobalObject_0040e0b0 g_011ccddc;
extern GlobalObject_0040e0b0 g_011cce30;
class Calendar
{
  public:
    unsigned int GetDaysPassed();
};
extern Calendar g_calendar;
float __cdecl fmodf(float, float);

}  // namespace fnv_00640020

// Updates the stars node's fade alpha from the sky hour, recolors star geometry, and rotates the stars node by the elapsed game time.
// @fnv 0x00640020
// pdb: Stars::Update | Stars.obj | retail, upstream name already matches, high
// pdb params: apSky, afSecondsPassed
// pdb locals: fhour, fSunsetBegin, fBeginFadeIn, fSunriseBegin, fSunsetEnd, fEndFadeOut, fSunriseEnd, icount, colorThisFrame, pchild, felapsed, axis, fangle, pstarsgeo, i, pStarsProp
void Stars::Update(Sky* apSky, float afSecondsPassed)
{
    using namespace fnv_00640020;
    if (!apSky->GetAudioBackendData())
        return;
    float fhour = apSky->GetFieldEC();
    fAlpha = 0.0f;
    static NiPoint3 lastColor(0.0f, 0.0f, 0.0f);
    SkyObject::Update(apSky, afSecondsPassed);
    if (apSky->Sky_GetMode() == 3 || apSky->Sky_GetMode() == 2)
    {
        if (!((NiAVObject*)(NiNode*)spRoot)->GetDisplayObject())
        {
            ((NiAVObject*)(NiNode*)spRoot)->SetAppCulled(true);
            return;
        }
        float fSunsetBegin = apSky->Sky_GetSunsetBegin();
        float fSunsetEnd = apSky->GetEndSunsetColors();
        float fSunriseEnd = apSky->Sky_GetSunriseEnd();
        float fSunriseBegin = apSky->GetBeginSunriseColors();
        float fBeginFadeIn = fSunsetEnd - (fSunsetEnd - fSunsetBegin) * 0.5f;
        float fEndFadeOut = fSunriseEnd - (fSunriseEnd - fSunriseBegin) * 0.5f;
        if (fSunriseBegin < fhour && fhour < fEndFadeOut)
            fAlpha = (fEndFadeOut - fhour) / (fEndFadeOut - fSunriseBegin);
        else if (fEndFadeOut <= fhour && fhour <= fBeginFadeIn)
            fAlpha = 0.0f;
        else if (fBeginFadeIn < fhour && fhour < fSunsetEnd)
            fAlpha = (fhour - fBeginFadeIn) / (fSunsetEnd - fBeginFadeIn);
        else
            fAlpha = 1.0f;
        if (fAlpha > 0.0)
        {
            ((NiAVObject*)(NiNode*)spRoot)->SetAppCulled(false);
            NiPoint3 colorThisFrame = *(NiPoint3*)((TESWaterForm*)apSky)->Sky_GetStarsColor();
            NiGeometry* pstarsgeo = 0;
            NiAVObject* pchild = 0;
            unsigned int icount = (NiNode*)spStars ? ((NiNode*)spStars)->NiNode_GetChildCount() : 0;
            for (unsigned int i = 0; i < icount; i++)
            {
                pchild = ((NiNode*)spStars)->GetAt(i);
                if (pchild)
                {
                    pstarsgeo = pchild->IsGeometry();
                    if (pstarsgeo)
                    {
                        SkyShaderProperty* pStarsProp =
                            (pstarsgeo->GetProperty(3) == 0
                                 ? 0
                                 : ((BGSTerrainManager*)pstarsgeo->GetProperty(3))->GetMinLevel() == 13)
                                ? (SkyShaderProperty*)pstarsgeo->GetProperty(3)
                                : 0;
                        if (pStarsProp)
                        {
                            if (lastColor != colorThisFrame)
                            {
                                *(float*)(pStarsProp->propertyData + 0x60) = colorThisFrame.x;
                                *(float*)(pStarsProp->propertyData + 0x64) = colorThisFrame.y;
                                *(float*)(pStarsProp->propertyData + 0x68) = colorThisFrame.z;
                            }
                            *(float*)(pStarsProp->propertyData + 0x6c) = fAlpha;
                        }
                    }
                }
            }
            lastColor = colorThisFrame;
            float felapsed = g_calendar.GetDaysPassed() + apSky->GetFieldEC() / 24.0;
            float fangle = fmodf(felapsed, originalRuntimeFloatSetting79.Float()[0]) * 6.2831854820251465 /
                           originalRuntimeFloatSetting79.Float()[0];
            NiPoint3 axis(g_011ccdc4.Float()[0], g_011cce30.Float()[0], g_011ccddc.Float()[0]);
            axis.ComputeMagnitude();
            if ((NiNode*)spStars)
                ((NiNode*)spStars)->SetLocalRotation(fangle, axis.x, axis.y, axis.z);
        }
        else
        {
            ((NiAVObject*)(NiNode*)spRoot)->SetAppCulled(true);
        }
    }
}
// @fnv 0x0063fd20
// pdb: Stars::~Stars | Stars.obj | retail, upstream name already matches, high
Stars::~Stars()
{
    spStars = 0;
}

// Minimal interfaces used by the stars model loader.
// pdb: class Model, size 0x10, 4 members
class Model
{
  public:
    unsigned int pFilename, iRefCount, iManualRefCount; // src: XBOX_RETAIL_PDB pFilename +0x0 (old: unk00); iRefCount +0x4 (old: unk04); iManualRefCount +0x8 (old: unk08)
    NiNode* spObject3D; // src: XBOX_RETAIL_PDB +0xc (old: object)
    Model(const char*, BSStream*, bool, bool);
    ~Model();
    NiNode* GetObject3D();
};
extern const char* const DAT_011f4428;
bool __cdecl NiObject_IsKindOf(const void*, NiNode*);
void __cdecl FNV_Warning(const char*);
// @fnv 0x0063fdb0
// src: XBOX_PDB 0x0063fdb0
// pdb: Stars::LoadGeometry | Stars.obj | retail, upstream name already matches, high
// pdb params: apFilename
// pdb locals: model, stream, strTemp
void Stars::LoadGeometry(const char* apFilename)
{
    if (!apFilename || !*apFilename)
    {
        spRoot->NiAVObject_SetDisplayObject(false);
        return;
    }
    BSStringT<char> fileName;
    fileName.PrintfString("%s%s%s", "Meshes", "\\", apFilename);
    {
        BSStream stream;
        if (!stream.BSStream_Load(fileName, false) || stream.NiStream_GetObjectCount() != 1 ||
            !NiObject_IsKindOf(&DAT_011f4428, stream.NiStream_GetObjectAt(0)))
        {
            FNV_Warning("DEFAULT: Cannot load the stars.");
            spRoot->NiAVObject_SetDisplayObject(false);
            return;
        }
        Model model(fileName, &stream, true, false);
        spRoot->NiAVObject_SetDisplayObject(true);
        spRoot->RemoveObject(spStars);
        spStars = model.GetObject3D();
        spStars->NiAVObject_SetSelectiveUpdate(true);
        spRoot->AttachChildInAvailableSlot((NiAVObject*)(NiNode*)spStars, true);
        spStars->UpdateProperties();
    }
}
