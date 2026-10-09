#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/applet/CTR/applet_Types.h"
#include "nn/fnd/fnd_TimeSpan.h"

namespace nn {
namespace applet {
namespace CTR {
// the callbacks the program sets (called from the client thread through ReceiveCallbackForCommands,
// or from the transitions); every callback gets its argument first (type names are ours)
typedef bool (*HomeButtonCallback)(uptr argument, bool isActive, nn::applet::CTR::HomeButtonState state);
typedef void (*RequestCallback)(uptr argument, u32 request, nn::Handle* pReplyHandle);
typedef void (*MessageCallback)(uptr argument, u32 senderAppId, const u8* pMessage, s32 size, nn::Handle handle);
typedef nn::applet::CTR::QueryReply (*SleepQueryCallback)(uptr argument);
typedef void (*NotificationCallback)(uptr argument);
typedef bool (*ParameterCallback)(uptr argument, u32 senderAppId, u32 command, u8* pBuffer, u32 bufferSize, s32 size, nn::Handle handle);

// the original calls them (noipa: GCC would inline them or merge the calls of the getters)
DECOMP_NOIPA void EnableSleep(bool sleepIfShellClosed); // 0x0012010C | nintendogs:bytes [tier A]
DECOMP_NOIPA void DisableSleep(bool rejectQuery); // 0x0047F8C4 | nintendogs:bytes [tier A]
DECOMP_NOIPA bool IsEnableSleep(); // 0x0047F9B0 | nintendogs:callgraph [tier A]
DECOMP_NOIPA u32 GetAttribute(); // 0x0012012C | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetAttribute(unsigned int attribute); // 0x0012015C | tier C
// the attribute says the program only reads the applet information (no Initialize of APT)
DECOMP_NOIPA bool IsInfoAccess(); // 0x0012013C | nintendogs:bytes [tier A]
bool IsInitialized(); // 0x0012016C | nintendogs:callgraph [tier A]
void ReplySleepQuery(nn::applet::CTR::QueryReply reply); // 0x0012017C | nintendogs:bytes [tier A]
DECOMP_NOIPA void InitializeWrapper(); // 0x001201C0 | nintendogs:bytes [tier A]
DECOMP_NOIPA void SetId(unsigned int id); // 0x00120254 | tier C
DECOMP_NOIPA u32 GetId(); // 0x0012ADB0 | nintendogs:callgraph [tier A]
// the kind of the program (attribute & 7; 0: application, 2: system applet)
DECOMP_NOIPA u32 GetAppletType(); // 0x00124800 | nintendogs:callgraph [tier A]
DECOMP_NOIPA bool IsApplication(); // 0x00124814 | nintendogs:bytes [tier A]
DECOMP_NOIPA bool IsSystemApplet(); // 0x001372F8 | nintendogs:bytes [tier A]
DECOMP_NOIPA bool IsReceivedWakeupByCancel(); // 0x00124858 | tier C
bool ReceiveCallbackForCommands(unsigned int argument); // 0x001248A8 | fefates:bytes [tier B]
DECOMP_NOIPA void SetExpectationToJumpToHome(bool isExpected); // 0x00124D60 | tier C
DECOMP_NOIPA bool IsExpectedToJumpToHomeMenu(); // 0x0047FD04 | nintendogs:callgraph [tier A]
bool IsExpectedToProcessHomeButton(); // 0x0047FE7C | nintendogs:bytes [tier A]
void ClearHomeButtonState(); // 0x001251C0 | nintendogs:bytes [tier A]
DECOMP_NOIPA void SetUserPortName(); // 0x0012ACD0 | tier C
DECOMP_NOIPA nn::applet::CTR::TransitionType GetTransitionType(); // 0x0012ACE0 | tier C
DECOMP_NOIPA void SetTransitionType(nn::applet::CTR::TransitionType type); // 0x0012AD00 | nintendogs:callgraph [tier A]
DECOMP_NOIPA u32 GetMessageCommand(); // 0x00131684 | tier C
DECOMP_NOIPA void SetMessageCommand(unsigned int command); // 0x0012ACF0 | tier C
DECOMP_NOIPA nn::applet::CTR::HomeButtonState GetHomeButtonState(); // 0x0012AD10 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetHomeButtonState(nn::applet::CTR::HomeButtonState state); // 0x0012AD20 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetShutdownCallbackFlag(); // 0x0012AD30 (name is ours)
DECOMP_NOIPA bool IsToCallShutdownCallback(); // 0x0012AD44 | tier C
DECOMP_NOIPA void ClearShutdownCallbackFlag(); // 0x0012AD54 | tier C
DECOMP_NOIPA void SetPowerButtonCallbackFlag(); // 0x0012AD78 (name is ours)
DECOMP_NOIPA bool IsToCallPowerButtonCallback(); // 0x0012AD8C | tier C
DECOMP_NOIPA void ClearPowerButtonCallbackFlag(); // 0x0012AD9C | tier C
DECOMP_NOIPA nn::applet::CTR::SleepNotificationState GetSleepNotificationState(); // 0x0047FBE0 | tier C
DECOMP_NOIPA void SetSleepNotificationState(nn::applet::CTR::SleepNotificationState state); // 0x0012AD68 | nintendogs:callgraph [tier A]
// a sleep query was answered REPLY_LATER and ReplySleepQuery has not been called yet
bool IsSleepReplyPending(); // 0x0047FD14 (name is ours)
// calls the two close hooks (and PrepareToExit first)
DECOMP_NOIPA void CloseAppletHook(bool prepareToExit); // 0x00131644 | fefates:bytes [tier B]
DECOMP_NOIPA void PrepareToExit(); // 0x001372E8 | tier C
void JumpToManual(); // 0x0047F8FC | fefates:bytes [tier B]
nn::Result GetAppletVersion(unsigned int appId, unsigned short* pVersion); // 0x0047F9C0 | fefates:bytes [tier B]
DECOMP_NOIPA bool IsHomeMenuResident(); // 0x0047F9F8 | fefates:bytes [tier B]
nn::Result SetApplicationCpuTimeLimit(u32 percent); // 0x0011E75C (name after the command)
nn::Result Initialize(u32 attribute); // 0x0011CDF4 (name is ours)
// never set in this program (0x00975AF8): Initialize does nothing then
DECOMP_NOIPA bool IsInitializedByLauncher(); // 0x0011E58C (name is ours)
nn::Result CloseApplication(const u8* pParameter, u32 size, nn::Handle handle); // 0x00125238 (name is ours)
// the internet browser applet (0x114) is there and the memory layout lets it start
bool IsInternetBrowserAvailable(); // 0x0047FA28 (name is ours)
// AppletUtility 0x10 (3dbrew StartExitTask)
nn::Result StartExitTask(bool flag); // 0x0047FA98 (name after 3dbrew)
// handles a HOME button press (jump to the HOME menu and wait for the return)
void JumpToHomeMenuIfRequested(); // 0x0047FAC8 (name is ours)
// starts the system applet the manager asks for (or jumps to the HOME menu) and waits
void JumpToRequestedSystemApplet(); // 0x0047FBF0 (name is ours)

// setters of single callbacks (names are ours)
void SetAwakeCallback(nn::applet::CTR::NotificationCallback callback, uptr argument); // 0x00124830 (name is ours)
void SetSleepQueryCallback(nn::applet::CTR::SleepQueryCallback callback, uptr argument); // 0x00124844 (name is ours)
void SetSleepCanceledCallback(nn::applet::CTR::NotificationCallback callback, uptr argument); // 0x00124894 (name is ours)
void ClearSleepCallbacks(); // 0x00124868 (name is ours)

// constants of applet_API.cpp, made by its static initializer (names are ours)
// 0 (no handle)
extern const nn::Handle INVALID_HANDLE;
// -1 ns (no timeout)
extern const nn::fnd::TimeSpan WAIT_INFINITE;
// 0 ns (do not wait)
extern const nn::fnd::TimeSpan WAIT_NONE;
} // namespace CTR
} // namespace applet
} // namespace nn
