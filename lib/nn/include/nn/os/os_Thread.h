#pragma once

#include "decomp.h"
#include <new>

#include "nn/Result.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/CTR/detail/detail_Api.h"
#include "nn/os/os_WaitObject.h"
#include "nn/svc/svc_Api.h"

namespace nn {
namespace os {
class AutoStackManager;

// A kernel thread. The thread function takes one parameter of any type: the typed start
// functions (templates, inlined into the callers) describe the type with a TypeInfo, the parameter is
// copied onto the new thread's stack and destroyed there when the function returns.
class Thread : public WaitObject
{
public:
    // how to handle the parameter type T of the thread function (member names are ours)
    struct TypeInfo {
        size_t size;                                            // sizeof(T)
        void (*copy)(const void* src, void* dst);               // copy construct a T at dst
        void (*destroy)(void* param);                           // ~T
        void (*invoke)(void (*f)(uptr), const void* param);     // f(*(T*)param)
    };

    // the TypeInfo functions for T (shared by all types of one size in the binary, e.g.
    // 0x007D3678 / 0x007D36C0 / 0x007D36A8 for 4 bytes; the name is ours)
    template <typename T>
    struct TypeInfoOf {
        static void Copy(const void* src, void* dst) { new (dst) T(*static_cast<const T*>(src)); }
        static void Destroy(void* param) { static_cast<T*>(param)->~T(); }
        static void Invoke(void (*f)(uptr), const void* param)
        {
            reinterpret_cast<void (*)(T)>(f)(*static_cast<const T*>(param));
        }
    };

    // core: -2 = the default core of the process (name is ours)
    static const s32 CORE_NO_DEFAULT = -2;

    Thread() : mIsJoined(true) {}

    // starts f(param) on a new thread with the given stack, fatal on failure
    template <typename T>
    void Start(void (*f)(T), T param, uptr stackBottom, s32 priority, s32 coreNo = CORE_NO_DEFAULT)
    {
        TypeInfo typeInfo = {sizeof(T), &TypeInfoOf<T>::Copy, &TypeInfoOf<T>::Destroy, &TypeInfoOf<T>::Invoke};
        nn::Result result = TryInitializeAndStartImpl(typeInfo, reinterpret_cast<void (*)(uptr)>(f), &param,
                                                      stackBottom, priority, coreNo, false);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
    }

    // the same, but a lack of resources is returned instead of being fatal (name is ours)
    template <typename T>
    nn::Result TryStart(void (*f)(T), T param, uptr stackBottom, s32 priority, s32 coreNo = CORE_NO_DEFAULT)
    {
        const bit32 SUMMARY_OUT_OF_RESOURCE = 3;
        TypeInfo typeInfo = {sizeof(T), &TypeInfoOf<T>::Copy, &TypeInfoOf<T>::Destroy, &TypeInfoOf<T>::Invoke};
        nn::Result result = TryInitializeAndStartImpl(typeInfo, reinterpret_cast<void (*)(uptr)>(f), &param,
                                                      stackBottom, priority, coreNo, false);
        if (result.GetSummary() != SUMMARY_OUT_OF_RESOURCE && result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
        return result;
    }

    // waits for the end of the thread
    void Join()
    {
        nn::Result result = nn::svc::WaitSynchronization1(mHandle, -1);
        if (result.IsFailure()) {
            CTR::detail::HandleInternalError(result);
        }
        mIsJoined = true;
    }

    // what the destructor does, for threads that live on (in a ThreadPool)
    void Finalize()
    {
        FinalizeImpl();
        Close();
    }

    static void CallDestructorAndExit(void* stackBottom); // 0x00128390 | nintendogs:bytes [tier B]
    nn::Result TryInitializeAndStartImpl(const nn::os::Thread::TypeInfo& typeInfo, void (*f)(uptr), const void* param, uptr stackBottom, s32 priority, s32 coreNo, bool isAutoStack); // 0x001299D4 | nintendogs:callgraph [tier A]
    static void ThreadStart(uptr startParam); // 0x0012F38C | fefates:bytes [tier B]
    static void SleepImpl(nn::fnd::TimeSpan span); // 0x00130960 | nintendogs:callgraph [tier A]
    void FinalizeImpl(); // 0x001365BC | nintendogs:callgraph [tier A]
    nn::Result TryInitializeAndStartImplUsingAutoStack(const nn::os::Thread::TypeInfo& typeInfo, void (*f)(uptr), const void* param, size_t stackSize, s32 priority, s32 coreNo); // 0x0034C82C | fefates:bytes [tier B]
    ~Thread(); // 0x0034C8C8 | nintendogs:bytes [tier A]

    // allocates the stacks of TryInitializeAndStartImplUsingAutoStack (name is ours)
    static AutoStackManager* s_pAutoStackManager; // 0x00975F84

private:
    // lies on the new thread's stack, right below the copy of the parameter (names are ours)
    struct StartParam {
        void (*destroy)(void* param);
        void (*invoke)(void (*f)(uptr), const void* param);
        void (*f)(uptr);
        void* param;            // the copy on this stack
        uptr autoStack;         // stack bottom to give back to the AutoStackManager, or 0
    };

    // mHandle (HandleObject)                                   // 0x00
    bool mIsJoined;             // FinalizeImpl waited for the end of the thread    // 0x04
    bool mUsingAutoStack;       // the stack comes from s_pAutoStackManager         // 0x05

    // layout checks (inside the class because the members are private; generates no code)
    static void CheckLayout()
    {
        ASSERT_OFFSET(Thread, mIsJoined, 0x4);
        ASSERT_OFFSET(Thread, mUsingAutoStack, 0x5);
    }
};

} // namespace os
} // namespace nn
