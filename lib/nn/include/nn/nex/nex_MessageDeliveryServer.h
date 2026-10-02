#pragma once

#include "decomp.h"
#include "nn/nex/nex__Proto_MessageDeliveryProtocolServer.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21MessageDeliveryServerE @ 0x008CEB18
// vtable 0x008FDE70 (vptr 0x008FDE78), offset_to_top 0, 24 entries
class MessageDeliveryServer : public ::nn::nex::_Proto_MessageDeliveryProtocolServer
{
public:
    MessageDeliveryServer(); // ctor address unknown
    virtual ~MessageDeliveryServer(); // 0x0039A678 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0039A624 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x4C(); // 0x0072CE0C slot 0x4C | virtual slot, introduced by nn::nex::ServerProtocol
    virtual void DispatchProtocolMessage(nn::nex::Message*, nn::nex::Message*, bool*, nn::nex::EndPoint*); // 0x003CC248 slot 0x50 | mk7dlp:callseq
    virtual void vf_0x5C(); // 0x0039A5D0 slot 0x5C | virtual slot, introduced by nn::nex::MessageDeliveryServer
};
} // namespace nex
} // namespace nn
