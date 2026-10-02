#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport11RttProtocolE @ 0x008D0158
// vtable 0x00901D48 (vptr 0x00901D50), offset_to_top 0, 9 entries
class RttProtocol : public ::nn::pia::transport::Protocol
{
public:
    virtual ~RttProtocol(); // 0x0044DAA8 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x0044DA98 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x00734B78 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x00734B70 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x0044D72C slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Cleanup(); // 0x0044D6D0 slot 0x14 | fefates:bytes
    virtual void Dispatch(); // 0x0044D778 slot 0x18 | fefates:callseq
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x0044D4A4 slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
    void Finalize(); // 0x0044DA2C | fefates:bytes [tier B]
    RttProtocol(); // 0x0044DA6C | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
