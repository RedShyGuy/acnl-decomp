#include "Audio/bgm/dStageOpusVillageDowntown.h"
#include "Audio/bgm/dBgmClock.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Other/dFdInfo.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"

// 0x00589310
void audio::bgm::StageOpusVillageDowntown::Setup(stage::Name stage)
{
    s_TownState.isLow = 0;
    m_IsReady = 0;
    m_IsSuspended = 0;
    s_TownState.isOceanLeft = GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 0;
    m_pEvent = s_EventOpusList.Select();
    if (m_pEvent != NULL && stage == stage::NAME_DOWNTOWN) {
        GetTrackMixer()->AddMix(49);
    }
    stage::Name previous = stage::GetPrevious();
    if (stage::IsKind(8, previous) && !stage::IsKind(16, previous) && stage == stage::NAME_DOWNTOWN && m_pEvent == NULL) {
        SoundMgr::SetNextBgmParam(1);
    }
    // the music of Main Street changes with the upgrades of the Nookling store (and with the buildings of the street)
    switch (GetGardenPlus()->data.townData.unk_0xEE2C.shopZakka.nooklingState) {
    case 2:
        SoundMgr::SSetDowntownLevel(1);
        break;
    case 3: {
        // the buildings of Main Street
        s32 count = 0;
        if (GetGardenPlus()->data.townData.unk_0xEE2C.fortuneTellerUnlockStatus == 1) {
            count = 1;
        }
        if (GetGardenPlus()->data.townData.unk_0xEE2C.dreamSuiteUnlockStatus == 1) {
            count++;
        }
        if (GetGardenPlus()->data.townData.unk_0xEE2C.clubLolUnlockState == 2) {
            count++;
        }
        if (GetGardenPlus()->data.townData.unk_0xEE2C.shampoodleUnlockStatus == 2) {
            count++;
        }
        if (GetGardenPlus()->data.townData.unk_0xEE2C.kickUnlockStatus == 2) {
            count++;
        }
        SoundMgr::SSetDowntownLevel(count >= 3 ? 2 : 1);
        break;
    }
    case 4:
        SoundMgr::SSetDowntownLevel(2);
        break;
    default:
        SoundMgr::SSetDowntownLevel(0);
        break;
    }
    m_BgmId = 0;
    m_NextBgmId = 0;
    m_State = STATE_STOPPED;
    m_IsSwitching = 0;
    m_IsClockChanged = 0;
    m_WaitsForChime = 1;
    s_ClockState.isShop = 0;
    UpdateAmbience();
}

// 0x005894EC
void audio::bgm::StageOpusVillageDowntown::Ready()
{
    m_Value = 7;
    if (m_pEvent != NULL) {
        m_pEvent->Begin();
    }
}

// 0x00589514
void audio::bgm::StageOpusVillageDowntown::Leave(stage::Name)
{
    Stop();
    if (m_pEvent != NULL) {
        m_pEvent->End(false);
        m_pEvent = NULL;
    }
    if (s_TownState.isLow) {
        GetTrackMixer()->RemoveMix(45);
    }
    GetTrackMixer()->RemoveMix(49);
    UpdateAmbience();
}

// 0x00589594
void audio::bgm::StageOpusVillageDowntown::UpdateLowMix()
{
    s8 isLow = s_TownState.isLow;
    if (stage::GetCurrent() != stage::NAME_VILLAGE) {
        isLow = 0;
    } else {
        const nn::math::VEC3* pos = Func5C37B0(4, true);
        if (pos == NULL) {
            isLow = 0;
        } else if (pos->y < 64.0f) {
            isLow = 1;
        } else {
            // three points: the player and one unit towards the ocean and to the south
            s32 high = pos->y > 96.0f ? -1 : 0;
            s32 kindX;
            s32 kindZ;
            Func307340(&kindX, &kindZ, pos);
            const f32 unit = 32.0f;
            FieldName field = {0};
            bool done = false;
            if (s_TownState.isOceanLeft ? kindX == 1 : kindX == 5) {
                nn::math::VEC3 point = *pos;
                if (s_TownState.isOceanLeft) {
                    point.x -= unit;
                } else {
                    point.x += unit;
                }
                f32 height = FdInfo::GetHeight(*reinterpret_cast<const nn::math::Vector<float, 3u>*>(&point), field);
                if (height < 64.0f) {
                    isLow = 1;
                    done = true;
                } else {
                    high += height > 96.0f ? -1 : 0;
                }
            } else {
                high--;
            }
            if (!done) {
                if (kindZ == 4) {
                    nn::math::VEC3 point = *pos;
                    point.z += unit;
                    f32 height = FdInfo::GetHeight(*reinterpret_cast<const nn::math::Vector<float, 3u>*>(&point), field);
                    if (height < 64.0f) {
                        isLow = 1;
                        done = true;
                    } else {
                        high += height > 96.0f ? -1 : 0;
                    }
                } else {
                    high--;
                }
                if (!done && high == -3) {
                    isLow = 0;
                }
            }
        }
    }
    if (s_TownState.isLow == isLow) {
        return;
    }
    s_TownState.isLow = isLow;
    if (isLow) {
        GetTrackMixer()->AddMix(45);
    } else {
        GetTrackMixer()->RemoveMix(45);
    }
}

// 0x005897A0
void audio::bgm::StageOpusVillageDowntown::UpdateAmbience()
{
    if (s_TownState.envSoundId != 0) {
        SoundMgr::SStopSound(s_TownState.envSoundId, 0);
        s_TownState.envSoundId = 0;
        GetTrackMixer()->RemoveMix(46);
        GetTrackMixer()->RemoveMix(47);
        return;
    }
    if (stage::GetCurrent() == stage::NAME_DOWNTOWN) {
        s_TownState.envSoundId =
            GetGardenPlus()->data.townData.unk_0xEE2C.kickUnlockStatus == 2 ? SE_ENV_DOWNTOWN_2 : SE_ENV_DOWNTOWN_1;
        SoundMgr::PlaySound(static_cast<SeID>(s_TownState.envSoundId), 0.0f);
    }
}

// 0x0058983C
void audio::bgm::StageOpusVillageDowntown::UpdateSideMixes()
{
    if (stage::GetCurrent() != stage::NAME_DOWNTOWN) {
        return;
    }
    const nn::math::VEC3* pos = Func5C37B0(4, false);
    if (pos == NULL) {
        return;
    }
    if (GetTrackMixer()->IsMixActive(46)) {
        if (s_TownState.isOceanLeft) {
            if (pos->x < 1696.0f) {
                GetTrackMixer()->RemoveMix(46);
            }
        } else if (pos->x > 448.0f) {
            GetTrackMixer()->RemoveMix(46);
        }
    } else if (GetTrackMixer()->IsMixActive(47)) {
        if (s_TownState.isOceanLeft) {
            if (pos->x > 416.0f) {
                GetTrackMixer()->RemoveMix(47);
            }
        } else if (pos->x < 1624.0f) {
            GetTrackMixer()->RemoveMix(47);
        }
    } else if (s_TownState.isOceanLeft) {
        if (pos->x > 1728.0f) {
            GetTrackMixer()->AddMix(46);
        } else if (pos->x < 352.0f) {
            GetTrackMixer()->AddMix(47);
        }
    } else if (pos->x < 416.0f) {
        GetTrackMixer()->AddMix(46);
    } else if (pos->x > 1688.0f) {
        GetTrackMixer()->AddMix(47);
    }
}

// 0x0058999C
void audio::bgm::StageOpusVillageDowntown::SetupKeep(stage::Name stage)
{
    if (stage == stage::NAME_DOWNTOWN) {
        m_IsReady = 1;
    }
    if (m_pEvent != NULL) {
        if (stage == stage::NAME_DOWNTOWN) {
            GetTrackMixer()->AddMix(49);
        } else {
            GetTrackMixer()->RemoveMix(49);
        }
    }
    UpdateAmbience();
}

// 0x005899F4
void audio::bgm::StageOpusVillageDowntown::EnterKeep()
{
    GetTrackMixer()->m_IsSequenceAlt = 1;
    if (m_pEvent != NULL) {
        m_pEvent->Resume();
    }
}

// 0x00589A28
void audio::bgm::StageOpusVillageDowntown::OnStart()
{
    if (m_pEvent != NULL && m_pEvent->HasBgm()) {
        m_Kind = KIND_STAGE_EVENT;
    } else {
        m_Kind = stage::IsKind(0x200, stage::GetCurrent()) ? KIND_STAGE_SPECIAL : KIND_STAGE;
    }
    if (m_pEvent != NULL && m_pEvent->StartRequest(m_Kind, m_BgmId)) {
        return;
    }
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(m_Kind, m_BgmId, 20, times);
}

// 0x00589AD8
void audio::bgm::StageOpusVillageDowntown::Update()
{
    UpdateSideMixes();
    UpdateLowMix();
    if (m_IsEventChanged && IsSubFree()) {
        if (g_Unknown9516C0.mode == 255 && g_Unknown9516C0.p4 != NULL) {
            Func6DDC18(g_Unknown9516C0.p4);
        }
        m_IsEventChanged = 0;
    }
    if (m_IsSuspended || m_IsCurrent) {
        return;
    }
    bool force = false;
    if (!m_IsReady) {
        if (!(IsPlayerReady() | s_TownState.isForced)) {
            return;
        }
        m_IsReady = 1;
        u32 id = GetBgmId(false);
        m_BgmId = id;
        m_NextBgmId = id;
        s_TownState.isForced = 0;
        force = true;
    }
    EvOpusBase* event = s_EventOpusList.Select();
    force |= m_pEvent != event;
    if (m_pEvent != NULL) {
        if (m_pEvent == event) {
            m_pEvent->Update();
        } else {
            m_pEvent->End(true);
            GetTrackMixer()->RemoveMix(49);
            if (m_State == STATE_PLAYING) {
                Stop();
            }
            m_pEvent = event;
            if (event == NULL) {
                UpdateHourly(true, true, force);
                return;
            }
            event->Begin();
            if (stage::GetCurrent() == stage::NAME_DOWNTOWN) {
                GetTrackMixer()->AddMix(49);
            }
        }
    } else {
        if (event == NULL) {
            UpdateHourly(true, true, force);
            return;
        }
        m_pEvent = event;
        event->Begin();
        if (stage::GetCurrent() == stage::NAME_DOWNTOWN) {
            GetTrackMixer()->AddMix(49);
        }
    }
    force |= m_pEvent->IsChanged();
    if (m_pEvent != NULL && !m_pEvent->HasBgm()) {
        return;
    }
    UpdateHourly(true, true, force);
}

// 0x00589CD8
void audio::bgm::StageOpusVillageDowntown::OnStop()
{
    if (m_IsSwitching || m_IsCurrent) {
        GetRequestMgr()->SetFade(m_Kind, m_BgmId, 0, 0, 0);
        m_IsSwitching = 0;
        return;
    }
    if (m_IsSuspended) {
        GetRequestMgr()->SetFade(m_Kind, m_BgmId, m_Fade, 0, 0);
        return;
    }
    if (m_pEvent != NULL && m_pEvent->StopRequest(m_Kind, m_BgmId)) {
        return;
    }
    GetRequestMgr()->SetFade(m_Kind, m_BgmId, GetFadeToHour(), 0, 0);
}

// 0x00589DA8
u32 audio::bgm::StageOpusVillageDowntown::GetBgmId(bool isNextHour)
{
    if (m_pEvent != NULL && m_pEvent->HasBgm()) {
        return m_pEvent->GetBgmId();
    }
    if (stage::IsKind(0x200, stage::GetCurrent())) {
        // Main Street: quiet at night
        s32 quiet;
        u8 weather;
        if (isNextHour) {
            quiet = GetHour() < 7 || GetHour() >= 20 ? 1 : 0;
        } else {
            quiet = GetHour() < 8 || GetHour() >= 21 ? 1 : 0;
        }
        if (isNextHour) {
            weather = Func62FEC8();
        } else {
            weather = Func62FB14();
        }
        switch (weather) {
        case 0:
        case 1:
        case 2:
            return s_DowntownBgm[0][quiet];
        case 3:
        case 4:
            return s_DowntownBgm[1][quiet];
        case 5:
        case 6:
            return s_DowntownBgm[2][quiet];
        }
        return 0;
    }
    s32 hour = GetHour() + isNextHour;
    switch (Func62FB14()) {
    case 0:
    case 1:
    case 2:
        return s_OutdoorBgm[0][hour];
    case 3:
    case 4:
        return s_OutdoorBgm[1][hour];
    case 5:
    case 6:
        return s_OutdoorBgm[2][hour];
    }
    return 0;
}

// 0x00589F3C
// 0x00589F38 (deleting dtor)
audio::bgm::StageOpusVillageDowntown::~StageOpusVillageDowntown()
{
    // empty (in the original too)
}
