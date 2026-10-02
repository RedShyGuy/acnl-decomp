#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// RTTI N5event19EventPlayerBirthdayE @ 0x008D28F0
// vtable 0x00908A00 (vptr 0x00908A08), offset_to_top 0, 3 entries
class EventPlayerBirthday : public ::event::EventBase
{
public:
    EventPlayerBirthday(); // ctor candidate(s) 0x0058F8AC (unverified)
    virtual void vf_0x00(); // 0x007545BC slot 0x00 | virtual slot, introduced by event::EventBase
    virtual void vf_0x04(); // 0x00754648 slot 0x04 | virtual slot, introduced by event::EventBase
    virtual void vf_0x08(); // 0x007546D0 slot 0x08 | virtual slot, introduced by event::EventBase
};
} // namespace event
