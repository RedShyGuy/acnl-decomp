#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21CloseParticipationJobE @ 0x008D008C
// vtable 0x00901A64 (vptr 0x00901A6C), offset_to_top 0, 6 entries
class CloseParticipationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    CloseParticipationJob(); // ctor candidate(s) 0x0043F474 (unverified)
    virtual ~CloseParticipationJob(); // 0x0043F4D4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043F4B0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734054 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
};
} // namespace session
} // namespace pia
} // namespace nn
