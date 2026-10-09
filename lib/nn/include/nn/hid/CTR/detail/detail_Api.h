#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
namespace detail {
// a raw value of the accelerometer, smoothed: changes within play from the last value are
// ignored, the rest is scaled by sensitivity / 128 (parameter names are ours)
s16 CalculateAccelerationTightly(s16 value, s16 last, s16 play, s16 sensitivity); // 0x00354524 | nintendogs:bytes [tier B]
} // namespace detail
} // namespace CTR
} // namespace hid
} // namespace nn
