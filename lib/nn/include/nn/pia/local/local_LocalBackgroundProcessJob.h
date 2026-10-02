#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalBackgroundProcessJobE @ 0x008CFC94
// vtable 0x009010B8 (vptr 0x009010C0), offset_to_top 0, 11 entries
class LocalBackgroundProcessJob : public ::nn::pia::common::StepSequenceJob
{
public:
    struct JobPriority { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~LocalBackgroundProcessJob(); // 0x0042026C slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00420258 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0073169C slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void StartupCreateNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalCreateNetworkSetting*); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void StartupDestroyNetwork(nn::pia::common::CallContext*); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void StartupScanNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalScanNetworkSetting*); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void StartupDisconnectNetwork(nn::pia::common::CallContext*); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    void PrepareDestroyNetwork(); // 0x0042005C | fefates:bytes [tier B]
    void PrepareDisconnectNetwork(); // 0x004200E4 | fefates:bytes [tier B]
    void Cleanup(); // 0x0042016C | fefates:bytes [tier B]
    void Startup(nn::pia::common::CallContext*, nn::pia::local::LocalBackgroundProcessJob::JobPriority); // 0x004201B0 | fefates:bytes [tier B]
    LocalBackgroundProcessJob(); // 0x00420228 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
