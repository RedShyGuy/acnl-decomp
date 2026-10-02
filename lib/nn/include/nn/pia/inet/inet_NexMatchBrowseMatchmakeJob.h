#pragma once

#include "decomp.h"
#include "nn/pia/session/session_BrowseMatchmakeJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet26NexMatchBrowseMatchmakeJobE @ 0x008CFA14
// vtable 0x00900600 (vptr 0x00900608), offset_to_top 0, 8 entries
class NexMatchBrowseMatchmakeJob : public ::nn::pia::session::BrowseMatchmakeJob
{
public:
    NexMatchBrowseMatchmakeJob(); // ctor candidate(s) 0x0040CBD8 (unverified)
    virtual ~NexMatchBrowseMatchmakeJob(); // 0x0040CC30 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0040CC0C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F854 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00439914 slot 0x18 | virtual slot, introduced by nn::pia::session::BrowseMatchmakeJob
    virtual void vf_0x1C(); // 0x0040C64C slot 0x1C | virtual slot, introduced by nn::pia::session::BrowseMatchmakeJob
};
} // namespace inet
} // namespace pia
} // namespace nn
