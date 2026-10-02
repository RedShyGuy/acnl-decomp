#pragma once

#include "decomp.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class JobCreateAccount
{
public:
    void LoginGuest(); // 0x0037FB10 | fefates:bytes [tier B]
    void CompleteJob(nn::nex::qResult); // 0x0037FBEC | fefates:bytes [tier B]
    void LogoutGuest(); // 0x0037FC84 | fefates:bytes [tier B]
    void ProcessLogoutGuestResult(); // 0x00380610 | fefates:bytes [tier B]
    void ProcessCreateAccountResult(); // 0x0038063C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
