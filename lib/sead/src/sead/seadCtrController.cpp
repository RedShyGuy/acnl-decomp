#include "sead/seadController.h"
#include "sead/seadCtrController.h"

namespace sead {
// ctor candidate(s) 0x00542208 (unverified)
sead::CtrController::CtrController()
{
}

// 0x0074B5E8 slot 0x00 | virtual slot, introduced by sead::ControllerBase
void sead::CtrController::vf_0x00()
{
}

// 0x0074B59C slot 0x04 | virtual slot, introduced by sead::ControllerBase
void sead::CtrController::vf_0x04()
{
}

// 0x00542238 slot 0x08 | virtual slot, introduced by sead::Controller
void sead::CtrController::vf_0x08()
{
}

// 0x00542234 slot 0x0C | virtual slot, introduced by sead::Controller
void sead::CtrController::vf_0x0C()
{
}

// 0x00541F3C slot 0x18 | nintendogs:callseq
void sead::CtrController::vf_0x18()
{
}

// 0x00542208 | nintendogs:bytes [tier A]
sead::CtrController::CtrController(sead::ControllerMgr*)
{
}

} // namespace sead
