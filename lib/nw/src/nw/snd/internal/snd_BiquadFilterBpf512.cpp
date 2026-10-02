#include "nw/snd/snd_BiquadFilterCallback.h"
#include "nw/snd/internal/snd_BiquadFilterBpf512.h"

namespace nw {
namespace snd {
namespace internal {
// ctor address unknown
nw::snd::internal::BiquadFilterBpf512::BiquadFilterBpf512()
{
}

// 0x004C85D0 slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf512
void nw::snd::internal::BiquadFilterBpf512::vf_0x00()
{
}

// 0x004C85CC slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterBpf512
void nw::snd::internal::BiquadFilterBpf512::vf_0x04()
{
}

// 0x00740568 slot 0x08 | nintendogs:bytes
void nw::snd::internal::BiquadFilterBpf512::GetCoefficients(int, float, nn::snd::CTR::BiquadFilterCoefficients*) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
