#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30SecureConnectionProtocolClientE @ 0x008CF1DC
// vtable 0x008FF464 (vptr 0x008FF46C), offset_to_top 0, 23 entries
class SecureConnectionProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    SecureConnectionProtocolClient(); // ctor candidate(s) 0x003C42BC (unverified)
    virtual ~SecureConnectionProtocolClient(); // 0x003C4338 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C4328 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C3C20 slot 0x50 | fefates:bytes
    virtual void CreateResponder() const; // 0x0072DEB0 slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
    void ProtoReturn_RequestConnectionData(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C3CAC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
