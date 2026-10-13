#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_InstanceTable.h"
#include "nn/nex/nex_PseudoSingleton.h"
#include "nn/nex/nex_Scheduler_SchedulerWorkerThread.h"
#include "nn/nex/nex_SpinTest.h"
#include "nn/nex/nex_SystemError.h"
#include "nn/nex/nex_ThreadVariableRoot.h"

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/Scheduler.cpp";
// the result of PreventRegularBlockCall on the scheduler thread or after the timeout
const int RESULT_TIMEOUT = static_cast<int>(0x8001000B);
// the time between two tests of the blocking calls (ms)
const u32 SPIN_SLEEP_TIME = 10;
} // namespace

// 0x0096C923 (name is ours)
bool Scheduler::s_DispatchesAllContexts = true;

// 0x003D7938
void nn::nex::Scheduler::ExecuteJob(nn::nex::Job* pJob, const nn::nex::Time& deadline)
{
    if (pJob->m_State == Job::STATE_READY) {
        pJob->SetState(Job::STATE_RUNNING);
        if (m_IsTracing) {
            pJob->AddActivity(nullptr);
        }
        pJob->m_DispatchDeadline = deadline;
        pJob->DecoratedExecute();
        if (pJob->m_State == Job::STATE_RUNNING) {
            pJob->SetDefaultPostExecutionState();
        }
        pJob->m_ExecutionCount++;
    }
    if (pJob->m_State != Job::STATE_COMPLETED) {
        Queue(pJob, false);
        return;
    }
    pJob->ReleaseRef();
}

// 0x003D7AC4
void nn::nex::Scheduler::ExecuteReadyJobs(const nn::nex::Time& deadline)
{
    bool isMoreReady = true;
    QueueNewlyReadyJobs();
    while (true) {
        {
            ScopedCS lock(m_QueueLock);
            if (m_ReadyJobs.size() != 0) {
                m_pCurrentJob = m_ReadyJobs.m_pFirst;
                m_IsJobRunning = true;
                m_pCurrentJob->AcquireRef();
                m_ReadyJobs.erase(m_ReadyJobs.begin());
            } else {
                m_pCurrentJob = nullptr;
                m_IsJobRunning = false;
            }
        }
        if (m_pCurrentJob != nullptr) {
            if (m_pCurrentJob->m_UsesGlobalLock) {
                ScopedCS globalLock(InstanceTable::s_Lock);
                ScopedCS lock(m_Lock);
                Job* pJob = m_pCurrentJob;
                if (m_IsJobRunning) {
                    ExecuteJob(pJob, deadline);
                }
                pJob->ReleaseRef();
                m_pCurrentJob = nullptr;
                m_IsJobRunning = false;
            } else {
                ScopedCS lock(m_Lock);
                Job* pJob = m_pCurrentJob;
                if (m_IsJobRunning) {
                    ExecuteJob(pJob, deadline);
                }
                pJob->ReleaseRef();
                m_pCurrentJob = nullptr;
                m_IsJobRunning = false;
            }
        } else if (!QueueNewlyReadyJobs()) {
            isMoreReady = false;
        }
        if (deadline.m_Value != 0 && Time::ConvertDeadlineToTimeout(deadline) == 0) {
            break;
        }
        if (!isMoreReady) {
            break;
        }
    }
}

// 0x003D7D10
void nn::nex::Scheduler::ExecuteRegularJobs(const nn::nex::Time& deadline)
{
    ScopedCS lock(m_Lock);
    ScopedCS regularLock(m_RegularLock);
    for (Job* pJob = m_RegularJobs.m_pFirst; pJob != m_RegularJobs.m_pEnd; pJob = pJob->m_pChainNext) {
        pJob->AcquireRef();
        pJob->SetState(Job::STATE_READY);
        ExecuteJob(pJob, deadline);
        pJob->SetState(Job::STATE_INITIAL);
    }
}

// 0x003D7EB0
u32 nn::nex::Scheduler::Dispatch(unsigned int timeout, unsigned int flags)
{
    s32 time = timeout == 0 ? -1 : static_cast<s32>(timeout);
    ScopedCall call(m_CallPolicy);
    m_LastDispatchTime = SystemClock::GetTime();
    Time deadline = time == -1 ? Time(0) : m_LastDispatchTime + time;
    ExecuteRegularJobs(deadline);
    if ((flags & DISPATCH_REGULAR_ONLY) == 0) {
        while (true) {
            ExecuteReadyJobs(Time(deadline));
            if ((flags & DISPATCH_WAIT) == 0) {
                break;
            }
            u32 waitTime;
            if (deadline.m_Value == 0) {
                waitTime = static_cast<u32>(-1);
            } else {
                waitTime = Time::ConvertDeadlineToTimeout(deadline);
                if (waitTime == 0) {
                    break;
                }
            }
            {
                ScopedCS lock(m_QueueLock);
                if (m_WaitingJobs.size() != 0) {
                    u32 nextTime = GetTimeout(Time(m_WaitingJobs.begin()->first));
                    if (waitTime > nextTime) {
                        waitTime = nextTime;
                    }
                }
            }
            Event* pEvent;
            m_EventHandler.WaitForBoolEvent(waitTime, &pEvent, true);
            if (deadline.m_Value != 0 && Time::ConvertDeadlineToTimeout(deadline) == 0) {
                break;
            }
        }
    }
    u32 count;
    {
        ScopedCS lock(m_QueueLock);
        count = m_ReadyJobs.size();
    }
    if (GetTimeout(Time(deadline)) != 0) {
        ExecuteRegularJobs(deadline);
    }
    return count;
}

// 0x003D80B0 | fefates:bytes-fuzzy [tier B]
bool nn::nex::Scheduler::PreventBlockCall(bool isBlocking, unsigned int timeout, bool errorValue, bool (*pCallback)(nn::nex::RootObject*, unsigned int), nn::nex::RootObject* pObject, bool* pResult)
{
    if (!isBlocking) {
        return false;
    }
    if (GetInstance()->m_pWorkerThread->m_Threads.size() != 0
        && ThreadVariableRoot::GetCurrentThreadId() != GetInstance()->m_CallPolicy.m_ThreadId) {
        return false;
    }
    if (ThreadVariableRoot::GetCurrentThreadId() == GetInstance()->m_CallPolicy.m_ThreadId) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 105, SystemError::ERROR_BLOCKING_CALL, 0);
        *pResult = errorValue;
        return true;
    }
    SpinTest spinTest(SPIN_SLEEP_TIME, timeout);
    spinTest.m_Mode = SpinTest::MODE_STOP;
    bool vResult;
    while ((vResult = pCallback(pObject, 0)) == errorValue) {
        GlobalSingleThreadDispatch(spinTest.GetRemainingTime());
        if (!spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 114, const_cast<wchar_t*>(L"(vResult = cbCallback(object, 0)) == bErrorValue"))) {
            break;
        }
    }
    if (vResult == errorValue) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 121, SystemError::ERROR_TIMEOUT, 0);
    }
    *pResult = vResult;
    return true;
}

// 0x003D8280 | fefates:callseq [tier C]
bool nn::nex::Scheduler::QueueNewlyReadyJobs()
{
    bool isQueued = false;
    Time now = SystemClock::GetTime();
    ScopedCS lock(m_Lock);
    ScopedCS queueLock(m_QueueLock);
    std::multimap<Time, Job*>::iterator it = m_WaitingJobs.begin();
    while (it != m_WaitingJobs.end()) {
        Job* pJob = it->second;
        if (!(m_IsTerminating && pJob->SkipWaitDelayAtTermination()) && pJob->m_State != Job::STATE_COMPLETED
            && now.m_Value < it->first.m_Value) {
            break;
        }
        std::multimap<Time, Job*>::iterator next = it;
        ++next;
        m_WaitingJobs.erase(it);
        if (pJob->m_State == Job::STATE_WAITING) {
            pJob->SetState(Job::STATE_READY);
            Queue(pJob, false);
            isQueued = true;
        } else {
            pJob->ReleaseRef();
        }
        it = next;
    }
    JobChain::iterator jobIt = m_SuspendedJobs.begin();
    while (jobIt != m_SuspendedJobs.end()) {
        Job* pJob = *jobIt;
        if (pJob->m_State == Job::STATE_SUSPENDED) {
            pJob->TestSuspendedJobState();
        }
        if (pJob->m_State == Job::STATE_SUSPENDED) {
            jobIt = JobChain::iterator(pJob->m_pChainNext);
        } else if (pJob->m_State == Job::STATE_READY) {
            jobIt = m_SuspendedJobs.erase(jobIt);
            Queue(pJob, false);
            isQueued = true;
        } else if (pJob->m_State == Job::STATE_COMPLETED) {
            jobIt = m_SuspendedJobs.erase(jobIt);
            pJob->ReleaseRef();
        }
    }
    return isQueued;
}

// 0x003D8530
void nn::nex::Scheduler::DispatchCurrentContext(unsigned int timeout)
{
    Scheduler* pScheduler;
    {
        ScopedCS lock(InstanceTable::s_Lock);
        if (!InstanceTable::IsValidContext(PseudoSingleton::GetCurrentContext())) {
            return;
        }
        pScheduler = GetInstance();
        if (pScheduler == nullptr || pScheduler->m_pWorkerThread->m_Threads.size() != 0) {
            return;
        }
        Core::AcquireInstance();
    }
    pScheduler->Dispatch(timeout, 0);
    Core::ReleaseInstance();
}

// 0x003D87C0 | fefates:bytes-fuzzy [tier B]
bool nn::nex::Scheduler::PreventRegularBlockCall(bool isBlocking, unsigned int timeout, bool (*pCallback)(nn::nex::RootObject*, unsigned int), nn::nex::RootObject* pObject, nn::nex::qResult* pResult)
{
    if (!isBlocking) {
        return false;
    }
    if (GetInstance()->m_pWorkerThread->m_Threads.size() != 0
        && ThreadVariableRoot::GetCurrentThreadId() != GetInstance()->m_CallPolicy.m_ThreadId) {
        return false;
    }
    if (ThreadVariableRoot::GetCurrentThreadId() == GetInstance()->m_CallPolicy.m_ThreadId) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 147, SystemError::ERROR_BLOCKING_CALL, 0);
        *pResult = qResult(RESULT_TIMEOUT);
        return true;
    }
    SpinTest spinTest(SPIN_SLEEP_TIME, timeout);
    spinTest.m_Mode = SpinTest::MODE_STOP;
    qResult vResult(RESULT_TIMEOUT);
    while (vResult.m_Code < 0) {
        vResult = pCallback(pObject, 0);
        if (!spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 160, const_cast<wchar_t*>(L"PreventRegularBlockCall"))) {
            break;
        }
        GlobalSingleThreadDispatch(spinTest.GetRemainingTime());
    }
    if (vResult.m_Code < 0) {
        SystemError::SignalError(const_cast<wchar_t*>(FILE_NAME), 169, SystemError::ERROR_TIMEOUT, 0);
        *pResult = qResult(RESULT_TIMEOUT);
    } else {
        *pResult = vResult;
    }
    return true;
}

// 0x003D89FC | fefates:callseq [tier C]
void nn::nex::Scheduler::WaitForAllJobsToComplete(unsigned int timeout, bool isCancelling)
{
    if (isCancelling) {
        ScopedCS lock(m_QueueLock);
        for (std::multimap<Time, Job*>::iterator it = m_WaitingJobs.begin(); it != m_WaitingJobs.end(); ++it) {
            it->second->SetState(Job::STATE_READY);
            Queue(it->second, false);
        }
        m_WaitingJobs.erase(m_WaitingJobs.begin(), m_WaitingJobs.end());
    }
    SpinTest spinTest(SPIN_SLEEP_TIME, timeout);
    spinTest.m_Mode = SpinTest::MODE_STOP;
    while (GetTotalNumberOfJobs() != 0) {
        GlobalSingleThreadDispatch(spinTest.GetRemainingTime());
        if (!spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 632, const_cast<wchar_t*>(L"GetTotalNumberOfJobs() != 0"))) {
            break;
        }
    }
    if (GetTotalNumberOfJobs()) {
        // (the trace "Could not complete %u jobs on finalizing scheduler. Timeout is %u msec." is
        // not in this build)
    }
}

// 0x003D8CC4 | fefates:callseq [tier C]
void nn::nex::Scheduler::GlobalSingleThreadDispatch(unsigned int timeout)
{
    if (!s_DispatchesAllContexts) {
        DispatchCurrentContext(timeout);
        return;
    }
    u32 savedContext = PseudoSingleton::GetCurrentContext();
    u32 lastContext = InstanceTable::GetLastContext();
    for (u32 context = 0; context <= lastContext; context++) {
        if (InstanceTable::IsValidContext(context)) {
            PseudoSingleton::SetContext(context);
            DispatchCurrentContext(timeout);
        }
    }
    PseudoSingleton::SetContext(savedContext);
}

// 0x003D8DEC
void nn::nex::Scheduler::RegisterRegularJob(nn::nex::Job* pJob, bool isFirst)
{
    ScopedCS lock(m_RegularLock);
    if (isFirst) {
        m_RegularJobs.push_front(pJob);
    } else {
        m_RegularJobs.push_back(pJob);
    }
}

// 0x003D8EB0
void nn::nex::Scheduler::UnregisterRegularJob(nn::nex::Job* pJob)
{
    ScopedCS lock(m_Lock);
    ScopedCS regularLock(m_RegularLock);
    JobChain::iterator it = m_RegularJobs.begin();
    while (it != m_RegularJobs.end() && *it != pJob) {
        it = JobChain::iterator((*it)->m_pChainNext);
    }
    m_RegularJobs.erase(it);
    pJob->ReleaseRef();
}

// 0x003D8F84 | fefates:callseq [tier C]
void nn::nex::Scheduler::GlobalSingleThreadSpecialDispatch()
{
    if (!s_DispatchesAllContexts) {
        Scheduler* pScheduler = GetInstance();
        if (pScheduler != nullptr && pScheduler->m_pWorkerThread->m_Threads.size() == 0) {
            pScheduler->ExecuteRegularJobs(Time(0));
        }
        return;
    }
    u32 savedContext = PseudoSingleton::GetCurrentContext();
    u32 lastContext = InstanceTable::GetLastContext();
    for (u32 context = 0; context <= lastContext; context++) {
        if (InstanceTable::IsValidContext(context)) {
            PseudoSingleton::SetContext(context);
            Scheduler* pScheduler = GetInstance();
            if (pScheduler != nullptr && pScheduler->m_pWorkerThread->m_Threads.size() == 0) {
                pScheduler->ExecuteRegularJobs(Time(0));
            }
        }
    }
    PseudoSingleton::SetContext(savedContext);
}

// 0x003D9184
void nn::nex::Scheduler::Queue(nn::nex::Job* pJob, bool isFirst)
{
    if (Core::s_IsThreadSafe) {
        ScopedCS lock(m_QueueLock);
        QueueImpl(pJob, isFirst);
        return;
    }
    QueueImpl(pJob, isFirst);
}

// 0x003D91EC | fefates:callgraph [tier C]
bool nn::nex::Scheduler::Cancel(nn::nex::Job* pJob)
{
    bool isFound = false;
    ScopedCS lock(m_Lock);
    ScopedCS queueLock(m_QueueLock);
    std::multimap<Time, Job*>::iterator it = m_WaitingJobs.begin();
    while (it != m_WaitingJobs.end() && it->second != pJob) {
        ++it;
    }
    if (it != m_WaitingJobs.end()) {
        m_WaitingJobs.erase(it);
        m_pWakeupEvent->Signal();
        isFound = true;
    } else {
        JobChain::iterator jobIt = m_ReadyJobs.begin();
        while (jobIt != m_ReadyJobs.end() && *jobIt != pJob) {
            jobIt = JobChain::iterator((*jobIt)->m_pChainNext);
        }
        if (jobIt != m_ReadyJobs.end()) {
            m_ReadyJobs.erase(jobIt);
            isFound = true;
        } else {
            jobIt = m_SuspendedJobs.begin();
            while (jobIt != m_SuspendedJobs.end() && *jobIt != pJob) {
                jobIt = JobChain::iterator((*jobIt)->m_pChainNext);
            }
            if (jobIt != m_SuspendedJobs.end()) {
                m_SuspendedJobs.erase(jobIt);
                isFound = true;
            } else if (m_pCurrentJob == pJob) {
                m_IsJobRunning = false;
                isFound = true;
            } else {
                return false;
            }
        }
    }
    pJob->ReleaseRef();
    return isFound;
}

// 0x003D93E4
u32 nn::nex::Scheduler::DispatchIfSingleThreaded(unsigned int timeout, unsigned int flags)
{
    if (m_pWorkerThread->m_Threads.size() != 0) {
        return 0;
    }
    return Dispatch(timeout, flags);
}

// 0x003D9408 | fefates:callseq [tier C]
void nn::nex::Scheduler::QueueImpl(nn::nex::Job* pJob, bool isFirst)
{
    switch (pJob->m_State) {
    case Job::STATE_INITIAL:
        pJob->SetState(Job::STATE_READY);
        // fall through
    case Job::STATE_READY:
        if (isFirst) {
            m_ReadyJobs.push_front(pJob);
        } else {
            m_ReadyJobs.push_back(pJob);
        }
        break;
    case Job::STATE_SUSPENDED:
        if (isFirst) {
            m_SuspendedJobs.push_front(pJob);
        } else {
            m_SuspendedJobs.push_back(pJob);
        }
        break;
    case Job::STATE_WAITING:
        m_WaitingJobs.insert(std::pair<const Time, Job*>(SystemClock::GetTime() + pJob->m_WaitTime, pJob));
        break;
    default:
        break;
    }
    m_pWakeupEvent->Signal();
}

// 0x003D95FC | fefates:callseq [tier C]
nn::nex::Scheduler::Scheduler(unsigned int lockLevel, nn::nex::Scheduler::SchedulerWorkerThread* pWorkerThread)
    : m_pCurrentJob(nullptr), m_IsJobRunning(false), m_EventHandler(1), m_Lock(lockLevel), m_QueueLock(1024), m_RegularLock(1024),
      m_ProfilingUnit(L"JobProcessing", 0), m_Unknown0x198(0), m_Unknown0x1A0(0), m_LastDispatchTime(0), m_IsTracing(false)
{
    m_IsTerminating = false;
    m_pWakeupEvent = m_EventHandler.CreateEventObject(0, 0);
    if (pWorkerThread != nullptr) {
        m_pWorkerThread = pWorkerThread;
    } else {
        m_pWorkerThread = new SchedulerWorkerThread();
    }
    m_pWorkerThread->m_pScheduler = this;
    m_Context = PseudoSingleton::GetCurrentContext();
}

// 0x003D987C
// 0x003D984C (deleting dtor)
nn::nex::Scheduler::~Scheduler()
{
    m_pWorkerThread->Stop();
    delete m_pWorkerThread;
    while (m_RegularJobs.size() != 0) {
        UnregisterRegularJob(m_RegularJobs.m_pFirst);
    }
    m_EventHandler.DeleteEventObject(m_pWakeupEvent);
}

// 0x0072EA8C
u32 nn::nex::Scheduler::GetTimeout(const nn::nex::Time& deadline) const
{
    if (deadline.m_Value == 0) {
        return static_cast<u32>(-1);
    }
    Time now = SystemClock::GetTime();
    if (now.m_Value >= deadline.m_Value) {
        return 0;
    }
    return static_cast<u32>(deadline.m_Value) - static_cast<u32>(now.m_Value);
}

// 0x0072EAD0
bool nn::nex::Scheduler::GetTotalNumberOfJobs()
{
    ScopedCS lock(m_QueueLock);
    return GetNumberOfQueuedJobs() + m_IsJobRunning;
}

// 0x007E44E8 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qChain<Job*, DefaultChainPolicy<Job*> >::iterator qChain<Job*, DefaultChainPolicy<Job*> >::erase(iterator it)
{
    return EraseInline(it);
}

// 0x007E45D0
// 0x007E4560 (deleting dtor)
template qChain<Job*, DefaultChainPolicy<Job*> >::~qChain();

} // namespace nex
} // namespace nn

namespace __rw {
// 0x007FBA94 (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::Job*>, __select1st<std::pair<const nn::nex::Time, nn::nex::Job*>, nn::nex::Time>,
          std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::Job*> > >::iterator
__rb_tree<nn::nex::Time, std::pair<const nn::nex::Time, nn::nex::Job*>, __select1st<std::pair<const nn::nex::Time, nn::nex::Job*>, nn::nex::Time>,
          std::less<nn::nex::Time>, nn::nex::MemAllocator<std::pair<const nn::nex::Time, nn::nex::Job*> > >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}
} // namespace __rw
