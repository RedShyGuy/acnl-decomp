#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_StepSequenceJob.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace common {
class CallContext;
} // namespace common
namespace inet {
class NatDetecter;
class NatTraverser;

// RTTI N2nn3pia4inet15NatDetectionJobE @ 0x008CF884
// vtable 0x008FFFF0 (vptr 0x008FFFF8), offset_to_top 0, 6 entries
//
// Runs the check of a NatDetecter: opens its socket, sends the queued messages until the
// replies arrive or the time is up, and lets the detecter evaluate them. The member names are
// ours.
class NatDetectionJob : public ::nn::pia::common::StepSequenceJob
{
public:
    NatDetectionJob(); // 0x003E7990 | fefates:bytes [tier B]
    virtual ~NatDetectionJob(); // 0x0042759C slot 0x00 | fefates:callgraph
    // 0x003E79D8 slot 0x04 (deleting dtor)
    virtual void CancelCleanup(); // 0x003E72B0 slot 0x10 | fefates:bytes

    // (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, nn::pia::inet::NatDetecter* pDetecter, nn::pia::inet::NatTraverser* pNatTraverser); // 0x003E7358

    common::ExecuteResult StepPreTest(); // 0x003E70C8 | fefates:bytes [tier B]
    common::ExecuteResult StepStart(); // 0x003E785C | fefates:bytes [tier B]
    common::ExecuteResult StepSend(); // 0x003E7464 | fefates:bytes [tier B]
    common::ExecuteResult StepWait(); // 0x003E7754 | fefates:bytes [tier B]
    common::ExecuteResult StepEnd(); // 0x003E73F0 | fefates:bytes [tier B]
    common::ExecuteResult StepComplete(); // 0x003E71BC | fefates:bytes [tier B]
    // the check runs once more or ends
    void stopReceivingMessage(); // 0x003E72EC | fefates:bytes [tier B]

    common::CallContext* m_pCallContext;  // 0x40
    NatDetecter* m_pDetecter;             // 0x44
    NatTraverser* m_pNatTraverser;        // 0x48
    // the end of the check and the time of the next send
    common::Time m_Deadline;              // 0x50
    common::Time m_NextSendTime;          // 0x58
    bool m_IsSending;                     // 0x60
    bool m_IsReceived;                    // 0x61
    common::Time m_SendStartTime;         // 0x68
    // the socket could not be closed
    bool m_IsCloseFailed;                 // 0x70
};
ASSERT_OFFSET(NatDetectionJob, m_Deadline, 0x50);
ASSERT_OFFSET(NatDetectionJob, m_SendStartTime, 0x68);
ASSERT_SIZE(NatDetectionJob, 0x78);
} // namespace inet
} // namespace pia
} // namespace nn
