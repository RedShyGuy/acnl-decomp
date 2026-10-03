#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace os {
namespace ipc {
class Session;
}
} // namespace os

namespace srv {
class NotificationHandler;

// connects to the srv: port (counted; waits until the port exists)
nn::Result Initialize(); // 0x0012A800 | nintendogs:callgraph [tier A]
nn::Result Finalize(); // 0x00467168 | mk7dlp:callseq-callee [tier A]
// a session of the service name (at most 8 characters)
nn::Result GetServiceHandle(nn::Handle* session, const char* name, s32 nameLength, u32 flags); // 0x0012A924 | nintendogs:callgraph [tier A]
nn::Result GetServiceHandle(nn::os::ipc::Session* session, const char* name); // 0x004670E0 | nintendogs:callgraph [tier A]
nn::Result GetServiceHandle(nn::os::ipc::Session* session, const char* name, s32 nameLength); // 0x0046712C | fefates:bytes [tier B]
// starts the thread that passes the notifications to the registered handlers
nn::Result StartNotification(); // 0x0011E34C | fefates:bytes [tier B]
nn::Result RegisterNotificationHandler(nn::srv::NotificationHandler* handler, u32 notificationId); // 0x0011E410 | fefates:bytes [tier B]
} // namespace srv
} // namespace nn
