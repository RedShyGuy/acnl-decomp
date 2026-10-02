#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex28TicketGrantingProtocolClientE @ 0x008CF0C0
// vtable 0x008FF0C4 (vptr 0x008FF0CC), offset_to_top 0, 23 entries
class TicketGrantingProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    TicketGrantingProtocolClient(); // ctor candidate(s) 0x003BF6DC (unverified)
    virtual ~TicketGrantingProtocolClient(); // 0x003BF758 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003BF748 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003BF08C slot 0x50 | fefates:bytes
    virtual void CreateResponder() const; // 0x0072DE60 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
    void ProtoReturn_RequestTicket(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003BEE84 | fefates:bytes [tier B]
    void ProtoReturn_LoginWithContext(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003BF118 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
