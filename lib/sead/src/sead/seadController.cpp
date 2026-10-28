#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerBase.h"
#include "sead/seadController.h"

namespace sead {
// ctor candidate(s) 0x0053E5D0 (unverified)
sead::Controller::Controller()
{
}

// 0x0053E640 slot 0x08 | virtual slot, introduced by sead::Controller
void sead::Controller::vf_0x08()
{
}

// 0x0053E63C slot 0x0C | virtual slot, introduced by sead::Controller
void sead::Controller::vf_0x0C()
{
}

// 0x0053E490 slot 0x10 | virtual slot, introduced by sead::Controller
void sead::Controller::vf_0x10()
{
}

// 0x00749EC8 slot 0x14 | virtual slot, introduced by sead::Controller
void sead::Controller::vf_0x14()
{
}

// 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
void sead::Controller::vf_0x18()
{
}

// 0x005440B8 slot 0x1C | virtual slot, introduced by sead::Controller
void sead::Controller::vf_0x1C()
{
}

// 0x0053E584 slot 0x20 | virtual slot, introduced by sead::Controller
void sead::Controller::vf_0x20()
{
}

} // namespace sead

// the type information of the classes below ControllerBase
// 0x00828E10 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControllerBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
