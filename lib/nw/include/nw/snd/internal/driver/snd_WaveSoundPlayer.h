#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver15WaveSoundPlayerE @ 0x008D0A90
// vtable 0x00903348 (vptr 0x00903350), offset_to_top 0, 10 entries
// vtable 0x00903378 (vptr 0x00903380), offset_to_top -68, 3 entries
// vtable 0x0090338C (vptr 0x00903394), offset_to_top -80, 5 entries
class WaveSoundPlayer : public ::nw::snd::internal::driver::BasicSoundPlayer, public ::nw::snd::internal::driver::DisposeCallback, public ::nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    struct PrepareArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct StartInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~WaveSoundPlayer(); // 0x004CE488 slot 0x00 | fefates:bytes
    // 0x004CE444 slot 0x04 | slot vf_0x04 of nw::snd::internal::driver::BasicSoundPlayer (deleting dtor)
    virtual void Initialize(); // 0x004CD9EC slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004CE290 slot 0x0C | fefates:bytes
    virtual void Start(); // 0x004CDF48 slot 0x10 | slot vf_0x10 of nw::snd::internal::driver::BasicSoundPlayer
    virtual void Stop(); // 0x004CDECC slot 0x14 | slot vf_0x14 of nw::snd::internal::driver::BasicSoundPlayer
    virtual void Pause(bool); // 0x004CDF0C slot 0x18 | fefates:bytes
    virtual void vf_0x1C(); // 0x004CDE44 slot 0x1C | virtual slot, introduced by nw::snd::internal::driver::WaveSoundPlayer
    virtual void vf_0x20(); // 0x004CDF54 slot 0x20 | virtual slot, introduced by nw::snd::internal::driver::WaveSoundPlayer
    virtual void vf_0x24(); // 0x004CDEC0 slot 0x24 | virtual slot, introduced by nw::snd::internal::driver::WaveSoundPlayer
    void RequestLoad(const nw::snd::internal::driver::WaveSoundPlayer::StartInfo&, const nw::snd::internal::driver::WaveSoundLoader::Arg&); // 0x004CDA6C | fefates:bytes [tier B]
    void StartChannel(); // 0x004CDAB4 | fefates:bytes [tier B]
    void UpdateChannel(); // 0x004CDC10 | fefates:bytes [tier B]
    void ChannelCallbackFunc(nw::snd::internal::driver::Channel*, nw::snd::internal::driver::Channel::ChannelCallbackStatus, unsigned); // 0x004CDEA4 | nintendogs:bytes [tier A]
    void Update(); // 0x004CDF58 | fefates:bytes [tier B]
    void Prepare(const nw::snd::internal::driver::WaveSoundPlayer::StartInfo&, const nw::snd::internal::driver::WaveSoundPlayer::PrepareArg&); // 0x004CE1D4 | fefates:bytes [tier B]
    WaveSoundPlayer(); // 0x004CE3B0 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
