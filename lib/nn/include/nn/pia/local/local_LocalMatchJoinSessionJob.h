#pragma once

#include "decomp.h"
#include "nn/pia/session/session_JoinSessionJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local24LocalMatchJoinSessionJobE @ 0x008CFC88
// vtable 0x0090106C (vptr 0x00901074), offset_to_top 0, 17 entries
class LocalMatchJoinSessionJob : public ::nn::pia::session::JoinSessionJob
{
public:
    LocalMatchJoinSessionJob(); // ctor candidate(s) 0x00420028 (unverified)
    virtual ~LocalMatchJoinSessionJob(); // 0x00420058 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00420048 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00731698 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x30(); // 0x0041F7E0 slot 0x30 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x38(); // 0x0041FFC0 slot 0x38 | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x3C(); // 0x0041FF54 slot 0x3C | virtual slot, introduced by nn::pia::session::JoinSessionJob
    virtual void vf_0x40(); // 0x0041FEEC slot 0x40 | virtual slot, introduced by nn::pia::session::JoinSessionJob
};
} // namespace local
} // namespace pia
} // namespace nn
