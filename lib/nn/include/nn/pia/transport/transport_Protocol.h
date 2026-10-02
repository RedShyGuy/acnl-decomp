#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport8ProtocolE @ 0x008D02B4
// vtable 0x00902050 (vptr 0x00902058), offset_to_top 0, 9 entries
class Protocol : public ::nn::pia::common::RootObject
{
public:
    virtual ~Protocol(); // 0x0045FA58 slot 0x00 | fefates:callgraph
    // 0x0045FA50 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x00736E80 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void Startup(nn::pia::StationIndex); // 0x0045FA18 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Cleanup(); // 0x0045F9D8 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x0045FA20 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x0045F9D0 slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
    virtual void IsEnableProtocolFiltering() const; // 0x00736E78 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
    void SetPort(unsigned short); // 0x0045F9DC | fefates:bytes [tier B]
    Protocol(); // 0x0045FA28 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
