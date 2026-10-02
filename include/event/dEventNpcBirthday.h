#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// RTTI N5event16EventNpcBirthdayE @ 0x008D28D8
// vtable 0x009089D8 (vptr 0x009089E0), offset_to_top 0, 3 entries
class EventNpcBirthday : public ::event::EventBase
{
public:
    EventNpcBirthday(); // ctor candidate(s) 0x0058F474 (unverified)
    virtual void vf_0x00(); // 0x00754340 slot 0x00 | virtual slot, introduced by event::EventBase
    virtual void vf_0x04(); // 0x007543BC slot 0x04 | virtual slot, introduced by event::EventBase
};
} // namespace event
