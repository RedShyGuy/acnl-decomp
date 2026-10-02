#pragma once

#include "decomp.h"
#include "nn/pia/session/session_KickoutManageJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalKickoutManageJobE @ 0x008CFBF8
// vtable 0x00900E18 (vptr 0x00900E20), offset_to_top 0, 8 entries
class LocalKickoutManageJob : public ::nn::pia::session::KickoutManageJob
{
public:
    LocalKickoutManageJob(); // ctor candidate(s) 0x0041C388 (unverified)
    virtual ~LocalKickoutManageJob(); // 0x004389B0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041C3A0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007311E4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0041C360 slot 0x18 | virtual slot, introduced by nn::pia::session::KickoutManageJob
    virtual void vf_0x1C(); // 0x0041C378 slot 0x1C | virtual slot, introduced by nn::pia::session::KickoutManageJob
};
} // namespace local
} // namespace pia
} // namespace nn
