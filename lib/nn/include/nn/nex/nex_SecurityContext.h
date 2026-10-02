#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15SecurityContextE @ 0x008CE4D8
// vtable 0x008FCF14 (vptr 0x008FCF1C), offset_to_top 0, 2 entries
class SecurityContext : public ::nn::nex::RootObject
{
public:
    SecurityContext(); // ctor candidate(s) 0x0039DF20, 0x003CF518, 0x0082F864 (unverified)
    virtual void vf_0x00(); // 0x0037AFD0 slot 0x00 | virtual slot, introduced by nn::nex::SecurityContext
    virtual void vf_0x04(); // 0x0037AFB8 slot 0x04 | virtual slot, introduced by nn::nex::SecurityContext
};
} // namespace nex
} // namespace nn
