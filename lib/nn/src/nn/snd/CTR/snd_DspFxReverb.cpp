#include "nn/snd/CTR/snd_DspFxReverb.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00460E64 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxReverb::Attach(nn::snd::CTR::AuxBusId)
{
}

// 0x00460EBC | fefates:bytes [tier B]
void nn::snd::CTR::DspFxReverb::Enable(bool)
{
}

// 0x00460F48 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxReverb::Finalize()
{
}

// 0x00461014 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxReverb::SetParam(const nn::snd::CTR::DspFxReverb::Param&)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
