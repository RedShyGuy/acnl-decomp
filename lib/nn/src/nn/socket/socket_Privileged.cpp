#include "nn/socket/socket_Privileged.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace socket {

namespace {
const bit32 COMMAND_GET_HOST_ID = 0x00060000;
} // namespace

// 0x00484B74 | fefates:bytes [tier B]
nn::Result nn::socket::Privileged::GetHostId(u32* hostId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_HOST_ID;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    *hostId = command[2];
    return nn::Result(command[1]);
}

} // namespace socket
} // namespace nn
