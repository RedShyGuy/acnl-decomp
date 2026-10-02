#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveSessionJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalMatchLeaveSessionJobE @ 0x008CFCB8
// vtable 0x00901128 (vptr 0x00901130), offset_to_top 0, 16 entries
class LocalMatchLeaveSessionJob : public ::nn::pia::session::LeaveSessionJob
{
public:
    LocalMatchLeaveSessionJob(); // ctor candidate(s) 0x00420A24 (unverified)
    virtual ~LocalMatchLeaveSessionJob(); // 0x00420A4C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00420A3C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316A8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x1C(); // 0x00420820 slot 0x1C | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x20(); // 0x00420968 slot 0x20 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x24(); // 0x004207C8 slot 0x24 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x28(); // 0x004209BC slot 0x28 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x2C(); // 0x00420960 slot 0x2C | virtual slot, introduced by nn::pia::session::LeaveSessionJob
};
} // namespace local
} // namespace pia
} // namespace nn
