#pragma once

#include "decomp.h"

// the region of the system (nn::cfg, cached by the game) as an index: 0 Japan, 1 Americas, 2 Europe,
// 3 Korea (names are ours; game code that is not decompiled yet)
u32 GetRegionIndex(); // 0x003012B8 (name is ours)

// the region of the game (event::Region, 0..9: Japan, America English/Spanish/French, Europe English/Spanish/French/
// Italian/German, Korea) from the region and the language of the console; reads the two bytes the start code 0x11DA38
// sets from nn::cfg (0x95823C region, 0x95823D language). Game code that is not decompiled yet (between math helpers and
// script, file unknown; name is ours).
s32 GetRegion(); // 0x005D51AC
