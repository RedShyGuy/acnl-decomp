#pragma once

#include "decomp.h"
#include "nn/nex/nex__Proto_NATTraversalProtocolServer.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25NATTraversalRelayProtocolE @ 0x008CEEC4
// vtable 0x008FEA94 (vptr 0x008FEA9C), offset_to_top 0, 31 entries
class NATTraversalRelayProtocol : public ::nn::nex::_Proto_NATTraversalProtocolServer
{
public:
    NATTraversalRelayProtocol(); // ctor candidate(s) 0x003B14A8 (unverified)
    virtual ~NATTraversalRelayProtocol(); // 0x003B846C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B8414 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003CB194 slot 0x50 | fefates:bytes-fuzzy
    virtual void RequestProbeInitiation(const nn::nex::qList<nn::nex::StationURL>&); // 0x003B8404 slot 0x5C | slot vf_0x5C of nn::nex::NATTraversalRelayProtocol
    virtual void InitiateProbe(const nn::nex::StationURL&); // 0x003B83E8 slot 0x60 | slot vf_0x60 of nn::nex::NATTraversalRelayProtocol
    virtual void RequestProbeInitiationExt(const nn::nex::qList<nn::nex::StationURL>&, const nn::nex::StationURL&); // 0x003B840C slot 0x64 | slot vf_0x64 of nn::nex::NATTraversalRelayProtocol
    virtual void ReportNATTraversalResult(const unsigned int&, const bool&, const unsigned int&); // 0x003B8408 slot 0x68 | slot vf_0x68 of nn::nex::NATTraversalRelayProtocol
    virtual void vf_0x6C(); // 0x003B83EC slot 0x6C | virtual slot, introduced by nn::nex::NATTraversalRelayProtocol
    virtual void vf_0x70(); // 0x003B83F0 slot 0x70 | virtual slot, introduced by nn::nex::NATTraversalRelayProtocol
    virtual void vf_0x74(); // 0x003B8410 slot 0x74 | virtual slot, introduced by nn::nex::NATTraversalRelayProtocol
    virtual void vf_0x78(); // 0x003B83F4 slot 0x78 | virtual slot, introduced by nn::nex::NATTraversalRelayProtocol
};
} // namespace nex
} // namespace nn
