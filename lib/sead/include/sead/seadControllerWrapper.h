#pragma once

#include "decomp.h"
#include "sead/seadControllerWrapperBase.h"

namespace sead {
// RTTI N4sead17ControllerWrapperE @ 0x008D1C00
// vtable 0x00906038 (vptr 0x00906040), offset_to_top 0, 7 entries
// vtable 0x0090605C (vptr 0x00906064), offset_to_top -308, 2 entries
class ControllerWrapper : public ::sead::ControllerWrapperBase
{
public:
    ControllerWrapper(); // ctor candidate(s) 0x00548E10 (unverified)
    virtual void vf_0x00(); // 0x0074C86C slot 0x00 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x04(); // 0x0074C820 slot 0x04 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x08(); // 0x0054B2C0 slot 0x08 | virtual slot, introduced by sead::ControllerWrapperBase
    virtual void vf_0x0C(); // 0x00548E4C slot 0x0C | virtual slot, introduced by sead::ControllerWrapperBase
    virtual void vf_0x10(); // 0x00548CA0 slot 0x10 | virtual slot, introduced by sead::ControllerWrapperBase
};
} // namespace sead
