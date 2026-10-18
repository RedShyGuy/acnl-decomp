#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class CallContext;

// The access to a game server without a nex login: a JobIndependentServer gets the service token of
// the game server from the friends service locator. The names of the members and of the functions
// other than the constructor are ours.
// RTTI N2nn3nex17IndependentServerE @ 0x008CE6B8
// vtable 0x008FD3D8 (vptr 0x008FD3E0), offset_to_top 0, 2 entries
class IndependentServer : public ::nn::nex::RootObject
{
public:
    IndependentServer(); // 0x00386F30
    virtual ~IndependentServer(); // 0x00386F78 slot 0x00
    // 0x00386F54 slot 0x04 (deleting dtor)

    // the call of pContext completes when the service token (256 characters) is in pToken;
    // checksRestriction: fails if the parental controls restrict the internet
    nn::nex::qResult RequestServiceToken(nn::nex::CallContext* pContext, unsigned int gameServerId, const char* pAccessKey, char* pToken,
                                         bool checksRestriction); // 0x00386D70

    u32 m_LastErrorCode; // 0x04 (the network error code of the last request; 0: none)
};
ASSERT_SIZE(IndependentServer, 0x08);
} // namespace nex
} // namespace nn
