#include "pead/peadBitUtil.h"

namespace pead {
// 0x00538218
int CountOnes(u32 value)
{
    value = value - ((value >> 1) & 0x55555555);
    value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
    value = (value + (value >> 4)) & 0x0F0F0F0F;
    value += value >> 8;
    value += value >> 16;
    return value & 0x3F;
}

// 0x0053825C | nintendogs:bytes [tier A]
int pead::BitFlagUtil::findOnBitFromRight(u32 value, int num)
{
    if (value == 0) {
        return -1;
    }
    for (num--; num > 0; num--) {
        value &= value - 1;
        if (value == 0) {
            return -1;
        }
    }
    return CountOnes((value & -value) - 1);
}
} // namespace pead
