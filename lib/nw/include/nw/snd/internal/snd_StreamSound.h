#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_StreamSoundPlayer.h"
#include "nw/snd/internal/snd_BasicSound.h"

namespace nw {
namespace snd {
namespace internal {
// RTTI N2nw3snd8internal11StreamSoundE @ 0x008D0990
// vtable 0x00903054 (vptr 0x0090305C), offset_to_top 0, 13 entries
class StreamSound : public ::nw::snd::internal::BasicSound
{
public:
    StreamSound(); // ctor candidate(s) 0x004C675C (unverified)
    virtual void GetRuntimeTypeInfo() const; // 0x0073F380 slot 0x00 | slot vf_0x00 of nw::snd::internal::BasicSound
    virtual ~StreamSound(); // 0x004C6818 slot 0x04 | fefates:bytes
    virtual void vf_0x08(); // 0x004C67D4 slot 0x08 | virtual slot, introduced by nw::snd::internal::BasicSound
    virtual void Initialize(); // 0x004C6168 slot 0x0C | nintendogs:callseq
    virtual void Finalize(); // 0x004C66EC slot 0x10 | fefates:bytes
    virtual void IsPrepared() const; // 0x0073F344 slot 0x14 | fefates:bytes
    virtual void IsAttachedTempSpecialHandle(); // 0x004C660C slot 0x18 | fefates:bytes
    virtual void DetachTempSpecialHandle(); // 0x004C08B0 slot 0x1C | slot vf_0x1C of nw::snd::internal::BasicSound
    virtual void GetBasicSoundPlayerHandle(); // 0x004C6604 slot 0x20 | slot vf_0x20 of nw::snd::internal::BasicSound
    virtual void OnUpdatePlayerPriority(); // 0x004C655C slot 0x24 | fefates:bytes
    virtual void UpdateMoveValue(); // 0x004C644C slot 0x28 | fefates:bytes
    virtual void UpdateParam(); // 0x004C6278 slot 0x30 | mk7dlp:bytes-fuzzy
    void SetTrackVolume(unsigned long, float, int); // 0x004C6340 | nintendogs:bytes-fuzzy [tier A]
    void PreparePrefetch(const void*, const nw::snd::internal::driver::StreamSoundPlayer::PrepareBaseArg&); // 0x004C63F4 | fefates:bytes [tier B]
    void Setup(const nw::snd::internal::driver::StreamSoundPlayer::SetupArg&); // 0x004C6620 | fefates:bytes [tier B]
    void Prepare(nw::io::FileStream*, const nw::snd::internal::driver::StreamSoundPlayer::PrepareBaseArg&); // 0x004C6688 | fefates:bytes [tier B]
    StreamSound(nw::snd::internal::SoundInstanceManager<nw::snd::internal::StreamSound>&); // 0x004C675C | fefates:bytes [tier B]
};
} // namespace internal
} // namespace snd
} // namespace nw
