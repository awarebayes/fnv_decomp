// fallout_shared/bgsidlecollection.cpp: BGSIdleCollection (14 functions)

#include "../../include/BGSIdleCollection.h"
#include "../../include/Actor.h"
#include "../../include/TESIdleForm.h"
#include "../../include/TESObjectANIO.h"
#include "bgsidlecollection_decls.h"

// @fnv 0x004797e0
void BGSIdleCollection::InitializeComponent()
{
    cIdleFlags = 0;
    cIdleCount = 0;
    pIdleArray = 0;
    fTimerCheckForIdle = 0.0f;
}

// Resolves each stored form ID to a TESObjectANIO idle, keeps valid idles, and warns and removes entries that are missing or lack an animation.
// @fnv 0x00479b00
// pdb: BGSIdleCollection::InitItem | BGSIdleCollection.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: pidle, i, iid
void BGSIdleCollection::InitItem(TESForm* apOwner)
{
    TESObjectANIO* idle;

    idle = 0;
    for (unsigned int index = 0; index < cIdleCount; ++index)
    {
        unsigned int formID;
        formID = reinterpret_cast<unsigned int>(pIdleArray[index]);
        TESForm::RemapFormIDForFile(formID, apOwner->GetFile(-1));
        idle = dynamic_cast<TESObjectANIO*>(TESFormInitFlag::GetFormByID(formID));
        if (idle)
        {
            if (reinterpret_cast<TESIdleForm*>(idle)->CheckIdleFile())
            {
                pIdleArray[index] = reinterpret_cast<TESIdleForm*>(idle);
            }
            else
            {
                Warning(kBGSIdleCollectionMissingAnim, reinterpret_cast<TESIdleForm*>(idle)->GetName(), apOwner->GetName(),
                        apOwner->GetFormID());
                RemoveIdleEntry(index);
                --index;
            }
        }
        else
        {
            Warning(kBGSIdleCollectionMissingIdle, formID, apOwner->GetName(), apOwner->GetFormID());
            RemoveIdleEntry(index);
            --index;
        }
    }
}

// @fnv 0x00479c40
// pdb: BGSIdleCollection::Load | BGSIdleCollection.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, icount, isize, ii
void BGSIdleCollection::Load(TESFile* apFile)
{
    int chunk = apFile->GetChunkID();
    if (FUN_00479e20(chunk))
    {
        switch (chunk)
        {
        case 0x464c4449:
            apFile->GetChunkData(&cIdleFlags, 1);
            break;
        case 0x434c4449:
            if (apFile->GetChunkSize() == 1)
                apFile->GetChunkData(&cIdleCount, 1);
            else
            {
                unsigned int count = 0;
                apFile->GetChunkData(&count);
                cIdleCount = (unsigned char)count;
            }
            if (cIdleCount)
            {
                if (pIdleArray)
                    delete[] pIdleArray;
                pIdleArray = new TESIdleForm*[cIdleCount];
                memset(pIdleArray, 0, cIdleCount * 4);
            }
            break;
        case 0x414c4449:
            if (pIdleArray)
            {
                unsigned int size = cIdleCount * 4;
                if (apFile->GetChunkSize() == size)
                {
                    apFile->GetChunkData(pIdleArray, size);
                    if (apFile->IsEndianConversionRequired())
                        for (int i = 0; i < cIdleCount; ++i)
                            BSEndian(reinterpret_cast<unsigned int*>(&pIdleArray[i]), EndianSizeTag<4>());
                }
            }
            break;
        case 0x544c4449:
            apFile->GetChunkData(reinterpret_cast<unsigned int*>(&fTimerCheckForIdle));
            break;
        }
    }
}

// @fnv 0x0047a030
// pdb: BGSIdleCollection::GetIdlesForActor | BGSIdleCollection.obj | debug, object-file run extension, low
// pdb params: apActor, apTargetRef, apOutIdleArray
// pdb locals: iidlecount, iarraysize, i, pidle
unsigned int BGSIdleCollection::GetAvailableIdles(Actor* actor, TESObjectREFR* target,
                                                  BSSimpleArray<TESIdleForm*, 1024>* idles)
{
    unsigned int count = 0;
    if (actor)
    {
        unsigned int size = 0;
        if (idles)
            size = idles->GetSize();
        for (int i = 0; i < cIdleCount; ++i)
        {
            TESIdleForm* idle = pIdleArray[i];
            if (idle && actor->HasIdleInFormArray(idle) &&
                idle->IsValidForReferences(reinterpret_cast<TESObjectREFR*>(actor), target, true))
            {
                if (idles)
                {
                    if (count < size)
                        idles->SetAt(count, idle);
                    else
                        idles->Add(idle);
                }
                ++count;
            }
        }
    }
    return count;
}

// Returns true if chunk matches one of six IDLE-family chunk IDs, otherwise false.
// @fnv 0x00479e20
bool IsIdleCollectionChunk(int chunk)
{
    switch (chunk)
    {
    case 0x464c4449:
    case 0x414c4449:
    case 0x424c4449:
    case 0x434c4449:
    case 0x4e4c4449:
    case 0x544c4449:
        return true;
    default:
        return false;
    }
}

// @fnv 0x00479e80
int BGSIdleCollection::GetPickType()
{
    if (cIdleFlags & 1)
        return 1;
    return 0;
}

// @fnv 0x00479790
// pdb: BGSIdleCollection::BGSIdleCollection | BGSIdleCollection.obj | debug, upstream name already matches, high
BGSIdleCollection::BGSIdleCollection()
{
    InitializeComponent();
}

struct Arr
{
    unsigned int* GetAt(int);
};
struct SArgs
{
    int i;
    unsigned int* v;
};
// Stores the value pointed to by a.v into the element of self at index a.i.
// @fnv 0x0047a110
// src: XBOX_PDB 0x0047a110
void __fastcall BSSimpleArray_TESAmmo_1024_SetAt(Arr* self, SArgs aArgs)
{
    *self->GetAt(aArgs.i) = *aArgs.v;
}

// @fnv 0x004797c0
// pdb: BGSIdleCollection::~BGSIdleCollection | BGSIdleCollection.obj | retail, upstream name already matches, high
BGSIdleCollection::~BGSIdleCollection()
{
    BGSIdleCollection::Free();
}

extern "C" int __cdecl memcmp(const void*, const void*, size_t);

// @fnv 0x00479930
// pdb: BGSIdleCollection::CompareComponent | BGSIdleCollection.obj | retail, vtable slot, high
// pdb params: apCompareFC
// pdb locals: pcompare
bool __thiscall BGSIdleCollection::CompareWithBase(BaseFormComponent* apCompareFC)
{
    BGSIdleCollection* idle = dynamic_cast<BGSIdleCollection*>(apCompareFC);
    if (!idle)
        return true;
    if (cIdleFlags != idle->cIdleFlags)
        return true;
    if (cIdleCount != idle->cIdleCount)
        return true;
    if (fTimerCheckForIdle != idle->fTimerCheckForIdle)
        return true;
    if (pIdleArray && idle->pIdleArray &&
        memcmp(pIdleArray, idle->pIdleArray,
               cIdleCount * 4))
        return true;
    return false;
}

void __cdecl _Deallocate(void*);
// @fnv 0x00479810
// pdb: BGSIdleCollection::ClearDataComponent | BGSIdleCollection.obj | retail, vtable slot, high
void BGSIdleCollection::Free()
{
    void* idleArray = pIdleArray;
    _Deallocate(idleArray);
    cIdleCount = 0;
    pIdleArray = 0;
}

struct _DeallocateHelper
{
    static void _Deallocate(void*);
};
struct __memcpyHelper
{
    static void __memcpy(void*, const void*, unsigned int);
};
// Copies the idle count, reallocates and copies the idle form array from apCopyFC, and copies the extra field.
// @fnv 0x00479850
// pdb: BGSIdleCollection::CopyComponent | BGSIdleCollection.obj | retail, vtable slot, high
// pdb params: apCopyFC
// pdb locals: pcopy
void BGSIdleCollection::CopyFromBase(BaseFormComponent* apCopyFC)
{
    BGSIdleCollection* pSource = dynamic_cast<BGSIdleCollection*>(apCopyFC);
    if (!pSource)
        return;
    cIdleFlags = pSource->cIdleFlags;
    {
        TESIdleForm** pOld = pIdleArray;
        _DeallocateHelper::_Deallocate(pOld);
    }
    pIdleArray = 0;
    cIdleCount = 0;
    if (pSource->cIdleCount)
    {
        pIdleArray = new TESIdleForm*[pSource->cIdleCount];
        __memcpyHelper::__memcpy(pIdleArray, pSource->pIdleArray, pSource->cIdleCount * 4);
        cIdleCount = pSource->cIdleCount;
    }
    fTimerCheckForIdle = pSource->fTimerCheckForIdle;
}

void __cdecl TESForm_AddChunk_3(unsigned int, unsigned char);
void __cdecl _FUN_00485910(unsigned int, float);
void __cdecl _FUN_00485710(unsigned int, unsigned int*, unsigned int);
extern "C" void* __cdecl _alloca(size_t);

// @fnv 0x004799f0
// src: XBOX_RETAIL_PDB 0x004799f0
// pdb: BGSIdleCollection::Save | BGSIdleCollection.obj | retail, call graph (shared callers), high
// pdb locals: isize, parray, i
void __fastcall BGSIdleCollection_Save(BGSIdleCollection* self)
{
    unsigned int isize3;
    unsigned int* parray;
    unsigned int i;

    if (self->cIdleCount)
    {
        TESForm_AddChunk_3(0x464c4449, self->cIdleFlags);
        TESForm_AddChunk_3(0x434c4449, self->cIdleCount);
        _FUN_00485910(0x544c4449, self->fTimerCheckForIdle);

        isize3 = self->cIdleCount << 2;
        parray = (unsigned int*)_alloca(isize3);

        for (i = 0; i < self->cIdleCount; i++)
        {
            if (self->pIdleArray[i] != 0)
                parray[i] = self->pIdleArray[i]->GetFormID();
            else
                parray[i] = 0;
        }

        _FUN_00485710(0x414c4449, parray, self->cIdleCount);
    }
}

void* __memcpy(void* dst, const void* src, unsigned int size);

// Removes the element at index from the array, shrinking the count and reallocating the array without that element.
// @fnv 0x00479ea0
// pdb: BGSIdleCollection::RemoveIdleAtIndex | BGSIdleCollection.obj | retail, object-file run extension, low
// pdb params: aiIndex
// pdb locals: icopycount, parray
void BGSIdleCollection::RemoveIdleEntry(unsigned int index)
{
    if (index < cIdleCount)
    {
        if (cIdleCount == 1)
        {
            cIdleCount = 0;
            delete pIdleArray;
            pIdleArray = 0;
        }
        else
        {
            cIdleCount--;
            int n;
            TESIdleForm** newArrayA = new TESIdleForm*[cIdleCount];
            n = index;
            if (n > 0)
            {
                __memcpy(newArrayA, pIdleArray, n * 4);
            }
            n = cIdleCount - index;
            if (n > 0)
            {
                __memcpy(&newArrayA[index], &pIdleArray[index + 1], n * 4);
            }
            delete pIdleArray;
            pIdleArray = newArrayA;
        }
    }
}
