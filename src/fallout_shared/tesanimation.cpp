// fallout_shared/tesanimation.cpp: TESAnimation, ActorValueOwner (10 functions)

#include "../../include/ActorValueOwner.h"
#include "layout_support.h"
#include "../../include/TESAnimation.h"

unsigned int __cdecl BSStringLength(const char*);

// @fnv 0x0047f9f0
// pdb: TESAnimation::~TESAnimation | TESAnimation.obj | retail, upstream name already matches, high
TESAnimation::~TESAnimation()
{
    ClearComponent();
}

// Removes and deletes every filename string from the animation list until it is empty.
// @fnv 0x0047fa50
// pdb: TESAnimation::ClearDataComponent | TESAnimation.obj | retail, vtable slot, high
// pdb locals: pkfFilename
void TESAnimation::ClearComponent()
{
    while (!kfFilenames.IsEmpty())
    {
        const char* filename = kfFilenames.GetItem();
        kfFilenames.RemoveHead();
        delete const_cast<char*>(filename);
    }
}

// @fnv 0x0047fd90
// src: XBOX_DEBUG_PDB 0x0047fd90
// pdb: TESAnimation::HasKFFiles | TESAnimation.obj | debug, call graph, high
bool TESAnimation::HasKFFiles()
{
    if (kfFilenames.IsEmpty())
    {
        return false;
    }
    return true;
}

// @fnv 0x005f78e0
ActorValueOwner::ActorValueOwner()
{}

// @fnv 0x0047faa0
// pdb: TESAnimation::LoadAnimationChunk | TESAnimation.obj | retail, object-file run extension, low
// pdb params: apAnimation, apFile
// pdb locals: chunk, pcurrentFile, pmessage
void TESAnimation::LoadComponent(TESForm* form, TESFile* file)
{
    if (!file || !form)
        return;
    unsigned int chunk = file->GetChunkID();
    switch (chunk)
    {
    case 0x5a46464b: {
        char* pCurrent;
        char* pData = new char[file->GetChunkSize()];
        pCurrent = pData;
        file->GetChunkData(pData, 0);
        while (*pCurrent)
        {
            AddPathToList(pCurrent);
            pCurrent = pCurrent + BSStringLength(pCurrent) + 1;
        }
        delete[] pData;
    }
    break;
    }
}

// @fnv 0x0047f9c0
// pdb: TESAIForm::TESAIForm | TESAIForm.obj | retail, object-file run extension, low
// pdb note: debug build links it to TESAnimation::TESAnimation
TESAnimation::TESAnimation()
{}

int __cdecl _stricmp(const char*, const char*);
// @fnv 0x0047fcb0
// pdb: TESAnimation::IsKFFileInList | TESAnimation.obj | retail, upstream name already matches, high
// pdb params: apFilename
// pdb locals: pkfFileList, pkfFilename
bool TESAnimation::IsKFFileInList(const char* apFilename)
{
    if (kfFilenames.IsEmpty())
        return false;
    BSSimpleList<const char*>* pNode = &kfFilenames;
    while (pNode)
    {
        const char* pcName = pNode->GetItem();
        if (!_stricmp(pcName, apFilename))
            return true;
        pNode = pNode->GetNext();
    }
    return false;
}

void* __cdecl operator new(size_t);
// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
struct TaskQueueInterface
{
    static void __cdecl CopyStringToBuffer(char*, unsigned int, const char*);
};
// If the path is not already present, allocates a copy of it, copies the string into it and appends it to the list.
// @fnv 0x0047fd20
// pdb: TESAnimation::AddKFFile | TESAnimation.obj | debug, object-file gap fill, medium
// pdb params: apFilename
// pdb locals: iFilenameLen, pkfFilename
void TESAnimation::AddPathToList(const char* apPath)
{
    char* pcPathCopy;
    if (!IsKFFileInList(apPath))
    {
        unsigned int uiSize = BSStringLength(apPath) + 1;
        pcPathCopy = new char[uiSize];
        TaskQueueInterface::CopyStringToBuffer(pcPathCopy, uiSize, apPath);
        kfFilenames.AddTail(reinterpret_cast<const char* const&>(pcPathCopy));
    }
}

// Casts apCopyFC to TESAnimation, frees this component, then copies each filename from the source's file list via AddPathToList.
// @fnv 0x0047fb50
// pdb: TESAnimation::CopyComponent | TESAnimation.obj | retail, vtable slot, high
// pdb params: apCopyFC
// pdb locals: pkfFileList, pcopy, pkfFilename
void TESAnimation::CopyFromBase(BaseFormComponent* apCopyFC)
{
    TESAnimation* pcopy = dynamic_cast<TESAnimation*>(apCopyFC);
    if (pcopy == 0)
        return;
    ((BaseFormComponent*)this)->Free();
    BSSimpleList<const char*>* pkfFileList = &pcopy->kfFilenames;
    while (pkfFileList != 0)
    {
        const char* pkfFilename = pkfFileList->GetItem();
        if (pkfFilename != 0)
            AddPathToList(pkfFilename);
        pkfFileList = pkfFileList->GetNext();
    }
}

int __cdecl CompareNamesNoCase(const char* a, const char* b);

// @fnv 0x0047fbd0
// pdb: TESAnimation::CompareComponent | TESAnimation.obj | retail, vtable slot, high
// pdb params: apCompareFC
// pdb locals: pkfFileList, pkfFileListCompare, pcompare, pkfFilename, pkfFilenameCompare
bool TESAnimation::CompareWithBase(BaseFormComponent* apCompareFC)
{
    const char* x;
    const char* y;
    TESAnimation* other_2 = dynamic_cast<TESAnimation*>(apCompareFC);
    if (other_2 == 0)
    {
        return true;
    }
    BSSimpleList<const char*>* a = &this->kfFilenames;
    BSSimpleList<const char*>* b8 = &other_2->kfFilenames;
    while (a != 0 && b8 != 0)
    {
        x = a->GetItem();
        y = b8->GetItem();
        if ((x == 0 && y != 0) || (x != 0 && y == 0))
        {
            return true;
        }
        else
        {
            if (x != 0 && y != 0)
            {
                if (CompareNamesNoCase(x, y) != 0)
                {
                    return true;
                }
            }
        }
        a = a->GetNext();
        b8 = b8->GetNext();
    }
    if (a != 0 || b8 != 0)
    {
        return true;
    }
    return false;
}
