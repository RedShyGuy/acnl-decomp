#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalScanNetworkJobE @ 0x008CFBB0
// vtable 0x00900CDC (vptr 0x00900CE4), offset_to_top 0, 6 entries
//
// The scan of LocalNetwork::ScanNetwork: the background job of the network scans with an own
// CallContext, this job waits for it and passes the result on. The member names are ours.
class LocalScanNetworkJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalScanNetworkJob(); // 0x0041A340
    virtual ~LocalScanNetworkJob(); // 0x0041A3C8 slot 0x00
    // 0x0041A384 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x007311CC slot 0x14

    common::ExecuteResult WaitForCancel(); // 0x0041A0E4 | fefates:bytes [tier B]
    common::ExecuteResult WaitScanNetwork(); // 0x0041A13C
    void Cleanup(); // 0x0041A254 (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalScanNetworkSetting* pSetting); // 0x0041A290 | fefates:bytes-fuzzy [tier B]

    common::CallContext* m_pCallContext;          // 0x40, of the caller
    common::CallContext* m_pBackgroundCallContext; // 0x44, of the background job
};
ASSERT_SIZE(LocalScanNetworkJob, 0x48);
} // namespace local
} // namespace pia
} // namespace nn
