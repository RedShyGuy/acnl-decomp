#pragma once

// Reference to a villager (NPC) from another town.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvPersonalId.h"

#pragma pack(push, 1)

struct SvNpcRef {
    /* 0x0000 */ SvTownId townData1;
    /* 0x0016 */ SvTownId townData2;
    /* 0x002C */ u16 villagerId; // Set to 0xFFFF in player ctor
    /* 0x002E */ u8 villagerPersonality; // Set to 0x8 in player ctor
    /* 0x002F */ u8 pad_0x2F; // Padding: Not set in ctor
};
ASSERT_SIZE(SvNpcRef, 0x30);

#pragma pack(pop)
