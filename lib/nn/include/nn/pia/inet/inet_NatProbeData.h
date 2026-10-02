#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet12NatProbeDataE @ 0x008CF83C
// vtable 0x008FFEE0 (vptr 0x008FFEE8), offset_to_top 0, 2 entries
class NatProbeData : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x003E4550 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProbeData
    virtual void vf_0x04(); // 0x003E454C slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProbeData
    void Deserialize(const unsigned char*, unsigned int); // 0x003E44B8 | fefates:bytes [tier B]
    NatProbeData(); // 0x003E4528 | fefates:bytes [tier B]
    void Serialize(unsigned char*, unsigned int*, unsigned int) const; // 0x0072EF60 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
