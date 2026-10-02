#pragma once

// Player houses: exterior and rooms.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgName.h"

#pragma pack(push, 1)

// If furniture is "active", meaning if the lamp is turned on, if the tv is turned on, etc.
// By default every spot in the room is active
// Its a u16 : 1 bitflag
struct SvRoomActiveFlags {
    /* 0x0000 */ u16 activeStates[0x10]; // default active
};
ASSERT_SIZE(SvRoomActiveFlags, 0x20);

// 0x5D904 and 0x5D90D
struct SvHouseExterior {
    /* 0x0000 */ u8 houseSize;
    /* 0x0001 */ u8 houseStyle;
    /* 0x0002 */ u8 houseDoorShape;
    /* 0x0003 */ u8 houseBrick;
    /* 0x0004 */ u8 houseRoof;
    /* 0x0005 */ u8 houseDoor;
    /* 0x0006 */ u8 houseFence;
    /* 0x0007 */ u8 housePavement;
    /* 0x0008 */ u8 houseMailBox;
};
ASSERT_SIZE(SvHouseExterior, 0x9);

struct SvRoomState {
    /* 0x0000 */ u8 brightLight; // or white light?
    /* 0x0001 */ u8 unk_0x1;
    /* 0x0002 */ u8 regularLight; // or yellow light?
    /* 0x0003 */ u8 unk_0x3;
    /* 0x0004 */ u8 unk_0x4; // lighting related
    /* 0x0005 */ u8 unk_0x5;
    /* 0x0006 */ u8 unk_0x6;
    /* 0x0007 */ u8 unk_0x7;
    /* 0x0008 */ u8 unk_0x8;
    /* 0x0009 */ u8 unk_0x9;
    /* 0x000A */ u8 unk_0xA;
    /* 0x000B */ u8 unk_0xB;
    /* 0x000C */ u8 unk_0xC;
    /* 0x000D */ u8 unk_0xD;
    /* 0x000E */ u8 lowLight; // dim light?
    /* 0x000F */ u8 unk_0xF;
    /* 0x0010 */ u8 unk_0x10; // lighting related
    /* 0x0011 */ u8 unk_0x11;
    /* 0x0012 */ u8 unk_0x12;
    /* 0x0013 */ u8 unk_0x13;
    /* 0x0014 */ u8 unk_0x14;
    /* 0x0015 */ u8 unk_0x15;
    /* 0x0016 */ u8 lightSwitchState; // 0 = Light OFF; 1 = Light ON
    /* 0x0017 */ u8 unk_0x17;
    /* 0x0018 */ u8 unk_0x18; // How often you went into the middle room???
    /* 0x0019 */ u8 unk_0x19;
    /* 0x001A */ u8 unk_0x1A; // lighting related
    /* 0x001B */ u8 unk_0x1B; // lighting related
    /* 0x001C */ u8 unk_0x1C; // lighting related
    /* 0x001D */ u8 unk_0x1D;
    /* 0x001E */ u8 roomSize;
    /* 0x001F */ bool isRoomUpgrading;
    /* 0x0020 */ u8 unk_0x20;
    /* 0x0021 */ u8 unk_0x21;
};
ASSERT_SIZE(SvRoomState, 0x22);

struct SvRoom {
    /* 0x0000 */ SvRoomState flags;
    /* 0x0022 */ SvRoomActiveFlags roomItemsActiveStates;
    /* 0x0042 */ SvRoomActiveFlags roomItemsPlacedOnOtherRoomItemsActiveStates;
    /* 0x0062 */ SvFgName roomItems[0x64];
    /* 0x01F2 */ SvFgName roomItemsPlacedOnOtherRoomItems[0x40];
    /* 0x02F2 */ SvFgName wallpaper;
    /* 0x02F6 */ SvFgName flooring;
    /* 0x02FA */ SvFgName playingSong;
    /* 0x02FE */ SvFgName unk_0x2FE;
};
ASSERT_SIZE(SvRoom, 0x302);

struct SvCockroachData {
    /* 0x0000 */ u16 cockroachAmount;
    /* 0x0002 */ u8 unk_0x2;
    /* 0x0003 */ u8 pad_0x3;
};
ASSERT_SIZE(SvCockroachData, 0x4);

struct SvPlayerHouse {
    /* 0x0000 */ u32 pad_0x0;
    /* 0x0004 */ SvHouseExterior exterior1;
    /* 0x000D */ SvHouseExterior exterior2; // maybe if you are upgrading? / 0x5D90D
    /* 0x0016 */ u16 pad_0x16;
    /* 0x0018 */ SvRoom middleRoom;
    /* 0x031A */ SvRoom secondRoom;
    /* 0x061C */ SvRoom basementRoom;
    /* 0x091E */ SvRoom rightRoom;
    /* 0x0C20 */ SvRoom leftRoom;
    /* 0x0F22 */ SvRoom backRoom;
    /* 0x1224 */ SvCockroachData cockroachData;
};
ASSERT_SIZE(SvPlayerHouse, 0x1228);

#pragma pack(pop)
