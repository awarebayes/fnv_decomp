// fallout_shared/packageeventaction.cpp: PackageEventActionData_packageeventaction (7 functions)

#include "../../include/BSStringT.h"

// PackageEventActionData_packageeventaction: local type (was include/PackageEventActionData.h, which had no evidence of one real class); its methods are 0x0067da80 PackageEventAction::~PackageEventAction, 0x0067dad0 PackageEventAction::Copy, 0x0067dbf0 PackageEventAction::Compare, 0x0067dc80 TESPackageOnBeginEndOrChange_Save
class TESIdleForm;
class Script;
class TESTopic;
class TESForm;
class TESFile;
enum PACK_EVENT_ACTION_TYPE;

// pdb: class PackageEventAction, size 0x10, 4 members
class PackageEventAction // src: XBOX_RETAIL_PDB 0x0067dad0 (first retail link of 5 high-tier links to its methods; old: PackageEventActionData_packageeventaction)
{
public:
    TESIdleForm* pIdle; // src: XBOX_RETAIL_PDB +0x0 (old: m_pIdleForm)
    Script* pResultScript; // src: XBOX_RETAIL_PDB +0x4 (old: m_pScript)
    TESTopic* pTopic;
    PACK_EVENT_ACTION_TYPE eType;

    PackageEventAction();
    void Clear();
    void Copy(PackageEventAction* apCopy);
    bool Compare(PackageEventAction* apCompare);
    void Save(int aeChunk);
    void* GetScript();
    void InitItem(TESForm* apOwner);
    void InitItem(int apOwner);
    void Load(TESFile* apFile);
};
#include "../../include/BSSimpleList.h"
#include "../../include/TESObjectREFR.h"

#include "../../include/TESObjectANIO.h"
#include "../../include/TESTopic.h"
#include "../../include/BGSCameraPath.h"
#include "../../include/Script.h"
#include "../../include/ScriptVariableInfo.h"
#include "../../include/ScriptReferencedObject.h"
#include "../../include/TESFile.h"
#include "../../include/MagicCaster.h"
#include "../../include/GO.h"
#include <malloc.h>
#include <string.h>

int Warning();
int Warning(const char*, ...);
unsigned int SetWarningState(bool);
void BGSMessageWarningAuxiliary(int);
TESForm* GetFormByID(unsigned int);
extern ScriptVariableInfo* DAT_011d66ac;

// @fnv 0x00680890
// pdb: TESCondition::TESCondition | TESCondition.obj | debug, call graph, medium
PackageEventAction::PackageEventAction()
{
    // The original initializes the two leading pointers through this list constructor.
    reinterpret_cast<BSSimpleList<TESForm*>*>(this)->BSSimpleList<TESForm*>::BSSimpleList();
}

// @fnv 0x0067e190
// pdb: PackageEventAction::InitItem | PackageEventAction.obj | retail, string-set bridge, high
// pdb params: apOwner
// pdb locals: pidle, ptopic, tempint, iwarnings, iprevwarnings, pscriptname
void PackageEventAction::InitItem(TESForm* apOwner)
{
    unsigned int id = (unsigned int)pIdle;
    TESForm::RemapFormIDForFile(id, apOwner->GetFile(-1));
    TESObjectANIO* idle = dynamic_cast<TESObjectANIO*>(GetFormByID(id));
    if (idle)
        pIdle = reinterpret_cast<TESIdleForm*>(idle);
    else if (id)
    {
        if (((BGSCameraPath*)apOwner)->HasNameForWarning())
            Warning("MASTERFILE: Unable to find Idle (%08X) on owner object \"%s\".", id, apOwner->GetName());
        else
            Warning("MASTERFILE: Unable to find Idle (%08X) on owner object (%08X).", id, apOwner->GetFormID());
        pIdle = 0;
    }
    id = (unsigned int)pTopic;
    TESForm::RemapFormIDForFile(id, apOwner->GetFile(-1));
    TESTopic* topic = dynamic_cast<TESTopic*>(GetFormByID(id));
    if (topic)
        pTopic = topic;
    else if (id)
    {
        if (((BGSCameraPath*)apOwner)->HasNameForWarning())
            Warning("MASTERFILE: Unable to find Topic (%08X) on owner object \"%s\".", id, apOwner->GetName());
        else
            Warning("MASTERFILE: Unable to find Topic (%08X) on owner object (%08X).", id, apOwner->GetFormID());
        pTopic = 0;
    }
    if (pResultScript)
    {
        int errors;
        int oldWarning = Warning();
        SetWarningState(true);
        BGSMessageWarningAuxiliary(0);
        ((Script*)pResultScript)->NotifySaveLoadGameOfFormChange();
        ((Script*)pResultScript)->InitItem(apOwner);
        ((Script*)pResultScript)->GetHeader()->m_bQuest = false;
        SetWarningState(false);
        errors = Warning();
        BGSMessageWarningAuxiliary(oldWarning);
        if (errors)
            Warning("MASTERFILE: Errors were encountered during InitItem for result script on eventaction for package '%s' (%08X)\n\nSee Warnings file for more information.",
                    apOwner->GetName(), apOwner->GetFormID());
    }
}

// Loads a Script from a TESFile: reads variable, condition, data, reference and item/topic chunks, then discards the script if it has no data.
// @fnv 0x0067dd20
// pdb: PackageEventAction::Load | PackageEventAction.obj | debug, call graph, high
// pdb params: apFile
// pdb locals: iid, bbreak, pMessage, tmpdata, n, pNewItem, tempID
void PackageEventAction::Load(TESFile* apFile)
{
    unsigned int id;
    pIdle = 0;
    pTopic = 0;
    if (pResultScript)
    {
        delete (Script*)pResultScript;
        pResultScript = 0;
    }
    pResultScript = new Script;
    for (;;)
    {
        bool done = false;
        switch ((int)apFile->GetChunkID())
        {
        case 0x44534c53:
        case 0x52564353:
            if (apFile->GetChunkID() == 0x44534c53)
            {
                DAT_011d66ac = new ScriptVariableInfo;
                DAT_011d66ac->Load(apFile);
                ((Script*)pResultScript)->GetVariableList()->AddTail(DAT_011d66ac);
            }
            else if (DAT_011d66ac)
            {
                char* name = (char*)_alloca(apFile->GetChunkSize());
                apFile->GetChunkData(name, 512);
                DAT_011d66ac->name.Set(name, 0);
            }
            break;
        case 0x52484353:
            apFile->GetChunkData(((Script*)pResultScript)->GetHeader(), 0);
            if (apFile->IsEndianConversionRequired())
                ((Script*)pResultScript)->GetHeader()->Endian();
            break;
        case 0x41444353: {
            unsigned int size = apFile->GetChunkSize();
            char* data = (char*)_alloca(size);
            memset(data, 0, size);
            apFile->GetChunkData(data, 0);
            ((Script*)pResultScript)->SetCompileData(size, data);
            break;
        }
        case 0x4f524353: {
            unsigned int formID;
            ScriptReferencedObject* object = new ScriptReferencedObject;
            apFile->GetChunkData(&formID);
            object->pForm = (TESForm*)formID;
            ((Script*)pResultScript)->GetReferencedObjectList()->AddTail(object);
            break;
        }
        case 0x56524353: {
            unsigned int formID;
            ScriptReferencedObject* object = new ScriptReferencedObject;
            apFile->GetChunkData(&formID);
            object->m_uiFormID = formID;
            ((Script*)pResultScript)->GetReferencedObjectList()->AddTail(object);
            break;
        }
        break;
        case 0x4d414e49:
            apFile->GetChunkData(&id);
            pIdle = (TESIdleForm*)id;
            break;
        case 0x4d414e54:
            apFile->GetChunkData(&id);
            pTopic = (TESTopic*)id;
            done = true;
            break;
        }
        if (done)
            break;
        apFile->NextChunk();
    }
    ((Script*)pResultScript)->SetInitializationState(false);
    if (!((Script*)pResultScript)->GetData())
    {
        delete (Script*)pResultScript;
        pResultScript = 0;
    }
}

// @fnv 0x0067da80
// pdb: PackageEventAction::~PackageEventAction | PackageEventAction.obj | debug, call graph, medium
void PackageEventAction::Clear()
{
    *(int*)this = 0;
    *((int*)this + 2) = 0;
    delete *((Script**)this + 1);
}

extern "C" void TESForm_AddChunk(int);
extern "C" void TESForm_AddChunk_00485910(int, int);
// These operations are not declared by the shared Script header yet.
// This adapter supplies only the missing methods, with no copied Script layout.
struct PackageEventScriptMethods
{
    void Script_SaveResultScript();
    bool Script_CompareResultScripts(void*);
};
// @fnv 0x0067dc80
// pdb: PackageEventAction::Save | PackageEventAction.obj | retail, object-file run extension, high
// pdb params: aeChunk
// pdb locals: iid
void PackageEventAction::Save(int aeChunk)
{
    TESForm_AddChunk(aeChunk);
    unsigned int iid = pIdle ? ((TESForm*)pIdle)->GetFormID() : 0;
    TESForm_AddChunk_00485910(0x4d414e49, iid);
    iid = pTopic ? pTopic->GetFormID() : 0;
    if (pResultScript)
        ((PackageEventScriptMethods*)pResultScript)->Script_SaveResultScript();
    TESForm_AddChunk_00485910(0x4d414e54, iid);
}

struct QuantityData
{
    void SetQuantity(unsigned int);
};


// Copies quantity, flags and form ID from apCopy, then clones or clears the script pointer depending on whether apCopy has one.
// @fnv 0x0067dad0
// src: XBOX_RETAIL_PDB 0x0067dad0
// pdb: PackageEventAction::Copy | PackageEventAction.obj | retail, object-file run extension, high
// pdb params: apCopy
void PackageEventAction::Copy(PackageEventAction* apCopy)
{
    reinterpret_cast<QuantityData*>(this)->SetQuantity(reinterpret_cast<const GO*>(apCopy)->operator unsigned int());

    reinterpret_cast<TESObjectREFR*>(this)->SetFlags(reinterpret_cast<TESObjectREFR*>(apCopy)->GetFlags());

    reinterpret_cast<MagicCaster*>(this)->SetSelectedMagicItem(reinterpret_cast<TESForm*>(apCopy)->GetFormID());

    if (apCopy->pResultScript)
    {
        if (!pResultScript)
        {
            pResultScript = new Script;
            ((Script*)pResultScript)->NotifySaveLoadGameOfFormChange();
        }
        ((Script*)pResultScript)->CopyResultScript((Script*)apCopy->pResultScript);
    }
    else
    {
        delete (Script*)pResultScript;
        pResultScript = 0;
    }
}

// @fnv 0x0067dbf0
// src: XBOX_RETAIL_PDB 0x0067dbf0
// pdb: PackageEventAction::Compare | PackageEventAction.obj | retail, call graph (shared callers), high
// pdb params: apCompare
bool PackageEventAction::Compare(PackageEventAction* apCompare)
{
    if ((int)pIdle != (int)reinterpret_cast<const GO*>(apCompare)->operator unsigned int())
        return true;
    if ((int)pTopic != reinterpret_cast<TESObjectREFR*>(apCompare)->GetFlags())
        return true;
    if ((void*)pResultScript || apCompare->GetScript())
    {
        if (!(void*)pResultScript || !apCompare->GetScript())
            return true;
        if (((PackageEventScriptMethods*)pResultScript)->Script_CompareResultScripts(apCompare->GetScript()))
            return true;
    }
    return false;
}
