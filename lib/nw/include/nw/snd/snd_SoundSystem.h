#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
class SoundSystem
{
public:
    struct SoundSystemParam { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void ClearEffect(nw::snd::AuxBus, int); // 0x001417FC | nintendogs:callgraph [tier A]
    void Initialize(const nw::snd::SoundSystem::SoundSystemParam&, unsigned, unsigned); // 0x004BFBEC | mk7dlp:callseq [tier A]
    void AppendEffect(nw::snd::AuxBus, nn::snd::CTR::FxDelay*); // 0x004BFF34 | fefates:bytes [tier B]
    void AppendEffect(nw::snd::AuxBus, nn::snd::CTR::FxReverb*); // 0x004BFF7C | fefates:bytes [tier B]
    void GetRequiredMemSize(const nw::snd::SoundSystem::SoundSystemParam&); // 0x004C0034 | nintendogs:callseq [tier A]
    void Finalize(); // 0x004C0128 | nintendogs:callseq [tier A]
};
} // namespace snd
} // namespace nw
