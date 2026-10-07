#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17NATRelayInterfaceE @ 0x008CE6D0
// vtable 0x008FD418 (vptr 0x008FD420), offset_to_top 0, 14 entries
//
// The interface nex calls for the NAT relay (pia::inet::NexNatRelayInterface implements it). The
// slot names are the ones of the RTTI symbols; the others and the return types are ours.
class NATRelayInterface : public ::nn::nex::RootObject
{
public:
    NATRelayInterface(); // 0x00387390 | fefates:callgraph [tier C]
    virtual ~NATRelayInterface(); // 0x003873BC slot 0x00
    // 0x003873A0 slot 0x04 (deleting dtor)
    virtual void RegisterRelayClient(nn::nex::NATTraversalRelayClient* pClient) = 0; // slot 0x08 (name is ours)
    virtual void UnregisterRelayClient() = 0; // slot 0x0C
    virtual void vf_0x10() = 0; // slot 0x10
    virtual void RelayProbeRequest(const nn::nex::qList<nn::nex::StationURL>& targets, const nn::nex::StationURL& source) = 0; // slot 0x14
    virtual void CheckCurrentPublicPort(nn::nex::CallContext* pContext, const nn::nex::InetAddress& address, bool flag) = 0; // slot 0x18
    virtual void vf_0x1C() = 0; // slot 0x1C
    virtual void RequestProbe(const nn::nex::StationURL& url) = 0; // slot 0x20
    virtual void ReportNATTraversalResult(const unsigned int& cid, const bool& isSucceeded, const unsigned int& rtt) = 0; // slot 0x24 (name is ours)
    virtual void ReportNATProperties(unsigned int mapping, unsigned int filtering, unsigned int rtt) = 0; // slot 0x28 (name is ours)
    virtual bool vf_0x2C() = 0; // slot 0x2C
    virtual void ReportNATTraversalResultDetail(const unsigned int& cid, const bool& isSucceeded, const nn::nex::NATTraversalResult& result, unsigned int& rtt) = 0; // slot 0x30
    virtual void UpdateConnectionState(unsigned int cid, const nn::nex::StationURL& url) = 0; // slot 0x34
};
} // namespace nex
} // namespace nn
