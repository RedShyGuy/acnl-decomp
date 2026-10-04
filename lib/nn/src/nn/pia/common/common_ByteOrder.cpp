#include "nn/pia/common/common_ByteOrder.h"

namespace nn {
namespace pia {
namespace common {
// 0x00429870 | fefates:bytes [tier B]
u64 nn::pia::common::ByteOrder::Swap64(unsigned long long value)
{
    value = ((value & 0xFF00FF00FF00FF00ull) >> 8) | ((value & 0x00FF00FF00FF00FFull) << 8);
    value = ((value & 0xFFFF0000FFFF0000ull) >> 16) | ((value & 0x0000FFFF0000FFFFull) << 16);
    return (value >> 32) | (value << 32);
}

} // namespace common
} // namespace pia
} // namespace nn
