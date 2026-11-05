#include "Sound/dSoundMelodyPlayerBase.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundMgr.h"
#include "netgame/dGameFunctions.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

// 0x0083C1B0
const s16 SoundMelodyPlayerBase::s_Pitches[10][19] = {
    {-5, -3, -1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, -6, -4, 1, 6, 10, 15},
    {-5, -4, -1, 0, 2, 3, 5, 7, 8, 11, 12, 14, 15, -6, -3, -2, 1, 4, 13},
    {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, -8, -7, -6, 8, 9, 10},
    {-11, -9, -7, -5, -3, -1, 1, 3, 5, 7, 9, 11, 13, -12, -10, -4, 0, 4, 10},
    {-5, -4, -1, 0, 1, 4, 5, 7, 8, 11, 12, 13, 16, -6, -3, 2, 6, 10, 15},
    {0, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 17, 18, 1, 4, 7, 10, 13, 16},
    {-5, -3, -3, 0, 2, 4, 7, 7, 9, 9, 12, 14, 16, -5, 0, 2, 9, 12, 14},
    {-6, -5, -3, -2, 0, 2, 3, 4, 5, 6, 7, 8, 9, -7, -4, -1, 1, 10, 11},
    {-10, -8, -6, -5, -3, -1, 0, 1, 4, 5, 6, 10, 10, -4, 9, 1, 3, 5, 8},
    {-5, -4, -2, 0, 2, 3, 5, 7, 8, 10, 12, 14, 15, -6, -3, -1, 1, 4, 13},
};
// 0x0083C32C
const s32 SoundMelodyPlayerBase::s_SkipSteps[NOTE_NUM] = {0, 2, 3, 4, 5, 6, 7, 9, 11, 15, 19, 25, 33, 41, 49, 57};
// 0x0083C36C
const f32 SoundMelodyPlayerBase::s_Velocities[NOTE_NUM] = {64.0f, 80.0f, 96.0f, 112.0f, 127.0f, 127.0f, 112.0f, 96.0f,
                                                           80.0f, 64.0f, 48.0f, 32.0f, 16.0f, 0.0f, 0.0f, 0.0f};

// 0x0012FCE8
SoundMelodyPlayerBase::SoundMelodyPlayerBase()
    : m_Volume(1.0f), m_pNotes(NULL), m_SoundId(0), m_State(STATE_NONE), m_LoadLevel(-1), m_IsPan(0),
      m_IsLoadFailed(false)
{
    m_NoteFrames = -1.0f;
    m_LengthFrames = -1.0f;
    m_SkipMode = -1;
    m_IsKeepOnRest = -1;
    m_Scale = -1;
    m_Transpose = -100;
    m_NoteIndex = -1;
    m_StepCount = -1;
    m_Frame = 0.0f;
    m_Timer = 0;
    m_NextNoteFrame = 0.0f;
    m_NextStopFrame = 0.0f;
    m_IsSkipChecked = false;
    m_IsSkip = false;
}

inline void SoundMelodyPlayerBase::UnloadData()
{
    if (m_LoadLevel != -1) {
        SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
        if (loadMgr != NULL && m_LoadLevel <= loadMgr->GetLevel(0) && loadMgr->UnloadOrDefer(m_LoadLevel, 0)) {
            m_LoadLevel = -1;
        }
    }
}

// 0x0013E570
// 0x0032C9A4 (deleting dtor)
SoundMelodyPlayerBase::~SoundMelodyPlayerBase()
{
    m_Handle.Stop(0);
    UnloadData();
    m_State = STATE_NONE;
}

// 0x0032BE68
bool SoundMelodyPlayerBase::Advance()
{
    if (m_Frame + 1.0f > m_NextStopFrame) {
        bool isLast = m_NoteIndex >= NOTE_NUM - 1;
        if (m_Handle.m_pSound != NULL) {
            if (static_cast<u32>(m_NoteIndex) >= NOTE_NUM) {
                m_NoteIndex = -1;
                return false;
            }
            if (isLast) {
                m_Handle.m_pSound->Stop(0);
            } else {
                u8 next = m_pNotes[m_NoteIndex + 1];
                if (next == NOTE_HOLD) {
                } else if (next == NOTE_REST) {
                    if (m_IsKeepOnRest == 0 && !IsSkip()) {
                        m_Handle.Stop(0);
                    }
                } else if (!IsSkip()) {
                    m_Handle.Stop(0);
                }
            }
        }
        if (isLast) {
            m_NoteIndex = -1;
            return false;
        }
        m_NextStopFrame += m_NoteFrames;
    }
    if (m_Frame + 1.0f > m_NextNoteFrame) {
        m_StepCount++;
        if (!IsSkip()) {
            m_NoteIndex++;
            PlayNote(m_NextNoteFrame - m_Frame);
        }
        m_IsSkipChecked = false;
        m_NextNoteFrame += m_NoteFrames;
    }
    m_Frame += 1.0f;
    return true;
}

// 0x0032C008
void SoundMelodyPlayerBase::Start(u32 soundId, const u8* notes)
{
    if (m_State != STATE_NONE) {
        // only a melody whose data could not be loaded starts again
        if (!m_IsLoadFailed || m_LoadLevel != -1) {
            return;
        }
        m_Handle.Stop(0);
        m_State = STATE_NONE;
    }
    m_NoteFrames = -1.0f;
    m_LengthFrames = -1.0f;
    m_SkipMode = -1;
    m_IsKeepOnRest = -1;
    m_Scale = -1;
    m_Transpose = -100;
    m_Volume = 1.0f;
    m_NoteIndex = -1;
    m_StepCount = -1;
    m_Frame = 0.0f;
    m_Timer = 0;
    m_NextNoteFrame = 0.0f;
    m_NextStopFrame = 0.0f;
    m_IsSkipChecked = false;
    m_IsSkip = false;
    m_SoundId = soundId;
    s32 level = SoundDataLoadMgr::s_pInstance->Load(soundId, 0);
    if (level == -1) {
        m_IsLoadFailed = true;
    } else {
        m_LoadLevel = level;
        m_IsLoadFailed = false;
    }
    if (notes != NULL) {
        m_pNotes = notes;
    } else {
        m_pNotes = SoundMgr::s_pInstance->m_MelodyMgr.m_Melody.notes;
    }
    m_State = STATE_LOAD;
}

// 0x0032C110
bool SoundMelodyPlayerBase::ReadParams()
{
    if (m_Handle.m_pSound == NULL) {
        m_State = STATE_END;
        m_Timer = 30;
        return false;
    }
    s16 noteFrames = -1;
    s16 lengthFrames = -1;
    s16 skipMode = -1;
    s16 isKeepOnRest = -1;
    s16 scale = -1;
    s16 transpose = -100;
    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(0, &noteFrames);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(1, &lengthFrames);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(2, &skipMode);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(3, &isKeepOnRest);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(4, &scale);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(5, &transpose);
    }
    if (noteFrames == -1 || lengthFrames == -1 || skipMode == -1 || isKeepOnRest == -1 || scale == -1 ||
        transpose == -100) {
        return false;
    }
    if (noteFrames <= 0 || lengthFrames < 0 || skipMode < 0 || skipMode > 3 || isKeepOnRest < 0 ||
        isKeepOnRest > 1 || scale < 0 || scale >= 10) {
        return false;
    }
    m_NoteFrames = noteFrames * 1.5f;
    m_NextNoteFrame = m_NoteFrames;
    if (noteFrames < lengthFrames) {
        lengthFrames = noteFrames;
    }
    if (lengthFrames != 0) {
        m_LengthFrames = lengthFrames * 1.5f;
        m_NextStopFrame = m_LengthFrames;
    } else {
        m_LengthFrames = 1.0f;
        m_NextStopFrame = 1.0f;
    }
    m_SkipMode = skipMode;
    m_IsKeepOnRest = isKeepOnRest;
    m_Scale = scale;
    m_Transpose = transpose;
    return true;
}

// 0x0032C2E4
void SoundMelodyPlayerBase::Stop()
{
    m_Handle.Stop(0);
    UnloadData();
    m_State = STATE_NONE;
}

// 0x0032C354
bool SoundMelodyPlayerBase::IsSkip()
{
    if (!m_IsSkipChecked) {
        m_IsSkipChecked = true;
        switch (m_SkipMode) {
        case SKIP_ALTERNATE:
            if (m_IsSkip) {
                m_IsSkip = false;
            } else if ((m_NoteIndex & 1) == 0) {
                m_IsSkip = true;
            }
            break;
        case SKIP_RANDOM:
            if (m_IsSkip) {
                m_IsSkip = false;
            } else if (netgame::Func2FF63C(2) == 0) {
                m_IsSkip = true;
            } else {
                m_IsSkip = false;
            }
            break;
        case SKIP_TABLE:
            if (m_StepCount < s_SkipSteps[m_NoteIndex]) {
                m_IsSkip = true;
            } else {
                m_IsSkip = false;
            }
            break;
        default:
            m_IsSkip = false;
            break;
        }
    }
    return m_IsSkip;
}

// 0x0032C404
s16 SoundMelodyPlayerBase::GetPitch(s32 scale, s32 note)
{
    if (scale < 10 && note < 19) {
        return s_Pitches[scale][note];
    }
    return 0;
}

// 0x0032C4B0
void SoundMelodyPlayerBase::Calc()
{
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    switch (m_State) {
    case STATE_LOAD: {
        if (loadMgr->IsBusy(0)) {
            return;
        }
        SoundMgr::s_pInstance->StartSound(&m_Handle, m_SoundId);
        nw::snd::SequenceSoundHandle sequence(&m_Handle);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteVariable(15, 1);
        }
        m_State = STATE_READ;
        m_NoteIndex = 0;
        break;
    }
    case STATE_UNLOAD:
        if (loadMgr->IsBusy(0)) {
            return;
        }
        UnloadData();
        m_State = STATE_NONE;
        break;
    case STATE_READ:
        if (!ReadParams()) {
            return;
        }
        m_Handle.Stop(0);
        PlayNote(0.0f);
        m_Frame += 1.0f;
        m_State = STATE_PLAY;
        break;
    case STATE_PLAY:
        if (Advance()) {
            return;
        }
        m_State = STATE_END;
        if (m_LoadLevel != -1) {
            m_Timer = 30;
        } else {
            m_Timer = 0;
        }
        break;
    case STATE_END:
        if (m_Handle.m_pSound != NULL || m_Timer > 0) {
            m_Timer--;
            return;
        }
        UnloadData();
        m_State = STATE_NONE;
        break;
    }
}

// 0x0032C6B4
void SoundMelodyPlayerBase::PlayNote(f32 frames)
{
    if (static_cast<u32>(m_NoteIndex) >= NOTE_NUM) {
        return;
    }
    if (static_cast<u16>(m_SkipMode) == SKIP_TABLE && m_NoteIndex >= 12) {
        return;
    }
    u8 note = m_pNotes[m_NoteIndex];
    if (note == NOTE_RANDOM) {
        note = static_cast<u8>(netgame::Func2FF63C(6) + NOTE_RANDOM);
    } else if (note == NOTE_HOLD || note == NOTE_REST) {
        return;
    }
    SoundMgr::s_pInstance->StartSound(&m_Handle, m_SoundId);
    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(15, 0);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(14, note);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(13, static_cast<s16>(static_cast<s32>(frames * 3.2f)));
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->WriteVariable(12, static_cast<s16>(s_Pitches[m_Scale][note] + m_Transpose));
    }
    if (m_IsPan == 1) {
        // the pan from the pitch between the lowest and the highest pitch of the scale
        const s16* pitches = s_Pitches[m_Scale];
        s16 lowest = 100;
        for (s32 i = 0; i < 19; i++) {
            if (lowest > pitches[i]) {
                lowest = pitches[i];
            }
        }
        s16 highest = -100;
        for (s32 i = 0; i < 19; i++) {
            if (highest < pitches[i]) {
                highest = pitches[i];
            }
        }
        f32 pan = SoundMgr::Lerp(pitches[note], lowest, highest, -0.5f, 0.5f);
        if (pan == 0.0f) {
            pan = 0.02f;
        }
        if (pan < -1.0f) {
            pan = -1.0f;
        } else if (pan > 1.0f) {
            pan = 1.0f;
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetPan(pan);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetSurroundPan(0.0f);
        }
    }
    if (static_cast<u16>(m_SkipMode) == SKIP_TABLE) {
        m_Volume = s_Velocities[m_NoteIndex] * (1.0f / 127.0f);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->SetVolume(m_Volume, 0);
    }
}

// 0x0032C934
void SoundMelodyPlayerBase::Finalize()
{
    m_Handle.Stop(0);
    UnloadData();
    m_State = STATE_NONE;
}
