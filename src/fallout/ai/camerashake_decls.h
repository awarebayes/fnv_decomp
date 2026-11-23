// fallout/ai/camerashake_decls.h: declares ModelLoader and FileFinder
// This unit calls the game's out-of-line fixed-string constructor and destructor.
#define FNV64_NIFIXEDSTRING_OWN_BODIES
#include "../../../include/NiNode.h"
#undef FNV64_NIFIXEDSTRING_OWN_BODIES

// pdb: class ModelLoader, size 0x30 (here 0x1), 12 members
class ModelLoader
{
  public:
    NiNode* LoadFile(const char*, int, bool, bool, bool, bool);
};
// pdb: class FileFinder, size 0x10 (here 0x1), 1 members
class FileFinder
{
  public:
    enum FILE_LOCATION
    {
        NONE = 0
    };
    static FILE_LOCATION Exist(const char*, char*, unsigned int, unsigned int);
};
extern ModelLoader* g_modelLoader;
extern NiNode* DAT_011dfecc;
extern "C" int __cdecl sprintf_s(char*, size_t, const char*, ...);
