// Classes from the anonymous namespace of the original init_Default.cpp

#include "decomp.h"
#include "nn/srv/srv_NotificationHandler.h"

namespace {

// vtable 0x0089E8F4 (vptr 0x0089E8FC), offset_to_top 0, 1 entries
// vtable 0x008B3FB0 (vptr 0x008B3FB8), offset_to_top 0, 1 entries
class ExitHandler : public nn::srv::NotificationHandler
{
public:
    virtual void HandleNotification() {} // 0x004DD2DC slot 0x00 | virtual slot, introduced by (anonymous namespace)::ExitHandler
    // also HandleNotification at 0x0011C7C0 slot 0x00 | virtual slot, introduced by (anonymous namespace)::ExitHandler
};

} // namespace
