#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class TimeSpan
{
public:
    void GetTicksPerSec(); // 0x00429784 | fefates:bytes [tier B]
    void GetTicksPerMSec(); // 0x00429824 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
