#pragma once

#include "decomp.h"
#include "nn/nex/nex_NATRelayInterface.h"

namespace nn {
namespace nex {
class NATTraversalRelayClient;
class ProtocolCallContext;
} // namespace nex
namespace pia {
namespace inet {
class NatRelayInterface;

// RTTI N2nn3pia4inet20NexNatRelayInterfaceE @ 0x008CF96C
// vtable 0x00900388 (vptr 0x00900390), offset_to_top 0, 15 entries
//
// The NAT relay interface that nex calls (NexNatRelay::m_RelayInterface): it passes the requests
// of the server on to the NatRelayInterface of pia and the reports of pia to the relay client of
// nex. The member names are ours.
class NexNatRelayInterface : public ::nn::nex::NATRelayInterface
{
public:
    // (inline in the constructor of NexNatRelay)
    NexNatRelayInterface() : m_pRelayClient(nullptr), m_pNatRelay(nullptr), m_pCallContext(nullptr) {}
    // (armlink placed it in front of the destructor of the base)
    virtual ~NexNatRelayInterface(); // 0x003873B8 slot 0x00
    // 0x00401560 slot 0x04 (deleting dtor)
    virtual void RegisterRelayClient(nn::nex::NATTraversalRelayClient* pClient); // 0x00401480 slot 0x08
    virtual void UnregisterRelayClient(); // 0x004014E0 slot 0x0C | slot vf_0x0C of nn::nex::NATRelayInterface
    virtual void vf_0x10(); // 0x00401438 slot 0x10
    virtual void RelayProbeRequest(const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source); // 0x0040143C slot 0x14 | fefates:bytes
    virtual void CheckCurrentPublicPort(nn::nex::CallContext* pContext, const nn::nex::InetAddress& address, bool flag); // 0x00401508 slot 0x18 | slot vf_0x18 of nn::nex::NATRelayInterface
    virtual void vf_0x1C(); // 0x00401558 slot 0x1C
    virtual void RequestProbe(const nn::nex::StationURL& url); // 0x004013C8 slot 0x20 | fefates:bytes
    virtual void ReportNATTraversalResult(const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt); // 0x0040150C slot 0x24
    virtual void ReportNATProperties(unsigned int mapping, unsigned int filtering, unsigned int rtt); // 0x0040148C slot 0x28
    virtual bool vf_0x2C(); // 0x004014D8 slot 0x2C
    virtual void ReportNATTraversalResultDetail(const unsigned int& cid, const bool& isSucceeded, const nn::nex::NATTraversalResult& result, unsigned int& rtt); // 0x0040155C slot 0x30 | slot vf_0x30 of nn::nex::NATRelayInterface
    virtual void UpdateConnectionState(unsigned int cid, const nn::nex::StationURL& url); // 0x004014EC slot 0x34 | fefates:bytes
    // (name is ours)
    virtual void Setup(nn::pia::inet::NatRelayInterface* pNatRelay, nn::nex::ProtocolCallContext* pContext); // 0x0040142C slot 0x38

    nex::NATTraversalRelayClient* m_pRelayClient;   // 0x04
    NatRelayInterface* m_pNatRelay;                 // 0x08
    nex::ProtocolCallContext* m_pCallContext;       // 0x0C
};
ASSERT_SIZE(NexNatRelayInterface, 0x10);
} // namespace inet
} // namespace pia
} // namespace nn
