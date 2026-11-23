// fallout_shared/sky/skytexturemanager.cpp: SkyTextureManager, SkyTask (5 functions)

#include "../../../include/BSShaderManager.h"
#include "../../../include/IOManager.h"
#include "../../../include/IOTask.h"
#include "../../../include/QueuedFile.h"
#include "../../../include/QueuedFileEntry.h"
#include "../../../include/SkyTask.h"
#include "../../../include/SkyTextureManager.h"
#include "AutoMemContext.h"

bool __cdecl CleanPath(const char* apInPath, char* output, unsigned int aiOutSize);
extern IOManager* DAT_01202d98;

// @fnv 0x0063fb70
// src: XBOX_RETAIL_PDB 0x0063fb70
// pdb: SkyTextureManager::LoadTextureInBackground | SkyTextureManager.obj | retail, call graph (shared callers), high
// pdb params: apspTexture, apFilename
// pdb locals: pnewtask, pfileName, pstandardizedFileName
void SkyTextureManager::LoadTextureInBackground(NiPointer<NiTexture>* apspTexture, const char* apFilename)
{
    struct TextureRequest
    {
        SkyTask* task;
        const char* path;
    } request;
    char standardizedPath[260];
    request.path = 0;
    if (apFilename)
    {
        CleanPath(apFilename, standardizedPath, sizeof(standardizedPath));
        request.path = standardizedPath;
    }
    request.task = new SkyTask(apspTexture, apFilename);
    DAT_01202d98->AddTask(reinterpret_cast<QIOTask*>(request.task));
}

// Creates a sky memory context and requests the sky texture via BSShaderManager::GetTexture using GetFileName().
// @fnv 0x0063fac0
// pdb: SkyTask::Run | SkyTextureManager.obj | retail, upstream name already matches, high
// pdb locals: tempMemContext
void SkyTask::Run()
{
    AutoMemContext context(static_cast<MEM_CONTEXT>(33), true,
                           "D:\\_Fallout3\\Platforms\\Common\\Code\\Fallout Shared\\sky\\SkyTextureManager.cpp", 50);
    BSShaderManager::GetTexture(GetFileName(), true, *pspTexture, true, false);
}

// @fnv 0x0063fa10
// pdb: SkyTask::SkyTask | SkyTextureManager.obj | debug, upstream name already matches, high
// pdb params: apspTexture, apFilename
SkyTask::SkyTask(NiPointer<NiTexture>* apspTexture, const char* apFilename) : QueuedFileEntry(3)
{
    pspTexture = apspTexture;
    SetFileName_00c3cee0(apFilename);
    LookupFileEntryByArchive(1);
}

// @fnv 0x0063fb40
// pdb: SkyTask::GetDescription | SkyTextureManager.obj | debug, upstream name already matches, high
// pdb params: apDescription, aiBufferSize
bool SkyTask::GetDescription(char* apDescription, unsigned int aiBufferSize)
{
    return QueuedFileEntry::GetDescription(apDescription, aiBufferSize, (char*)"skytask");
}
