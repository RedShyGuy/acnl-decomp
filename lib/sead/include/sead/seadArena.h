#pragma once

#include "decomp.h"

namespace sead {
// The memory of the root heaps: initialize takes the device memory of the process (the members are
// ours; layout from the constructor and initialize).
class Arena
{
public:
    Arena(); // 0x0055D0F8 (name is ours)
    ~Arena(); // 0x0055D10C (name is ours)
    void initialize(u32 size); // 0x00139110 | nintendogs:bytes [tier A]

    // set by initialize: the device memory is used (name is ours)
    static bool s_IsDeviceMemoryUsed;

    u8* mStart;          // 0x0
    u32 mSize;           // 0x4
    bool mIsInitialized; // 0x8 (not set by initialize)
};
ASSERT_SIZE(Arena, 0xC);
} // namespace sead
