#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// an event on a fixed day of the year
// RTTI N5event9EventDateE @ 0x008D2904
// vtable 0x00908A28 (vptr 0x00908A30), offset_to_top 0, 3 entries
class EventDate : public ::event::EventBase
{
public:
    typedef MonthDay Param;

    EventDate(const EventBase& base, const Param& param); // 0x0058FFB4
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x00754BD4 slot 0x00
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x00754C68 slot 0x04
    virtual void Update(); // 0x00754CAC slot 0x08

    Param m_Param; // 0x3C
};
ASSERT_SIZE(EventDate, 0x40);
} // namespace event
