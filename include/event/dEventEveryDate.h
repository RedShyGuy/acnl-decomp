#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// an event on a date that changes every year: a table of MonthDay for the years 2012..2050
// RTTI N5event14EventEveryDateE @ 0x008D28C0
// vtable 0x009089B0 (vptr 0x009089B8), offset_to_top 0, 3 entries
class EventEveryDate : public ::event::EventBase
{
public:
    EventEveryDate(const EventBase& base, const MonthDay* dates); // 0x0058EC94
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x007541CC slot 0x00
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x00754298 slot 0x04
    virtual void Update(); // 0x00754308 slot 0x08

    const MonthDay* m_pDates; // 0x3C
};
ASSERT_SIZE(EventEveryDate, 0x40);
} // namespace event
