#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
class TimeSpan
{
public:
    void DivideNanoSeconds(long long, int) const; // 0x00134718 | fefates:bytes [tier B]

    s64 GetNanoSeconds() const { return mNanoSeconds; }

private:
    s64 mNanoSeconds;
};
ASSERT_SIZE(TimeSpan, 8);
} // namespace fnd
} // namespace nn
