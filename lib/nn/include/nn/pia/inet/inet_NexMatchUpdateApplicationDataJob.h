#pragma once

#include "decomp.h"
#include "nn/pia/session/session_UpdateApplicationDataJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet32NexMatchUpdateApplicationDataJobE @ 0x008CFA8C
// vtable 0x009007A0 (vptr 0x009007A8), offset_to_top 0, 8 entries
class NexMatchUpdateApplicationDataJob : public ::nn::pia::session::UpdateApplicationDataJob
{
public:
    NexMatchUpdateApplicationDataJob(); // ctor candidate(s) 0x00411F78 (unverified)
    virtual ~NexMatchUpdateApplicationDataJob(); // 0x00442D1C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00411FA4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072FA6C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00411F4C slot 0x18 | virtual slot, introduced by nn::pia::session::UpdateApplicationDataJob
    virtual void vf_0x1C(); // 0x00411900 slot 0x1C | virtual slot, introduced by nn::pia::session::UpdateApplicationDataJob
};
} // namespace inet
} // namespace pia
} // namespace nn
