#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
class CommonMatchmakeSession;
// RTTI N2nn3pia7session18ModifyAttributeJobE @ 0x008D0038
// vtable 0x009019CC (vptr 0x009019D4), offset_to_top 0, 8 entries
//
// Session::ModifyAttributeAsync: the network changes an attribute of the matchmake session in
// its steps and ends with FailureProcess / CompleteProcess.
class ModifyAttributeJob : public ::nn::pia::common::StepSequenceJob
{
public:
    ModifyAttributeJob(); // 0x00439C60
    virtual ~ModifyAttributeJob(); // 0x00439CBC slot 0x00
    // 0x00439C94 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x0073391C slot 0x14
    virtual void Cleanup(); // 0x00439B40 slot 0x18
    // the network sets its first step
    virtual nn::Result vf_0x1C() = 0; // slot 0x1C

    // (name is ours)
    nn::Result Startup(nn::pia::common::CallContext* pCallContext, u32 sessionId, u32 index, u32 value,
                       nn::pia::session::CommonMatchmakeSession* pSession); // 0x00439B94

    // the last steps (the network fails the call context itself)
    common::ExecuteResult FailureProcess(); // 0x00439AB0
    common::ExecuteResult CompleteProcess(); // 0x00439AF8

    common::CallContext* m_pCallContext; // 0x40, of Session
    common::CallContext m_CallContext;   // 0x44, of the network
    u32 m_SessionId;                     // 0x58
    u32 m_Index;                         // 0x5C, of the attribute
    u32 m_Value;                         // 0x60
};
ASSERT_OFFSET(ModifyAttributeJob, m_SessionId, 0x58);
ASSERT_SIZE(ModifyAttributeJob, 0x68);
} // namespace session
} // namespace pia
} // namespace nn
