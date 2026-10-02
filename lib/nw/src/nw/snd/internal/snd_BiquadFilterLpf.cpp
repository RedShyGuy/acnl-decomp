#include "nw/snd/snd_BiquadFilterCallback.h"
#include "nw/snd/internal/snd_BiquadFilterLpf.h"

namespace nw {
namespace snd {
namespace internal {
// ctor address unknown
nw::snd::internal::BiquadFilterLpf::BiquadFilterLpf()
{
}

// 0x004C82BC slot 0x00 | virtual slot, introduced by nw::snd::internal::BiquadFilterLpf
void nw::snd::internal::BiquadFilterLpf::vf_0x00()
{
}

// 0x004C82B8 slot 0x04 | virtual slot, introduced by nw::snd::internal::BiquadFilterLpf
void nw::snd::internal::BiquadFilterLpf::vf_0x04()
{
}

// 0x0073F9E4 slot 0x08 | nintendogs:bytes
void nw::snd::internal::BiquadFilterLpf::GetCoefficients(int, float, nn::snd::CTR::BiquadFilterCoefficients*) const
{
}

} // namespace internal
} // namespace snd
} // namespace nw
