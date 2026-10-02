#pragma once

#include "decomp.h"
#include "nn/hid/CTR/hid_HidBase.h"

namespace nn {
namespace hid {
namespace CTR {
// RTTI N2nn3hid3CTR9GyroscopeE @ 0x008CDF18
// vtable 0x008FC120 (vptr 0x008FC128), offset_to_top 0, 2 entries
class Gyroscope : public ::nn::hid::CTR::HidBase
{
public:
    Gyroscope(); // ctor candidate(s) 0x00785770 (unverified)
    virtual void vf_0x00(); // 0x003546C8 slot 0x00 | virtual slot, introduced by nn::hid::CTR::Gyroscope
    virtual void vf_0x04(); // 0x00354698 slot 0x04 | virtual slot, introduced by nn::hid::CTR::Gyroscope
};
} // namespace CTR
} // namespace hid
} // namespace nn
