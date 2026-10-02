#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local9UdsHandleE @ 0x008CFDF0
// vtable 0x009014FC (vptr 0x00901504), offset_to_top 0, 2 entries
class UdsHandle : public ::nn::pia::common::RootObject
{
public:
    UdsHandle(); // ctor candidate(s) 0x00425F64 (unverified)
    virtual void vf_0x00(); // 0x00425F84 slot 0x00 | virtual slot, introduced by nn::pia::local::UdsHandle
    virtual void vf_0x04(); // 0x00425F80 slot 0x04 | virtual slot, introduced by nn::pia::local::UdsHandle
    void CreateHandle(unsigned short); // 0x00425E80 | fefates:bytes [tier B]
    void DestroyHandle(); // 0x00425F18 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
