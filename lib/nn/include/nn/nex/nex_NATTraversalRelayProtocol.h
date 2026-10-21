#pragma once

#include "decomp.h"
#include "nn/nex/nex_NATTraversalProtocolClient.h"
#include "nn/nex/nex_NATTraversalReportInternalProtocolClient.h"
#include "nn/nex/nex__Proto_NATTraversalProtocolServer.h"

namespace nn {
namespace nex {
class Credentials;
class NATTraversalRelayClient;

// The NAT traversal protocol of the relay client: the server side (requests of the server, all
// ignored here; the Impl passes InitiateProbe on) and the client protocol for the reports. The
// names of the members and of slot 0x78 are ours.
// RTTI N2nn3nex25NATTraversalRelayProtocolE @ 0x008CEEC4
// vtable 0x008FEA94 (vptr 0x008FEA9C), offset_to_top 0, 31 entries
class NATTraversalRelayProtocol : public ::nn::nex::_Proto_NATTraversalProtocolServer
{
public:
    // the id of the NAT traversal protocol
    static const u16 PROTOCOL_ID = 3;

    // (inline in NATTraversalRelayClient::Init and CreateNATTraversalRelayProtocol)
    NATTraversalRelayProtocol() : m_pClient(nullptr), m_ReportClient(0)
    {
        m_pClient = new NATTraversalProtocolClient(0);
        m_pClient->m_ProtocolId = PROTOCOL_ID;
    }
    virtual ~NATTraversalRelayProtocol(); // 0x003B846C slot 0x00
    // 0x003B8414 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void RequestProbeInitiation(const nn::nex::qList<nn::nex::StationURL>& targets); // 0x003B8404 slot 0x5C
    virtual void InitiateProbe(const nn::nex::StationURL& url); // 0x003B83E8 slot 0x60
    virtual void RequestProbeInitiationExt(const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source); // 0x003B840C slot 0x64
    virtual void ReportNATTraversalResult(const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt); // 0x003B8408 slot 0x68
    virtual void ReportNATProperties(const unsigned int& mapping, const unsigned int& filtering, const unsigned int& rtt); // 0x003B83EC slot 0x6C
    virtual void GetRelayParameters(unsigned int* pMode, nn::nex::DateTime* pTime, nn::nex::String* pAddress, unsigned short* pPort, unsigned int* pAddressType,
                                    unsigned int* pServerTime); // 0x003B83F0 slot 0x70
    virtual void ReportNATTraversalResultDetail(const unsigned int& cid, const bool& isSucceeded, const nn::nex::NATTraversalResult& detail, const unsigned int& rtt); // 0x003B8410 slot 0x74
    // the default credentials of the client protocol
    virtual void SetDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x003B83F4 slot 0x78

    NATTraversalRelayClient* m_pOwner;                     // 0x40 (set by the relay client)
    NATTraversalProtocolClient* m_pClient;                 // 0x44
    NATTraversalReportInternalProtocolClient m_ReportClient; // 0x48
    u32 m_Unknown0x8C;                                     // 0x8C (not used in this build)
};
ASSERT_OFFSET(NATTraversalRelayProtocol, m_ReportClient, 0x48);
ASSERT_SIZE(NATTraversalRelayProtocol, 0x90);
} // namespace nex
} // namespace nn
