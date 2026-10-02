#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15StepSequenceJobE @ 0x008CE508
// vtable 0x008FCFB8 (vptr 0x008FCFC0), offset_to_top 0, 13 entries
class StepSequenceJob : public ::nn::nex::Job
{
public:
    class Step;
    StepSequenceJob(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~StepSequenceJob(); // 0x0037DC4C slot 0x00 | fefates:callgraph
    // 0x0037DC20 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void Execute(); // 0x0037DB18 slot 0x0C | fefates:bytes
    virtual void AddActivity(const wchar_t*); // 0x0037D958 slot 0x14 | slot vf_0x14 of nn::nex::Job
    virtual void vf_0x18(); // 0x0072B6E0 slot 0x18 | virtual slot, introduced by nn::nex::Job
    virtual void CheckExceptions(); // 0x0037D95C slot 0x30 | slot vf_0x30 of nn::nex::StepSequenceJob
    void ProcessCallResult(nn::nex::StepSequenceJob::Step*); // 0x0037D960 | fefates:bytes [tier B]
    void ResumeOnCallCompletion(nn::nex::CallContext*, nn::nex::StepSequenceJob::Step*); // 0x0037DA0C | fefates:bytes [tier B]
    void SetStep(const nn::nex::StepSequenceJob::Step&); // 0x0037DB88 | fefates:bytes [tier B]
    StepSequenceJob(nn::nex::Job::JobType, const nn::nex::DebugString&); // 0x0037DBC8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
