#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local22LocalConnectNetworkJobE @ 0x008CFC34
// vtable 0x00900F80 (vptr 0x00900F88), offset_to_top 0, 6 entries
class LocalConnectNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalConnectNetworkJob(); // ctor candidate(s) 0x0041D948 (unverified)
    virtual ~LocalConnectNetworkJob(); // 0x0041D9D0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041D98C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007315C0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void ProcessSucceeded(); // 0x0041D624 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
