#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport24BandwidthCheckerProtocolE @ 0x008D0284
// vtable 0x00901FE4 (vptr 0x00901FEC), offset_to_top 0, 9 entries
class BandwidthCheckerProtocol : public ::nn::pia::transport::Protocol
{
public:
    BandwidthCheckerProtocol(); // ctor candidate(s) 0x0045E360 (unverified)
    virtual ~BandwidthCheckerProtocol(); // 0x0045E418 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x0045E3E8 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x007367B4 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x007367A4 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x0045E23C slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual void IsEnableProtocolFiltering() const; // 0x007367AC slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
};
} // namespace transport
} // namespace pia
} // namespace nn
