#pragma once

#include <stddef.h>

#include "Menu.h"
#include "ListBox_ItemChange___.h"
#include "BSSoundHandle.h"
#include "StrMem.h"
#include "BSSoundHandle.h"

class Actor;
class ActorView;
class ExpectedReference;
class InventoryEntry;
class ItemEntry;
class MenuForm;
class MenuList;
class PlaybackHandle;
template <class T> class BSStringT;
class PlaybackTopic;
class TESBoundObject;
class TESObjectREFR;
class TileObj;

enum Interface__CONTAINER_MENU_MODE;
enum ContainerMenu__FILTER_MODE;
enum MENU_SOUNDS;

// pdb: class ContainerMenu, size 0x10c, bases: Menu, 15 members
class ContainerMenu : public Menu
{
public:
    Tile* m_pTiles[19];
    TESObjectREFR* m_pContainer;
    BSSimpleList<TESBoundObject*> m_kContainerItems;
    Interface__CONTAINER_MENU_MODE m_eContainerMenuMode;
    int m_iStolenItemCount;
    bool m_bFirstContainerFlag;
    bool m_bSecondContainerFlag;
    unsigned char unk8A[2];
    ContainerMenu__FILTER_MODE m_eFilterMode;
    ContainerMenu__FILTER_MODE m_eSecondFilterMode;
    MENU_SOUNDS m_eMenuSounds;
    ListBox_ItemChange___ m_kFirstItemChangeList;
    ListBox_ItemChange___ m_kSecondItemChangeList;
    ListBox_ItemChange___* m_pCurrentList;
    unsigned int m_uiCurrentItemIndex;
    BSSoundHandle m_kFadeSequence;

    ContainerMenu();
    virtual ~ContainerMenu();
    virtual void AssignTileByID(int, Tile*);
    virtual void HandleSelection(int, Tile*);
    virtual void HandleMouseover(unsigned int, Tile*);
    virtual void HandleMouseoverTileAlt(unsigned int, Tile*);
    virtual void HandleMousewheel(unsigned int, Tile*);
    virtual void UpdateDialog();
    virtual bool HandleKeyboardInput(unsigned int);
    virtual int GetClass();
    virtual bool HandleMenuInput(int, float);
    virtual bool HandleControllerInput(int, Tile*);

    static void __cdecl Close();
    void UpdateContainer(TESObjectREFR*, bool);
    void ContainerMenu_UpdateLists(int);
    void ContainerMenu_UpdateLists(MenuForm*);
    bool HasTiles();
    void PlayContainerSound(TESObjectREFR*, bool);
    void InitContainerMode();
    void UpdateItemInList(void*, void*, void*);
    void AttachAndDeliver(MenuForm*, void*, MenuList*);
    bool IsPlacingMine(InventoryEntry*, ExpectedReference*, ActorView*);
    int GetStolenValue(ItemEntry*, int, bool);
    bool PickpocketCheck(Actor*, int);
    void SetContainerTitle(const char*);
    void SetFilterModeTitle(TileObj*);
    void PlaySelectedTopicInfo(PlaybackTopic*);
    void ShowMessageWithSound(BSStringT<char>, PlaybackHandle);
    void ShowMessageWithSound(StrMem, BSSoundHandle);
};

typedef char ContainerMenu_check_size[1];
typedef char ContainerMenu_check_m_pTiles[1];
typedef char ContainerMenu_check_m_pContainer[1];
typedef char ContainerMenu_check_m_kContainerItems[1];
typedef char ContainerMenu_check_m_eContainerMenuMode[1];
typedef char ContainerMenu_check_m_iStolenItemCount[1];
typedef char ContainerMenu_check_m_bFirstContainerFlag[1];
typedef char ContainerMenu_check_m_bSecondContainerFlag[1];
typedef char ContainerMenu_check_unk8A[1];
typedef char ContainerMenu_check_m_eFilterMode[1];
typedef char ContainerMenu_check_m_eSecondFilterMode[1];
typedef char ContainerMenu_check_m_eMenuSounds[1];
typedef char ContainerMenu_check_m_kFirstItemChangeList[1];
typedef char ContainerMenu_check_m_kSecondItemChangeList[1];
typedef char ContainerMenu_check_m_pCurrentList[1];
typedef char ContainerMenu_check_m_uiCurrentItemIndex[1];
typedef char ContainerMenu_check_m_kFadeSequence[1];
