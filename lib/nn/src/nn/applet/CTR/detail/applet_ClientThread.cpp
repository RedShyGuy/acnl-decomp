// applet_ClientThread.cpp (file name from the static initializer 0x0079CB78: s_Events, s_Thread,
// s_ControlEvent): the thread that waits for the notifications and parameters of APT and hands
// them to the program through the control event
#include "nn/applet/CTR/detail/applet_APPLET.h"
#include "nn/applet/CTR/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/os/os_Event.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Thread.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace applet {
namespace CTR {
namespace detail {
namespace {
// the events the thread waits for
const s32 EVENT_NOTIFICATION = 0;
const s32 EVENT_PARAMETER = 1;
const s32 EVENT_CONTROL = 2;
const s32 EVENT_COUNT = 3;

// the notifications of InquireNotification (3dbrew "NS and APT Services", Notification)
const u8 NOTIFICATION_HOME_BUTTON_1 = 1;
const u8 NOTIFICATION_HOME_BUTTON_2 = 2;
const u8 NOTIFICATION_SLEEP_QUERY = 3;
const u8 NOTIFICATION_SLEEP_CANCELED_BY_OPEN = 4;
const u8 NOTIFICATION_SLEEP_ACCEPTED = 5;
const u8 NOTIFICATION_SLEEP_AWAKE = 6;
const u8 NOTIFICATION_SHUTDOWN = 7;
const u8 NOTIFICATION_POWER_BUTTON_CLICK = 8;
const u8 NOTIFICATION_POWER_BUTTON_CLEAR = 9;
const u8 NOTIFICATION_TRY_SLEEP = 10;
const u8 NOTIFICATION_ORDER_TO_CLOSE = 11;
const u8 NOTIFICATION_COUNT = 12;

// the commands the thread makes from the HOME button and the control event (3dbrew Command)
const u32 COMMAND_HOME_BUTTON_SINGLE = 6;
const u32 COMMAND_HOME_BUTTON_DOUBLE = 7;
const u32 COMMAND_CONTROL = 12;

// what TRY_SLEEP asks the system for (meaning of the value unknown)
const u64 TRY_SLEEP_TIME = 0x4000000000ULL;

const size_t STACK_SIZE = 0x1000;
const s64 RETRY_WAIT_MSEC = 10;

// 0x00AE9454
nn::os::Event s_Events[EVENT_COUNT];
// set by FinalizeClientThread
// 0x0097E7EC
bool s_IsStopRequested;
// 0x0097E7F0
bool (*s_ReceiveCallback)(unsigned);
// 0x0097E7F4
unsigned s_ReceiveCallbackArgument;
// 0x0097E7F8
nn::os::Thread s_Thread;
// the program waits for it in Receive
// 0x0097E800
nn::os::LightEvent s_ControlEvent;
// 0x00946000
u64 s_ThreadStack[STACK_SIZE / sizeof(u64)];
} // namespace

// the parameters that ReceiveCallbackForCommands glances at
// 0x00947000
u8 s_ParameterBuffer[0x1000];

// 0x00120300 | nintendogs:callseq [tier A]
void InitializeClientThread(int priority, nn::Handle parameterEvent, nn::Handle notificationEvent)
{
    s_Events[EVENT_PARAMETER].Initialize(nn::os::RESET_TYPE_ONESHOT);
    s_Events[EVENT_PARAMETER].AttachHandle(parameterEvent);
    s_Events[EVENT_NOTIFICATION].Initialize(nn::os::RESET_TYPE_ONESHOT);
    s_Events[EVENT_NOTIFICATION].AttachHandle(notificationEvent);
    s_Events[EVENT_CONTROL].Initialize(nn::os::RESET_TYPE_ONESHOT);
    s_ControlEvent.Initialize(true);
    s_IsStopRequested = false;
    s_Thread.Start(ThreadFunc, 0, reinterpret_cast<uptr>(s_ThreadStack + sizeof(s_ThreadStack) / sizeof(u64)), priority);
}

// 0x00124D84 (name from symbols.json, which puts it at 0x00124D70)
void ThreadFunc(int)
{
    nn::Handle handles[EVENT_COUNT];
    handles[0] = s_Events[0].GetHandle();
    handles[1] = s_Events[1].GetHandle();
    handles[2] = s_Events[2].GetHandle();
    while (!s_IsStopRequested) {
        s32 index;
        nn::err::CTR::ThrowFatalErrIfFailure(nn::svc::WaitSynchronizationN(&index, handles, EVENT_COUNT, false, -1));
        if (s_IsStopRequested) {
            break;
        }
        if (s_ControlEvent.TryWait()) {
            // the program has not taken the last event yet: later again
            nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(RETRY_WAIT_MSEC));
            s_Events[index].Signal();
            continue;
        }
        s_Events[index].ClearSignal();

        switch (index) {
        case EVENT_CONTROL:
            SetMessageCommand(COMMAND_CONTROL);
            s_ControlEvent.Signal();
            break;
        case EVENT_PARAMETER:
            if (s_ReceiveCallback == 0 || s_ReceiveCallback(s_ReceiveCallbackArgument)) {
                s_ControlEvent.Signal();
            }
            break;
        case EVENT_NOTIFICATION: {
            u8 notification;
            LockAndConnect();
            nn::Result result = APPLET::InquireNotification(GetId(), &notification);
            DisconnectAndUnlock();
            if (result.IsFailure()) {
                break;
            }
            switch (notification) {
            case NOTIFICATION_HOME_BUTTON_1:
            case NOTIFICATION_HOME_BUTTON_2:
                if (GetAbsoluteHomeButtonState() == HOME_BUTTON_NONE) {
                    SetAbsoluteHomeButtonState(notification == NOTIFICATION_HOME_BUTTON_1 ? HOME_BUTTON_SINGLE : HOME_BUTTON_DOUBLE);
                }
                if (s_ReceiveCallback == 0 || s_ReceiveCallback(s_ReceiveCallbackArgument)) {
                    SetMessageCommand(notification == NOTIFICATION_HOME_BUTTON_1 ? COMMAND_HOME_BUTTON_SINGLE
                                                                                 : COMMAND_HOME_BUTTON_DOUBLE);
                    s_ControlEvent.Signal();
                }
                break;
            case NOTIFICATION_SLEEP_QUERY:
            case NOTIFICATION_SLEEP_CANCELED_BY_OPEN:
            case NOTIFICATION_SLEEP_ACCEPTED:
            case NOTIFICATION_SLEEP_AWAKE:
                {
                    u8 state;
                    switch (notification) {
                    case NOTIFICATION_SLEEP_QUERY:
                        state = 1;
                        break;
                    case NOTIFICATION_SLEEP_CANCELED_BY_OPEN:
                        state = 4;
                        break;
                    case NOTIFICATION_SLEEP_ACCEPTED:
                        state = 2;
                        break;
                    default:
                        state = 3;
                        break;
                    }
                    SetSleepSysState(state);
                }
                if (s_ReceiveCallback) {
                    s_ReceiveCallback(s_ReceiveCallbackArgument);
                }
                break;
            case NOTIFICATION_SHUTDOWN:
                SetShutdownCallbackFlag();
                SetShutdownState(1);
                SetOrderToCloseState(ORDER_TO_CLOSE_ORDERED);
                if (s_ReceiveCallback) {
                    s_ReceiveCallback(s_ReceiveCallbackArgument);
                }
                break;
            case NOTIFICATION_POWER_BUTTON_CLICK:
                SetPowerButtonCallbackFlag();
                SetPowerButtonState(POWER_BUTTON_CLICKED);
                if (s_ReceiveCallback) {
                    s_ReceiveCallback(s_ReceiveCallbackArgument);
                }
                break;
            case NOTIFICATION_POWER_BUTTON_CLEAR:
                SetPowerButtonState(POWER_BUTTON_NONE);
                break;
            case NOTIFICATION_TRY_SLEEP:
                LockAndConnect();
                nn::err::CTR::ThrowFatalErrIfFailure(APPLET::SleepSystem(TRY_SLEEP_TIME));
                DisconnectAndUnlock();
                break;
            case NOTIFICATION_ORDER_TO_CLOSE:
                SetOrderToCloseState(ORDER_TO_CLOSE_ORDERED);
                break;
            default:
                nndbgPanic();
                break;
            }
            break;
        }
        }
    }
}

// 0x001251B0 | nintendogs:callgraph [tier A]
void SetReceiveCallback(bool (*callback)(unsigned), unsigned argument)
{
    s_ReceiveCallback = callback;
    s_ReceiveCallbackArgument = argument;
}

// 0x001316AC | tier C
void ClearControlEvent()
{
    s_ControlEvent.ClearSignal();
}

// 0x001316B8 | tier C
void WaitForControlEvent()
{
    s_ControlEvent.Wait();
}

// 0x001316C4 | fefates:bytes [tier B]
void FinalizeClientThread()
{
    s_IsStopRequested = true;
    s_Events[EVENT_NOTIFICATION].Signal();
    s_Thread.Join();
    s_Thread.Finalize();
    for (s32 i = 0; i < EVENT_COUNT; i++) {
        s_Events[i].Close();
    }
}

// 0x00131764 | tier C
bool TryWaitForControlEvent()
{
    return s_ControlEvent.TryWait();
}

} // namespace detail
} // namespace CTR
} // namespace applet
} // namespace nn
