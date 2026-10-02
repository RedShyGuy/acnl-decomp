#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex22JobWaitForNotificationE @ 0x008CEC24
// vtable 0x008FE114 (vptr 0x008FE11C), offset_to_top 0, 14 entries
class JobWaitForNotification : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    JobWaitForNotification(); // ctor address unknown
    virtual ~JobWaitForNotification(); // 0x0039D4FC slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0039D44C slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x34(); // 0x0039CF1C slot 0x34 | virtual slot, introduced by nn::nex::JobWaitForNotification
};
} // namespace nex
} // namespace nn
