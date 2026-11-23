// fallout_shared/region system/tesregiondatagrass.cpp: TESRegionDataGrass, GrassRegion, RegionSaveShell, GrassMergeRegion (6 functions)

#include "../../../include/TESRegionData.h"
#include "tesregiondatagrass_decls.h"
// Default constructor for TESRegionDataGrass; the body is empty.
// @fnv 0x004f1ed0
// pdb: TESRegionDataImposter::TESRegionDataImposter | TESRegionDataImposter.obj | debug, call graph, medium
LocalTESRegionDataGrass::LocalTESRegionDataGrass()
{}

void __cdecl _Deallocate(void*);
struct Dummy1690
{
    void FUN_004f1730();
    void* FUN_004f1690(unsigned int);
};
// Calls FUN_004f1730, then frees this via _Deallocate when flags has bit 1 set, and returns this.
// @fnv 0x004f1690
// pdb: TESRegionDataGrass::`scalar deleting destructor' | TESRegionDataGrass.obj | retail, vtable slot, high
void* Dummy1690::FUN_004f1690(unsigned int flags)
{
    FUN_004f1730();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

void* __cdecl operator new(size_t);

struct FUN_004f16c0
{
    unsigned int data[3];

    FUN_004f16c0(const void* pSource);
};

struct FUN_004f1b60_shell : LocalTESRegionDataGrass
{
    TESRegionData* TESRegionDataGrass_Copy();
};

// @fnv 0x004f1b60
// src: XBOX_PDB 0x004f1b60
// pdb: TESRegionDataGrass::Copy | TESRegionDataGrass.obj | retail, upstream name already matches, high
// pdb locals: pdata
TESRegionData* FUN_004f1b60_shell::TESRegionDataGrass_Copy()
{
    FUN_004f16c0* pdata = new FUN_004f16c0((const void*)this);

    return (TESRegionData*)pdata;
}

FUN_004f16c0::FUN_004f16c0(const void* pSource)
{}

void* __cdecl operator new(size_t size)
{
    return 0;
}

struct GrassRecord
{
    unsigned int formID;
    unsigned int flags;
    void TwoValues_Endian();
};
struct FUN_004f5620
{
    virtual void v0();
    virtual int Validate();
    virtual ~FUN_004f5620();
    unsigned int formID;
    unsigned int flags;
    FUN_004f5620(const GrassRecord&);
};
struct FileReader
{
    unsigned int GetChunkID();
    unsigned int GetChunkSize();
    const char* GetFilename();
    bool GetChunkData(void*, unsigned int);
    bool IsEndianConversionRequired();
};
struct GrassList
{
    void AddGrassObject(FUN_004f5620*);
};
int Warning(const char*, ...);
struct GrassRegion
{
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual GrassList* GetGrassList();
    bool LoadGrassRecords(FileReader*);
};
// @fnv 0x004f1980
// pdb: TESRegionDataGrass::LoadRegionDataGrass | TESRegionDataGrass.obj | retail, string-set bridge, high
// pdb params: apFile
// pdb locals: icount, isize, pdataArray, pobj, i
bool GrassRegion::LoadGrassRecords(FileReader* apFile)
{
    GrassRecord* records = 0;
    FUN_004f5620* objectX = 0;
    if (!apFile || apFile->GetChunkID() != 0x53474452)
        return false;
    unsigned int size = apFile->GetChunkSize();
    unsigned int count_ = size / 8;
    if (size % 8)
    {
        Warning("MODELS: Invalid Region Grass Object Data in file \"%s\".", apFile->GetFilename());
        return false;
    }
    if (count_)
    {
        records = new GrassRecord[count_];
        apFile->GetChunkData(records, size);
        for (unsigned int index = 0; index < count_; index++)
        {
            if (apFile->IsEndianConversionRequired())
                records[index].TwoValues_Endian();
            objectX = new FUN_004f5620(records[index]);
            if (objectX)
            {
                if (objectX->Validate())
                    GetGrassList()->AddGrassObject(objectX);
                else
                    delete objectX;
            }
        }
        delete[] records;
    }
    return true;
}

struct SaveObj
{
    virtual void v00();
    virtual void* GetObj();
    virtual void v08();
    virtual void* GetObjC();
};
struct NamedForm
{
    unsigned int GetRefID();
};
struct ObjNode
{
    SaveObj** Item();
    ObjNode* Next();
};
struct ListHead
{
    int unk00;
};
struct ObjList
{
    int unk00;
};
struct ObjListOwner : ListHead, ObjNode
{
    unsigned int Count();
};
struct SavePair
{
    unsigned int form;
    unsigned int extra;
    void Swap();
};
bool IsBigEndian();
void WriteChunk(unsigned int type, void* data, unsigned int size);

struct RegionSaveShell
{
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual ObjListOwner* GetList();
    void Prepare();
    void WriteRegionSaveChunk();
};

// @fnv 0x004f17c0
// pdb: TESRegionDataGrass::Save | TESRegionDataGrass.obj | retail, vtable slot, high
// pdb locals: icount, pobjectArray, pList, i, pitem
void RegionSaveShell::WriteRegionSaveChunk()
{
    unsigned int count;
    SavePair* pPairs = 0;
    count = GetList()->Count();
    Prepare();
    if (count)
    {
        ObjNode* pNode = GetList();
        unsigned int uiIndex = 0;
        pPairs = new SavePair[count];
        while (pNode)
        {
            SaveObj* pObj = *pNode->Item();
            if (pObj)
            {
                pPairs[uiIndex].form = ((NamedForm*)pObj->GetObj())->GetRefID();
                pPairs[uiIndex].extra = pObj->GetObjC() ? ((NamedForm*)pObj->GetObjC())->GetRefID() : 0;
                uiIndex++;
            }
            pNode = pNode->Next();
        }
        if (IsBigEndian())
        {
            for (unsigned int uiSwapIndex = 0; uiSwapIndex < count; uiSwapIndex++)
                pPairs[uiSwapIndex].Swap();
        }
        WriteChunk(0x53474452, pPairs, count * 8);
        if (IsBigEndian())
        {
            for (unsigned int uiRestoreIndex = 0; uiRestoreIndex < count; uiRestoreIndex++)
                pPairs[uiRestoreIndex].Swap();
        }
        delete[] pPairs;
    }
}

struct GrassMergeList
{
    void TESRegionGrassObjectList_Clear();
    void CopyTargetList(GrassMergeList* apCopyInto);
    void TESRegionGrassObjectList_MergeLists(float afPriority, GrassMergeList* apOtherList, float afOtherPriority);
};
float __cdecl floor(float value);
struct GrassMergeRegion : TESRegionData
{
    virtual REGION_DATA_ID GetType();
    virtual void v9();
    virtual GrassMergeList* GetGrassList();
    bool TESRegionData_GetOverride();
    unsigned char TESRegionData_GetPriority();
    void FUN_004f15a0(int value);
    bool FUN_004f15c0(unsigned char value);
    void MergeGrassRegion(GrassMergeRegion* apRegionData, void* aiTotalBlending);
};
// Merges another grass region's data into this one when the source type is 6, choosing between override, priority and weighted blending of the grass lists.
// @fnv 0x004f1be0
// pdb: TESRegionDataGrass::BlendInto | TESRegionDataGrass.obj | retail, vtable slot, high
// pdb params: apRegionData, aiTotalBlending
// pdb locals: pdataIn, fnewPriority
void GrassMergeRegion::MergeGrassRegion(GrassMergeRegion* apRegionData, void* aiTotalBlending)
{
    GrassMergeRegion* priority = apRegionData;
    float source;
    if (!apRegionData || apRegionData->GetType() != 6 || !aiTotalBlending)
        return;
    if (m_bHasRegionData)
    {
        FUN_004f15a0(priority->TESRegionData_GetOverride());
        FUN_004f15c0(priority->TESRegionData_GetPriority());
        GetGrassList()->TESRegionGrassObjectList_Clear();
        priority->GetGrassList()->CopyTargetList(GetGrassList());
    }
    else if (priority->m_bHasRegionData)
    {
        return;
    }
    else if (bOverride)
    {
        if (priority->TESRegionData_GetOverride() && priority->TESRegionData_GetPriority() > TESRegionData_GetPriority())
        {
            FUN_004f15a0(priority->TESRegionData_GetOverride());
            FUN_004f15c0(priority->TESRegionData_GetPriority());
            GetGrassList()->TESRegionGrassObjectList_Clear();
            priority->GetGrassList()->CopyTargetList(GetGrassList());
        }
    }
    else if (priority->TESRegionData_GetOverride())
    {
        FUN_004f15a0(priority->TESRegionData_GetOverride());
        FUN_004f15c0(priority->TESRegionData_GetPriority());
        GetGrassList()->TESRegionGrassObjectList_Clear();
        priority->GetGrassList()->CopyTargetList(GetGrassList());
    }
    else
    {
        GetGrassList()->TESRegionGrassObjectList_MergeLists((float)TESRegionData_GetPriority(), priority->GetGrassList(), (float)priority->TESRegionData_GetPriority());
        source = TESRegionData_GetPriority() * TESRegionData_GetPriority() + (100 - TESRegionData_GetPriority()) * priority->TESRegionData_GetPriority();
        source +=
            (100 - priority->TESRegionData_GetPriority()) * TESRegionData_GetPriority() + priority->TESRegionData_GetPriority() * priority->TESRegionData_GetPriority();
        source *= 0.005f;
        FUN_004f15c0((unsigned char)floor(source));
    }
}
