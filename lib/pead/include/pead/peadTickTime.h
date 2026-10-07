#pragma once

// pead::TickTime - a point in time in system ticks (the class and its constructor are from the
// nintendogs symbols, there for sead). The member name is ours.

#include "decomp.h"

namespace pead {
class TickTime
{
public:
    // the system tick count now (svc GetSystemTick)
    TickTime(); // 0x0053D960 | nintendogs:bytes [tier B]

    u64 mTick; // 0x0
};
} // namespace pead
