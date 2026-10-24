#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd20BiquadFilterCallbackE @ 0x008D094C
// The coefficients of a biquad filter type for a value 0 to 1 (the class name is from the
// reference symbols, GetCoefficients from the derived classes).
class BiquadFilterCallback
{
public:
    // (inline)
    virtual ~BiquadFilterCallback() {}

    virtual void GetCoefficients(int type, f32 value, nn::snd::CTR::BiquadFilterCoefficients* pCoefficients) const = 0;
};
} // namespace snd
} // namespace nw
