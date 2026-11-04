#pragma once

#include "decomp.h"

// the region of the system (nn::cfg, cached by the game) as an index: 0 Japan, 1 Americas, 2 Europe,
// 3 Korea (names are ours; game code that is not decompiled yet)
u32 GetRegionIndex(); // 0x003012B8 (name is ours)
