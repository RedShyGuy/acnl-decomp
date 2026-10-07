#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session13CreateMeshJobE @ 0x008CFF60
// vtable 0x009016AC (vptr 0x009016B4), offset_to_top 0, 9 entries
//
// Creates the mesh with the local station as its host: sets the protocols of the mesh up, creates
// the local station and starts the protocols, the job that takes the join requests and the clock.
// The steps are from the strings of the binary; the member names and the names marked so are
// ours.
class CreateMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    CreateMeshJob(); // 0x00431098
    virtual ~CreateMeshJob(); // 0x004310CC slot 0x00
    // 0x004310B8 slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x0073386C slot 0x14
    virtual nn::Result StartupImpl(); // 0x00430A38 slot 0x18 | fefates:bytes
    virtual void CleanupImpl(); // 0x00430A34 slot 0x1C
    // the monitoring data of the session begin (empty here; name is ours)
    virtual void SetupMonitoringData(); // 0x00430FE0 slot 0x20

    nn::Result Startup(nn::pia::common::CallContext* pCallContext); // 0x0043102C | fefates:callgraph
    void Cleanup(); // 0x00430FE4 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult SetupSystemProtocols(); // 0x00430EEC | fefates:bytes [tier B]
    common::ExecuteResult SetupLocalStation(); // 0x00430D18
    common::ExecuteResult SetupMeshStatus(); // 0x00430AA0

    common::CallContext* m_pCallContext; // 0x40, of the caller (null when signaled)
};
ASSERT_OFFSET(CreateMeshJob, m_pCallContext, 0x40);
ASSERT_SIZE(CreateMeshJob, 0x48);
} // namespace session
} // namespace pia
} // namespace nn
