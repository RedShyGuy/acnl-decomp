#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmClock.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Audio/bgm/dStageOpusAblesisters.h"
#include "Audio/bgm/dStageOpusAward.h"
#include "Audio/bgm/dStageOpusBarcarolle.h"
#include "Audio/bgm/dStageOpusBeauty.h"
#include "Audio/bgm/dStageOpusBirthday.h"
#include "Audio/bgm/dStageOpusCafe.h"
#include "Audio/bgm/dStageOpusCafeStaff.h"
#include "Audio/bgm/dStageOpusCamping.h"
#include "Audio/bgm/dStageOpusCeremony.h"
#include "Audio/bgm/dStageOpusChipTutorial.h"
#include "Audio/bgm/dStageOpusClub444.h"
#include "Audio/bgm/dStageOpusClub444Live.h"
#include "Audio/bgm/dStageOpusComeHomeDemo.h"
#include "Audio/bgm/dStageOpusConvenience.h"
#include "Audio/bgm/dStageOpusDepart.h"
#include "Audio/bgm/dStageOpusDream.h"
#include "Audio/bgm/dStageOpusEntryDemo.h"
#include "Audio/bgm/dStageOpusEvidence.h"
#include "Audio/bgm/dStageOpusExchange.h"
#include "Audio/bgm/dStageOpusExhibition.h"
#include "Audio/bgm/dStageOpusFortune.h"
#include "Audio/bgm/dStageOpusGallery.h"
#include "Audio/bgm/dStageOpusGrace.h"
#include "Audio/bgm/dStageOpusGrowUp.h"
#include "Audio/bgm/dStageOpusHomeCenter.h"
#include "Audio/bgm/dStageOpusImpro.h"
#include "Audio/bgm/dStageOpusLoad.h"
#include "Audio/bgm/dStageOpusLobbyIsland.h"
#include "Audio/bgm/dStageOpusMuseum.h"
#include "Audio/bgm/dStageOpusNull.h"
#include "Audio/bgm/dStageOpusOffice.h"
#include "Audio/bgm/dStageOpusPavilion.h"
#include "Audio/bgm/dStageOpusPlantDemo.h"
#include "Audio/bgm/dStageOpusPoliceBox.h"
#include "Audio/bgm/dStageOpusPostOffice.h"
#include "Audio/bgm/dStageOpusPrologue1.h"
#include "Audio/bgm/dStageOpusPrologue2.h"
#include "Audio/bgm/dStageOpusPrologue3.h"
#include "Audio/bgm/dStageOpusPrologue4.h"
#include "Audio/bgm/dStageOpusRealEstate.h"
#include "Audio/bgm/dStageOpusRecycle.h"
#include "Audio/bgm/dStageOpusResult.h"
#include "Audio/bgm/dStageOpusRetireDemo.h"
#include "Audio/bgm/dStageOpusSave.h"
#include "Audio/bgm/dStageOpusSelect.h"
#include "Audio/bgm/dStageOpusShoes.h"
#include "Audio/bgm/dStageOpusShop.h"
#include "Audio/bgm/dStageOpusShowroom.h"
#include "Audio/bgm/dStageOpusStation.h"
#include "Audio/bgm/dStageOpusSuper.h"
#include "Audio/bgm/dStageOpusTakumiImport.h"
#include "Audio/bgm/dStageOpusTitle.h"
#include "Audio/bgm/dStageOpusTour.h"
#include "Audio/bgm/dStageOpusTourDesk.h"
#include "Audio/bgm/dStageOpusTrain.h"
#include "Audio/bgm/dStageOpusUpdatePrologue.h"
#include "Audio/bgm/dStageOpusVillageDowntown.h"
#include "Sound/dSoundLabel.h"
#include "event/event_Api.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"
#include "netgame/netgame_Api.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"
#include "time/time_Api.h"

template <typename T>
audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create()
{
    return new (m_Allocator.m_pHeap, 4) T();
}

// 0x00584C38
void audio::bgm::StageOpusMgr::Initialize()
{
    s_ClockState.hour = GetHour();
    s_ClockState.second = GetSecond();
    s_ClockState.isNearHour =
        IsInTime(59, 45, 0, 0) &&
        !(Func3189EC() == event::EVENT_ID_COUNTDOWN && Func309C6C(event::EVENT_ID_COUNTDOWN, time::GetCurrentDate()));
    s_ClockState.isChimeEnabled = 0;
    s_ClockState.isShop = 0;
    m_Allocator.CreateExpHeap(1024, sead::HeapMgr::s_pInstance->getCurrentHeap(), sead::SafeStringBase<char>("StageOpusMgr"),
                              sead::Heap::HEAP_DIRECTION_FORWARD, false);
}

// 0x00584D20
void audio::bgm::StageOpusMgr::StartCarnival()
{
    s_EventOpusList.m_Carnival.m_State = 1;
    SoundMgr::SPlayCarnival(0);
}

// 0x00584D38
void audio::bgm::StageOpusMgr::ResetHide()
{
    s_EventOpusList.m_Hide.m_IsWaiting = 0;
    s_EventOpusList.m_Hide.m_StopsAtOnce = 0;
}

// 0x00584D50
void audio::bgm::StageOpusMgr::StartFortuneDemo()
{
    if (m_OpusIndex == 24) {
        GetRequestMgr()->SetDelay(KIND_JINGLE_B, STRM_BGM_IND_FORTUNEDEMO, 0);
        SoundMgr::SSetFx7(true);
    }
}

// 0x00584D88
void audio::bgm::StageOpusMgr::ChangeStage(stage::Name stage)
{
    if (m_Flags & FLAG_KEEP) {
        if (m_pOpus != NULL) {
            m_pOpus->SetupKeep(stage);
        }
    } else {
        if (m_pOpus != NULL) {
            delete m_pOpus;
            m_pOpus = NULL;
            m_OpusIndex = OPUS_NONE;
        }
        m_OpusIndex = SelectOpus(stage);
        m_pOpus = (this->*s_CreateFuncs[m_OpusIndex])();
        m_pOpus->Initialize();
        m_pOpus->m_IsCurrent = 1;
        m_pOpus->Setup(stage);
    }
    if (!s_ClockState.isChimeEnabled && SoundMgr::SIsChimeStage(stage)) {
        s_ClockState.SetChime(true, false);
    }
    m_Flags &= ~(FLAG_CEREMONY | FLAG_PLANT_DEMO);
}

// 0x00584EA0
void audio::bgm::StageOpusMgr::StopFortuneDemo(s32 fade)
{
    if (m_OpusIndex == 24) {
        GetRequestMgr()->SetFade(KIND_JINGLE_B, STRM_BGM_IND_FORTUNEDEMO, fade, -1, 0);
        SoundMgr::SSetFx7(false);
    }
}

// 0x00584EF4
void audio::bgm::StageOpusMgr::Ready()
{
    if (m_pOpus != NULL) {
        if (m_Flags & FLAG_KEEP) {
            m_pOpus->ReadyKeep();
            m_pOpus->m_IsKept = 0;
            m_pOpus->m_IsCurrent = 0;
        } else {
            m_pOpus->Ready();
            m_pOpus->m_IsCurrent = 0;
        }
    }
    m_Flags |= FLAG_READY;
    if (m_Flags & FLAG_HIDE_END) {
        s_EventOpusList.m_Hide.m_IsResumed = 0;
        m_Flags &= ~FLAG_HIDE_END;
    }
    s_TownState.isShopMixOff = 0;
}

// 0x00584F80
void audio::bgm::StageOpusMgr::StopOpus()
{
    StageOpusBase* opus = m_pOpus;
    if (opus == NULL) {
        return;
    }
    if (opus->m_IsPlaying || opus->m_State == StageOpusBase::STATE_PLAYING) {
        opus->Stop();
    } else if (opus->m_State == StageOpusBase::STATE_FADED) {
        GetRequestMgr()->Remove(opus->m_Kind, opus->m_BgmId);
    }
    opus->m_State = StageOpusBase::STATE_STOPPED;
    opus->m_IsReady = 0;
}

// 0x00584FF0
void audio::bgm::StageOpusMgr::RequestFortuneDemo(u32 fade)
{
    if (m_OpusIndex == 24) {
        RequestMgr::Times times = {-1, -1, 0};
        GetRequestMgr()->Request(KIND_JINGLE_B, STRM_BGM_IND_FORTUNEDEMO, fade, times);
    }
}

// 0x0058504C
void audio::bgm::StageOpusMgr::EndCarnivalLong()
{
    EvOpusCarnival& carnival = s_EventOpusList.m_Carnival;
    carnival.m_State = 3;
    SoundMgr::SPlayCarnival(2);
    GetRequestMgr()->Remove(KIND_CARNIVAL, carnival.m_BgmId);
    carnival.m_BgmId = SEQ_BGM_EVT_CARNIVAL_END_L;
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_CARNIVAL, carnival.m_BgmId, 5, times);
}

// 0x005850B8
void audio::bgm::StageOpusMgr::StartHarvestCook(u32 fade, u32 delay)
{
    EvOpusHarvest& harvest = s_EventOpusList.m_Harvest;
    harvest.m_State = EvOpusHarvest::STATE_COOKING;
    harvest.m_CookDelay = delay;
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_B, harvest.m_CookBgmId, fade, times);
}

// 0x00585104
void audio::bgm::StageOpusMgr::Begin(stage::Name stage, bool isOnline)
{
    if (m_Flags & FLAG_HIDE) {
        s_EventOpusList.m_Hide.m_IsStarted = 1;
        if (!s_ClockState.isNearHour) {
            u32 id = s_EventOpusList.m_Hide.GetBgmId();
            RequestMgr::Times times = {-1, -1, 0};
            GetRequestMgr()->Request(KIND_STAGE_EVENT, id, 20, times);
        }
        GetTrackMixer()->m_IsSequenceAlt = 1;
        m_Flags &= ~FLAG_HIDE;
    }
    if (m_Flags & FLAG_HIDE_END) {
        s_EventOpusList.m_Hide.m_IsResumed = 1;
        s_EventOpusList.m_Hide.m_StopsAtOnce = 1;
        u32 id = s_EventOpusList.m_Hide.GetBgmId();
        GetRequestMgr()->SetFade(KIND_STAGE_EVENT, id, 20, -1, 0);
        GetTrackMixer()->m_IsSequenceAlt = 1;
        s_EventOpusList.m_Hide.m_IsWaiting = 0;
    }
    m_Flags &= ~FLAG_READY;
    if (m_pOpus == NULL) {
        return;
    }
    if (m_pOpus->CanKeep(stage, isOnline)) {
        m_Flags |= FLAG_KEEP;
    } else {
        m_Flags &= ~FLAG_KEEP;
    }
    if (m_Flags & FLAG_KEEP) {
        m_pOpus->m_IsKept = 1;
        m_pOpus->m_IsCurrent = 1;
        m_pOpus->EnterKeep();
    } else {
        m_pOpus->m_IsCurrent = 1;
        m_pOpus->Enter();
    }
}

// 0x0058528C
void audio::bgm::StageOpusMgr::StopHarvestCook(s32 stopWait)
{
    EvOpusHarvest& harvest = s_EventOpusList.m_Harvest;
    harvest.m_State = EvOpusHarvest::STATE_IDLE;
    GetRequestMgr()->SetFade(KIND_JINGLE_B, harvest.m_CookBgmId, 0, stopWait, 0);
}

// 0x005852D0
void audio::bgm::StageOpusMgr::Leave(stage::Name next)
{
    if (!(m_Flags & FLAG_KEEP)) {
        if (m_pOpus != NULL) {
            m_pOpus->Leave(next);
            m_pOpus->m_IsCurrent = 0;
            delete m_pOpus;
            m_pOpus = NULL;
            m_OpusIndex = OPUS_NONE;
        }
        GetTrackMixer()->MaskMixes();
    }
    if (s_ClockState.isChimeEnabled == 1 && !SoundMgr::SIsChimeStage(next)) {
        s_ClockState.SetChime(false, false);
    }
    StopSub();
}

// 0x00585388
void audio::bgm::StageOpusMgr::SetFortuneDemoWait(s32 wait)
{
    if (m_OpusIndex == 24) {
        GetRequestMgr()->SetWait(KIND_JINGLE_B, STRM_BGM_IND_FORTUNEDEMO, wait);
    }
}

// 0x005853BC
u8 audio::bgm::StageOpusMgr::SelectOpus(stage::Name stage)
{
    if (IsGrowUp()) {
        return 51;
    }
    if (IsEvidence()) {
        return 30;
    }
    if (stage::IsKind(0xA0016, stage)) {
        return 40;
    }
    if (m_Flags & FLAG_CEREMONY) {
        return 47;
    }
    if (stage::IsKind(0x80000, stage) || stage::IsKind(0x100000, stage)) {
        return 3;
    }
    if (stage == stage::NAME_NONE) {
        return 56;
    }
    if (stage == stage::NAME_TRAIN || stage == stage::NAME_GARDEN_MANUAL_IMPORT || stage == stage::NAME_GARDEN_PLUS_UPDATE) {
        return 33;
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL && stage == stage::NAME_VILLAGE) {
        if (player->playerFlags.unk_0x2B_7) {
            return 34;
        }
    } else {
        if (stage == stage::NAME_STATION_ENTRY_DEMO) {
            return 35;
        }
        if (stage == stage::NAME_STATION_RETIRE_DEMO) {
            return 36;
        }
        if (stage == stage::NAME_STATION_COME_HOME_DEMO) {
            return 37;
        }
        if (stage == stage::NAME_TITLE) {
            return 0;
        }
    }
    if (m_Flags & FLAG_PLANT_DEMO) {
        return 45;
    }
    if (stage == stage::NAME_VILLAGE_PROLOGUE || stage == stage::NAME_STATION_PROLOGUE_DEMO) {
        return 41;
    }
    SvPlayer* current = g_pSvCurrentPlayer;
    if (current != NULL && !current->playerFlags.chooseHouseLocation &&
        (stage == stage::NAME_VILLAGE || stage == stage::NAME_DOWNTOWN)) {
        return 42;
    }
    if (current != NULL && !current->playerFlags.mainStreetUnlocked && current->playerFlags.playerSetNameAndTownName &&
        stage == stage::NAME_OFFICE) {
        return 43;
    }
    if (current != NULL && !current->playerFlags.finishedFirstDay &&
        (stage == stage::NAME_VILLAGE || stage == stage::NAME_DOWNTOWN)) {
        return 44;
    }
    s32 value = Func31336C(stage::GetCurrent());
    if ((Func300928(&GetGardenPlus()->data.npcData.npcs, value) && Func11A3A8()) || g_Unknown951157) {
        return 46;
    }
    if (stage::IsKind(32, stage)) {
        return 1;
    }
    if (stage == stage::NAME_INSECT_AWARD_TENT_DEMO || stage == stage::NAME_FISH_AWARD_TENT_DEMO) {
        return 39;
    }
    if (stage::IsKind(0x200016, stage) && netgame::GetNetgameType() == 2) {
        return 38;
    }
    if (stage::IsKind(0x40000, stage)) {
        return 2;
    }
    if (stage::IsKind(0x400, stage)) {
        return 27;
    }
    if (stage::IsKind(0x2000, stage)) {
        return 6;
    }
    if (stage::IsKind(0x8000, stage)) {
        return 5;
    }
    if (stage == stage::NAME_POST_OFFICE) {
        return 32;
    }
    if (stage == stage::NAME_OFFICE) {
        return 4;
    }
    if (stage == stage::NAME_CLUB444) {
        return 29;
    }
    if (stage == stage::NAME_CLUB444_LIVE) {
        return 31;
    }
    if (stage == stage::NAME_CAFE) {
        return 7;
    }
    if (stage == stage::NAME_CAFE_ARBEIT) {
        return 8;
    }
    if (stage == stage::NAME_SHOP) {
        return 9;
    }
    if (stage == stage::NAME_CONVENIENCE_STORE) {
        return 10;
    }
    if (stage == stage::NAME_SUPERMARKET) {
        return 11;
    }
    if (stage == stage::NAME_HOME_CENTER || stage == stage::NAME_FLORISTS_HOME_IMPROVEMENT_CENTER) {
        return 12;
    }
    if (stage == stage::NAME_DEPARTMENT_STORE_1F || stage == stage::NAME_DEPARTMENT_STORE_2F) {
        return 14;
    }
    if (stage == stage::NAME_FLORISTS) {
        return 13;
    }
    if (stage == stage::NAME_TOUR_DESK) {
        return 15;
    }
    if (stage == stage::NAME_SHOE_SHOP) {
        return 16;
    }
    if (stage == stage::NAME_BEAUTY_SALON) {
        return 17;
    }
    if (stage == stage::NAME_STATION) {
        return 18;
    }
    if (stage == stage::NAME_RECYCLE_SHOP) {
        return 19;
    }
    if (stage == stage::NAME_REAL_ESTATE_AGENCY) {
        return 20;
    }
    if (stage == stage::NAME_DEPARTMENT_STORE_3F) {
        return 21;
    }
    if (stage == stage::NAME_GALLERY) {
        return 22;
    }
    if (stage::IsKind(0x10000, stage)) {
        return 23;
    }
    if (stage == stage::NAME_FORTUNE || stage == stage::NAME_FORTUNE_TENT) {
        return 24;
    }
    if (stage == stage::NAME_CAMP_NPC) {
        return 25;
    }
    if (stage == stage::NAME_TAILOR || stage == stage::NAME_ACCESSORY_SHOP) {
        return 26;
    }
    if (stage == stage::NAME_DREAM) {
        return 28;
    }
    if (stage::IsKind(0x200, stage)) {
        return 1;
    }
    if (stage == stage::NAME_PLAYER_SELECT) {
        return 50;
    }
    if (stage == stage::NAME_TAKUMI_IMPORT) {
        return 52;
    }
    if (stage == stage::NAME_CHIP_TUTORIAL) {
        return 53;
    }
    if (stage == stage::NAME_AUTO_CAMP_L || stage == stage::NAME_AUTO_CAMP_R) {
        return 54;
    }
    if (stage == stage::NAME_UPDATE_SELECT) {
        return 55;
    }
    return 56;
}

// 0x0058585C
void audio::bgm::StageOpusMgr::Nothing()
{
    // empty (in the original too)
}

// 0x00585860
void audio::bgm::StageOpusMgr::RequestPrologue10()
{
    if (m_OpusIndex != 44) {
        return;
    }
    StageOpusPrologue4* opus = static_cast<StageOpusPrologue4*>(m_pOpus);
    if (opus->m_IsTown) {
        RequestMgr::Times times = {12, -1, 0};
        GetRequestMgr()->Request(KIND_JINGLE_B, STRM_BGM_PROLOGUE_10, 5, times);
        opus->m_IsFanfareHeard = 0;
        opus->m_HasFanfare = 1;
        opus->m_IsHouseChosen = 1;
    }
}

// 0x005858C8
void audio::bgm::StageOpusMgr::SetPlantDemoStep2()
{
    if (m_OpusIndex == 45) {
        SoundMgr::SSetPrologueStep(2);
    }
}

// 0x005858E0
void audio::bgm::StageOpusMgr::DisableChime()
{
    s_ClockState.SetChime(false, true);
}

// 0x005858F4
void audio::bgm::StageOpusMgr::StartPrologue3()
{
    if (m_OpusIndex == 43) {
        m_pOpus->Start();
    }
}

// 0x00585914
void audio::bgm::StageOpusMgr::ClearHarvestCookAccepted()
{
    s_EventOpusList.m_Harvest.m_IsCookAccepted = 0;
}

// 0x00585928
void audio::bgm::StageOpusMgr::RequestRetireFanfare()
{
    if (m_OpusIndex == 36) {
        GetRequestMgr()->RequestJingleDelayed(STRM_BGM_FNFR_TRAIN_RETIRE, 165);
    }
}

// 0x00585954
void audio::bgm::StageOpusMgr::AddMix9(u32 fade)
{
    s_EventFade = fade;
    GetTrackMixer()->AddMix(9);
}

// 0x00585974
void audio::bgm::StageOpusMgr::EndResult()
{
    if (m_OpusIndex != 40) {
        return;
    }
    static_cast<StageOpusResult*>(m_pOpus)->m_IsFinished = 0;
    SoundMgr::SStopBgmFade(5);
    SoundMgr::SPlayJingle(SEQ_BGM_ISL_AWARD_FIN);
}

// 0x005859A8
void audio::bgm::StageOpusMgr::AcceptHarvestCook()
{
    if (s_EventOpusList.Select() == &s_EventOpusList.m_Harvest) {
        s_EventOpusList.m_Harvest.m_IsCookAccepted = 1;
    }
}

// 0x005859D0
void audio::bgm::StageOpusMgr::StopOffice()
{
    if (m_OpusIndex != 4) {
        return;
    }
    StageOpusOffice* opus = static_cast<StageOpusOffice*>(m_pOpus);
    if (g_pSvCurrentPlayer != NULL && !g_pSvCurrentPlayer->playerFlags.finishedFirstDay) {
        GetRequestMgr()->SetFade(KIND_SHOP, STRM_BGM_IND_OFFICE, 180, 0, 0);
        opus->m_IsPlantDemo = 1;
    }
    m_Flags |= FLAG_PLANT_DEMO;
}

// 0x00585A50
void audio::bgm::StageOpusMgr::RequestKind10()
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(10, 0, 5, times);
}

// 0x00585A94
void audio::bgm::StageOpusMgr::StartCarnival2()
{
    EvOpusCarnival& carnival = s_EventOpusList.m_Carnival;
    carnival.m_State = 2;
    SoundMgr::SPlayCarnival(1);
    GetRequestMgr()->SetWait(KIND_CARNIVAL, carnival.m_BgmId, 1);
}

// 0x00585AC8
void audio::bgm::StageOpusMgr::StopStation()
{
    if (m_OpusIndex == 18) {
        StageOpusStation* opus = static_cast<StageOpusStation*>(m_pOpus);
        opus->m_IsTrainLeaving = 1;
        opus->Stop();
    }
}

// 0x00585AF0
void audio::bgm::StageOpusMgr::RemoveMix21()
{
    GetTrackMixer()->RemoveMix(21);
}

// 0x00585B04
void audio::bgm::StageOpusMgr::SetTownChanged()
{
    if (m_OpusIndex == 1) {
        static_cast<StageOpusVillageDowntown*>(m_pOpus)->m_IsEventChanged = 1;
    }
}

// 0x00585B30
void audio::bgm::StageOpusMgr::StartCarnival4()
{
    EvOpusCarnival& carnival = s_EventOpusList.m_Carnival;
    carnival.m_State = 4;
    SoundMgr::SPlayCarnival(3);
    GetRequestMgr()->SetWait(KIND_CARNIVAL, carnival.m_BgmId, 1);
}

// 0x00585B64
void audio::bgm::StageOpusMgr::RequestPrologue03()
{
    GetRequestMgr()->RequestHeldSound(STRM_BGM_PROLOGUE_03, 23);
}

// 0x00585B80
void audio::bgm::StageOpusMgr::RequestPrologue05()
{
    GetRequestMgr()->RequestHeldSound(STRM_BGM_PROLOGUE_05, 23);
}

// 0x00585B9C
void audio::bgm::StageOpusMgr::SetPlantDemoStep1()
{
    if (m_OpusIndex == 45) {
        SoundMgr::SSetPrologueStep(1);
    }
}

// 0x00585BB4
void audio::bgm::StageOpusMgr::RequestPrologue04()
{
    GetRequestMgr()->RequestHeldSound(STRM_BGM_PROLOGUE_04, 16);
}

// 0x00585BD0
void audio::bgm::StageOpusMgr::AddMix8()
{
    s_EventFade = 60;
    GetTrackMixer()->AddMix(8);
}

// 0x00585BF4
void audio::bgm::StageOpusMgr::RequestPrologue06()
{
    if (m_OpusIndex != 42) {
        return;
    }
    StageOpusPrologue2* opus = static_cast<StageOpusPrologue2*>(m_pOpus);
    RequestMgr::Times times = {50, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_C, STRM_BGM_PROLOGUE_06, 0, times);
    opus->m_StartsLater = 1;
}

// 0x00585C40
void audio::bgm::StageOpusMgr::StopUpdatePrologue()
{
    if (m_OpusIndex == 34) {
        m_pOpus->m_Fade = 8;
        m_pOpus->Stop();
    }
}

// 0x00585C68
void audio::bgm::StageOpusMgr::StopExchange()
{
    if (m_OpusIndex == 55) {
        m_pOpus->m_Fade = 5;
        m_pOpus->Stop();
    }
}

// 0x00585C90
void audio::bgm::StageOpusMgr::StartExchangeReset()
{
    if (m_OpusIndex == 55) {
        m_pOpus->m_BgmId = STRM_BGM_FLD_RESET;
        m_pOpus->Start();
    }
}

// 0x00585CBC
void audio::bgm::StageOpusMgr::StopExchange2()
{
    if (m_OpusIndex == 55) {
        m_pOpus->m_Fade = 5;
        m_pOpus->Stop();
    }
}

// 0x00585CE4
void audio::bgm::StageOpusMgr::StartExchangeTanukichi()
{
    if (m_OpusIndex == 55) {
        m_pOpus->m_BgmId = STRM_BGM_IND_TANUKICHI;
        m_pOpus->Start();
    }
}

// 0x00585D10
void audio::bgm::StageOpusMgr::Clear()
{
    if (m_pOpus != NULL) {
        delete m_pOpus;
        m_pOpus = NULL;
        m_OpusIndex = OPUS_NONE;
    }
    m_Flags = 0;
}

// 0x00585D58
void audio::bgm::StageOpusMgr::StartClub444Live()
{
    if (m_OpusIndex != 31) {
        return;
    }
    StageOpusClub444Live* opus = static_cast<StageOpusClub444Live*>(m_pOpus);
    opus->Start();
    opus->m_IsStopped = 0;
    opus->m_ShopState = StageOpusShopBase::SHOP_OPEN;
}

// 0x00585D9C
void audio::bgm::StageOpusMgr::StopCarnival()
{
    EvOpusCarnival& carnival = s_EventOpusList.m_Carnival;
    carnival.m_State = EvOpusCarnival::STATE_IDLE;
    GetRequestMgr()->SetFade(KIND_CARNIVAL, carnival.m_BgmId, 5, 0, 0);
}

// 0x00585DDC
void audio::bgm::StageOpusMgr::Update()
{
    if (m_pOpus != NULL) {
        m_pOpus->Update();
    }
    s_ClockState.Update((m_Flags & FLAG_READY) != 0);
}

// 0x00585E1C
void audio::bgm::StageOpusMgr::SetTownFlag5()
{
    s_TownState.flag5 = 1;
}

// 0x00585E30
void audio::bgm::StageOpusMgr::ClearTownFlag5()
{
    s_TownState.flag5 = 0;
}

// 0x00585E44
void audio::bgm::StageOpusMgr::SetHideWaiting()
{
    s_EventOpusList.m_Hide.m_IsWaiting = 1;
}

// 0x00585E68
void audio::bgm::StageOpusMgr::StopClub444Live()
{
    if (m_OpusIndex != 31) {
        return;
    }
    StageOpusClub444Live* opus = static_cast<StageOpusClub444Live*>(m_pOpus);
    opus->Stop();
    opus->m_IsStopped = 1;
}

// 0x00585EA8
void audio::bgm::StageOpusMgr::EndCarnivalShort()
{
    EvOpusCarnival& carnival = s_EventOpusList.m_Carnival;
    carnival.m_State = 0;
    carnival.m_BgmId = SEQ_BGM_EVT_CARNIVAL_END_S;
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_CARNIVAL, carnival.m_BgmId, 5, times);
}

// 0x00585EF8
audio::bgm::StageOpusMgr::StageOpusMgr() : m_pOpus(NULL), m_OpusIndex(OPUS_NONE), m_Flags(FLAG_4) {}

// 0x00585F1C
audio::bgm::StageOpusMgr::~StageOpusMgr()
{
    // (only the members are destroyed)
}

// 0x005870F4
audio::bgm::StageOpusMgr* audio::bgm::GetStageOpusMgr()
{
    return &s_StageOpusMgr;
}

namespace audio {
namespace bgm {
// the factory table of the opus (an index of SelectOpus)
// 0x008465BC
const StageOpusMgr::CreateFunc StageOpusMgr::s_CreateFuncs[OPUS_NUM] = {
    &StageOpusMgr::Create<StageOpusTitle>,           &StageOpusMgr::Create<StageOpusVillageDowntown>,
    &StageOpusMgr::Create<StageOpusLobbyIsland>,     &StageOpusMgr::Create<StageOpusTour>,
    &StageOpusMgr::Create<StageOpusOffice>,          &StageOpusMgr::Create<StageOpusMuseum>,
    &StageOpusMgr::Create<StageOpusShowroom>,        &StageOpusMgr::Create<StageOpusCafe>,
    &StageOpusMgr::Create<StageOpusCafeStaff>,       &StageOpusMgr::Create<StageOpusShop>,
    &StageOpusMgr::Create<StageOpusConvenience>,     &StageOpusMgr::Create<StageOpusSuper>,
    &StageOpusMgr::Create<StageOpusHomeCenter>,      &StageOpusMgr::Create<StageOpusImpro>,
    &StageOpusMgr::Create<StageOpusDepart>,          &StageOpusMgr::Create<StageOpusTourDesk>,
    &StageOpusMgr::Create<StageOpusShoes>,           &StageOpusMgr::Create<StageOpusBeauty>,
    &StageOpusMgr::Create<StageOpusStation>,         &StageOpusMgr::Create<StageOpusRecycle>,
    &StageOpusMgr::Create<StageOpusRealEstate>,      &StageOpusMgr::Create<StageOpusGrace>,
    &StageOpusMgr::Create<StageOpusGallery>,         &StageOpusMgr::Create<StageOpusPoliceBox>,
    &StageOpusMgr::Create<StageOpusFortune>,         &StageOpusMgr::Create<StageOpusPavilion>,
    &StageOpusMgr::Create<StageOpusAblesisters>,     &StageOpusMgr::Create<StageOpusExhibition>,
    &StageOpusMgr::Create<StageOpusDream>,           &StageOpusMgr::Create<StageOpusClub444>,
    &StageOpusMgr::Create<StageOpusEvidence>,        &StageOpusMgr::Create<StageOpusClub444Live>,
    &StageOpusMgr::Create<StageOpusPostOffice>,      &StageOpusMgr::Create<StageOpusTrain>,
    &StageOpusMgr::Create<StageOpusUpdatePrologue>,  &StageOpusMgr::Create<StageOpusEntryDemo>,
    &StageOpusMgr::Create<StageOpusRetireDemo>,      &StageOpusMgr::Create<StageOpusComeHomeDemo>,
    &StageOpusMgr::Create<StageOpusBarcarolle>,      &StageOpusMgr::Create<StageOpusAward>,
    &StageOpusMgr::Create<StageOpusResult>,          &StageOpusMgr::Create<StageOpusPrologue1>,
    &StageOpusMgr::Create<StageOpusPrologue2>,       &StageOpusMgr::Create<StageOpusPrologue3>,
    &StageOpusMgr::Create<StageOpusPrologue4>,       &StageOpusMgr::Create<StageOpusPlantDemo>,
    &StageOpusMgr::Create<StageOpusBirthday>,        &StageOpusMgr::Create<StageOpusCeremony>,
    &StageOpusMgr::Create<StageOpusSave>,            &StageOpusMgr::Create<StageOpusLoad>,
    &StageOpusMgr::Create<StageOpusSelect>,          &StageOpusMgr::Create<StageOpusGrowUp>,
    &StageOpusMgr::Create<StageOpusTakumiImport>,    &StageOpusMgr::Create<StageOpusChipTutorial>,
    &StageOpusMgr::Create<StageOpusCamping>,         &StageOpusMgr::Create<StageOpusExchange>,
    &StageOpusMgr::Create<StageOpusNull>,
};
} // namespace bgm
} // namespace audio

// 0x00815048
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusCafe>();
// 0x00815094
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusLoad>();
// 0x008150E0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusNull>();
// 0x00815108
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusSave>();
// 0x00815154
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusShop>();
// 0x008151A0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusTour>();
// 0x008151C8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusAward>();
// 0x00815214
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusDream>();
// 0x0081523C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusGrace>();
// 0x00815288
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusImpro>();
// 0x008152D4
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusShoes>();
// 0x00815320
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusSuper>();
// 0x0081536C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusTitle>();
// 0x00815394
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusTrain>();
// 0x008153E0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusBeauty>();
// 0x0081542C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusDepart>();
// 0x00815478
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusGrowUp>();
// 0x008154A8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusMuseum>();
// 0x008154D0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusOffice>();
// 0x00815500
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusResult>();
// 0x00815528
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusSelect>();
// 0x00815550
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusCamping>();
// 0x00815588
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusClub444>();
// 0x008155D4
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusFortune>();
// 0x00815620
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusGallery>();
// 0x0081566C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusRecycle>();
// 0x008156B8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusStation>();
// 0x00815704
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusBirthday>();
// 0x0081572C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusCeremony>();
// 0x00815754
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusEvidence>();
// 0x0081577C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusExchange>();
// 0x008157C0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPavilion>();
// 0x008157F0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusShowroom>();
// 0x0081583C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusTourDesk>();
// 0x00815888
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusCafeStaff>();
// 0x008158B0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusEntryDemo>();
// 0x008158D8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPlantDemo>();
// 0x00815900
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPoliceBox>();
// 0x0081594C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPrologue1>();
// 0x00815974
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPrologue2>();
// 0x0081599C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPrologue3>();
// 0x008159C4
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPrologue4>();
// 0x00815A00
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusBarcarolle>();
// 0x00815A28
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusExhibition>();
// 0x00815A74
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusHomeCenter>();
// 0x00815AC0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusPostOffice>();
// 0x00815AE8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusRealEstate>();
// 0x00815B34
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusRetireDemo>();
// 0x00815B5C
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusAblesisters>();
// 0x00815BA8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusClub444Live>();
// 0x00815BF8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusConvenience>();
// 0x00815C44
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusLobbyIsland>();
// 0x00815C74
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusChipTutorial>();
// 0x00815CC0
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusComeHomeDemo>();
// 0x00815CE8
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusTakumiImport>();
// 0x00815D10
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusUpdatePrologue>();
// 0x00815D54
template audio::bgm::StageOpusBase* audio::bgm::StageOpusMgr::Create<audio::bgm::StageOpusVillageDowntown>();
