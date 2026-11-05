#include "event/dEventPlayerBirthday.h"
#include "Sv/dSvApi.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"

namespace event {
// 0x0058F8AC (name is ours)
event::EventPlayerBirthday::EventPlayerBirthday()
    : EventBase("プレイヤーの誕生日", "プレイヤーの誕生日", Times{Time(6, 0), Time(6, 0), TIME_NONE, TIME_NONE},
                Info{0xFFFF, 2, EVENT_ID_PLAYER_BIRTHDAY, 2}, Range{Time(6, 0), Time(0, 0), 1}, Schedule())
{
}

// 0x007545BC slot 0x00 (name is ours)
bool event::EventPlayerBirthday::IsActive(const nn::fnd::DateTimeParameters& date) const
{
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player == NULL) {
        return false;
    }
    // born on February 29: February 28 in the other years
    if (player->birthDay == 29 && player->birthMonth == 2 && !IsLeapYear(date.year)) {
        return date.day == 28 && date.month == 2;
    }
    return player->birthMonth == static_cast<u8>(date.month) && player->birthDay == static_cast<u8>(date.day);
}

// 0x00754648 slot 0x04 (name is ours)
bool event::EventPlayerBirthday::IsOnDate(s32 year, s32 month, s32 day) const
{
    SvPlayer* player = g_pSvCurrentPlayer;
    if (player == NULL) {
        return false;
    }
    if (player->birthDay == 29 && player->birthMonth == 2 && !IsLeapYear(year)) {
        return month == 2 && day == 28;
    }
    return player->birthMonth == static_cast<u8>(month) && player->birthDay == static_cast<u8>(day);
}

// 0x007546D0 slot 0x08 (name is ours)
void event::EventPlayerBirthday::Update()
{
    // empty (in the original too)
}
} // namespace event
