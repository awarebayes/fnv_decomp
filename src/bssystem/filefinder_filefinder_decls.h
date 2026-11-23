// bssystem/filefinder_filefinder_decls.h: declares FileFinder, ArchiveManager
#ifndef FNV_FILEFINDER_FILEFINDER_DECLS_H
#define FNV_FILEFINDER_FILEFINDER_DECLS_H

#include "../../include/BSFile.h"

// pdb: class FileFinder, size 0x10 (here 0x1), 1 members
class FileFinder
{
  public:
    enum FILE_LOCATION
    {
        NOTFOUND = 0,
        HARDDRIVE = 1,
        ARCHIVE = 2,
        CACHE = 3,
        PC = 4,
        DVD = 5,
        DVDTOHD = 6
    };
    static NiFile* GetFile(const char*, NiFile::OpenMode, unsigned int, unsigned int);
    static FILE_LOCATION __cdecl Exist(const char*, char*, unsigned int, unsigned int);
    FILE_LOCATION GetFileLocation(char*, char*, unsigned int, unsigned int);
    unsigned int FileFinder_LookForFile(char*, unsigned int, unsigned int);
};
extern "C" FileFinder* g_fileFinder;
extern "C" int __cdecl strcpy_s(char*, size_t, const char*);

extern MemoryManager g_memoryManager;
// pdb: class ArchiveManager, size 0x1, 0 members
class ArchiveManager
{
  public:
    static NiFile* GetFile(const char*, unsigned int, unsigned int);
};

#endif
