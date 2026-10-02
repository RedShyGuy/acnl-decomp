#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// RTTI N5event19EventMonthlyWeekdayE @ 0x008D28E4
// vtable 0x009089EC (vptr 0x009089F4), offset_to_top 0, 3 entries
class EventMonthlyWeekday : public ::event::EventBase
{
public:
    EventMonthlyWeekday(); // ctor candidate(s) 0x0058F820 (unverified)
    virtual void vf_0x00(); // 0x0075446C slot 0x00 | virtual slot, introduced by event::EventBase
    virtual void vf_0x04(); // 0x00754534 slot 0x04 | virtual slot, introduced by event::EventBase
    virtual void vf_0x08(); // 0x007545B8 slot 0x08 | virtual slot, introduced by event::EventBase
};
} // namespace event
