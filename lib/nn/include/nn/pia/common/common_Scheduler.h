#pragma once

#include "decomp.h"
#include "nn/pia/common/common_BackgroundScheduler.h"
#include "nn/pia/common/common_CriticalSection.h"
#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_ListBase.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace common {
class BackgroundScheduler;

// RTTI N2nn3pia6common9SchedulerE @ 0x008CFF24
// vtable 0x00901624 (vptr 0x0090162C), offset_to_top 0, 1 entries
//
// Executes the jobs of pia: the foreground ones in Dispatch (called by the application), the
// background ones in the thread of the BackgroundScheduler. One instance (CreateInstance). The
// member names are ours.
class Scheduler : public ::nn::pia::common::RootObject
{
public:
    // (inline in CreateInstance)
    explicit Scheduler(int priority) : m_DispatchCount(0), m_CriticalSection(6)
    {
        m_JobList.SetOffset(offsetof(Job, m_ListNode));
        m_NextJobList.SetOffset(offsetof(Job, m_ListNode));
        m_pBackgroundScheduler = new BackgroundScheduler(priority, &m_CriticalSection);
    }
    // (inline in DestroyInstance)
    ~Scheduler() { delete m_pBackgroundScheduler; }
    virtual void Trace(u64 flag) const; // 0x0073357C slot 0x00 (name after StepSequenceJob::Trace)

    static Scheduler* GetInstance() { return s_pInstance; }

    // the parameter is the priority of the background thread (less than 32); name is ours
    static nn::Result CreateInstance(int priority); // 0x00429B2C | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x00429C44 | fefates:bytes [tier B]

    // executes the foreground jobs that are due, for at most timeoutMSec (0 = no limit)
    void Dispatch(unsigned int timeoutMSec); // 0x00429CB0 | fefates:bytes [tier B]
    // queues the job by its time (in the background scheduler if background)
    void EntryJob(nn::pia::common::Job* job, bool background); // 0x00429F04 | fefates:bytes-fuzzy [tier B]
    // queues the job for the next Dispatch
    void EntryJobNext(nn::pia::common::Job* job); // 0x004295DC | fefates:bytes [tier B]
    void ResetJob(nn::pia::common::Job* job); // 0x00429FA8 | fefates:bytes [tier B]
    // the dispatch count goes to the monitoring data
    void SetMonitoringData(); // 0x00429C9C | fefates:callseq-callee [tier C]

    CriticalSection& GetCriticalSection() { return m_CriticalSection; }
    const Time& GetDispatchTime() const { return m_DispatchTime; }

    OffsetList<Job> m_JobList;       // 0x04, by time, the latest first
    OffsetList<Job> m_NextJobList;   // 0x18, for the next Dispatch
    Time m_DispatchTime;             // 0x30, when the last Dispatch began
    u32 m_DispatchCount;             // 0x38
    CriticalSection m_CriticalSection; // 0x3C
    BackgroundScheduler* m_pBackgroundScheduler; // 0x48

    static Scheduler* s_pInstance; // 0x00975A38
};
ASSERT_SIZE(Scheduler, 0x50);
ASSERT_OFFSET(Scheduler, m_DispatchTime, 0x30);
ASSERT_OFFSET(Scheduler, m_CriticalSection, 0x3C);
} // namespace common
} // namespace pia
} // namespace nn
