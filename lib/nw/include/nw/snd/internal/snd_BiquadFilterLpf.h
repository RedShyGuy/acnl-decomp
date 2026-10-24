#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal15BiquadFilterLpfE @ 0x008D09D8
// vtable 0x00903170 (vptr 0x00903178), offset_to_top 0, 3 entries
// The built-in Lpf filter (the class name is from the reference symbols): coefficients from a
// table, low-pass: the cutoff from value 0 (closed) to 1 (open).
class BiquadFilterLpf : public ::nw::snd::BiquadFilterCallback
{
public:
    // (inline)
    BiquadFilterLpf() {}
    virtual ~BiquadFilterLpf();

    virtual void GetCoefficients(int type, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const;
};
} // namespace internal
} // namespace snd
} // namespace nw
