#include "nn/nex/nex_JobNameResolve.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_CallContextRegister.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_Network.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
namespace {
// the name could not be resolved
const int RESULT_RESOLUTION_FAILED = static_cast<int>(0x80050014);
// the call was cancelled
const int RESULT_CANCELLED = static_cast<int>(0x80010004);
// the times between the tests of the waiting steps (ms)
const int START_WAIT_TIME = 50;
const int THREAD_WAIT_TIME = 10;

// (inline) the lock of the scheduler
inline CriticalSection& GetSchedulerLock()
{
    return Scheduler::GetInstanceInline()->m_Lock;
}
} // namespace

// a JobNameResolve has a thread (name is ours)
// 0x00AE04BC
PseudoGlobalVariable<bool> g_IsNameResolutionRunning;

// 0x0037105C (name is ours)
void nn::nex::JobNameResolve::Initialize(unsigned int callContextId)
{
    ScopedCS lock(GetSchedulerLock());
    m_CallContextId = callContextId;
    SetStepInline(&JobNameResolve::Start, "");
}

// 0x003710E4 (name is ours)
void nn::nex::JobNameResolve::Resolve(int unused)
{
    Network* pNetwork = Network::GetInstance();
    RootTransport* pTransport = pNetwork != nullptr ? pNetwork->m_pRootTransport : nullptr;
    bool isResolved = (pTransport->*m_Method)();
    ScopedCS lock(m_Lock);
    m_Result = qResult(isResolved ? qResult::SUCCESS : RESULT_RESOLUTION_FAILED);
}

// 0x0037117C | fefates:bytes [tier B]
void nn::nex::JobNameResolve::CompleteJob(const nn::nex::qResult& result)
{
    CallContext* pContext = Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId);
    if (pContext != nullptr) {
        pContext->SetStateImpl(result.m_Code >= 0 ? CallContext::STATE_SUCCESS : CallContext::STATE_FAILURE, result, true);
    }
    SetState(STATE_COMPLETED);
}

// 0x003711E0 (name is ours)
void nn::nex::JobNameResolve::Start()
{
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        CompleteJob(qResult(RESULT_CANCELLED));
        return;
    }
    if (g_IsNameResolutionRunning.Get()) {
        SetState(STATE_WAITING);
        m_WaitTime = START_WAIT_TIME;
        return;
    }
    m_pThread = new ObjectThread<JobNameResolve, int>(String(L"DNS Check Thread"));
    m_pThread->m_Priority = m_ThreadPriority;
    m_pThread->m_CoreNo = m_CoreNo;
    g_IsNameResolutionRunning.Get() = true;
    m_pThread->SetMethod(this, &JobNameResolve::Resolve, 0);
    m_pThread->Launch(0);
    SetStepInline(&JobNameResolve::WaitForResolution, "");
}

// 0x0037143C (name is ours)
void nn::nex::JobNameResolve::WaitForThread()
{
    ScopedCS lock(m_Lock);
    if (m_Result.m_Code == qResult::FAILURE) {
        SetState(STATE_WAITING);
        m_WaitTime = THREAD_WAIT_TIME;
    } else {
        CompleteJob(qResult(RESULT_CANCELLED));
    }
}

// 0x003714CC (name is ours)
void nn::nex::JobNameResolve::WaitForResolution()
{
    ScopedCS lock(m_Lock);
    if (Core::GetInstance()->m_pCallContextRegister->GetCallContext(m_CallContextId) == nullptr) {
        if (m_Result.m_Code == qResult::FAILURE) {
            SetStepInline(&JobNameResolve::WaitForThread, "");
        } else {
            CompleteJob(qResult(RESULT_CANCELLED));
        }
        return;
    }
    if (m_Result.m_Code == qResult::FAILURE) {
        SetState(STATE_WAITING);
        m_WaitTime = THREAD_WAIT_TIME;
    } else {
        CompleteJob(m_Result);
    }
}

// the thread of the name resolution (the names are ours)
// 0x0096C9E0
u32 g_NameResolutionThreadPriority = 10;
// 0x0096C9E4
s32 g_NameResolutionCoreNo = static_cast<s32>(0x80000000);

// 0x003715FC
nn::nex::JobNameResolve::JobNameResolve(unsigned int threadPriority, int coreNo, ResolveMethod method)
    : StepSequenceJob(JOB_TYPE_DEFAULT, DebugString()), m_CallContextId(0), m_Result(qResult::FAILURE), m_pThread(nullptr), m_Lock(1), m_ThreadPriority(threadPriority),
      m_CoreNo(coreNo), m_Method(method)
{
    Network::AcquireInstanceInline();
}

// 0x00371710 slot 0x00
// 0x003716E0 (deleting dtor)
nn::nex::JobNameResolve::~JobNameResolve()
{
    if (m_pThread != nullptr) {
        m_pThread->Wait(static_cast<u32>(-1));
        if (m_pThread != nullptr) {
            delete m_pThread;
        }
        g_IsNameResolutionRunning.Get() = false;
    }
    Network::GetInstance()->ReleaseRef();
}

// 0x007D44B4
template void nn::nex::ObjectThread<nn::nex::JobNameResolve, int>::Run();

// 0x007D450C
// 0x007D44E8 (deleting dtor)
template nn::nex::ObjectThread<nn::nex::JobNameResolve, int>::~ObjectThread();

} // namespace nex
} // namespace nn
