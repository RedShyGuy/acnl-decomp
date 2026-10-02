#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local28LocalEventCheckBackgroundJobE @ 0x008CFD24
// vtable 0x00901264 (vptr 0x0090126C), offset_to_top 0, 6 entries
class LocalEventCheckBackgroundJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalEventCheckBackgroundJob(); // ctor candidate(s) 0x00423018 (unverified)
    virtual ~LocalEventCheckBackgroundJob(); // 0x00423048 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00423038 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316C8 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
};
} // namespace local
} // namespace pia
} // namespace nn
