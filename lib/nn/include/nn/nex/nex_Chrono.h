#pragma once

#include "decomp.h"
#include "nn/nex/nex_LocalClock.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// A stopwatch in milliseconds. The names of the members, of the states and of the functions other
// than Check are ours.
// RTTI N2nn3nex6ChronoE @ 0x008CF57C
// vtable 0x008FFA0C (vptr 0x008FFA14), offset_to_top 0, 2 entries
class Chrono : public ::nn::nex::RootObject
{
public:
    enum State
    {
        STATE_RUNNING = 0,
        STATE_STOPPED = 2
    };

    Chrono(); // 0x003D0310
    virtual ~Chrono(); // 0x003D03B0 slot 0x00
    // 0x003D0380 slot 0x04 (deleting dtor)

    // stops the stopwatch; the milliseconds it ran (-1 if it was not running)
    u32 Stop(); // 0x003D01E4
    // the milliseconds it has run
    u32 Check(); // 0x003D0254 | mk7dlp:bytes [tier B]
    bool Start(); // 0x003D029C

    LocalClock m_Clock;  // 0x08
    Time m_StartOffset;  // 0x18, the time of the clock when it was started
    u32 m_Elapsed;       // 0x20
    u32 m_State;         // 0x24
};
ASSERT_SIZE(Chrono, 0x28);
} // namespace nex
} // namespace nn
