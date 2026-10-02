#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex7JobHttpE @ 0x008CF654
// vtable 0x008FFB18 (vptr 0x008FFB20), offset_to_top 0, 14 entries
class JobHttp : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobHttp(); // ctor address unknown
    virtual ~JobHttp(); // 0x003D227C slot 0x00 | fefates:bytes-fuzzy
    // 0x003D224C slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x34(); // 0x003D1860 slot 0x34 | fefates:callseq
    void CompleteJob(const nn::nex::qResult&); // 0x003D1930 | fefates:bytes-fuzzy [tier B]
};
} // namespace nex
} // namespace nn
