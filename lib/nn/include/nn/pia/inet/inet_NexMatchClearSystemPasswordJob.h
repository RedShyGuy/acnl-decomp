#pragma once

#include "decomp.h"
#include "nn/pia/session/session_ClearMatchmakeSystemPasswordJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet30NexMatchClearSystemPasswordJobE @ 0x008CFA68
// vtable 0x0090073C (vptr 0x00900744), offset_to_top 0, 8 entries
class NexMatchClearSystemPasswordJob : public ::nn::pia::session::ClearMatchmakeSystemPasswordJob
{
public:
    NexMatchClearSystemPasswordJob(); // ctor candidate(s) 0x004114C4 (unverified)
    virtual ~NexMatchClearSystemPasswordJob(); // 0x00447A2C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004114E4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072FA64 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x004114AC slot 0x18 | virtual slot, introduced by nn::pia::session::ClearMatchmakeSystemPasswordJob
    virtual void vf_0x1C(); // 0x00411158 slot 0x1C | virtual slot, introduced by nn::pia::session::ClearMatchmakeSystemPasswordJob
};
} // namespace inet
} // namespace pia
} // namespace nn
