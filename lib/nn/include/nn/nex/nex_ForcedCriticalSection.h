#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21ForcedCriticalSectionE @ 0x008CEA90
// vtable 0x008FDCFC (vptr 0x008FDD04), offset_to_top 0, 5 entries
class ForcedCriticalSection : public ::nn::nex::CriticalSection
{
public:
    ForcedCriticalSection(); // ctor candidate(s) 0x002E6F5C, 0x0037E580, 0x00798FA4 (unverified)
    virtual void vf_0x00(); // 0x003986AC slot 0x00 | virtual slot, introduced by nn::nex::CriticalSection
    virtual void vf_0x04(); // 0x00398680 slot 0x04 | virtual slot, introduced by nn::nex::CriticalSection
    virtual void vf_0x08(); // 0x00398670 slot 0x08 | virtual slot, introduced by nn::nex::CriticalSection
    virtual void vf_0x0C(); // 0x00398678 slot 0x0C | virtual slot, introduced by nn::nex::CriticalSection
    virtual void vf_0x10(); // 0x0072CE04 slot 0x10 | virtual slot, introduced by nn::nex::CriticalSection
};
} // namespace nex
} // namespace nn
