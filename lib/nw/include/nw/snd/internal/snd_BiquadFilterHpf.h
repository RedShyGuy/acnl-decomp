#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal15BiquadFilterHpfE @ 0x008D09CC
// vtable 0x0090315C (vptr 0x00903164), offset_to_top 0, 3 entries
class BiquadFilterHpf : public ::nw::snd::BiquadFilterCallback
{
public:
    BiquadFilterHpf(); // ctor address unknown
    virtual void vf_0x00(); // 0x004C82B4 slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterHpf
    virtual void vf_0x04(); // 0x004C82B0 slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterHpf
    virtual void GetCoefficients(int, float, nn::snd::CTR::BiquadFilterCoefficients*) const; // 0x0073F990 slot 0x08 | nintendogs:bytes
};
} // namespace internal
} // namespace snd
} // namespace nw
