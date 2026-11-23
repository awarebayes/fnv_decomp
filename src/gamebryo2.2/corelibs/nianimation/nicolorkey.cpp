#define FNV64_NIANIMATIONKEY_OWN_KEY_ACCESSOR // Preserve the out-of-line accessor call in exe color-key code.

// src/gamebryo2.2/corelibs/nianimation/nicolorkey.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/gamebryo2.2/corelibs/nianimation/nicolorkey.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../../../include/NiColorA.h"

struct blob16
{
    char b[16];
};
#include "../../../../include/NiAnimationKey.h"
typedef char _sizecheck_NiAnimationKey[1];
struct ColorAnimationKey : public NiAnimationKey
{
    static int RegisterObjectLoader();
    static void RegisterSupportedFunctions(int, int);
    static NiAnimationKey* CreateObjectFromStream(NiStream*, unsigned int);
    void LoadBinary(NiStream*);
    static void SaveToStream(NiStream*, NiAnimationKey*, unsigned int);
    static void SaveBinary(NiStream*, NiAnimationKey*);
    static void Copy(NiAnimationKey*, void*);
    static NiAnimationKey* CreateArray(unsigned int);
    static void DeleteArray(NiAnimationKey*);
    static bool Insert(float, NiAnimationKey**, unsigned int*);
    static float ComputeMaxCurvature(NiAnimationKey*, unsigned int);
    static void Interpolate(float, void*, void*, void*);
    static void InterpolateD1(float, void*, void*, void*);
    static void InterpolateD2(float, void*, void*, void*);
    static bool Equal(void*, void*);
    static void FillDerivedVals(NiAnimationKey*, unsigned int, unsigned char);
    void SetColor(void*);
    void* GetColor();
    static struct blob16 InterpolatePosition(float, ColorAnimationKey*, int, unsigned int, unsigned int*,
                                             unsigned char);
    ColorAnimationKey* GetKeyPointerByIndex(unsigned int, unsigned char);
    static unsigned char GetKeySize(int);
    static int* GetInterpFunction(int);
    static int* GetCreateFunction(int);
    static int* GetSaveFunction(int);
    static int* GetEqualFunction(int);
    static int* GetCopyFunction(int);
    static int* GetArrayFunction(int);
    static int* GetDeleteFunction(int);
    static int* GetInsertFunction(int);
    static void SetDefault(void*);
    static struct blob16 GenInterpDefault(float, ColorAnimationKey*, int, unsigned int, unsigned char);
    void* __vecDelDtor(unsigned int);
    char m_kRgbaColor[16];
};
typedef char _sizecheck_NiColorKey[1];

typedef char _sizecheck_NiColorA[1];

typedef void(__cdecl* InterpFn)(float, const char*, const char*, void*);
extern InterpFn DAT_011f3cd8[];
extern double DAT_010241b0;

// Returns a table color directly when count is one or f matches DAT_010241b0; otherwise interpolates adjacent entries and updates pIdx.
// @fnv 0x00a296c0
// src: XBOX_RETAIL_PDB 0x00a296c0
// pdb: NiColorKey::GenInterp | NiColorKey.obj | retail, call graph (shared callers), high
// pdb params: fTime, pkKeys, eType, uiNumKeys, uiLastIdx, ucSize
// pdb locals: fLastTime, uiNextIdx, fNormTime, uiNumKeysM1, kResult, interp, fNextTime, uiLastIdxCopy
NiColorA __cdecl NiColorKey_GenInterp(float fTime, const char* pkKeys, unsigned int eType, unsigned int uiNumKeys,
                                   unsigned int* uiLastIdx, unsigned char ucSize)
{
    if (uiNumKeys == 1 || fTime == DAT_010241b0)
    {
        return *(const NiColorA*)(pkKeys + 4);
    }
    unsigned int idx = *uiLastIdx;
    unsigned int last = uiNumKeys - 1;
    float prevVal = *(const float*)(pkKeys + ucSize * idx);
    if (fTime < prevVal)
    {
        prevVal = *(const float*)pkKeys;
        idx = 0;
    }
    float fCurTime = 0.0f;
    unsigned int i = idx + 1;
    __assume(i <= last);
    for (; i <= last; ++i, ++idx)
    {
        fCurTime = *(const float*)(pkKeys + ucSize * i);
        if (fTime <= fCurTime)
            break;
        prevVal = fCurTime;
    }
    // The original temporary is initialized below without invoking a constructor.
    float colorStorage[4];
    NiColorA& color = *reinterpret_cast<NiColorA*>(colorStorage);
    color.r = 0.0f;
    color.g = 0.0f;
    color.b = 0.0f;
    color.a = 0.0f;
    float t = (fTime - prevVal) / (fCurTime - prevVal);
    DAT_011f3cd8[eType](t, pkKeys + ucSize * idx, pkKeys + ucSize * i, &color);
    *uiLastIdx = idx;
    return color;
}
