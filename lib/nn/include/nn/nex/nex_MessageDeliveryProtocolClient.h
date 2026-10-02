#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29MessageDeliveryProtocolClientE @ 0x008CF13C
// vtable 0x008FF24C (vptr 0x008FF254), offset_to_top 0, 23 entries
class MessageDeliveryProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    MessageDeliveryProtocolClient(); // ctor candidate(s) 0x003C10E4 (unverified)
    virtual ~MessageDeliveryProtocolClient(); // 0x003C11DC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C11CC slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C105C slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
    virtual void CreateResponder() const; // 0x0072DE8C slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
};
} // namespace nex
} // namespace nn
