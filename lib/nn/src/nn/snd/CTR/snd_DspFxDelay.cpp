#include "nn/snd/CTR/snd_DspFxDelay.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x004608CC | fefates:bytes [tier B]
void nn::snd::CTR::DspFxDelay::Attach(nn::snd::CTR::AuxBusId)
{
}

// 0x0046095C | fefates:bytes [tier B]
void nn::snd::CTR::DspFxDelay::Enable(bool)
{
}

// 0x004609E8 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxDelay::Finalize()
{
}

// 0x00460AB4 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxDelay::SetParam(const nn::snd::CTR::DspFxDelay::Param&)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
