
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

struct B6B610Node;
struct B6B610Geometry;

extern B6B610Node* g_B6B610Root;

B6B610Geometry* __cdecl FUN_00a7fc00(void* kName, void* uiWidth, void* uiHeight, B6B610Node* apkRenderer, void* kPrefs, void* eMSAAPref, void* apAliasTex, void* uiAliasOffset);

class B6B610Wrapper
{
  public:
    static void* operator new(size_t size);
    static void operator delete(void* p);

    B6B610Wrapper(B6B610Geometry* geometry, void* f, void* g);

    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
};

// Builds a geometry via FUN_00a7fc00 and, if it succeeds, allocates and returns a B6B610Wrapper for it; otherwise returns 0.
// @fnv 0x00b6b610
// src: XBOX_RETAIL_PDB 0x00b6b610
// pdb: BSRenderedTexture::Create | BSRenderedTexture.obj | retail, call graph, high
// pdb params: aName, aiWidth, aiHeight, arPrefs, aeMultiSamplePreference, abDepthBuffer, apSharedDepthBuffer, apAliasTex, uiAliasOffset
// pdb locals: prt, ptex
B6B610Wrapper* __cdecl BSRenderedTexture_Create(void* aName, void* aiWidth, void* aiHeight, void* arPrefs, void* aeMultiSamplePreference,
                                                 void* abDepthBuffer, void* apSharedDepthBuffer, void* apAliasTex, void* uiAliasOffset)
{
    B6B610Geometry* geometry = FUN_00a7fc00(aName, aiWidth, aiHeight, g_B6B610Root, arPrefs, aeMultiSamplePreference, apAliasTex, uiAliasOffset);
    if (geometry)
        return new B6B610Wrapper(geometry, abDepthBuffer, apSharedDepthBuffer);
    return 0;
}
