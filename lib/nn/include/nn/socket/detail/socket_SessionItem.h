#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_IntrusiveLinkedList.h"
#include "nn/os/os_IpcSession.h"

namespace nn {
namespace socket {
namespace detail {
// One session of a SessionPool, linked into one of its lists. The member name is ours.
class SessionItem : public ::nn::fnd::IntrusiveLinkedList<SessionItem, void>::Item
{
public:
    SessionItem(); // 0x00485874 | mk7dlp:bytes [tier A]

    nn::os::ipc::Session mSession;  // 0x8
};
ASSERT_SIZE(SessionItem, 0xC);
} // namespace detail
} // namespace socket
} // namespace nn
