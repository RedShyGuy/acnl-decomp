#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local13LocalEventJobE @ 0x008CFAE0
// vtable 0x00900894 (vptr 0x0090089C), offset_to_top 0, 6 entries
class LocalEventJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalEventJob(); // ctor candidate(s) 0x00416700 (unverified)
    virtual ~LocalEventJob(); // 0x00416728 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00416718 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00730080 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
};
} // namespace local
} // namespace pia
} // namespace nn
