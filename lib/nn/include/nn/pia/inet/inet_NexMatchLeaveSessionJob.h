#pragma once

#include "decomp.h"
#include "nn/pia/session/session_LeaveSessionJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet23NexMatchLeaveSessionJobE @ 0x008CF9B4
// vtable 0x00900490 (vptr 0x00900498), offset_to_top 0, 16 entries
class NexMatchLeaveSessionJob : public ::nn::pia::session::LeaveSessionJob
{
public:
    NexMatchLeaveSessionJob(); // ctor candidate(s) 0x00404A54 (unverified)
    virtual ~NexMatchLeaveSessionJob(); // 0x00434238 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00404AB4 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x0072F528 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x0040498C slot 0x18 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x1C(); // 0x004042A4 slot 0x1C | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x20(); // 0x00404840 slot 0x20 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x24(); // 0x00404104 slot 0x24 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x2C(); // 0x00404818 slot 0x2C | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x30(); // 0x004047B8 slot 0x30 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x34(); // 0x0072F51C slot 0x34 | virtual slot, introduced by nn::pia::session::LeaveSessionJob
    virtual void vf_0x3C(); // 0x00134CE4 slot 0x3C | virtual slot, introduced by nn::pia::session::LeaveSessionJob
};
} // namespace inet
} // namespace pia
} // namespace nn
