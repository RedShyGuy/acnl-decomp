#pragma once

#include "decomp.h"
#include "Sound/dSoundHandleBindSeq.h"
#include "Sound/dSoundIHaniwaControl.h"
#include "Sound/dSoundSongQueue.h"
#include "nw/snd/snd_SoundStartable.h"
#include "nw/snd/internal/driver/snd_SequenceSoundPlayer.h"

namespace sead { class Heap; }

// RTTI 13SoundDJPlayer @ 0x008CBABC
// vtable 0x008EF794 (vptr 0x008EF79C), offset_to_top 0, 9 entries
// DJ K.K.: a queue of songs played with two sequence handles in turns (one prepares the next song) and a third sequence for the
// breaks. The sequences report the end of a part with user procedures; the time of the set is counted in ticks, 48 per beat
// (the class name is from the RTTI, everything else is ours).
class SoundDJPlayer : public ::SoundIHaniwaControl
{
public:
    class SingletonDisposer_;

    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_SONG = 1,
        STATE_BREAK = 2,
        STATE_BREAK_ALTERNATE = 3,
    };
    // m_Part
    enum Part
    {
        PART_NONE = 0,
        PART_INTRO = 1,
        PART_MAIN = 2,
        PART_FIN = 3,
        PART_BREAK = 4,
        PART_BREAK_ALTERNATE = 5,
    };
    // where a set starts (CalcStartPosition)
    struct StartPosition
    {
        bool isSong;    // 0x0 else in a break
        bool isEnding;  // 0x1 in the last ticks of the part
        u32 position;   // 0x4
    };

    SoundDJPlayer(); // 0x00246D30
    // 0x00246E94
    // 0x00246E34 (deleting dtor)
    virtual ~SoundDJPlayer();
    virtual f32 GetTempo() override; // 0x00714954 slot 0x08
    virtual s16 GetBeat() override; // 0x0071494C slot 0x0C
    virtual s32 GetTicksPerBeat() override; // 0x007147F4 slot 0x10
    virtual s32 GetFramesPerBeat() override; // 0x007147FC slot 0x14
    virtual s16 GetTickInBeat() override; // 0x00245AAC slot 0x18
    virtual s32 GetFramesToBeat() override; // 0x007148C0 slot 0x1C
    virtual u32 GetRandom() override; // 0x007147A0 slot 0x20

    static SoundDJPlayer* createInstance(sead::Heap* heap); // 0x00245104
    static void destroyInstance(); // 0x00123564
    // the procedures of the sequences of DJ K.K.
    static void SequenceCallback(u16 procId, nw::snd::SequenceUserprocCallbackParam* param, void* arg); // 0x002450DC

    void Initialize(); // 0x00244FBC
    void SetForceFirstSong(bool isForced); // 0x00245014
    void SetForceSecondSong(bool isForced); // 0x00245040
    DECOMP_NOINLINE void SetPart(u8 part, u8 nextSong); // 0x0024506C
    DECOMP_NOINLINE void StartSequence(SoundHandleBindSeq* handle, u32 soundId,
                       const nw::snd::SoundStartable::StartInfo* info); // 0x00245170
    DECOMP_NOINLINE const StartPosition* CalcStartPosition(u32 milliSeconds); // 0x002451B8
    void OnSequenceCallback(u16 procId, nw::snd::SequenceUserprocCallbackParam* param); // 0x002456AC
    // the key of the song for the track that plays the melody of the town
    DECOMP_NOINLINE void ApplyTownMelodyKey(); // 0x00245AB4
    DECOMP_NOINLINE void UpdatePhase(); // 0x00245BBC
    DECOMP_NOINLINE void ResetTiming(); // 0x00245EA0
    DECOMP_NOINLINE void SetPartDuration(u8 part, u8 song); // 0x00245F1C
    DECOMP_NOINLINE void SetPartPosition(u32 tick); // 0x00245FC0
    DECOMP_NOINLINE void SeekTiming(u32 tick, u32 beatsPerBar); // 0x002461AC
    // the beat counter
    s16 Update(); // 0x00246310
    void Stop(); // 0x0024661C
    void Play(const SoundSongQueueData* data, u32 milliSeconds); // 0x002466DC
    void Finalize(); // 0x00246C5C

    DECOMP_EMIT_COPY s32 GetSong() const // 0x00714728
    {
        return m_State != STATE_NONE ? m_Song & SoundSongQueue::SONG_MASK : -1;
    }
    DECOMP_EMIT_COPY u8 GetSongAfterCurrent() const; // 0x00714740
    DECOMP_EMIT_COPY s32 GetNextSong() const // 0x00714788
    {
        return m_State != STATE_NONE ? m_NextSong & SoundSongQueue::SONG_MASK : -1;
    }
    u8 GetBeatsPerBar() const; // 0x007147C8
    const SoundSongQueueData* GetQueueData(); // 0x00714858
    u8 GetBeatsPerBar(u32 song) const; // 0x007148B0
    f32 GetPartBeats() const; // 0x007148C8
    DECOMP_NOINLINE u32 GetPartBeats(u8 part, u8 song) const; // 0x007148E4

    static const u8 s_FixedSongs[2];
    static const s16 s_Tempos[2];
    static const u32 s_FirstSongs[2];
    static const u32 s_BreakSounds[2][2];
    static StartPosition s_StartPosition;
    static SoundDJPlayer* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4]; // 0x04
    bool m_IsInitialized;             // 0x14
    u8 m_State;                       // 0x15 State
    s32 m_DateState;                  // 0x18 Func1668A0: selects the sounds and the tempo
    u8 m_Song;                        // 0x1C
    u8 m_NextSong;                    // 0x1D
    SoundSongQueue m_Queue;           // 0x20
    SoundHandleBindSeq m_Handles[2];  // 0x58
    u8 m_HandleIndex;                 // 0x70
    SoundHandleBindSeq m_BreakHandle; // 0x74
    bool m_IsBreakPending;            // 0x80
    bool m_IsBreakStarting;           // 0x81
    bool m_IsSongPending;             // 0x82
    bool m_IsSongStarting;            // 0x83
    u16 m_BeatCount;                  // 0x84
    u32 m_BaseTick;                   // 0x88 the tick of the sequence at the start
    u32 m_StartPosition;              // 0x8C
    u8 m_Part;                        // 0x90 Part
    u8 m_PlayType;                    // 0x91 0 none, 1 break, 2 main, 3 fin, 4 break before the song, 5 before the alternate
    f32 m_BeatPhase;                  // 0x94
    f32 m_BarPhase;                   // 0x98
    f32 m_PartElapsed;                // 0x9C
    f32 m_PartRemaining;              // 0xA0
    s32 m_PartElapsedFrames;          // 0xA4
    s32 m_PartRemainingFrames;        // 0xA8
    s16 m_Beat;                       // 0xAC
    u32 m_Position;                   // 0xB0
    u32 m_BeatPosition;               // 0xB4
    s16 m_TickInBeat;                 // 0xB8
    s32 m_FramesToBeat;               // 0xBC
    s8 m_LightMode;                   // 0xC0
    s16 _C2;                          // 0xC2
    u8 m_IsForceFirstSong;            // 0xC4
    u8 m_IsForceSecondSong;           // 0xC5
    s32 m_FixedSongIndex;             // 0xC8 2 = no fixed song
};
ASSERT_OFFSET(SoundDJPlayer, m_IsInitialized, 0x14);
ASSERT_OFFSET(SoundDJPlayer, m_Queue, 0x20);
ASSERT_OFFSET(SoundDJPlayer, m_Handles, 0x58);
ASSERT_OFFSET(SoundDJPlayer, m_BreakHandle, 0x74);
ASSERT_OFFSET(SoundDJPlayer, m_BeatCount, 0x84);
ASSERT_OFFSET(SoundDJPlayer, m_BeatPhase, 0x94);
ASSERT_OFFSET(SoundDJPlayer, m_Beat, 0xAC);
ASSERT_OFFSET(SoundDJPlayer, m_FixedSongIndex, 0xC8);
ASSERT_SIZE(SoundDJPlayer, 0xCC);
