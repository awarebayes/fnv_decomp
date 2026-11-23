// bsshader/imagespaceeffectblur_decls.h: local image-space effect declarations
#ifndef IMAGESPACEEFFECTBLUR_DECLS_H
#define IMAGESPACEEFFECTBLUR_DECLS_H

#include "../../include/ImageSpaceEffectParam.h"
#include "../../include/MemoryManager.h"
#include "../../include/NiTexturingProperty.h"
extern MemoryManager g_memoryManager;
extern float DAT_011ae6c8, DAT_011ae6cc;
// SetAt changes array storage without retaining the argument reference.
template <>
__declspec(noalias) void NiTArray<ImageSpaceEffectParam*, NiTMallocInterface<ImageSpaceEffectParam*> >::SetAt(unsigned int, ImageSpaceEffectParam* const&);

class ImageSpaceEffect;
#include "../../include/ImageSpaceManager.h"
// inlined into the Setup functions (/O2); the shared header only declares it
inline ImageSpaceEffect* ImageSpaceManager::GetEffect(int index)
{
    return reinterpret_cast<NiTPrimitiveArray<ImageSpaceEffect*>*>(pEffectList)->m_pBase[index];
}
class ImageSpaceTexture;
// pdb: class ImageSpaceEffect, size 0x58 (here 0x38), 7 members
class ImageSpaceEffect
{
  public:
    char prefix[24];
    NiTArray<ImageSpaceEffectParam*, NiTMallocInterface<ImageSpaceEffectParam*> > pEffectParamList;
    NiTArray<ImageSpaceTexture*, NiTMallocInterface<ImageSpaceTexture*> > pTextures;
    void SetTextureArrayCount(int);
    void SetEffectParamAt(int, int, int);
    void SetEffectAt(int, ImageSpaceEffect*, ImageSpaceEffectParam*, int);
    void SetTextureArrayAt(int, int, int, NiTexturingProperty::FilterMode, NiTexturingProperty::FilterMode);
};
class ImageSpaceEffectFourTexturePass : public ImageSpaceEffect
{
  public:
    void Setup(ImageSpaceManager*, ImageSpaceEffectParam*);
};
// pdb: class ImageSpaceShaderParam, size 0x88, bases: ImageSpaceEffectParam, 13 members
class ImageSpaceShaderParam : public ImageSpaceEffectParam
{
  public:
    char data[0x88];
    ImageSpaceShaderParam();
    static __forceinline void* operator new(size_t n)
    {
        return g_memoryManager.Allocate(n);
    }
    static __forceinline void operator delete(void* p)
    {
        g_memoryManager.Deallocate(p);
    }
    void AllocateShaderConstants(int, int);
    void SetShaderConstantVector4(int, float, float, float, float);
};
#endif
