#include "nn/os/os_Thread.h"
#include "nn/os/os_AutoStackManager.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {

// 0x00975F84
AutoStackManager* Thread::s_pAutoStackManager;

// 0x00128390 | nintendogs:bytes [tier B]
// last act of a thread with an automatic stack: the thread frees its own stack
void nn::os::Thread::CallDestructorAndExit(void* stackBottom)
{
    s_pAutoStackManager->Destruct(stackBottom, false);
    nn::svc::ExitThread();
}

// 0x001299D4 | nintendogs:callgraph [tier A]
nn::Result nn::os::Thread::TryInitializeAndStartImpl(const nn::os::Thread::TypeInfo& typeInfo, void (*f)(uptr), const void* param, uptr stackBottom, s32 priority, s32 coreNo, bool isAutoStack)
{
    uptr autoStack = isAutoStack ? stackBottom : 0;

    // the stack of the new thread: [copy of param][StartParam][stack ...]
    void* paramCopy = reinterpret_cast<void*>((stackBottom - typeInfo.size) & ~7u);
    typeInfo.copy(param, paramCopy);
    StartParam* start = reinterpret_cast<StartParam*>((reinterpret_cast<uptr>(paramCopy) - sizeof(StartParam)) & ~7u);
    start->destroy = typeInfo.destroy;
    start->invoke = typeInfo.invoke;
    start->f = f;
    start->param = paramCopy;
    start->autoStack = autoStack;

    nn::Handle handle;
    nn::Result result = nn::svc::CreateThread(&handle, ThreadStart, reinterpret_cast<uptr>(start),
                                              reinterpret_cast<uptr>(start),
                                              detail::ConvertLibraryToSvcPriority(priority), coreNo);
    if (result.IsFailure()) {
        return result;
    }
    mHandle = handle;
    mIsJoined = false;
    mUsingAutoStack = false;
    return nn::Result();
}

// 0x0012F38C | fefates:bytes [tier B]
// entry point of every thread
void nn::os::Thread::ThreadStart(uptr startParam)
{
    StartParam* start = reinterpret_cast<StartParam*>(startParam);
    detail::InitializeThreadEnvrionment();
    start->invoke(start->f, start->param);
    start->destroy(start->param);
    detail::InvokeAllTlsDestructors();
    if (start->autoStack) {
        CallDestructorAndExit(reinterpret_cast<void*>(start->autoStack));
    }
    nn::svc::ExitThread();
}

// 0x00130960 | nintendogs:callgraph [tier A]
void nn::os::Thread::SleepImpl(nn::fnd::TimeSpan span)
{
    svcSleepThread(span.GetNanoSeconds());
}

// 0x001365BC | nintendogs:callgraph [tier A]
// waits for the end of the thread (once)
void nn::os::Thread::FinalizeImpl()
{
    if (!mIsJoined) {
        nn::Result result = nn::svc::WaitSynchronization1(mHandle, -1);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
        mIsJoined = true;
    }
}

// 0x0034C82C | fefates:bytes [tier B]
nn::Result nn::os::Thread::TryInitializeAndStartImplUsingAutoStack(const nn::os::Thread::TypeInfo& typeInfo, void (*f)(uptr), const void* param, size_t stackSize, s32 priority, s32 coreNo)
{
    void* stack = s_pAutoStackManager->Construct(stackSize);
    nn::Result result = TryInitializeAndStartImpl(typeInfo, f, param, reinterpret_cast<uptr>(stack), priority, coreNo, true);
    if (result.IsSuccess()) {
        mUsingAutoStack = true;
        return nn::Result();
    }
    s_pAutoStackManager->Destruct(stack, true);
    return result;
}

// 0x0034C8C8 | nintendogs:bytes [tier A]
nn::os::Thread::~Thread()
{
    FinalizeImpl();
    // ~HandleObject closes the handle
}

} // namespace os
} // namespace nn
