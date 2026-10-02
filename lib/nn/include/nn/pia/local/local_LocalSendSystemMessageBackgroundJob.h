#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local35LocalSendSystemMessageBackgroundJobE @ 0x008CFDCC
// vtable 0x00901490 (vptr 0x00901498), offset_to_top 0, 7 entries
class LocalSendSystemMessageBackgroundJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalSendSystemMessageBackgroundJob(); // ctor candidate(s) 0x0042589C (unverified)
    virtual ~LocalSendSystemMessageBackgroundJob(); // 0x004258C4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x004258B4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00731800 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00425820 slot 0x18 | virtual slot, introduced by nn::pia::local::LocalSendSystemMessageBackgroundJob
};
} // namespace local
} // namespace pia
} // namespace nn
