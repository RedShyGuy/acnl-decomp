// Constant tables of the field object code that GCC would fold into the code if they were visible there (the name of the
// file is ours).
#include "fgobj/fgobj_Api.h"
#include "fgobj/dPlace.h"
#include "fgobj/dWaterPacket.h"
#include "fgobj/dProc.h"
#include "Sv/dSvFgName.h"

namespace fgobj {
// (names are ours)
// 0x00860284
extern const u16 s_CoinStoneItems[10] = {
    SvFgName::NAME_100_BELLS,   SvFgName::NAME_100_BELLS,   SvFgName::NAME_200_BELLS,   SvFgName::NAME_300_BELLS,
    SvFgName::NAME_500_BELLS,   SvFgName::NAME_1_000_BELLS, SvFgName::NAME_2_000_BELLS, SvFgName::NAME_4_000_BELLS,
    SvFgName::NAME_8_000_BELLS, SvFgName::NAME_16_000_BELLS,
};
// 0x00860298
extern const u16 s_LuckyStoneItems[6] = {
    SvFgName::NAME_GOLD_NUGGET, SvFgName::NAME_SILVER_NUGGET, SvFgName::NAME_RUBY,
    SvFgName::NAME_SAPPHIRE,    SvFgName::NAME_EMERALD,       SvFgName::NAME_AMETHYST,
};
// 0x008602A4
extern const s32 s_LuckyStoneWeights[6] = {15, 15, 20, 20, 20, 20};
// 0x00860214
extern const s8 s_FruitDropOffsets[2][3][8] = {
    {{1, 1, 1, 2, 2, 2, 0, 0}, {-1, -1, -1, -2, -2, -2, 0, 0}, {0, -1, 1, 0, -1, 1, -1, 1}},
    {{0, 1, -1, 0, 1, -1, 1, -1}, {0, 1, -1, 0, 1, -1, 1, -1}, {1, 1, 1, 2, 2, 2, 0, 0}},
};
// 0x00860244
extern const s32 s_ShakeDropOffsets[2][8] = {{1, 0, 1, 1, 2, 2, 0, 2}, {1, 1, 0, 2, 1, 2, 2, 0}};
// (names are ours) the offsets of the three fruits of a fruit tree, the angles of the two fruits of a palm / of a banana
// palm (Proc::PlaceFruits / PlacePalmFruits)
// 0x008475DC
extern const f32 s_PalmFruitAngles[2] = {10.0f, 5.0f};
// 0x008475E4
extern const f32 s_BananaFruitAngles[2] = {0.0f, 16.0f};
// 0x008476A4
extern const f32 s_SwayScales[10][2] = {{1.04999995f, 0.931989014f}, {1.10000002f, 0.863978982f}, {1.08437502f, 0.899160981f}, {1.04999995f, 0.976562023f}, {1.015625f, 1.05396199f}, {1.0f, 1.08914399f}, {1.0f, 1.07521498f}, {1.0f, 1.04457104f}, {1.0f, 1.01392806f}, {1.0f, 1.0f}};
// 0x008475EC
extern const nn::math::VEC3_ s_FruitOffsets[3] = {{16.0f, 46.0f, 18.0f}, {-16.0f, 46.0f, 18.0f}, {-1.2f, 67.0f, 2.0f}};
// 0x00847610
extern const s8 s_WaterEffectUnitOffsets[2][WATER_UNIT_NUM] = {{0, -1, 1, 0, 0, -1, -1, 1, 1}, {0, 0, 0, -1, 1, -1, 1, -1, 1}};
// 0x0085FE58
extern const s8 s_PlaceUnitOffsets[2][PLACE_UNIT_NUM] = {{0, 0, -1, 1, -1, 1, -1, 1, 0}, {0, -1, -1, -1, 0, 0, 1, 1, 1}};
// 0x0085FE6C
extern const s32 s_PlaceUnitOrders[8][PLACE_UNIT_NUM] = {
    {0, 8, 6, 7, 4, 5, 2, 3, 1}, {0, 7, 8, 5, 6, 3, 4, 1, 2}, {0, 5, 7, 3, 8, 1, 6, 2, 4}, {0, 3, 5, 1, 7, 2, 8, 4, 6},
    {0, 1, 3, 2, 5, 4, 7, 6, 8}, {0, 2, 1, 4, 3, 6, 5, 8, 7}, {0, 4, 2, 6, 1, 8, 3, 7, 5}, {0, 6, 8, 4, 7, 2, 5, 1, 3},
};
// 0x0085FF8C
extern const u16 s_PlayerDirectionAngles[8] = {0x0000, 0x2000, 0x4000, 0x6000, 0x8000, 0xA000, 0xC000, 0xE000};
// 0x0085FF9C
extern const u16 s_OreRockItems[6] = {
    SvFgName::NAME_GOLD_NUGGET, SvFgName::NAME_SILVER_NUGGET, SvFgName::NAME_RUBY,
    SvFgName::NAME_SAPPHIRE,    SvFgName::NAME_EMERALD,       SvFgName::NAME_AMETHYST,
};
// the world size of a unit in x and z for the areas of the Proc (name is ours)
// 0x008602D4
extern const f32 s_AreaUnitSizeX = 32.0f;
// 0x008602D8
extern const f32 s_AreaUnitSizeZ = 32.0f;
// 0x008601BC
extern const s8 s_DropUnitOffsets[2][PLACE_UNIT_NUM] = {{0, 0, 0, 1, -1, 1, -1, 1, -1}, {0, 1, -1, 0, 0, 1, 1, -1, -1}};
// 0x008601CE
extern const s8 s_WaterUnitOffsets[2][WATER_UNIT_NUM] = {{0, -1, 1, 0, 0, -1, -1, 1, 1}, {0, 0, 0, -1, 1, -1, 1, -1, 1}};
// 0x008601E0
extern const s8 s_HeightCheckUnitOffsets[2][WATER_UNIT_NUM] = {{0, -1, 1, 0, 0, -1, -1, 1, 1}, {0, 0, 0, -1, 1, -1, 1, -1, 1}};
// 0x008601F4
extern const s32 s_DirectionOffsets[8] = {0, -3, -2, -1, 0, 1, 2, 3};
} // namespace fgobj
