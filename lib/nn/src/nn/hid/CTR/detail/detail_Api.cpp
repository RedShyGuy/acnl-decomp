#include "nn/hid/CTR/detail/detail_Api.h"

namespace nn {
namespace hid {
namespace CTR {
namespace detail {
// 0x00354524 | nintendogs:bytes [tier B]
s16 CalculateAccelerationTightly(s16 value, s16 last, s16 play, s16 sensitivity)
{
    int difference = value - last;
    if (difference < -play) {
        return static_cast<s16>(last + (((difference + play) * sensitivity) >> 7));
    }
    if (difference > play) {
        return static_cast<s16>(last + (((difference - play) * sensitivity) >> 7));
    }
    return last;
}

} // namespace detail
} // namespace CTR
} // namespace hid
} // namespace nn
