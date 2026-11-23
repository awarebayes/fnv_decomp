#include "../../include/NiFormArray.h"
#include "../../include/TESIdleForm.h"

void* __cdecl operator new(size_t);
void __cdecl operator delete(void*);

// @fnv 0x005fe770
// pdb: TESIdleForm::CreateChildArray | TESIdleForm.obj | retail, call graph (shared callers), high
NiFormArray* TESIdleForm::AssignParentPath()
{
    return pChildIdles ? pChildIdles : (pChildIdles = new NiFormArray);
}
