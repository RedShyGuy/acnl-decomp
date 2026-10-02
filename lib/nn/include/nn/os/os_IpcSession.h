#pragma once

// nn::os::ipc::Session - a client session to a service (the class name is from the binary:
// nn::srv::GetServiceHandle(nn::os::ipc::Session*, const char*)). It is the session handle only.

#include "decomp.h"
#include "nn/os/os_HandleObject.h"

namespace nn {
namespace os {
namespace ipc {

class Session : public nn::os::HandleObject
{
};
ASSERT_SIZE(Session, 0x4);

} // namespace ipc
} // namespace os
} // namespace nn
