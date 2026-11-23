#pragma once

#include <stddef.h>
#include "NiCriticalSection.h"
#include "NiAlphaAccumulator.h"
#include "NiAVObject.h"
#include "BSShaderManager.h"
#include "NiTPointerMap.h"
#include "NiColorA.h"

// Define FNV64_INCLUDE_BSMULTIBOUND_CAPSULE to expose the capsule allocation type.
#if defined(FNV64_INCLUDE_BSMULTIBOUND_CAPSULE)
#include "BSMultiBoundShape.h"
#include "NiPoint3.h"
#endif

class BSBatchRenderer;
class BSMultiBoundCapsule;
class BSInstanceRendering;
class BSOcclusionQuery;
class BSRenderedTexture;
class ReferenceVolume;
class RenderQuery;
class ShadowSceneLight;
class ShadowSceneNode;
class BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry_____ListItem;

// pdb: class BSShaderAccumulator, size 0x280, bases: NiAlphaAccumulator, 43 members
class BSShaderAccumulator : public NiAlphaAccumulator
{
public:
    enum etAccumulationMode
    {
        BSSA_ACCUMNORMAL = 0,
        BSSA_ACCUMDEPTHMAP = 1,
        BSSA_ACCUMLOD = 2,
        BSSA_ACCUMNONE = 3
    };

    struct RenderQueryState
    {
        RenderQuery* m_pRenderQuery;
        bool m_bQueryPending;
        unsigned char unk05[3];
        float m_fQueryValue;
        unsigned int m_uiQueryFlags;
        unsigned int m_uiQueryResult;
    };

    struct BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry___
    {
        virtual ~BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry___();
        BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry_____ListItem* pHead;
        BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry_____ListItem* pTail;
        BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry_____ListItem* m_pCachedListItems;
        struct AntiBloatAllocator
        {
            unsigned int ItemUpdateArray;
        } Allocator;
    };

    struct BSShaderProperty__RenderPass
    {
        NiGeometry* m_pShaderGeometry;
        unsigned short m_usShaderPassType;
        unsigned char m_ucLightIndex;
        bool m_bIsBest;
        bool m_bRendered;
        unsigned char m_ucLightCount;
        unsigned char m_ucLightCapacity;
        unsigned char m_ucPassIndex;
        ShadowSceneLight** m_ppShadowSceneLights;
    };

    BSShaderAccumulator();
    virtual ~BSShaderAccumulator();
    BSShaderAccumulator(int aeMode, unsigned int auiA, unsigned int auiB);
    BSShaderAccumulator(BSShaderManager::BATCHRENDERER_CREATION_MODE eMode, unsigned int uiA, unsigned int uiB);
    static void* __cdecl operator new(size_t uiSize);
    static void operator delete(void* pvMem);
    static void operator delete(void* pvMem, size_t uiSize);

    void ClearActivePasses();
    void SetupDepth(NiCamera* apWorldCamera, NiCamera* ap1stPersonCamera, BSRenderedTexture* apCurRenderTarget);
    void SetupDepthCameras(NiCamera* apWorldCamera, NiCamera* ap1stPersonCamera);
    void SetAccumulate(bool bAccumulate);
    bool __thiscall HasListedItemInRange(unsigned int a, int b);
    void BSShaderAccumulator_SetShadowSceneNode(void*);
    void FUN_004a1040(unsigned int);
    void BSShaderAccumulator_SetWaterReflection(unsigned char abFlag);
    void BSShaderAccumulator_SetSilhouetteColor(const NiColorA* apData);
    void BSShaderAccumulator_SetRenderWaterDepth(bool);
    void BSShaderAccumulator_SetRenderDecals(bool);
    void MobileObject_SetSoundCallBackSuccess(bool);
    void __thiscall GetWaterRefractionPassWithinRange(unsigned int a, int b);
    void BSShaderAccumulator_RenderNormals(void* apCamera, BSRenderedTexture* apCurRenderTarget);

    etAccumulationMode eAccumulationMode; // src: XBOX_RETAIL_PDB +0x34 (old: _unverified_eAccumulationMode)
    bool bWorldGeometry;
    bool bCacheGeometry; // src: XBOX_RETAIL_PDB +0x39 (old: _unverified_bCacheGeometry)
    bool bDoDepthPasses;
    unsigned char _pad3B[1];
    unsigned int uiSunPixelCount;
    bool bWaitingForSunQueury;
    unsigned char _pad41[3];
    float fPercentSunOccludedStored;
    RenderQueryState pSunTests[3];
    bool bAccumulate;
    bool b1stPerson;
    bool bRenderNoShader;
    unsigned char _pad87[1];
    unsigned short usQueryCount;
    unsigned short usQuerySize;
    BSOcclusionQuery** ppOcclusionQuery;
    NiTPointerList<NiGeometry*> pVolumetricFogGeometry;
    BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry___ pRenderedGeometryCache[4];
    BSTPersistentList_NiTPointerAllocator_unsigned_int__NiGeometry___ pRendered1stPersonGeometryCache[4];
    NiPointer<NiGeometry> spBoundTestGeometry;
    NiTPointerList<ReferenceVolume*> ReferenceVolumeList;
    unsigned int uiLastOcclusionRef;
    bool bLastOcclusionResult;
    unsigned char _pad151[3];
    NiColorA kSilhouetteColor;
    bool bWaterReflection;
    bool bWaterRefraction;
    bool bCellHasWater;
    bool bUnderWater;
    int iCurrentWaterHeight;
    bool bRenderDecals;
    unsigned char _pad16D[3];
    BSInstanceRendering* pInstanceRenderer;
    BSBatchRenderer* pBatchRenderer[2];
    unsigned int* puicurrentPassA;
    int* picurrentBucketA;
    bool* pbcurrentFirstPassA;
    bool* pbcurrentActiveA;
    unsigned int uiBatchRendererCount;
    unsigned int uiBatchRendererPassCount;
    ShadowSceneNode* pActiveShadowSceneNode;
    NiPointer<BSRenderedTexture> spWaterFogRenderTarget;
    int eRenderMode;
    NiPointer<ShadowSceneLight> spShadowLight;
    BSShaderProperty__RenderPass kAlphaRenderPass;
    NiTPointerMap<NiAVObject*, bool> kRoomSharedMap;
    unsigned char _pad1C4[60];
    NiCriticalSection BSCPSharedCrit;
};

#if defined(FNV64_INCLUDE_BSMULTIBOUND_CAPSULE)
// pdb: class BSMultiBoundCapsule, size 0x28, bases: BSMultiBoundShape, 3 members
class BSMultiBoundCapsule : public BSMultiBoundShape
{
public:
    BSMultiBoundCapsule();

    NiPoint3 PointA; // src: XBOX_RETAIL_PDB +0xc (old: unk0C)
    NiPoint3 PointB; // src: XBOX_RETAIL_PDB +0x18 (old: unk18)
    float Radius; // src: XBOX_RETAIL_PDB +0x24 (old: unk24)
};
#endif
