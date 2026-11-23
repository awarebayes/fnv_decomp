// gamebryo2.2/corelibs/nisystem/console/nifile_console.h: NiBinaryStream and NiFile come from include/
#ifndef FNV_ASTRA_NIFILE_H
#define FNV_ASTRA_NIFILE_H

#include <stddef.h>
#include "../../../../../include/NiFile.h"
extern "C" int __cdecl fclose(FILE*);
void __cdecl NiFree(void*);
typedef char check_NiBinaryStream_size[1];
typedef char check_NiFile_size[1];
typedef char check_NiFile_buffer[1];
typedef char check_NiFile_file[1];
typedef char check_NiFile_mode[1];
typedef char check_NiFile_good[1];
#endif
