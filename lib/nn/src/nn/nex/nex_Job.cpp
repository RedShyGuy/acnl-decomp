#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_UserContext.h"
#include "nn/nex/nex_Scheduler.h"
#include "nn/nex/nex_SpinTest.h"

namespace nn {
namespace nex {
namespace {
const wchar_t* const FILE_NAME = L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/Job.cpp";
} // namespace

// 0x0017F290 (name is ours)
bool nn::nex::Job::WaitForCompletionCallback(nn::nex::RootObject* pObject, unsigned int timeout)
{
    return static_cast<Job*>(pObject)->WaitForCompletion(timeout);
}

// 0x003CC588 (slot name from the reference symbols)
void nn::nex::Job::AddActivity(const wchar_t* pActivity)
{
    // nothing to do in this build
}

// 0x003CC578
void nn::nex::Job::ReadyOnCompletion(nn::nex::CallContext* pContext, const nn::nex::UserContext* pUserContext)
{
    static_cast<Job*>(pUserContext->m_pPointer)->SetToReady();
}

// 0x003CC580 (name is ours)
void nn::nex::Job::SetToReady()
{
    SetState(STATE_READY);
}

// 0x003CC718 (name is ours)
void nn::nex::Job::SetReadyState()
{
    SetState(STATE_READY);
}

// 0x003CC58C (name is ours)
void nn::nex::Job::SetToRunning()
{
    SetState(STATE_RUNNING);
}

// 0x003CC594 | fefates:bytes [tier B]
void nn::nex::Job::SetToWaiting(int waitTime)
{
    SetState(STATE_WAITING);
    m_WaitTime = waitTime;
}

// 0x003CC630 | fefates:callgraph [tier C]
void nn::nex::Job::SetToComplete()
{
    SetState(STATE_COMPLETED);
}

// 0x003CC638
void nn::nex::Job::vf_0x2C()
{
    // nothing to do in this build
}

// 0x003CC63C | fefates:callgraph [tier C]
void nn::nex::Job::SetToSuspended()
{
    SetState(STATE_SUSPENDED);
}

// 0x003CC644 (slot name from the reference symbols)
void nn::nex::Job::DecoratedExecute()
{
    Execute();
}

// 0x003CC650 | fefates:callgraph [tier C]
bool nn::nex::Job::WaitForCompletion(unsigned int timeout)
{
    bool isComplete = false;
    if (Scheduler::PreventBlockCall(timeout != 0, timeout, false, WaitForCompletionCallback, this, &isComplete)) {
        return isComplete;
    }
    SpinTest spinTest(10, timeout);
    spinTest.m_Mode = SpinTest::MODE_STOP;
    while (m_State != STATE_COMPLETED) {
        if (!spinTest.SpinOnce(const_cast<wchar_t*>(FILE_NAME), 208, const_cast<wchar_t*>(L"GetState()!=Complete"))) {
            break;
        }
    }
    return m_State == STATE_COMPLETED;
}

// 0x003CC700
void nn::nex::Job::vf_0x28()
{
    // nothing to do in this build
}

// 0x003CC704 (slot name from the reference symbols)
void nn::nex::Job::TestSuspendedJobState()
{
    // nothing to do in this build
}

// 0x003CC708 (slot name from the reference symbols)
bool nn::nex::Job::SkipWaitDelayAtTermination()
{
    return false;
}

// 0x003CC710 (slot name from the reference symbols)
void nn::nex::Job::SetDefaultPostExecutionState()
{
    SetState(STATE_COMPLETED);
}

// 0x003CC750 | fefates:bytes [tier B]
void nn::nex::Job::SetState(nn::nex::Job::State state)
{
    m_State = state;
    if (state == STATE_COMPLETED && m_pCompletionCallback != nullptr) {
        m_pCompletionCallback(this, &m_CompletionContext);
    }
}

// 0x003CC774 (slot name from the reference symbols)
void nn::nex::Job::CancelJob()
{
    // nothing to do in this build
}

// 0x003CC778 | fefates:callgraph [tier C]
nn::nex::Job::Job(nn::nex::Job::JobType type, const nn::nex::DebugString& debugString)
    : m_pChainNext(nullptr), m_pChainPrev(nullptr), m_Unknown0x14(0), m_pCompletionCallback(nullptr), m_CompletionContext(0), m_Type(type),
      m_DebugString(debugString), m_State(STATE_INITIAL), m_WaitTime(0), m_ExecutionCount(0), m_DispatchDeadline(0), m_UsesGlobalLock(false), m_Unknown0x39(0)
{
}

// 0x003CC80C | fefates:callgraph
// 0x003CC7E0 (deleting dtor)
nn::nex::Job::~Job()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072DF28
nn::nex::String nn::nex::Job::GetTraceString() const
{
    return String("");
}

} // namespace nex
} // namespace nn
