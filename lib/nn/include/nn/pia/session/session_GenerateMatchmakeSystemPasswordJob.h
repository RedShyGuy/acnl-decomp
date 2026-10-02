#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace session {
// RTTI N2nn3pia7session34GenerateMatchmakeSystemPasswordJobE @ 0x008D0134
// vtable 0x00901D00 (vptr 0x00901D08), offset_to_top 0, 8 entries
class GenerateMatchmakeSystemPasswordJob : public ::nn::pia::common::StepSequenceJob
{
public:
    GenerateMatchmakeSystemPasswordJob(); // ctor candidate(s) 0x00447B38 (unverified)
    virtual ~GenerateMatchmakeSystemPasswordJob(); // 0x00447B94 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x00447B6C slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x00734214 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    virtual void vf_0x18(); // 0x00447AE0 slot 0x18 | virtual slot, introduced by nn::pia::session::GenerateMatchmakeSystemPasswordJob
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
};
} // namespace session
} // namespace pia
} // namespace nn
