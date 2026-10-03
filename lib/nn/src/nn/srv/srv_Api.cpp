#include "nn/srv/srv_Api.h"
#include "nn/dbg/dbg_Api.h"
#include "nn/fnd/fnd_IntrusiveLinkedList.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_IpcSession.h"
#include "nn/os/os_Thread.h"
#include "nn/srv/detail/srv_Service.h"
#include "nn/srv/srv_NotificationHandler.h"
#include "nn/svc/svc_Api.h"

#include <new>
#include <string.h>

namespace nn {
namespace srv {
namespace {

// results (module srv, 25); the names are ours
const bit32 RESULT_ALREADY_INITIALIZED = 0x082067F9;    // info, nothing happened, 1017
const bit32 RESULT_STILL_INITIALIZED = 0x082067F0;      // info, nothing happened, 1008: other users left
const bit32 RESULT_NOT_INITIALIZED = 0xD8A067F8;        // permanent, invalid state, 1016
const bit32 RESULT_NAME_TOO_LONG = 0xD9006405;          // permanent, wrong argument, 5

const char PORT_NAME[] = "srv:";
const s32 MAX_SERVICE_NAME_LENGTH = 8;

// ConnectToPort fails with this while the port does not exist yet: permanent, not found, 1018
const s32 LEVEL_PERMANENT = -5;
const bit32 SUMMARY_NOT_FOUND = 4;
const bit32 DESCRIPTION_PORT_NOT_FOUND = 1018;

// the dispatcher thread: library priority 0 of the first system range (kernel priority 24, see
// nn::os::detail::ConvertLibraryToSvcPriority)
const s32 DISPATCHER_PRIORITY = 0x5109D500;
const size_t DISPATCHER_STACK_SIZE = 512;

// what Initialize makes on its first call (names are ours)
struct Data
{
    Data() : lock(nn::os::CriticalSection::InitializeTag()), handlersLock(nn::os::CriticalSection::InitializeTag()) {}

    nn::os::CriticalSection lock;               // 0x00 Initialize / Finalize
    nn::Handle notificationSemaphore;           // 0x0C from EnableNotification
    nn::os::Thread dispatcherThread;            // 0x10
    nn::fnd::IntrusiveLinkedList<NotificationHandler, void> handlers;  // 0x18
    nn::os::CriticalSection handlersLock;       // 0x1C
    DECOMP_ALIGN(8) u8 dispatcherStack[DISPATCHER_STACK_SIZE];  // 0x28
};
ASSERT_SIZE(Data, 0x228);

// the result of ConnectToPort while srv: is not there yet (inline)
bool IsPortNotFound(nn::Result result)
{
    return (static_cast<s32>(result.GetPrintableBits()) >> 27) == LEVEL_PERMANENT &&
           result.GetSummary() == SUMMARY_NOT_FOUND && result.GetDescription() == DESCRIPTION_PORT_NOT_FOUND;
}

} // namespace

// the globals of this file (ARMCC addresses the first ones from 0x0097F084; names are ours)
// 0x0097F084
bool s_IsDataConstructed;
// 0x0097F088
s32 s_InitializeCount;
// the memory of the Data (8 byte aligned for the stack)
// 0x00AEE830
u64 s_DataStorage[sizeof(Data) / sizeof(u64)];

namespace {

Data& GetData()
{
    return *reinterpret_cast<Data*>(s_DataStorage);
}

// 0x0011F6FC (name is ours)
// the thread function: calls its parameter
void InvokeFunction(void (*f)())
{
    f();
}

// 0x0011FFC0 | fefates:bytes [tier B]
// waits for notifications and passes each to the handler registered for its id
void DispatcherThread()
{
    Data& data = GetData();
    for (;;) {
        nn::Result result = nn::svc::WaitSynchronization1(data.notificationSemaphore, -1);
        if (result.IsFailure()) {
            nn::os::CTR::detail::HandleInternalError(result);
        }
        u32 notificationId;
        result = detail::Service::ReceiveNotification(&notificationId);
        if (result.IsSuccess()) {
            nn::os::CriticalSection::ScopedLock lock(data.handlersLock);
            NotificationHandler* handler = data.handlers.GetFront();
            while (handler != 0 && handler->GetNotificationId() != notificationId) {
                handler = data.handlers.GetNext(handler);
            }
            if (handler != 0) {
                handler->HandleNotification();
            }
            result = nn::Result();
        }
        if (result.IsFailure()) {
            nndbgPanic();
        }
    }
}

} // namespace

// 0x0011E34C | fefates:bytes [tier B]
nn::Result StartNotification()
{
    Data& data = GetData();
    nn::Handle semaphore;
    nn::Result result = detail::Service::EnableNotification(&semaphore);
    if (result.IsSuccess()) {
        data.notificationSemaphore = semaphore;
    }
    if (result.IsFailure()) {
        return result;
    }
    data.dispatcherThread.Start(&InvokeFunction, &DispatcherThread,
                                reinterpret_cast<uptr>(data.dispatcherStack + DISPATCHER_STACK_SIZE),
                                DISPATCHER_PRIORITY);
    return result;
}

// 0x0011E410 | fefates:bytes [tier B]
nn::Result RegisterNotificationHandler(nn::srv::NotificationHandler* handler, u32 notificationId)
{
    Data& data = GetData();
    nn::os::CriticalSection::ScopedLock lock(data.handlersLock);
    handler->SetNotificationId(notificationId);
    data.handlers.PushBack(handler);
    return nn::Result();
}

// 0x0012A800 | nintendogs:callgraph [tier A]
nn::Result Initialize()
{
    if (!s_IsDataConstructed) {
        new (s_DataStorage) Data();
        s_IsDataConstructed = true;
    }
    Data& data = GetData();
    nn::os::CriticalSection::ScopedLock lock(data.lock);
    if (s_InitializeCount > 0) {
        s_InitializeCount++;
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    nn::Result result;
    for (;;) {
        result = nn::svc::ConnectToPort(&detail::Service::s_Session, PORT_NAME);
        if (!IsPortNotFound(result)) {
            break;
        }
        nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMicroSeconds(500));
    }
    if (result.IsSuccess()) {
        result = detail::Service::RegisterClient();
        s_InitializeCount++;
    }
    return result;
}

// 0x0012A924 | nintendogs:callgraph [tier A]
nn::Result GetServiceHandle(nn::Handle* session, const char* name, s32 nameLength, u32 flags)
{
    if (s_InitializeCount <= 0) {
        return nn::Result(RESULT_NOT_INITIALIZED);
    }
    if (nameLength > MAX_SERVICE_NAME_LENGTH) {
        return nn::Result(RESULT_NAME_TOO_LONG);
    }
    return detail::Service::GetServiceHandle(session, name, nameLength, flags);
}

// 0x004670E0 | nintendogs:callgraph [tier A]
nn::Result GetServiceHandle(nn::os::ipc::Session* session, const char* name)
{
    nn::Handle handle;
    nn::Result result = GetServiceHandle(&handle, name, strlen(name), 0);
    if (result.IsSuccess()) {
        session->Attach(handle);
    }
    return result;
}

// 0x0046712C | fefates:bytes [tier B]
nn::Result GetServiceHandle(nn::os::ipc::Session* session, const char* name, s32 nameLength)
{
    nn::Handle handle;
    nn::Result result = GetServiceHandle(&handle, name, nameLength, 0);
    if (result.IsSuccess()) {
        session->Attach(handle);
    }
    return result;
}

// 0x00467168 | mk7dlp:callseq-callee [tier A]
nn::Result Finalize()
{
    Data& data = GetData();
    nn::os::CriticalSection::ScopedLock lock(data.lock);
    if (s_InitializeCount > 1) {
        s_InitializeCount--;
        return nn::Result(RESULT_STILL_INITIALIZED);
    }
    nn::Result result = nn::svc::CloseHandle(detail::Service::s_Session);
    if (result.IsSuccess()) {
        s_InitializeCount--;
    }
    if (result.IsFailure()) {
        nndbgPanic();
    }
    if (result.IsSuccess()) {
        detail::Service::s_Session = nn::Handle();
    }
    return result;
}

} // namespace srv
} // namespace nn
