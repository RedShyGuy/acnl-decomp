#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session31ClearMatchmakeSystemPasswordJobE @ 0x008D0128
// vtable 0x00901CD8 (vptr 0x00901CE0), offset_to_top 0, 8 entries
//
// The network (inet) clears the system password of the matchmake session in its steps and ends
// with FailureProcess / CompleteProcess.
class ClearMatchmakeSystemPasswordJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ClearMatchmakeSystemPasswordJob(); // 0x004479D8
    virtual ~ClearMatchmakeSystemPasswordJob(); // 0x00447A30 slot 0x00
    // 0x00447A08 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734210 slot 0x14
    virtual void Cleanup(); // 0x0044798C slot 0x18
    // the network sets its first step
    virtual nn::Result vf_0x1C() = 0; // slot 0x1C

    // the last steps (the network fails the call context itself)
    common::ExecuteResult FailureProcess(); // 0x00447918
    common::ExecuteResult CompleteProcess(); // 0x00447960

    u32 m_SessionId;                     // 0x40
    common::CallContext* m_pCallContext; // 0x44, of Session
    common::CallContext m_CallContext;   // 0x48, of the network
};
ASSERT_OFFSET(ClearMatchmakeSystemPasswordJob, m_CallContext, 0x48);
ASSERT_SIZE(ClearMatchmakeSystemPasswordJob, 0x60);
} // namespace session
} // namespace pia
} // namespace nn
