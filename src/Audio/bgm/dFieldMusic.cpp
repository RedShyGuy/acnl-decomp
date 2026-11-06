#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Bs/dBsTelop.h"
#include "Other/dTourResult.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "stage/stage_Extern.h"
#include "time/time_Api.h"

// 0x0058A948
void audio::bgm::FieldMusic::FadeHornet(s32 stopWait, u32 fade)
{
    GetRequestMgr()->SetFade(KIND_FIELD, STRM_BGM_FLD_HORNET, fade, stopWait, 0);
}

// 0x0058A984
void audio::bgm::FieldMusic::FadeFanfareGet(u32 fade, s32 stopWait)
{
    GetRequestMgr()->SetFade(KIND_JINGLE_B, SEQ_BGM_FNFR_GET, fade, stopWait, 0);
    fanfareState = 3;
}

// 0x0058A9CC
void audio::bgm::FieldMusic::RequestObjSong(u32 song, u32 fade)
{
    GetTrackMixer()->AddMix(22);
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_OBJ_SONG, song, fade, times);
    objSongId = song;
}

// 0x0058AA1C
void audio::bgm::FieldMusic::RequestCoffee(u32 fade)
{
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_B, STRM_BGM_FNFR_COFFEE, fade, times);
}

// 0x0058AA5C
void audio::bgm::FieldMusic::RequestFieldBgm(bool isRacket)
{
    fieldBgmId = isRacket ? STRM_BGM_FLD_RACKET : STRM_BGM_FLD_RESET;
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_FIELD, fieldBgmId, 5, times);
    GetTrackMixer()->AddMix(21);
}

// 0x0058AABC
void audio::bgm::FieldMusic::FadeObjSong(u32 fade)
{
    GetRequestMgr()->SetFade(KIND_OBJ_SONG, objSongId, fade, 0, 0);
    GetTrackMixer()->RemoveMix(22);
}

// 0x0058AB00
void audio::bgm::FieldMusic::RequestShobonLoop()
{
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_B, STRM_BGM_FNFR_SHOBON_LOOP, 5, times);
}

// 0x0058AB3C
void audio::bgm::FieldMusic::SetHornetDelay(s32 delay)
{
    GetRequestMgr()->SetDelay(KIND_FIELD, STRM_BGM_FLD_HORNET, delay);
}

// 0x0058AB60
void audio::bgm::FieldMusic::FadeComplete(u32 fade, s32 stopWait)
{
    GetRequestMgr()->SetFade(KIND_JINGLE_A, STRM_BGM_FNFR_COMPLETE, fade, stopWait, 0);
}

// 0x0058AB9C
void audio::bgm::FieldMusic::FadeCoffee(s32 stopWait)
{
    GetRequestMgr()->SetFade(KIND_JINGLE_B, STRM_BGM_FNFR_COFFEE, 0, stopWait, 0);
}

// 0x0058ABD4
void audio::bgm::FieldMusic::RequestHornetEvent()
{
    RequestMgr::EventParam param = {5, -1, -1, 1};
    GetRequestMgr()->RequestEvent(KIND_FIELD, STRM_BGM_FLD_HORNET, 20, param, 0);
}

// 0x0058AC20
void audio::bgm::FieldMusic::FadeFanfareGetWait(u32 fade)
{
    GetRequestMgr()->SetFade(KIND_JINGLE_B, SEQ_BGM_FNFR_GET, fade, -1, 0);
    fanfareState = 2;
}

// 0x0058AC64
void audio::bgm::FieldMusic::StopIslandGame()
{
    GetRequestMgr()->SetFade(isFast ? 20 : 19, s_IslandGameBgm[gameIndex][gameColumn], 5, 0, 0);
    GetTrackMixer()->m_IsSequenceAlt2 = 0;
}

// 0x0058ACC0
void audio::bgm::FieldMusic::RequestFanfareGet()
{
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_B, SEQ_BGM_FNFR_GET, 5, times);
    fanfareState = 0;
}

// 0x0058AD04
void audio::bgm::FieldMusic::FadeKind10(s32 time, u32 fade)
{
    GetRequestMgr()->SetFade(10, 0, fade, 0, time);
}

// 0x0058AD38
void audio::bgm::FieldMusic::StartCoffee()
{
    GetRequestMgr()->SetDelay(KIND_JINGLE_B, STRM_BGM_FNFR_COFFEE, 0);
}

// 0x0058AD58
void audio::bgm::FieldMusic::SetHornetWait(s32 wait)
{
    GetRequestMgr()->SetWait(KIND_FIELD, STRM_BGM_FLD_HORNET, wait);
}

// 0x0058AD7C
void audio::bgm::FieldMusic::RestartFieldBgm()
{
    GetRequestMgr()->Remove(KIND_FIELD, fieldBgmId);
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_FIELD, fieldBgmId, 5, times);
}

// 0x0058ADC4
void audio::bgm::FieldMusic::FadeFieldBgm()
{
    GetRequestMgr()->SetFade(KIND_FIELD, fieldBgmId, 5, -1, 0);
}

// 0x0058ADF8
void audio::bgm::FieldMusic::StartIslandGame()
{
    SoundMgr::PlaySound(SE_SYS_TOUR_GAME_START, 0.0f);
    RequestMgr::EventParam param = {5, 0, -1, 0};
    GetRequestMgr()->RequestEvent(19, s_IslandGameBgm[gameIndex][gameColumn], 18, param, 1);
    GetTrackMixer()->m_IsSequenceAlt2 = 1;
}

// 0x0058AE74
void audio::bgm::FieldMusic::FadeResetScramble(u32 fade)
{
    GetRequestMgr()->SetFade(KIND_BGM_PREPARED, STRM_BGM_IND_RESET_SCRAMBLE, fade, 0, 0);
}

// 0x0058AEAC
void audio::bgm::FieldMusic::RequestComplete()
{
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_A, STRM_BGM_FNFR_COMPLETE, 5, times);
}

// 0x0058AEE8
void audio::bgm::FieldMusic::SetShobonLoopDelay(s32 delay)
{
    GetRequestMgr()->SetDelay(KIND_JINGLE_B, STRM_BGM_FNFR_SHOBON_LOOP, delay);
}

// 0x0058AF0C
void audio::bgm::FieldMusic::RequestBingo()
{
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_BINGO, 0, s_Kind5Fade, times);
}

// 0x0058AF48
void audio::bgm::FieldMusic::PlayUpaClose()
{
    SoundMgr::SStopJingle(5);
    SoundMgr::SPlayJingleDelayed(STRM_BGM_UPA_CLOSE);
}

// 0x0058AF7C
void audio::bgm::FieldMusic::SetFanfareGetWait(s32 wait)
{
    GetRequestMgr()->SetWait(KIND_JINGLE_B, SEQ_BGM_FNFR_GET, wait);
    fanfareState = 3;
}

// 0x0058AFAC
void audio::bgm::FieldMusic::StartBingo()
{
    GetRequestMgr()->SetDelay(KIND_BINGO, 0, 0);
}

// 0x0058AFC8
void audio::bgm::FieldMusic::StartResetScramble()
{
    GetRequestMgr()->SetDelay(KIND_BGM_PREPARED, STRM_BGM_IND_RESET_SCRAMBLE, 0);
}

// 0x0058AFE8
void audio::bgm::FieldMusic::FadeFieldBgm2()
{
    GetRequestMgr()->SetFade(KIND_FIELD, fieldBgmId, 5, -1, 0);
}

// 0x0058B01C
void audio::bgm::FieldMusic::RequestKind10(u32 fade)
{
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(10, 0, fade, times);
}

// 0x0058B054
void audio::bgm::FieldMusic::FadeFieldBgmMix(s32 stopWait)
{
    GetRequestMgr()->SetFade(KIND_FIELD, fieldBgmId, 5, stopWait, 0);
    GetTrackMixer()->RemoveMix(21);
}

// 0x0058B098
void audio::bgm::FieldMusic::RemoveMix25()
{
    GetTrackMixer()->RemoveMix(25);
}

// 0x0058B0AC
void audio::bgm::FieldMusic::RemoveMix24()
{
    GetTrackMixer()->RemoveMix(24);
}

// 0x0058B0C0
void audio::bgm::FieldMusic::FadeKind17(s32 time)
{
    GetRequestMgr()->SetFade(17, 0, 0, 0, time);
}

// 0x0058B0F0
void audio::bgm::FieldMusic::StartFanfareGet(s32 delay)
{
    GetRequestMgr()->SetDelay(KIND_JINGLE_B, SEQ_BGM_FNFR_GET, delay);
    fanfareState = 1;
}

// 0x0058B120
void audio::bgm::FieldMusic::FadeBingo()
{
    GetRequestMgr()->SetFade(KIND_BINGO, 0, s_Kind5Fade2, s_Kind5Wait, 0);
}

// 0x0058B164
void audio::bgm::FieldMusic::SetupIslandGame()
{
    gameIndex = GetTourType(TourResult::s_pInstance->m_Tour);
    if (time::GetCurrentDate()->hour > 6 && time::GetCurrentDate()->hour <= 18) {
        gameColumn = 0;
    } else {
        gameColumn = 1;
    }
    SoundMgr::SPlayBgm(s_IslandGameBgm[gameIndex][gameColumn]);
    GetTrackMixer()->AddMix(21);
    isFast = 0;
}

// 0x0058B1F0
void audio::bgm::FieldMusic::StopShobon()
{
    GetRequestMgr()->MarkRemoved(KIND_JINGLE_B, STRM_BGM_FNFR_SHOBON);
    GetStageOpusMgr()->StopOpus();
}

// 0x0058B214
void audio::bgm::FieldMusic::SetCompleteDelay(s32 delay)
{
    GetRequestMgr()->SetDelay(KIND_JINGLE_A, STRM_BGM_FNFR_COMPLETE, delay);
}

// 0x0058B238
void audio::bgm::FieldMusic::SetBoostOn()
{
    SoundMgr::SSetBoost(true);
}

// 0x0058B240
void audio::bgm::FieldMusic::RequestResetScramble()
{
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_BGM_PREPARED, STRM_BGM_IND_RESET_SCRAMBLE, 5, times);
}

// 0x0058B27C
void audio::bgm::FieldMusic::RequestFieldBgmNoMix(bool isRacket)
{
    fieldBgmId = isRacket ? STRM_BGM_FLD_RACKET : STRM_BGM_FLD_RESET;
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_FIELD, fieldBgmId, 5, times);
}

// 0x0058B2D0
void audio::bgm::FieldMusic::PlayAwardFanfare()
{
    SoundMgr::SPlayJingleDelayed(STRM_BGM_IND_FNFR_AWARD);
}

// 0x0058B2DC
void audio::bgm::FieldMusic::StartFieldBgm()
{
    GetRequestMgr()->SetDelay(KIND_FIELD, fieldBgmId, 0);
}

// 0x0058B2FC
void audio::bgm::FieldMusic::RequestKind17()
{
    if (GetRequestMgr()->Has(17, 0)) {
        GetRequestMgr()->SetTime(17, 0, -1);
        return;
    }
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(17, 0, 5, times);
}

// 0x0058B368
void audio::bgm::FieldMusic::RemoveFieldBgm()
{
    GetRequestMgr()->Remove(KIND_FIELD, fieldBgmId);
}

// 0x0058B384
void audio::bgm::FieldMusic::SetBoostOff()
{
    SoundMgr::SSetBoost(false);
}

// 0x0058B38C
void audio::bgm::FieldMusic::StartJinglePrepared()
{
    SoundMgr::SStartJinglePrepared();
}

// 0x0058B3A0
void audio::bgm::FieldMusic::AddMix25()
{
    GetTrackMixer()->AddMix(25);
}

// 0x0058B3B4
void audio::bgm::FieldMusic::AddMix24()
{
    GetTrackMixer()->AddMix(24);
}

// 0x0058B3C8
void audio::bgm::FieldMusic::FadeHornetNow(u32 fade)
{
    GetRequestMgr()->SetFade(KIND_FIELD, STRM_BGM_FLD_HORNET, fade, 0, 0);
}

// 0x0058B400
void audio::bgm::FieldMusic::RequestShobon(s32 delay)
{
    RequestMgr::Times times = {delay, -1, 0};
    GetRequestMgr()->Request(KIND_JINGLE_B, STRM_BGM_FNFR_SHOBON, 5, times);
}

// 0x0058B440
void audio::bgm::FieldMusic::RemoveMix27()
{
    GetTrackMixer()->RemoveMix(27);
}

// 0x0058B454
void audio::bgm::FieldMusic::SpeedUpIslandGame()
{
    GetRequestMgr()->SetFade(19, s_IslandGameBgm[gameIndex][gameColumn], 5, 0, 0);
    gameColumn += 2;
    BsTelop::ShowTelop(static_cast<BsTelop::TelopName>(28));
    SoundMgr::PlaySound(SE_SYS_TOUR_GAME_REMAINING, 0.0f);
    RequestMgr::EventParam param = {5, 0, -1, 0};
    GetRequestMgr()->RequestEvent(20, s_IslandGameBgm[gameIndex][gameColumn], 18, param, 0);
    isFast = 1;
}

// 0x0058B508
void audio::bgm::FieldMusic::RemoveBingo()
{
    GetRequestMgr()->Remove(KIND_BINGO, 0);
}

// 0x0058B520
void audio::bgm::FieldMusic::AddMix27()
{
    GetTrackMixer()->AddMix(27);
}

// 0x0058B534
void audio::bgm::FieldMusic::RemoveMix28()
{
    GetTrackMixer()->RemoveMix(28);
}

// 0x0058B548
void audio::bgm::FieldMusic::RequestBingoWithParam(bool resetParam)
{
    if (resetParam) {
        SoundMgr::SetNextBgmParam(0);
    }
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(KIND_BINGO, 0, s_Kind5Fade, times);
}

// 0x0058B590
void audio::bgm::FieldMusic::Update()
{
    // empty (in the original too)
}

// 0x0058B594
void audio::bgm::FieldMusic::FadeShobonLoop(u32 fade, s32 stopWait)
{
    GetRequestMgr()->SetFade(KIND_JINGLE_B, STRM_BGM_FNFR_SHOBON_LOOP, fade, stopWait, 0);
}

// 0x0058B5D0
audio::bgm::FieldMusic::FieldMusic() : fanfareState(3) {}

// 0x0058CC84
audio::bgm::FieldMusic* audio::bgm::GetFieldMusic()
{
    return &s_FieldMusic;
}

// 0x0058C704
void audio::bgm::FieldMusic::StartFieldBgm2()
{
    GetRequestMgr()->SetDelay(KIND_FIELD, fieldBgmId, 0);
}
