#include "nn/ndm/CTR/detail/ndm_Interface.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace ndm {
namespace CTR {
namespace detail {
namespace {
// command headers (3dbrew "NDM Services")
const bit32 COMMAND_ENTER_EXCLUSIVE_STATE = 0x00010042;
const bit32 COMMAND_LEAVE_EXCLUSIVE_STATE = 0x00020002;
const bit32 COMMAND_QUERY_EXCLUSIVE_MODE = 0x00030000;
const bit32 COMMAND_SUSPEND_DAEMONS = 0x00060040;
const bit32 COMMAND_RESUME_DAEMONS = 0x00070040;
const bit32 COMMAND_OVERRIDE_DEFAULT_DAEMONS = 0x00140040;

// the kernel fills in the process id after this descriptor (3dbrew "IPC")
const bit32 DESCRIPTOR_PROCESS_ID = 0x20;

inline nn::Result Send(bit32* command)
{
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}
} // namespace

// 0x0097E8E8
nn::Handle s_Session;

// 0x00124724 | nintendogs:bytes [tier A]
nn::Result nn::ndm::CTR::detail::Interface::SuspendDaemons(bit32 mask)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_SUSPEND_DAEMONS;
    command[1] = mask;
    return Send(command);
}

// 0x0012475C (name is ours)
nn::Result nn::ndm::CTR::detail::Interface::OverrideDefaultDaemonsEntry(bit32 mask)
{
    return OverrideDefaultDaemons(mask);
}

// 0x00124760 | nintendogs:bytes [tier A]
nn::Result nn::ndm::CTR::detail::Interface::OverrideDefaultDaemons(bit32 mask)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_OVERRIDE_DEFAULT_DAEMONS;
    command[1] = mask;
    return Send(command);
}

// 0x00144C20 | nintendogs:bytes [tier A]
nn::Result nn::ndm::CTR::detail::Interface::ResumeDaemons(bit32 mask)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RESUME_DAEMONS;
    command[1] = mask;
    return Send(command);
}

// 0x00354AF4 | nintendogs:bytes [tier B]
nn::Result nn::ndm::CTR::detail::Interface::QueryExclusiveMode(int* pMode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_QUERY_EXCLUSIVE_MODE;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *pMode = command[2];
    return nn::Result(command[1]);
}

// 0x00354B30 (name is ours)
nn::Result nn::ndm::CTR::detail::Interface::EnterExclusiveStateEntry(s32 mode)
{
    return EnterExclusiveState(mode);
}

// 0x00354B34 | nintendogs:bytes [tier A]
nn::Result nn::ndm::CTR::detail::Interface::EnterExclusiveState(int mode)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ENTER_EXCLUSIVE_STATE;
    command[1] = mode;
    command[2] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

// 0x00354B74 (name is ours)
nn::Result nn::ndm::CTR::detail::Interface::LeaveExclusiveStateEntry()
{
    return LeaveExclusiveState();
}

// 0x00354B78 | nintendogs:bytes [tier A]
nn::Result nn::ndm::CTR::detail::Interface::LeaveExclusiveState()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_LEAVE_EXCLUSIVE_STATE;
    command[1] = DESCRIPTOR_PROCESS_ID;
    return Send(command);
}

} // namespace detail
} // namespace CTR
} // namespace ndm
} // namespace nn
