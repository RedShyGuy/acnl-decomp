#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Job.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common15StepSequenceJobE @ 0x008CFE80
// vtable 0x00901580 (vptr 0x00901588), offset_to_top 0, 6 entries
class StepSequenceJob : public ::nn::pia::common::Job
{
public:
    class Step;
    virtual ~StepSequenceJob(); // 0x004275A0 slot 0x00 | fefates:callgraph
    // 0x00427580 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Reset(bool); // 0x00427514 slot 0x08 | fefates:bytes
    virtual void ExecuteCore(); // 0x004273C0 slot 0x0C | fefates:bytes
    virtual void CancelCleanup(); // 0x00427434 slot 0x10 | slot vf_0x10 of nn::pia::common::StepSequenceJob
    virtual void Trace(unsigned long long) const; // 0x00731AF4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void WaitForCompletion(unsigned int); // 0x0042744C | fefates:bytes [tier B]
    StepSequenceJob(); // 0x0042752C | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn
