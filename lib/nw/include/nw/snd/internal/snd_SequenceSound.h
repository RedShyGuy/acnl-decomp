#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_SequenceSoundLoader.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_SoundInstanceManager.h"

namespace nw {
namespace snd {
class SequenceSoundHandle;

namespace internal {
// A sequence sound: the BasicSound with its SequenceSoundPlayer; the functions send commands to the player
// in the sound thread (the class and function names are from the reference symbols, the members and the
// names of the unnamed functions are ours, after the functions of the player they call).
// RTTI N2nw3snd8internal13SequenceSoundE @ 0x008D09C0
// vtable 0x00903120 (vptr 0x00903128), offset_to_top 0, 13 entries
class SequenceSound : public ::nw::snd::internal::BasicSound
{
public:
    typedef SoundInstanceManager<SequenceSound> SequenceSoundInstanceManager;
    typedef driver::SequenceSoundPlayer::StartInfo StartInfo;

    static const TypeInfo s_TypeInfo;

    explicit SequenceSound(SequenceSoundInstanceManager& manager); // 0x004C80D0 | fefates:bytes [tier B]
    virtual const TypeInfo* GetRuntimeTypeInfo() const; // 0x0073F460 slot 0x00 (name after nw::gfx)
    virtual ~SequenceSound(); // 0x004C8138 slot 0x04 | fefates:bytes

    virtual void Initialize(); // 0x004C7A68 slot 0x0C
    virtual void Finalize(); // 0x004C8070 slot 0x10
    virtual bool IsPrepared() const; // 0x0073F404 slot 0x14 | fefates:bytes
    virtual bool IsAttachedTempSpecialHandle(); // 0x004C7EB4 slot 0x18
    virtual void DetachTempSpecialHandle(); // 0x00132528 slot 0x1C
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle(); // 0x004C7EAC slot 0x20
    virtual void OnUpdatePlayerPriority(); // 0x004C7E20 slot 0x24
    virtual void OnUpdateParam(); // 0x004C7B34 slot 0x2C

    void Setup(driver::SequenceTrackAllocator* pAllocator, u32 allocTracks, driver::NoteOnCallback* pCallback, int channelPriority,
               bool isReleasePriorityFix, SequenceUserprocCallback userproc, void* pUserprocArg); // 0x004C7EC4 | fefates:bytes [tier B]
    // the data is prepared: the player starts with it
    // (symbols.json: Prepare(const void*, const StartInfo&, const LoadItemInfo*, bool) from a nintendogs call sequence;
    // the code takes the prepared data and the start info)
    void Prepare(const driver::SequenceSoundPlayer::PrepareArg& arg, const StartInfo& startInfo); // 0x004C7F3C (name after nintendogs)
    // the data is not loaded: the player loads it
    void RegisterDataLoadTask(const driver::SequenceSoundLoader::LoadInfo& loadInfo,
                              const driver::SequenceSoundPlayer::StartInfo& startInfo); // 0x004C7D78 | fefates:bytes [tier B]

    void SetTempoRatio(f32 tempoRatio); // 0x004C7B38 | nintendogs:bytes-fuzzy [tier A]
    void SetChannelPriority(int priority); // 0x004C7C9C (name is ours)
    void SetTrackMute(u32 trackBitFlag, SeqMute mute); // 0x004C7A8C (name after SequenceSoundPlayer)
    void SetTrackMute(u32 trackBitFlag, bool isMute); // 0x004C7AD8 (name after SequenceSoundPlayer)
    void SetTrackSilence(u32 trackBitFlag, bool isSilence, int fadeTimes); // 0x004C7C2C (name after SequenceSoundPlayer)
    void SetTrackVolume(u32 trackBitFlag, f32 volume); // 0x004C7BD4 (name after SequenceSoundPlayer)
    bool ReadVariable(int varNo, s16* pValue) const; // 0x0073F438 (name is ours)
    void WriteVariable(int varNo, s16 value); // 0x004C7B88 | nintendogs:bytes-fuzzy [tier A]
    static bool ReadGlobalVariable(int varNo, s16* pValue); // 0x004C7C84 (name is ours)
    static void WriteGlobalVariable(int varNo, s16 value); // 0x004C7D34 (name is ours)
    void WriteTrackVariable(int trackNo, int varNo, s16 value); // 0x004C7CE4 (name is ours)
    // the ticks played (0: no player)
    u32 GetTick() const; // 0x0073F46C (name is ours)

    SequenceSoundHandle* m_pTempSpecialHandle;      // 0x118
    SequenceSoundInstanceManager* m_pManager;       // 0x11C
    bool m_InitializeFlag;                          // 0x120
    bool m_PreparedFlag;                            // 0x121
    u8 padding0x122[6];                             // 0x122
    driver::SequenceSoundPlayer m_SequenceSoundPlayer; // 0x128
};
ASSERT_SIZE(SequenceSound, 0x2D8);
} // namespace internal
} // namespace snd
} // namespace nw
