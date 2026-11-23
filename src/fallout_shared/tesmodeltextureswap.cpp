// fallout_shared/tesmodeltextureswap.cpp: TESModelTextureSwap, NiFixedString, TESModel, DistantTreeTextureInfo (23 functions)

#include "tesmodeltextureswap_decls.h"
#include "../../include/BGSDistantTreeBlock.h"
#include "../../include/TESModelTextureSwap.h"
#include "../../include/BGSEquipType.h"
#include "../../include/BGSMessageIcon.h"
#include "../../include/BaseFormComponent.h"
#include "../../include/TESFullName.h"
#include "../../include/TESIcon.h"
#include "../../include/TESTexture.h"
#include "../../include/TESScriptableForm.h"
#include "../../include/TESWeightForm.h"

// Deletes every entry in the list by taking the head item, removing it, and freeing it, until the list is empty.
// @fnv 0x0048ade0
// src: XBOX_RETAIL_PDB 0x0048ade0
// pdb: TESModelTextureSwap::ClearTexSwapList | TESModelTextureSwap.obj | retail, call graph, high
// pdb locals: pswap
void TESModelTextureSwap::ClearTexSwapList()
{
    void* pDelete;
    BGSPerkEntry* pItem;
    while (!reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(&textureList)->IsEmpty())
    {
        pItem = reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(&textureList)->GetItem();
        reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(&textureList)->RemoveHead();
        pDelete = pItem;
        ::operator delete(pDelete);
    }
}

// Resolves each texture-swap entry's formID to a BGSTextureSet, warns and removes entries that fail to resolve.
// @fnv 0x0048aa80
// pdb: TESModelTextureSwap::InitItem | TESModelTextureSwap.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: plist, plastlist, iid, pswap
void TESModelTextureSwap::InitItem(TESForm* apOwner)
{
    struct InitItemLocals
    {
        unsigned int formID;
        DistantTreeTextureInfo* swap;
        BSSimpleList<PerkButtonEntry*>* list;
    } locals;

    BSSimpleList<PerkButtonEntry*>* previous;

    locals.list =
        reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(reinterpret_cast<TESAIForm*>(this)->GetPackageList());
    previous = 0;
    while (locals.list && !locals.list->IsEmpty())
    {
        locals.swap = reinterpret_cast<DistantTreeTextureInfo*>(
            reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(locals.list)->GetItem());
        locals.formID = locals.swap->formID;
        TESForm::RemapFormIDForFile(locals.formID, apOwner->GetFile(-1));
        locals.swap->formID =
            reinterpret_cast<unsigned int>(dynamic_cast<BGSTextureSet*>(TESForm::GetFormByID(locals.formID)));
        if (!locals.swap->formID)
        {
            Warning(kModelTextureSwapInitWarning, locals.formID, apOwner->GetName(), apOwner->GetFormID());
            if (previous)
            {
                reinterpret_cast<BSSimpleList<TESGrass*>*>(previous)->Remove(reinterpret_cast<TESGrass*&>(locals.swap));
                locals.list = reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(
                    reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(previous)->GetNext());
            }
            else
            {
                locals.list->RemoveHead();
            }
            delete locals.swap;
        }
        else
        {
            previous = locals.list;
            locals.list = reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(
                reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(locals.list)->GetNext());
        }
    }
}

// @fnv 0x0048b130
// pdb: TESModelTextureSwap::SwapTexturesRecurse | TESModelTextureSwap.obj | debug, object-file gap fill, medium
// pdb params: ap3D, apSwapMap, aiGeomIndex
// pdb locals: pnode, icount, i, pchild, pswap, pswapname, pparent, plastParent, parentName, pbssprop, pprop, pnewset
bool TESModelTextureSwap::ApplyTextureSwapToNode(NiAVObject* object, BSMap<int, DistantTreeTextureInfo*>* apSwapMap,
                                                 int& index)
{
    if (object && apSwapMap)
    {
        NiNode* node = object->IsSceneNode();
        if (node)
        {
            unsigned int count = node->FUN_0043b480();
            for (unsigned int i = 0; i < count; i++)
            {
                NiAVObject* child = node->GetAt(i);
                if (child)
                {
                    if (ApplyTextureSwapToNode(child, apSwapMap, index))
                        return true;
                }
            }
        }
        else if (object->IsGeometry())
        {
            DistantTreeTextureInfo* swap = 0;
            if (reinterpret_cast<NiTPointerMap<unsigned int, TESObjectREFR*>*>(apSwapMap)->GetAt(
                    index, reinterpret_cast<TESObjectREFR*&>(swap)) &&
                swap)
            {
                if (reinterpret_cast<BGSTextureSet*>(swap->formID) == BGSTextureSet::GetEmptyTextureSet())
                    reinterpret_cast<NiGeometry*>(object)->SetAppCulled(true);
                else
                {
                    NiProperty* property = object->GetProperty(3);
                    if (BSShaderPPLightingProperty* shader = static_cast<BSShaderPPLightingProperty*>(
                            (!property ? false
                                       : (static_cast<int>(
                                              reinterpret_cast<BGSDehydrationStage*>(property)->FUN_00441110()) >= 8 &&
                                          static_cast<int>(
                                              reinterpret_cast<BGSDehydrationStage*>(property)->FUN_00441110()) <= 12))
                                ? property
                                : 0))
                    {
                        BSShaderTextureSet* textureSet =
                            reinterpret_cast<BGSTextureSet*>(swap->formID)->BuildShaderTextureSet();
                        shader->SetTextureSet(textureSet);
                        shader->BuildShaderTextureSet(0, 0);
                        // The shared shader header does not yet expose virtual slot 0x108.
                        reinterpret_cast<void (__thiscall *)(BSShaderPPLightingProperty*)>((*reinterpret_cast<void***>(shader))[0x108 / 4])(shader);
                    }
                }
                reinterpret_cast<NiTMapBase<DFALL<NiTMapItem<unsigned int, CombatThreatRecord*> >, unsigned int,
                                            CombatThreatRecord*>*>(apSwapMap)
                    ->RemoveAt(index);
                if (!reinterpret_cast<TESForm*>(apSwapMap)->GetFormID())
                    return true;
            }
            index++;
        }
    }
    return false;
}

// @fnv 0x0048afe0
// pdb: TESModelTextureSwap::SwapTextures | TESModelTextureSwap.obj | retail, string-set bridge, high
// pdb params: ap3D
// pdb locals: plist, swapmap, iwarncount, pswap, iindex
void TESModelTextureSwap::ApplyTextureSwaps(NiAVObject* ap3D)
{
    int warningCount;
    if (ap3D)
    {
        warningCount = Warning();
        SetWarningState(true);
        BSMapBase<int, DistantTreeTextureInfo*> swapMap(37);
        {
            BSSimpleList<PerkButtonEntry*>* list =
                reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(reinterpret_cast<TESAIForm*>(this)->GetPackageList());
            while (list && !list->IsEmpty())
            {
                DistantTreeTextureInfo* swap = reinterpret_cast<DistantTreeTextureInfo*>(
                    reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(list)->GetItem());
                list = reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(
                    reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(list)->GetNext());
                if (swap->index != -1)
                    reinterpret_cast<BSMapBase<unsigned int, BGSDistantTreeBlock::InstanceData*>*>(&swapMap)->SetAt(
                        swap->index, reinterpret_cast<BGSDistantTreeBlock::InstanceData*>(swap));
                else
                    Warning("Found invalid swap index.");
            }
        }
        if (reinterpret_cast<TESForm*>(&swapMap)->GetFormID())
        {
            int index = 0;
            ApplyTextureSwapToNode(ap3D, reinterpret_cast<BSMap<int, DistantTreeTextureInfo*>*>(&swapMap), index);
            if (reinterpret_cast<TESForm*>(&swapMap)->GetFormID())
                Warning("Not all texture swaps were performed.");
        }
        SetWarningState(false);
        if (warningCount != Warning())
            Warning(
                "Warnings were encountered during texture swap for model '%s'. Check warnings file for more details.",
                GetModelFilePath());
    }
}

// @fnv 0x0048a990
// src: XBOX_RETAIL_PDB 0x0048a990
// pdb: TESModelTextureSwap::LoadModelTextureSwapChunk | TESModelTextureSwap.obj | retail, object-file run extension, high
// pdb params: apModel, apFile
// pdb locals: chunk, isize, pdata
void TESModelTextureSwap::LoadModelTextureSwapChunk(TESModelTextureSwap* apModel, TESFile* apFile)
{
    if (!apFile || !apModel)
        return;
    int chunk = apFile->GetChunkID();
    switch (chunk)
    {
    case 0x53324f4d:
    case 0x53334f4d:
    case 0x53344f4d:
    case 0x53444f4d: {
        struct ChunkBuffer
        {
            unsigned int size;
            char* data;
        } buffer;
        buffer.size = apFile->GetChunkSize();
        buffer.data = static_cast<char*>(_alloca(buffer.size));
        apFile->GetChunkData(buffer.data, buffer.size);
        apModel->UnpackTextureSwapChunkData(buffer.data, buffer.size,
                                    apFile->GetFormVersion());
    }
    break;
    default:
        if (TESModel_IsModelChunk(chunk))
            TESModel::LoadModelData(reinterpret_cast<TESModel*>(apModel), apFile);
        break;
    }
}

// @fnv 0x004892d0
void TESModel::LoadModelData(TESModel* model, TESFile* file)
{
    if (!file || !model)
        return;
    int chunk = file->GetChunkID();
    switch (chunk)
    {
    case 0x4c444f4d:
    case 0x4d414e50: {
        char* data = static_cast<char*>(_alloca(file->GetChunkSize()));
        file->GetChunkData(data, 0);
        model->SetModelFilePath(data);
    }
    break;
    case 0x54444f4d:
        LoadTextureSwapData(model, file);
        break;
    case 0x44444f4d:
    case 0x44534f4d: {
        unsigned char flags;
        file->GetChunkData(&flags, 1);
        model->SetFlags_004893c0(flags);
    }
    break;
    }
}

// @fnv 0x0048ae30
// pdb: TESModelTextureSwap::AddTexSwap | TESModelTextureSwap.obj | retail, upstream name already matches, high
// pdb params: apObjName, aiIndex, apTexSet
// pdb locals: pswap
void TESModelTextureSwap::AddTexSwap(const char* apObjName, int aiIndex, BGSTextureSet* apTexSet)
{
    if (apObjName && apTexSet)
    {
        DistantTreeTextureInfo* swap = GetTexSwap(aiIndex);
        if (!swap)
        {
            swap = new DistantTreeTextureInfo;
            TaskQueueInterface::CopyStringToBuffer(swap->objectName, 128, apObjName);
            swap->index = aiIndex;
            reinterpret_cast<BSSimpleList<SpellItem*>*>(&textureList)
                ->AddHead(reinterpret_cast<SpellItem*&>(swap));
        }
        swap->formID = reinterpret_cast<unsigned int>(apTexSet);
    }
}

// Walks the package list and returns the entry whose index matches the given index, or null if index is -1 or no match is found.
// @fnv 0x0048ad70
// pdb: TESModelTextureSwap::GetTexSwap | TESModelTextureSwap.obj | debug, upstream name already matches, high
// pdb params: aiIndex
// pdb locals: pretswap, plist
DistantTreeTextureInfo* TESModelTextureSwap::GetTexSwap(int aiIndex)
{
    DistantTreeTextureInfo* result = 0;
    if (aiIndex != -1)
    {
        BSSimpleList<PerkButtonEntry*>* list =
            reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(reinterpret_cast<TESAIForm*>(this)->GetPackageList());
        while (list && !list->IsEmpty())
        {
            if (aiIndex == reinterpret_cast<DistantTreeTextureInfo*>(
                             reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(list)->GetItem())
                             ->index)
            {
                result = reinterpret_cast<DistantTreeTextureInfo*>(
                    reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(list)->GetItem());
                break;
            }
            list = reinterpret_cast<BSSimpleList<PerkButtonEntry*>*>(
                reinterpret_cast<BSSimpleList<BGSPerkEntry*>*>(list)->GetNext());
        }
    }
    return result;
}

// Initializes formID to 0, index to -1, and clears the first character of objectName.
// @fnv 0x0048af10
DistantTreeTextureInfo::DistantTreeTextureInfo()
{
    formID = 0;
    index = -1;
    objectName[0] = 0;
}

// Copy constructor: retains the other string handle, then copies it.
// @fnv 0x0048af40
// pdb: NiFixedString::NiFixedString | TESModelTextureSwap.obj | debug, call graph, medium
// pdb params: kString
NiFixedString::NiFixedString(const NiFixedString& other)
{
    NiGlobalStringTable_IncRefCount(other);
    m_kHandle = other.m_kHandle;
}

// @fnv 0x0048af70
int NiFixedString::IsNotEmpty() const
{
    return m_kHandle != EmptyFS.m_kHandle;
}

// @fnv 0x0048b2f0
// pdb: BSShaderPPLightingProperty::SetTextureSet | TESModelTextureSwap.obj | debug, upstream name already matches, high
// pdb params: apTextureSet
void BSShaderPPLightingProperty::SetTextureSet(BSTextureSet* apTextureSet)
{
    *reinterpret_cast<NiPointer<NiNode>*>(&spTextureSet) = reinterpret_cast<NiNode*>(apTextureSet);
}

extern "C" int __cdecl _NiStricmp(const char*, const char*);

struct FUN_0048af90_shell
{
    const char* m_str;
    bool Equals(const char* a1);
};

// Returns true if m_str and a1 are the same pointer, false if either is null, otherwise compares them case-insensitively with _NiStricmp.
// @fnv 0x0048af90
// pdb: NiFixedString::EqualsNoCase | TESModelTextureSwap.obj | retail, call graph, medium
// pdb params: pcStr
bool FUN_0048af90_shell::Equals(const char* apOther)
{
    if (m_str == apOther)
        return true;
    if (apOther == 0 || m_str == 0)
        return false;
    return _NiStricmp(m_str, apOther) == 0;
}

struct FUN_00409aa0_shell
{
    void IngredientItem_SetFlag(unsigned int flags, bool set);
};

// Sets or clears the given flag bits in the byte at offset 0xa0 of this object.
// @fnv 0x00409aa0
// src: COMMUNITY 0x00409aa0
void FUN_00409aa0_shell::IngredientItem_SetFlag(unsigned int flags, bool set)
{
    set ? ((unsigned char*)this)[0xa0] |= flags : ((unsigned char*)this)[0xa0] &= ~flags;
}

struct BipedAnim;
struct NiPoint3;
struct Script;
struct TESBoundObject;
struct TESObject;
struct TESFormList;
struct TESObjectREFR;
struct TESWaterForm;
struct blob12
{
    char b[12];
};
typedef char _sizecheck_BaseFormComponent[1];
typedef char _sizecheck_TESFullName[1];
typedef char _sizecheck_TESTexture[1];
typedef char _sizecheck_TESIcon[1];
typedef char _sizecheck_BGSMessageIcon[1];
typedef char _sizecheck_TESScriptableForm[1];
typedef char _sizecheck_TESWeightForm[1];
typedef char _sizecheck_BGSEquipType[1];

// @fnv 0x0048a470
// pdb: TESModelTextureSwap::InitializeDataComponent | TESModelTextureSwap.obj | debug, vtable slot, medium
void TESModelTextureSwap::InitializeComponent()
{
    TESModel::InitializeComponent();
}

// @fnv 0x0048a500
// pdb: TESModelTextureSwap::ClearDataComponent | TESModelTextureSwap.obj | retail, vtable slot, high
void TESModelTextureSwap::Free()
{
    ClearTexSwapList();
    TESModel::Free();
}

void* __cdecl __memcpy(void*, const void*, unsigned int);
// Reads a count-prefixed list of texture swaps (name, texture pointer, optional geometry index) from a buffer and adds each valid entry via AddTexSwap.
// @fnv 0x0048a7b0
// src: XBOX_RETAIL_PDB 0x0048a7b0
// pdb: TESModelTextureSwap::UnpackTextureSwapChunkData | TESModelTextureSwap.obj | retail, call graph (shared callers), high
// pdb params: apData, aiDataSize, aiVersion
// pdb locals: icount, pdata, ireadsize, isize, igeomindex, pname, pnewtexture, iid
bool TESModelTextureSwap::UnpackTextureSwapChunkData(void* apData, unsigned int aiDataSize, unsigned int aiVersion)
{
    unsigned int uiRemaining;
    unsigned char* pdata;
    unsigned int ireadsize;
    if (apData && aiDataSize > 4)
    {
        pdata = (unsigned char*)apData;
        uiRemaining = *(unsigned int*)pdata;
        ireadsize = 4;
        pdata += ireadsize;
        aiDataSize -= ireadsize;
        while (uiRemaining)
        {
            uiRemaining--;
            unsigned int uiNameLength = 0;
            BGSTextureSet* pnewtexture = 0;
            int igeomindex = -1;
            char acName[260];
            acName[0] = 0;
            ireadsize = 4;
            if (ireadsize <= aiDataSize)
            {
                uiNameLength = *(unsigned int*)pdata;
                pdata += ireadsize;
                aiDataSize -= ireadsize;
            }
            else
            {
                return false;
            }
            ireadsize = uiNameLength;
            if (ireadsize <= aiDataSize && ireadsize < 0x104)
            {
                __memcpy(acName, pdata, uiNameLength);
                acName[uiNameLength] = 0;
                pdata += ireadsize;
                aiDataSize -= ireadsize;
            }
            else
            {
                return false;
            }
            ireadsize = 4;
            if (ireadsize <= aiDataSize)
            {
                unsigned int uiTexturePtr = *(unsigned int*)pdata;
                pdata += ireadsize;
                aiDataSize -= ireadsize;
                pnewtexture = (BGSTextureSet*)uiTexturePtr;
            }
            else
            {
                return false;
            }
            if (aiVersion >= 15)
            {
                ireadsize = 4;
                if (ireadsize <= aiDataSize)
                {
                    igeomindex = *(int*)pdata;
                    pdata += ireadsize;
                    aiDataSize -= ireadsize;
                }
                else
                {
                    return false;
                }
            }
            if (acName[0] && pnewtexture)
                AddTexSwap(acName, igeomindex, pnewtexture);
        }
    }
    return true;
}

void __cdecl AppendChunk(unsigned int, unsigned int, unsigned int);
void __cdecl Deallocate(void*);

// Calls WriteToBuffer with a and b, then if SerializeList yields a data pointer and size, passes them with c to AppendChunk and deallocates the pointer.
// @fnv 0x0048a520
// pdb: TESModelTextureSwap::Save | TESModelTextureSwap.obj | retail, call graph (shared callers), high
// pdb params: aeModelID, aeTextureID, aeSwapID
// pdb locals: pdata, idatasize
void TESModelTextureSwap::ProcessAndFreeData(unsigned int aeModelID, unsigned int aeTextureID, unsigned int aeSwapID)
{
    WriteToBuffer(aeModelID, aeTextureID);
    unsigned int pdata = 0;
    unsigned int idatasize = 0;
    if (SerializeList(reinterpret_cast<char**>(&pdata), &idatasize))
    {
        AppendChunk(aeSwapID, pdata, idatasize);
        Deallocate((void*)pdata);
    }
}

// @fnv 0x0048a490
// pdb: TESModelTextureSwap::~TESModelTextureSwap | TESModelTextureSwap.obj | retail, upstream name already matches, high
TESModelTextureSwap::~TESModelTextureSwap()
{
    Free();
}

extern "C" char DAT_01183040;
extern "C" char DAT_011831c4;
void* __cdecl FUN_00ec43fb(unsigned int, int, const void*, const void*, int);
struct TextureSwapRecord
{
    unsigned int formID;
    int index;
    char objectName[128];
};
struct TextureSwapNode
{
    unsigned char FUN_008256d0();
    TextureSwapRecord** FUN_006815c0();
    TextureSwapNode* FUN_00726070();
};
struct TextureSwapNodeAccessor
{
    TextureSwapNode* FUN_00500940();
};

// Looks up a texture swap list for formID, then for each node applies its record's objectName, index and formID via FUN_0048ae30.
// @fnv 0x0048aba0
// pdb: TESModelTextureSwap::CopyComponent | TESModelTextureSwap.obj | retail, vtable slot, high
// pdb params: apCopyFC
// pdb locals: plist, pcopy, pswap
void TESModelTextureSwap::ApplyTextureSwapOverrides(unsigned int apCopyFC)
{
    TESModel::BaseFormRecord(reinterpret_cast<BaseFormComponent*>(apCopyFC));
    struct TextureSwapNodeAccessor* list =
        static_cast<struct TextureSwapNodeAccessor*>(FUN_00ec43fb(apCopyFC, 0, &DAT_01183040, &DAT_011831c4, 0));
    if (!list)
        return;
    ClearTexSwapList();
    TextureSwapNode* node = list->FUN_00500940();
    while (node && !node->FUN_008256d0())
    {
        TextureSwapRecord* record = *node->FUN_006815c0();
        AddTexSwap(record->objectName, record->index, reinterpret_cast<BGSTextureSet*>(record->formID));
        node = node->FUN_00726070();
    }
}

struct TextureSwapListHead
{
    TextureSwapNode* FUN_00500940();
};
struct TextureSwapCount
{
    int GetSize();
};
int BSstricmp(const char*, const char*);

// @fnv 0x0048ac40
// pdb: TESModelTextureSwap::CompareComponent | TESModelTextureSwap.obj | retail, vtable slot, high
// pdb params: apCompareFC
// pdb locals: pcomp, plist, pswap, pcompswap
bool TESModelTextureSwap::CompareTextureSwaps(BaseFormComponent* apCompareFC)
{
    if (TESModel::CompareWithBase(apCompareFC))
        return true;
    TESModelTextureSwap* pOther = dynamic_cast<TESModelTextureSwap*>(apCompareFC);
    if (!pOther)
        return true;
    if (((TextureSwapCount*)((TextureSwapListHead*)pOther)->FUN_00500940())->GetSize() !=
        ((TextureSwapCount*)((TextureSwapListHead*)this)->FUN_00500940())->GetSize())
        return true;
    TextureSwapNode* pNode = ((TextureSwapListHead*)pOther)->FUN_00500940();
    while (pNode && !pNode->FUN_008256d0())
    {
        TextureSwapRecord* pSwap = *pNode->FUN_006815c0();
        TextureSwapRecord* pMine = reinterpret_cast<TextureSwapRecord*>(GetTexSwap(pSwap->index));
        if (!pMine)
            return true;
        if (pMine->formID != pSwap->formID)
            return true;
        if (pMine->objectName[0] && pSwap->objectName[0] && BSstricmp(pMine->objectName, pSwap->objectName))
            return true;
        pNode = pNode->FUN_00726070();
    }
    return false;
}

class TextureSwapSerializationNode
{
  public:
    bool FUN_008256d0();
    unsigned int* FUN_006815c0();
    TextureSwapSerializationNode* FUN_00726070();
};

class ObjectRef
{
  public:
    int GetHavokWorldObjectCountInSceneGraph_0084e3a0();
};

char* __cdecl FormHeap_Allocate(unsigned int);
unsigned int __cdecl BSstrlen(unsigned int);
bool __cdecl TESForm_QEndianSwapOnSave();
struct FUN_00401080_tag
{
};
void __cdecl BSEndian_00401080(int*, FUN_00401080_tag);
void __cdecl FUN_00401460(char*, unsigned int, unsigned int);

// @fnv 0x0048a590
// pdb: TESModelTextureSwap::BuildTextureSwapChunkData | TESModelTextureSwap.obj | debug, object-file gap fill, medium
// pdb params: apOutData, aiOutDataSize
// pdb locals: plist, icount, itempint, pdata, pswap, iid, isize
bool TESModelTextureSwap::SerializeList(char** apBuffer, unsigned int* auiSize)
{
    *apBuffer = 0;
    *auiSize = 0;
    TextureSwapSerializationNode* iT = reinterpret_cast<TextureSwapSerializationNode*>(reinterpret_cast<TESAIForm*>(this)->GetPackageList());
    unsigned int v;
    unsigned int P;
    unsigned int r;
    unsigned int t;
    int count;
    int loA2;
    char* _w;
    if (!iT->FUN_008256d0())
    {
        count = 0;
        *auiSize = 4;
        while (iT != 0 && !iT->FUN_008256d0())
        {
            v = *iT->FUN_006815c0();
            *auiSize += 4;
            *auiSize += BSstrlen(v + 8);
            *auiSize += 4;
            *auiSize += 4;
            count++;
            iT = iT->FUN_00726070();
        }
        *apBuffer = FormHeap_Allocate(*auiSize);
        _w = *apBuffer;
        loA2 = count;
        if (TESForm_QEndianSwapOnSave())
        {
            BSEndian_00401080(&loA2, FUN_00401080_tag());
        }
        *(int*)_w = loA2;
        _w += 4;
        iT = reinterpret_cast<TextureSwapSerializationNode*>(reinterpret_cast<TESAIForm*>(this)->GetPackageList());
        while (iT != 0 && !iT->FUN_008256d0())
        {
            P = *iT->FUN_006815c0();
            r = BSstrlen(P + 8);
            if (TESForm_QEndianSwapOnSave())
            {
                BSEndian_00401080(&loA2, FUN_00401080_tag());
            }
            *(unsigned int*)_w = r;
            _w += 4;
            FUN_00401460(_w, P + 8, r);
            _w += r;
            t = 0;
            if (*(unsigned int*)P != 0)
                t = ((ObjectRef*)*(unsigned int*)P)->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
            *(unsigned int*)_w = t;
            _w += 4;
            *(unsigned int*)_w = *((unsigned int*)P + 1);
            _w += 4;
            iT = iT->FUN_00726070();
        }
    }
    return *apBuffer != 0;
}

