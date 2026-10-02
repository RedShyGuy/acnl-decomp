#pragma once

#include "decomp.h"
#include "nn/pia/session/session_DestroySessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet25NexMatchDestroySessionJobE @ 0x008CF9FC
// vtable 0x009005B0 (vptr 0x009005B8), offset_to_top 0, 10 entries
class NexMatchDestroySessionJob : public ::nn::pia::session::DestroySessionJob
{
public:
    NexMatchDestroySessionJob(); // ctor candidate(s) 0x0040C360 (unverified)
    virtual ~NexMatchDestroySessionJob(); // 0x00438F30 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0040C378 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F850 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00438D38 slot 0x18 | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x1C(); // 0x0040BCFC slot 0x1C | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x20(); // 0x0040BD9C slot 0x20 | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x24(); // 0x0040BD48 slot 0x24 | virtual slot, introduced by nn::pia::session::DestroySessionJob
};
} // namespace inet
} // namespace pia
} // namespace nn
