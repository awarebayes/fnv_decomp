// imagespaceshaderblur_blur_reload_support.h: blur shader support declarations
#ifndef FNV_BLUR_RELOAD_SUPPORT_H
#define FNV_BLUR_RELOAD_SUPPORT_H

#include "../include/BSShader.h"
#include "../include/BSShaderManager.h"
#include "../include/INISettingCollection.h"
#include "../include/Setting.h"

// Declaration facts: public yvileapsis/yUI-NVSE CommonLib shader headers;
// NiTArray/ImageSpaceEffect layout follows the accepted bloom declarations
// (decomp/src/bsshader/imagespaceeffectbloom_decls.h).
extern "C" void* __cdecl fnv_zlib_memset(void*, int, unsigned int);
typedef char ShaderMacroSize[1];
#include "../include/NiTMallocInterface.h"
#include "../include/NiTArray.h"
#include "../include/NiTPrimitiveArray.h"
class ImageSpaceTexture;
class ImageSpaceEffectParam;
// pdb: class ImageSpaceEffect, size 0x58, 7 members
class ImageSpaceEffect
{
  public:
    virtual ~ImageSpaceEffect();
    bool bIsActive;
    bool bParamsChanged;
    unsigned char layoutPadding6[2];
    NiTPrimitiveArray<ImageSpaceEffect*> pEffectList;
    NiTPrimitiveArray<ImageSpaceEffectParam*> pEffectParamList;
    NiTPrimitiveArray<ImageSpaceTexture*> pTextures;
    struct ImageSpaceEffectInput;
    NiTPrimitiveArray<ImageSpaceEffectInput*> pEffectInputs;
    NiTPrimitiveArray<int> pEffectOutput;
};
typedef char ImageSpaceEffectSizeCheck[1];
typedef char BSShaderSizeCheck[1];
class ImageSpaceEffectShader : public BSShader, public ImageSpaceEffect
{
  public:
    ImageSpaceEffectShader();
    virtual void InitShaders(const BSShaderManager::BSShaderMacro&, const BSShaderManager::BSShaderMacro&, const char*,
                             const char*, const char*);
    virtual void SetupShaderDeclaration();
    virtual void Func_81();
    virtual void SetParameters();
    virtual bool SetupPasses();
};
typedef char ImageSpaceShaderSizeCheck[1];
#endif
