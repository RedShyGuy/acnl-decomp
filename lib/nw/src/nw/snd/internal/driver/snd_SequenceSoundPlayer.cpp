#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D3A20 slot 0x00 | fefates:bytes
nw::snd::internal::driver::SequenceSoundPlayer::~SequenceSoundPlayer()
{
}

// 0x004D26C0 slot 0x08 | fefates:bytes
void nw::snd::internal::driver::SequenceSoundPlayer::Initialize()
{
}

// 0x004D3614 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::SequenceSoundPlayer::Finalize()
{
}

// 0x004D3148 slot 0x10 | slot vf_0x10 of nw::snd::internal::driver::BasicSoundPlayer
void nw::snd::internal::driver::SequenceSoundPlayer::Start()
{
}

// 0x004D29E0 slot 0x14 | slot vf_0x14 of nw::snd::internal::driver::BasicSoundPlayer
void nw::snd::internal::driver::SequenceSoundPlayer::Stop()
{
}

// 0x004D303C slot 0x18 | nintendogs:bytes-fuzzy
void nw::snd::internal::driver::SequenceSoundPlayer::Pause(bool)
{
}

// 0x004D2B3C slot 0x1C | virtual slot, introduced by nw::snd::internal::driver::SequenceSoundPlayer
void nw::snd::internal::driver::SequenceSoundPlayer::vf_0x1C()
{
}

// 0x004D2BF4 slot 0x20 | slot vf_0x20 of nw::snd::internal::driver::SequenceSoundPlayer
void nw::snd::internal::driver::SequenceSoundPlayer::ChannelCallback(nw::snd::internal::driver::Channel*)
{
}

// 0x004D3184 slot 0x24 | virtual slot, introduced by nw::snd::internal::driver::SequenceSoundPlayer
void nw::snd::internal::driver::SequenceSoundPlayer::vf_0x24()
{
}

// 0x004D2EC0 slot 0x28 | virtual slot, introduced by nw::snd::internal::driver::SequenceSoundPlayer
void nw::snd::internal::driver::SequenceSoundPlayer::vf_0x28()
{
}

// 0x004D279C | mk7dlp:callseq [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::UpdateTick()
{
}

// 0x004D2974 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::RequestLoad(const nw::snd::internal::driver::SequenceSoundPlayer::StartInfo&, const nw::snd::internal::driver::SequenceSoundLoader::Arg&)
{
}

// 0x004D29C8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackPan(unsigned, float)
{
}

// 0x004D29E4 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::FinishPlayer()
{
}

// 0x004D2A60 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackMute(unsigned, nw::snd::SeqMute)
{
}

// 0x004D2AE4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackPitch(unsigned, float)
{
}

// 0x004D2AFC | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::GetPlayerTrack(int)
{
}

// 0x004D2B10 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::GetVariablePtr(int)
{
}

// 0x004D2BDC | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackVolume(unsigned, float)
{
}

// 0x004D2BF8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackLpfFreq(unsigned, float)
{
}

// 0x004D2C10 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackSilence(unsigned long, bool, int)
{
}

// 0x004D2CA4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackPanRange(unsigned, float)
{
}

// 0x004D2CE4 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackBankIndex(unsigned int, int)
{
}

// 0x004D2D80 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackTranspose(unsigned int, signed char)
{
}

// 0x004D2E18 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackSurroundPan(unsigned, float)
{
}

// 0x004D2E30 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackBiquadFilter(unsigned, int, float)
{
}

// 0x004D2ECC | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SetTrackVelocityRange(unsigned int, unsigned char)
{
}

// 0x004D2F50 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::InitSequenceSoundPlayer()
{
}

// 0x004D2F88 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::CallSequenceUserprocCallback(unsigned short, nw::snd::internal::driver::SequenceTrack*)
{
}

// 0x004D2FF8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::SequenceSoundPlayer::Skip(nw::snd::internal::driver::SequenceSoundPlayer::OffsetType, int)
{
}

// 0x004D307C | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::Setup(const nw::snd::internal::driver::SequenceSoundPlayer::SetupArg&)
{
}

// 0x004D3154 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::NoteOn(unsigned char, const nw::snd::internal::driver::NoteOnInfo&)
{
}

// 0x004D3188 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::Update()
{
}

// 0x004D3520 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::Prepare(const nw::snd::internal::driver::SequenceSoundPlayer::PrepareArg&)
{
}

// 0x004D36F8 | fefates:bytes [tier B]
void nw::snd::internal::driver::SequenceSoundPlayer::SkipTick()
{
}

// 0x004D38E8 | fefates:bytes [tier B]
nw::snd::internal::driver::SequenceSoundPlayer::SequenceSoundPlayer()
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
