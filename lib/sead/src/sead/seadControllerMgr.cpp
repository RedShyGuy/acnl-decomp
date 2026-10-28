#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCalculateTask.h"
#include "sead/seadControllerMgr.h"

namespace sead {
// ctor candidate(s) 0x00541DB0 (unverified)
sead::ControllerMgr::ControllerMgr()
{
}

// 0x00541E54 slot 0x00 | nintendogs:bytes
sead::ControllerMgr::~ControllerMgr()
{
}

// 0x00541BF0 slot 0x28 | slot vf_0x28 of sead::TaskBase
void sead::ControllerMgr::prepare()
{
}

// 0x00541B78 slot 0x58 | virtual slot, introduced by sead::CalculateTask
void sead::ControllerMgr::vf_0x58()
{
}

// 0x00541B54 | nintendogs:bytes [tier A]
void sead::ControllerMgr::setInstance_(sead::TaskBase*)
{
}

// 0x0074B404 | nintendogs:bytes [tier A]
void sead::ControllerMgr::getControlDevice(sead::ControllerDefine::DeviceId) const
{
}

} // namespace sead

// the type information of the classes below CalculateTask
// 0x00828BCC (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::CalculateTask>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
