#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/driver/snd_WaveSoundPlayer.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "nw/snd/internal/snd_SoundInstanceManager.h"

namespace nw {
namespace snd {
class WaveSoundHandle;

namespace internal {
// A wave sound: the BasicSound with its WaveSoundPlayer (the class and function names are from the
// reference symbols, the members are ours).
// RTTI N2nw3snd8internal9WaveSoundE @ 0x008D0B60
// vtable 0x00903514 (vptr 0x0090351C), offset_to_top 0, 13 entries
class WaveSound : public ::nw::snd::internal::BasicSound
{
public:
    typedef SoundInstanceManager<WaveSound> WaveSoundInstanceManager;

    static const TypeInfo s_TypeInfo;

    explicit WaveSound(WaveSoundInstanceManager& manager); // 0x004D4E54 | fefates:bytes [tier B]
    virtual const TypeInfo* GetRuntimeTypeInfo() const; // 0x0074320C slot 0x00 (name after nw::gfx)
    virtual ~WaveSound(); // 0x004D4EBC slot 0x04 | fefates:bytes

    virtual void Initialize(); // 0x004D4BEC slot 0x0C
    virtual void Finalize(); // 0x004D4DF4 slot 0x10
    virtual bool IsPrepared() const; // 0x007431D8 slot 0x14 | fefates:bytes
    virtual bool IsAttachedTempSpecialHandle(); // 0x004D4D7C slot 0x18
    virtual void DetachTempSpecialHandle(); // 0x004D4D70 slot 0x1C
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle(); // 0x004D4D74 slot 0x20
    virtual void OnUpdatePlayerPriority(); // 0x004D4CE4 slot 0x24
    virtual void OnUpdateParam(); // 0x004D4C10 slot 0x2C

    // the data is prepared: the player starts with it
    void Prepare(const void* pWaveSoundFile, const void* pWaveFile, const driver::WaveSoundPlayer::StartInfo& startInfo,
                 s8 waveType); // 0x004D4D8C | fefates:bytes [tier B]
    // the data is not loaded: the player loads it
    void RegisterDataLoadTask(const driver::WaveSoundLoader::LoadInfo& loadInfo,
                              const driver::WaveSoundPlayer::StartInfo& startInfo); // 0x004D4C14 | fefates:bytes [tier B]
    void InitializeChannelParam(int priority, bool isReleasePriorityFix); // 0x004D4C94 | fefates:bytes [tier B]

    WaveSoundHandle* m_pTempSpecialHandle;   // 0x118
    WaveSoundInstanceManager* m_pManager;    // 0x11C
    bool m_InitializeFlag;                   // 0x120
    bool m_PreparedFlag;                     // 0x121
    u8 padding0x122[2];                      // 0x122
    driver::WaveSoundPlayer m_WaveSoundPlayer; // 0x124
};
ASSERT_SIZE(WaveSound, 0x1E8);
} // namespace internal
} // namespace snd
} // namespace nw
