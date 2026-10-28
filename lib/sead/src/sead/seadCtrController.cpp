#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadController.h"
#include "sead/seadCtrController.h"

namespace sead {
// ctor candidate(s) 0x00542208 (unverified)
sead::CtrController::CtrController()
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

// the type information of the classes below Controller
// 0x00828830 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::Controller>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
