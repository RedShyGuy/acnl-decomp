#pragma once

#include "decomp.h"
#include "nn/srv/srv_NotificationHandler.h"

namespace nn {
namespace srv {
// Instantiations found in the binary:
//   nn::srv::EventNotificationHandlerBase<nn::os::LightEvent>  typeinfo 0x008D0300  vtable 0x009020C0
//
// Signals an event when the notification comes. Member names are ours; all inline (the
// instantiation's out-of-line copies are at 0x007E53EC and 0x00467164).
template <typename EventT>
class EventNotificationHandlerBase : public NotificationHandler
{
public:
    EventNotificationHandlerBase() : mEvent(0) {}
    ~EventNotificationHandlerBase() {}

    virtual void HandleNotification() { mEvent->Signal(); }

private:
    EventT* mEvent;     // 0x10
};
} // namespace srv
} // namespace nn
