#pragma once

#include "decomp.h"
#include "nn/nex/nex_JobProcessProtocolEvent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex25JobProcessProtocolMessageE @ 0x008CEEA0
// vtable 0x008FE9E4 (vptr 0x008FE9EC), offset_to_top 0, 13 entries
class JobProcessProtocolMessage : public ::nn::nex::JobProcessProtocolEvent
{
public:
    JobProcessProtocolMessage(); // ctor candidate(s) 0x003B6FBC (unverified)
    virtual ~JobProcessProtocolMessage(); // 0x003B712C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003B708C slot 0x04 | fefates:bytes (deleting dtor)
    virtual void Execute(); // 0x003B6F70 slot 0x0C | fefates:bytes
    virtual void vf_0x30(); // 0x003B6F6C slot 0x30 | virtual slot, introduced by nn::nex::JobProcessProtocolEvent
};
} // namespace nex
} // namespace nn
