#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace common {
class ByteOrder
{
public:
    void Swap64(unsigned long long); // 0x00429870 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
