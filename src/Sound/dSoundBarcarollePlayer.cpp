#include "Sound/dSoundBarcarollePlayer.h"
#include "Sound/dSoundBarcarollePlayer_SingletonDisposer_.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadNew.h"

namespace {
const u32 END_TICK = 0x7FFFFFFF;
const u32 MATCHING_TRACK = 12;
const f32 FRAMES_PER_TICK = 37.5f;
const s16 NOTE_KIND_MATCHING = 6;
const u8 FIRST_SONG_PHRASE_WAIT = 45;
const u8 SECOND_SONG_PHRASE_WAIT = 30;
} // namespace

// 0x0094DCB8
SoundBarcarollePlayer* SoundBarcarollePlayer::s_pInstance = NULL;
// 0x0094DCBC
SoundBarcarollePlayer::SingletonDisposer_* SoundBarcarollePlayer::s_pSingletonDisposer = NULL;

// 0x0032BDBC
SoundBarcarollePlayer::SoundBarcarollePlayer()
    : m_State(STATE_NONE), m_NoteIndex(-1), m_Index(-1), m_BaseIndex(0), m_Frames(0), m_NoteKind(-1), m_Phrase(-1), _1EB8(0),
      m_StartWait(0), m_FirstEndWait(0), m_SecondWait(0), m_SecondEndWait(0), m_Part(0), m_IsSinging(false), m_Kind(0),
      m_Song(255), m_Variation(255)
{
}

// 0x0032B410
SoundBarcarollePlayer* SoundBarcarollePlayer::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundBarcarollePlayer)];
        SoundBarcarollePlayer* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundBarcarollePlayer();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x00123AB8
void SoundBarcarollePlayer::destroyInstance()
{
    if (s_pSingletonDisposer != NULL) {
        SingletonDisposer_* disposer = s_pSingletonDisposer;
        s_pSingletonDisposer = NULL;
        disposer->~SingletonDisposer_();
        if (s_pInstance != NULL) {
            s_pInstance->~SoundBarcarollePlayer();
            ::operator delete(s_pInstance);
        }
        s_pInstance = NULL;
    }
}

// 0x0032AB14
void SoundBarcarollePlayer::UpdateRandomMatching()
{
    if (m_StartWait != 0) {
        if (--m_StartWait == 0) {
            SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, s_MatchingSounds[m_Kind]);
        }
        return;
    }
    if (m_Handle.m_pSound == NULL) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    u32 tick = sequence.m_pSound != NULL ? sequence.m_pSound->GetTick() : 0;
    u32 now = tick + s_LookaheadTicks[m_Kind];
    u32 loopEnd = s_LoopEnds[m_Kind];
    if (loopEnd <= now) {
        u32 loopStart = s_LoopStarts[m_Kind];
        now = (now - loopStart) % (loopEnd - loopStart) + loopStart;
    }
    s16 index = m_Events.m_Count - 1;
    for (s16 i = -1; i < m_Events.m_Count - 1; i++) {
        if (m_Events.At(i + 1)->tick > now) {
            index = i;
            break;
        }
    }
    if (index >= 0 && m_Events.At(index)->tick + m_Events.At(index)->length >= now) {
        m_NoteKind = NOTE_KIND_MATCHING;
    } else {
        m_NoteKind = -1;
    }
}

// 0x0032ACE8
void SoundBarcarollePlayer::UpdateFirstSong()
{
    if (m_StartWait != 0) {
        u8 wait = --m_StartWait;
        if (m_Kind == KIND_GO && wait == FIRST_SONG_PHRASE_WAIT) {
            m_Phrase = 0;
        } else if (wait == 0) {
            SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, s_FirstSounds[m_Kind] + m_Song);
        }
        return;
    }
    if (m_FirstEndWait != 0) {
        if (--m_FirstEndWait == 0) {
            m_State = STATE_BETWEEN;
            m_BaseIndex = m_Index + 1;
        }
        return;
    }
    if (m_Handle.m_pSound == NULL) {
        m_Phrase = s_PhraseCounts[m_Kind].first;
        m_FirstEndWait = s_FirstEndWaits[m_Kind];
        m_NoteKind = -1;
        m_IsSinging = false;
        return;
    }

    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    u32 tick = sequence.m_pSound != NULL ? sequence.m_pSound->GetTick() : 0;
    s32 kind = m_Kind;
    u32 now = tick + s_LookaheadTicks[kind];
    s16 index;
    for (index = -1; index < m_Phrases.m_Count - 1; index++) {
        if (m_Phrases.At(index + 1)->tick > now) {
            break;
        }
    }
    m_Index = index;
    if (index < 0) {
        m_Frames = 0;
        m_NoteKind = -1;
    } else {
        m_Frames = m_Phrases.At(index)->frames;
        if (m_Events.At(index)->tick + m_Events.At(index)->length < now) {
            m_NoteKind = -1;
        } else {
            m_NoteKind = s_NoteKinds[m_Events.At(index)->table];
        }
    }
    m_NoteIndex = index;
    if (!m_IsSinging && s_SingTicks[kind] < now) {
        m_IsSinging = true;
    }
    if (m_Part == 0 && s_PartTicks[kind][0] <= now) {
        m_Part = 1;
    }
    if (m_Phrase < s_PhraseCounts[kind].first && s_PhraseTicks[kind][m_Phrase + 1] <= now) {
        m_Phrase++;
        if (m_Phrase == s_PhraseCounts[kind].first) {
            m_FirstEndWait = s_FirstEndWaits[kind];
            m_NoteKind = -1;
            m_IsSinging = false;
        }
    }
}

// 0x0032B084
void SoundBarcarollePlayer::UpdateSecondSong()
{
    if (m_SecondWait != 0) {
        u8 wait = --m_SecondWait;
        if (m_Kind == KIND_GO && wait == SECOND_SONG_PHRASE_WAIT) {
            m_Phrase++;
        } else if (wait == 0) {
            SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, s_FirstSounds[m_Kind] + m_Variation + SONG_NUM);
        }
        return;
    }
    if (m_Phrase == -1) {
        if (m_SecondEndWait != 0 && --m_SecondEndWait == 0) {
            m_State = STATE_NONE;
        }
        return;
    }
    if (m_Handle.m_pSound == NULL) {
        m_Phrase = -1;
        m_NoteKind = -1;
        m_IsSinging = false;
        return;
    }

    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    u32 tick = sequence.m_pSound != NULL ? sequence.m_pSound->GetTick() : 0;
    s32 kind = m_Kind;
    u32 now = tick + s_LookaheadTicks[kind];
    s16 index;
    for (index = -1; index < m_Phrases.m_Count - 1; index++) {
        if (m_Phrases.At(index + 1)->tick > now) {
            break;
        }
    }
    s16 total = m_BaseIndex + index;
    m_Index = total;
    if (index < 0) {
        m_Frames = 0;
        m_NoteKind = -1;
    } else {
        m_Frames = m_Phrases.At(index)->frames;
        if (m_Events.At(index)->tick + m_Events.At(index)->length < now) {
            m_NoteKind = -1;
        } else {
            m_NoteKind = s_NoteKinds[m_Events.At(index)->table];
        }
    }
    m_NoteIndex = total;
    if (m_Part == 2 && s_PartTicks[kind][1] <= now) {
        m_Part = 3;
    }
    if (m_Phrase != -1 && s_PhraseTicks[kind][m_Phrase + 1] <= now) {
        m_Phrase++;
        if (s_PhraseCounts[kind].second <= m_Phrase) {
            m_Phrase = -1;
            m_NoteKind = -1;
            m_IsSinging = false;
            if (kind != KIND_GO) {
                SoundMgr::s_pInstance->PlaySystemSoundChecked(SE_OBJ_SHIP_RUN_LAST_SONG_2, 0.0f);
            } else {
                SoundMgr::s_pInstance->PlaySystemSoundChecked(SE_OBJ_SHIP_RUN_LAST_SONG, 0.0f);
            }
        }
    }
}

// 0x0032B40C
void SoundBarcarollePlayer::UpdateNone()
{
    // empty (in the original too)
}

// 0x0032B478
void SoundBarcarollePlayer::PlaySecond()
{
    u32 soundId = s_FirstSounds[m_Kind] + m_Variation + SONG_NUM;
    Prepare(soundId, m_Kind, 1);
    m_SecondWait = 0;
    m_SecondEndWait = 0;
    m_Part = 2;
    m_IsSinging = true;
    u8 wait = s_SecondWaits[m_Kind];
    if (wait != 0) {
        m_SecondWait = wait;
    } else {
        SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, soundId);
    }
    m_SecondEndWait = s_SecondEndWaits[m_Kind];
    m_State = STATE_SECOND_SONG;
}

// 0x0032B548
void SoundBarcarollePlayer::Stop()
{
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(5);
    }
    m_State = STATE_NONE;
    m_Part = 0;
}

// 0x0032B580
void SoundBarcarollePlayer::Play(u32 kind, u32 song, u32 variation)
{
    if (song >= SONG_NUM || variation >= SONG_NUM) {
        return;
    }
    m_State = STATE_NONE;
    m_NoteIndex = -1;
    m_Index = -1;
    m_BaseIndex = 0;
    m_Frames = 0;
    m_NoteKind = -1;
    m_Phrase = -1;
    _1EB8 = 0;
    m_StartWait = 0;
    m_FirstEndWait = 0;
    m_SecondWait = 0;
    m_SecondEndWait = 0;
    m_Part = 0;
    m_IsSinging = false;
    u32 soundId = s_FirstSounds[kind] + song;
    Prepare(soundId, kind, 0);
    u8 wait = s_StartWaits[kind];
    if (wait != 0) {
        m_StartWait = wait;
    } else {
        SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, soundId);
    }
    m_Kind = kind;
    m_Song = song;
    m_Variation = variation;
    m_State = STATE_FIRST_SONG;
}

// 0x0032B768
void SoundBarcarollePlayer::Prepare(u32 soundId, s32 kind, s32 arg)
{
    (void)kind;
    (void)arg;
    SoundSeqParser* parser = SoundSeqParser::s_pInstance;
    if (parser == NULL) {
        return;
    }
    parser->Load(soundId);
    for (u8 i = 0; i < NOTE_TABLE_NUM; i++) {
        parser->Parse(m_Notes[i], i + 1, SoundSeqParser::MODE_LONGEST_NOTE, NOTE_NUM);
    }
    parser->Parse(m_Tempos, 0, SoundSeqParser::MODE_TEMPO, TEMPO_NUM);
    m_Phrases.Clear();
    m_Events.Clear();

    s32 positions[NOTE_TABLE_NUM] = {0, 0, 0, 0, 0, 0};
    while (m_Phrases.m_Count < m_Phrases.m_Capacity) {
        s32 table = 0;
        s32 tick = END_TICK;
        for (s32 i = 0; i < NOTE_TABLE_NUM; i++) {
            s32 entryTick = m_Notes[i][positions[i]].tick;
            if (entryTick < tick) {
                table = i;
                tick = entryTick;
            }
        }
        if (tick == static_cast<s32>(END_TICK)) {
            break;
        }
        Entry& entry = m_Notes[table][positions[table]];
        s32 end = entry.tempo + tick;

        // the length of the note in frames through the tempo changes
        s32 first = -1;
        s32 last = -1;
        for (s32 i = 0; i < TEMPO_NUM - 1; i++) {
            if (first == -1 && static_cast<s32>(m_Tempos[i + 1].tick) > tick) {
                first = i;
            }
            if (static_cast<s32>(m_Tempos[i + 1].tick) >= end) {
                last = i;
                break;
            }
        }
        s32 frames;
        if (first == -1 || last == -1) {
            frames = 1;
        } else {
            f32 sum = 0.0f;
            for (s32 i = first; i <= last; i++) {
                s32 from = m_Tempos[i].tick;
                if (from < tick) {
                    from = tick;
                }
                s32 to = m_Tempos[i + 1].tick;
                if (to > end) {
                    to = end;
                }
                sum += static_cast<f32>(to - from) * FRAMES_PER_TICK / static_cast<f32>(static_cast<s32>(m_Tempos[i].tempo));
            }
            frames = static_cast<s32>(sum);
        }
        Phrase phrase = {static_cast<u32>(tick), frames};
        m_Phrases.PushBack(phrase);

        NoteEvent note;
        note.tick = tick;
        note.length = entry.tempo;
        note.table = table;
        NoteEvent* lastNote = m_Events.Get(m_Events.m_Count - 1);
        if (lastNote != NULL) {
            u32 gap = tick - lastNote->tick;
            if (note.table == lastNote->table) {
                if (lastNote->length + 4 > gap) {
                    lastNote->length = gap - 4;
                }
            } else if (lastNote->length + 24 > gap) {
                lastNote->length = gap;
            }
        }
        m_Events.PushBack(note);
        positions[table]++;
    }
}

// 0x0032BAAC
void SoundBarcarollePlayer::StopNow(s32 arg)
{
    (void)arg;
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(0);
    }
    m_State = STATE_NONE;
}

// 0x0032BAD4
void SoundBarcarollePlayer::PlayRandomMatching(u32 kind)
{
    m_State = STATE_NONE;
    m_NoteIndex = -1;
    m_Index = -1;
    m_BaseIndex = 0;
    m_Frames = 0;
    m_NoteKind = -1;
    m_Phrase = -1;
    _1EB8 = 0;
    m_StartWait = 0;
    m_FirstEndWait = 0;
    m_SecondWait = 0;
    m_SecondEndWait = 0;
    m_Part = 0;
    m_IsSinging = false;
    u32 soundId = s_MatchingSounds[kind];
    PrepareRandomMatching(soundId, kind);
    u8 wait = s_StartWaits[kind];
    if (wait != 0) {
        m_StartWait = wait;
    } else {
        SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, soundId);
    }
    m_Kind = kind;
    m_State = STATE_RANDOM_MATCHING;
}

// 0x0032BBB8
void SoundBarcarollePlayer::PrepareRandomMatching(u32 soundId, s32 kind)
{
    SoundSeqParser* parser = SoundSeqParser::s_pInstance;
    if (parser == NULL) {
        return;
    }
    parser->Load(soundId);
    parser->Parse(m_MatchingNotes, MATCHING_TRACK, SoundSeqParser::MODE_LONGEST_NOTE, MATCHING_NOTE_NUM);
    parser->Parse(m_Tempos, 0, SoundSeqParser::MODE_TEMPO, TEMPO_NUM);
    m_Events.Clear();
    NoteEvent first;
    first.tick = m_MatchingNotes[0].tick;
    first.length = m_MatchingNotes[0].tempo;
    first.table = MATCHING_TRACK;
    m_Events.PushBack(first);
    for (s32 i = 1; i < MATCHING_NOTE_NUM; i++) {
        u32 tick = m_MatchingNotes[i].tick;
        if (tick == END_TICK) {
            break;
        }
        NoteEvent note;
        note.tick = tick;
        note.length = m_MatchingNotes[i].tempo;
        note.table = MATCHING_TRACK;
        if (m_Events.m_Count != 0) {
            NoteEvent* last = m_Events.Get(m_Events.m_Count - 1);
            if (last != NULL && last->length + s_MatchingGaps[kind] > tick - last->tick) {
                last->length = tick - last->tick;
            }
        }
        m_Events.PushBack(note);
    }
}

// 0x0032BD60
void SoundBarcarollePlayer::Update()
{
    if (!SoundMgr::s_pInstance->IsPaused()) {
        (this->*s_UpdateFuncs[m_State])();
    }
}
