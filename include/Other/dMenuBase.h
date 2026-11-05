#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "ssys/st/dListNode.h"

// the base of the menus: registered in the list of the open menus while it lives (class, slot and member names after
// libgarden; the FUN_ slots are named after their address there)
// RTTI 8MenuBase @ 0x008CD508
// vtable 0x008F9C48 (vptr 0x008F9C50), offset_to_top 0, 25 entries
class MenuBase : public ::Base
{
public:
    // the node of the menu in the list of the open menus
    // RTTI N8MenuBase4ListE @ 0x008D4028
    // vtable 0x0090C0E4 (vptr 0x0090C0EC), offset_to_top 0, 2 entries
    class List : public ::ssys::st::ListNode
    {
    public:
        List() : m_pMenu(NULL) {}
        virtual ~List(); // 0x006AB408 slot 0x00 (name after libgarden)
        // 0x006AB404 slot 0x04 (deleting dtor)

        MenuBase* m_pMenu; // 0x0C (name after libgarden)
    };

    MenuBase(); // 0x006AB4A8 | libgarden [tier A]
    virtual ~MenuBase(); // 0x006AB508 slot 0x00 | libgarden
    // 0x006AB4E8 slot 0x04 (deleting dtor)
    virtual bool CanInitialize() const; // 0x006AB460 slot 0x08 | libgarden
    virtual oml::framework::Result Initialize(); // 0x006AB414 slot 0x0C
    virtual void HandleInitializationResult(oml::framework::Result result); // 0x006AB39C slot 0x10
    virtual bool CanFinalize() const; // 0x006AB494 slot 0x14 | libgarden
    virtual oml::framework::Result Finalize(); // 0x006AB458 slot 0x18
    virtual void HandleFinalizationResult(oml::framework::Result result); // 0x006AB3A0 slot 0x1C | libgarden
    virtual bool CanCalc() const; // 0x006AB3C4 slot 0x20 | libgarden
    virtual oml::framework::Result Calc(); // 0x006AB448 slot 0x24
    virtual void HandleCalcResult(oml::framework::Result result); // 0x006AB3DC slot 0x28
    virtual oml::framework::Result Draw(); // 0x006AB40C slot 0x30
    virtual void FUN_006ab3d8(); // 0x006AB3D8 slot 0x40 | libgarden
    virtual void OnClose(); // 0x006AB3E8 slot 0x44 | libgarden
    virtual void FUN_006ab3f4(bool value); // 0x006AB3F4 slot 0x48 | libgarden (sets unk1)
    virtual bool FUN_006ab450(); // 0x006AB450 slot 0x4C | libgarden (unk1)
    virtual void FUN_006ab3e0(bool value); // 0x006AB3E0 slot 0x50 | libgarden (sets unk0)
    virtual bool FUN_006ab440(); // 0x006AB440 slot 0x54 | libgarden (unk0)
    virtual void FUN_006ab3fc(bool value); // 0x006AB3FC slot 0x58 | libgarden (sets unk2)
    virtual bool FUN_006ab3ec(); // 0x006AB3EC slot 0x5C | libgarden (unk2)
    virtual bool FUN_00767b30(); // 0x00767B30 slot 0x60 | libgarden

    List m_List; // 0x14 (name after libgarden)
    u8 m_Kind;   // 0x24 (name after libgarden) the argument of the process
    bool unk0;   // 0x25 (name after libgarden)
    bool unk1;   // 0x26 (name after libgarden)
    bool unk2;   // 0x27 (name after libgarden)
};
ASSERT_SIZE(MenuBase, 0x28);

// the list of the open menus (names are ours)
void AddToMenuList(ssys::st::ListNode* node); // 0x006D2848
void RemoveFromMenuList(ssys::st::ListNode* node); // 0x006D3350
