#pragma once

#include <stddef.h>
#include "TESObjectACTI.h"
#include "TESDescription.h"
#include "MagicTarget.h"
#include "TerminalCompareData.h"

class BGSNote;
class TerminalMenuItem;

// pdb: class BGSTerminal, size 0xa8, bases: TESObjectACTI, TESDescription, 3 members
class BGSTerminal : public TESObjectACTI, public TESDescription
{
public:
    BGSTerminal();
    virtual ~BGSTerminal();

    virtual void InitializeComponent();
    virtual void ClearData();
    virtual bool LoadForm(TESFile* modInfo);
    virtual void SaveForm(void);
    virtual void InitItem(void);
    virtual void CopyFrom(const TESForm* apCopy);
    virtual bool Compare(TESForm* apCompare);

    void ResetFormData();
    bool Load(TESFile* apFile);
    void BGSTerminal_SetPassword(BGSNote* note);
    void AddMenuItem(TerminalMenuItem* apItem);
    BSSimpleList<TerminalMenuItem*>* FUN_00461150();

    BSSimpleList<TerminalMenuItem*> m_kMenuItems;
    BGSNote* pPassword;
    TerminalCompareData Data;
};

typedef char BGSTerminal_size_check[1];
typedef char BGSTerminal_menu_items_offset_check[1];
typedef char BGSTerminal_password_offset_check[1];
typedef char BGSTerminal_data_offset_check[1];
