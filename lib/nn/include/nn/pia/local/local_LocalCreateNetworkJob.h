#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalCreateNetworkJobE @ 0x008CFBE0
// vtable 0x00900DD8 (vptr 0x00900DE0), offset_to_top 0, 6 entries
class LocalCreateNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalCreateNetworkJob(); // ctor candidate(s) 0x0041AF3C (unverified)
    virtual ~LocalCreateNetworkJob(); // 0x0041AFC4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041AF80 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007311DC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void WaitCreateNetwork(); // 0x0041AD10 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
