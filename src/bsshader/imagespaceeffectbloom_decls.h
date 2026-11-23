// bsshader/imagespaceeffectbloom_decls.h: declares ImageSpaceShaderParam, RenderStateMap, ImageSpaceManager
#ifndef FNV_IMAGESPACEEFFECTBLOOM_DECLS_H
#define FNV_IMAGESPACEEFFECTBLOOM_DECLS_H


#include "../../include/MemoryManager.h"
#include "../../include/NiColorA.h"
extern MemoryManager g_memoryManager;

#include "../../include/NiTMallocInterface.h"
#include "../../include/NiTArray.h"
#include "../../include/NiTPrimitiveArray.h"

class ImageSpaceTexture;
class ImageSpaceEffect;
class BloomEffectParamLocalForm;
class ImageSpaceShaderParam;
class BSRenderedTexture;
class NiTexture;
class NiD3DShaderConstantMap;


class BloomEffectParamLocalForm
{ // local form of ImageSpaceEffectParam: 0x00c03410 needs the 0x24-byte vtable/two-array base; the frozen shared size-1 declaration fails the exe-derived size/offset assertions.
  public:
    virtual ~BloomEffectParamLocalForm();
    NiTPrimitiveArray<int> m_kTextureTypes;
    NiTPrimitiveArray<int> m_kFilterModes;
};

// pdb: class ImageSpaceShaderParam, size 0x88, bases: ImageSpaceEffectParam, 13 members
class ImageSpaceShaderParam : public BloomEffectParamLocalForm
{
  public:
    int m_eShaderFilterMode;
    int m_eClearFlags;
    NiColorA m_eTextureClampMode;
    float* m_pfVertexShaderConstants;
    int m_iVertexShaderConstantCount;
    float* m_pfPixelShaderConstants;
    int m_iPixelShaderConstantCount;
    NiD3DShaderConstantMap* m_pVertexShaderConstantMap;
    NiD3DShaderConstantMap* m_pPixelShaderConstantMap;
    bool m_bTextureFlags[3];
    unsigned char layoutPadding57[1];
    NiTPrimitiveArray<NiTexture*> m_kShaderTextures;
    NiTPrimitiveArray<int> m_kTextureFilterModes;
    struct RenderStateMap
    {
        void** m_ppVirtualMethodTable;
        unsigned int m_uiHashSize;
        void* m_ppkHashTable;
        unsigned int m_uiCount;
    } m_kRenderStateMap;

    ImageSpaceShaderParam();
    void AllocateShaderConstants(int aiVSize, int aiPSize);
    void SetShaderConstantVector4(int index, float x, float y, float z, float w);
    static void* operator new(size_t size) throw()
    {
        return g_memoryManager.Allocate(size);
    }
    static void operator delete(void* pointer)
    {
        g_memoryManager.Deallocate(pointer);
    }
};

// pdb: class ImageSpaceEffect, size 0x58, 7 members
class ImageSpaceEffect
{
  public:
    virtual ~ImageSpaceEffect();
    bool bIsActive;
    bool bParamsChanged;
    unsigned char layoutPadding6[2];
    NiTPrimitiveArray<ImageSpaceEffect*> pEffectList;
    NiTPrimitiveArray<BloomEffectParamLocalForm*> pEffectParamList;
    NiTPrimitiveArray<ImageSpaceTexture*> pTextures;
    struct ImageSpaceEffectInput;
    NiTPrimitiveArray<ImageSpaceEffectInput*> pEffectInputs;
    NiTPrimitiveArray<int> pEffectOutput;
    void SetTextureArrayCount(int aiCount);
    void SetEffectAt(int index, ImageSpaceEffect* effect, BloomEffectParamLocalForm* param, int output);
    // No identity candidate is available for the one-input setup helper.
    void SetEffectParamAt(int effectIndex, int textureIndex, int filterMode);
};

// Only the PC prefix read by Setup is required here.
#include "../../include/ImageSpaceManager.h"
// inlined into the Setup functions (/O2); the shared header only declares it
inline ImageSpaceEffect* ImageSpaceManager::GetEffect(int index)
{
    return reinterpret_cast<NiTPrimitiveArray<ImageSpaceEffect*>*>(pEffectList)->m_pBase[index];
}

class BloomImageSpaceEffect : public ImageSpaceEffect
{
  public:
    void Setup(ImageSpaceManager* manager, BloomEffectParamLocalForm* param);
};

extern float DAT_012023f4;
extern float DAT_012023f8;

typedef char BloomShaderParamSizeCheck[1];
typedef char BloomEffectSizeCheck[1];
#endif
