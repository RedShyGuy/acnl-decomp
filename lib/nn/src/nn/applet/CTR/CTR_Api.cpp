#include "nn/applet/CTR/CTR_Api.h"

namespace nn {
namespace applet {
namespace CTR {
// 0x0012010C | nintendogs:bytes [tier A]
void EnableSleep(bool)
{
}

// 0x0012012C | nintendogs:callgraph [tier A]
void GetAttribute()
{
}

// 0x0012013C | nintendogs:bytes [tier A]
void IsInfoAccess()
{
}

// 0x0012016C | nintendogs:callgraph [tier A]
bool IsInitialized()
{
}

// 0x0012017C | nintendogs:bytes [tier A]
void ReplySleepQuery(nn::applet::CTR::QueryReply)
{
}

// 0x001201C0 | nintendogs:bytes [tier A]
void InitializeWrapper()
{
}

// 0x00124800 | nintendogs:callgraph [tier A]
void GetAppletType()
{
}

// 0x00124814 | nintendogs:bytes [tier A]
void IsApplication()
{
}

// 0x001248A8 | fefates:bytes [tier B]
void ReceiveCallbackForCommands(unsigned int)
{
}

// 0x001251C0 | nintendogs:bytes [tier A]
void ClearHomeButtonState()
{
}

// 0x0012AD00 | nintendogs:callgraph [tier A]
void SetTransitionType(nn::applet::CTR::TransitionType)
{
}

// 0x0012AD10 | nintendogs:callgraph [tier A]
void GetHomeButtonState()
{
}

// 0x0012AD20 | nintendogs:callgraph [tier A]
void SetHomeButtonState(nn::applet::CTR::HomeButtonState)
{
}

// 0x0012AD68 | nintendogs:callgraph [tier A]
void SetSleepNotificationState(nn::applet::CTR::SleepNotificationState)
{
}

// 0x0012ADB0 | nintendogs:callgraph [tier A]
void GetId()
{
}

// 0x00131644 | fefates:bytes [tier B]
void CloseAppletHook(bool)
{
}

// 0x001372F8 | nintendogs:bytes [tier A]
void IsSystemApplet()
{
}

// 0x0047F8C4 | nintendogs:bytes [tier A]
void DisableSleep(bool)
{
}

// 0x0047F8FC | fefates:bytes [tier B]
void JumpToManual()
{
}

// 0x0047F9B0 | nintendogs:callgraph [tier A]
void IsEnableSleep()
{
}

// 0x0047F9C0 | fefates:bytes [tier B]
void GetAppletVersion(unsigned int, unsigned short*)
{
}

// 0x0047F9F8 | fefates:bytes [tier B]
void IsHomeMenuResident()
{
}

// 0x0047FD04 | nintendogs:callgraph [tier A]
void IsExpectedToJumpToHomeMenu()
{
}

// 0x0047FE7C | nintendogs:bytes [tier A]
void IsExpectedToProcessHomeButton()
{
}

} // namespace CTR
} // namespace applet
} // namespace nn
