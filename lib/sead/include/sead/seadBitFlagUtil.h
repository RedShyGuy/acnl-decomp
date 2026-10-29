#pragma once

#include "decomp.h"

namespace sead {
// Helpers for bit masks (the class and function names are ours).
class BitFlagUtil
{
public:
    // the number of set bits
    static s32 countOnBit(u32 x); // 0x0053FD88
};
} // namespace sead
