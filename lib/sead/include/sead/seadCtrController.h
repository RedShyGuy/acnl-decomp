#pragma once

#include "decomp.h"
#include "sead/seadController.h"

namespace sead {
// RTTI N4sead13CtrControllerE @ 0x008D1614
// vtable 0x00905414 (vptr 0x0090541C), offset_to_top 0, 9 entries
class CtrController : public ::sead::Controller
{
public:
    CtrController(); // ctor candidate(s) 0x00542208 (unverified)
    virtual void vf_0x00(); // 0x0074B5E8 slot 0x00 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x04(); // 0x0074B59C slot 0x04 | virtual slot, introduced by sead::ControllerBase
    virtual void vf_0x08(); // 0x00542238 slot 0x08 | virtual slot, introduced by sead::Controller
    virtual void vf_0x0C(); // 0x00542234 slot 0x0C | virtual slot, introduced by sead::Controller
    virtual void vf_0x18(); // 0x00541F3C slot 0x18 | nintendogs:callseq
    CtrController(sead::ControllerMgr*); // 0x00542208 | nintendogs:bytes [tier A]
};
} // namespace sead
