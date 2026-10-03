#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace srv {
namespace detail {
// The commands of the srv: port (3dbrew "Services API"). All static; the member is ours.
class Service
{
public:
    static nn::Result RegisterClient(); // 0x0011E4A0 | nintendogs:bytes [tier A]
    static nn::Result EnableNotification(nn::Handle* semaphore); // 0x001200BC | nintendogs:bytes [tier A]
    static nn::Result GetServiceHandle(nn::Handle* session, const char* name, s32 nameLength, u32 flags); // 0x0012A958 | nintendogs:bytes [tier A]
    static nn::Result ReceiveNotification(u32* notificationId); // 0x00124798 | tier C (confirmed by the code)

    static nn::Handle s_Session; // 0x0097F08C, the srv: port session
};
} // namespace detail
} // namespace srv
} // namespace nn
