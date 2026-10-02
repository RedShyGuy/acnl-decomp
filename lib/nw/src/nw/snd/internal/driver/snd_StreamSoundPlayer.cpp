#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D2324 slot 0x00 | fefates:bytes
nw::snd::internal::driver::StreamSoundPlayer::~StreamSoundPlayer()
{
}

// 0x004D05D0 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Initialize()
{
}

// 0x004D21F0 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Finalize()
{
}

// 0x004D1E90 slot 0x10 | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Start()
{
}

// 0x004D1C74 slot 0x14 | nintendogs:callseq
void nw::snd::internal::driver::StreamSoundPlayer::Stop()
{
}

// 0x004D1CE4 slot 0x18 | fefates:bytes
void nw::snd::internal::driver::StreamSoundPlayer::Pause(bool)
{
}

// 0x004D1FDC slot 0x1C | virtual slot, introduced by nw::snd::internal::driver::StreamSoundPlayer
void nw::snd::internal::driver::StreamSoundPlayer::vf_0x1C()
{
}

// 0x004D0BB4 slot 0x20 | virtual slot, introduced by nw::snd::internal::driver::StreamSoundPlayer
void nw::snd::internal::driver::StreamSoundPlayer::vf_0x20()
{
}

// 0x004D1C24 slot 0x24 | virtual slot, introduced by nw::snd::internal::driver::StreamSoundPlayer
void nw::snd::internal::driver::StreamSoundPlayer::vf_0x24()
{
}

// 0x004D0694 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::LoadHeader(bool, nn::snd::CTR::AdpcmParam**, unsigned short)
{
}

// 0x004D0B74 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackPan(unsigned int, float)
{
}

// 0x004D0BB8 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::UpdateBuffer()
{
}

// 0x004D0EB8 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::LoadStreamData(bool, const nw::snd::internal::LoadDataParam&)
{
}

// 0x004D0ED8 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::LoadStreamData(bool, const nw::snd::internal::LoadDataParam&, bool, unsigned int, unsigned int)
{
}

// 0x004D10A4 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackVolume(unsigned int, float)
{
}

// 0x004D10E4 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::PreparePrefetch(const nw::snd::internal::driver::StreamSoundPlayer::PreparePrefetchArg&)
{
}

// 0x004D1318 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::UpdateVoiceParams(nw::snd::internal::driver::StreamTrack*)
{
}

// 0x004D1618 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::VoiceCallbackFunc(nw::snd::internal::driver::MultiVoice*, nw::snd::internal::driver::MultiVoice::VoiceCallbackStatus, void*)
{
}

// 0x004D1650 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::ApplyTrackDataInfo(const nw::snd::internal::StreamDataInfoDetail&)
{
}

// 0x004D1774 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::LoadPrefetchBlocks(nw::snd::internal::StreamSoundPrefetchFileReader&)
{
}

// 0x004D1B14 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::ApplyStreamDataInfo(const nw::snd::internal::StreamDataInfoDetail&)
{
}

// 0x004D1BE4 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackSurroundPan(unsigned int, float)
{
}

// 0x004D1C30 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::SetTrackInitialVolume(unsigned int, unsigned int)
{
}

// 0x004D1D64 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::Setup(const nw::snd::internal::driver::StreamSoundPlayer::SetupArg&)
{
}

// 0x004D1FE0 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::Update()
{
}

// 0x004D211C | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::Prepare(const nw::snd::internal::driver::StreamSoundPlayer::PrepareArg&)
{
}

// 0x004D22B4 | fefates:bytes [tier B]
nw::snd::internal::driver::StreamSoundPlayer::StreamSoundPlayer()
{
}

// 0x00741554 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::ReadStreamDataInfo(nw::snd::StreamDataInfo*) const
{
}

// 0x007415C8 | fefates:bytes [tier B]
void nw::snd::internal::driver::StreamSoundPlayer::GetPlaySamplePosition(bool) const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
