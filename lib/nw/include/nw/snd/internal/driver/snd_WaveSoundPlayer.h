#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_BasicSoundPlayer.h"
#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/driver/snd_SoundThread_PlayerCallback.h"
#include "nw/snd/internal/driver/snd_WaveSoundLoader.h"
#include "nw/snd/internal/snd_CurveLfoParam.h"
#include "nw/snd/internal/snd_LoaderManager.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// The player of a wave sound in the sound thread: one channel for the wave of the first note; the data is
// either prepared by the sound or loaded by a WaveSoundLoader into a player heap (the class and function
// names are from the reference symbols, the members are ours).
// RTTI N2nw3snd8internal6driver15WaveSoundPlayerE @ 0x008D0A90
// vtable 0x00903348 (vptr 0x00903350), offset_to_top 0, 10 entries
// vtable 0x00903378 (vptr 0x00903380), offset_to_top -68, 3 entries
// vtable 0x0090338C (vptr 0x00903394), offset_to_top -80, 5 entries
class WaveSoundPlayer : public ::nw::snd::internal::driver::BasicSoundPlayer,
                        public ::nw::snd::internal::driver::DisposeCallback,
                        public ::nw::snd::internal::driver::SoundThread::PlayerCallback
{
public:
    // where the data comes from (names are ours)
    enum ResState : u8
    {
        RES_STATE_NONE = 0,
        RES_STATE_LOAD_REQUESTED = 1,
        RES_STATE_LOADING = 2,
        RES_STATE_PREPARED = 3,
    };
    // the unit of StartInfo::startOffset (names are ours)
    enum StartOffsetType : u8
    {
        START_OFFSET_TYPE_SAMPLE = 0,
        START_OFFSET_TYPE_MILLISEC = 1,
    };
    // the channel priority of a player with priority 0
    static const int CHANNEL_PRIORITY_BASE = 64; // (name is ours)

    // what to play (names are ours)
    struct StartInfo
    {
        int index;                        // 0x0 the wave sound in the file
        StartOffsetType startOffsetType;  // 0x4
        int startOffset;                  // 0x8
    };
    ASSERT_SIZE(StartInfo, 0xC);
    // the prepared data (names are ours)
    struct PrepareArg
    {
        const void* waveSoundFile;  // 0x0
        const void* waveFile;       // 0x4
        s8 waveType;                // 0x8 (WaveFileReader)
    };
    ASSERT_SIZE(PrepareArg, 0xC);

    WaveSoundPlayer(); // 0x004CE3B0 | fefates:bytes [tier B]
    virtual ~WaveSoundPlayer(); // 0x004CE488 slot 0x00 | fefates:bytes

    virtual void Initialize(); // 0x004CD9EC slot 0x08 | fefates:bytes
    virtual void Finalize(); // 0x004CE290 slot 0x0C | fefates:bytes
    virtual void Start(); // 0x004CDF48 slot 0x10
    virtual void Stop(); // 0x004CDECC slot 0x14
    virtual void Pause(bool isPause); // 0x004CDF0C slot 0x18 | fefates:bytes

    // the wave sound file goes away: the player stops
    virtual void InvalidateData(const void* pStart, const void* pEnd); // 0x004CDE44 slot 0x1C
    virtual void OnUpdateFrame(); // 0x004CDF54 slot 0x20
    virtual void OnShutdown(); // 0x004CDEC0 slot 0x24

    void Prepare(const StartInfo& startInfo, const PrepareArg& arg); // 0x004CE1D4 | fefates:bytes [tier B]
    // the data is loaded by a loader of m_pLoaderManager
    void RequestLoad(const StartInfo& startInfo, const WaveSoundLoader::Arg& arg); // 0x004CDA6C | fefates:bytes [tier B]
    void Update(); // 0x004CDF58 | fefates:bytes [tier B]

    // false if the wave cannot be played
    bool StartChannel(); // 0x004CDAB4 | fefates:bytes [tier B]
    void UpdateChannel(); // 0x004CDC10 | fefates:bytes [tier B]
    static void ChannelCallbackFunc(Channel* pChannel, Channel::ChannelCallbackStatus status, u32 userData); // 0x004CDEA4 | nintendogs:bytes [tier A]

    bool m_IsChannelStarted;                        // 0x5C
    bool m_IsReleasePriorityFix;                    // 0x5D
    u8 m_Priority;                                  // 0x5E
    s8 m_WaveType;                                  // 0x5F
    f32 m_PanRange;                                 // 0x60
    const void* m_pWaveSoundFile;                   // 0x64
    const void* m_pWaveFile;                        // 0x68
    StartInfo m_StartInfo;                          // 0x6C
    CurveLfoParam m_LfoParam;                       // 0x78
    WaveSoundInfo m_WaveSoundInfo;                  // 0x88
    Channel* m_pChannel;                            // 0x9C
    ResState m_ResState;                            // 0xA0
    bool m_IsInitialized;                           // 0xA1
    bool m_IsRegisteredPlayerCallback;              // 0xA2
    bool m_IsLoop;                                  // 0xA3
    LoaderManager<WaveSoundLoader>* m_pLoaderManager; // 0xA4
    WaveSoundLoader* m_pLoader;                     // 0xA8
    WaveSoundLoader::Arg m_LoaderArg;               // 0xAC
};
ASSERT_SIZE(WaveSoundPlayer, 0xC4);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
