#include "nw/snd/snd_StreamSoundHandle.h"

namespace nw {
namespace snd {
// TODO: default ctor added so derived stubs compile - may not exist
nw::snd::StreamSoundHandle::StreamSoundHandle()
{
}

// 0x004C08BC | fefates:bytes [tier B]
void nw::snd::StreamSoundHandle::DetachSound()
{
}

// 0x004C08F4 | nintendogs:bytes-fuzzy [tier A]
nw::snd::StreamSoundHandle::StreamSoundHandle(nw::snd::SoundHandle*)
{
}

} // namespace snd
} // namespace nw
