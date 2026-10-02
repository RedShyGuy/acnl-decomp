#pragma once

#include "decomp.h"
#include "nn/nex/nex_MatchMakingClient.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24MatchmakeExtensionClientE @ 0x008CEDF0
// vtable 0x008FE868 (vptr 0x008FE870), offset_to_top 0, 10 entries
class MatchmakeExtensionClient : public ::nn::nex::MatchMakingClient
{
public:
    virtual ~MatchmakeExtensionClient(); // 0x003B4C1C slot 0x00 | fefates:bytes
    // 0x003B4BE0 slot 0x04 | slot vf_0x04 of nn::nex::ServiceClient (deleting dtor)
    virtual void Bind(nn::nex::Credentials*); // 0x003B4A88 slot 0x0C | fefates:bytes
    virtual void Unbind(); // 0x003B4B5C slot 0x10 | fefates:bytes
    MatchmakeExtensionClient(); // 0x003B4B94 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
