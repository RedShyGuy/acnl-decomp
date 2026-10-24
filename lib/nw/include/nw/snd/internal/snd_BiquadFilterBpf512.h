#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal18BiquadFilterBpf512E @ 0x008D09FC
// vtable 0x00903244 (vptr 0x0090324C), offset_to_top 0, 3 entries
// The built-in Bpf512 filter (the class name is from the reference symbols): coefficients from a
// table, band-pass around 512 Hz: the width from value 0 to 1.
class BiquadFilterBpf512 : public ::nw::snd::BiquadFilterCallback
{
public:
    // (inline)
    BiquadFilterBpf512() {}
    virtual ~BiquadFilterBpf512();

    virtual void GetCoefficients(int type, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const;
};
} // namespace internal
} // namespace snd
} // namespace nw
