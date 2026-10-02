#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local22LocalDestroyNetworkJobE @ 0x008CFC40
// vtable 0x00900FA0 (vptr 0x00900FA8), offset_to_top 0, 6 entries
class LocalDestroyNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalDestroyNetworkJob(); // ctor candidate(s) 0x0041E408 (unverified)
    virtual ~LocalDestroyNetworkJob(); // 0x0041E4B8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041E474 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007315C4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
};
} // namespace local
} // namespace pia
} // namespace nn
