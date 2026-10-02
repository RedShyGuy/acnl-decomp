#include "nw/snd/snd_SoundStartable.h"

namespace nw {
namespace snd {
// ctor address unknown
nw::snd::SoundStartable::SoundStartable()
{
}

// 0x004C0384 | fefates:bytes [tier B]
void nw::snd::SoundStartable::StartSound(nw::snd::SoundHandle*, const char*, const nw::snd::SoundStartable::StartInfo*)
{
}

// 0x004C03F8 | nintendogs:bytes [tier A]
void nw::snd::SoundStartable::StartSound(nw::snd::SoundHandle*, unsigned, const nw::snd::SoundStartable::StartInfo*)
{
}

// 0x004C0438 | nintendogs:bytes [tier A]
void nw::snd::SoundStartable::PrepareSound(nw::snd::SoundHandle*, unsigned, const nw::snd::SoundStartable::StartInfo*)
{
}

} // namespace snd
} // namespace nw
