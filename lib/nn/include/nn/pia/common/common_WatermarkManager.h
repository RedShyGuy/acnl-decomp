#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common16WatermarkManagerE @ 0x008CFE94
// vtable 0x009015AC (vptr 0x009015B4), offset_to_top 0, 1 entries
class WatermarkManager : public ::nn::pia::common::RootObject
{
public:
    WatermarkManager(); // ctor candidate(s) 0x00427F84 (unverified)
    virtual void vf_0x00(); // 0x00731AFC slot 0x00 | virtual slot, introduced by nn::pia::common::WatermarkManager
    void GetWatermark(int); // 0x00427F64 | fefates:bytes [tier B]
    void DestroyInstance(); // 0x00427F84 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
