#include "nn/snd/CTR/snd_DspFxManager.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x0046143C | nintendogs:callgraph [tier A]
void nn::snd::CTR::DspFxManager::GetInstance()
{
}

// 0x00461448 | nintendogs:bytes [tier A]
void nn::snd::CTR::DspFxManager::GetDspCycles()
{
}

// 0x004614BC | fefates:bytes [tier B]
void nn::snd::CTR::DspFxManager::Attach(nn::snd::CTR::DspFxManager::DspEffectType, nn::snd::CTR::AuxBusId)
{
}

// 0x004614DC | fefates:bytes [tier B]
void nn::snd::CTR::DspFxManager::Detach(nn::snd::CTR::DspFxManager::DspEffectType, nn::snd::CTR::AuxBusId)
{
}

// 0x00462A2C | nintendogs:bytes [tier A]
void nn::snd::CTR::DspFxManager::Initialize()
{
}

// 0x00462C48 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxManager::SetDspDelayEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxDelayParams&)
{
}

// 0x00462CE4 | fefates:bytes [tier B]
void nn::snd::CTR::DspFxManager::SetDspReverbEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::DspFxReverbParams&)
{
}

// 0x00462DE8 | nintendogs:callgraph [tier A]
void nn::snd::CTR::DspFxManager::Finalize()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
