#pragma once

#include "decomp.h"

namespace nn {
namespace applet {
namespace CTR {
void EnableSleep(bool); // 0x0012010C | nintendogs:bytes [tier A]
void GetAttribute(); // 0x0012012C | nintendogs:callgraph [tier A]
void IsInfoAccess(); // 0x0012013C | nintendogs:bytes [tier A]
bool IsInitialized(); // 0x0012016C | nintendogs:callgraph [tier A]
void ReplySleepQuery(nn::applet::CTR::QueryReply); // 0x0012017C | nintendogs:bytes [tier A]
void InitializeWrapper(); // 0x001201C0 | nintendogs:bytes [tier A]
void GetAppletType(); // 0x00124800 | nintendogs:callgraph [tier A]
void IsApplication(); // 0x00124814 | nintendogs:bytes [tier A]
void ReceiveCallbackForCommands(unsigned int); // 0x001248A8 | fefates:bytes [tier B]
void ClearHomeButtonState(); // 0x001251C0 | nintendogs:bytes [tier A]
void SetTransitionType(nn::applet::CTR::TransitionType); // 0x0012AD00 | nintendogs:callgraph [tier A]
void GetHomeButtonState(); // 0x0012AD10 | nintendogs:callgraph [tier A]
void SetHomeButtonState(nn::applet::CTR::HomeButtonState); // 0x0012AD20 | nintendogs:callgraph [tier A]
// the value of SetSleepNotificationState (one byte)
u8 GetSleepNotificationState(); // 0x0047FBE0 | tier C
void SetSleepNotificationState(nn::applet::CTR::SleepNotificationState); // 0x0012AD68 | nintendogs:callgraph [tier A]
void GetId(); // 0x0012ADB0 | nintendogs:callgraph [tier A]
void CloseAppletHook(bool); // 0x00131644 | fefates:bytes [tier B]
void IsSystemApplet(); // 0x001372F8 | nintendogs:bytes [tier A]
void DisableSleep(bool); // 0x0047F8C4 | nintendogs:bytes [tier A]
void JumpToManual(); // 0x0047F8FC | fefates:bytes [tier B]
void IsEnableSleep(); // 0x0047F9B0 | nintendogs:callgraph [tier A]
void GetAppletVersion(unsigned int, unsigned short*); // 0x0047F9C0 | fefates:bytes [tier B]
void IsHomeMenuResident(); // 0x0047F9F8 | fefates:bytes [tier B]
void IsExpectedToJumpToHomeMenu(); // 0x0047FD04 | nintendogs:callgraph [tier A]
void IsExpectedToProcessHomeButton(); // 0x0047FE7C | nintendogs:bytes [tier A]
} // namespace CTR
} // namespace applet
} // namespace nn
