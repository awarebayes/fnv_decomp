// fallout_shared/bgsdebris.cpp: BGSDebris and MagicItemForm (19 functions)

#include "../../include/BGSDebris.h"
#include "../../include/TESForm.h"
#include "../../include/BSSimpleList.h"
#include "../../include/BGSPreloadable.h"
#include "../../include/MagicItemForm.h"
#include "../../include/TESFile.h"
#include "../../include/FileEntryList.h"
#include "../../include/NiPoint3.h"

// The shared FileEntryList changes /GS stack placement in LoadForm.

extern const char DAT_01015890[];
extern const char DAT_01024988[];
int Warning(const char*, ...);
extern "C" void* _alloca(size_t);

// DebrisModelEntry and ModelLoader do not yet have shared headers.
class DebrisModelEntry
{
  public:
    char chance, flags;
    short pad;
    char* path;
    FileEntryList model;
    DebrisModelEntry();
    ~DebrisModelEntry();
    void BGSDebrisData_SetFileName(const char*);
};
void Deallocate(void*);
void* Allocate(unsigned int);
template <class T> unsigned int BSStringLength(const T*);
// pdb: class ModelLoader, size 0x30 (here 0x1), 12 members
class ModelLoader
{
  public:
    void QueueModel(const char*, void*, int, void*, int, int, int, int);
};
extern ModelLoader* g_modelLoader;

// @fnv 0x00409140
BSSimpleList<DebrisModelEntry*>* BGSDebris::GetDataList()
{
    return &DataList;
}

// Iterates DataList, deallocating each entry's path and deleting the entry, then removes the head until the list is empty.
// @fnv 0x004f9e30
// pdb: BGSDebris::ClearData | BGSDebris.obj | retail, upstream name already matches, high
// pdb locals: plist, pdata
void BGSDebris::ClearData()
{
    BSSimpleList<DebrisModelEntry*>* list = &DataList;
    while (!DataList.IsEmpty())
    {
        DebrisModelEntry* data = list->GetItem();
        if (data)
        {
            char* path = data->path;
            Deallocate(path);
            delete data;
        }
        DataList.RemoveHead();
    }
}

// Iterates the debris data list and asks g_modelLoader to load each entry's non-empty path into its model with the given priority and file.
// @fnv 0x004fa970
// pdb: BGSDebris::QueueFiles | BGSDebris.obj | retail, vtable slot, high
// pdb params: aePriority, apParent
// pdb locals: plist, pdebrisData
void BGSDebris::QueueResourceLoad(IO_TASK_PRIORITY aePriority, QueuedFile* apParent)
{
    BSSimpleList<DebrisModelEntry*>* list = GetDataList();
    while (list)
    {
        DebrisModelEntry* data = list->GetItem();
        if (data && data->path && BSStringLength(data->path))
            g_modelLoader->QueueModel(data->path, &data->model, aePriority, apParent, 1, 1, 0, 0);
        list = list->GetNext();
    }
}

// @fnv 0x004fa500
// src: XBOX_DEBUG_PDB 0x004fa500
// pdb: BGSDebris::AddDebrisData | BGSDebris.obj | debug, call graph (shared callers), high
// pdb params: acPercentage, apFileName, acFlags
// pdb locals: ilength, pdata
DebrisModelEntry* BGSDebris::AddDebrisData(char acPercentage, const char* apFileName, char acFlags)
{
    if (!apFileName || !acPercentage)
        return 0;
    unsigned int length = BSStringLength(apFileName);
    if (length)
    {
        DebrisModelEntry* data = new DebrisModelEntry;
        data->chance = acPercentage;
        data->BGSDebrisData_SetFileName(apFileName);
        data->flags = acFlags;
        InsertDebrisIfValid(data);
        return data;
    }
    return 0;
}

// Reads a BGSDebris record from a TESFile, handling EDID, OBND, DATA and MODL chunks, and returns false if the form type is not 0x52.
// @fnv 0x004f9f10
// pdb: BGSDebris::Load | BGSDebris.obj | retail, unique string literal, high
// pdb params: apFile
// pdb locals: echunkid, formid, plastDebrisData, pMessage, pdataEnd, isize, pdata, pdataSave, cflags, pfilename, cpercentage, tempList
bool BGSDebris::LoadForm(TESFile* apFile)
{
    // Local identifiers preserve MSVC9 stack-slot ordering.
    unsigned char v0 = apFile->GetTESForm();
    if (v0 != 0x52)
        return false;
    ClearData();
    LoadRecordHeader(apFile);
    SetInitializationState(false);
    DebrisModelEntry* v1 = 0;  // most recently loaded debris entry
    while (int v2 = apFile->GetChunkID())
    {
        switch (v2)
        {
        case 0x44494445: {
            char* v3 = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(v3, 512);
            SetEditorID_AtRuntime(v3);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x41544144: {
            // v5: chunk size; v6: allocation; v4: parse cursor; v7: end.
            unsigned int v5 = apFile->GetChunkSize();
            unsigned char* v6 = new unsigned char[v5];
            unsigned char* v4 = v6;
            apFile->GetChunkData(v4, v5);
            unsigned char* v7 = v4 + v5;
            if (v4 < v7)
            {
                // v10: chance; v9: model path; v8: flags.
                unsigned char v10 = *v4;
                v4++;
                const char* v9 = (const char*)v4;
                v4 += BSStringLength((const char*)v4) + 1;
                unsigned char v8 = 0;
                if (v4 < v7)
                {
                    v8 = *v4;
                    v4++;
                }
                v1 = AddDebrisData(v10, v9, v8);
            }
            delete[] v6;
            break;
        }
        case 0x54444f4d:
            if (v1)
                v1->model.Load(apFile, this, v1->path);
            else
            {
                Warning(DAT_01024988, GetName(), GetFormID());
                FileEntryList v11;
                v11.Load(apFile, this, DAT_01015890);
            }
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}

// Constructs a MagicItemForm object; the body is empty.
// @fnv 0x00409190
// pdb: MagicItemForm::MagicItemForm | EnchantmentItem.obj | retail, upstream name already matches, high
MagicItemForm::MagicItemForm()
{}

// @fnv 0x004f9d90
// pdb: BGSPreloadable::BGSPreloadable | BGSDebris.obj | debug, upstream name already matches, high
BGSPreloadable::BGSPreloadable()
{}

int __cdecl BSstrcmp(char const*, char const*);

struct S_004fa4b0
{
    char c;
    const char* s;
};

// Compares two entries by their char field, returning -1 or 1, and otherwise compares their string fields via FUN_00408b20.
// @fnv 0x004fa4b0
// src: COMMUNITY 0x004fa4b0
// pdb: DebrisSortFunction | BGSDebris.obj | debug, upstream name already matches, high
// pdb params: apData1, apData2
int __cdecl DebrisSortFunction(const S_004fa4b0* apData1, const S_004fa4b0* apData2)
{
    if (apData1->c > apData2->c)
    {
        return -1;
    }
    else if (apData1->c < apData2->c)
    {
        return 1;
    }
    else
    {
        return BSstrcmp(apData1->s, apData2->s);
    }
}

// Initializes the object: sets byte 0 to 10, byte 1 to 0, int at offset 4 to 0, constructs the object at offset 8, and returns pThis.
// @fnv 0x004fa5e0
// src: COMMUNITY 0x004fa5e0
// pdb: BGSDebrisData::BGSDebrisData | BGSDebris.obj | debug, upstream name already matches, high
void* __fastcall BGSDebrisData_BGSDebrisData(char* pObject)
{
    pObject[0] = 10;
    pObject[1] = 0;
    *reinterpret_cast<int*>(pObject + 4) = 0;
    reinterpret_cast<FileEntryList*>(pObject + 8)->FileEntryList::FileEntryList();
    return pObject;
}

struct DebrisData
{
    char c;
    char pad[3];
    const char* p;
};
struct DArg
{
    DebrisData* d;
};
struct AEList
{
    void Insert(DebrisData*, int(__cdecl*)(DebrisData*, DebrisData*));
};
int __cdecl DebrisSortFunction(DebrisData*, DebrisData*);
unsigned int __cdecl BSStringLength(const char*);
// Checks that a.d, its flag, and its string are non-empty, then inserts a.d into the AEList at self+0x1c and returns true; otherwise returns false.
// @fnv 0x004fa610
// pdb: BGSDebris::AddDebrisData | BGSDebris.obj | debug, call graph, medium
// pdb params: apDebrisData
bool __fastcall InsertDebrisIfValid(char* self, DArg aDebrisArg)
{
    if (!aDebrisArg.d || !aDebrisArg.d->c || !aDebrisArg.d->p || !BSStringLength(aDebrisArg.d->p))
        return false;
    ((AEList*)(self + 0x1c))->Insert(aDebrisArg.d, DebrisSortFunction);
    return true;
}

struct FUN_004fa670_shell : public DebrisModelEntry
{
    void BGSDebrisData_SetFileName(char*);
};

template <class T> unsigned int __cdecl BSStringLength(T const*);

// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
class TaskQueueInterface
{
  public:
    static void __cdecl CopyStringToBuffer(char*, unsigned int, char const*);
};

// @fnv 0x004fa670
// src: COMMUNITY 0x004fa670
// pdb: BGSDebrisData::SetFileName | BGSDebris.obj | retail, upstream name already matches, high
// pdb params: apFileName
// pdb locals: iFilenameLen
void FUN_004fa670_shell::BGSDebrisData_SetFileName(char* apFileName)
{
    if (*(char**)((char*)this + 4))
    {
        delete *(char**)((char*)this + 4);
    }
    *(char**)((char*)this + 4) = 0;
    if (apFileName)
    {
        int iFilenameLen = BSStringLength<char>(apFileName) + 1;
        *(char**)((char*)this + 4) = new char[iFilenameLen];
        TaskQueueInterface::CopyStringToBuffer(*(char**)((char*)this + 4), iFilenameLen, apFileName);
    }
}

// Sets the form type to 0x52 and clears the form data.
// @fnv 0x004f9cd0
// pdb: BGSDebris::BGSDebris | BGSDebris.obj | retail, upstream name already matches, high
BGSDebris::BGSDebris()
{
    SetFormType(static_cast<ENUM_FORM_ID>(0x52));
    TESForm::ClearData();
}

struct FUN_004fa950_shell
{
    int __thiscall IsFlagSet();
};

// @fnv 0x004fa950
int __thiscall FUN_004fa950_shell::IsFlagSet()
{
    return (*reinterpret_cast<signed char*>(this + 1) & 1);
}

// @fnv 0x004fa2f0
// pdb: BGSDebris::Copy | BGSDebris.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: plist, pcopy, pdebrisData
void BGSDebris::CopyFrom(const TESForm* apCopy)
{
    BGSDebris* debris2 = dynamic_cast<BGSDebris*>(const_cast<TESForm*>(apCopy));
    if (!debris2)
        return;
    ClearData();
    TESForm::CopyFrom(debris2);
    BSSimpleList<DebrisModelEntry*>* list = debris2->GetDataList();
    while (list)
    {
        DebrisModelEntry* data = list->GetItem();
        if (data)
            AddDebrisData(data->chance, data->path, data->flags);
        list = list->GetNext();
    }
}

struct DNode
{
    void** FUN_006815c0();
    DNode* FUN_00726070();
};
struct DItem
{
    char chance;
    char pad[3];
    void* model;
    unsigned char IsFlagSet();
};
void* __cdecl Object_DispatchCommandFour(unsigned int);
struct DebrisObj
{
    char b[0x24];
    static void* operator new(size_t n)
    {
        return Object_DispatchCommandFour(n);
    }
    static void operator delete(void*, size_t);
    DebrisObj(void* a1, float f, void* m, int z, NiPoint3* pos, NiPoint3* v1, NiPoint3* v2, NiPoint3* v3, float s, int z1, int z2);
};
extern const float DAT_01012054;
extern const float DAT_010249d8;
extern const float DAT_01017b78;
extern const double DAT_01020758;
extern NiPoint3 DAT_011f426c;
float __cdecl FUN_00476b70(float, float);

void __cdecl BSTempEffectParticle_Spawn(void* apCell, float afLifetime, void* apParticleFilename, NiPoint3 aVector, NiPoint3 aPosition, float afScale, int aiFlags, int apFollowObject);
// @fnv 0x004fa6f0
// pdb: BGSDebris::SpawnDebris | BGSDebris.obj | retail, upstream name already matches, high
// pdb params: apCell, arLocation, aiNumParticles, afScale
// pdb locals: plist, tempAutoEvent, inumParticlesSpawned, pdebrisData, inumparticles, i, debrisVector, velocity, pdebris
void BGSDebris::SpawnDebris(void* apCell, NiPoint3* arLocation, unsigned int aiNumParticles, float afScale)
{
    if (!apCell)
        return;
    unsigned int i8 = 0;
    DNode* node_ = (DNode*)GetDataList();
    while (node_)
    {
        DItem* d9 = (DItem*)*node_->FUN_006815c0();
        if (d9 && d9->model && d9->chance)
        {
            unsigned int n = d9->chance * aiNumParticles / 100;
            if (n == 0 && i8 < aiNumParticles)
                n = 1;
            for (unsigned int j3 = 0; j3 < n; j3++)
            {
                NiPoint3 v4;
                v4.x = FUN_00476b70(DAT_01012054, 1.0f);
                v4.y = FUN_00476b70(DAT_01012054, 1.0f);
                v4.z = FUN_00476b70(DAT_01012054, 1.0f);
                v4.Normalize();
                if (d9->IsFlagSet())
                {
                    NiPoint3 dir = DAT_011f426c;
                    DebrisObj* o3 =
                        new DebrisObj(apCell, DAT_010249d8, d9->model, 0, arLocation, &v4, &dir, &dir, afScale, 0, 0);
                    (void)o3;
                }
                else
                {
                    BSTempEffectParticle_Spawn(apCell, DAT_01017b78, d9->model, v4, *arLocation, (float)(afScale * DAT_01020758), 7, 0);
                }
                i8++;
            }
        }
        node_ = node_->FUN_00726070();
    }
}

struct DebrisModel4fa
{
    void FUN_008d0600(int iType, int iValue);
};

struct DebrisEntry4fa
{
    char cChance;
    char cFlags;
    short sPad;
    char* pcPath;
    DebrisModel4fa kModel;
};

void __cdecl TESForm_AddChunkArray(unsigned int aeChunkID, void* apData, unsigned int aiSize);

// @fnv 0x004fa1a0
// pdb: BGSDebris::Save | BGSDebris.obj | retail, vtable slot, high
// pdb locals: plist, pdebrisData, pdata, pdataSave, idataSize
void BGSDebris::WriteDebrisEntries()
{
    BeginFormRecord();
    DNode* pkNode = (DNode*)GetDataList();
    while (pkNode)
    {
        DebrisEntry4fa* pkData = (DebrisEntry4fa*)*pkNode->FUN_006815c0();
        if (pkData && pkData->pcPath && BSStringLength(pkData->pcPath))
        {
            unsigned int uiSize = BSStringLength(pkData->pcPath) + 3;
            char* pTmpcBuffer = new char[uiSize];
            char* pcCur = pTmpcBuffer;
            *pcCur = pkData->cChance;
            pcCur++;
            TaskQueueInterface::CopyStringToBuffer(pcCur, uiSize, pkData->pcPath);
            pcCur += BSStringLength(pkData->pcPath) + 1;
            *pcCur = pkData->cFlags;
            pcCur++;
            TESForm_AddChunkArray('ATAD', pTmpcBuffer, uiSize);
            delete pTmpcBuffer;
            pkData->kModel.FUN_008d0600('TDOM', (int)pkData->pcPath);
        }
        pkNode = pkNode->FUN_00726070();
    }
    FinishFormRecord();
}

int __cdecl BSstrcmp(char const*, char const*);

struct DebrisListNode
{
    int GetCount();
    DebrisModelEntry** GetItemPtr();
    DebrisListNode* NextNode();
};

// Compares this debris list against another BGSDebris's list, returning true if counts, entries, chance, path or flags differ.
// @fnv 0x004fa390
// pdb: BGSDebris::Compare | BGSDebris.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pcomp, pcompareList, plocalList, pcompareData, plocalData
bool BGSDebris::Compare(TESForm* apCompare)
{
    BGSDebris* other7 = dynamic_cast<BGSDebris*>(apCompare);
    if (!other7)
        return true;
    DebrisListNode* otherIter2 = (DebrisListNode*)other7->GetDataList();
    if (otherIter2->GetCount() != ((DebrisListNode*)&DataList)->GetCount())
        return true;
    DebrisListNode* iter = (DebrisListNode*)&DataList;
    while (otherIter2)
    {
        DebrisModelEntry* srcData = *otherIter2->GetItemPtr();
        DebrisModelEntry* dstData = *iter->GetItemPtr();
        if ((!srcData && dstData) || (srcData && !dstData))
            return true;
        if (srcData && dstData)
        {
            if (srcData->chance != dstData->chance)
                return true;
            if (BSstrcmp(srcData->path, dstData->path))
                return true;
            if (srcData->flags != dstData->flags)
                return true;
        }
        iter = iter->NextNode();
        otherIter2 = otherIter2->NextNode();
    }
    return false;
}

// @fnv 0x004f9db0
// pdb: BGSDebris::~BGSDebris | BGSDebris.obj | debug, upstream name already matches, high
BGSDebris::~BGSDebris()
{
    ClearData();
}
