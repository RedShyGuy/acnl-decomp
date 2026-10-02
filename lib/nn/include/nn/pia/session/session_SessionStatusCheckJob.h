#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21SessionStatusCheckJobE @ 0x008D00BC
// vtable 0x00901AC4 (vptr 0x00901ACC), offset_to_top 0, 6 entries
class SessionStatusCheckJob : public ::nn::pia::common::StepSequenceJob
{
public:
    SessionStatusCheckJob(); // ctor candidate(s) 0x00440E80 (unverified)
    virtual ~SessionStatusCheckJob(); // 0x00440ED0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00440EAC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734064 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
};
} // namespace session
} // namespace pia
} // namespace nn
