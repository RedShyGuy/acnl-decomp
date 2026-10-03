#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
class TimeSpan
{
public:
    void DivideNanoSeconds(long long, int) const; // 0x00134718 | fefates:bytes [tier B]

    s64 GetNanoSeconds() const { return mNanoSeconds; }

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
