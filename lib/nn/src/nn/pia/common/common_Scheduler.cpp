#include "nn/pia/common/common_Scheduler.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_BackgroundScheduler.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_SessionStateMonitoringContent.h"

namespace nn {
namespace pia {
namespace common {
namespace {
// the priorities of the background thread (svc thread priorities)
const int PRIORITY_MAX = 32;
} // namespace

// 0x00975A38
Scheduler* Scheduler::s_pInstance;

// 0x0073357C (name after StepSequenceJob::Trace)
void nn::pia::common::Scheduler::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00429B2C | fefates:bytes [tier B]
nn::Result nn::pia::common::Scheduler::CreateInstance(int priority)
{
    if (!IsInitialized()) {
        return RESULT_NOT_INITIALIZED;
    }
    if (!IsInSetupMode()) {
        return RESULT_INVALID_STATE;
    }
    if (s_pInstance != nullptr) {
        return RESULT_ALREADY_EXISTS;
    }
    if (priority >= PRIORITY_MAX) {
        return RESULT_INVALID_ARGUMENT;
    }
    s_pInstance = new Scheduler(priority);
    return nn::Result();
}

// 0x00429C44 | fefates:bytes [tier B]
void nn::pia::common::Scheduler::DestroyInstance()
{
    if (s_pInstance != nullptr) {
        delete s_pInstance;
        s_pInstance = nullptr;
    }
}

// 0x00429C9C | fefates:callseq-callee [tier C]
void nn::pia::common::Scheduler::SetMonitoringData()
{
    g_SessionStateMonitoringContent.m_DispatchCount = m_DispatchCount;
}

// 0x00429CB0 | fefates:bytes [tier B]
void nn::pia::common::Scheduler::Dispatch(unsigned int timeoutMSec)
{
    Time now;
    now.SetNow();
    m_DispatchTime = now;
    m_DispatchCount++;

    Time deadline;
    if (timeoutMSec == 0) {
        deadline = Time::INFINITE_TIME;
    } else {
        deadline = m_DispatchTime + TimeSpan(TimeSpan::GetTicksPerMSec().m_Tick * timeoutMSec);
    }

    Time current;
    m_CriticalSection.Lock();
    while (m_JobList.GetCount() != 0) {
        current.SetNow();
        if (deadline < current) {
            // no time left: the jobs for the next Dispatch go into the list by their time
            Job* pos = m_JobList.Front();
            Job* job;
            while ((job = m_NextJobList.PopFront()) != nullptr) {
                while (pos != nullptr && job->m_ExecuteTime < pos->m_ExecuteTime) {
                    pos = m_JobList.Next(pos);
                }
                if (pos != nullptr) {
                    m_JobList.InsertBefore(pos, job);
                } else {
                    m_JobList.PushBack(job);
                }
            }
            m_CriticalSection.Unlock();
            return;
        }
        Job* job = m_JobList.Back();
        if (current < job->m_ExecuteTime) {
            break;
        }
        m_JobList.PopBackNode();
        job->Execute(false);
    }
    while (m_NextJobList.GetCount() != 0) {
        m_JobList.PushBack(m_NextJobList.PopFront());
    }
    m_CriticalSection.Unlock();
}

// 0x00429F04 | fefates:bytes-fuzzy [tier B]
void nn::pia::common::Scheduler::EntryJob(nn::pia::common::Job* job, bool background)
{
    if (background) {
        m_pBackgroundScheduler->EntryJob(job);
        return;
    }
    for (Job* pos = m_JobList.Begin(); pos != m_JobList.End(); pos = m_JobList.Advance(pos)) {
        if (!(job->m_ExecuteTime < pos->m_ExecuteTime)) {
            m_JobList.InsertBefore(pos, job);
            return;
        }
    }
    m_JobList.PushBack(job);
}

// 0x004295DC | fefates:bytes [tier B]
void nn::pia::common::Scheduler::EntryJobNext(nn::pia::common::Job* job)
{
    m_NextJobList.PushFront(job);
}

// 0x00429FA8 | fefates:bytes [tier B]
void nn::pia::common::Scheduler::ResetJob(nn::pia::common::Job* job)
{
    if (job->IsForeground()) {
        if (m_JobList.IsInclude(job)) {
            m_JobList.Erase(job);
        } else if (m_NextJobList.IsInclude(job)) {
            m_NextJobList.Erase(job);
        }
    } else if (job->IsBackground()) {
        m_pBackgroundScheduler->ResetJob(job);
    }
}

} // namespace common
} // namespace pia
} // namespace nn
