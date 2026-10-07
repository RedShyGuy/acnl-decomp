#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalBackgroundProcessJobE @ 0x008CFC94
// vtable 0x009010B8 (vptr 0x009010C0), offset_to_top 0, 11 entries
//
// The background job of LocalNetwork that runs the blocking calls of the network (scan, creation,
// connection, end) one at a time; a request with a higher priority (a lower JobPriority) stops a
// running one with Prepare...Network first. The layout is from the constructor; the member names
// are ours.
class LocalBackgroundProcessJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // the priority of the request (lower values first)
    enum JobPriority : u8
    {
        JOB_PRIORITY_DESTROY_NETWORK = 1,
        JOB_PRIORITY_DISCONNECT_NETWORK = 2,
        JOB_PRIORITY_CREATE_OR_CONNECT_NETWORK = 3,
        JOB_PRIORITY_SCAN_NETWORK = 7,
        JOB_PRIORITY_NONE = 10,
    };

    LocalBackgroundProcessJob(); // 0x00420228 | fefates:bytes [tier B]
    virtual ~LocalBackgroundProcessJob(); // 0x0042026C slot 0x00
    // 0x00420258 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073169C slot 0x14
    virtual nn::Result StartupCreateNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting) = 0; // slot 0x18
    virtual nn::Result StartupDestroyNetwork(nn::pia::common::CallContext* pCallContext) = 0; // slot 0x1C
    virtual nn::Result StartupScanNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalScanNetworkSetting* pSetting) = 0; // slot 0x20
    virtual nn::Result StartupConnectNetwork(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting) = 0; // slot 0x24
    virtual nn::Result StartupDisconnectNetwork(nn::pia::common::CallContext* pCallContext) = 0; // slot 0x28

    // the running request is stopped for the end of the network
    nn::Result PrepareDestroyNetwork(); // 0x0042005C | fefates:bytes [tier B]
    nn::Result PrepareDisconnectNetwork(); // 0x004200E4 | fefates:bytes [tier B]
    void Cleanup(); // 0x0042016C | fefates:bytes [tier B]
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, nn::pia::local::LocalBackgroundProcessJob::JobPriority priority); // 0x004201B0 | fefates:bytes [tier B]

    common::CallContext* m_pCallContext; // 0x40
    JobPriority m_JobPriority;           // 0x44
    bool m_IsDestroyPrepared;            // 0x45
    bool m_IsDisconnectPrepared;         // 0x46
};
ASSERT_SIZE(LocalBackgroundProcessJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
