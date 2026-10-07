#pragma once

#include "decomp.h"

namespace pead {
// number of set bits (name is ours)
int CountOnes(u32 value); // 0x00538218

// The class and findOnBitFromRight are from the nintendogs symbols (there for sead).
class BitFlagUtil
{
public:
    // the index of the num-th set bit from the right (num from 1), -1 if there are fewer
    static int findOnBitFromRight(u32 value, int num); // 0x0053825C | nintendogs:bytes [tier A]
};
} // namespace pead
