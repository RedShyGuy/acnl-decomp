#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex32MatchmakeExtensionProtocolClientE @ 0x008CF290
// vtable 0x008FF57C (vptr 0x008FF584), offset_to_top 0, 23 entries
class MatchmakeExtensionProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    MatchmakeExtensionProtocolClient(); // ctor candidate(s) 0x003CA4F8 (unverified)
    virtual ~MatchmakeExtensionProtocolClient(); // 0x003CA5F0 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003CA5E0 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C7E24 slot 0x50 | slot vf_0x50 of nn::nex::ClientProtocol
    virtual void CreateResponder() const; // 0x0072DEDC slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
    void ProtoReturn_AutoMatchmakeWithSearchCriteria_Postpone(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003C8E28 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
