#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
class TimeSpan
{
public:
    // the nanoseconds divided by a constant: multiplied by its reciprocal (2^(64 + shift) / divisor)
    s64 DivideNanoSeconds(long long reciprocal, int shift) const; // 0x00134718 | fefates:bytes [tier B]

    s64 GetNanoSeconds() const { return mNanoSeconds; }
    // inline (name is ours): nanoseconds / 1000000
    s64 GetMilliSeconds() const { return DivideNanoSeconds(0x431BDE82D7B634DBLL, 18); }

    // inline (names are ours)
    static TimeSpan FromNanoSeconds(s64 nanoSeconds)
    {
        TimeSpan span;
        span.mNanoSeconds = nanoSeconds;
        return span;
    }
    static TimeSpan FromMicroSeconds(s64 microSeconds) { return FromNanoSeconds(microSeconds * 1000); }
    static TimeSpan FromMilliSeconds(s64 milliSeconds) { return FromNanoSeconds(milliSeconds * 1000000); }

private:
    s64 mNanoSeconds;
};
ASSERT_SIZE(TimeSpan, 8);
} // namespace fnd
} // namespace nn
