#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// an event on a day of the week in some months (week 1..: the n-th such day of the month, 0: every)
// RTTI N5event19EventMonthlyWeekdayE @ 0x008D28E4
// vtable 0x009089EC (vptr 0x009089F4), offset_to_top 0, 3 entries
class EventMonthlyWeekday : public ::event::EventBase
{
public:
    struct Param
    {
        u16 monthMask; // 0x0 bit (month - 1)
        u8 dayOfWeek;  // 0x2
        s8 week;       // 0x3
    };

    EventMonthlyWeekday(const EventBase& base, const Param& param); // 0x0058F820
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x0075446C slot 0x00
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x00754534 slot 0x04
    virtual void Update(); // 0x007545B8 slot 0x08

    Param m_Param; // 0x3C
};
ASSERT_SIZE(EventMonthlyWeekday, 0x40);
} // namespace event
