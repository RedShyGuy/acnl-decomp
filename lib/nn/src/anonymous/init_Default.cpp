// Classes from the anonymous namespace of the original init_Default.cpp (the file name is from the
// static initializer __sti___16_init_Default_cpp)

#include "decomp.h"
#include "nn/srv/srv_NotificationHandler.h"
#include "nn/svc/svc_Api.h"

namespace {

// a handler for the exit notification of srv: it ends the process (the class name is from the RTTI;
// nn::applet has its own class of the same name, vtable 0x0089E8F4)
// vtable 0x008B3FB0 (vptr 0x008B3FB8), offset_to_top 0, 1 entries
class ExitHandler : public nn::srv::NotificationHandler
{
public:
    virtual void HandleNotification(); // 0x0011C7C0 slot 0x00
};

// (only the static initializer refers to it)
// 0x00AE95EC (name is ours)
ExitHandler s_ExitHandler;

// 0x0011C7C0
void ExitHandler::HandleNotification()
{
    // (armlink removed a call in front of it)
    nn::svc::ExitProcess();
}

} // namespace

// ARMCC's static initializer of the file: the constructor of the global above
// 0x00796780 (static initializer)
