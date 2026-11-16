#pragma once

#include "decomp.h"
#include "field/dFieldTypes.h"
#include "Sv/dSvFgName.h"

// The items of one block (acre) of a field, 16*16 units (name and layout after libgarden sv/SvFgBlock.hpp; the function
// names are ours). The town and island data hold them for the blocks with items (SvTownDataUnk5C, SvTownDataUnk1CA92).
struct SvFgBlock {
    DECOMP_EMIT_COPY SvFgBlock() {} // 0x006F08EC
    // (only the items are destroyed)
    ~SvFgBlock(); // 0x001399DC

    // sets all items to s_EmptyItem
    void Clear(); // 0x006F0890

    // the item of a unit (the position is taken modulo 16; two copies)
    // 0x007713B4 (copy)
    DECOMP_EMIT_COPY DECOMP_NOINLINE SvFgName* GetFg(s32 x, s32 z) // 0x002FAEF0
    {
        return &items[z & (UNIT_BASE_NUM - 1)][x & (UNIT_BASE_NUM - 1)];
    }

    // the item of a unit; s_EmptyItem outside of the block
    SvFgName* GetFgChecked(s32 x, s32 z); // 0x00771380
    // sets the item of a unit (nothing outside of the block)
    void SetFg(const SvFgName& item, s32 x, s32 z); // 0x002FCC78

    // the item that Clear writes and GetFgChecked returns outside of the block
    static SvFgName s_EmptyItem;

    /* 0x0000 */ SvFgName items[UNIT_BASE_NUM][UNIT_BASE_NUM];
};
ASSERT_SIZE(SvFgBlock, 0x400);

// An item at a unit of a block and its layer, returned by value by the item lists of the rooms (names are ours).
struct FgPlacement {
    /* 0x0 */ SvFgName item;
    /* 0x4 */ s32 x;
    /* 0x8 */ s32 z;
    /* 0xC */ u8 layer;
};
ASSERT_SIZE(FgPlacement, 0x10);
