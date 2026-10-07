#pragma once

#include "decomp.h"

// Globals of nex that pia reads (the monitoring data of inet::NatTraverser, the state check of
// inet::NexFacade::Startup). They are defined in nex sources that are not decompiled yet; the
// names and the file are ours.
namespace nn {
namespace nex {
// 2 or 3 while nex is running (NexFacade::Startup requires it)
extern u8 g_Unknown0x96C91E; // 0x0096C91E
extern u8 g_Unknown0x96C91F; // 0x0096C91F
extern u8 g_Unknown0x96C92B; // 0x0096C92B
extern u32 g_Unknown0x96C9C8; // 0x0096C9C8
extern u32 g_Unknown0x96C9D4; // 0x0096C9D4
extern u32 g_Unknown0x96C9D8; // 0x0096C9D8
} // namespace nex
} // namespace nn
