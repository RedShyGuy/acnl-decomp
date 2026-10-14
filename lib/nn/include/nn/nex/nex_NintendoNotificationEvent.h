#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_NintendoNotificationEvent.h"

namespace nn {
namespace nex {
// (0x003CD77C, named NintendoNotificationEvent::NintendoNotificationEvent() [ctor?] in symbols.json,
// is a block inside a function of the notification protocol server, not a constructor)
// RTTI N2nn3nex25NintendoNotificationEventE @ 0x008CEED0
// vtable 0x008FEB18 (vptr 0x008FEB20), offset_to_top 0, 2 entries
class NintendoNotificationEvent : public ::nn::nex::_DDL_NintendoNotificationEvent
{
public:
    // (inline)
    NintendoNotificationEvent() {}
    virtual ~NintendoNotificationEvent(); // 0x003B84EC slot 0x00
    // 0x003B84B4 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(NintendoNotificationEvent, 0x18);
} // namespace nex
} // namespace nn
