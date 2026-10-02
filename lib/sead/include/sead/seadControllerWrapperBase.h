#pragma once

#include "decomp.h"
#include "sead/seadControllerBase.h"
#include "sead/seadIDisposer.h"

namespace sead {
// RTTI N4sead21ControllerWrapperBaseE @ 0x008D1E64
// vtable 0x00906534 (vptr 0x0090653C), offset_to_top 0, 7 entries
// vtable 0x00906558 (vptr 0x00906560), offset_to_top -308, 2 entries
class ControllerWrapperBase : public ::sead::ControllerBase, public ::sead::IDisposer
{
public:
    ControllerWrapperBase(); // ctor candidate(s) 0x0054B210 (unverified)
    virtual void vf_0x00(); // 0x0074D5D8 slot 0x00 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x04(); // 0x0074D58C slot 0x04 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x08(); // 0x0054B2C4 slot 0x08 | virtual slot, introduced by sead::ControllerWrapperBase
    virtual void vf_0x0C(); // 0x0054B264 slot 0x0C | virtual slot, introduced by sead::ControllerWrapperBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0054415C slot 0x14 | virtual slot, introduced by sead::ControllerWrapperBase
    virtual void vf_0x18(); // 0x0054B20C slot 0x18 | virtual slot, introduced by sead::ControllerWrapperBase
};
} // namespace sead
