#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session34GenerateMatchmakeSystemPasswordJobE @ 0x008D0134
// vtable 0x00901D00 (vptr 0x00901D08), offset_to_top 0, 8 entries
//
// The network (inet) makes a system password for the matchmake session in its steps and ends
// with FailureProcess / CompleteProcess.
class GenerateMatchmakeSystemPasswordJob : public ::nn::pia::common::StepSequenceJob
{
public:
    GenerateMatchmakeSystemPasswordJob(); // 0x00447B38
    virtual ~GenerateMatchmakeSystemPasswordJob(); // 0x00447B94 slot 0x00
    // 0x00447B6C slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734214 slot 0x14
    virtual void Cleanup(); // 0x00447AE0 slot 0x18
    // the network sets its first step
    virtual nn::Result vf_0x1C() = 0; // slot 0x1C

    // the last steps (the network fails the call context itself)
    common::ExecuteResult FailureProcess(); // 0x00447A50
    common::ExecuteResult CompleteProcess(); // 0x00447A98

    u32 m_SessionId;                     // 0x40
    common::CallContext* m_pCallContext; // 0x44, of Session
    common::CallContext m_CallContext;   // 0x48, of the network
    wchar_t* m_pUnknown0x5C;             // 0x5C, the password of the network
};
ASSERT_OFFSET(GenerateMatchmakeSystemPasswordJob, m_pUnknown0x5C, 0x5C);
ASSERT_SIZE(GenerateMatchmakeSystemPasswordJob, 0x60);
} // namespace session
} // namespace pia
} // namespace nn
