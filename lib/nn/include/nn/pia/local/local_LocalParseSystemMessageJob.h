#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local26LocalParseSystemMessageJobE @ 0x008CFCE8
// vtable 0x009011D0 (vptr 0x009011D8), offset_to_top 0, 7 entries
class LocalParseSystemMessageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalParseSystemMessageJob(); // ctor candidate(s) 0x004213C8 (unverified)
    virtual ~LocalParseSystemMessageJob(); // 0x004213F0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004213E0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316B0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00421378 slot 0x18 | virtual slot, introduced by nn::pia::local::LocalParseSystemMessageJob
};
} // namespace local
} // namespace pia
} // namespace nn
