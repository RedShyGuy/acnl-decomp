#include "Sound/dSoundHum.h"
#include "Sound/dSoundHistoryMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundObjBase.h"
#include "netgame/dGameFunctions.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include <cstring>

// 0x0083B954
const u8 SoundHum::s_TuneWeights[3] = {3, 3, 1};
// 0x0083B957
const u8 SoundHum::s_TuneLengths[3] = {4, 2, 2};
// 0x0083B95A
const s16 SoundHum::s_Pitches[NOTE_KIND_NUM + 6] = {-5, -3, -1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, -6, -4, 1, 6, 10, 15};

namespace {
const s32 NO_PITCH = 100;
} // namespace

// 0x002E8A10
SoundHum::SoundHum()
    : m_pOwner(NULL), m_IsRequested(false), m_IsMelody(false), m_Frame(0), m_NoteIndex(0), m_IsNoteStart(false),
      m_Length(0), m_pMelody(NULL), m_NoteKindCount(0), m_NoteKinds(), m_IsRecent(false), m_IdleFrames(IDLE_FRAMES),
      m_IsBarStart(false), m_IsAlwaysMadeUp(false), m_PauseFrames(0)
{
    memset(m_Tune, NO_PITCH, sizeof(m_Tune));
}

s32 SoundHum::GetTunePitch(u8 note)
{
    switch (note) {
        case 0:
            return -5;
        case 1:
            return -3;
        case 2:
            return -1;
        case 4:
            return 2;
        case 5:
            return 4;
        case 6:
            return 5;
        case 7:
            return 7;
        case 8:
            return 9;
        case 9:
            return 11;
        case 10:
            return 12;
        case 11:
            return 14;
        case 12:
            return 16;
        default:
            return 0;
    }
}

// 0x002E83EC
void SoundHum::Begin()
{
    if (m_IsAlwaysMadeUp) {
        m_IsMelody = false;
        m_IsRecent = false;
    } else if (m_IsRecent) {
        m_IsRecent = false;
    } else if (m_pMelody == NULL) {
        m_IsMelody = false;
    } else {
        m_IsMelody = !m_IsMelody;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    const SoundMelody* melody = &mgr->m_MelodyMgr.m_Melody;
    u8 count = 0;
    for (s32 i = 0; i < MELODY_LENGTH; i++) {
        u8 note = melody->notes[i];
        if (note != SoundMelodyPlayerBase::NOTE_HOLD && note != SoundMelodyPlayerBase::NOTE_REST) {
            count++;
        }
    }
    if (count < 5) {
        melody = mgr->m_MelodyMgr.GetDefaultMelody(2);
    }
    m_pMelody = melody;
    if (m_IsMelody) {
        m_Length = MELODY_LENGTH;
    } else {
        MakeUpTune();
    }
    m_Frame = 0;
    m_NoteIndex = 0;
    m_IsNoteStart = true;
    m_IsBarStart = false;
}

// 0x002E84D0
void SoundHum::MakeUpTune()
{
    m_NoteKindCount = 0;
    u8 counts[16] = {0};
    for (s32 i = 0; i < MELODY_LENGTH; i++) {
        u8 note = m_pMelody->notes[i];
        if (note < NOTE_KIND_NUM) {
            counts[note]++;
        }
    }
    for (s32 i = 0; i < NOTE_KIND_NUM; i++) {
        if (counts[i] != 0) {
            m_NoteKinds[m_NoteKindCount] = i;
            m_NoteKindCount++;
        }
    }
    if (m_NoteKindCount == 0) {
        m_NoteKindCount = 1;
        m_NoteKinds[0] = 3;
    }
    m_Length = 0;
    u32 random = netgame::Func2FF63C(7);
    if (random < 3) {
        m_Length = 12;
    } else if (random < 6) {
        m_Length = 8;
    } else if (random < 7) {
        m_Length = 4;
    }
    if (m_Length == 0) {
        m_Length = 12;
    }
    u8 pos = 0;
    while (pos < m_Length) {
        u32 value = netgame::Func2FF63C(7);
        u8 sum = 0;
        for (s32 kind = 0; kind < 3; kind++) {
            sum += s_TuneWeights[kind];
            if (value >= sum || s_TuneLengths[kind] + pos > m_Length) {
                continue;
            }
            // two notes of two beats only after a held note
            if (pos != 0 && m_Tune[pos - 1] != NO_NOTE && kind == 2) {
                break;
            }
            if (kind == 0) {
                m_Tune[pos] = m_NoteKinds[netgame::Func2FF63C(m_NoteKindCount)];
                m_Tune[pos + 1] = NO_NOTE;
                m_Tune[pos + 2] = NO_NOTE;
                m_Tune[pos + 3] = NO_NOTE;
            } else if (kind == 1) {
                m_Tune[pos] = m_NoteKinds[netgame::Func2FF63C(m_NoteKindCount)];
                m_Tune[pos + 1] = NO_NOTE;
            } else if (kind == 2) {
                u8 note = m_NoteKinds[netgame::Func2FF63C(m_NoteKindCount)];
                m_Tune[pos] = note;
                m_Tune[pos + 1] = note;
            }
            pos += s_TuneLengths[kind];
            break;
        }
    }
}

// 0x002E86EC
void SoundHum::Update()
{
    if (m_pOwner == NULL) {
        return;
    }
    if (!m_IsRequested) {
        if (m_pMelody != NULL) {
            m_pMelody = NULL;
            m_IsRecent = true;
            m_IdleFrames = 0;
        } else if (m_IdleFrames < IDLE_FRAMES) {
            m_IdleFrames++;
        } else {
            m_IsRecent = false;
        }
        return;
    }
    m_pOwner->UpdateChanged();
    if (m_pMelody == NULL) {
        Begin();
        if (m_IsAlwaysMadeUp) {
            m_PauseFrames = netgame::Func2FF63C(3) * 2;
        }
    }
    s32 humming = SoundHistoryMgr::s_pInstance->Count(SE_ACT_FEEL_HAPPY);
    if (m_IsNoteStart && m_Length > m_NoteIndex) {
        s32 pitch = NO_PITCH;
        if (m_IsMelody) {
            u8 note = m_pMelody->notes[m_NoteIndex];
            if (note < NOTE_KIND_NUM) {
                pitch = s_Pitches[note];
            } else if (note == SoundMelodyPlayerBase::NOTE_RANDOM) {
                pitch = s_Pitches[NOTE_KIND_NUM + netgame::Func2FF63C(6)];
            }
        } else {
            u8 note = m_Tune[m_NoteIndex];
            if (note != NO_NOTE) {
                pitch = GetTunePitch(note);
            }
        }
        // at most two characters hum at the same time
        if (pitch != NO_PITCH && humming < 2) {
            SoundMgr::s_pInstance->StartSound(&m_Handle, SE_ACT_FEEL_HAPPY);
            if (m_Handle.m_pSound != NULL) {
                m_Handle.m_pSound->m_LpfFreq = -0.2f;
            }
            nw::snd::SequenceSoundHandle sequence(&m_Handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->WriteVariable(0, pitch - 9);
            }
            SoundHistoryMgr::s_pInstance->Add(SE_ACT_FEEL_HAPPY, 8);
        }
    }
    if (m_PauseFrames > 0) {
        m_PauseFrames--;
    } else {
        m_Frame++;
        if ((m_NoteIndex + 1) * 8 > static_cast<u32>(m_Frame)) {
            m_IsNoteStart = false;
            m_IsBarStart = false;
        } else {
            m_NoteIndex++;
            m_IsNoteStart = true;
            m_IsBarStart = (m_NoteIndex & 3) == 0;
        }
        if (m_NoteIndex >= static_cast<u32>(m_Length + 2)) {
            Begin();
        }
    }
    f32 volume = m_pOwner->m_Volume;
    if (humming != 0) {
        volume *= 0.7f;
    }
    m_Handle.SetVolume(volume, 0);
    m_Handle.SetPan(m_pOwner->m_Pan);
    m_Handle.SetSurroundPan(m_pOwner->m_SurroundPan);
    m_IsRequested = false;
}
