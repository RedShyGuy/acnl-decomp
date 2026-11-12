#include "Audio/bgm/dStageOpusPrologue4.h"
#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dStageOpusVillageDowntown.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"

// 0x005887E0
void audio::bgm::StageOpusPrologue4::Initialize()
{
    m_BgmId = STRM_BGM_PROLOGUE_09;
    if (g_pSvCurrentPlayer != NULL && g_pSvCurrentPlayer->playerFlags.houseLocationChosen) {
        m_BgmId = STRM_BGM_PROLOGUE_11;
        m_IsHouseChosen = 1;
    }
    m_IsReady = 0;
    s_TownState.isOceanLeft = GetGardenPlus()->data.townData.unk_0x5C.oceanSide == 0;
}

// 0x00588860
void audio::bgm::StageOpusPrologue4::Setup(stage::Name)
{
    s_TownState.isLow = 0;
    m_IsTown = 1;
    StageOpusVillageDowntown::UpdateAmbience();
}

// 0x0058887C
void audio::bgm::StageOpusPrologue4::Ready()
{
    if (m_BgmId == STRM_BGM_PROLOGUE_09) {
        Start();
    }
}

// 0x0058889C
void audio::bgm::StageOpusPrologue4::Leave(stage::Name)
{
    Stop();
    if (s_TownState.isLow) {
        GetTrackMixer()->RemoveMix(45);
    }
    m_IsTown = 0;
    StageOpusVillageDowntown::UpdateAmbience();
}

// 0x005888E4
void audio::bgm::StageOpusPrologue4::SetupKeep(stage::Name)
{
    StageOpusVillageDowntown::UpdateAmbience();
}

// 0x005888E8
void audio::bgm::StageOpusPrologue4::ReadyKeep()
{
    if (m_KeepsMix21) {
        m_KeepsMix21 = 0;
        return;
    }
    if (g_pSvCurrentPlayer == NULL) {
        return;
    }
    if (!m_IsHouseChosen) {
        GetTrackMixer()->RemoveMix(21);
    }
}

// 0x00588934
void audio::bgm::StageOpusPrologue4::EnterKeep()
{
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player == NULL || player->playerFlags.houseLocationChosen) {
        GetTrackMixer()->m_IsSequenceAlt = 1;
    } else {
        GetTrackMixer()->m_IsSequenceDisabled = 1;
    }
    if (m_IsHouseChosen) {
        return;
    }
    if (GetTrackMixer()->IsMixActive(21)) {
        return;
    }
    if (player == NULL || player->playerFlags.houseLocationChosen) {
        return;
    }
    GetTrackMixer()->AddMix(21);
    m_KeepsMix21 = 1;
}

// 0x005889E4
void audio::bgm::StageOpusPrologue4::OnStart()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_STAGE, m_BgmId, 20, times);
}

// 0x00588A1C
void audio::bgm::StageOpusPrologue4::Update()
{
    if (m_HasFanfare) {
        if (!m_IsFanfareHeard) {
            m_IsFanfareHeard = SoundMgr::SIsJingleAttached();
        } else if (!SoundMgr::SIsJingleAttached()) {
            // the fanfare is over
            if (m_IsPlaying) {
                GetRequestMgr()->Remove(KIND_JINGLE_B, STRM_BGM_PROLOGUE_10);
                Stop();
            } else {
                m_BgmId = STRM_BGM_PROLOGUE_11;
                Start();
                m_HasFanfare = 0;
            }
        }
    }
    StageOpusVillageDowntown::UpdateSideMixes();
    if (m_IsSuspended || m_IsCurrent) {
        return;
    }
    if (!m_IsReady) {
        if (!IsPlayerReady()) {
            return;
        }
        m_IsReady = 1;
        Start();
    }
    StageOpusVillageDowntown::UpdateLowMix();
}

// 0x00588B14
void audio::bgm::StageOpusPrologue4::OnStop()
{
    if (m_IsSuspended) {
        GetRequestMgr()->SetFade(KIND_STAGE, m_BgmId, m_Fade, 0, 0);
        return;
    }
    GetRequestMgr()->Remove(KIND_STAGE, m_BgmId);
}

// 0x00588B74
// 0x00588B70 (deleting dtor)
audio::bgm::StageOpusPrologue4::~StageOpusPrologue4()
{
    // empty (in the original too)
}
