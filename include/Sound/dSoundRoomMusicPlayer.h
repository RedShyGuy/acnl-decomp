#pragma once

#include "decomp.h"
#include "Sound/dSoundChordTracker.h"
#include "Sound/dSoundConductor.h"
#include "Sound/dSoundFader.h"
#include "nw/snd/snd_SoundHandle.h"

namespace sead { class Heap; }
class SoundObjFurnitureAudio;

// The music of a stereo in a room: the sequence of a song is loaded on demand and plays from the position of the clock, so that
// it runs on while the player is away (the class name is from the RTTI of its disposer, everything else is ours).
class SoundRoomMusicPlayer
{
public:
    class SingletonDisposer_;

    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_LOADING = 1,
        STATE_LOADED = 2,
        STATE_LOADING_PLAY_PENDING = 3,
        STATE_WAIT_UNLOAD = 4,
        STATE_WAIT_RELOAD = 5,
        STATE_WAIT_CONDUCTOR = 6,
        STATE_PLAYING = 7,
        STATE_STOPPING = 8,
        STATE_STOPPING_RELOAD = 9,
    };
    enum
    {
        TEMPO_NUM = 50,
        LOAD_INDEX = 1,
    };

    SoundRoomMusicPlayer();
    ~SoundRoomMusicPlayer(); // 0x00128F18
    static SoundRoomMusicPlayer* createInstance(sead::Heap* heap); // 0x003225DC
    static void destroyInstance(); // 0x00123A70
    // SoundChordTracker::IsSessionActive of the instance (name is ours; nothing in the program refers to it directly)
    static bool SIsSessionActive(); // 0x0070F980

    void Initialize(); // 0x003222C0
    // the ticks of the music that take the milliseconds from a tick on
    u32 CalcTicks(u32 start, u32 milliSeconds); // 0x00322354
    DECOMP_NOINLINE void UpdateVolume(); // 0x003224A0
    DECOMP_NOINLINE void Play(s8 isFromStart, u32 milliSeconds); // 0x003226CC
    DECOMP_NOINLINE void Load(u32 song); // 0x00322A48
    DECOMP_NOINLINE void Start(u32 tick); // 0x00322C88
    void Update(); // 0x00322E68
    void Stop(s32 fadeFrames); // 0x0012FBF0
    DECOMP_NOINLINE void Unload(bool isAll); // 0x0012FC94
    DECOMP_EMIT_COPY bool IsPlaying() const // 0x00724DD0
    {
        return m_State == STATE_PLAYING || m_State == STATE_STOPPING || m_State == STATE_STOPPING_RELOAD;
    }

    static SoundRoomMusicPlayer* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];       // 0x000
    nw::snd::SoundHandle m_Handle;          // 0x010
    s32 m_LoadId;                           // 0x014
    bool m_IsInitialized;                   // 0x018
    u8 m_State;                             // 0x019 State
    u32 m_Song;                             // 0x01C
    s32 m_Wait;                             // 0x020
    u32 m_EndTick;                          // 0x024
    u32 m_FadeTick;                         // 0x028
    SoundFader m_StartFader;                // 0x02C
    SoundFader m_EndFader;                  // 0x040
    bool m_IsFinished;                      // 0x054
    SoundConductor::TempoEntry m_Tempos[TEMPO_NUM]; // 0x058
    u32 m_Tempo;                            // 0x1E8
    s8 m_PendingIsFromStart;                // 0x1EC
    u32 m_PendingMilliSeconds;              // 0x1F0
    SoundConductor m_Conductor;             // 0x1F4
    SoundChordTracker m_Chords;             // 0x244
    SoundObjFurnitureAudio* m_pObj;         // 0x794
};
ASSERT_OFFSET(SoundRoomMusicPlayer, m_StartFader, 0x2C);
ASSERT_OFFSET(SoundRoomMusicPlayer, m_Tempos, 0x58);
ASSERT_OFFSET(SoundRoomMusicPlayer, m_Tempo, 0x1E8);
ASSERT_OFFSET(SoundRoomMusicPlayer, m_Conductor, 0x1F4);
ASSERT_OFFSET(SoundRoomMusicPlayer, m_Chords, 0x244);
ASSERT_OFFSET(SoundRoomMusicPlayer, m_pObj, 0x794);
ASSERT_SIZE(SoundRoomMusicPlayer, 0x798);
