#pragma once

#include "decomp.h"
#include "event/dEventBase.h"

namespace event {
// RTTI N5event14EventEveryDateE @ 0x008D28C0
// vtable 0x009089B0 (vptr 0x009089B8), offset_to_top 0, 3 entries
class EventEveryDate : public ::event::EventBase
{
public:
    EventEveryDate(); // ctor candidate(s) 0x0058EC94 (unverified)
    virtual void vf_0x00(); // 0x007541CC slot 0x00 | virtual slot, introduced by event::EventBase
    virtual void vf_0x04(); // 0x00754298 slot 0x04 | virtual slot, introduced by event::EventBase
    virtual void vf_0x08(); // 0x00754308 slot 0x08 | virtual slot, introduced by event::EventBase
};
} // namespace event
