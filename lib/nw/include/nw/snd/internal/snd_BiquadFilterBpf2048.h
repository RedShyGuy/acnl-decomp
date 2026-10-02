#pragma once

#include "decomp.h"
#include "nw/snd/snd_BiquadFilterCallback.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal19BiquadFilterBpf2048E @ 0x008D0A28
// vtable 0x009032A8 (vptr 0x009032B0), offset_to_top 0, 3 entries
class BiquadFilterBpf2048 : public ::nw::snd::BiquadFilterCallback
{
public:
    BiquadFilterBpf2048(); // ctor address unknown
    virtual void vf_0x00(); // 0x004C8D14 slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf2048
    virtual void vf_0x04(); // 0x004C8D10 slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf2048
    virtual void vf_0x08(); // 0x0074085C slot 0x08 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf2048
};
} // namespace internal
} // namespace snd
} // namespace nw
