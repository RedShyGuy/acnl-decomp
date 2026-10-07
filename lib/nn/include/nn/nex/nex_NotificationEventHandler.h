#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class NotificationEvent;
// RTTI N2nn3nex24NotificationEventHandlerE @ 0x008CEDFC
//
// Gets the notifications of the server (MyNotificationEventHandler, the handler of pia). The slot
// name is ours.
class NotificationEventHandler : public ::nn::nex::RootObject
{
public:
    NotificationEventHandler() {} // (inline)
    virtual ~NotificationEventHandler() {} // slot 0x00 (inline)
    // slot 0x04 (deleting dtor)
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event) = 0; // slot 0x08
};
} // namespace nex
} // namespace nn
