// fallout_shared/bgsnote.cpp: BGSNote and note serialization operations (25 functions)

void* __cdecl operator new(size_t);
void __cdecl operator delete(void*);

#include "../../include/BGSNote.h"
#include "../../include/BSSimpleList.h"
#include "../../include/TESBoundObject.h"
#include "../../include/TESModel.h"
#include "../../include/TESFullName.h"
#include "../../include/TESIcon.h"
#include "../../include/MagicTarget.h"
#include "../../include/TESDescription.h"
#include "../../include/TESTexture.h"
#include "../../include/TESQuest.h"
#include "../../include/TESSound.h"
#include "../../include/TESTopic.h"
#include "../../include/TESActorBase.h"
#include "../../include/TESFile.h"
#include "../../include/ENUM_FORM_ID.h"
#include "../../include/BGSLoadFormBuffer.h"
#include "../../include/FormChangeMask.h"
#include "../../include/BGSPickupPutdownSounds.h"

// 0x00472660 is compared as a full ENUM_FORM_ID here; TESFile's shared declaration returns a byte.
class RecordFormTypeReader
{
  public:
    ENUM_FORM_ID GetTESForm();
};

int Warning(const char*, ...);
template <class T> struct BGSNoteReference
{
    unsigned int id;
    T* form;
};
bool __cdecl IsChunkTypeIcon(int);
extern "C" void* __cdecl _alloca(unsigned int);
#pragma intrinsic(_alloca)

// @fnv 0x005e8240
void BGSNote::ResetFormData()
{
    cType = 0;
    m_pText = 0;
    pSpeaker = 0;
    bHasBeenRead = false;
}

// Returns the note's TESDescription when GetNoteType reports 1, otherwise returns null.
// @fnv 0x005e91d0
// src: XBOX_DEBUG_PDB 0x005e91d0
// pdb: BGSNote::GetNoteText | BGSNote.obj | debug, call graph, high
TESDescription* BGSNote::GetNoteText()
{
    if (GetNoteType() == 1)
        return (TESDescription*)m_pText;
    return 0;
}

// @fnv 0x005e8060
// pdb: BGSNote::BGSNote | BGSNote.obj | retail, upstream name already matches, high
BGSNote::BGSNote()
{
    SetFormType(ENUM_FORM_ID_NOTE_ID);
    ResetFormData();
}

// If type differs from cType, deletes the current TESDescription or TESTexture, stores the new type, and allocates the matching object.
// @fnv 0x005e8f90
// src: XBOX_DEBUG_PDB 0x005e8f90
// pdb: BGSNote::SetNoteType | BGSNote.obj | debug, call graph, high
// pdb params: aeVal
void BGSNote::SetNoteType(NOTE_TYPE aeVal)
{
    if (aeVal != cType)
    {
        switch (cType)
        {
        case 1:
            delete (TESDescription*)m_pText;
            m_pText = 0;
            break;
        case 2:
            delete (TESTexture*)m_pText;
            m_pText = 0;
            break;
        }
        cType = (signed char)aeVal;
        switch (cType)
        {
        case 1:
            m_pText = new TESDescription;
            break;
        case 2:
            m_pText = new TESTexture;
            break;
        }
    }
}

// @fnv 0x005e8900
// pdb: BGSNote::InitItem | BGSNote.obj | retail, string-set bridge, high
// pdb locals: plist, plastlist, itempint, pquest, psound, ptopic, pspeaker
void BGSNote::InitItem()
{
    if (GetInitialized())
        return;
    BSSimpleList<TESQuest*>* current = &OwnerQuests;
    BSSimpleList<TESQuest*>* previous = 0;
    while (current && !current->IsEmpty())
    {
        BGSNoteReference<TESQuest> noteRef;
        noteRef.id = (unsigned int)current->GetItem();
        RemapFormIDForFile(noteRef.id, GetFile(-1));
        noteRef.form = dynamic_cast<TESQuest*>(GetFormByID(noteRef.id));
        if (noteRef.form)
        {
            current->SetItem(noteRef.form);
            previous = current;
            current = current->GetNext();
        }
        else
        {
            Warning("MASTERFILE: Unable to find owner quest (%08X) for Note '%s' (%08X).", noteRef.id, GetName(),
                    GetFormID());
            if (previous)
            {
                previous->Remove(current->GetItem());
                current = previous->GetNext();
            }
            else
                current->RemoveHead();
        }
    }
    if (GetNoteSound())
    {
        BGSNoteReference<TESSound> noteRef;
        noteRef.id = (unsigned int)GetNoteSound();
        RemapFormIDForFile(noteRef.id, GetFile(-1));
        noteRef.form = dynamic_cast<TESSound*>(GetFormByID(noteRef.id));
        SetNoteSound(noteRef.form);
        if (!noteRef.form)
            Warning("MASTERFILE: Unable to find sound (%08X) for Note '%s' (%08X).", noteRef.id, GetName(),
                    GetFormID());
    }
    if (GetNoteTopic())
    {
        BGSNoteReference<TESTopic> noteRef;
        noteRef.id = (unsigned int)GetNoteTopic();
        RemapFormIDForFile(noteRef.id, GetFile(-1));
        noteRef.form = dynamic_cast<TESTopic*>(GetFormByID(noteRef.id));
        SetNoteTopic(noteRef.form);
        if (!noteRef.form)
            Warning("MASTERFILE: Unable to find topic (%08X) for Note '%s' (%08X).", noteRef.id, GetName(),
                    GetFormID());
    }
    if (GetNoteSpeaker())
    {
        BGSNoteReference<TESActorBase> noteRef;
        noteRef.id = (unsigned int)GetNoteSpeaker();
        RemapFormIDForFile(noteRef.id, GetFile(-1));
        noteRef.form = dynamic_cast<TESActorBase*>(GetFormByID(noteRef.id));
        SetNoteSpeaker(noteRef.form);
        if (!noteRef.form)
            Warning("MASTERFILE: Unable to find actor base (%08X) for Note '%s' (%08X).", noteRef.id, GetName(),
                    GetFormID());
    }
    BGSPickupPutdownSounds::InitItem(this);
    SetInitializationState(true);
}

// Reads a NOTE record's chunks from the file, dispatching editor ID, bounds, full name, model, texture, sounds, type, owner quests and type-specific chunks.
// @fnv 0x005e84e0
// pdb: BGSNote::Load | BGSNote.obj | retail, upstream name already matches, high
// pdb params: apFile
// pdb locals: chunkid, pMessage, itempint, ctype, isize, ptemp, pdesc, pimage
bool BGSNote::Load(TESFile* apFile)
{
    if (reinterpret_cast<RecordFormTypeReader*>(apFile)->GetTESForm() != ENUM_FORM_ID_NOTE_ID)
        return false;
    LoadRecordHeader(apFile);
    int chunk;
    while (chunk = apFile->GetChunkID())
    {
        switch (chunk)
        {
        case 0x44494445: {
            char* editorID = (char*)_alloca(apFile->GetChunkSize());
            apFile->GetChunkData(editorID, 0x200);
            SetEditorID_AtRuntime(editorID);
            break;
        }
        case 0x444e424f:
            readOBNDSubRecord(apFile);
            break;
        case 0x4c4c5546:
            TESFullName::LoadFullNameChunk(this, apFile);
            break;
        case 0x4c444f4d:
        case 0x54444f4d:
            TESModel::LoadModelData(this, apFile);
            break;
        case 0x4e4f4349:
            TESTexture::ReadTextureData(this, apFile);
            break;
        case 0x4d414e59: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            SetPickupSound((TESSound*)id);
            break;
        }
        case 0x4d414e5a: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            SetPutdownSound((TESSound*)id);
            break;
        }
        case 0x41544144: {
            unsigned char type = 0;
            apFile->GetChunkData(&type, 1);
            SetNoteType((NOTE_TYPE)type);
            break;
        }
        case 0x4d414e4f: {
            unsigned int id = 0;
            apFile->GetChunkData(&id);
            OwnerQuests.AddHead((TESQuest*)id);
            break;
        }
        case 0x4d414e58: {
            unsigned int size = apFile->GetChunkSize();
            if (size)
            {
                char* name = (char*)_alloca(size);
                apFile->GetChunkData(name, size);
                if (GetNoteImage())
                    GetNoteImage()->AssignTextureName(name);
            }
            break;
        }
        default:
            switch (cType)
            {
            case 0:
                if (chunk == 0x4d414e53)
                {
                    unsigned int id = 0;
                    apFile->GetChunkData(&id);
                    SetNoteSound((TESSound*)id);
                }
                break;
            case 1:
                if (chunk == 0x4d414e54)
                {
                    TESDescription* description = GetNoteText();
                    if (description)
                        TESDescription::ReadDescriptionFromFile(description, apFile);
                }
                break;
            case 2:
                if (IsChunkTypeIcon(chunk))
                {
                    TESTexture* texture = GetNoteImage();
                    if (texture)
                        TESTexture::ReadTextureData(texture, apFile);
                }
                break;
            case 3:
                if (chunk == 0x4d414e53)
                {
                    unsigned int id = 0;
                    apFile->GetChunkData(&id);
                    SetNoteSpeaker((TESActorBase*)id);
                }
                else if (chunk == 0x4d414e54)
                {
                    unsigned int id = 0;
                    apFile->GetChunkData(&id);
                    SetNoteTopic((TESTopic*)id);
                }
                break;
            }
            break;
        }
        if (!apFile->NextChunk())
            break;
    }
    return true;
}


// @fnv 0x005e8d40
int BGSNote::GetNoteType()
{
    return cType;
}

// Returns the note's sound if its type is NT_SOUND, otherwise returns null.
// @fnv 0x005e9100
// src: XBOX_PDB 0x005e9100
// pdb: BGSNote::GetNoteSound | BGSNote.obj | debug, upstream name already matches, high
TESSound* BGSNote::GetNoteSound()
{
    if (GetNoteType() == NT_SOUND)
        return (TESSound*)m_pText;
    return 0;
}

// Returns the note texture if the note type is NT_IMAGE, otherwise returns null.
// @fnv 0x005e9160
// src: XBOX_DEBUG_PDB 0x005e9160
// pdb: BGSNote::GetNoteImage | BGSNote.obj | debug, call graph, high
TESTexture* BGSNote::GetNoteImage()
{
    if (GetNoteType() == NT_IMAGE)
        return (TESTexture*)m_pText;
    return 0;
}

// Returns the stored topic when GetNoteType() equals 3, otherwise returns null.
// @fnv 0x005e9240
// src: XBOX_PDB 0x005e9240
// pdb: BGSNote::GetNoteTopic | BGSNote.obj | debug, upstream name already matches, high
TESTopic* BGSNote::GetNoteTopic()
{
    if (GetNoteType() == 3)
        return (TESTopic*)m_pText;
    return 0;
}


// @fnv 0x005e9270
// pdb: BGSNote::SetNoteTopic | BGSNote.obj | debug, upstream name already matches, high
// pdb params: apTopic
void BGSNote::SetNoteTopic(TESTopic* apTopic)
{
    if (GetNoteType() == 3)
        m_pText = apTopic;
}

// Returns the stored TESActorBase pointer when GetNoteType() equals 3, otherwise returns null.
// @fnv 0x005e92a0
// src: XBOX_PDB 0x005e92a0
// pdb: BGSNote::GetNoteSpeaker | BGSNote.obj | debug, upstream name already matches, high
TESActorBase* BGSNote::GetNoteSpeaker()
{
    if (GetNoteType() == 3)
        return (TESActorBase*)pSpeaker;
    return 0;
}


// If GetNoteType() returns 3, stores apSpeaker into a field.
// @fnv 0x005e92d0
// pdb: BGSNote::SetNoteSpeaker | BGSNote.obj | debug, upstream name already matches, high
// pdb params: apSpeaker
void BGSNote::SetNoteSpeaker(TESActorBase* apSpeaker)
{
    if (GetNoteType() == 3)
        pSpeaker = apSpeaker;
}


// Stores the sound when the note type is NT_SOUND.
// @fnv 0x005e9130
// pdb: BGSNote::SetNoteSound | BGSNote.obj | debug, upstream name already matches, high
// pdb params: apSound
void BGSNote::SetNoteSound(TESSound* apSound)
{
    if (GetNoteType() == 0)
        m_pText = apSound;
}

void _Deallocate(void*);
// Runs the note destructor, frees this when flags has bit 1 set, and returns this.
// @fnv 0x005e8140
// pdb: BGSNote::`scalar deleting destructor' | BGSNote.obj | retail, vtable slot, high
void* BGSNote::Release(unsigned int flags)
{
    this->BGSNote::~BGSNote();
    if (flags & 1)
        _Deallocate(this);
    return this;
}

// @fnv 0x005e9190
// src: XBOX_DEBUG_PDB 0x005e9190
// pdb: BGSNote::SetNoteImage | BGSNote.obj | debug, call graph, high
// pdb params: apTex
void BGSNote::SetNoteImage(TESTexture* apTex)
{
    if (GetNoteType() == 2)
        ((TESTexture*)m_pText)->CopyFromBase((BaseFormComponent*)apTex);
}

// @fnv 0x005e9200
// pdb: BGSNote::SetNoteText | BGSNote.obj | debug, upstream name already matches, high
// pdb params: apDesc
void BGSNote::SetNoteText(TESDescription* apDesc)
{
    if (GetNoteType() == 1)
        ((TESDescription*)m_pText)->CopyFromBase((BaseFormComponent*)apDesc);
}

// @fnv 0x005e9480
// pdb: BGSNote::LoadGame | BGSNote.obj | retail, upstream name already matches, high
// pdb params: apLoadGameBuffer
void BGSNote::LoadGame(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::LoadGame(apLoadGameBuffer);
    if (apLoadGameBuffer->GetChangeFlags(0x80000000).operator bool())
        bHasBeenRead = true;
}

// @fnv 0x005e94d0
// pdb: BGSNote::Revert | BGSNote.obj | retail, upstream name already matches, high
// pdb params: apLoadGameBuffer
void BGSNote::Revert(BGSLoadFormBuffer* apLoadGameBuffer)
{
    TESForm::Revert(apLoadGameBuffer);
    if (apLoadGameBuffer->GetOldChangeFlags(0x80000000).operator bool())
        bHasBeenRead = false;
}

// @fnv 0x005e9300
// pdb: BGSNote::SetHasBeenRead | BGSNote.obj | debug, object-file gap fill, medium
// pdb params: abRead
void BGSNote::SetSoundNotePlayed(bool abRead)
{
    if (bHasBeenRead == abRead)
        return;
    bHasBeenRead = abRead;
    if (bHasBeenRead)
        MarkAsModified(0x80000000);
    else
        MarkAsUnmodified(0x80000000);
}

// @fnv 0x005e8170
// pdb: BGSNote::~BGSNote | BGSNote.obj | retail, upstream name already matches, high
BGSNote::~BGSNote()
{
    ClearNoteData();
}

// @fnv 0x005e8270
// pdb: BGSNote::ClearData | BGSNote.obj | retail, vtable slot, high
void BGSNote::ClearNoteData()
{
    OwnerQuests.RemoveAll();
    switch ((unsigned char)cType)
    {
    case 1:
        delete (TESDescription*)m_pText;
        m_pText = 0;
        break;
    case 2:
        delete (TESTexture*)m_pText;
        m_pText = 0;
        break;
    }
}

void SaveByteChunk(unsigned int, unsigned char);
void SaveIntChunk(unsigned int, unsigned int);
void SaveDataChunk(unsigned int, const void*, unsigned int);
void TESDescription_SaveDescriptionData(void*, unsigned int);

// @fnv 0x005e8300
// pdb: BGSNote::Save | BGSNote.obj | retail, vtable slot, high
// pdb locals: plist, itempint, pquest, psound, pdesc, pimage, ptopic, pspeaker
void BGSNote::SaveNote()
{
    TESForm::BeginFormRecord();
    TESFullName::Save_00487010();
    TESModel::WriteToBuffer(0x4c444f4d, 0x54444f4d);
    TESTexture::Save(0x4e4f4349);
    BGSPickupPutdownSounds::Save();
    SaveByteChunk(0x41544144, cType);
    BSSimpleList<TESQuest*>* pNode = GetOwnerQuests();
    while (pNode && !pNode->IsEmpty())
    {
        TESQuest* pOwner = pNode->GetItem();
        unsigned int uiOwnerQuestID = pOwner->GetFormID();
        SaveIntChunk(0x4d414e4f, uiOwnerQuestID);
        pNode = pNode->GetNext();
    }
    switch (cType)
    {
    case 0: {
        TESSound* SoundAssetTmp = GetNoteSound();
        if (SoundAssetTmp)
        {
            unsigned int uiSoundID = SoundAssetTmp->GetFormID();
            SaveIntChunk(0x4d414e53, uiSoundID);
        }
        break;
    }
    case 1: {
        void* pText = GetNoteText();
        if (pText)
            TESDescription_SaveDescriptionData(pText, 0x4d414e54);
        break;
    }
    case 2: {
        TESTexture* pNoteTexture = GetNoteImage();
        if (pNoteTexture)
            SaveDataChunk(0x4d414e58, pNoteTexture->GetTextureResourceName(), pNoteTexture->GetTextureNameLength() + 1);
        break;
    }
    case 3: {
        TESTopic* pTopic = GetNoteTopic();
        if (pTopic)
        {
            unsigned int uiTopicFormID = pTopic->GetFormID();
            SaveIntChunk(0x4d414e54, uiTopicFormID);
        }
        TESActorBase* m_pDialogueSpeaker = GetNoteSpeaker();
        if (m_pDialogueSpeaker)
        {
            unsigned int uiSpeakerFormID = m_pDialogueSpeaker->GetFormID();
            SaveIntChunk(0x4d414e53, uiSpeakerFormID);
        }
        break;
    }
    }
    TESForm::FinishFormRecord();
}

// Copies generic components, flags, speaker, type-specific data and owner quests from another BGSNote into this one.
// @fnv 0x005e8c00
// pdb: BGSNote::Copy | BGSNote.obj | retail, vtable slot, high
// pdb params: apCopy
// pdb locals: plist, pcopy
void BGSNote::CopyFrom(const TESForm* apCopy)
{
    BGSNote* pSource = dynamic_cast<BGSNote*>(const_cast<TESForm*>(apCopy));
    if (!pSource)
        return;

    TESForm::CopyFrom(apCopy);
    BSSimpleList<TESQuest*>* pIter = 0;
    SetNoteType((NOTE_TYPE)pSource->GetNoteType());
    SetNoteSpeaker(pSource->GetNoteSpeaker());
    switch (cType)
    {
    case 0:
        SetNoteSound(pSource->GetNoteSound());
        break;
    case 1:
        SetNoteText(pSource->GetNoteText());
        break;
    case 2:
        SetNoteImage(pSource->GetNoteImage());
        break;
    case 3:
        SetNoteTopic(pSource->GetNoteTopic());
        break;
    }

    OwnerQuests.RemoveAll();
    pIter = pSource->GetOwnerQuests();
    while (pIter && !pIter->IsEmpty())
    {
        OwnerQuests.AddHead(pIter->GetItem());
        pIter = pIter->GetNext();
    }
    bHasBeenRead = pSource->bHasBeenRead;
}

// Compares this note against apForm by base data, type-specific data and owner quest lists, returning true when they differ.
// @fnv 0x005e8d60
// pdb: BGSNote::Compare | BGSNote.obj | retail, upstream name already matches, high
// pdb params: apCompare
// pdb locals: pcomp, plist, pdesc, pcompdesc, ptexture, pcomptexture, pquest
bool BGSNote::Compare(TESForm* apCompare)
{
    BGSNote* pNote = dynamic_cast<BGSNote*>(apCompare);
    if (!pNote)
        return true;
    if (TESForm::Compare(pNote))
        return true;
    if (pNote->cType != cType)
        return true;

    switch (cType)
    {
    case 0:
        if (GetNoteSound() != pNote->GetNoteSound())
            return true;
        break;
    case 1: {
        TESDescription* pMine = GetNoteText();
        TESDescription* pTheirs = pNote->GetNoteText();
        if (pMine && pTheirs)
        {
            if (pMine->CompareWithBase((BaseFormComponent*)pTheirs))
                return true;
        }
        else if (pMine || pTheirs)
            return true;
        break;
    }
    case 2: {
        TESTexture* pMine = GetNoteImage();
        TESTexture* pTheirs = pNote->GetNoteImage();
        if (pMine && pTheirs)
        {
            if (pMine->CompareWithBase((BaseFormComponent*)pTheirs))
                return true;
        }
        else if (pMine || pTheirs)
            return true;
        break;
    }
    case 3:
        if (GetNoteTopic() != pNote->GetNoteTopic())
            return true;
        if (GetNoteSpeaker() != pNote->GetNoteSpeaker())
            return true;
        break;
    }

    if (GetOwnerQuests()->ItemsInList() != pNote->GetOwnerQuests()->ItemsInList())
        return true;

    BSSimpleList<TESQuest*>* pNode = GetOwnerQuests();
    while (pNode && !pNode->IsEmpty())
    {
        TESQuest* pQuest = pNode->GetItem();
        if (!pNote->GetOwnerQuests()->IsInList(pQuest))
            return true;
        pNode = pNode->GetNext();
    }
    return false;
}
