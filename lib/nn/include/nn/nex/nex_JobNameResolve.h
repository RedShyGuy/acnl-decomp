#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14JobNameResolveE @ 0x008CE31C
// vtable 0x008FCB74 (vptr 0x008FCB7C), offset_to_top 0, 13 entries
class JobNameResolve : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobNameResolve(); // ctor address unknown
    virtual ~JobNameResolve(); // 0x00371710 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003716E0 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void CompleteJob(const nn::nex::qResult&); // 0x0037117C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
