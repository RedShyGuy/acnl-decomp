#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace socket {
// The commands of the privileged socket service on one session. The member is ours.
class Privileged
{
public:
    explicit Privileged(nn::Handle session) : mSession(session) {}

    nn::Result GetHostId(u32* hostId); // 0x00484B74 | fefates:bytes [tier B]

private:
    nn::Handle mSession;    // 0x0
};
ASSERT_SIZE(Privileged, 0x4);
} // namespace socket
} // namespace nn
