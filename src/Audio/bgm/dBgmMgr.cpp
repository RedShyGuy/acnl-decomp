#include "Audio/bgm/dBgmMgr.h"
#include "Audio/bgm/dBgmClock.h"
#include "Audio/bgm/dBgmRequestMgr.h"
#include "Audio/bgm/dBgmTrackMixer.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "Sound/sound_Extern.h"
#include "event/event_Api.h"
#include "time/time_Api.h"

namespace audio {
namespace bgm {

// 0x0094F494
BgmState s_State;
// 0x0094F4B4
ClockState s_ClockState;
// 0x0094F4B9
SubState s_SubState;

// The global objects of the BGM manager, built by the static initializer below.
// 0x0098ABC0
FieldMusic s_FieldMusic;
// 0x0098ABD8
RequestMgr s_RequestMgr;
// 0x0098B018
StageOpusMgr s_StageOpusMgr;
// 0x0098B030
TrackMixer s_TrackMixer;

} // namespace bgm
} // namespace audio

// FieldMusic, RequestMgr, StageOpusMgr (its destructor is registered) and TrackMixer
// 0x0079FF14 (static initializer)

// 0x00584B68
void audio::bgm::Initialize()
{
    // (armlink removed a call on s_RequestMgr here)
    s_StageOpusMgr.Initialize();
    s_TrackMixer.AddMuteMix();
}

// 0x00584B94
s8 audio::bgm::IsEvidence()
{
    return s_State.isEvidence;
}

// 0x00584BA4
void audio::bgm::ChangeStage(stage::Name stage)
{
    s_State.isPlayerReady = 0;
    s_StageOpusMgr.ChangeStage(stage);
    s_TrackMixer.EndSequence();
}

// 0x00584BE0
void audio::bgm::Ready()
{
    if (s_State.isOnline) {
        s_RequestMgr.Suspend();
    }
    s_StageOpusMgr.Ready();
    s_State.isGrowUp = 0;
    s_State.isEvidence = 0;
}

// 0x00584C28
s8 audio::bgm::IsGrowUp()
{
    return s_State.isGrowUp;
}

// 0x00585F20
void audio::bgm::Begin(stage::Name stage, bool isOnline)
{
    s_State.isOnline = isOnline;
    s_StageOpusMgr.Begin(stage, isOnline);
    s_TrackMixer.BeginSequence(isOnline);
}

// 0x00585F5C
s8 audio::bgm::IsPlayerReady()
{
    return s_State.isPlayerReady;
}

// 0x00586178
void audio::bgm::Leave(stage::Name next)
{
    if (s_State.isOnline) {
        s_RequestMgr.Clear();
    }
    s_StageOpusMgr.Leave(next);
    s_TrackMixer.Leave();
}

// 0x00586D84
void audio::bgm::Reset()
{
    s_TrackMixer.Reset();
    s_StageOpusMgr.Clear();
    s_RequestMgr.Reset();
    EventOpusList::Reset();
}

// 0x005881B0
void audio::bgm::SetGrowUp()
{
    s_State.isGrowUp = 1;
}

// 0x00588D0C
void audio::bgm::SetEvidence()
{
    s_State.isEvidence = 1;
}

// 0x0058A190
bool audio::bgm::IsSubFree()
{
    return !GetRequestMgr()->Has(26, 0x100);
}

// 0x0058A1AC
s8 audio::bgm::IsSubActive()
{
    return s_SubState.isActive;
}

// 0x0058A1BC
void audio::bgm::StartSub()
{
    if (s_SubState.isActive) {
        return;
    }
    s_SubState.isActive = 1;
    s_SubState.isPlaying = 0;
    RequestMgr::Times times = {-1, -1, 0};
    GetRequestMgr()->Request(KIND_SUB, 0, s_SubStartFade, times);
    GetTrackMixer()->AddMix(16);
    SoundMgr::SMuteForEvent(true, s_SubStartFade);
    if (g_Unknown9516C0.mode == 255 && g_Unknown9516C0.p4 != NULL) {
        Func6DDE90(g_Unknown9516C0.p4);
    }
}

// 0x0058A268
void audio::bgm::CheckSubEnd()
{
    if (!GetRequestMgr()->Has(26, 0)) {
        s_SubState.isActive = 0;
    }
}

// 0x0058A298
bool audio::bgm::IsBgmInactive()
{
    return !SoundMgr::SIsBgmActive();
}

// 0x0058A2A8
void audio::bgm::FadeSubToMix17()
{
    if (!s_SubState.isActive) {
        return;
    }
    s_SubState.isPlaying = 0;
    GetRequestMgr()->SetFade(KIND_SUB, 0, s_SubMixFade, 0, 0);
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(17, 0, 20, times);
    GetTrackMixer()->RemoveMix(16);
    GetTrackMixer()->AddMix(17);
    GetTrackMixer()->Update();
}

// 0x0058A33C
bool audio::bgm::IsSubStopped()
{
    if (s_SubState.isActive && GetRequestMgr()->m_Player.IsPlaying(KIND_SUB, 0)) {
        return false;
    }
    return true;
}

// 0x0058A380
void audio::bgm::EndSub()
{
    if (!s_SubState.isActive) {
        return;
    }
    u32 fade = GetTrackMixer()->IsMixActive(16) ? s_SubEndFade : s_SubEndFadeShort;
    GetRequestMgr()->Remove(17, 0);
    if (!GetTrackMixer()->IsMixActive(17)) {
        s_SubState.isActive = 0;
    } else {
        RequestMgr::Times times = {0, static_cast<s32>(s_SubEndTime), 0};
        GetRequestMgr()->Request(26, 0, 1, times);
    }
    GetTrackMixer()->RemoveMix(16);
    GetTrackMixer()->RemoveMix(17);
    SoundMgr::SMuteForEvent(false, fade);
    RequestMgr::Times times = {0, static_cast<s32>(fade), 0};
    GetRequestMgr()->Request(26, 0x100, 1, times);
    GetStageOpusMgr()->SetTownChanged();
}

// 0x0058A494
bool audio::bgm::IsSubPlayingE2()
{
    if ((s_SubState.isPlaying & s_SubState.isActive) && SoundMgr::SGetBgmE2()) {
        return true;
    }
    return false;
}

// 0x0058A4C8
void audio::bgm::FadeSubLong()
{
    if (!s_SubState.isActive) {
        return;
    }
    s_SubState.isPlaying = 0;
    GetRequestMgr()->SetFade(KIND_SUB, 0, s_SubLongFade, s_SubLongWait, 0);
    RequestMgr::Times times = {0, -1, 0};
    GetRequestMgr()->Request(17, 0, 20, times);
}

// 0x0058A544
void audio::bgm::ResumeSub()
{
    if (!s_SubState.isActive) {
        return;
    }
    s_SubState.isPlaying = 1;
    GetRequestMgr()->SetDelay(KIND_SUB, 0, s_SubResumeDelay);
}

// 0x0058A584
bool audio::bgm::IsSubPlaying()
{
    if ((s_SubState.isPlaying & s_SubState.isActive) &&
        (GetRequestMgr()->m_Player.m_State & (Player::STATE_PLAYING | Player::STATE_WAITING))) {
        return true;
    }
    return false;
}

// 0x0058A5BC
bool audio::bgm::IsSubPlayingE1()
{
    if ((s_SubState.isPlaying & s_SubState.isActive) && SoundMgr::SGetBgmE1()) {
        return true;
    }
    return false;
}

// 0x0058A5F0
void audio::bgm::StopSub()
{
    if (s_SubState.isActive) {
        GetRequestMgr()->Remove(KIND_SUB, 0);
        GetRequestMgr()->Remove(17, 0);
        GetRequestMgr()->Remove(26, 0);
        GetTrackMixer()->RemoveMix(16);
        GetTrackMixer()->RemoveMix(17);
        SoundMgr::SMuteForEvent(false, 0);
        s_SubState.isActive = 0;
    }
    GetRequestMgr()->Remove(26, 0x100);
}

// 0x0058A67C
s32 audio::bgm::GetFadeToHour()
{
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    if (date->minute != 59) {
        return 20;
    }
    s32 ms = 60000 - (date->milliSecond + (date->second + 5) * 1000);
    if (ms * 30 <= -1000) {
        return 0;
    }
    return ms * 30 / 1000;
}

// 0x0058A6F8
void audio::bgm::ClockState::Update(bool isCountdown)
{
    if (!UpdateSecond(&second)) {
        return;
    }
    if (isNearHour) {
        if (UpdateHour(&hour)) {
            if (!IsInTime(0, 0, 0, 5)) {
                isNearHour = 0;
                return;
            }
            if (isChimeEnabled) {
                SoundMgr::SPlayTimeChime();
            }
        }
        isNearHour = IsInTime(59, 45, 0, 15);
    } else {
        bool near = IsInTime(59, 45, 0, 0) &&
                    !(Func3189EC() == event::EVENT_ID_COUNTDOWN &&
                      Func309C6C(event::EVENT_ID_COUNTDOWN, time::GetCurrentDate()));
        isNearHour = near;
        hour = near ? GetHour() : hour;
    }
    if (!isChimeEnabled || !isCountdown) {
        return;
    }
    if (Func3189EC() != event::EVENT_ID_COUNTDOWN) {
        return;
    }
    // the countdown of New Year's Eve
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    if (GetHour() == 23 && date->minute == 59) {
        u32 id;
        if (second < 50) {
            id = SE_EVT_COUNTDOWN_COUNT_S;
        } else if (second < 57) {
            id = SE_EVT_COUNTDOWN_COUNT_M;
        } else {
            id = SE_EVT_COUNTDOWN_COUNT_L;
        }
        SoundMgr::SPlayMuffledSound(id);
        return;
    }
    if (GetHour() != 0 || date->minute != 0 || second != 0) {
        return;
    }
    if (isChimeEnabled) {
        SoundMgr::SPlayTimeChime();
    }
    SoundMgr::SPlayMuffledSound(SE_EVT_COUNTDOWN_COUNT_F);
}

// 0x0058A890
void audio::bgm::ClockState::SetChime(bool isEnabled, bool keepChime)
{
    if (!isChimeEnabled && isEnabled) {
        hour = GetHour();
        second = GetSecond();
        isNearHour = IsInTime(59, 45, 0, 0) &&
                     !(Func3189EC() == event::EVENT_ID_COUNTDOWN &&
                       Func309C6C(event::EVENT_ID_COUNTDOWN, time::GetCurrentDate()));
        isChimeEnabled = 0;
        isShop = 0;
    }
    isChimeEnabled = isEnabled;
    if (!(isEnabled || keepChime) && SoundMgr::SIsTimeChimeActive()) {
        SoundMgr::SStopTimeChime();
    }
}

// 0x0058B86C
void audio::bgm::Update()
{
    if (!s_State.isPlayerReady) {
        s_State.isPlayerReady = Func5C68C8();
    }
    UpdateClock();
    s_RequestMgr.Update();
    s_StageOpusMgr.Update();
    s_TrackMixer.Update();
    s_FieldMusic.Update();
}
