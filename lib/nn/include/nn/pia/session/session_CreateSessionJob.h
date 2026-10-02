#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session16CreateSessionJobE @ 0x008CFFCC
// vtable 0x009018C4 (vptr 0x009018CC), offset_to_top 0, 10 entries
class CreateSessionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    CreateSessionJob(); // ctor candidate(s) 0x00438248 (unverified)
    virtual ~CreateSessionJob(); // 0x004382A4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0043827C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007338F4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00438148 slot 0x18 | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
