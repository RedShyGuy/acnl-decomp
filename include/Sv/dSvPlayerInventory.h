#pragma once

// The pockets of a player: 16 items and how each one looks.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvFgName.h"

#pragma pack(push, 1)

struct SvPlayerInventory {
    // how an item in the inventory looks (names after libgarden)
    enum ItemAppearance : u8 {
        ITEM_APPEARANCE_DEFAULT = 0,
        ITEM_APPEARANCE_PRESENT = 1,
        ITEM_APPEARANCE_MAIL = 2,
        ITEM_APPEARANCE_WRAP_PRESENT = 3,
    };

    static const u32 SIZE = 0x10;

    // the item in the slot (the empty item for a wrong index or an item that cannot be there)
    const SvFgName* GetItem(unsigned int index, ItemAppearance* pAppearance = NULL) const; // 0x007250DC | libgarden [tier A]
    // puts the item into the first empty slot
    bool AddItem(const SvFgName& item, ItemAppearance appearance, SvFgName* pNewItem); // 0x003233A4 | libgarden [tier A]
    bool SetItem(unsigned int index, const SvFgName& item, ItemAppearance appearance, SvFgName* pNewItem); // 0x00323484 | libgarden [tier A]
    // empties all slots
    void Clear(); // 0x00323444 (name is ours)

    /* 0x0000 */ SvFgName items[0x10]; // SIZE
    /* 0x0040 */ u8 appearances[0x10]; // ItemAppearance (research name: item locks; the code reads and writes the appearance)
};
ASSERT_SIZE(SvPlayerInventory, 0x50);

#pragma pack(pop)
