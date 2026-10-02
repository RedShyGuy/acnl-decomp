#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session16AutoMatchmakeJobE @ 0x008CFFC0
// vtable 0x0090187C (vptr 0x00901884), offset_to_top 0, 16 entries
class AutoMatchmakeJob : public ::nn::pia::common::StepSequenceJob
{
public:
    AutoMatchmakeJob(); // ctor candidate(s) 0x00437C24 (unverified)
    virtual ~AutoMatchmakeJob(); // 0x00437CCC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00437C9C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007338F0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x004378B0 slot 0x18 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x1C(); // 0x007338E8 slot 0x1C | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x20(); // 0x00437768 slot 0x20 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x24(); // 0x00437764 slot 0x24 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x2C(); // 0x00436D60 slot 0x2C | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
