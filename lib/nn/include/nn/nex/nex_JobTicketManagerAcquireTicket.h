#pragma once

#include "decomp.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex29JobTicketManagerAcquireTicketE @ 0x008CF124
// vtable 0x008FF1D4 (vptr 0x008FF1DC), offset_to_top 0, 13 entries
class JobTicketManagerAcquireTicket : public ::nn::nex::StepSequenceJob
{
public:
    JobTicketManagerAcquireTicket(); // ctor address unknown
    virtual ~JobTicketManagerAcquireTicket(); // 0x003C0954 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003C0928 slot 0x04 | fefates:bytes (deleting dtor)
    void ProcessResponse(); // 0x003C06E0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
