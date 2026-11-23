// fallout_shared/bgscamerapath.cpp: BGSCameraPath and related functions (24 functions)

// Keep the unit support types while using the shared form hierarchy.
#include "layout_support.h"
#define FNV64_BSSIMPLELIST_DEFINED
#include "../../include/BGSCameraShot.h"
#define FNV_BASEFORMCOMPONENT_H
#define FNV_TESFORM_H
#include "../../include/BGSAcousticSpace.h"
#include "../../include/BGSImpactData.h"
#include "../../include/NiFormArray.h"
#include "../../include/CameraShotData.h"
#include "../../include/BGSCameraPath.h"
#include "bgscamerapath.h"
#include "../../include/Script.h"

// @fnv 0x0058a620
void BGSCameraPath::ResetFormData()
{
    data.cFlags = 0;
    pParentPath = 0;
    pPrevPath = 0;
}

// @fnv 0x005f4bb0
void BGSCameraPath::AssignParentPath(BGSCameraPath* parent)
{
    pParentPath = parent;
}

// @fnv 0x0058a650
// pdb: BGSCameraPath::ClearData | BGSCameraPath.obj | retail, upstream name already matches, high
void BGSCameraPath::ClearData()
{
    GetConditions()->InitializeConditionItems();
    ShotList.RemoveAll();
}

// @fnv 0x0058a380
// pdb: BGSCameraPath::BGSCameraPath | BGSCameraPath.obj | retail, upstream name already matches, high
BGSCameraPath::BGSCameraPath()
{
    SetFormType(ENUM_FORM_ID_CPTH_ID);
    pChildPaths = 0;
    ResetFormData();
}

extern int __cdecl Warning(const char* format, ...);
extern "C" const char fnv_MessageWithName[];
extern "C" const char fnv_MessageWithFormID[];

// Initializes conditions and walks the shot list, resolving each form ID to a BGSCameraShot and removing entries that fail to resolve, warning for each.
// @fnv 0x0058ad90
// pdb: BGSCameraPath::InitItem | BGSCameraPath.obj | retail, string-set bridge, high
// pdb locals: plist, plastlist, pshot, tempint
void BGSCameraPath::InitItem()
{
    if (IsFormInitialized())
        return;

    ((TESConditionInit*)GetConditions())->InitItem(this);

    BSSimpleList<BGSCameraShot*>* pShotNode = GetShotList();
    BSSimpleList<BGSCameraShot*>* previous = 0;
    struct InitItemLocals
    {
        BGSCameraShot* cameraShot;
        unsigned int formID;
    } shotEntry;
    while (pShotNode && !pShotNode->IsEmpty())
    {
        shotEntry.formID = (unsigned int)pShotNode->GetItem();
        TESForm::RemapFormIDForFile(shotEntry.formID, GetFile(-1));
        shotEntry.cameraShot = dynamic_cast<BGSCameraShot*>(TESForm::GetFormByID(shotEntry.formID));
        if (!shotEntry.cameraShot)
        {
            if (previous)
            {
                previous->Remove(pShotNode->GetItem());
                pShotNode = previous->GetNext();
            }
            else
                pShotNode->RemoveHead();

            if (HasNameForWarning())
                Warning(fnv_MessageWithName, shotEntry.formID, GetName());
            else
                Warning(fnv_MessageWithFormID, shotEntry.formID, GetFormID());
        }
        else
        {
            pShotNode->SetItem(shotEntry.cameraShot);
            previous = pShotNode;
            pShotNode = pShotNode->GetNext();
        }
    }

    SetInitializationState(true);
}

extern "C" const char fnv_MessageMultipleNext[];

// @fnv 0x0058b190
// src: XBOX_RETAIL_PDB 0x0058b190
// pdb: BGSCameraPath::FixNextPaths | BGSCameraPath.obj | retail, string-set bridge, high
// pdb params: apArray
// pdb locals: izerocount, inextindex, iindex, pnextpath, isize, i, ppath, tempstring
void BGSCameraPath::FixNextPaths(NiFormArray* apArray)
{
    if (!apArray)
        return;

    struct FixNextLocals
    {
        unsigned int index;
        unsigned int invalidCount;
        unsigned int nextIndex;
        unsigned int currentIndex;
        BGSCameraPath* nextPath;
        unsigned int pathCount;
    } locals;

    locals.invalidCount = 0;
    locals.currentIndex = 0xffffffff;
    locals.pathCount = apArray->GetSize();
    locals.nextIndex = 0xffffffff;
    locals.nextPath = 0;
    locals.index = 0;

    for (; locals.index < locals.pathCount; ++locals.index)
    {
        BGSCameraPath* path = dynamic_cast<BGSCameraPath*>(apArray->FUN_00877a30(locals.index));
        if (path)
        {
            if (path == this)
                locals.currentIndex = locals.index;
            else if (path->GetPrevPath() == this)
            {
                if (!locals.nextPath)
                {
                    locals.nextPath = path;
                    locals.nextIndex = locals.index;
                }
                else
                {
                    BSStringT<char> name;
                    GetDebugName(&name);
                    Warning(fnv_MessageMultipleNext, name.String());
                }
            }
        }
        else
            ++locals.invalidCount;
    }

    if (locals.currentIndex != 0xffffffff && locals.nextIndex != 0xffffffff &&
        locals.nextIndex != locals.currentIndex + 1)
    {
        unsigned int oldSize = 0;
        apArray->SetSize(locals.nextIndex, &oldSize);
        apArray->Compact();
        if (locals.nextIndex < locals.currentIndex)
            ++locals.invalidCount;
        locals.currentIndex -= locals.invalidCount;
        apArray->SetFormAtIndex(locals.currentIndex + 1, locals.nextPath);
        apArray->Compact();
        locals.nextPath->FixNextPaths(apArray);
    }
}

extern "C" void* __cdecl _alloca(size_t);
#pragma intrinsic(_alloca)
extern "C" void* __cdecl memset(void*, int, size_t);
// @fnv 0x0058a9e0
// pdb: BGSCameraPath::Load | BGSCameraPath.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, iindex, parray, pMessage, itempint, ids
bool BGSCameraPath::Load(TESFile* apFile)
{
    int chunk;
    if (((CameraFileFormType*)apFile)->GetTESForm() != 0x5c)
        return false;
    if (this == GetTextureResourcePath())
    {
        struct
        {
            unsigned int index;
            NiFormArray* paths;
        } removalInfo;
        removalInfo.paths = DAT_011ca700->GetPathArray();
        removalInfo.index = removalInfo.paths->GetIndexOfForm(this);
        if (removalInfo.index != 0xffffffff)
        {
            ((NiTLargeArray<TESTopicInfo*, NiTMallocInterface<TESTopicInfo*> >*)removalInfo.paths)
                ->RemoveAt(removalInfo.index);
            removalInfo.paths->Compact();
        }
    }
    LoadRecordHeader(apFile);
    while ((chunk = apFile->GetChunkID()) != 0)
    {
        switch (chunk)
        {
        case 0x44494445: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 512);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x41544144:
            ((BGSImpactData*)this)->ReadTypedData(apFile, &data, 1);
            break;
        case 0x41445443:
            ((CameraCondition*)GetConditions())->Load(apFile);
            break;
        case 0x4d414e53: {
            unsigned int formID = 0;
            apFile->GetChunkData(&formID);
            ((BSSimpleList<ScriptVariableInfo*>*)&ShotList)->AddTail((ScriptVariableInfo*)formID);
            break;
        }
        case 0x4d414e41: {
            FUN_005a71d0 parentInfo;
            memset(&parentInfo, 0, 8);
            apFile->GetChunkData(&parentInfo, 8);
            if (apFile->IsEndianConversionRequired())
                parentInfo.TwoValues_Endian();
            pParentPath = (BGSCameraPath*)parentInfo.parent;
            pPrevPath = (BGSCameraPath*)parentInfo.previous;
            DAT_011ca700->AddCameraPath(this);
            break;
        }
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

extern "C" const char fnv_MessageMissingParent[];
extern "C" const char fnv_MessageMissingPrevious[];
extern "C" const char fnv_MessageMissingArray[];
// Resolves the parent and previous camera paths by form ID, warns if missing, and inserts this path into the parent's or global path array at the computed index.
// @fnv 0x0058aef0
// src: XBOX_RETAIL_PDB 0x0058aef0
// pdb: BGSCameraPath::InitParents | BGSCameraPath.obj | retail, string-set bridge, high
// pdb locals: pparent, itempint, iindex, pprev, parray, bretval, tempstring, iprevindex
bool BGSCameraPath::InitParents()
{
    struct ParentLocals
    {
        BGSCameraPath* parent;
        unsigned int formID;
        unsigned int index;
        BGSCameraPath* previous;
        NiFormArray* paths;
        char pad0, pad1, pad2;
        bool success;
    } locals;
    locals.success = true;
    locals.parent = 0;
    locals.formID = (unsigned int)pParentPath;
    if (locals.formID)
    {
        TESForm::RemapFormIDForFile(locals.formID, GetFile(-1));
        locals.parent = dynamic_cast<BGSCameraPath*>(TESForm::GetFormByID(locals.formID));
        if (!locals.parent)
        {
            BSStringT<char> name;
            GetDebugName(&name);
            Warning(fnv_MessageMissingParent, locals.formID, name.String());
            locals.success = false;
        }
        AssignParentPath(locals.parent);
    }
    locals.previous = 0;
    locals.formID = (unsigned int)pPrevPath;
    if (locals.formID)
    {
        TESForm::RemapFormIDForFile(locals.formID, GetFile(-1));
        locals.previous = dynamic_cast<BGSCameraPath*>(TESForm::GetFormByID(locals.formID));
        if (!locals.previous)
        {
            BSStringT<char> name;
            GetDebugName(&name);
            Warning(fnv_MessageMissingPrevious, locals.formID, name.String());
            locals.success = false;
        }
        ((BGSAcousticSpace*)this)->SetDawnSound((TESSound*)locals.previous);
    }
    locals.index = 0;
    locals.paths = 0;
    if (locals.parent)
        locals.paths = locals.parent->CreateChildArray();
    else
        locals.paths = DAT_011ca700->GetPathArray();
    if (locals.paths)
    {
        if (locals.previous)
        {
            unsigned int previousIndex = locals.paths->GetIndexOfForm(locals.previous);
            if (previousIndex != 0xffffffff)
                ++previousIndex;
            locals.index = previousIndex;
        }
        if (locals.index == 0xffffffff || IsDeleted())
            locals.index = locals.paths->GetSize() + 1;
        locals.paths->SetFormAtIndex(locals.index, this);
        FixNextPaths(locals.paths);
    }
    else
    {
        BSStringT<char> name;
        GetDebugName(&name);
        Warning(fnv_MessageMissingArray, name.String());
        locals.success = false;
    }
    return locals.success;
}

// @fnv 0x0058b3f0
// pdb: NiFormArray::NiFormArray | BGSCameraPath.obj | debug, upstream name already matches, high
NiFormArray::NiFormArray() : NiTLargePrimitiveArray<TESForm*>(0, 1)
{}

// Returns the number of child paths, or 0 if there is no child array.
// @fnv 0x0058b600
// pdb: BGSCameraPath::GetChildCount | BGSCameraPath.obj | debug, upstream name already matches, high
// pdb locals: iretval
unsigned int BGSCameraPath::GetChildCount()
{
    unsigned int result = 0;
    if (pChildPaths)
        result = pChildPaths->GetSize();
    return result;
}

// @fnv 0x0058b450
// pdb: NiFormArray::~NiFormArray | BGSCameraPath.obj | debug, call graph, medium
void __fastcall DestroyNiFormArray(NiFormArray* self)
{
    self->NiTLargePrimitiveArray<TESForm*>::~NiTLargePrimitiveArray();
}

// @fnv 0x0058b4f0
bool __thiscall BGSCameraPath::IsFlag2Set()
{
    if (data.cFlags & 2)
        return true;
    return false;
}

// Returns true when the cFlags bit 0x80 is set, otherwise false.
// @fnv 0x0058b470
bool BGSCameraPath::IsFlag80Set()
{
    if (data.cFlags & 0x80)
        return true;
    else
        return false;
}

// Returns true when neither bit 0 nor bit 1 of cFlags is set, otherwise false.
// @fnv 0x0058b4a0
// src: COMMUNITY 0x0058b4a0
bool BGSCameraPath::GetZoomDefault()
{
    if (data.cFlags & 1 || data.cFlags & 2)
        return false;
    return true;
}

// @fnv 0x0058b4d0
// src: COMMUNITY 0x0058b4d0
bool BGSCameraPath::GetZoomDisable()
{
    if (data.cFlags & 1)
        return true;
    return false;
}

struct IdentifierHistoryEntry
{
    unsigned int m_uiCurrentId;
    unsigned int ipreviousID;
};

// TESConditionSave_bgscamerapath: local type (was include/TESConditionSave.h, which had no evidence of one real class); its methods are 0x00680910 TESCondition::Save
class TESConditionSave_bgscamerapath
{
public:
    void Save();
};
struct BSSimpleList_shell
{
    void** GetItem();
    BSSimpleList_shell* GetNext();
    bool _ref_008256d0();
};

struct FUN_00485e00_shell
{
    void __thiscall TESForm___SaveData(int, int);
};

bool _FUN_00401500();
void _FUN_00485990(unsigned int, void*, unsigned int);
void _FUN_00485910(unsigned int, unsigned int);

// @fnv 0x0058ac10
// src: XBOX_PDB 0x0058ac10
// pdb: BGSCameraPath::Save | BGSCameraPath.obj | retail, vtable slot, high
// pdb locals: plist, ids, pprev, pshot, itempint
void BGSCameraPath::Save_0058ac10()
{
    BSSimpleList_shell* pShotList;
    IdentifierHistoryEntry ids;

    {
        BGSCameraPath* pprev;
        BGSCameraShot* pshot;
        unsigned int shotFormID;

        TESForm::BeginFormRecord();
        reinterpret_cast<TESConditionSave_bgscamerapath*>(GetConditions())->Save();

        memset(&ids, 0, 8);

        if (reinterpret_cast<Script*>(this)->GetData())
            ids.m_uiCurrentId = reinterpret_cast<TESForm*>(reinterpret_cast<Script*>(this)->GetData())->GetFormID();

        if (!IsDeleted())
        {
            pprev = GetPrevPath();
            while (pprev && pprev->IsDeleted())
                pprev = pprev->GetPrevPath();

            if (pprev)
                ids.ipreviousID = pprev->GetFormID();
        }

        if (_FUN_00401500())
            reinterpret_cast<FUN_005a71d0*>(&ids)->TwoValues_Endian();

        _FUN_00485990(0x4d414e41, &ids, 8);

        if (_FUN_00401500())
            reinterpret_cast<FUN_005a71d0*>(&ids)->TwoValues_Endian();

        if (_FUN_00401500())
            reinterpret_cast<TESForm*>(&data)->TESForm::ClearData();

        reinterpret_cast<FUN_00485e00_shell*>(this)->TESForm___SaveData(reinterpret_cast<int>(&data), 1);

        if (_FUN_00401500())
            reinterpret_cast<TESForm*>(&data)->TESForm::ClearData();

        pShotList = reinterpret_cast<BSSimpleList_shell*>(reinterpret_cast<TESForm*>(this)->TESForm::GetName());

        while (pShotList && !pShotList->_ref_008256d0())
        {
            pshot = reinterpret_cast<BGSCameraShot*>(pShotList->GetItem()[0]);
            pShotList = pShotList->GetNext();
            shotFormID = pshot->GetFormID();
            _FUN_00485910(0x4d414e53, shotFormID);
        }
    }

    TESForm::FinishFormRecord();
}

void* __cdecl __memcpy(void*, const void*, unsigned int);

struct Cond_58a680
{
    void Copy(PerkEntryConditionSet* apCopy, void* apOwner);
};

// Casts apForm to BGSCameraPath, copies common data, conditions, data and the shot list into this camera path.
// @fnv 0x0058a680
// pdb: BGSCameraPath::Copy | BGSCameraPath.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: plist, pcopy, pshot
void BGSCameraPath::CopyFromForm(TESForm* apCopy)
{
    BGSCameraPath* p = dynamic_cast<BGSCameraPath*>(apCopy);
    if (!p)
        return;
    CopyCommonData(p);
    ((Cond_58a680*)GetConditions())->Copy(p->GetConditions(), 0);
    __memcpy(&data, &p->data, sizeof(CameraShotData));
    BSSimpleList<BGSCameraShot*>* h = 0;
    ShotList.RemoveAll();
    h = p->GetShotList();
    while (h && !h->IsEmpty())
    {
        BGSCameraShot* e = h->GetItem();
        h = h->GetNext();
        ShotList.AddTail(e);
    }
}

extern "C" int __cdecl memcmp(const void*, const void*, size_t);

struct FormCompareStub
{
    bool IsDifferentForm(BGSCameraPath* apCompare);
};
struct ConditionCompareStub
{
    bool IsDifferent(PerkEntryConditionSet* apCompare);
};

// Compares this camera path against another form's camera path: base form, conditions, a data byte, and shot lists; returns true if any differ.
// @fnv 0x0058a750
// src: XBOX_PDB 0x0058a750
// pdb: BGSCameraPath::Compare | BGSCameraPath.obj | retail, vtable slot, high
// pdb params: apCompare
// pdb locals: pcomp, plist, pcomplist
bool BGSCameraPath::Compare_0058a750(TESForm* apCompare)
{
    BGSCameraPath* pathForm = dynamic_cast<BGSCameraPath*>(apCompare);
    if (!pathForm)
        return true;
    if (((FormCompareStub*)this)->IsDifferentForm(pathForm))
        return true;
    if (((ConditionCompareStub*)GetConditions())->IsDifferent(pathForm->GetConditions()))
        return true;
    if (memcmp(&data, &pathForm->data, 1))
        return true;
    BSSimpleList<BGSCameraShot*>* pShotNode = GetShotList();
    BSSimpleList<BGSCameraShot*>* pOtherShotNode = pathForm->GetShotList();
    while (pOtherShotNode && !pOtherShotNode->IsEmpty())
    {
        if (!pShotNode || pShotNode->GetItem() != pOtherShotNode->GetItem())
            return true;
        pOtherShotNode = pOtherShotNode->GetNext();
        pShotNode = pShotNode->GetNext();
    }
    if (pShotNode && !pShotNode->IsEmpty())
        return true;
    return false;
}

class DataHandlerStub
{
  public:
    bool IsLoading();
};
extern DataHandlerStub* g_dataHandler;

// Clears data, unlinks the path from its parent or the global path array while not loading, then deletes all children and the child array.
// @fnv 0x0058a450
// pdb: BGSCameraPath::~BGSCameraPath | BGSCameraPath.obj | retail, upstream name already matches, high
// pdb locals: parray, iindex, pnextpath, icount, i, pchildpath
BGSCameraPath::~BGSCameraPath()
{
    ClearData();
    if (!g_dataHandler->IsLoading())
    {
        if (GetParentPath())
        {
            GetParentPath()->RemoveChild(this);
        }
        else
        {
            NiFormArray* pArray = DAT_011ca700->GetPathArray();
            if (pArray)
            {
                unsigned int uiPathIndex = pArray->GetIndexOfForm(this);
                if (uiPathIndex != -1)
                {
                    BGSCameraPath* pNextPath = (BGSCameraPath*)pArray->GetFormAt(uiPathIndex + 1);
                    if (pNextPath)
                    {
                        pNextPath->SetPrevPathStub(GetPrevPathStub());
                    }
                    ((NiTLargeArray<TESTopicInfo*, NiTMallocInterface<TESTopicInfo*> >*)pArray)->RemoveAt(uiPathIndex);
                    pArray->Compact();
                }
            }
        }
    }
    if (pChildPaths)
    {
        unsigned int ItemUpdateArray = GetChildCount();
        if (ItemUpdateArray)
        {
            for (unsigned int i = 0; i < ItemUpdateArray; i++)
            {
                BGSCameraPath* pChildPath = GetChildAt(i);
                pChildPath->SetParentStub(0);
                delete pChildPath;
            }
        }
        delete pChildPaths;
    }
}

struct CameraPathMap
{
    bool GetAt(BGSCameraPath* key, BGSCameraPath*& value);
};

struct CameraPathArrayResize
{
    void Resize(unsigned int size);
};

// Clones the base camera path, remaps its parent and previous paths through apMap, and copies the child form array when present.
// @fnv 0x0058a870
// src: XBOX_PDB 0x0058a870
// pdb: BGSCameraPath::CreateDuplicateForm | BGSCameraPath.obj | retail, upstream name already matches, high
// pdb params: abCreateEditorID, apCopyMap
// pdb locals: pparentpath, pduppath, pprevpath, pform, icount, pduparray, plastchild, i, pchildpath, pdupchild
TESForm* BGSCameraPath::CreateDuplicateForm(bool abCreateEditorID, CameraPathMap* apCopyMap)
{
    BGSCameraPath* pClone_ =
        dynamic_cast<BGSCameraPath*>(CreateBaseClone(abCreateEditorID, apCopyMap));
    BGSCameraPath* pFound4 = 0;
    if (!apCopyMap->GetAt(pParentPath, pFound4) || !pFound4)
        pFound4 = pParentPath;
    BGSCameraPath* pParent3 = pFound4;
    pClone_->SetParentPath(pParent3);
    pFound4 = 0;
    if (!apCopyMap->GetAt(pPrevPath, pFound4) || !pFound4)
        pFound4 = pPrevPath;
    BGSCameraPath* pPrev = pFound4;
    pClone_->SetPrevPathForm(pPrev);
    if (pChildPaths)
    {
        pChildPaths->Compact();
        unsigned int uiCount5 = pChildPaths->GetSize();
        NiFormArray* pArray = pClone_->CreateChildArray();
        ((CameraPathArrayResize*)pArray)->Resize(uiCount5);
        TESForm* pLastVal = 0;
        for (unsigned int i5 = 0; i5 < uiCount5; i5++)
        {
            TESForm* pChildA = pChildPaths->FUN_00877a30(i5);
            TESForm* pNewChildB1 = pChildA->CreateForm((void*)(int)abCreateEditorID, apCopyMap);
            unsigned int uiValueC2 = (unsigned int)pNewChildB1;
            pArray->SetSize(i5, &uiValueC2);
            pLastVal = pNewChildB1;
        }
    }
    return pClone_;
}

struct CameraResponse
{
    bool __thiscall FUN_00680c30();
};
struct CameraFlag
{
    bool __thiscall FUN_008256d0();
};

// @fnv 0x0058b510
// src: COMMUNITY 0x0058b510
// pdb: BGSCameraPath::GetCameraPathToPlay | BGSCameraPath.obj | retail, upstream name already matches, high
// pdb params: apActionRef, apTargetRef
// pdb locals: pretpath, icount, pgoodchild, i, pchildpath
BGSCameraPath* BGSCameraPath::GetCameraPathToPlay(unsigned int apActionRef, unsigned int apTargetRef)
{
    BGSCameraPath* result = 0;
    if (!FUN_00440d80())
    {
        if (FUN_00500940(apActionRef, apTargetRef)->FUN_00680c30())
        {
            if (IsFlag80Set() || !FUN_00891170()->FUN_008256d0())
            {
                result = this;
            }
            unsigned int count = GetChildCount();
            if (count != 0)
            {
                BGSCameraPath* descendant = 0;
                for (unsigned int i = 0; i < count; ++i)
                {
                    BGSCameraPath* child = GetCameraPathAt(i);
                    if (child != 0)
                        descendant = child->GetCameraPathToPlay(apActionRef, apTargetRef);
                    if (descendant != 0)
                        break;
                }
                if (descendant != 0)
                    result = descendant;
            }
        }
    }
    return result;
}

// Returns the child path array, allocating it if it is null.
// @fnv 0x0058b350
// src: XBOX_RETAIL_PDB 0x0058b350
// pdb: BGSCameraPath::CreateChildArray | BGSCameraPath.obj | retail, object-file run extension, high
NiFormArray* BGSCameraPath::CreateChildArray()
{
    return pChildPaths ? pChildPaths : (pChildPaths = new NiFormArray);
}
