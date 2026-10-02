#pragma once

#include "decomp.h"
#include "nn/pia/session/session_CreateSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet24NexMatchCreateSessionJobE @ 0x008CF9E4
// vtable 0x0090056C (vptr 0x00900574), offset_to_top 0, 10 entries
class NexMatchCreateSessionJob : public ::nn::pia::session::CreateSessionJob
{
public:
    NexMatchCreateSessionJob(); // ctor candidate(s) 0x0040B25C (unverified)
    virtual ~NexMatchCreateSessionJob(); // 0x004382A0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0040B288 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F848 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0040B240 slot 0x18 | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x1C(); // 0x0040A128 slot 0x1C | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x20(); // 0x0040AF14 slot 0x20 | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x24(); // 0x0040AF60 slot 0x24 | virtual slot, introduced by nn::pia::session::CreateSessionJob
};
} // namespace inet
} // namespace pia
} // namespace nn
