#include "../../include/BSStringT.h"
// fallout_shared/tesfile.cpp: TESFile and related operations (87 functions)

#include <windows.h>

// Isolate legacy support definitions that overlap the shared headers.
#include "../../include/layout_support.h"

#define FNV64_BSSIMPLELIST_DEFINED
#include "OSGlobals.h"
#include "original_threads.h"
#include "game_constants.h"
#define __local_vftable_ctor_closure FNV64_LocalVftableConstructor
#define __vecDelDtor FNV64_VectorDeletingDestructor
#include "../../include/BGSSaveFormBuffer.h"
#include "../../include/BSFile.h"
#include "../../include/BSThread.h"
#include "../../include/BaseExtraList.h"
#include "../../include/BaseFormComponent.h"
#include "../../include/BaseProcess.h"
#include "../../include/DummyPointerMap.h"
#include "../../include/HighProcess.h"
#include "../../include/Menu.h"
#include "../../include/TESForm.h"
#include "../../include/TESSound.h"
#include "../../include/RecursiveSpinLock.h"

#undef __local_vftable_ctor_closure
#undef __vecDelDtor
#include "../../include/TESFile.h"

// The shared group-test declaration omits the boolean arguments used below.
extern unsigned int DAT_011873c8;

// @fnv 0x004739b0
// pdb: TESFile::GetThreadSafeFile | TESFile.obj | retail, call graph (shared callers), high
// pdb locals: pmainfile
TESFile* TESFile::GetParentTESFile()
{
    TESFile* file = this;
    while (file->GetThreadSafeRootFile())
    {
        file = file->GetThreadSafeRootFile();
    }
    if (file && gm_GetCurrentThreadId() == g_osGlobals->GetUptimeMilliseconds())
    {
        return file;
    }
    return file->GetFileForThread(gm_GetCurrentThreadId());
}

#ifndef FNV_FIRST_TARGET_ONLY
// @fnv 0x00473c70
// pdb: TESFile::CopyThreadSafeParentAuxBuffer | TESFile.obj | debug, object-file run extension, low
// pdb locals: pparentfile
TESFile* TESFile::GetThreadSafeRootFile()
{
    TESFile* parent = pThreadSafeParent;
    while (parent && parent->pThreadSafeParent)
    {
        parent = parent->pThreadSafeParent;
    }
    return parent;
}

// @fnv 0x00401680
bool TESFile::IsEndianConversionRequired()
{
    return bMustEndianConvert;
}

extern unsigned int kGroupFormType;

// Returns true when the current form is not a group form and its flags include kCompressedRecordFlag.
// @fnv 0x00472100
bool TESFile::IsCurrentFormCompressed()
{
    return m_currentform.form != kGroupFormType && (m_currentform.flags & kCompressedRecordFlag);
}

extern TESDataHandler* g_dataHandler;
// Returns the TESFile cached for threadID, creating and registering a new read-only TESFile from this file's path and metadata if none exists.
// @fnv 0x00473ae0
// pdb: TESFile::GetThreadSafeFileForThread | TESFile.obj | debug, object-file run extension, low
// pdb params: aiThreadID
// pdb locals: pretfile
TESFile* TESFile::GetFileForThread(unsigned int threadID)
{
    TESFile* file = 0;
    if (pThreadSafeFileMap)
    {
        pThreadSafeFileMap->GetAt(threadID, file);
    }
    if (!file)
    {
        file = new TESFile(GetPath(), GetFilename(), NiFile::READ_ONLY);
        file->SetMasterFile(IsMasterFile());
        file->SetCompileIndex(GetFileCompileIndex());
        file->GenIndexTable(g_dataHandler->GetFileList(), false);
        file->SetThreadSafeParent(this);
        file->OpenTESFile(NiFile::READ_ONLY, false);
        if (!pThreadSafeFileMap)
        {
            pThreadSafeFileMap = new NiTPointerMap<unsigned int, TESFile*>(kThreadSafeFileMapBuckets);
        }
        pThreadSafeFileMap->SetAt(threadID, file);
        file->InitializeComponent();
    }
    return file;
}
#endif

// @fnv 0x00470b90
BSSimpleList<_ULARGE_INTEGER*>* TESFile::GetMasterDataList()
{
    return &listMastersData;
}

// @fnv 0x00470c70
// pdb: TESFile::OpenTES | TESFile.obj | debug, call graph, high
// pdb params: accessmode, lock
bool TESFile::OpenTESFile(NiFile::OpenMode accessmode, bool lock)
{
    return OpenTES((const char*)m_Path, (const char*)m_Filename, accessmode, lock);
}

// @fnv 0x00471c20
// pdb: TESFile::ClearMasters | TESFile.obj | debug, object-file run extension, low
bool TESFile::IsMasterFile()
{
    if (m_Flags & 1)
        return true;
    else
        return false;
}

// @fnv 0x00471c50
void TESFile::SetMasterFile(bool master)
{
    if (master)
        m_Flags |= 1;
    else
        m_Flags &= ~1;
}

// @fnv 0x00471ca0
bool TESFile::GetOptimizedFile()
{
    if (m_Flags & 0x10)
        return true;
    else
        return false;
}

// @fnv 0x00471cd0
bool TESFile::GetIsChecked()
{
    if (m_Flags & 4)
        return true;
    else
        return false;
}

// Resets the current chunk id to CHUNK_ID_NO_CHUNK and clears the chunk size to 0.
// @fnv 0x004720d0
void TESFile::ClearCurrentChunk()
{
    m_currentchunkID = CHUNK_ID_NO_CHUNK;
    m_actualChunkSize = 0;
}

// @fnv 0x00473250
unsigned char TESFile::GetFileCompileIndex()
{
    return cCompileIndex;
}

// @fnv 0x00473640
bool TESFile::GetHasGroups()
{
    return bHasGroups;
}

// Walks up the parent chain to the topmost parent and stores it as this file's parent.
// @fnv 0x00473cb0
// pdb: TESFile::ClearAuxBuffer | TESFile.obj | debug, object-file run extension, low
// pdb locals: pparentfile
void TESFile::SetThreadSafeParent(TESFile* parent)
{
    while (parent->pThreadSafeParent)
        parent = parent->pThreadSafeParent;
    pThreadSafeParent = parent;
}

// @fnv 0x00473d20
void TESFile::SetTempIDOwner(bool owner)
{
    if (owner)
        m_Flags |= 0x20;
    else
        m_Flags &= ~0x20;
}

// @fnv 0x00473d70
// pdb: TESFile::AddAuxBuffer | TESFile.obj | debug, object-file run extension, low
// pdb params: aiMinIndex, aiMaxIndex
// pdb locals: bretval
bool TESFile::GetTempIDOwner()
{
    if (m_Flags & 0x20)
        return true;
    return false;
}

// @fnv 0x004724c0
// pdb: TESFile::GetOffsetChunk | TESFile.obj | debug, upstream name already matches, high
unsigned int TESFile::GetOffsetChunk()
{
    return m_fileoffset + GetFORMSize() + m_formoffset;
}

// @fnv 0x00473660
// pdb: TESFile::NextGroup | TESFile.obj | debug, call graph, high
// pdb locals: bsuccess
bool TESFile::IsGroupOpen()
{
    bool next = false;
    if (m_currentform.form == kGroupFormType)
    {
        m_currentform.form = 0;
        m_currentform.length -= GetFORMSize();
        next = HasNextForm();
    }
    return next;
}

struct GlobalObject_00680890;
extern GlobalObject_00680890 g_011c3400;
TESForm** __fastcall _ref_006815c0(GlobalObject_00680890*);
GlobalObject_00680890* __fastcall _ref_00726070(GlobalObject_00680890*);

// Iterates the global form list and calls the virtual operation at slot 0x22.
// @fnv 0x00408df0
// src: COMMUNITY 0x00408df0
// pdb: EffectSettingCollection::InitItems | EffectSettingCollection.obj | retail, upstream name already matches, high
// pdb locals: peffect, pnode
void EffectSettingCollection_InitItems()
{
    TESForm* pItem;
    for (GlobalObject_00680890* pNode = &g_011c3400; pNode && (pItem = *_ref_006815c0(pNode));
         pNode = _ref_00726070(pNode))
    {
        // This no-argument virtual overload is absent from the shared header.
        ((void (__thiscall*)(TESForm*))(*(void***)pItem)[0x22])(pItem);
    }
}

// Retrieves extra data of type 0x8c from the list and returns the int stored at offset 0xc, or 0 if absent.
// @fnv 0x0041c460
// src: XBOX_PDB 0x0041c460
// pdb: ExtraDataList::GetImpactSwap | ExtraDataList.obj | retail, upstream name already matches, high
// pdb locals: pExtra
int __fastcall ExtraDataList_GetImpactSwap(BaseExtraList* apExtraList)
{
    BSExtraData* pExtra = apExtraList->GetExtraData(0x8c);
    if (!pExtra)
    {
        return 0;
    }
    return *(int*)((char*)pExtra + 0xc);
}

extern float DAT_01019dd4;
extern unsigned int DAT_01186740;
void* __cdecl memset(void*, int, size_t);
int __fastcall FUN_00471400(void*);
bool __fastcall FUN_00471130(void*);
void __cdecl _FNV_Warning(const char*, ...);

// @fnv 0x00470710
// pdb: TESFile::TESFile | TESFile.obj | retail, upstream name already matches, high
// pdb params: ppath, pfilename, accessmode
TESFile::TESFile(const char* ppath, const char* pfilename, NiFile::OpenMode accessmode)
{
    bCached = 0;
    bCloseFileOverride = 0;
    pDecompressedFormBuffer = 0;
    iDecompressedFormBufferSize = 0;
    m_firstCellOffset = 0;
    m_currCellOffset = 0;
    m_pCurrCell = 0;
    pThreadSafeParent = 0;
    pThreadSafeFileMap = 0;
    bMustEndianConvert = 0;
    m_lastError = (TES_RETURN_CODE)0;
    m_filesize = 0;
    m_fileoffset = 0;
    m_formoffset = 0;
    m_chunkoffset = 0;
    m_Flags = 0;
    m_pMasterPtrs = 0;
    m_pLockedFile = 0;
    m_pFile = 0;
    iMasterCount = 0;
    cCompileIndex = 0xFF;
    m_currRefOffset = 0;
    m_saveformoffset = 0;
    m_savechunkoffset = 0;
    m_pBuffer = 0;
    m_uiBufferAllocSize = DAT_01186740;
    memset(&m_currentform, 0, 0x18);
    memset(&m_FileInfo, 0, 0x140);
    memset(&DeletedFormTime, 0, 8);
    m_actualChunkSize = 0;
    m_currentchunkID = (CHUNK_ID)0;
    memset(&fileHeaderInfo.m_fRecordValue, 0, 0xC);
    fileHeaderInfo.m_uiThirdUnsignedValue = 0x800;
    fileHeaderInfo.m_fRecordValue = DAT_01019dd4;
    if (!((TESFile*)this)->OpenTES(ppath, pfilename, accessmode, false))
        return;
    if ((m_FileInfo.nFileSizeHigh != 0 || m_FileInfo.nFileSizeLow != 0) && FUN_00471400(this) != 0)
        _FNV_Warning("MASTERFILE: File '%s' is not a valid TES file.", pfilename);
    if (!::FUN_00471130(this))
        _FNV_Warning("MASTERFILE: Could not close file '%s'.", pfilename);
}

BSSimpleList<char*>* __fastcall GetMasterList_00464df0(TESFile* pThis);

// @fnv 0x00470bc0
// pdb: TESFile::ClearMasterLists | TESFile.obj | retail, upstream name already matches, high
void TESFile::ClearMasterLists()
{
    while (::GetMasterList_00464df0(this)->GetItem())
    {
        delete ::GetMasterList_00464df0(this)->GetItem();
        ::GetMasterList_00464df0(this)->RemoveHead();
    }
    while (GetMasterDataList()->GetItem())
    {
        delete GetMasterDataList()->GetItem();
        GetMasterDataList()->RemoveHead();
    }
    iMasterCount = 0;
}

int __cdecl strcpy_s(char*, unsigned int, const char*);
int __cdecl strcat_s(char*, unsigned int, const char*);
char* __cdecl strchr(char*, int);
bool __fastcall TESDataHandler_GetClearingData(TESDataHandler*);
void __fastcall ProcessGroups(TESFile*);
void __fastcall FUN_00473960(TESFile*);
int __cdecl BSSystemFile_DeleteFileA(const char*);
int __cdecl FUN_00ec862c(const char*, const char*);
void __cdecl MessageHandler_ForceNextWarningToDisplay();
void __cdecl _FNV_Warning(const char*, ...);

// @fnv 0x00471130
// pdb: TESFile::CloseTES | TESFile.obj | retail, unique string literal, high
// pdb locals: pExt, szTempname, szFilename
bool TESFile::CloseTESFile()
{
    if (!bCloseFileOverride && g_dataHandler && !TESDataHandler_GetClearingData(g_dataHandler))
        return true;

    ProcessGroups(this);

    if (m_pFile)
    {
        delete m_pFile;
        m_pFile = 0;
    }

    FUN_00473960(this);

    if (m_pLockedFile)
    {
        char szFilename[260];
        char szTempname[260];
        char* pExt;

        strcpy_s(szFilename, 0x104, (char*)this + 0x124);
        strcat_s(szFilename, 0x104, (char*)this + 0x20);
        strcpy_s(szTempname, 0x104, (char*)this + 0x124);
        pExt = strchr((char*)this + 0x20, '.');
        if (pExt)
        {
            *pExt = 0;
            strcat_s(szTempname, 0x104, (char*)this + 0x20);
            strcat_s(szTempname, 0x104, ".tes");
            *pExt = '.';
        }
        else
        {
            strcat_s(szTempname, 0x104, (char*)this + 0x20);
            strcat_s(szTempname, 0x104, ".tes");
        }

        delete m_pLockedFile;
        m_pLockedFile = 0;

        if (BSSystemFile_DeleteFileA(szFilename))
        {
            MessageHandler_ForceNextWarningToDisplay();
            *(int*)this = 9;
            _FNV_Warning(
                "MASTERFILE: Unable to complete operation due to failure removing previous file.\r\nPlugin '%s' may be locked or read only.\r\n\r\nTemp file '%s' contains saved data.",
                szFilename, szTempname);
            return false;
        }
        if (FUN_00ec862c(szTempname, szFilename))
        {
            MessageHandler_ForceNextWarningToDisplay();
            *(int*)this = 9;
            _FNV_Warning(
                "MASTERFILE: Unable to complete operation due to failure renaming temp file.\r\nTemp file '%s' contains saved data.",
                szTempname);
            return false;
        }
    }

    return true;
}

// @fnv 0x00471820
void TESFile::SetGroupMode(bool abVal)
{
    bHasGroups = abVal;
}

unsigned int __fastcall GetFieldAt3FC(TESFile* _this);

// Returns this for index 0, otherwise the TESFile* at index-1 in the array at offset 0x400, or null if index is out of range or the array is unset.
// @fnv 0x00471a10
// pdb: TESFile::GetIndexFile | TESFile.obj | debug, call graph, medium
// pdb params: index
TESFile* TESFile::GetFileByIndex(unsigned int index)
{
    if (index < 0 || index > GetFieldAt3FC(this))
    {
        return 0;
    }
    if (index == 0)
    {
        return this;
    }
    if (!m_pMasterPtrs)
    {
        return 0;
    }
    return (m_pMasterPtrs)[index - 1];
}

unsigned int __fastcall GetFieldAt3FC(TESFile*);

// @fnv 0x00471ad0
// src: COMMUNITY 0x00471ad0
// pdb: TESFile::GetIndexCount | TESFile.obj | debug, upstream name already matches, high
unsigned int TESFile::GetIndexCount()
{
    return GetFieldAt3FC(this);
}

void* __cdecl memset(void*, int, unsigned int);
extern int DAT_010a2480;

// Seeks the stream to offset DAT_010a2480, zeroes the fields at 0x264-0x26c and the 0x18-byte block at 0x240, then calls GetTESForm if getform is set.
// @fnv 0x00471f80
// src: XBOX_RETAIL_PDB 0x00471f80
// pdb: TESFile::TESRewind | TESFile.obj | retail, call graph (shared callers), high
// pdb params: getform
void TESFile::TESRewind(bool getform)
{
    if (m_pFile != 0)
    {
        (m_pFile)->Seek(0, DAT_010a2480);
    }
    m_fileoffset = 0;
    m_formoffset = 0;
    m_chunkoffset = 0;
    memset((char*)this + 0x240, 0, 0x18);
    if (getform)
    {
        GetTESForm();
    }
}

// Returns the value at offset 0x38 unless it is -1, in which case it returns the value at offset 0x150.
// @fnv 0x004720a0
// src: XBOX_PDB 0x004720a0
int __fastcall BSFile_Tell(char* _this)
{
    return *(int*)(_this + 0x38) != -1 ? *(int*)(_this + 0x38) : *(int*)(_this + 0x150);
}

void __fastcall FUN_00473960(TESFile*);

// @fnv 0x00472150
// src: XBOX_PDB 0x00472150
// pdb: TESFile::NextForm | TESFile.obj | retail, call graph, high
// pdb params: abSkipIgnored
// pdb locals: bresult
bool TESFile::NextForm_00472150(bool abSkipIgnored)
{
    FUN_00473960(this);
    bool bresult = HasNextForm();
    while (bresult && abSkipIgnored &&
           m_currentform.form != kGroupFormType &&
           m_currentform.form !=
               DAT_011873c8 &&
           (m_currentform.flags & 0x1000))
    {
        bresult = HasNextForm();
    }
    return bresult;
}

// @fnv 0x00472380
// src: XBOX_PDB 0x00472380
int __fastcall MissileProjectile_GetImpactResult(int param_1)
{
    return *(int*)(param_1 + 0x150);
}

bool __fastcall ReadFormHeader(TESFile* _this);
void __fastcall ReadChunkHeader(TESFile* _this);

// If the file's field at offset 0x240 is zero, calls ReadFormHeader and, on success, clears the current chunk and calls ReadChunkHeader.
// @fnv 0x00472620
// pdb: TESFile::GetTESFormNoRet | TESFile.obj | debug, object-file gap fill, medium
void TESFile::ResetCurrentForm()
{
    if (*(int*)((char*)this + 0x240) == 0)
    {
        if (!::ReadFormHeader(this))
            return;
        ClearCurrentChunk();
        ::ReadChunkHeader(this);
    }
}

bool __fastcall ReadFormHeader(TESFile*);
int __cdecl FindFormEnumIndex(unsigned int);

// Keep the full enum index: the shared GetTESForm declaration returns only a byte.
// @fnv 0x00472660
// pdb: TESFile::GetTESForm | TESFile.obj | retail, call graph, high
int __fastcall TESFileGetFormType(TESFile* file)
{
    if (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(file) + 0x240) == 0)
    {
        if (!::ReadFormHeader(file))
        {
            return 0;
        }
        file->ClearCurrentChunk();
        file->GetChunkID();
    }
    return FindFormEnumIndex(*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(file) + 0x240));
}

template <int> struct EndianSizeTag
{
};

void __cdecl BSEndian(unsigned short*, EndianSizeTag<2>);

// Reads a 2-byte chunk into value, byte-swaps it if GetChunkData succeeds, and returns whether the read succeeded.
// @fnv 0x00472840
// pdb: TESFile::GetChunkData | TESFile.obj | retail, call graph (shared callers), high
// pdb params: apData
// pdb locals: bres
bool TESFile::GetChunkData(unsigned short* apData)
{
    bool ret = GetChunkData(apData, 2);
    if (GetChunkData())
        BSEndian(apData, EndianSizeTag<2>());
    return ret;
}

// @fnv 0x004731c0
// pdb: TESFile::GetPercentage | TESFile.obj | debug, object-file run extension, low
// pdb locals: currentPos
void TESFile::SetFileLanguage(int language)
{
    int iShiftedLanguage = language;
    iShiftedLanguage <<= 24;
    m_Flags &= 0x00FFFFFF;
    m_Flags |= iShiftedLanguage;
}

struct BGSLoadFormBuffer;
struct CHUNK;
struct HWND__;
struct NiColor;
struct NiPoint3;
struct TESBoundObject;
struct TESObjectREFR;
struct blob8
{
    char b[8];
};

typedef char _sizecheck_BaseFormComponent[1];

extern unsigned int DAT_01187014;
struct FormEnumEntry
{
    unsigned int formID;
    unsigned int b;
    unsigned int c;
};
extern FormEnumEntry DAT_01187008[];

// Seeks the file to apForm's form type and ID, skipping groups and other records, and returns true when the matching record is found.
// @fnv 0x004734d0
// src: XBOX_PDB 0x004734d0
// pdb: TESFile::FindForm | TESFile.obj | retail, upstream name already matches, high
// pdb params: apForm
// pdb locals: bretval, bkeeplooking, ifileformid, ifileformtype
bool TESFile::FindForm(TESForm* apForm)
{
    bool bretval = false;
    if (apForm && GetHasGroups() && *(BSThread**)((char*)this + 0x10) &&
        (*(BSThread**)((char*)this + 0x10))->TESAnimGroup_GetDecal())
    {
        if (apForm->Unk_1F(this))
        {
            return true;
        }
        this->TESRewind(true);
        while (m_currentform.form == DAT_01187014)
        {
            this->NextForm_00472150(true);
        }
        unsigned int uiFormType = DAT_01187008[apForm->GetTypeID()].formID;
        unsigned int ifileformid = ((TESSound*)apForm)->Deallocate();
        bool bkeeplooking = true;
        while (m_currentform.form != 0 && bkeeplooking)
        {
            if (m_currentform.form == kGroupFormType)
            {
                if (((bool (__thiscall*)(TESForm*, void*, bool, bool))(*(void***)apForm)[0x44])(apForm, (char*)this + 0x240, true, false))
                {
                    this->NextForm_00472150(true);
                }
                else
                {
                    IsGroupOpen();
                }
            }
            else if (m_currentform.form == uiFormType &&
                     *(unsigned int*)((char*)this + 0x24c) == ifileformid)
            {
                bkeeplooking = false;
                bretval = true;
            }
            else
            {
                this->NextForm_00472150(true);
            }
        }
    }
    return bretval;
}

unsigned int __cdecl SetWarningState(bool);
void __cdecl _FNV_Warning(const char*, ...);
int __cdecl _sprintf(char*, const char*, ...);

// Returns true and warns if the file version exceeds 1.34, otherwise returns false.
// @fnv 0x004738a0
// src: XBOX_RETAIL_PDB 0x004738a0
// pdb: TESFile::IsFileVersionTooHigh | TESFile.obj | retail, string-set bridge, high
// pdb locals: bretval, ptempstr
bool TESFile::IsFileVersionTooHigh()
{
    bool bretval = false;
    if (this->GetFileVersion() > 1.34f)
    {
        bretval = true;
        SetWarningState(false);
        char pMessage[260];
        _sprintf(pMessage, "File %s is a higher version than this EXE can load.", GetFilename());
        _FNV_Warning(pMessage);
        SetWarningState(true);
    }
    return bretval;
}

namespace fnv_00473ce0 {
extern int DAT_01186740;

// @fnv 0x00473ce0
void __fastcall SetField22C(char* pThis, float)
{
    *(int*)(pThis + 0x22c) = DAT_01186740;
}
}  // namespace fnv_00473ce0

extern NiTPointerMap<unsigned int, unsigned int>* DAT_011c40b0;
extern int DAT_011c40b4;
void* __fastcall NiTMap_unsigned_int_TESFile_NiTMap(void*, int, unsigned int);

struct TempMap_00473f20
{
    int pad[4];
    TempMap_00473f20(unsigned int n)
    {
        NiTMap_unsigned_int_TESFile_NiTMap(this, 0, n);
    }
};

// Creates the global ID-to-TESFile map if needed, maps aiID to apFile, and increments the counter when the file has no temp ID owner yet.
// @fnv 0x00473f20
// pdb: TESFile::SetTempIDOwnedByFile | TESFile.obj | debug, object-file gap fill, medium
// pdb params: aiID, apFile
void __cdecl RegisterTempFileID(unsigned int aiID, TESFile* apFile)
{
    if (!DAT_011c40b0)
    {
        DAT_011c40b0 = (NiTPointerMap<unsigned int, unsigned int>*)(new TempMap_00473f20(1001));
    }
    DAT_011c40b0->SetAt(aiID, (unsigned int)apFile);
    if (!apFile->GetTempIDOwner())
    {
        apFile->SetTempIDOwner(true);
        DAT_011c40b4 = DAT_011c40b4 + 1;
    }
}

extern int DAT_01031fac;

struct FUN_004744d0_arg
{
    unsigned int val;
};

void __fastcall FUN_005890c0(int* self, FUN_004744d0_arg a);

// Calls FUN_005890c0 on self, then stores the address of DAT_01031fac into *self and returns self.
// @fnv 0x004744d0
// src: EXE 0x004744d0
// pdb: NiTMap<unsigned int,TESFile *>::NiTMap<unsigned int,TESFile *> | TESFile.obj | debug, object-file gap fill, medium
// pdb params: uiHashSize
int* __fastcall NiTMap_unsigned_int_TESFile_NiTMap(int* self, FUN_004744d0_arg a)
{
    FUN_005890c0(self, a);
    *self = (int)&DAT_01031fac;
    return self;
}

// @fnv 0x0050f9c0
void BGSSaveFormBuffer::SetForm(TESForm* apForm)
{
    pForm = apForm;
}

// @fnv 0x008905f0
// src: XBOX_PDB 0x008905f0
bool __fastcall TESAnimGroup_GetDecal(bool* pFlags)
{
    return pFlags[0x2c];
}

// If GetSourceFiles returns a list, calls RemoveAll on it to clear the source files.
// @fnv 0x00460110
// pdb: TESForm::ClearFiles | TESDataHandler.obj | debug, call graph (shared callers), medium
void TESForm::ClearFormData()
{
    if (this->GetSourceFiles())
        this->GetSourceFiles()->RemoveAll();
}

void __cdecl _Deallocate(void* p);

// @fnv 0x00471850
unsigned int __thiscall TESFile::GetMasterFileCount()
{
    return iMasterCount;
}

extern int DAT_011c54d0;

// @fnv 0x00473080
int __fastcall GetTESFileGlobalValue(TESFile* file)
{
    return DAT_011c54d0;
}

// @fnv 0x004732f0
void* TESFile::GetGroupList()
{
    return &m_grouplist;
}

// This byte-returning helper has no matching shared member signature.
// @fnv 0x00473880
unsigned char __fastcall TESFileZeroByte(TESFile* file)
{
    unsigned char v = 0;
    return v;
}

// @fnv 0x00473d00
bool __thiscall TESFile::IsTempIDLoadable(unsigned int a, unsigned int b)
{
    bool c = true;
    (void)a;
    (void)b;
    return c;
}

void __fastcall NiTArray_unsigned_int_NiTMallocInterface_unsigned_int_dtor(void* map);

// @fnv 0x00473f00
// src: XBOX_DEBUG_PDB 0x00473f00
// pdb: NiTPrimitiveArray<unsigned int>::~NiTPrimitiveArray<unsigned int> | TESFile.obj | debug, call graph, high
void __fastcall NiTPrimitiveArray_unsigned_int_dtor(void* map)
{
    NiTArray_unsigned_int_NiTMallocInterface_unsigned_int_dtor(map);
}

extern NiTPointerMap<unsigned int, unsigned int>* DAT_011c40b0;

// Looks up aiID in DAT_011c40b0 and returns the mapped owner file value, or 0 if the map is null or the key is absent.
// @fnv 0x00474060
// src: XBOX_RETAIL_PDB 0x00474060
// pdb: TESFile::GetFileForTempID | TESFile.obj | retail, object-file run extension, high
// pdb params: aiID
// pdb locals: pownerfile
unsigned int __cdecl TESFile_GetFileForTempID(unsigned int aiID)
{
    unsigned int uiOwnerFile = 0;
    if (DAT_011c40b0 && ((DummyPointerMap*)DAT_011c40b0)->GetAt(aiID, uiOwnerFile))
        return uiOwnerFile;
    return 0;
}

extern int DAT_01202c50;

// @fnv 0x00c3bb80
// src: XBOX_PDB 0x00c3bb80
int* MessageHandler_GetWarningFileName()
{
    return &DAT_01202c50;
}

extern void* DAT_011c54cc;

// @fnv 0x00473070
void* __fastcall GetTESFileGlobalBuffer(TESFile* file)
{
    return DAT_011c54cc;
}

// @fnv 0x00473930
// pdb: TESFile::GetDecompressedBuffer | TESFile.obj | debug, upstream name already matches, high
char* TESFile::GetDecompressedBuffer()
{
    if (pDecompressedFormBuffer == 0)
        DecompressCurrentForm();
    return pDecompressedFormBuffer;
}

extern __int64 __cdecl FUN_00ec3bf0(__int64);
// Replaces the value at p with the result of FUN_00ec3bf0 applied to it.
// @fnv 0x004717f0
// src: GUESS 0x004717f0 (confidence: low)
void __cdecl TESFile_TransformReferencedInt64(__int64* pValue)
{
    __int64* q = pValue;
    *q = FUN_00ec3bf0(*q);
}

unsigned int __fastcall GetFieldAt3FC(TESFile*);
struct LItem_006815c0
{
    void** _ref_006815c0();
};
int _stricmp(const char*, const char*);
void _FNV_Warning(const char*, ...);

// Matches each required master filename against the file list, stores the matching TESFile pointers in the array at offset 0x400, and warns about missing masters.
// @fnv 0x00471870
// src: XBOX_RETAIL_PDB 0x00471870
// pdb: TESFile::GenIndexTable | TESFile.obj | retail, string-set bridge, high
// pdb params: pFileList, showerror
// pdb locals: pFList, foundall, pfile, size, pMList, pcheckfile
bool TESFile::GenIndexTable(void* pFileList, bool showerror)
{
    BSSimpleList<TESFile*>* pFList;
    bool foundall;
    TESFile** pfile;
    unsigned int size;
    BSSimpleList<char*>* pMList;
    TESFile* pcheckfile;
    pFList = (BSSimpleList<TESFile*>*)pFileList;
    pMList = (BSSimpleList<char*>*)GetMasterList();
    size = GetFieldAt3FC(this);
    if (m_pMasterPtrs)
        delete[] m_pMasterPtrs;
    m_pMasterPtrs = 0;
    if (!size)
        return true;
    m_pMasterPtrs = new TESFile*[size];
    pfile = m_pMasterPtrs;
    size = 0;
    foundall = true;
    while (pMList && *((LItem_006815c0*)pMList)->_ref_006815c0())
    {
        pFList = (BSSimpleList<TESFile*>*)pFileList;
        while (pFList && *((LItem_006815c0*)pFList)->_ref_006815c0())
        {
            pcheckfile = (TESFile*)*((LItem_006815c0*)pFList)->_ref_006815c0();
            if (!_stricmp((const char*)*((LItem_006815c0*)pMList)->_ref_006815c0(), pcheckfile->GetFilename()))
            {
                pfile[size] = pcheckfile;
                break;
            }
            pFList = pFList->GetNext();
            if (!pFList || !*((LItem_006815c0*)pFList)->_ref_006815c0())
            {
                pfile[size] = 0;
                if (showerror)
                    _FNV_Warning("MASTERFILE: Missing Masterfile: %s", *((LItem_006815c0*)pMList)->_ref_006815c0());
                foundall = false;
            }
        }
        size++;
        pMList = pMList->GetNext();
    }
    return foundall;
}

namespace fnv_00472bc0 {

struct MyForm
{
    void FUN_00470650();
};
extern int DAT_00ffffff;
bool FUN_00484b40(unsigned int formID);
extern "C" void* __cdecl memset(void*, int, size_t);

}  // namespace fnv_00472bc0

// @fnv 0x00472bc0
// pdb: TESFile::ReadFormHeader | TESFile.obj | retail, upstream name already matches, high
// pdb locals: bsuccess, pMaster
bool TESFile::ReadFormHeader()
{
    using namespace fnv_00472bc0;
    if (!this->m_pFile)
        return false;
    bool bsuccess = true;
    if (this->m_pFile->Read(&this->m_currentform.form, 0x18) != 0x18)
        bsuccess = false;
    if (!bsuccess)
    {
        fnv_00472bc0::memset(&this->m_currentform.form, 0, 0x18);
        return false;
    }
    if (IsEndianConversionRequired())
        ((MyForm*)&this->m_currentform.form)->FUN_00470650();
    if (this->m_currentform.form != kGroupFormType &&
        this->m_currentform.form != DAT_011873c8)
    {
        TESFile* pMaster = 0;
        if (this->m_pMasterPtrs)
            pMaster = GetMasterFile((this->m_currentform.iFormID >> 24) + 1);
        if (pMaster)
            this->m_currentform.iFormID = (pMaster->GetFileCompileIndex() << 24) |
                                                 (this->m_currentform.iFormID & (unsigned int)&fnv_00472bc0::DAT_00ffffff);
        else
            this->m_currentform.iFormID = (this->cCompileIndex << 24) |
                                                 (this->m_currentform.iFormID & (unsigned int)&fnv_00472bc0::DAT_00ffffff);
        if (FUN_00484b40(this->m_currentform.iFormID & (unsigned int)&fnv_00472bc0::DAT_00ffffff))
            this->m_currentform.iFormID = this->m_currentform.iFormID & (unsigned int)&fnv_00472bc0::DAT_00ffffff;
    }
    return true;
}

namespace fnv_00472d30 {

template <int N> struct EndianSizeTag
{
};
void __cdecl BSEndian(unsigned char*, EndianSizeTag<1>);
void* __cdecl __memcpy(void*, const void*, unsigned int);
struct FUN_00472d30_CHUNK
{
    unsigned int id;
    unsigned short size;
    void CHUNK_Endian();
};

}  // namespace fnv_00472d30

// @fnv 0x00472d30
// pdb: TESFile::ReadChunkHeader | TESFile.obj | retail, upstream name already matches, high
// pdb locals: tempchunk, pbuff, ichunkoffset, tempint
bool TESFile::ReadChunkHeader()
{
    using namespace fnv_00472d30;
    FUN_00472d30_CHUNK tempchunk;
    if (!IsCurrentFormCompressed())
    {
        if ((m_pFile)->Read(&tempchunk, 6) != 6)
        {
            ClearCurrentChunk();
            return false;
        }
    }
    else
    {
        unsigned int ichunkoffset = m_formoffset;
        char* pbuff = (char*)GetDecompressedBuffer();
        if (pbuff && ichunkoffset < ((HighProcess*)this)->HighProcess::GetPostAnimationAction())
        {
            pbuff += ichunkoffset;
            __memcpy(&tempchunk, pbuff, 6);
        }
        else
        {
            ClearCurrentChunk();
            return false;
        }
    }
    if (IsEndianConversionRequired())
        tempchunk.CHUNK_Endian();
    *(unsigned int*)((char*)this + 0x258) = tempchunk.id;
    m_actualChunkSize = tempchunk.size;
    if (tempchunk.id == 0x58585858)
    {
        unsigned int tempint = 0;
        GetChunkData(&tempint, 0);
        if (IsEndianConversionRequired())
            fnv_00472d30::BSEndian((unsigned char*)&tempint, fnv_00472d30::EndianSizeTag<1>());
        NextChunk();
        m_actualChunkSize = tempint;
    }
    return true;
}

extern "C" void* __cdecl _alloca(size_t);

struct S7010
{
    unsigned int Length();
};
extern BSStringT<char> g_menuParsedXML;
extern int DAT_011c5490;
struct FormTypeInfo
{
    unsigned char cType;
    char pad[11];
};
extern FormTypeInfo DAT_01187000[];

struct FUN_00482f40_t : Menu
{
    void TESDescription_LoadDescription(TESForm* apForm, unsigned int aeChunk);
};
// @fnv 0x00482f40
// src: XBOX_PDB 0x00482f40
// pdb: TESDescription::LoadDescription | TESDescription.obj | retail, upstream name already matches, high
// pdb params: apForm, aeChunk
// pdb locals: chunkid, pfile, bfoundchunk, formid, pMessage
void FUN_00482f40_t::TESDescription_LoadDescription(TESForm* apForm, unsigned int aeChunk)
{
    unsigned char cType;
    bool bFound;
    if (!apForm)
        return;
    if (DAT_011c5490 == (int)this && ((S7010*)&g_menuParsedXML)->Length())
        return;
    DAT_011c5490 = (int)this;
    ((::BSStringT<char>*)&g_menuParsedXML)->Set(0, 0);
    {
        TESFile* pFile = apForm->GetFile(-1);
        if (!pFile || !pMenu)
            return;
        pFile = pFile->GetParentTESFile();
        if (!pFile->SetOffset((unsigned int)pMenu))
            return;
        cType = pFile->GetTESForm();
        if (cType != DAT_01187000[apForm->FNV_GetFormType()].cType)
            return;
        bFound = false;
        do
        {
            unsigned int uiChunk = pFile->GetChunkID();
            if (uiChunk == aeChunk)
            {
                char* pBuf = (char*)_alloca(pFile->GetChunkSize());
                pFile->GetChunkData(pBuf, 0);
                ParseMenuXML(pBuf);
                bFound = true;
            }
        } while (!bFound && pFile->NextChunk());
    }
}

namespace fnv_00471400 {
struct _ULARGE_INTEGER
{
    unsigned long HighPart;
    unsigned long LowPart;
};
typedef char _sizecheck__ULARGE_INTEGER[1];

extern "C" void* __cdecl _alloca(size_t);
template <int N> struct EndianSizeTag
{
};
void __cdecl TESFile_TransformReferencedInt64(void*, EndianSizeTag<0>);
struct ContainerItemExtraFileData
{
    void Endian();
};
struct SoundListView
{
    void AddTail(char* const&);
};


extern unsigned int DAT_01187014;
extern int DAT_00ffffff;

}  // namespace fnv_00471400

// @fnv 0x00471400
// pdb: TESFile::LoadTESInfo | TESFile.obj | retail, call graph (shared callers), medium
// pdb locals: inexttype, pMessage, pfilename, length, plargeint
int TESFile::ReadTESInfo()
{
    using namespace fnv_00471400;
    unsigned char inexttype;

    ClearMasterLists();
    if (m_pFile == 0)
    {
        if (!OpenTES((const char*)this + 0x124, (const char*)this + 0x20, NiFile::READ_ONLY, false))
            return 2;
    }
    else
    {
        this->TESRewind(true);
    }
    m_filesize = (m_pFile)->GetSize();
    if (m_currentform.form != fnv_00471400::DAT_01187014)
        return 0xb;

    do
    {
        switch (*(int*)((char*)this + 0x258))
        {
        case 0x52444548:
            GetChunkData((char*)this + 0x3dc, 0xc);
            if (IsEndianConversionRequired())
                ((ContainerItemExtraFileData*)((char*)this + 0x3dc))->Endian();
            if (fileHeaderInfo.m_uiThirdUnsignedValue <= 0x7ff)
                fileHeaderInfo.m_uiThirdUnsignedValue = 0x800;
            fileHeaderInfo.m_uiThirdUnsignedValue =
                (*((unsigned char*)this + 0x40c) << 24) |
                (fileHeaderInfo.m_uiThirdUnsignedValue & (unsigned int)&fnv_00471400::DAT_00ffffff);
            break;
            break;
        case 0x4d414e43: {
            char* pMessage = (char*)fnv_00471400::_alloca(GetChunkSize());
            GetChunkData(pMessage, 0x200);
            ((::BSStringT<char>*)((char*)this + 0x410))->Set(pMessage, 0);
        }
        break;
        case 0x4d414e53: {
            char* pMessage = (char*)fnv_00471400::_alloca(GetChunkSize());
            GetChunkData(pMessage, 0x200);
            ((::BSStringT<char>*)((char*)this + 0x418))->Set(pMessage, 0);
        }
        break;
        case 0x5453414d: {
            unsigned int length = GetChunkSize();
            char* pfilename = new char[length];
            GetChunkData(pfilename, 0);
            ((SoundListView*)GetMasterList())->AddTail(pfilename);
            (iMasterCount)++;
        }
        break;
        case 0x41544144: {
            fnv_00471400::_ULARGE_INTEGER* plargeint = new fnv_00471400::_ULARGE_INTEGER;
            GetChunkData(plargeint, 8);
            if (IsEndianConversionRequired())
                fnv_00471400::TESFile_TransformReferencedInt64(plargeint, fnv_00471400::EndianSizeTag<0>());
            ((SoundListView*)GetMasterDataList())->AddTail(*(char**)&plargeint);
        }
        break;
        }
    } while (NextChunk());

    if (m_currentform.flags & 1)
        m_Flags |= 1;
    else
        m_Flags &= ~1;
    if (m_currentform.flags & 0x10)
        m_Flags |= 0x10;
    else
        m_Flags &= ~0x10;
    if (m_currentform.flags & 0x80)
        m_Flags |= 0x80;
    else
        m_Flags &= ~0x80;
    SetFileLanguage(m_Flags >> 24);

    while (TESFileGetFormType(this) == 1)
        this->NextForm_00472150(true);
    inexttype = TESFileGetFormType(this);
    SetGroupMode(inexttype == 2 || inexttype == 0);
    this->TESRewind(true);
    return 0;
}

namespace fnv_004721d0 {

extern "C" void* __cdecl memset(void*, int, size_t);

extern int DAT_010a2480;
int __fastcall MissileProjectile_GetImpactResult(int);
void __cdecl _FNV_Warning(const char*, ...);

struct FileFormHeader
{
    unsigned int type;
    unsigned int length;
    unsigned int flags;
    unsigned int formID;
    unsigned int stamp;
    unsigned int version;
};
struct FileChunkHeader
{
    unsigned int type;
    unsigned int size;
};
}  // namespace fnv_004721d0

// @fnv 0x004721d0
// pdb: TESFile::GetNextForm | TESFile.obj | debug, call graph, medium
// pdb locals: currentoffset
bool TESFile::HasNextForm()
{
    using namespace fnv_004721d0;
    unsigned int currentoffset;
    currentoffset = this->m_fileoffset;
    if (this->m_currentform.form == kGroupFormType ||
        this->m_currentform.form == DAT_011873c8)
        this->m_fileoffset += GetFORMSize();
    else
        this->m_fileoffset += GetFORMSize() + this->m_currentform.length;
    if (this->m_fileoffset >= this->m_filesize)
    {
        fnv_004721d0::memset(&this->m_currentform, 0, sizeof(FileFormHeader));
        return false;
    }
    currentoffset = this->m_fileoffset;
    if (fnv_004721d0::MissileProjectile_GetImpactResult((int)this->m_pFile) != currentoffset)
    {
        this->m_pFile->Seek(currentoffset, fnv_004721d0::DAT_010a2480);
        this->m_fileoffset = fnv_004721d0::MissileProjectile_GetImpactResult((int)this->m_pFile);
    }
    if (this->m_fileoffset == -1)
    {
        char* lpMsgBuf;
        FormatMessageA(0x1300, 0, GetLastError(), 0x400, (char*)&lpMsgBuf, 0, 0);
        fnv_004721d0::_FNV_Warning("FORMS: SetFilePointer() in NextForm failed with error:\n%s", lpMsgBuf);
        LocalFree(lpMsgBuf);
        return false;
    }
    this->m_formoffset = 0;
    this->m_chunkoffset = 0;
    fnv_004721d0::memset(&this->m_currentform, 0, sizeof(FileFormHeader));
    ResetCurrentForm();
    if (this->m_currentform.form == 0)
    {
        fnv_004721d0::_FNV_Warning("MASTERFILE: Trying to load a bad form in TESFile::NextForm.\r\nFile = %s\r\nOffset = %d",
                     this->m_Filename, this->m_fileoffset);
        return false;
    }
    return true;
}

// @fnv 0x00471a60
// src: COMMUNITY 0x00471a60
// pdb: TESFile::GetIndexName | TESFile.obj | debug, upstream name already matches, high
// pdb params: index
// pdb locals: pList, count
const char* TESFile::GetIndexName(unsigned int count)
{
    BSSimpleList<BGSPerk*>* list = reinterpret_cast<BSSimpleList<BGSPerk*>*>(GetMasterList_00464df0());
    if (list->GetItem() == 0)
        return 0;
    unsigned int index = 1;
    for (; index < count; index++)
    {
        list = static_cast<BSSimpleList<BGSPerk*>*>(list->GetNext());
        if (list == 0)
            return 0;
    }
    return (const char*)list->GetItem();
}

void __cdecl __memcpy(void* dst, const void* src, unsigned int size);
struct G2
{
    void AddHead(int*);
};

// @fnv 0x00473440
// pdb: TESFile::OpenGroup | TESFile.obj | debug, upstream name already matches, high
// pdb params: apGroupData
// pdb locals: pgroup
void TESFile::OpenGroup(FormRecordHeader* apGroupData)
{
    if (apGroupData)
    {
        void* pgroup = operator new(0x1c);
        void* plocal = pgroup;
        ((G2*)&m_grouplist)->AddHead(reinterpret_cast<int*>(&plocal));
        __memcpy(plocal, apGroupData, 0x18);
    }
}

namespace fnv_00473490 { void __fastcall ClearGroupStackEntry(TESFile*); }
struct FUN_004516b0_shell
{
    bool FUN_004516b0();
};
// Loops over groups from FUN_004732d0, calling FUN_004733a0 or FreeIfPresent depending on g_dataHandler's FUN_004516b0 result.
// @fnv 0x00473830
// pdb: TESFile::CloseAllOpenGroups | TESFile.obj | debug, object-file run extension, low
// pdb locals: pgroup
void TESFile::ClearGroupStack()
{
    void* pgroup = GetCurrentGroup();
    while (pgroup)
    {
        if (reinterpret_cast<FUN_004516b0_shell*>(g_dataHandler)->FUN_004516b0())
            CloseGroup();
        else
            fnv_00473490::ClearGroupStackEntry(this);
        pgroup = GetCurrentGroup();
    }
}

int DAT_010a2480;
int __fastcall BSFile_Tell(char*);

// Resets the form offset fields, seeks past the current form when it is not compressed, clears the current chunk and returns the chunk ID.
// @fnv 0x00472000
// pdb: TESFile::TESRewindChunk | TESFile.obj | debug, object-file run extension, low
// pdb locals: currentoffset
unsigned int __fastcall RewindTESFileForm(TESFile* file)
{
    *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(file) + 0x268) = 0;
    *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(file) + 0x26c) = 0;

    unsigned int position = *reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(file) + 0x268) +
                            (*reinterpret_cast<unsigned int*>(reinterpret_cast<char*>(file) + 0x264) + file->GetFORMSize());

    if (!file->IsCurrentFormCompressed())
    {
        (*reinterpret_cast<BSFile**>(reinterpret_cast<char*>(file) + 0x10))
            ->Seek(position, DAT_010a2480);
        position = BSFile_Tell(*reinterpret_cast<char**>(reinterpret_cast<char*>(file) + 0x10));
    }

    file->ClearCurrentChunk();
    return file->GetChunkID();
}

namespace fnv_00472e60 {
// Exe 0x00472e60: local storage view; GetFlags binds the folded DWORD-at-+8 accessor 0x0044ddc0.
struct TesfileOffset08View
{
    unsigned int GetFlags();
};

struct FormTypeInfo
{
    unsigned int formID;
    unsigned int flags;
    unsigned int unknown;
};

extern FormTypeInfo g_FormTypes[];

unsigned int __fastcall _FNV_GetFormType(TESForm*);
int __cdecl return15();
int __fastcall BSFile_Tell(char*);
extern int DAT_010a2488;

}  // namespace fnv_00472e60

// @fnv 0x00472e60
// pdb: TESFile::StartForm | TESFile.obj | debug, object-file gap fill, medium
// pdb params: apForm
int __thiscall TESFile::WriteForm(TESForm* apForm)
{
    using namespace fnv_00472e60;
    this->StartAndEndGroupsForForm(apForm);

    m_saveform.flags =
        reinterpret_cast<TesfileOffset08View*>(apForm)->GetFlags();
    m_saveform.flags =
        m_saveform.flags & 0x30032FE0;

    *reinterpret_cast<unsigned int*>(&m_saveform) = g_FormTypes[_FNV_GetFormType(apForm)].formID;

    m_saveform.iFormID =
        reinterpret_cast<TESSound*>(apForm)->Deallocate();

    *reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(this) + 0x284) = return15();
    *reinterpret_cast<unsigned short*>(reinterpret_cast<char*>(this) + 0x286) = 0;
    m_saveform.m_uiVersionControlInfo = 0;
    m_saveform.length = 0;

    m_pFile->Seek(0, fnv_00472e60::DAT_010a2488);

    m_saveformoffset =
        fnv_00472e60::BSFile_Tell(reinterpret_cast<char*>(m_pFile));
    m_savechunkoffset = 0;

    return this->EasyWrite(&m_saveform, 0x18);
}

// @fnv 0x004732d0
// pdb: TESFile::GetCurrentGroup | TESFile.obj | debug, upstream name already matches, high
TESFileGroup* TESFile::GetCurrentGroup()
{
    return m_grouplist.GetItem();
}

namespace fnv_00473490 {

struct BSSimpleList_standin
{
    void FUN_0063f7b0(void);
};

void _Deallocate(void* p);

// Calls FUN_004732d0 on this; if it returns non-null, calls FUN_0063f7b0 on the list at this+0x290 and deallocates the returned pointer.
// @fnv 0x00473490
// pdb: TESFile::CurrentGroupMayContainForm | TESFile.obj | debug, object-file run extension, low
// pdb params: apForm
// pdb locals: bsuccess, pgroup
void __fastcall ClearGroupStackEntry(TESFile* file)
{
    void* p = file->GetCurrentGroup();
    if (p)
    {
        ((BSSimpleList_standin*)((char*)file + 0x290))->FUN_0063f7b0();
        void* tmp = p;
        _Deallocate(tmp);
    }
}
}  // namespace fnv_00473490

// Nonvirtual operations not yet declared by the shared form header.
struct TESFormFileAccess : TESForm
{
    void FUN_004f15a0(int);
    bool TESForm_GetTemporary();
    void FUN_00403550(int);
    void FUN_00484f50(TESFile*);
};

// @fnv 0x00472f60
// pdb: TESFile::LoadForm | TESFile.obj | retail, upstream name already matches, high
// pdb params: apForm
void TESFile::LoadForm(TESForm* apForm)
{
    ((TESFormFileAccess*)apForm)->FUN_004f15a0(TESFileGetFormType(this));
    if (((TESFormFileAccess*)apForm)->TESForm_GetTemporary())
    {
        ((TESFormFileAccess*)apForm)->FUN_00403550(GetFlags() | 0x4000);
    }
    else
    {
        ((TESFormFileAccess*)apForm)->FUN_00403550(GetFlags());
    }
    ((void (__thiscall*)(TESForm*, unsigned int, bool))(*(void***)apForm)[0x4a])(apForm, GetFormID(), true);
    ((TESFormFileAccess*)apForm)->FUN_00484f50(this);
}

struct Fun472Param
{
    void* Probe1();
    int Probe2();
    void TESForm_FreeFormBuffer();
};
extern int DAT_010a2488;
// @fnv 0x00472fe0
// pdb: TESFile::AddTESForm | TESFile.obj | retail, call graph (shared callers), medium
// pdb params: apForm
// pdb locals: retVal
int TESFile::WriteFormContents(TESForm* apParam)
{
    int result;
    this->StartAndEndGroupsForForm(apParam);
    m_pFile->Seek(0, DAT_010a2488);
    result = this->EasyWrite(((Fun472Param*)apParam)->Probe1(), ((Fun472Param*)apParam)->Probe2());
    if (((Fun472Param*)apParam)->Probe1() && ((Fun472Param*)apParam)->Probe2())
    {
        fileHeaderInfo.m_uiFileRecordTotal++;
    }
    ((Fun472Param*)apParam)->TESForm_FreeFormBuffer();
    return result;
}

int* __cdecl FUN_00ec85e3();
void __cdecl Warning_005b5e40(const char* message);
extern const char DAT_0101a174[];

// @fnv 0x00473110
// src: XBOX_RETAIL_PDB 0x00473110
// pdb: TESFile::EasyWrite | TESFile.obj | retail, string-set bridge, high
// pdb params: ptr, count
// pdb locals: error
int __thiscall TESFile::EasyWrite(void* ptr, unsigned int count)
{
    if (ptr != 0)
    {
        if ((unsigned int)m_pFile->NiBinaryStream_Write((const char*)ptr, count) < count)
        {
            int error = *FUN_00ec85e3();
            m_lastError = TES_RETURN_CODE_TES_ERROR_WRITE_FAILURE;
            Warning_005b5e40(DAT_0101a174);
            return m_lastError;
        }
    }

    m_lastError = TES_RETURN_CODE_TES_ERROR_NONE;
    return m_lastError;
}

// SpawnObject (thiscall, 1 stack arg)

class SpawnedObject_b4476
{
  public:
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    unsigned int m_18;
};

extern unsigned int g_dat010a2488;

// If arg is non-zero, calls CalleeA(arg); when m_10 is set, spawns an object via CalleeB, copies m_10's value into its m_18, calls CalleeD, and increments m_3e0.
// @fnv 0x00473310
// pdb: TESFile::StartGroup | TESFile.obj | retail, call graph (shared callers), high
// pdb params: apNewGroupData
// pdb locals: pgroup
void TESFile::GetAnimationSequence(FormRecordHeader* apNewGroupData)
{
    if (apNewGroupData != 0)
    {
        OpenGroup(apNewGroupData);
        if (m_pFile != 0)
        {
            SpawnedObject_b4476* tmp = (SpawnedObject_b4476*)GetCurrentGroup();
            m_pFile->Seek(0, g_dat010a2488);
            tmp->m_18 = m_pFile->GetFilePosition();
            EasyWrite(tmp, 0x18);
            fileHeaderInfo.m_uiFileRecordTotal = fileHeaderInfo.m_uiFileRecordTotal + 1;
        }
    }
}

extern int DAT_010a2480;
extern int DAT_010a2488;
int __fastcall BSFile_Tell(char* _this);

struct FUN_004733a0_rec
{
    int unk00;
    int m_04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int m_18;
};

// @fnv 0x004733a0
// pdb: TESFile::EndGroup | TESFile.obj | retail, object-file run extension, low
// pdb note: debug build links it to TESFile::CloseGroup
// pdb locals: pgroup
void TESFile::CloseGroup()
{
    FUN_004733a0_rec* x = (FUN_004733a0_rec*)GetCurrentGroup();
    if (x != 0)
    {
        if (m_pFile != 0)
        {
            m_pFile->Seek(0, DAT_010a2488);
            x->m_04 = BSFile_Tell((char*)m_pFile) - x->m_18;
            m_pFile->Seek(x->m_18, DAT_010a2480);
            EasyWrite(x, 0x18);
        }
        fnv_00473490::ClearGroupStackEntry(this);
    }
}

extern unsigned int kGroupFormType;
void FNV_Warning(const char*, ...);

struct FormHeader
{
    unsigned int type;
    unsigned int length;
    unsigned int flags;
    unsigned int formID;
    unsigned int unk10;
    unsigned int unk14;
};

struct GroupForm;

struct GroupNode
{
    GroupForm** GetItem();
    GroupNode* GetNext();
};

// @fnv 0x004736c0
// src: XBOX_RETAIL_PDB 0x004736c0
// pdb: TESFile::StartAndEndGroupsForForm | TESFile.obj | retail, string-set bridge, high
// pdb params: apForm
// pdb locals: pgroup, pstopgroup, pgrouplist, groupform
void TESFile::StartAndEndGroupsForForm(TESForm* apForm)
{
    if (apForm && apForm->FNV_GetFormType() != 1)
    {
        GroupForm* group = 0;
        GroupForm* last = 0;
        GroupNode* node2 = (GroupNode*)GetGroupList();
        while (node2)
        {
            group = *node2->GetItem();
            if (group && !((bool (__thiscall*)(TESForm*, void*, bool, bool))(*(void***)apForm)[0x44])(apForm, group, true, true))
            {
                last = group;
            }
            node2 = node2->GetNext();
        }
        if (last)
        {
            group = (GroupForm*)GetCurrentGroup();
            while (group)
            {
                CloseGroup();
                if (group == last)
                {
                    group = 0;
                }
                else
                {
                    group = (GroupForm*)GetCurrentGroup();
                }
            }
        }
        group = (GroupForm*)GetCurrentGroup();
        while (!group || !((bool (__thiscall*)(TESForm*, void*, bool, bool))(*(void***)apForm)[0x44])(apForm, group, false, true))
        {
            FormHeader header;
            apForm->InitFormGRUP(&header, group);
            if (header.type == kGroupFormType)
            {
                GetAnimationSequence((FormRecordHeader*)&header);
                group = (GroupForm*)GetCurrentGroup();
            }
            else
            {
                FNV_Warning("FORMS: Failed to CreateGroupData for %s form '%s' (%08X)", apForm->TESForm_GetFormTypeString(),
                            apForm->GetName(), apForm->GetHavokWorldObjectCountInSceneGraph_0084e3a0());
                return;
            }
        }
    }
}

struct TESFormReferenceAccess : TESForm
{
    bool Check();
};

class FUN_00841250_pair
{
  public:
    unsigned int m_00;
    unsigned int m_04;
    FUN_00841250_pair* Build(TESFormReferenceAccess* r, unsigned int v);
};

class FUN_00841250_guard
{
  public:
    unsigned int m_00;
    unsigned int m_04;
    void Init();
};

class FUN_00841250_cls
{
  public:
    void Helper(int v);
    void Emit(void* p, FUN_00841250_pair v);
    void ProcessRef(TESFormReferenceAccess* apForm, void* aiFormID, unsigned int aiFlags);
};

// @fnv 0x00841250
// pdb: BGSReconstructFormsMap::AddForm | BGSReconstructFormsInAllFilesMap.obj | retail, call graph, high
// pdb params: apForm, aiFormID, aiFlags
// pdb locals: iformID, tempFormData, pref
void FUN_00841250_cls::ProcessRef(TESFormReferenceAccess* apForm, void* aiFormID, unsigned int aiFlags)
{
    void* p = apForm ? (void*)apForm->GetHavokWorldObjectCountInSceneGraph_0084e3a0() : aiFormID;
    FUN_00841250_guard gTmp;
    gTmp.Init();
    FUN_00841250_pair pr;
    TESFormReferenceAccess* r7;
    Emit(p, *pr.Build(apForm, aiFlags));
    if (apForm == 0)
    {
        Helper(6);
    }
    else
    {
        if (apForm->GetIsReference())
        {
            r7 = apForm;
            if (r7->Check())
            {
                Helper(2);
            }
            else
            {
                Helper(4);
            }
        }
        else
        {
            if (apForm->FUN_00401170() == 0x43)
            {
                Helper(4);
            }
        }
    }
}

struct FormPointReader
{
    float x;
    float y;
    float z;
    unsigned char ReadFromFile(TESFile* pFile);
};

void __cdecl BSEndian_00401080(float*, EndianSizeTag<4>);

// @fnv 0x00438fc0
// pdb: MultiBoundMarkerData::Load | MultiBoundMarkerData.obj | debug, object-file run extension, low
// pdb params: apFile
// pdb locals: bsuccess
unsigned char FormPointReader::ReadFromFile(TESFile* pFile)
{
    char result = 0;
    if (pFile != 0)
    {
        if (pFile->GetChunkID() == 0x4f424d58)
        {
            pFile->GetChunkData(this, 12);
            if (pFile->IsEndianConversionRequired())
            {
                BSEndian_00401080(&x, EndianSizeTag<4>());
                BSEndian_00401080(&y, EndianSizeTag<4>());
                BSEndian_00401080(&z, EndianSizeTag<4>());
            }
            result = 1;
        }
    }
    return result;
}

// @fnv 0x004709f0
// pdb: TESFile::~TESFile | TESFile.obj | retail, upstream name already matches, high
// pdb locals: pMList, pDList, pname, pdata
TESFile::~TESFile()
{
    bCloseFileOverride = true;
    CloseTESFile();
    BSSimpleList<char*>* pMaster = (BSSimpleList<char*>*)GetMasterList();
    while (pMaster)
    {
        char* pName7 = pMaster->GetItem();
        if (pName7)
            delete pName7;
        pMaster = pMaster->GetNext();
    }
    ((BSSimpleList<char*>*)GetMasterList())->RemoveAll();
    BSSimpleList<_ULARGE_INTEGER*>* pData8 = GetMasterDataList();
    while (pData8)
    {
        _ULARGE_INTEGER* pSize = pData8->GetItem();
        if (pSize)
            delete pSize;
        pData8 = pData8->GetNext();
    }
    GetMasterDataList()->RemoveAll();
    delete m_pBuffer;
    delete m_pMasterPtrs;
    m_pMasterPtrs = 0;
    ClearTempIDOwner();
    ClearThreadSafeFiles();
}

int __cdecl sprintf_s(char* buffer, unsigned int size, const char* format, ...);

// @fnv 0x00471af0
// pdb: TESFile::GetFileIndex | TESFile.obj | debug, object-file run extension, low
// pdb params: pFile
// pdb locals: index, count
bool TESFile::HasMasterFileChanged()
{
    _ULARGE_INTEGER* pSize4;
    void* hFind6;
    BSSimpleList<char*>* pMasters = (BSSimpleList<char*>*)::GetMasterList_00464df0(this);
    BSSimpleList<_ULARGE_INTEGER*>* pMastersData = GetMasterDataList();
    bool bChanged_ = false;
    while (pMasters && pMasters->GetItem() && pMastersData && pMastersData->GetItem())
    {
        char filePathX[260];
        _WIN32_FIND_DATAA findData;
        sprintf_s(filePathX, 260, "%s", pMasters->GetItem());
        pSize4 = pMastersData->GetItem();
        hFind6 = FindFirstFileA(filePathX, &findData);
        if (hFind6 != (void*)-1)
        {
            FindClose(hFind6);
            if (findData.nFileSizeLow != pSize4->LowPart || findData.nFileSizeHigh != pSize4->HighPart)
                bChanged_ = true;
        }
        else if (pSize4->LowPart || pSize4->HighPart)
        {
            bChanged_ = true;
        }
        pMasters = pMasters->GetNext();
        pMastersData = pMastersData->GetNext();
    }
    return bChanged_;
}

// @fnv 0x00473090
// pdb: TESFile::RewriteFormHeader | TESFile.obj | debug, upstream name already matches, high
// pdb locals: retVal
int TESFile::RewriteFormHeader()
{
    fileHeaderInfo.m_uiFileRecordTotal++;
    m_saveform.length = m_savechunkoffset;
    reinterpret_cast<BSFile*>(m_pFile)->Seek(m_saveformoffset, DAT_010a2480);
    int result = EasyWrite(&m_saveform, 0x18);
    return result;
}

struct Probe_A
{
    bool __thiscall FUN_008256d0();
    void __thiscall FUN_0063f7b0();
};

struct Probe_B
{
    unsigned int* __thiscall FUN_006815c0();
};

// @fnv 0x00473270
// src: COMMUNITY 0x00473270
// pdb: TESFile::DeleteFileList | TESFile.obj | retail, upstream name already matches, high
// pdb params: apFileList
// pdb locals: pfile
void __cdecl TESFile_DeleteFileList(void* apFileList)
{
    while (!((Probe_A*)apFileList)->FUN_008256d0())
    {
        unsigned int a = *((Probe_B*)apFileList)->FUN_006815c0();
        delete (TESFile*)a;
        ((Probe_A*)apFileList)->FUN_0063f7b0();
    }
}

typedef void* NiTMapIterator473;

class Map473
{
  public:
    virtual ~Map473();
    NiTMapIterator473 GetFirstPos();
    void GetNext(NiTMapIterator473& pos, unsigned int& uiKey, TESFile*& pkValue);
    void RemoveAll();
};

// Initializes and deletes the per-thread files, then destroys and clears their map.
// @fnv 0x00473a10
// pdb: TESFile::ClearThreadSafeFiles | TESFile.obj | retail, upstream name already matches, high
// pdb locals: iter, pfile, iignore
void TESFile::ClearThreadSafeFiles()
{
    if (pThreadSafeFileMap)
    {
        NiTMapIterator473 kPos = ((Map473*)pThreadSafeFileMap)->GetFirstPos();
        while (kPos)
        {
            unsigned int uiKey = 0;
            TESFile* pkValue = 0;
            ((Map473*)pThreadSafeFileMap)->GetNext(kPos, uiKey, pkValue);
            pkValue->InitializeComponent();
            delete pkValue;
        }
        ((Map473*)pThreadSafeFileMap)->RemoveAll();
        delete (Map473*)pThreadSafeFileMap;
        pThreadSafeFileMap = 0;
    }
}

struct TempKeys
{
    void* table;
    unsigned int* data;
    unsigned short capacity, size, count, grow;
    TempKeys(unsigned int, unsigned int);
    ~TempKeys();
    void SetAt(unsigned int, unsigned int&);
    unsigned int& GetAt(unsigned int);
};
struct MapView
{
    virtual ~MapView();
    void RemoveAll();
    void* GetFirstPos();
    void GetNext(void*&, unsigned int&, TESFile*&);
    bool RemoveAt(unsigned int);
};
// Clears the temp-ID owner flag; when the last owner is released it frees the map, otherwise it collects and removes this file's entries from it.
// @fnv 0x00473d90
// pdb: TESFile::RemoveFromOwnedIDMap | TESFile.obj | retail, call graph, medium
// pdb locals: iindex, ownedIDs, iter, iid, pfile, i
void TESFile::ClearTempIDOwner()
{
    if (GetTempIDOwner())
    {
        SetTempIDOwner(false);
        if (--DAT_011c40b4 == 0)
        {
            ((MapView*)DAT_011c40b0)->RemoveAll();
            delete (MapView*)DAT_011c40b0;
            DAT_011c40b0 = 0;
        }
        else
        {
            TempKeys position(100, 100);
            unsigned int keys = 0;
            void* count = ((MapView*)DAT_011c40b0)->GetFirstPos();
            while (count)
            {
                TESFile* key = 0;
                unsigned int owner = 0;
                ((MapView*)DAT_011c40b0)->GetNext(count, owner, key);
                if (key == this)
                {
                    position.SetAt(keys, owner);
                    keys = keys + 1;
                }
            }
            for (unsigned int i = 0; i < keys; ++i)
            {
                unsigned int owner = position.GetAt(i);
                ((MapView*)DAT_011c40b0)->RemoveAt(owner);
            }
        }
    }
}

namespace fnv_004740a0 {
struct AutoMemX
{
    int m;
    AutoMemX(int memContext, int overridable, const char* f, int aiLine);
    ~AutoMemX();
};
struct TagX
{
};
void BSEndian_00401080(void* p, TagX t);
void* FormHeap_Allocate(unsigned int n);
void FormHeapFreeAndLog_00401030(void* p);
bool FindFormEnumIndex(int id);
void Warning_005b5e40(const char* fmt, ...);
extern int g_iDat01187020;
void* AllocateProduct(void* opaque, unsigned int items, unsigned int size);
void simpleFree(void* opaque, void* apPtr);
struct ZStreamX
{
    char* m_pNextInputByte;
    unsigned int m_uiAvailableInputBytes;
    unsigned long m_ulTotalInputBytes;
    char* m_pNextOutputByte;
    unsigned int m_uiAvailableOutputBytes;
    unsigned long m_ulTotalOutputBytes;
    char* msg;
    void* state;
    void* (*m_pAllocateCallback)(void*, unsigned int, unsigned int);
    void (*m_pFreeCallback)(void*, void*);
    void* opaque;
    int data_type;
    unsigned long m_ulAdler32Checksum;
    unsigned long reserved;
};
int FUN_00b43fe0(ZStreamX* s, const char* ver, int size);
int FUN_00b44000(ZStreamX* s, int flush);
int FUN_00b45db0(ZStreamX* s);

// Reads a compressed form buffer from m10, inflates it with ZLib into m420, and frees the source buffer on success or failure.
}  // namespace fnv_004740a0

// @fnv 0x004740a0
// src: XBOX_RETAIL_PDB 0x004740a0
// pdb: TESFile::DecompressCurrentForm | TESFile.obj | retail, unique string literal, high
// pdb locals: tempMemContext, pbuffer, eform, ret, ioriginalsize, zstream, localState, pcompressedbuffer
void TESFile::DecompressCurrentForm()
{
    using namespace fnv_004740a0;
    AutoMemX kMem(0x30, 1, "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\TESFile.cpp", 0xf8e);
    if (m_currentform.length && m_currentform.form != g_iDat01187020 && IsCurrentFormCompressed())
    {
        bool bOk = fnv_004740a0::FindFormEnumIndex(m_currentform.form);
        if (!bOk)
            return;
        char* pBufferVal = (char*)FormHeap_Allocate(m_currentform.length + 1);
        if (!pBufferVal)
            return;
        if (m_pFile->Read(pBufferVal, m_currentform.length) != m_currentform.length)
        {
            fnv_004740a0::Warning_005b5e40("TESFile: Failed to read in buffer data for compressed form.");
        }
        else
        {
            unsigned int uiSize7 = *(unsigned int*)pBufferVal;
            char* pData4 = pBufferVal + 4;
            if (IsEndianConversionRequired())
                BSEndian_00401080(&uiSize7, TagX());
            pBufferVal[m_currentform.length] = 0;
            pDecompressedFormBuffer = (char*)FormHeap_Allocate(uiSize7);
            iDecompressedFormBufferSize = uiSize7;
            ZStreamX kStream;
            char acState[0x1ba4];
            kStream.m_pAllocateCallback = AllocateProduct;
            kStream.m_pFreeCallback = simpleFree;
            kStream.opaque = 0;
            kStream.m_uiAvailableInputBytes = 0;
            kStream.m_pNextInputByte = 0;
            kStream.state = acState;
            int iRet6 = FUN_00b43fe0(&kStream, "1.2.1", 0x38);
            if (iRet6)
            {
                FUN_00b45db0(&kStream);
                fnv_004740a0::Warning_005b5e40("TESFile: Error initializing ZLib inflate stream.");
                ClearDecompressedFormBuffer();
                FormHeapFreeAndLog_00401030(pBufferVal);
                return;
            }
            kStream.m_uiAvailableInputBytes = m_currentform.length - 4;
            kStream.m_pNextInputByte = pData4;
            kStream.m_uiAvailableOutputBytes = uiSize7;
            kStream.m_pNextOutputByte = pDecompressedFormBuffer;
            iRet6 = FUN_00b44000(&kStream, 0);
            if (iRet6 == -2 || iRet6 == 2 || iRet6 == -3 || iRet6 == -4)
            {
                FUN_00b45db0(&kStream);
                fnv_004740a0::Warning_005b5e40("TESFile: Error inflating ZLib stream.");
                ClearDecompressedFormBuffer();
                FormHeapFreeAndLog_00401030(pBufferVal);
                return;
            }
            if (iRet6 != 1)
            {
                FUN_00b45db0(&kStream);
                fnv_004740a0::Warning_005b5e40("TESFile: Error: ZLib stream did not terminate.");
                ClearDecompressedFormBuffer();
                FormHeapFreeAndLog_00401030(pBufferVal);
                return;
            }
            FUN_00b45db0(&kStream);
        }
        FormHeapFreeAndLog_00401030(pBufferVal);
    }
}

namespace fnv_00472890 {

void Warning_005b5e40(const char*, ...);
void* FUN_00401460(void*, const void*, unsigned int);
extern unsigned int DAT_010a2480;

// Reads the current chunk into data, seeking the file or copying from compressed memory, and truncates with a warning if the buffer is too small.
}  // namespace fnv_00472890

// @fnv 0x00472890
// pdb: TESFile::GetChunkData | TESFile.obj | retail, unique string literal, high
// pdb params: apData
// pdb locals: bres
bool TESFile::GetChunkData(void* pdata, unsigned int maxSize)
{
    using namespace fnv_00472890;
    if (m_actualChunkSize == 0)
        return true;
    bool compressed = IsCurrentFormCompressed();
    if (m_chunkoffset > 0)
    {
        if (!compressed)
        {
            unsigned int offset = m_fileoffset + GetFORMSize() + m_formoffset + 6;
            ((BSFile*)m_pFile)->Seek(offset, fnv_00472890::DAT_010a2480);
        }
        m_chunkoffset = 0;
    }
    if (maxSize != 0 && m_actualChunkSize > maxSize)
    {
        ((char*)pdata)[maxSize - 1] = 0;
        if (!compressed)
        {
            m_chunkoffset = ((BSFile*)m_pFile)->Read(pdata, maxSize - 1);
            if (m_chunkoffset != maxSize - 1)
            {
                char* message;
                FormatMessageA(0x1300, 0, GetLastError(), 0x400, (char*)&message, 0, 0);
                fnv_00472890::Warning_005b5e40("MASTERFILE: First ReadFile() in GetChunkData failed with error:\n%s", message);
                LocalFree(message);
                return false;
            }
        }
        else
        {
            unsigned int offset = m_formoffset + 6;
            char* source = GetDecompressedBuffer() + offset;
            FUN_00401460(pdata, source, maxSize - 1);
            m_chunkoffset = maxSize - 1;
        }
        char b[5];
        char a[5];
        b[0] = (char)(m_currentchunkID & 0xff);
        b[1] = (char)((m_currentchunkID & 0xff00) >> 8);
        b[2] = (char)((m_currentchunkID & 0xff0000) >> 16);
        b[3] = (char)((m_currentchunkID & 0xff000000) >> 24);
        b[4] = 0;
        a[0] = (char)(m_currentform.form & 0xff);
        a[1] = (char)((m_currentform.form & 0xff00) >> 8);
        a[2] = (char)((m_currentform.form & 0xff0000) >> 16);
        a[3] = (char)((m_currentform.form & 0xff000000) >> 24);
        a[4] = 0;
        fnv_00472890::Warning_005b5e40(
            "FORMS: Chunk size %d too big in chunk %s_ID in form %s_ID (%08X).\r\nMax size is %d, data truncated to \"%s\".\r\n",
            m_actualChunkSize, b, a, m_currentform.iFormID, maxSize, pdata);
    }
    else
    {
        if (!compressed)
        {
            m_chunkoffset = ((BSFile*)m_pFile)->Read(pdata, m_actualChunkSize);
            if (m_chunkoffset != m_actualChunkSize)
            {
                char* message;
                FormatMessageA(0x1300, 0, GetLastError(), 0x400, (char*)&message, 0, 0);
                fnv_00472890::Warning_005b5e40("MASTERFILE: Second ReadFile() in GetChunkData failed with error:\n%s", message);
                LocalFree(message);
                return false;
            }
        }
        else
        {
            unsigned int offset = m_formoffset + 6;
            char* source = GetDecompressedBuffer() + offset;
            FUN_00401460(pdata, source, m_actualChunkSize);
            m_chunkoffset = m_actualChunkSize;
        }
    }
    return true;
}

// Read raw bytes (component size 1)
// Reads uiBytes into pvBuffer via CallAndAccumulate with a single 1-byte element size and returns the number of bytes read.
// @fnv 0x00473180
// src: XBOX_PDB 0x00473180
// pdb: NiBinaryStream::Write | TESFile.obj | retail, upstream name already matches, high
// pdb params: pvBuffer, uiBytes
// pdb locals: uiBytesWritten, uiSize
int BSFile::NiBinaryStream_Write(const char* pvBuffer, unsigned int uiBytes)
{
    unsigned int uiElemSize = 1;
    unsigned int uiRead = NiBinaryStream::BinaryWrite(pvBuffer, uiBytes, &uiElemSize, 1);
    return uiRead;
}

void* __cdecl memset(void*, int, unsigned int);
int __fastcall BSFile_Tell(char*);
void _FNV_Warning(const char*, ...);
extern int DAT_010a2480;
void __fastcall FUN_00473960(TESFile*);

// Sets the record offset, seeks there when valid, reads a size, and resets record state on the invalid-offset and success paths.
// @fnv 0x004723a0
// pdb: TESFile::SetOffset | TESFile.obj | retail, upstream name already matches, high
// pdb params: newoffset
// pdb locals: currentoffset
bool TESFile::SetOffset(unsigned int newoffset)
{
    unsigned int currentoffset;
    FUN_00473960(this);
    m_fileoffset = newoffset;
    if (m_fileoffset >= m_filesize)
    {
        memset(&m_currentform, 0, 0x18);
        return false;
    }
    currentoffset = m_fileoffset;
    ((BSFile*)m_pFile)->Seek(currentoffset, DAT_010a2480);
    m_fileoffset = BSFile_Tell((char*)m_pFile);
    if (m_fileoffset == 0xFFFFFFFF)
    {
        char* lpMsgBuf;
        FormatMessageA(0x1300, 0, GetLastError(), 0x400, (char*)&lpMsgBuf, 0, 0);
        _FNV_Warning("MASTERFILE: SetFilePointer() in SetOffset failed with error:\n%s", lpMsgBuf);
        LocalFree(lpMsgBuf);
        return false;
    }
    m_formoffset = 0;
    m_chunkoffset = 0;
    memset(&m_currentform, 0, 0x18);
    GetTESForm();
    return true;
}

// For an uncompressed form, checks the computed record offset, seeks to newoffset when valid, and reloads current chunk state on success.
// @fnv 0x004724f0
// pdb: TESFile::SetOffsetChunk | TESFile.obj | retail, upstream name already matches, high
// pdb params: newoffset
// pdb locals: currentoffset
bool TESFile::SetOffsetChunk(unsigned int newoffset)
{
    unsigned int currentoffset;
    if (IsCurrentFormCompressed())
    {
        ::_FNV_Warning("FORMS: SetOffsetChunk failed for compressed form.");
        return false;
    }
    m_formoffset = newoffset - m_fileoffset - GetFORMSize();
    if (m_formoffset >= m_currentform.length)
    {
        ClearCurrentChunk();
        ::memset(&m_currentform, 0, sizeof(m_currentform));
        return false;
    }
    currentoffset = newoffset;
    ((BSFile*)m_pFile)->Seek(currentoffset, ::DAT_010a2480);
    currentoffset = ::BSFile_Tell((char*)m_pFile);
    if (currentoffset == -1)
    {
        char* lpMsgBuf;
        ::FormatMessageA(0x1300, 0, ::GetLastError(), 0x400, (char*)&lpMsgBuf, 0, 0);
        ::_FNV_Warning("FORMS: SetFilePointer() in SetOffsetChunk failed with error:\n%s", lpMsgBuf);
        ::LocalFree(lpMsgBuf);
        return false;
    }
    m_chunkoffset = 0;
    ClearCurrentChunk();
    GetChunkID();
    return true;
}
