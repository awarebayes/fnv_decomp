#include "../../include/BSStringT.h"
// fallout_shared/tesscriptcompiler.cpp: ScriptCompiler, TESForm, S7f, S5a and more (36 functions)

#include "tesscriptcompiler_packet_decls.h"
#include "../../include/TESForm.h"
#include "../../include/TESObjectREFR.h"
#include "../../include/SL.h"
#include "../../include/Script.h"
#include "../../include/StrMem.h"
#include "../../include/TESDataHandler.h"
#include "../../include/ScriptCompiler.h"
#include "../../include/BGSMessage.h"
#include <stddef.h>

// If DAT_011c40c8 is clear, sets it true, reads GetName(), resets it, and returns the name's length or 0 when there is no name.
// @fnv 0x00474cb0
// src: XBOX_PDB 0x00474cb0
// pdb: TESForm::GetFormEditorIDLength | BGSAmmoForm.obj | retail, upstream name already matches, high
int TESForm::GetFormEditorIDLength()
{
    if (DAT_011c40c8)
        return 0;
    DAT_011c40c8 = true;
    const char* name = GetName();
    DAT_011c40c8 = false;
    return name ? BSStringLength(name) : 0;
}

// @fnv 0x005b1120
ScriptFunctionInfo* GetFunctionByOpcode(SCRIPT_OUTPUT output)
{
    ScriptFunctionInfo* result = 0;
    if (output >= 0x100 && output < 0x1cd)
        result = &DAT_0118e8e0[output - 0x100];
    else if (output >= 0x1000 && output < 0x1280)
        result = (ScriptFunctionInfo*)&g_scriptCommands[output - 0x1000];
    return result;
}

struct ScriptVariableData;
struct CStringArgument
{
    bool GetVariable(ScriptVariableData*);
    ScriptReferencedObject* GetReferencedForm(unsigned int);
    bool GetReferencedForm(ScriptVariableData*, bool, bool);
    bool IsPartialScript();
    void* GetScriptLineList();
    void* GetReferencedObjectList();
    void* GetVariableList();
    void ClearAllScriptLines();
    void ClearAllReferenceObjects();
    void ClearAllVariables();
    void* __vecDelDtor(unsigned int);
    char* input;
    unsigned int m_uiInputOffset;
    int m_eCompilerIndex;
    char m_kScriptName[8];
    int m_eErrorCode;
    bool m_bIsPartialScript;
    char _pad_19[3];
    unsigned int m_uiLastLineNumber;
    char* pOutput;
    unsigned int m_uiOutputOffset;
    char header[20];
    char m_kVariableList[8];
    char m_kReferencedObjectList[8];
    Script* m_pCurrentScript;
    char m_kLineList[8];
};
typedef char _sizecheck_ScriptCompileData[1];
// @fnv 0x00613790
void* CStringArgument::GetScriptLineList()
{
    return &m_kLineList;
}

struct S7f
{
    char pad[0x2a];
    unsigned char v;
    void SetValue(unsigned char a);
};
// @fnv 0x007f7c90
// pdb: FORenderedMenu::GetParentNodeToRender | FOPipboyManager.obj | debug, object-file gap fill, medium
void S7f::SetValue(unsigned char aucValue)
{
    v = aucValue;
}

// Returns 1 if the string is a valid integer (optional leading '-', then digits only), otherwise 0.
// @fnv 0x004b1230
int IsIntegerString(const char* pcString)
{
    char c = *pcString;
    pcString++;
    if ((c >= '0' && c <= '9') || c == '-')
    {
        if (*pcString == 0 && c == '-')
            return 0;
        c = *pcString;
        pcString++;
        while (c != 0)
        {
            if (c < '0' || c > '9')
                return 0;
            c = *pcString;
            pcString++;
        }
        return 1;
    }
    return 0;
}

// Exe 0x005aea50: unit-local Interface view; declarations and layout are preserved.
struct TesscriptcompilerInterfaceView
{
    static void __cdecl PrintDebugMessage(const char*);
};
extern int DAT_011908f0;
extern int DAT_01196d18;
struct S5a
{
    S5a* ScriptCompiler_ctor();
};
// @fnv 0x005aea50
// src: XBOX_RETAIL_PDB 0x005aea50
// pdb: ScriptCompiler::ScriptCompiler | TESScriptCompiler.obj | retail, string-set bridge, high
S5a* S5a::ScriptCompiler_ctor()
{
    if (DAT_01196d18 != 0x1280)
        TesscriptcompilerInterfaceView::PrintDebugMessage("The script function list doesn't have all the names filled in.\r\n");
    if (DAT_011908f0 != 0x1cd)
        TesscriptcompilerInterfaceView::PrintDebugMessage("The script console-command list doesn't have all the names filled in.\r\n");
    return this;
}

extern "C" void* __cdecl memset(void*, int, unsigned int);

struct ScriptVariableData
{
    char m_auiShapeKeys[0x200];
    unsigned int m_uiVariableID;
    char m_cTypeCode;
    unsigned int m_uiReferenceID;
    unsigned int m_uiFormID;
    void* pForm;
    ScriptVariableData* SCRIPT_WORD_ctor();
};

// Clears the object's fields and zeroes its 0x200-byte buffer, then returns this.
// @fnv 0x005af310
// src: XBOX_RETAIL_PDB 0x005af310
// pdb: SCRIPT_WORD::SCRIPT_WORD | TESScriptCompiler.obj | retail, call graph, high
ScriptVariableData* ScriptVariableData::SCRIPT_WORD_ctor()
{
    m_uiVariableID = 0;
    m_uiFormID = 0;
    m_cTypeCode = 0;
    m_uiReferenceID = 0;
    pForm = 0;
    memset(this, 0, 0x200);
    return this;
}

void __cdecl FUN_00ec8d33(int a, int b, int c);

// @fnv 0x005afe10
// src: XBOX_PDB 0x005afe10
// pdb: BSstrtoul | TESScriptCompiler.obj | debug, upstream name already matches, high
// pdb params: apString, apEndPtr, aiBase
void __cdecl BSstrtoul(int apString, int apEndPtr, int aiBase)
{
    FUN_00ec8d33(apString, apEndPtr, aiBase);
}

// Advances *apOffset past tab, space and comma characters in apExpLine, stopping at the first other character.
// @fnv 0x005b1b60
// src: XBOX_PDB 0x005b1b60
void __cdecl ScriptCompiler_SkipWhiteSpace(const char* apExpLine, int* apOffset)
{
    for (;;)
    {
        switch (apExpLine[*apOffset])
        {
        case 9:
        case 0x20:
        case 0x2c:
            ++*apOffset;
            break;
        default:
            return;
        }
    }
}

int __cdecl ScriptCompiler_MessageBoxCompile(unsigned short asParamCount, void* apParameters, void* apScriptLine, void* arg4, int abIsMessageBox, int abStringOnly);

// Calls FUN_005b3cd0 with the first four arguments and zero for the last two.
// @fnv 0x005b3c70
// src: EXE 0x005b3c70
// pdb: ScriptCompiler::MessageCompile | TESScriptCompiler.obj | debug, object-file gap fill, medium
// pdb params: asParamCount, apParameters, apScriptLine, apCompileData
int __cdecl Cmd_ShowMessage_Parse(unsigned short arg1, void* arg2, void* arg3, void* arg4)
{
    return ScriptCompiler_MessageBoxCompile(arg1, arg2, arg3, arg4, 0, 0);
}

int __cdecl ScriptCompiler_MessageBoxCompile(unsigned short, void*, void*, void*, int, int);

// @fnv 0x005b3c40
// src: EXE 0x005b3c40
// pdb: ScriptCompiler::MessageBoxCompile | TESScriptCompiler.obj | debug, object-file gap fill, medium
// pdb params: asParamCount, apParameters, apScriptLine, apCompileData
int __cdecl Cmd_EssentialDeathReload_Parse(unsigned short param1, void* param2, void* param3, void* param4)
{
    return ScriptCompiler_MessageBoxCompile(param1, param2, param3, param4, 1, 0);
}

int __cdecl ScriptCompiler_MessageBoxCompile(unsigned short, void*, void*, void*, int, int);

// Calls FUN_005b3cd0 with the given arguments plus 0 and 1, returning its result.
// @fnv 0x005b3ca0
// src: EXE 0x005b3ca0
// pdb: ScriptCompiler::OldMessageCompile | TESScriptCompiler.obj | debug, object-file gap fill, medium
// pdb params: asParamCount, apParameters, apScriptLine, apCompileData
int __cdecl Cmd_ShowWarning_Parse(unsigned short arg1, void* arg2, void* arg3, void* arg4)
{
    return ScriptCompiler_MessageBoxCompile(arg1, arg2, arg3, arg4, 0, 1);
}

void __cdecl _Deallocate(void*);
struct List
{
    bool _ref_008256d0();
    void*& _ref_006815c0();
    void _ref_0063f7b0();
};
// @fnv 0x005ae670
// pdb: ScriptCompileData::ClearAllScriptLines | TESScriptCompiler.obj | debug, call graph, medium
// pdb locals: pList
void TESDataHandler::ClearHairList()
{
    List* pList = (List*)GetTESHairList();
    while (pList && !pList->_ref_008256d0())
    {
        void* item = pList->_ref_006815c0();
        _Deallocate(item);
        pList->_ref_0063f7b0();
    }
}

void* __cdecl memset(void* dst, int c, unsigned int n);

struct FUN_005af370_shell
{
    unsigned int m_uiValueBeforeData;
    char m_data[0x200];
    unsigned int uiSize;
    unsigned int uiElapsedMilliseconds;
    char m_auiShapeKeys[0x40c - 0x20c];
    unsigned int m_uiValueAfterSecondBuffer;
    char m_pAuxiliaryObject[8];
    unsigned int m_uiTrailingValue;
    FUN_005af370_shell();
};

// @fnv 0x005af370
// pdb: SCRIPT_LINE::SCRIPT_LINE | TESScriptCompiler.obj | retail, call graph, high
FUN_005af370_shell::FUN_005af370_shell()
{
    m_uiValueBeforeData = 0;
    uiSize = 0;
    uiElapsedMilliseconds = 0;
    m_uiValueAfterSecondBuffer = 0;
    m_uiTrailingValue = 0;
    memset(m_data, 0, 0x200);
    memset((char*)this + 0x20c, 0, 0x200);
}

namespace fnv_005afeb0 {
struct TaskQueueInterface
{
    static void __cdecl CopyStringToBuffer(char*, unsigned int, const char*);
};
struct ScriptVariableData
{
    char pText[512];
    char pad[24];
    ScriptVariableData* SCRIPT_WORD_ctor();
};
bool __cdecl ScriptCompiler_GetFunction(char*);
// @fnv 0x005afeb0
// src: COMMUNITY 0x005afeb0
// pdb: ScriptCompiler::IsValidVariableName | TESScriptCompiler.obj | retail, upstream name already matches, high
// pdb params: apVarName
// pdb locals: checkword, pform
bool __cdecl ScriptCompiler_IsValidVariableName(const char* apVarName)
{
    if (!apVarName || !apVarName[0])
        return false;
    TESForm* pform = TESForm::LookupFormByEditorID(apVarName);
    if (pform)
        return false;
    ScriptVariableData checkword;
    checkword.SCRIPT_WORD_ctor();
    TaskQueueInterface::CopyStringToBuffer(checkword.pText, 512, apVarName);
    if (ScriptCompiler_GetFunction(checkword.pText))
        return false;
    return true;
}
}  // namespace fnv_005afeb0

#include "../../include/COMPILER_NAME.h"

namespace fnv_005aedb0 {

class Str6a70
{
  public:
    char* pString;
    unsigned short usLen, usMax;
    Str6a70& operator=(const char*);
};
void* __memcpy(void*, const void*, unsigned int);
extern const char* DAT_0118ca7c[];

struct FUN_005ae490;
struct FUN_005ae5c0;
template <class A, class B> class ScriptCompileDataT
{
  public:
    char* pText;
    unsigned int uiElapsedMilliseconds;
    COMPILER_NAME m_eCompilerIndex;
    Str6a70 m_pName;
    unsigned int unk14;
    bool bPartial;
    unsigned int unk1C[3];
    unsigned int kHeader[5];
    unsigned int unk3C[2];
    unsigned int kRefObjects[2];
    Script* pScript;
    unsigned int unk50;
    ScriptCompileDataT();
    ~ScriptCompileDataT();
};
typedef ScriptCompileDataT<FUN_005ae490, FUN_005ae5c0> CStringArgument;

}  // namespace fnv_005aedb0

// Builds compile data from the script and optional owner object, sets the compiler name, then calls FUN_005aeb90 to compile the script.
// @fnv 0x005aedb0
// src: XBOX_PDB 0x005aedb0
// pdb: ScriptCompiler::CompilePartialScript | TESScriptCompiler.obj | retail, upstream name already matches, high
// pdb params: apScript, apOwnerObject, index, abOnLoad
// pdb locals: bret, compileData
bool ScriptCompiler::CompilePartialScript(Script* apScript, TESObjectREFR* apOwnerObject, COMPILER_NAME index,
                                  bool abOnLoad)
{
    bool bResult;
    fnv_005aedb0::CStringArgument compileData;
    compileData.m_eCompilerIndex = index;
    compileData.bPartial = true;
    if (apOwnerObject && apOwnerObject->TESObjectREFR_GetScriptPointer())
    {
        compileData.pScript = apOwnerObject->TESObjectREFR_GetScriptPointer();
        fnv_005aedb0::__memcpy(compileData.kHeader, compileData.pScript->GetHeader(), 0x14);
        Script::CopyScriptReferences(
            compileData.pScript->GetReferencedObjectList(),
            (BSSimpleList<ScriptReferencedObject*>*)compileData.kRefObjects, 0);
    }
    else
    {
        compileData.pScript = apScript;
    }
    if (index == 1 || !((TESForm*)compileData.pScript)->GetFormEditorIDLength())
        compileData.m_pName = fnv_005aedb0::DAT_0118ca7c[compileData.m_eCompilerIndex];
    else
        ((::BSStringT<char>*)&compileData.m_pName)->Set(((TESForm*)compileData.pScript)->GetName(), 0);
    compileData.pText = apScript->GetText();
    bResult = Compile_005aeb90(apScript, (CStringArgument*)&compileData);
    return bResult;
}

void* __cdecl memset(void*, int, unsigned int);
char* __cdecl ourCalloc(int, int);

extern char $SG3295[];
extern int _data_011c16bc;

struct FUN_005ae490
{
    unsigned char _padding0[4];
    int _field04;
    int _field08;
    StrMem _field0c;
    int _field14;
    unsigned char _field18;
    int _field1c;
    char* _field20;
    int _field24;
    unsigned char _field28[0x14];
    SL _field3c;
    SL _field44;
    int _field4c;
    SL _field50;

    FUN_005ae490();
};

// @fnv 0x005ae490
// pdb: ScriptCompileData::ScriptCompileData | TESScriptCompiler.obj | debug, call graph, high
FUN_005ae490::FUN_005ae490()
{
    _field1c = 0;
    _field04 = 0;
    _field24 = 0;

    _field20 = ourCalloc(1, 0x4000);
    memset(_field20, 0, 4);
    memset(_field28, 0, 0x14);
    memset(_field20, 0, 4);

    _field08 = 0;
    _field18 = 0;
    _field4c = 0;

    _field0c.Set($SG3295, 0);

    _field14 = 0;
}

namespace fnv_005ae5c0 {


struct ListX
{
    unsigned char padding[8];
    ~ListX();
};

void __cdecl _Deallocate(void* ptr);
void __fastcall ScriptCompileData_ClearAllReferenceObjects(void* ptr);
void __fastcall ScriptCompileData_ClearAllVariables(void* ptr);

struct FUN_005ae5c0
{
    unsigned char padding00[0x0c];
    ::BSStringT<char> _field0c;
    unsigned char stringTailPadding[0x0c];
    char* pOutput;
    unsigned char padding[0x18];
    ListX list1;
    ListX list2;
    unsigned int value;
    ListX list3;

    ~FUN_005ae5c0();
};

// Runs TESDataHandler cleanup, calls two helper teardown routines on this, then deallocates pOutput.
// @fnv 0x005ae5c0
// pdb: ScriptCompileData::~ScriptCompileData | TESScriptCompiler.obj | retail, call graph, high
FUN_005ae5c0::~FUN_005ae5c0()
{
    ((TESDataHandler*)this)->ClearHairList();
    ScriptCompileData_ClearAllReferenceObjects(this);
    ScriptCompileData_ClearAllVariables(this);
    _Deallocate(pOutput);
}
}  // namespace fnv_005ae5c0

extern "C" int __cdecl isalnum(int);
extern "C" void* __cdecl memset(void*, int, unsigned int);

struct ScriptLineBuffer
{
    unsigned int m_uiValueBeforeData;
    char cLine[0x200];
    unsigned int uiSize;
    unsigned int uiElapsedMilliseconds;
    char cOutput[0x200];
    unsigned int m_uiValueAfterSecondBuffer;
    int unk410;
    int unk414;
    unsigned int m_uiTrailingValue;
};

struct ScriptSource
{
    char* pInput;
    unsigned int m_uiInputOffset;
};

void __cdecl ScriptCompileError(ScriptSource* apData, const char* apFormat, ...);

// Reads one line from the script source into the line buffer, skipping comments and leading whitespace and trimming trailing spaces and tabs.
// @fnv 0x005af3f0
// src: XBOX_RETAIL_PDB 0x005af3f0
// pdb: ScriptCompiler::GetNextLine | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData, pScriptLine
// pdb locals: ch, blankLine, offset, input
int __cdecl ScriptCompiler_GetNextLine(ScriptSource* apCompileData, ScriptLineBuffer* pScriptLine)
{
    char* pText5 = apCompileData->pInput + apCompileData->m_uiInputOffset;
    int _iOffset = 0;
    bool bSkip = true;
    unsigned char cChar;
    for (;;)
    {
        cChar = pText5[_iOffset++];
        switch ((char)cChar)
        {
        case ';':
            while (cChar && cChar != '\n')
            {
                cChar = pText5[_iOffset++];
            }
        case '\r':
            if (cChar && pText5[_iOffset] == '\n')
            {
                _iOffset++;
            }
        case 0:
            if (!bSkip)
            {
                while (pScriptLine->uiSize &&
                       (pScriptLine->cLine[pScriptLine->uiSize - 1] == ' ' || pScriptLine->cLine[pScriptLine->uiSize - 1] == '\t'))
                {
                    pScriptLine->uiSize--;
                    pScriptLine->cLine[pScriptLine->uiSize] = 0;
                }
                if (pScriptLine->uiSize)
                {
                    pScriptLine->cLine[pScriptLine->uiSize] = 0;
                    return _iOffset;
                }
            }
            else if (!cChar)
            {
                return 0;
            }
            pScriptLine->m_uiValueBeforeData++;
            pScriptLine->uiSize = 0;
            memset(pScriptLine->cLine, 0, 0x200);
            break;
        default:
            if (pScriptLine->uiSize + 1 >= 0x200)
            {
                ScriptCompileError(apCompileData, "Max script line length (%d characters) exceeded.", 0x200);
                pScriptLine->m_uiTrailingValue = 0x10;
                return 0;
            }
            if (isalnum(cChar) || cChar == '"')
            {
                bSkip = false;
            }
            if (!bSkip)
            {
                pScriptLine->cLine[pScriptLine->uiSize] = cChar;
                pScriptLine->uiSize++;
            }
            break;
        }
    }
}

struct Node_5afe30
{
    void** _ref_006815c0();
    Node_5afe30* _ref_00726070();
    bool _ref_008256d0();
};

struct Cls_5afe30
{
    int unk00;
    int unk04;
    int unk08;
    int unk0c;
    int unk10;
    int unk14;
    int unk18;
    int unk1c;
    int unk20;
    int unk24;
    int unk28;
    unsigned int count2c;
    Node_5afe30* BSFile_GetDevice();
    void* GetNodeValue(unsigned int idx);
};

// Returns the value stored in the idx-th node of the list, or null if idx is 0 or exceeds count2c.
// @fnv 0x005afe30
// pdb: ScriptCompileData::GetReferencedObject | TESScriptCompiler.obj | debug, object-file gap fill, medium
// pdb params: aiIndex
// pdb locals: aiCount, pList
void* Cls_5afe30::GetNodeValue(unsigned int uiIndex)
{
    if (uiIndex == 0)
        return 0;
    if (uiIndex > count2c)
        return 0;
    unsigned int uiPos = 1;
    Node_5afe30* p = BSFile_GetDevice();
    while (p && !p->_ref_008256d0() && uiPos < uiIndex)
    {
        ++uiPos;
        p = p->_ref_00726070();
    }
    return *p->_ref_006815c0();
}

struct ScriptCommandView
{
    const char* longName;
    const char* shortName;
    unsigned int opcode;
    const char* helpText;
    unsigned int m_uiParameterCount;
    void* parameters;
    void* execute;
    void* parse;
    void* evaluate;
    void* reserved;
};
struct ScriptWordView
{
    char text[0x200];
    unsigned int referencedObjectIndex;
    char variableType;
    unsigned int functionCode;
};
extern ScriptCommandView DAT_0118cb50[];
int __cdecl BSstricmp(const char*, const char*);

// Matches word->text against the long or short names of three command tables and, on a hit, stores 'X' in variableType and the command opcode in functionCode.
// @fnv 0x005b1910
// src: XBOX_RETAIL_PDB 0x005b1910
// pdb: ScriptCompiler::GetFunction | TESScriptCompiler.obj | retail, call graph, high
// pdb params: apWord
// pdb locals: n
bool __cdecl ScriptCompiler_GetFunction(ScriptWordView* apWord)
{
    int index;
    for (index = 0; index < 16; ++index)
    {
        if (BSstricmp(DAT_0118cb50[index].longName, apWord->text) == 0 ||
            BSstricmp(DAT_0118cb50[index].shortName, apWord->text) == 0)
        {
            apWord->variableType = 'X';
            apWord->functionCode = DAT_0118cb50[index].opcode;
            return true;
        }
    }
    for (index = 0; index < 640; ++index)
    {
        if (BSstricmp(apWord->text, reinterpret_cast<ScriptCommandView*>(g_scriptCommands)[index].longName) == 0 ||
            BSstricmp(apWord->text, reinterpret_cast<ScriptCommandView*>(g_scriptCommands)[index].shortName) == 0)
        {
            apWord->variableType = 'X';
            apWord->functionCode = reinterpret_cast<ScriptCommandView*>(g_scriptCommands)[index].opcode;
            return true;
        }
    }
    for (index = 0; index < 205; ++index)
    {
        if (BSstricmp(apWord->text, reinterpret_cast<ScriptCommandView*>(DAT_0118e8e0)[index].longName) == 0 ||
            BSstricmp(apWord->text, reinterpret_cast<ScriptCommandView*>(DAT_0118e8e0)[index].shortName) == 0)
        {
            apWord->variableType = 'X';
            apWord->functionCode = reinterpret_cast<ScriptCommandView*>(DAT_0118e8e0)[index].opcode;
            return true;
        }
    }
    return false;
}

// Scans text from start to the null terminator, counting '(' and ')', and returns whether the final depth is zero.
// @fnv 0x005b1a90
// src: COMMUNITY 0x005b1a90
bool __cdecl ScriptCompiler_CheckParentheses(const char* text, unsigned int start)
{
    int depth_ = 0;
    unsigned int i = start;
    while (text[i] != 0)
    {
        switch (text[i])
        {
        case '(':
            depth_++;
            break;
        case ')':
            depth_--;
            break;
        }
        i++;
    }
    if (depth_ != 0)
    {
        return false;
    }
    return true;
}

// Counts double-quote characters in str from offset onward and returns true only if there are at least two and the count is even.
// @fnv 0x005b1b00
bool __cdecl HasBalancedQuotes(const char* pcString, int offset)
{
    int quoteCount = 0;
    int i = offset;
    while (pcString[i])
    {
        switch (pcString[i])
        {
        case '"':
            quoteCount++;
            break;
        }
        i++;
    }
    if (quoteCount < 2 || (quoteCount & 1))
        return false;
    return true;
}

struct FUN_005ae730_shell
{
    List* ComputeValue_005a8080();
    void ScriptCompileData_ClearAllVariables();
};
struct HairObject
{
    void* DestroyAndFree(unsigned int);
};
// Walks the list from FUN_005a8080 and, while the list is non-null and its check fails, calls DestroyAndFree(1) on each item and advances with FUN_0063f7b0.
// @fnv 0x005ae730
// src: COMMUNITY 0x005ae730
// pdb: ScriptCompileData::ClearAllVariables | TESScriptCompiler.obj | retail, upstream name already matches, high
// pdb locals: pList
void FUN_005ae730_shell::ScriptCompileData_ClearAllVariables()
{
    List* list = ComputeValue_005a8080();
    while (list && !list->_ref_008256d0())
    {
        HairObject* object;
        void* item = list->_ref_006815c0();
        object = static_cast<HairObject*>(item);
        object ? object->DestroyAndFree(1) : 0;
        list->_ref_0063f7b0();
    }
}

struct ScriptRefHolder;
struct Target;
struct TesscriptcompilerListNodeView;
struct Item;

extern char DAT_01183028[];
extern char DAT_011841cc[];

struct Mgr
{
    void* TESDataHandler_GetGlobal(ScriptRefHolder* apGlobalID);
};
extern Mgr* DAT_011c3f2c;

int __cdecl BSstrlen(ScriptRefHolder* p);
void* __cdecl FUN_00ec43fb(void* obj, int vfdelta, void* src, void* dst, int isRef);
int __cdecl FUN_00ec7ec0(char* s, ScriptRefHolder* p, int idx);
int __cdecl FUN_00ec89e9(int c);
Target* __cdecl TESScriptableForm_GetFormScript(TESForm* f);



struct Target
{
    bool FUN_005ac6a0(ScriptRefHolder* varName, int* auiID);
};

// Exe 0x005ae7a0: unit-local ListNode view; declarations and layout are preserved.
struct TesscriptcompilerListNodeView
{
    void*& Slot();
    TesscriptcompilerListNodeView* Next();
};

struct Buf
{
    char* GetBuf();
};

struct Item
{
    int value;
    char unk04[0x0c];
    unsigned char flag;
    char unk11[7];
    Buf m_18;
};

struct ScriptRefHolder
{
    char unk00[0x200];
    int m_200;
    signed char m_204;
    char unk205[7];
    int m_20c;
    TESForm* m_210;
};

struct CompilerCtx
{
    char unk00[0x4c];
    Target* m_4c;

    TesscriptcompilerListNodeView* ComputeValue_005a8080();
    bool ScriptCompileData_GetReferencedObject(ScriptRefHolder* apCurrentWord, int abAddRefVariable, int abAllowUnknownRef);
    bool ScriptCompileData_GetVariable(ScriptRefHolder* apCurrentWord);
};

// Resolves p's form to a Target and fills m_204/m_20c, falling back to scanning list items and m_4c, returning whether a match was found.
// @fnv 0x005ae7a0
// src: XBOX_RETAIL_PDB 0x005ae7a0
// pdb: ScriptCompileData::GetVariable | TESScriptCompiler.obj | retail, object-file run extension, high
// pdb params: apCurrentWord
// pdb locals: size, pList, pScript, pRef, pVar, nextchar
bool CompilerCtx::ScriptCompileData_GetVariable(ScriptRefHolder* apCurrentWord)
{
    TesscriptcompilerListNodeView* cur;
    int idx;
    TESObjectREFR* r2;
    Target* t5;
    Item* item;
    char cA;

    idx = BSstrlen(apCurrentWord);
    if (apCurrentWord->m_210 != 0)
    {
        t5 = 0;
        r2 = (TESObjectREFR*)FUN_00ec43fb(apCurrentWord->m_210, 0, DAT_01183028, DAT_011841cc, 0);
        if (r2 != 0)
        {
            r2->TESObjectREFR_InitScript();
            t5 = (Target*)r2->TESObjectREFR_GetScriptPointer();
        }
        else
        {
            if (apCurrentWord->m_210 != 0)
            {
                if (apCurrentWord->m_210->FUN_00401170() == 0x47)
                {
                    t5 = TESScriptableForm_GetFormScript(apCurrentWord->m_210);
                }
            }
        }
        if (t5 == 0)
        {
            apCurrentWord->m_20c = 0;
            return false;
        }
        apCurrentWord->m_204 = t5->FUN_005ac6a0(apCurrentWord, &apCurrentWord->m_20c);
        if (apCurrentWord->m_204 == 0)
        {
            apCurrentWord->m_20c = 0;
            return false;
        }
        return true;
    }
    else
    {
        if (apCurrentWord->m_200 != 0)
        {
            apCurrentWord->m_20c = 0;
            apCurrentWord->m_204 = 0;
            return false;
        }
    }

    cur = ComputeValue_005a8080();
    apCurrentWord->m_20c = 0;
    while (cur != 0 && cur->Slot() != 0)
    {
        item = (Item*)cur->Slot();
        if (FUN_00ec7ec0(item->m_18.GetBuf(), apCurrentWord, idx) == 0)
        {
            cA = item->m_18.GetBuf()[idx];
            if (FUN_00ec89e9(cA) == 0 && cA != '-' && cA != '_')
            {
                if (item->flag != 0)
                {
                    apCurrentWord->m_204 = 's';
                }
                else
                {
                    apCurrentWord->m_204 = 'f';
                }
                apCurrentWord->m_20c = item->value;
                return true;
            }
        }
        cur = cur->Next();
    }

    if (m_4c != 0)
    {
        apCurrentWord->m_204 = m_4c->FUN_005ac6a0(apCurrentWord, &apCurrentWord->m_20c);
        if (apCurrentWord->m_204)
        {
            return true;
        }
    }

    if (DAT_011c3f2c->TESDataHandler_GetGlobal(apCurrentWord) != 0)
    {
        if (ScriptCompileData_GetReferencedObject(apCurrentWord, 0, 0))
        {
            apCurrentWord->m_20c = 0;
            apCurrentWord->m_204 = 'G';
            return true;
        }
    }
    apCurrentWord->m_20c = 0;
    apCurrentWord->m_204 = 0;
    return false;
}

struct StrBuf8
{
    StrBuf8();
    ~StrBuf8();
    char* GetText();
    int _field00;
    int _field04;
};

struct ScriptNameRef
{
    char* GetName();
};

void __cdecl BSvsprintf(char* buf, const char* fmt, void* args);
void __cdecl BSStringT_char_SPrintF(StrBuf8* out, const char* fmt, char* name, unsigned int line, char* msg);
int __cdecl ConvertFromUni(char* a, int b);
void __cdecl Interface_PrintLine(int a);
void __cdecl Warning_005b5e40(int a);

// @fnv 0x005aea90
// pdb: ScriptCompiler::ScriptError | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData, text, buffer
// pdb locals: cErrorMessage, varArgs
void __cdecl ReportScriptError(CStringArgument* self, const char* pcFormat, ...)
{
    void* args = (char*)&pcFormat + sizeof(pcFormat);
    char msg[0x108];
    BSvsprintf(msg, pcFormat, args);
    args = 0;
    StrBuf8 strBuf;
    BSStringT_char_SPrintF(&strBuf, "SCRIPTS: Script '%s', line %d:\n%s", ((ScriptNameRef*)&self->m_kScriptName)->GetName(),
                 self->m_uiLastLineNumber, msg);
    if (self->m_eCompilerIndex == 1)
    {
        Interface_PrintLine(ConvertFromUni(strBuf.GetText(), 0));
    }
    else
    {
        Warning_005b5e40(ConvertFromUni(strBuf.GetText(), 0));
    }
}

static const char scriptNameError[] = "You need to start scripts with \"scriptname ScriptName\"";

struct ScriptHeaderView
{
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int variableCount;
    unsigned char scriptType;
    unsigned char flags;
};
// @fnv 0x005aeb90
// src: XBOX_PDB 0x005aeb90
// pdb: ScriptCompiler::Compile | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apScript, apCompileData
// pdb locals: pCurrentLine, pList, eExp, pLastList, pFirstLine
bool ScriptCompiler::Compile_005aeb90(Script* apScript, CStringArgument* apCompileData)
{
    if (!apScript || !apCompileData)
        return false;
    ((S7f*)apScript)->SetValue(0);
    Node_5afe30* buffer = 0;
    Node_5afe30* previous = 0;
    unsigned int opcode = 0xffff;
    ((ScriptHeaderView*)apCompileData->header)->scriptType = apScript->FUN_00500940()->scriptType;
    ((ScriptHeaderView*)apCompileData->header)->flags = apScript->FUN_00500940()->flags;
    ((ScriptHeaderView*)apCompileData->header)->variableCount = apScript->FUN_00500940()->variableCount;
    if (apCompileData->input)
    {
        do
        {
            opcode = GetNextExpression(apCompileData);
        } while (opcode != 0xffff);
        if (apCompileData->m_eErrorCode > 0)
            return false;
        apCompileData->m_uiLastLineNumber = 0;
        if (!apCompileData->m_bIsPartialScript)
        {
            buffer = (Node_5afe30*)apCompileData->GetScriptLineList();
            if (!buffer || (buffer && buffer->_ref_008256d0()))
            {
                ScriptCompileError((ScriptSource*)apCompileData, scriptNameError);
                return false;
            }
            ScriptLineBuffer* first = (ScriptLineBuffer*)*buffer->_ref_006815c0();
            opcode = first->unk410;
            if (opcode == 0xffff)
            {
                return false;
            }
            else if (opcode != 0x1d)
            {
                ScriptCompileError((ScriptSource*)apCompileData, scriptNameError);
                return false;
            }
            if (!VerifyBlocks(apCompileData))
                return false;
        }
    }
    buffer = (Node_5afe30*)apCompileData->GetScriptLineList();
    ScriptLineBuffer* currentLine = 0;
    while (buffer && !buffer->_ref_008256d0())
    {
        currentLine = (ScriptLineBuffer*)*buffer->_ref_006815c0();
        opcode = currentLine->unk410;
        apCompileData->m_uiLastLineNumber = currentLine->m_uiValueBeforeData;
        if (!ScriptCompiler_CompileLine(apScript, apCompileData, currentLine))
            return false;
        previous = buffer;
        buffer = buffer->_ref_00726070();
    }
    ((ScriptHeaderView*)apCompileData->header)->unk08 = apCompileData->m_uiOutputOffset;
    if (!apCompileData->IsPartialScript())
        return false;
    apScript->InitFromCompileData(apCompileData);
    ((S7f*)apScript)->SetValue(1);
    return true;
}

struct Word95
{
    char text[512];
    unsigned int reference;
    char kind;
    int code;
    int variable;
    void* form;
    Word95();
};
struct Line95
{
    unsigned int number;
    char input[512];
    unsigned int size;
    unsigned int cursor;
    char output[512];
    unsigned int outputSize;
    int code;
    unsigned int reference;
    int error;
    Line95();
};
struct Queue95
{
    void FUN_00905820(Line95**);
};
struct Name95
{
    int value;
    int length;
    void FUN_004037f0(const char*, int);
};
unsigned int __cdecl FUN_00ec6130(const char*);
int __cdecl ScriptCompiler_GetNextLine(CStringArgument*, Line95*);
int __cdecl ScriptCompiler_GetNextWord(CStringArgument*, Word95*, const char*, unsigned int*, int, int);
void __cdecl ReportScriptError(CStringArgument*, const char*, ...);
void __cdecl FUN_00406d30(char*, unsigned int, const char*);
// @fnv 0x005aeed0
// src: XBOX_RETAIL_PDB 0x005aeed0
// pdb: ScriptCompiler::GetNextExpression | TESScriptCompiler.obj | retail, unique string literal, high
// pdb params: apCompileData
// pdb locals: pNewLine, iOffset, bFound, currentWordSize, currentWord, uiOffset, scriptWord
unsigned int ScriptCompiler::GetNextExpression(CStringArgument* apCompileData)
{
    if (apCompileData->m_uiInputOffset >= FUN_00ec6130(apCompileData->input))
        return 65535;
    Line95* line8 = new Line95;
    line8->number = apCompileData->m_uiLastLineNumber + 1;
    int consumed38 = ScriptCompiler_GetNextLine(apCompileData, line8);
    if (!consumed38)
    {
        apCompileData->m_eErrorCode = line8->error;
        delete line8;
        return 65535;
    }
    apCompileData->m_uiInputOffset += consumed38;
    Word95 word26;
    int parsed2 = ScriptCompiler_GetNextWord(apCompileData, &word26, line8->input, &line8->cursor, 1, 1);
    if (!parsed2)
    {
        ReportScriptError(apCompileData, "Syntax Error\r\n%s\r\nCould not parse this line.", line8->input);
        delete line8;
        return 65535;
    }
    line8->reference = word26.reference;
    bool valid46 = word26.kind == 'X';
    if (valid46)
    {
        if (word26.code >= 256 && word26.code < 461 && apCompileData->m_eCompilerIndex != 1)
        {
            valid46 = false;
            ReportScriptError(apCompileData, "Script command \"%s\" is a console-only command.", word26.text);
        }
    }
    else
    {
        ++apCompileData->m_uiLastLineNumber;
        ReportScriptError(apCompileData, "Script command \"%s\" not found.", word26.text);
        apCompileData->m_eErrorCode = 13;
    }
    if (valid46)
    {
        line8->code = word26.code;
        FUN_00406d30(line8->input, 512, line8->input + line8->cursor);
        line8->size -= line8->cursor;
        apCompileData->m_uiLastLineNumber = line8->number;
        if (line8->code == 29)
        {
            unsigned int cursor33 = 0;
            Word95 name21;
            if (ScriptCompiler_GetNextWord(apCompileData, &name21, line8->input, &cursor33, 0, 0))
                ((Name95*)&apCompileData->m_kScriptName)->FUN_004037f0(name21.text, 0);
            else
            {
                ReportScriptError(apCompileData, "Syntax Error.  Missing script name.");
                apCompileData->m_eErrorCode = 1;
                delete line8;
                return 65535;
            }
        }
        if ((line8->code == 22 || line8->code == 24) && !line8->input[0])
        {
            ReportScriptError(apCompileData, "Syntax Error.  No condition specified for 'if' or 'elseif'.");
            apCompileData->m_eErrorCode = 6;
            delete line8;
            return 65535;
        }
        ((Queue95*)apCompileData->GetScriptLineList())->FUN_00905820(&line8);
        return line8->code;
    }
    else
    {
        delete line8;
        return 65535;
    }
}

struct LineListNode
{
    bool IsEmpty();
    ScriptLineBuffer*& Item();
    LineListNode* Next();
};

void __cdecl ReportCompileError(CStringArgument* apData, const char* apFormat, ...);

struct ScriptBlockScanner
{
    int ScriptCompiler_CountIfLines(CStringArgument* apCompileData, unsigned int* apStartingLine);
};

// Counts lines until the if/else/endif block starting after *apStartLine closes.
// @fnv 0x005b13e0
// src: XBOX_RETAIL_PDB 0x005b13e0
// pdb: ScriptCompiler::CountIfLines | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData, apStartingLine
// pdb locals: done, pCurrentLine, pList, exp, nested, count
int ScriptBlockScanner::ScriptCompiler_CountIfLines(CStringArgument* apCompileData, unsigned int* apStartingLine)
{
    int iCount8 = 0;
    int iDepthA = 0;
    int eType4;
    LineListNode* pNodeX;
    ScriptLineBuffer* pLine;
    int bFound = 0;

    pNodeX = (LineListNode*)apCompileData->GetScriptLineList();
    while (pNodeX && !pNodeX->IsEmpty() && !bFound)
    {
        pLine = pNodeX->Item();
        if (pLine->m_uiValueBeforeData > *apStartingLine)
        {
            eType4 = pLine->unk410;
            switch (eType4)
            {
            case 0xffff:
                ReportCompileError(apCompileData, "Mismatched if/else/endif.");
                return -1;
            case 0x12:
            case 0x13:
            case 0x14:
                break;
            case 0x16:
                ++iCount8;
                ++iDepthA;
                if (iDepthA >= 10)
                {
                    ReportCompileError(apCompileData, "Max nesting of %d exceeded.", 10);
                    return -1;
                }
                break;
            case 0x17:
            case 0x18:
                if (iDepthA == 0)
                    bFound = 1;
                else
                    ++iCount8;
                break;
            case 0x19:
                if (iDepthA == 0)
                {
                    bFound = 1;
                }
                else
                {
                    --iDepthA;
                    ++iCount8;
                }
                break;
            default:
                ++iCount8;
                break;
            }
        }
        pNodeX = pNodeX->Next();
    }

    if (!bFound)
    {
        ReportCompileError(apCompileData, "Mismatched if/else/endif block starting on line %d.", *apStartingLine);
        return -1;
    }
    return iCount8;
}

// CRT isalnum in the game image
int __cdecl FUN_00ec86e1(int c);
int strcpy_s(char* dst, unsigned int size, const char* src);
void* __memcpy(void* dst, const void* src, unsigned int size);

typedef bool(__cdecl* ScriptParseFunctionB15)(unsigned short usParamCount, void* pParams,
                                              FUN_005af370_shell* pkLineBuffer, CStringArgument* pkData);

struct ScriptFunctionB15
{
    const char* pFunctionName;
    const char* m_pShortName;
    unsigned int m_eOutputType;
    const char* m_pDescription;
    bool m_bRequiresReference;
    unsigned short usParamCount;
    void* pParameters;
    void* m_pfnExecute;
    ScriptParseFunctionB15 m_pfnCompile;
};

int __cdecl ScriptCompiler_GetNextWord(CStringArgument* apCompileData, ScriptVariableData* apNextWord, char* pInput, unsigned int* auiLineOffset,
                         int abAllowVariable, int abAllowUnknownRef);
ScriptFunctionB15* __cdecl FUN_005b1120(unsigned int m_uiReferenceID);

struct FUN_005b1570_Class
{
    int ScriptCompiler_ReplaceVariablesAndFunctions(CStringArgument* apCompileData, unsigned int* apScriptLine, char* apPostfix, char* apExpLine);
};

// @fnv 0x005b1570
// src: XBOX_RETAIL_PDB 0x005b1570
// pdb: ScriptCompiler::ReplaceVariablesAndFunctions | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData, apScriptLine, apPostfix, apExpLine
// pdb locals: inSize, expCount, n, currentWord, pFunctionDef, pParameterDef, tempLine, sCount, pCompile
int FUN_005b1570_Class::ScriptCompiler_ReplaceVariablesAndFunctions(CStringArgument* apCompileData, unsigned int* apScriptLine, char* apPostfix,
                                          char* apExpLine)
{
    unsigned int uiIndex;
    unsigned int uiOutOffset;
    unsigned int uiLength;

    uiLength = BSStringLength(apPostfix);
    uiOutOffset = 0;
    for (uiIndex = 0; uiIndex < uiLength; uiIndex++)
    {
        if (FUN_00ec86e1(apPostfix[uiIndex]) || apPostfix[uiIndex] == '"')
        {
            ScriptVariableData kWord;
            kWord.SCRIPT_WORD_ctor();
            if (!ScriptCompiler_GetNextWord(apCompileData, &kWord, apPostfix, &uiIndex, 1, 1))
                return 0;

            if (kWord.m_uiVariableID > 0)
            {
                if (kWord.m_cTypeCode == 'G')
                {
                    apExpLine[uiOutOffset] = 'G';
                    uiOutOffset++;
                }
                else
                {
                    apExpLine[uiOutOffset] = 'r';
                    uiOutOffset++;
                }
                *(unsigned short*)(apExpLine + uiOutOffset) = kWord.m_uiVariableID;
                uiOutOffset += 2;
            }

            if (kWord.m_uiFormID > 0)
            {
                apExpLine[uiOutOffset] = kWord.m_cTypeCode;
                uiOutOffset++;
                *(unsigned short*)(apExpLine + uiOutOffset) = kWord.m_uiFormID;
                uiOutOffset += 2;
            }
            else if (kWord.m_cTypeCode == 'X')
            {
                apExpLine[uiOutOffset] = 'X';
                uiOutOffset++;
                *(unsigned short*)(apExpLine + uiOutOffset) = kWord.m_uiReferenceID;
                uiOutOffset += 2;

                ScriptFunctionB15* pkFunction = FUN_005b1120(kWord.m_uiReferenceID);
                if (!pkFunction)
                {
                    ScriptCompileError((ScriptSource*)apCompileData, "Syntax Error.  Undefined function '%s'.", &kWord);
                    return 0;
                }
                else if (pkFunction->m_bRequiresReference && *(bool*)&apCompileData->header[0x10] && kWord.m_uiVariableID == 0)
                {
                    ScriptCompileError((ScriptSource*)apCompileData,
                                       "Reference function '%s' requires explicit reference in quest script.", &kWord);
                    return 0;
                }

                if (pkFunction->pParameters)
                {
                    void* pParams = pkFunction->pParameters;
                    ScriptParseFunctionB15 pfnCompile = pkFunction->m_pfnCompile;
                    unsigned short usParamCount = pkFunction->usParamCount;
                    FUN_005af370_shell kLineBuffer;
                    strcpy_s(kLineBuffer.m_data, 0x200, apPostfix + uiIndex);
                    kLineBuffer.m_uiValueAfterSecondBuffer = 0;
                    kLineBuffer.m_uiValueBeforeData = *apScriptLine;
                    if (!pfnCompile(usParamCount, pParams, &kLineBuffer, apCompileData))
                        return 0;
                    *(unsigned short*)(apExpLine + uiOutOffset) = kLineBuffer.m_uiValueAfterSecondBuffer;
                    uiOutOffset += 2;
                    __memcpy(apExpLine + uiOutOffset, kLineBuffer.m_auiShapeKeys, kLineBuffer.m_uiValueAfterSecondBuffer);
                    uiOutOffset += kLineBuffer.m_uiValueAfterSecondBuffer;
                    uiIndex += kLineBuffer.uiElapsedMilliseconds;
                }
                else
                {
                    *(unsigned short*)(apExpLine + uiOutOffset) = 0;
                    uiOutOffset += 2;
                }
            }
            else if (kWord.m_cTypeCode != 'G')
            {
                if (kWord.m_uiVariableID == 0 && apCompileData->GetReferencedForm(&kWord, false, false))
                {
                    apExpLine[uiOutOffset] = 'Z';
                    uiOutOffset++;
                    *(unsigned short*)(apExpLine + uiOutOffset) = kWord.m_uiVariableID;
                    uiOutOffset += 2;
                }
                else
                {
                    ScriptCompileError((ScriptSource*)apCompileData, "Syntax Error.  Unknown command '%s'.", &kWord);
                    return 0;
                }
            }
        }

        if (uiIndex < uiLength)
        {
            apExpLine[uiOutOffset] = apPostfix[uiIndex];
            uiOutOffset++;
        }
    }

    apExpLine[uiOutOffset + 1] = 0;
    return uiOutOffset;
}

struct ScriptLineS
{
    unsigned int m_uiLineNumber;
    char padCur[0x40c];
    unsigned int m_uiType;
};
struct ScriptLineNode
{
    ScriptLineS*& FUN_006815c0();
    ScriptLineNode* FUN_00726070();
};
bool __fastcall FUN_008256d0(ScriptLineNode* pkNode);
// @fnv 0x005b1180
// src: XBOX_PDB 0x005b1180
// pdb: ScriptCompiler::VerifyBlocks | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData
// pdb locals: pCurrentLine, bInsideBlock, pList, exp
bool ScriptCompiler::VerifyBlocks(CStringArgument* apCompileData)
{
    unsigned int uiType5;
    bool bInBlock = false;
    ScriptLineNode* pIterCur = (ScriptLineNode*)apCompileData->GetScriptLineList();
    while (pIterCur && !FUN_008256d0(pIterCur))
    {
        ScriptLineS* pkLine = pIterCur->FUN_006815c0();
        uiType5 = pkLine->m_uiType;
        switch (uiType5)
        {
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x1d:
        case 0x1f:
            break;
        case 0x10:
            if (bInBlock)
            {
                apCompileData->m_uiLastLineNumber = pkLine->m_uiLineNumber;
                ReportScriptError(apCompileData, "Nested begin/end blocks are not allowed.");
                return false;
            }
            else
            {
                bInBlock = true;
            }
            break;
        case 0x11:
            if (!bInBlock)
            {
                apCompileData->m_uiLastLineNumber = pkLine->m_uiLineNumber;
                ReportScriptError(apCompileData, "End without matching Begin.");
                return false;
            }
            else
            {
                bInBlock = false;
            }
            break;
        default:
            if (!bInBlock)
            {
                apCompileData->m_uiLastLineNumber = pkLine->m_uiLineNumber;
                ReportScriptError(apCompileData, "All script commands must be inside a begin/end block.");
                return false;
            }
            break;
        }
        pIterCur = pIterCur->FUN_00726070();
    }
    return true;
}

namespace fnv_005b12c0 {
struct ScriptLine
{
    unsigned int m_uiValueBeforeData;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    int unk6C;
    int unk70;
    int unk74;
    int unk78;
    int unk7C;
    int unk80;
    int unk84;
    int unk88;
    int unk8C;
    int unk90;
    int unk94;
    int unk98;
    int unk9C;
    int unkA0;
    int unkA4;
    int unkA8;
    int unkAC;
    int unkB0;
    int unkB4;
    int unkB8;
    int unkBC;
    int unkC0;
    int unkC4;
    int unkC8;
    int unkCC;
    int unkD0;
    int unkD4;
    int unkD8;
    int unkDC;
    int unkE0;
    int unkE4;
    int unkE8;
    int unkEC;
    int unkF0;
    int unkF4;
    int unkF8;
    int unkFC;
    int unk100;
    int unk104;
    int unk108;
    int unk10C;
    int unk110;
    int unk114;
    int unk118;
    int unk11C;
    int unk120;
    int unk124;
    int unk128;
    int unk12C;
    int unk130;
    int unk134;
    int unk138;
    int unk13C;
    int unk140;
    int unk144;
    int unk148;
    int unk14C;
    int unk150;
    int unk154;
    int unk158;
    int unk15C;
    int unk160;
    int unk164;
    int unk168;
    int unk16C;
    int unk170;
    int unk174;
    int unk178;
    int unk17C;
    int unk180;
    int unk184;
    int unk188;
    int unk18C;
    int unk190;
    int unk194;
    int unk198;
    int unk19C;
    int unk1A0;
    int unk1A4;
    int unk1A8;
    int unk1AC;
    int unk1B0;
    int unk1B4;
    int unk1B8;
    int unk1BC;
    int unk1C0;
    int unk1C4;
    int unk1C8;
    int unk1CC;
    int unk1D0;
    int unk1D4;
    int unk1D8;
    int unk1DC;
    int unk1E0;
    int unk1E4;
    int unk1E8;
    int unk1EC;
    int unk1F0;
    int unk1F4;
    int unk1F8;
    int unk1FC;
    int unk200;
    int unk204;
    int unk208;
    int unk20C;
    int unk210;
    int unk214;
    int unk218;
    int unk21C;
    int unk220;
    int unk224;
    int unk228;
    int unk22C;
    int unk230;
    int unk234;
    int unk238;
    int unk23C;
    int unk240;
    int unk244;
    int unk248;
    int unk24C;
    int unk250;
    int unk254;
    int unk258;
    int unk25C;
    int unk260;
    int unk264;
    int unk268;
    int unk26C;
    int unk270;
    int unk274;
    int unk278;
    int unk27C;
    int unk280;
    int unk284;
    int unk288;
    int unk28C;
    int unk290;
    int unk294;
    int unk298;
    int unk29C;
    int unk2A0;
    int unk2A4;
    int unk2A8;
    int unk2AC;
    int unk2B0;
    int unk2B4;
    int unk2B8;
    int unk2BC;
    int unk2C0;
    int unk2C4;
    int unk2C8;
    int unk2CC;
    int unk2D0;
    int unk2D4;
    int unk2D8;
    int unk2DC;
    int unk2E0;
    int unk2E4;
    int unk2E8;
    int unk2EC;
    int unk2F0;
    int unk2F4;
    int unk2F8;
    int unk2FC;
    int unk300;
    int unk304;
    int unk308;
    int unk30C;
    int unk310;
    int unk314;
    int unk318;
    int unk31C;
    int unk320;
    int unk324;
    int unk328;
    int unk32C;
    int unk330;
    int unk334;
    int unk338;
    int unk33C;
    int unk340;
    int unk344;
    int unk348;
    int unk34C;
    int unk350;
    int unk354;
    int unk358;
    int unk35C;
    int unk360;
    int unk364;
    int unk368;
    int unk36C;
    int unk370;
    int unk374;
    int unk378;
    int unk37C;
    int unk380;
    int unk384;
    int unk388;
    int unk38C;
    int unk390;
    int unk394;
    int unk398;
    int unk39C;
    int unk3A0;
    int unk3A4;
    int unk3A8;
    int unk3AC;
    int unk3B0;
    int unk3B4;
    int unk3B8;
    int unk3BC;
    int unk3C0;
    int unk3C4;
    int unk3C8;
    int unk3CC;
    int unk3D0;
    int unk3D4;
    int unk3D8;
    int unk3DC;
    int unk3E0;
    int unk3E4;
    int unk3E8;
    int unk3EC;
    int unk3F0;
    int unk3F4;
    int unk3F8;
    int unk3FC;
    int unk400;
    int unk404;
    int unk408;
    int unk40C;
    int unk410;
};

struct LineNode
{
    ScriptLine** FUN_006815c0();
    LineNode* FUN_00726070();
    bool FUN_008256d0();
};

struct CStringArgument
{
    LineNode* FUN_00613790();
};

void ReportScriptError(CStringArgument* apData, const char* apMsg, ...);

struct FUN_005b12c0_owner
{
    int ScriptCompiler_CountBlockLines(CStringArgument* apCompileData, unsigned int* apStartingLine);
};

// @fnv 0x005b12c0
// src: XBOX_RETAIL_PDB 0x005b12c0
// pdb: ScriptCompiler::CountBlockLines | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData, apStartingLine
// pdb locals: done, pCurrentLine, pList, exp, count
int FUN_005b12c0_owner::ScriptCompiler_CountBlockLines(CStringArgument* apCompileData, unsigned int* apStartingLine)
{
    int count = 0;
    int type;
    int done = 0;
    LineNode* node5 = apCompileData->FUN_00613790();
    ScriptLine* line6;
    while (node5 && !node5->FUN_008256d0() && !done)
    {
        line6 = *node5->FUN_006815c0();
        if (line6->m_uiValueBeforeData > *apStartingLine)
        {
            type = line6->unk410;
            switch (type)
            {
            case 0xffff:
                ReportScriptError(apCompileData, "Begin without matching end starting on line %d.", *apStartingLine);
                return -1;
            case 18:
            case 19:
            case 20:
                break;
            case 16:
                ReportScriptError(apCompileData, "Nested begin/end blocks are not allowed.");
                return -1;
            case 17:
                done = 1;
                break;
            default:
                count++;
                break;
            }
        }
        node5 = node5->FUN_00726070();
    }
    if (!done)
    {
        ReportScriptError(apCompileData, "Mismatched begin/end block starting on line %d.", *apStartingLine);
        return -1;
    }
    return count;
}
}  // namespace fnv_005b12c0

class Item5AE6C0
{
  public:
    ~Item5AE6C0();
    int unk00;
};
class List5AE6C0
{
  public:
    Item5AE6C0* m_item;
    List5AE6C0* m_pkNext;
    bool IsEmpty();
    Item5AE6C0*& GetItem();
    void RemoveHead();
};
class Owner5AE6C0
{
  public:
    List5AE6C0* GetList();
    void ScriptCompileData_ClearAllReferenceObjects();
};

// Deletes each head item, then removes that head, until the list is empty.
// @fnv 0x005ae6c0
// src: XBOX_RETAIL_PDB 0x005ae6c0
// pdb: ScriptCompileData::ClearAllReferenceObjects | TESScriptCompiler.obj | retail, object-file run extension, high
// pdb locals: pList
void Owner5AE6C0::ScriptCompileData_ClearAllReferenceObjects()
{
    List5AE6C0* pkList = GetList();
    while (pkList && !pkList->IsEmpty())
    {
        delete pkList->GetItem();
        pkList->RemoveHead();
    }
}

void __cdecl ReportScriptError(CStringArgument* a, const char* fmt, ...);
unsigned char __cdecl ScriptCompiler_GetFunction(ScriptVariableData* apWord);
extern "C" char* strchr(char* s, int c);
int __cdecl strcpy_s(char* dst, unsigned int n, const char* src);
extern "C" int isalnum(int c);
extern "C" int isalpha(int c);
extern "C" void* _FNV_DynamicCast(void* obj, int a, void* src, void* dst, int b);
extern int DAT_01186500;
struct ScriptFuncDef
{
    int field00;
    int field04;
    TESForm* pForm;
};
struct CompilerShell : public CStringArgument
{
    bool ScriptCompileData_GetReferencedObject(ScriptVariableData* apCurrentWord, int abAddRefVariable, bool abAllowUnknownRef);
    bool ScriptCompileData_GetVariable(ScriptVariableData* apCurrentWord);
    ScriptFuncDef* GetNodeValue(unsigned int idx);
};
// Parses a quoted string or identifier from apExpLine into apWord, resolving references or script commands when aAllowRef is set.
// @fnv 0x005af5f0
// src: XBOX_PDB 0x005af5f0
// pdb: ScriptCompiler::GetNextWord | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb params: apCompileData, apNextWord, apExpLine, auiLineOffset, abAllowVariable, abAllowUnknownRef
// pdb locals: offset, quote1, quote2, pRefTest, pQuestTest, pScriptRefObj
int __cdecl ScriptCompiler_GetNextWord(CompilerShell* apCompileData, ScriptVariableData* apNextWord, char* apExpLine, int* auiLineOffset,
                         bool abAllowVariable, bool abAllowUnknownRef)
{
    ScriptCompiler_SkipWhiteSpace(apExpLine, auiLineOffset);
    int len = 0;
    if (apExpLine[*auiLineOffset] == '"')
    {
        if (!HasBalancedQuotes(apExpLine, *auiLineOffset))
        {
            ReportScriptError((CStringArgument*)apCompileData, "Syntax Error.  Mismatched quotes.");
            return 0;
        }
        char* pEnd;
        char* pStart;
        pStart = strchr(apExpLine + *auiLineOffset, '"');
        *pStart = ' ';
        pEnd = strchr(apExpLine + *auiLineOffset, '"');
        *pEnd = 0;
        strcpy_s((char*)apNextWord, 0x200, pStart + 1);
        len = BSStringLength((const char*)apNextWord);
        *pStart = '"';
        *pEnd = '"';
        *auiLineOffset = len + *auiLineOffset + 2;
        if (apExpLine[*auiLineOffset] == '.')
        {
            if (!abAllowVariable)
            {
                ReportScriptError((CStringArgument*)apCompileData,
                                  "Syntax Error.  Reference not allowed in this context.");
                return 0;
            }
            if (apNextWord->m_uiVariableID > 0)
            {
                ReportScriptError((CStringArgument*)apCompileData, "Syntax Error.  Invalid reference syntax.");
                return 0;
            }
            *auiLineOffset = *auiLineOffset + 1;
            if (!apCompileData->ScriptCompileData_GetReferencedObject(apNextWord, 0, abAllowUnknownRef))
            {
                ReportScriptError((CStringArgument*)apCompileData, "Unknown object '%s'.", apNextWord);
                return 0;
            }
            return ScriptCompiler_GetNextWord(apCompileData, apNextWord, apExpLine, auiLineOffset, true, abAllowUnknownRef);
        }
        ((char*)apNextWord)[len] = 0;
    }
    if (len == 0)
    {
        while (isalnum(apExpLine[*auiLineOffset]) || apExpLine[*auiLineOffset] == '.' || apExpLine[*auiLineOffset] == '-' ||
               apExpLine[*auiLineOffset] == '_')
        {
            {
                ((char*)apNextWord)[len] = apExpLine[*auiLineOffset];
                len = len + 1;
                *auiLineOffset = *auiLineOffset + 1;
                if (((char*)apNextWord)[len - 1] == '.')
                {
                    if (isalpha(((char*)apNextWord)[0]))
                    {
                        if (!abAllowVariable)
                        {
                            ReportScriptError((CStringArgument*)apCompileData,
                                              "Syntax Error.  Reference not allowed in this context.");
                            return 0;
                        }
                        if (apNextWord->m_uiVariableID != 0)
                        {
                            ReportScriptError((CStringArgument*)apCompileData,
                                              "Syntax Error.  Invalid reference syntax.");
                            return 0;
                        }
                        ((char*)apNextWord)[len - 1] = 0;
                        if (!apCompileData->ScriptCompileData_GetReferencedObject(apNextWord, 0, abAllowUnknownRef))
                        {
                            ReportScriptError((CStringArgument*)apCompileData, "Unknown object '%s'.", apNextWord);
                            return 0;
                        }
                        return ScriptCompiler_GetNextWord(apCompileData, apNextWord, apExpLine, auiLineOffset, true, abAllowUnknownRef);
                    }
                }
                ((char*)apNextWord)[len] = 0;
            }
        }
    }
    if (abAllowVariable)
    {
        if (apNextWord->m_uiVariableID > 0)
        {
            if (!apCompileData->ScriptCompileData_GetVariable(apNextWord) && !ScriptCompiler_GetFunction(apNextWord))
            {
                ReportScriptError((CStringArgument*)apCompileData, "Unknown variable or function '%s'.", apNextWord);
                return 0;
            }
            ScriptFuncDef* pScriptFunction = apCompileData->GetNodeValue(apNextWord->m_uiVariableID);
            void* castA;
            void* castB;
            castB = _FNV_DynamicCast(pScriptFunction->pForm, 0, &DAT_01183028, &DAT_011841cc, 0);
            castA = _FNV_DynamicCast(pScriptFunction->pForm, 0, &DAT_01183028, &DAT_01186500, 0);
            if (castB == 0 && castA == 0 && !abAllowUnknownRef)
            {
                ReportScriptError(
                    (CStringArgument*)apCompileData,
                    "Syntax error.  Invalid reference '%s' (only quests and object references are allowed in this context).",
                    pScriptFunction->pForm->GetName());
                return 0;
            }
            if (castA != 0)
            {
                if (apNextWord->m_cTypeCode == 'X')
                {
                    ReportScriptError((CStringArgument*)apCompileData,
                                      "Syntax error.  Functions may not be called on quests.");
                    return 0;
                }
            }
        }
        if (apNextWord->m_cTypeCode == 0 && !apCompileData->ScriptCompileData_GetVariable(apNextWord))
        {
            ScriptCompiler_GetFunction(apNextWord);
        }
    }
    return len;
}

namespace fnv_005afa50 {
struct FUN_005afa50_list
{
    void*& head();
    FUN_005afa50_list* next();
};

struct CStringArgument
{
    int unk00;
    int unk04;
    int unk08;
    int unk0C;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    FUN_005afa50_list m_kReferencedObjectList;
};

struct FUN_005afa50_shell : public CStringArgument
{
    bool ScriptCompileData_VerifyReferencedObjectList();
};

struct FUN_005afa50_obj
{
    unsigned char _pad0[8];
    unsigned int m_08;
    unsigned int m_0c;
    char* GetName();
};

void __cdecl ReportScriptError(void* owner, const char* fmt, const char* name);

// Walks m_kReferencedObjectList and returns false, logging via FUN_005aea90, if any item has both m_08 and m_0c zero.
// @fnv 0x005afa50
// src: XBOX_PDB 0x005afa50
// pdb: ScriptCompileData::VerifyReferencedObjectList | TESScriptCompiler.obj | retail, string-set bridge, high
// pdb locals: pList, pRefObj
bool FUN_005afa50_shell::ScriptCompileData_VerifyReferencedObjectList()
{
    FUN_005afa50_list* list = &m_kReferencedObjectList;
    while (list != 0 && list->head() != 0)
    {
        FUN_005afa50_obj* item = (FUN_005afa50_obj*)list->head();
        list = list->next();
        if (item->m_08 == 0 && item->m_0c == 0)
        {
            ReportScriptError(this, "Unknown referenced object '%s'.", item->GetName());
            return false;
        }
    }
    return true;
}
}  // namespace fnv_005afa50

#include <stdlib.h>
int __cdecl IsIntegerString(const char*);
void* __cdecl FUN_00401460(void*, const void*, unsigned int);
unsigned int __cdecl BSstrlen(const char*);

namespace MsgCompileData {
extern "C" char DAT_01186130[];
}
struct MessageParseWord_5b3c
{
    char text[0x200];
    unsigned int reference;
    char type;
    unsigned int function;
    unsigned int variable;
    TESForm* form;
    MessageParseWord_5b3c* SCRIPT_WORD_ctor();
};
struct MessageArrayWord_5b3c : MessageParseWord_5b3c
{
    MessageArrayWord_5b3c();
};
struct MessageParameter_5b3c
{
    char* name;
};
int __cdecl ScriptCompiler_GetNextWord(void*, MessageParseWord_5b3c*, char*, unsigned int*, bool, bool);
struct CompileMessageContext_5b3c
{
    bool ScriptCompileData_GetReferencedObject(MessageParseWord_5b3c*, bool, bool);
};
void __cdecl ReportScriptError(void*, const char*, ...);

// Parses and compiles a MessageBox script line into binary output, handling literal text, parameters, variables, buttons, and duration.
// @fnv 0x005b3cd0
// src: XBOX_RETAIL_PDB 0x005b3cd0 (was COMMUNITY)
// pdb: ScriptCompiler::MessageBoxCompile | TESScriptCompiler.obj | retail, unique string literal, high
// pdb params: asParamCount, apParameters, apScriptLine, apCompileData
bool __cdecl ScriptCompiler_MessageBoxCompile(unsigned short asParamCount, MessageParameter_5b3c* apParameters, ScriptLineBuffer* apScriptLine,
                          CompileMessageContext_5b3c* context, bool abIsMessageBox, bool abStringOnly)
{
    int size;
    {
        MessageParseWord_5b3c message;
        {
            BGSMessage* form;
            {
                unsigned short version;
                {
                    int count;
                    {
                        const char* text;
                        {
                            char* output;
                            apScriptLine->uiElapsedMilliseconds = 0;
                            version = 1;
                            *(unsigned short*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] = version;
                            apScriptLine->m_uiValueAfterSecondBuffer += 2;
                            message.SCRIPT_WORD_ctor();
                            form = 0;
                            size = 0;
                            if (abStringOnly)
                            {
                                size = ScriptCompiler_GetNextWord(context, &message, apScriptLine->cLine, &apScriptLine->uiElapsedMilliseconds, false,
                                                    false);
                                if (!size)
                                {
                                    ReportScriptError(context, "No message text.");
                                    return false;
                                }
                                *(unsigned short*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] =
                                    (unsigned short)size;
                                apScriptLine->m_uiValueAfterSecondBuffer += 2;
                                FUN_00401460(&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer], &message, size);
                                apScriptLine->m_uiValueAfterSecondBuffer += size;
                            }
                            else
                            {
                                MessageParseWord_5b3c word;
                                word.SCRIPT_WORD_ctor();
                                size = ScriptCompiler_GetNextWord(context, &word, apScriptLine->cLine, &apScriptLine->uiElapsedMilliseconds, false,
                                                    false);
                                if (!size)
                                {
                                    ReportScriptError(context, "Missing parameter %s.\r\nCompiled script not saved!",
                                                 apParameters->name);
                                    return false;
                                }
                                if (word.reference > 0 || word.type)
                                {
                                    ReportScriptError(context,
                                                 "Parameter %s may not be a variable.\r\nCompiled script not saved!",
                                                 apParameters->name);
                                    return false;
                                }
                                if (!context->ScriptCompileData_GetReferencedObject(&word, false, false) || word.reference == 0)
                                {
                                    ReportScriptError(context,
                                                 "Item '%s' not found for parameter %s.\r\nCompiled script not saved!",
                                                 &word, apParameters->name);
                                    return false;
                                }
                                form = static_cast<BGSMessage*>(
                                    _FNV_DynamicCast(word.form, 0, DAT_01183028, MsgCompileData::DAT_01186130, 0));
                                if (word.variable == 0 && form == 0)
                                {
                                    ReportScriptError(context,
                                                 "Invalid message '%s' for parameter %s.\r\nCompiled script not saved!",
                                                 &word, apParameters->name);
                                    return false;
                                }
                                char* output = apScriptLine->cOutput;
                                output[apScriptLine->m_uiValueAfterSecondBuffer] = 'r';
                                apScriptLine->m_uiValueAfterSecondBuffer++;
                                *(unsigned short*)&output[apScriptLine->m_uiValueAfterSecondBuffer] =
                                    (unsigned short)word.reference;
                                apScriptLine->m_uiValueAfterSecondBuffer += 2;
                            }
                            text = 0;
                            if (abStringOnly)
                                text = message.text;
                            else
                            {
                                // Address the description subobject directly: /Od adds a null check to base casts.
                                text = form ? reinterpret_cast<TESDescription*>(
                                                  (char*)form + offsetof(BGSMessage, TESDescription::m_uiDescriptionFileOffset) -
                                                  offsetof(TESDescription, m_uiDescriptionFileOffset))
                                                  ->GetDescription(0, static_cast<CHUNK_ID>(0x43534544))
                                            : 0;
                            }
                            count = 0;
                            while (text && *text)
                            {
                                if (*text == '%')
                                {
                                    if (text[1] == '%')
                                        text++;
                                    else
                                        count++;
                                }
                                text++;
                            }
                            if (count >= 10)
                            {
                                ReportScriptError(context, "Max variables of %d exceeded.", 10);
                                return false;
                            }
                            *(unsigned short*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] = (unsigned short)count;
                            apScriptLine->m_uiValueAfterSecondBuffer += 2;
                            output = apScriptLine->cOutput;
                            for (int i = 0; i < count; i++)
                            {
                                MessageParseWord_5b3c word;
                                word.SCRIPT_WORD_ctor();
                                size = ScriptCompiler_GetNextWord(context, &word, apScriptLine->cLine, &apScriptLine->uiElapsedMilliseconds, true,
                                                    false);
                                if (!size)
                                {
                                    ReportScriptError(context,
                                                 "Too few variables in MessageBox parameters; expected %d, found %d.",
                                                 count, i);
                                    return false;
                                }
                                if (!word.type)
                                {
                                    ReportScriptError(context, "Unknown variable '%s' in MessageBox parameters.", &word);
                                    return false;
                                }
                                if (word.reference > 0)
                                {
                                    if (word.type == 'G')
                                    {
                                        output[apScriptLine->m_uiValueAfterSecondBuffer] = 'G';
                                        apScriptLine->m_uiValueAfterSecondBuffer++;
                                    }
                                    else
                                    {
                                        output[apScriptLine->m_uiValueAfterSecondBuffer] = 'r';
                                        apScriptLine->m_uiValueAfterSecondBuffer++;
                                    }
                                    *(unsigned short*)&output[apScriptLine->m_uiValueAfterSecondBuffer] =
                                        (unsigned short)word.reference;
                                    apScriptLine->m_uiValueAfterSecondBuffer += 2;
                                }
                                if (word.type != 'G')
                                {
                                    if (word.variable > 0)
                                    {
                                        output[apScriptLine->m_uiValueAfterSecondBuffer] = word.type;
                                        apScriptLine->m_uiValueAfterSecondBuffer++;
                                        *(unsigned short*)&output[apScriptLine->m_uiValueAfterSecondBuffer] =
                                            (unsigned short)word.variable;
                                        apScriptLine->m_uiValueAfterSecondBuffer += 2;
                                    }
                                    else
                                    {
                                        ReportScriptError(
                                            context,
                                            "Unknown compiler error in MessageBoxCompile.  Failed to parse variable '%s'.",
                                            &word);
                                        return false;
                                    }
                                }
                            }
                            if (!abStringOnly)
                            {
                                if (abIsMessageBox)
                                {
                                    int buttons = 0;
                                    {
                                        MessageArrayWord_5b3c words[10];
                                        do
                                        {
                                            size = ScriptCompiler_GetNextWord(context, &words[buttons], apScriptLine->cLine,
                                                                &apScriptLine->uiElapsedMilliseconds, false, false);
                                            if (size > 0)
                                                buttons++;
                                        } while (size > 0 && buttons < 10);
                                        *(unsigned short*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] =
                                            (unsigned short)buttons;
                                        apScriptLine->m_uiValueAfterSecondBuffer += 2;
                                        for (int j = 0; j < buttons; j++)
                                        {
                                            *(unsigned short*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] = 1;
                                            apScriptLine->m_uiValueAfterSecondBuffer += 2;
                                            size = BSstrlen(words[j].text);
                                            *(unsigned short*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] =
                                                (unsigned short)size;
                                            apScriptLine->m_uiValueAfterSecondBuffer += 2;
                                            FUN_00401460(&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer], &words[j],
                                                         size);
                                            apScriptLine->m_uiValueAfterSecondBuffer += size;
                                        }
                                    }
                                }
                                else
                                {
                                    int duration = 0;
                                    MessageParseWord_5b3c word;
                                    word.SCRIPT_WORD_ctor();
                                    size = ScriptCompiler_GetNextWord(context, &word, apScriptLine->cLine, &apScriptLine->uiElapsedMilliseconds,
                                                        false, false);
                                    if (size > 0)
                                    {
                                        if (!IsIntegerString(word.text))
                                        {
                                            ReportScriptError(
                                                context,
                                                "Message time must be an integer.\r\nCompiled script not saved!");
                                            return false;
                                        }
                                        else
                                            duration = atol(word.text);
                                    }
                                    *(int*)&apScriptLine->cOutput[apScriptLine->m_uiValueAfterSecondBuffer] = duration;
                                    apScriptLine->m_uiValueAfterSecondBuffer += 4;
                                }
                            }
                            return true;
                        }
                    }
                }
            }
        }
    }
}
