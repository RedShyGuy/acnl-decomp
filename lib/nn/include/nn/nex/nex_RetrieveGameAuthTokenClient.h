#pragma once

#include "decomp.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// Starts a JobRetrieveGameAuthToken (no members).
class RetrieveGameAuthTokenClient
{
public:
    // the token and the key of the HPP server; the call completes with the job
    nn::nex::qResult RetrieveGameAuthToken(nn::nex::CallContext* pContext, unsigned int principalId, const nn::nex::String& password, nn::nex::String* pToken,
                                           nn::nex::String* pKey); // 0x003BD2EC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
