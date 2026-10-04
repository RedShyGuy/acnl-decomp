#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Job.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common15StepSequenceJobE @ 0x008CFE80
// vtable 0x00901580 (vptr 0x00901588), offset_to_top 0, 6 entries
//
// A job that executes a sequence of steps: the current step is a member function of the derived
// job (with its name for the trace); each step sets the next one. Layout from the constructor;
// the member names are ours.
class StepSequenceJob : public ::nn::pia::common::Job
{
public:
    typedef ExecuteResult (StepSequenceJob::*StepFunction)();

    // RTTI N2nn3pia6common15StepSequenceJob4StepE @ 0x008CFE74
    // vtable 0x0090156C (vptr 0x00901574), offset_to_top 0, 3 entries
    class Step : public ::nn::pia::common::RootObject
    {
    public:
        Step() : m_Function(nullptr), m_pName(nullptr) {}
        virtual ~Step(); // 0x00427510 slot 0x00
        // 0x0042750C slot 0x04 (deleting dtor)
        virtual void Trace(u64 flag) const; // 0x00731AF0 slot 0x08 (name after StepSequenceJob::Trace)

        StepFunction m_Function; // 0x04
        const char* m_pName;     // 0x0C
    };

    StepSequenceJob(); // 0x0042752C | fefates:bytes [tier B]
    virtual ~StepSequenceJob(); // 0x004275A0 slot 0x00 | fefates:callgraph
    // 0x00427580 slot 0x04 (deleting dtor)
    virtual void Reset(bool clearAll); // 0x00427514 slot 0x08 | fefates:bytes
    virtual ExecuteResult ExecuteCore(); // 0x004273C0 slot 0x0C | fefates:bytes
    // called when a canceled job stops (empty here)
    virtual void CancelCleanup(); // 0x00427434 slot 0x10 | slot vf_0x10 of nn::pia::common::StepSequenceJob
    virtual void Trace(unsigned long long flag) const; // 0x00731AF4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob

    // waits while the job runs in the background; a foreground or suspended job is canceled and
    // reset (the parameter is the sleep time in milliseconds; its name is ours)
    void WaitForCompletion(unsigned int intervalMSec); // 0x0042744C | fefates:bytes [tier B]

    // the next step (inline in the derived jobs)
    template <typename T>
    void SetStep(ExecuteResult (T::*function)(), const char* pName)
    {
        m_Step.m_Function = static_cast<StepFunction>(function);
        m_Step.m_pName = pName;
    }

    Step m_Step;               // 0x20
    bool m_IsCancelRequested;  // 0x30
    s64 m_Unknown0x38;         // 0x38 (0 in the constructor, not used by the functions here)
};
ASSERT_SIZE(StepSequenceJob, 0x40);
ASSERT_OFFSET(StepSequenceJob, m_IsCancelRequested, 0x30);
} // namespace common
} // namespace pia
} // namespace nn
