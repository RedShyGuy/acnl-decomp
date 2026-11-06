#pragma once

#include "decomp.h"
#include "Sound/dSoundRing.h"
#include "Sound/dSoundSeqParser.h"
#include "nw/snd/snd_SoundHandle.h"
#include "nw/snd/internal/snd_BasicSound.h"
#include "sead/seadNew.h"

namespace sead { class Heap; }

// K.K.'s live show: before the song starts, the notes of eleven tracks of its sequence are read and merged into one timeline,
// so that the show (the band on the stage) can follow which part plays; the control track 14 gives the speed changes and the
// marks of the show (the class name is from the RTTI of its disposer, everything else is ours).
class SoundLivePlayer
{
public:
    class SingletonDisposer_;

    enum
    {
        NOTE_TABLE_NUM = 11,
        TEMPO_NUM = 80,
        SPEED_NUM = 20,
        MARK_NUM = 80,
        NOTE_EVENT_NUM = 610,
        SPEED_EVENT_NUM = 20,
    };
    typedef SoundSeqParser::Entry Entry;
    // a note of the merged timeline
    struct NoteEvent
    {
        u32 tick;   // 0x0
        u32 length; // 0x4
        u8 table;   // 0x8 index of the note table
    };
    // a change of the speed of the show
    struct SpeedEvent
    {
        u32 tick;    // 0x0
        f32 speed;   // 0x4
        u32 barTick; // 0x8 the position in the bar at the change
    };

    SoundLivePlayer(); // 0x002A5088
    ~SoundLivePlayer() { FreeBuffers(); }
    static SoundLivePlayer* createInstance(sead::Heap* heap); // 0x002A3C20
    static void destroyInstance(); // 0x0012371C

    void AllocateBuffers(); // 0x002A3BA8
    void Prepare(u32 soundId); // 0x002A3C88
    DECOMP_NOINLINE bool StartSound(u32 tick); // 0x002A43D4
    void UpdateCountdown(); // 0x002A4654
    void Update(); // 0x002A4764
    DECOMP_EMIT_COPY void FreeBuffers() // 0x002A4CF0
    {
        if (!m_IsAllocated) {
            return;
        }
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->Stop(0);
        }
        for (s32 i = 0; i < NOTE_TABLE_NUM; i++) {
            delete[] m_pNoteTables[i];
        }
        m_IsAllocated = false;
    }
    // the tick at the milliseconds from the start
    DECOMP_NOINLINE u32 CalcStartTick(u32 milliSeconds); // 0x002A4D44
    // the songs after the 91 of the archive
    bool PlayExtra(u32 index, u32 milliSeconds); // 0x002A4E88
    DECOMP_NOINLINE bool Play(u32 song, u32 milliSeconds); // 0x002A4E90
    DECOMP_EMIT_COPY s16 GetBeatsPerBar() const // 0x0071CD04
    {
        return m_IsPlaying ? m_BeatsPerBar : 0;
    }

    static const u32 s_NoteTableSizes[NOTE_TABLE_NUM];
    static const u8 s_NoteTracks[NOTE_TABLE_NUM];
    static const s16 s_NoteKinds[NOTE_TABLE_NUM];
    static const SpeedEvent s_DefaultSpeed;
    static SoundLivePlayer* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;
    static u32 s_LookaheadTicks;
    static u32 s_TicksPerBeat;

    u32 m_SingletonDisposerBuffer[4];          // 0x0000
    bool m_IsPlaying;                          // 0x0010
    u32 m_Song;                                // 0x0014
    u32 m_SoundId;                             // 0x0018
    nw::snd::SoundHandle m_Handle;             // 0x001C
    s32 m_Tempo;                               // 0x0020
    u32 m_LengthMs;                            // 0x0024
    u32 m_EndMs;                               // 0x0028 2 seconds before the end of the show
    u32 m_IntroFrames;                         // 0x002C
    s32 m_Wait;                                // 0x0030 frames until the song starts
    Entry* m_pNoteTables[NOTE_TABLE_NUM];      // 0x0034
    Entry m_Tempos[TEMPO_NUM];                 // 0x0060
    Entry m_SpeedEntries[SPEED_NUM];           // 0x02E0
    Entry m_Marks[MARK_NUM];                   // 0x0380
    SoundRing<NoteEvent, NOTE_EVENT_NUM> m_Notes; // 0x0600
    s16 m_NoteKind;                            // 0x22A8 of the note that plays (s_NoteKinds), -1: none
    s16 m_Variable0;                           // 0x22AA
    s16 m_Variable1;                           // 0x22AC
    s16 m_Variable2;                           // 0x22AE the largest value so far
    s16 m_Mark;                                // 0x22B0
    s16 m_BeatsPerBar;                         // 0x22B2
    SoundRing<SpeedEvent, SPEED_EVENT_NUM> m_Speeds; // 0x22B4
    f32 m_BarPhase;                            // 0x23B4
    f32 m_MeasurePhase;                        // 0x23B8
    f32 m_BeatPhase;                           // 0x23BC
    bool m_IsAllocated;                        // 0x23C0
    bool m_IsLatencyCorrected;                 // 0x23C1
    u32 m_MarkTick;                            // 0x23C4
};
ASSERT_OFFSET(SoundLivePlayer, m_pNoteTables, 0x34);
ASSERT_OFFSET(SoundLivePlayer, m_Tempos, 0x60);
ASSERT_OFFSET(SoundLivePlayer, m_SpeedEntries, 0x2E0);
ASSERT_OFFSET(SoundLivePlayer, m_Marks, 0x380);
ASSERT_OFFSET(SoundLivePlayer, m_Notes, 0x600);
ASSERT_OFFSET(SoundLivePlayer, m_NoteKind, 0x22A8);
ASSERT_OFFSET(SoundLivePlayer, m_Speeds, 0x22B4);
ASSERT_OFFSET(SoundLivePlayer, m_BarPhase, 0x23B4);
ASSERT_OFFSET(SoundLivePlayer, m_IsAllocated, 0x23C0);
ASSERT_SIZE(SoundLivePlayer, 0x23C8);
