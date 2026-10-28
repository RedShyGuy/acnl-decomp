#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadControllerWrapperBase.h"
#include "sead/seadControllerWrapper.h"

namespace sead {
// ctor candidate(s) 0x00548E10 (unverified)
sead::ControllerWrapper::ControllerWrapper()
{
}

// 0x0054B2C0 slot 0x08 | virtual slot, introduced by sead::ControllerWrapperBase
void sead::ControllerWrapper::vf_0x08()
{
}

// 0x00548E4C slot 0x0C | virtual slot, introduced by sead::ControllerWrapperBase
void sead::ControllerWrapper::vf_0x0C()
{
}

// 0x00548CA0 slot 0x10 | virtual slot, introduced by sead::ControllerWrapperBase
void sead::ControllerWrapper::vf_0x10()
{
}

} // namespace sead

// the type information of the classes below ControllerWrapperBase
// 0x0082930C (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::ControllerWrapperBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
