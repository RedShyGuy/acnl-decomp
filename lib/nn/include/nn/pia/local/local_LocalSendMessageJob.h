#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace local {
// RTTI N2nn3pia5local19LocalSendMessageJobE @ 0x008CFBBC
// vtable 0x00900CFC (vptr 0x00900D04), offset_to_top 0, 6 entries
class LocalSendMessageJob : public ::nn::pia::common::StepSequenceJob
{
public:
    LocalSendMessageJob(); // ctor candidate(s) 0x0041A660 (unverified)
    virtual ~LocalSendMessageJob(); // 0x0041A750 slot 0x00 | slot vf_0x00 of nn::pia::common::Job
    // 0x0041A6F8 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void Trace(unsigned long long) const; // 0x007311D0 slot 0x14 | slot vf_0x14 of nn::pia::common::StepSequenceJob
    void ReceiveAck(unsigned char, unsigned int); // 0x0041A404 | fefates:bytes [tier B]
    void Startup(); // 0x0041A5B4 | fefates:bytes [tier B]
};
} // namespace local
} // namespace pia
} // namespace nn
