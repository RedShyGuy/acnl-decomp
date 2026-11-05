#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"

// a melody of 16 notes like the town tune: 0..12 the pitches, NOTE_RANDOM a random pitch, NOTE_HOLD the note before goes on,
// NOTE_REST silence (name is ours)
struct SoundMelody
{
    u8 notes[16];
};

// RTTI 21SoundMelodyPlayerBase @ 0x008CCD38
// vtable 0x008F6578 (vptr 0x008F6580), offset_to_top 0, 3 entries
// Plays a melody note by note with a sequence: the sequence gives the timing in its variables 0..5 (frames per note, the
// length of a note, how notes are skipped, whether a rest stops the note, the scale and the transposition), the player
// starts the sequence again for each note with the note in the variables 12..15 (the class name is from the RTTI,
// everything else is ours).
class SoundMelodyPlayerBase
{
public:
    enum Note
    {
        NOTE_RANDOM = 13,
        NOTE_HOLD = 14,
        NOTE_REST = 15,
        NOTE_NUM = 16, // the notes of a melody
    };

    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_LOAD = 1,     // waits for the sound data, then starts the sequence
        STATE_UNLOAD = 2,   // waits for the sound data, then unloads it
        STATE_READ = 3,     // reads the timing from the sequence
        STATE_PLAY = 4,
        STATE_END = 5,      // waits for the end of the sound
    };

    // m_SkipMode (the sequence variable 2)
    enum SkipMode
    {
        SKIP_NONE = 0,
        SKIP_ALTERNATE = 1, // every other step at even notes
        SKIP_RANDOM = 2,
        SKIP_TABLE = 3,     // by the steps of s_SkipSteps, with the velocities of s_Velocities (12 notes)
    };

    SoundMelodyPlayerBase(); // 0x0012FCE8
    virtual ~SoundMelodyPlayerBase(); // 0x0013E570 slot 0x00
    // the note m_NoteIndex; frames: the frames until the next step
    virtual void PlayNote(f32 frames); // 0x0032C6B4 slot 0x08

    // the melody (NULL: the melody of SoundMelodyMgr) with the sequence soundId
    void Start(u32 soundId, const u8* notes); // 0x0032C008
    void Stop(); // 0x0032C2E4
    // (the same code as Stop)
    void Finalize(); // 0x0032C934
    void Calc(); // 0x0032C4B0

    // the pitch of a note in a scale (0 outside the table)
    DECOMP_NOINLINE static s16 GetPitch(s32 scale, s32 note); // 0x0032C404

    // the pitches of the 19 notes (13 and the random ones) in the 10 scales
    static const s16 s_Pitches[10][19];
    // SKIP_TABLE: a note is played when the step count reaches its value
    static const s32 s_SkipSteps[NOTE_NUM];
    // SKIP_TABLE: the velocity of each note (1/127)
    static const f32 s_Velocities[NOTE_NUM];

    nw::snd::SoundHandle m_Handle; // 0x04
    f32 m_Volume;                  // 0x08
    const u8* m_pNotes;            // 0x0C
    u32 m_SoundId;                 // 0x10
    f32 m_NoteFrames;              // 0x14 the frames per step
    f32 m_LengthFrames;            // 0x18 the frames a note sounds
    s16 m_SkipMode;                // 0x1C SkipMode
    s16 m_IsKeepOnRest;            // 0x1E a rest does not stop the note
    s16 m_Scale;                   // 0x20
    s16 m_Transpose;               // 0x22
    u8 m_State;                    // 0x24 State
    s32 m_NoteIndex;               // 0x28
    s32 m_StepCount;               // 0x2C
    f32 m_Frame;                   // 0x30
    s32 m_Timer;                   // 0x34
    f32 m_NextNoteFrame;           // 0x38
    f32 m_NextStopFrame;           // 0x3C
    bool m_IsSkipChecked;          // 0x40 m_IsSkip is for this step
    bool m_IsSkip;                 // 0x41
    s32 m_LoadLevel;               // 0x44 the level of the sound data (-1: none)
    u8 m_IsPan;                    // 0x48 the pan follows the pitch
    bool m_IsLoadFailed;           // 0x49

private:
    // false at the end of the melody
    DECOMP_NOINLINE bool Advance(); // 0x0032BE68
    // the timing from the sequence variables (false: not valid)
    DECOMP_NOINLINE bool ReadParams(); // 0x0032C110
    // the step is skipped
    DECOMP_NOINLINE bool IsSkip(); // 0x0032C354
    void UnloadData();
};
ASSERT_OFFSET(SoundMelodyPlayerBase, m_State, 0x24);
ASSERT_OFFSET(SoundMelodyPlayerBase, m_LoadLevel, 0x44);
ASSERT_SIZE(SoundMelodyPlayerBase, 0x4C);
