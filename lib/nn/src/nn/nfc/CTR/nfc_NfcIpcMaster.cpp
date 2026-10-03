#include "nn/nfc/CTR/nfc_NfcIpcMaster.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace nfc {
namespace CTR {

namespace {
const bit32 COMMAND_STOP_COMMUNICATION = 0x00040000;
} // namespace

// 0x003DA48C | fefates:bytes [tier B]
nn::Result nn::nfc::CTR::NfcIpcMaster::Disconnect()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_STOP_COMMUNICATION;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

} // namespace CTR
} // namespace nfc
} // namespace nn
