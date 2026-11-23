#include "../../include/NiFormArray.h"
#include "../../include/TESIdleForm.h"


// @fnv 0x005fe770
// pdb: TESIdleForm::CreateChildArray | TESIdleForm.obj | retail, call graph (shared callers), high
NiFormArray* TESIdleForm::AssignParentPath()
{
    return pChildIdles ? pChildIdles : (pChildIdles = new NiFormArray);
}
