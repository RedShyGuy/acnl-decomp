#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_Scheduler.h"

namespace nn {
namespace pia {
namespace common {
// 0x00428B74 | fefates:bytes [tier B]
nn::pia::common::Job::Job() : m_State(STATE_IDLE), m_ExecuteCount(0)
{
}

// 0x00428BA8 | fefates:callgraph [tier C]
// 0x00428BA0 (deleting dtor)
nn::pia::common::Job::~Job()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x004288C4 | fefates:bytes [tier B]
void nn::pia::common::Job::Ready(bool background)
{
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Lock();
    if (m_State != STATE_IDLE) {
        cs.Unlock();
        return;
    }
    m_State = background ? STATE_BACKGROUND_READY : STATE_READY;
    m_ExecuteTime.SetNow();
    Scheduler::GetInstance()->EntryJob(this, background);
    cs.Unlock();
}

// 0x00428934 | fefates:bytes-fuzzy [tier B]
void nn::pia::common::Job::Reset(bool)
{
    Scheduler* scheduler = Scheduler::GetInstance();
    CriticalSection& cs = scheduler->GetCriticalSection();
    cs.Lock();
    switch (m_State) {
    case STATE_WAITING:
    case STATE_READY:
        Scheduler::GetInstance()->ResetJob(this);
        break;
    default:
        break;
    }
    m_ExecuteCount = 0;
    m_State = STATE_IDLE;
    m_ExecuteTime = Time();
    cs.Unlock();
}

// 0x004289A4 | fefates:bytes [tier B]
void nn::pia::common::Job::Resume(bool background)
{
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Lock();
    if (m_State != STATE_SUSPENDED) {
        cs.Unlock();
        return;
    }
    m_State = background ? STATE_BACKGROUND_READY : STATE_READY;
    m_ExecuteTime.SetNow();
    Scheduler::GetInstance()->EntryJob(this, background);
    cs.Unlock();
}

// 0x00428A14 | fefates:bytes-fuzzy [tier B]
void nn::pia::common::Job::Execute(bool background)
{
    m_State = background ? STATE_BACKGROUND_RUNNING : STATE_RUNNING;
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Unlock();
    ExecuteResult result = ExecuteCore();
    cs.Lock();
    m_ExecuteCount++;
    switch (result.m_State) {
    case ExecuteResult::STATE_CONTINUE:
    case ExecuteResult::STATE_CONTINUE_FOREGROUND:
    case ExecuteResult::STATE_CONTINUE_BACKGROUND:
        m_ExecuteTime.SetNow();
        if (result.m_State == ExecuteResult::STATE_CONTINUE_FOREGROUND) {
            background = false;
        } else if (result.m_State == ExecuteResult::STATE_CONTINUE_BACKGROUND) {
            background = true;
        }
        Scheduler::GetInstance()->EntryJob(this, background);
        m_State = background ? STATE_BACKGROUND_READY : STATE_READY;
        break;
    case ExecuteResult::STATE_SUCCESS:
    case ExecuteResult::STATE_FAILURE:
        m_State = STATE_FINISHED;
        break;
    case ExecuteResult::STATE_SUSPEND:
        m_State = STATE_SUSPENDED;
        break;
    case ExecuteResult::STATE_WAIT:
        m_ExecuteTime.SetNow();
        m_ExecuteTime += TimeSpan(TimeSpan::GetTicksPerMSec().m_Tick * result.m_WaitMSec);
        Scheduler::GetInstance()->EntryJob(this, background);
        m_State = background ? STATE_BACKGROUND_WAITING : STATE_WAITING;
        break;
    case ExecuteResult::STATE_NEXT_DISPATCH:
        m_ExecuteTime.SetNow();
        Scheduler::GetInstance()->EntryJobNext(this);
        m_State = background ? STATE_BACKGROUND_WAITING : STATE_WAITING;
        break;
    default:
        break;
    }
}

// 0x00733184 | fefates:callgraph [tier C]
bool nn::pia::common::Job::IsBackground() const
{
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Lock();
    switch (m_State) {
    case STATE_BACKGROUND_WAITING:
    case STATE_BACKGROUND_READY:
    case STATE_BACKGROUND_RUNNING:
        cs.Unlock();
        return true;
    default:
        cs.Unlock();
        return false;
    }
}

// 0x007331F0 | fefates:bytes-fuzzy [tier B]
bool nn::pia::common::Job::IsForeground() const
{
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Lock();
    switch (m_State) {
    case STATE_WAITING:
    case STATE_READY:
    case STATE_RUNNING:
        cs.Unlock();
        return true;
    default:
        cs.Unlock();
        return false;
    }
}

// 0x00733250 | fefates:callgraph [tier C]
Job::ExecuteState nn::pia::common::Job::GetState() const
{
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Lock();
    switch (m_State) {
    case STATE_WAITING:
    case STATE_BACKGROUND_WAITING:
        cs.Unlock();
        return EXECUTE_STATE_WAITING;
    case STATE_READY:
    case STATE_BACKGROUND_READY:
        cs.Unlock();
        return EXECUTE_STATE_READY;
    case STATE_RUNNING:
    case STATE_BACKGROUND_RUNNING:
        cs.Unlock();
        return EXECUTE_STATE_RUNNING;
    case STATE_SUSPENDED:
        cs.Unlock();
        return EXECUTE_STATE_SUSPENDED;
    case STATE_FINISHED:
        cs.Unlock();
        return EXECUTE_STATE_FINISHED;
    default:
        cs.Unlock();
        return EXECUTE_STATE_IDLE;
    }
}

// 0x00733304 | fefates:callgraph [tier C]
bool nn::pia::common::Job::IsRunning() const
{
    CriticalSection& cs = Scheduler::GetInstance()->GetCriticalSection();
    cs.Lock();
    switch (m_State) {
    case STATE_WAITING:
    case STATE_READY:
    case STATE_RUNNING:
    case STATE_BACKGROUND_WAITING:
    case STATE_BACKGROUND_READY:
    case STATE_BACKGROUND_RUNNING:
    case STATE_SUSPENDED:
        cs.Unlock();
        return true;
    default:
        cs.Unlock();
        return false;
    }
}

} // namespace common
} // namespace pia
} // namespace nn
