// fallout_shared/tesobjectanio.cpp: TESObjectANIO (6 functions)

#include "tesobjectanio_decls.h"

// @fnv 0x00511a90
// pdb: TESObjectANIO::InitItem | TESObjectANIO.obj | retail, upstream name already matches, high
// pdb locals: itempint
void TESObjectANIO::InitItem()
{
    if (IsFormInitialized())
        return;
    ((TESModelTextureSwap*)((char*)this + 0x18))->InitItem(this);
    if (pIdleAnim)
    {
        unsigned int formID = (unsigned int)pIdleAnim;
        TESForm::RemapFormIDForFile(formID, GetFile(-1));
        pIdleAnim = dynamic_cast<TESObjectANIO*>(TESForm::GetFormByID(formID));
    }
    SetInitializationState(true);
}

// @fnv 0x00511b20
// pdb: TESObjectANIO::Load | TESObjectANIO.obj | retail, vtable slot, high
// pdb params: apFile
// pdb locals: echunkid, formid, pMessage, isize, pdata, itempint
bool TESObjectANIO::LoadForm(TESFile* apFile)  // TESForm vslot 8
{
    unsigned char formType = apFile->GetTESForm();
    if (formType != 0x4d)
        return false;
    LoadRecordHeader(apFile);
    int chunk;
    while (chunk = apFile->GetChunkID())
    {
        switch (chunk)
        {
        case 0x44494445: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c444f4d:
        case 0x54444f4d:
        case 0x44444f4d:
        case 0x44534f4d:
            TESModel::LoadModelData(this, apFile);
            break;
        case 0x53444f4d: {
            unsigned int size = apFile->GetChunkSize();
            void* textureData = _alloca(size);
            apFile->GetChunkData(textureData, size);
            UnpackTextureSwapChunkData(textureData, size, apFile->GetFormVersion());
            break;
        }
        case 0x41544144: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            pIdleAnim = (TESObjectANIO*)formID;
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

struct SFTX
{
    void SetFormType(int);
};
struct B2_4fe
{
    B2_4fe();
    virtual ~B2_4fe();
    unsigned char pad[0x1c];
};

struct FUN_00511950 : public TESForm, public B2_4fe
{
    int pAnim;
    FUN_00511950();
    virtual ~FUN_00511950();
};

// Sets the form type to 0x4d and initializes pAnim to 0.
// @fnv 0x00511950
// pdb: TESObjectANIO::TESObjectANIO | TESObjectANIO.obj | retail, call graph, high
FUN_00511950::FUN_00511950()
{
    ((SFTX*)this)->SetFormType(0x4d);
    pAnim = 0;
}


struct FUN_005119e0_obj
{
    void FUN_00511a10();
    void* Destroy(unsigned int flags);
};

// Calls FUN_00511a10, then deletes this if flags has bit 0 set, and returns this.
// @fnv 0x005119e0
// pdb: TESObjectANIO::`scalar deleting destructor' | TESObjectANIO.obj | retail, vtable slot, high
void* FUN_005119e0_obj::Destroy(unsigned int flags)
{
    FUN_00511a10();
    if (flags & 1)
        operator delete(this);
    return this;
}

struct FUN_004855a0
{
    void __thiscall M();
};
struct FUN_00485680
{
    void __thiscall M();
};
struct S_0048a520
{
    void __thiscall ProcessAndFreeData(unsigned int, unsigned int, unsigned int);
};
struct _ref_0084e3a0_t
{
    unsigned int __thiscall _ref_0084e3a0();
};
unsigned int __cdecl _FUN_00485910(unsigned int, unsigned int);

struct FUN_00511d10_shell
{
    void __thiscall TESObjectANIO_Save();
};

// @fnv 0x00511d10
// src: XBOX_PDB 0x00511d10
// pdb: TESObjectANIO::Save | TESObjectANIO.obj | retail, upstream name already matches, high
// pdb locals: iid
void __thiscall FUN_00511d10_shell::TESObjectANIO_Save()
{
    ((FUN_004855a0*)this)->M();
    ((S_0048a520*)((char*)this + 0x18))->ProcessAndFreeData(0x4c444f4d, 0x54444f4d, 0x53444f4d);
    if (((int*)this)[0x38 / 4])
    {
        unsigned int iid = ((_ref_0084e3a0_t*)(((int*)this)[0x38 / 4]))->_ref_0084e3a0();
        _FUN_00485910(0x41544144, iid);
    }
    ((FUN_00485680*)this)->M();
}

class B511A10Form
{
  public:
    virtual ~B511A10Form();

    unsigned char m_cFormType;
    unsigned int m_iFormFlags;
    unsigned int m_iFormID;
    void* m_pkSourceFile;
    void* m_pkNextSourceFile;
};

class B511A10TextureSwap
{
  public:
    virtual ~B511A10TextureSwap();

    const char* m_pcModel;
    unsigned short m_usLength;
    unsigned short m_usMaxLength;
    float m_fBound;
    void* m_pkTextures;
    unsigned int m_uiValue1;
    void* m_pkSwapItem;
    void* m_pkSwapNext;
};

class FUN_00511a10_cls : public B511A10Form, public B511A10TextureSwap
{
  public:
    virtual ~FUN_00511a10_cls();
    void ClearData();
};

// @fnv 0x00511a10
// pdb: TESObjectANIO::~TESObjectANIO | TESObjectANIO.obj | debug, object-file gap fill, medium
FUN_00511a10_cls::~FUN_00511a10_cls()
{
    ClearData();
}
