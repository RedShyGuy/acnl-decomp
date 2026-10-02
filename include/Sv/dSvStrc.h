#pragma once

// Buildings ("Strc", structures) and public works projects.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvMyDesign.h"

#pragma pack(push, 1)

struct SvStrc {
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

struct SvDesignStand {
    /* 0x0000 */ SvMyDesign pattern;
    /* 0x0870 */ u32 xCoord; // if none placed (idk why 32bit was needed, world coords are 8bit..)
    /* 0x0874 */ u32 yCoord; // if none placed (idk why 32bit was needed, world coords are 8bit..)
};
ASSERT_SIZE(SvDesignStand, 0x878);

struct SvStrcData {
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

struct SvPwpProject {
    /* 0x0000 */ u8 unk_0x0; // ctor sets to 0
    /* 0x0001 */ u8 unk_0x1; // ctor sets to 0
    /* 0x0002 */ u8 unk_0x2; // ctor sets to 0
    /* 0x0003 */ u8 pad_0x3;
    /* 0x0004 */ u32 unk_0x4; // Money accumulated???
    /* 0x0008 */ u32 unk_0x8; // Total cost amount???
    /* 0x000C */ SvStrc strcs;
};
ASSERT_SIZE(SvPwpProject, 0x10);

#pragma pack(pop)
