#pragma once

#include "decomp.h"
#include "nn/pia/session/session_DestroySessionJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local27LocalMatchDestroySessionJobE @ 0x008CFD18
// vtable 0x00901234 (vptr 0x0090123C), offset_to_top 0, 10 entries
class LocalMatchDestroySessionJob : public ::nn::pia::session::DestroySessionJob
{
public:
    LocalMatchDestroySessionJob(); // ctor address unknown
    virtual ~LocalMatchDestroySessionJob(); // 0x00422E94 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00422E84 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316C4 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00422E68 slot 0x18 | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x1C(); // 0x00422C44 slot 0x1C | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x20(); // 0x00422E20 slot 0x20 | virtual slot, introduced by nn::pia::session::DestroySessionJob
    virtual void vf_0x24(); // 0x00422DC4 slot 0x24 | virtual slot, introduced by nn::pia::session::DestroySessionJob
};
} // namespace local
} // namespace pia
} // namespace nn
