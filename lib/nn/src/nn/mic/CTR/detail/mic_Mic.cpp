#include "nn/mic/CTR/detail/mic_Mic.h"

namespace nn {
namespace mic {
namespace CTR {
namespace detail {
// 0x00131084 | nintendogs:bytes [tier B]
void nn::mic::CTR::detail::Mic::GetPGAB(unsigned char*)
{
}

// 0x00140AF0 | nintendogs:bytes [tier A]
void nn::mic::CTR::detail::Mic::FreeBuffer()
{
}

// 0x00140B20 | nintendogs:bytes [tier A]
void nn::mic::CTR::detail::Mic::IsSampling(bool*)
{
}

// 0x00140B60 | nintendogs:bytes [tier A]
void nn::mic::CTR::detail::Mic::SetMicBias(bool)
{
}

// 0x00140BA0 | nintendogs:bytes [tier A]
void nn::mic::CTR::detail::Mic::StopSampling()
{
}

// 0x00354A90 | nintendogs:bytes [tier A]
void nn::mic::CTR::detail::Mic::SetPGAB(unsigned char)
{
}

} // namespace detail
} // namespace CTR
} // namespace mic
} // namespace nn
