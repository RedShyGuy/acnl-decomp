#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
class CallContext;

// A job that runs a call (Call) and passes its outcome to its slots. armlink kept only its steps
// (no constructor, no vtable, no RTTI); the class name is from the fefates symbols, the names of
// the members, of the slots and of the steps are ours.
class JobCallContextCallback : public ::nn::nex::StepSequenceJob
{
public:
    virtual void vf_0x34() = 0; // slot 0x34
    // starts the call of m_pContext
    virtual void Call() = 0; // slot 0x38
    virtual void OnSuccess(nn::nex::CallContext* pContext) = 0; // slot 0x3C
    virtual void OnFailure(nn::nex::CallContext* pContext) = 0; // slot 0x40
    virtual void OnCancel() = 0; // slot 0x44

    void ProcessCallResult(); // 0x0039C274
    void CompleteJob(const nn::nex::qResult& result); // 0x0039C2F8 | fefates:bytes [tier B]
    // waits for the end of a call in progress, then calls
    void WaitForCall(); // 0x0039C3B8
    void StartCall(); // 0x0039C4C8

    u32 m_CallContextId;     // 0x68 (the call of the caller)
    u32 m_Unknown0x6C;       // 0x6C
    CallContext* m_pContext; // 0x70 (the call of the job)
};
ASSERT_OFFSET(JobCallContextCallback, m_pContext, 0x70);
} // namespace nex
} // namespace nn
