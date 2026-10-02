#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal15BiquadFilterLpfE @ 0x008D09D8
// vtable 0x00903170 (vptr 0x00903178), offset_to_top 0, 3 entries
class BiquadFilterLpf : public ::nw::snd::BiquadFilterCallback
{
public:
    BiquadFilterLpf(); // ctor address unknown
    virtual void vf_0x00(); // 0x004C82BC slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterLpf
    virtual void vf_0x04(); // 0x004C82B8 slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterLpf
    virtual void GetCoefficients(int, float, nn::snd::CTR::BiquadFilterCoefficients*) const; // 0x0073F9E4 slot 0x08 | nintendogs:bytes
};
} // namespace internal
} // namespace snd
} // namespace nw
