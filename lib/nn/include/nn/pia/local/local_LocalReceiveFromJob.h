#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalReceiveFromJobE @ 0x008CFBA4
// vtable 0x00900CB8 (vptr 0x00900CC0), offset_to_top 0, 7 entries
class LocalReceiveFromJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalReceiveFromJob(); // ctor candidate(s) 0x0041A0B8 (unverified)
    virtual ~LocalReceiveFromJob(); // 0x0041A0E0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041A0D0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007311C8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0041A048 slot 0x18 | virtual slot, introduced by nn::pia::local::LocalReceiveFromJob
    void CheckLocalInputStream(); // 0x00419FB8 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
