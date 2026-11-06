#include "Sound/dSoundDJPlayer.h"
#include "Sound/dSoundDJPlayer_SingletonDisposer_.h"
#include "Sound/dSoundHaniwaMgr.h"
#include "Sound/dSoundKKInfo.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMelodyPlayerBase.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "Other/dHeapInit.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "sead/seadAudioPlayerCtr.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"
#include "sead/seadRandom.h"

extern const f32 FIELD_UNIT_SIZE_X;

namespace {
// ticks and frames of the music
const u32 TICKS_PER_BEAT = 48;
const u32 TICKS_PER_STEP = 24;
const u32 BREAK_TICKS = 1536;
const u32 BREAK_BAR_TICKS = 192;
const u32 BREAK_BEATS = 32;
// the rest of the part before the end is not worth starting a song
const u32 MIN_TICKS = 6;
const u32 END_TICKS = 24;
const f32 TICKS_PER_MILLISECOND = 0.0008f; // per beat per minute
const f32 FRAMES_PER_BEAT = 1800.0f;       // at 1 beat per minute
const f32 FRAMES_PER_STEP = 900.0f;
const f32 FRAMES_PER_TICK = 37.5f;

const u32 VARIABLE_PLAY = 0;
const u32 VARIABLE_TEMPO = 7;
const u32 VARIABLE_POSITION = 8;
const u32 LOCAL_VARIABLE_NEXT = 1;
const u32 GLOBAL_VARIABLE_BEAT = 1;
const u32 GLOBAL_VARIABLE_SONG_COUNT = 3;
const u32 GLOBAL_VARIABLE_BREAK_COUNT = 4;
const u32 TRANSPOSE_TRACK = 15;
const u32 MELODY_NOTE_NUM = 16;
const s32 TRANSPOSE_OFF = 10000;
// the last songs of the set share the sequence of the song before
const u32 SONG_LAST = 94;
const u32 SONG_LAST_SEQUENCE = 91;

// the sequences of the songs lock against the sound thread
// 0x00983878
sead::CriticalSection s_SequenceLock;

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

inline s32 Ceil(f32 value)
{
    s32 result = static_cast<s32>(value);
    if (static_cast<f32>(result) != value && !(0.0f > value)) {
        result++;
    }
    return result;
}

inline s32 Floor(f32 value)
{
    s32 result = static_cast<s32>(value);
    if (static_cast<f32>(result) != value && 0.0f > value) {
        result--;
    }
    return result;
}

inline void WriteVariable(SoundHandleBindSeq& handle, s32 variable, s16 value)
{
    if (handle.m_SequenceHandle.m_pSound != NULL) {
        handle.m_SequenceHandle.m_pSound->WriteVariable(variable, value);
    }
}

inline void StopHandle(SoundHandleBindSeq& handle)
{
    if (handle.m_pSound != NULL) {
        handle.m_pSound->Stop(0);
    }
}
} // namespace

// 0x00796CE8 (static initializer)

// 0x0094DD08
SoundDJPlayer* SoundDJPlayer::s_pInstance = NULL;
// 0x0094DD0C
SoundDJPlayer::SingletonDisposer_* SoundDJPlayer::s_pSingletonDisposer = NULL;

namespace {
// nothing reads it
// 0x0094DD14
f32 s_UnusedSixthOfUnit = static_cast<f32>(static_cast<u16>(FIELD_UNIT_SIZE_X * 0.2f));
} // namespace

// 0x0094DD18
SoundDJPlayer::StartPosition SoundDJPlayer::s_StartPosition;

// 0x00246D30
SoundDJPlayer::SoundDJPlayer()
    : m_IsInitialized(false), m_State(STATE_NONE), m_DateState(0), m_Song(255), m_NextSong(255), m_HandleIndex(0),
      m_IsBreakPending(false), m_IsBreakStarting(false), m_IsSongPending(false), m_IsSongStarting(false), m_BeatCount(0),
      m_BaseTick(0), m_StartPosition(0), m_Part(PART_NONE), m_PlayType(0), m_BeatPhase(0.0f), m_BarPhase(0.0f),
      m_PartElapsed(0.0f), m_PartRemaining(0.0f), m_PartElapsedFrames(0), m_PartRemainingFrames(0), m_Beat(-1), m_Position(0),
      m_BeatPosition(0), m_TickInBeat(0), m_FramesToBeat(0), m_LightMode(0), _C2(0), m_IsForceFirstSong(0),
      m_IsForceSecondSong(0), m_FixedSongIndex(2)
{
}

// 0x00246E94
// 0x00246E34 (deleting dtor)
SoundDJPlayer::~SoundDJPlayer()
{
    Finalize();
}

// 0x00245104
SoundDJPlayer* SoundDJPlayer::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundDJPlayer)];
        s_pSingletonDisposer = new (buffer + 4) SingletonDisposer_(heap);
        s_pInstance = buffer != NULL ? new (buffer) SoundDJPlayer() : NULL;
    }
    return s_pInstance;
}

// 0x00123564
void SoundDJPlayer::destroyInstance()
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

// 0x00244FBC
void SoundDJPlayer::Initialize()
{
    if (m_IsInitialized || HeapInit::s_pStageHeap == NULL) {
        return;
    }
    SoundHaniwaMgr::createInstance(HeapInit::s_pStageHeap);
    SoundHaniwaMgr::s_pInstance->SetControl(SoundHaniwaMgr::CONTROL_DJ, this);
    m_DateState = Func1668A0();
    m_IsInitialized = true;
}

// 0x00245014
void SoundDJPlayer::SetForceFirstSong(bool isForced)
{
    m_IsForceFirstSong = isForced;
    if (isForced == 1) {
        m_FixedSongIndex = 0;
    } else if (m_IsForceSecondSong == 1) {
        m_FixedSongIndex = 1;
    } else {
        m_FixedSongIndex = 2;
    }
}

// 0x00245040
void SoundDJPlayer::SetForceSecondSong(bool isForced)
{
    m_IsForceSecondSong = isForced;
    if (m_IsForceFirstSong == 1) {
        m_FixedSongIndex = 0;
    } else if (isForced == 1) {
        m_FixedSongIndex = 1;
    } else {
        m_FixedSongIndex = 2;
    }
}

// 0x0024506C
void SoundDJPlayer::SetPart(u8 part, u8 nextSong)
{
    m_Part = part;
    switch (part) {
    case PART_NONE:
        m_PlayType = 0;
        break;
    case PART_MAIN:
        if (nextSong >= SoundSongQueue::SONG_FLAG) {
            m_PlayType = 3;
            break;
        }
        // fall through
    case PART_INTRO:
        m_PlayType = 2;
        break;
    case PART_FIN:
        m_PlayType = m_Queue.m_IsAlternate == 0 ? 4 : 5;
        break;
    case PART_BREAK:
    case PART_BREAK_ALTERNATE:
        m_PlayType = 1;
        break;
    }
}

// 0x002450DC
void SoundDJPlayer::SequenceCallback(u16 procId, nw::snd::SequenceUserprocCallbackParam* param, void* arg)
{
    (void)arg;
    if (s_pInstance != NULL) {
        s_pInstance->OnSequenceCallback(procId, param);
    }
}

// 0x00245170
void SoundDJPlayer::StartSequence(SoundHandleBindSeq* handle, u32 soundId, const nw::snd::SoundStartable::StartInfo* info)
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (info != NULL) {
        mgr->StartSoundChecked(handle, soundId, info);
    } else {
        mgr->StartSoundChecked(handle, soundId);
    }
    handle->BindSequence();
}

// 0x002451B8
const SoundDJPlayer::StartPosition* SoundDJPlayer::CalcStartPosition(u32 milliSeconds)
{
    StartPosition* start = &s_StartPosition;
    if (milliSeconds == 0) {
        u8 song = *m_Queue.At(m_Queue.m_Index);
        u8 nextSong = GetSongAfterCurrent();
        if (nextSong == SoundSongQueueData::INVALID_SONG) {
            SoundSongQueue::Advance(&m_Queue);
            SoundSongQueue::Advance(&m_Queue);
            SoundSongQueue::Advance(&m_Queue);
            nextSong = GetSongAfterCurrent();
        }
        if (song >= SoundSongQueue::SONG_FLAG && m_FixedSongIndex != 2) {
            song = s_FixedSongs[m_FixedSongIndex];
        }
        m_Song = song;
        if (nextSong >= SoundSongQueue::SONG_FLAG && m_FixedSongIndex != 2) {
            nextSong = s_FixedSongs[m_FixedSongIndex];
        }
        m_NextSong = nextSong;
        m_BaseTick = 0;
        ResetTiming();
        start->isSong = true;
        start->isEnding = false;
        start->position = 0;
        return start;
    }

    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    u32 ticks = static_cast<u32>(static_cast<f32>(milliSeconds) * TICKS_PER_MILLISECOND *
                                 static_cast<f32>(s_Tempos[m_DateState]));
    for (;;) {
        u8 song = *m_Queue.At(m_Queue.m_Index);
        u8 nextSong;
        if (m_Queue.m_Index + 1 < m_Queue.m_Count) {
            nextSong = *m_Queue.At(m_Queue.m_Index + 1);
        } else {
            nextSong = SoundSongQueueData::INVALID_SONG;
        }
        if (nextSong == SoundSongQueueData::INVALID_SONG) {
            SoundSongQueue::Advance(&m_Queue);
            nextSong = m_Queue.m_Index + 1 < m_Queue.m_Count ? *m_Queue.At(m_Queue.m_Index + 1)
                                                             : SoundSongQueueData::INVALID_SONG;
        }
        if (song >= SoundSongQueue::SONG_FLAG && m_FixedSongIndex != 2) {
            song = s_FixedSongs[m_FixedSongIndex];
        }
        if (nextSong >= SoundSongQueue::SONG_FLAG && m_FixedSongIndex != 2) {
            nextSong = s_FixedSongs[m_FixedSongIndex];
        }

        u32 songIndex = song & SoundSongQueue::SONG_MASK;
        u32 beats = info->GetMainBeats(songIndex);
        if (song >= SoundSongQueue::SONG_FLAG) {
            beats += info->GetIntroBeats(songIndex);
        }
        if (nextSong >= SoundSongQueue::SONG_FLAG) {
            beats += info->GetFinBeats(songIndex);
        }
        u32 songTicks = beats * TICKS_PER_BEAT;

        if (ticks + MIN_TICKS < songTicks) {
            u32 position = static_cast<u32>(SoundMgr::s_pInstance->m_TickScale *
                                            (static_cast<f32>(s_Tempos[m_DateState]) * static_cast<f32>(ticks))) + ticks;
            if (position + END_TICKS >= songTicks) {
                start->isSong = true;
                start->isEnding = true;
                start->position = songTicks - ticks;
                SeekTiming(ticks, info->GetBeatsPerBar(songIndex));
            } else {
                start->isSong = true;
                start->isEnding = false;
                start->position = position;
                m_BaseTick = 0;
                ResetTiming();
            }
            m_Song = song;
            m_NextSong = nextSong;
            return start;
        }

        ticks = songTicks <= ticks ? ticks - songTicks : 0;
        if (nextSong >= SoundSongQueue::SONG_FLAG) {
            if (ticks + MIN_TICKS < BREAK_TICKS) {
                u32 position = static_cast<u32>(SoundMgr::s_pInstance->m_TickScale *
                                                (static_cast<f32>(s_Tempos[m_DateState]) * static_cast<f32>(ticks))) + ticks;
                if (position + END_TICKS >= BREAK_TICKS) {
                    start->isSong = false;
                    start->isEnding = true;
                    start->position = BREAK_TICKS - ticks;
                    SeekTiming(ticks, 4);
                } else {
                    start->isSong = false;
                    start->isEnding = false;
                    start->position = position;
                    m_BaseTick = 0;
                    ResetTiming();
                }
                m_Song = song;
                m_NextSong = nextSong;
                return start;
            }
            ticks = ticks < BREAK_TICKS ? 0 : ticks - BREAK_TICKS;
            m_Queue.m_IsAlternate = m_Queue.m_IsAlternate == 1 ? 0 : 1;
        }
        SoundSongQueue::Advance(&m_Queue);
        m_Queue.m_Index++;
    }
}

namespace {
// the next song is taken from the queue
template <typename T>
DECOMP_ALWAYS_INLINE void NextSong(T* player, nw::snd::SequenceUserprocCallbackParam* param, u32 tick)
{
    player->m_BaseTick = tick;
    player->ResetTiming();
    player->m_Beat++;
    if (player->m_Beat >= 48) {
        player->m_Beat = 0;
    }
    param->globalVariable[GLOBAL_VARIABLE_BEAT] = player->m_Beat;
}

template <typename T>
DECOMP_ALWAYS_INLINE void TakeNextSong(T* player)
{
    player->m_Song = player->m_NextSong;
    player->m_Queue.m_Index++;
    player->m_NextSong = player->GetSongAfterCurrent();
    if (player->m_NextSong == SoundSongQueueData::INVALID_SONG) {
        SoundSongQueue::Advance(&player->m_Queue);
        player->m_NextSong = player->GetSongAfterCurrent();
    }
    if (player->m_NextSong >= SoundSongQueue::SONG_FLAG && player->m_FixedSongIndex != 2) {
        player->m_NextSong = SoundDJPlayer::s_FixedSongs[player->m_FixedSongIndex];
    }
    SoundSongQueue::Advance(&player->m_Queue);
    if (player->m_Song >= SoundSongQueue::SONG_FLAG) {
        player->m_Queue.m_IsAlternate = player->m_Queue.m_IsAlternate == 1 ? 0 : 1;
    }
    player->m_BeatCount++;
}

template <typename T>
DECOMP_ALWAYS_INLINE u32 NextHandleTick(T* player)
{
    player->m_HandleIndex = player->m_HandleIndex + 1;
    if (player->m_HandleIndex >= 2) {
        player->m_HandleIndex = 0;
    }
    SoundHandleBindSeq& handle = player->m_Handles[player->m_HandleIndex];
    return handle.m_SequenceHandle.m_pSound != NULL ? handle.m_SequenceHandle.m_pSound->GetTick() : 0;
}
} // namespace

// 0x002456AC
void SoundDJPlayer::OnSequenceCallback(u16 procId, nw::snd::SequenceUserprocCallbackParam* param)
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (procId == 0) {
        if (m_NextSong >= SoundSongQueue::SONG_FLAG) {
            m_IsBreakPending = true;
            s_SequenceLock.lock();
            SetPart(PART_FIN, SoundSongQueueData::INVALID_SONG);
            SetPartDuration(PART_FIN, m_Song);
            s_SequenceLock.unlock();
            return;
        }
        s_SequenceLock.lock();
        param->localVariable[LOCAL_VARIABLE_NEXT] = 1;
        param->globalVariable[GLOBAL_VARIABLE_SONG_COUNT]++;
        u32 tick = NextHandleTick(this);
        NextSong(this, param, tick);
        TakeNextSong(this);
        s_SequenceLock.unlock();
        m_IsSongPending = true;
    } else if (procId == 1) {
        if (m_State == STATE_SONG) {
            param->globalVariable[GLOBAL_VARIABLE_BREAK_COUNT]++;
            s_SequenceLock.lock();
            u32 tick = m_BreakHandle.m_SequenceHandle.m_pSound != NULL ? m_BreakHandle.m_SequenceHandle.m_pSound->GetTick() : 0;
            NextSong(this, param, tick);
            if (m_Queue.m_IsAlternate != 0) {
                m_State = STATE_BREAK_ALTERNATE;
                SetPart(PART_BREAK_ALTERNATE, SoundSongQueueData::INVALID_SONG);
                SetPartDuration(PART_BREAK_ALTERNATE, m_Song);
            } else {
                m_State = STATE_BREAK;
                SetPart(PART_BREAK, SoundSongQueueData::INVALID_SONG);
                SetPartDuration(PART_BREAK, m_Song);
            }
            s_SequenceLock.unlock();
        } else {
            s_SequenceLock.lock();
            param->globalVariable[GLOBAL_VARIABLE_SONG_COUNT]++;
            u32 tick = NextHandleTick(this);
            NextSong(this, param, tick);
            TakeNextSong(this);
            m_IsSongPending = true;
            m_State = STATE_SONG;
            SetPart(PART_INTRO, SoundSongQueueData::INVALID_SONG);
            SetPartDuration(PART_INTRO, m_Song);
            s_SequenceLock.unlock();
        }
    } else if (procId == 2) {
        s_SequenceLock.lock();
        SetPart(PART_MAIN, m_NextSong);
        SetPartDuration(PART_MAIN, m_Song);
        s_SequenceLock.unlock();
    }
}

// 0x00245AAC
s16 SoundDJPlayer::GetTickInBeat()
{
    return m_TickInBeat;
}

// 0x00245AB4
void SoundDJPlayer::ApplyTownMelodyKey()
{
    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    u8 key = info->GetKey(m_Song & SoundSongQueue::SONG_MASK);
    s32 transpose = info->ConvertKey(key);
    if (key >= 12) {
        transpose = static_cast<s16>(transpose + 3);
    }
    if (transpose > 6) {
        transpose = static_cast<s16>(transpose - 12);
    }
    SoundMelody* melody = &SoundMgr::s_pInstance->m_MelodyMgr.m_Melody;
    if (melody == NULL) {
        return;
    }
    s_SequenceLock.lock();
    if (m_BreakHandle.m_SequenceHandle.m_pSound != NULL) {
        for (s32 i = 0; i < static_cast<s32>(MELODY_NOTE_NUM); i++) {
            s32 note = melody->notes[i];
            s32 value;
            if (note == 13 || note == -TRANSPOSE_OFF) {
                value = static_cast<s16>(transpose - TRANSPOSE_OFF);
            } else if (note == 14 || note == 15 || note == TRANSPOSE_OFF) {
                value = TRANSPOSE_OFF;
            } else {
                value = static_cast<s16>(SoundMelodyPlayerBase::GetPitch(0, note) + transpose);
            }
            if (m_BreakHandle.m_SequenceHandle.m_pSound != NULL) {
                m_BreakHandle.m_SequenceHandle.m_pSound->WriteTrackVariable(TRANSPOSE_TRACK, i, value);
            }
        }
    }
    s_SequenceLock.unlock();
}

// 0x00245BBC
void SoundDJPlayer::UpdatePhase()
{
    s_SequenceLock.lock();
    u32 position = 0;
    if (m_State == STATE_SONG) {
        nw::snd::internal::SequenceSound* sound = m_Handles[m_HandleIndex].m_SequenceHandle.m_pSound;
        if (sound != NULL) {
            position = sound->GetTick() - m_BaseTick + m_StartPosition;
            f32 beat = static_cast<f32>(position % TICKS_PER_BEAT);
            u32 beatsPerBar = SoundKKInfo::s_pInstance->GetBeatsPerBar(GetSong());
            s32 barTicks = beatsPerBar * TICKS_PER_BEAT;
            u32 bar = position % barTicks;
            m_BeatPhase = Clamp01(beat * (1.0f / TICKS_PER_BEAT));
            m_BarPhase = Clamp01(static_cast<f32>(bar) / static_cast<f32>(barTicks));
            SetPartPosition(position);
        }
    } else if (m_State == STATE_BREAK || m_State == STATE_BREAK_ALTERNATE) {
        nw::snd::internal::SequenceSound* sound = m_BreakHandle.m_SequenceHandle.m_pSound;
        if (sound != NULL) {
            position = sound->GetTick() - m_BaseTick + m_StartPosition;
            f32 beat = static_cast<f32>(position % TICKS_PER_BEAT);
            f32 bar = static_cast<f32>(position % BREAK_BAR_TICKS);
            m_BeatPhase = Clamp01(beat * (1.0f / TICKS_PER_BEAT));
            m_BarPhase = Clamp01(bar * (1.0f / BREAK_BAR_TICKS));
            SetPartPosition(position);
        }
    }

    s32 step = position / TICKS_PER_STEP;
    if (step > static_cast<s32>(m_Position / TICKS_PER_STEP)) {
        m_BeatPosition = step * TICKS_PER_STEP;
        m_Beat++;
        if (m_Beat >= 48) {
            m_Beat = 0;
        }
        nw::snd::internal::SequenceSound::WriteGlobalVariable(GLOBAL_VARIABLE_BEAT, m_Beat);
    }
    m_Position = position;
    u32 tickInStep = position - m_BeatPosition;
    if (tickInStep > TICKS_PER_STEP) {
        tickInStep %= TICKS_PER_STEP;
    }
    m_TickInBeat = tickInStep;
    m_FramesToBeat = Ceil(static_cast<f32>(TICKS_PER_STEP - tickInStep) * FRAMES_PER_TICK / static_cast<f32>(s_Tempos[m_DateState]));
    s_SequenceLock.unlock();
}

// 0x00245EA0
void SoundDJPlayer::ResetTiming()
{
    m_Position = 0;
    m_StartPosition = 0;
    m_BeatPosition = 0;
    m_TickInBeat = 0;
    m_FramesToBeat = Ceil(FRAMES_PER_STEP / static_cast<f32>(s_Tempos[m_DateState]));
    m_BeatPhase = 0.0f;
    m_BarPhase = 0.0f;
}

// 0x00245F1C
void SoundDJPlayer::SetPartDuration(u8 part, u8 song)
{
    if (part == PART_NONE) {
        m_PartRemaining = 0.0f;
        m_PartElapsed = 0.0f;
    } else {
        m_PartElapsed = 0.0f;
        m_PartRemaining = static_cast<f32>(GetPartBeats(part, song));
    }
    m_PartElapsedFrames = 0;
    m_PartRemainingFrames = Ceil(m_PartRemaining * FRAMES_PER_BEAT / static_cast<f32>(s_Tempos[m_DateState]));
}

// 0x00245FC0
void SoundDJPlayer::SetPartPosition(u32 tick)
{
    u32 beats;
    switch (m_Part) {
    case PART_INTRO:
    case PART_MAIN:
    case PART_FIN: {
        u8 part = m_Part;
        u8 song = m_Song;
        u32 songIndex = song & SoundSongQueue::SONG_MASK;
        SoundKKInfo* info = SoundKKInfo::s_pInstance;
        beats = 0;
        if (song >= SoundSongQueue::SONG_FLAG) {
            beats = info->GetIntroBeats(songIndex);
        }
        if (part == PART_MAIN) {
            beats += info->GetMainBeats(songIndex);
        } else if (part == PART_FIN) {
            beats += info->GetMainBeats(songIndex);
            beats += info->GetFinBeats(songIndex);
        }
        break;
    }
    case PART_BREAK:
    case PART_BREAK_ALTERNATE:
        beats = GetPartBeats(m_Part, SoundSongQueueData::INVALID_SONG);
        break;
    default:
        m_PartRemaining = 0.0f;
        m_PartElapsed = 0.0f;
        m_PartElapsedFrames = 0;
        m_PartRemainingFrames = 0;
        return;
    }

    f32 ticksLeft = static_cast<f32>(beats * TICKS_PER_BEAT - tick);
    if (ticksLeft < 0.0f) {
        ticksLeft = 0.0f;
    }
    m_PartRemaining = ticksLeft * (1.0f / TICKS_PER_BEAT);
    m_PartElapsed = static_cast<f32>(GetPartBeats(m_Part, m_Song)) - m_PartRemaining;
    if (!(m_PartElapsed > 0.0f)) {
        m_PartElapsed = 0.0f;
    }
    f32 tempo = static_cast<f32>(s_Tempos[m_DateState]);
    m_PartElapsedFrames = Floor(m_PartElapsed * FRAMES_PER_BEAT / tempo);
    m_PartRemainingFrames = Ceil(m_PartRemaining * FRAMES_PER_BEAT / static_cast<f32>(s_Tempos[m_DateState]));
}

// 0x002461AC
void SoundDJPlayer::SeekTiming(u32 tick, u32 beatsPerBar)
{
    f32 beat = static_cast<f32>(tick % TICKS_PER_BEAT);
    s32 barTicks = beatsPerBar * TICKS_PER_BEAT;
    u32 bar = tick % barTicks;
    m_BaseTick = 0;
    m_Position = tick;
    m_StartPosition = tick;
    m_BeatPosition = (tick / TICKS_PER_STEP) * TICKS_PER_STEP;
    s16 tickInStep = static_cast<s16>(tick - m_BeatPosition);
    m_TickInBeat = static_cast<s16>(static_cast<u32>(static_cast<s32>(tickInStep)) % TICKS_PER_STEP);
    m_FramesToBeat = Ceil(static_cast<f32>(static_cast<u32>(TICKS_PER_STEP - m_TickInBeat)) * FRAMES_PER_TICK /
                          static_cast<f32>(s_Tempos[m_DateState]));
    m_BeatPhase = Clamp01(beat * (1.0f / TICKS_PER_BEAT));
    m_BarPhase = Clamp01(static_cast<f32>(bar) / static_cast<f32>(barTicks));
}

// 0x00246310
s16 SoundDJPlayer::Update()
{
    if (SoundMgr::s_pInstance->IsPaused() || m_State == STATE_NONE) {
        return m_BeatCount;
    }

    if (m_IsBreakPending) {
        if (m_Queue.m_IsAlternate == 1) {
            StartSequence(&m_BreakHandle, s_BreakSounds[1][m_DateState], NULL);
        } else {
            StartSequence(&m_BreakHandle, s_BreakSounds[0][m_DateState], NULL);
            ApplyTownMelodyKey();
        }
        m_IsBreakPending = false;
        m_IsBreakStarting = true;
    } else if (m_IsBreakStarting) {
        bool isStarted;
        s_SequenceLock.lock();
        nw::snd::internal::SequenceSound* sound = m_BreakHandle.m_SequenceHandle.m_pSound;
        if (sound != NULL && sound->GetTick() != 0) {
            WriteVariable(m_BreakHandle, VARIABLE_PLAY, 0);
            s_SequenceLock.unlock();
            isStarted = true;
        } else {
            s_SequenceLock.unlock();
            isStarted = false;
        }
        m_IsBreakStarting = !isStarted;
    }

    if (m_IsSongPending) {
        s_SequenceLock.lock();
        SoundHandleBindSeq& handle = m_Handles[m_HandleIndex ^ 1];
        u32 song = m_NextSong & SoundSongQueue::SONG_MASK;
        StopHandle(handle);
        if (song == SONG_LAST) {
            song = SONG_LAST_SEQUENCE;
        }
        StartSequence(&handle, s_FirstSongs[m_DateState] + song, NULL);
        s_SequenceLock.unlock();
        m_IsSongPending = false;
        m_IsSongStarting = true;
    } else if (m_IsSongStarting) {
        bool isStarted;
        s_SequenceLock.lock();
        SoundHandleBindSeq& handle = m_Handles[m_HandleIndex ^ 1];
        nw::snd::internal::SequenceSound* sound = handle.m_SequenceHandle.m_pSound;
        if (sound != NULL && sound->GetTick() != 0) {
            if (m_NextSong >= SoundSongQueue::SONG_FLAG) {
                WriteVariable(handle, VARIABLE_PLAY, 0);
            } else {
                WriteVariable(handle, VARIABLE_PLAY, 1);
            }
            s_SequenceLock.unlock();
            isStarted = true;
        } else {
            s_SequenceLock.unlock();
            isStarted = false;
        }
        m_IsSongStarting = !isStarted;
    }

    UpdatePhase();

    s8 lightMode = 0;
    if (m_Part == PART_BREAK_ALTERNATE) {
        lightMode = m_PartRemaining > 0.2f ? 1 : 0;
    } else if (m_Part == PART_FIN && m_PlayType == 5) {
        lightMode = m_PartRemaining > 0.4f ? 0 : 1;
    }
    if (m_LightMode != lightMode) {
        Func4E7AAC(lightMode, 1);
        SoundHaniwaMgr* haniwa = SoundHaniwaMgr::s_pInstance;
        u32 count = haniwa->GetDancingCount();
        if (lightMode != 0) {
            if (SoundHaniwaMgr::s_pInstance->m_ObjNum == count) {
                m_LightMode = lightMode;
            }
        } else if (count == 0) {
            m_LightMode = lightMode;
        }
    }
    return m_BeatCount;
}

// 0x0024661C
void SoundDJPlayer::Stop()
{
    if (m_State == STATE_NONE) {
        return;
    }
    s_SequenceLock.lock();
    StopHandle(m_Handles[0]);
    StopHandle(m_Handles[1]);
    StopHandle(m_BreakHandle);
    ResetTiming();
    SetPart(PART_NONE, SoundSongQueueData::INVALID_SONG);
    SetPartDuration(PART_NONE, m_Song);
    m_Song = SoundSongQueueData::INVALID_SONG;
    m_LightMode = 0;
    m_State = STATE_NONE;
    static_cast<nw::snd::SoundArchivePlayer*>(SoundMgr::s_pInstance->m_pPlayer)->SetSequenceUserprocCallback(NULL, NULL);
    s_SequenceLock.unlock();
}

// 0x002466DC
void SoundDJPlayer::Play(const SoundSongQueueData* data, u32 milliSeconds)
{
    if (!m_IsInitialized || m_State != STATE_NONE) {
        return;
    }
    data->Load(&m_Queue);
    const StartPosition* start = CalcStartPosition(milliSeconds);
    static_cast<nw::snd::SoundArchivePlayer*>(SoundMgr::s_pInstance->m_pPlayer)
        ->SetSequenceUserprocCallback(SequenceCallback, this);
    m_HandleIndex = 0;
    m_BeatCount = 0;

    if (start->isSong) {
        if (start->isEnding) {
            if (m_NextSong >= SoundSongQueue::SONG_FLAG) {
                StartSequence(&m_Handles[0], SE_DJ_CTRL_WAIT_TO_FIN, NULL);
                WriteVariable(m_Handles[m_HandleIndex], VARIABLE_TEMPO, s_Tempos[m_DateState]);
                WriteVariable(m_Handles[m_HandleIndex], VARIABLE_POSITION, start->position);
                SetPart(PART_FIN, SoundSongQueueData::INVALID_SONG);
                SetPartPosition(m_StartPosition);
                m_IsBreakPending = true;
            } else {
                StartSequence(&m_Handles[0], SE_DJ_CTRL_WAIT_TO_OUTRO, NULL);
                WriteVariable(m_Handles[m_HandleIndex], VARIABLE_TEMPO, s_Tempos[m_DateState]);
                WriteVariable(m_Handles[m_HandleIndex], VARIABLE_POSITION, start->position);
                SetPart(PART_MAIN, m_NextSong);
                SetPartPosition(m_StartPosition);
            }
        } else {
            u32 song = m_Song & SoundSongQueue::SONG_MASK;
            if (start->position != 0) {
                nw::snd::SoundStartable::StartInfo info;
                info.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
                info.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
                info.startOffset = start->position;
                if (song == SONG_LAST) {
                    song = SONG_LAST_SEQUENCE;
                }
                StartSequence(&m_Handles[m_HandleIndex], s_FirstSongs[m_DateState] + song, &info);
            } else {
                if (song == SONG_LAST) {
                    song = SONG_LAST_SEQUENCE;
                }
                StartSequence(&m_Handles[0], s_FirstSongs[m_DateState] + song, NULL);
            }
            WriteVariable(m_Handles[m_HandleIndex], VARIABLE_PLAY, m_Song >= SoundSongQueue::SONG_FLAG ? 0 : 1);
            SetPart(PART_INTRO, SoundSongQueueData::INVALID_SONG);
            SetPartDuration(PART_INTRO, m_Song);
        }
        m_State = STATE_SONG;
    } else {
        bool isAlternate = m_Queue.m_IsAlternate == 1;
        if (start->isEnding) {
            StartSequence(&m_BreakHandle, SE_DJ_CTRL_WAIT_TO_FIN, NULL);
            WriteVariable(m_BreakHandle, VARIABLE_TEMPO, s_Tempos[m_DateState]);
            WriteVariable(m_BreakHandle, VARIABLE_POSITION, start->position);
            if (isAlternate) {
                SetPart(PART_BREAK_ALTERNATE, SoundSongQueueData::INVALID_SONG);
                SetPartPosition(m_StartPosition);
                m_State = STATE_BREAK_ALTERNATE;
            } else {
                SetPart(PART_BREAK, SoundSongQueueData::INVALID_SONG);
                SetPartPosition(m_StartPosition);
                ApplyTownMelodyKey();
                m_State = STATE_BREAK;
            }
        } else {
            if (start->position != 0) {
                nw::snd::SoundStartable::StartInfo info;
                info.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
                info.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
                info.startOffset = start->position;
                StartSequence(&m_BreakHandle, s_BreakSounds[isAlternate][m_DateState], &info);
            } else {
                StartSequence(&m_BreakHandle, s_BreakSounds[isAlternate][m_DateState], NULL);
            }
            WriteVariable(m_BreakHandle, VARIABLE_PLAY, 0);
            if (isAlternate) {
                SetPart(PART_BREAK_ALTERNATE, SoundSongQueueData::INVALID_SONG);
                SetPartDuration(PART_BREAK_ALTERNATE, m_Song);
                m_State = STATE_BREAK_ALTERNATE;
            } else {
                SetPart(PART_BREAK, SoundSongQueueData::INVALID_SONG);
                SetPartDuration(PART_BREAK, m_Song);
                ApplyTownMelodyKey();
                m_State = STATE_BREAK;
            }
        }
    }
    m_IsSongPending = true;
}

// 0x00246C5C
void SoundDJPlayer::Finalize()
{
    if (!m_IsInitialized) {
        return;
    }
    Stop();
    SoundHaniwaMgr::destroyInstance();
    m_IsInitialized = false;
}

// 0x00714740
u8 SoundDJPlayer::GetSongAfterCurrent() const
{
    if (m_Queue.m_Index + 1 >= m_Queue.m_Count) {
        return SoundSongQueueData::INVALID_SONG;
    }
    return *m_Queue.At(m_Queue.m_Index + 1);
}

// 0x007147A0
u32 SoundDJPlayer::GetRandom()
{
    sead::Random random;
    random.init(m_Queue.m_Seed);
    return random.getU32();
}

// 0x007147C8
u8 SoundDJPlayer::GetBeatsPerBar() const
{
    if (m_State != STATE_SONG) {
        return 4;
    }
    return SoundKKInfo::s_pInstance->GetBeatsPerBar(m_Song & SoundSongQueue::SONG_MASK);
}

// 0x007147F4
s32 SoundDJPlayer::GetTicksPerBeat()
{
    return 24;
}

// 0x007147FC
s32 SoundDJPlayer::GetFramesPerBeat()
{
    return Ceil(FRAMES_PER_STEP / static_cast<f32>(s_Tempos[m_DateState]));
}

// 0x00714858
const SoundSongQueueData* SoundDJPlayer::GetQueueData()
{
    // 0x00983850 (guard 0x0094DD10)
    static SoundSongQueueData s_Data;
    s_Data.Store(&m_Queue);
    return &s_Data;
}

// 0x007148B0
u8 SoundDJPlayer::GetBeatsPerBar(u32 song) const
{
    return SoundKKInfo::s_pInstance->GetBeatsPerBar(song);
}

// 0x007148C0
s32 SoundDJPlayer::GetFramesToBeat()
{
    return m_FramesToBeat;
}

// 0x007148C8
f32 SoundDJPlayer::GetPartBeats() const
{
    return static_cast<f32>(GetPartBeats(m_Part, m_Song));
}

// 0x007148E4
u32 SoundDJPlayer::GetPartBeats(u8 part, u8 song) const
{
    if (part == PART_NONE) {
        return 0;
    }
    if (part == PART_BREAK || part == PART_BREAK_ALTERNATE) {
        return BREAK_BEATS;
    }
    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    u32 songIndex = song != SoundSongQueueData::INVALID_SONG ? song & SoundSongQueue::SONG_MASK : 0;
    if (part == PART_INTRO) {
        return info->GetIntroBeats(songIndex);
    }
    if (part == PART_MAIN) {
        return info->GetMainBeats(songIndex);
    }
    if (part == PART_FIN) {
        return info->GetFinBeats(songIndex);
    }
    return 0;
}

// 0x0071494C
s16 SoundDJPlayer::GetBeat()
{
    return m_Beat;
}

// 0x00714954
f32 SoundDJPlayer::GetTempo()
{
    return static_cast<f32>(s_Tempos[m_DateState]);
}
