#include "Sound/dSoundObjHaniwa.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundHaniwaMgr.h"
#include "Sound/dSoundIHaniwaControl.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "netgame/dGameFunctions.h"
#include "nw/snd/snd_SoundSystem.h"
#include "nw/snd/internal/driver/snd_SoundThread.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"

namespace {
const f32 START_SPEED = 0.6f;
const f32 SPEED_STEP = 0.2f;
const s32 BEATS_PER_LOOP = 48;
const s32 TRACK_DANCE = 0;
const s32 TRACK_DJ = 1;
const s32 TYPE_HANIWA = 3;

// (name is ours)
inline sead::SoundHandle* FindHandle(sead::CheckedArray<sead::SoundHandle, 2>& handles, u32 soundId)
{
    for (s32 i = 0; i < 2; i++) {
        if (handles[i].GetId() == soundId) {
            return &handles[i];
        }
    }
    return NULL;
}

// the variables of the dance before the sequence sets them
inline void ResetDance(SoundObjHaniwa* obj)
{
    obj->m_Mode = -1;
    obj->m_Beats3 = -1;
    obj->m_StepMask = 0;
    obj->m_Group = 0;
    obj->m_Beat = -1;
    obj->m_BeatsFallback = -1;
    obj->m_Step = 0;
    obj->m_Beats = -1;
    obj->m_StartBeat = -1;
    obj->m_Speed = START_SPEED;
    obj->m_IsStopRequested = false;
    obj->m_LastPhase = -1.0f;
}
} // namespace

// 0x00278108
SoundObjHaniwa::SoundObjHaniwa()
    : m_SoundId(0xFFFFFFFF), m_Index(0xFFFFFFFF), m_pMgr(NULL), m_DanceState(DANCE_NONE), m_IsFree(false), m_Beat(-1),
      m_BeatsFallback(-1), m_Step(0), m_Beats(-1), m_StartBeat(-1), m_Variation(0), m_Speed(1.0f), m_IsStopRequested(false),
      m_LastPhase(-1.0f), m_Mode(-1), m_Beats3(-1), m_StepMask(0), m_Group(0), m_Note(7)
{
    m_Priority = TYPE_HANIWA;
}

// 0x002781D8
// 0x002781C8 (deleting dtor)
SoundObjHaniwa::~SoundObjHaniwa()
{
    SoundObjHaniwa::Stop();
}

// 0x00277254
void SoundObjHaniwa::Start(u32 kind, const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    if (m_IsActive || kind >= KIND_NUM) {
        return;
    }
    m_SoundId = kind + SE_HNW_000_BCN_DKA;
    SoundObjBase::Start(world, relative);
}

// 0x0027728C
void SoundObjHaniwa::StopDance()
{
    m_IsStopRequested = true;
}

// 0x00277298
void SoundObjHaniwa::ApplyTempo()
{
    sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
    if (handle != NULL) {
        nw::snd::SequenceSoundHandle sequence(handle);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTempoRatio(m_pMgr->m_BeatScale);
        }
    }
}

// 0x0027731C
f32 SoundObjHaniwa::UpdateDance(const nn::math::VEC3* world, const nn::math::VEC3* relative)
{
    SoundObj<2>::SetPosition(world, relative);
    f32 phase = -1.0f;
    s32 state = m_DanceState;
    if (state != DANCE_NONE && state != 4) {
        sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
        if (handle != NULL) {
            if (state == DANCE_PREPARING) {
                if (handle->m_pSound != NULL && handle->m_pSound->IsPrepared()) {
                    if (handle->m_pSound != NULL) {
                        handle->m_pSound->StartPrepared();
                    }
                    ApplyTempo();
                    if (m_IsFree) {
                        m_Variation = netgame::Func2FF63C(6) + 2;
                    } else if (m_pMgr->m_ControlKind == SoundHaniwaMgr::CONTROL_DJ) {
                        u32 random = m_pMgr->GetRandom() % 5;
                        s32 factor = random == 0 ? 2 : (random == 1 ? 4 : 0);
                        m_Variation = factor * m_Index + 1;
                    } else {
                        m_Variation = 1;
                    }
                    nw::snd::SequenceSoundHandle sequence(handle);
                    if (sequence.m_pSound != NULL) {
                        sequence.m_pSound->WriteTrackVariable(TRACK_DANCE, 7, ~m_Variation);
                    }
                    if (sequence.m_pSound != NULL) {
                        sequence.m_pSound->WriteTrackVariable(TRACK_DJ, 13, 1);
                    }
                    m_DanceState = DANCE_STARTING;
                }
            } else if (state == DANCE_STARTING) {
                ApplyTempo();
                if (ReadVariables()) {
                    m_Note = s_LevelNotes[m_Group][m_pMgr->m_Level];
                    nw::snd::SequenceSoundHandle sequence(handle);
                    if (sequence.m_pSound != NULL) {
                        sequence.m_pSound->WriteTrackVariable(TRACK_DANCE, 6, m_Note);
                    }
                    m_DanceState = DANCE_DANCING;
                }
            } else if (state == DANCE_DANCING) {
                ApplyTempo();
                nw::snd::SequenceSoundHandle sequence(handle);
                s16 beat = -1;
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->ReadVariable(9, &beat);
                }
                if (m_Beat != beat) {
                    if (m_Mode != 3 && m_Step == beat) {
                        // the next step that the mask allows, from the beat on
                        s32 bit;
                        for (bit = 14 - beat; bit > -1; bit--) {
                            if ((m_StepMask >> bit) & 1) {
                                goto found;
                            }
                        }
                        for (bit = 15; bit > -1; bit--) {
                            if ((m_StepMask >> bit) & 1) {
                                goto found;
                            }
                        }
                        goto done;
                    found:
                        m_Step = 15 - bit;
                        if (sequence.m_pSound != NULL) {
                            sequence.m_pSound->WriteVariable(11, m_Step);
                        }
                    }
                done:
                    m_Beat = beat;
                }
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->ReadVariable(8, &m_StartBeat);
                }
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->ReadVariable(12, &m_Beats);
                }
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->ReadVariable(7, &m_BeatsFallback);
                }
                phase = CalcPhase();
            }
        }
    }

    if (!m_IsStopRequested) {
        m_LastPhase = phase;
        return phase;
    }
    // the dance runs out while the gyroids stop
    if (phase >= 0.0f && m_LastPhase >= 0.0f) {
        f32 target = phase;
        if (target < m_LastPhase) {
            target += 1.0f;
        }
        phase = SoundMgr::Lerp(m_pMgr->m_Wait, SoundHaniwaMgr::s_StopFrames, 0, m_LastPhase, target);
        if (phase >= 1.0f) {
            phase -= 1.0f;
        }
    }
    if (m_pMgr->m_Wait == 0) {
        m_IsStopRequested = false;
    }
    return phase;
}

// 0x002777F8
void SoundObjHaniwa::Register()
{
    if (m_Index <= SoundHaniwaMgr::OBJ_NUM) {
        return;
    }
    m_pMgr = SoundHaniwaMgr::s_pInstance;
    if (m_pMgr == NULL) {
        return;
    }
    m_Index = m_pMgr->Register(this);
    ResetDance(this);
}

// 0x0027787C
void SoundObjHaniwa::SetLevel(u8 level, u32 index)
{
    if (level >= LEVEL_NUM || m_DanceState != DANCE_DANCING) {
        return;
    }
    s16 note = s_LevelNotes[m_Group][level];
    if (m_Note == note) {
        return;
    }
    sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
    if (handle != NULL) {
        nw::snd::SequenceSoundHandle sequence(handle);
        if (note > m_Note && index > 0) {
            // the gyroids join one after the other
            s16 delay = (netgame::Func2FF63C(4) + 1) * 0xFFFE;
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->WriteTrackVariable(TRACK_DANCE, 7, delay);
            }
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->WriteTrackVariable(TRACK_DANCE, 6, note);
        }
    }
    m_Note = note;
}

// 0x00277984
bool SoundObjHaniwa::ReadVariables()
{
    sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
    nw::snd::SequenceSoundHandle sequence(handle);
    if (sequence.m_pSound == NULL || sequence.m_pSound->GetTick() == 0) {
        return false;
    }
    if (m_Mode == -1) {
        s16 steps[4] = {-1, -1, -1, -1};
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(0, &m_Mode);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(1, &steps[0]);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(2, &steps[1]);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(3, &steps[2]);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(4, &steps[3]);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(5, &m_Beats3);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(6, &m_Group);
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->ReadVariable(8, &m_StartBeat);
        }
        if (m_Mode != -1) {
            if (m_Group >= GROUP_NUM) {
                m_Group = 0;
            }
            if (steps[0] >= 0) {
                if (steps[3] == -1) {
                    steps[3] = 0;
                }
                m_StepMask |= steps[3];
                if (steps[2] == -1) {
                    steps[2] = 0;
                }
                m_StepMask |= steps[2] << 4;
                if (steps[1] == -1) {
                    steps[1] = 0;
                }
                m_StepMask = static_cast<u16>(m_StepMask | steps[1] << 8) | steps[0] << 12;
            }
        }
    }
    return true;
}

// 0x00277B6C
f32 SoundObjHaniwa::CalcPhase()
{
    if (m_Beat < 0) {
        return 0.0f;
    }
    s16 beat;
    f32 tickInBeat;
    u32 ticksPerBeat;
    {
        nw::snd::SoundSystem::SoundThreadScopedLock lock;
        beat = m_pMgr->GetBeat();
        tickInBeat = static_cast<f32>(m_pMgr->GetTickInBeat());
        ticksPerBeat = m_pMgr->GetTicksPerBeat();
    }
    s16 beats = beat - m_StartBeat;
    if (beats < 0) {
        beats += BEATS_PER_LOOP;
    }
    s16 period = m_Beats;
    s16 length = period;
    if (m_Mode == 3) {
        length = m_Beats3;
        if (length == 0) {
            length = m_BeatsFallback;
            if (length <= 0) {
                length = 0;
            }
        }
    }
    if (length <= beats || period <= 0) {
        return 0.0f;
    }
    f32 phase = (tickInBeat / static_cast<f32>(ticksPerBeat) + static_cast<f32>(beats)) / static_cast<f32>(length);
    if (m_Speed < 1.0f) {
        phase *= m_Speed;
        m_Speed += SPEED_STEP;
    }
    if (phase < 0.0f) {
        return 0.0f;
    }
    if (phase > 1.0f) {
        return 1.0f;
    }
    return phase;
}

// 0x00277CE4
s32 SoundObjHaniwa::GetFramesToStart()
{
    sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
    if (handle == NULL) {
        return -1;
    }
    nw::snd::SequenceSoundHandle sequence(handle);
    if (m_DanceState == DANCE_PREPARING || (m_DanceState != DANCE_STARTING && m_DanceState != DANCE_DANCING)) {
        return -1;
    }
    s16 beat = -1;
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(9, &beat);
    }
    if (beat >= 0) {
        return 0;
    }
    s32 framesToBeat = m_pMgr->GetFramesToBeat();
    s32 frames = m_pMgr->GetFramesPerBeat() * ~beat + framesToBeat;
    if (frames > 10) {
        return -1;
    }
    if (frames >= 3) {
        frames -= 3;
    }
    return frames;
}

// 0x00277E00
void SoundObjHaniwa::Stop()
{
    if (m_Index < SoundHaniwaMgr::OBJ_NUM) {
        sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
        if (handle != NULL && handle->m_pSound != NULL) {
            handle->m_pSound->Stop(0);
        }
        m_DanceState = DANCE_NONE;
        m_pMgr->Unregister(m_Index);
        m_pMgr = NULL;
        m_Index = 0xFFFFFFFF;
    }
    if (m_IsActive) {
        m_SoundId = 0xFFFFFFFF;
        SoundObj<2>::Stop();
    }
}

// 0x00277EFC
void SoundObjHaniwa::StartDance(bool isFree)
{
    if (m_Index == 0xFFFFFFFF) {
        SoundHaniwaMgr* mgr = SoundHaniwaMgr::s_pInstance;
        if (mgr == NULL || mgr->m_ControlKind != SoundHaniwaMgr::CONTROL_DJ) {
            return;
        }
        m_pMgr = mgr;
        m_Index = mgr->Register(this);
        ResetDance(this);
    }
    if (m_pMgr->m_ControlKind == SoundHaniwaMgr::CONTROL_DJ) {
        SetIsNoDistance(true);
        m_VolumeScale = 1.0f;
    }
    if (FindHandle(m_HandleSet.m_Handles, m_SoundId) != NULL) {
        return;
    }
    if (!SoundDataLoadMgr::s_pInstance->IsLoaded(m_SoundId)) {
        return;
    }
    if (PrepareSound(m_SoundId) == NULL) {
        return;
    }
    m_DanceState = DANCE_PREPARING;
    ResetDance(this);
    m_IsFree = isFree;
}

// 0x00278094
void SoundObjHaniwa::StopSound()
{
    sead::SoundHandle* handle = FindHandle(m_HandleSet.m_Handles, m_SoundId);
    if (handle != NULL && handle->m_pSound != NULL) {
        handle->m_pSound->Stop(0);
    }
    m_DanceState = DANCE_NONE;
}
