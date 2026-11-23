// fallout_shared/creaturesounds.cpp: CreatureSounds (14 functions)

#include "creaturesounds_decls.h"

#include "../../include/TESCreature.h"
#include "../../include/TESSound.h"

#include "../../include/CreatureSounds.h"
extern "C" void* __cdecl memset(void*, int, size_t);

// @fnv 0x005f0360
// pdb: CreatureSounds::CreatureSounds | CreatureSounds.obj | retail, upstream name already matches, high
CreatureSounds::CreatureSounds()
{
    memset(this, 0, 0x58);
}

// Copies text into the BSStringT<char> at offset 0x1c and returns true.
// @fnv 0x005f0040
// pdb: BGSVoiceType::SetFormEditorID | BGSVoiceType.obj | retail, vtable slot, high
// pdb params: apEditorID
bool CreatureSounds::SetSoundName(const char* apEditorID)
{
    ((BSStringT<char>*)((char*)this + 0x1c))->Set(apEditorID, 0);
    return true;
}

// @fnv 0x005f0380
// pdb: CreatureSounds::~CreatureSounds | CreatureSounds.obj | debug, object-file gap fill, medium
void CreatureSounds::ClearSounds()
{
    FreeAllCreatureSoundLists();
}

// @fnv 0x005f7bf0
// src: XBOX_DEBUG_PDB 0x005f7bf0
// pdb: CreatureSounds::`scalar deleting destructor' | TESCreature.obj | debug, call graph, high
void* CreatureSounds::CreatureSounds_ScalarDtor(unsigned int flags)
{
    ClearSounds();
    if (flags & 1)
        operator delete(this);
    return this;
}

// @fnv 0x005f0900
// pdb: CreatureSounds::AddCreatureSound | CreatureSounds.obj | debug, call graph, medium
// pdb params: apSound, aeType
// pdb locals: plist, plastlist
void CreatureSounds::AddSoundForType(SoundListEntry* sound, CREATURE_SOUND_TYPE type)
{
    if (sound && type >= 0 && type < 22)
    {
        if (m_pSoundListsByType[type] == 0)
        {
            m_pSoundListsByType[type] = new BSSimpleList<SoundListEntry*>;
        }

        SoundListView* current = (SoundListView*)GetSoundListForType(type);
        SoundListView* previous = 0;
        while (current && !current->IsEmpty() && current->GetItem()->m_ucSortOrder <= sound->m_ucSortOrder)
        {
            previous = current;
            current = current->GetNext();
        }

        if (current)
            current->AddHead(sound);
        else if (previous)
            previous->AddTail(sound);
    }
}

// @fnv 0x005f0480
// src: XBOX_RETAIL_PDB 0x005f0480
// pdb: CreatureSounds::LoadCreatureSound | CreatureSounds.obj | retail, string-set bridge, high
// pdb params: apFile, aeType, apOwner
// pdb locals: bsuccess, pdata, tempint, tempbuff
bool CreatureSounds::LoadCreatureSound(TESFile* apFile, CREATURE_SOUND_TYPE aeType, TESCreature* apOwner)
{
    bool result = false;
    if (apFile)
    {
        SoundListEntry* sound = 0;
        if (apFile->GetChunkID() == 0x49445343)
        {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            sound = new SoundListEntry;
            if (id)
            {
                TESForm::RemapFormIDForFile(id, apFile);
                sound->SoundAssetTmp = dynamic_cast<TESSound*>(GetFormByID(id));
                if (!sound->SoundAssetTmp)
                    Warning(DAT_010475a0, id, apOwner->GetName(), apOwner->GetFormID());
            }
            else
            {
                sound->SoundAssetTmp = 0;
            }
            apFile->NextChunk();
        }
        else if (apFile->GetChunkID() == 0x46445343)
        {
            char filename[260];
            apFile->GetChunkData(filename, 260);
            sound = new SoundListEntry;
            sound->SoundAssetTmp = TESSound::FindSoundByFilename(filename);
            apFile->NextChunk();
        }
        if (sound)
        {
            if (apFile->GetChunkID() == 0x43445343)
            {
                apFile->GetChunkData(&sound->m_ucSortOrder, 1);
                AddSoundForType(sound, aeType);
                result = true;
            }
            else
            {
                delete sound;
            }
        }
    }
    return result;
}

struct FUN_005f0860_helper
{
    void ClearSoundList(int);
};

// Calls ClearSoundList for each index from 0 to 21 on this object.
// @fnv 0x005f0820
// src: COMMUNITY 0x005f0820
// pdb: CreatureSounds::FreeAllCreatureSoundLists | CreatureSounds.obj | retail, upstream name already matches, high
// pdb locals: i
void CreatureSounds::FreeAllCreatureSoundLists()
{
    for (char i = 0; i < 22; ++i)
    {
        ((FUN_005f0860_helper*)this)->ClearSoundList(i);
    }
}

struct SoundListView_helper : SoundListView
{
    void _ref_0063f7b0();
};

struct CreatureSounds_shell : CreatureSounds
{
    void ClearSoundList(CREATURE_SOUND_TYPE type);
};

// @fnv 0x005f0860
// pdb: TESRegionDataSound::ClearSoundList | TESRegionDataSound.obj | retail, upstream name already matches, high, not renamed: the link is the existing name itself, so its spelling stays
// pdb locals: psnd, tmpList
void CreatureSounds_shell::ClearSoundList(CREATURE_SOUND_TYPE type)
{
    SoundListView* list = (SoundListView*)GetSoundListForType(type);
    while (list && !list->IsEmpty())
    {
        SoundListEntry* item = list->GetItem();
        operator delete(item);
        ((SoundListView_helper*)list)->_ref_0063f7b0();
    }
    delete ((BSSimpleList<const char*>**)this)[type];
    ((BSSimpleList<SoundListEntry*>**)this)[type] = 0;
}

// Returns the list stored in the sound table at index aeType when it is in [0,22), otherwise returns null.
// @fnv 0x005f0ac0
// pdb: BSSimpleList<CREATURE_SOUND *>::~BSSimpleList<CREATURE_SOUND *> | CreatureSounds.obj | debug, object-file run extension, low
BSSimpleList<SoundListEntry*>* CreatureSounds::GetSoundListForType(CREATURE_SOUND_TYPE aeType)
{
    BSSimpleList<SoundListEntry*>* pretlist = 0;
    if (aeType >= 0 && aeType < 22)
    {
        pretlist = m_pSoundListsByType[aeType];
    }
    return pretlist;
}

// If both dst and src are non-null, copies the pointer at src into dst and the byte at offset 4 of src into offset 4 of dst.
// @fnv 0x005f07f0
void CopyPointerAndByte(void* apDest, void* apSource)
{
    if (apDest && apSource)
    {
        *(void**)apDest = *(void**)apSource;
        *(char*)((char*)apDest + 4) = *(char*)((char*)apSource + 4);
    }
}

void __cdecl CopyPointerAndByte(void*, void*);

struct FUN_005f0640_shell : public CreatureSounds
{
    void CreatureSounds_Copy(CreatureSounds* apCopy);
};

// @fnv 0x005f0640
// src: XBOX_DEBUG_PDB 0x005f0640
// pdb: CreatureSounds::Copy | CreatureSounds.obj | debug, call graph, high
// pdb params: apCopy
// pdb locals: i, plist, pnewdata, pdata
void FUN_005f0640_shell::CreatureSounds_Copy(CreatureSounds* apCopy)
{
    FreeAllCreatureSoundLists();
    if (apCopy)
    {
        for (char i = 0; i < 22; ++i)
        {
            BSSimpleList<SoundListEntry*>* pCurrentElementNext = apCopy->GetSoundListForType((CREATURE_SOUND_TYPE)i);
            while (pCurrentElementNext && !pCurrentElementNext->IsEmpty())
            {
                SoundListEntry* pdata = pCurrentElementNext->GetItem();
                SoundListEntry* pnewdata = new SoundListEntry;
                CopyPointerAndByte(pnewdata, pdata);
                AddSoundForType(pnewdata, (CREATURE_SOUND_TYPE)i);
                pCurrentElementNext = pCurrentElementNext->GetNext();
            }
        }
    }
}

unsigned int __cdecl DialogueRandom(void);

struct FUN_005f0a30_shell : CreatureSounds
{
    unsigned int CreatureSounds_PickCreatureSound(CREATURE_SOUND_TYPE aeType);
};

// Walks the sound list for a creature sound type and returns the first entry whose chance roll succeeds, or 0 if none.
// @fnv 0x005f0a30
// src: XBOX_DEBUG_PDB 0x005f0a30
// pdb: CreatureSounds::PickCreatureSound | CreatureSounds.obj | debug, call graph, high
// pdb params: aeType
// pdb locals: plist, presult, pdata
unsigned int FUN_005f0a30_shell::CreatureSounds_PickCreatureSound(CREATURE_SOUND_TYPE aeType)
{
    unsigned int result2 = 0;
    BSSimpleList<SoundListEntry*>* pSoundList = GetSoundListForType(aeType);
    while (pSoundList && !pSoundList->IsEmpty() && !result2)
    {
        SoundListEntry* sound = pSoundList->GetItem();
        if (sound->SoundAssetTmp && DialogueRandom() % 100 < sound->m_ucSortOrder)
            result2 = (unsigned int)sound->SoundAssetTmp;
        pSoundList = pSoundList->GetNext();
    }
    return result2;
}

void __cdecl TESForm_AddChunk_3(unsigned int, unsigned char);
void __cdecl _FUN_00485910(unsigned int, unsigned int);

struct FUN_005f03a0_shell : public CreatureSounds
{
    void ProcessEntries_005f03a0();
};

// @fnv 0x005f03a0
// src: GUESS 0x005f03a0 (confidence: low)
// pdb: CreatureSounds::AddCreatureSound | CreatureSounds.obj | retail, object-file run extension, low
// pdb note: debug build links it to CreatureSounds::Save
// pdb params: apSound, aeType
// pdb locals: plist, plastlist
void FUN_005f03a0_shell::ProcessEntries_005f03a0()
{
    for (int i = 0; i < 22; ++i)
    {
        BSSimpleList<SoundListEntry*>* pSoundList = GetSoundListForType(static_cast<CREATURE_SOUND_TYPE>(i));

        if (pSoundList && !pSoundList->IsEmpty())
        {
            _FUN_00485910(0x54445343, i);

            while (pSoundList && !pSoundList->IsEmpty())
            {
                SoundListEntry* SoundAssetTmp = pSoundList->GetItem();
                unsigned int formID = 0;

                if (SoundAssetTmp->SoundAssetTmp)
                    formID = SoundAssetTmp->SoundAssetTmp->GetFormID();

                _FUN_00485910(0x49445343, formID);
                TESForm_AddChunk_3(0x43445343, SoundAssetTmp->m_ucSortOrder);

                pSoundList = pSoundList->GetNext();
            }
        }
    }
}

struct FUN_005f06f0_shell : CreatureSounds
{
    bool CreatureSounds_Compare(CreatureSounds* apCompare);
};

// Compares the 22 creature sound lists of this object and apCompare, returning true if any entry differs or list lengths mismatch.
// @fnv 0x005f06f0
// src: XBOX_DEBUG_PDB 0x005f06f0
// pdb: CreatureSounds::Compare | CreatureSounds.obj | debug, call graph, high
// pdb params: apCompare
// pdb locals: i, plist, pcomplist, pdata, pcompdata
bool FUN_005f06f0_shell::CreatureSounds_Compare(CreatureSounds* apCompare)
{
    char i;

    if (apCompare == 0)
        return true;

    for (i = 0; i < 22; ++i)
    {
        BSSimpleList<SoundListEntry*>* pCurrentElementNext = this->GetSoundListForType(static_cast<CREATURE_SOUND_TYPE>(i));
        BSSimpleList<SoundListEntry*>* pOtherList = apCompare->GetSoundListForType(static_cast<CREATURE_SOUND_TYPE>(i));

        while (pCurrentElementNext != 0 && !pCurrentElementNext->IsEmpty() && pOtherList != 0 && !pOtherList->IsEmpty())
        {
            SoundListEntry* pdata = pCurrentElementNext->GetItem();
            SoundListEntry* pOtherEntry = pOtherList->GetItem();

            if (pdata->m_ucSortOrder != pOtherEntry->m_ucSortOrder)
                return true;

            if (pdata->SoundAssetTmp != pOtherEntry->SoundAssetTmp)
                return true;

            pCurrentElementNext = pCurrentElementNext->GetNext();
            pOtherList = pOtherList->GetNext();
        }

        if (pCurrentElementNext != 0 || pOtherList != 0)
            return true;
    }

    return false;
}
