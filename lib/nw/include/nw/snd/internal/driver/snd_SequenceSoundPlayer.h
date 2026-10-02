#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver19SequenceSoundPlayerE @ 0x008D0B10
// vtable 0x00903468 (vptr 0x00903470), offset_to_top 0, 11 entries
// vtable 0x0090349C (vptr 0x009034A4), offset_to_top -68, 3 entries
// vtable 0x009034B0 (vptr 0x009034B8), offset_to_top -80, 5 entries
class SequenceSoundPlayer : public ::nw::snd::internal::driver::BasicSoundPlayer, public ::nw::snd::internal::driver::DisposeCallback, public ::nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    struct OffsetType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct PrepareArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SetupArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct StartInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~SequenceSoundPlayer(); // 0x004D3A20 slot 0x00 | fefates:bytes
    // 0x004D39DC slot 0x04 | slot vf_0x04 of nw::snd::internal::driver::BasicSoundPlayer (deleting dtor)
    virtual void Initialize(); // 0x004D26C0 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004D3614 slot 0x0C | fefates:bytes
    virtual void Start(); // 0x004D3148 slot 0x10 | slot vf_0x10 of nw::snd::internal::driver::BasicSoundPlayer
    virtual void Stop(); // 0x004D29E0 slot 0x14 | slot vf_0x14 of nw::snd::internal::driver::BasicSoundPlayer
    virtual void Pause(bool); // 0x004D303C slot 0x18 | nintendogs:bytes-fuzzy
    virtual void vf_0x1C(); // 0x004D2B3C slot 0x1C | virtual slot, introduced by nw::snd::internal::driver::SequenceSoundPlayer
    virtual void ChannelCallback(nw::snd::internal::driver::Channel*); // 0x004D2BF4 slot 0x20 | slot vf_0x20 of nw::snd::internal::driver::SequenceSoundPlayer
    virtual void vf_0x24(); // 0x004D3184 slot 0x24 | virtual slot, introduced by nw::snd::internal::driver::SequenceSoundPlayer
    virtual void vf_0x28(); // 0x004D2EC0 slot 0x28 | virtual slot, introduced by nw::snd::internal::driver::SequenceSoundPlayer
    void UpdateTick(); // 0x004D279C | mk7dlp:callseq [tier A]
    void RequestLoad(const nw::snd::internal::driver::SequenceSoundPlayer::StartInfo&, const nw::snd::internal::driver::SequenceSoundLoader::Arg&); // 0x004D2974 | fefates:bytes [tier B]
    void SetTrackPan(unsigned, float); // 0x004D29C8 | nintendogs:bytes [tier A]
    void FinishPlayer(); // 0x004D29E4 | nintendogs:callseq [tier A]
    void SetTrackMute(unsigned, nw::snd::SeqMute); // 0x004D2A60 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackPitch(unsigned, float); // 0x004D2AE4 | nintendogs:bytes [tier A]
    void GetPlayerTrack(int); // 0x004D2AFC | nintendogs:callseq-callee [tier A]
    void GetVariablePtr(int); // 0x004D2B10 | nintendogs:callseq-callee [tier A]
    void SetTrackVolume(unsigned, float); // 0x004D2BDC | nintendogs:bytes [tier A]
    void SetTrackLpfFreq(unsigned, float); // 0x004D2BF8 | nintendogs:bytes [tier A]
    void SetTrackSilence(unsigned long, bool, int); // 0x004D2C10 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackPanRange(unsigned, float); // 0x004D2CA4 | nintendogs:bytes [tier A]
    void SetTrackBankIndex(unsigned int, int); // 0x004D2CE4 | fefates:bytes [tier B]
    void SetTrackTranspose(unsigned int, signed char); // 0x004D2D80 | fefates:bytes [tier B]
    void SetTrackSurroundPan(unsigned, float); // 0x004D2E18 | nintendogs:bytes [tier A]
    void SetTrackBiquadFilter(unsigned, int, float); // 0x004D2E30 | nintendogs:bytes-fuzzy [tier A]
    void SetTrackVelocityRange(unsigned int, unsigned char); // 0x004D2ECC | fefates:bytes [tier B]
    void InitSequenceSoundPlayer(); // 0x004D2F50 | nintendogs:bytes [tier A]
    void CallSequenceUserprocCallback(unsigned short, nw::snd::internal::driver::SequenceTrack*); // 0x004D2F88 | fefates:bytes [tier B]
    void Skip(nw::snd::internal::driver::SequenceSoundPlayer::OffsetType, int); // 0x004D2FF8 | nintendogs:bytes [tier A]
    void Setup(const nw::snd::internal::driver::SequenceSoundPlayer::SetupArg&); // 0x004D307C | fefates:bytes [tier B]
    void NoteOn(unsigned char, const nw::snd::internal::driver::NoteOnInfo&); // 0x004D3154 | fefates:bytes [tier B]
    void Update(); // 0x004D3188 | fefates:bytes [tier B]
    void Prepare(const nw::snd::internal::driver::SequenceSoundPlayer::PrepareArg&); // 0x004D3520 | fefates:bytes [tier B]
    void SkipTick(); // 0x004D36F8 | fefates:bytes [tier B]
    SequenceSoundPlayer(); // 0x004D38E8 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
