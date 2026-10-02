#pragma once

#include "decomp.h"
#include "nn/pia/session/session_AutoMatchmakeJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet24NexMatchAutoMatchmakeJobE @ 0x008CF9D8
// vtable 0x00900524 (vptr 0x0090052C), offset_to_top 0, 16 entries
class NexMatchAutoMatchmakeJob : public ::nn::pia::session::AutoMatchmakeJob
{
public:
    NexMatchAutoMatchmakeJob(); // ctor candidate(s) 0x0040A0CC (unverified)
    virtual ~NexMatchAutoMatchmakeJob(); // 0x00437CC8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0040A118 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F844 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x20(); // 0x00408F30 slot 0x20 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x24(); // 0x00408F14 slot 0x24 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x28(); // 0x00407DB4 slot 0x28 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x2C(); // 0x00407D7C slot 0x2C | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x30(); // 0x004099A4 slot 0x30 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x34(); // 0x004099F0 slot 0x34 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x38(); // 0x004096F0 slot 0x38 | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
    virtual void vf_0x3C(); // 0x004094B0 slot 0x3C | virtual slot, introduced by nn::pia::session::AutoMatchmakeJob
};
} // namespace inet
} // namespace pia
} // namespace nn
