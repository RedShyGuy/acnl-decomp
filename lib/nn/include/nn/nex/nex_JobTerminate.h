#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12JobTerminateE @ 0x008CE0E0
// vtable 0x008FC530 (vptr 0x008FC538), offset_to_top 0, 13 entries
class JobTerminate : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobTerminate(); // ctor address unknown
    virtual ~JobTerminate(); // 0x0035D4F8 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0035D4D4 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void StepWaitOnPendingJobs(); // 0x0035D428 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
