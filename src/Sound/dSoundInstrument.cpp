#include "Sound/dSoundInstrument.h"
#include "Sound/dSoundChordTracker.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundObjBase.h"
#include "Sound/dSoundRoomMusicPlayer.h"
#include "netgame/dGameFunctions.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadNew.h"
#include <new>

namespace {
const s32 VARIABLE_NOTE = 13;
const s32 CANDIDATE_NUM = 60;
const u32 OCTAVE = 12;
// a chord ends with the octave
const s16 CHORD_END = 12;
// after this many notes near each other all notes of the chord are candidates
const u8 NEAR_COUNT_WIDE = 12;

// a ring of notes in a buffer of the heap (name is ours)
struct NoteRing
{
    NoteRing() : m_pData(NULL), m_Capacity(0), m_Head(0), m_Count(0) {}
    ~NoteRing()
    {
        if (m_pData != NULL) {
            delete[] m_pData;
        }
    }
    void Allocate(s32 capacity)
    {
        s16* data = static_cast<s16*>(::operator new(capacity * sizeof(s16), static_cast<sead::Heap*>(NULL), 4, std::nothrow));
        if (data != NULL) {
            m_Capacity = capacity;
            m_pData = data;
            m_Head = 0;
        }
    }
    s16* At(u32 i)
    {
        if (static_cast<u32>(m_Count) <= i) {
            return m_pData;
        }
        s32 pos = i + m_Head;
        if (m_Capacity <= pos) {
            pos -= m_Capacity;
        }
        return &m_pData[pos];
    }
    void PushBack(s16 note)
    {
        if (m_Count < m_Capacity) {
            s32 pos = m_Count++ + m_Head;
            if (pos >= m_Capacity) {
                pos -= m_Capacity;
            }
            m_pData[pos] = note;
        }
    }

    s16* m_pData;
    s32 m_Capacity;
    s32 m_Head;
    s32 m_Count;
};
} // namespace

// 0x002E9F9C
SoundInstrument::SoundInstrument()
    : m_Index(0xFFFFFFFF), m_pObj(NULL), m_pTracker(NULL), m_IsStopPrevious(false), m_PhraseNotes(0), m_Frames(0),
      m_LastNote(0), m_NearCount(0), m_SameCount(0), m_NoteMin(NOTE_MIN), m_NoteMax(NOTE_MAX)
{
}

// 0x002E9FE0
SoundInstrument::~SoundInstrument()
{
    if (m_Index < SoundChordTracker::USER_NUM) {
        m_pTracker->Unregister(m_Index);
        m_pTracker = NULL;
        m_Index = 0xFFFFFFFF;
        m_pObj = NULL;
    }
}

// 0x002E99B8
void SoundInstrument::Initialize(bool isStopPrevious, SoundObjBase* obj)
{
    if (m_Index != 0xFFFFFFFF || SoundRoomMusicPlayer::s_pInstance == NULL) {
        return;
    }
    m_pTracker = &SoundRoomMusicPlayer::s_pInstance->m_Chords;
    if (m_pTracker == NULL) {
        return;
    }
    m_Index = m_pTracker->Register(this);
    m_IsStopPrevious = isStopPrevious;
    m_pObj = obj;
}

// 0x002E9A0C
s16 SoundInstrument::PickNote()
{
    const u16* chord = NULL;
    if (m_pTracker == NULL) {
        return 0;
    }
    u16 root;
    m_pTracker->GetChord(&chord, &root);
    if (chord == NULL) {
        return 0;
    }

    // the notes of the chord below and above the note before
    NoteRing below;
    below.Allocate(CANDIDATE_NUM);
    NoteRing above;
    above.Allocate(CANDIDATE_NUM);
    bool isSameCandidate = false;
    for (s16 octave = root - 2 * OCTAVE; octave < 3 * static_cast<s32>(OCTAVE); octave += OCTAVE) {
        for (const s16* note = reinterpret_cast<const s16*>(chord); *note < CHORD_END; note++) {
            s16 value = *note + octave;
            if (value < m_LastNote) {
                if (value >= NOTE_MIN) {
                    below.PushBack(value);
                }
            } else if (value == m_LastNote) {
                if (m_SameCount == 0) {
                    isSameCandidate = true;
                }
            } else {
                if (value > NOTE_MAX) {
                    goto collected;
                }
                above.PushBack(value);
            }
        }
    }
collected:
    // the nearest two on each side (all after many near notes) and the same note
    NoteRing candidates;
    s32 candidateNum = below.m_Count + above.m_Count + 1;
    if (candidateNum > 0) {
        candidates.Allocate(candidateNum);
    }
    for (s32 i = 0; i < below.m_Count; i++) {
        candidates.PushBack(*below.At(below.m_Count - i - 1));
        if (m_NearCount < NEAR_COUNT_WIDE && i >= 1) {
            break;
        }
    }
    for (s32 i = 0; i < above.m_Count; i++) {
        candidates.PushBack(*above.At(i));
        if (m_NearCount < NEAR_COUNT_WIDE && i >= 1) {
            break;
        }
    }
    if (isSameCandidate && candidates.m_Count < candidates.m_Capacity) {
        candidates.PushBack(m_LastNote);
    }
    s16 result = m_LastNote;
    if (candidates.m_Count != 0) {
        result = *candidates.At(netgame::Func2FF63C(candidates.m_Count));
    }
    return result;
}

// 0x002E9D94
void SoundInstrument::PlayNoteAt(u32 soundId, s16 note)
{
    if (m_pTracker != NULL) {
        PlayNote(soundId, note + NOTE_MIN);
    }
}

// 0x002E9DC4
void SoundInstrument::Update()
{
    if (m_Handle.m_pSound != NULL) {
        m_pObj->UpdateChanged();
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetVolume(m_pObj->m_Volume, 0);
        }
        m_Handle.SetPan(m_pObj->m_Pan);
        m_Handle.SetSurroundPan(m_pObj->m_SurroundPan);
    }
    if (++m_Frames >= PHRASE_FRAMES) {
        m_PhraseNotes = 0;
    }
}

// 0x002E9E44
void SoundInstrument::PlayNote(u32 soundId, s16 note)
{
    if (m_IsStopPrevious && m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(0);
    }
    SoundMgr::s_pInstance->StartSound(&m_Handle, soundId);
    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(VARIABLE_NOTE, note);
    }
    m_pTracker->OnUserPlayed();
    s16 last = m_LastNote;
    if (last - 1 > note || note > last + 2) {
        m_NearCount = 0;
        m_SameCount = 0;
    } else {
        m_NearCount++;
        if (last != note) {
            m_SameCount = 0;
        } else {
            m_SameCount++;
        }
    }
    if (m_Frames < PHRASE_FRAMES) {
        m_PhraseNotes++;
    } else {
        m_PhraseNotes = 1;
    }
    m_LastNote = note;
    m_Frames = 0;
}

// 0x002E9F20
void SoundInstrument::Unregister()
{
    if (m_Index < SoundChordTracker::USER_NUM) {
        m_pTracker->Unregister(m_Index);
        m_pTracker = NULL;
        m_Index = 0xFFFFFFFF;
        m_pObj = NULL;
    }
}

// 0x002E9F54
u8 SoundInstrument::Play(u32 soundId)
{
    if (m_pTracker == NULL) {
        return 0;
    }
    s16 note = PickNote();
    PlayNote(soundId, note);
    if (note >= NOTE_MIN) {
        return note - NOTE_MIN;
    }
    return 0;
}
