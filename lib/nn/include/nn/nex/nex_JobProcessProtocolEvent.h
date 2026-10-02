#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23JobProcessProtocolEventE @ 0x008CECF4
// vtable 0x008FE570 (vptr 0x008FE578), offset_to_top 0, 13 entries
class JobProcessProtocolEvent : public ::nn::nex::Job
{
public:
    JobProcessProtocolEvent(); // ctor address unknown
    virtual ~JobProcessProtocolEvent(); // 0x003B113C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B1100 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void vf_0x30(); // 0x003B10FC slot 0x30 | virtual slot, introduced by nn::nex::JobProcessProtocolEvent
};
} // namespace nex
} // namespace nn
