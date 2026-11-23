#pragma once

class RunNode;
class SStr;
class Info_;

// pdb: structure TextureFile, size 0x24, 2 members
struct TextureFile
{
    unsigned char iType[4]; // src: XBOX_RETAIL_PDB +0x0 (old: _unverified_iType)
    unsigned char pFilename[32]; // src: XBOX_RETAIL_PDB +0x4 (old: _unverified_pFilename)
};

// pdb: structure UVMap, size 0x8, 2 members
struct UVMap
{
    float fU;
    float fV;
};

// pdb: structure FontLetter, size 0x38, 7 members
struct FontLetter
{
    unsigned char iTextureIndex[4]; // src: XBOX_RETAIL_PDB +0x0 (old: _unverified_iTextureIndex)
    UVMap pMapping[4];
    float fWidth;
    float fHeight;
    float fLeadingEdge;
    float fSpacing;
    float fTopEdge;
};

// pdb: structure FontData, size 0x3928, 4 members
class FontData
{
public:
    float fBaseLine;
    int iTextureCount;
    TextureFile pTextureFiles[8];
    FontLetter pFontLetters[256];

    static void* operator new(size_t size);
    RunNode* FontManager_PrepHypertext(SStr*, Info_*);
    RunNode* FontManager_PrepText(SStr* axTextString, Info_* axData);
};
