#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal19BiquadFilterBpf1024E @ 0x008D0A1C
// vtable 0x00903294 (vptr 0x0090329C), offset_to_top 0, 3 entries
class BiquadFilterBpf1024 : public ::nw::snd::BiquadFilterCallback
{
public:
    BiquadFilterBpf1024(); // ctor address unknown
    virtual void vf_0x00(); // 0x004C8D0C slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf1024
    virtual void vf_0x04(); // 0x004C8D08 slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf1024
    virtual void vf_0x08(); // 0x007407F8 slot 0x08 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf1024
};
} // namespace internal
} // namespace snd
} // namespace nw
