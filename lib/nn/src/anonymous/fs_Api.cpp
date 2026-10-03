// Classes and objects from the anonymous namespace of the original fs_Api.cpp. The objects are
// constructed by __sti___10_fs_Api_cpp (0x00784EF8), which also registers their destructors;
// nothing else refers to them by address. Their names are ours.

#include "decomp.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/os/os_LightEvent.h"
#include "nn/srv/srv_EventNotificationHandlerBase.h"
#include "nn/srv/srv_NotificationHandler.h"

namespace nn {
namespace fs {
namespace {

// status, not found, module fs, 170 (name is ours)
const bit32 RESULT_FATAL_NOTIFICATION = 0xC88044AA;

// vtable 0x0089E9D4 (vptr 0x0089E9DC), offset_to_top 0, 1 entries
// a notification that the file system cannot go on: the fatal error screen
class FatalNotificationHandler : public nn::srv::NotificationHandler
{
public:
    virtual void HandleNotification();
};

// 0x00346A54 slot 0x00
void FatalNotificationHandler::HandleNotification()
{
    nn::err::CTR::ThrowFatalErrAll(nn::Result(RESULT_FATAL_NOTIFICATION), nn::err::CTR::GetCurrentAddress());
}

typedef nn::srv::EventNotificationHandlerBase<nn::os::LightEvent> EventHandler;

// 0x00AE1B68
EventHandler s_EventHandler0;
// 0x00AE1B7C
EventHandler s_EventHandler1;
// 0x00AE1B90
EventHandler s_EventHandler2;
// 0x00AE1BA4
EventHandler s_EventHandler3;
// 0x00AE1BB8
FatalNotificationHandler s_FatalNotificationHandler;

} // namespace
} // namespace fs
} // namespace nn
