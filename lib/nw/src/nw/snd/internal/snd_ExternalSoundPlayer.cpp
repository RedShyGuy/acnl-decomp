#include "nw/snd/internal/snd_ExternalSoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004BF6F4 | mk7dlp:bytes-fuzzy [tier B]
void nw::snd::internal::ExternalSoundPlayer::PauseAllSound(bool, int)
{
}

// 0x004C4F64 | fefates:bytes [tier B]
void nw::snd::internal::ExternalSoundPlayer::RemoveSound(nw::snd::internal::BasicSound*)
{
}

// 0x004C8D18 | fefates:bytes [tier B]
void nw::snd::internal::ExternalSoundPlayer::AppendSound(nw::snd::internal::BasicSound*)
{
}

// 0x004C8DE8 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::ExternalSoundPlayer::detail_CanPlaySound(int)
{
}

// 0x004C8E3C | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::ExternalSoundPlayer::GetLowestPrioritySound()
{
}

} // namespace internal
} // namespace snd
} // namespace nw
