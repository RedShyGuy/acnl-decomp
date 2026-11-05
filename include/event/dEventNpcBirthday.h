#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// the birthday of a villager (event id 36)
// RTTI N5event16EventNpcBirthdayE @ 0x008D28D8
// vtable 0x009089D8 (vptr 0x009089E0), offset_to_top 0, 3 entries
class EventNpcBirthday : public ::event::EventBase
{
public:
    EventNpcBirthday(); // 0x0058F474
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x00754340 slot 0x00
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x007543BC slot 0x04
};
ASSERT_SIZE(EventNpcBirthday, 0x3C);
} // namespace event
