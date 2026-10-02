#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local37LocalAroundNetworkSearchBackgroundJobE @ 0x008CFDE4
// vtable 0x009014D8 (vptr 0x009014E0), offset_to_top 0, 7 entries
class LocalAroundNetworkSearchBackgroundJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~LocalAroundNetworkSearchBackgroundJob(); // 0x00425DD4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00425DC0 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00731808 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    void AroundNetworkSearch(); // 0x00425C30 | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, const nn::pia::local::LocalAroundNetworkSearchSetting&); // 0x00425CBC | fefates:bytes [tier B]
    LocalAroundNetworkSearchBackgroundJob(); // 0x00425D74 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
