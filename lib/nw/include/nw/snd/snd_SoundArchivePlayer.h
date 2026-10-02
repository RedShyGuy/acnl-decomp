#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/snd_SoundArchive.h"
#include "nw/snd/snd_SoundStartable.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd18SoundArchivePlayerE @ 0x008D0940
// vtable 0x00902FAC (vptr 0x00902FB4), offset_to_top 0, 4 entries
class SoundArchivePlayer : public ::nw::snd::SoundStartable
{
public:
    class SequenceNoteOnCallback;
    struct InitializeParam { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~SoundArchivePlayer(); // 0x004C393C slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x004C3914 slot 0x04 | virtual slot, introduced by nw::snd::SoundArchivePlayer
    virtual void detail_SetupSound(nw::snd::SoundHandle*, unsigned, bool, const nw::snd::SoundStartable::StartInfo*); // 0x004C1038 slot 0x08 | nintendogs:bytes
    virtual void detail_GetItemId(const char*); // 0x0073EDA4 slot 0x0C | slot vf_0x0C of nw::snd::SoundArchivePlayer
    SoundArchivePlayer(); // 0x001323C4 | fefates:bytes [tier B]
    void Initialize(const nw::snd::SoundArchivePlayer::InitializeParam&); // 0x004C0B04 | fefates:bytes [tier B]
    void GetSoundPlayer(unsigned int); // 0x004C0C80 | fefates:bytes [tier B]
    void SetupWaveSound(int, void**, const void*); // 0x004C0C98 | fefates:bytes [tier B]
    void SetupSoundPlayer(const nw::snd::SoundArchive*, void**, const void*); // 0x004C0E8C | fefates:bytes [tier B]
    void SetupSequenceSound(int, void**, const void*); // 0x004C1068 | fefates:bytes [tier B]
    void PrepareWaveSoundImpl(nw::snd::internal::WaveSound*, const nw::snd::SoundArchive::SoundInfo*, const nw::snd::SoundArchive::WaveSoundInfo*, nw::snd::SoundStartable::StartInfo::StartOffsetType, int, const nw::snd::SoundStartable::StartInfo::WaveSoundInfo*); // 0x004C1294 | fefates:bytes [tier B]
    void detail_SetupSoundImpl(nw::snd::SoundHandle*, unsigned, nw::snd::internal::BasicSound::AmbientInfo*, nw::snd::SoundActor*, bool, const nw::snd::SoundStartable::StartInfo*); // 0x004C14F8 | nintendogs:callgraph [tier A]
    void PrepareStreamSoundImpl(nw::snd::internal::StreamSound*, const nw::snd::SoundArchive::SoundInfo*, const nw::snd::SoundArchive::StreamSoundInfo*, const nw::snd::SoundArchive::StreamSoundInfo2*, nw::snd::SoundStartable::StartInfo::StartOffsetType, int, const nw::snd::SoundStartable::StartInfo::StreamSoundInfo*); // 0x004C24BC | fefates:bytes [tier B]
    void PrepareSequenceSoundImpl(nw::snd::internal::SequenceSound*, const nw::snd::SoundArchive::SoundInfo*, const nw::snd::SoundArchive::SequenceSoundInfo*, nw::snd::SoundStartable::StartInfo::StartOffsetType, int, const nw::snd::SoundStartable::StartInfo::SeqSoundInfo*); // 0x004C2B80 | nintendogs:callseq [tier A]
    void SetupUserParamForBasicSound(void**, const void*, unsigned int); // 0x004C30F0 | fefates:bytes [tier B]
    void Update(); // 0x004C31C4 | nintendogs:callseq [tier A]
    void Finalize(); // 0x004C3488 | fefates:bytes [tier B]
    void SetupMram(const nw::snd::SoundArchive*, void*, unsigned int, unsigned int); // 0x004C3744 | fefates:bytes [tier B]
    void GetRequiredMemSize(const nw::snd::SoundArchive*, unsigned int) const; // 0x0073F124 | fefates:bytes [tier B]
    void GetRequiredStreamCacheSize(const nw::snd::SoundArchive*, unsigned int) const; // 0x0073F288 | fefates:bytes [tier B]
    void GetRequiredStreamBufferSize(const nw::snd::SoundArchive*); // 0x0073F2B8 | nintendogs:bytes [tier A]
};
} // namespace snd
} // namespace nw
