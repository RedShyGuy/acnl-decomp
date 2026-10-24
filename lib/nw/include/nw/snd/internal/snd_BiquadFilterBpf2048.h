#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal19BiquadFilterBpf2048E @ 0x008D0A28
// vtable 0x009032A8 (vptr 0x009032B0), offset_to_top 0, 3 entries
// The built-in Bpf2048 filter (the class name is from the reference symbols): coefficients from a
// table, band-pass around 2048 Hz: the width from value 0 to 1.
class BiquadFilterBpf2048 : public ::nw::snd::BiquadFilterCallback
{
public:
    // (inline)
    BiquadFilterBpf2048() {}
    virtual ~BiquadFilterBpf2048();

    virtual void GetCoefficients(int type, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const;
};
} // namespace internal
} // namespace snd
} // namespace nw
