#pragma once

#include "decomp.h"
#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27ClientProtocolRequestBrokerE @ 0x008CEFF4
// vtable 0x008FEED0 (vptr 0x008FEED8), offset_to_top 0, 17 entries
class ClientProtocolRequestBroker : public ::nn::nex::ProtocolRequestBrokerInterface
{
public:
    ClientProtocolRequestBroker(); // ctor address unknown
    virtual ~ClientProtocolRequestBroker(); // 0x003BCBD0 slot 0x00 | slot vf_0x00 of nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x04(); // 0x003BCBAC slot 0x04 | fefates:callseq
    virtual void ProcessMessage(nn::nex::CallProtocolMethodOperation*, nn::nex::EndPoint*, nn::nex::Buffer*); // 0x003BC368 slot 0x08 | fefates:bytes
    virtual void ProcessCallRequest(unsigned short, nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003BC4B8 slot 0x0C | slot vf_0x0C of nn::nex::ProtocolRequestBrokerInterface
    virtual void ProcessCallResponse(unsigned short, nn::nex::Message*, nn::nex::EndPoint*); // 0x003BC9B4 slot 0x10 | fefates:bytes
    virtual void RegisterProtocol(nn::nex::Protocol*); // 0x003BC3F0 slot 0x14 | slot vf_0x14 of nn::nex::ProtocolRequestBrokerInterface
    virtual void UnregisterProtocol(nn::nex::Protocol*); // 0x003BC9B0 slot 0x18 | slot vf_0x18 of nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x1C(); // 0x003BC308 slot 0x1C | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x20(); // 0x003BC310 slot 0x20 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x24(); // 0x003BC9F4 slot 0x24 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x28(); // 0x003BCADC slot 0x28 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x2C(); // 0x003BC3F8 slot 0x2C | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x30(); // 0x003BC384 slot 0x30 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x34(); // 0x003BCAC0 slot 0x34 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x38(); // 0x003BCB90 slot 0x38 | virtual slot, introduced by nn::nex::ProtocolRequestBrokerInterface
    virtual void vf_0x3C(); // 0x003BC3A0 slot 0x3C | virtual slot, introduced by nn::nex::ClientProtocolRequestBroker
    virtual void vf_0x40(); // 0x003BC318 slot 0x40 | virtual slot, introduced by nn::nex::ClientProtocolRequestBroker
};
} // namespace nex
} // namespace nn
