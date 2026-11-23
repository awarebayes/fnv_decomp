// fallout/ai/modifierlist.cpp: ModifierListCalc (21 functions)

#include "modifierlist_decls.h"
#include "../../../include/ActorValueModifierList.h"
#include "../../../include/ActorValueInfo.h"
#include "../../../include/GameTimerRecord.h"
#include "../../../include/TESSaveLoadGame.h"

extern "C" int gModifierListIndex[];
extern "C" const double gModifierZero;

// Deletes every Modifier returned by GetItem while removing list heads, then deletes and nulls the stored object.
// @fnv 0x009370b0
// src: XBOX_RETAIL_PDB 0x009370b0
// pdb: ModifierList::DeleteAllModifiers | ModifierList.obj | retail, call graph, high
// pdb locals: pitem
void ActorValueModifierList::DeleteAllModifiers()
{
    Modifier* modifier;
    while ((modifier = GetItem()) != 0)
    {
        Modifier* node = modifier;
        ::operator delete(node);
        RemoveHead();
    }

    if (pFastModifiers)
    {
        delete pFastModifiers;
        pFastModifiers = 0;
    }
}

// Removes any existing modifier for key, then stores modifier in the fast-modifier table unless it is null or the zero value with no override.
// @fnv 0x00937280
// pdb: ModifierList::AddFastModifier | ModifierList.obj | retail, object-file run extension, low
// pdb params: acKey, apNew
void ActorValueModifierList::AddFastModifier(char key, Modifier* modifier)
{
    if (!modifier && !pFastModifiers)
        return;

    if (pFastModifiers)
        pFastModifiers->RemoveModifier(key);

    if (!modifier || (modifier->GetValue() == gModifierZero && !bZeroValuesAllowed))
        return;

    if (!pFastModifiers)
        pFastModifiers = new ModifierPointerList();

    pFastModifiers->m_ppModifiers[gModifierListIndex[key]] = modifier;
}

// @fnv 0x00937760
// pdb: ModifierList::GetModifierItem | ModifierList.obj | debug, call graph, high
// pdb params: acKey
// pdb locals: presult, plist, pitem
Modifier* ActorValueModifierList::GetModifierItem(char acKey)
{
    struct SearchState
    {
        ActorValueModifierList* current;
        Modifier* modifier;
    } searchState;
    searchState.modifier = 0;
    if (pFastModifiers && gModifierListIndex[acKey] < 0x4d)
    {
        searchState.modifier = pFastModifiers->m_ppModifiers[gModifierListIndex[acKey]];
    }
    else
    {
        searchState.current = this;
        while (searchState.current && searchState.current->GetItem() != 0 && !searchState.modifier)
        {
            Modifier* item = searchState.current->GetItem();
            if (item && item->GetKey() == acKey)
                searchState.modifier = item;
            searchState.current = (ActorValueModifierList*)searchState.current->GetNext();
        }
    }
    return searchState.modifier;
}

// Calls the two-argument overload with a fresh found flag and returns its float result for the given key.
// @fnv 0x00937730
// pdb: ModifierList::GetModifier | ModifierList.obj | retail, object-file run extension, high
// pdb params: acKey, abExists
// pdb locals: pModifier
float ActorValueModifierList::GetActorValueModifierAmount(char acKey)
{
    bool found = false;
    return GetActorValueModifierAmount(acKey, found);
}

// Looks up the modifier for key, sets found based on whether it exists, and returns its value or 0.0f.
// @fnv 0x009376e0
// pdb: ModifierList::GetModifier | ModifierList.obj | retail, object-file run extension, high
// pdb params: acKey, abExists
// pdb locals: pModifier
float ActorValueModifierList::GetActorValueModifierAmount(char acKey, bool& abExists)
{
    Modifier* modifier = GetModifierItem(acKey);
    abExists = (unsigned char)(modifier != 0);
    return modifier ? modifier->GetValue() : 0.0f;
}

// @fnv 0x00937160
// src: XBOX_DEBUG_PDB 0x00937160
// pdb: ModifierList::SetAllModifiers | ModifierList.obj | debug, call graph (shared callers), high
// pdb params: apSource
// pdb locals: plist, pitem
void ActorValueModifierList::SetAllModifiers(ActorValueModifierList* apSource)
{
    DeleteAllModifiers();
    if (!apSource)
        return;
    if (apSource->pFastModifiers)
        pFastModifiers = new ModifierPointerList(apSource->pFastModifiers);
    ActorValueModifierList* current = apSource;
    while (current && current->GetItem() != 0)
    {
        Modifier* item = current->GetItem();
        AppendModifier(new Modifier(item));
        current = (ActorValueModifierList*)current->GetNext();
    }
}

// @fnv 0x00937480
// pdb: ModifierList::AdjustModifier | ModifierList.obj | debug, call graph, high
// pdb params: acKey, afValue, aeClampStyle
// pdb locals: pModifier, fnewValue
void ActorValueModifierList::ModifyModifier(char acKey, float afValue, ClampStyle aeClampStyle)
{
    Modifier* modifier = GetModifierItem(acKey);
    if (!modifier)
    {
        if (afValue != gModifierZero || bZeroValuesAllowed)
        {
            modifier = new Modifier(acKey, afValue);
            AppendModifier(modifier);
        }
    }
    else
    {
        float value = ModifyAndClamp(modifier->GetValue(), afValue, aeClampStyle);
        modifier->SetValue(value);
        if (modifier->GetValue() == gModifierZero && !bZeroValuesAllowed)
            EraseModifier(modifier);
    }
}

struct FUN_009375b0_shell : Modifier
{
    Modifier* Modifier_ctor(char acKey, float afValue);
};

// Stores the key character, applies the float value via SetValue, and returns this modifier.
// @fnv 0x009375b0
// src: XBOX_DEBUG_PDB 0x009375b0
// pdb: Modifier::Modifier | ModifierList.obj | debug, call graph, high
// pdb params: acKey, afValue
Modifier* FUN_009375b0_shell::Modifier_ctor(char acKey, float afValue)
{
    cKey = acKey;
    SetValue(afValue);
    return this;
}

void* __cdecl operator new(size_t);
void __cdecl FastModifierCollection_CheckInitializeMapping();
extern int DAT_011e0750;

// Calls FUN_00937fd0, then allocates an int array of DAT_011e0750 elements, zeroes it, and stores the pointer through pThis.
// @fnv 0x00937d70
// src: XBOX_PDB 0x00937d70
// pdb: FastModifierCollection::FastModifierCollection | ModifierList.obj | debug, object-file run extension, low
// pdb locals: i
int** __fastcall FastModifierCollection_FastModifierCollection(int** apArrayOut)
{
    FastModifierCollection_CheckInitializeMapping();
    if (DAT_011e0750)
    {
        *apArrayOut = DAT_011e0750 ? new int[DAT_011e0750] : 0;
        for (int i = 0; i < DAT_011e0750; ++i)
        {
            (*apArrayOut)[i] = 0;
        }
    }
    else
    {
        *apArrayOut = 0;
    }
    return apArrayOut;
}

// @fnv 0x009375e0
// src: COMMUNITY 0x009375e0
// pdb: ModifierList::SetModifier | ModifierList.obj | retail, upstream name already matches, high
// pdb params: acKey, afValue
// pdb locals: pModifier
void ActorValueModifierList::ModifierList_SetModifier(char acKey, float afValue)
{
    Modifier* modifier = GetModifierItem(acKey);
    if (!modifier)
    {
        if (afValue != gModifierZero || bZeroValuesAllowed)
        {
            modifier = new Modifier(acKey, afValue);
            AppendModifier(modifier);
        }
    }
    else if (afValue == gModifierZero && !bZeroValuesAllowed)
    {
        EraseModifier(modifier);
    }
    else
    {
        modifier->SetValue(afValue);
    }
}

extern double _gModifierZero;

// Adds param1 and param2, then clamps the sum to 0.0f when param3 is 0 (if above _gModifierZero) or 1 (if below), returning the result.
// @fnv 0x00937810
float __cdecl ClampSumByMode(float afValueA, float afValueB, int aiMode)
{
    float result = afValueA + afValueB;
    switch (aiMode)
    {
    case 0:
        if (result > _gModifierZero)
            result = 0.0f;
        break;
    case 1:
        if (result < _gModifierZero)
            result = 0.0f;
        break;
    default:
        break;
    }
    return result;
}

// @fnv 0x00937b30
// pdb: ModifierList::Revert | ModifierList.obj | debug, call graph, medium
void ActorValueModifierList::Revert()
{
    DeleteAllModifiers();
}

struct FUN_00936fd0_shell
{
    char cKey;
    FUN_00936fd0_shell* CopyFrom(Modifier*);
};
// Copies the key and value from another Modifier into this one and returns this.
// @fnv 0x00936fd0
// pdb: Modifier::Modifier | ModifierList.obj | debug, object-file run extension, low
// pdb params: apSource
FUN_00936fd0_shell* FUN_00936fd0_shell::CopyFrom(Modifier* other)
{
    cKey = other->cKey;
    ((Modifier*)this)->SetValue(other->GetValue());
    return this;
}

void _Deallocate(void*);
struct Unk3713
{
    void FreeArrayItems();
    void* ReleaseIfFlagged(unsigned int);
};
// @fnv 0x00937130
// pdb: FastModifierCollection::`scalar deleting destructor' | ModifierList.obj | debug, object-file gap fill, medium
void* Unk3713::ReleaseIfFlagged(unsigned int flags)
{
    FreeArrayItems();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

extern TESSaveLoadGame* g_TESSaveLoadGame;
struct Host37870
{
    bool ModifierList_WriteModifier(Modifier* p);
};
// @fnv 0x00937870
// src: COMMUNITY 0x00937870
// pdb: ModifierList::WriteModifier | ModifierList.obj | debug, upstream name already matches, high
// pdb params: apModifier
// pdb locals: ckey, fvalue
bool Host37870::ModifierList_WriteModifier(Modifier* apModifier)
{
    if (apModifier == 0)
        return false;
    char key = apModifier->GetKey();
    float value = apModifier->GetValue();
    g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&key, 1);
    g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&value, 4);
    return true;
}

namespace fnv_009379c0 {
extern TESSaveLoadGame* g_TESSaveLoadGame;
// @fnv 0x009379c0
// src: XBOX_RETAIL_PDB 0x009379c0
// pdb: ModifierList::LoadGame | ModifierList.obj | retail, call graph (shared callers), high
// pdb locals: icount, i, pmod, ckey, fvalue
void __fastcall ModifierList_LoadGame(ActorValueModifierList* apLoadGameBuffer)
{
    unsigned short count;
    g_TESSaveLoadGame->ReadTypedData(&count, 2);
    for (unsigned int i = 0; i < count; i++)
    {
        char key;
        float value;
        Modifier* modifier = 0;
        g_TESSaveLoadGame->ReadTypedData(&key, 1);
        g_TESSaveLoadGame->ReadTypedData(&value, 4);
        modifier = new Modifier(key, value);
        apLoadGameBuffer->AppendModifier(modifier);
    }
}
}  // namespace fnv_009379c0

namespace fnv_009378d0 {
struct Modifier;

struct BSSimpleList_helper
{
    Modifier** _ref_006815c0();
};

extern TESSaveLoadGame* g_TESSaveLoadGame;

struct Host37870
{
    bool ModifierList_WriteModifier(Modifier*);
};

extern int DAT_011e0750;

// @fnv 0x009378d0
// src: XBOX_RETAIL_PDB 0x009378d0
// pdb: ModifierList::SaveGame | ModifierList.obj | retail, call graph (shared callers), high
// pdb locals: icount, pcount, ifastCount, pmodlist, pmod, i
void __fastcall ModifierList_SaveGame(ActorValueModifierList* apSaveGameBuffer)
{
    unsigned short icount = 0;
    unsigned short* pusCount = (unsigned short*)((const GameTimerRecord*)g_TESSaveLoadGame)->GetTime();
    g_TESSaveLoadGame->TESSaveLoadGame_SaveGameDataOLD(&icount, 2);

    BSSimpleList<Modifier*>* pmodlist = (BSSimpleList<Modifier*>*)apSaveGameBuffer;
    while (pmodlist && *((BSSimpleList_helper*)pmodlist)->_ref_006815c0())
    {
        Modifier* pmod = *((BSSimpleList_helper*)pmodlist)->_ref_006815c0();
        if (((Host37870*)apSaveGameBuffer)->ModifierList_WriteModifier(pmod))
            icount++;
        pmodlist = pmodlist->GetNext();
    }

    int ifastCount = apSaveGameBuffer->pFastModifiers ? DAT_011e0750 : 0;
    if (ifastCount)
    {
        for (int i = 0; i < ifastCount; i++)
        {
            ((Host37870*)apSaveGameBuffer)->ModifierList_WriteModifier((Modifier*)apSaveGameBuffer->pFastModifiers->m_ppModifiers[i]);
            icount++;
        }
    }

    *pusCount = icount;
}
}  // namespace fnv_009378d0

namespace fnv_00938060 {
struct FUN_00938060_s
{
    void** m_pEntries;
    void FreeModifierEntry(int aiIndex);
};

extern "C" void __cdecl _Deallocate(void*);
extern "C" int gModifierListIndex[];

// @fnv 0x00938060
// pdb: FastModifierCollection::DeleteFastModifier | ModifierList.obj | debug, object-file run extension, low
// pdb params: aiIndex
void FUN_00938060_s::FreeModifierEntry(int aiIndex)
{
    if (aiIndex >= 0 && aiIndex < 77 && m_pEntries)
    {
        if (gModifierListIndex[aiIndex] < 77 && m_pEntries[gModifierListIndex[aiIndex]])
        {
            void* pv = m_pEntries[gModifierListIndex[aiIndex]];
            _Deallocate(pv);
            m_pEntries[gModifierListIndex[aiIndex]] = 0;
        }
    }
}
}  // namespace fnv_00938060

extern unsigned int _data_011c16bc;
extern int DAT_011e0750;

struct FUN_00937e10
{
    Modifier** m_ppModifiers;

    FUN_00937e10(FUN_00937e10* other);
};

// Copy constructor: allocates an array of DAT_011e0750 Modifier pointers and deep-copies each non-null element from other, or stores null.
// @fnv 0x00937e10
// pdb: FastModifierCollection::FastModifierCollection | ModifierList.obj | debug, object-file run extension, low
// pdb params: apSource
// pdb locals: i
FUN_00937e10::FUN_00937e10(FUN_00937e10* other)
{
    if (other->m_ppModifiers)
    {
        m_ppModifiers = DAT_011e0750 ? new Modifier*[DAT_011e0750] : 0;

        for (int i = 0; i < DAT_011e0750; ++i)
        {
            if (other->m_ppModifiers[i])
            {
                m_ppModifiers[i] = new Modifier(other->m_ppModifiers[i]);
            }
            else
            {
                m_ppModifiers[i] = 0;
            }
        }
    }
    else
    {
        m_ppModifiers = 0;
    }
}

extern int DAT_011e0750;

struct ModifierListCalc : public ActorValueModifierList
{
    int GetSlotCount();
    unsigned short ModifierList_GetSaveSize();
};

// Computes a total from key4 times GetSlotCount() plus 2, plus key4 times a mask derived from DAT_011e0750, and returns it.
// @fnv 0x00937ab0
// src: COMMUNITY 0x00937ab0
// pdb: ModifierList::LoadGame | ModifierList.obj | retail, object-file run extension, low
// pdb note: debug build links it to ModifierList::GetSaveSize
// pdb locals: icount, i, pmod, ckey, fvalue
unsigned short ModifierListCalc::ModifierList_GetSaveSize()
{
    unsigned short total = 0;
    int mask;
    unsigned short key4 = 0;
    key4 = 5;
    if (key4 == 0)
    {
        return 0;
    }
    total += key4 * GetSlotCount() + 2;
    mask = ((pFastModifiers != 0) ? -1 : 0) & DAT_011e0750;
    total += key4 * mask;
    return total;
}

namespace ActorValue {
ActorValueInfo* GetActorValueInfo(Index aeActorValueIndex);
}

// Removes a modifier from the list, or from m_ppModifiedActorValues when the actor value is flagged and tracked there.
// @fnv 0x00937400
// pdb: ModifierList::DeleteModifier | ModifierList.obj | debug, object-file run extension, low
// pdb params: apModifier
// pdb locals: pavInfo
void ActorValueModifierList::EraseModifier(Modifier* modifier)
{
    if (!modifier)
        return;
    ActorValueInfo* pkInfo = ActorValue::GetActorValueInfo((ActorValue::Index)modifier->GetKey());
    if (pkInfo && (pkInfo->uFlags & 1) && pFastModifiers)
    {
        pFastModifiers->RemoveModifier(modifier->GetKey());
        return;
    }
    Remove(modifier);
    delete modifier;
}
