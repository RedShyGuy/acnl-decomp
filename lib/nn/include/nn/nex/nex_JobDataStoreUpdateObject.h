#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex24JobDataStoreUpdateObjectE @ 0x008CEDB0
// vtable 0x008FE7EC (vptr 0x008FE7F4), offset_to_top 0, 14 entries
class JobDataStoreUpdateObject : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    virtual ~JobDataStoreUpdateObject(); // 0x003B4378 slot 0x00 | fefates:bytes
    // 0x003B4368 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x34(); // 0x003B3980 slot 0x34 | virtual slot, introduced by nn::nex::JobDataStoreUpdateObject
    void CompleteJob(const nn::nex::qResult&); // 0x003B3B08 | fefates:bytes [tier B]
    void StepFileServerPostObject(); // 0x003B3B88 | fefates:bytes [tier B]
    void StepLogicServerPrepareUpdateObject(); // 0x003B3EA4 | fefates:bytes [tier B]
    void StepLogicServerCompleteUpdateObject(); // 0x003B3FB8 | fefates:bytes [tier B]
    void StepWaitingLogicServerCompleteUpdateObject(); // 0x003B4130 | fefates:bytes [tier B]
    JobDataStoreUpdateObject(); // 0x003B4204 | mk7dlp:callseq [tier A]
};
} // namespace nex
} // namespace nn
