#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20JobIndependentServerE @ 0x008CE8B0
// vtable 0x008FD848 (vptr 0x008FD850), offset_to_top 0, 13 entries
class JobIndependentServer : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobIndependentServer(); // ctor address unknown
    virtual ~JobIndependentServer(); // 0x00397440 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003973FC slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
};
} // namespace nex
} // namespace nn
