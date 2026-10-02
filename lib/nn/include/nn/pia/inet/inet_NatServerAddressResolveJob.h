#pragma once

#include "decomp.h"
#include "nn/pia/common/common_StepSequenceJob.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet26NatServerAddressResolveJobE @ 0x008CFA08
// vtable 0x009005E0 (vptr 0x009005E8), offset_to_top 0, 6 entries
class NatServerAddressResolveJob : public ::nn::pia::common::StepSequenceJob
{
public:
    virtual ~NatServerAddressResolveJob(); // 0x0040C648 slot 0x00 | fefates:callgraph
    // 0x0040C638 slot 0x04 | slot vf_0x04 of nn::pia::common::Job (deleting dtor)
    virtual void CancelCleanup(); // 0x0040C524 slot 0x10 | fefates:bytes
    void StepComplete(); // 0x0040C4A0 | fefates:bytes [tier B]
    NatServerAddressResolveJob(); // 0x0040C5FC | fefates:bytes [tier B]
};
} // namespace inet
} // namespace pia
} // namespace nn
