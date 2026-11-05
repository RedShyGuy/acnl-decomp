#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// the birthday of the current player (event id 35; on February 28 in other years than leap years)
// RTTI N5event19EventPlayerBirthdayE @ 0x008D28F0
// vtable 0x00908A00 (vptr 0x00908A08), offset_to_top 0, 3 entries
class EventPlayerBirthday : public ::event::EventBase
{
public:
    EventPlayerBirthday(); // 0x0058F8AC
    virtual bool IsActive(const nn::fnd::DateTimeParameters& date) const; // 0x007545BC slot 0x00
    virtual bool IsOnDate(s32 year, s32 month, s32 day) const; // 0x00754648 slot 0x04
    virtual void Update(); // 0x007546D0 slot 0x08
};
ASSERT_SIZE(EventPlayerBirthday, 0x3C);
} // namespace event
