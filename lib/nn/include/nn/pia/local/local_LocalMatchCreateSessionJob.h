#pragma once

#include "decomp.h"
#include "nn/pia/session/session_CreateSessionJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local26LocalMatchCreateSessionJobE @ 0x008CFCDC
// vtable 0x009011A0 (vptr 0x009011A8), offset_to_top 0, 10 entries
class LocalMatchCreateSessionJob : public ::nn::pia::session::CreateSessionJob
{
public:
    LocalMatchCreateSessionJob(); // ctor candidate(s) 0x00421314 (unverified)
    virtual ~LocalMatchCreateSessionJob(); // 0x00421348 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00421338 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007316AC slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00438144 slot 0x18 | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x1C(); // 0x00420C24 slot 0x1C | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x20(); // 0x00421258 slot 0x20 | virtual slot, introduced by nn::pia::session::CreateSessionJob
    virtual void vf_0x24(); // 0x004212A4 slot 0x24 | virtual slot, introduced by nn::pia::session::CreateSessionJob
};
} // namespace local
} // namespace pia
} // namespace nn
