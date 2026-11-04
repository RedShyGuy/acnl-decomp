#pragma once

#include "decomp.h"

namespace applet {
// The state of the game around library applets (game code that is not decompiled yet; the names are
// ours, the meaning of the state byte 0x0094F3B8 is unknown).

// whether the state byte is 2 or more (the camera of ModuleAmiiboCamera runs then)
bool IsStateAtLeast2(); // 0x0011EF18
// validates the whole GPU state again after a library applet
void RestoreGpuState(); // 0x005CF7CC
} // namespace applet
