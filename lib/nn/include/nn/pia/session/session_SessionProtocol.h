#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session15SessionProtocolE @ 0x008CFFB4
// vtable 0x00901850 (vptr 0x00901858), offset_to_top 0, 9 entries
class SessionProtocol : public ::nn::pia::transport::Protocol
{
public:
    SessionProtocol(); // ctor candidate(s) 0x00436C64 (unverified)
    virtual ~SessionProtocol(); // 0x00436CA4 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x00436C94 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x007338E4 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x007338DC slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x0043687C slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Cleanup(); // 0x00436810 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x004368CC slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x004350EC slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
};
} // namespace session
} // namespace pia
} // namespace nn
