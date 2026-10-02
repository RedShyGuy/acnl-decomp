#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session17DestroySessionJobE @ 0x008CFFF0
// vtable 0x0090191C (vptr 0x00901924), offset_to_top 0, 10 entries
class DestroySessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    DestroySessionJob(); // ctor candidate(s) 0x00438EC8 (unverified)
    virtual ~DestroySessionJob(); // 0x00438F34 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00438F0C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007338FC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00438D3C slot 0x18 | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
