#include "event/dEventEveryDate.h"
#include "time/time_Api.h"

namespace event {
// 0x0058EC94 (name is ours)
event::EventEveryDate::EventEveryDate(const EventBase& base, const MonthDay* dates) : EventBase(base), m_pDates(dates)
{
}

// 0x007541CC slot 0x00 (name is ours)
bool event::EventEveryDate::IsActive(const nn::fnd::DateTimeParameters& date) const
{
    if ((GetRegionMask() & m_Info.regionMask) == 0) {
        return false;
    }
    s32 year = date.year;
    if (year < time::FIRST_YEAR) {
        year = time::FIRST_YEAR;
    } else if (year > time::END_YEAR) {
        year = time::END_YEAR;
    }
    const MonthDay& day = m_pDates[year - time::FIRST_YEAR];
    if (date.month != day.month) {
        return false;
    }
    if (date.day != day.day) {
        return false;
    }
    return IsBeforeEnd(date);
}

// 0x00754298 slot 0x04 (name is ours)
bool event::EventEveryDate::IsOnDate(s32 year, s32 month, s32 day) const
{
    if ((GetRegionMask() & m_Info.regionMask) == 0) {
        return false;
    }
    if (year < time::FIRST_YEAR) {
        year = time::FIRST_YEAR;
    } else if (year > time::END_YEAR) {
        year = time::END_YEAR;
    }
    const MonthDay& date = m_pDates[year - time::FIRST_YEAR];
    return date.month == month && date.day == day;
}

// 0x00754308 slot 0x08 (name is ours)
void event::EventEveryDate::Update()
{
    // empty (in the original too)
}
} // namespace event
