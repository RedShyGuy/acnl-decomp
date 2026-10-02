#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21ProcessDestroyMeshJobE @ 0x008D0098
// vtable 0x00901A84 (vptr 0x00901A8C), offset_to_top 0, 6 entries
class ProcessDestroyMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~ProcessDestroyMeshJob(); // 0x0043F844 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043F834 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734058 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void Startup(); // 0x0043F630 | fefates:bytes-fuzzy [tier B]
    ProcessDestroyMeshJob(); // 0x0043F7FC | fefates:bytes [tier B]
};
} // namespace session
} // namespace pia
} // namespace nn
