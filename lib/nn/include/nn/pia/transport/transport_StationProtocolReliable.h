#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport23StationProtocolReliableE @ 0x008D0278
// vtable 0x00901FB8 (vptr 0x00901FC0), offset_to_top 0, 9 entries
class StationProtocolReliable : public ::nn::pia::transport::Protocol
{
public:
    virtual ~StationProtocolReliable(); // 0x0045D454 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x0045D444 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x00736778 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x00736770 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x0045D2DC slot 0x10 | fefates:bytes
    virtual void Cleanup(); // 0x0045D1A0 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x0045D2F4 slot 0x18 | fefates:bytes
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x0045D0E8 slot 0x1C | fefates:bytes
    void Receive(nn::pia::StationIndex, unsigned int, unsigned char*, unsigned int*, nn::pia::common::StationAddress*); // 0x0045D1AC | fefates:bytes [tier B]
    StationProtocolReliable(); // 0x0045D388 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
