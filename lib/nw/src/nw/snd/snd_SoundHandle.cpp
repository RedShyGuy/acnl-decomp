#include "nw/snd/snd_SoundHandle.h"

namespace nw {
namespace snd {
// ctor address unknown
nw::snd::SoundHandle::SoundHandle()
{
}

// 0x00137B30 | nintendogs:bytes [tier A]
void nw::snd::SoundHandle::DetachSound()
{
}

// 0x004BF660 | nintendogs:bytes [tier A]
void nw::snd::SoundHandle::detail_AttachSound(nw::snd::internal::BasicSound*)
{
}

// 0x004BF68C | nintendogs:bytes [tier A]
void nw::snd::SoundHandle::detail_AttachSoundAsTempHandle(nw::snd::internal::BasicSound*)
{
}

} // namespace snd
} // namespace nw
