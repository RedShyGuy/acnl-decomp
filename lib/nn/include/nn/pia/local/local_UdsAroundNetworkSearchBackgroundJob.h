#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchBackgroundJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local35UdsAroundNetworkSearchBackgroundJobE @ 0x008CFDD8
// vtable 0x009014B4 (vptr 0x009014BC), offset_to_top 0, 7 entries
class UdsAroundNetworkSearchBackgroundJob : public ::nn::pia::local::LocalAroundNetworkSearchBackgroundJob
{
public:
    virtual ~UdsAroundNetworkSearchBackgroundJob(); // 0x00425DD0 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00425C20 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00731804 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x004258C8 slot 0x18 | virtual slot, introduced by nn::pia::local::LocalAroundNetworkSearchBackgroundJob
    UdsAroundNetworkSearchBackgroundJob(); // 0x00425BFC | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
