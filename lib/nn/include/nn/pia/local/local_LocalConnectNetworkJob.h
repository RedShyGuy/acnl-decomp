#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local22LocalConnectNetworkJobE @ 0x008CFC34
// vtable 0x00900F80 (vptr 0x00900F88), offset_to_top 0, 6 entries
//
// The connection of LocalNetwork::ConnectNetwork: the background job of the network connects
// with an own CallContext, this job waits for it (and for the end of the background job) and
// passes the result on. The member names are ours.
class LocalConnectNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalConnectNetworkJob(); // 0x0041D948
    virtual ~LocalConnectNetworkJob(); // 0x0041D9D0 slot 0x00
    // 0x0041D98C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007315C0 slot 0x14

    common::ExecuteResult WaitForCancel(); // 0x0041D5D4
    common::ExecuteResult ProcessSucceeded(); // 0x0041D624 | fefates:bytes [tier B]
    common::ExecuteResult WaitConnectNetwork(); // 0x0041D6C4
    void Cleanup(); // 0x0041D844 (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalConnectNetworkSetting* pSetting); // 0x0041D880 (name is ours)

    common::CallContext* m_pCallContext;          // 0x40, of the caller
    common::CallContext* m_pBackgroundCallContext; // 0x44, of the background job
};
ASSERT_SIZE(LocalConnectNetworkJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
