#pragma once

#include "decomp.h"
#include "nn/nex/nex_NotificationEventFilterInterface.h"
#include "nn/nex/nex_NotificationEventHandler.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex26MyNotificationEventHandlerE @ 0x008CEF68
// vtable 0x008FEDA0 (vptr 0x008FEDA8), offset_to_top 0, 5 entries
// vtable 0x008FEDBC (vptr 0x008FEDC4), offset_to_top -4, 4 entries
class MyNotificationEventHandler : public ::nn::nex::NotificationEventHandler, public ::nn::nex::NotificationEventFilterInterface
{
public:
    MyNotificationEventHandler(); // ctor address unknown
    virtual ~MyNotificationEventHandler(); // 0x003BA150 slot 0x00
    // 0x003BA0FC slot 0x04 (deleting dtor)
    virtual void ProcessNotificationEvent(const nn::nex::NotificationEvent& event); // 0x003B9C3C slot 0x08
    virtual void vf_0x0C(); // 0x003B9AD4 slot 0x0C | virtual slot, introduced by nn::nex::MyNotificationEventHandler
    virtual void vf_0x10(); // 0x003B9B84 slot 0x10 | virtual slot, introduced by nn::nex::MyNotificationEventHandler
};
} // namespace nex
} // namespace nn
