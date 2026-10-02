#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24JobRetrieveGameAuthTokenE @ 0x008CEDD0
// vtable 0x008FE82C (vptr 0x008FE834), offset_to_top 0, 13 entries
class JobRetrieveGameAuthToken : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobRetrieveGameAuthToken(); // ctor candidate(s) 0x003BD2EC (unverified)
    virtual ~JobRetrieveGameAuthToken(); // 0x003B47DC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B47B8 slot 0x04 | fefates:callseq (deleting dtor)
    void CompleteJob(const nn::nex::qResult&); // 0x003B45B8 | fefates:bytes [tier B]
    void StepDeriveKey(); // 0x003B465C | fefates:bytes [tier B]
    void StepDummyToComplete(); // 0x003B473C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
