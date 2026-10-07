#pragma once

#include "decomp.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session12LeaveMeshJobE @ 0x008CFF48
// vtable 0x00901660 (vptr 0x00901668), offset_to_top 0, 6 entries
//
// Leaves the mesh: sends the leave request to the host, waits for the response (MeshProtocol
// clears m_IsWaitingResponse), disconnects the other stations and waits until they are gone or
// the time is up. The steps are from the strings of the binary; the layout is from the
// constructor, the member names and the names marked so are ours.
class LeaveMeshJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LeaveMeshJob(); // 0x0042CAA8 | fefates:bytes [tier B]
    virtual ~LeaveMeshJob(); // 0x0042CAFC slot 0x00
    // 0x0042CAEC slot 0x04 (deleting dtor)
    virtual void Trace(unsigned long long flag) const; // 0x00733838 slot 0x14

    // false if the job runs (name is ours)
    bool Startup(nn::pia::common::CallContext* pCallContext); // 0x0042C950
    void Cleanup(); // 0x0042C8EC | fefates:bytes [tier B]
    // a second call context that is signaled with the first one; false if there is one already
    bool RegisterExtraCallback(nn::pia::common::CallContext* pCallContext); // 0x0042C744 | fefates:bytes [tier B]

    // the steps
    common::ExecuteResult SendLeaveRequest(); // 0x0042C4C0 | fefates:bytes [tier B]
    common::ExecuteResult WaitLeaveResponse(); // 0x0042C544 | fefates:bytes [tier B]
    common::ExecuteResult StartDisconnectStations(); // 0x0042C76C | fefates:bytes [tier B]
    common::ExecuteResult WaitLeavingProcess(); // 0x0042C660 | fefates:bytes [tier B]
    common::ExecuteResult LeaveSuccess(); // 0x0042C3FC

    // (inline; name is ours)
    void SignalSuccessToCallers()
    {
        if (m_pCallContext != nullptr) {
            if (m_pCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
                m_pCallContext->SignalSuccess(nn::Result());
            }
            m_pCallContext = nullptr;
        }
        if (m_pExtraCallContext != nullptr) {
            if (m_pExtraCallContext->GetState() == common::CallContext::STATE_CALL_IN_PROGRESS) {
                m_pExtraCallContext->SignalSuccess(nn::Result());
            }
            m_pExtraCallContext = nullptr;
        }
    }

    common::Time m_Deadline;                  // 0x40
    s32 m_TimeoutMSec;                        // 0x48 (5000)
    common::CallContext* m_pCallContext;      // 0x4C, of the caller (null when signaled)
    common::CallContext* m_pExtraCallContext; // 0x50 (RegisterExtraCallback)
    bool m_IsWaitingResponse;                 // 0x54
};
ASSERT_OFFSET(LeaveMeshJob, m_TimeoutMSec, 0x48);
ASSERT_OFFSET(LeaveMeshJob, m_IsWaitingResponse, 0x54);
ASSERT_SIZE(LeaveMeshJob, 0x58);
} // namespace session
} // namespace pia
} // namespace nn
