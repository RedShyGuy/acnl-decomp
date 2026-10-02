#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session15LeaveSessionJobE @ 0x008CFF90
// vtable 0x009017B0 (vptr 0x009017B8), offset_to_top 0, 16 entries
class LeaveSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LeaveSessionJob(); // ctor candidate(s) 0x004341C8 (unverified)
    virtual ~LeaveSessionJob(); // 0x0043423C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00434214 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007338D8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00433E1C slot 0x18 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x00433DD4 slot 0x28 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x00433CB4 slot 0x30 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x34(); // 0x007338D0 slot 0x34 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x38(); // 0x00433CA4 slot 0x38 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x3C(); // 0x0043341C slot 0x3C | virtual slot, introduced by nn::pia::session::LeaveSessionJob
};
} // namespace session
} // namespace pia
} // namespace nn
