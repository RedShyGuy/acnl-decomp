#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundHandle.h"

class SoundChordTracker;
class SoundObjBase;

// An instrument among the furniture: each touch plays a note of the chord of the room music (SoundChordTracker), near the
// note played before; the note goes to the sequence as variable 13 (the class is ours, nothing names it).
class SoundInstrument
{
public:
    enum
    {
        NOTE_MIN = -5,
        NOTE_MAX = 19,
        // the notes played within this many frames count as a phrase
        PHRASE_FRAMES = 30,
    };

    SoundInstrument(); // 0x002E9F9C
    ~SoundInstrument(); // 0x002E9FE0

    // isStopPrevious: a new note stops the note before
    void Initialize(bool isStopPrevious, SoundObjBase* obj); // 0x002E99B8
    DECOMP_NOINLINE s16 PickNote(); // 0x002E9A0C
    // note: from 0 (= NOTE_MIN)
    void PlayNoteAt(u32 soundId, s16 note); // 0x002E9D94
    void Update(); // 0x002E9DC4
    DECOMP_NOINLINE void PlayNote(u32 soundId, s16 note); // 0x002E9E44
    void Unregister(); // 0x002E9F20
    // the note played, from 0 (= NOTE_MIN)
    u8 Play(u32 soundId); // 0x002E9F54

    u32 m_Index;                   // 0x00 in the tracker
    SoundObjBase* m_pObj;          // 0x04
    SoundChordTracker* m_pTracker; // 0x08
    nw::snd::SoundHandle m_Handle; // 0x0C
    bool m_IsStopPrevious;         // 0x10
    s32 m_PhraseNotes;             // 0x14 the notes of the current phrase (SoundChordTracker::m_MinValue)
    s32 m_Frames;                  // 0x18 since the last note
    s16 m_LastNote;                // 0x1C
    u8 m_NearCount;                // 0x1E notes near the one before
    u8 m_SameCount;                // 0x1F the same note again
    s16 m_NoteMin;                 // 0x20
    s16 m_NoteMax;                 // 0x22
};
ASSERT_OFFSET(SoundInstrument, m_PhraseNotes, 0x14);
ASSERT_SIZE(SoundInstrument, 0x24);
