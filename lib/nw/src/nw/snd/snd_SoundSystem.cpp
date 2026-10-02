#include "nw/snd/snd_SoundSystem.h"

namespace nw {
namespace snd {
// 0x001417FC | nintendogs:callgraph [tier A]
void nw::snd::SoundSystem::ClearEffect(nw::snd::AuxBus, int)
{
}

// 0x004BFBEC | mk7dlp:callseq [tier A]
void nw::snd::SoundSystem::Initialize(const nw::snd::SoundSystem::SoundSystemParam&, unsigned, unsigned)
{
}

// 0x004BFF34 | fefates:bytes [tier B]
void nw::snd::SoundSystem::AppendEffect(nw::snd::AuxBus, nn::snd::CTR::FxDelay*)
{
}

// 0x004BFF7C | fefates:bytes [tier B]
void nw::snd::SoundSystem::AppendEffect(nw::snd::AuxBus, nn::snd::CTR::FxReverb*)
{
}

// 0x004C0034 | nintendogs:callseq [tier A]
void nw::snd::SoundSystem::GetRequiredMemSize(const nw::snd::SoundSystem::SoundSystemParam&)
{
}

// 0x004C0128 | nintendogs:callseq [tier A]
void nw::snd::SoundSystem::Finalize()
{
}

} // namespace snd
} // namespace nw
