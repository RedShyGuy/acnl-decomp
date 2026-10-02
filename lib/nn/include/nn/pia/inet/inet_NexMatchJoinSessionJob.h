#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet22NexMatchJoinSessionJobE @ 0x008CF990
// vtable 0x0090040C (vptr 0x00900414), offset_to_top 0, 17 entries
class NexMatchJoinSessionJob : public ::nn::pia::session::JoinSessionJob
{
public:
    NexMatchJoinSessionJob(); // ctor candidate(s) 0x00403DC8 (unverified)
    virtual ~NexMatchJoinSessionJob(); // 0x004320C8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00403E08 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F1D4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x004027AC slot 0x18 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x1C(); // 0x00402790 slot 0x1C | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x30(); // 0x00401638 slot 0x30 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x34(); // 0x0040161C slot 0x34 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x38(); // 0x00403208 slot 0x38 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x3C(); // 0x00402FCC slot 0x3C | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x40(); // 0x00402F6C slot 0x40 | virtual slot, introduced by nn::pia::session::JoinSessionJob
};
} // namespace inet
} // namespace pia
} // namespace nn
