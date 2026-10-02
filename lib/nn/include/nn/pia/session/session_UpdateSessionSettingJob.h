#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session23UpdateSessionSettingJobE @ 0x008D00EC
// vtable 0x00901C00 (vptr 0x00901C08), offset_to_top 0, 8 entries
class UpdateSessionSettingJob : public ::nn::pia::common::StepSequenceJob
{
public:
    UpdateSessionSettingJob(); // ctor candidate(s) 0x00442B9C (unverified)
    virtual ~UpdateSessionSettingJob(); // 0x00442BEC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00442BC4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073416C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00442B50 slot 0x18 | virtual slot, introduced by nn::pia::session::UpdateSessionSettingJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
