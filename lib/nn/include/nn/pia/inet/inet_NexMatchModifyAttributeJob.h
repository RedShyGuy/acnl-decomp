#pragma once

#include "decomp.h"
#include "nn/pia/session/session_ModifyAttributeJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet26NexMatchModifyAttributeJobE @ 0x008CFA20
// vtable 0x00900628 (vptr 0x00900630), offset_to_top 0, 8 entries
class NexMatchModifyAttributeJob : public ::nn::pia::session::ModifyAttributeJob
{
public:
    NexMatchModifyAttributeJob(); // ctor candidate(s) 0x0040CFC0 (unverified)
    virtual ~NexMatchModifyAttributeJob(); // 0x00439CB8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0040CFE0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F858 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0040CFA8 slot 0x18 | virtual slot, introduced by nn::pia::session::ModifyAttributeJob
    virtual void vf_0x1C(); // 0x0040CC50 slot 0x1C | virtual slot, introduced by nn::pia::session::ModifyAttributeJob
};
} // namespace inet
} // namespace pia
} // namespace nn
