#include "nn/ptm/CTR/detail/ptm_PtmIpc.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/ptm/CTR/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ptm {
namespace CTR {
namespace detail {

namespace {
// command headers (3dbrew "PTM Services")
const bit32 COMMAND_GET_STEP_HISTORY = 0x000B00C2;
const bit32 COMMAND_GET_TOTAL_STEP_COUNT = 0x000C0000;
} // namespace

// 0x0012A768 (name after 3dbrew)
nn::Result nn::ptm::CTR::detail::PtmIpc::GetStepHistory(u16* steps, u32 hours, s64 start)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_STEP_HISTORY;
    command[1] = hours;
    command[2] = static_cast<bit32>(start);
    command[3] = static_cast<bit32>(start >> 32);
    // the output buffer, mapped for writing
    command[4] = ((hours * sizeof(u16)) << 4) | 0xC;
    command[5] = reinterpret_cast<uptr>(steps);
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x0012A7B4 | nintendogs:bytes [tier B]
nn::Result nn::ptm::CTR::detail::PtmIpc::GetTotalStepCount(u32* count)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_TOTAL_STEP_COUNT;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *count = command[2];
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace CTR
} // namespace ptm
} // namespace nn
