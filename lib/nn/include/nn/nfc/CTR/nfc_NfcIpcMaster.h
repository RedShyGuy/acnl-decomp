#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace nfc {
namespace CTR {
// More commands of the NFC service on a session (nn::nfp keeps a copy of its session in one, next to
// its NfcIpc). The member is ours.
class NfcIpcMaster
{
public:
    explicit NfcIpcMaster(nn::Handle session) : mSession(session) {}

    // NFC:StopCommunication
    nn::Result Disconnect(); // 0x003DA48C | fefates:bytes [tier B]

private:
    nn::Handle mSession;    // 0x0
};
ASSERT_SIZE(NfcIpcMaster, 0x4);
} // namespace CTR
} // namespace nfc
} // namespace nn
