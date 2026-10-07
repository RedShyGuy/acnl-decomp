#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local21LocalCreateNetworkJobE @ 0x008CFBE0
// vtable 0x00900DD8 (vptr 0x00900DE0), offset_to_top 0, 6 entries
//
// The creation of LocalNetwork::CreateNetwork: the background job of the network creates it with
// an own CallContext, this job waits for it and passes the result on. The member names are ours.
class LocalCreateNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalCreateNetworkJob(); // 0x0041AF3C
    virtual ~LocalCreateNetworkJob(); // 0x0041AFC4 slot 0x00
    // 0x0041AF80 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007311DC slot 0x14

    common::ExecuteResult WaitForCancel(); // 0x0041ACC0
    common::ExecuteResult WaitCreateNetwork(); // 0x0041AD10 | fefates:bytes [tier B]
    void Cleanup(); // 0x0041AE38 (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalCreateNetworkSetting* pSetting); // 0x0041AE74 (name is ours)

    common::CallContext* m_pCallContext;          // 0x40, of the caller
    common::CallContext* m_pBackgroundCallContext; // 0x44, of the background job
};
ASSERT_SIZE(LocalCreateNetworkJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
