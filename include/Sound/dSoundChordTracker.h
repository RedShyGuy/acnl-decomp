#pragma once

#include "decomp.h"
#include "Sound/dSoundSeqParser.h"

class SoundRoomMusicPlayer;

class SoundInstrument;

// The chord of the room music at the moment: the control variables 32 (kind) and 33 (root) of track 15 of the sequence are read
// beforehand, without them a fixed progression is used. The instrument furniture plays along with it (the class is ours).
class SoundChordTracker
{
public:
    enum
    {
        USER_NUM = 48,
        ENTRY_NUM = 70,
        PROGRESSION_NUM = 8,
    };
    typedef SoundSeqParser::Entry Entry;
    // a kind of chord
    struct ChordKind
    {
        u16 group; // 0x0 major, minor, diminished
        u16 entry; // 0x2
    };

    SoundChordTracker(); // 0x002A564C

    void Initialize(); // 0x002A5150
    void Start(u32 soundId); // 0x002A5164
    void OnUserPlayed(); // 0x002A51C8
    void Unregister(u32 index); // 0x002A51FC
    void UpdateCounters(); // 0x002A522C
    void Stop(); // 0x002A52E0
    void Update(); // 0x002A5314
    // the notes of the chord and its root
    void GetChord(const u16** notes, u16* root); // 0x002A53CC
    u32 Register(SoundInstrument* user); // 0x002A55F8
    // the users have been playing for a while, or the gyroids have come to the last levels (name is ours; nothing
    // in the program refers to it directly)
    bool IsSessionActive() const; // 0x0071CD1C

    static const u16* s_Chords[3][10];
    static const ChordKind s_ProgressionKinds[PROGRESSION_NUM];
    static const u16 s_ProgressionRoots[PROGRESSION_NUM];

    SoundRoomMusicPlayer* m_pPlayer;  // 0x000
    SoundInstrument* m_pUsers[USER_NUM]; // 0x004
    u32 m_UserNum;                    // 0x0C4
    s32 m_MinValue;                   // 0x0C8 the smallest value of the users (read by SoundHaniwaMgr)
    s16 m_ChordGroup;                 // 0x0CC
    s16 m_ChordEntry;                 // 0x0CE
    u16 m_ChordRoot;                  // 0x0D0
    bool m_IsChordSet;                // 0x0D2
    Entry m_KindEntries[ENTRY_NUM];   // 0x0D4
    Entry m_RootEntries[ENTRY_NUM];   // 0x304
    bool m_IsParsed;                  // 0x534
    u32 m_Frames;                     // 0x538
    u32 m_PlayCount;                  // 0x53C
    u32 m_FramesSincePlay;            // 0x540
    bool m_IsCounting;                // 0x544
    bool m_IsActive;                  // 0x545
    s32 m_ActiveFrames;               // 0x548
    s16 m_LastBeat;                   // 0x54C
    s16 m_BeatCount;                  // 0x54E
};
ASSERT_OFFSET(SoundChordTracker, m_MinValue, 0xC8);
ASSERT_OFFSET(SoundChordTracker, m_KindEntries, 0xD4);
ASSERT_OFFSET(SoundChordTracker, m_RootEntries, 0x304);
ASSERT_OFFSET(SoundChordTracker, m_IsParsed, 0x534);
ASSERT_OFFSET(SoundChordTracker, m_LastBeat, 0x54C);
ASSERT_SIZE(SoundChordTracker, 0x550);
