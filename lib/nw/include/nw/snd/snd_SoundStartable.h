#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd14SoundStartableE @ 0x008D08F4
class SoundStartable
{
public:
    struct StartInfo { // TODO: real type unknown (placeholder)
        u32 _unknown;
        struct SeqSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
        struct StartOffsetType { u32 _unknown; }; // TODO: real type unknown (placeholder)
        struct StreamSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
        struct WaveSoundInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    };
    SoundStartable(); // ctor address unknown
    void StartSound(nw::snd::SoundHandle*, const char*, const nw::snd::SoundStartable::StartInfo*); // 0x004C0384 | fefates:bytes [tier B]
    void StartSound(nw::snd::SoundHandle*, unsigned, const nw::snd::SoundStartable::StartInfo*); // 0x004C03F8 | nintendogs:bytes [tier A]
    void PrepareSound(nw::snd::SoundHandle*, unsigned, const nw::snd::SoundStartable::StartInfo*); // 0x004C0438 | nintendogs:bytes [tier A]
};
} // namespace snd
} // namespace nw
