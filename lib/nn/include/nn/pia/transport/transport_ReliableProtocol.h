#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport16ReliableProtocolE @ 0x008D01C4
// vtable 0x00901E38 (vptr 0x00901E40), offset_to_top 0, 9 entries
class ReliableProtocol : public ::nn::pia::transport::Protocol
{
public:
    virtual ~ReliableProtocol(); // 0x0045311C slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x00453084 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void vf_0x08(); // 0x007356D0 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x00735668 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x00452CEC slot 0x10 | fefates:bytes-fuzzy
    virtual void Cleanup(); // 0x00452C18 slot 0x14 | fefates:bytes
    virtual void Dispatch(); // 0x00452D4C slot 0x18 | fefates:callseq
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x00452924 slot 0x1C | fefates:bytes
    void Initialize(unsigned int, unsigned int); // 0x00452664 | fefates:bytes [tier B]
    void ReceiveImpl(nn::pia::StationIndex*, void*, unsigned int*, unsigned int, bool); // 0x004527C0 | fefates:bytes [tier B]
    void Send(nn::pia::StationId, const void*, unsigned int); // 0x00452BCC | fefates:bytes [tier B]
    void Receive(nn::pia::StationId*, void*, unsigned int*, unsigned int); // 0x00452C84 | fefates:bytes [tier B]
    void Finalize(); // 0x00452F48 | fefates:bytes [tier B]
    ReliableProtocol(); // 0x00452FC4 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
