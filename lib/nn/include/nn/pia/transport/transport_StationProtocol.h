#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport15StationProtocolE @ 0x008D01B8
// vtable 0x00901E0C (vptr 0x00901E14), offset_to_top 0, 9 entries
class StationProtocol : public ::nn::pia::transport::Protocol
{
public:
    virtual ~StationProtocol(); // 0x00452660 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x00452650 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x00735664 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x0073551C slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x00452440 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Cleanup(); // 0x00452324 slot 0x14 | slot vf_0x14 of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x00452450 slot 0x18 | fefates:bytes
    virtual void IsEnableProtocolFiltering() const; // 0x00735524 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
    void ParseHelper(const nn::pia::transport::ReceivedMessageAccessor&); // 0x00451458 | fefates:bytes-fuzzy [tier B]
    void SendDisconnectionRequest(nn::pia::StationIndex, bool); // 0x004515E4 | fefates:bytes [tier B]
    void SendDisconnectionRequest(const nn::pia::common::StationAddress&); // 0x0045164C | fefates:bytes [tier B]
    void ParseDisconnectionRequest(const nn::pia::transport::ReceivedMessageAccessor&); // 0x004516B0 | fefates:bytes [tier B]
    void SendDenyingConnectionResponse(const nn::pia::common::StationAddress&, unsigned char); // 0x0045229C | fefates:bytes [tier B]
    void SendAck(unsigned int, nn::pia::StationIndex); // 0x00452330 | fefates:bytes [tier B]
    void SendAck(unsigned int, const nn::pia::common::StationAddress&); // 0x004523B8 | fefates:bytes [tier B]
    StationProtocol(); // 0x0045262C | fefates:bytes [tier B]
    void MakeConnectionRequestData(unsigned char*, unsigned char, bool, bool) const; // 0x0073552C | fefates:bytes [tier B]
    void MakeConnectionResponseData(unsigned char*, bool) const; // 0x00735598 | fefates:bytes [tier B]
    void GetConnectionRequestDataSize() const; // 0x00735640 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn
