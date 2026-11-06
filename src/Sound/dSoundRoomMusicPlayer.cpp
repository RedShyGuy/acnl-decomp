#include "Sound/dSoundRoomMusicPlayer.h"
#include "Sound/dSoundRoomMusicPlayer_SingletonDisposer_.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundHaniwaMgr.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundMuInfo.h"
#include "Sound/dSoundObjFurnitureAudio.h"
#include "Other/dHeapInit.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

namespace {
const u32 END_TICK = 0x7FFFFFFF;
const f32 TEMPO_MILLISECONDS = 1250.0f;
const f32 MILLISECONDS_TICKS = 0.0008f;
const u32 DEFAULT_TICKS = 480;
const u32 DEFAULT_TEMPO = 120;
const s32 LOAD_WAIT = 15;
const s32 FADE_IN_FRAMES = 60;
const s32 FADE_OUT_FRAMES = 300;
const u32 FADE_MILLISECONDS = 11000;
const u32 END_MILLISECONDS = 10000;
const u32 LAST_MILLISECONDS = 999;
const u32 CONTROL_TRACK_MASK = 1 << 15;
const s32 MUSIC_PRIORITY = 127;

inline f32 Clamp01(f32 value)
{
    if (value < 0.0f) {
        return 0.0f;
    }
    if (value > 1.0f) {
        return 1.0f;
    }
    return value;
}

// the tempo at a tick
inline u32 FindTempo(const SoundConductor::TempoEntry* tempos, u32 tick)
{
    for (s32 i = 0; i < SoundRoomMusicPlayer::TEMPO_NUM - 1; i++) {
        if (tempos[i + 1].tick > tick) {
            return tempos[i].tempo;
        }
    }
    return tempos[0].tempo;
}
} // namespace

// 0x0094E558
SoundRoomMusicPlayer* SoundRoomMusicPlayer::s_pInstance = NULL;
// 0x0094E55C
SoundRoomMusicPlayer::SingletonDisposer_* SoundRoomMusicPlayer::s_pSingletonDisposer = NULL;

SoundRoomMusicPlayer::SoundRoomMusicPlayer()
    : m_LoadId(-1), m_IsInitialized(false), m_State(STATE_NONE), m_Song(0), m_Wait(0), m_EndTick(DEFAULT_TICKS),
      m_FadeTick(DEFAULT_TICKS), m_IsFinished(false), m_Tempo(DEFAULT_TEMPO), m_PendingIsFromStart(0), m_PendingMilliSeconds(0),
      m_pObj(NULL)
{
}

// 0x00128F18
SoundRoomMusicPlayer::~SoundRoomMusicPlayer()
{
    if (m_IsInitialized) {
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->Stop(0);
            m_State = STATE_NONE;
        }
        Unload(true);
        m_Conductor.Finalize();
        SoundHaniwaMgr::destroyInstance();
        m_IsFinished = false;
        m_IsInitialized = false;
    }
}

// 0x003225DC
SoundRoomMusicPlayer* SoundRoomMusicPlayer::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundRoomMusicPlayer)];
        SoundRoomMusicPlayer* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundRoomMusicPlayer();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x00123A70
void SoundRoomMusicPlayer::destroyInstance()
{
    if (s_pSingletonDisposer != NULL) {
        SingletonDisposer_* disposer = s_pSingletonDisposer;
        s_pSingletonDisposer = NULL;
        disposer->~SingletonDisposer_();
        if (s_pInstance != NULL) {
            delete s_pInstance;
        }
        s_pInstance = NULL;
    }
}

// 0x0012FBF0
void SoundRoomMusicPlayer::Stop(s32 fadeFrames)
{
    m_IsFinished = false;
    if (m_State == STATE_NONE) {
        return;
    }
    if (fadeFrames != 0) {
        if (m_Handle.m_pSound == NULL) {
            m_State = STATE_WAIT_UNLOAD;
            return;
        }
        m_Handle.m_pSound->Stop(fadeFrames);
        if (m_State != STATE_STOPPING) {
            m_Conductor.Stop();
            m_Chords.Stop();
        }
        m_State = STATE_STOPPING;
        return;
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(0);
        if (m_State != STATE_STOPPING) {
            m_Conductor.Stop();
            m_Chords.Stop();
        }
    }
    m_State = STATE_NONE;
    Unload(true);
}

// 0x0012FC94
void SoundRoomMusicPlayer::Unload(bool isAll)
{
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    if (loadMgr == NULL) {
        return;
    }
    if (isAll) {
        loadMgr->UnloadOrDefer(0, LOAD_INDEX);
    } else {
        if (m_LoadId == -1) {
            return;
        }
        loadMgr->Unload(m_LoadId, LOAD_INDEX);
    }
    m_LoadId = -1;
}

// 0x003222C0
void SoundRoomMusicPlayer::Initialize()
{
    if (m_IsInitialized || HeapInit::s_pStageHeap == NULL) {
        return;
    }
    sead::ExpHeap* heap = HeapInit::s_pStageHeap;
    m_Conductor.Start();
    m_Conductor.m_pTempoTable = m_Tempos;
    SoundHaniwaMgr::createInstance(heap);
    SoundHaniwaMgr::s_pInstance->SetControl(SoundHaniwaMgr::CONTROL_ROOM_MUSIC, &m_Conductor);
    m_Chords.Initialize();
    m_StartFader.Reset(1.0f);
    m_EndFader.Reset(1.0f);
    m_IsInitialized = true;
}

// 0x00322354
u32 SoundRoomMusicPlayer::CalcTicks(u32 start, u32 milliSeconds)
{
    u32 ticks = 0;
    const SoundMuInfo::Song* info = SoundMuInfo::s_pInstance->GetInfo(m_Song);
    f32 elapsed = 0.0f;
    bool isFirstLoop = true;
    for (;;) {
        for (s32 i = 0; i < TEMPO_NUM - 1; i++) {
            u32 tick = m_Tempos[i].tick;
            if (tick == END_TICK) {
                break;
            }
            u32 nextTick = m_Tempos[i + 1].tick;
            u32 endTick = nextTick;
            if (!isFirstLoop) {
                if (info->loopStart > endTick) {
                    continue;
                }
                if (tick < info->loopStart) {
                    tick = info->loopStart;
                }
            }
            if (nextTick == END_TICK) {
                endTick = info->loopEnd;
            }
            u32 length = endTick - tick;
            if (start > length) {
                start -= length;
                continue;
            }
            length -= start;
            start = 0;
            f32 time = static_cast<f32>(length) * TEMPO_MILLISECONDS / static_cast<f32>(static_cast<s32>(m_Tempos[i].tempo));
            if (milliSeconds <= static_cast<u32>(time + elapsed)) {
                u32 rest = (milliSeconds - static_cast<u32>(elapsed)) * m_Tempos[i].tempo;
                return static_cast<u32>(static_cast<f32>(rest) * MILLISECONDS_TICKS) + ticks;
            }
            elapsed += time;
            ticks += length;
        }
        isFirstLoop = false;
    }
}

// 0x003224A0
void SoundRoomMusicPlayer::UpdateVolume()
{
    SoundObjFurnitureAudio* obj = m_pObj;
    if (obj == NULL) {
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetVolume(m_EndFader.GetValue(), 0);
        }
        return;
    }
    if (m_Handle.m_pSound == NULL) {
        return;
    }
    obj->UpdateChanged();
    f32 volume = Clamp01(m_pObj->m_DistanceVolume * m_pObj->m_VolumeScale * (m_StartFader.GetValue() * m_EndFader.GetValue()));
    m_Handle.SetPan(m_pObj->m_Pan);
    m_Handle.SetSurroundPan(m_pObj->m_SurroundPan);
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetVolume(volume, 0);
    }
    s32 type = m_pObj->GetBiquadType();
    if (type != 0) {
        if (m_Handle.m_pSound != NULL) {
            m_Handle.m_pSound->SetBiquadFilter(type, 1.0f);
        }
    } else if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetBiquadFilter(0, 0.0f);
    }
}

// 0x003226CC
void SoundRoomMusicPlayer::Play(s8 isFromStart, u32 milliSeconds)
{
    if (m_State == STATE_LOADING) {
        m_PendingIsFromStart = isFromStart;
        m_State = STATE_LOADING_PLAY_PENDING;
        m_PendingMilliSeconds = milliSeconds;
        return;
    }
    if (m_State != STATE_LOADED) {
        return;
    }

    SoundMuInfo* info = SoundMuInfo::s_pInstance;
    u32 soundId = info->GetSoundId(m_Song);
    SoundSeqParser* parser = SoundSeqParser::s_pInstance;
    parser->Load(soundId);
    parser->Parse(m_Tempos, 0, SoundSeqParser::MODE_TEMPO, TEMPO_NUM);
    m_EndFader.Reset(1.0f);

    if (milliSeconds == 0) {
        if (isFromStart != 0) {
            m_EndTick = 0;
            m_FadeTick = 0;
        } else {
            m_FadeTick = info->GetFadeTick(m_Song);
            m_EndTick = CalcTicks(m_FadeTick, FADE_MILLISECONDS) + m_FadeTick;
        }
        m_Tempo = FindTempo(m_Tempos, 0);
        Start(0);
        m_State = STATE_PLAYING;
        return;
    }

    u32 ticks = CalcTicks(0, milliSeconds);
    u32 position = info->WrapPosition(m_Song, ticks);
    u32 tempo = FindTempo(m_Tempos, position);
    u32 offset = static_cast<u32>(SoundMgr::s_pInstance->m_TickScale *
                                  (static_cast<f32>(position) * static_cast<f32>(static_cast<s32>(tempo))));
    if (isFromStart != 0) {
        m_Tempo = tempo;
        Start(position + offset);
        m_State = STATE_PLAYING;
        m_EndTick = 0;
        m_FadeTick = 0;
        return;
    }

    u32 fadeTick = info->GetFadeTick(m_Song);
    u32 fadeLength = CalcTicks(fadeTick, END_MILLISECONDS);
    u32 endTick = fadeTick + fadeLength;
    endTick += CalcTicks(endTick, LAST_MILLISECONDS);
    if (ticks + offset >= endTick) {
        m_State = STATE_WAIT_UNLOAD;
        m_IsFinished = true;
        return;
    }

    u32 loops = ticks - position;
    u32 fadeStart = fadeTick - loops;
    m_FadeTick = fadeStart;
    m_EndTick = endTick - loops;
    if (position >= fadeStart + fadeLength) {
        m_EndFader.Reset(0.0f);
    } else if (position >= fadeStart) {
        f32 rest = Clamp01(static_cast<f32>(fadeStart + fadeLength - position) / static_cast<f32>(fadeLength));
        m_EndFader.Reset(rest);
        m_EndFader.SetTarget(0.0f, static_cast<s32>(rest * FADE_OUT_FRAMES));
    }
    m_Tempo = tempo;
    Start(position + offset);
    m_State = STATE_PLAYING;
}

// 0x00322A48
void SoundRoomMusicPlayer::Load(u32 song)
{
    switch (m_State) {
    case STATE_NONE:
        m_LoadId = SoundDataLoadMgr::s_pInstance->Load(SoundMuInfo::s_pInstance->GetSoundId(song), LOAD_INDEX);
        m_Song = song;
        m_State = STATE_LOADING;
        m_Wait = LOAD_WAIT;
        m_IsFinished = false;
        break;
    case STATE_WAIT_UNLOAD:
        m_State = STATE_WAIT_RELOAD;
        m_Song = song;
        m_IsFinished = false;
        break;
    case STATE_STOPPING:
        m_State = STATE_STOPPING_RELOAD;
        m_Song = song;
        m_IsFinished = false;
        break;
    }
}

// 0x00322C88
void SoundRoomMusicPlayer::Start(u32 tick)
{
    u32 soundId = SoundMuInfo::s_pInstance->GetSoundId(m_Song);
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (tick != 0) {
        nw::snd::SoundStartable::StartInfo startInfo;
        startInfo.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
        startInfo.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
        startInfo.startOffset = tick;
        mgr->StartSoundChecked(&m_Handle, soundId, &startInfo);
        m_StartFader.Reset(0.0f);
        m_StartFader.SetTarget(1.0f, FADE_IN_FRAMES);
    } else {
        mgr->StartSoundChecked(&m_Handle, soundId);
        m_StartFader.Reset(1.0f);
    }
    UpdateVolume();

    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->SetTrackMute(CONTROL_TRACK_MASK, nw::snd::SEQ_MUTE_STOP);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->SetChannelPriority(MUSIC_PRIORITY);
    }
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->SetPlayerPriority(MUSIC_PRIORITY);
    }
    m_Conductor.Follow(m_Song, &m_Handle, tick, m_Tempo);
    SoundHaniwaMgr::s_pInstance->StopAll();
    m_Chords.Start(soundId);
}

// 0x00322E68
void SoundRoomMusicPlayer::Update()
{
    if (SoundMgr::s_pInstance->IsPaused() || !m_IsInitialized) {
        return;
    }
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    SoundMuInfo* info = SoundMuInfo::s_pInstance;
    switch (m_State) {
    case STATE_LOADING: {
        bool isBusy = loadMgr->IsBusy(LOAD_INDEX);
        if (m_Wait != 0) {
            m_Wait--;
        } else if (!isBusy) {
            if (loadMgr->IsLoaded(info->GetSoundId(m_Song))) {
                m_State = STATE_LOADED;
            } else {
                m_LoadId = loadMgr->Load(info->GetSoundId(m_Song), LOAD_INDEX);
            }
        }
        break;
    }
    case STATE_LOADING_PLAY_PENDING:
        if (!loadMgr->IsBusy(LOAD_INDEX)) {
            m_State = STATE_LOADED;
            Play(m_PendingIsFromStart, m_PendingMilliSeconds);
        }
        break;
    case STATE_WAIT_UNLOAD:
        if (!loadMgr->IsBusy(LOAD_INDEX)) {
            Unload(false);
            m_State = STATE_NONE;
        }
        break;
    case STATE_WAIT_RELOAD:
        if (!loadMgr->IsBusy(LOAD_INDEX)) {
            Unload(false);
            m_State = STATE_NONE;
            Load(m_Song);
        }
        break;
    case STATE_WAIT_CONDUCTOR:
        if (m_Conductor.SetSong(info->GetSoundId(m_Song))) {
            Start(0);
            m_State = STATE_PLAYING;
        }
        break;
    case STATE_PLAYING:
        UpdateVolume();
        if (m_EndTick != 0) {
            nw::snd::SequenceSoundHandle sequence(&m_Handle);
            u32 tick = sequence.m_pSound != NULL ? sequence.m_pSound->GetTick() : 0;
            if (m_EndTick <= tick) {
                if (m_Handle.m_pSound != NULL) {
                    m_Handle.m_pSound->Stop(0);
                }
                m_Conductor.Stop();
                m_Chords.Stop();
                m_State = STATE_STOPPING;
                m_IsFinished = true;
            } else if (m_FadeTick <= tick && m_EndFader.SoundFader::GetRemainingFrames() <= 0 &&
                       0.0f < m_EndFader.GetValue()) {
                m_EndFader.SetTarget(0.0f, FADE_OUT_FRAMES);
            }
        }
        break;
    case STATE_STOPPING:
        if (m_Handle.m_pSound == NULL) {
            Unload(false);
            m_State = STATE_NONE;
        }
        break;
    case STATE_STOPPING_RELOAD:
        if (m_Handle.m_pSound == NULL) {
            Unload(false);
            m_State = STATE_NONE;
            Load(m_Song);
        }
        break;
    }
    m_Conductor.Update();
    SoundHaniwaMgr::s_pInstance->Update();
    m_Chords.Update();
    m_StartFader.Update();
    m_EndFader.Update();
}

// 0x0070F980
bool SoundRoomMusicPlayer::SIsSessionActive()
{
    if (s_pInstance != NULL) {
        return s_pInstance->m_Chords.IsSessionActive();
    }
    return false;
}
