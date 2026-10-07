#include "nn/nex/nex_NotificationEventHandler.h"
#include "nn/nex/nex_NotificationEventFilterInterface.h"
#include "nn/nex/nex_MyNotificationEventHandler.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::MyNotificationEventHandler::MyNotificationEventHandler()
{
}

// 0x003B9C3C
void nn::nex::MyNotificationEventHandler::ProcessNotificationEvent(const nn::nex::NotificationEvent&)
{
}

// 0x003B9AD4 slot 0x0C | virtual slot, introduced by nn::nex::MyNotificationEventHandler
void nn::nex::MyNotificationEventHandler::vf_0x0C()
{
}

// 0x003B9B84 slot 0x10 | virtual slot, introduced by nn::nex::MyNotificationEventHandler
void nn::nex::MyNotificationEventHandler::vf_0x10()
{
}

} // namespace nex
} // namespace nn
