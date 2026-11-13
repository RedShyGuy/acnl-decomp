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

    // a town is set (tid != 0)
    bool IsValid() const; // 0x0076D6F0 (name is ours)
};
ASSERT_SIZE(SvTownId, 0x16);

// (names are ours) a town id is made from a name (9 characters, cut with the word functions) and a region: with the id
// 0xFFFF / a new random id (0x8000 | 0..0x7FFE) / a given id
void MakeTownId(SvTownId* town, const char16* name, u32 region); // 0x006BC944
void MakeNewTownId(SvTownId* town, const char16* name, u32 region); // 0x006BCA08
void MakeTownId(SvTownId* town, const char16* name, u16 tid, u32 region); // 0x006BCADC

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

    // (empty; ARMCC's copy for the array constructors is a bx lr)
    SvPersonalId() {} // 0x001B5138
    // a player and a town are set
    bool IsValid() const; // 0x002FBE08 (name is ours)
};
ASSERT_SIZE(SvPersonalId, 0x2E);

#pragma pack(pop)
