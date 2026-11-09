#pragma once

#include "decomp.h"

namespace sead {
class Heap;
} // namespace sead

namespace netgame {
// Globals of the game that netgame reads; they are defined in dGameGlobals.cpp until their files are
// decompiled (all names are ours).

// the state of the tour
struct TourStatus
{
    u8 state;    // 0x0
    s8 tourName; // 0x1
    u8 unk_0x2[2];
    u8 unk_0x4;
};

extern TourStatus g_TourStatus;
extern void* g_pUnknown954238;
extern volatile u8 g_PlayerFlags;
extern const void* g_pUnknown94EB2C;
extern void* g_pUnknown94FD84;
extern void* g_pUnknown954884;
extern void* g_pUnknown95488C;
extern u8 g_UnknownA83000[];
extern u8 g_UnknownAAFF14;
extern u8 g_EnteringPlayer;
extern sead::Heap* g_pNetGameHeap;
} // namespace netgame
