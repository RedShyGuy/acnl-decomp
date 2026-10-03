#include "nn/srv/detail/srv_Service.h"
#include "nn/os/os_ThreadLocalRegion.h"
#include "nn/svc/svc_Api.h"

#include <string.h>

namespace nn {
namespace srv {
namespace detail {

namespace {
// command headers (3dbrew "Services API")
const bit32 COMMAND_REGISTER_CLIENT = 0x00010002;
const bit32 COMMAND_ENABLE_NOTIFICATION = 0x00020000;
const bit32 COMMAND_GET_SERVICE_HANDLE = 0x00050100;
const bit32 COMMAND_RECEIVE_NOTIFICATION = 0x000B0000;
// translate descriptor (3dbrew "IPC"): the kernel adds the process id
const bit32 IPC_PROCESS_ID = 0x20;
} // namespace

// 0x0097F08C
nn::Handle nn::srv::detail::Service::s_Session;

// 0x0011E4A0 | nintendogs:bytes [tier A]
nn::Result nn::srv::detail::Service::RegisterClient()
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_REGISTER_CLIENT;
    command[1] = IPC_PROCESS_ID;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result(command[1]);
}

// 0x001200BC | nintendogs:bytes [tier A]
nn::Result nn::srv::detail::Service::EnableNotification(nn::Handle* semaphore)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_ENABLE_NOTIFICATION;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *semaphore = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

// 0x00124798 | tier C (confirmed by the code)
nn::Result nn::srv::detail::Service::ReceiveNotification(u32* notificationId)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_RECEIVE_NOTIFICATION;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *notificationId = command[2];
    return nn::Result(command[1]);
}

// 0x0012A958 | nintendogs:bytes [tier A]
nn::Result nn::srv::detail::Service::GetServiceHandle(nn::Handle* session, const char* name, s32 nameLength, u32 flags)
{
    bit32* command = nn::os::detail::GetIpcCommandBuffer();
    command[0] = COMMAND_GET_SERVICE_HANDLE;
    memcpy(&command[1], name, 8);
    command[3] = nameLength;
    command[4] = flags;
    nn::Result result = nn::svc::SendSyncRequest(s_Session);
    if (result.IsFailure()) {
        return result;
    }
    *session = nn::Handle(command[3]);
    return nn::Result(command[1]);
}

} // namespace detail
} // namespace srv
} // namespace nn
