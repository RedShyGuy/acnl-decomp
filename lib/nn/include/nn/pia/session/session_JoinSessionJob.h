#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session14JoinSessionJobE @ 0x008CFF78
// vtable 0x009016F8 (vptr 0x00901700), offset_to_top 0, 17 entries
class JoinSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    JoinSessionJob(); // ctor candidate(s) 0x00432030 (unverified)
    virtual ~JoinSessionJob(); // 0x004320CC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043209C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007338C8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00431CD8 slot 0x18 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x1C(); // 0x00431CD4 slot 0x1C | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x20(); // 0x00431CDC slot 0x20 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x24(); // 0x00431CF8 slot 0x24 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x28(); // 0x00431D14 slot 0x28 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x2C(); // 0x00733874 slot 0x2C | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x004315A0 slot 0x34 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
