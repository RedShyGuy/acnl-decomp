#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// the anniversary of the town (event id 37)
// RTTI N5event16EventAnniversaryE @ 0x008D28CC
// vtable 0x009089C4 (vptr 0x009089CC), offset_to_top 0, 3 entries
class EventAnniversary : public ::event::EventBase
{
public:
    EventAnniversary(); // 0x0058F33C
    // (the date is checked by IsAnniversary)
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x0058EFE8 slot 0x00
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x0075430C slot 0x04

    nn::fnd::DateTimeParameters m_Date; // 0x3C the date of the last update (set by Update of the events)
};
ASSERT_SIZE(EventAnniversary, 0x48);
} // namespace event
