
// Functions are listed by their @fnv annotations; compile flags inferred from the PC code.

// pdb: structure ID3DXBuffer, size 0x4, bases: IUnknown, 0 members
struct ID3DXBuffer
{
    virtual long __stdcall QueryInterface(const void* riid, void** ppv) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
    virtual void* __stdcall GetBufferPointer() = 0;
    virtual unsigned long __stdcall GetBufferSize() = 0;
};

extern "C" long __stdcall D3DXAssembleShaderFromFileA(const char* pSrcFile, const void* pDefines, void* pInclude,
                                                      unsigned long Flags, ID3DXBuffer** ppShader,
                                                      ID3DXBuffer** ppErrorMsgs);

bool NiD3DShaderProgramFactory_ResolveShaderFileName(const char* pcOriginalName, char* pcTrueName, unsigned int uiStringLen);
int ShaderLog(int iLevel, int aiMemoryContext, const char* pcFormat, ...);
void* NiMalloc(unsigned int uiSize);
void NiFree(void* pvMem);
int strcpy_s(char* dst, size_t size, char* src);
void* memcpy(void* dst, const void* src, size_t size);

extern unsigned int g_uiShaderAssembleFlags;
inline void NiMemcpy(void* d, const void* s, unsigned int n)
{
    memcpy(d, s, n);
}

struct ShaderLoader
{
    void* m_vtbl;
    unsigned int m_uiValue1;

    bool AssembleShaderFile(const char* pcFileName, void** pvCode, unsigned int* uiCodeSize, int* pvConstantData, int* uiConstantSize,
                            int bRecoverable);
};

// Resolves the shader path, assembles it with D3DXAssembleShaderFromFileA, copies the bytecode into a NiMalloc buffer, and logs errors on failure.
// @fnv 0x00e97ea0
// pdb: NiD3DShaderProgramCreatorAsm::LoadShaderCodeFromFile | NiD3DShaderProgramCreatorAsm_Xenon.obj | retail, string-set bridge, high
// pdb params: pcFileName, pvCode, uiCodeSize, pvConstantData, uiConstantSize, bRecoverable
// pdb locals: uiFlags, pkErrors, acShaderPath, eResult, pkCode, pcErr, pvBuff, uiLen
bool ShaderLoader::AssembleShaderFile(const char* pcFileName, void** pvCode, unsigned int* uiCodeSize,
                                      int* pvConstantData, int* uiConstantSize, int bRecoverable)
{
    *pvCode = 0;
    *uiCodeSize = 0;
    *pvConstantData = 0;
    *uiConstantSize = 0;

    if (pcFileName && *pcFileName)
    {
        ID3DXBuffer* pkErrors;
        ID3DXBuffer* pkShader;
        char acPath[260];
        if (!NiD3DShaderProgramFactory_ResolveShaderFileName(pcFileName, acPath, 260))
        {
            ShaderLog(1, bRecoverable, "Failed to find shader program file %s\n", pcFileName);
            return false;
        }

        unsigned int uiAssembleFlags = (m_uiValue1 | g_uiShaderAssembleFlags) & 3;
        if (D3DXAssembleShaderFromFileA(acPath, 0, 0, uiAssembleFlags, &pkShader, &pkErrors) < 0)
        {
            char* pcErrorText = 0;
            if (pkErrors)
            {
                char* pcErrorMsg = (char*)pkErrors->GetBufferPointer();
                if (pcErrorMsg)
                {
                    unsigned int uiErrorLen = pkErrors->GetBufferSize();
                    pcErrorText = (char*)NiMalloc(uiErrorLen);
                    strcpy_s(pcErrorText, uiErrorLen, pcErrorMsg);
                    ShaderLog(1, bRecoverable, "Failed to assemble shader %s\nError: %s\n", pcFileName, pcErrorText);
                }
                pkErrors->Release();
            }
            else
            {
                ShaderLog(1, bRecoverable, "Failed to assemble shader %s\nError: NONE REPORTED\n", pcFileName);
            }
            NiFree(pcErrorText);
            if (pkShader)
                pkShader->Release();
            return false;
        }

        *uiCodeSize = pkShader->GetBufferSize();
        *pvCode = NiMalloc(*uiCodeSize);
        NiMemcpy(*pvCode, pkShader->GetBufferPointer(), *uiCodeSize);
        pkShader->Release();
        if (pkErrors)
            pkErrors->Release();
        return true;
    }

    ShaderLog(1, 0, "Invalid shader file name\n");
    return false;
}

extern "C" long __stdcall D3DXAssembleShader_00ee6e52(const char*, unsigned int, const void*, void*, unsigned int, ID3DXBuffer**,
                                       ID3DXBuffer**);
extern "C" void* __cdecl memmove(void*, const void*, size_t);

inline void Move266(void* d, const void* s, unsigned int n)
{
    memmove(d, s, n);
}

struct FUN_00e98080_shell
{
    void* vtable;
    unsigned int flags;

    bool NiD3DShaderProgramCreatorAsm_LoadShaderCodeFromBuffer(const char*, unsigned int, void**, unsigned int*, void**, unsigned int*, int);
};

// Assembles source, logs failures, and copies successful output into a newly allocated buffer returned through data.
// @fnv 0x00e98080
// src: COMMUNITY 0x00e98080
// pdb: NiD3DShaderProgramCreatorAsm::LoadShaderCodeFromBuffer | NiD3DShaderProgramCreatorAsm_Xenon.obj | retail, string-set bridge, high
// pdb params: pvBuffer, uiBufferSize, pvCode, uiCodeSize, pvConstantData, uiConstantSize, bRecoverable
// pdb locals: uiFlags, pkErrors, eResult, pkCode, pcErr, pvBuff, uiLen
bool FUN_00e98080_shell::NiD3DShaderProgramCreatorAsm_LoadShaderCodeFromBuffer(const char* pvBuffer, unsigned int uiBufferSize, void** pvCode,
                                      unsigned int* uiCodeSize, void** pvConstantData, unsigned int* uiConstantSize,
                                      int bRecoverable)
{
    void** const volatile& originalData = pvCode;
    void** dataOut = originalData;
    unsigned int* sizeOut = uiCodeSize;
    *dataOut = 0;
    *sizeOut = 0;
    *pvConstantData = 0;
    *uiConstantSize = 0;

    if (!pvBuffer || !uiBufferSize)
        goto invalid;

    {
        unsigned int options = (flags | g_uiShaderAssembleFlags) & 3;
        long status =
            D3DXAssembleShader_00ee6e52(pvBuffer, uiBufferSize, 0, 0, options, (ID3DXBuffer**)&pvCode, (ID3DXBuffer**)&uiCodeSize);
        if (status < 0)
        {
            char* pszError = 0;
            ID3DXBuffer* error = (ID3DXBuffer*)uiCodeSize;
            if (error)
            {
                char* text = (char*)error->GetBufferPointer();
                if (text)
                {
                    unsigned int n = ((ID3DXBuffer*)uiCodeSize)->GetBufferSize();
                    pszError = (char*)NiMalloc(n);
                    strcpy_s(pszError, n, text);
                    ShaderLog(1, bRecoverable, "Failed to assemble shader from memory\nError: %s\n", pszError);
                }
                ((ID3DXBuffer*)uiCodeSize)->Release();
            }
            else
            {
                ShaderLog(1, bRecoverable, "Failed to assemble shader from memory\nError: NONE REPORTED\n");
            }
            NiFree(pszError);
            if (pvCode)
                ((ID3DXBuffer*)pvCode)->Release();
            return false;
        }

        *sizeOut = ((ID3DXBuffer*)pvCode)->GetBufferSize();
        *dataOut = NiMalloc(*sizeOut);
        Move266(*dataOut, ((ID3DXBuffer*)pvCode)->GetBufferPointer(), *sizeOut);
        ((ID3DXBuffer*)pvCode)->Release();
        if (uiCodeSize)
            ((ID3DXBuffer*)uiCodeSize)->Release();
        return true;
    }

invalid:
    ShaderLog(1, bRecoverable, "Invalid shader buffer\n");
    return false;
}
