#pragma once

#include "decomp.h"
#include "sead/seadControllerBase.h"

namespace sead {
// RTTI N4sead10ControllerE @ 0x008D1358
// vtable 0x00904E0C (vptr 0x00904E14), offset_to_top 0, 9 entries
class Controller : public ::sead::ControllerBase
{
public:
    Controller(); // ctor candidate(s) 0x0053E5D0 (unverified)
    virtual void vf_0x00(); // 0x00749F1C slot 0x00 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x04(); // 0x00749ED0 slot 0x04 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x08(); // 0x0053E640 slot 0x08 | virtual slot, introduced by sead::Controller
    virtual void vf_0x0C(); // 0x0053E63C slot 0x0C | virtual slot, introduced by sead::Controller
    virtual void vf_0x10(); // 0x0053E490 slot 0x10 | virtual slot, introduced by sead::Controller
    virtual void vf_0x14(); // 0x00749EC8 slot 0x14 | virtual slot, introduced by sead::Controller
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x005440B8 slot 0x1C | virtual slot, introduced by sead::Controller
    virtual void vf_0x20(); // 0x0053E584 slot 0x20 | virtual slot, introduced by sead::Controller
};
} // namespace sead
