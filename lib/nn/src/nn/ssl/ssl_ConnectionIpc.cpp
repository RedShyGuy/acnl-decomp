#include "nn/ssl/ssl_ConnectionIpc.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ssl {

namespace {
// command headers (3dbrew "SSL Services": ssl:C 0x0001 Initialize, 0x0011 GenerateRandomData)
const bit32 COMMAND_INITIALIZE = 0x00010002;
const bit32 COMMAND_GENERATE_RANDOM_DATA = 0x00110042;
// translate descriptors (3dbrew "IPC")
const bit32 IPC_PROCESS_ID = 0x20;
const bit32 IPC_BUFFER_WRITE = 0xC;
} // namespace

// 0x004672D4 | fefates:bytes [tier A]
nn::Result nn::ssl::ConnectionIpc::GenerateRandomBytes(u8* buffer, size_t size)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GENERATE_RANDOM_DATA;
    command[1] = size;
    command[2] = (size << 4) | IPC_BUFFER_WRITE;
    command[3] = reinterpret_cast<uptr>(buffer);
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x00467314 | fefates:bytes [tier A]
// the process id is added by the kernel (the word after the descriptor is left out)
nn::Result nn::ssl::ConnectionIpc::InitializeGeneralSession()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_INITIALIZE;
    command[1] = IPC_PROCESS_ID;
    nn::Result result = nn::svc::SendSyncRequest(mSession);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

} // namespace ssl
} // namespace nn
