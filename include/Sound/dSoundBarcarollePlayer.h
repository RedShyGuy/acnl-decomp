#pragma once

#include "decomp.h"
#include "Sound/dSoundLivePlayer.h"
#include "Sound/dSoundRing.h"
#include "Sound/dSoundSeqParser.h"
#include "nw/snd/snd_SoundHandle.h"

namespace sead { class Heap; }

// Kapp'n's songs on the boat to the island and back: a first and a second song (SEQ_BGM_KAPPEI_GO_* / _BACK_*), and the
// song of the random matching. The notes of the sequence are read beforehand so that Kapp'n can sing along (the class name is
// from the RTTI of its disposer, everything else is ours).
class SoundBarcarollePlayer
{
public:
    class SingletonDisposer_;

    // m_Kind
    enum Kind
    {
        KIND_GO = 0,
        KIND_BACK = 1,
        KIND_NUM = 2,
    };
    // m_State
    enum State
    {
        STATE_NONE = 0,
        STATE_FIRST_SONG = 1,
        STATE_BETWEEN = 2,
        STATE_SECOND_SONG = 3,
        STATE_RANDOM_MATCHING = 4,
        STATE_NUM = 5,
    };
    enum
    {
        SONG_NUM = 30,
        NOTE_TABLE_NUM = 6,
        NOTE_NUM = 30,
        TEMPO_NUM = 50,
        MATCHING_NOTE_NUM = 270,
        PHRASE_NUM = 70,
    };
    typedef SoundSeqParser::Entry Entry;
    typedef SoundLivePlayer::NoteEvent NoteEvent;
    // a note with its length in frames
    struct Phrase
    {
        u32 tick;   // 0x0
        s32 frames; // 0x4
    };
    struct PhraseCount
    {
        s16 first;  // 0x0 of the first song
        s16 second; // 0x2 of both songs
    };

    SoundBarcarollePlayer(); // 0x0032BDBC
    ~SoundBarcarollePlayer()
    {
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->Stop(0);
        }
    }
    static SoundBarcarollePlayer* createInstance(sead::Heap* heap); // 0x0032B410
    static void destroyInstance(); // 0x00123AB8

    // (the kind and the second argument are not used)
    DECOMP_NOINLINE void Prepare(u32 soundId, s32 kind, s32 arg); // 0x0032B768
    DECOMP_NOINLINE void PrepareRandomMatching(u32 soundId, s32 kind); // 0x0032BBB8
    void PlaySecond(); // 0x0032B478
    void Stop(); // 0x0032B548
    void Play(u32 kind, u32 song, u32 variation); // 0x0032B580
    // (the argument is not used)
    void StopNow(s32 arg); // 0x0032BAAC
    void PlayRandomMatching(u32 kind); // 0x0032BAD4
    void Update(); // 0x0032BD60
    void UpdateNone(); // 0x0032B40C
    void UpdateFirstSong(); // 0x0032ACE8
    void UpdateSecondSong(); // 0x0032B084
    void UpdateRandomMatching(); // 0x0032AB14
    DECOMP_EMIT_COPY u8 GetPart() const // 0x00725BF4
    {
        if (m_State == STATE_NONE) {
            return 0;
        }
        if (m_State == STATE_BETWEEN) {
            return 1;
        }
        return m_Part;
    }

    static const u8 s_StartWaits[KIND_NUM];
    static const u8 s_FirstEndWaits[KIND_NUM];
    static const u8 s_SecondWaits[KIND_NUM];
    static const u8 s_SecondEndWaits[KIND_NUM];
    static const u32 s_SingTicks[KIND_NUM];
    static const u32 s_LookaheadTicks[KIND_NUM];
    static const u32 s_MatchingGaps[KIND_NUM];
    static const u32 s_FirstSounds[KIND_NUM];
    static const PhraseCount s_PhraseCounts[KIND_NUM];
    static const u32 s_MatchingSounds[KIND_NUM];
    static const u32 s_LoopStarts[KIND_NUM];
    static const u32 s_LoopEnds[KIND_NUM];
    static void (SoundBarcarollePlayer::*const s_UpdateFuncs[STATE_NUM])();
    static const u32 s_PhraseTicks[KIND_NUM][5];
    static const u32 s_PartTicks[KIND_NUM][2];
    static const s16 s_NoteKinds[NOTE_TABLE_NUM];
    static SoundBarcarollePlayer* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];               // 0x0000
    u8 m_State;                                     // 0x0010 State
    s16 m_NoteIndex;                                // 0x0012
    s16 m_Index;                                    // 0x0014
    s16 m_BaseIndex;                                // 0x0016
    s32 m_Frames;                                   // 0x0018 of the current note
    Entry m_Notes[NOTE_TABLE_NUM][NOTE_NUM];        // 0x001C
    Entry m_Tempos[TEMPO_NUM];                      // 0x05BC
    Entry m_MatchingNotes[MATCHING_NOTE_NUM];       // 0x074C
    SoundRing<Phrase, PHRASE_NUM> m_Phrases;        // 0x0FBC
    SoundRing<NoteEvent, MATCHING_NOTE_NUM> m_Events; // 0x11FC
    s16 m_NoteKind;                                 // 0x1EB4
    s16 m_Phrase;                                   // 0x1EB6
    s16 _1EB8;                                      // 0x1EB8
    u8 m_StartWait;                                 // 0x1EBA
    u8 m_FirstEndWait;                              // 0x1EBB
    u8 m_SecondWait;                                // 0x1EBC
    u8 m_SecondEndWait;                             // 0x1EBD
    u8 m_Part;                                      // 0x1EBE
    bool m_IsSinging;                               // 0x1EBF
    s8 m_Kind;                                      // 0x1EC0 Kind
    u8 m_Song;                                      // 0x1EC1
    u8 m_Variation;                                 // 0x1EC2
    nw::snd::SoundHandle m_Handle;                  // 0x1EC4
};
ASSERT_OFFSET(SoundBarcarollePlayer, m_Notes, 0x1C);
ASSERT_OFFSET(SoundBarcarollePlayer, m_Tempos, 0x5BC);
ASSERT_OFFSET(SoundBarcarollePlayer, m_MatchingNotes, 0x74C);
ASSERT_OFFSET(SoundBarcarollePlayer, m_Phrases, 0xFBC);
ASSERT_OFFSET(SoundBarcarollePlayer, m_Events, 0x11FC);
ASSERT_OFFSET(SoundBarcarollePlayer, m_NoteKind, 0x1EB4);
ASSERT_OFFSET(SoundBarcarollePlayer, m_StartWait, 0x1EBA);
ASSERT_OFFSET(SoundBarcarollePlayer, m_Kind, 0x1EC0);
ASSERT_OFFSET(SoundBarcarollePlayer, m_Handle, 0x1EC4);
ASSERT_SIZE(SoundBarcarollePlayer, 0x1EC8);
