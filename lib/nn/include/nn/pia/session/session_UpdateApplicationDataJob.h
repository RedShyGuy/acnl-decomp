#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session24UpdateApplicationDataJobE @ 0x008D00F8
// vtable 0x00901C28 (vptr 0x00901C30), offset_to_top 0, 8 entries
class UpdateApplicationDataJob : public ::nn::pia::common::StepSequenceJob
{
public:
    UpdateApplicationDataJob(); // ctor candidate(s) 0x00442CD0 (unverified)
    virtual ~UpdateApplicationDataJob(); // 0x00442D20 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00442CF8 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734170 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00442C84 slot 0x18 | virtual slot, introduced by nn::pia::session::UpdateApplicationDataJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
