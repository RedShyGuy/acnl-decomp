#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session18ModifyAttributeJobE @ 0x008D0038
// vtable 0x009019CC (vptr 0x009019D4), offset_to_top 0, 8 entries
class ModifyAttributeJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ModifyAttributeJob(); // ctor candidate(s) 0x00439C60 (unverified)
    virtual ~ModifyAttributeJob(); // 0x00439CBC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00439C94 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073391C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00439B40 slot 0x18 | virtual slot, introduced by nn::pia::session::ModifyAttributeJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
