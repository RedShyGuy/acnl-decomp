#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
class NATRelayInterface;
class NATTraversalRelayProtocol;
class ProtocolCallContext;
class StationURL;
template <typename T0>
class qList;

// The client of the NAT traversal protocol for a relay interface (pia's NAT relay): the reports
// go to the server, the probe requests of the server to the interface. The member names are ours.
// RTTI N2nn3nex23NATTraversalRelayClientE @ 0x008CED0C
// vtable 0x008FE610 (vptr 0x008FE618), offset_to_top 0, 11 entries
class NATTraversalRelayClient : public ::nn::nex::ServiceClient
{
public:
    // the id of the service client (and of the protocol)
    static const u16 PROTOCOL_ID = 3;

    NATTraversalRelayClient(); // 0x003B1580 | fefates:bytes [tier B]
    virtual ~NATTraversalRelayClient(); // 0x003B160C slot 0x00 | fefates:bytes
    // 0x003B15A8 slot 0x04 (deleting dtor)
    virtual bool IsConnected() const; // 0x0072D280 slot 0x14 | fefates:bytes
    virtual void ConnectionStateHasChanged(); // 0x003B1310 slot 0x1C | mk7dlp:callseq
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x003B1430 slot 0x20 | fefates:bytes
    virtual nn::nex::NATTraversalRelayProtocol* CreateNATTraversalRelayProtocol(); // 0x003B13C0 slot 0x28 | fefates:bytes
    // the protocols are registered and the relay protocol is created
    void Init(); // 0x003B14A8 | fefates:bytes-fuzzy [tier B]
    // (the return types are ours)
    bool CallReportNATProperties(nn::nex::ProtocolCallContext* pContext, const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt); // 0x003B12F4 | fefates:callgraph [tier C]
    bool CallReportNATTraversalResult(nn::nex::ProtocolCallContext* pContext, const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt); // 0x003B13A4 | fefates:callgraph [tier C]
    // (a nop in the original that falls into the call of the protocol client)
    bool CallRequestProbeInitiationExt(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source); // 0x003BA4F0 | fefates:callgraph [tier C]

    NATRelayInterface* m_pRelayInterface;        // 0x10
    NATTraversalRelayProtocol* m_pRelayProtocol; // 0x14
};
ASSERT_SIZE(NATTraversalRelayClient, 0x18);
} // namespace nex
} // namespace nn
