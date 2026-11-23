// fallout/misc/saveload/bgssaveloadfile.cpp: BGSSaveLoadFileEntry, SaveLoadFile, BGSSaveLoadManager (23 functions)

// The generated layout support has an enum-only NiFile declaration.
// Keep it separate from the shared BSFile base class.
#include <windows.h> // before layout_support.h: the shared BGSSaveLoadFileEntry.h needs the SDK's FILETIME
#include "layout_support.h"

#include "../../../../include/NiTexture.h"
#include "bgssaveloadfile_types_BGSSaveLoadFile.h"
#include "bgssaveloadfile_types_BGSSaveLoadFileEntry.h"
#include "../../../../include/BGSSaveLoadManager.h"
#include "bgssaveloadfile_types_BGSSaveLoadHeader.h"
#include "MemoryManager.h"
#include "../../../../include/BSFile.h"

extern BGSSaveLoadManager* DAT_011de134;

// These two operations are not declared by the shared manager yet.
struct SaveLoadManagerFileOperations : public BGSSaveLoadManager
{
    void DeleteWeaponFile(unsigned int, int);
    void RemoveAndDelete(void*);
};
extern int DAT_010a2480;

void __cdecl ReadSaveFileHeader(void*, void*);
NiTexture* __cdecl BGSSaveLoadHeader_LoadHeaderScreenshot(SaveLoadHeader*, unsigned int, unsigned int, unsigned int);
// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
class TaskQueueInterface
{
  public:
    static void CopyStringToBuffer(char*, unsigned int, const char*);
};
template <class T> unsigned int __cdecl BSStringLength(const T*);

// @fnv 0x00846b10
bool BGSSaveLoadFileEntry::IsAutoSave()
{
    return DAT_011de134->IsAutoSaveFileName(pFileName);
}

// @fnv 0x00846b30
// src: XBOX_RETAIL_PDB 0x00846b30
// pdb: BGSSaveLoadFileEntry::IsCorrupt | BGSSaveLoadFile.obj | retail, call graph (shared callers), high
bool BGSSaveLoadFileEntry::IsCorrupt()
{
    if (m_bSaveEntryValid)
        return true;
    LoadData();
    return m_bSaveEntryValid;
}

// Forwards a read of size bytes into buffer to the object's pFile stream and returns the result.
// @fnv 0x00846300
// pdb: BGSSaveLoadFile::Read | BGSSaveLoadFile.obj | retail, call graph (shared callers), high
// pdb params: apBuffer, aiBytes
unsigned int SaveLoadFile::Read(void* apBuffer, unsigned int aiBytes)
{
    return pFile->Read(apBuffer, aiBytes);
}

// @fnv 0x00846400
// pdb: SaveGameFormInfo::SaveGameFormInfo | BGSSaveLoadGame.obj | debug, call graph, medium
unsigned int SaveLoadFile::GetFilePosition()
{
    if (DAT_011de134->InterfaceManager_GetMenuMode())
        return m_uiFilePosition;
    return pFile->GetFilePosition();
}

// @fnv 0x00846440
// src: XBOX_DEBUG_PDB 0x00846440
// pdb: BGSSaveLoadFile::SeekSet | BGSSaveLoadFile.obj | debug, call graph, high
// pdb params: aiPosition
void SaveLoadFile::SeekSet(int aiPosition)
{
    if (DAT_011de134->InterfaceManager_GetMenuMode())
        m_uiFilePosition = aiPosition;
    else
        pFile->Seek(aiPosition, DAT_010a2480);
}

// @fnv 0x00846820
// src: XBOX_DEBUG_PDB 0x00846820
// pdb: BGSSaveLoadFileEntry::GetScreenshot | BGSSaveLoadFile.obj | debug, call graph, high
// pdb locals: ptexture, pfile
NiTexture* BGSSaveLoadFileEntry::GetScreenshot()
{
    NiTexture* screenshot = 0;
    void* header = DAT_011de134->ReadHeaderFromFile(pFileName, 0, 0, m_iScreenshotOffset);
    if (!header)
        return 0;
    if (!m_bHeaderParsed)
    {
        ReadSaveFileHeader(header, this);
        if (!m_uiHeaderRecordValue && !m_uiFileSize)
            m_bSaveEntryValid = true;
        if (!pLocation)
            m_bSaveEntryValid = true;
        m_bHeaderParsed = true;
    }
    if (m_uiHeaderRecordValue && m_uiFileSize)
        screenshot =
            BGSSaveLoadHeader_LoadHeaderScreenshot((SaveLoadHeader*)header, SaveFileMetadata, m_uiHeaderRecordValue, m_uiFileSize);
    DAT_011de134->CloseSaveFile((SaveLoadHeader*)header, 0);
    return screenshot;
}

// @fnv 0x008461e0
// pdb: BGSSaveLoadFile::BGSSaveLoadFile | BGSSaveLoadFile.obj | debug, object-file run extension, low
// pdb params: apFileName, abWrite, aiBufferSize
SaveLoadFile::SaveLoadFile(const char* fileName, bool flag, unsigned int size)
{
    pFile = 0;
    m_uiBufferSize = 0;
    m_uiFilePosition = 0;
    m_cFileName[0] = 0;
    if (!DAT_011de134->InterfaceManager_GetMenuMode())
        pFile = new BSFile(fileName, flag ? 1 : 0, size, false);
}

// @fnv 0x00846900
// src: XBOX_DEBUG_PDB 0x00846900
// pdb: BGSSaveLoadFileEntry::LoadData | BGSSaveLoadFile.obj | debug, call graph (shared callers), high
// pdb locals: pfile, ilength
void BGSSaveLoadFileEntry::LoadData()
{
    if (m_bHeaderParsed || m_bSaveEntryValid)
        return;
    void* header = DAT_011de134->ReadHeaderFromFile(pFileName, 0, 1, m_iScreenshotOffset);
    if (!header)
    {
        m_bSaveEntryValid = true;
        return;
    }
    ReadSaveFileHeader(header, this);
    if (!m_uiHeaderRecordValue && !m_uiFileSize)
        m_bSaveEntryValid = true;
    if (!pLocation)
        m_bSaveEntryValid = true;
    if (DAT_011de134->IsSaveFileNameCustom(pFileName))
    {
        char* location = pLocation;
        operator delete(location);
        unsigned int length = BSStringLength(pFileName) + 1;
        char* pAllocatedPath = (char*)operator new(length);
        pLocation = pAllocatedPath;
        TaskQueueInterface::CopyStringToBuffer(pLocation, length, pFileName);
    }
    DAT_011de134->CloseSaveFile((SaveLoadHeader*)header, 0);
    m_bHeaderParsed = true;
}

// Deletes the BSFile object stored at offset 0x104 of pThis.
// @fnv 0x008462c0
// src: GUESS 0x008462c0 (confidence: low)
// pdb: BGSSaveLoadFile::~BGSSaveLoadFile | BGSSaveLoadFile.obj | debug, object-file run extension, low
void __fastcall Object_ProcessState_008462c0(char* pThis)
{
    delete *reinterpret_cast<BSFile**>(pThis + 0x104);
}

struct UnknownOwner008463c0
{
    char pad[0x104];
    BSFile* unk104;
};

// @fnv 0x008463c0
// src: COMMUNITY 0x008463c0
// pdb: BGSSaveLoadFile::Open | BGSSaveLoadFile.obj | debug, upstream name already matches, high
bool __fastcall BGSSaveLoadFile_Open(UnknownOwner008463c0* _this)
{
    if (DAT_011de134->InterfaceManager_GetMenuMode())
        return true;
    return _this->unk104->Open(false, false);
}

extern int DAT_010a2484;

struct FUN_00846490_t
{
    char pad_000[0x104];
    BSFile* file;
    int unk_108;
    int pos_10c;
    void BGSSaveLoadFile_SeekCurrent(int aiPosition);
};

// If DAT_011de134->FUN_00424940() is true, adds offset to pos_10c; otherwise seeks file by offset using DAT_010a2484.
// @fnv 0x00846490
// src: XBOX_DEBUG_PDB 0x00846490
// pdb: BGSSaveLoadFile::SeekCurrent | BGSSaveLoadFile.obj | debug, call graph (shared callers), high
// pdb params: aiPosition
void FUN_00846490_t::BGSSaveLoadFile_SeekCurrent(int aiPosition)
{
    if (DAT_011de134->InterfaceManager_GetMenuMode())
    {
        pos_10c += aiPosition;
    }
    else
    {
        file->Seek(aiPosition, DAT_010a2484);
    }
}

struct FUN_008464f0_arg
{
    const char* str;
};

// @fnv 0x008464f0
// src: COMMUNITY 0x008464f0
// pdb: BGSSaveLoadFile::SetSaveFileName | BGSSaveLoadFile.obj | debug, upstream name already matches, high
// pdb params: apFileName
void __fastcall BGSSaveLoadFile_SetSaveFileName(char* pThis, FUN_008464f0_arg aStringArg)
{
    TaskQueueInterface::CopyStringToBuffer(pThis, 0x104, aStringArg.str);
}

// Allocates and constructs a new object from a name string, a bool flag and an unsigned int value, returning the new instance.
// @fnv 0x00846520
// src: COMMUNITY 0x00846520
// pdb: BGSSaveLoadFile::CreateFileA | BGSSaveLoadFile.obj | debug, object-file gap fill, medium
// pdb params: apFileName, abWrite, aiBufferSize
SaveLoadFile* __cdecl BGSSaveLoadFile_Create(const char* apName, bool abFlag, unsigned int auiValue)
{
    return new SaveLoadFile(apName, abFlag, auiValue);
}

// Deletes the objects stored at indices 0, 6, 5, 3 and 4 of the array pointed to by pThis.
// @fnv 0x00846690
// src: GUESS 0x00846690 (confidence: low)
// pdb: BGSSaveLoadFileEntry::~BGSSaveLoadFileEntry | BGSSaveLoadFile.obj | debug, object-file gap fill, medium
void __fastcall Bgssaveloadfile_ReleaseStorage_00846690(void** pThis)
{
    delete pThis[0];
    delete pThis[6];
    delete pThis[5];
    delete pThis[3];
    delete pThis[4];
}

struct FUN_008467b0_shell : public BGSSaveLoadFileEntry
{
    bool LoadSaveEntry();
};

// @fnv 0x008467b0
// pdb: BGSSaveLoadFileEntry::LoadGame | BGSSaveLoadFile.obj | debug, object-file gap fill, medium
bool FUN_008467b0_shell::LoadSaveEntry()
{
    return DAT_011de134->LoadGame(*(const char**)this, *(int*)((char*)this + 0x34), false, false);
}

namespace fnv_00846a20 {
struct FileTimeValue
{
    unsigned int dwLowDateTime;
    unsigned int dwHighDateTime;
};

struct _WIN32_FIND_DATAA_
{
    unsigned int dwFileAttributes;
    FileTimeValue ftCreationTime;
    FileTimeValue ftLastAccessTime;
    FileTimeValue ftLastWriteTime;
    unsigned int nFileSizeHigh;
    unsigned int nFileSizeLow;
    unsigned int dwReserved0;
    unsigned int dwReserved1;
    char cFileName[260];
    char cAlternateFileName[14];
};

__declspec(dllimport) void* __stdcall FindFirstFileA(const char*, _WIN32_FIND_DATAA_*);
__declspec(dllimport) int __stdcall FindClose(void*);

// @fnv 0x00846a20
// pdb: BGSSaveLoadFileEntry::CalculateFileTime | BGSSaveLoadFile.obj | debug, object-file gap fill, medium
void __fastcall UpdateFileWriteTime(char* pThis)
{
    if (*(unsigned int*)(pThis + 0x30) == 0 && *(unsigned int*)(pThis + 0x2c) == 0)
    {
        char path[260];
        _WIN32_FIND_DATAA_ findData;
        DAT_011de134->GetFullPath(*(char**)pThis, path, false);
        void* hFind = FindFirstFileA(path, &findData);
        if (hFind != (void*)-1)
        {
            *(FileTimeValue*)(pThis + 0x2c) = findData.ftLastWriteTime;
        }
        FindClose(hFind);
    }
}
}  // namespace fnv_00846a20

namespace fnv_00846ad0 {
// @fnv 0x00846ad0
// src: XBOX_PDB 0x00846ad0
// pdb: BGSSaveLoadFileEntry::IsQuicksave | BGSSaveLoadFile.obj | debug, upstream name already matches, high
bool __fastcall BGSSaveLoadFileEntry_IsQuicksave(const char** pThis)
{
    return DAT_011de134->IsQuickSaveFileName(*pThis);
}
}  // namespace fnv_00846ad0

namespace fnv_00846af0 {
// @fnv 0x00846af0
// pdb: BGSSaveLoadFileEntry::IsAutosave | BGSSaveLoadFile.obj | debug, call graph (shared callers), high
bool __fastcall IsLegacyAutosaveFileName(const char** apFileName)
{
    return DAT_011de134->IsSaveFileNameAutosave(*apFileName);
}
}  // namespace fnv_00846af0

struct MemoryManagerStatic
{
    static void* __cdecl Allocate(unsigned int);
};
void* __cdecl memset(void*, int, unsigned int);
int __cdecl strcpy_s(char*, unsigned int, const char*);
unsigned int __cdecl _strlen(const char*);
struct FUN_008465b0_stub
{
    char* name;
    char f4;
    char f5;
    int f8, fc, f10, f14, f18, f1c, f20, f24, f28;
    char blk[8];
    int f34;
    FUN_008465b0_stub(const char* src, int p);
};
// Zeroes the fields, stores p in f34, allocates a buffer for src, copies src into name, and clears blk.
// @fnv 0x008465b0
// pdb: BGSSaveLoadHeader::LoadHeaderData | BGSSaveLoadHeader.obj | retail, call graph (shared callers), low
// pdb params: apFile, apFileEntry
// pdb locals: isaveGameNumber, apLoadGameBuffer, iversion, iscreenshotWidth, loadBuffer, pheaderString, iscreenshotHeight, lname, iplayerLevel
FUN_008465b0_stub::FUN_008465b0_stub(const char* pcName, int p)
{
    f4 = 0;
    f5 = 0;
    f8 = 0;
    fc = 0;
    f10 = 0;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    f20 = 0;
    f24 = 0;
    f28 = 0;
    f34 = p;
    memset(blk, 0, 8);
    unsigned int len = _strlen(pcName);
    char* buf = (char*)MemoryManagerStatic::Allocate(len + 1);
    name = buf;
    strcpy_s(name, len + 1, pcName);
}

struct FUN_00846330_shell
{
    unsigned int BGSSaveLoadFile_Write(const void* apBuffer, unsigned int aiBytes);
};
// @fnv 0x00846330
// src: XBOX_RETAIL_PDB 0x00846330
// pdb: BGSSaveLoadFile::Write | BGSSaveLoadFile.obj | retail, object-file run extension, high
// pdb params: apBuffer, aiBytes
// pdb locals: ifileSizePositionDiff
unsigned int FUN_00846330_shell::BGSSaveLoadFile_Write(const void* apBuffer, unsigned int aiBytes)
{
    if (DAT_011de134->InterfaceManager_GetMenuMode())
    {
        unsigned int ifileSizePositionDiff =
            *(unsigned int*)((char*)this + 0x108) - *(unsigned int*)((char*)this + 0x10c);
        if (aiBytes > ifileSizePositionDiff)
            *(unsigned int*)((char*)this + 0x108) += aiBytes - ifileSizePositionDiff;
        *(unsigned int*)((char*)this + 0x10c) += aiBytes;
        return aiBytes;
    }
    return (*reinterpret_cast<BSFile**>((char*)this + 0x104))->NiBinaryStream_Write((const char*)apBuffer, aiBytes);
}

struct FUN_00846710_cls : public BGSSaveLoadFileEntry
{
    void BGSSaveLoadFileEntry_DeleteGame();
    bool BGSSaveLoadFileEntry_DeleteSave();
};

// @fnv 0x00846710
// src: COMMUNITY 0x00846710
// pdb: BGSSaveLoadFileEntry::SaveGame | BGSSaveLoadFile.obj | debug, object-file gap fill, medium
bool FUN_00846710_cls::BGSSaveLoadFileEntry_DeleteSave()
{
    if (DAT_011de134->IsSaveFileNameGenerated(pFileName) ||
        DAT_011de134->IsSaveFileNameAutosave(pFileName))
    {
        if (DAT_011de134->SaveGame_008503b0(0, m_iScreenshotOffset, 0))
        {
            BGSSaveLoadFileEntry_DeleteGame();
            return true;
        }
    }
    else
    {
        if (DAT_011de134->SaveGame_008503b0(pFileName, m_iScreenshotOffset, 0))
        {
            static_cast<SaveLoadManagerFileOperations*>(DAT_011de134)->RemoveAndDelete(this);
            return true;
        }
    }
    return false;
}

class C8467E0Event
{
  public:
    unsigned int m_uiType;
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
    int m_iData;

    void BGSSaveLoadFileEntry_DeleteGame();
};

// @fnv 0x008467e0
// src: XBOX_DEBUG_PDB 0x008467e0
// pdb: BGSSaveLoadFileEntry::DeleteGame | BGSSaveLoadFile.obj | debug, call graph, high
void C8467E0Event::BGSSaveLoadFileEntry_DeleteGame()
{
    static_cast<SaveLoadManagerFileOperations*>(DAT_011de134)->DeleteWeaponFile(m_uiType, m_iData);
    static_cast<SaveLoadManagerFileOperations*>(DAT_011de134)->RemoveAndDelete(this);
}

struct FUN_00850330_class
{
    char* BGSSaveLoadFile_ScalarDtor(unsigned int);
};
struct GlobalObject_0045cfe0
{
    int* Int();
};
extern GlobalObject_0045cfe0 DAT_011de2c8;
int __cdecl strcat_s(char*, unsigned int, const char*);
int __cdecl FUN_00ecbf05(const char*, int);
int __cdecl FUN_00ec862c(const char*, const char*);
__declspec(dllimport) int __stdcall DeleteFileA(const char*);
struct FUN_006815c0_Class
{
    void* FUN_006815c0();
};

// Builds successive .bak paths, deletes an existing destination, and calls FUN_00ec862c with source and destination paths.
// @fnv 0x00850100
// src: COMMUNITY 0x00850100
// pdb: BGSSaveLoadManager::CloseSaveFile | BGSSaveLoadManager.obj | retail, upstream name already matches, high
// pdb params: apFile, abWrite
// pdb locals: pfileName, psaveFileName
void BGSSaveLoadManager::CloseSaveFile(SaveLoadHeader* apFile, int abWrite)
{
    FUN_00850330_class* p2;
    FUN_00850330_class* p1;
    int inner4;
    char bufD_[260];
    char _bufE[260];
    int count2;
    char bufB[260];
    char bufC4[260];
    char bufA3[260];
    char* filename9;

    filename9 = (char*)((FUN_006815c0_Class*)apFile)->FUN_006815c0();
    strcpy_s(bufA3, 260, filename9);
    p1 = (FUN_00850330_class*)apFile;
    p2 = p1;
    (p2 ? p2->BGSSaveLoadFile_ScalarDtor(1) : (char*)0);
    if (*(unsigned char*)&abWrite)
    {
        GetFullPath(bufA3, bufB, *(bool*)&abWrite);
        strcpy_s(bufC4, 260, bufB);
        bufC4[BSStringLength(bufC4) - 4] = 0;
        for (count2 = *DAT_011de2c8.Int() - 1; count2 >= 0; count2--)
        {
            strcpy_s(bufD_, 260, bufC4);
            for (inner4 = 0; inner4 < count2; inner4++)
                strcat_s(bufD_, 260, ".bak");
            strcpy_s(_bufE, 260, bufD_);
            strcat_s(_bufE, 260, ".bak");
            if (FUN_00ecbf05(bufD_, 0) != -1)
            {
                if (FUN_00ecbf05(_bufE, 0) != -1)
                    DeleteFileA(_bufE);
                FUN_00ec862c(bufD_, _bufE);
            }
        }
        FUN_00ec862c(bufB, bufC4);
        strcpy_s(bufB, 260, bufC4);
    }
}
