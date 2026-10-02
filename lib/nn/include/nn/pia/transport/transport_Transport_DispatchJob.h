#pragma once

#include "decomp.h"
#include "nn/pia/common/common_Job.h"
#include "nn/pia/transport/transport_Transport.h"

// RTTI N2nn3pia9transport9Transport11DispatchJobE @ 0x008D02C0
// vtable 0x0090207C (vptr 0x00902084), offset_to_top 0, 4 entries
class nn::pia::transport::Transport::DispatchJob : public ::nn::pia::common::Job
{
public:
    DispatchJob(); // ctor address unknown
    virtual ~DispatchJob(); // 0x00428BA4 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0045FD94 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void ExecuteCore(); // 0x0045FD6C slot 0x0C | fefates:bytes
};
