#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/nfc/CTR/nfc_Types.h"
#include "nn/nfp/nfp_Types.h"

namespace nn {
namespace nfc {
namespace CTR {
// The commands of the nfc:u service on one session (3dbrew "NFC Services"). The member is ours.
class NfcIpc
{
public:
    explicit NfcIpc(nn::Handle session) : mSession(session) {}

    nn::Result Initialize(nn::nfc::CTR::Mode mode); // 0x003DA4B4 | fefates:bytes [tier B]
    nn::Result Finalize(nn::nfc::CTR::Mode mode); // 0x003DA668 | fefates:bytes [tier B]
    nn::Result StartCommunication(); // 0x003DA5F0 (name after 3dbrew)
    nn::Result StartDetection(u16 protocol); // 0x003DA550 | tier C (confirmed by the code)
    nn::Result StopDetection(); // 0x003DA528 | fefates:bytes [tier B]
    nn::Result ResetTagScanState(); // 0x003DA640 (name after 3dbrew)
    nn::Result Command0A(); // 0x003DA618 (name is ours: command 0x000A, unnamed on 3dbrew)
    nn::Result GetStatus(nn::nfc::CTR::NfcState* state); // 0x003DA6C8 | fefates:bytes [tier B]
    nn::Result GetTargetConnectionStatus(s32* status); // 0x003DA5BC | fefates:bytes [tier B]
    nn::Result GetConnectResult(nn::Result* result); // 0x003DA588 | fefates:bytes [tier B]
    nn::Result Command1A(); // 0x003DA6A0 (name is ours: command 0x001A, unnamed on 3dbrew)
    nn::Result GetNfpRomInfo(nn::nfp::RomInfo* info); // 0x003DA4EC | fefates:bytes [tier B]

private:
    nn::Handle mSession;    // 0x0
};
ASSERT_SIZE(NfcIpc, 0x4);
} // namespace CTR
} // namespace nfc
} // namespace nn
