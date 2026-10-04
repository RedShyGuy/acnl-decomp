#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_ListBase.h"
#include "pead/peadDelegate2.h"
#include "pead/peadDelegateThread.h"
#include "pead/peadEvent.h"

namespace nn {
namespace pia {
namespace common {
class CriticalSection;

// Executes the background jobs in its own thread ("Pia BackgroundScheduler"), under the lock of
// the Scheduler. Layout from the constructor; the member names are ours.
class BackgroundScheduler : public RootObject
{
public:
    BackgroundScheduler(int priority, nn::pia::common::CriticalSection* pCriticalSection); // 0x0042828C | fefates:bytes [tier B]
    ~BackgroundScheduler(); // 0x00428378 | fefates:bytes [tier B]

    // the thread function (called by the delegate)
    void Dispatch(pead::Thread* thread, int message); // 0x00428128 | fefates:bytes [tier B]
    // queues the job by its time and wakes the thread (Scheduler::EntryJob; name is ours)
    void EntryJob(nn::pia::common::Job* job); // 0x00429618
    void ResetJob(nn::pia::common::Job* job); // 0x00428250 | fefates:bytes [tier B]

    pead::Delegate2<BackgroundScheduler, pead::Thread*, int> m_Delegate; // 0x00
    pead::DelegateThread m_Thread;                  // 0x10
    nn::pia::common::CriticalSection* m_pCriticalSection; // 0x9C, the one of the Scheduler
    pead::Event m_Event;                            // 0xA0, signaled when a job comes
    bool m_IsFinalizing;                            // 0xB4
    OffsetList<Job> m_JobList;                      // 0xB8, by time, the latest first
};
ASSERT_SIZE(BackgroundScheduler, 0xCC);
} // namespace common
} // namespace pia
} // namespace nn
