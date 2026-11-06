#include "Sound/dSoundObjFurnitureOrgel.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundSeqParser.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include <cmath>

namespace {
const u32 SONG_NUM = 91;
const u32 SONG_FRAMES = 2760;
// from here on the music box winds down
const u32 WIND_DOWN_FRAME = 1770;
const f32 WIND_DOWN_TEMPO = 40.0f;
const f32 HALF_PI = 1.5707964f;
const f32 WIND_DOWN_SCALE = 0.001010101f; // 1 / 990
const f32 TICKS_PER_FRAME = 0.026666667f; // per beat per minute
const f32 WIND_DOWN_START_TICKS = 47.2f;
const f32 WIND_DOWN_TICKS_PER_FRAME = 1.0666667f;
const s32 FADE_IN_FRAMES = 60;
const u32 DEFAULT_TEMPO = 1000000;
const u32 MUTE_ALL_TRACKS = 0xFFFF;

// the tempo ratio while winding down
inline f32 WindDownRatio(u32 tempo, u32 frame)
{
    f32 value = cosf(HALF_PI * static_cast<f32>(frame - WIND_DOWN_FRAME) * WIND_DOWN_SCALE);
    return (WIND_DOWN_TEMPO + (static_cast<f32>(tempo) - WIND_DOWN_TEMPO) * value) / static_cast<f32>(tempo);
}

inline void ApplyObj(SoundObjFurnitureOrgel* obj)
{
    obj->UpdateChanged();
    if (obj->m_Handle.m_pSound != NULL) {
        obj->m_Handle.m_pSound->SetVolume(obj->m_Volume * obj->m_Fader.GetValue(), 0);
    }
    obj->m_Handle.SetPan(obj->m_Pan);
    obj->m_Handle.SetSurroundPan(obj->m_SurroundPan);
}
} // namespace

// the loop of each song in ticks
// 0x0094E278
SoundObjFurnitureOrgel::Loop SoundObjFurnitureOrgel::s_Loops[91] = {
    {0, 2016},   {0, 1536},   {0, 1152},   {480, 2784}, {562, 3634}, {492, 3756},  {0, 3264},   {0, 4608},
    {0, 3072},   {48, 3120},  {0, 3456},   {0, 1536},   {0, 3840},   {384, 3456},  {768, 3840}, {0, 2880},
    {936, 2472}, {0, 2304},   {0, 5184},   {0, 3072},   {0, 2880},   {0, 3456},    {384, 2688}, {0, 3648},
    {0, 3744},   {24, 2712},  {0, 1920},   {0, 3840},   {0, 1920},   {0, 2112},    {0, 3648},   {0, 2304},
    {16, 5776},  {0, 3840},   {0, 2304},   {0, 4800},   {0, 3072},   {1536, 4608}, {0, 3840},   {0, 1920},
    {72, 1896},  {0, 2496},   {192, 2112}, {36, 2340},  {768, 2880}, {0, 3840},    {0, 2928},   {48, 3504},
    {385, 4225}, {0, 4032},   {384, 3840}, {72, 2376},  {0, 1152},   {0, 3552},    {0, 3888},   {0, 1920},
    {0, 1920},   {0, 4608},   {0, 1536},   {96, 3168},  {0, 3456},   {0, 3840},    {0, 2304},   {0, 2880},
    {0, 4896},   {384, 5184}, {0, 2304},   {768, 4608}, {48, 4080},  {0, 3264},    {0, 2880},   {0, 3072},
    {0, 1800},   {0, 4608},   {48, 3696},  {0, 5040},   {0, 4608},   {0, 2304},    {0, 2688},   {0, 1728},
    {0, 2304},   {0, 3456},   {6, 1734},   {0, 2304},   {96, 2400},  {48, 2640},   {48, 3312},  {96, 5472},
    {0, 2880},   {0, 3456},   {48, 4368},
};
// x: the frequency, y: the ticks while winding down
// 0x0094E270
nn::math::VEC2 SoundObjFurnitureOrgel::s_WindDown(0x1.9fef26p-10f, 0x1.3b2076p+9f);

// 0x0079EB18 (static initializer)

// 0x00332B38
SoundObjFurnitureOrgel::SoundObjFurnitureOrgel()
    : m_State(STATE_NONE), m_Song(0xFFFFFFFF), m_SoundId(0xFFFFFFFF), m_Frame(0), m_Wait(0), m_Tempo(120), m_TickRate(0.0f),
      m_WindDownTicks(0.0f)
{
    m_Priority = 4;
}

// 0x00332C14
// 0x00332BA8 (deleting dtor)
SoundObjFurnitureOrgel::~SoundObjFurnitureOrgel()
{
    StopSong();
}

// 0x00332370
void SoundObjFurnitureOrgel::Pause(bool isPause)
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (isPause) {
        if (m_State == STATE_PLAYING) {
            if (m_Handle.m_pSound == NULL || !m_Handle.m_pSound->IsPause()) {
                nw::snd::SequenceSoundHandle sequence(&m_Handle);
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->SetTrackMute(MUTE_ALL_TRACKS, nw::snd::SEQ_MUTE_RELEASE);
                }
                if (m_Handle.m_pSound != NULL) {
                    m_Handle.m_pSound->Pause(true, 0);
                }
            }
            m_State = STATE_PAUSED;
        } else if (m_State == STATE_WAITING) {
            m_State = STATE_WAITING_PAUSED;
        }
    } else {
        if (m_State == STATE_PAUSED) {
            if (m_Handle.m_pSound != NULL && m_Handle.m_pSound->IsPause()) {
                if (m_Handle.m_pSound != NULL) {
                    m_Handle.m_pSound->Pause(false, 0);
                }
            }
            nw::snd::SequenceSoundHandle sequence(&m_Handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->SetTrackMute(0xFFFFFFFF, nw::snd::SEQ_MUTE_OFF);
            }
            m_State = STATE_PLAYING;
        } else if (m_State == STATE_WAITING_PAUSED) {
            m_State = STATE_WAITING;
        }
    }
}

// 0x00332494
void SoundObjFurnitureOrgel::Play(u32 song, s32 wait)
{
    if (song >= SONG_NUM || m_State != STATE_NONE) {
        return;
    }
    if (wait < 0) {
        m_State = STATE_WAITING;
        m_Song = song;
        m_Wait = wait;
        return;
    }
    Start(song, wait);
}

// 0x003324CC
u32 SoundObjFurnitureOrgel::GetSongFrames(u32 song)
{
    return song < SONG_NUM ? SONG_FRAMES : 0;
}

// 0x003324E0
void SoundObjFurnitureOrgel::Start(u32 song, u32 frame)
{
    if (GetSongFrames(song) <= frame) {
        m_Wait = 0;
        m_Song = 0xFFFFFFFF;
        m_State = STATE_NONE;
        return;
    }
    m_Song = song;
    m_SoundId = song + SEQ_BGM_RM01_SANBIKA;
    m_Frame = frame;
    m_TickRate = 0.0f;
    m_WindDownTicks = 0.0f;
    SoundSeqParser* parser = SoundSeqParser::s_pInstance;
    parser->Load(m_SoundId);
    SoundSeqParser::Entry tempo[2];
    parser->Parse(tempo, 0, SoundSeqParser::MODE_TEMPO, 2);
    if (tempo[0].tempo == DEFAULT_TEMPO) {
        return;
    }
    m_Tempo = tempo[0].tempo;
    SoundMgr* mgr = SoundMgr::s_pInstance;

    if (frame == 0) {
        if (!mgr->StartSoundChecked(&m_Handle, m_SoundId)) {
            return;
        }
        m_Fader.Reset(1.0f);
    } else {
        f32 ratio = 1.0f;
        if (frame > WIND_DOWN_FRAME) {
            ratio = WindDownRatio(m_Tempo, frame);
        }
        u32 tick;
        if (frame <= WIND_DOWN_FRAME) {
            tick = static_cast<u32>(static_cast<f32>(m_Tempo) * TICKS_PER_FRAME * static_cast<f32>(frame));
        } else {
            if (m_TickRate == 0.0f) {
                m_TickRate = static_cast<f32>(m_Tempo) * TICKS_PER_FRAME;
                m_WindDownTicks = static_cast<f32>(static_cast<s32>(m_Tempo - 40)) * TICKS_PER_FRAME * s_WindDown.y;
            }
            u32 start = static_cast<u32>(static_cast<f32>(m_Tempo) * WIND_DOWN_START_TICKS);
            u32 elapsed = frame - WIND_DOWN_FRAME;
            tick = start + static_cast<u32>(sinf(s_WindDown.x * static_cast<f32>(elapsed)) * m_WindDownTicks +
                                            static_cast<f32>(elapsed) * WIND_DOWN_TICKS_PER_FRAME);
        }
        const Loop& loop = s_Loops[m_Song];
        if (loop.end <= tick) {
            tick = (tick - loop.start) % (loop.end - loop.start) + loop.start;
            tick += static_cast<u32>(mgr->m_TickScale * (static_cast<f32>(m_Tempo) * static_cast<f32>(tick)));
        }
        nw::snd::SoundStartable::StartInfo startInfo;
        startInfo.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
        startInfo.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_TICK;
        startInfo.startOffset = tick;
        if (!mgr->StartSoundChecked(&m_Handle, m_SoundId, &startInfo)) {
            return;
        }
        if (ratio != 1.0f) {
            nw::snd::SequenceSoundHandle sequence(&m_Handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->SetTempoRatio(ratio);
            }
        }
        m_Fader.Reset(0.0f);
        m_Fader.SetTarget(1.0f, FADE_IN_FRAMES);
    }
    ApplyObj(this);
    m_State = STATE_PLAYING;
}

// 0x003328F4
void SoundObjFurnitureOrgel::Update(const nn::math::VEC3* world, const nn::math::VEC3* relative, f32 frame)
{
    SoundObjFurniture::Update(world, relative, frame);
    m_Fader.SoundFader::Update();
    if (m_State == STATE_PLAYING) {
        if (GetSongFrames(m_Song) <= m_Frame) {
            if (m_Handle.m_pSound != NULL) {
                m_Handle.m_pSound->Stop(0);
            }
            m_SoundId = 0xFFFFFFFF;
            m_Song = 0xFFFFFFFF;
            m_Frame = 0;
            m_State = STATE_NONE;
            return;
        }
        f32 ratio = 1.0f;
        if (m_Frame > WIND_DOWN_FRAME) {
            ratio = WindDownRatio(m_Tempo, m_Frame);
        }
        ApplyObj(this);
        if (ratio < 1.0f) {
            nw::snd::SequenceSoundHandle sequence(&m_Handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->SetTempoRatio(ratio);
            }
        }
        m_Frame++;
    } else if (m_State == STATE_WAITING && m_Wait < 0) {
        if (++m_Wait == 0) {
            Start(m_Song, 0);
        }
    }
}

// 0x00332AA8
void SoundObjFurnitureOrgel::Stop()
{
    StopSong();
}

// 0x00332AF4
void SoundObjFurnitureOrgel::StopSong()
{
    if (m_State == STATE_NONE) {
        return;
    }
    if (m_Handle.m_pSound != NULL) {
        m_Handle.m_pSound->Stop(0);
    }
    m_Song = 0xFFFFFFFF;
    m_Frame = 0;
    m_SoundId = 0xFFFFFFFF;
    m_Wait = 0;
    m_State = STATE_NONE;
}
