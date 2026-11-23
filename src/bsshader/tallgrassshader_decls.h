// bsshader/tallgrassshader_decls.h: declares MemoryManager, TallGrassShader

#include "../../include/MemoryManager.h"
extern MemoryManager g_memoryManager;
// The lazily allocated constants object; constructor identity is unresolved.
class FUN_00ba94b0
{
  public:
    static void* operator new(size_t n)
    {
        return g_memoryManager.Allocate(n);
    }
    static void operator delete(void* p)
    {
        g_memoryManager.Deallocate(p);
    }
    FUN_00ba94b0(unsigned int);
    char data[20];
};
// pdb: class TallGrassShader, size 0x1d8 (here 0x40), bases: BSShader, 12 members
class TallGrassShader
{
  public:
    char prefix[60];
    FUN_00ba94b0* constants;
    void InitializeConstants(unsigned int);
};
