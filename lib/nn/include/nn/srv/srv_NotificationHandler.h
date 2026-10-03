#pragma once

#include "decomp.h"
#include "nn/fnd/fnd_IntrusiveLinkedList.h"

namespace nn {
namespace srv {
// RTTI N2nn3srv19NotificationHandlerE @ 0x008D02E8
// A handler for a notification of the srv: service (RegisterNotificationHandler); the dispatcher
// thread calls HandleNotification for the id. The name of the slot and of the member are ours.
class NotificationHandler : public ::nn::fnd::IntrusiveLinkedList<NotificationHandler, void>::Item
{
public:
    NotificationHandler() : mNotificationId(0) {}
    ~NotificationHandler() {}

    virtual void HandleNotification() = 0; // slot 0x00

    u32 GetNotificationId() const { return mNotificationId; }
    void SetNotificationId(u32 id) { mNotificationId = id; }

private:
    u32 mNotificationId;    // 0x0C
};
ASSERT_SIZE(NotificationHandler, 0x10);
} // namespace srv
} // namespace nn
