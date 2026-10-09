#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_HidBase.h"

namespace nn {
namespace hidlow {
namespace CTR {
class GyroscopeLowLifoRing;
} // namespace CTR
} // namespace hidlow

namespace hid {
namespace CTR {
// RTTI N2nn3hid3CTR9GyroscopeE @ 0x008CDF18
// vtable 0x008FC120 (vptr 0x008FC128), offset_to_top 0, 2 entries
class Gyroscope : public ::nn::hid::CTR::HidBase
{
public:
    Gyroscope() {}
    virtual ~Gyroscope();

    nn::hidlow::CTR::GyroscopeLowLifoRing* m_pRing; // 0x8 (name is ours)
};
ASSERT_SIZE(Gyroscope, 0xC);
} // namespace CTR
} // namespace hid
} // namespace nn
