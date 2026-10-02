#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25MatchMakingProtocolClientE @ 0x008CEEB8
// vtable 0x008FEA30 (vptr 0x008FEA38), offset_to_top 0, 23 entries
class MatchMakingProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    MatchMakingProtocolClient(); // ctor candidate(s) 0x003B82EC (unverified)
    virtual ~MatchMakingProtocolClient(); // 0x003B83E4 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B83D4 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003B7C5C slot 0x50 | fefates:bytes
    virtual void CreateResponder() const; // 0x0072D68C slot 0x54 | slot vf_0x54 of nn::nex::ClientProtocol
    void ProtoReturn_FindByOwner(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003B790C | fefates:bytes [tier B]
    void ProtoReturn_GetSessionURLs(nn::nex::Message*, nn::nex::ProtocolCallContext*); // 0x003B7E18 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
