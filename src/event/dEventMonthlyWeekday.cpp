#include "event/dEventMonthlyWeekday.h"
#include "ssys/ma/ma_Math.h"
#include "time/time_Api.h"

namespace event {
// 0x0058F820 (name is ours)
event::EventMonthlyWeekday::EventMonthlyWeekday(const EventBase& base, const Param& param)
    : EventBase(base), m_Param(param)
{
}

// 0x0075446C slot 0x00 (name is ours)
bool event::EventMonthlyWeekday::IsActive(const nn::fnd::DateTimeParameters& date) const
{
    if ((GetRegionMask() & m_Info.regionMask) == 0) {
        return false;
    }
    if ((m_Param.monthMask & static_cast<u16>(1 << (static_cast<u8>(date.month) - 1))) == 0) {
        return false;
    }
    if (m_Param.dayOfWeek != date.dayOfWeek) {
        return false;
    }
    if (m_Param.week != 0 && !ssys::ma::IsInRange(date.day, m_Param.week * 7 - 6, m_Param.week * 7)) {
        return false;
    }
    return IsBeforeEnd(date);
}

// 0x00754534 slot 0x04 (name is ours)
bool event::EventMonthlyWeekday::IsOnDate(s32 year, s32 month, s32 day) const
{
    if ((GetRegionMask() & m_Info.regionMask) == 0) {
        return false;
    }
    if ((m_Param.monthMask & (1 << (month - 1))) == 0) {
        return false;
    }
    if (m_Param.week != 0 && !ssys::ma::IsInRange(day, m_Param.week * 7 - 6, m_Param.week * 7)) {
        return false;
    }
    return time::GetDayOfWeek(year, month, day) == m_Param.dayOfWeek;
}

// 0x007545B8 slot 0x08 (name is ours)
void event::EventMonthlyWeekday::Update()
{
    // empty (in the original too)
}
} // namespace event
