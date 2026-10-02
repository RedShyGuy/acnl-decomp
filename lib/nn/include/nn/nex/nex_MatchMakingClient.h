#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17MatchMakingClientE @ 0x008CE6C4
// vtable 0x008FD3E8 (vptr 0x008FD3F0), offset_to_top 0, 10 entries
class MatchMakingClient : public ::nn::nex::ServiceClient
{
public:
    MatchMakingClient(); // ctor address unknown
    virtual ~MatchMakingClient(); // 0x0038735C slot 0x00 | fefates:bytes
    // 0x00387324 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials*); // 0x00387088 slot 0x20 | fefates:bytes-fuzzy
    void EndParticipation(nn::nex::ProtocolCallContext*, unsigned, const nn::nex::String&); // 0x00386FD4 | mk7dlp:bytes-fuzzy [tier A]
    void UnregisterGathering(nn::nex::ProtocolCallContext*, unsigned int); // 0x00387030 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
