#pragma once

// Buildings ("Strc", structures) and public works projects.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvMyDesign.h"

struct SvStrc {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvStrc(); // 0x0020C0E0

    // (names are ours) no structure (id 0xFC and above) / the id (the low byte) / a structure is placed when the place is
    // empty and the id valid / the place is emptied (id 0xFC, x and y 0)
    DECOMP_NOINLINE bool IsEmpty() const; // 0x00718EC8
    DECOMP_NOINLINE u8 GetId() const; // 0x00302720
    DECOMP_NOINLINE void Set(u16 id, u8 x, u8 y); // 0x0024EF94
    DECOMP_NOINLINE void Clear(); // 0x0024EFD8

    /* 0x0000 */ u16 id; // is none, only below 0xFC considered valid
    /* 0x0002 */ u8 xCoord;
    /* 0x0003 */ u8 yCoord;
};
ASSERT_SIZE(SvStrc, 0x4);

struct SvStrcList {
    /* 0x0000 */ SvStrc strcs[0x38];
    /* 0x00E0 */ SvStrc eventPwps[2];
};
ASSERT_SIZE(SvStrcList, 0xE8);

struct SvPwpUnlockFlags {
    /* 0x0000.0 */ u8 defaultPwPs : 1; // All unlocked at default (cobblestone bridge, suspension bridge, yellow bench, water well, fountain, park clock, street lamp, campsite, fence, fire hydrant, custom-design sign, face-cutout standee, do-not-enter sign)
    /* 0x0000.1 */ u8 topiaries : 1; // round, square, tulip
    /* 0x0000.2 */ u8 dreamSuite : 1;
    /* 0x0000.3 */ u8 museumRenovation : 1;
    /* 0x0000.4 */ u8 cafe : 1;
    /* 0x0000.5 */ u8 resetCenter : 1;
    /* 0x0000.6 */ u8 perfectTownPwPs : 1; // town-hall renovations & flower clock
    /* 0x0000.7 */ u8 stationReconstruction : 1;
    /* 0x0001.0 */ u8 drinkingFountain : 1;
    /* 0x0001.1 */ u8 garbageCan : 1;
    /* 0x0001.2 */ u8 flowerBed : 1;
    /* 0x0001.3 */ u8 outdoorChair : 1;
    /* 0x0001.4 */ u8 flowerArch : 1;
    /* 0x0001.5 */ u8 fairyTaleClock : 1;
    /* 0x0001.6 */ u8 fairyTaleBench : 1;
    /* 0x0001.7 */ u8 fairyTaleStreetlight : 1;
    /* 0x0002.0 */ u8 fairyTaleBridge : 1;
    /* 0x0002.1 */ u8 metalBench : 1;
    /* 0x0002.2 */ u8 roundStreetlight : 1;
    /* 0x0002.3 */ u8 illuminatedHeart : 1;
    /* 0x0002.4 */ u8 illuminatedClock : 1;
    /* 0x0002.5 */ u8 illuminatedTree : 1;
    /* 0x0002.6 */ u8 bell : 1;
    /* 0x0002.7 */ u8 archwaySculpture : 1;
    /* 0x0003.0 */ u8 statueFountain : 1;
    /* 0x0003.1 */ u8 hotSpring : 1;
    /* 0x0003.2 */ u8 streetlight : 1;
    /* 0x0003.3 */ u8 illuminatedArch : 1;
    /* 0x0003.4 */ u8 tower : 1;
    /* 0x0003.5 */ u8 modernClock : 1;
    /* 0x0003.6 */ u8 modernBench : 1;
    /* 0x0003.7 */ u8 modernStreetlight : 1;
    /* 0x0004.0 */ u8 scarecrow : 1;
    /* 0x0004.1 */ u8 geyser : 1;
    /* 0x0004.2 */ u8 windmill : 1;
    /* 0x0004.3 */ u8 woodBench : 1;
    /* 0x0004.4 */ u8 wisteriaTrellis : 1;
    /* 0x0004.5 */ u8 logBench : 1;
    /* 0x0004.6 */ u8 busStop : 1;
    /* 0x0004.7 */ u8 picnicBlanket : 1;
    /* 0x0005.0 */ u8 balloonArch : 1;
    /* 0x0005.1 */ u8 tireToy : 1;
    /* 0x0005.2 */ u8 pileOfPipes : 1;
    /* 0x0005.3 */ u8 campingCot : 1;
    /* 0x0005.4 */ u8 jungleGym : 1;
    /* 0x0005.5 */ u8 sandbox : 1;
    /* 0x0005.6 */ u8 hammock : 1;
    /* 0x0005.7 */ u8 waterPump : 1;
    /* 0x0006.0 */ u8 instrumentShelter : 1;
    /* 0x0006.1 */ u8 torch : 1;
    /* 0x0006.2 */ u8 firePit : 1;
    /* 0x0006.3 */ u8 solarPanel : 1;
    /* 0x0006.4 */ u8 blueBench : 1;
    /* 0x0006.5 */ u8 trafficSignal : 1;
    /* 0x0006.6 */ u8 stadiumLight : 1;
    /* 0x0006.7 */ u8 videoScreen : 1;
    /* 0x0007.0 */ u8 woodenBridge : 1;
    /* 0x0007.1 */ u8 zenGarden : 1;
    /* 0x0007.2 */ u8 zenBell : 1;
    /* 0x0007.3 */ u8 rackOfRice : 1;
    /* 0x0007.4 */ u8 drillingRig : 1;
    /* 0x0007.5 */ u8 zenClock : 1;
    /* 0x0007.6 */ u8 zenBench : 1;
    /* 0x0007.7 */ u8 zenStreetlight : 1;
    /* 0x0008.0 */ u8 sphinx : 1;
    /* 0x0008.1 */ u8 totemPole : 1;
    /* 0x0008.2 */ u8 parabolicAntenna : 1;
    /* 0x0008.3 */ u8 moaiStatue : 1;
    /* 0x0008.4 */ u8 stonehenge : 1;
    /* 0x0008.5 */ u8 pyramid : 1;
    /* 0x0008.6 */ u8 cubeSculpture : 1;
    /* 0x0008.7 */ u8 chairSculpture : 1;
    /* 0x0009.0 */ u8 policeStations : 1; // modern & classic
    /* 0x0009.1 */ u8 lighthouse : 1;
    /* 0x0009.2 */ u8 brickBridge : 1;
    /* 0x0009.3 */ u8 modernBridge : 1;
    /* 0x0009.4 */ u8 stoneTablet : 1;
    /* 0x0009.5 */ u8 windTurbine : 1;
    /* 0x0009.6 */ u8 cautionSign : 1;
    /* 0x0009.7 */ u8 yieldSign : 1;
    /* 0x000A.0 */ u8 fortuneTellersShop : 1;
    /* 0x000A.1 */ u8 unk_0xA_1 : 1;
    /* 0x000A.2 */ u8 unk_0xA_2 : 1;
    /* 0x000A.3 */ u8 unk_0xA_3 : 1;
    /* 0x000A.4 */ u8 unk_0xA_4 : 1;
    /* 0x000A.5 */ u8 unk_0xA_5 : 1;
    /* 0x000A.6 */ u8 unk_0xA_6 : 1;
    /* 0x000A.7 */ u8 unk_0xA_7 : 1;
    /* 0x000B.0 */ u8 unk_0xB_0 : 1;
    /* 0x000B.1 */ u8 unk_0xB_1 : 1;
    /* 0x000B.2 */ u8 unk_0xB_2 : 1;
    /* 0x000B.3 */ u8 unk_0xB_3 : 1;
    /* 0x000B.4 */ u8 unk_0xB_4 : 1;
    /* 0x000B.5 */ u8 unk_0xB_5 : 1;
    /* 0x000B.6 */ u8 unk_0xB_6 : 1;
    /* 0x000B.7 */ u8 unk_0xB_7 : 1;
};
ASSERT_SIZE(SvPwpUnlockFlags, 0xC);

// The index of a bit of SvPwpUnlockFlags (SvStrcData::UnlockPwp; names after the fields)
enum SvPwpUnlockId : u8 {
    PWP_UNLOCK_DEFAULT = 0, // All unlocked at default (cobblestone bridge, suspension bridge, yellow bench, water well, fountain, park clock, street lamp, campsite, fence, fire hydrant, custom-design sign, face-cutout standee, do-not-enter sign)
    PWP_UNLOCK_TOPIARIES = 1, // round, square, tulip
    PWP_UNLOCK_DREAM_SUITE = 2,
    PWP_UNLOCK_MUSEUM_RENOVATION = 3,
    PWP_UNLOCK_CAFE = 4,
    PWP_UNLOCK_RESET_CENTER = 5,
    PWP_UNLOCK_PERFECT_TOWN = 6, // town-hall renovations & flower clock
    PWP_UNLOCK_STATION_RECONSTRUCTION = 7,
    PWP_UNLOCK_DRINKING_FOUNTAIN = 8,
    PWP_UNLOCK_GARBAGE_CAN = 9,
    PWP_UNLOCK_FLOWER_BED = 10,
    PWP_UNLOCK_OUTDOOR_CHAIR = 11,
    PWP_UNLOCK_FLOWER_ARCH = 12,
    PWP_UNLOCK_FAIRY_TALE_CLOCK = 13,
    PWP_UNLOCK_FAIRY_TALE_BENCH = 14,
    PWP_UNLOCK_FAIRY_TALE_STREETLIGHT = 15,
    PWP_UNLOCK_FAIRY_TALE_BRIDGE = 16,
    PWP_UNLOCK_METAL_BENCH = 17,
    PWP_UNLOCK_ROUND_STREETLIGHT = 18,
    PWP_UNLOCK_ILLUMINATED_HEART = 19,
    PWP_UNLOCK_ILLUMINATED_CLOCK = 20,
    PWP_UNLOCK_ILLUMINATED_TREE = 21,
    PWP_UNLOCK_BELL = 22,
    PWP_UNLOCK_ARCHWAY_SCULPTURE = 23,
    PWP_UNLOCK_STATUE_FOUNTAIN = 24,
    PWP_UNLOCK_HOT_SPRING = 25,
    PWP_UNLOCK_STREETLIGHT = 26,
    PWP_UNLOCK_ILLUMINATED_ARCH = 27,
    PWP_UNLOCK_TOWER = 28,
    PWP_UNLOCK_MODERN_CLOCK = 29,
    PWP_UNLOCK_MODERN_BENCH = 30,
    PWP_UNLOCK_MODERN_STREETLIGHT = 31,
    PWP_UNLOCK_SCARECROW = 32,
    PWP_UNLOCK_GEYSER = 33,
    PWP_UNLOCK_WINDMILL = 34,
    PWP_UNLOCK_WOOD_BENCH = 35,
    PWP_UNLOCK_WISTERIA_TRELLIS = 36,
    PWP_UNLOCK_LOG_BENCH = 37,
    PWP_UNLOCK_BUS_STOP = 38,
    PWP_UNLOCK_PICNIC_BLANKET = 39,
    PWP_UNLOCK_BALLOON_ARCH = 40,
    PWP_UNLOCK_TIRE_TOY = 41,
    PWP_UNLOCK_PILE_OF_PIPES = 42,
    PWP_UNLOCK_CAMPING_COT = 43,
    PWP_UNLOCK_JUNGLE_GYM = 44,
    PWP_UNLOCK_SANDBOX = 45,
    PWP_UNLOCK_HAMMOCK = 46,
    PWP_UNLOCK_WATER_PUMP = 47,
    PWP_UNLOCK_INSTRUMENT_SHELTER = 48,
    PWP_UNLOCK_TORCH = 49,
    PWP_UNLOCK_FIRE_PIT = 50,
    PWP_UNLOCK_SOLAR_PANEL = 51,
    PWP_UNLOCK_BLUE_BENCH = 52,
    PWP_UNLOCK_TRAFFIC_SIGNAL = 53,
    PWP_UNLOCK_STADIUM_LIGHT = 54,
    PWP_UNLOCK_VIDEO_SCREEN = 55,
    PWP_UNLOCK_WOODEN_BRIDGE = 56,
    PWP_UNLOCK_ZEN_GARDEN = 57,
    PWP_UNLOCK_ZEN_BELL = 58,
    PWP_UNLOCK_RACK_OF_RICE = 59,
    PWP_UNLOCK_DRILLING_RIG = 60,
    PWP_UNLOCK_ZEN_CLOCK = 61,
    PWP_UNLOCK_ZEN_BENCH = 62,
    PWP_UNLOCK_ZEN_STREETLIGHT = 63,
    PWP_UNLOCK_SPHINX = 64,
    PWP_UNLOCK_TOTEM_POLE = 65,
    PWP_UNLOCK_PARABOLIC_ANTENNA = 66,
    PWP_UNLOCK_MOAI_STATUE = 67,
    PWP_UNLOCK_STONEHENGE = 68,
    PWP_UNLOCK_PYRAMID = 69,
    PWP_UNLOCK_CUBE_SCULPTURE = 70,
    PWP_UNLOCK_CHAIR_SCULPTURE = 71,
    PWP_UNLOCK_POLICE_STATIONS = 72, // modern & classic
    PWP_UNLOCK_LIGHTHOUSE = 73,
    PWP_UNLOCK_BRICK_BRIDGE = 74,
    PWP_UNLOCK_MODERN_BRIDGE = 75,
    PWP_UNLOCK_STONE_TABLET = 76,
    PWP_UNLOCK_WIND_TURBINE = 77,
    PWP_UNLOCK_CAUTION_SIGN = 78,
    PWP_UNLOCK_YIELD_SIGN = 79,
    PWP_UNLOCK_FORTUNE_TELLERS_SHOP = 80,
    PWP_UNLOCK_NUM = 81,
};

struct SvDesignStand {
    /* 0x0000 */ SvMyDesign pattern;
    /* 0x0870 */ u32 xCoord; // if none placed (idk why 32bit was needed, world coords are 8bit..)
    /* 0x0874 */ u32 yCoord; // if none placed (idk why 32bit was needed, world coords are 8bit..)
};
ASSERT_SIZE(SvDesignStand, 0x878);

struct SvStrcData {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvStrcData(); // 0x005CE840

    // the checksum of the data behind it (names are ours; isLogged: passed, not used)
    DECOMP_NOINLINE void UpdateChecksum(); // 0x0075A2B4
    DECOMP_NOINLINE bool IsChecksumValid(bool isLogged); // 0x0075A2DC

    // (names are ours) the house of a player (structure id = save number 0..3) is removed / a public works project is
    // unlocked (bit of unlockedPwPs, index < 81) / a structure (index 0 for 56 or more) / the two event structures are
    // emptied (true) / the design stands / a structure is added in the first empty place (the index; 0 if none is
    // empty) / an event structure is added / a structure with this id and place is removed (true if found) / for a new
    // town: no town tree, the design stands and the unlocked projects start again / everything is cleared / the size of
    // the town tree: the smaller of the steps by play hours (5, 20, 50, 100, 150, 200, 250) and by days played (5, 20,
    // 50, 100, 180, 300, 500)
    void RemovePlayerHouse(u32 saveNo); // 0x005CE370
    void UnlockPwp(SvPwpUnlockId index); // 0x005CE494
    SvStrc* GetStrc(u32 index); // 0x005CE4C8
    bool ClearEventPwps(); // 0x005CE4D8
    SvDesignStand* GetDesignStands(); // 0x005CE50C
    u32 AddStrc(u16 id, u8 x, u8 y); // 0x005CE514
    u32 AddEventPwp(u16 id, u8 x, u8 y); // 0x005CE584
    bool RemoveStrc(u16 id, u8 x, u8 y); // 0x005CE5F4
    void SetupNewTown(); // 0x005CE688
    void Clear(); // 0x005CE6C8
    void UpdateTownTreeSize(); // 0x005CE748

    /* 0x0000 */ u32 checksum; // Checksum of the 0x44B8 of this data
    /* 0x0004 */ u8 normalPwPsAmount;
    /* 0x0005 */ u8 eventPwPsAmount;
    /* 0x0006 */ u8 townTreeSize; // 1 <-> 7
    /* 0x0007 */ u8 pad_0x7;
    /* 0x0008 */ SvStrcList strcs;
    /* 0x00F0 */ SvDesignStand stands[8];
    /* 0x44B0 */ SvPwpUnlockFlags unlockedPwPs;
};
ASSERT_SIZE(SvStrcData, 0x44BC);

// (game code, not decompiled yet; names are ours) the design stands are set up for a new town / cleared
void Func27A61C(SvDesignStand* stands); // 0x0027A61C
void Func27A65C(SvDesignStand* stands); // 0x0027A65C

struct SvPwpProject {
    // (game code, not decompiled yet; the constructor of the save data builds it)
    SvPwpProject(); // 0x001B2000

    /* 0x0000 */ u8 unk_0x0; // ctor sets to 0
    /* 0x0001 */ u8 unk_0x1; // ctor sets to 0
    /* 0x0002 */ u8 unk_0x2; // ctor sets to 0
    /* 0x0003 */ u8 pad_0x3;
    /* 0x0004 */ u32 unk_0x4; // Money accumulated???
    /* 0x0008 */ u32 unk_0x8; // Total cost amount???
    /* 0x000C */ SvStrc strcs;
};
ASSERT_SIZE(SvPwpProject, 0x10);

