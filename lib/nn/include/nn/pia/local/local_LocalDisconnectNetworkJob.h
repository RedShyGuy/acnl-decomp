#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalDisconnectNetworkJobE @ 0x008CFCAC
// vtable 0x00901108 (vptr 0x00901110), offset_to_top 0, 6 entries
class LocalDisconnectNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalDisconnectNetworkJob(); // ctor candidate(s) 0x004206C0 (unverified)
    virtual ~LocalDisconnectNetworkJob(); // 0x00420748 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00420704 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316A4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void WaitDisconnectNetwork(); // 0x0042034C | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*); // 0x00420648 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
