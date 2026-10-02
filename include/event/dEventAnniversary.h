#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// RTTI N5event16EventAnniversaryE @ 0x008D28CC
// vtable 0x009089C4 (vptr 0x009089CC), offset_to_top 0, 3 entries
class EventAnniversary : public ::event::EventBase
{
public:
    EventAnniversary(); // ctor candidate(s) 0x0058F33C (unverified)
    virtual void vf_0x00(); // 0x0058EFE8 slot 0x00 | virtual slot, introduced by event::EventBase
    virtual void vf_0x04(); // 0x0075430C slot 0x04 | virtual slot, introduced by event::EventBase
};
} // namespace event
