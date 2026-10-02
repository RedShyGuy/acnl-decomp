#include "nn/snd/CTR/snd_FxDelay.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00465874 slot 0x00 | fefates:bytes
nn::snd::CTR::FxDelay::~FxDelay()
{
}

// 0x004653F4 | nintendogs:bytes [tier A]
void nn::snd::CTR::FxDelay::Initialize()
{
}

// 0x0046549C | nintendogs:bytes [tier A]
void nn::snd::CTR::FxDelay::UpdateBuffer(unsigned)
{
}

// 0x00465594 | nintendogs:bytes [tier A]
void nn::snd::CTR::FxDelay::AssignWorkBuffer(unsigned, unsigned)
{
}

// 0x004655C8 | nintendogs:bytes [tier A]
void nn::snd::CTR::FxDelay::GetRequiredMemSize()
{
}

// 0x004655E4 | nintendogs:bytes [tier A]
void nn::snd::CTR::FxDelay::Finalize()
{
}

// 0x00465638 | nintendogs:bytes [tier A]
void nn::snd::CTR::FxDelay::SetParam(const nn::snd::CTR::FxDelay::Param&)
{
}

// 0x00465768 | nintendogs:bytes [tier A]
nn::snd::CTR::FxDelay::FxDelay()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
