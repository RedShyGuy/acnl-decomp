#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace fnd {
// 0x00134718 | fefates:bytes [tier B]
s64 nn::fnd::TimeSpan::DivideNanoSeconds(long long reciprocal, int shift) const
{
    // the high 64 bits of the 128 bit product mNanoSeconds * reciprocal (signed), in 32 bit parts
    s64 value = mNanoSeconds;
    s64 valueHigh = value >> 32;
    u64 valueLow = static_cast<u32>(value);
    s64 reciprocalHigh = reciprocal >> 32;
    u64 reciprocalLow = static_cast<u32>(reciprocal);
    s64 middle1 = valueHigh * static_cast<s64>(reciprocalLow) + static_cast<s64>((valueLow * reciprocalLow) >> 32);
    s64 middle2 = static_cast<s64>(valueLow) * reciprocalHigh;
    s64 high = valueHigh * reciprocalHigh + ((middle1 >> 32) + ((middle2 + static_cast<u32>(middle1)) >> 32));
    // a reciprocal of 2^63 or more is meant unsigned
    if (reciprocal < 0) {
        high += value;
    }
    high >>= shift;
    // rounds towards zero
    return high + (static_cast<u64>(value) >> 63);
}

} // namespace fnd
} // namespace nn
