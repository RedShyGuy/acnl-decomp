#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalBackgroundProcessJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local23UdsBackgroundProcessJobE @ 0x008CFC70
// vtable 0x0090101C (vptr 0x00901024), offset_to_top 0, 11 entries
class UdsBackgroundProcessJob : public ::nn::pia::local::LocalBackgroundProcessJob
{
public:
    virtual ~UdsBackgroundProcessJob(); // 0x00420268 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041F6FC slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00731688 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void StartupCreateNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalCreateNetworkSetting*); // 0x0041F16C slot 0x18 | slot vf_0x18 of nn::pia::local::LocalBackgroundProcessJob
    virtual void StartupDestroyNetwork(nn::pia::common::CallContext*); // 0x0041F41C slot 0x1C | fefates:bytes
    virtual void StartupScanNetwork(nn::pia::common::CallContext*, const nn::pia::local::LocalScanNetworkSetting*); // 0x0041F0E8 slot 0x20 | slot vf_0x20 of nn::pia::local::LocalBackgroundProcessJob
    virtual void vf_0x24(); // 0x0041F298 slot 0x24 | fefates:callseq
    virtual void StartupDisconnectNetwork(nn::pia::common::CallContext*); // 0x0041F600 slot 0x28 | fefates:bytes
    void CreateNetwork(); // 0x0041EBF8 | fefates:bytes [tier B]
    void ConnectNetwork(); // 0x0041ED64 | fefates:bytes [tier B]
    void DestroyNetwork(); // 0x0041EF58 | fefates:bytes [tier B]
    void DisconnectNetwork(); // 0x0041F000 | fefates:bytes [tier B]
    UdsBackgroundProcessJob(); // 0x0041F670 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
