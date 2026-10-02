#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session20OpenParticipationJobE @ 0x008D0068
// vtable 0x00901A14 (vptr 0x00901A1C), offset_to_top 0, 6 entries
class OpenParticipationJob : public ::nn::pia::common::StepSequenceJob
{
public:
    OpenParticipationJob(); // ctor candidate(s) 0x0043AD18 (unverified)
    virtual ~OpenParticipationJob(); // 0x0043AD6C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043AD48 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073392C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
};
} // namespace session
} // namespace pia
} // namespace nn
