#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal18BiquadFilterBpf512E @ 0x008D09FC
// vtable 0x00903244 (vptr 0x0090324C), offset_to_top 0, 3 entries
class BiquadFilterBpf512 : public ::nw::snd::BiquadFilterCallback
{
public:
    BiquadFilterBpf512(); // ctor address unknown
    virtual void vf_0x00(); // 0x004C85D0 slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf512
    virtual void vf_0x04(); // 0x004C85CC slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf512
    virtual void GetCoefficients(int, float, nn::snd::CTR::BiquadFilterCoefficients*) const; // 0x00740568 slot 0x08 | nintendogs:bytes
};
} // namespace internal
} // namespace snd
} // namespace nw
