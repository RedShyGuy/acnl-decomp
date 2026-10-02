#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30_DDL_NintendoNotificationEventE @ 0x008CF218
// vtable 0x008FF4F8 (vptr 0x008FF500), offset_to_top 0, 2 entries
class _DDL_NintendoNotificationEvent : public ::nn::nex::RootObject
{
public:
    _DDL_NintendoNotificationEvent(); // ctor address unknown
    virtual void vf_0x00(); // 0x003C4BDC slot 0x00 | virtual slot, introduced by nn::nex::_DDL_NintendoNotificationEvent
    virtual void vf_0x04(); // 0x003C4BA4 slot 0x04 | virtual slot, introduced by nn::nex::_DDL_NintendoNotificationEvent
    void Extract(nn::nex::Message*, nn::nex::_DDL_NintendoNotificationEvent*); // 0x003C4A7C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
