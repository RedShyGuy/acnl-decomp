#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session26ConfigParticipationJobBaseE @ 0x008D0110
// vtable 0x00901C78 (vptr 0x00901C80), offset_to_top 0, 18 entries
class ConfigParticipationJobBase : public ::nn::pia::common::StepSequenceJob
{
public:
    ConfigParticipationJobBase(); // ctor candidate(s) 0x004462AC (unverified)
    virtual ~ConfigParticipationJobBase(); // 0x004463AC slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00446388 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073420C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x004459F0 slot 0x18 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x1C(); // 0x00444D40 slot 0x1C | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x20(); // 0x00444D20 slot 0x20 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x24(); // 0x00734178 slot 0x24 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x28(); // 0x00444D10 slot 0x28 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x2C(); // 0x00444D0C slot 0x2C | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x30(); // 0x0044598C slot 0x30 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x34(); // 0x00734180 slot 0x34 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x38(); // 0x00445984 slot 0x38 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x3C(); // 0x00445988 slot 0x3C | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x40(); // 0x0044453C slot 0x40 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
    virtual void vf_0x44(); // 0x00445C64 slot 0x44 | virtual slot, introduced by nn::pia::session::ConfigParticipationJobBase
};
} // namespace session
} // namespace pia
} // namespace nn
