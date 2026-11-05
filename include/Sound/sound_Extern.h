#pragma once

#include "decomp.h"

// game functions that the sound code calls (not decompiled yet; names after the address until their meaning is known)

// the rain level of the weather (0 none; the rain sounds are full at 32, the heavy rain at 128)
s32 Func22A20C(); // 0x0022A20C
// the kind of weather of the current date (3: rain, 4: heavy rain for the sounds)
s32 Func62FF00(); // 0x0062FF00
