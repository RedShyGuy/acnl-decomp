#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace ssl {
// The commands of the ssl:C service (3dbrew "SSL Services") on a session handle. The member and
// the constructor are ours.
class ConnectionIpc
{
public:
    ConnectionIpc() : mSession() {}

    void SetSession(nn::Handle session) { mSession = session; } // inline (name is ours)

    nn::Result GenerateRandomBytes(u8* buffer, size_t size); // 0x004672D4 | fefates:bytes [tier A]
    nn::Result InitializeGeneralSession(); // 0x00467314 | fefates:bytes [tier A]

private:
    nn::Handle mSession;    // 0x0
};
ASSERT_SIZE(ConnectionIpc, 0x4);
} // namespace ssl
} // namespace nn
