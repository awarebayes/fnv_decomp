#pragma once

#include "NiPointer.h"

class BSShader;
class NiRefObject;
class NiD3DPass;
class NiD3DVertexShader;
class NiD3DPixelShader;
class NiPropertyState;

// pdb: class BSShaderBloodSplatter, size 0x84, bases: BSShader, 3 members
class BSShaderBloodSplatter
{
public:
    unsigned char unk00[0x6C];
    NiPointer<NiD3DPass> spPass[2]; // src: XBOX_RETAIL_PDB +0x6c (old: spRenderPass)
    NiPointer<NiD3DVertexShader> spVertexShader[2]; // src: XBOX_RETAIL_PDB +0x74 (old: m_spTreeVertexShader)
    NiPointer<NiD3DPixelShader> spPixelShader[2]; // src: XBOX_RETAIL_PDB +0x7c (old: m_spPixelShaderVariants)

    BSShaderBloodSplatter();
    ~BSShaderBloodSplatter();
    static void* __cdecl operator new(size_t size);

    BSShader* CreateShader();
    void LoadShaders();
    bool LoadStagesAndPasses();
    void BSShaderBloodSplatterProperty_SetClampD3D(unsigned int);
    void BSShaderBloodSplatter_InitPass(unsigned int);
    void NiD3DPass_SetPixelShader(NiRefObject*);
    void SetupConstantMaps();
    int ImageSpaceShader_GetShaderDeclaration(void*, void*);
    void X_Shader_ClearPassStages();
    bool BSShaderBloodSplatter_ReloadShaders();
    void* FUN_00b80e40(unsigned int);
    void CreateShaderConstantMaps();
    void InitPassItems(NiPropertyState*);
    void ApplyBloodShader(NiPropertyState*);
};
