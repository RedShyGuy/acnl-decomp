#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class ByteOrder
{
public:
    // reverses the byte order
    static u64 Swap64(unsigned long long value); // 0x00429870 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
