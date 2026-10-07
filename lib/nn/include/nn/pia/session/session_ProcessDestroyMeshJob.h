#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session21ProcessDestroyMeshJobE @ 0x008D0098
// vtable 0x00901A84 (vptr 0x00901A8C), offset_to_top 0, 6 entries
//
// A client whose host destroys the mesh: answers the host, then disconnects the stations. The
// steps are from the strings of the binary; the layout is from the constructor, the member names
// and the names marked so are ours.
class ProcessDestroyMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ProcessDestroyMeshJob(); // 0x0043F7FC | fefates:bytes [tier B]
    virtual ~ProcessDestroyMeshJob(); // 0x0043F844 slot 0x00
    // 0x0043F834 slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00734058 slot 0x14

    // false if it runs already or the station leaves the mesh anyway
    bool Startup(); // 0x0043F630 | fefates:bytes-fuzzy [tier B]
    // (name is ours)
    void Cleanup(); // 0x0043F624

    // the steps
    common::ExecuteResult SendDestroyResponse(); // 0x0043F57C
    common::ExecuteResult CleanupMesh(); // 0x0043F4F4

    common::Time m_Deadline;               // 0x40, of the response
    s32 m_TimeoutMSec;                     // 0x48 (1000)
    bool m_IsRunning;                      // 0x4C
};
ASSERT_OFFSET(ProcessDestroyMeshJob, m_IsRunning, 0x4C);
ASSERT_SIZE(ProcessDestroyMeshJob, 0x50);
} // namespace session
} // namespace pia
} // namespace nn
