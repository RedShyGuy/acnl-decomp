#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_NotificationEvent.h"
#include "nn/nex/nex_StringStream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17NotificationEventE @ 0x008CE6DC
// vtable 0x008FD458 (vptr 0x008FD460), offset_to_top 0, 2 entries
class NotificationEvent : public ::nn::nex::_DDL_NotificationEvent
{
public:
    // (inline)
    NotificationEvent() {}
    virtual ~NotificationEvent(); // 0x003873E0 slot 0x00
    // 0x003873C0 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(NotificationEvent, 0x24);

// the fields of the event as text; the type is split into type * 1000 + subtype (the name is ours)
nn::nex::StringStream& operator<<(nn::nex::StringStream& stream, const nn::nex::NotificationEvent& event); // 0x003D9EE0
} // namespace nex
} // namespace nn
