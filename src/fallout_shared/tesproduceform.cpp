
// src/fallout_shared/tesproduceform.cpp - unit created 2026-10-16 from the whole-binary attribution (360 source path: d:/_fallout3/platforms/common/code/fallout shared/tesproduceform.cpp).
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

#include "../../include/BaseFormComponent.h"
#include "../../include/IngredientItem.h"
#include "../../include/TESProduceForm.h"
#include "../../include/TESRaceForm.h"

void* __cdecl memset(void* p, int c, unsigned int n);

// Sets the int at offset 4 to 0 and zeroes the 4 bytes at offset 8 of this object.
// @fnv 0x0048b590
// pdb: TESProduceForm::InitializeDataComponent | TESProduceForm.obj | debug, vtable slot, medium
void TESProduceForm::Init()
{
    *(reinterpret_cast<int*>(this) + 1) = 0;
    memset(reinterpret_cast<char*>(this) + 8, 0, 4);
}

extern "C" void* __cdecl __memcpy(void* dst, const void* src, unsigned int count);

// Copies 4 bytes from src into this object at offset 8 when src is non-null.
// @fnv 0x0048b670
// src: XBOX_DEBUG_PDB 0x0048b670
// pdb: TESProduceForm::SetAllProductionChances | TESProduceForm.obj | debug, call graph, high
// pdb params: apiChances
void TESProduceForm::SetAllProductionChances(void* apiChances)
{
    if (apiChances)
        __memcpy((char*)this + 8, apiChances, 4);
}

void __cdecl TESForm_AddChunkArray(unsigned int, void*, unsigned int);
void __cdecl _FUN_00485910(unsigned int, unsigned int);
// @fnv 0x0048b5c0
// pdb: TESProduceForm::Save | TESProduceForm.obj | retail, upstream name already matches, high
// pdb locals: itempint
void TESProduceForm::Save()
{
    unsigned int tmp;
    if (pFormIngredient != 0)
    {
        tmp = pFormIngredient->GetHavokWorldObjectCountInSceneGraph_0084e3a0();
        _FUN_00485910(0x47494650, tmp);
    }
    TESForm_AddChunkArray(0x43504650, iProduceChance, 4);
}

// Casts apCopyFC to TESProduceForm, initializes this from its ingredient value, then copies the data at offset 8.
// @fnv 0x0048b610
// pdb: TESProduceForm::CopyComponent | TESProduceForm.obj | retail, vtable slot, high
// pdb params: apCopyFC
// pdb locals: pCopy
void TESProduceForm::CopyFromBase(BaseFormComponent* apCopyFC)
{
    TESProduceForm* pCopy = dynamic_cast<TESProduceForm*>(apCopyFC);
    if (!pCopy)
        return;
    ((TESRaceForm*)this)->Initialize(pCopy->GetIngredient());
    SetAllProductionChances((void*)((char*)pCopy + 8));
}

extern "C" int __cdecl memcmp(const void* a, const void* b, size_t n);
// Compares production chance bytes and the ingredient value against another component, returning true when they differ or the other is not a TESProduceForm.
// @fnv 0x0048b6a0
// pdb: TESProduceForm::CompareComponent | TESProduceForm.obj | retail, vtable slot, high
// pdb params: apCompareFC
// pdb locals: pcompare
bool TESProduceForm::CompareWithBase(BaseFormComponent* apCompareFC)
{
    TESProduceForm* pOther = dynamic_cast<TESProduceForm*>(apCompareFC);
    if (!pOther)
        return true;
    if (GetIngredient() != pOther->GetIngredient())
        return true;
    if (memcmp(iProduceChance, pOther->iProduceChance, 4))
        return true;
    return false;
}

void ResolveFormID(unsigned int* apFormID, TESFile* apFile);
TESForm* TESForm_GetFormByNumericID(unsigned int aFormID);
void FNV_Warning(const char* apMsg, ...);

extern const char DAT_0101d2b4[];
extern const char DAT_0101d2f8[];
extern const char DAT_0101d340[];

// Resolves pIngredient's form ID through the file, re-looks it up as an IngredientItem, and warns if the lookup fails.
// @fnv 0x0048b720
// src: XBOX_RETAIL_PDB 0x0048b720
// pdb: TESProduceForm::InitItem | TESProduceForm.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: itempint
void TESProduceForm::InitItem(TESForm* apOwner)
{
    if (pFormIngredient != 0)
    {
        unsigned int formID = (unsigned int)pFormIngredient;
        ResolveFormID(&formID, apOwner->GetFile(-1));
        pFormIngredient = dynamic_cast<IngredientItem*>(TESForm_GetFormByNumericID(formID));
        if (pFormIngredient == 0)
        {
            if (apOwner != 0)
            {
                if (apOwner->GetFormEditorIDLength())
                {
                    FNV_Warning(DAT_0101d340, formID, apOwner->GetName());
                }
                else
                {
                    FNV_Warning(DAT_0101d2f8, formID, apOwner->GetHavokWorldObjectCountInSceneGraph_0084e3a0());
                }
            }
            else
            {
                FNV_Warning(DAT_0101d2b4, formID);
            }
        }
    }
}
