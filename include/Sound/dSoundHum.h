#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"

class SoundObjBase;
struct SoundMelody;

// The humming of a character (a member of SoundObjNpc): while it is requested each frame, the character hums a note every
// 8 frames with SE_ACT_FEEL_HAPPY (the pitch in the sequence variable 0), by turns the town tune (SoundMelodyMgr::m_Melody,
// with fewer than five notes the third default melody) and a tune of 4, 8 or 12 notes made up from the notes of the town tune
// (the class and all names are ours).
class SoundHum
{
public:
    enum
    {
        MELODY_LENGTH = 16,
        NOTE_KIND_NUM = 13, // the notes of a melody that have a pitch
        TUNE_LENGTH_MAX = 12,
        NO_NOTE = 255,      // in m_Tune: the note before goes on
        IDLE_FRAMES = 120,
    };

    SoundHum(); // 0x002E8A10

    void Update(); // 0x002E86EC
    void Detach() { m_pOwner = NULL; }

    // the pitches of the notes (13: random, one of the six after it)
    static const s16 s_Pitches[NOTE_KIND_NUM + 6];
    // the three kinds of a note of a made up tune: one note for four beats, one for two beats, two notes of two beats
    static const u8 s_TuneWeights[3];
    static const u8 s_TuneLengths[3];

    nw::snd::SoundHandle m_Handle; // 0x00
    SoundObjBase* m_pOwner;        // 0x04
    bool m_IsRequested;            // 0x08 for this frame
    bool m_IsMelody;               // 0x09 the town tune, else the made up tune
    s32 m_Frame;                   // 0x0C
    u32 m_NoteIndex;               // 0x10
    bool m_IsNoteStart;            // 0x14
    u8 m_Length;                   // 0x15 of the tune
    const SoundMelody* m_pMelody;  // 0x18
    u8 m_NoteKindCount;            // 0x1C
    u8 m_NoteKinds[NOTE_KIND_NUM]; // 0x1D the notes the melody has
    u8 m_Tune[TUNE_LENGTH_MAX];    // 0x2A the made up tune
    bool m_IsRecent;               // 0x36 the humming stopped less than IDLE_FRAMES ago (it goes on with the same tune)
    s32 m_IdleFrames;              // 0x38
    bool m_IsBarStart;             // 0x3C every fourth note
    bool m_IsAlwaysMadeUp;         // 0x3D only made up tunes, with a short random pause before
    s32 m_PauseFrames;             // 0x40

private:
    // a new tune
    void Begin(); // 0x002E83EC
    void MakeUpTune(); // 0x002E84D0
    DECOMP_ALWAYS_INLINE static s32 GetTunePitch(u8 note);
};
ASSERT_OFFSET(SoundHum, m_pMelody, 0x18);
ASSERT_OFFSET(SoundHum, m_Tune, 0x2A);
ASSERT_OFFSET(SoundHum, m_IdleFrames, 0x38);
ASSERT_SIZE(SoundHum, 0x44);
