#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal19BiquadFilterBpf1024E @ 0x008D0A1C
// vtable 0x00903294 (vptr 0x0090329C), offset_to_top 0, 3 entries
// The built-in Bpf1024 filter (the class name is from the reference symbols): coefficients from a
// table, band-pass around 1024 Hz: the width from value 0 to 1.
class BiquadFilterBpf1024 : public ::nw::snd::BiquadFilterCallback
{
public:
    // (inline)
    BiquadFilterBpf1024() {}
    virtual ~BiquadFilterBpf1024();

    virtual void GetCoefficients(int type, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const;
};
} // namespace internal
} // namespace snd
} // namespace nw
