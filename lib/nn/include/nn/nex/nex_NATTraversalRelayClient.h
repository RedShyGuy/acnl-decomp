#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23NATTraversalRelayClientE @ 0x008CED0C
// vtable 0x008FE610 (vptr 0x008FE618), offset_to_top 0, 11 entries
class NATTraversalRelayClient : public ::nn::nex::ServiceClient
{
public:
    virtual ~NATTraversalRelayClient(); // 0x003B160C slot 0x00 | fefates:bytes
    // 0x003B15A8 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void IsConnected() const; // 0x0072D280 slot 0x14 | fefates:bytes
    virtual void ConnectionStateHasChanged(); // 0x003B1310 slot 0x1C | mk7dlp:callseq
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x003B1430 slot 0x20 | fefates:bytes
    virtual void CreateNATTraversalRelayProtocol(); // 0x003B13C0 slot 0x28 | fefates:bytes
    void Init(); // 0x003B14A8 | fefates:bytes-fuzzy [tier B]
    NATTraversalRelayClient(); // 0x003B1580 | fefates:bytes [tier B]
    // (return types are ours)
    bool CallReportNATProperties(nn::nex::ProtocolCallContext* pContext, const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt); // 0x003B12F4 | fefates:callgraph [tier C]
    bool CallReportNATTraversalResult(nn::nex::ProtocolCallContext* pContext, const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt); // 0x003B13A4 | fefates:callgraph [tier C]
    bool CallRequestProbeInitiationExt(nn::nex::ProtocolCallContext* pContext, const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source); // 0x003BA4F0 | fefates:callgraph [tier C]

    // (only the member pia uses; the name is ours)
    u32 m_Unknown0xC;                          // 0x0C
    NATRelayInterface* m_pRelayInterface;      // 0x10
};
} // namespace nex
} // namespace nn
