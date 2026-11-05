#include "Sound/dSoundBgmMgr.h"
#include "Sound/dSoundDataLoadMgr.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/dSoundMuInfo.h"
#include "Sound/dSoundObjBase.h"
#include "netgame/dGameFunctions.h"
#include <cmath>
#include "nw/snd/snd_Global.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"
#include "nw/snd/internal/snd_SequenceSound.h"
#include "nw/snd/internal/snd_StreamSound.h"
#include "nw/snd/snd_SequenceSoundHandle.h"
#include "nw/snd/snd_StreamSoundHandle.h"

// 0x0083A0E0
SoundBgmMgr::SubUpdate SoundBgmMgr::s_SubUpdates[4] = {
    &SoundBgmMgr::UpdateSubNone,
    &SoundBgmMgr::UpdateCarnivalEnd,
    &SoundBgmMgr::UpdateSymbolTree,
    &SoundBgmMgr::UpdatePrologue,
};
// 0x0083A0B4
const s8 SoundBgmMgr::s_IsTrackSurround[2] = {1, 1};
// 0x0083A0B8
const f32 SoundBgmMgr::s_MinVolumes[2] = {0.3125f, 0.28f};
// 0x0083A0C0
const f32 SoundBgmMgr::s_NearDistances[2] = {60.0f, 50.0f};
// 0x0083A0C8
const f32 SoundBgmMgr::s_FarDistances[2] = {380.0f, 680.0f};
// 0x0083A0D0
const s32 SoundBgmMgr::s_FadeInFrames[2] = {4, 47};
// 0x0083A0D8
const s32 SoundBgmMgr::s_FadeOutFrames[2] = {20, 47};
// 0x0083A100
const u32 SoundBgmMgr::s_StreamTracks[7] = {0x3, 0x0, 0x0, 0x2, 0x1, 0x0, 0x0};
// 0x0083A11C
const u32 SoundBgmMgr::s_SeqTracks[7] = {0x1FFF, 0xF, 0x1C0F, 0x0, 0x0, 0x38F, 0x7F};

namespace {
// the sound id relative to the first item of the archive
inline u32 Index(u32 soundId)
{
    return soundId - 0x1000000;
}
} // namespace

// 0x001286DC
SoundBgmMgr::SoundBgmMgr()
    : m_Kind(0), m_State(STATE_NONE), m_SoundId(0xFFFFFFFF), m_IsBiquad(false), m_JingleState(0),
      m_JingleId(0xFFFFFFFF), m_ObjSong(-1), m_ObjState(0), m_ObjLoadLevel(-1), m_MixId(0xFFFFFFFF), m_Weather(0), m_WeatherVariant(-1),
      m_IsKeepVariant(false), m_MuseumRoom(0), m_MixCounter(0), m_StreamTracksOn(0), m_SeqTracksOn(0),
      m_StreamTracksOff(0), m_SeqTracksOff(0), m_pObj(NULL), _9C(false), _B4(2), m_BoostTimer(0), m_SubState(4),
      m_SubKind(0), m_SubMode(4), _E1(0), _E2(0), m_LoopStart(0), m_LoopLength(0), m_PrologueStep(5), _F0(0),
      m_VolumeDelay(0)
{
    m_WeatherIds[0] = STRM_BGM_DWNTWN_DAY_1_SUNNY;
    m_WeatherIds[1] = STRM_BGM_DWNTWN_DAY_1_SUNNY + 1;
    m_WeatherIds[2] = STRM_BGM_DWNTWN_DAY_1_SUNNY + 2;
    m_VolumeFader.SoundFader::Reset(1.0f);
}

// 0x001D8440
// 0x001D83EC (deleting dtor)
SoundBgmMgr::~SoundBgmMgr()
{
    // (only the members are destroyed)
}

s32 SoundBgmMgr::GetMuseumRoom(stage::Name stage)
{
    switch (stage) {
    case stage::NAME_MUSEUM_ENTRANCE:
    case stage::NAME_MUSEUM_SHOP:
        return 2;
    case stage::NAME_MUSEUM_FISH_0:
    case stage::NAME_MUSEUM_FISH_1:
    case stage::NAME_MUSEUM_FISH_2:
        return 3;
    case stage::NAME_MUSEUM_INSECT_0:
    case stage::NAME_MUSEUM_INSECT_1:
    case stage::NAME_MUSEUM_INSECT_2:
        return 4;
    case stage::NAME_MUSEUM_FOSSIL_0:
    case stage::NAME_MUSEUM_FOSSIL_1:
    case stage::NAME_MUSEUM_FOSSIL_2:
        return 5;
    case stage::NAME_MUSEUM_PICTURE_0:
    case stage::NAME_MUSEUM_PICTURE_1:
        return 6;
    default:
        return 2;
    }
}

// 0x001D58B0
void SoundBgmMgr::SetBoost(bool isBoost)
{
    if (m_Kind == 14) {
        if (isBoost) {
            m_BoostFader.SetTarget(1.47f, 55);
        } else {
            m_BoostFader.SetTarget(1.0f, 30);
        }
    } else if (m_Kind == 15) {
        if (isBoost) {
            m_BoostTimer = 60;
        }
    }
}

// 0x001D5904
void SoundBgmMgr::StopJingle(s32 fadeFrames)
{
    if (m_JingleHandle.IsAttached()) {
        m_JingleHandle.m_pSound->Stop(fadeFrames);
        m_JingleState = 5;
        return;
    }
    m_JingleState = 0;
    if (m_JingleId == STRM_BGM_FNFR_TRAIN_ENTRY && stage::GetCurrent() == stage::NAME_STATION_ENTRY_DEMO) {
        SoundMgr::s_pInstance->m_EnvMgr.StartLoop(SE_ENV_STATION_ENTRY_WAIT, 0);
    }
    m_JingleId = 0xFFFFFFFF;
}

// 0x001D597C
void SoundBgmMgr::StopSub(s32 fadeFrames)
{
    if (m_SubState == 2) {
        m_SubHandles[0].Stop(fadeFrames);
        m_SubState = 0;
    } else if (m_SubState == 3) {
        m_SubHandles[0].Stop(0);
        m_SubState = 4;
        m_SubKind = 0;
        _E1 = 0;
        _E2 = 0;
        m_LoopStart = 0;
        m_LoopLength = 0;
    }
}

// 0x001D59E0
void SoundBgmMgr::PlayCarnival(u8 mode)
{
    if (mode == 4) {
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    switch (mode) {
    case 0: {
        mgr->StartSound(&m_SubHandles[0], STRM_BGM_EVT_CARNIVAL);
        nw::snd::StreamSoundHandle stream(&m_SubHandles[0]);
        if (stream.m_pSound == NULL) {
            return;
        }
        stream.m_pSound->SetTrackVolume(1, 0.0f, 0);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->m_Pitch = 1.09f;
        }
        SoundDataLoadMgr::s_pInstance->Load(SEQ_BGM_EVT_CARNIVAL_END_S, 1);
        break;
    }
    case 1:
    case 3:
        m_SubHandles[0].Stop(5);
        m_SubState = 0;
        break;
    case 2: {
        mgr->StartSound(&m_SubHandles[0], STRM_BGM_EVT_CARNIVAL);
        nw::snd::StreamSoundHandle stream(&m_SubHandles[0]);
        if (stream.m_pSound == NULL) {
            return;
        }
        stream.m_pSound->SetTrackVolume(1, 0.0f, 0);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->m_Pitch = 1.15f;
        }
        break;
    }
    }
    m_SubMode = mode;
    m_SubKind = 1;
}

// 0x001D5B6C
void SoundBgmMgr::SetVolume(f32 volume, s32 frames)
{
    if (m_Handle.isAttached()) {
        m_Handle.setVolume(volume, frames);
    }
}

// 0x001D5BB0
void SoundBgmMgr::StopSubNow(s32 fadeFrames)
{
    m_SubHandles[0].Stop(fadeFrames);
}

// 0x001D5BC4
void SoundBgmMgr::ResetSub()
{
    m_SubHandles[0].Stop(0);
    m_SubHandles[1].Stop(0);
    m_SubMode = 4;
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    if (loadMgr->IsLoaded(SEQ_BGM_EVT_CARNIVAL_END_S) || loadMgr->IsLoaded(SEQ_BGM_EVT_CARNIVAL_END_L)) {
        SoundDataLoadMgr::s_pInstance->UnloadOrDefer(0, 1);
    }
    m_SubState = 4;
    m_SubKind = 0;
}

// 0x001D5C5C
void SoundBgmMgr::PlayBingo(bool isOffset)
{
    nw::snd::SoundStartable::StartInfo info;
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (isOffset) {
        u32 offset = mgr->ReadSoundUserParam(STRM_BGM_FLD_BINGO);
        if (offset != 0) {
            info.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
            info.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE;
            info.startOffset = offset;
            mgr->StartSound(&m_SubHandles[0], STRM_BGM_FLD_BINGO, &info);
            return;
        }
    }
    mgr->StartSound(&m_SubHandles[0], STRM_BGM_FLD_BINGO);
}

// 0x001D5D80
void SoundBgmMgr::ApplyTracks()
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    switch (m_Kind) {
    case 3:
        mgr->StartSound(&m_MixHandle, m_MixId);
        m_TrackFader.SoundFader::Reset(1.0f);
        m_MuseumRoom = GetMuseumRoom(stage::GetCurrent());
        CrossfadeTracks(0, m_MuseumRoom, 0);
        break;
    case 4:
        SetHomeCenterTracks(0);
        break;
    case 5:
        SetShopTracks(0);
        break;
    case 6:
        if (mgr->m_EnvMgr.IsRaining()) {
            nw::snd::StreamSoundHandle stream(&m_Handle);
            if (stream.m_pSound != NULL) {
                stream.m_pSound->SetTrackVolume(1, 0.71f, 0);
            }
        }
        break;
    case 7:
        mgr->StartSound(&m_MixHandle, m_MixId);
        if (mgr->m_EnvMgr.IsRaining()) {
            nw::snd::StreamSoundHandle stream(&m_Handle);
            if (stream.m_pSound != NULL) {
                stream.m_pSound->SetTrackVolume(1, 0.71f, 0);
            }
        }
        break;
    case 8:
        _B4 = 0;
        m_FaderA0.SoundFader::Reset(0.0f);
        UpdateDistanceTracks();
        break;
    case 9:
        _B4 = 1;
        m_FaderA0.SoundFader::Reset(0.0f);
        UpdateDistanceTracks();
        break;
    case 10: {
        m_TrackFader.SoundFader::Reset(0.0f);
        nw::snd::SequenceSoundHandle sequence(&m_Handle);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackVolume(0x80, 0.0f);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->SetTrackMute(0x80, nw::snd::SEQ_MUTE_STOP);
            }
        }
        break;
    }
    case 12: {
        nw::snd::StreamSoundHandle stream(&m_Handle);
        if (stream.m_pSound != NULL) {
            if (stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent())) {
                stream.m_pSound->SetTrackVolume(1, 1.0f, 0);
            } else {
                stream.m_pSound->SetTrackVolume(1, 1.0f, 0);
            }
        }
        break;
    }
    case 13: {
        nw::snd::StreamSoundHandle stream(&m_Handle);
        if (stream.m_pSound != NULL) {
            if (stage::IsKind(stage::KIND_INDOOR, stage::GetCurrent())) {
                stream.m_pSound->SetTrackVolume(1, 0.71f, 0);
            } else {
                stream.m_pSound->SetTrackVolume(1, 1.0f, 0);
            }
        }
        break;
    }
    case 16: {
        nw::snd::StreamSoundHandle stream(&m_Handle);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(2, 0.0f, 0);
        }
        break;
    }
    }
}

// 0x001D6120
void SoundBgmMgr::PlayPrologue()
{
    if (m_SubKind != 0) {
        return;
    }
    SoundMgr::s_pInstance->StartSound(&m_SubHandles[0], SEQ_BGM_PROLOGUE_12);
    m_PrologueStep = 0;
    m_SubKind = 3;
    _F0 = 0;
}

// 0x001D6168
void SoundBgmMgr::SetPrologueStep(u8 step)
{
    if (m_SubKind != 3) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(&m_SubHandles[0]);
    if (sequence.m_pSound == NULL) {
        return;
    }
    if (step == 1) {
        sequence.m_pSound->WriteVariable(0, 1);
    } else if (step == 2) {
        sequence.m_pSound->WriteVariable(0, 2);
        SoundMgr::s_pInstance->PrepareSound(&m_SubHandles[1], STRM_BGM_PROLOGUE_13);
    } else {
        return;
    }
    m_PrologueStep = step;
}

// 0x001D6204
void SoundBgmMgr::SetKind(u32 soundId)
{
    nw::snd::OutputMode mode = nw::snd::internal::driver::HardwareManager::GetInstance().GetOutputMode();
    u32 index = Index(soundId);
    if (index - 63 < 48 || index == 150) {
        if (mode == nw::snd::OUTPUT_MODE_SURROUND) {
            m_Kind = 1;
        }
        return;
    }
    if (index - 117 <= 28) {
        m_Kind = 2;
    } else if (index == 184) {
        m_Kind = 3;
    } else if (index == 171) {
        m_Kind = 4;
    } else if (index == 177) {
        m_Kind = 5;
    } else if (index == 198) {
        m_Kind = 6;
    } else if (index == 199 || index == 201 || index == 203 || index == 205) {
        m_Kind = 7;
    } else if (index == 147) {
        m_Kind = 8;
    } else if (index == 216) {
        m_Kind = 9;
    } else if (soundId == SEQ_BGM_IND_CAFEARBEIT) {
        m_Kind = 10;
    } else if (index == 213) {
        m_Kind = 11;
    } else if (index == 204) {
        m_Kind = 12;
    } else if (index == 174) {
        m_Kind = 13;
    } else if (index == 193) {
        m_Kind = 14;
    } else if (index == 183) {
        m_Kind = 15;
    } else {
        if (index - 227 >= 40) {
            return;
        }
        if (index == 250 || index == 251 || index == 252 || soundId == STRM_BGM_PNP_PUZZLEMODE_PLAY) {
            m_Kind = 16;
        } else if (index == 230 || index == 234) {
            if (mode == nw::snd::OUTPUT_MODE_SURROUND) {
                m_Kind = 1;
            }
        }
        SoundMgr::s_pInstance->SetExternalFileRoot(false);
        return;
    }
}

// 0x001D63C8
void SoundBgmMgr::PlaySymbolTree()
{
    if (m_SubState != 4) {
        m_SubHandles[0].Stop(0);
    }
    _E1 = 0;
    _E2 = 0;
    m_LoopStart = 0;
    m_LoopLength = 0;
    SoundMgr::s_pInstance->PrepareSound(&m_SubHandles[0], STRM_BGM_FLD_SYMBOLTREE);
    m_SubState = 3;
    m_SubKind = 2;
}

// 0x001D6430
void SoundBgmMgr::SetWeather(u8 weather)
{
    if (weather < 3) {
        m_Weather = weather;
    }
}

// 0x001D643C
void SoundBgmMgr::Update()
{
    m_VolumeFader.Update();
    if (m_VolumeDelay > 0 && --m_VolumeDelay == 0) {
        m_VolumeFader.SetTarget(1.0f, 30);
    }
    m_Handle.SetVolume(m_VolumeFader.GetValue());
    switch (m_State) {
    case STATE_PREPARING:
        if (m_Handle.IsAttached() && m_Handle.m_pSound->IsPrepared()) {
            m_State = STATE_PREPARED;
        }
        break;
    case STATE_PREPARING_START:
        if (m_Handle.IsAttached() && m_Handle.m_pSound->IsPrepared()) {
            if (m_Handle.IsAttached()) {
                m_Handle.m_pSound->StartPrepared();
            }
            m_Handle.SetVolume(m_VolumeFader.GetValue());
            ApplyTracks();
            m_State = STATE_PLAYING;
        }
        break;
    case STATE_PLAYING:
        switch (m_Kind) {
        case 2:
            UpdateDowntown();
            return;
        case 3: {
            if (m_MixCounter == 0) {
                return;
            }
            m_TrackFader.Update();
            {
                nw::snd::SequenceSoundHandle sequence(&m_MixHandle);
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->SetTrackVolume(m_SeqTracksOn, m_TrackFader.GetValue());
                }
                if (sequence.m_pSound != NULL) {
                    sequence.m_pSound->SetTrackVolume(m_SeqTracksOff, 1.0f - m_TrackFader.GetValue());
                }
            }
            if (--m_MixCounter != 0) {
                return;
            }
            nw::snd::SequenceSoundHandle sequence(&m_MixHandle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->SetTrackMute(m_SeqTracksOff, true);
            }
            return;
        }
        case 8:
        case 9:
            m_FaderA0.Update();
            if (_9C) {
                if (m_FaderA0.GetTarget() == 0.0f) {
                    m_FaderA0.SetTarget(1.0f, s_FadeInFrames[_B4]);
                }
            } else if (m_FaderA0.GetTarget() == 1.0f) {
                m_FaderA0.SetTarget(0.0f, s_FadeOutFrames[_B4]);
            }
            UpdateDistanceTracks();
            return;
        case 10: {
            m_TrackFader.Update();
            if (m_TrackFader.GetValue() <= 0.0f) {
                return;
            }
            nw::snd::SequenceSoundHandle sequence(&m_Handle);
            if (sequence.m_pSound != NULL) {
                sequence.m_pSound->SetTrackVolume(0x80, m_TrackFader.GetValue());
            }
            return;
        }
        case 14:
            m_BoostFader.Update();
            if (m_Handle.IsAttached()) {
                m_Handle.m_pSound->SetVolume(m_BoostFader.GetValue(), 0);
            }
            return;
        case 15:
            if (m_BoostTimer > 0 && --m_BoostTimer == 0) {
                SoundMgr::s_pInstance->SetVolume(0, 2.0f, 70);
            }
            return;
        default:
            break;
        }
        if (m_Handle.IsAttached()) {
            return;
        }
        Reset();
        break;
    case STATE_STOPPING:
        if (m_Kind == 2) {
            UpdateDowntown();
            return;
        }
        if (m_Handle.IsAttached()) {
            return;
        }
        Reset();
        break;
    }
}

// 0x001D67CC
void SoundBgmMgr::Play(u32 soundId)
{
    Play(soundId, 1, -1);
}

// 0x001D67D8
void SoundBgmMgr::Play(u32 soundId, u8 mode, s8 param)
{
    if (m_State != STATE_NONE) {
        Stop(0);
    }
    ResetSub();
    Reset();
    nw::snd::SoundStartable::StartInfo info;
    bool isInfo = false;
    SoundMgr* mgr = SoundMgr::s_pInstance;
    SetKind(soundId);
    switch (m_Kind) {
    case 1:
        soundId++;
        break;
    case 2: {
        // the weather variants of the Main Street BGM
        u32 step = 1;
        u32 surround = 0;
        if (Index(soundId) == 123 || Index(soundId) == 141) {
            step = 2;
            if (nw::snd::internal::driver::HardwareManager::GetInstance().GetOutputMode() ==
                nw::snd::OUTPUT_MODE_SURROUND) {
                surround = 1;
            }
        }
        m_WeatherIds[0] = soundId + surround;
        m_WeatherIds[1] = soundId + step + surround;
        m_WeatherIds[2] = soundId + step * 2 + surround;
        if (m_IsKeepVariant) {
            m_IsKeepVariant = false;
        } else if (param != 1) {
            u32 base = 0;
            if (soundId == STRM_BGM_DWNTWN_DAY_1_SUNNY) {
                base = STRM_BGM_DWNTWN_DAY_1_SUNNY - 6;
            } else if (Index(soundId) == 117) {
                base = STRM_BGM_DWNTWN_QUIET_INTR1_SUNNY;
            } else if (Index(soundId) == 120 || Index(soundId) == 123) {
                base = STRM_BGM_DWNTWN_QUIET_INTR1_RAINY;
            } else if (soundId == STRM_BGM_DWNTWN_DAY_1_SUNNY + 3 || soundId == STRM_BGM_DWNTWN_DAY_1_SUNNY + 6) {
                base = STRM_BGM_DWNTWN_DAY_INTR1_RAINY;
            }
            soundId = base + m_Weather;
            m_WeatherVariant = -1;
            m_WeatherFader.SoundFader::Reset(1.0f);
            break;
        }
        soundId = m_WeatherIds[m_Weather];
        m_WeatherVariant = m_Weather;
        m_WeatherFader.SoundFader::Reset(1.0f);
        break;
    }
    case 3:
        m_MixId = SEQ_BGM_IND_MUSEUM;
        m_MixCounter = 0;
        break;
    case 7:
        m_MixId = STRM_BGM_ISL_LAST30FANFARE;
        break;
    case 11:
        if (param == 1) {
            if (stage::GetCurrent() == stage::NAME_VILLAGE) {
                SoundVolumeMgr& volumeMgr = mgr->m_VolumeMgr;
                if (volumeMgr.GetPlayerVolume(0) < 1.0f && !volumeMgr.m_IsBgmOff) {
                    volumeMgr.SetPlayerVolume(0, 1.0f, 0);
                }
            }
            break;
        }
        // fall through
    case 12:
        if (m_Kind == 12 && stage::IsKind(stage::KIND_KOTOBUKI, stage::GetCurrent())) {
            break;
        }
        {
            u32 offset = mgr->ReadSoundUserParam(soundId);
            if (offset != 0) {
                isInfo = true;
                info.enableFlag = nw::snd::SoundStartable::StartInfo::ENABLE_START_OFFSET;
                info.startOffsetType = nw::snd::SoundStartable::StartInfo::START_OFFSET_TYPE_SAMPLE;
                info.startOffset = offset;
            } else {
                isInfo = false;
            }
        }
        break;
    case 14:
        m_BoostFader.SoundFader::Reset(1.0f);
        break;
    case 15:
        m_BoostTimer = 0;
        break;
    }

    if (mode == 1 || m_Kind == 3) {
        if (isInfo) {
            mgr->PrepareSound(&m_Handle, soundId, &info);
        } else {
            mgr->PrepareSound(&m_Handle, soundId);
        }
        m_SoundId = soundId;
        m_State = mode;
    } else {
        if (isInfo) {
            mgr->StartSound(&m_Handle, soundId, &info);
        } else {
            mgr->StartSound(&m_Handle, soundId);
        }
        if (m_Handle.IsAttached()) {
            u32 index = Index(soundId);
            m_IsBiquad = false;
            if (index - 15 <= 23 || index - 111 <= 2 || index - 117 <= 2 || index - 129 <= 2 ||
                soundId == STRM_BGM_DWNTWN_DAY_1_SUNNY) {
                m_Handle.m_pSound->SetBiquadFilter(64, mgr->m_EnvMgr.m_OutdoorFader.GetValue());
            } else if (index == 2 || index == 196) {
                m_IsBiquad = true;
                m_Handle.m_pSound->SetBiquadFilter(64, mgr->m_EnvMgr.m_OutdoorFader.GetValue());
            }
            m_Handle.SetVolume(m_VolumeFader.GetValue());
        }
        m_SoundId = soundId;
        ApplyTracks();
        m_State = STATE_PLAYING;
    }
    mgr->SetExternalFileRoot(true);
}

// 0x001D6CA0
void SoundBgmMgr::SetShopTracks(s32 frames)
{
    nw::snd::StreamSoundHandle stream(&m_Handle);
    if (stream.m_pSound == NULL) {
        return;
    }
    switch (stage::GetCurrent()) {
    case stage::NAME_ACCESSORY_SHOP:
        stream.m_pSound->SetTrackVolume(1, 0.0f, frames);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(2, 1.0f, frames);
        }
        break;
    case stage::NAME_TAILOR:
        stream.m_pSound->SetTrackVolume(1, 1.0f, frames);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(2, 0.0f, frames);
        }
        break;
    default:
        break;
    }
}

// 0x001D6D70
void SoundBgmMgr::ChangeMuseumRoom(stage::Name stage)
{
    if (m_Kind != 3) {
        return;
    }
    if (m_MuseumRoom != GetMuseumRoom(stage)) {
        CrossfadeTracks(m_MuseumRoom, 1, 20);
        m_MuseumRoom = 1;
    }
}

// 0x001D6E24
void SoundBgmMgr::StartPrepared()
{
    if (m_State == STATE_PREPARED) {
        if (m_Handle.IsAttached()) {
            m_Handle.m_pSound->StartPrepared();
        }
        m_Handle.SetVolume(m_VolumeFader.GetValue());
        ApplyTracks();
        m_State = STATE_PLAYING;
    } else if (m_State == STATE_PREPARING) {
        m_State = STATE_PREPARING_START;
    }
}

// 0x001D6E84
void SoundBgmMgr::EnterStage(stage::Name stage)
{
    switch (m_Kind) {
    case 3:
        if (m_MuseumRoom == 1) {
            s32 room = GetMuseumRoom(stage);
            CrossfadeTracks(m_MuseumRoom, room, 20);
            m_MuseumRoom = room;
        }
        break;
    case 4:
        SetHomeCenterTracks(48);
        break;
    case 5:
        SetShopTracks(52);
        break;
    }
    m_VolumeFader.SetTarget(1.0f, 0);
    m_IsKeepVariant = false;
}

// 0x001D6F94
void SoundBgmMgr::UpdateCarnivalEnd()
{
    if (m_SubMode == 4) {
        return;
    }
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    if (m_SubMode == 0) {
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    switch (m_SubMode) {
    case 1:
        if (m_SubState == 0) {
            if (m_SubHandles[0].IsAttached() || loadMgr->IsBusy(1)) {
                return;
            }
            mgr->StartSound(&m_SubHandles[1], SEQ_BGM_EVT_CARNIVAL_END_S);
            m_SubState = 2;
            return;
        }
        if (m_SubState != 2 || m_SubHandles[1].IsAttached()) {
            return;
        }
        break;
    case 2:
        if (m_SubHandles[1].IsAttached() || loadMgr->IsBusy(1) || loadMgr->IsLoaded(SEQ_BGM_EVT_CARNIVAL_END_L)) {
            return;
        }
        loadMgr->Unload(0, 1);
        loadMgr->Load(SEQ_BGM_EVT_CARNIVAL_END_L, 1);
        return;
    case 3:
        if (m_SubState == 0) {
            if (m_SubHandles[0].IsAttached() || loadMgr->IsBusy(1) || !loadMgr->IsLoaded(SEQ_BGM_EVT_CARNIVAL_END_L)) {
                return;
            }
            mgr->StartSound(&m_SubHandles[1], SEQ_BGM_EVT_CARNIVAL_END_L);
            m_SubState = 2;
            return;
        }
        if (m_SubState != 2 || m_SubHandles[1].IsAttached()) {
            return;
        }
        break;
    default:
        return;
    }
    loadMgr->Unload(0, 1);
    m_SubState = 4;
    m_SubKind = 0;
    m_SubMode = 4;
}

// 0x001D7150
void SoundBgmMgr::CrossfadeTracks(s32 from, s32 to, s32 frames)
{
    u32 streamFrom = s_StreamTracks[from];
    u32 streamTo = s_StreamTracks[to];
    u32 seqFrom = s_SeqTracks[from];
    u32 seqTo = s_SeqTracks[to];
    u32 streamDiff = streamFrom ^ streamTo;
    u32 seqDiff = seqFrom ^ seqTo;
    m_StreamTracksOff = streamDiff & streamFrom;
    m_SeqTracksOff = seqDiff & seqFrom;
    m_StreamTracksOn = streamTo & streamDiff;
    m_SeqTracksOn = seqTo & seqDiff;
    {
        nw::snd::StreamSoundHandle stream(&m_Handle);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(m_StreamTracksOn, 1.0f, frames);
        }
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(m_StreamTracksOff, 0.0f, frames);
        }
        nw::snd::SequenceSoundHandle sequence(&m_MixHandle);
        if (m_SeqTracksOn != 0 && sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackMute(m_SeqTracksOn, false);
        }
        m_TrackFader.SetTarget(0.0f, 0);
        m_TrackFader.SetTarget(1.0f, frames);
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackVolume(m_SeqTracksOn, m_TrackFader.GetValue());
        }
        if (sequence.m_pSound != NULL) {
            sequence.m_pSound->SetTrackVolume(m_SeqTracksOff, 1.0f - m_TrackFader.GetValue());
        }
    }
    if (frames != 0) {
        m_MixCounter = frames;
        return;
    }
    nw::snd::SequenceSoundHandle sequence(&m_MixHandle);
    if (sequence.m_pSound != NULL) {
        sequence.m_pSound->SetTrackMute(m_SeqTracksOff, true);
    }
}

inline void SoundBgmMgr::Reset()
{
    m_SoundId = 0xFFFFFFFF;
    m_MixId = 0xFFFFFFFF;
    m_Kind = 0;
    m_State = STATE_NONE;
    m_IsBiquad = false;
    m_WeatherVariant = -1;
    m_MuseumRoom = 0;
}

inline void SoundBgmMgr::ApplyBiquad()
{
    u32 index = Index(m_SoundId);
    m_IsBiquad = false;
    if (index - 15 <= 23 || index - 111 <= 2 || index - 117 <= 2 || index - 129 <= 2 ||
        m_SoundId == STRM_BGM_DWNTWN_DAY_1_SUNNY) {
        m_Handle.m_pSound->SetBiquadFilter(64, SoundMgr::s_pInstance->m_EnvMgr.m_OutdoorFader.GetValue());
    } else if (index == 2 || index == 196) {
        m_IsBiquad = true;
        m_Handle.m_pSound->SetBiquadFilter(64, SoundMgr::s_pInstance->m_EnvMgr.m_OutdoorFader.GetValue());
    }
}

// 0x001D72D4
void SoundBgmMgr::UpdateDowntown()
{
    if (!m_Handle.IsAttached()) {
        // the next variant for the weather
        s8 variant = 0;
        switch (m_WeatherVariant) {
        case -1:
            variant = m_Weather;
            break;
        case 0:
            if (m_Weather == 1) {
                variant = 1;
            } else if (m_Weather == 2) {
                variant = static_cast<s8>(netgame::Func2FF63C(2) + 1);
            }
            break;
        case 1:
            if (m_Weather == 2) {
                variant = static_cast<s8>(netgame::Func2FF63C(2));
                if (variant == 1) {
                    variant = 2;
                }
            }
            break;
        case 2:
            if (m_Weather == 2) {
                variant = static_cast<s8>(netgame::Func2FF63C(2));
            }
            break;
        }
        m_WeatherVariant = variant;
        if (static_cast<u32>(variant) >= 3) {
            m_WeatherVariant = 0;
        }
        SoundMgr::s_pInstance->StartSoundChecked(&m_Handle, m_WeatherIds[m_WeatherVariant]);
        m_SoundId = m_WeatherIds[m_WeatherVariant];
        if (m_Handle.IsAttached()) {
            ApplyBiquad();
        }
    }
    m_WeatherFader.Update();
    if (m_WeatherFader.GetValue() > 0.0f) {
        if (m_Handle.IsAttached()) {
            m_Handle.m_pSound->SetVolume(m_WeatherFader.GetValue(), 0);
        }
        return;
    }
    m_SoundId = 0xFFFFFFFF;
    m_Handle.Stop(0);
    m_State = STATE_NONE;
    m_Kind = 0;
}

// 0x001D74C8
void SoundBgmMgr::UpdateSubNone()
{
    // empty (in the original too)
}

// 0x001D74CC
void SoundBgmMgr::UpdateSymbolTree()
{
    if (m_SubState == 0) {
        if (!m_SubHandles[0].IsAttached()) {
            m_SubState = 4;
            m_SubKind = 0;
            _E1 = 0;
            _E2 = 0;
            m_LoopStart = 0;
            m_LoopLength = 0;
        }
        return;
    }
    if (m_SubState != 2) {
        return;
    }
    nw::snd::StreamSoundHandle stream(&m_SubHandles[0]);
    if (stream.m_pSound == NULL) {
        m_SubState = 4;
        m_SubKind = 0;
        _E1 = 0;
        _E2 = 0;
        m_LoopStart = 0;
        m_LoopLength = 0;
        return;
    }
    if (m_LoopStart == 0) {
        nw::snd::internal::StreamDataInfo info;
        if (stream.m_pSound->ReadStreamDataInfo(&info)) {
            m_LoopStart = info.loopStart;
            m_LoopLength = info.loopEnd - info.loopStart;
        }
        if (stream.m_pSound == NULL) {
            return;
        }
    }
    s32 position = stream.m_pSound->GetPlaySamplePosition(true);
    if (position < 0 || stream.m_pSound == NULL) {
        return;
    }
    s32 loops = stream.m_pSound->GetPlayLoopCount();
    if (loops > 0) {
        position += m_LoopLength * loops;
    }
    if (position < 0) {
        return;
    }
    if (!_E1 && position > 0x65800) {
        _E1 = 1;
    }
    if (!_E2 && position > 0x357C00) {
        _E2 = 1;
    }
}

// 0x001D762C
void SoundBgmMgr::Duck(bool isDown)
{
    if (isDown) {
        m_VolumeFader.SetTarget(0.0f, 2);
        m_VolumeDelay = 0;
    } else {
        m_VolumeDelay = 30;
    }
}

// 0x001D7660
void SoundBgmMgr::PlayJingle(u32 soundId)
{
    PlayJingle(soundId, 4);
}

// 0x001D7668
void SoundBgmMgr::PlayJingle(u32 soundId, u8 mode)
{
    if (m_JingleHandle.IsAttached()) {
        m_JingleHandle.m_pSound->Stop(0);
        m_JingleState = 5;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (Index(soundId) == 158) {
        if (nw::snd::internal::driver::HardwareManager::GetInstance().GetOutputMode() == nw::snd::OUTPUT_MODE_SURROUND) {
            soundId++;
        }
    } else if (Index(soundId) - 227 < 0x438) {
        mgr->SetExternalFileRoot(false);
    }
    if (mode == 2) {
        mgr->PrepareSound(&m_JingleHandle, soundId);
        m_JingleState = 2;
    } else {
        mgr->StartSound(&m_JingleHandle, soundId);
        m_JingleState = 4;
        m_IsKeepVariant = true;
    }
    if (mgr->m_FxMgr.m_FxType == SoundFxMgr::FX_DREAM) {
        nw::snd::SoundArchive::SoundInfo info;
        if (mgr->ReadSoundInfo(&info, soundId) && info.playerId == PLAYER_BGM_FANFARE) {
            if (m_JingleHandle.IsAttached()) {
                m_JingleHandle.m_pSound->SetFxSend(nw::snd::AUX_BUS_A, SoundFxMgr::s_SendLevels[SoundFxMgr::FX_DREAM]);
            }
        }
    }
    m_JingleId = soundId;
    mgr->SetExternalFileRoot(true);
}

// 0x001D7784
void SoundBgmMgr::StopObjSong(s32 fadeFrames)
{
    if (m_ObjHandle.IsAttached()) {
        m_ObjHandle.m_pSound->Stop(fadeFrames);
        m_ObjState = 5;
    } else if (m_ObjState != 0) {
        m_ObjState = 2;
    }
}

// 0x001D77BC
void SoundBgmMgr::UpdateDistanceTracks()
{
    if (!m_Handle.IsAttached()) {
        return;
    }
    f32 nearVolume;
    f32 farVolume = 1.0f;
    if (m_pObj == NULL) {
        nearVolume = 0.0f;
    } else {
        f32 distance = sqrtf(m_pObj->m_PosX * m_pObj->m_PosX + m_pObj->m_PosZ * m_pObj->m_PosZ);
        f32 nearDistance = s_NearDistances[_B4];
        f32 farDistance = s_FarDistances[_B4];
        if (nearDistance > distance) {
            distance = nearDistance;
        } else if (distance > farDistance) {
            distance = farDistance;
        }
        f32 c = -cosf(3.1415927f * (distance - nearDistance) / (farDistance - nearDistance));
        f32 minVolume = s_MinVolumes[_B4];
        nearVolume = m_FaderA0.GetValue() * ((1.0f - c) * 0.5f);
        farVolume = (minVolume + (1.0f + (1.0f - minVolume) * c)) * 0.5f;
    }
    nw::snd::StreamSoundHandle stream(&m_Handle);
    if (stream.m_pSound != NULL) {
        stream.m_pSound->SetTrackVolume(1, farVolume, 0);
    }
    if (stream.m_pSound != NULL) {
        stream.m_pSound->SetTrackVolume(2, nearVolume, 0);
    }
    if (nearVolume <= 0.0f) {
        return;
    }
    m_pObj->Calc();
    if (s_IsTrackSurround[_B4] == 1) {
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackPan(2, m_pObj->m_Pan);
        }
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackSurroundPan(2, m_pObj->m_SurroundPan);
        }
    } else {
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackPan(2, m_pObj->m_Pan);
        }
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackSurroundPan(2, 0.0f);
        }
    }
}

// 0x001D79A0
void SoundBgmMgr::UpdatePrologue()
{
    if (m_PrologueStep == 2) {
        if (m_SubHandles[0].IsAttached()) {
            return;
        }
        if (m_SubHandles[1].IsAttached()) {
            m_SubHandles[1].m_pSound->StartPrepared();
        }
        m_PrologueStep = 3;
    } else if (m_PrologueStep == 3) {
        if (_F0 == 0) {
            if (m_SubHandles[1].IsAttached()) {
                return;
            }
            _F0 = 35;
            SoundMgr::s_pInstance->PrepareSound(&m_SubHandles[0], STRM_BGM_PLAYERSELECT);
            return;
        }
        if (--_F0 != 0) {
            return;
        }
        if (m_SubHandles[0].IsAttached()) {
            m_SubHandles[0].m_pSound->StartPrepared();
        }
        m_PrologueStep = 4;
    }
}

// 0x001D7A44
void SoundBgmMgr::SetPuzzleLevel(f32 level)
{
    if (!m_Handle.IsAttached() || m_Kind != 16) {
        return;
    }
    f32 volume1 = 1.0f;
    f32 volume2 = 0.0f;
    if (level > 50.0f) {
        volume2 = (level - 50.0f) * 0.02f;
        volume1 = 1.0f - volume2 * 0.8f;
    }
    f32 pitch = 1.0f;
    if (level > 75.0f) {
        pitch = 1.0f + (level - 75.0f) * 0.04f * 0.70000005f;
    }
    {
        nw::snd::StreamSoundHandle stream(&m_Handle);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(1, volume1, 0);
            if (stream.m_pSound != NULL) {
                stream.m_pSound->SetTrackVolume(2, volume2, 0);
            }
        }
    }
    if (m_Handle.IsAttached()) {
        m_Handle.m_pSound->SetPitch(pitch);
    }
}

// 0x001D7B68
void SoundBgmMgr::StartCafe()
{
    if (m_Kind != 10) {
        return;
    }
    nw::snd::SequenceSoundHandle sequence(&m_Handle);
    if (sequence.m_pSound == NULL) {
        return;
    }
    sequence.m_pSound->SetTrackMute(0x80, nw::snd::SEQ_MUTE_OFF);
    m_TrackFader.SetTarget(1.0f, 40);
}

// 0x001D7BCC
void SoundBgmMgr::StartJinglePrepared()
{
    if (m_JingleState == 3) {
        if (m_JingleHandle.IsAttached()) {
            m_JingleHandle.m_pSound->StartPrepared();
        }
        m_JingleState = 4;
        m_IsKeepVariant = true;
    } else if (m_JingleState == 2) {
        m_JingleState = 1;
    }
}

// 0x001D7C0C
void SoundBgmMgr::StartSubPrepared()
{
    if (m_SubState == 3) {
        if (m_SubHandles[0].IsAttached()) {
            m_SubHandles[0].m_pSound->StartPrepared();
        }
        m_SubState = 2;
    }
}

// 0x001D7C38
void SoundBgmMgr::PlayObjSong(u32 song)
{
    if (song >= SoundMuInfo::SONG_NUM) {
        return;
    }
    if (m_ObjState == 2) {
        m_ObjState = 3;
    } else if (m_ObjState == 5) {
        m_ObjState = 6;
    } else if (m_ObjState == 0) {
        m_ObjLoadLevel = SoundDataLoadMgr::s_pInstance->Load(SoundMuInfo::s_pInstance->GetSoundId(song), 1);
        m_ObjState = 1;
    } else {
        return;
    }
    m_ObjSong = song;
}

// 0x001D7CB0
void SoundBgmMgr::SetHomeCenterTracks(s32 frames)
{
    nw::snd::StreamSoundHandle stream(&m_Handle);
    if (stream.m_pSound == NULL) {
        return;
    }
    switch (stage::GetCurrent()) {
    case stage::NAME_HOME_CENTER:
        stream.m_pSound->SetTrackVolume(1, 1.0f, frames);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(2, 0.0f, frames);
        }
        break;
    case stage::NAME_FLORISTS_HOME_IMPROVEMENT_CENTER:
        stream.m_pSound->SetTrackVolume(1, 0.0f, frames);
        if (stream.m_pSound != NULL) {
            stream.m_pSound->SetTrackVolume(2, 1.0f, frames);
        }
        break;
    default:
        break;
    }
}

// 0x001D7D80
void SoundBgmMgr::SetTrackVolume(u32 trackBitFlag, f32 volume, s32 frames)
{
    if (!m_Handle.isAttached()) {
        return;
    }
    nw::snd::StreamSoundHandle stream(&m_Handle);
    if (stream.m_pSound != NULL) {
        stream.m_pSound->SetTrackVolume(trackBitFlag, volume, frames);
    }
}

// 0x001D7DE4
void SoundBgmMgr::calc()
{
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (mgr->IsPaused()) {
        return;
    }
    Update();
    switch (m_JingleState) {
    case 1:
        if (m_JingleHandle.IsAttached() && m_JingleHandle.m_pSound->IsPrepared()) {
            if (m_JingleHandle.IsAttached()) {
                m_JingleHandle.m_pSound->StartPrepared();
            }
            m_JingleState = 4;
            m_IsKeepVariant = true;
        }
        break;
    case 2:
        if (m_JingleHandle.IsAttached() && m_JingleHandle.m_pSound->IsPrepared()) {
            m_JingleState = 3;
        }
        break;
    case 4:
    case 5:
        if (!m_JingleHandle.IsAttached()) {
            m_JingleState = 0;
            if (m_JingleId == STRM_BGM_FNFR_TRAIN_ENTRY && stage::GetCurrent() == stage::NAME_STATION_ENTRY_DEMO) {
                mgr->m_EnvMgr.StartLoop(SE_ENV_STATION_ENTRY_WAIT, 0);
            }
            m_JingleId = 0xFFFFFFFF;
        }
        break;
    }
    UpdateObjSong();
    (this->*s_SubUpdates[m_SubKind])();
    if (m_IsBiquad && m_Handle.IsAttached()) {
        ApplyBiquad();
    }
    _9C = false;
}

// 0x001D7FC4
void SoundBgmMgr::Stop()
{
    Stop(90);
}

// 0x001D7FCC
void SoundBgmMgr::Stop(s32 fadeFrames)
{
    switch (m_State) {
    case STATE_PREPARING:
    case STATE_PREPARING_START:
    case STATE_PREPARED:
        m_Handle.Stop(0);
        m_MixHandle.Stop(0);
        Reset();
        break;
    case STATE_PLAYING:
    case STATE_STOPPING:
        if (m_Kind == 2) {
            if (fadeFrames > 0) {
                m_WeatherFader.SetTarget(0.0f, fadeFrames);
                m_State = STATE_STOPPING;
                return;
            }
            m_Handle.Stop(0);
            m_MixHandle.Stop(0);
            Reset();
            return;
        }
        if (!m_Handle.IsAttached()) {
            Reset();
            return;
        }
        if (fadeFrames > 0) {
            m_Handle.m_pSound->Stop(fadeFrames);
            m_MixHandle.Stop(fadeFrames);
            m_State = STATE_STOPPING;
            return;
        }
        m_Handle.m_pSound->Stop(0);
        m_MixHandle.Stop(0);
        Reset();
        break;
    }
}

// 0x001D80FC
void SoundBgmMgr::Pause(bool isPause, s32 frames)
{
    if (!m_Handle.isAttached()) {
        return;
    }
    if (isPause) {
        m_Handle.pause(frames);
    } else {
        m_Handle.unpause(frames);
    }
}

// 0x001D8140
void SoundBgmMgr::PlayNow(u32 soundId, s8 param)
{
    Play(soundId, 2, param);
}

// 0x001D814C
void SoundBgmMgr::ClearObj(SoundObjBase* obj)
{
    if (m_pObj == obj) {
        m_pObj = NULL;
    }
}

// 0x001D8160
void SoundBgmMgr::StopEvent()
{
    ResetSub();
}

// 0x001D8164
void SoundBgmMgr::UpdateObjSong()
{
    SoundDataLoadMgr* loadMgr = SoundDataLoadMgr::s_pInstance;
    SoundMuInfo* muInfo = SoundMuInfo::s_pInstance;
    switch (m_ObjState) {
    case 1:
        if (loadMgr->IsBusy(1)) {
            return;
        }
        SoundMgr::s_pInstance->StartSound(&m_ObjHandle, muInfo->GetSoundId(m_ObjSong));
        if (m_pObj != NULL) {
            m_pObj->Calc();
            m_ObjHandle.SetVolume(m_pObj->m_Volume);
            m_ObjHandle.SetPan(m_pObj->m_Pan);
            m_ObjHandle.SetSurroundPan(m_pObj->m_SurroundPan);
        }
        if (stage::GetCurrent() == stage::NAME_CAMP_NPC) {
            m_ObjHandle.SetBiquadFilter(65, 0.1f);
        }
        m_ObjState = 4;
        break;
    case 2:
        if (loadMgr->IsBusy(1)) {
            return;
        }
        if (m_ObjLoadLevel != -1) {
            loadMgr->Unload(m_ObjLoadLevel, 1);
            m_ObjLoadLevel = -1;
        }
        m_ObjState = 0;
        break;
    case 3:
        if (loadMgr->IsBusy(1)) {
            return;
        }
        if (m_ObjLoadLevel != -1) {
            loadMgr->Unload(m_ObjLoadLevel, 1);
            m_ObjLoadLevel = -1;
        }
        m_ObjLoadLevel = loadMgr->Load(muInfo->GetSoundId(m_ObjSong), 1);
        m_ObjState = 1;
        break;
    case 4:
        if (m_ObjHandle.IsAttached()) {
            if (m_pObj == NULL) {
                return;
            }
            m_pObj->Calc();
            m_ObjHandle.SetVolume(m_pObj->m_Volume);
            m_ObjHandle.SetPan(m_pObj->m_Pan);
            m_ObjHandle.SetSurroundPan(m_pObj->m_SurroundPan);
            return;
        }
        if (m_ObjLoadLevel != -1) {
            loadMgr->Unload(m_ObjLoadLevel, 1);
            m_ObjLoadLevel = -1;
        }
        m_ObjState = 0;
        m_ObjSong = -1;
        break;
    case 5:
        if (m_ObjHandle.IsAttached()) {
            return;
        }
        if (m_ObjLoadLevel != -1) {
            loadMgr->Unload(m_ObjLoadLevel, 1);
            m_ObjLoadLevel = -1;
        }
        m_ObjState = 0;
        m_ObjSong = -1;
        break;
    case 6:
        if (m_ObjHandle.IsAttached()) {
            return;
        }
        if (m_ObjLoadLevel != -1) {
            loadMgr->Unload(m_ObjLoadLevel, 1);
            m_ObjLoadLevel = -1;
        }
        m_ObjLoadLevel = loadMgr->Load(muInfo->GetSoundId(m_ObjSong), 1);
        m_ObjState = 1;
        break;
    }
}
