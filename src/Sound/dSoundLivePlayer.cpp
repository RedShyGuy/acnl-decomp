#include "Sound/dSoundLivePlayer.h"
#include "Sound/dSoundLivePlayer_SingletonDisposer_.h"
#include "Sound/dSoundFxMgr.h"
#include "Sound/dSoundKKInfo.h"
#include "Sound/dSoundMgr.h"
#include "Other/dHeapInit.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "sead/seadExpHeap.h"
#include "sead/seadNew.h"

namespace {
const u32 DEFAULT_VALUE = 1000000;
const u32 END_TICK = 0x7FFFFFFF;
const s32 TEMPO_TABLE_TRACK = 0;
const s32 CONTROL_TRACK = 14;
const u8 VARIABLE_BEATS = 4;
const u8 VARIABLE_START = 2;
const u8 VARIABLE_MARK = 3;
const u8 VARIABLE_SPEED = 32;
const s32 MARK_END = 2;
const u32 MAIN_TRACK = 2;
const u32 SONG_NUM = 94;
const u32 EXTRA_SONG_FIRST = 91;
const u32 END_MARGIN_MS = 2000;
const f32 TEMPO_MILLISECONDS = 1250.0f;
const f32 MILLISECONDS_TICKS = 0.0008f;
const f32 MEASURE_PHASE_OFFSET = 0.36666667f;
const f32 INTRO_SCALE = 1800.0f;
const f32 INTRO_FRAMES = 33.333332f;
const f32 FRAMES_PER_MILLISECOND = 0.03f;
const f32 MARK_LOOKAHEAD = 0.04f;

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

inline f32 SpeedOf(u32 value, f32 other)
{
    if (value == 5) {
        return 0.5f;
    }
    if (value == 10) {
        return 1.0f;
    }
    if (value == 15) {
        return 1.5f;
    }
    if (value == 20) {
        return 2.0f;
    }
    return other;
}

// the phase of the measure: the cycle starts a little before the beat
inline f32 MeasurePhase(f32 value)
{
    value += MEASURE_PHASE_OFFSET;
    if (value > 1.0f) {
        value -= 1.0f;
    }
    if (value < 0.0f) {
        value = 0.0f;
    }
    return value;
}
} // namespace

// 0x0094E06C
SoundLivePlayer* SoundLivePlayer::s_pInstance = NULL;
// 0x0094E070
SoundLivePlayer::SingletonDisposer_* SoundLivePlayer::s_pSingletonDisposer = NULL;
// the ticks the timeline is read ahead
// 0x0094E074
u32 SoundLivePlayer::s_LookaheadTicks = 1;
// 0x0094E078
u32 SoundLivePlayer::s_TicksPerBeat = 48;

// 0x002A5088
SoundLivePlayer::SoundLivePlayer()
    : m_IsPlaying(false), m_Song(0xFFFFFFFF), m_SoundId(0xFFFFFFFF), m_Tempo(120), m_LengthMs(0), m_EndMs(0), m_IntroFrames(0),
      m_Wait(0), m_NoteKind(-1), m_Variable0(-1), m_Variable1(0), m_Variable2(-1), m_Mark(-1), m_BeatsPerBar(4),
      m_BarPhase(0.0f), m_MeasurePhase(0.0f), m_BeatPhase(0.0f), m_IsAllocated(false), m_IsLatencyCorrected(true), m_MarkTick(0)
{
}

// 0x002A3C20
SoundLivePlayer* SoundLivePlayer::createInstance(sead::Heap* heap)
{
    if (s_pInstance == NULL) {
        u8* buffer = new (heap, 4) u8[sizeof(SoundLivePlayer)];
        SoundLivePlayer* instance = NULL;
        if (buffer == NULL) {
            s_pSingletonDisposer = NULL;
        } else {
            s_pSingletonDisposer = new (buffer) SingletonDisposer_(heap);
            instance = new (buffer) SoundLivePlayer();
        }
        s_pInstance = instance;
    }
    return s_pInstance;
}

// 0x0012371C
void SoundLivePlayer::destroyInstance()
{
    if (s_pSingletonDisposer != NULL) {
        SingletonDisposer_* disposer = s_pSingletonDisposer;
        s_pSingletonDisposer = NULL;
        disposer->~SingletonDisposer_();
        if (s_pInstance != NULL) {
            s_pInstance->~SoundLivePlayer();
            ::operator delete(s_pInstance);
        }
        s_pInstance = NULL;
    }
}

// 0x002A3BA8
void SoundLivePlayer::AllocateBuffers()
{
    if (m_IsAllocated) {
        return;
    }
    sead::ExpHeap* heap = HeapInit::s_pStageHeap;
    if (heap == NULL) {
        return;
    }
    for (s32 i = 0; i < NOTE_TABLE_NUM; i++) {
        m_pNoteTables[i] = new (heap, 4) Entry[s_NoteTableSizes[i]];
        if (m_pNoteTables[i] == NULL) {
            return;
        }
    }
    m_IsAllocated = true;
}

// 0x002A3C88
void SoundLivePlayer::Prepare(u32 soundId)
{
    SoundSeqParser* parser = SoundSeqParser::s_pInstance;
    if (parser == NULL) {
        return;
    }
    parser->Load(soundId);
    for (u8 i = 0; i < NOTE_TABLE_NUM; i++) {
        parser->Parse(m_pNoteTables[i], s_NoteTracks[i], SoundSeqParser::MODE_LONGEST_NOTE, s_NoteTableSizes[i]);
    }
    parser->Parse(m_Tempos, TEMPO_TABLE_TRACK, SoundSeqParser::MODE_TEMPO, TEMPO_NUM);

    Entry beats[3];
    parser->ParseVariable(beats, TEMPO_TABLE_TRACK, VARIABLE_BEATS, 3);
    m_BeatsPerBar = beats[0].tempo == 3 ? 3 : 4;
    parser->ParseVariable(m_SpeedEntries, CONTROL_TRACK, VARIABLE_SPEED, SPEED_NUM);

    // the notes of all tables in the order of their ticks
    m_Notes.Clear();
    s32 positions[NOTE_TABLE_NUM] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    while (m_Notes.m_Count < m_Notes.m_Capacity) {
        u8 table = 0;
        u32 tick = END_TICK;
        for (u8 i = 0; i < NOTE_TABLE_NUM; i++) {
            u32 entryTick = m_pNoteTables[i][positions[i]].tick;
            if (entryTick < tick) {
                table = i;
                tick = entryTick;
            }
        }
        if (tick == END_TICK) {
            break;
        }
        NoteEvent note;
        note.tick = tick;
        note.length = m_pNoteTables[table][positions[table]].tempo;
        note.table = table;

        // a note ends where the next one of the same part starts
        NoteEvent* last = m_Notes.Get(m_Notes.m_Count - 1);
        if (last != NULL) {
            u8 track = s_NoteTracks[table];
            if (track == MAIN_TRACK) {
                if (s_NoteTracks[last->table] == MAIN_TRACK && last->length + 48 > tick - last->tick) {
                    last->length = tick - last->tick;
                }
            } else if (track != 8 && track != 10 && track != 11) {
                switch (s_NoteTracks[last->table]) {
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 12:
                case 13:
                    if (last->length + 24 > tick - last->tick) {
                        last->length = tick - last->tick;
                    }
                    break;
                }
            }
        }
        m_Notes.PushBack(note);
        positions[table]++;
    }

    // the speed of the show
    m_Speeds.Clear();
    if (m_SpeedEntries[0].tick == END_TICK) {
        m_Speeds.PushBack(s_DefaultSpeed);
    } else {
        f32 speed = SpeedOf(m_SpeedEntries[0].tempo, -1.0f);
        u32 tick = m_SpeedEntries[0].tick;
        if (tick == 0) {
            SpeedEvent first = {0, speed, 0};
            m_Speeds.PushBack(first);
        } else {
            m_Speeds.PushBack(s_DefaultSpeed);
            u32 barTick = tick % (m_BeatsPerBar * s_TicksPerBeat);
            if (speed == -1.0f) {
                speed = 1.0f;
            }
            SpeedEvent event = {tick, speed, barTick};
            m_Speeds.PushBack(event);
        }
        for (s32 i = 1; i < SPEED_NUM; i++) {
            u32 entryTick = m_SpeedEntries[i].tick;
            if (entryTick == END_TICK) {
                break;
            }
            u32 value = m_SpeedEntries[i].tempo;
            if (value != 5 && value != 10 && value != 15 && value != 20) {
                continue;
            }
            f32 entrySpeed = SpeedOf(value, 0.0f);
            SpeedEvent* last = m_Speeds.Get(m_Speeds.m_Count - 1);
            if (last == NULL) {
                continue;
            }
            u32 barTick = (static_cast<u32>(static_cast<f32>(entryTick - last->tick) / last->speed) + last->barTick) %
                          (m_BeatsPerBar * s_TicksPerBeat);
            SpeedEvent event = {entryTick, entrySpeed, barTick};
            m_Speeds.PushBack(event);
        }
    }

    // where the show starts and ends
    Entry starts[4];
    parser->ParseVariable(starts, CONTROL_TRACK, VARIABLE_START, 4);
    u32 start = 0;
    for (s32 i = 0; i < 4; i++) {
        if (starts[i].tempo == 0) {
            start = starts[i].tick;
            break;
        }
    }
    parser->ParseVariable(m_Marks, CONTROL_TRACK, VARIABLE_MARK, MARK_NUM);
    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    u32 end = 0xFFFFFFFF;
    for (s32 i = 0; i < MARK_NUM; i++) {
        u32 value = m_Marks[i].tempo;
        if (value == DEFAULT_VALUE) {
            break;
        }
        if (value == MARK_END) {
            end = m_Marks[i].tick;
            break;
        }
    }
    if (end == 0xFFFFFFFF) {
        end = info->GetEndTick(m_Song);
    }

    u32 milliSeconds = 0;
    if (start < end) {
        f32 sum = 0.0f;
        for (s32 i = 0; i < TEMPO_NUM - 1; i++) {
            u32 next = m_Tempos[i + 1].tick;
            if (next <= start) {
                continue;
            }
            u32 from = m_Tempos[i].tick;
            if (start > from) {
                from = start;
            }
            bool isLast = false;
            if (next == END_TICK) {
                next = info->GetEndTick(m_Song);
                isLast = true;
            }
            if (next > end) {
                next = end;
                isLast = true;
            }
            sum += static_cast<f32>(next - from) * TEMPO_MILLISECONDS / static_cast<f32>(static_cast<s32>(m_Tempos[i].tempo));
            if (isLast) {
                break;
            }
        }
        milliSeconds = static_cast<u32>(sum);
    }
    m_EndMs = milliSeconds - END_MARGIN_MS;
}

// 0x002A43D4
bool SoundLivePlayer::StartSound(u32 tick)
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    bool isStarted;
    if (tick != 0) {
        if (SoundKKInfo::s_pInstance->GetEndTick(m_Song) <= tick) {
            m_Song = 0xFFFFFFFF;
            m_SoundId = 0xFFFFFFFF;
            return false;
        }
        nw::snd::SoundStartable::StartInfo startInfo;
        startInfo.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
        startInfo.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
        startInfo.startOffset = tick;
        isStarted = mgr->StartSoundChecked(&m_Handle, m_SoundId, &startInfo);
    } else {
        isStarted = mgr->StartSoundChecked(&m_Handle, m_SoundId);
    }
    if (isStarted && m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->SetFxSend(nw::snd::AUX_BUS_A, SoundFxMgr::s_SendLevels[SoundFxMgr::FX_CLUB]);
    }
    return isStarted;
}

// 0x002A4654
void SoundLivePlayer::UpdateCountdown()
{
    f32 ratio = static_cast<f32>(m_IntroFrames - m_Wait) / static_cast<f32>(m_IntroFrames);
    s16 beatsPerBar = m_BeatsPerBar;
    m_BarPhase = Clamp01((static_cast<f32>(beatsPerBar - 1) + ratio) / static_cast<f32>(beatsPerBar));
    m_BeatPhase = Clamp01(ratio);
    u32 beats = SoundKKInfo::s_pInstance->Get6(m_Song);
    m_MeasurePhase = MeasurePhase((static_cast<f32>(static_cast<s32>(beats - 1)) + ratio) / static_cast<f32>(beats));
}

// 0x002A4764
void SoundLivePlayer::Update()
{
    if (SoundMgr::s_pInstance->IsPaused() || !m_IsPlaying) {
        return;
    }
    if (m_Wait != 0) {
        UpdateCountdown();
        if (--m_Wait == 0) {
            StartSound(0);
        }
        return;
    }
    if (m_Handle.m_pSound == NULL) {
        m_IsPlaying = false;
        m_LengthMs = 0;
        m_EndMs = 0;
        return;
    }

    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    u32 tick = sequence.m_pSound != NULL ? sequence.m_pSound->GetTick() : 0;
    u32 now = tick + s_LookaheadTicks;

    // the note that plays
    s16 index;
    for (index = -1; index < m_Notes.m_Count - 1; index++) {
        if (m_Notes.At(index + 1)->tick > now) {
            break;
        }
    }
    if (index != -1 && m_Notes.At(index)->tick + m_Notes.At(index)->length >= now) {
        m_NoteKind = s_NoteKinds[m_Notes.At(index)->table];
    } else {
        m_NoteKind = -1;
    }

    s16 value = -1;
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(0, &value);
    }
    if (static_cast<u32>(value + 1) <= 2) {
        m_Variable0 = value;
    }
    value = -1;
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(1, &value);
    }
    if (static_cast<u32>(value) <= 1) {
        m_Variable1 = value;
    }
    value = -1;
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->ReadVariable(2, &value);
    }
    if (static_cast<u32>(value + 1) <= 2) {
        m_Variable2 = m_Variable2 >= value ? m_Variable2 : value;
    }

    // the mark a little ahead
    for (s32 i = 0; i < TEMPO_NUM - 1; i++) {
        u32 next = m_Tempos[i + 1].tick;
        if (next == END_TICK || next > tick) {
            s32 tempo = m_Tempos[i].tempo;
            if (tempo != 0) {
                u32 ahead = static_cast<u32>(static_cast<f32>(tempo) * MARK_LOOKAHEAD) + tick;
                if (m_MarkTick > ahead) {
                    ahead = m_MarkTick;
                }
                s16 mark = -1;
                for (s32 j = -1; j < MARK_NUM - 1; j++) {
                    if (m_Marks[j + 1].tick > ahead) {
                        if (j >= 0) {
                            mark = m_Marks[j].tempo;
                        }
                        break;
                    }
                }
                m_MarkTick = ahead;
                m_Mark = mark;
            }
            break;
        }
    }

    // the phases
    u32 barTicks = m_BeatsPerBar * s_TicksPerBeat;
    s16 speedIndex;
    for (speedIndex = -1; speedIndex < m_Speeds.m_Count - 1; speedIndex++) {
        if (m_Speeds.At(speedIndex + 1)->tick > now) {
            break;
        }
    }
    if (speedIndex >= 0) {
        SpeedEvent* speed = m_Speeds.At(speedIndex);
        f32 position = static_cast<f32>(now - speed->tick) / speed->speed;
        u32 barTick = (static_cast<u32>(position) + speed->barTick) % barTicks;
        m_BarPhase = Clamp01(static_cast<f32>(barTick) / static_cast<f32>(barTicks));
    }
    u32 measureTicks = SoundKKInfo::s_pInstance->Get6(m_Song) * 48;
    m_MeasurePhase = MeasurePhase(static_cast<f32>(now % measureTicks) / static_cast<f32>(static_cast<s32>(measureTicks)));
    measureTicks >>= 1;
    m_BeatPhase = Clamp01(static_cast<f32>(now % measureTicks) / static_cast<f32>(measureTicks));
}

// 0x002A4D44
u32 SoundLivePlayer::CalcStartTick(u32 milliSeconds)
{
    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    u32 ticks = 0;
    u32 result = 0xFFFFFFFF;
    f32 sum = 0.0f;
    for (s32 i = 0; i < TEMPO_NUM - 1; i++) {
        u32 next = m_Tempos[i + 1].tick;
        u32 length;
        if (next != END_TICK) {
            length = next - m_Tempos[i].tick;
        } else {
            length = info->GetEndTick(m_Song) - m_Tempos[i].tick;
        }
        s32 tempo = m_Tempos[i].tempo;
        f32 time = static_cast<f32>(length) * TEMPO_MILLISECONDS / static_cast<f32>(tempo);
        if (result == 0xFFFFFFFF && static_cast<u32>(time + sum) >= milliSeconds) {
            m_Tempo = tempo;
            u32 done = static_cast<u32>(sum);
            u32 rest = milliSeconds > done ? milliSeconds - done : 0;
            result = static_cast<u32>(static_cast<f32>(tempo * rest) * MILLISECONDS_TICKS) + ticks;
        }
        sum += time;
        ticks += length;
        if (m_Tempos[i + 1].tick == END_TICK || i == TEMPO_NUM - 2) {
            m_LengthMs = static_cast<u32>(sum);
            if (result == 0xFFFFFFFF) {
                result = info->GetEndTick(m_Song);
                m_Tempo = m_Tempos[i].tempo;
            }
            break;
        }
    }
    return result;
}

// 0x002A4E88
bool SoundLivePlayer::PlayExtra(u32 index, u32 milliSeconds)
{
    return Play(index + EXTRA_SONG_FIRST, milliSeconds);
}

// 0x002A4E90
bool SoundLivePlayer::Play(u32 song, u32 milliSeconds)
{
    if (!m_IsAllocated) {
        return false;
    }
    if (m_IsPlaying) {
        return false;
    }
    SoundKKInfo* info = SoundKKInfo::s_pInstance;
    if (song < SONG_NUM) {
        m_Song = song;
        m_SoundId = info->GetSoundId(song);
    } else {
        m_Song = 0;
        m_SoundId = info->GetSoundId(0);
    }
    m_NoteKind = -1;
    m_Variable0 = -1;
    m_Variable1 = 0;
    m_Variable2 = -1;
    m_Mark = -1;
    m_BeatsPerBar = 4;
    m_BarPhase = 0.0f;
    m_MeasurePhase = MEASURE_PHASE_OFFSET;
    m_BeatPhase = 0.0f;
    m_MarkTick = 0;
    Prepare(m_SoundId);

    // (the original reads the length of the first note as a float here)
    union
    {
        u32 bits;
        f32 value;
    } length;
    length.bits = m_Notes.At(0)->length;
    u32 introMs = static_cast<u32>(length.value * INTRO_SCALE / static_cast<f32>(static_cast<s32>(m_Tempos[0].tempo)) *
                                   INTRO_FRAMES);
    bool isStarted;
    if (introMs >= milliSeconds) {
        CalcStartTick(milliSeconds);
        m_IntroFrames = static_cast<u32>(static_cast<f32>(introMs) * FRAMES_PER_MILLISECOND);
        m_Wait = static_cast<u32>(static_cast<f32>(introMs - milliSeconds) * FRAMES_PER_MILLISECOND);
        if (m_Wait == 0) {
            StartSound(0);
        }
        UpdateCountdown();
        isStarted = true;
    } else {
        u32 tick = CalcStartTick(milliSeconds - introMs);
        if (m_IsLatencyCorrected) {
            tick += static_cast<u32>(SoundMgr::s_pInstance->m_TickScale *
                                     (static_cast<f32>(m_Tempo) * static_cast<f32>(tick)));
        }
        isStarted = StartSound(tick);
    }
    m_IsPlaying = isStarted;
    return isStarted;
}
