#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalScanNetworkJobE @ 0x008CFBB0
// vtable 0x00900CDC (vptr 0x00900CE4), offset_to_top 0, 6 entries
class LocalScanNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalScanNetworkJob(); // ctor candidate(s) 0x0041A340 (unverified)
    virtual ~LocalScanNetworkJob(); // 0x0041A3C8 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041A384 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007311CC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void WaitForCancel(); // 0x0041A0E4 | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, const nn::pia::local::LocalScanNetworkSetting*); // 0x0041A290 | fefates:bytes-fuzzy [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
