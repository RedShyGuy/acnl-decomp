#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The filter of the notification event that a JobWaitForNotification waits for (the second base of
// MyNotificationEventHandler). The slot names are ours.
// RTTI N2nn3nex32NotificationEventFilterInterfaceE @ 0x008CF2A8
class NotificationEventFilterInterface
{
public:
    // (inline)
    NotificationEventFilterInterface() {}
    virtual ~NotificationEventFilterInterface() {} // slot 0x00 (inline)
    // slot 0x04 (deleting dtor)
    // the event with the first parameter
    virtual void FilterByParam1(unsigned int param1) = 0; // slot 0x08
    // the event with the first two parameters
    virtual void FilterByParam64(unsigned long long param) = 0; // slot 0x0C
};
} // namespace nex
} // namespace nn
