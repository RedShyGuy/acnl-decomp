#include "event/dEventAnniversary.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"

namespace event {
// 0x0058F33C (name is ours)
event::EventAnniversary::EventAnniversary()
    : EventBase("周年イベント", "", Times{Time(6, 0), Time(0, 0), TIME_NONE, TIME_NONE},
                Info{0xFFFF, 3, EVENT_ID_TOWN_ANNIVERSARY, 2}, Range{Time(6, 0), Time(0, 0), 1}, Schedule())
{
}

// 0x0058EFE8 slot 0x00 (name is ours)
bool event::EventAnniversary::IsActive(const nn::fnd::DateTimeParameters& date) const
{
    return IsAnniversary(date);
}

// 0x0075430C slot 0x04 (name is ours)
bool event::EventAnniversary::IsOnDate(s32 year, s32 month, s32 day) const
{
    nn::fnd::DateTimeParameters date = MakeDateTime(year, month, day, 0, 0, 0, 0);
    return IsAnniversary(date);
}
} // namespace event
