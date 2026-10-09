#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/applet_Types.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/ptm/CTR/ptm_Types.h"

namespace nn {
namespace os {
class ReaderWriterLock;
class TransferMemoryBlock;
class CriticalSection;
} // namespace os
namespace applet {
namespace CTR {
class SysSleepAcceptedCallbackInfo;

namespace detail {
// --- connection (applet_API.cpp) ---
nn::Result GetTargetPlatform(nn::ptm::CTR::TargetPlatform* pPlatform); // 0x0011E59C | tier C
nn::Result InitializeConnect(unsigned int appId, unsigned int attribute, int priority); // 0x0011E5C0 | fefates:bytes [tier B]
nn::Result SetApplicationCpuTimeLimit(u32 fixed, u32 percent); // 0x0011E768 (name after the command)
void Enable(bool waitForStart); // 0x0011E7D0 | nintendogs:bytes [tier A]
DECOMP_NOIPA nn::Result Disconnect(); // 0x00120264 | nintendogs:bytes [tier A]
DECOMP_NOIPA void InitializeMutex(nn::Handle mutex); // 0x001202B0 | tier C
DECOMP_NOIPA void SetInitialParamSize(int size); // 0x001202C0 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetInitialParamValid(); // 0x001202D0 | nintendogs:callgraph [tier A]
DECOMP_NOIPA u8* GetInitialParamBuffer(); // 0x001202E4 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetInitialWakeupState(nn::applet::CTR::WakeupState state); // 0x001202F0 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetInitialParamSenderId(unsigned senderAppId); // 0x001204B8 | nintendogs:callgraph [tier A]
DECOMP_NOIPA nn::Result Connect(); // 0x00120624 | fefates:bytes [tier B]
DECOMP_NOIPA void LockAndConnect(); // 0x0012B09C | nintendogs:callgraph [tier A]
DECOMP_NOIPA void DisconnectAndUnlock(); // 0x0012B6FC | nintendogs:bytes [tier A]
DECOMP_NOIPA void SetPortName(const char* pName); // 0x00131694 | fefates:bytes [tier B]
nn::Result GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode* pMode); // 0x00480DBC (name is ours)

// --- state (applet_API.cpp) ---
// (symbols.json names 0x00124D70 ThreadFunc; it sets the flag, ThreadFunc follows at 0x00124D84)
DECOMP_NOIPA void SetReceivedWakeupByCancel(); // 0x00124D70 (name is ours)
DECOMP_NOIPA void SetPowerButtonState(nn::applet::CTR::PowerButtonState state); // 0x00125208 | tier C
DECOMP_NOIPA nn::applet::CTR::OrderToCloseState GetOrderToCloseState(); // 0x00125218 | tier C
DECOMP_NOIPA void SetOrderToCloseState(nn::applet::CTR::OrderToCloseState state); // 0x00125228 | tier C
DECOMP_NOIPA bool IsActive(); // 0x00125500 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void SetActive(); // 0x00125510 | tier C
DECOMP_NOIPA void SetInactive(); // 0x0012AFC0 | tier C
DECOMP_NOIPA u8 GetSleepSysState(); // 0x0012B55C | tier C
DECOMP_NOIPA void SetShutdownState(u8 state); // 0x0012B56C (name is ours)
DECOMP_NOIPA void SetSleepSysState(u8 state); // 0x0012B57C (name is ours)
DECOMP_NOIPA void ClearSleepSysState(); // 0x0012B6C4 | tier C
DECOMP_NOIPA nn::applet::CTR::HomeButtonState GetAbsoluteHomeButtonState(); // 0x0012B9D0 | tier C
DECOMP_NOIPA void SetAbsoluteHomeButtonState(nn::applet::CTR::HomeButtonState state); // 0x0012B9E0 (name is ours)
DECOMP_NOIPA void ClearAbsoluteHomeButtonState(); // 0x0012B9F0 | tier C
DECOMP_NOIPA nn::applet::CTR::PowerButtonState GetPowerButtonState(); // 0x0048088C (name is ours)

// --- transitions and parameters (applet_API.cpp) ---
DECOMP_NOIPA void LockTransition(unsigned transition, bool flag); // 0x001250DC | nintendogs:bytes [tier A]
DECOMP_NOIPA void UnlockTransition(unsigned transition); // 0x00125110 | nintendogs:bytes [tier A]
DECOMP_NOIPA void SleepIfShellClosed(); // 0x001251DC | nintendogs:bytes [tier A]
nn::Result RestartApplication(const void* pParameter, unsigned int size); // 0x0012513C | fefates:bytes [tier B]
DECOMP_NOIPA nn::Result RestoreVramSysArea(); // 0x00125188 | nintendogs:bytes [tier A]
nn::Result Receive(unsigned int* pSenderAppId, unsigned int* pCommand, unsigned char* pBuffer, unsigned int bufferSize, int* pSize, nn::Handle* pHandle, nn::fnd::TimeSpan timeout); // 0x0012523C | fefates:bytes [tier B]
nn::Result TryReceive(unsigned* pSenderAppId, unsigned* pCommand, unsigned char* pBuffer, unsigned bufferSize, int* pSize, nn::Handle* pHandle, bool noWait); // 0x0012ADC0 | nintendogs:bytes-fuzzy [tier A]
DECOMP_NOIPA nn::Result CallUtility(unsigned utilityId, const unsigned char* pInput, unsigned inputSize, unsigned char* pOutput, unsigned outputSize, int* pResult); // 0x0012AEF4 | nintendogs:bytes [tier A]
DECOMP_NOIPA void AssignDspRight(bool isAssigned); // 0x0012AFD4 | nintendogs:bytes [tier A]
DECOMP_NOIPA void AssignGpuRight(bool isAssigned); // 0x0012B024 | nintendogs:bytes [tier A]
DECOMP_NOIPA void AssignCameraRight(bool isAssigned); // 0x00131D24 | tier C
DECOMP_NOIPA bool CancelParameter(bool checkSender, unsigned senderAppId, bool checkReceiver, unsigned receiverAppId); // 0x0012B0D0 | nintendogs:bytes [tier A]
nn::applet::CTR::WakeupState WaitForStarting(unsigned* pSenderAppId, unsigned char* pBuffer, unsigned bufferSize, int* pSize, nn::Handle* pHandle, nn::fnd::TimeSpan timeout); // 0x0012B124 | nintendogs:bytes-fuzzy [tier A]
nn::Result DoApplicationJump(const unsigned char* pParameter, unsigned int size, const unsigned char* pHmac, unsigned int hmacSize); // 0x0012B58C | fefates:bytes [tier B]
DECOMP_NOIPA void NotifyToWait(); // 0x0012B6D8 | nintendogs:bytes [tier A]
nn::Result CloseApplication(const unsigned char* pParameter, unsigned int size, nn::Handle handle); // 0x0012B76C | fefates:bytes [tier B]
nn::Result CloseApplicationCore(const unsigned char* pParameter, unsigned int size, nn::Handle handle); // 0x0012B7A8 | fefates:bytes [tier B]
nn::Result PrepareToJumpApplication(nn::applet::CTR::AppJumpType type, unsigned long long titleId, nn::fs::MediaType mediaType); // 0x0012B908 | fefates:bytes [tier B]
DECOMP_NOIPA void ReplySleepQueryToManager(nn::applet::CTR::QueryReply reply); // 0x0012B9A4 | nintendogs:bytes [tier A]
DECOMP_NOIPA void ReplySleepNotificationCompleteToManager(); // 0x0012BA04 | nintendogs:bytes [tier A]
nn::Result Send(unsigned destinationAppId, unsigned command, const unsigned char* pParameter, unsigned size, nn::Handle handle, nn::fnd::TimeSpan timeout); // 0x0012BA28 | nintendogs:callseq-callee [tier A]
nn::Result Glance(unsigned* pSenderAppId, unsigned* pCommand, unsigned char* pBuffer, unsigned bufferSize, int* pSize, nn::Handle* pHandle); // 0x0012BCD0 | nintendogs:bytes [tier A]
nn::Result CancelLibraryAppletIfRegistered(bool isExiting, nn::applet::CTR::WakeupState* pWakeupState); // 0x00131770 | nintendogs:bytes [tier A]

// --- client thread (applet_ClientThread.cpp) ---
void InitializeClientThread(int priority, nn::Handle parameterEvent, nn::Handle notificationEvent); // 0x00120300 | nintendogs:callseq [tier A]
void ThreadFunc(int); // 0x00124D84 (name from symbols.json, which puts it at 0x00124D70)
DECOMP_NOIPA void SetReceiveCallback(bool (*callback)(unsigned), unsigned argument); // 0x001251B0 | nintendogs:callgraph [tier A]
DECOMP_NOIPA void ClearControlEvent(); // 0x001316AC | tier C
DECOMP_NOIPA void WaitForControlEvent(); // 0x001316B8 | tier C
DECOMP_NOIPA bool TryWaitForControlEvent(); // 0x00131764 | tier C
void FinalizeClientThread(); // 0x001316C4 | fefates:bytes [tier B]

// --- screen capture and applets (applet_Wrapper.cpp) ---
nn::Result CaptureScreen(unsigned appId); // 0x0047FEA0 | nintendogs:callseq [tier A]
DECOMP_NOIPA void GetDisplayInfo(nn::applet::CTR::AppletDisplayInfo* pInfo); // 0x0047FFD4 | nintendogs:bytes [tier A]
DECOMP_NOIPA nn::Result JumpToHomeMenu(const unsigned char* pParameter, unsigned int size, nn::Handle handle); // 0x00480038 | tier C
void ConvertL16ToB16(unsigned* pOutput, const unsigned* pInput, unsigned stride, const nn::applet::CTR::detail::OffsetTable& table); // 0x00480198 | nintendogs:bytes [tier A]
void ConvertL24ToB24(unsigned* pOutput, const unsigned* pInput, unsigned stride, const nn::applet::CTR::detail::OffsetTable& table); // 0x00480208 | nintendogs:bytes [tier A]
DECOMP_NOIPA bool WaitForRegister(unsigned appId, nn::fnd::TimeSpan timeout); // 0x004802E8 | nintendogs:callseq-callee [tier A]
DECOMP_NOIPA void GetAppletManInfo(nn::applet::CTR::AppletPos pos, nn::applet::CTR::AppletPos* pPos, unsigned* pRequestedAppId, unsigned* pHomeMenuAppId, unsigned* pActiveAppId); // 0x0048046C | nintendogs:bytes [tier A]
nn::Result StartSystemApplet(unsigned int appId, const unsigned char* pParameter, unsigned int size, nn::Handle handle); // 0x004804F0 | tier C (confirmed by the code)
nn::Result StartLibraryApplet(unsigned appId, const unsigned char* pParameter, unsigned size, nn::Handle handle); // 0x00480710 | nintendogs:callgraph [tier A]
DECOMP_NOIPA nn::Result WaitToCaptureScreen(unsigned appId, nn::Handle* pHandle); // 0x0048089C | nintendogs:callseq-callee [tier A]
DECOMP_NOIPA void CalcCaptureBufferInfo(nn::applet::CTR::CaptureBufferInfo* pInfo); // 0x00480998 | nintendogs:callseq-callee [tier A]
DECOMP_NOIPA nn::Result SendCaptureBufferInfo(const unsigned char* pInfo, unsigned size); // 0x00480ACC | nintendogs:callseq [tier A]
DECOMP_NOIPA nn::Result PrepareToJumpToHomeMenu(); // 0x00480AF8 | nintendogs:bytes [tier A]
DECOMP_NOIPA void CaptureDisplayBuffer(unsigned address, const nn::applet::CTR::AppletDisplayInfo* pDisplayInfo, const nn::applet::CTR::CaptureBufferInfo* pBufferInfo); // 0x00480B84 | nintendogs:bytes [tier A]
DECOMP_NOIPA void CaptureDisplayBufferCore(unsigned address, const nn::applet::CTR::AppletDisplayInfo* pDisplayInfo, bool isBottom, bool isRight); // 0x00480C18 | nintendogs:callgraph [tier A]
DECOMP_NOIPA nn::Result AttachTransferMemoryHandle(nn::os::TransferMemoryBlock* pBlock, nn::Handle handle, unsigned size, unsigned permission); // 0x00480DE0 | nintendogs:bytes [tier A]
nn::Result PrepareToStartSystemApplet(unsigned appId); // 0x00480DF4 | nintendogs:bytes [tier A]
nn::Result PrepareToStartLibraryApplet(unsigned appId); // 0x00480F00 | nintendogs:bytes [tier A]
DECOMP_NOIPA nn::Result CaptureScreenForSystemApplet(unsigned appId); // 0x00480FEC | nintendogs:callseq [tier A]
nn::Result SetScreenCapturePostPermission(u8 permission); // 0x004810B8 (name after the command)
nn::Result Wrap(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize); // 0x004810DC | tier C
nn::Result Wrap1(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize); // 0x00481128 (name after the command)
nn::Result Unwrap(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize); // 0x00481614 | tier C
nn::Result Unwrap1(void* pOutput, const void* pInput, unsigned size, int nonceOffset, unsigned nonceSize); // 0x00481660 (name after the command)
DECOMP_NOIPA bool IsInternetBrowserAvailable(); // 0x0047FA2C (name is ours)

// a Finalize of a library of result module 98 that the applet library calls before it leaves the
// application; this program has only the stub that returns "not supported" (name is ours)
DECOMP_NOIPA nn::Result FinalizeModule98(); // 0x003E2270 (name is ours)

// --- globals of applet_API.cpp (names are ours) ---
extern bool s_IsVramSaved;
extern bool s_IsInitialized;
extern bool s_HasGpuRight;
extern bool s_IsDspSleptByApplet;
extern nn::os::ReaderWriterLock* s_pExitLock;
extern bool s_IsInitializedByLauncher;
extern bool s_IsActive;
extern nn::applet::CTR::HomeButtonState s_AbsoluteHomeButtonState;
extern u8 s_SleepSysState;
extern u8 s_ShutdownState;
extern nn::applet::CTR::PowerButtonState s_PowerButtonState;
extern nn::applet::CTR::OrderToCloseState s_OrderToCloseState;
extern bool s_IsToCallPowerButtonCallback;
extern bool s_IsToCallShutdownCallback;
extern bool s_IsReceivedWakeupByCancel;
extern nn::applet::CTR::TransitionType s_TransitionType;
extern nn::applet::CTR::SleepNotificationState s_SleepNotificationState;
extern nn::applet::CTR::HomeButtonState s_HomeButtonState;
extern bool s_IsExpectedToJumpToHome;
extern u32 s_MessageCommand;
extern u32 s_Id;
extern u32 s_Attribute;
// the service names: APT:S, APT:U
extern const char* s_PortNames[2];

// the callbacks of the program (one argument per callback in s_CallbackArguments)
struct Callbacks
{
    nn::applet::CTR::HomeButtonCallback homeButton;      // 0x00
    nn::applet::CTR::RequestCallback request;            // 0x04
    nn::applet::CTR::MessageCallback message;            // 0x08
    nn::applet::CTR::NotificationCallback dspSleep;      // 0x0C
    nn::applet::CTR::NotificationCallback dspWakeup;     // 0x10
    nn::applet::CTR::SleepQueryCallback sleepQuery;      // 0x14
    nn::applet::CTR::NotificationCallback sleepCanceled; // 0x18
    nn::applet::CTR::NotificationCallback sleepAccepted; // 0x1C
    nn::applet::CTR::NotificationCallback awake;         // 0x20
    nn::applet::CTR::NotificationCallback shutdown;      // 0x24
    nn::applet::CTR::NotificationCallback powerButton;   // 0x28
    nn::applet::CTR::NotificationCallback leaveApplet;   // 0x2C, before jumping to the HOME menu
    nn::applet::CTR::NotificationCallback orderToClose;  // 0x30
    nn::applet::CTR::NotificationCallback closeHook1;    // 0x34, CloseAppletHook
    nn::applet::CTR::NotificationCallback closeHook2;    // 0x38, CloseAppletHook
    nn::applet::CTR::ParameterCallback parameter;        // 0x3C, WaitForStarting / WaitToCaptureScreen
};
ASSERT_SIZE(Callbacks, 0x40);

enum CallbackIndex {
    CALLBACK_HOME_BUTTON,
    CALLBACK_REQUEST,
    CALLBACK_MESSAGE,
    CALLBACK_DSP_SLEEP,
    CALLBACK_DSP_WAKEUP,
    CALLBACK_SLEEP_QUERY,
    CALLBACK_SLEEP_CANCELED,
    CALLBACK_SLEEP_ACCEPTED,
    CALLBACK_AWAKE,
    CALLBACK_SHUTDOWN,
    CALLBACK_POWER_BUTTON,
    CALLBACK_LEAVE_APPLET,
    CALLBACK_ORDER_TO_CLOSE,
    CALLBACK_CLOSE_HOOK_1,
    CALLBACK_CLOSE_HOOK_2,
    CALLBACK_PARAMETER,
    CALLBACK_COUNT,
};

extern bool s_IsSleepEnabled;
extern nn::applet::CTR::SysSleepAcceptedCallbackInfo* s_pSleepAcceptedCallbackHead;
extern nn::applet::CTR::SysSleepAcceptedCallbackInfo* s_pSleepAcceptedCallbackTail;
extern Callbacks s_Callbacks;
extern uptr s_CallbackArguments[CALLBACK_COUNT];

// --- globals of applet_Wrapper.cpp (names are ours) ---
extern nn::os::CriticalSection s_SleepAcceptedCallbackLock;
extern u8 s_ParameterBuffer[0x1000];
} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
