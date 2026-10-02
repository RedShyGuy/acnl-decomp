#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet11NatPropertyE @ 0x008CF824
// vtable 0x008FFEA4 (vptr 0x008FFEAC), offset_to_top 0, 3 entries
class NatProperty : public ::nn::pia::common::RootObject
{
public:
    virtual void vf_0x00(); // 0x003E3B38 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatProperty
    virtual void vf_0x04(); // 0x003E3B34 slot 0x04 | virtual slot, introduced by nn::pia::inet::NatProperty
    virtual void vf_0x08(); // 0x003E3B04 slot 0x08 | virtual slot, introduced by nn::pia::inet::NatProperty
    void SetNexNatProperties(const nn::nex::NATProperties&); // 0x003E3A4C | fefates:bytes [tier B]
    NatProperty(); // 0x003E3B08 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
