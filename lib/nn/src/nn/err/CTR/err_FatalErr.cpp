#include "nn/err/CTR/err_FatalErr.h"
#include <string.h>
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace err {
namespace CTR {
namespace {
// command header (3dbrew "ERR:Throw": the info as 32 normal words)
const bit32 COMMAND_THROW = 0x00010800;
} // namespace

// 0x00130C84 | nintendogs:bytes [tier A]
nn::Result nn::err::CTR::FatalErr::Throw(const nn::err::CTR::FatalErrInfo& info)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_THROW;
    memcpy(&command[1], &info, sizeof(info));
    nn::Result result = nn::svc::SendSyncRequest(m_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

} // namespace CTR
} // namespace err
} // namespace nn
