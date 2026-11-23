#include "../../include/BSStringT.h"

// tesscriptcompiler_ctx_variant.cpp: context variant of src/fallout_shared/tesscriptcompiler.cpp: its stripped skeleton, as these functions were checked
// against, for verified matches whose own declarations clash with the unit's current types.
#include "../../include/ScriptReferencedObject.h"
#include "../../include/TESForm.h"
#include "../../include/TESObjectREFR.h"
#include "tesscriptcompiler_packet_decls.h"
ScriptFunctionInfo* GetFunctionByOpcode(SCRIPT_OUTPUT output);

int BSstricmp(const char*, const char*);
extern TESForm* g_thePlayer;
#include "../../include/BSSimpleList.h"
struct ScriptInfo
{
    unsigned int variableCount, m_uiReferenceID, dataSize, lastID;
    unsigned short type;
    bool compiled;
    unsigned char unk13;
};
struct ScriptToken
{
    char name[0x200];
    unsigned int refIndex;
    unsigned char type;
    unsigned int opcode;
    unsigned int variableID;
    TESForm* form;
    unsigned int unk214, unk218, paramTextLen;
};
struct CStringArgument
{
    char* scriptText;
    unsigned int textOffset;
    int runtimeMode;
    ::BSStringT<char> scriptName;
    unsigned int errorCode;
    bool partialScript;
    unsigned int curLineNumber;
    unsigned char* scriptData;
    unsigned int dataOffset;
    ScriptInfo info;
    BSSimpleList<void*> vars;
    BSSimpleList<ScriptReferencedObject*> refVars;
    void* currentScript;
    BSSimpleList<void*> lines;
    BSSimpleList<ScriptReferencedObject*>* BSFile_GetDevice();
    bool ScriptCompileData_GetReferencedObject(ScriptToken*, bool, bool);
};
TESForm* FUN_00483a00(const char*);
TESForm* TESForm_GetFormByNumericID(unsigned int);
unsigned int BSstrtoul(const char*, char**, int);
void ReportScriptError(CStringArgument*, const char*, ...);
// @fnv 0x005afad0
// src: XBOX_PDB 0x005afad0
// pdb: ScriptCompileData::GetReferencedObject | TESScriptCompiler.obj | retail, upstream name already matches, high
// pdb params: apCurrentWord, abAddRefVariable, abAllowUnknownRef
// pdb locals: bFound, pList, n, pRefObject, pstop, iNumericID, pTestRef
bool CStringArgument::ScriptCompileData_GetReferencedObject(ScriptToken* apCurrentWord, bool abAddRefVariable, bool abAllowUnknownRef)
{
    bool result = false;
    unsigned int index = 1;
    BSSimpleList<ScriptReferencedObject*>* entry = &refVars;
    while (entry && entry->GetItem())
    {
        if (!reinterpret_cast<const ::BSStringT<char>*>(&entry->GetItem()->m_kFormName)->StrCmp(apCurrentWord->name, true))
        {
            apCurrentWord->refIndex = index;
            apCurrentWord->form = entry->GetItem()->pForm;
            if (abAddRefVariable)
                entry->GetItem()->m_uiFormID = apCurrentWord->variableID;
            else
                apCurrentWord->variableID = entry->GetItem()->m_uiFormID;
            result = true;
            break;
        }
        entry = entry->GetNext();
        index++;
    }
    if (!result)
    {
        ScriptReferencedObject* reference = new ScriptReferencedObject;
        if (abAddRefVariable)
            result = true;
        else
        {
            if (!BSstricmp(apCurrentWord->name, "Player"))
            {
                reference->pForm = g_thePlayer;
                result = true;
            }
            if (!result)
            {
                reference->pForm = FUN_00483a00(apCurrentWord->name);
                if (reference->pForm)
                {
                    if (reference->pForm->FUN_00401170() == 3)
                        reference->pForm = 0;
                    else
                        result = true;
                }
            }
            if (!result)
            {
                char* end = 0;
                unsigned int formID = BSstrtoul(apCurrentWord->name, &end, 16);
                if (formID && end && !*end)
                {
                    reference->pForm = TESForm_GetFormByNumericID(formID);
                    if (reference->pForm)
                        result = true;
                }
            }
            if (result && runtimeMode != 1)
            {
                TESObjectREFR* refr = dynamic_cast<TESObjectREFR*>(reference->pForm);
                if (refr && !refr->TESObjectREFR_GetRefPersists_005653d0())
                {
                    ReportScriptError(this, "Non-persistent reference '%s' cannot be used in a script.", apCurrentWord->name);
                    result = false;
                }
            }
        }
        if (result || abAllowUnknownRef)
        {
            result = true;
            if (abAddRefVariable)
            {
                reference->pForm = 0;
                reference->m_uiFormID = apCurrentWord->variableID;
            }
            else
            {
                apCurrentWord->form = reference->pForm;
                reference->m_uiFormID = 0;
            }
            reinterpret_cast<::BSStringT<char>*>(&reference->m_kFormName)->Set(apCurrentWord->name, 0);
            info.m_uiReferenceID++;
            BSFile_GetDevice()->AddTail(reference);
            apCurrentWord->refIndex = info.m_uiReferenceID;
        }
        else
            delete reference;
    }
    return result;
}

#include <ctype.h>
extern const char missingSetExpression[];

struct CompileLine;

struct CompileState;

struct CompileWord
{
    char text[512];
    unsigned refIndex;
    char variableType;
    unsigned functionCode;
    unsigned variableID;
    void* form;
    CompileWord();
};



struct CompileVariable
{
    unsigned id;
    unsigned unk04, unk08, unk0c;
    bool integer;
    unsigned char unk11;
    unsigned short unk12;
    unsigned unk14;
    ::BSStringT<char> name;
    CompileVariable();
};

struct CompileVariables
{
    void AddTail(CompileVariable* const&);
};

struct CompileScript
{
    signed char FUN_005ac6a0(const char*, CompileVariable*);
};

struct CompileState
{
    unsigned unk00, unk04, unk08, unk0c, unk10, unk14, unk18, unk1c;
    char* output;
    unsigned outputSize;
    unsigned unk28, unk2c, unk30;
    unsigned variableCount;
    bool partial;
    CompileVariables* ComputeValue_005a8080();
    bool ScriptCompileData_GetReferencedObject(CompileWord*, bool, bool);
    ScriptReferencedObject* GetNodeValue(unsigned);
};

struct CompileLine
{
    unsigned lineNumber;
    char input[512];
    unsigned inputSize, inputOffset;
    char output[512];
    unsigned outputSize;
    int kind;
    unsigned refIndex;
    unsigned error;
    CompileLine();
};

typedef bool(__cdecl* CompileCallback)(unsigned short, void*, CompileLine*, CompileState*);

struct CompileCommand
{
    const char* name;
    const char* alias;
    unsigned short opcode, unk0a;
    unsigned unk0c;
    bool requiresReference;
    unsigned char unk11;
    unsigned short parameterCount;
    void* parameters;
    unsigned unk18;
    CompileCallback compile;
    unsigned unk20, unk24;
};

extern CompileCommand DAT_0118e2f0[];

extern unsigned DAT_011cadbc;

extern const char* DAT_0118c594[];

int ScriptCompiler_GetNextWord(CompileState*, CompileWord*, char*, unsigned*, unsigned char, int);

int BSstricmp(const char*, const char*);

void ReportScriptError(CompileState*, const char*, ...);

int FUN_00406d30(char*, unsigned, const char*);

void* FUN_00401460(void*, const void*, unsigned);

void* ourMemset(void*, int, unsigned);

bool ScriptCompiler_IsValidVariableName(const char*);

bool ScriptCompiler_CheckParentheses(const char*, unsigned);

void ScriptCompiler_SkipWhiteSpace(const char*, unsigned*);

CompileCommand* FUN_005b1120(int);

struct CompileOperand
{
    unsigned kind, unk04, unk08, unk0c;
};

struct CompileOperators
{
    unsigned values[64];
    int top;
};

struct CompileOperands
{
    CompileOperand values[32];
    int top;
};

struct CompileExpression
{
    int error;
    char output[1024];
    CompileOperators operators;
    CompileOperands operands;
    CompileExpression();
    ~CompileExpression();
    char* ConvertToPostfix(char*, int*);
    int Modelloader_GetReferencedThis();
};

struct LineCompiler
{
    int ScriptCompiler_CountBlockLines(CompileState*, CompileLine*);
    int ScriptCompiler_CountIfLines(CompileState*, CompileLine*);
    int ScriptCompiler_ReplaceVariablesAndFunctions(CompileState*, CompileLine*, const char*, char*);
    bool ScriptCompiler_CompileLine(CompileScript*, CompileState*, CompileLine*);
};

// @fnv 0x005aff40
// src: COMMUNITY 0x005aff40
// pdb: ScriptCompiler::CompileLine | TESScriptCompiler.obj | retail, upstream name already matches, high
// pdb params: apScript, apCompileData, apScriptLine
// pdb locals: ce, postfix, uiOffset, scriptWord, bFound, n, pFunctionDef, lines, pParameterDef, tempLine, sCount, pCompile, ibeginEndJumpDelta, pvarname, pNewVariable, expLine, scriptWord2, uiSize, pRefTest, pScriptRefObj
bool LineCompiler::ScriptCompiler_CompileLine(CompileScript* apScript, CompileState* apCompileData, CompileLine* apScriptLine)
{
    CompileWord word6;

    unsigned offset_2 = 0;

    CompileExpression expression5;
    char* expressionCodeVal9;

    apScriptLine->outputSize = 0;
    apScriptLine->inputOffset = 0;

    switch (apScriptLine->kind)
    {
    case 29:
        break;

    case 16:
        if (ScriptCompiler_GetNextWord(apCompileData, &word6, apScriptLine->input, &apScriptLine->inputOffset, 0, 0))
        {
            int i;
            bool found7 = false;

            for (i = 0; i < 38; ++i)
            {
                if (!BSstricmp(DAT_0118e2f0[i].name, word6.text) || !BSstricmp(DAT_0118e2f0[i].alias, word6.text))
                {
                    found7 = true;
                    break;
                }
            }
            if (found7)
            {
                *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(DAT_0118e2f0[i].opcode);
                apScriptLine->outputSize += 2;

                int length = 0;
                length = ScriptCompiler_CountBlockLines(apCompileData, apScriptLine);

                if (length == -1)
                {
                    ReportScriptError(apCompileData, "Mismatched begin/end block.");
                    return false;
                }
                *(unsigned*)(apScriptLine->output + apScriptLine->outputSize) = 0x55555555;

                DAT_011cadbc = apCompileData->outputSize + apScriptLine->outputSize;

                apScriptLine->outputSize += 4;

                CompileCommand* command = &DAT_0118e2f0[i];

                if (command)
                {
                    if (command->requiresReference && apCompileData->partial)
                    {
                        ReportScriptError(apCompileData, "Invalid block type for quest script.");
                        return false;
                    }
                    if (command && command->parameters)
                    {
                        void* blockParameters0 = command->parameters;

                        CompileCallback blockCallback11 = command->compile;

                        unsigned short count = command->parameterCount;

                        CompileLine parametersLine;

                        FUN_00406d30(parametersLine.input, 512, apScriptLine->input + apScriptLine->inputOffset);

                        parametersLine.outputSize = 0;

                        parametersLine.lineNumber = apScriptLine->lineNumber;

                        if (!blockCallback11(count, blockParameters0, &parametersLine, apCompileData))
                            return false;

                        FUN_00401460(apScriptLine->output + apScriptLine->outputSize, parametersLine.output, parametersLine.outputSize);

                        apScriptLine->outputSize += parametersLine.outputSize;
                    }
                }
                if (DAT_011cadbc)
                {
                    *(unsigned*)(apScriptLine->output + (DAT_011cadbc - apCompileData->outputSize)) =
                        apCompileData->outputSize + apScriptLine->outputSize;
                }
            }
            else
            {
                ReportScriptError(apCompileData, "Syntax Error.  Invalid block type in 'begin' command.");
                return false;
            }
        }
        else
        {
            ReportScriptError(apCompileData, "Syntax Error.  Missing block type in 'begin' command.");
            return false;
        }
        break;

    case 17:
        if (DAT_011cadbc)
        {
            unsigned start6 = *(unsigned*)(apCompileData->output + (DAT_011cadbc + 4));

            *(unsigned*)(apCompileData->output + (DAT_011cadbc + 4)) = apCompileData->outputSize - start6;
            DAT_011cadbc = 0;
        }
        else
        {
            ReportScriptError(apCompileData, "Syntax Error.  Failed to store the 'begin' 'end' jump block.");
            return false;
        }
    case 30:
        break;
    case 18:
    case 19:
    case 20:
    case 31:
        if (ScriptCompiler_GetNextWord(apCompileData, &word6, apScriptLine->input, &apScriptLine->inputOffset, 0, 0))
        {
            const char* variableName1 = word6.text;

            if (!ScriptCompiler_IsValidVariableName(variableName1))
            {
                ReportScriptError(apCompileData, "Invalid variable name '%s'.\r\nCompiled script not saved!", variableName1);
                return false;
            }
            CompileVariable* variableInfo0 = new CompileVariable;

            variableInfo0->integer = (apScriptLine->kind != 20 && apScriptLine->kind != 31) ? 1 : 0;

            variableInfo0->name.Set(variableName1, 0);

            if (apScript->FUN_005ac6a0(variableInfo0->name.String(), variableInfo0))
            {
            }
            else
            {
                apCompileData->variableCount++;
                variableInfo0->id = apCompileData->variableCount;
            }
            apCompileData->ComputeValue_005a8080()->AddTail(variableInfo0);

            if (apScriptLine->kind == 31)
            {
                word6.variableID = variableInfo0->id;
                apCompileData->ScriptCompileData_GetReferencedObject(&word6, true, false);
            }
        }
        else
        {
            ReportScriptError(apCompileData, "Missing variable name in variable declaration.");
            return false;
        }
        break;

    case 21: {
        if (!ScriptCompiler_GetNextWord(apCompileData, &word6, apScriptLine->input, &apScriptLine->inputOffset, 1, 1))
        {
            ReportScriptError(apCompileData, "Missing variable name in set command.");
            return false;
        }
        if (!word6.variableType || word6.variableType == 'X')
        {
            ReportScriptError(apCompileData, "Unknown variable '%s'.", &word6);
            return false;
        }
        CompileWord setWord20;

        ScriptCompiler_GetNextWord(apCompileData, &setWord20, apScriptLine->input, &apScriptLine->inputOffset, 0, 0);

        if (tolower(setWord20.text[0]) != 't' || tolower(setWord20.text[1]) != 'o')
        {
            ReportScriptError(apCompileData, "Syntax Error.  Missing \"to\" in set command.");
            return false;
        }
        if (word6.refIndex > 0)
        {
            if (word6.variableType == 'G')
            {
                apScriptLine->output[apScriptLine->outputSize] = 'G';
                apScriptLine->outputSize++;
            }
            else
            {
                apScriptLine->output[apScriptLine->outputSize] = 'r';
                apScriptLine->outputSize++;
            }
            *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(word6.refIndex);
            apScriptLine->outputSize += 2;
        }
        if (word6.variableID > 0)
        {
            apScriptLine->output[apScriptLine->outputSize] = word6.variableType;
            apScriptLine->outputSize++;
            *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(word6.variableID);
            apScriptLine->outputSize += 2;
        }
        if (!ScriptCompiler_CheckParentheses(apScriptLine->input, apScriptLine->inputOffset))
        {
            ReportScriptError(apCompileData, "Syntax Error.  Mismatched parentheses in set command.");
            return false;
        }
        ScriptCompiler_SkipWhiteSpace(apScriptLine->input, &apScriptLine->inputOffset);

        char setText20[512];
        ourMemset(setText20, 0, 512);

        int setLength1 = 0;
        setLength1 = ScriptCompiler_ReplaceVariablesAndFunctions(apCompileData, apScriptLine, apScriptLine->input + apScriptLine->inputOffset, setText20);

        if (!setLength1)
        {
            ReportScriptError(apCompileData, missingSetExpression);
            return false;
        }
        expressionCodeVal9 = expression5.ConvertToPostfix(setText20, &setLength1);

        if (expression5.Modelloader_GetReferencedThis())
        {
            ReportScriptError(apCompileData, "InfixToPostfix Error.");
            return false;
        }
        *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(setLength1);
        apScriptLine->outputSize += 2;

        FUN_00401460(apScriptLine->output + apScriptLine->outputSize, expressionCodeVal9, setLength1);
        apScriptLine->outputSize += setLength1;
    }
    break;

    case 22:
    case 24: {
        offset_2 = 0;

        if (!ScriptCompiler_CheckParentheses(apScriptLine->input, offset_2))
        {
            ReportScriptError(apCompileData, "Mismatched parentheses.");
            return false;
        }
        ScriptCompiler_SkipWhiteSpace(apScriptLine->input, &offset_2);

        char branchText7[512];
        ourMemset(branchText7, 0, 512);

        int branchLength10 = 0;
        branchLength10 = ScriptCompiler_ReplaceVariablesAndFunctions(apCompileData, apScriptLine, apScriptLine->input + offset_2, branchText7);

        if (!branchLength10)
            return false;

        expressionCodeVal9 = expression5.ConvertToPostfix(branchText7, &branchLength10);

        if (expression5.Modelloader_GetReferencedThis())
        {
            ReportScriptError(apCompileData, DAT_0118c594[expression5.Modelloader_GetReferencedThis()]);
            return false;
        }
        int branchJump11 = ScriptCompiler_CountIfLines(apCompileData, apScriptLine);

        if (branchJump11 == -1)
        {
            ReportScriptError(apCompileData, "Mismatched if/then/else block.");
            return false;
        }
        *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(branchJump11);
        apScriptLine->outputSize += 2;
        *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(branchLength10);
        apScriptLine->outputSize += 2;

        FUN_00401460(apScriptLine->output + apScriptLine->outputSize, expressionCodeVal9, branchLength10);
        apScriptLine->outputSize += branchLength10;
    }
    break;

    case 23: {
        int jump = 0;
        jump = ScriptCompiler_CountIfLines(apCompileData, apScriptLine);

        if (jump == -1)
        {
            ReportScriptError(apCompileData, "Mismatched if/then/else block.");
            return false;
        }
        *(unsigned short*)(apScriptLine->output + apScriptLine->outputSize) = (unsigned short)(jump);
        apScriptLine->outputSize += 2;
    }
    break;

    case 28:
        break;

    case 25:
        break;

    default: {
        CompileCommand* command = FUN_005b1120(apScriptLine->kind);

        if (!command)
        {
            ReportScriptError(apCompileData, "Unknown function code %d.", apScriptLine->kind);
            return false;
        }
        if (command)
        {
            if (command->requiresReference && apCompileData->partial && !apScriptLine->refIndex)
            {
                ReportScriptError(apCompileData, "Reference function requires explicit reference in quest script.");
                return false;
            }
            if (command->requiresReference && apScriptLine->refIndex > 0)
            {
                ScriptReferencedObject* ref2 = apCompileData->GetNodeValue(apScriptLine->refIndex);

                TESObjectREFR* object = dynamic_cast<TESObjectREFR*>(ref2->pForm);

                if (!object && !ref2->m_uiFormID)
                {
                    ReportScriptError(
                        apCompileData,
                        "Syntax error.  Invalid reference '%s' (only object references and reference variables are allowed in this context).",
                        reinterpret_cast<::BSStringT<char>*>(&ref2->m_kFormName)->String());
                    return false;
                }
            }
            if (command->parameters)
            {
                void* commandParameters0 = command->parameters;

                CompileCallback commandCallback1 = command->compile;

                unsigned short commandCount3 = command->parameterCount;

                if (!commandCallback1(commandCount3, commandParameters0, apScriptLine, apCompileData))
                    return false;
            }
        }
    }
    break;
    }
    switch (apScriptLine->kind)
    {
    case 18:
    case 19:
    case 20:
    case 31:
        break;

    default:
        if (apCompileData->outputSize + apScriptLine->outputSize + 10 >= 0x4000)
        {
            ReportScriptError(apCompileData, "MAX_SCRIPT_SIZE exceeded.\r\nCompiled script not saved!");
            return false;
        }
        if (apScriptLine->refIndex > 0)
        {
            *(unsigned short*)(apCompileData->output + apCompileData->outputSize) = (unsigned short)(28);
            apCompileData->outputSize += 2;
            *(unsigned short*)(apCompileData->output + apCompileData->outputSize) = (unsigned short)(apScriptLine->refIndex);
            apCompileData->outputSize += 2;
        }
        *(unsigned short*)(apCompileData->output + apCompileData->outputSize) = (unsigned short)(apScriptLine->kind);
        apCompileData->outputSize += 2;
        *(unsigned short*)(apCompileData->output + apCompileData->outputSize) = (unsigned short)(apScriptLine->outputSize);
        apCompileData->outputSize += 2;

        FUN_00401460(apCompileData->output + apCompileData->outputSize, apScriptLine->output, apScriptLine->outputSize);
        apCompileData->outputSize += apScriptLine->outputSize;
    }
    return true;
}
