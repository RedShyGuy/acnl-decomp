#include "Audio/bgm/dStageOpusPrologue2.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dStageOpusVillageDowntown.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"

// 0x005883B0
void audio::bgm::StageOpusPrologue2::Initialize()
{
    m_IsArrival = Func5C58C0() == 1;
    SvPlayer* player = g_pSvCurrentPlayer;
    m_IsNamedArrival = m_IsArrival & player->playerFlags.playerSetNameAndTownName;
    if (m_IsNamedArrival) {
        GetTrackMixer()->AddMix(21);
    }
    m_StartsLater = !m_IsNamedArrival;
    m_BgmId = player->playerFlags.mainStreetUnlocked ? STRM_BGM_PROLOGUE_08 : STRM_BGM_PROLOGUE_07;
    m_IsReady = 0;
    s_TownState.isOceanLeft = GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 0;
}

// 0x0058846C
void audio::bgm::StageOpusPrologue2::Setup(stage::Name)
{
    if (m_IsArrival) {
        GetRequestMgr()->RequestJingleDelayed(STRM_BGM_PROLOGUE_02, 20);
    }
    s_TownState.isLow = 0;
    StageOpusVillageDowntown::UpdateAmbience();
}

// 0x005884A8
void audio::bgm::StageOpusPrologue2::Ready()
{
    if (m_IsArrival && !m_IsNamedArrival) {
        m_StartsLater = 1;
    }
}

// 0x005884C8
void audio::bgm::StageOpusPrologue2::Leave(stage::Name)
{
    Stop();
    if (s_TownState.isLow) {
        GetTrackMixer()->RemoveMix(45);
    }
    GetTrackMixer()->RemoveMix(21);
    GetRequestMgr()->Remove(KIND_JINGLE_C, STRM_BGM_PROLOGUE_06);
    StageOpusVillageDowntown::UpdateAmbience();
}

// 0x00588534
void audio::bgm::StageOpusPrologue2::EnterKeep()
{
    GetTrackMixer()->m_IsSequenceAlt = 1;
}

// 0x00588548
void audio::bgm::StageOpusPrologue2::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_STAGE, m_BgmId, 20, times);
}

// 0x00588580
void audio::bgm::StageOpusPrologue2::Update()
{
    StageOpusVillageDowntown::UpdateSideMixes();
    if (m_IsSuspended || m_IsCurrent) {
        return;
    }
    if (!m_IsReady) {
        if (!IsPlayerReady()) {
            return;
        }
        m_IsReady = 1;
        if (m_StartsLater) {
            Start();
            m_StartsLater = 0;
        }
    }
    StageOpusVillageDowntown::UpdateLowMix();
    if (!m_StartsLater || m_IsPlaying) {
        return;
    }
    if (!GetRequestMgr()->Has(KIND_JINGLE_C, STRM_BGM_PROLOGUE_06)) {
        return;
    }
    if (SoundMgr::SIsJingleAttached()) {
        return;
    }
    GetTrackMixer()->RemoveMix(21);
    GetRequestMgr()->Remove(KIND_JINGLE_C, STRM_BGM_PROLOGUE_06);
    Start();
}

// 0x00588678
void audio::bgm::StageOpusPrologue2::OnStop()
{
    if (m_IsSuspended) {
        GetRequestMgr()->SetFade(KIND_STAGE, m_BgmId, m_Fade, 0, 0);
        return;
    }
    GetRequestMgr()->Remove(KIND_STAGE, m_BgmId);
}

// 0x005886D8
// 0x005886D4 (deleting dtor)
audio::bgm::StageOpusPrologue2::~StageOpusPrologue2()
{
    // empty (in the original too)
}

// 0x0058979C
void audio::bgm::StageOpusPrologue2::SetupKeep(stage::Name)
{
    StageOpusVillageDowntown::UpdateAmbience();
}
