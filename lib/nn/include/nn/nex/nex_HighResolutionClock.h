#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
class HighResolutionClock
{
public:
    // the microseconds of the system tick
    static u64 GetHighResolutionTime(); // 0x0039333C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
