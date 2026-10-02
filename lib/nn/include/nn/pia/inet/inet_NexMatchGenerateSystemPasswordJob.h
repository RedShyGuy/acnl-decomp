#pragma once

#include "decomp.h"
#include "nn/pia/session/session_GenerateMatchmakeSystemPasswordJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet33NexMatchGenerateSystemPasswordJobE @ 0x008CFA98
// vtable 0x009007C8 (vptr 0x009007D0), offset_to_top 0, 8 entries
class NexMatchGenerateSystemPasswordJob : public ::nn::pia::session::GenerateMatchmakeSystemPasswordJob
{
public:
    NexMatchGenerateSystemPasswordJob(); // ctor candidate(s) 0x004122E8 (unverified)
    virtual ~NexMatchGenerateSystemPasswordJob(); // 0x00447B90 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00412308 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072FA70 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x004122D0 slot 0x18 | virtual slot, introduced by nn::pia::session::GenerateMatchmakeSystemPasswordJob
    virtual void vf_0x1C(); // 0x00411FB4 slot 0x1C | virtual slot, introduced by nn::pia::session::GenerateMatchmakeSystemPasswordJob
};
} // namespace inet
} // namespace pia
} // namespace nn
