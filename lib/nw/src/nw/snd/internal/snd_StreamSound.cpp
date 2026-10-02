#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/snd_StreamSound.h"

namespace nw {
namespace snd {
namespace internal {
// ctor candidate(s) 0x004C675C (unverified)
nw::snd::internal::StreamSound::StreamSound()
{
}

// 0x0073F380 slot 0x00 | slot vf_0x00 of nw::snd::internal::BasicSound
void nw::snd::internal::StreamSound::GetRuntimeTypeInfo() const
{
}

// 0x004C6818 slot 0x04 | fefates:bytes
nw::snd::internal::StreamSound::~StreamSound()
{
}

// 0x004C67D4 slot 0x08 | virtual slot, introduced by nw::snd::internal::BasicSound
void nw::snd::internal::StreamSound::vf_0x08()
{
}

// 0x004C6168 slot 0x0C | nintendogs:callseq
void nw::snd::internal::StreamSound::Initialize()
{
}

// 0x004C66EC slot 0x10 | fefates:bytes
void nw::snd::internal::StreamSound::Finalize()
{
}

// 0x0073F344 slot 0x14 | fefates:bytes
void nw::snd::internal::StreamSound::IsPrepared() const
{
}

// 0x004C660C slot 0x18 | fefates:bytes
void nw::snd::internal::StreamSound::IsAttachedTempSpecialHandle()
{
}

// 0x004C08B0 slot 0x1C | slot vf_0x1C of nw::snd::internal::BasicSound
void nw::snd::internal::StreamSound::DetachTempSpecialHandle()
{
}

// 0x004C6604 slot 0x20 | slot vf_0x20 of nw::snd::internal::BasicSound
void nw::snd::internal::StreamSound::GetBasicSoundPlayerHandle()
{
}

// 0x004C655C slot 0x24 | fefates:bytes
void nw::snd::internal::StreamSound::OnUpdatePlayerPriority()
{
}

// 0x004C644C slot 0x28 | fefates:bytes
void nw::snd::internal::StreamSound::UpdateMoveValue()
{
}

// 0x004C6278 slot 0x30 | mk7dlp:bytes-fuzzy
void nw::snd::internal::StreamSound::UpdateParam()
{
}

// 0x004C6340 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::StreamSound::SetTrackVolume(unsigned long, float, int)
{
}

// 0x004C63F4 | fefates:bytes [tier B]
void nw::snd::internal::StreamSound::PreparePrefetch(const void*, const nw::snd::internal::driver::StreamSoundPlayer::PrepareBaseArg&)
{
}

// 0x004C6620 | fefates:bytes [tier B]
void nw::snd::internal::StreamSound::Setup(const nw::snd::internal::driver::StreamSoundPlayer::SetupArg&)
{
}

// 0x004C6688 | fefates:bytes [tier B]
void nw::snd::internal::StreamSound::Prepare(nw::io::FileStream*, const nw::snd::internal::driver::StreamSoundPlayer::PrepareBaseArg&)
{
}

// 0x004C675C | fefates:bytes [tier B]
nw::snd::internal::StreamSound::StreamSound(nw::snd::internal::SoundInstanceManager<nw::snd::internal::StreamSound>&)
{
}

} // namespace internal
} // namespace snd
} // namespace nw
