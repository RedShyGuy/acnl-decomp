#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/local/local_LocalNetworkTypes.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local37LocalAroundNetworkSearchBackgroundJobE @ 0x008CFDE4
// vtable 0x009014D8 (vptr 0x009014E0), offset_to_top 0, 7 entries
//
// One scan of the search of the networks around in the background thread (the implementation
// does the scan, see UdsAroundNetworkSearchBackgroundJob). The member names are ours.
class LocalAroundNetworkSearchBackgroundJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalAroundNetworkSearchBackgroundJob(); // 0x00425D74
    // (the destructor of UdsAroundNetworkSearchBackgroundJob is a nop that falls into it)
    virtual ~LocalAroundNetworkSearchBackgroundJob(); // 0x00425DD4 slot 0x00
    // 0x00425DC0 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00731808 slot 0x14
    // the scan and the update of the found networks (name is ours)
    virtual nn::Result ScanNetwork() = 0; // slot 0x18

    common::ExecuteResult AroundNetworkSearch(); // 0x00425C30 | fefates:bytes [tier B]
    void Cleanup(); // 0x00425C88 (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, const nn::pia::local::LocalAroundNetworkSearchSetting& setting); // 0x00425CBC | fefates:bytes [tier B]

    nn::pia::local::LocalAroundNetworkSearchSetting m_Setting; // 0x40
    common::CallContext* m_pCallContext;                       // 0x54
};
ASSERT_SIZE(LocalAroundNetworkSearchBackgroundJob, 0x58);
} // namespace local
} // namespace pia
} // namespace nn
