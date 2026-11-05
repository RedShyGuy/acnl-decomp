#include "event/dEventDate.h"

namespace event {
// 0x0058FFB4 (name is ours)
event::EventDate::EventDate(const EventBase& base, const Param& param) : EventBase(base), m_Param(param)
{
}

// 0x00754BD4 slot 0x00 (name is ours)
bool event::EventDate::IsActive(const nn::fnd::DateTimeParameters& date) const
{
    if ((GetRegionMask() & m_Info.regionMask) == 0) {
        return false;
    }
    if (date.month != m_Param.month) {
        return false;
    }
    if (date.day != m_Param.day) {
        return false;
    }
    return IsBeforeEnd(date);
}

// 0x00754C68 slot 0x04 (name is ours)
bool event::EventDate::IsOnDate(s32 /* year */, s32 month, s32 day) const
{
    if ((GetRegionMask() & m_Info.regionMask) == 0) {
        return false;
    }
    return m_Param.month == month && m_Param.day == day;
}

// 0x00754CAC slot 0x08 (name is ours)
void event::EventDate::Update()
{
    // empty (in the original too)
}
} // namespace event
