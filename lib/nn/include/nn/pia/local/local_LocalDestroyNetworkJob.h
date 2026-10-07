#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local22LocalDestroyNetworkJobE
// vtable 0x00900FA0 (vptr 0x00900FA8), offset_to_top 0, 6 entries
//
// The end of the network by the host (LocalNetwork::DestroyNetwork; with the host migration also
// LocalNetwork::DisconnectNetwork of the host): it stops the background job, tells the clients
// (after the last node table when the host migration follows) until they left or 10 seconds
// passed, and destroys the network in the background job. The member names are ours.
class LocalDestroyNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    static const s32 RESEND_INTERVAL_MSEC = 300;
    static const s32 TIMEOUT_MSEC = 10000;

    LocalDestroyNetworkJob(); // 0x0041E408
    virtual ~LocalDestroyNetworkJob(); // 0x0041E4B8 slot 0x00
    // 0x0041E474 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007315C4 slot 0x14

    common::ExecuteResult WaitForCancel(); // 0x0041DA0C
    common::ExecuteResult WaitDestroyNetwork(); // 0x0041DA54
    common::ExecuteResult TryPrepareDestroyNetwork(); // 0x0041DBE0
    common::ExecuteResult SendDestroyNetworkMessage(); // 0x0041DD58
    common::ExecuteResult WaitUntilAllClientsDisconnection(); // 0x0041DEA8
    common::ExecuteResult WaitUntilAllClientsReceiveUpdateSessionMessage(); // 0x0041E14C
    void Cleanup(); // 0x0041E2DC (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, bool isHostMigration); // 0x0041E344 (name is ours)

    // the cancel of the caller (in every step but WaitForCancel; name is ours)
    inline void StartCancel();

    common::CallContext* m_pCallContext;           // 0x40, of the caller
    common::CallContext* m_pBackgroundCallContext; // 0x44, of the background job
    common::Time m_SendTime;                       // 0x48, of the last destroy message
    common::Time m_StartTime;                      // 0x50, of the first destroy message
    common::Time m_UpdateSessionTime;              // 0x58, of the last node table
    bool m_IsHostMigration;                        // 0x60
};
ASSERT_OFFSET(LocalDestroyNetworkJob, m_SendTime, 0x48);
ASSERT_SIZE(LocalDestroyNetworkJob, 0x68);
} // namespace local
} // namespace pia
} // namespace nn
