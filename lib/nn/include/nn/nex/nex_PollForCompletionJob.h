#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qResult.h"

namespace nn {
namespace nex {
// A job that polls a function until it reports the completion (with success or not) or the
// deadline passes. armlink kept only its steps (no constructor, no vtable, no RTTI); the class name
// is from the fefates symbols, the names of the members and of the step are ours.
class PollForCompletionJob : public ::nn::nex::StepSequenceJob
{
public:
    typedef bool (*TestFunction)(void* pArgument);

    void CompleteJob(nn::nex::qResult result); // 0x00397A04 | fefates:bytes [tier B]
    void Poll(); // 0x00397A68

    TestFunction m_pIsCompleted; // 0x68
    TestFunction m_pIsSucceeded; // 0x6C
    void* m_pArgument;           // 0x70
    u32 m_CallContextId;         // 0x74
    Time m_Deadline;             // 0x78
};
ASSERT_OFFSET(PollForCompletionJob, m_Deadline, 0x78);
} // namespace nex
} // namespace nn
