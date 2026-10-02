#include "nw/snd/snd_BiquadFilterCallback.h"
#include "nw/snd/internal/snd_BiquadFilterHpf.h"

namespace nw {
namespace snd {
namespace internal {
// ctor address unknown
nw::snd::internal::BiquadFilterHpf::BiquadFilterHpf()
{
}

// 0x004C82B4 slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterHpf
void nw::snd::internal::BiquadFilterHpf::vf_0x00()
{
}

// 0x004C82B0 slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterHpf
void nw::snd::internal::BiquadFilterHpf::vf_0x04()
{
}

// 0x0073F990 slot 0x08 | nintendogs:bytes
void nw::snd::internal::BiquadFilterHpf::GetCoefficients(int, float, nn::snd::CTR::BiquadFilterCoefficients*) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
