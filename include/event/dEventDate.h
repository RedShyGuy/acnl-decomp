#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// RTTI N5event9EventDateE @ 0x008D2904
// vtable 0x00908A28 (vptr 0x00908A30), offset_to_top 0, 3 entries
class EventDate : public ::event::EventBase
{
public:
    EventDate(); // ctor candidate(s) 0x0058FFB4 (unverified)
    virtual void vf_0x00(); // 0x00754BD4 slot 0x00 | virtual slot, introduced by event::EventBase
    virtual void vf_0x04(); // 0x00754C68 slot 0x04 | virtual slot, introduced by event::EventBase
    virtual void vf_0x08(); // 0x00754CAC slot 0x08 | virtual slot, introduced by event::EventBase
};
} // namespace event
