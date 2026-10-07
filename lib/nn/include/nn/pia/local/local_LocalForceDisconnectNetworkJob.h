#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local30LocalForceDisconnectNetworkJobE
// vtable 0x00901320 (vptr 0x00901328), offset_to_top 0, 6 entries
//
// A second leave request while the first one runs (LocalNetwork::DisconnectNetwork): it waits for
// the end of the host migration and leaves then, or waits until the station is out of the network.
// The member names are ours.
class LocalForceDisconnectNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    // what the job waits for
    enum ProcType
    {
        PROC_TYPE_WAIT_HOST_MIGRATION_END = 0,
        PROC_TYPE_WAIT_DISCONNECTED = 1,
    };

    LocalForceDisconnectNetworkJob(); // 0x0042388C
    virtual ~LocalForceDisconnectNetworkJob(); // 0x004238BC slot 0x00
    // 0x004238AC slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073176C slot 0x14

    common::ExecuteResult WaitDisconnected(); // 0x00423614
    common::ExecuteResult WaitHostMigrationEnd(); // 0x004236AC
    void Cleanup(); // 0x00423778
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, nn::pia::local::LocalForceDisconnectNetworkJob::ProcType procType); // 0x004237AC

    common::CallContext* m_pCallContext; // 0x40, of the caller
};
ASSERT_SIZE(LocalForceDisconnectNetworkJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
