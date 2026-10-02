#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexNatTraversalProtocolE @ 0x008CF9CC
// vtable 0x009004F8 (vptr 0x00900500), offset_to_top 0, 9 entries
class NexNatTraversalProtocol : public ::nn::pia::transport::Protocol
{
public:
    virtual ~NexNatTraversalProtocol(); // 0x00407AF0 slot 0x00 | fefates:bytes
    // 0x00407AE0 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x0072EFE4 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x0072F530 slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x00407374 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual void IsEnableProtocolFiltering() const; // 0x0072F538 slot 0x20 | slot vf_0x20 of nn::pia::transport::Protocol
    void IsTraversed(const nn::pia::transport::StationLocation&); // 0x004054A4 | fefates:bytes [tier B]
    void IsTraversed(unsigned int); // 0x00405570 | fefates:bytes [tier B]
    void RegisterRelay(nn::pia::inet::NatRelayInterface*); // 0x004055B0 | fefates:bytes [tier B]
    void UnregisterRelay(); // 0x00405A64 | fefates:bytes [tier B]
    void addProbeRequest(const nn::pia::inet::NatProbeRequest&); // 0x00405A98 | fefates:bytes [tier B]
    void sendDummyPacket(const nn::pia::transport::StationLocation&, unsigned char, unsigned char); // 0x00405C54 | fefates:bytes [tier B]
    void ResetProbeStatus(const nn::pia::transport::StationLocation&, bool); // 0x00405D54 | fefates:bytes [tier B]
    void SendProbeRequest(const nn::pia::transport::StationLocation&, const nn::pia::transport::StationLocation&); // 0x00405E3C | fefates:bytes [tier B]
    void getPeer2PeerPort(const nn::pia::transport::StationLocation&); // 0x00405E64 | fefates:bytes [tier B]
    void sendProbeRequests(); // 0x00406508 | fefates:bytes [tier B]
    void CleanupNatTraversal(); // 0x004067F0 | fefates:bytes-fuzzy [tier B]
    void ReportNatProperties(const unsigned int&, const unsigned int&, const unsigned int&); // 0x00406F28 | fefates:bytes [tier B]
    void StartServerKeepAlive(); // 0x00407050 | fefates:bytes [tier B]
    void scheduleProbeRequest(const nn::pia::inet::NatProbeRequest&); // 0x00407090 | fefates:bytes [tier B]
    void GetLatestStationLocation(const nn::pia::transport::StationLocation&, nn::pia::transport::StationLocation*); // 0x0040716C | fefates:bytes [tier B]
    void ReportNatTraversalResult(const nn::pia::transport::StationLocation&, bool); // 0x004071BC | fefates:bytes [tier B]
    void findProbeRequestByConnectionId(unsigned int); // 0x00407240 | fefates:bytes [tier B]
    void eraseProbeRequestByConnectionId(unsigned int); // 0x0040727C | fefates:bytes [tier B]
    NexNatTraversalProtocol(); // 0x00407808 | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
