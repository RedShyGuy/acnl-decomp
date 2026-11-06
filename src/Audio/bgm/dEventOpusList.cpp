#include "Audio/bgm/dBgmMgr.h"
#include "Sound/sound_Extern.h"
#include "event/event_Api.h"
#include "netgame/netgame_Api.h"
#include "stage/stage_Extern.h"
#include "time/time_Api.h"

namespace audio {
namespace bgm {

// 0x0094F4A0
TownState s_TownState = {0, 0, 0, 0, 0, 0, 0, static_cast<f32>(static_cast<u16>(FIELD_UNIT_SIZE_X * 0.2f))};
// 0x0094F4B0
ParameterHIO s_ParameterHIO;
// 0x0098B074
EventOpusList s_EventOpusList;

} // namespace bgm
} // namespace audio

// TownState::distance, s_EventOpusList (its destructor is registered) and s_ParameterHIO
// 0x00796EE8 (static initializer)

// 0x0058C930
audio::bgm::EvOpusBase* audio::bgm::EventOpusList::Select()
{
    if (netgame::GetNetgameType() == 3) {
        return NULL;
    }
    const nn::fnd::DateTimeParameters* date = time::GetCurrentDate();
    u8 event = Func3189EC();
    if (Func2932E4()) {
        return &m_Cleaning;
    }
    if (m_Hide.m_IsStarted) {
        return &m_Hide;
    }
    if (m_Hide.m_IsWaiting && g_Unknown951718.mode == 1 && g_Unknown951718.flag5 == 0) {
        return &m_Hide;
    }
    if (m_Hide.m_IsResumed) {
        return &m_Hide;
    }
    if (event == event::EVENT_ID_COUNTDOWN && Func309C6C(event::EVENT_ID_COUNTDOWN, time::GetCurrentDate())) {
        return &m_Countdown;
    }
    if (date->month == 1 && date->day == 1 && date->hour < 6) {
        return &m_NewYear;
    }
    if (Func30BA74() == event::EVENT_ID_NEW_YEARS_DAY && Func309C6C(event::EVENT_ID_NEW_YEARS_DAY, time::GetCurrentDate())) {
        return &m_NewYearNext;
    }
    switch (event) {
    case event::EVENT_ID_FIREWORKS_SHOW:
        if (Func309C6C(event::EVENT_ID_FIREWORKS_SHOW, time::GetCurrentDate())) {
            return &m_Fireworks;
        }
        break;
    case event::EVENT_ID_HALLOWEEN:
        if (Func309C6C(event::EVENT_ID_HALLOWEEN, time::GetCurrentDate())) {
            return &m_Halloween;
        }
        break;
    case event::EVENT_ID_HARVEST_FESTIVAL:
        if (m_Harvest.m_IsCookAccepted ||
            event::IsBeforeStart(event::EVENT_ID_HARVEST_FESTIVAL, time::GetCurrentDate()) ||
            Func309C6C(event::EVENT_ID_HARVEST_FESTIVAL, time::GetCurrentDate())) {
            return &m_Harvest;
        }
        break;
    case event::EVENT_ID_TOY_DAY:
        return &m_XmasEve;
    case event::EVENT_ID_FESTIVALE:
        return &m_Carnival;
    case event::EVENT_ID_BUNNY_DAY:
        return &m_Easter;
    }
    return NULL;
}

// 0x0058CB0C
void audio::bgm::EventOpusList::Reset()
{
    s_EventOpusList.m_Cleaning.EvOpusCleaning::Reset();
    s_EventOpusList.m_Hide.EvOpusHide::Reset();
    s_EventOpusList.m_Fireworks.OpusBase::Reset();
    s_EventOpusList.m_Countdown.OpusBase::Reset();
    s_EventOpusList.m_NewYear.OpusBase::Reset();
    s_EventOpusList.m_NewYearNext.OpusBase::Reset();
    s_EventOpusList.m_Halloween.OpusBase::Reset();
    s_EventOpusList.m_Harvest.EvOpusHarvest::Reset();
    s_EventOpusList.m_XmasEve.OpusBase::Reset();
    s_EventOpusList.m_Carnival.EvOpusCarnival::Reset();
    s_EventOpusList.m_Easter.OpusBase::Reset();
}

// 0x0058CB74
audio::bgm::EventOpusList::EventOpusList()
{
    // (only the members are constructed)
}

// 0x0058CC80
audio::bgm::EventOpusList::~EventOpusList()
{
    // (only the members are destroyed)
}
