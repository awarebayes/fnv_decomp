
// src/bsshader/bsshaderbloodsplatterproperty.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/bsshader/bsshaderbloodsplatterproperty.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

struct S_00b81420_Property
{
    int unk00, unk04, unk08, unk0c, unk10, unk14, unk18;
    int m_eRequestType;
    int unk20, unk24, unk28, unk2c, unk30, unk34, unk38, unk3c;
    int unk40, unk44, unk48, unk4c, unk50, unk54, unk58, unk5c;
    int unk60, unk64, unk68;
    int m_iValue;
};

struct S_00b81420_Node;

struct S_00b81420_AVObject
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual S_00b81420_Node* IsSceneNode();
    virtual void v4();
    virtual void v5();
    virtual int IsGeometry();
    S_00b81420_Property* GetProperty(int type);
};

struct S_00b81420_Node : S_00b81420_AVObject
{
    int unk04, unk08, unk0c, unk10, unk14, unk18, unk1c;
    int unk20, unk24, unk28, unk2c, unk30, unk34, unk38, unk3c;
    int unk40, unk44, unk48, unk4c, unk50, unk54, unk58, unk5c;
    int unk60, unk64, unk68, unk6c, unk70, unk74, unk78, unk7c;
    int unk80, unk84, unk88, unk8c, unk90, unk94, unk98, unk9c;
    S_00b81420_AVObject** m_ppObjects;
    unsigned short m_usMaxSize;
    unsigned short m_usSize;
};

// If obj is geometry, sets property 3's m_iValue when its type is 0x17; otherwise recurses into each child of obj's node.
// @fnv 0x00b81420
// src: XBOX_PDB 0x00b81420
// pdb: BSShaderBloodSplatterProperty::SetFadeSourceRecurse | BSShaderBloodSplatterProperty.obj | retail, upstream name already matches, high
// pdb params: apObject, apfAlphaSource
// pdb locals: pgeometry, pproperty, pprop, pnode, ucount, uindex, pchild
void BSShaderBloodSplatterProperty_SetFadeSourceRecurse(S_00b81420_AVObject* apObject, int apfAlphaSource)
{
    if (!apObject)
        return;
    if (apObject->IsGeometry())
    {
        S_00b81420_Property* prop = apObject->GetProperty(3);
        if (prop && prop->m_eRequestType == 0x17)
            prop->m_iValue = apfAlphaSource;
    }
    else
    {
        S_00b81420_Node* node = apObject->IsSceneNode();
        if (node)
        {
            unsigned int count = node->m_usSize;
            for (unsigned int i = 0; i < count; i++)
            {
                S_00b81420_AVObject* child = node->m_ppObjects[i];
                if (child)
                    BSShaderBloodSplatterProperty_SetFadeSourceRecurse(child, apfAlphaSource);
            }
        }
    }
}

// FUN_00b81330: thiscall, 6 stack params, callee pops 0x18.
struct SpawnedQ
{
    unsigned int m_00;
};

struct SpawnedP
{
    unsigned int unk00;
    SpawnedQ* m_04;
    unsigned int unk08;
    unsigned int unk0c;
    unsigned int m_10;

    void FUN_00ba9ee0(unsigned int apGeometry, unsigned int ausPassEnum, unsigned int abFirstPass, unsigned int ausNumLights, unsigned int apSceneLight,
                      unsigned int apSceneLight1, unsigned int apSceneLight2, unsigned int apSceneLight3);
};

struct SpawnedRec
{
    unsigned int m_00;
    unsigned int m_04;
};

extern SpawnedRec DAT_011fedd0[100];
extern int DAT_011ff0f0;

void __stdcall FUN_00ba95e0(int aiNumPasses);

class FUN_00b81330_cls
{
  public:
    unsigned int unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c;
    unsigned int unk20, unk24, unk28, unk2c, unk30, unk34;
    unsigned int m_38;
    SpawnedP* volatile m_3c;
    unsigned int unk40, unk44, unk48, unk4c, unk50, unk54, unk58, unk5c, unk60;
    int m_64;

    SpawnedP* BSShaderBloodSplatterProperty_GetRenderPasses(unsigned int apGeom, unsigned int aiEnabledPasses, unsigned int asCount, unsigned int auiRenderMode, unsigned int apAccumulator,
                           unsigned int abUpdateList);
};

// @fnv 0x00b81330
// src: XBOX_PDB 0x00b81330
// pdb: BSShaderBloodSplatterProperty::GetRenderPasses | BSShaderBloodSplatterProperty.obj | retail, upstream name already matches, high
// pdb params: apGeom, aiEnabledPasses, asCount, auiRenderMode, apAccumulator, abUpdateList
// pdb locals: idebugPasses, ambPass
SpawnedP* FUN_00b81330_cls::BSShaderBloodSplatterProperty_GetRenderPasses(unsigned int apGeom, unsigned int aiEnabledPasses, unsigned int asCount, unsigned int auiRenderMode,
                                         unsigned int apAccumulator, unsigned int abUpdateList)
{
    unsigned int uiCacheKey = (auiRenderMode << 8) | aiEnabledPasses;
    if (this->m_38 != uiCacheKey)
    {
        FUN_00ba95e0(1);
        SpawnedP* s = this->m_3c;
        s->m_10 = 0;
        SpawnedP* p = this->m_3c;

        int flag = (this->m_64 != 0);
        p->FUN_00ba9ee0(apGeom, flag + 0x2f0, 1, 0, 0, 0, 0, 0);
        this->m_38 = uiCacheKey;
    }
    int count = DAT_011ff0f0;
    if (count < 100)
    {
        DAT_011fedd0[count].m_00 = apGeom;
        DAT_011fedd0[count].m_04 = this->m_3c->m_04->m_00;
        DAT_011ff0f0 = count + 1;
    }
    return this->m_3c;
}

struct RTTI
{
    const char* name;
    RTTI* parent;
};

extern RTTI g_rttiA;
extern RTTI g_rttiB;

struct Child
{
    virtual void vfn00();
    virtual void vfn04();
    virtual RTTI* GetRTTI();
};

typedef void(__cdecl* Callback)(Child*, int, int);

struct Self
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    int unk2c;
    int unk30;
    int unk34;
    int unk38;
    int unk3c;
    int unk40;
    int unk44;
    int unk48;
    int unk4c;
    int unk50;
    int unk54;
    int unk58;
    int unk5c;
    Child* children[2];
    void __thiscall ForEachChild(Callback cb, int arg);
};

// Iterates over the two child pointers, checks each child's RTTI chain against g_rttiA and g_rttiB, and invokes cb on each non-null child with 0 and arg.
// @fnv 0x00b813b0
// pdb: BSShaderBloodSplatterProperty::GetTextureUse | BSShaderBloodSplatterProperty.obj | debug, object-file run extension, low
// pdb params: apTextureUseCB, apParam
// pdb locals: i, ulsize
void __thiscall Self::ForEachChild(Callback apCallback, int arg)
{
    for (int i = 0; i < 2; i++)
    {
        Child* pChild = children[i];
        if (pChild)
        {
            bool found = false;
            for (RTTI* pRTTI = pChild->GetRTTI(); pRTTI; pRTTI = pRTTI->parent)
            {
                if (pRTTI == &g_rttiA)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                Child* pChild2 = children[i];
                if (pChild2)
                {
                    for (RTTI* pRTTI = pChild2->GetRTTI(); pRTTI && pRTTI != &g_rttiB; pRTTI = pRTTI->parent)
                    {
                    }
                }
            }
            apCallback(children[i], 0, arg);
        }
    }
}

struct Item
{
    static void* operator new(size_t size);
    static void operator delete(void* p);
    Item();
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
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
};

struct Owner
{
    void BSShaderBloodSplatterProperty_CopyMembers(Item* pDest, int arCloneData);
    Item* BSShaderBloodSplatterProperty_CreateClone(int arCloneData);
};

// @fnv 0x00b814a0
// src: XBOX_RETAIL_PDB 0x00b814a0
// pdb: BSShaderBloodSplatterProperty::CreateClone | BSShaderBloodSplatterProperty.obj | retail, object-file gap fill, high
// pdb params: arCloneData
// pdb locals: pprop
Item* Owner::BSShaderBloodSplatterProperty_CreateClone(int arCloneData)
{
    Item* item = new Item;
    BSShaderBloodSplatterProperty_CopyMembers(item, arCloneData);
    return item;
}

struct StringArray
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned short m_08;
    unsigned short m_0a;
    unsigned short unk0c;
    unsigned short m_0e;

    void appendInner(unsigned int& home, unsigned int value)
    {
        unsigned int index = m_0a;
        if (index >= m_08)
        {
            home = value;
            FUN_0096ad30(index + m_0e);
        }
        else
        {
            home = value;
        }
        FUN_0096ae90(index, &home);
    }
    void append(unsigned int& home, unsigned int value)
    {
        appendInner(home, value);
    }

    void FUN_0096ad30(unsigned int v);
    void FUN_0096ae90(unsigned int v, unsigned int* out);
};

struct StringOwner
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0C;
    unsigned int unk10;
    unsigned int unk14;
    unsigned int unk18;
    unsigned int unk1C;
    unsigned int unk20;
    unsigned int unk24;
    unsigned int unk28;
    unsigned int unk2C;
    unsigned int unk30;
    unsigned int unk34;
    unsigned int unk38;
    unsigned int unk3C;
    unsigned int unk40;
    unsigned int unk44;
    unsigned int unk48;
    unsigned int unk4C;
    unsigned int unk50;
    unsigned int unk54;
    unsigned int unk58;
    unsigned int unk5C;
    unsigned int children[2];

    void WriteToSettingArray(StringArray* apStrings);
    void BSShaderBloodSplatterProperty_GetViewerStrings(unsigned int apStrings);
};

extern unsigned int BSShaderBloodSplatterProperty_ms_RTTI; // src: XBOX_RETAIL_PDB 0x011ff0f4 (old: DAT_011ff0f4/g_011ff0f4)
unsigned int __cdecl NiViewerStrings_NiGetViewerString_Title(unsigned int v);
unsigned int __cdecl BSUtilities_GetTextureName(unsigned int apTexture);
unsigned int __cdecl NiGetViewerString_00a7d5e0(const char* pcPrefix, unsigned int pcValue);
unsigned int __cdecl NiGetViewerString_00a7d5a0(const char* pcPrefix, unsigned int pvValue);
extern const char str_010ae79c[];

// Appends a value and formatted child labels to a StringArray via append.
// @fnv 0x00b81520
// src: XBOX_RETAIL_PDB 0x00b81520
// pdb: BSShaderBloodSplatterProperty::GetViewerStrings | BSShaderBloodSplatterProperty.obj | retail, string-set bridge, high
// pdb params: apStrings
// pdb locals: pname, i
void StringOwner::BSShaderBloodSplatterProperty_GetViewerStrings(unsigned int apStrings)
{
    StringArray* pArray = (StringArray*)apStrings;
    WriteToSettingArray(pArray);
    pArray->append(apStrings, NiViewerStrings_NiGetViewerString_Title(BSShaderBloodSplatterProperty_ms_RTTI));
    for (unsigned int index = 0; index < 2; ++index)
    {
        unsigned int label = children[index];
        label = BSUtilities_GetTextureName(label);
        if (label)
        {
            unsigned int text = NiGetViewerString_00a7d5e0(str_010ae79c, label);
            pArray->append(apStrings, text);
        }
        else
        {
            unsigned int child = children[index];
            unsigned int text = NiGetViewerString_00a7d5a0(str_010ae79c, child);
            pArray->append(apStrings, text);
        }
    }
}
