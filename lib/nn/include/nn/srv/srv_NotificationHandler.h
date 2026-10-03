#pragma once

#include "decomp.h"

namespace nn {
namespace srv {
// RTTI N2nn3srv19NotificationHandlerE @ 0x008D02E8
// A handler for a notification of the srv: service. RTTI: derives from
// IntrusiveLinkedList<NotificationHandler, void>::Item at +4; that part is kept as words until the
// list is worked out. The name of the slot and of the members are ours.
class NotificationHandler
{
public:
    NotificationHandler() : mLinks(), mUnknown0C(0) {}
    ~NotificationHandler() {}

    virtual void HandleNotification() = 0; // slot 0x00

private:
    uptr mLinks[2];     // 0x04, IntrusiveLinkedList<NotificationHandler, void>::Item
    u32 mUnknown0C;     // 0x0C
};
ASSERT_SIZE(NotificationHandler, 0x10);
} // namespace srv
} // namespace nn
