#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_WaveSoundPlayer.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CE488 slot 0x00 | fefates:bytes
nw::snd::internal::driver::WaveSoundPlayer::~WaveSoundPlayer()
{
}

// 0x004CD9EC slot 0x08 | fefates:bytes
void nw::snd::internal::driver::WaveSoundPlayer::Initialize()
{
}

// 0x004CE290 slot 0x0C | fefates:bytes
void nw::snd::internal::driver::WaveSoundPlayer::Finalize()
{
}

// 0x004CDF48 slot 0x10 | slot vf_0x10 of nw::snd::internal::driver::BasicSoundPlayer
void nw::snd::internal::driver::WaveSoundPlayer::Start()
{
}

// 0x004CDECC slot 0x14 | slot vf_0x14 of nw::snd::internal::driver::BasicSoundPlayer
void nw::snd::internal::driver::WaveSoundPlayer::Stop()
{
}

// 0x004CDF0C slot 0x18 | fefates:bytes
void nw::snd::internal::driver::WaveSoundPlayer::Pause(bool)
{
}

// 0x004CDE44 slot 0x1C | virtual slot, introduced by nw::snd::internal::driver::WaveSoundPlayer
void nw::snd::internal::driver::WaveSoundPlayer::vf_0x1C()
{
}

// 0x004CDF54 slot 0x20 | virtual slot, introduced by nw::snd::internal::driver::WaveSoundPlayer
void nw::snd::internal::driver::WaveSoundPlayer::vf_0x20()
{
}

// 0x004CDEC0 slot 0x24 | virtual slot, introduced by nw::snd::internal::driver::WaveSoundPlayer
void nw::snd::internal::driver::WaveSoundPlayer::vf_0x24()
{
}

// 0x004CDA6C | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::RequestLoad(const nw::snd::internal::driver::WaveSoundPlayer::StartInfo&, const nw::snd::internal::driver::WaveSoundLoader::Arg&)
{
}

// 0x004CDAB4 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::StartChannel()
{
}

// 0x004CDC10 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::UpdateChannel()
{
}

// 0x004CDEA4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::WaveSoundPlayer::ChannelCallbackFunc(nw::snd::internal::driver::Channel*, nw::snd::internal::driver::Channel::ChannelCallbackStatus, unsigned)
{
}

// 0x004CDF58 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::Update()
{
}

// 0x004CE1D4 | fefates:bytes [tier B]
void nw::snd::internal::driver::WaveSoundPlayer::Prepare(const nw::snd::internal::driver::WaveSoundPlayer::StartInfo&, const nw::snd::internal::driver::WaveSoundPlayer::PrepareArg&)
{
}

// 0x004CE3B0 | fefates:bytes [tier B]
nw::snd::internal::driver::WaveSoundPlayer::WaveSoundPlayer()
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
