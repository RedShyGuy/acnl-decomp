#pragma once

// Identities of towns and players as stored in the save data.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"

#pragma pack(push, 1)

struct SvTownId {
    /* 0x0000 */ u16 tid; // Default is 0
    /* 0x0002 */ char16 dataTownName[9]; // Default is 0
    /* 0x0014 */ u8 unk_0x14; // Default is 0xA
    /* 0x0015 */ u8 unk_0x15;
};
ASSERT_SIZE(SvTownId, 0x16);

struct SvPlayerId {
    /* 0x0000 */ u16 pid;
    /* 0x0002 */ char16 playerName[9];
    /* 0x0014 */ u8 gender;
    /* 0x0015 */ u8 pad_0x15;
};
ASSERT_SIZE(SvPlayerId, 0x16);

struct SvPersonalId {
    /* 0x0000 */ SvPlayerId playerData;
    /* 0x0016 */ SvTownId townData;
    /* 0x002C */ u8 tpcCountry;
    /* 0x002D */ u8 tpcCounty;
};
ASSERT_SIZE(SvPersonalId, 0x2E);

#pragma pack(pop)
