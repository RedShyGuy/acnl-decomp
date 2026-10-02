#pragma once

#include "decomp.h"
#include "nn/pia/session/session_UpdateApplicationDataJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local34LocalMatchUpdateApplicationDataJobE @ 0x008CFDC0
// vtable 0x00901468 (vptr 0x00901470), offset_to_top 0, 8 entries
class LocalMatchUpdateApplicationDataJob : public ::nn::pia::session::UpdateApplicationDataJob
{
public:
    LocalMatchUpdateApplicationDataJob(); // ctor candidate(s) 0x004257C8 (unverified)
    virtual ~LocalMatchUpdateApplicationDataJob(); // 0x004257F0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004257E0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007317FC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00442C80 slot 0x18 | virtual slot, introduced by nn::pia::session::UpdateApplicationDataJob
    virtual void vf_0x1C(); // 0x00425654 slot 0x1C | virtual slot, introduced by nn::pia::session::UpdateApplicationDataJob
};
} // namespace local
} // namespace pia
} // namespace nn
