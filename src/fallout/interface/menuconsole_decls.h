#include "../../../include/InterfaceManager.h"
// fallout/interface/menuconsole_decls.h: console helpers and external declarations.
#ifndef FNV_MENUCONSOLE_DECLS_H
#define FNV_MENUCONSOLE_DECLS_H

#include "menuconsole_support.h"

extern "C" char* __cdecl __insecure_strcpy(char*, const char*);
extern "C" size_t __cdecl strlen(const char*);
extern "C" int __cdecl sprintf(char*, const char*, ...);
extern "C" int __cdecl sprintf_s(char*, size_t, const char*, ...);
extern "C" int __cdecl sscanf(const char*, const char*, ...);
extern "C" void* __cdecl fnv_zlib_memset(void*, int, size_t);
namespace console_crt
{
    int __cdecl strcat_s(char*, unsigned int, const char*);
}
int __cdecl _mbsicmp(const unsigned char*, const unsigned char*);
int __cdecl _mbscmp(const unsigned char*, const unsigned char*);
int __cdecl _mbsnicmp(const unsigned char*, const unsigned char*, unsigned int);
template <class T> unsigned int BSStringLength(const T*);
// pdb: class TaskQueueInterface, size 0x10 (here 0x1), 4 members
class TaskQueueInterface
{
  public:
    static void CopyStringToBuffer(char*, unsigned int, const char*);
};
void DebugLog(const char*, ...);
int _tcsncmp(const char*, const char*, unsigned int);
int _mbsnbicmp_(const char*, const char*, unsigned int);
void _mbsnbcpy_(char*, const char*, unsigned int);
void _MenuConsole_PrintLine(ConsoleManager*, const char*, ...);
void Interface_UpdateScreenShape(int, int);
void Interface_ToggleSafeZone(int);
void Interface_SetFirstChanceTextureRelease();
void Interface_IsolateMenuElements(bool, bool);
void Interface_RestoreMenuElements();
int StartMenu_GetClassStatic();
int MinInt(int, int);
int BSMAX_00647b70(int, int);

#include "../../../include/SETTING_VALUE.h"
#include "../../../include/SettingT_GameSettingCollection_.h"
extern SettingT_GameSettingCollection_ DAT_011d8cfc;
extern SettingT_GameSettingCollection_ DAT_011d8d38;
extern bool DAT_011d8ce4;

#include "../../../include/COMPILER_NAME.h"
FNV_STATIC_ASSERT(sizeof(Script) == 0x54);
Tile* FindNamedTile(Tile*, const char*);
Menu* FindMenuById(int);

InterfaceManager* InterfaceManager_GetSingleton();
class MenuSystem
{
  public:
    static MenuSystem* Instance(bool);
    Tile* TileReloadContext(int);
    bool MenuManager_DeleteMenu(int);
    Menu* GetFrontmostMenu();
    bool IsMenuClassVisible(int);
};
void* DynamicCastByRTTI(void*, NiNode*);
extern unsigned char DAT_011f4428;
const char* GetName_00700300(Tile*);

FNV_STATIC_ASSERT(sizeof(BSFile) == 0x158);
FNV_STATIC_ASSERT(sizeof(NiFrustum) == 28);
#endif
