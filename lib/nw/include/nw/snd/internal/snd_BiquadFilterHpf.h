#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal15BiquadFilterHpfE @ 0x008D09CC
// vtable 0x0090315C (vptr 0x00903164), offset_to_top 0, 3 entries
// The built-in Hpf filter (the class name is from the reference symbols): coefficients from a
// table, high-pass: the cutoff from value 0 (open) to 1.
class BiquadFilterHpf : public ::nw::snd::BiquadFilterCallback
{
public:
    // (inline)
    BiquadFilterHpf() {}
    virtual ~BiquadFilterHpf();

    virtual void GetCoefficients(int type, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const;
};
} // namespace internal
} // namespace snd
} // namespace nw
