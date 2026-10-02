#pragma once

#include "decomp.h"
#include "nn/nex/nex_InstanceControl.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15PseudoSingletonE @ 0x008CE4CC
// vtable 0x008FCF04 (vptr 0x008FCF0C), offset_to_top 0, 2 entries
class PseudoSingleton : public ::nn::nex::InstanceControl
{
public:
    PseudoSingleton(); // ctor candidate(s) 0x0037AF48 (unverified)
    virtual ~PseudoSingleton(); // 0x00378640 slot 0x00 | slot vf_0x00 of nn::nex::InstanceControl
    // 0x0037AF94 slot 0x04 | slot vf_0x04 of nn::nex::InstanceControl (deleting dtor)
};
} // namespace nex
} // namespace nn
