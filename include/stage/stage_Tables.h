#pragma once

#include "stage/stage_Api.h"

// the tables of the stage code (names are ours; in their own file, so that GCC does not fold their values into the code)
namespace stage {
namespace detail {
extern const u32 s_Kinds[NAME_MAX];
// the furniture of a stage at most (BsFtrMgr)
extern const u32 s_FurnitureCapacities[NAME_MAX];
// the structure of a stage on the field (0xFC: none)
extern const u8 s_Structures[NAME_MAX];
extern const u8 s_LightKinds[NAME_MAX];
// the sound groups (item IDs of type 6) and the environment sounds (type 1; -1: none) of the stages
extern const u32 s_SoundGroups[NAME_MAX];
extern const u32 s_EnvironmentSounds[NAME_MAX];
// the rooms of the four player houses
extern const u8 s_HouseRooms[4][6];
// the game stages of the island and their result stages
extern const u8 s_IslandGames[12];
extern const u8 s_IslandGameResults[12];
} // namespace detail
} // namespace stage
