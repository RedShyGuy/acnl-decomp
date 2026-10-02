#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_Protocol.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session12MeshProtocolE @ 0x008CFF54
// vtable 0x00901680 (vptr 0x00901688), offset_to_top 0, 9 entries
class MeshProtocol : public ::nn::pia::transport::Protocol
{
public:
    MeshProtocol(); // ctor candidate(s) 0x00430918 (unverified)
    virtual ~MeshProtocol(); // 0x0045FA54 slot 0x00 | slot vf_0x00 of nn::pia::transport::Protocol
    // 0x00430A24 slot 0x04 | slot vf_0x04 of nn::pia::transport::Protocol (deleting dtor)
    virtual void vf_0x08(); // 0x00733868 slot 0x08 | virtual slot, introduced by nn::pia::transport::Protocol
    virtual void GetProtocolType() const; // 0x0073383C slot 0x0C | slot vf_0x0C of nn::pia::transport::Protocol
    virtual void Startup(nn::pia::StationIndex); // 0x0045FA14 slot 0x10 | slot vf_0x10 of nn::pia::transport::Protocol
    virtual void Dispatch(); // 0x0042FB30 slot 0x18 | slot vf_0x18 of nn::pia::transport::Protocol
    virtual void UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent&); // 0x0042E8F8 slot 0x1C | fefates:callseq
    void ParseHelper(const nn::pia::transport::ReceivedMessageAccessor&); // 0x0042CE28 | fefates:bytes-fuzzy [tier B]
    void SendGreeting(nn::pia::StationIndex); // 0x0042D7C4 | fefates:bytes-fuzzy [tier B]
    void SendDestroyMesh(nn::pia::StationIndex); // 0x0042DDB8 | fefates:bytes-fuzzy [tier B]
    void ParseJoinRequest(const nn::pia::transport::ReceivedMessageAccessor&); // 0x0042DEFC | fefates:bytes [tier B]
    void SendLeaveResponse(nn::pia::transport::Station*); // 0x0042E3EC | fefates:bytes-fuzzy [tier B]
    void MakeJoinRequestData(unsigned char*); // 0x0042E648 | fefates:bytes [tier B]
    void SendMigrationFinish(bool); // 0x0042E7FC | fefates:bytes-fuzzy [tier B]
    void SendConnectionReport(nn::pia::StationIndex, unsigned int, unsigned int, unsigned int*); // 0x0042ED90 | fefates:bytes-fuzzy [tier B]
    void SendRelayRouteDirections(); // 0x0042F518 | fefates:bytes-fuzzy [tier B]
    void SendConnectionFailureNotice(nn::pia::StationIndex, nn::pia::StationIndex, nn::pia::StationIndex, unsigned char, unsigned int); // 0x0042F68C | fefates:bytes-fuzzy [tier B]
    void SendMultiMigrationRankDecision(nn::pia::StationIndex, long long, unsigned int*); // 0x0042F89C | fefates:bytes-fuzzy [tier B]
    void GetJoinRequestDataSize() const; // 0x00733844 | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
