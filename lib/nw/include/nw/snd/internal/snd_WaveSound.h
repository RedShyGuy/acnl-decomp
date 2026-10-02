#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/driver/snd_WaveSoundPlayer.h"
#include "nw/snd/internal/snd_BasicSound.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal9WaveSoundE @ 0x008D0B60
// vtable 0x00903514 (vptr 0x0090351C), offset_to_top 0, 13 entries
class WaveSound : public ::nw::snd::internal::BasicSound
{
public:
    WaveSound(); // ctor candidate(s) 0x004D4E54 (unverified)
    virtual void GetRuntimeTypeInfo() const; // 0x0074320C slot 0x00 | slot vf_0x00 of nw::snd::internal::BasicSound
    virtual ~WaveSound(); // 0x004D4EBC slot 0x04 | fefates:bytes
    virtual void vf_0x08(); // 0x004D4E8C slot 0x08 | virtual slot, introduced by nw::snd::internal::BasicSound
    virtual void Initialize(); // 0x004D4BEC slot 0x0C | slot vf_0x0C of nw::snd::internal::BasicSound
    virtual void Finalize(); // 0x004D4DF4 slot 0x10 | slot vf_0x10 of nw::snd::internal::BasicSound
    virtual void IsPrepared() const; // 0x007431D8 slot 0x14 | fefates:bytes
    virtual void IsAttachedTempSpecialHandle(); // 0x004D4D7C slot 0x18 | slot vf_0x18 of nw::snd::internal::BasicSound
    virtual void DetachTempSpecialHandle(); // 0x004D4D70 slot 0x1C | slot vf_0x1C of nw::snd::internal::BasicSound
    virtual void GetBasicSoundPlayerHandle(); // 0x004D4D74 slot 0x20 | slot vf_0x20 of nw::snd::internal::BasicSound
    virtual void OnUpdatePlayerPriority(); // 0x004D4CE4 slot 0x24 | slot vf_0x24 of nw::snd::internal::BasicSound
    virtual void OnUpdateParam(); // 0x004D4C10 slot 0x2C | slot vf_0x2C of nw::snd::internal::BasicSound
    void RegisterDataLoadTask(const nw::snd::internal::driver::WaveSoundLoader::LoadInfo&, const nw::snd::internal::driver::WaveSoundPlayer::StartInfo&); // 0x004D4C14 | fefates:bytes [tier B]
    void InitializeChannelParam(int, bool); // 0x004D4C94 | fefates:bytes [tier B]
    void Prepare(const void*, const void*, const nw::snd::internal::driver::WaveSoundPlayer::StartInfo&, signed char); // 0x004D4D8C | fefates:bytes [tier B]
    WaveSound(nw::snd::internal::SoundInstanceManager<nw::snd::internal::WaveSound>&); // 0x004D4E54 | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
