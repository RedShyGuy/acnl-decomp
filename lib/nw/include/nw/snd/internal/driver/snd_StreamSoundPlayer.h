#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// RTTI N2nw3snd8internal6driver17StreamSoundPlayerE @ 0x008D0AE4
// vtable 0x00903408 (vptr 0x00903410), offset_to_top 0, 10 entries
// vtable 0x00903438 (vptr 0x00903440), offset_to_top -68, 5 entries
class StreamSoundPlayer : public ::nw::snd::internal::driver::BasicSoundPlayer, public ::nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    struct PrepareArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct PrepareBaseArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct PreparePrefetchArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct SetupArg { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~StreamSoundPlayer(); // 0x004D2324 slot 0x00 | fefates:bytes
    // 0x004D230C slot 0x04 | slot vf_0x04 of nw::snd::internal::driver::BasicSoundPlayer (deleting dtor)
    virtual void Initialize(); // 0x004D05D0 slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004D21F0 slot 0x0C | fefates:bytes
    virtual void Start(); // 0x004D1E90 slot 0x10 | fefates:bytes
    virtual void Stop(); // 0x004D1C74 slot 0x14 | nintendogs:callseq
    virtual void Pause(bool); // 0x004D1CE4 slot 0x18 | fefates:bytes
    virtual void vf_0x1C(); // 0x004D1FDC slot 0x1C | virtual slot, introduced by nw::snd::internal::driver::StreamSoundPlayer
    virtual void vf_0x20(); // 0x004D0BB4 slot 0x20 | virtual slot, introduced by nw::snd::internal::driver::StreamSoundPlayer
    virtual void vf_0x24(); // 0x004D1C24 slot 0x24 | virtual slot, introduced by nw::snd::internal::driver::StreamSoundPlayer
    void LoadHeader(bool, nn::snd::CTR::AdpcmParam**, unsigned short); // 0x004D0694 | fefates:bytes [tier B]
    void SetTrackPan(unsigned int, float); // 0x004D0B74 | fefates:bytes [tier B]
    void UpdateBuffer(); // 0x004D0BB8 | fefates:bytes [tier B]
    void LoadStreamData(bool, const nw::snd::internal::LoadDataParam&); // 0x004D0EB8 | fefates:bytes [tier B]
    void LoadStreamData(bool, const nw::snd::internal::LoadDataParam&, bool, unsigned int, unsigned int); // 0x004D0ED8 | fefates:bytes [tier B]
    void SetTrackVolume(unsigned int, float); // 0x004D10A4 | fefates:bytes [tier B]
    void PreparePrefetch(const nw::snd::internal::driver::StreamSoundPlayer::PreparePrefetchArg&); // 0x004D10E4 | fefates:bytes [tier B]
    void UpdateVoiceParams(nw::snd::internal::driver::StreamTrack*); // 0x004D1318 | fefates:bytes [tier B]
    void VoiceCallbackFunc(nw::snd::internal::driver::MultiVoice*, nw::snd::internal::driver::MultiVoice::VoiceCallbackStatus, void*); // 0x004D1618 | fefates:bytes [tier B]
    void ApplyTrackDataInfo(const nw::snd::internal::StreamDataInfoDetail&); // 0x004D1650 | fefates:bytes [tier B]
    void LoadPrefetchBlocks(nw::snd::internal::StreamSoundPrefetchFileReader&); // 0x004D1774 | fefates:bytes [tier B]
    void ApplyStreamDataInfo(const nw::snd::internal::StreamDataInfoDetail&); // 0x004D1B14 | fefates:bytes [tier B]
    void SetTrackSurroundPan(unsigned int, float); // 0x004D1BE4 | fefates:bytes [tier B]
    void SetTrackInitialVolume(unsigned int, unsigned int); // 0x004D1C30 | fefates:bytes [tier B]
    void Setup(const nw::snd::internal::driver::StreamSoundPlayer::SetupArg&); // 0x004D1D64 | fefates:bytes [tier B]
    void Update(); // 0x004D1FE0 | fefates:bytes [tier B]
    void Prepare(const nw::snd::internal::driver::StreamSoundPlayer::PrepareArg&); // 0x004D211C | fefates:bytes [tier B]
    StreamSoundPlayer(); // 0x004D22B4 | fefates:bytes [tier B]
    void ReadStreamDataInfo(nw::snd::StreamDataInfo*) const; // 0x00741554 | fefates:bytes [tier B]
    void GetPlaySamplePosition(bool) const; // 0x007415C8 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
