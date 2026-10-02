#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex11PeriodicJobE @ 0x008CE068
// vtable 0x008FC3E8 (vptr 0x008FC3F0), offset_to_top 0, 12 entries
class PeriodicJob : public ::nn::nex::Job
{
public:
    PeriodicJob(); // ctor address unknown
    virtual ~PeriodicJob(); // 0x0035B67C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0035B650 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void SetDefaultPostExecutionState(); // 0x003CC720 slot 0x1C | fefates:bytes
    virtual void SkipWaitDelayAtTermination(); // 0x0035B63C slot 0x20 | fefates:bytes
};
} // namespace nex
} // namespace nn
