#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Api.h"

// Watering the flowers with a watering can, also on the other consoles (all names are ours).
namespace fgobj {
// the units that are watered around the target unit (the rows of s_WaterUnitOffsets: 1, 5 or 9 units; with the area
// WATER_AREA_SQUARE black roses become gold roses)
enum WaterArea : u8
{
    WATER_AREA_UNIT,   // the unit
    WATER_AREA_CROSS,  // the unit and the four next to it
    WATER_AREA_SQUARE, // the 3 x 3 units around it
};
// the unit offsets of the areas ([0]: x, [1]: z; the first 1, 5 or 9 entries), one table for WaterFlowers and one for
// GetHeightDifferenceMask
const u32 WATER_UNIT_NUM = 9;
extern const s8 s_WaterUnitOffsets[2][WATER_UNIT_NUM];
extern const s8 s_HeightCheckUnitOffsets[2][WATER_UNIT_NUM];

// the bits (by the index of s_HeightCheckUnitOffsets) of the units of area around x, z whose height differs from height by
// 8 or more
DECOMP_NOINLINE u16 GetHeightDifferenceMask(s32 x, s32 z, stage::Name stage, s32 area, f32 height); // 0x005970A8
// the flowers of the units of area around x, z on stage are watered (not the units of the bits of skipMask); isMine: the
// player of this console waters them (the census and the initiative count newly watered flowers)
void WaterFlowers(s32 x, s32 z, stage::Name stage, s32 area, u16 skipMask, bool isMine); // 0x00597CCC
// this player waters the flowers of area around x, z (the units on another height than the player are left out) and the
// other consoles get a WaterPacket (isUnknown goes inverted to isMine)
void Water(s32 x, s32 z, s32 area, bool isUnknown); // 0x00598F4C

// the sparkle effects of the watered flowers of area around x, z (the units on another height than height are left out,
// none with -1000; one after the other, 5 frames apart; isMine: with a sound; the larger cans have the other effect when
// other players are there; name is ours)
void StartWaterEffects(s32 x, s32 z, f32 height, s32 area, bool isMine); // 0x0059DC4C
// the unit offsets of StartWaterEffects (a copy of s_WaterUnitOffsets)
extern const s8 s_WaterEffectUnitOffsets[2][WATER_UNIT_NUM];

// the watering of another player (packet type 0x49)
struct WaterPacket {
    // (inline; a copy for the packet handler)
    DECOMP_EMIT_COPY WaterPacket() : stage(stage::NAME_NONE), x(0), z(0), area(0), skipMask(0) {} // 0x005958B4

    // the flowers are watered as by the other player (WaterFlowers)
    void Process() const; // 0x00595884

    static const u8 PACKET_TYPE = 0x49;

    /* 0x0 */ stage::Name stage;
    /* 0x1 */ u8 x;
    /* 0x2 */ u8 z;
    /* 0x3 */ u8 area;
    /* 0x4 */ u16 skipMask;
};
ASSERT_SIZE(WaterPacket, 0x6);
} // namespace fgobj
