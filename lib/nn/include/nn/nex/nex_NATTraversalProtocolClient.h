#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex26NATTraversalProtocolClientE @ 0x008CEF88
// vtable 0x008FEDD4 (vptr 0x008FEDDC), offset_to_top 0, 23 entries
class NATTraversalProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    NATTraversalProtocolClient(); // ctor candidate(s) 0x003BA68C (unverified)
    virtual ~NATTraversalProtocolClient(); // 0x003BA784 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003BA774 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003BA308 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
    virtual void CreateResponder() const; // 0x0072DDB8 slot 0x54 | fefates:callseq
};
} // namespace nex
} // namespace nn
