#include "nn/nex/nex_ObjectThreadRoot.h"
#include "nn/nex/nex_GlobalVariables.h"
#include "nn/nex/nex_SpinTest.h"
#include "nn/nex/nex_SystemError.h"
#include "nn/nex/nex_ThreadVariableList.h"
#include "nn/os/detail/detail_Api.h"
#include "nn/os/os_Api.h"
#include "nn/os/os_Thread.h"

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Platform/Core/ObjectThreadRoot.cpp";
// the stacks are aligned to pages; the first word of the stack gets a marker
const u32 STACK_ALIGNMENT = 0x1000;
const u32 STACK_MARKER = 0xCC;
// the time between two checks of Launch (1 ms)
const s64 LAUNCH_POLL_NS = 1000000;

uptr AlignStack(u8* pStack)
{
    return (reinterpret_cast<uptr>(pStack) + STACK_ALIGNMENT - 1) / STACK_ALIGNMENT * STACK_ALIGNMENT;
}

// (the destructor of the thread is inline in the original; nex frees the memory)
void DeleteThread(nn::os::Thread* pThread)
{
    pThread->~Thread();
    MemoryManager::FreeInline(pThread);
}
} // namespace

// 0x0096C911 (name is ours)
bool ObjectThreadRoot::s_UsesAutoStack;
// 0x0096C968 (name is ours)
u32 ObjectThreadRoot::s_DefaultStackSize = 0x7000;
// 0x0096C96C (name is ours)
s32 ObjectThreadRoot::s_DefaultPriority = 16;
// 0x0096CA98 (name is ours)
volatile s32 ObjectThreadRoot::s_RunningCount;

// 0x00382718 | fefates:bytes [tier B]
bool nn::nex::ObjectThreadRoot::LaunchImpl()
{
    if (!s_UsesAutoStack && m_pData->m_pStack == nullptr) {
        m_pData->m_StackSize = m_StackSize;
        m_pData->m_pStack = NewArray<u8>(m_StackSize + STACK_ALIGNMENT - 1);
        *reinterpret_cast<u32*>(AlignStack(m_pData->m_pStack)) = STACK_MARKER;
    }
    if (m_pData->m_pThread == nullptr) {
        m_pData->m_pThread = new (MemoryManager::Allocate(sizeof(nn::os::Thread))) nn::os::Thread;
    }
    m_pData->m_IsJoined = false;
    s32 coreNo = m_CoreNo;
    if (coreNo == CORE_NO_DEFAULT) {
        coreNo = nn::os::Thread::CORE_NO_DEFAULT;
    }
    nn::os::Thread* pThread = m_pData->m_pThread;
    ObjectThreadRoot* pParam = this;
    nn::os::Thread::TypeInfo typeInfo = {sizeof(ObjectThreadRoot*), &nn::os::Thread::TypeInfoOf<ObjectThreadRoot*>::Copy,
                                         &nn::os::Thread::TypeInfoOf<ObjectThreadRoot*>::Destroy,
                                         &nn::os::Thread::TypeInfoOf<ObjectThreadRoot*>::Invoke};
    nn::Result result;
    if (s_UsesAutoStack) {
        result = pThread->TryInitializeAndStartImplUsingAutoStack(typeInfo, reinterpret_cast<void (*)(uptr)>(ObjectThreadProc), &pParam,
                                                                  m_StackSize, m_Priority, coreNo);
    } else {
        uptr stackBottom = m_pData->m_StackSize + AlignStack(m_pData->m_pStack);
        result = pThread->TryInitializeAndStartImpl(typeInfo, reinterpret_cast<void (*)(uptr)>(ObjectThreadProc), &pParam, stackBottom,
                                                    m_Priority, coreNo, false);
    }
    const bit32 SUMMARY_OUT_OF_RESOURCE = 3;
    if (result.GetSummary() != SUMMARY_OUT_OF_RESOURCE && result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    if (result.IsFailure()) {
        DeleteThread(m_pData->m_pThread);
        m_pData->m_pThread = nullptr;
        return false;
    }
    u32 threadId;
    result = nn::svc::GetThreadId(&threadId, m_pData->m_pThread->GetHandle());
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    m_ThreadId = threadId;
    result = nn::svc::SetThreadPriority(m_pData->m_pThread->GetHandle(), nn::os::detail::ConvertLibraryToSvcPriority(m_Priority));
    if (result.IsFailure()) {
        nn::os::CTR::detail::HandleInternalError(result);
    }
    return true;
}

// 0x003829A0 (name is ours)
void nn::nex::ObjectThreadRoot::SetUsesAutoStack(bool usesAutoStack)
{
    s_UsesAutoStack = usesAutoStack;
}

// 0x003829B0 (name from the reference symbols)
void nn::nex::ObjectThreadRoot::ObjectThreadProc(nn::nex::ObjectThreadRoot* pThread)
{
    GlobalVariables::s_pInstance->m_CurrentThread.Set(pThread);
    u32 threadId = ThreadVariableRoot::GetCurrentThreadId();
    pThread->m_IsFinished = false;
    pThread->m_ThreadId = threadId;
    nn::os::detail::AtomicAdd(&s_RunningCount, 1);
    pThread->Run();
    pThread->m_IsFinished = true;
    nn::os::detail::AtomicAdd(&s_RunningCount, -1);
}

// 0x00382AA8 | fefates:bytes [tier B]
bool nn::nex::ObjectThreadRoot::Wait(unsigned int timeout)
{
    if (m_pData->m_pThread == nullptr) {
        return true;
    }
    if (timeout != static_cast<unsigned int>(-1)) {
        SpinTest spinTest(100, timeout);
        spinTest.m_Mode = SpinTest::MODE_STOP;
        while (!m_IsFinished && spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 803, const_cast<wchar_t*>(L"!m_bFinished"))) {
        }
        if (!m_IsFinished) {
            SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 806, SystemError::ERROR_TIMEOUT, 0);
            return false;
        }
    } else {
        while (!m_IsFinished) {
            SpinTest spinTest(100, 100);
            spinTest.m_Mode = SpinTest::MODE_STOP;
            while (!m_IsFinished && spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 847, const_cast<wchar_t*>(L"!m_bFinished"))) {
            }
        }
    }
    m_pData->m_pThread->Join();
    m_pData->m_IsJoined = true;
    DeleteThread(m_pData->m_pThread);
    m_pData->m_pThread = nullptr;
    return true;
}

// 0x00382CF8 | fefates:callgraph [tier C]
bool nn::nex::ObjectThreadRoot::Launch(unsigned int stackSize)
{
    if (stackSize == 0) {
        stackSize = s_DefaultStackSize;
    }
    m_StackSize = stackSize;
    if (m_pData->m_pThread != nullptr) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 364, SystemError::ERROR_INVALID_STATE, 0);
        return false;
    }
    m_IsRunning = false;
    m_IsLaunched = true;
    LaunchImpl();
    while (!m_IsRunning) {
        OsSleepThread(LAUNCH_POLL_NS);
    }
    return true;
}

// 0x00382DC4 | fefates:callseq [tier C]
// 0x00382D90 (deleting dtor)
nn::nex::ObjectThreadRoot::~ObjectThreadRoot()
{
    if (m_IsLaunched) {
        ThreadVariableList::s_pInstance->ClearThread(m_ThreadId);
    }
    ThreadData* pData = m_pData;
    if (pData != nullptr) {
        if (pData->m_pThread != nullptr) {
            if (!pData->m_IsJoined) {
                u32 threadId;
                nn::Result result = nn::svc::GetThreadId(&threadId, pData->m_pThread->GetHandle());
                if (result.IsFailure()) {
                    nn::os::CTR::detail::HandleInternalError(result);
                }
                pData->m_pThread->Join();
                SystemError::ClearThread(threadId);
            }
            DeleteThread(pData->m_pThread);
            pData->m_pThread = nullptr;
        }
        if (pData->m_pStack != nullptr) {
            DeleteArray(pData->m_pStack);
            pData->m_pStack = nullptr;
        }
        MemoryManager::FreeInline(pData);
    }
    m_pData = nullptr;
}

} // namespace nex
} // namespace nn
