// fallout/interface/fadermanager.cpp: FaderManager, FM7 (10 functions)

// Keep the unit's fixed-string helpers out of line.
#define FNV64_NIFIXEDSTRING_OWN_BODIES
#include "../../../include/AutoMemContext.h"
#include "../../../include/BSFadeNode.h"
#include "../../../include/BSShaderManager.h"
#include "../../../include/BSStringT.h"
#include "../../../include/NiAlphaProperty.h"
#include "../../../include/NiBound.h"
#include "../../../include/NiDirectionalLight.h"
#include "../../../include/NiLight.h"
#include "../../../include/NiNode.h"
#include "../../../include/NiObject.h"
#include "../../../include/NiObjectNET.h"
#include "../../../include/NiPoint2.h"
#include "../../../include/NiPoint3.h"
#include "../../../include/NiRefObject.h"
#include "../../../include/NiSortAdjustNode.h"
#include "../../../include/NiTexture.h"
#include "../../../include/NiTriShape.h"
#include "../../../include/NiUpdateData.h"
#include "../../../include/TES.h"
#include "../../../include/TileShaderProperty.h"
#include "../../../include/GameTimerRecord.h"
#include "fadermanager_object_decls.h"

// Returns true if the fader's fade node exists, its flag is set, and its alpha equals 1.0.
// @fnv 0x00701450
bool FaderManager::HasFader(FADER_ENUM eFader)
{
    FaderEntry* pFader = &s_faders[eFader];
    if (pFader->pFadeNode != 0)
        if (pFader->bUnk08)
            if (pFader->fAlpha == 1.0)
                return true;
    return false;
}

// @fnv 0x007014e0
float FaderManager::GetFadeValue(FADER_ENUM eFader)
{
    return s_faders[eFader].fAlpha;
}

// @fnv 0x007010e0
// src: XBOX_RETAIL_PDB 0x007010e0
// pdb: FaderManager::RemoveFader | FaderManager.obj | retail, call graph (shared callers), high
// pdb params: aeFader, abForce
void FaderManager::RemoveFader(FADER_ENUM aeFader, bool abForce)
{
    if (s_faders[aeFader].pFadeNode == 0)
        return;
    if (abForce || s_bAllowDetach)
    {
        s_faders[aeFader].pFadeNode->GetParent()->RemoveObject(s_faders[aeFader].pFadeNode);
        s_faders[aeFader].pFadeNode = 0;
        s_faders[aeFader].fAlpha = 0.0f;
        s_faders[aeFader].m_pParentNode = 0;
        g_TES->RefreshWorldState(0, 0, 0);
    }
    else
    {
        s_faders[aeFader].bUnk08 = false;
        if (s_faders[aeFader].fAlpha == 1.0)
            s_faders[aeFader].fAlpha = s_faderClearedAlpha;
    }
}

#include "fadermanager_update_decls.h"
static signed char s_iFadeDelay;

// Steps each fader's alpha toward 1 or 0 using elapsed time, removes faded-out nodes, and applies alpha to their shader properties.
// @fnv 0x007011d0
// src: XBOX_RETAIL_PDB 0x007011d0
// pdb: FaderManager::UpdateFaders | FaderManager.obj | retail, call graph, high
// pdb params: abUpdateLoading
// pdb locals: tempMemContext, n, pfd
void FaderManager::UpdateFaders(bool abUpdateLoading)
{
    AutoMemContext context(MEM_CONTEXT_MEM_CONTEXT_INTERFACE, true,
                           "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout\\Interface\\FaderManager.cpp", 266);
    for (int i = 0; i < FADER_ENUM_FADER_COUNT; ++i)
    {
        FaderEntry* pFader = &s_faders[i];
        if (pFader->pFadeNode)
        {
            if (pFader->bUnk08)
            {
                if (pFader->fAlpha < 1.0)
                {
                    pFader->fAlpha += g_timeGlobal.GetSecondsPassed() / pFader->fUnk0C;
                    if (pFader->fAlpha > 1.0)
                        pFader->fAlpha = 1.0f;
                    if (pFader->m_pParentNode)
                        ((TileShaderProperty*)(NiNode*)pFader->m_pParentNode)->SetAlpha(pFader->fAlpha);
                }
            }
            else
            {
                if (pFader->fAlpha <= 0.0)
                {
                    if (pFader->pFadeNode && pFader->pFadeNode->GetParent())
                        pFader->pFadeNode->GetParent()->RemoveObject(pFader->pFadeNode);
                    pFader->pFadeNode = 0;
                    pFader->fAlpha = 0.0f;
                }
                else
                {
                    if (s_iFadeDelay)
                        --s_iFadeDelay;
                    else
                        pFader->fAlpha -= g_timeGlobal.GetSecondsPassed() / pFader->fUnk0C;
                    if (pFader->fAlpha < 0.0)
                        pFader->fAlpha = 0.0f;
                    if (pFader->m_pParentNode)
                        ((TileShaderProperty*)(NiNode*)pFader->m_pParentNode)->SetAlpha(pFader->fAlpha);
                }
            }
        }
    }
}

#include "fadermanager_create_decls.h"

// @fnv 0x00700960
// pdb: FaderManager::CreateFader | FaderManager.obj | retail, unique string literal, high
// pdb params: aeFader, afFadeSpeed, abBeginBlack
// pdb locals: fhalfScreenHeight, pparent, ptexture, UpdateData, textureFilename, pvertOrder, ptri, palphaProp, ptsp, tempMemContext, pvertices, falpha, ptextures, fhalfScreenWidth
void FaderManager::AddFader(FADER_ENUM aeFader, float afFadeSpeed, bool abBeginBlack)
{
    AutoMemContext kAllocationContext(MEM_CONTEXT_MEM_CONTEXT_INTERFACE, true,
                                      "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout\\Interface\\FaderManager.cpp",
                                      135);
    if (s_faders[aeFader].pFadeNode)
    {
        if (!s_faders[aeFader].bUnk08)
            RemoveFader(aeFader, true);
        else
            return;
    }
    NiPoint3* pScreenVertices = new NiPoint3[4];
    FaderSurfaceParameters surface;
    surface.pTexCoords = new NiPoint2[4];
    unsigned short* pTriangles = (unsigned short*)_NiMalloc(12);
    float fHalfScreenWidth = Renderer_GetScreenWidth() * 0.5;
    float fHalfHeight = Renderer_GetScreenHeight() * 0.5;
    pScreenVertices[0] = NiPoint3(-fHalfScreenWidth, 0.0f, fHalfHeight);
    pScreenVertices[1] = NiPoint3(-fHalfScreenWidth, 0.0f, -fHalfHeight);
    pScreenVertices[2] = NiPoint3(fHalfScreenWidth, 0.0f, fHalfHeight);
    pScreenVertices[3] = NiPoint3(fHalfScreenWidth, 0.0f, -fHalfHeight);
    surface.pTexCoords[0] = NiPoint2(0.0f, 0.0f);
    surface.pTexCoords[1] = NiPoint2(0.0f, 1.0f);
    surface.pTexCoords[2] = NiPoint2(1.0f, 0.0f);
    surface.pTexCoords[3] = NiPoint2(1.0f, 1.0f);
    pTriangles[0] = 0;
    pTriangles[1] = 1;
    pTriangles[2] = 2;
    pTriangles[3] = 2;
    pTriangles[4] = 1;
    pTriangles[5] = 3;
    s_faders[aeFader].pFadeNode = (BSFadeNode*)new NiSortAdjustNode(0);
    s_faders[aeFader].pFadeNode->SetName(NiFixedString(s_faders[aeFader].pTextureName));
    NiAlphaProperty* pBlendingProperty = new NiAlphaProperty;
    pBlendingProperty->NiAlphaProperty_SetAlphaBlending(true);
    s_faders[aeFader].pFadeNode->AttachProperty(pBlendingProperty);
    NiTriShape* faderShape = new NiTriShape(4, pScreenVertices, 0, 0, surface.pTexCoords, 1, 0, 2, pTriangles);
    s_faders[aeFader].pFadeNode->AttachChildInAvailableSlot(faderShape, true);
    BSStringT<char> textureName;
    textureName.PrintfString("Data\\Textures\\Interface\\Faders\\%s", s_faders[aeFader].pTextureName);
    faderShape->SetName(NiFixedString(textureName.String()));
    NiPointer<NiTexture> spTextureImage(
        NiSourceTexture::Create(NiFixedString(textureName.String()), DAT_011a9598, true, false));
    surface.fInitialAlpha = 0.0f;
    if (abBeginBlack || afFadeSpeed == 0.0)
    {
        surface.fInitialAlpha = 1.0f;
        s_iFadeDelay = 3;
    }
    TileShaderProperty* pTileShader = new TileShaderProperty(false);
    pTileShader->SetTextureImage(spTextureImage);
    pTileShader->EffectSetting_SetAreaSound(3);
    pTileShader->SetAlpha(surface.fInitialAlpha);
    faderShape->AttachProperty((NiObject*)pTileShader);
    s_faders[aeFader].m_pParentNode = (NiNode*)pTileShader;
    BSShaderManager::UpdateObjectTypeRecurse(faderShape, false, false);
    faderShape->NiGeometry_GetModelBound()->ComputeFromPoints(4, pScreenVertices);
    NiNode* pRoot = m_pRootAboveMenus;
    if (s_faders[aeFader].m_bUseAlternateRoot)
        pRoot = m_pRootBelowMenus;
    pRoot->AttachChildInAvailableSlot((NiAVObject*)s_faders[aeFader].pFadeNode, true);
    pRoot->UpdateSceneObject();
    NiUpdateData updateParams(0.0f, false, false);
    pRoot->Update(updateParams);
    s_faders[aeFader].fUnk0C = afFadeSpeed;
    s_faders[aeFader].bUnk08 = !abBeginBlack;
    s_faders[aeFader].fAlpha = surface.fInitialAlpha;
}

// @fnv 0x007006c0
// pdb: FaderManager::AddRoot | FaderManager.obj | retail, upstream name already matches, high
// pdb params: apNode, afOneToOneDistance, abBelowMenus
// pdb locals: fscreenWidth, fscale, tempMemContext, pnode, plight, fscreenHeight
void FaderManager::AddRoot(NiNode* apNode, float afOneToOneDistance, bool abBelowMenus)
{
    AutoMemContext kContext(MEM_CONTEXT_MEM_CONTEXT_INTERFACE, true,
                            "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout\\Interface\\FaderManager.cpp", 80);
    NiSortAdjustNode* pSortAdjustNode = new NiSortAdjustNode(0);
    if (abBelowMenus)
    {
        m_pRootBelowMenus = pSortAdjustNode;
        pSortAdjustNode->SetName(NiFixedString("FaderNode Below Menus"));
    }
    else
    {
        m_pRootAboveMenus = pSortAdjustNode;
        pSortAdjustNode->SetName(NiFixedString("FaderNode Above Menus"));
        afOneToOneDistance -= 20.0;
    }
    NiDirectionalLight* apTargetLight = new NiDirectionalLight;
    apTargetLight->SetName(NiFixedString("FaderNodeLight"));
    apTargetLight->SetAmbientColor(NiColor(1.0f, 1.0f, 1.0f));
    // This helper's canonical binding is a folded NiObject::LinkObject alias.
    apTargetLight->NiObject::LinkObject(*(NiStream*)pSortAdjustNode);
    pSortAdjustNode->AttachChildInAvailableSlot(apTargetLight, true);
    float m_fGlyphWidthInPixels = (float)Renderer_GetScreenWidth();
    float halfHeight = (float)Renderer_GetScreenHeight();
    float fScale = 1.0f;
    if (halfHeight / m_fGlyphWidthInPixels != 0.75)
        fScale = 4.0f / 3.0f;
    if (m_fGlyphWidthInPixels != 1280.0)
        fScale *= 1280.0 / m_fGlyphWidthInPixels;
    if (is_bEyeFinity())
        fScale = 1.0f;
    pSortAdjustNode->NiAVObject_SetScale(fScale);
    pSortAdjustNode->NiAVObject_SetTranslate(NiPoint3(0.0f, afOneToOneDistance, 0.0f));
    apNode->AttachChildInAvailableSlot((NiAVObject*)pSortAdjustNode, true);
}

typedef char _sizecheck_BSTimer[1];

extern float DAT_011ac3a0;

// @fnv 0x007013e0
float GameTimerRecord::GetRealTimeSeconds()
{
    return fDelta / DAT_011ac3a0;
}

struct FaderRec7
{
    char pad0[8];
    unsigned char f8;
    char pad1[7];
    int f10;
    float f14;
    char pad2[4];
};
struct FM7
{
    bool IsFaderActive(int);
};
// Returns true when the fader at idx has a nonzero f10 and f8 and its f14 is below 1.0.
// @fnv 0x00701400
bool FM7::IsFaderActive(int aiIndex)
{
    FaderRec7* f = (FaderRec7*)&s_faders[aiIndex];
    if (f->f10 && f->f8 && f->f14 < 1.0)
        return true;
    return false;
}

namespace fnv_007014a0 {
struct FaderRec7
{
    char pad0[8];
    unsigned char f8;
    char pad1[7];
    int f10;
    char pad2[8];
};
struct FM7
{
    bool FaderManager_IsFaderActive(int);
};
// Returns true when the fader record at idx has a nonzero f10 and a nonzero f8.
// @fnv 0x007014a0
// src: COMMUNITY 0x007014a0
bool FM7::FaderManager_IsFaderActive(int aiIndex)
{
    FaderRec7* f = (FaderRec7*)&s_faders[aiIndex];
    if (f->f10 && f->f8)
        return true;
    return false;
}
}  // namespace fnv_007014a0

extern int DAT_0119f278;
extern int DAT_0119f27c;
extern int DAT_0119f280;
struct FUN_006ffe80_shell
{
    int m_00, m_04, m_08, m_0c, m_10, m_14, m_18, m_1c, m_20, m_24;
    unsigned char m_28, m_29;
    void AnimatingText_SetTraits(int, int);
    void AnimatingText_Initialize();
};
// Calls FUN_006ffe80(0, 0), then initializes fields from DAT_0119f278/27c/280 and zeroes m_20, m_1c, m_24, m_29 while setting m_28 to 1.
// @fnv 0x006ffe10
// src: XBOX_PDB 0x006ffe10
// pdb: AnimatingText::Initialize | AnimatingText.obj | debug, upstream name already matches, high
void FUN_006ffe80_shell::AnimatingText_Initialize()
{
    AnimatingText_SetTraits(0, 0);
    m_10 = DAT_0119f278;
    m_14 = DAT_0119f27c;
    m_18 = DAT_0119f280;
    m_20 = 0;
    m_1c = 0;
    m_24 = 0;
    m_28 = 1;
    m_29 = 0;
}
