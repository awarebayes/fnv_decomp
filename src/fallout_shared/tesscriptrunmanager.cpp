#include "../../include/ScriptExecutor.h"
// fallout_shared/tesscriptrunmanager.cpp: ScriptRunner, ScriptExecutor (11 functions)

#include "../../include/BSStringT.h"
#include "../../include/ScriptRunner.h"
#include "../../include/Script.h"
#include "../../include/ScriptReferencedObject.h"
#include "../../include/TESObjectREFR.h"
#include "../../include/TESBoundObject.h"
#include "../../include/TESObjectCELL.h"
#include "../../include/ScriptLocals.h"
#include "../../include/TESQuest.h"
#include "../../include/TESGlobal.h"
#include "game_constants.h"
#include <new>



ScriptFunctionInfo* GetFunctionByOpcode(SCRIPT_OUTPUT output);
int Warning(const char* format, ...);
bool ExtractArgs(const ScriptParameter* parameters, const char* data, unsigned int* offset,
                 TESObjectREFR* reference, TESObjectREFR* container, Script* script, ScriptLocals* locals, ...);
extern "C" const char g_nestedScriptRunFormat[];
extern "C" char g_scriptRunLock;
extern int __cdecl InitScriptRunLock(void* lock);
extern void __cdecl FinishScriptRunLock(void* lock);
extern ScriptFunctionInfo g_scriptBlocks[kScriptBlockCount];
extern const char* g_expressionErrorStrings[];
extern void DebugLog(const char* format, ...);
extern bool IsScriptDebuggingEnabled();
extern const char* PrepareScriptMessage(const char* message, bool localized);

// Reads the output opcode, optional reference ID and line size from the script data at offset, resolving the reference and reporting an error if it is unknown.
// @fnv 0x005e13a0
// src: XBOX_RETAIL_PDB 0x005e13a0
// pdb: ScriptRunner::GetNextLine | TESScriptRunManager.obj | retail, call graph (shared callers), high
// pdb params: apScript, aeNextExp, auiDataOffset, aiLineSize, appRefObject, abSkip
// pdb locals: uiRefIndex, bIsRefVar, pScriptRefObj, pFunctionDef
bool ScriptRunner::GetNextLine(Script* apScript, SCRIPT_OUTPUT& aeNextExp, unsigned int& auiDataOffset, int& aiLineSize,
                               TESObjectREFR** appRefObject, bool abSkip)
{
    aiLineSize = 0;
    aeNextExp = static_cast<SCRIPT_OUTPUT>(*reinterpret_cast<short*>(apScript->GetData() + auiDataOffset));
    auiDataOffset += 2;
    if (aeNextExp == SCRIPT_OUTPUT_FLOW_REFERENCE_FUNCTION)
    {
        bool isVariable = false;
        unsigned int referenceID = *reinterpret_cast<short*>(apScript->GetData() + auiDataOffset);
        auiDataOffset += 2;
        if (!abSkip)
        {
            ScriptReferencedObject* object = apScript->GetReferencedObject(referenceID, m_currentVars);
            if (object)
            {
                isVariable = object->m_uiFormID > 0;
                if (object->pForm)
                {
                    *appRefObject = dynamic_cast<TESObjectREFR*>(object->pForm);
                }
            }
        }
        aeNextExp = static_cast<SCRIPT_OUTPUT>(*reinterpret_cast<short*>(apScript->GetData() + auiDataOffset));
        auiDataOffset += 2;
        if (!abSkip && !*appRefObject && !isVariable)
        {
            ScriptFunctionInfo* function = GetFunctionByOpcode(aeNextExp);
            if (function)
            {
                SetErrorMessage(apScript,
                                "Unknown reference object for function '%s'.  Execution of script '%s' stopped.",
                                function->pFunctionName, apScript->GetName());
            }
            else
            {
                SetErrorMessage(apScript, "Unknown reference object.  Execution of script '%s' stopped.",
                                apScript->GetName());
            }
            return false;
        }
    }
    else
    {
        *appRefObject = 0;
    }
    aiLineSize = *reinterpret_cast<short*>(apScript->GetData() + auiDataOffset);
    auiDataOffset += 2;
    return true;
}

// Executes one script command at the given offset, handling BEGIN blocks, SET, IF/ELSEIF/ELSE/ENDIF and function calls, updating blockSize and lineOffset.
// @fnv 0x005e1550
// src: XBOX_RETAIL_PDB 0x005e1550
// pdb: ScriptRunner::EvaluateLine | TESScriptRunManager.obj | retail, unique string literal, high
// pdb params: apScript, aeExp, apFunctionOwner, aiLineSize, auiDataOffset, aiSkipLines, auiCurrentLine, abEndian, abActionInit
// pdb locals: pCompiledData, uiCurrentOffset, pParameterDef, ibeginEndJumpDelta, pExecute, eBlockType, fVal, pFunctionDef, pScriptLocals, ce, cType, sVariableID, pRefObj, val, sSize, sRefObjectIndex, pRefForm, pQuest, pref, iBlockLines, uiSize, message, sVarCount, n ...
bool ScriptRunner::EvaluateLine(Script* apScript, const SCRIPT_OUTPUT aeExp, TESObjectREFR* apFunctionOwner, int& aiLineSize,
                              const unsigned int auiDataOffset, unsigned int& aiSkipLines, const unsigned int auiCurrentLine,
                              bool abEndian, bool abActionInit)
{
    unsigned int offset = auiDataOffset;
    const char* data = apScript->GetData();
    if (aeExp == SCRIPT_OUTPUT_FLOW_BEGIN)
    {
        int blockID = *reinterpret_cast<const short*>(data + offset);
        offset += 2;
        if (blockID < 0 || blockID >= kScriptBlockCount)
        {
            Warning("Invalid script block for BEGIN on line %i of script '%s' (%08X).", auiCurrentLine, apScript->GetName(),
                    apScript->GetFormID());
            return false;
        }
        int skipSize = *reinterpret_cast<const int*>(data + offset);
        offset += 4;
        const ScriptParameter* blockParams = g_scriptBlocks[blockID].pParameters;
        FNVScriptExecuteFunction blockExecute = g_scriptBlocks[blockID].m_pfnExecute;
        if (abEndian)
        {
            if (blockParams && aiLineSize)
                ExtractArgs(blockParams, data, &offset, apFunctionOwner, m_currentContainer, apScript, m_currentVars);
        }
        else
        {
            if (g_scriptBlocks[blockID].m_bRequiresReference && !apFunctionOwner)
            {
                Warning("SCRIPTS: Script '%s', line %d: Null for a required ref pointer.", apScript->GetName(),
                        auiCurrentLine);
                return false;
            }
            double result;
            if (!blockExecute ||
                !blockExecute(blockParams, data, apFunctionOwner, m_currentContainer, apScript, m_currentVars, result, offset))
            {
                Warning("SCRIPTS: Script '%s', line %d: Error Executing line (Offset %d).", apScript->GetName(),
                        auiCurrentLine, offset);
                return false;
            }
            if (!result || abActionInit)
                aiLineSize += skipSize;
        }
    }
    else if (!abActionInit)
    {
        const ScriptFunctionInfo* function;
        switch (aeExp)
        {
        case SCRIPT_OUTPUT_FLOW_SCRIPTNAME:
            break;
        case SCRIPT_OUTPUT_FLOW_BEGIN:
            break;
        case SCRIPT_OUTPUT_FLOW_END:
            break;
        case SCRIPT_OUTPUT_FLOW_SET: {
            char varType = data[offset];
            offset++;
            short variableID = 0;
            ScriptLocals* currentPScriptVariables = m_currentVars;
            ScriptReferencedObject* referencedObject = 0;
            switch (varType)
            {
            case kScriptGlobalVariable:
            case kScriptReferenceVariable: {
                short referenceID = *reinterpret_cast<const short*>(data + offset);
                offset += 2;
                if (varType == kScriptReferenceVariable)
                {
                    varType = data[offset];
                    offset++;
                }
                referencedObject = apScript->GetReferencedObject(referenceID, m_currentVars);
                if (!abEndian)
                {
                    if (varType != kScriptReferenceVariable && referencedObject && referencedObject->pForm)
                    {
                        TESObjectREFR* object = dynamic_cast<TESObjectREFR*>(referencedObject->pForm);
                        if (object)
                            currentPScriptVariables = object->GetReferenceLocals();
                        else
                        {
                            TESQuest* quest = dynamic_cast<TESQuest*>(referencedObject->pForm);
                            if (quest)
                                currentPScriptVariables = quest->GetScriptLocalData();
                        }
                        if (!currentPScriptVariables)
                        {
                            Warning("Missing script variables on object for SET on line %i of script '%s' (%08X).",
                                    auiCurrentLine, apScript->GetName(), apScript->GetFormID());
                            return false;
                        }
                    }
                    else
                    {
                        Warning("Missing ref object in SET on line %i of script '%s' (%08X).", auiCurrentLine,
                                apScript->GetName(), apScript->GetFormID());
                        return false;
                    }
                }
            }
            break;
            }
            switch (varType)
            {
            case kScriptGlobalVariable:
                break;
            default: {
                variableID = *reinterpret_cast<const short*>(data + offset);
                offset += 2;
            }
            break;
            }
            short expressionSize = *reinterpret_cast<const short*>(data + offset);
            offset += 2;
            CExpression variableExpression;
            double expressionValue = variableExpression.Eval(data + offset, apFunctionOwner, m_currentContainer, apScript,
                                                             m_currentVars, expressionSize, abEndian);
            offset += expressionSize;
            if (variableExpression.ExpressionError())
            {
                Warning("SCRIPTS: Script '%s', line %d: Set expression returned an error: %s.\r\n", apScript->GetName(),
                        auiCurrentLine, g_expressionErrorStrings[variableExpression.ExpressionError()]);
                apScript->GetHeader()->dataSize = 0;
                return false;
            }
            if (!abEndian)
            {
                if (varType == kScriptGlobalVariable)
                {
                    if (referencedObject && referencedObject->pForm)
                    {
                        if (!abEndian)
                        {
                            static_cast<TESGlobal*>(referencedObject->pForm)
                                ->SetValue(static_cast<float>(expressionValue));
                            if (IsScriptDebuggingEnabled())
                                DebugLog("set global >> %0.2f", static_cast<float>(expressionValue));
                        }
                    }
                    else
                    {
                        Warning("Invalid variable type for SET on line %i of script '%s' (%08X).", auiCurrentLine,
                                apScript->GetName(), apScript->GetFormID());
                        return false;
                    }
                }
                else
                {
                    if (currentPScriptVariables)
                    {
                        switch (varType)
                        {
                        case kScriptLongVariable:
                        case kScriptShortVariable:
                            currentPScriptVariables->SetVariable(variableID, static_cast<int>(expressionValue));
                            if (IsScriptDebuggingEnabled())
                                DebugLog("set %s >> %i",
                                         currentPScriptVariables->m_pMasterScript->GetVariableName(variableID),
                                         static_cast<int>(expressionValue));
                            break;
                        case kScriptFloatVariable:
                            currentPScriptVariables->SetVariable(variableID, expressionValue);
                            if (IsScriptDebuggingEnabled())
                                DebugLog("set %s >> %.2f",
                                         currentPScriptVariables->m_pMasterScript->GetVariableName(variableID),
                                         expressionValue);
                            break;
                        }
                        if ((referencedObject && referencedObject->pForm) || apFunctionOwner)
                        {
                            TESObjectREFR* changedReference = 0;
                            if (referencedObject)
                            {
                                if (referencedObject->pForm && referencedObject->pForm->GetIsReference())
                                    changedReference = static_cast<TESObjectREFR*>(referencedObject->pForm);
                            }
                            else
                                changedReference = apFunctionOwner;
                            if (changedReference && !changedReference->IsTemporary())
                            {
                                if (!changedReference->GetParentCell() ||
                                    !changedReference->GetParentCell()->IsOutdoorCell())
                                    changedReference->MarkAsModified(TESObjectREFR::CHANGE_SCRIPT_VARIABLES);
                            }
                        }
                    }
                    else
                    {
                        Warning("Missing scriptvariables for object in SET on line %i of script '%s' (%08X).",
                                auiCurrentLine, apScript->GetName(), apScript->GetFormID());
                        return false;
                    }
                }
            }
            break;
        }
        case SCRIPT_OUTPUT_FLOW_IF:
            if (m_ifFlags[m_ifNested])
                m_ifNested++;
            m_ifFlags[m_ifNested] = kScriptIfActiveFlag;
        case SCRIPT_OUTPUT_FLOW_ELSEIF: {
            short skipCount = *reinterpret_cast<const short*>(data + offset);
            offset += 2;
            short conditionSize = *reinterpret_cast<const short*>(data + offset);
            offset += 2;
            CExpression condition;
            if (abEndian)
            {
                condition.Eval(data + offset, apFunctionOwner, m_currentContainer, apScript, m_currentVars, conditionSize, abEndian);
                m_ifFlags[m_ifNested] |= kScriptIfConditionPassedFlag;
            }
            else
            {
                if (!(m_ifFlags[m_ifNested] & kScriptIfConditionPassedFlag) &&
                    condition.Eval(data + offset, apFunctionOwner, m_currentContainer, apScript, m_currentVars, conditionSize,
                                   abEndian))
                    m_ifFlags[m_ifNested] |= kScriptIfConditionPassedFlag;
                else
                    aiSkipLines += skipCount;
            }
            if (condition.ExpressionError())
            {
                SetError(apScript, SCRIPT_ERROR_ERROR_SCRIPT_EXPRESSION);
                Warning("SCRIPTS: Script '%s', line %d: failed to evaluate expression.", apScript->GetName(), auiCurrentLine);
                apScript->GetHeader()->dataSize = 0;
                m_expError = condition.ExpressionError();
                return false;
            }
            offset += conditionSize;
            break;
        }
        case SCRIPT_OUTPUT_FLOW_ELSE: {
            short skipCount = *reinterpret_cast<const short*>(data + offset);
            offset += 2;
            if (!(m_ifFlags[m_ifNested] & kScriptIfConditionPassedFlag))
                m_ifFlags[m_ifNested] |= kScriptIfConditionPassedFlag;
            else if (!abEndian)
                aiSkipLines += skipCount;
            break;
        }
        case SCRIPT_OUTPUT_FLOW_ENDIF:
            m_ifFlags[m_ifNested] = 0;
            if (m_ifNested)
                m_ifNested--;
            break;
        case SCRIPT_OUTPUT_FLOW_REFERENCE_FUNCTION:
            break;
        case SCRIPT_OUTPUT_FLOW_RETURN:
            if (abEndian)
                break;
        default: {
            function = GetFunctionByOpcode(aeExp);
            if (!function)
            {
                BSStringT<char> message;
                message.PrintfString("SCRIPTS: Unable to find function definition for command %d in script '%s'.", aeExp,
                                apScript->GetName());
                if (IsScriptDebuggingEnabled())
                    DebugLog(PrepareScriptMessage(message.String(), false));
                else
                    Warning(PrepareScriptMessage(message.String(), false));
                return false;
            }
            else
            {
                const ScriptParameter* blockParams = function->pParameters;
                if (function->m_bRequiresReference && !apFunctionOwner && !abEndian)
                {
                    BSStringT<char> message;
                    message.PrintfString("SCRIPTS: Script '%s', line %d: Function '%s' requires a reference.",
                                    apScript->GetName(), auiCurrentLine, function->pFunctionName);
                    if (IsScriptDebuggingEnabled())
                        DebugLog(PrepareScriptMessage(message.String(), false));
                    else
                        Warning(PrepareScriptMessage(message.String(), false));
                    return false;
                }
                if (abEndian)
                {
                    if (blockParams && aiLineSize)
                    {
                        ExtractArgs(blockParams, data, &offset, apFunctionOwner, m_currentContainer, apScript, m_currentVars);
                        switch (aeExp)
                        {
                        case SCRIPT_OUTPUT_FUNCTION_MESSAGE: {
                            short valueCount = *reinterpret_cast<const short*>(data + offset);
                            offset += 2;
                            for (short valueIndex = 0; valueIndex < valueCount; valueIndex++)
                            {
                                double expressionValue;
                                Script::GetValue(expressionValue, data, offset, apFunctionOwner, m_currentContainer, apScript,
                                                 m_currentVars, true);
                            }
                            offset += 4;
                            break;
                        }
                        }
                    }
                }
                else
                {
                    FNVScriptExecuteFunction blockExecute = function->m_pfnExecute;
                    if (function->m_bSetsExecutionFlag)
                        m_bStopObjectScriptExecution = true;
                    double result;
                    if (!blockExecute || !blockExecute(blockParams, data, apFunctionOwner, m_currentContainer, apScript,
                                                       m_currentVars, result, offset))
                    {
                        Warning("Failed to execute function '%s' on line %i of script '%s' (%08X).",
                                function->pFunctionName, auiCurrentLine, apScript->GetName(), apScript->GetFormID());
                        return false;
                    }
                }
            }
        }
        }
    }
    return true;
}

// Returns a pointer to the function-local static instance of the class.
// @fnv 0x005e24d0
// pdb: ScriptRunManager::Instance | TESScriptRunManager.obj | retail, call graph, high
ScriptRunManager* ScriptRunManager::Instance()
{
    static ScriptRunManager manager;
    return &manager;
}

// Acquires the script run lock, runs the script through a shared or temporary ScriptRunner, then releases the lock and returns the result.
// @fnv 0x005e2590
// pdb: ScriptRunManager::Run | TESScriptRunManager.obj | retail, call graph, high
// pdb params: apScript, apOwnerObject, apOwnerVars, apOwnerContainer, abIsPartialScript, abScriptEffectStart, abScriptEffectFinish, afSecondsElapsed
// pdb locals: bStopObjectScriptExecution, poldcurrent, pTempScriptRunner
bool ScriptRunManager::Run(Script* apScript, TESObjectREFR* apOwnerObject, ScriptLocals* apOwnerVars, TESObjectREFR* apOwnerContainer,
                         bool abIsPartialScript, bool abScriptEffectStart, bool abScriptEffectFinish, float afSecondsElapsed)
{
    bool result = false;
    if (InitScriptRunLock(&g_scriptRunLock) == 1)
    {
        if (!m_pScriptRunner)
            m_pScriptRunner = new ScriptRunner;
        pCurrentRunner = m_pScriptRunner;
        result = m_pScriptRunner->Run(apScript, apOwnerObject, apOwnerVars, apOwnerContainer, abIsPartialScript, abScriptEffectStart, abScriptEffectFinish, afSecondsElapsed);
        pCurrentRunner = 0;
    }
    else
    {
        ScriptRunner* runner = new ScriptRunner;
        ScriptRunner* previousRunner = pCurrentRunner;
        pCurrentRunner = runner;
        result = runner->Run(apScript, apOwnerObject, apOwnerVars, apOwnerContainer, abIsPartialScript, abScriptEffectStart, abScriptEffectFinish, afSecondsElapsed);
        pCurrentRunner = previousRunner;
        delete runner;
    }
    FinishScriptRunLock(&g_scriptRunLock);
    return result;
}

void* __cdecl memset(void*, int, size_t);

// Initializes the runner's script state and execution stacks.
// @fnv 0x005e0c60
// pdb: ScriptRunner::ScriptRunner | TESScriptRunManager.obj | retail, upstream name already matches, high
ScriptRunner::ScriptRunner()
{
    m_ifNested = 0;
    m_whileNested = 0;
    m_error = SCRIPT_ERROR_ERROR_SCRIPT_NOERROR;
    m_expError = EXPRESSION_ERROR_ERROR_EXPRESSION_NOERROR;
    memset(m_ifFlags, 0, sizeof(m_ifFlags));
    memset(m_whileFlags, 0, sizeof(m_whileFlags));
    memset(m_whileOffset, 0, sizeof(m_whileOffset));
    m_currentContainer = 0;
    m_currentObject = 0;
    m_currentVars = 0;
    m_currentInfo = 0;
    m_global = 0;
    m_scriptRunning = 0;
    m_bScriptErrorFound = false;
    m_bStopObjectScriptExecution = false;
}

struct TLSData_005e23f0
{
    char pad[0x268];
    bool bVal;
};

static __declspec(thread) TLSData_005e23f0 s_tlsData_005e23f0;

// @fnv 0x005e23f0
bool __cdecl GetThreadLocalValue()
{
    return s_tlsData_005e23f0.bVal;
}

int __cdecl BSvsprintf(char* buffer, char const* format, char* varArgs);

// Formats a message into a local buffer, logs it via DebugLog or Warning, then zeroes the script header size and flags the runner as failed.
// @fnv 0x005e2410
void ScriptRunner::SetErrorMessage(Script* apScript, char* apFormat, ...)
{
    char buffer[260];
    char* varArgs;

    varArgs = (char*)(&apFormat + 1);
    BSvsprintf(buffer, apFormat, varArgs);
    varArgs = 0;

    if (IsScriptDebuggingEnabled())
    {
        DebugLog(PrepareScriptMessage(buffer, false));
    }
    else
    {
        Warning(PrepareScriptMessage(buffer, false));
    }

    apScript->GetHeader()->dataSize = 0;
    m_bScriptErrorFound = true;
}

// @fnv 0x005e24b0
void ScriptRunner::SetError(Script* script, SCRIPT_ERROR error)
{
    m_error = error;
}

extern void __cdecl _Deallocate(void*);

// Releases the reusable runner and clears the active runner.
// @fnv 0x005e2550
// pdb: ScriptRunManager::~ScriptRunManager | TESScriptRunManager.obj | debug, object-file gap fill, medium
ScriptRunManager::~ScriptRunManager()
{
    void* tmp;
    if (m_pScriptRunner)
    {
        tmp = m_pScriptRunner;
        _Deallocate(tmp);
        m_pScriptRunner = 0;
    }
    pCurrentRunner = 0;
}

// The shared runner header does not yet declare the active-state query.
extern bool __fastcall IsScriptRunnerActive(ScriptRunner* runner);

// Creates a ScriptRunner if needed, then calls RunScript on either it or a temporary runner.
// @fnv 0x005e2730
// src: XBOX_PDB 0x005e2730
// pdb: ScriptRunManager::InitActionList | TESScriptRunManager.obj | retail, upstream name already matches, high, not renamed: the link is the existing name itself, so its spelling stays
// pdb params: apScript, apOwnerObject, apOwnerVars
// pdb locals: pTempScriptRunner
bool ScriptRunManager::ScriptRunManager_InitActionList(Script* apScript, TESObjectREFR* apOwnerObject, ScriptLocals* apOwnerVars)
{
    if (!m_pScriptRunner)
        m_pScriptRunner = new ScriptRunner;
    if (IsScriptRunnerActive(m_pScriptRunner))
    {
        ScriptRunner* pTempScriptRunner = new ScriptRunner;
        pTempScriptRunner->RunScript(apScript, apOwnerObject, apOwnerVars);
        delete pTempScriptRunner;
    }
    else
    {
        m_pScriptRunner->RunScript(apScript, apOwnerObject, apOwnerVars);
    }
    return true;
}

// Parses script output blocks, updates m_pScriptObject and m_pScriptLocals, and returns true after the loop.
// @fnv 0x005e1160
// pdb: ScriptRunner::InitActionList | TESScriptRunManager.obj | retail, call graph, high
// pdb params: apScript, apOwnerObject, apOwnerVars
// pdb locals: ce, iCurrentLine, uiDataOffset, bbadscript, eNextExp, pRefObject, iLineSize, iSkipLines
bool ScriptRunner::RunScript(Script* apScript, TESObjectREFR* apOwnerObject, ScriptLocals* apOwnerVars)
{
    if (!apScript || !apScript->GetInitialized())
        return false;
    if (m_scriptRunning)
    {
        Warning(g_nestedScriptRunFormat, m_scriptRunning->GetName(), apScript->GetName());
        return false;
    }
    m_scriptRunning = apScript;
    CExpression j;
    SCRIPT_OUTPUT k = (SCRIPT_OUTPUT)0xffff;
    unsigned int c = 0;
    if (apOwnerObject)
        m_currentObject = apOwnerObject->GetObjectReference();
    else
        m_currentObject = 0;
    m_currentVars = apOwnerVars;
    int q = 0;
    TESObjectREFR* b = 0;
    unsigned int f = 0;
    bool l = false;
    unsigned int e = 0;
    while (c < apScript->GetHeader()->dataSize)
    {
        e += 1;
        m_global = 0;
        q = 0;
        if (!GetNextLine(apScript, k, c, q, &b, f != 0))
        {
            apScript->GetHeader()->dataSize = 0;
            break;
        }
        if (f != 0)
        {
            f -= 1;
        }
        else
        {
            if (!b)
                b = apOwnerObject;
            if (!EvaluateLine(apScript, k, b, q, c, f, e, false, true))
            {
                l = true;
                break;
            }
        }
        c += q;
    }
    m_scriptRunning = 0;
    return true;
}
