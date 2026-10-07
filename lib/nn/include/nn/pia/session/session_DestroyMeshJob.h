#pragma once

#include "decomp.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session14DestroyMeshJobE @ 0x008CFF6C
// vtable 0x009016D8 (vptr 0x009016E0), offset_to_top 0, 6 entries
//
// Destroys the mesh (host): tells every station and waits for their responses or the time, then
// cleans the mesh up. Started by the application (Mesh::DestroyMesh) or by pia itself, with a call
// context that can be associated later (AssociateSystemWith). The steps are from the strings of
// the binary; the layout is from the constructor, the member names are ours.
class DestroyMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    DestroyMeshJob(); // 0x00431540 | fefates:bytes [tier B]
    virtual ~DestroyMeshJob(); // 0x0043159C slot 0x00
    // 0x0043158C slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00733870 slot 0x14

    // false if the job runs; isSystem: started by pia (the disconnect reason stays)
    bool Startup(nn::pia::common::CallContext* pCallContext, bool isSystem); // 0x00431444 | fefates:bytes-fuzzy [tier B]
    void Cleanup(); // 0x004313EC | fefates:bytes [tier B]
    // the call context of the application for a destroy pia started; false if there is none
    bool AssociateSystemWith(nn::pia::common::CallContext* pCallContext); // 0x004312D4 | fefates:bytes [tier B]
    void ReceiveDestroyResponse(nn::pia::StationIndex stationIndex); // 0x004313D4 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult SendDestroyMesh(); // 0x004311B4
    common::ExecuteResult WaitDestroyResponse(); // 0x00431318 | fefates:bytes [tier B]
    common::ExecuteResult CleanupMesh(); // 0x004310F0

    common::CallContext* m_pCallContext;     // 0x40, of the caller (null when signaled)
    common::Time m_Deadline;                 // 0x48
    s32 m_TimeoutMSec;                       // 0x50 (5000)
    bool m_IsWaitingStation[12];             // 0x54, the stations whose response is awaited
    bool m_IsWaitingResponse;                // 0x60
    bool m_IsRunning;                        // 0x61
    bool m_IsSystem;                         // 0x62, started by pia
};
ASSERT_OFFSET(DestroyMeshJob, m_TimeoutMSec, 0x50);
ASSERT_OFFSET(DestroyMeshJob, m_IsWaitingStation, 0x54);
ASSERT_OFFSET(DestroyMeshJob, m_IsWaitingResponse, 0x60);
ASSERT_SIZE(DestroyMeshJob, 0x68);
} // namespace session
} // namespace pia
} // namespace nn
