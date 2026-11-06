#include "Sound/dSoundConductor.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundMuInfo.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadAudioPlayerCtr.h"

namespace {
const u32 TICKS_PER_BEAT = 48;
const s32 BEATS_PER_LOOP = 48;
const u32 GRID_TICKS_NORMAL = 24;
const s32 TEMPO_TABLE_NUM = 49;
const u32 DEFAULT_TEMPO = 1000000; // no change
const u32 END_TICK = 0x7FFFFFFF;   // the end of the table
const f32 TEMPO_NORMAL = 120.0f;
const f32 TICK_FRAMES = 37.5f;
const f32 MAX_TEMPO = 400.0f;
const u32 GLOBAL_VARIABLE_BEAT = 1;

inline s32 Ceil(f32 value)
{
    s32 result = static_cast<s32>(value);
    if (static_cast<f32>(result) != value && !(0.0f > value)) {
        result++;
    }
    return result;
}

inline void SetSequenceTempo(SoundHandleBindSeq& handle, f32 ratio)
{
    if (handle.m_SequenceHandle.m_pSound != NULL) {
        handle.m_SequenceHandle.m_pSound->SetTempoRatio(ratio);
    }
}
} // namespace

// 0x0094DD06
bool SoundConductor::s_IsStarted = false;

// 0x0083A1C4
const u32 SoundConductor::s_ControlSounds[2] = {SE_MU_CTRL_GRID_NORMAL, SE_MU_CTRL_GRID_SHUFFLE};
// 0x0083A1CC
const u32 SoundConductor::s_BeatTicks[3] = {24, 32, 16};

// 0x00276880
SoundConductor::SoundConductor()
    : m_State(STATE_NONE), m_IsChanged(false), m_SoundId(0xFFFFFFFF), m_pTempoTable(NULL), m_pMusic(NULL), m_ControlTick(0),
      m_MusicTick(0), m_Tempo(TEMPO_NORMAL), m_TempoRatio(1.0f), m_Beat(-1), m_GlobalBeat(-1), m_TicksPerBeat(0), m_BaseTick(0),
      m_FramesToBeat(-1), m_GridKind(0), m_UpbeatTicks(0), m_Wait(0)
{
}

// 0x0027693C
// 0x002768FC (deleting dtor)
SoundConductor::~SoundConductor()
{
    // (only the members are destroyed)
}

// 0x00276060
void SoundConductor::Start()
{
    if (s_IsStarted) {
        return;
    }
    static_cast<nw::snd::SoundArchivePlayer*>(SoundMgr::s_pInstance->m_pPlayer)
        ->SetSequenceUserprocCallback(SequenceCallback, this);
    nw::snd::internal::SequenceSound::WriteGlobalVariable(GLOBAL_VARIABLE_BEAT, -1);
    m_pMusic = NULL;
    m_SoundId = 0xFFFFFFFF;
    m_TempoRatio = 1.0f;
    m_ControlTick = 0;
    m_MusicTick = 0;
    m_GlobalBeat = -1;
    SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, s_ControlSounds[m_GridKind]);
    m_Handle.BindSequence();
    m_State = STATE_STARTED;
    s_IsStarted = true;
}

// 0x00276108
bool SoundConductor::SetSong(u32 soundId)
{
    if (m_State == STATE_STARTED) {
        if (m_SoundId == 0xFFFFFFFF) {
            SoundMuInfo* info = SoundMuInfo::s_pInstance;
            m_SoundId = soundId;
            u32 song = info->GetSong(soundId);
            if (song != 0xFFFFFFFF) {
                m_GridKind = info->GetGridKind(song);
                u32 upbeat = info->GetUpbeatTicks(song);
                if (upbeat != 0) {
                    upbeat %= TICKS_PER_BEAT;
                }
                m_UpbeatTicks = upbeat;
            }
            m_State = STATE_SONG_SET;
        }
    } else if (m_State == STATE_FOLLOWING) {
        return true;
    }
    return false;
}

// 0x002761B0
void SoundConductor::SequenceCallback(u16 procId, nw::snd::SequenceUserprocCallbackParam* param, void* arg)
{
    if (s_IsStarted) {
        static_cast<SoundConductor*>(arg)->OnSequenceCallback(procId, param);
    }
}

// 0x002761DC
void SoundConductor::Follow(u32 song, nw::snd::SoundHandle* musicHandle, u32 position, s32 tempo)
{
    SoundMuInfo* info = SoundMuInfo::s_pInstance;
    m_pMusic = musicHandle;
    m_SoundId = info->GetSoundId(song);
    m_GridKind = info->GetGridKind(song);
    u32 upbeat = info->GetUpbeatTicks(song);
    if (upbeat == 0) {
        m_UpbeatTicks = 0;
    } else {
        m_UpbeatTicks = upbeat % TICKS_PER_BEAT;
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(0);
    }
    m_ControlTick = 0;
    m_MusicTick = 0;
    m_TempoRatio = info->GetTempoRatio(song);
    m_Wait = 2;
    f32 ratio = m_TempoRatio * (static_cast<f32>(tempo) * (1.0f / TEMPO_NORMAL));

    nw::snd::SoundStartable::StartInfo startInfo;
    startInfo.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
    startInfo.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
    startInfo.startOffset = static_cast<s32>(m_TempoRatio * static_cast<f32>(position));
    SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, s_ControlSounds[m_GridKind], &startInfo);
    m_Handle.BindSequence();
    SetSequenceTempo(m_Handle, ratio);

    u32 next = m_UpbeatTicks;
    m_BaseTick = 0;
    if (next != 0) {
        if (position > next) {
            u32 grid = m_GridKind == 0 ? GRID_TICKS_NORMAL : TICKS_PER_BEAT;
            u32 distance = position - next;
            u32 count = distance / grid;
            if (distance % grid != 0) {
                count++;
            }
            next += count * grid;
        }
        m_UpbeatTicks = 0;
    } else {
        u32 grid = m_GridKind == 0 ? GRID_TICKS_NORMAL : TICKS_PER_BEAT;
        u32 count = position / grid;
        if (position % grid != 0) {
            count++;
        }
        next = count * grid;
    }
    m_TicksPerBeat = next - position;
    if (m_Handle.m_SequenceHandle.m_pSound != NULL) {
        m_Handle.m_SequenceHandle.m_pSound->WriteVariable(0, next);
    }
    m_State = STATE_FOLLOWING;
}

// 0x00276448
void SoundConductor::Stop()
{
    m_State = STATE_STOPPING;
    m_SoundId = 0xFFFFFFFF;
}

// 0x0012F9A4
void SoundConductor::Finalize()
{
    if (!s_IsStarted) {
        return;
    }
    m_State = STATE_NONE;
    s_IsStarted = false;
    static_cast<nw::snd::SoundArchivePlayer*>(SoundMgr::s_pInstance->m_pPlayer)->SetSequenceUserprocCallback(NULL, NULL);
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(0);
    }
}

// 0x0027645C
s16 SoundConductor::GetTickInBeat()
{
    s16 result = 0;
    if (m_Handle.m_pSound != NULL) {
        u32 tick = m_Handle.m_SequenceHandle.m_pSound != NULL ? m_Handle.m_SequenceHandle.m_pSound->GetTick() : 0;
        if ((m_State != STATE_FOLLOWING || m_pMusic != NULL) && m_BaseTick <= tick) {
            result = tick - m_BaseTick;
        }
    }
    return result;
}

// 0x002764AC
void SoundConductor::OnSequenceCallback(u32 procId, nw::snd::SequenceUserprocCallbackParam* param)
{
    if (procId >= 3) {
        return;
    }
    u32 state = m_State;
    if (m_Handle.m_pSound == NULL || m_Handle.m_SequenceHandle.m_pSound == NULL) {
        return;
    }
    m_BaseTick = m_Handle.m_SequenceHandle.m_pSound->GetTick();
    m_Beat++;
    if (m_Beat >= BEATS_PER_LOOP) {
        m_Beat = 0;
    }
    param->globalVariable[GLOBAL_VARIABLE_BEAT] = m_Beat;
    m_TicksPerBeat = s_BeatTicks[procId];
    m_FramesToBeat = Ceil(static_cast<f32>(m_TicksPerBeat) * TICK_FRAMES / m_Tempo);

    if (m_State == STATE_STOPPING) {
        state = STATE_STARTED;
    } else {
        if (m_State == STATE_SONG_SET) {
            if (procId == 0) {
                state = STATE_FOLLOWING;
            } else if (procId == 1) {
                if (m_UpbeatTicks == 0) {
                    state = STATE_FOLLOWING;
                }
            } else if (m_UpbeatTicks != 0) {
                state = STATE_FOLLOWING;
            }
        }
        if (m_State == state) {
            return;
        }
    }
    m_IsChanged = true;
    m_State = state;
    if (state == STATE_FOLLOWING) {
        if (m_UpbeatTicks != 0) {
            m_TicksPerBeat = m_UpbeatTicks;
        } else {
            m_TicksPerBeat = s_BeatTicks[m_GridKind];
        }
    }
}

// 0x00276600
void SoundConductor::Update()
{
    if (!s_IsStarted) {
        return;
    }
    if (m_pMusic != NULL && m_pMusic->m_pSound != NULL) {
        nw::snd::SequenceSoundHandle music(m_pMusic);
        u32 tick = music.m_pSound != NULL ? music.m_pSound->GetTick() : 0;
        m_MusicTick = tick;
        if (m_State == STATE_FOLLOWING || m_State == STATE_STOPPING) {
            u32 tempo = DEFAULT_TEMPO;
            for (s32 i = 0; i < TEMPO_TABLE_NUM; i++) {
                u32 end = m_pTempoTable[i + 1].tick;
                if (end == END_TICK || end > tick) {
                    tempo = m_pTempoTable[i].tempo;
                    break;
                }
            }
            if (tempo != DEFAULT_TEMPO) {
                m_Tempo = m_TempoRatio * static_cast<f32>(tempo);
            }
        }
    }
    if (m_Handle.m_pSound != NULL) {
        u32 tick = m_Handle.m_SequenceHandle.m_pSound != NULL ? m_Handle.m_SequenceHandle.m_pSound->GetTick() : 0;
        m_ControlTick = tick;
        m_FramesToBeat = Ceil(static_cast<f32>(m_TicksPerBeat - (tick - m_BaseTick)) * TICK_FRAMES / m_Tempo);
    }

    s16 beat = m_GlobalBeat;
    if (nw::snd::internal::SequenceSound::ReadGlobalVariable(GLOBAL_VARIABLE_BEAT, &beat) && m_GlobalBeat != beat) {
        m_GlobalBeat = beat;
        if (m_State == STATE_STARTED) {
            if (m_IsChanged) {
                m_SoundId = 0xFFFFFFFF;
                m_pMusic = NULL;
                SetSequenceTempo(m_Handle, m_Tempo * (1.0f / TEMPO_NORMAL));
            }
        } else if (m_State == STATE_FOLLOWING || m_State == STATE_STOPPING) {
            if (m_Wait > 0) {
                m_Wait--;
            } else {
                u32 musicTick = static_cast<u32>(static_cast<f32>(m_MusicTick) * m_TempoRatio);
                if (m_ControlTick != 0 && musicTick != 0) {
                    f32 ticks = static_cast<f32>(m_TicksPerBeat);
                    f32 tempo = static_cast<f32>(musicTick - m_ControlTick + m_TicksPerBeat) * m_Tempo / ticks;
                    if (tempo >= MAX_TEMPO) {
                        tempo = MAX_TEMPO / TEMPO_NORMAL;
                    } else {
                        tempo *= 1.0f / TEMPO_NORMAL;
                    }
                    SetSequenceTempo(m_Handle, tempo);
                }
            }
        }
    }
    m_IsChanged = false;
}

// 0x00719A60
s32 SoundConductor::GetTicksPerBeat()
{
    return m_TicksPerBeat;
}

// 0x00719A68
s32 SoundConductor::GetFramesPerBeat()
{
    return Ceil(static_cast<f32>(m_TicksPerBeat) * TICK_FRAMES / m_Tempo);
}

// 0x00719AB8
s32 SoundConductor::GetFramesToBeat()
{
    return m_FramesToBeat;
}

// 0x00719AC0
s16 SoundConductor::GetBeat()
{
    return m_Beat;
}

// 0x00719AC8
f32 SoundConductor::GetTempo()
{
    return m_Tempo;
}

// 0x007247D0
u32 SoundConductor::GetRandom()
{
    return 0;
}
