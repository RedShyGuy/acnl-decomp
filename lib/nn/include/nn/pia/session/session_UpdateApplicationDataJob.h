#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_CallContext.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session24UpdateApplicationDataJobE @ 0x008D00F8
// vtable 0x00901C28 (vptr 0x00901C30), offset_to_top 0, 8 entries
//
// The network (inet / local) changes the application data of the matchmake session in its
// steps and ends with FailureProcess / CompleteProcess.
class UpdateApplicationDataJob : public ::nn::pia::common::StepSequenceJob
{
public:
    UpdateApplicationDataJob(); // 0x00442CD0
    virtual ~UpdateApplicationDataJob(); // 0x00442D20 slot 0x00
    // 0x00442CF8 slot 0x04 (deleting dtor)
    virtual void Trace(u64 flag) const; // 0x00734170 slot 0x14
    virtual void Cleanup(); // 0x00442C84 slot 0x18
    // the network takes the application data (and sets its first step)
    virtual nn::Result vf_0x1C(const void* pData, u32 size) = 0; // slot 0x1C

    // the last steps (the network fails the call context itself)
    common::ExecuteResult FailureProcess(); // 0x00442C0C
    common::ExecuteResult CompleteProcess(); // 0x00442C54

    common::CallContext* m_pCallContext; // 0x40, of Session
    common::CallContext m_CallContext;   // 0x44, of the network
    u32 m_SessionId;                     // 0x58
};
ASSERT_OFFSET(UpdateApplicationDataJob, m_SessionId, 0x58);
ASSERT_SIZE(UpdateApplicationDataJob, 0x60);
} // namespace session
} // namespace pia
} // namespace nn
