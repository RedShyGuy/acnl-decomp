#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23MessagingProtocolClientE @ 0x008CED00
// vtable 0x008FE5AC (vptr 0x008FE5B4), offset_to_top 0, 23 entries
class MessagingProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    MessagingProtocolClient(); // ctor candidate(s) 0x003B11F8 (unverified)
    virtual ~MessagingProtocolClient(); // 0x003B12F0 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B12E0 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003B1174 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
    virtual void CreateResponder() const; // 0x0072D25C slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
};
} // namespace nex
} // namespace nn
