#include "nn/snd/CTR/snd_VoiceManager.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00130820 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceManager::Initialize()
{
}

// 0x00461690 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceManager::SetPriority(nn::snd::CTR::Voice*, int)
{
}

// 0x004617D4 | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceManager::AdjustVoicePlayState(int, int)
{
}

// 0x00461980 | nintendogs:callgraph [tier A]
void nn::snd::CTR::VoiceManager::Finalize()
{
}

// 0x0046198C | nintendogs:bytes [tier A]
void nn::snd::CTR::VoiceManager::FreeVoice(nn::snd::CTR::Voice*)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
