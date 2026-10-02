#include "nw/snd/snd_SoundStartable.h"
#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/snd_SoundArchivePlayer.h"

namespace nw {
namespace snd {
// 0x004C393C slot 0x00 | fefates:bytes
nw::snd::SoundArchivePlayer::~SoundArchivePlayer()
{
}

// 0x004C3914 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchivePlayer
void nw::snd::SoundArchivePlayer::vf_0x04()
{
}

// 0x004C1038 slot 0x08 | nintendogs:bytes
void nw::snd::SoundArchivePlayer::detail_SetupSound(nw::snd::SoundHandle*, unsigned, bool, const nw::snd::SoundStartable::StartInfo*)
{
}

// 0x0073EDA4 slot 0x0C | slot vf_0x0C of nw::snd::SoundArchivePlayer
void nw::snd::SoundArchivePlayer::detail_GetItemId(const char*)
{
}

// 0x001323C4 | fefates:bytes [tier B]
nw::snd::SoundArchivePlayer::SoundArchivePlayer()
{
}

// 0x004C0B04 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::Initialize(const nw::snd::SoundArchivePlayer::InitializeParam&)
{
}

// 0x004C0C80 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::GetSoundPlayer(unsigned int)
{
}

// 0x004C0C98 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::SetupWaveSound(int, void**, const void*)
{
}

// 0x004C0E8C | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::SetupSoundPlayer(const nw::snd::SoundArchive*, void**, const void*)
{
}

// 0x004C1068 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::SetupSequenceSound(int, void**, const void*)
{
}

// 0x004C1294 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::PrepareWaveSoundImpl(nw::snd::internal::WaveSound*, const nw::snd::SoundArchive::SoundInfo*, const nw::snd::SoundArchive::WaveSoundInfo*, nw::snd::SoundStartable::StartInfo::StartOffsetType, int, const nw::snd::SoundStartable::StartInfo::WaveSoundInfo*)
{
}

// 0x004C14F8 | nintendogs:callgraph [tier A]
void nw::snd::SoundArchivePlayer::detail_SetupSoundImpl(nw::snd::SoundHandle*, unsigned, nw::snd::internal::BasicSound::AmbientInfo*, nw::snd::SoundActor*, bool, const nw::snd::SoundStartable::StartInfo*)
{
}

// 0x004C24BC | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::PrepareStreamSoundImpl(nw::snd::internal::StreamSound*, const nw::snd::SoundArchive::SoundInfo*, const nw::snd::SoundArchive::StreamSoundInfo*, const nw::snd::SoundArchive::StreamSoundInfo2*, nw::snd::SoundStartable::StartInfo::StartOffsetType, int, const nw::snd::SoundStartable::StartInfo::StreamSoundInfo*)
{
}

// 0x004C2B80 | nintendogs:callseq [tier A]
void nw::snd::SoundArchivePlayer::PrepareSequenceSoundImpl(nw::snd::internal::SequenceSound*, const nw::snd::SoundArchive::SoundInfo*, const nw::snd::SoundArchive::SequenceSoundInfo*, nw::snd::SoundStartable::StartInfo::StartOffsetType, int, const nw::snd::SoundStartable::StartInfo::SeqSoundInfo*)
{
}

// 0x004C30F0 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::SetupUserParamForBasicSound(void**, const void*, unsigned int)
{
}

// 0x004C31C4 | nintendogs:callseq [tier A]
void nw::snd::SoundArchivePlayer::Update()
{
}

// 0x004C3488 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::Finalize()
{
}

// 0x004C3744 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::SetupMram(const nw::snd::SoundArchive*, void*, unsigned int, unsigned int)
{
}

// 0x0073F124 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::GetRequiredMemSize(const nw::snd::SoundArchive*, unsigned int) const
{
}

// 0x0073F288 | fefates:bytes [tier B]
void nw::snd::SoundArchivePlayer::GetRequiredStreamCacheSize(const nw::snd::SoundArchive*, unsigned int) const
{
}

// 0x0073F2B8 | nintendogs:bytes [tier A]
void nw::snd::SoundArchivePlayer::GetRequiredStreamBufferSize(const nw::snd::SoundArchive*)
{
}

} // namespace snd
} // namespace nw
