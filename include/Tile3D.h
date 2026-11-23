#pragma once

#include "Tile.h"

class NiAVObject;
template <class T> class BSStringT;
class NiNodeV;

// pdb: class Tile3D, size 0x50, bases: Tile, 4 members
class Tile3D : public Tile
{
  public:
    static void* operator new(size_t bytes);
    static void operator delete(void* memory);

    Tile3D();
    __forceinline bool TestFlag(unsigned int mask) const; // inlined in exe destructor 0x00a20650
    virtual ~Tile3D();
    virtual void Init(Tile* parent, const char* name, Tile* replacedChild);
    virtual NiNode* CalculateNode();
    virtual int GetType();
    virtual const char* GetTypeStr();
    virtual Tile* SetTraitValue(int aiTrait, float afValue, const char* axString);
    virtual void SetAlphaAndOverlayColor(NiNode* node, float alpha, NiColorAlpha* color);

    void SetParent(Tile3D* parent, Tile3D* replacedChild);
    bool FindManagerNode(NiAVObject* apNode);
    void UpdateNIF();
    void RemovePropertyRecursive(char* target);
    void RemoveProperties(NiAVObject* object);
    void ConvertShadersToTileShader(NiNode* node);
    void SetupShaderPropertiesRecurse(NiAVObject* apNode);
    bool UpdateAnimation(float value, bool force);
    void UpdateAnimation(float value, int force);
    const char* __thiscall GetTypeName_00a0bdd0();
    NiNodeV* GetModel_0056c7f0();
    bool Tile3D_PlaySequence(const char* sequenceName);

    void* pAniManager; // src: XBOX_RETAIL_PDB +0x38 (old: m_pParent)
    void* pCurrentSequence;            // +0x3c, zeroed by the constructor 0x00a0bc80 // src: XBOX_RETAIL_PDB +0x3c (old: m_pChildTile)
    BSStringT<char> strCurrentName;       // +0x40, cleared by SetTraitValue 0x00a20c50 (trait 0xfc4) // src: XBOX_RETAIL_PDB +0x40 (old: m_kName)
    BSStringT<char> strFilename;   // +0x48; the destructor 0x00a20650 frees +0x48 then +0x40
};
