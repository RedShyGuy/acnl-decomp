#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// A job that waits until its test gives the result of the call of the caller. armlink kept only
// its step (through the shared step table 0x8998E8; no constructor, vtable or RTTI); the class
// name and the names of the members and the functions are ours.
class JobWaitForResult : public ::nn::nex::StepSequenceJob
{
public:
    // true when the result is known (it goes to *pResult)
    virtual bool IsCompleted(nn::nex::qResult* pResult) = 0; // slot 0x34

    void WaitForResult(); // 0x0036D7B0
    // (inline) the call of the caller ends with the result
    void CompleteJob(nn::nex::qResult result);

    u32 m_CallContextId; // 0x68 (the call of the caller)
};
ASSERT_OFFSET(JobWaitForResult, m_CallContextId, 0x68);
} // namespace nex
} // namespace nn
