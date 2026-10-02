#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session17SyncClockProtocolE @ 0x008D0014
// vtable 0x0090195C (vptr 0x00901964), offset_to_top 0, 9 entries
class SyncClockProtocol : public ::nn::pia::transport::Protocol
{
public:
    virtual ~SyncClockProtocol(); // 0x004398C8 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x004398A4 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void vf_0x08(); // 0x00733908 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x00733900 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x004394B4 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Cleanup(); // 0x0043949C slot 0x14 | fefates:bytes
    virtual void Dispatch(); // 0x004395DC slot 0x18 | fefates:callseq
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x004393CC slot 0x1C | slot vf_0x1C of nn::pia::transport::Protocol
    void processByClient(const nn::pia::transport::ProtocolMessageReader*); // 0x00439028 | fefates:bytes [tier B]
    SyncClockProtocol(); // 0x00439854 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
