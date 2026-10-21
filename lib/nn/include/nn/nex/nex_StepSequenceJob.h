#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class CallContext;

// A job that runs one step (a member function) after the other: a step sets the next one with
// SetStep, or waits for a call with ContinueAfterCall. The names of the members and of the
// functions without a reference name are ours.
// RTTI N2nn3nex15StepSequenceJobE @ 0x008CE508
// vtable 0x008FCFB8 (vptr 0x008FCFC0), offset_to_top 0, 13 entries
class StepSequenceJob : public ::nn::nex::Job
{
public:
    // a step: a member function of the job and its name (empty in this build)
    // RTTI N2nn3nex15StepSequenceJob4StepE @ 0x008CE4FC
    // vtable 0x008FCFA8 (vptr 0x008FCFB0), offset_to_top 0, 2 entries
    class Step : public ::nn::nex::RootObject
    {
    public:
        typedef void (StepSequenceJob::*Function)();

        // (inline)
        Step() : m_pFunction(nullptr), m_pName(nullptr) {}
        template <typename T>
        Step(void (T::*pFunction)(), const char* pName) : m_pFunction(static_cast<Function>(pFunction)), m_pName(pName)
        {
        }
        Step(const Step& other) : m_pFunction(other.m_pFunction), m_pName(other.m_pName) {}
        virtual ~Step(); // 0x0037DB14 slot 0x00
        // 0x0037DB10 slot 0x04 (deleting dtor)

        Function m_pFunction; // 0x04
        const char* m_pName;  // 0x0C
    };

    StepSequenceJob(nn::nex::Job::JobType type, const nn::nex::DebugString& debugString); // 0x0037DBC8 | fefates:bytes [tier B]
    virtual ~StepSequenceJob(); // 0x0037DC4C slot 0x00 | fefates:callgraph
    // 0x0037DC20 slot 0x04 (deleting dtor)
    virtual void Execute(); // 0x0037DB18 slot 0x0C | fefates:bytes
    virtual void AddActivity(const wchar_t* pActivity); // 0x0037D958 slot 0x14 | slot vf_0x14 of nn::nex::Job
    virtual nn::nex::String GetTraceString() const; // 0x0072B6E0 slot 0x18
    virtual void CheckExceptions(); // 0x0037D95C slot 0x30 | slot vf_0x30 of nn::nex::StepSequenceJob

    // the step is done: the job is ready again with the step (it is deleted) and releases the
    // reference of the call
    void ProcessCallResult(nn::nex::StepSequenceJob::Step* pStep); // 0x0037D960 | fefates:bytes [tier B]
    // the step runs when the call completes (the job holds a reference until then)
    void ResumeOnCallCompletion(nn::nex::CallContext* pContext, nn::nex::StepSequenceJob::Step* pStep); // 0x0037DA0C | fefates:bytes [tier B]
    // the step runs now if the call is not in progress, else when it completes
    void ContinueAfterCall(nn::nex::CallContext* pContext, nn::nex::StepSequenceJob::Step* pStep); // 0x0037DA8C (name is ours)
    void SetStep(const nn::nex::StepSequenceJob::Step& step); // 0x0037DB88 | fefates:bytes [tier B]

    // (inline; the name is ours) the step of a function of the job (ARMCC loads the function from a
    // constant)
    template <typename T>
    DECOMP_ALWAYS_INLINE void SetStepInline(void (T::*pFunction)(), const char* pName)
    {
        m_StepStartTime = SystemClock::GetTime();
        m_Step.m_pFunction = static_cast<Step::Function>(pFunction);
        m_Step.m_pName = pName;
    }
    // (inline; the name is ours)
    DECOMP_ALWAYS_INLINE void SetStepInline(const Step& step)
    {
        m_StepStartTime = SystemClock::GetTime();
        m_Step.m_pFunction = step.m_pFunction;
        m_Step.m_pName = step.m_pName;
    }

protected:
    // (for the derived jobs that are not decompiled yet)
    StepSequenceJob();

public:
    Time m_StepStartTime; // 0x40
    u32 m_StepCount;      // 0x48
    Step m_Step;          // 0x4C
    u32 m_Unknown0x5C;    // 0x5C (not set by the constructor)
    u64 m_TraceFlags;     // 0x60 (the name is ours)
};
ASSERT_OFFSET(StepSequenceJob, m_Step, 0x4C);
ASSERT_SIZE(StepSequenceJob, 0x68);
} // namespace nex
} // namespace nn
