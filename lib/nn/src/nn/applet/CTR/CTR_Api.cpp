// The public functions of applet_API.cpp and applet_Wrapper.cpp (file names from the static
// initializers 0x00788744 and 0x00797D1C)
#include "nn/applet/CTR/CTR_Api.h"
#include "nn/applet/CTR/applet_SysSleepAcceptedCallbackInfo.h"
#include "nn/applet/CTR/detail/applet_APPLET.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_ReaderWriterLock.h"
#include "nn/srv/srv_Api.h"
#include "nn/srv/srv_NotificationHandler.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace applet {
namespace CTR {
namespace {
// the program id of an application (3dbrew "NS and APT Services", AppID)
const u32 APP_ID_APPLICATION = 0x300;
// the priority of the client thread of an application
const s32 CLIENT_THREAD_PRIORITY = 15;
// the srv: notification "terminate" (3dbrew "SRV:GetNotificationType")
const u32 NOTIFICATION_EXIT = 0x100;
// GetAppletProgramInfo flags (3dbrew "APT:GetAppletProgramInfo")
const u32 PROGRAM_INFO_FLAGS = 17;
// AppletUtility ids (3dbrew "APT:AppletUtility")
const u32 UTILITY_START_EXIT_TASK = 0x10;
// LockTransition / UnlockTransition: the HOME menu transition
const u32 TRANSITION_HOME_MENU = 1;
// the attribute bits of IsHomeMenuResident (names are ours)
const u32 ATTRIBUTE_RESIDENT = 0x20000000;
const u32 ATTRIBUTE_HOME_MENU = 0x10000000;
const u32 ATTRIBUTE_TYPE_MASK = 7;
const u32 APPLET_TYPE_SYSTEM = 2;
const u32 APPLET_TYPE_INFO_ACCESS = 6;

// the parameter of the jump to the manual: "ASHP" and a flag (meaning unknown)
const size_t MANUAL_PARAMETER_SIZE = 5;

// a handler for the exit notification of srv: (it does nothing; the class name is from the RTTI)
class ExitHandler : public nn::srv::NotificationHandler
{
public:
    virtual void HandleNotification(); // 0x004DD2DC
};

// 0x00AE0A4C
ExitHandler s_ExitHandler;

// 0x004DD2DC
void ExitHandler::HandleNotification()
{
}
} // namespace

// 0x00975ADC (name is ours)
const nn::Handle INVALID_HANDLE;
// 0x00975AE8 (name is ours)
const nn::fnd::TimeSpan WAIT_INFINITE = nn::fnd::TimeSpan::FromNanoSeconds(-1);
// 0x00975AF0 (name is ours)
const nn::fnd::TimeSpan WAIT_NONE = nn::fnd::TimeSpan::FromNanoSeconds(0);

// 0x0011CDF4 (name is ours)
nn::Result Initialize(u32 attribute)
{
    if (IsInitializedByLauncher()) {
        return nn::Result();
    }
    detail::s_HasGpuRight = true;
    attribute &= ~ATTRIBUTE_TYPE_MASK;
    nn::srv::RegisterNotificationHandler(&s_ExitHandler, NOTIFICATION_EXIT);
    nn::Result result = detail::InitializeConnect(APP_ID_APPLICATION, attribute, CLIENT_THREAD_PRIORITY);
    if (result.IsFailure()) {
        return result;
    }
    detail::LockAndConnect();
    detail::APPLET::SetScreenCapturePostPermission(0);
    detail::DisconnectAndUnlock();
    return nn::Result();
}

// 0x0011E58C (name is ours)
bool IsInitializedByLauncher()
{
    return detail::s_IsInitializedByLauncher;
}

// 0x0011E75C (name after the command)
nn::Result SetApplicationCpuTimeLimit(u32 percent)
{
    return detail::SetApplicationCpuTimeLimit(1, percent);
}

// 0x0012010C | nintendogs:bytes [tier A]
void EnableSleep(bool sleepIfShellClosed)
{
    detail::s_IsSleepEnabled = true;
    if (sleepIfShellClosed) {
        detail::SleepIfShellClosed();
    }
}

// 0x0012012C | nintendogs:callgraph [tier A]
u32 GetAttribute()
{
    return detail::s_Attribute;
}

// 0x0012013C | nintendogs:bytes [tier A]
bool IsInfoAccess()
{
    return (detail::s_Attribute & ATTRIBUTE_TYPE_MASK) == APPLET_TYPE_INFO_ACCESS;
}

// 0x0012015C | tier C
void SetAttribute(unsigned int attribute)
{
    detail::s_Attribute = attribute;
}

// 0x0012016C | nintendogs:callgraph [tier A]
bool IsInitialized()
{
    return detail::s_IsInitialized;
}

// 0x0012017C | nintendogs:bytes [tier A]
void ReplySleepQuery(nn::applet::CTR::QueryReply reply)
{
    switch (reply) {
    case REPLY_REJECT:
        SetSleepNotificationState(SLEEP_NOTIFICATION_REJECTED);
        detail::ReplySleepQueryToManager(reply);
        break;
    case REPLY_ACCEPT:
        SetSleepNotificationState(SLEEP_NOTIFICATION_ACCEPTED);
        detail::ReplySleepQueryToManager(reply);
        break;
    case REPLY_LATER:
        SetSleepNotificationState(SLEEP_NOTIFICATION_LATER);
        break;
    }
}

// 0x001201C0 | nintendogs:bytes [tier A]
void InitializeWrapper()
{
    detail::SetReceiveCallback(ReceiveCallbackForCommands, 0);
    detail::s_Callbacks.homeButton = 0;
    detail::s_CallbackArguments[detail::CALLBACK_HOME_BUTTON] = 0;
    detail::s_Callbacks.request = 0;
    detail::s_CallbackArguments[detail::CALLBACK_REQUEST] = 0;
    detail::s_Callbacks.message = 0;
    detail::s_CallbackArguments[detail::CALLBACK_MESSAGE] = 0;
    detail::s_Callbacks.dspSleep = 0;
    detail::s_CallbackArguments[detail::CALLBACK_DSP_SLEEP] = 0;
    detail::s_Callbacks.dspWakeup = 0;
    detail::s_CallbackArguments[detail::CALLBACK_DSP_WAKEUP] = 0;
    detail::s_Callbacks.sleepQuery = 0;
    detail::s_CallbackArguments[detail::CALLBACK_SLEEP_QUERY] = 0;
    detail::s_Callbacks.sleepCanceled = 0;
    detail::s_CallbackArguments[detail::CALLBACK_SLEEP_CANCELED] = 0;
    detail::s_Callbacks.sleepAccepted = 0;
    detail::s_CallbackArguments[detail::CALLBACK_SLEEP_ACCEPTED] = 0;
    detail::s_Callbacks.awake = 0;
    detail::s_CallbackArguments[detail::CALLBACK_AWAKE] = 0;
    detail::s_Callbacks.shutdown = 0;
    detail::s_CallbackArguments[detail::CALLBACK_SHUTDOWN] = 0;
    detail::s_Callbacks.powerButton = 0;
    detail::s_CallbackArguments[detail::CALLBACK_POWER_BUTTON] = 0;
    detail::s_Callbacks.leaveApplet = 0;
    detail::s_CallbackArguments[detail::CALLBACK_LEAVE_APPLET] = 0;
    detail::s_Callbacks.orderToClose = 0;
    detail::s_CallbackArguments[detail::CALLBACK_ORDER_TO_CLOSE] = 0;
    detail::s_Callbacks.parameter = 0;
    detail::s_CallbackArguments[detail::CALLBACK_PARAMETER] = 0;
}

// 0x00120254 | tier C
void SetId(unsigned int id)
{
    detail::s_Id = id;
}

// 0x00124800 | nintendogs:callgraph [tier A]
u32 GetAppletType()
{
    return detail::s_Attribute & ATTRIBUTE_TYPE_MASK;
}

// 0x00124814 | nintendogs:bytes [tier A]
bool IsApplication()
{
    return (detail::s_Attribute & ATTRIBUTE_TYPE_MASK) == 0;
}

// 0x00124830 (name is ours)
void SetAwakeCallback(nn::applet::CTR::NotificationCallback callback, uptr argument)
{
    detail::s_Callbacks.awake = callback;
    detail::s_CallbackArguments[detail::CALLBACK_AWAKE] = argument;
}

// 0x00124844 (name is ours)
void SetSleepQueryCallback(nn::applet::CTR::SleepQueryCallback callback, uptr argument)
{
    detail::s_Callbacks.sleepQuery = callback;
    detail::s_CallbackArguments[detail::CALLBACK_SLEEP_QUERY] = argument;
}

// 0x00124858 | tier C
bool IsReceivedWakeupByCancel()
{
    return detail::s_IsReceivedWakeupByCancel;
}

// 0x00124868 (name is ours)
void ClearSleepCallbacks()
{
    SetSleepQueryCallback(0, 0);
    SetAwakeCallback(0, 0);
    SetSleepCanceledCallback(0, 0);
}

// 0x00124894 (name is ours)
void SetSleepCanceledCallback(nn::applet::CTR::NotificationCallback callback, uptr argument)
{
    detail::s_Callbacks.sleepCanceled = callback;
    detail::s_CallbackArguments[detail::CALLBACK_SLEEP_CANCELED] = argument;
}

// the callback of the client thread: handles the notifications and parameters the program has not
// taken itself; true when the program should see the event too
// 0x001248A8 | fefates:bytes [tier B]
bool ReceiveCallbackForCommands(unsigned int)
{
    bool result = true;
    HomeButtonState absoluteState = detail::GetAbsoluteHomeButtonState();

    if (IsToCallShutdownCallback()) {
        if (detail::s_Callbacks.shutdown) {
            detail::s_Callbacks.shutdown(detail::s_CallbackArguments[detail::CALLBACK_SHUTDOWN]);
        }
        ClearShutdownCallbackFlag();
    }

    if (detail::GetOrderToCloseState() != ORDER_TO_CLOSE_NONE) {
        if (detail::s_Callbacks.orderToClose) {
            detail::s_Callbacks.orderToClose(detail::s_CallbackArguments[detail::CALLBACK_ORDER_TO_CLOSE]);
        }
        return result;
    }

    if (absoluteState == HOME_BUTTON_SINGLE || absoluteState == HOME_BUTTON_DOUBLE) {
        bool isActive = detail::IsActive();
        HomeButtonState state = detail::GetAbsoluteHomeButtonState();
        detail::ClearAbsoluteHomeButtonState();
        if (detail::s_Callbacks.homeButton == 0 ||
            detail::s_Callbacks.homeButton(detail::s_CallbackArguments[detail::CALLBACK_HOME_BUTTON], isActive, state)) {
            if (isActive && GetHomeButtonState() == HOME_BUTTON_NONE) {
                SetHomeButtonState(state);
            }
        }
        return false;
    }

    if (detail::GetSleepSysState() != 0) {
        switch (detail::GetSleepSysState()) {
        case 1: {
            // NOTIFICATION_SLEEP_QUERY
            QueryReply reply = REPLY_REJECT;
            if (detail::GetOrderToCloseState() == ORDER_TO_CLOSE_NONE && !IsReceivedWakeupByCancel()) {
                if (detail::s_Callbacks.sleepQuery) {
                    reply = detail::s_Callbacks.sleepQuery(detail::s_CallbackArguments[detail::CALLBACK_SLEEP_QUERY]);
                }
                if (!detail::s_IsSleepEnabled && detail::IsActive()) {
                    reply = REPLY_REJECT;
                }
            }
            ReplySleepQuery(reply);
            break;
        }
        case 2: {
            // NOTIFICATION_SLEEP_ACCEPTED
            SetSleepNotificationState(SLEEP_NOTIFICATION_SLEEPING);
            if (detail::s_Callbacks.sleepAccepted && detail::GetOrderToCloseState() == ORDER_TO_CLOSE_NONE &&
                !IsReceivedWakeupByCancel()) {
                detail::s_Callbacks.sleepAccepted(detail::s_CallbackArguments[detail::CALLBACK_SLEEP_ACCEPTED]);
            }
            {
                nn::os::CriticalSection::ScopedLock lock(detail::s_SleepAcceptedCallbackLock);
                for (SysSleepAcceptedCallbackInfo* pInfo = detail::s_pSleepAcceptedCallbackHead; pInfo != 0;
                     pInfo = pInfo->m_pNext) {
                    if (pInfo->m_Callback) {
                        pInfo->m_Callback(pInfo->m_Argument);
                    }
                }
            }
            detail::ReplySleepNotificationCompleteToManager();
            break;
        }
        case 3:
            // NOTIFICATION_SLEEP_AWAKE
            nn::dsp::CTR::Awake();
            if (detail::s_Callbacks.awake && detail::GetOrderToCloseState() == ORDER_TO_CLOSE_NONE &&
                !IsReceivedWakeupByCancel()) {
                detail::s_Callbacks.awake(detail::s_CallbackArguments[detail::CALLBACK_AWAKE]);
            }
            SetSleepNotificationState(SLEEP_NOTIFICATION_AWAKE);
            break;
        case 4:
            // NOTIFICATION_SLEEP_CANCELED_BY_OPEN
            if (detail::s_Callbacks.sleepCanceled && detail::GetOrderToCloseState() == ORDER_TO_CLOSE_NONE &&
                !IsReceivedWakeupByCancel()) {
                detail::s_Callbacks.sleepCanceled(detail::s_CallbackArguments[detail::CALLBACK_SLEEP_CANCELED]);
            }
            break;
        }
        detail::ClearSleepSysState();
        return false;
    }

    if (IsToCallPowerButtonCallback()) {
        if (detail::s_Callbacks.powerButton) {
            detail::s_Callbacks.powerButton(detail::s_CallbackArguments[detail::CALLBACK_POWER_BUTTON]);
        }
        ClearPowerButtonCallbackFlag();
        return result;
    }

    // a parameter
    nn::Handle handle;
    u32 senderAppId;
    u32 command;
    s32 size;
    nn::err::CTR::ThrowFatalErrIfFailure(
        detail::Glance(&senderAppId, &command, detail::s_ParameterBuffer, sizeof(detail::s_ParameterBuffer), &size, &handle));
    switch (command) {
    case 2: {
        // COMMAND_REQUEST: answered with COMMAND_RESPONSE
        nn::Handle replyHandle;
        u32 request = size >= 32 ? *reinterpret_cast<u32*>(detail::s_ParameterBuffer) : 0;
        if (detail::s_Callbacks.request) {
            detail::s_Callbacks.request(detail::s_CallbackArguments[detail::CALLBACK_REQUEST], request, &replyHandle);
        }
        detail::CancelParameter(true, senderAppId, false, 0);
        detail::Send(senderAppId, 3, 0, 0, replyHandle, WAIT_INFINITE);
        result = false;
        break;
    }
    case 5:
        // COMMAND_MESSAGE
        detail::CancelParameter(true, senderAppId, false, 0);
        if (detail::s_Callbacks.message) {
            detail::s_Callbacks.message(detail::s_CallbackArguments[detail::CALLBACK_MESSAGE], senderAppId,
                                        detail::s_ParameterBuffer, size, handle);
        }
        result = false;
        break;
    case 8:
        // COMMAND_DSP_SLEEP
        if (detail::s_Callbacks.dspSleep) {
            detail::s_Callbacks.dspSleep(detail::s_CallbackArguments[detail::CALLBACK_DSP_SLEEP]);
        }
        detail::AssignDspRight(false);
        result = false;
        break;
    case 9:
        // COMMAND_DSP_WAKEUP
        detail::AssignDspRight(true);
        if (detail::s_Callbacks.dspWakeup) {
            detail::s_Callbacks.dspWakeup(detail::s_CallbackArguments[detail::CALLBACK_DSP_WAKEUP]);
        }
        result = false;
        break;
    }
    if (handle.IsValid()) {
        nn::svc::CloseHandle(handle);
    }
    return result;
}

// 0x00124D60 | tier C
void SetExpectationToJumpToHome(bool isExpected)
{
    detail::s_IsExpectedToJumpToHome = isExpected;
}

// 0x001251C0 | nintendogs:bytes [tier A]
void ClearHomeButtonState()
{
    SetHomeButtonState(HOME_BUTTON_NONE);
    detail::UnlockTransition(TRANSITION_HOME_MENU);
    detail::SleepIfShellClosed();
}

// 0x00125238 (name is ours)
nn::Result CloseApplication(const u8* pParameter, u32 size, nn::Handle handle)
{
    return detail::CloseApplicationCore(pParameter, size, handle);
}

// 0x0012ACD0 | tier C
void SetUserPortName()
{
    detail::SetPortName(detail::s_PortNames[1]);
}

// 0x0012ACE0 | tier C
nn::applet::CTR::TransitionType GetTransitionType()
{
    return detail::s_TransitionType;
}

// 0x0012ACF0 | tier C
void SetMessageCommand(unsigned int command)
{
    detail::s_MessageCommand = command;
}

// 0x0012AD00 | nintendogs:callgraph [tier A]
void SetTransitionType(nn::applet::CTR::TransitionType type)
{
    detail::s_TransitionType = type;
}

// 0x0012AD10 | nintendogs:callgraph [tier A]
nn::applet::CTR::HomeButtonState GetHomeButtonState()
{
    return detail::s_HomeButtonState;
}

// 0x0012AD20 | nintendogs:callgraph [tier A]
void SetHomeButtonState(nn::applet::CTR::HomeButtonState state)
{
    detail::s_HomeButtonState = state;
}

// 0x0012AD30 (name is ours)
void SetShutdownCallbackFlag()
{
    detail::s_IsToCallShutdownCallback = true;
}

// 0x0012AD44 | tier C
bool IsToCallShutdownCallback()
{
    return detail::s_IsToCallShutdownCallback;
}

// 0x0012AD54 | tier C
void ClearShutdownCallbackFlag()
{
    detail::s_IsToCallShutdownCallback = false;
}

// 0x0012AD68 | nintendogs:callgraph [tier A]
void SetSleepNotificationState(nn::applet::CTR::SleepNotificationState state)
{
    detail::s_SleepNotificationState = state;
}

// 0x0012AD78 (name is ours)
void SetPowerButtonCallbackFlag()
{
    detail::s_IsToCallPowerButtonCallback = true;
}

// 0x0012AD8C | tier C
bool IsToCallPowerButtonCallback()
{
    return detail::s_IsToCallPowerButtonCallback;
}

// 0x0012AD9C | tier C
void ClearPowerButtonCallbackFlag()
{
    detail::s_IsToCallPowerButtonCallback = false;
}

// 0x0012ADB0 | nintendogs:callgraph [tier A]
u32 GetId()
{
    return detail::s_Id;
}

// 0x00131644 | fefates:bytes [tier B]
void CloseAppletHook(bool prepareToExit)
{
    if (prepareToExit) {
        PrepareToExit();
    }
    if (detail::s_Callbacks.closeHook1) {
        detail::s_Callbacks.closeHook1(detail::s_CallbackArguments[detail::CALLBACK_CLOSE_HOOK_1]);
    }
    if (detail::s_Callbacks.closeHook2) {
        detail::s_Callbacks.closeHook2(detail::s_CallbackArguments[detail::CALLBACK_CLOSE_HOOK_2]);
    }
}

// 0x00131684 | tier C
u32 GetMessageCommand()
{
    return detail::s_MessageCommand;
}

// 0x001372E8 | tier C
void PrepareToExit()
{
    detail::s_pExitLock->LockForWrite();
}

// 0x001372F8 | nintendogs:bytes [tier A]
bool IsSystemApplet()
{
    return (detail::s_Attribute & ATTRIBUTE_TYPE_MASK) == APPLET_TYPE_SYSTEM;
}

// 0x0047F8C4 | nintendogs:bytes [tier A]
void DisableSleep(bool rejectQuery)
{
    detail::s_IsSleepEnabled = false;
    if (rejectQuery) {
        SetSleepNotificationState(SLEEP_NOTIFICATION_REJECTED);
        detail::ReplySleepQueryToManager(REPLY_REJECT);
    }
}

// 0x0047F8FC | fefates:bytes [tier B]
void JumpToManual()
{
    u8 parameter[MANUAL_PARAMETER_SIZE];
    parameter[0] = 'A';
    parameter[4] = 1;
    parameter[1] = 'S';
    parameter[2] = 'H';
    parameter[3] = 'P';
    nn::Handle handle = INVALID_HANDLE;
    detail::LockTransition(TRANSITION_HOME_MENU, true);
    SetExpectationToJumpToHome(false);
    if (GetAppletType() == 0 || GetAppletType() == APPLET_TYPE_SYSTEM) {
        detail::CancelLibraryAppletIfRegistered(false, 0);
        if (detail::s_Callbacks.leaveApplet) {
            detail::s_Callbacks.leaveApplet(detail::s_CallbackArguments[detail::CALLBACK_LEAVE_APPLET]);
        }
    }
    detail::PrepareToJumpToHomeMenu();
    detail::JumpToHomeMenu(parameter, MANUAL_PARAMETER_SIZE, handle);
}

// 0x0047F9B0 | nintendogs:callgraph [tier A]
bool IsEnableSleep()
{
    return detail::s_IsSleepEnabled;
}

// 0x0047F9C0 | fefates:bytes [tier B]
nn::Result GetAppletVersion(unsigned int appId, unsigned short* pVersion)
{
    detail::LockAndConnect();
    nn::Result result = detail::APPLET::GetAppletProgramInfo(appId, PROGRAM_INFO_FLAGS, pVersion);
    detail::DisconnectAndUnlock();
    return result;
}

// 0x0047F9F8 | fefates:bytes [tier B]
bool IsHomeMenuResident()
{
    u32 attribute = GetAttribute();
    return IsSystemApplet() && (attribute & ATTRIBUTE_RESIDENT) && (attribute & ATTRIBUTE_HOME_MENU);
}

// 0x0047FA28 (name is ours)
bool IsInternetBrowserAvailable()
{
    return detail::IsInternetBrowserAvailable();
}

// 0x0047FA98 (name after 3dbrew)
nn::Result StartExitTask(bool flag)
{
    return detail::CallUtility(UTILITY_START_EXIT_TASK, reinterpret_cast<const u8*>(&flag), sizeof(flag), 0, 0, 0);
}

// 0x0047FAC8 (name is ours)
void JumpToHomeMenuIfRequested()
{
    bool isSleepEnabled = IsEnableSleep();
    if (isSleepEnabled) {
        DisableSleep(true);
    }
    if (IsExpectedToJumpToHomeMenu() || GetHomeButtonState() != HOME_BUTTON_NONE) {
        SetExpectationToJumpToHome(false);
        if (GetAppletType() == 0 || GetAppletType() == APPLET_TYPE_SYSTEM) {
            detail::CancelLibraryAppletIfRegistered(false, 0);
            if (detail::s_Callbacks.leaveApplet) {
                detail::s_Callbacks.leaveApplet(detail::s_CallbackArguments[detail::CALLBACK_LEAVE_APPLET]);
            }
        }
        detail::PrepareToJumpToHomeMenu();
        detail::JumpToHomeMenu(0, 0, INVALID_HANDLE);
    }
    detail::WaitForStarting(0, 0, 0, 0, 0, WAIT_INFINITE);
    if (isSleepEnabled) {
        EnableSleep(true);
    }
}

// 0x0047FBE0 | tier C
nn::applet::CTR::SleepNotificationState GetSleepNotificationState()
{
    return detail::s_SleepNotificationState;
}

// 0x0047FBF0 (name is ours)
void JumpToRequestedSystemApplet()
{
    bool isSleepEnabled = IsEnableSleep();
    if (isSleepEnabled) {
        DisableSleep(true);
    }
    detail::CancelLibraryAppletIfRegistered(false, 0);
    if (detail::s_Callbacks.leaveApplet) {
        detail::s_Callbacks.leaveApplet(detail::s_CallbackArguments[detail::CALLBACK_LEAVE_APPLET]);
    }
    u32 homeMenuAppId;
    u32 requestedAppId;
    detail::GetAppletManInfo(POS_SYS, 0, &requestedAppId, &homeMenuAppId, 0);
    if (requestedAppId == 0 || requestedAppId == homeMenuAppId) {
        detail::PrepareToJumpToHomeMenu();
        detail::JumpToHomeMenu(0, 0, INVALID_HANDLE);
    } else {
        detail::PrepareToStartSystemApplet(requestedAppId);
        detail::StartSystemApplet(requestedAppId, 0, 0, INVALID_HANDLE);
    }
    detail::WaitForStarting(0, 0, 0, 0, 0, WAIT_INFINITE);
    if (isSleepEnabled) {
        EnableSleep(true);
    }
}

// 0x0047FD04 | nintendogs:callgraph [tier A]
bool IsExpectedToJumpToHomeMenu()
{
    return detail::s_IsExpectedToJumpToHome;
}

// 0x0047FD14 (name is ours)
bool IsSleepReplyPending()
{
    return GetSleepNotificationState() == SLEEP_NOTIFICATION_LATER;
}

// 0x0047FE7C | nintendogs:bytes [tier A]
bool IsExpectedToProcessHomeButton()
{
    return IsExpectedToJumpToHomeMenu() || GetHomeButtonState() != HOME_BUTTON_NONE;
}

} // namespace CTR
} // namespace applet
} // namespace nn
