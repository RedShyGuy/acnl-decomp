#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_Api.h"
#include "nn/nex/nex_CallContext.h"
#include "nn/nex/nex_Callback.h"
#include "nn/nex/nex_Core.h"
#include "nn/nex/nex_UserContext.h"

namespace nn {
namespace nex {
// 0x0037DB14 slot 0x00
// 0x0037DB10 (deleting dtor)
nn::nex::StepSequenceJob::Step::~Step()
{
    // nothing to do
}

// (for the derived jobs that are not decompiled yet)
nn::nex::StepSequenceJob::StepSequenceJob() : m_StepCount(0), m_TraceFlags(0)
{
}

// 0x0037DBC8 | fefates:bytes [tier B]
nn::nex::StepSequenceJob::StepSequenceJob(nn::nex::Job::JobType type, const nn::nex::DebugString& debugString)
    : Job(type, debugString), m_StepStartTime(0), m_Step(), m_StepCount(0), m_TraceFlags(0)
{
}

// 0x0037DC4C slot 0x00 | fefates:callgraph
// 0x0037DC20 (deleting dtor)
nn::nex::StepSequenceJob::~StepSequenceJob()
{
    // nothing to do
}

// 0x0037DB18 slot 0x0C | fefates:bytes
void nn::nex::StepSequenceJob::Execute()
{
    do {
        CheckExceptions();
        if (m_State != STATE_RUNNING) {
            return;
        }
        Core::GetInstance();
        (this->*m_Step.m_pFunction)();
        m_StepCount++;
    } while (m_State == STATE_RUNNING);
}

// 0x0037D958 slot 0x14 | slot vf_0x14 of nn::nex::Job
void nn::nex::StepSequenceJob::AddActivity(const wchar_t* pActivity)
{
    // nothing to do
}

// 0x0072B6E0 slot 0x18
nn::nex::String nn::nex::StepSequenceJob::GetTraceString() const
{
    return String("");
}

// 0x0037D95C slot 0x30 | slot vf_0x30 of nn::nex::StepSequenceJob
void nn::nex::StepSequenceJob::CheckExceptions()
{
    // nothing to do
}

// 0x0037D960 | fefates:bytes [tier B]
void nn::nex::StepSequenceJob::ProcessCallResult(nn::nex::StepSequenceJob::Step* pStep)
{
    SetState(STATE_READY);
    SetStepInline(*pStep);
    if (pStep != nullptr) {
        delete pStep;
    }
    ReleaseRef();
}

// 0x0037DA0C | fefates:bytes [tier B]
void nn::nex::StepSequenceJob::ResumeOnCallCompletion(nn::nex::CallContext* pContext, nn::nex::StepSequenceJob::Step* pStep)
{
    AcquireRef();
    UserContext context;
    context.m_pPointer = new Callback<StepSequenceJob, Step*>(this, &StepSequenceJob::ProcessCallResult, pStep);
    pContext->RegisterCompletionCallback(InvokeCallbackOnCompletion, context, true);
}

// 0x0037DA8C (name is ours)
void nn::nex::StepSequenceJob::ContinueAfterCall(nn::nex::CallContext* pContext, nn::nex::StepSequenceJob::Step* pStep)
{
    if (pContext->m_State == CallContext::STATE_CALL_IN_PROGRESS) {
        ResumeOnCallCompletion(pContext, pStep);
        return;
    }
    SetState(STATE_READY);
    SetStepInline(*pStep);
    if (pStep != nullptr) {
        delete pStep;
    }
}

// 0x0037DB88 | fefates:bytes [tier B]
void nn::nex::StepSequenceJob::SetStep(const nn::nex::StepSequenceJob::Step& step)
{
    SetStepInline(step);
}

// 0x007E4D28
// 0x007E4D10 (deleting dtor)
template Callback<StepSequenceJob, StepSequenceJob::Step*>::~Callback();
// 0x0035C670 (name is ours)
template void Callback<StepSequenceJob, StepSequenceJob::Step*>::Invoke();
// 0x007E4CE4 | mk7dlp
template void Callback<StepSequenceJob, StepSequenceJob::Step*>::CallObjectMethod();

} // namespace nex
} // namespace nn
