#pragma once

#include "decomp.h"
#include "Sound/dSoundHandleBindSeq.h"
#include "Sound/dSoundIHaniwaControl.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"

// RTTI 14SoundConductor @ 0x008CBD9C
// vtable 0x008F073C (vptr 0x008F0744), offset_to_top 0, 9 entries
// The beat of a piece of room music: a silent control sequence runs along with the music and reports each beat with a user
// procedure; its tempo follows the tempo table and the position of the music (the class name is from the RTTI, the rest is ours).
class SoundConductor : public ::SoundIHaniwaControl
{
public:
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_STARTED = 1,
        STATE_SONG_SET = 2,
        STATE_FOLLOWING = 3,
        STATE_STOPPING = 4,
    };
    // the tempo from a tick of the music on
    struct TempoEntry
    {
        u32 tempo; // 0x0
        u32 tick;  // 0x4
    };

    SoundConductor(); // 0x00276880
    // 0x0027693C
    // 0x002768FC (deleting dtor)
    virtual ~SoundConductor();
    virtual f32 GetTempo() override; // 0x00719AC8 slot 0x08
    virtual s16 GetBeat() override; // 0x00719AC0 slot 0x0C
    virtual s32 GetTicksPerBeat() override; // 0x00719A60 slot 0x10
    virtual s32 GetFramesPerBeat() override; // 0x00719A68 slot 0x14
    virtual s16 GetTickInBeat() override; // 0x0027645C slot 0x18
    virtual s32 GetFramesToBeat() override; // 0x00719AB8 slot 0x1C
    virtual u32 GetRandom() override; // 0x007247D0 slot 0x20

    static void SequenceCallback(u16 procId, nw::snd::SequenceUserprocCallbackParam* param, void* arg); // 0x002761B0

    void Start(); // 0x00276060
    // true when it follows the song
    bool SetSong(u32 soundId); // 0x00276108
    void Follow(u32 song, nw::snd::SoundHandle* musicHandle, u32 position, s32 tempo); // 0x002761DC
    void Stop(); // 0x00276448
    void Finalize(); // 0x0012F9A4
    void OnSequenceCallback(u32 procId, nw::snd::SequenceUserprocCallbackParam* param); // 0x002764AC
    void Update(); // 0x00276600

    static const u32 s_ControlSounds[2];
    static const u32 s_BeatTicks[3];
    static bool s_IsStarted;

    u32 m_State;                     // 0x04 State
    bool m_IsChanged;                // 0x08
    u32 m_SoundId;                   // 0x0C
    const TempoEntry* m_pTempoTable; // 0x10
    nw::snd::SoundHandle* m_pMusic;  // 0x14
    SoundHandleBindSeq m_Handle;     // 0x18
    u32 m_ControlTick;               // 0x24
    u32 m_MusicTick;                 // 0x28
    f32 m_Tempo;                     // 0x2C
    f32 m_TempoRatio;                // 0x30
    s16 m_Beat;                      // 0x34
    s16 m_GlobalBeat;                // 0x36
    u32 m_TicksPerBeat;              // 0x38
    u32 m_BaseTick;                  // 0x3C
    s32 m_FramesToBeat;              // 0x40
    u8 m_GridKind;                   // 0x44 of SoundMuInfo
    u32 m_UpbeatTicks;               // 0x48
    s32 m_Wait;                      // 0x4C
};
ASSERT_OFFSET(SoundConductor, m_Handle, 0x18);
ASSERT_OFFSET(SoundConductor, m_Tempo, 0x2C);
ASSERT_OFFSET(SoundConductor, m_GridKind, 0x44);
ASSERT_SIZE(SoundConductor, 0x50);
