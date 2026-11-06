#include "Sound/dSoundEnvMgr.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "nw/snd/internal/snd_SequenceSound.h"

// 0x0083B894
const s32 SoundEnvMgr::s_OutdoorFadeFrames[2] = {750, 360};
// 0x0083B89C
const SoundEnvMgr::RainSound SoundEnvMgr::s_RainSounds[2] = {
    {{0.6f, 0.8f}, {1.6f, 1.6f}, {5, 5}, {0.83f, 0.5f}},  // SE_ENV_RAIN(_START)
    {{0.6f, 0.8f}, {1.6f, 1.6f}, {5, 5}, {0.88f, 0.6f}},  // SE_ENV_RAIN_HEAVY
};
// 0x0083B8DC
const u32 SoundEnvMgr::s_TourSoundIds[2][2] = {
    {SE_ENV_TOUR_LOBBY_DAY, SE_ENV_TOUR_LOBBY_NIGHT},
    {SE_ENV_TOUR_GAME_DAY, SE_ENV_TOUR_GAME_NIGHT},
};

// 0x00128868
SoundEnvMgr::SoundEnvMgr()
    : m_RainState(RAIN_NONE), m_PrevRainState(RAIN_NONE), m_IsStopped(true), m_RainArea(RAIN_AREA_NONE),
      m_IsStation(0), m_RainLevel(0), m_IsRainFading(false), m_IsHeavyRainFading(false), m_IsRainPending(false),
      m_TourKind(2), m_PositionX(-1), m_PositionY(-1)
{
    m_RainRange.from = 0;
    m_RainRange.to = 0;
    m_RainRange.pivot = 0;
    m_HeavyRainRange.from = 0;
    m_HeavyRainRange.to = 0;
    m_HeavyRainRange.pivot = 0;
    m_RainFader.SoundFader::Reset(0.0f);
    m_HeavyRainFader.SoundFader::Reset(0.0f);
    m_OutdoorFader.SoundFader::Reset(0.0f);
    m_TourFaders[0].SoundFader::Reset(1.0f);
    m_TourFaders[1].SoundFader::Reset(1.0f);
}

// 0x001D9A2C
// 0x001D99C8 (deleting dtor)
SoundEnvMgr::~SoundEnvMgr()
{
    // (only the members are destroyed)
}

// 0x001D8494
void SoundEnvMgr::SetRainState(s32 state)
{
    m_RainState = state;
}

// 0x001D849C
void SoundEnvMgr::StartSea()
{
    if (stage::GetCurrent() == stage::NAME_SEA_RETURN) {
        SoundMgr::s_pInstance->StartSound(&m_SeaHandle, SE_ENV_OCEAN);
    }
}

// 0x001D84D4
void SoundEnvMgr::SetPosition(s32 x, s32 y)
{
    m_PositionX = x;
    m_PositionY = y;
}

// 0x001D84E0
void SoundEnvMgr::StartLoop(u32 soundId, s32 slot)
{
    m_LoopHandles[slot].Stop(0);
    SoundMgr::s_pInstance->StartSound(&m_LoopHandles[slot], soundId);
}

// 0x001D8698
void SoundEnvMgr::StopLoops(s32 fadeFrames)
{
    m_LoopHandles[0].Stop(fadeFrames);
    m_LoopHandles[1].Stop(fadeFrames);
}

// 0x001D8518
void SoundEnvMgr::StartRainNow()
{
    m_IsRainPending = false;
    if (m_RainArea != RAIN_AREA_OUTDOOR || m_RainState >= RAIN_RAIN || m_IsRainFading || m_IsHeavyRainFading) {
        return;
    }
    if (Func22A20C() <= 0) {
        if (stage::GetCurrent() == stage::NAME_SEA_DEPARTURE) {
            m_IsRainPending = true;
        }
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    s32 kind = Func62FF00();
    if (kind == 3) {
        if (!m_RainHandle.IsAttached()) {
            mgr->StartSound(&m_RainHandle, SE_ENV_RAIN);
            m_RainHandle.SetVolume(0.0f);
        }
        m_IsRainFading = true;
        m_RainRange.from = RAIN_LEVEL;
        m_RainRange.SetTarget(0, 1.0f);
    } else if (kind == 4) {
        if (!m_HeavyRainHandle.IsAttached()) {
            mgr->StartSound(&m_HeavyRainHandle, SE_ENV_RAIN_HEAVY);
            m_HeavyRainHandle.SetVolume(0.0f);
        }
        m_IsHeavyRainFading = true;
        m_HeavyRainRange.from = HEAVY_RAIN_LEVEL;
        m_HeavyRainRange.SetTarget(0, 1.0f);
    } else {
        return;
    }
    m_RainLevel = -1;
}

// 0x001D86CC
void SoundEnvMgr::UpdateRain()
{
    if (m_RainArea == RAIN_AREA_NONE) {
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;

    // the sound of the previous state fades out
    switch (m_PrevRainState) {
    case RAIN_RAIN:
        if (m_RainArea == RAIN_AREA_OUTDOOR) {
            if (!m_RainHandle.IsAttached()) {
                mgr->StartSound(&m_RainHandle, SE_ENV_RAIN);
                if (!m_RainHandle.IsAttached()) {
                    break;
                }
                m_RainHandle.SetVolume(0.0f);
                if (!m_RainHandle.IsAttached()) {
                    break;
                }
            }
            m_IsRainFading = true;
            m_RainRange.from = RAIN_LEVEL;
            if (m_RainState == RAIN_HEAVY) {
                m_RainRange.SetTarget(HEAVY_RAIN_LEVEL, 1.0f);
            } else {
                m_RainRange.SetTarget(0, 1.0f);
            }
            m_RainLevel = -1;
        } else {
            m_RainHandle.Stop(300);
        }
        break;
    case RAIN_HEAVY:
        if (m_RainArea == RAIN_AREA_OUTDOOR) {
            if (!m_HeavyRainHandle.IsAttached()) {
                mgr->StartSound(&m_HeavyRainHandle, SE_ENV_RAIN_HEAVY);
                if (!m_HeavyRainHandle.IsAttached()) {
                    break;
                }
                m_HeavyRainHandle.SetVolume(0.0f);
                if (!m_HeavyRainHandle.IsAttached()) {
                    break;
                }
            }
            m_IsHeavyRainFading = true;
            m_HeavyRainRange.from = HEAVY_RAIN_LEVEL;
            if (m_RainState == RAIN_RAIN || m_RainState == RAIN_START) {
                m_HeavyRainRange.SetTarget(RAIN_LEVEL, 1.0f);
            } else {
                m_HeavyRainRange.SetTarget(0, 1.0f);
            }
            m_RainLevel = -1;
        } else {
            m_HeavyRainHandle.Stop(300);
        }
        break;
    }

    // the outdoor sounds fade with the rain
    if (m_RainState != RAIN_NONE) {
        if (m_OutdoorFader.GetValue() < 1.0f) {
            if (mgr->IsBgmFlag14()) {
                m_OutdoorFader.SetTarget(1.0f, s_OutdoorFadeFrames[stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent())]);
            } else {
                m_OutdoorFader.SetTarget(1.0f, 0);
            }
        }
    } else {
        if (m_OutdoorFader.GetValue() <= 0.0f) {
            return;
        }
        if (mgr->IsBgmFlag14()) {
            m_OutdoorFader.SetTarget(0.0f, s_OutdoorFadeFrames[stage::IsKind(stage::KIND_ISLAND, stage::GetCurrent())]);
        } else {
            m_OutdoorFader.SetTarget(0.0f, 0);
        }
    }

    // the sound of the new state
    const RainSound& rain = s_RainSounds[0];
    const RainSound& heavy = s_RainSounds[1];
    switch (m_RainState) {
    case RAIN_START:
        if (!m_RainHandle.IsAttached()) {
            mgr->StartSound(&m_RainHandle, SE_ENV_RAIN_START);
            if (m_RainArea == RAIN_AREA_INDOOR && m_RainHandle.IsAttached()) {
                m_RainHandle.SetVolume(rain.volume[m_IsStation]);
                m_RainHandle.SetSurroundPan(rain.surroundPan[m_IsStation]);
                m_RainHandle.SetBiquadFilter(rain.biquadType[m_IsStation], rain.biquadValue[m_IsStation]);
            }
        }
        m_RainState = RAIN_RAIN;
        break;
    case RAIN_RAIN:
        if (m_RainHandle.IsAttached()) {
            break;
        }
        mgr->StartSound(&m_RainHandle, SE_ENV_RAIN);
        if (m_RainArea == RAIN_AREA_OUTDOOR) {
            m_RainHandle.SetVolume(0.0f);
            m_IsRainFading = true;
            m_RainRange.from = m_PrevRainState == RAIN_HEAVY ? HEAVY_RAIN_LEVEL : 0;
            m_RainRange.SetTarget(RAIN_LEVEL, 1.0f);
            m_RainLevel = -1;
        } else if (m_RainArea == RAIN_AREA_INDOOR) {
            if (m_PrevRainState == RAIN_HEAVY) {
                m_RainHandle.FadeIn(600);
            }
            if (m_RainHandle.IsAttached() && m_RainArea == RAIN_AREA_INDOOR) {
                m_RainHandle.SetVolume(rain.volume[m_IsStation]);
                m_RainHandle.SetSurroundPan(rain.surroundPan[m_IsStation]);
                m_RainHandle.SetBiquadFilter(rain.biquadType[m_IsStation], rain.biquadValue[m_IsStation]);
            }
        }
        break;
    case RAIN_HEAVY:
        if (m_HeavyRainHandle.IsAttached()) {
            break;
        }
        mgr->StartSound(&m_HeavyRainHandle, SE_ENV_RAIN_HEAVY);
        if (m_RainArea == RAIN_AREA_OUTDOOR) {
            m_HeavyRainHandle.SetVolume(0.0f);
            m_IsHeavyRainFading = true;
            m_HeavyRainRange.from = m_PrevRainState == RAIN_RAIN ? RAIN_LEVEL : 0;
            m_HeavyRainRange.SetTarget(HEAVY_RAIN_LEVEL, 0.4f);
            m_RainLevel = -1;
        } else if (m_RainArea == RAIN_AREA_INDOOR) {
            m_HeavyRainHandle.FadeIn(300);
            if (m_HeavyRainHandle.IsAttached() && m_RainArea == RAIN_AREA_INDOOR) {
                m_HeavyRainHandle.SetVolume(heavy.volume[m_IsStation]);
                m_HeavyRainHandle.SetSurroundPan(heavy.surroundPan[m_IsStation]);
                m_HeavyRainHandle.SetBiquadFilter(heavy.biquadType[m_IsStation], heavy.biquadValue[m_IsStation]);
            }
        }
        break;
    }
}

// 0x001D8D1C
void SoundEnvMgr::OnStageChange()
{
    stage::Name current = stage::GetCurrent();
    stage::Name next = stage::GetNext();
    SoundMgr* mgr = SoundMgr::s_pInstance;
    if (current == stage::NAME_SEA_DEPARTURE) {
        m_SeaHandle.Stop(20);
    } else if (current == stage::NAME_TOUR_DESK) {
        if (stage::IsKind(stage::KIND_TOUR, next) || stage::IsKind(stage::KIND_ISLAND_DEMO, next)) {
            nw::snd::SoundHandle handle;
            mgr->StartSound(&handle, SE_SYS_TOUR_SHIP_WHISTLE);
        }
    }
    if (next == stage::NAME_SEA_DEPARTURE) {
        mgr->StartSound(&m_SeaHandle, SE_ENV_OCEAN);
    }
}

// 0x001D8DE8
void SoundEnvMgr::StartShaper()
{
    StartLoop(SE_ENV_WAV_SHAPER, 0);
    StartLoop(SE_ESCAPE_ENV_SHAPER, 1);
}

// 0x001D8E18
void SoundEnvMgr::UpdateRainVariables()
{
    s16 isRaining = 0;
    s16 level = 0;
    if (m_RainArea == RAIN_AREA_OUTDOOR) {
        isRaining = static_cast<u32>(m_RainState - RAIN_RAIN) <= RAIN_HEAVY - RAIN_RAIN;
        if (!m_IsStopped) {
            s32 rain = Func22A20C();
            f32 value;
            if (rain < RAIN_LEVEL) {
                value = SoundMgr::Lerp(rain, 0, RAIN_LEVEL, 0.0f, 64.0f);
            } else {
                value = SoundMgr::Lerp(rain, RAIN_LEVEL, HEAVY_RAIN_LEVEL, 64.0f, 127.0f);
            }
            s32 i = static_cast<s32>(value);
            if (i < 0) {
                i = 0;
            } else if (i > 127) {
                i = 127;
            }
            level = i;
        }
    }
    nw::snd::internal::SequenceSound::WriteGlobalVariable(10, isRaining);
    nw::snd::internal::SequenceSound::WriteGlobalVariable(11, level);
}

// 0x001D8F0C
void SoundEnvMgr::StartStage()
{
    m_IsStopped = false;
    stage::Name current = stage::GetCurrent();
    s32 area;
    if (stage::IsKind(stage::KIND_INCLUDE_SAVE, current) || stage::IsKind(stage::KIND_MUSEUM, current) ||
        stage::IsKind(stage::KIND_CLUB, current) || current == stage::NAME_TRAIN ||
        current == stage::NAME_INCLUDE_SAVE || current == stage::NAME_GARDEN_MANUAL_IMPORT ||
        current == stage::NAME_TAKUMI_IMPORT || current == stage::NAME_GARDEN_PLUS_UPDATE ||
        current == stage::NAME_AMIIBO_CAMERA || current == stage::NAME_PLAYER_SELECT ||
        current == stage::NAME_UPDATE_SELECT || current == stage::NAME_DREAM || current == stage::NAME_FORTUNE_TENT ||
        current == stage::NAME_FORTUNE || current == stage::NAME_RESET_MONITORING_CENTER ||
        current == stage::NAME_PLAYER_HOUSE_0_BF || current == stage::NAME_PLAYER_HOUSE_1_BF ||
        current == stage::NAME_PLAYER_HOUSE_2_BF || current == stage::NAME_PLAYER_HOUSE_3_BF) {
        area = RAIN_AREA_NONE;
    } else if (stage::IsKind(stage::KIND_INDOOR, current)) {
        m_IsStation = stage::IsKind(stage::KIND_TRAIN_STATION_ENVIRONMENT, current);
        area = RAIN_AREA_INDOOR;
    } else {
        area = stage::IsKind(stage::KIND_OUTDOOR, current) ? RAIN_AREA_OUTDOOR : RAIN_AREA_NONE;
    }
    m_RainArea = area;
    if (m_RainState != RAIN_NONE) {
        m_OutdoorFader.SoundFader::Reset(1.0f);
        if (m_RainState == RAIN_START) {
            m_RainState = RAIN_RAIN;
        }
    }
    UpdateRain();
    m_PrevRainState = m_RainState;
    UpdateRainVariables();
    StartRainNow();

    SoundMgr* mgr = SoundMgr::s_pInstance;
    switch (current) {
    case stage::NAME_MUSEUM_FISH_0:
        StartLoop(SE_ENV_MUSEUM_AQUARIUM_S, 0);
        return;
    case stage::NAME_MUSEUM_FISH_1:
        StartLoop(SE_ENV_MUSEUM_AQUARIUM_M, 0);
        return;
    case stage::NAME_MUSEUM_FISH_2:
        StartLoop(SE_ENV_MUSEUM_AQUARIUM_L, 0);
        return;
    case stage::NAME_PHOTO_BOX:
        StartLoop(SE_ENV_DOWNTOWN_PHOTOBOX, 0);
        return;
    case stage::NAME_STATION_RETIRE_DEMO:
        StartLoop(SE_ENV_STATION_RETIRE_WAIT, 0);
        return;
    case stage::NAME_VILLAGE:
        m_SeaHandle.Stop(20);
        return;
    default:
        break;
    }
    if (stage::IsKind(stage::KIND_ISLAND, current)) {
        StartLoop(s_TourSoundIds[0][mgr->IsNight()], 0);
        m_TourKind = 0;
        return;
    }
    if (stage::IsKind(stage::KIND_TOUR, current)) {
        if (m_RainState == RAIN_START || m_RainState == RAIN_RAIN || m_RainState == RAIN_HEAVY) {
            return;
        }
        StartLoop(s_TourSoundIds[1][mgr->IsNight()], 0);
        m_TourKind = 1;
        return;
    }
    if (!stage::IsKind(stage::KIND_PLAYER_HOUSE, current) && !stage::IsKind(stage::KIND_NPC_HOUSE, current)) {
        return;
    }
    if (current == stage::NAME_PLAYER_HOUSE_0_BF || current == stage::NAME_PLAYER_HOUSE_1_BF ||
        current == stage::NAME_PLAYER_HOUSE_2_BF || current == stage::NAME_PLAYER_HOUSE_3_BF) {
        return;
    }
    s32 x = m_PositionX;
    s32 y = m_PositionY;
    if (x < 0 || y < 0) {
        return;
    }
    // the house is near the sea
    if (y <= 64) {
        if (y > 61) {
            if (mgr->_CC5 ? x <= 76 : x >= 36) {
                return;
            }
        } else if (mgr->_CC5 ? x <= 79 : x >= 33) {
            return;
        }
    }
    if (m_RainState == RAIN_START || m_RainState == RAIN_RAIN || m_RainState == RAIN_HEAVY) {
        return;
    }
    StartLoop(SE_ENV_SEA_IND, 0);
    if (m_LoopHandles[0].IsAttached() && m_LoopHandles[0].GetId() == SE_ENV_SEA_IND) {
        m_LoopHandles[0].SetVolume(0.8f);
        m_LoopHandles[0].SetSurroundPan(1.5f);
        m_LoopHandles[0].SetBiquadFilter(1, 0.52f);
    }
}

// 0x001D9300
void SoundEnvMgr::SetLoopParam(f32 distance, f32 pan)
{
    if (!m_LoopHandles[0].IsAttached() || !m_LoopHandles[1].IsAttached()) {
        return;
    }
    f32 volume = 1.0f;
    if (distance > 320.0f) {
        volume = 1.0f - (distance - 320.0f) * 0.0013888889f;
        if (volume < 0.0f) {
            volume = 0.0f;
        } else if (volume > 1.0f) {
            volume = 1.0f;
        }
    }
    m_LoopHandles[0].SetVolume(volume);
    m_LoopHandles[1].SetVolume(volume);
    if (pan > 1.0f) {
        pan = 1.0f;
    } else if (pan < -1.0f) {
        pan = -1.0f;
    }
    m_LoopHandles[0].SetPan(pan);
    m_LoopHandles[1].SetPan(pan);
}

// 0x001D9418
void SoundEnvMgr::UpdateTour()
{
    if (m_IsStopped || m_TourKind != 0) {
        return;
    }
    SoundMgr* mgr = SoundMgr::s_pInstance;
    s32 night = mgr->IsNight();
    if (!m_LoopHandles[night].IsAttached()) {
        mgr->StartSound(&m_LoopHandles[night], s_TourSoundIds[m_TourKind][night]);
        m_TourFaders[night].SetTarget(0.0f, 0);
        m_TourFaders[night].SetTarget(1.0f, 900);
        m_TourFaders[night == 0 ? 1 : 0].SetTarget(0.0f, 900);
    }
    m_TourFaders[0].SoundFader::Update();
    m_TourFaders[1].SoundFader::Update();
    for (s32 i = 0; i < 2; i++) {
        if (!m_LoopHandles[i].IsAttached()) {
            continue;
        }
        if (m_TourFaders[i].GetValue() > 0.0f) {
            m_LoopHandles[i].SetVolume(m_TourFaders[i].GetValue());
        } else {
            m_LoopHandles[i].Stop(0);
        }
    }
}

// 0x001D9548
void SoundEnvMgr::calc()
{
    if (SoundMgr::s_pInstance->IsPaused()) {
        return;
    }
    if (m_RainState != m_PrevRainState) {
        if (!m_IsStopped) {
            UpdateRain();
        }
        m_PrevRainState = m_RainState;
    }
    UpdateRainVariables();
    m_OutdoorFader.Update();

    bool isLevelChanged = false;
    if (!m_IsStopped) {
        isLevelChanged = Func22A20C() != m_RainLevel;
        m_RainLevel = Func22A20C();
    }

    if (m_IsRainFading) {
        if (m_RainHandle.IsAttached()) {
            bool isRising = false;
            s32 level = Func22A20C();
            f32 volume;
            if (m_RainRange.to == RAIN_LEVEL) {
                isRising = true;
                volume = SoundMgr::Lerp(level, m_RainRange.from, m_RainRange.pivot, 0.0f, 1.0f);
            } else {
                volume = SoundMgr::Lerp(level, m_RainRange.from, m_RainRange.pivot, 1.0f, 0.0f);
            }
            if (volume < 0.0f) {
                volume = 0.0f;
            } else if (volume > 1.0f) {
                volume = 1.0f;
            }
            if (isLevelChanged) {
                m_RainFader.SetTarget(volume, 30);
            }
            m_RainFader.Update();
            m_RainHandle.SetVolume(m_RainFader.GetValue());
            if (m_RainRange.to == level && m_RainFader.GetRemainingFrames() <= 0) {
                if (!isRising) {
                    m_RainHandle.Stop(5);
                }
                m_IsRainFading = false;
            }
        } else {
            m_IsRainFading = false;
            m_RainFader.SoundFader::Reset(0.0f);
            m_RainRange.from = 0;
            m_RainRange.to = 0;
        }
    }

    if (m_IsHeavyRainFading) {
        if (m_HeavyRainHandle.IsAttached()) {
            bool isRising = false;
            s32 level = Func22A20C();
            f32 volume;
            if (m_HeavyRainRange.from < m_HeavyRainRange.to) {
                isRising = true;
                volume = SoundMgr::Lerp(level, m_HeavyRainRange.from, m_HeavyRainRange.pivot, 0.0f, 1.0f);
            } else {
                volume = SoundMgr::Lerp(level, m_HeavyRainRange.from, m_HeavyRainRange.pivot, 1.0f, 0.0f);
            }
            if (volume < 0.0f) {
                volume = 0.0f;
            } else if (volume > 1.0f) {
                volume = 1.0f;
            }
            if (isLevelChanged) {
                m_HeavyRainFader.SetTarget(volume, 30);
            }
            m_HeavyRainFader.Update();
            m_HeavyRainHandle.SetVolume(m_HeavyRainFader.GetValue());
            if (m_HeavyRainRange.to == level && m_HeavyRainFader.GetRemainingFrames() <= 0) {
                if (!isRising) {
                    m_HeavyRainHandle.Stop(5);
                }
                m_IsHeavyRainFading = false;
            }
        } else {
            m_IsHeavyRainFading = false;
            m_HeavyRainFader.SoundFader::Reset(0.0f);
            m_HeavyRainRange.from = 0;
            m_HeavyRainRange.to = 0;
        }
    }

    if (m_IsRainPending && Func22A20C() > 0) {
        if (!m_RainHandle.IsAttached()) {
            SoundMgr::s_pInstance->StartSound(&m_RainHandle, SE_ENV_RAIN);
            m_RainHandle.SetVolume(0.0f);
        }
        m_IsRainFading = true;
        m_RainRange.from = RAIN_LEVEL;
        m_RainRange.SetTarget(0, 1.0f);
        m_RainLevel = -1;
        m_IsRainPending = false;
    }
    UpdateTour();
}

// 0x001D98E4
void SoundEnvMgr::Stop()
{
    m_IsStopped = true;
    m_IsRainPending = false;
    m_RainHandle.Stop(0);
    m_HeavyRainHandle.Stop(0);
    if (!stage::IsKind(stage::KIND_EXHIBITION, stage::GetCurrent()) ||
        !stage::IsKind(stage::KIND_EXHIBITION, stage::GetNext())) {
        for (s32 i = 0; i < 2; i++) {
            m_LoopHandles[i].Stop(0);
        }
        m_TourKind = 2;
    }
    m_IsRainFading = false;
    m_RainFader.SoundFader::Reset(0.0f);
    m_RainRange.from = 0;
    m_RainRange.to = 0;
    m_IsHeavyRainFading = false;
    m_HeavyRainFader.SoundFader::Reset(0.0f);
    m_HeavyRainRange.from = 0;
    m_PositionX = -1;
    m_HeavyRainRange.to = 0;
    m_PositionY = -1;
}
