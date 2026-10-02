#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_NotificationEvent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17NotificationEventE @ 0x008CE6DC
// vtable 0x008FD458 (vptr 0x008FD460), offset_to_top 0, 2 entries
class NotificationEvent : public ::nn::nex::_DDL_NotificationEvent
{
public:
    NotificationEvent(); // ctor candidate(s) 0x0039D29C, 0x003B4C5C, 0x003B9CB0, 0x003C24E8, 0x003C9888, 0x003CB76C (unverified)
    virtual void vf_0x00(); // 0x003873E0 slot 0x00 | virtual slot, introduced by nn::nex::_DDL_NotificationEvent
    virtual void vf_0x04(); // 0x003873C0 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_NotificationEvent
};
} // namespace nex
} // namespace nn
