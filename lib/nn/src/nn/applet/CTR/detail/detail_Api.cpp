// The detail functions of applet_API.cpp (file name from the static initializer 0x00788744): the
// connection to APT, the state, the transitions and the parameters
#include "nn/applet/CTR/detail/detail_Api.h"
#include <new>
#include <string.h>
#include "nn/applet/CTR/detail/applet_APPLET.h"
#include "nn/applet/CTR/detail/applet_Timeout.h"
#include "nn/camera/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/dsp/CTR/CTR_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/fs_Types.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_ReaderWriterLock.h"
#include "nn/os/os_Thread.h"
#include "nn/srv/srv_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
namespace {
// results (module 51 applet; the names are ours)
const bit32 RESULT_NO_PARAMETER = 0xC8A0CFEF;       // status, invalid state, 1007
const bit32 RESULT_BUSY_1 = 0xC8A0CFF0;             // status, invalid state, 1008
const bit32 RESULT_BUSY_2 = 0xE0A0CC08;             // permanent, invalid state, 8
const bit32 RESULT_BUSY_3 = 0xC8A0CC02;             // status, invalid state, 2
const bit32 RESULT_NOT_CONNECTED = 0xE0A0CFF8;      // permanent, invalid state, 1016
const bit32 RESULT_ALREADY_INITIALIZED = 0xE0A0CFF9; // permanent, invalid state, 1017
// what ReleaseGpuRight returns when the right is not held (gsp results, ignored)
const bit32 RESULT_GPU_RIGHT_NOT_HELD_1 = 0xD8A02A05;
const bit32 RESULT_GPU_RIGHT_NOT_HELD_2 = 0xD9001BF7;

// the slots asked by CancelLibraryAppletIfRegistered (names are ours)
const u32 APP_ID_LIBRARY_APPLET_OF_APPLICATION = 0x400;
const u32 APP_ID_LIBRARY_APPLET_OF_SYSTEM_APPLET = 0x200;

// AppletUtility ids (3dbrew "APT:AppletUtility")
const u32 UTILITY_SLEEP_IF_SHELL_CLOSED = 4;
const u32 UTILITY_LOCK_TRANSITION = 5;
const u32 UTILITY_UNLOCK_TRANSITION = 7;

// the commands of parameters (3dbrew "NS and APT Services", Command)
const u32 COMMAND_WAKEUP = 1;
const u32 COMMAND_WAKEUP_BY_EXIT = 10;
const u32 COMMAND_WAKEUP_BY_PAUSE = 11;
const u32 COMMAND_WAKEUP_BY_CANCEL = 12;
const u32 COMMAND_WAKEUP_BY_CANCELALL = 13;
const u32 COMMAND_WAKEUP_BY_POWER_BUTTON_CLICK = 14;
const u32 COMMAND_WAKEUP_TO_JUMP_HOME = 15;
const u32 COMMAND_WAKEUP_TO_LAUNCH_APPLICATION = 17;
const u32 COMMAND_18 = 18;
const u32 COMMAND_64 = 64;
const u32 COMMAND_65 = 65;
// a command with this bit ends the client thread once it is sent
const u32 COMMAND_FLAG_EXIT = 0x10000;

const size_t PARAMETER_SIZE_MAX = 0x1000;
const size_t JUMP_PARAMETER_SIZE_MAX = 0x300;
const size_t JUMP_HMAC_SIZE_MAX = 0x20;

// how long the loops wait before they try a busy service again
const s64 RETRY_WAIT_MSEC = 10;

// the HOME menu transition of LockTransition / UnlockTransition
const u32 TRANSITION_HOME_MENU = 1;

// the service answers later
inline bool IsBusy(nn::Result result)
{
    return result == RESULT_BUSY_1 || result == RESULT_BUSY_2 || result == RESULT_BUSY_3;
}

inline void WaitToRetry()
{
    nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(RETRY_WAIT_MSEC));
}

// the transition bit of the kind of the program for UnlockTransition
inline u32 GetTransitionBit()
{
    switch (GetAppletType()) {
    case 0:
        return 0x10;
    case 1:
        return 0x20;
    case 2:
        return 0x40;
    case 3:
        return 0x80;
    }
    return 0;
}

} // namespace

// 0x00975AD1
bool s_IsVramSaved;
// 0x00975AD2
bool s_IsInitialized;
// 0x00975AD3
bool s_HasGpuRight;
// 0x00975AD4
bool s_IsDspSleptByApplet;
// taken for writing by PrepareToExit (made on the first InitializeConnect)
// 0x00975AD8
nn::os::ReaderWriterLock* s_pExitLock;
// 0x00975AE0
u64 s_ExitLockStorage[1];
// 0x00975AF8
bool s_IsInitializedByLauncher;
// 0x00975AF9
bool s_IsActive;
// 0x00975AFA
nn::applet::CTR::HomeButtonState s_AbsoluteHomeButtonState;
// the notification of the sleep that is not handled yet (1 query, 2 accepted, 3 awake, 4 canceled)
// 0x00975AFB
u8 s_SleepSysState;
// 0x00975AFC
u8 s_ShutdownState;
// 0x00975AFD
nn::applet::CTR::PowerButtonState s_PowerButtonState;
// 0x00975AFE
nn::applet::CTR::OrderToCloseState s_OrderToCloseState;
// 0x00975AFF
bool s_IsToCallPowerButtonCallback;
// 0x00975B00
bool s_IsToCallShutdownCallback;
// 0x00975B01
bool s_IsReceivedWakeupByCancel;
// 0x00975B02
nn::applet::CTR::TransitionType s_TransitionType;
// 0x00975B03
nn::applet::CTR::SleepNotificationState s_SleepNotificationState;
// 0x00975B04
nn::applet::CTR::HomeButtonState s_HomeButtonState;
// 0x00975B05
bool s_IsExpectedToJumpToHome;
// the command the client thread made from a notification (COMMAND_HOME_BUTTON_*)
// 0x00975B08
u32 s_MessageCommand;
// 0x00975B0C
u32 s_Id;
// 0x00975B10
u32 s_Attribute;
// the parameter of the first wakeup of an application (Enable)
// 0x00975B14
bool s_IsInitialParamValid;
// 0x00975B15
nn::applet::CTR::WakeupState s_InitialWakeupState;
// 0x00975B18
u32 s_InitialParamSenderId;
// 0x00975B1C
s32 s_InitialParamSize;
// 0x00AE0A5C
u8 s_InitialParamBuffer[0x1000];
// 0x00975B20
bool s_IsSleepEnabled;
// 0x00975B24
nn::applet::CTR::SysSleepAcceptedCallbackInfo* s_pSleepAcceptedCallbackHead;
// 0x00975B28
nn::applet::CTR::SysSleepAcceptedCallbackInfo* s_pSleepAcceptedCallbackTail;
// 0x00975B2C
Callbacks s_Callbacks;
// 0x00975B6C
uptr s_CallbackArguments[CALLBACK_COUNT];
// 0x0097E7E4
const char* s_PortNames[2] = {"APT:S", "APT:U"};
// with it Connect tries the user port when the first port fails (never set in this program)
// 0x0097E808
bool s_IsPortFallbackEnabled;
// 0x0097E80C
const char* s_DefaultPortName = "APT:A";
// 0x0097E810
const char* s_pPortName;
// the lock of the service across the programs (GetLockHandle)
// 0x0097E814
nn::Handle s_Mutex;
// 0x008B3330
const nn::Handle INVALID_SESSION;

namespace {
// a session to the current port name (the default when none is set)
inline nn::Result ConnectToPort()
{
    if (s_pPortName == 0) {
        s_pPortName = s_DefaultPortName;
    }
    if (s_Session.IsValid()) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    return nn::srv::GetServiceHandle(&s_Session, s_pPortName, strlen(s_pPortName), 0);
}
} // namespace

// 0x0011E59C | tier C
nn::Result GetTargetPlatform(nn::ptm::CTR::TargetPlatform* pPlatform)
{
    LockAndConnect();
    nn::Result result = APPLET::GetTargetPlatform(pPlatform);
    DisconnectAndUnlock();
    return result;
}

// 0x0011E5C0 | fefates:bytes [tier B]
nn::Result InitializeConnect(unsigned int appId, unsigned int attribute, int priority)
{
    if (s_IsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    s_IsInitialized = true;
    if (s_pExitLock == 0) {
        s_pExitLock = new (s_ExitLockStorage) nn::os::ReaderWriterLock();
        s_pExitLock->Initialize();
    }
    Connect();

    nn::Handle mutex;
    unsigned state;
    unsigned newAttribute;
    nn::Result result = APPLET::GetLockHandle(&mutex, attribute, &newAttribute, &state);
    if (result.IsFailure()) {
        Disconnect();
        return result;
    }
    InitializeMutex(mutex);
    SetPowerButtonState(static_cast<PowerButtonState>(state & 1));
    SetOrderToCloseState(static_cast<OrderToCloseState>((state & 2) >> 1));
    Disconnect();
    SetId(appId);
    SetAttribute(newAttribute);
    if (IsInfoAccess()) {
        return nn::Result();
    }

    LockAndConnect();
    SetActive();
    nn::Handle parameterEvent;
    nn::Handle notificationEvent;
    nn::err::CTR::ThrowFatalErrIfFailure(APPLET::Initialize(GetId(), GetAttribute(), &notificationEvent, &parameterEvent));
    InitializeWrapper();
    InitializeClientThread(priority, parameterEvent, notificationEvent);
    DisconnectAndUnlock();
    if (!IsApplication()) {
        nn::gxlow::CTR::SetAppletMode();
    }
    return nn::Result();
}

// 0x0011E768 (name after the command)
nn::Result SetApplicationCpuTimeLimit(u32 fixed, u32 percent)
{
    LockAndConnect();
    nn::Result result = APPLET::SetApplicationCpuTimeLimit(fixed, percent);
    DisconnectAndUnlock();
    return result;
}

// 0x0011E7D0 | nintendogs:bytes [tier A]
void Enable(bool waitForStart)
{
    if (waitForStart) {
        EnableSleep(false);
    }
    LockAndConnect();
    nn::err::CTR::ThrowFatalErrIfFailure(APPLET::Enable(GetAttribute()));
    DisconnectAndUnlock();
    if (IsApplication() && (GetAttribute() & 0x20) == 0) {
        SetTransitionType(TRANSITION_FIRST_WAKEUP);
        unsigned senderAppId;
        int size;
        WakeupState state = WaitForStarting(&senderAppId, GetInitialParamBuffer(), sizeof(s_InitialParamBuffer), &size, 0, WAIT_INFINITE);
        SetInitialParamSenderId(senderAppId);
        SetInitialParamSize(size);
        SetInitialParamValid();
        SetInitialWakeupState(state);
    }
}

// 0x00120264 | nintendogs:bytes [tier A]
nn::Result Disconnect()
{
    nn::Result result;
    if (!s_Session.IsValid()) {
        result = nn::Result(RESULT_NOT_CONNECTED);
    } else {
        result = nn::svc::CloseHandle(s_Session);
        s_Session = INVALID_SESSION;
    }
    nn::err::CTR::ThrowFatalErrIfFailure(result);
    return result;
}

// 0x001202B0 | tier C
void InitializeMutex(nn::Handle mutex)
{
    s_Mutex = mutex;
}

// 0x001202C0 | nintendogs:callgraph [tier A]
void SetInitialParamSize(int size)
{
    s_InitialParamSize = size;
}

// 0x001202D0 | nintendogs:callgraph [tier A]
void SetInitialParamValid()
{
    s_IsInitialParamValid = true;
}

// 0x001202E4 | nintendogs:callgraph [tier A]
u8* GetInitialParamBuffer()
{
    return s_InitialParamBuffer;
}

// 0x001202F0 | nintendogs:callgraph [tier A]
void SetInitialWakeupState(nn::applet::CTR::WakeupState state)
{
    s_InitialWakeupState = state;
}

// 0x001204B8 | nintendogs:callgraph [tier A]
void SetInitialParamSenderId(unsigned senderAppId)
{
    s_InitialParamSenderId = senderAppId;
}

// 0x00120624 | fefates:bytes [tier B]
nn::Result Connect()
{
    if (s_IsPortFallbackEnabled) {
        nn::Result result = ConnectToPort();
        if (result.IsFailure()) {
            s_pPortName = 0;
            SetUserPortName();
            result = ConnectToPort();
            nn::err::CTR::ThrowFatalErrIfFailure(result);
        }
        return result;
    }
    nn::Result result = ConnectToPort();
    nn::err::CTR::ThrowFatalErrIfFailure(result);
    return result;
}

// 0x00124D70 (name is ours)
void SetReceivedWakeupByCancel()
{
    s_IsReceivedWakeupByCancel = true;
}

// 0x001250DC | nintendogs:bytes [tier A]
void LockTransition(unsigned transition, bool flag)
{
    struct
    {
        u32 transition;
        bool flag;
    } input;
    input.transition = transition;
    input.flag = flag;
    CallUtility(UTILITY_LOCK_TRANSITION, reinterpret_cast<const u8*>(&input), sizeof(input), 0, 0, 0);
}

// 0x00125110 | nintendogs:bytes [tier A]
void UnlockTransition(unsigned transition)
{
    CallUtility(UTILITY_UNLOCK_TRANSITION, reinterpret_cast<const u8*>(&transition), sizeof(transition), 0, 0, 0);
}

// 0x0012513C | fefates:bytes [tier B]
nn::Result RestartApplication(const void* pParameter, unsigned int size)
{
    nn::Result result = PrepareToJumpApplication(APP_JUMP_TYPE_RESTART, 0xFFFFFFFFFFFFFFFFULL, nn::fs::MEDIA_TYPE_NAND);
    if (result.IsFailure()) {
        return result;
    }
    return DoApplicationJump(static_cast<const unsigned char*>(pParameter), size, 0, 0);
}

// 0x00125188 | nintendogs:bytes [tier A]
nn::Result RestoreVramSysArea()
{
    if (!s_IsVramSaved) {
        return nn::Result();
    }
    s_IsVramSaved = false;
    return nn::gxlow::CTR::RestoreVramSysArea();
}

// 0x001251DC | nintendogs:bytes [tier A]
void SleepIfShellClosed()
{
    CallUtility(UTILITY_SLEEP_IF_SHELL_CLOSED, 0, 0, 0, 0, 0);
}

// 0x00125208 | tier C
void SetPowerButtonState(nn::applet::CTR::PowerButtonState state)
{
    s_PowerButtonState = state;
}

// 0x00125218 | tier C
nn::applet::CTR::OrderToCloseState GetOrderToCloseState()
{
    return s_OrderToCloseState;
}

// 0x00125228 | tier C
void SetOrderToCloseState(nn::applet::CTR::OrderToCloseState state)
{
    s_OrderToCloseState = state;
}

// waits for a parameter (or a command of the client thread) up to the timeout
// 0x0012523C | fefates:bytes [tier B]
nn::Result Receive(unsigned int* pSenderAppId, unsigned int* pCommand, unsigned char* pBuffer, unsigned int bufferSize, int* pSize, nn::Handle* pHandle, nn::fnd::TimeSpan timeout)
{
    if (timeout.GetNanoSeconds() == WAIT_INFINITE.GetNanoSeconds()) {
        return TryReceive(pSenderAppId, pCommand, pBuffer, bufferSize, pSize, pHandle, false);
    }
    s64 startTick = GetTimeoutStart(timeout);
    for (;;) {
        nn::Result result = TryReceive(pSenderAppId, pCommand, pBuffer, bufferSize, pSize, pHandle, true);
        if (result.IsSuccess()) {
            return result;
        }
        if (result == RESULT_NO_PARAMETER && IsTimedOut(timeout, startTick)) {
            return result;
        }
        WaitToRetry();
    }
}

// 0x00125500 | nintendogs:callgraph [tier A]
bool IsActive()
{
    return s_IsActive;
}

// 0x00125510 | tier C
void SetActive()
{
    s_IsActive = true;
}

// takes a parameter when the control event is signalled (or waits for it); a command of the
// client thread comes first
// 0x0012ADC0 | nintendogs:bytes-fuzzy [tier A]
nn::Result TryReceive(unsigned* pSenderAppId, unsigned* pCommand, unsigned char* pBuffer, unsigned bufferSize, int* pSize, nn::Handle* pHandle, bool noWait)
{
    if (noWait) {
        if (!TryWaitForControlEvent()) {
            return nn::Result(RESULT_NO_PARAMETER);
        }
    } else {
        WaitForControlEvent();
    }

    nn::Result result;
    if (GetMessageCommand() != 0) {
        *pCommand = GetMessageCommand();
        SetMessageCommand(0);
        ClearControlEvent();
        if (pSenderAppId) {
            *pSenderAppId = 0;
        }
        if (pSize) {
            *pSize = 0;
        }
        if (pHandle) {
            *pHandle = INVALID_HANDLE;
        }
        result = nn::Result();
    } else {
        unsigned senderAppId;
        unsigned command;
        u32 buffer;
        int size;
        nn::Handle handle;
        if (pSenderAppId == 0) {
            pSenderAppId = &senderAppId;
        }
        if (pCommand == 0) {
            pCommand = &command;
        }
        if (pBuffer == 0 || bufferSize == 0) {
            bufferSize = 0;
            pBuffer = reinterpret_cast<unsigned char*>(&buffer);
        }
        if (pSize == 0) {
            pSize = &size;
        }
        if (pHandle == 0) {
            pHandle = &handle;
        }
        LockAndConnect();
        result = APPLET::ReceiveParameter(pSenderAppId, GetId(), pCommand, pBuffer, bufferSize, pSize, pHandle);
        DisconnectAndUnlock();
        if (handle.IsValid()) {
            nn::svc::CloseHandle(handle);
        }
    }
    ClearControlEvent();
    return result;
}

// AppletUtility with the session; an empty input or output is one byte
// 0x0012AEF4 | nintendogs:bytes [tier A]
nn::Result CallUtility(unsigned utilityId, const unsigned char* pInput, unsigned inputSize, unsigned char* pOutput, unsigned outputSize, int* pResult)
{
    u32 input = 0;
    u32 output = 0;
    s32 utilityResult;
    LockAndConnect();
    unsigned char* pOutputBuffer = pOutput;
    unsigned outputBufferSize = outputSize;
    if (pOutput == 0 || outputSize == 0) {
        outputBufferSize = 1;
        pOutputBuffer = reinterpret_cast<unsigned char*>(&output);
    }
    if (pInput == 0 || inputSize == 0) {
        inputSize = 1;
        pInput = reinterpret_cast<const unsigned char*>(&input);
    }
    nn::Result result = APPLET::AppletUtility(utilityId, pInput, inputSize, pOutputBuffer, outputBufferSize, &utilityResult);
    nn::err::CTR::ThrowFatalErrIfFailure(result);
    if (pOutput == 0 || outputSize == 0) {
        utilityResult = 0;
    }
    if (pResult) {
        *pResult = utilityResult;
    }
    DisconnectAndUnlock();
    return result;
}

// 0x0012AFC0 | tier C
void SetInactive()
{
    s_IsActive = false;
}

// the DSP sleeps while another program runs
// 0x0012AFD4 | nintendogs:bytes [tier A]
void AssignDspRight(bool isAssigned)
{
    if (isAssigned) {
        if (s_IsDspSleptByApplet) {
            nn::dsp::CTR::WakeUp();
            s_IsDspSleptByApplet = false;
        }
    } else if (nn::dsp::CTR::IsComponentLoaded()) {
        nn::dsp::CTR::Sleep();
        s_IsDspSleptByApplet = true;
    }
}

// 0x0012B024 | nintendogs:bytes [tier A]
void AssignGpuRight(bool isAssigned)
{
    if (isAssigned) {
        s_HasGpuRight = true;
        nn::err::CTR::ThrowFatalErrIfFailure(nn::gxlow::CTR::AcquireGpuRight());
    } else if (s_HasGpuRight) {
        s_HasGpuRight = false;
        nn::Result result = nn::gxlow::CTR::ReleaseGpuRight();
        if (result != RESULT_GPU_RIGHT_NOT_HELD_1 && result != RESULT_GPU_RIGHT_NOT_HELD_2) {
            nn::err::CTR::ThrowFatalErrIfFailure(result);
        }
    }
}

// 0x0012B09C | nintendogs:callgraph [tier A]
void LockAndConnect()
{
    if (s_Mutex.IsValid()) {
        nn::Result result = nn::svc::WaitSynchronization1(s_Mutex, -1);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
    }
    Connect();
}

// 0x0012B0D0 | nintendogs:bytes [tier A]
bool CancelParameter(bool checkSender, unsigned senderAppId, bool checkReceiver, unsigned receiverAppId)
{
    LockAndConnect();
    bool isCanceled;
    nn::err::CTR::ThrowFatalErrIfFailure(APPLET::CancelParameter(checkSender, senderAppId, checkReceiver, receiverAppId, &isCanceled));
    DisconnectAndUnlock();
    return isCanceled;
}

// waits until the program runs again (after it started another one); what woke it up
// 0x0012B124 | nintendogs:bytes-fuzzy [tier A]
nn::applet::CTR::WakeupState WaitForStarting(unsigned* pSenderAppId, unsigned char* pBuffer, unsigned bufferSize, int* pSize, nn::Handle* pHandle, nn::fnd::TimeSpan timeout)
{
    unsigned lastSenderAppId = 0;
    unsigned command = 0;
    TransitionType transition = GetTransitionType();
    SetTransitionType(TRANSITION_NONE);
    if (transition == TRANSITION_NO_WAIT) {
        if (pSenderAppId) {
            *pSenderAppId = 0;
        }
        if (pSize) {
            *pSize = 0;
        }
        if (pHandle) {
            *pHandle = INVALID_HANDLE;
        }
        return WAKEUP_STATE_NONE;
    }
    NotifyToWait();
    SetInactive();
    if (transition == TRANSITION_FIRST_WAKEUP) {
        SetTransitionType(TRANSITION_NONE);
    } else {
        SleepIfShellClosed();
    }

    nn::Result result;
    for (;;) {
        bool isDone = true;
        nn::Handle handle;
        unsigned senderAppId;
        unsigned receivedCommand;
        int size;
        result = Receive(&senderAppId, &receivedCommand, pBuffer, bufferSize, &size, &handle, timeout);
        if (result.GetDescription() != (RESULT_NO_PARAMETER & 0x3FF)) {
            if (result.IsSuccess() && s_Callbacks.parameter) {
                isDone = s_Callbacks.parameter(s_CallbackArguments[CALLBACK_PARAMETER], senderAppId, receivedCommand,
                                               pBuffer, bufferSize, size, handle);
            }
            lastSenderAppId = senderAppId;
            command = receivedCommand;
            if (pSize) {
                *pSize = size;
            }
            if (pHandle) {
                *pHandle = handle;
            }
        }
        if (!isDone) {
            continue;
        }
        if (pSenderAppId) {
            *pSenderAppId = lastSenderAppId;
        }
        if (result.IsFailure()) {
            break;
        }
        if (command == COMMAND_WAKEUP || command == COMMAND_WAKEUP_BY_PAUSE || command == COMMAND_WAKEUP_BY_EXIT ||
            command == COMMAND_WAKEUP_BY_CANCEL || command == COMMAND_WAKEUP_BY_CANCELALL ||
            command == COMMAND_WAKEUP_TO_JUMP_HOME || command == COMMAND_WAKEUP_BY_POWER_BUTTON_CLICK ||
            command == COMMAND_WAKEUP_TO_LAUNCH_APPLICATION || command == COMMAND_18 || command == COMMAND_64 ||
            command == COMMAND_65) {
            SetActive();
            break;
        }
    }
    if (result.GetDescription() == (RESULT_NO_PARAMETER & 0x3FF)) {
        return static_cast<WakeupState>(-1);
    }

    if (GetAppletType() == 0 && transition != TRANSITION_CANCEL_LIBRARY_APPLET && command != COMMAND_WAKEUP_BY_CANCEL &&
        command != COMMAND_WAKEUP) {
        AssignGpuRight(true);
        nn::err::CTR::ThrowFatalErrIfFailure(RestoreVramSysArea());
    }

    WakeupState state;
    switch (command) {
    case COMMAND_WAKEUP:
        state = WAKEUP_STATE_WAKEUP;
        break;
    case COMMAND_WAKEUP_BY_EXIT:
        state = WAKEUP_STATE_BY_EXIT;
        break;
    case COMMAND_WAKEUP_BY_PAUSE:
        state = WAKEUP_STATE_BY_PAUSE;
        break;
    case COMMAND_WAKEUP_BY_CANCEL:
        state = WAKEUP_STATE_BY_CANCEL;
        nn::dsp::CTR::OrderToWaitForFinalize();
        SetReceivedWakeupByCancel();
        break;
    case COMMAND_WAKEUP_BY_CANCELALL:
        state = WAKEUP_STATE_BY_CANCEL_ALL;
        nn::dsp::CTR::OrderToWaitForFinalize();
        break;
    case COMMAND_WAKEUP_BY_POWER_BUTTON_CLICK:
        state = WAKEUP_STATE_BY_POWER_BUTTON_CLICK;
        break;
    case COMMAND_WAKEUP_TO_JUMP_HOME:
        state = WAKEUP_STATE_TO_JUMP_HOME;
        SetHomeButtonState(HOME_BUTTON_SINGLE);
        SetExpectationToJumpToHome(true);
        break;
    case COMMAND_WAKEUP_TO_LAUNCH_APPLICATION:
        state = WAKEUP_STATE_TO_LAUNCH_APPLICATION;
        break;
    case COMMAND_18:
        state = WAKEUP_STATE_COMMAND_18;
        break;
    case COMMAND_64:
    case COMMAND_65:
        state = static_cast<WakeupState>(command);
        break;
    default:
        state = WAKEUP_STATE_WAKEUP;
        break;
    }

    // the DSP (and the camera) come back unless the program is canceled or replaced
    if (state != WAKEUP_STATE_BY_CANCEL && state != WAKEUP_STATE_BY_CANCEL_ALL &&
        state != WAKEUP_STATE_TO_LAUNCH_APPLICATION) {
        AssignDspRight(true);
        if (GetAppletType() == 0 &&
            (transition == TRANSITION_JUMP_TO_HOME_MENU || transition == TRANSITION_START_SYSTEM_APPLET)) {
            AssignCameraRight(true);
        }
    }

    if (state == WAKEUP_STATE_WAKEUP || state == WAKEUP_STATE_BY_EXIT || state == WAKEUP_STATE_BY_PAUSE ||
        state == WAKEUP_STATE_BY_CANCEL || state == WAKEUP_STATE_BY_CANCEL_ALL ||
        state == WAKEUP_STATE_BY_POWER_BUTTON_CLICK || state == WAKEUP_STATE_TO_LAUNCH_APPLICATION ||
        state == WAKEUP_STATE_COMMAND_64 || state == WAKEUP_STATE_COMMAND_65) {
        UnlockTransition(GetTransitionBit());
        SleepIfShellClosed();
    }
    if (state == WAKEUP_STATE_TO_JUMP_HOME) {
        LockTransition(TRANSITION_HOME_MENU, true);
    }
    if (transition == TRANSITION_JUMP_TO_HOME_MENU || transition == TRANSITION_START_LIBRARY_APPLET ||
        transition == TRANSITION_START_SYSTEM_APPLET || transition == TRANSITION_APPLICATION_JUMP) {
        if (state != WAKEUP_STATE_BY_CANCEL && state != WAKEUP_STATE_BY_CANCEL_ALL &&
            state != WAKEUP_STATE_COMMAND_18) {
            AssignGpuRight(true);
        }
        if (GetAppletType() == 0 && state != WAKEUP_STATE_TO_JUMP_HOME) {
            SetHomeButtonState(HOME_BUTTON_NONE);
            UnlockTransition(TRANSITION_HOME_MENU);
            SleepIfShellClosed();
        }
    }
    return state;
}

// 0x0012B55C | tier C
u8 GetSleepSysState()
{
    return s_SleepSysState;
}

// 0x0012B56C (name is ours)
void SetShutdownState(u8 state)
{
    s_ShutdownState = state;
}

// 0x0012B57C (name is ours)
void SetSleepSysState(u8 state)
{
    s_SleepSysState = state;
}

// starts another application (or this one again); does not return
// 0x0012B58C | fefates:bytes [tier B]
nn::Result DoApplicationJump(const unsigned char* pParameter, unsigned int size, const unsigned char* pHmac, unsigned int hmacSize)
{
    SetTransitionType(TRANSITION_APPLICATION_JUMP);
    nn::gxlow::CTR::StopLcdDisplay();
    AssignDspRight(false);
    AssignGpuRight(false);
    nn::camera::CTR::detail::LeaveApplication();
    if (pParameter == 0) {
        size = 0;
    } else if (size > JUMP_PARAMETER_SIZE_MAX) {
        size = JUMP_PARAMETER_SIZE_MAX;
    }
    if (pHmac == 0) {
        hmacSize = 0;
    } else if (hmacSize > JUMP_HMAC_SIZE_MAX) {
        hmacSize = JUMP_HMAC_SIZE_MAX;
    }
    nn::Result result;
    for (;;) {
        LockAndConnect();
        result = APPLET::DoApplicationJump(pParameter, size, pHmac, hmacSize);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    nn::err::CTR::ThrowFatalErrIfFailure(result);
    SetInactive();
    CloseAppletHook(true);
    nn::svc::ExitProcess();
}

// 0x0012B6C4 | tier C
void ClearSleepSysState()
{
    s_SleepSysState = 0;
}

// 0x0012B6D8 | nintendogs:bytes [tier A]
void NotifyToWait()
{
    LockAndConnect();
    nn::err::CTR::ThrowFatalErrIfFailure(APPLET::NotifyToWait(GetId()));
    DisconnectAndUnlock();
}

// 0x0012B6FC | nintendogs:bytes [tier A]
void DisconnectAndUnlock()
{
    Disconnect();
    if (s_Mutex.IsValid()) {
        nn::Result result = nn::svc::ReleaseMutex(s_Mutex);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
    }
}

// only after the order to close (or a cancel)
// 0x0012B76C | fefates:bytes [tier B]
nn::Result CloseApplication(const unsigned char* pParameter, unsigned int size, nn::Handle handle)
{
    if (GetOrderToCloseState() == ORDER_TO_CLOSE_NONE && !IsReceivedWakeupByCancel()) {
        nndbgPanic();
    }
    return CloseApplicationCore(pParameter, size, handle);
}

// ends the application; does not return
// 0x0012B7A8 | fefates:bytes [tier B]
nn::Result CloseApplicationCore(const unsigned char* pParameter, unsigned int size, nn::Handle handle)
{
    nn::Result result;
    if (GetTransitionType() != TRANSITION_CLOSE_APPLICATION) {
        CancelLibraryAppletIfRegistered(false, 0);
        SetTransitionType(TRANSITION_CLOSE_APPLICATION);
        for (;;) {
            LockAndConnect();
            result = APPLET::PrepareToCloseApplication(true);
            DisconnectAndUnlock();
            if (!IsBusy(result)) {
                break;
            }
            WaitToRetry();
        }
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    CloseAppletHook(true);
    AssignGpuRight(false);
    for (;;) {
        LockAndConnect();
        result = APPLET::CloseApplication(pParameter, size, handle);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    SetInactive();
    nn::svc::ExitProcess();
}

// 0x0012B908 | fefates:bytes [tier B]
nn::Result PrepareToJumpApplication(nn::applet::CTR::AppJumpType type, unsigned long long titleId, nn::fs::MediaType mediaType)
{
    nn::Result result;
    for (;;) {
        LockAndConnect();
        result = APPLET::PrepareToDoApplicationJump(type, titleId, mediaType);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            return result;
        }
        WaitToRetry();
    }
}

// 0x0012B9A4 | nintendogs:bytes [tier A]
void ReplySleepQueryToManager(nn::applet::CTR::QueryReply reply)
{
    LockAndConnect();
    nn::err::CTR::ThrowFatalErrIfFailure(APPLET::ReplySleepQuery(GetId(), reply));
    DisconnectAndUnlock();
}

// 0x0012B9D0 | tier C
nn::applet::CTR::HomeButtonState GetAbsoluteHomeButtonState()
{
    return s_AbsoluteHomeButtonState;
}

// 0x0012B9E0 (name is ours)
void SetAbsoluteHomeButtonState(nn::applet::CTR::HomeButtonState state)
{
    s_AbsoluteHomeButtonState = state;
}

// 0x0012B9F0 | tier C
void ClearAbsoluteHomeButtonState()
{
    s_AbsoluteHomeButtonState = HOME_BUTTON_NONE;
}

// 0x0012BA04 | nintendogs:bytes [tier A]
void ReplySleepNotificationCompleteToManager()
{
    LockAndConnect();
    nn::err::CTR::ThrowFatalErrIfFailure(APPLET::ReplySleepNotificationComplete(GetId()));
    DisconnectAndUnlock();
}

// sends a parameter, tries again while the receiver has not taken the last one
// 0x0012BA28 | nintendogs:callseq-callee [tier A]
nn::Result Send(unsigned destinationAppId, unsigned command, const unsigned char* pParameter, unsigned size, nn::Handle handle, nn::fnd::TimeSpan timeout)
{
    s64 startTick = GetTimeoutStart(timeout);
    bool isExit = (command & COMMAND_FLAG_EXIT) >> 16;
    for (;;) {
        const unsigned char* pData = pParameter;
        unsigned dataSize = size;
        if (size > PARAMETER_SIZE_MAX) {
            nndbgPanic();
        }
        if (pData == 0 || dataSize == 0) {
            pData = 0;
            dataSize = 0;
        }
        LockAndConnect();
        nn::Result result = APPLET::SendParameter(GetId(), destinationAppId, command, pData, dataSize, handle);
        if (result.IsSuccess() && isExit) {
            FinalizeClientThread();
        }
        DisconnectAndUnlock();
        if (result.IsSuccess() || result != RESULT_BUSY_3) {
            return result;
        }
        if (IsTimedOut(timeout, startTick)) {
            return result;
        }
        WaitToRetry();
    }
}

// looks at the next parameter without taking it
// 0x0012BCD0 | nintendogs:bytes [tier A]
nn::Result Glance(unsigned* pSenderAppId, unsigned* pCommand, unsigned char* pBuffer, unsigned bufferSize, int* pSize, nn::Handle* pHandle)
{
    LockAndConnect();
    unsigned senderAppId;
    unsigned command;
    u32 buffer;
    int size;
    nn::Handle handle;
    if (pSenderAppId == 0) {
        pSenderAppId = &senderAppId;
    }
    if (pCommand == 0) {
        pCommand = &command;
    }
    if (pBuffer == 0 || bufferSize == 0) {
        pBuffer = reinterpret_cast<unsigned char*>(&buffer);
        bufferSize = 0;
    }
    if (pSize == 0) {
        pSize = &size;
    }
    if (pHandle == 0) {
        pHandle = &handle;
    }
    nn::Result result = APPLET::GlanceParameter(pSenderAppId, GetId(), pCommand, pBuffer, bufferSize, pSize, pHandle);
    if (handle.IsValid()) {
        nn::svc::CloseHandle(handle);
    }
    DisconnectAndUnlock();
    return result;
}

// 0x00131694 | fefates:bytes [tier B]
void SetPortName(const char* pName)
{
    if (s_pPortName == 0) {
        s_pPortName = pName;
    }
}

// cancels the library applet this program started (if one is registered) and waits until this
// program runs again
// 0x00131770 | nintendogs:bytes [tier A]
nn::Result CancelLibraryAppletIfRegistered(bool isExiting, nn::applet::CTR::WakeupState* pWakeupState)
{
    nn::Result result;
    if (pWakeupState) {
        *pWakeupState = WAKEUP_STATE_NONE;
    }
    if (IsApplication()) {
        bool isRegistered;
        LockAndConnect();
        nn::err::CTR::ThrowFatalErrIfFailure(APPLET::IsRegistered(APP_ID_LIBRARY_APPLET_OF_APPLICATION, &isRegistered));
        DisconnectAndUnlock();
        if (!isRegistered) {
            return result;
        }
    }
    if (IsSystemApplet()) {
        bool isRegistered;
        LockAndConnect();
        nn::err::CTR::ThrowFatalErrIfFailure(APPLET::IsRegistered(APP_ID_LIBRARY_APPLET_OF_SYSTEM_APPLET, &isRegistered));
        DisconnectAndUnlock();
        if (!isRegistered) {
            return result;
        }
    }
    SetTransitionType(TRANSITION_CANCEL_LIBRARY_APPLET);
    for (;;) {
        LockAndConnect();
        result = APPLET::CancelLibraryApplet(isExiting);
        DisconnectAndUnlock();
        if (!IsBusy(result)) {
            break;
        }
        WaitToRetry();
    }
    if (result != nn::Result()) {
        return result;
    }
    WakeupState state = WaitForStarting(0, 0, 0, 0, 0, WAIT_INFINITE);
    if (pWakeupState) {
        *pWakeupState = state;
    }
    return result;
}

// 0x00131D24 | tier C
void AssignCameraRight(bool isAssigned)
{
    if (isAssigned) {
        nn::camera::CTR::detail::ArriveApplication();
    } else {
        nn::camera::CTR::detail::LeaveApplication();
    }
}

// 0x00480DBC (name is ours)
nn::Result GetApplicationRunningMode(nn::applet::CTR::ApplicationRunningMode* pMode)
{
    LockAndConnect();
    nn::Result result = APPLET::GetApplicationRunningMode(pMode);
    DisconnectAndUnlock();
    return result;
}

// 0x0048088C (name is ours)
nn::applet::CTR::PowerButtonState GetPowerButtonState()
{
    return s_PowerButtonState;
}

} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
