#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local25LocalDisconnectNetworkJobE
// vtable 0x00901108 (vptr 0x00901110), offset_to_top 0, 6 entries
//
// A client leaves the network (LocalNetwork::DisconnectNetwork): it stops the background job and
// disconnects in it. The member names are ours.
class LocalDisconnectNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalDisconnectNetworkJob(); // 0x004206C0
    virtual ~LocalDisconnectNetworkJob(); // 0x00420748 slot 0x00
    // 0x00420704 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007316A4 slot 0x14

    common::ExecuteResult WaitForCancel(); // 0x00420304
    common::ExecuteResult WaitDisconnectNetwork(); // 0x0042034C
    common::ExecuteResult TryPrepareDisconnectNetwork(); // 0x004204A0
    void Cleanup(); // 0x0042060C (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext); // 0x00420648

    // the cancel of the caller (name is ours)
    inline void StartCancel();

    common::CallContext* m_pCallContext;           // 0x40, of the caller
    common::CallContext* m_pBackgroundCallContext; // 0x44, of the background job
};
ASSERT_SIZE(LocalDisconnectNetworkJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
