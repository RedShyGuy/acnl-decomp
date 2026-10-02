#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace srv {
void StartNotification(); // 0x0011E34C | fefates:bytes [tier B]
void RegisterNotificationHandler(nn::srv::NotificationHandler*, unsigned int); // 0x0011E410 | fefates:bytes [tier B]
nn::Result Initialize(); // 0x0012A800 | nintendogs:callgraph [tier A]
nn::Result GetServiceHandle(nn::Handle* session, const char* name, int nameLength, unsigned flags); // 0x0012A924 | nintendogs:callgraph [tier A]
nn::Result GetServiceHandle(nn::os::ipc::Session* session, const char* name); // 0x004670E0 | nintendogs:callgraph [tier A]
void GetServiceHandle(nn::os::ipc::Session*, const char*, int); // 0x0046712C | fefates:bytes [tier B]
void Finalize(); // 0x00467168 | mk7dlp:callseq-callee [tier A]
} // namespace srv
} // namespace nn
