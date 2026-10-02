#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex8JobLoginE @ 0x008CF6DC
// vtable 0x008FFC68 (vptr 0x008FFC70), offset_to_top 0, 14 entries
class JobLogin : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobLogin(); // ctor address unknown
    virtual ~JobLogin(); // 0x003D5104 slot 0x00 | fefates:bytes
    // 0x003D50F4 slot 0x04 | fefates:callseq (deleting dtor)
    virtual void StepFirst(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    void Complete(const nn::nex::qResult&); // 0x003D5064 | mk7dlp:callseq [tier A]
};
} // namespace nex
} // namespace nn
