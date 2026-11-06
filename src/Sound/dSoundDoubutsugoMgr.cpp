#include "Sound/dSoundDoubutsugoMgr.h"
#include "Other/dRegion.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "netgame/dGameFunctions.h"
#include "stage/stage_Extern.h"

namespace {
inline bool IsSyllableSound(u32 soundId)
{
    return soundId - SE_DOU_BOY_000_A_L <= SoundDoubutsugoMgr::SYLLABLE_NUM - 1;
}
} // namespace

// 0x00128D1C
SoundDoubutsugoMgr::SoundDoubutsugoMgr()
    : m_HandleIndex(0), m_Language(LANGUAGE_JAPANESE), m_Mood(0), m_Voice(5), m_CharFrames(2), m_Level(1), m_Queue(),
      m_QueueRead(0), m_QueueNext(1), m_QueueWrite(0), m_QueueCount(0), m_LastSyllable(0), m_IsSyllable(false),
      m_Pitch(1.0f), m_IdleFrames(0), m_StepFrame(0), m_IsSpeaking(false), m_IsFalling(false), m_PhaseFrame(0),
      m_PhaseFrames(20), m_Effect(EFFECT_NONE), m_EffectPitch(1.0f), m_IsVoicePlaying(false), m_IsFx6(false),
      m_IsFilterRequested(false), m_IsFilterOn(false)
{
    for (s32 i = 0; i < QUEUE_SIZE; i++) {
        m_QueueFrames[i] = 0;
    }
    m_IsEffectEnabled[0] = true;
    m_IsEffectEnabled[1] = true;
    m_IsEffectEnabled[2] = true;
    m_IsEffectEnabled[3] = true;
    m_IsEffectEnabled[4] = false;
    m_IsEffectEnabled[5] = false;
    m_IsEffectEnabled[6] = false;
    m_IsEffectEnabled[7] = false;
    _A4 = false;
    UpdateParam();
    Reset();
}

// 0x002E829C
// 0x002E824C (deleting dtor)
SoundDoubutsugoMgr::~SoundDoubutsugoMgr()
{
    // (only the members are destroyed)
}

void SoundDoubutsugoMgr::UpdateParam()
{
    const VoiceParam* param = s_pVoiceParams[m_Voice][m_Mood];
    m_pParam = param;
    m_StepRiseFrames = static_cast<u32>(PITCH_STEPS) / (param->steps + 1u);
    m_StepScale = static_cast<f32>(param->steps + 1) / (static_cast<f32>(param->steps) * 12.0f);
}

// 0x0012FAC8
void SoundDoubutsugoMgr::Reset()
{
    m_IsSpeaking = false;
    const VoiceParam* param = m_pParam;
    m_Pitch = param->pitchMax + 1.0f;
    if (param->isStartMid == 1) {
        m_StepFrame = m_StepRiseFrames;
    } else {
        m_StepFrame = 0;
    }
    m_IsFalling = false;
    m_PhaseFrame = 0;
    if (param->riseFramesMax > param->riseFramesMin) {
        m_PhaseFrames = netgame::Func2FF63C(param->riseFramesMax - param->riseFramesMin) + m_pParam->riseFramesMin;
    } else {
        m_PhaseFrames = param->riseFramesMin;
    }
    m_StepRange = m_pParam->pitchMax;
    m_StepFall = m_StepRange * m_StepScale;
    if (m_IsFilterOn && !m_IsFilterRequested) {
        m_IsFilterOn = false;
    }
}

// 0x002E703C
void SoundDoubutsugoMgr::SetMood(u32 mood)
{
    if (mood >= MOOD_NUM) {
        return;
    }
    if (m_Mood == mood) {
        return;
    }
    m_Mood = mood;
    UpdateParam();
    Reset();
}

// 0x002E70C8
void SoundDoubutsugoMgr::SetVoices(const sead::CheckedArray<u8, 4>& voices)
{
    m_Voice = 0;
    m_ExtraGroups.Clear();
    s32 num = 0;
    for (u32 i = 0; i < 4; i++) {
        if (voices[i] >= VOICE_NUM || voices[i] == 0) {
            continue;
        }
        if (m_Voice == 0) {
            num++;
            m_Voice = voices[i];
            continue;
        }
        num++;
        if (s_VoiceGroups[m_Voice] == s_VoiceGroups[voices[i]]) {
            continue;
        }
        bool isKnown = false;
        for (s32 j = 0; j < m_ExtraGroups.m_Count; j++) {
            if (*m_ExtraGroups.At(j) == s_VoiceGroups[voices[i]]) {
                isKnown = true;
                break;
            }
        }
        if (!isKnown) {
            m_ExtraGroups.PushBack(s_VoiceGroups[voices[i]]);
        }
    }
    // every speaker after the first one is a voice along
    while (m_ExtraGroups.m_Count < num - 1) {
        m_ExtraGroups.PushBack(s_VoiceGroups[m_Voice]);
    }
    UpdateParam();
    Reset();
}

// 0x002E72E8
void SoundDoubutsugoMgr::SetCharFrames(u32 frames)
{
    if (frames < QUEUE_SIZE && frames != 0) {
        m_CharFrames = frames;
    }
}

// 0x002E72FC
void SoundDoubutsugoMgr::OnEnterStage()
{
    switch (GetRegion()) {
        case 0:
            m_Language = LANGUAGE_JAPANESE;
            break;
        case 1:
        case 4:
            m_Language = LANGUAGE_ENGLISH;
            break;
        case 2:
        case 5:
            m_Language = LANGUAGE_SPANISH;
            break;
        case 3:
        case 6:
            m_Language = LANGUAGE_FRENCH;
            break;
        case 7:
            m_Language = LANGUAGE_ITALIAN;
            break;
        case 8:
            m_Language = LANGUAGE_GERMAN;
            break;
        case 9:
            m_Language = LANGUAGE_KOREAN;
            break;
    }
}

void SoundDoubutsugoMgr::Pop()
{
    if (m_QueueCount > 0) {
        m_Queue[m_QueueRead] = 0;
        m_QueueFrames[m_QueueRead] = 0;
        m_QueueCount--;
        m_QueueRead = m_QueueNext;
        m_QueueNext++;
        if (m_QueueNext == QUEUE_SIZE) {
            m_QueueNext = 0;
        }
    }
}

u32 SoundDoubutsugoMgr::GetLanguageSound(u32 c, u32 next) const
{
    switch (m_Language) {
        case LANGUAGE_ENGLISH:
            return GetEnglishSound(c, next);
        case LANGUAGE_GERMAN:
            return GetGermanSound(c, next);
        case LANGUAGE_FRENCH:
            return GetFrenchSound(c, next);
        case LANGUAGE_ITALIAN:
            return GetItalianSound(c, next);
        case LANGUAGE_SPANISH:
            return GetSpanishSound(c, next);
        default:
            return 0;
    }
}

// 0x002E737C
void SoundDoubutsugoMgr::Play(u32 c, u32 next)
{
    m_IsSpeaking = true;
    if (m_IdleFrames >= IDLE_RESET_FRAMES) {
        Reset();
    }
    switch (c) {
        case CHAR_LONG:
            if (m_IsEffectEnabled[0]) {
                m_Effect = EFFECT_HOLD;
            }
            break;
        case CHAR_DOT:
            if (m_IsEffectEnabled[1]) {
                m_Effect = EFFECT_NO_PITCH;
            }
            break;
        case CHAR_MONOLOGUE:
            m_Effect = EFFECT_NO_PITCH;
            break;
        case 160:
            if (m_IsEffectEnabled[2]) {
                m_Effect = EFFECT_RISE;
            }
            break;
        case 161:
            if (m_IsEffectEnabled[3]) {
                m_Effect = EFFECT_RISE;
            }
            break;
        case 162:
        case 165:
            if (m_IsEffectEnabled[4]) {
                m_Effect = EFFECT_CUT;
            }
            break;
        case 164:
            if (m_IsEffectEnabled[5]) {
                m_Effect = EFFECT_CUT;
            }
            break;
        case '#':
            if (m_IsEffectEnabled[6]) {
                m_Effect = EFFECT_CUT;
            }
            break;
        case 166:
            if (m_IsEffectEnabled[7]) {
                m_Effect = EFFECT_CUT;
            }
            break;
        default:
            m_Effect = EFFECT_NONE;
            goto applied;
    }
    if (m_Effect != EFFECT_NONE) {
        m_EffectFrame = 0;
    }
applied:
    if (m_Effect == EFFECT_RISE) {
        m_EffectPitch = m_Pitch;
    } else if (m_Effect == EFFECT_CUT) {
        m_Handles[m_HandleIndex].Stop(0);
        for (s32 i = 0; i < m_ExtraGroups.m_Count; i++) {
            m_ExtraHandles[i][m_HandleIndex].Stop(0);
        }
    }
    u32 chars[2];
    chars[0] = ConvertChar(c);
    chars[1] = ConvertChar(next);
    u32 sound = GetSound(chars[0], chars[1], true);
    if (sound == 0) {
        m_IsSyllable = false;
        return;
    }
    if (sound == SE_SYS_SCRIPT_DOT || sound == SE_SYS_SCRIPT_MONOLOGY) {
        m_IsSyllable = false;
    } else {
        m_LastSyllable = sound;
        m_IsSyllable = true;
    }
    u32 played = sound;
    bool isSyllable = IsSyllableSound(sound);
    if (isSyllable) {
        played = sound + s_VoiceGroups[m_Voice] * SYLLABLE_NUM;
    }
    // the sounds before fade out
    m_Handles[m_HandleIndex].SetVolume(0.0f, 2);
    for (s32 i = 0; i < m_ExtraGroups.m_Count; i++) {
        m_ExtraHandles[i][m_HandleIndex].SetVolume(0.0f, 2);
    }
    m_HandleIndex++;
    if (m_HandleIndex >= HANDLE_NUM) {
        m_HandleIndex = 0;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    mgr->StartSound(&m_Handles[m_HandleIndex], played);
    if (m_Effect != EFFECT_NO_PITCH) {
        m_Handles[m_HandleIndex].SetPitch(m_Pitch * m_pParam->pitchScale);
    }
    s32 extraNum = m_ExtraGroups.m_Count;
    if (extraNum > 0) {
        m_Handles[m_HandleIndex].SetVolume(0.5f, 0);
    } else {
        m_Handles[m_HandleIndex].SetVolume(m_pParam->volume * s_LevelVolumes[m_Level], 0);
    }
    for (s32 i = 0; i < extraNum; i++) {
        u32 extra = sound;
        if (isSyllable) {
            extra = sound + *m_ExtraGroups.At(i) * SYLLABLE_NUM;
        }
        mgr->StartSound(&m_ExtraHandles[i][m_HandleIndex], extra);
        if (m_Effect != EFFECT_NO_PITCH) {
            m_ExtraHandles[i][m_HandleIndex].SetPitch(m_Pitch * m_pParam->pitchScale * s_ExtraPitches[i]);
        }
        m_ExtraHandles[i][m_HandleIndex].SetVolume(0.5f, 0);
        if (m_ExtraHandles[i][m_HandleIndex].m_pSound != NULL) {
            m_ExtraHandles[i][m_HandleIndex].m_pSound->SetPlayerPriority(32);
        }
    }
    // the effect send of the stage
    SoundFxMgr& fx = mgr->m_FxMgr;
    if (fx.m_State == SoundFxMgr::STATE_ON) {
        f32 send = 0.0f;
        s32 type = fx.m_FxType;
        if (m_IsFx6) {
            if (type != SoundFxMgr::FX_NONE && type < SoundFxMgr::FX_NUM) {
                send = s_FxSends[type];
            }
        } else if (type == SoundFxMgr::FX_DREAM) {
            if (m_IsSyllable) {
                send = 0.2f;
            }
        } else if (type == SoundFxMgr::FX_7 || type == SoundFxMgr::FX_8) {
            send = 0.4f;
        } else if (type == SoundFxMgr::FX_TRAIN && m_Voice == 27) {
            send = 0.25f;
        }
        if (send > 0.0f) {
            if (m_Handles[m_HandleIndex].m_pSound != NULL) {
                m_Handles[m_HandleIndex].m_pSound->SetFxSend(nw::snd::AUX_BUS_A, send);
            }
            for (s32 i = 0; i < extraNum; i++) {
                if (m_ExtraHandles[i][m_HandleIndex].m_pSound != NULL) {
                    m_ExtraHandles[i][m_HandleIndex].m_pSound->SetFxSend(nw::snd::AUX_BUS_A, send);
                }
            }
        }
    }
    if (m_IsFilterOn) {
        m_Handles[m_HandleIndex].SetBiquadFilter(70, 1.0f);
    }
    // a pair of letters takes the next character too
    if (m_Language >= LANGUAGE_ENGLISH && m_Language <= LANGUAGE_SPANISH) {
        if (GetLanguageSound(chars[0], chars[1]) != 0 && m_QueueCount > 0) {
            Pop();
        }
    }
}

// 0x002E7A3C
void SoundDoubutsugoMgr::SetFilter(bool isOn)
{
    m_IsFilterRequested = isOn;
    if (isOn) {
        m_IsFilterOn = true;
    }
}

// 0x002E7A50
void SoundDoubutsugoMgr::PlayChar(u32 c, f32 pan)
{
    if (c >= 123) {
        c = CHAR_DOT;
    }
    PlaySingle(GetSound(c, 0, false), pan);
}

// 0x002E7A7C
void SoundDoubutsugoMgr::PlaySingle(u32 soundId, f32 pan)
{
    if (soundId == 0) {
        return;
    }
    f32 volume = 1.0f;
    f32 pitch = 1.0f;
    if (IsSyllableSound(soundId)) {
        if (soundId == SE_DOU_BOY_032_TU_L) {
            soundId = SE_DOU_BOY_032_TU;
            volume = 0.5f;
        }
        if (Func5C116C()) {
            pitch = 1.1f;
        } else {
            pitch = 1.05f;
            soundId += SYLLABLE_NUM; // GAL
        }
    }
    if (m_IsVoicePlaying) {
        m_VoiceHandle.Stop(0);
    }
    SoundMgr::s_pInstance->StartSound(&m_VoiceHandle, soundId);
    m_VoiceHandle.SetVolume(volume * 0.65f, 0);
    if (pan < -1.0f) {
        pan = -1.0f;
    } else if (pan > 1.0f) {
        pan = 1.0f;
    }
    m_VoiceHandle.SetPan(pan * s_SinglePanScale);
    m_VoiceHandle.SetPitch(pitch);
    m_IsVoicePlaying = true;
}

// 0x002E7BA4
void SoundDoubutsugoMgr::PlayLongChar(u32 c, f32 pan)
{
    if (c >= 87) {
        c = CHAR_DOT;
    }
    u32 sound = GetSound(static_cast<u16>(c), 0, false);
    u32 played = SE_SYS_SCRIPT_DOT;
    if (sound != 0 && IsSyllableSound(sound)) {
        u8 index = s_LongSounds[sound - SE_DOU_BOY_000_A_L];
        if (index != 0xFF) {
            played = SE_DOU_BOY_000_A_L + index;
        }
    }
    PlaySingle(played, pan);
}

// 0x002E7C18
void SoundDoubutsugoMgr::StopVoice()
{
    if (m_IsVoicePlaying) {
        m_VoiceHandle.Stop(0);
    }
}

// 0x002E7C3C
void SoundDoubutsugoMgr::calc()
{
    if (SoundMgr::s_pInstance->IsPaused()) {
        return;
    }
    if (m_Handles[m_HandleIndex].m_pSound != NULL) {
        m_IdleFrames = 0;
    } else {
        m_IsSyllable = false;
        m_IdleFrames++;
    }
    if (m_QueueCount != 0 && static_cast<u32>(m_QueueFrames[m_QueueRead]) <= 1) {
        if (m_QueueCount == 1) {
            Play(m_Queue[m_QueueRead], 0);
        } else {
            Play(m_Queue[m_QueueRead], m_Queue[m_QueueNext]);
        }
        if (m_Language >= LANGUAGE_ENGLISH && m_Language <= LANGUAGE_SPANISH) {
            Pop();
            // the characters without time
            while (m_QueueCount >= 1 && m_QueueFrames[m_QueueRead] == 0) {
                Pop();
            }
        } else {
            Pop();
            // the characters without a sound
            while (m_QueueCount >= 1 && static_cast<u32>(m_QueueFrames[m_QueueRead]) <= 1) {
                if (GetSound(ConvertChar(m_Queue[m_QueueRead]), 0, true) != 0) {
                    break;
                }
                Pop();
            }
        }
    }
    for (s32 i = 0; i < QUEUE_SIZE; i++) {
        if (m_QueueFrames[i] != 0) {
            m_QueueFrames[i]--;
        }
    }
    if (m_IsSpeaking) {
        f32 pitch = 1.0f;
        switch (m_Effect) {
            case EFFECT_NONE:
                if (m_IsFalling) {
                    pitch = m_Pitch;
                    if (m_PhaseFrame < m_PhaseFrames) {
                        m_PhaseFrame++;
                    } else {
                        m_IsFalling = false;
                        m_PhaseFrame = 0;
                        if (m_pParam->riseFramesMax > m_pParam->riseFramesMin) {
                            m_PhaseFrames = netgame::Func2FF63C(m_pParam->riseFramesMax - m_pParam->riseFramesMin) +
                                            m_pParam->riseFramesMin;
                        } else {
                            m_PhaseFrames = m_pParam->riseFramesMin;
                        }
                    }
                } else {
                    if (m_StepFrame >= PITCH_STEPS) {
                        m_StepFrame = 0;
                        if (m_pParam->pitchMax > m_pParam->pitchMin) {
                            m_StepRange = GetRandomF(m_pParam->pitchMax - m_pParam->pitchMin) + m_pParam->pitchMin;
                        } else {
                            m_StepRange = m_pParam->pitchMin;
                        }
                        m_StepFall = m_StepRange * m_StepScale;
                    } else if (m_StepFrame < m_StepRiseFrames) {
                        pitch += m_StepRange * m_StepFrame / m_StepRiseFrames;
                    } else {
                        pitch += m_StepFall * (PITCH_STEPS - m_StepFrame);
                    }
                    if (m_PhaseFrame < m_PhaseFrames) {
                        m_PhaseFrame++;
                    } else {
                        if (m_Pitch < m_pParam->fallLimit + 1.0f) {
                            m_IsFalling = true;
                        }
                        m_PhaseFrame = 0;
                        if (m_pParam->fallFramesMax > m_pParam->fallFramesMin) {
                            m_PhaseFrames = netgame::Func2FF63C(m_pParam->fallFramesMax - m_pParam->fallFramesMin) +
                                            m_pParam->fallFramesMin;
                        } else {
                            m_PhaseFrames = m_pParam->fallFramesMin;
                        }
                    }
                    m_StepFrame++;
                }
                break;
            case EFFECT_HOLD:
                pitch = m_Pitch;
                break;
            case EFFECT_RISE:
                if (m_EffectFrame >= m_pParam->riseFrames) {
                    m_Effect = EFFECT_NONE;
                } else {
                    m_EffectFrame++;
                    pitch = m_EffectPitch + m_pParam->riseAmount * m_EffectFrame / m_pParam->riseFrames;
                }
                break;
        }
        f32 scaled = m_pParam->pitchScale * pitch;
        for (s32 i = 0; i < HANDLE_NUM; i++) {
            if (m_Handles[i].m_pSound != NULL && m_Effect != EFFECT_NO_PITCH) {
                m_Handles[i].m_pSound->m_Pitch = scaled;
            }
            for (s32 j = 0; j < m_ExtraGroups.m_Count; j++) {
                if (m_Effect != EFFECT_NO_PITCH) {
                    m_ExtraHandles[j][i].SetPitch(s_ExtraPitches[j] * scaled);
                }
            }
        }
        if (m_Effect == EFFECT_NONE) {
            m_Pitch = pitch;
        }
    }
    m_IsVoicePlaying = false;
}

// 0x002E8178
void SoundDoubutsugoMgr::Queue(u16 c, s32 level)
{
    if (m_QueueCount >= QUEUE_SIZE) {
        return;
    }
    if (c == 164) {
        u16 prev = m_QueueWrite >= 1 ? m_Queue[m_QueueWrite - 1] : m_Queue[QUEUE_SIZE - 1];
        if (prev == 164 || prev == 166) {
            return;
        }
    } else if (c == 166) {
        u16 prev = m_QueueWrite >= 1 ? m_Queue[m_QueueWrite - 1] : m_Queue[QUEUE_SIZE - 1];
        if (prev == 166) {
            return;
        }
    }
    u8 lv;
    if (level < 85) {
        lv = 0;
    } else if (level < 130) {
        lv = 1;
    } else if (level < 180) {
        lv = 2;
    } else if (level < 230) {
        lv = 3;
    } else {
        lv = 4;
    }
    m_Level = lv;
    m_Queue[m_QueueWrite] = c;
    m_QueueFrames[m_QueueWrite] = m_CharFrames;
    m_QueueCount++;
    m_QueueWrite++;
    if (m_QueueWrite == QUEUE_SIZE) {
        m_QueueWrite = 0;
    }
}

// 0x002E7A30
void SoundMgr::SetSpeechFilter(bool isOn)
{
    m_DoubutsugoMgr.SetFilter(isOn);
}

// 0x007217B0
u32 SoundDoubutsugoMgr::ConvertChar(u32 c) const
{
    if (c - 123 < EXTENDED_CHAR_NUM) {
        return s_ExtendedChars[c - 123];
    }
    if (c <= 219) {
        return c;
    }
    u32 result = 0;
    if (c - 0x1100 <= 18) {
        result = s_InitialJamo[c - 0x1100];
    } else if (c - 0x1161 <= 20) {
        result = s_MedialJamo[c - 0x1161];
    } else if (c - 0x11A8 <= 26) {
        result = s_FinalJamo[c - 0x11A8];
    } else if (c - 0x3131 <= 50) {
        result = s_CompatibilityJamo[c - 0x3131];
    } else if (c - 0xAC00 <= 0x2BA3) {
        // a syllable: (initial * 21 + medial) * 28 + final
        s32 syllable = c - 0xAC00;
        s32 initial = syllable / (HANGUL_MEDIAL_NUM * HANGUL_FINAL_NUM);
        s32 medial = (syllable % (HANGUL_MEDIAL_NUM * HANGUL_FINAL_NUM)) / HANGUL_FINAL_NUM;
        if (initial < HANGUL_INITIAL_NUM && medial < HANGUL_MEDIAL_NUM) {
            result = s_HangulSyllables[initial][medial];
        }
    }
    return result;
}

// 0x00721554
u32 SoundDoubutsugoMgr::GetSound(u32 c, u32 next, bool isSpeech) const
{
    switch (c) {
        case 0:
            return 0;
        case CHAR_LONG:
            if (m_LastSyllable != 0 && IsSyllableSound(m_LastSyllable)) {
                u8 index = s_LongSounds[m_LastSyllable - SE_DOU_BOY_000_A_L];
                if (index != 0xFF) {
                    return SE_DOU_BOY_000_A_L + index;
                }
            }
            return 0;
        case CHAR_S:
            switch (next) {
                case 'i':
                    return SE_DOU_BOY_021_SI;
                case 'a':
                    return SE_DOU_BOY_020_SA;
                case 'e':
                    return SE_DOU_BOY_023_SE;
                case 'o':
                    return SE_DOU_BOY_024_SO;
                default:
                    return SE_DOU_BOY_022_SU;
            }
        case CHAR_DOT:
            return SE_SYS_SCRIPT_DOT;
        case CHAR_MONOLOGUE:
            return SE_SYS_SCRIPT_MONOLOGY;
    }
    // the syllables are the sound index
    u32 sound = SE_DOU_BOY_000_A_L - 1 + c;
    if (c < 87) {
        return sound;
    }
    if (c < 97) {
        // the digits
        if (isSpeech || m_Language != LANGUAGE_JAPANESE) {
            return s_DigitSounds[m_Language][c - 87];
        }
        return sound;
    }
    if (c >= 123) {
        return 0;
    }
    // the letters
    if (isSpeech) {
        u32 pair = GetLanguageSound(c, next);
        if (pair != 0) {
            return pair;
        }
        return s_LetterSounds[m_Language][c - 97];
    }
    if (m_Language == LANGUAGE_JAPANESE) {
        return sound;
    }
    if (m_Language == LANGUAGE_ENGLISH || m_Language == LANGUAGE_KOREAN) {
        if (c == 'm') {
            return SE_DOU_BOY_607_EM;
        }
        if (c == 'n') {
            return SE_DOU_BOY_608_EN;
        }
        return sound;
    }
    return s_LetterSounds[m_Language][c - 97];
}
