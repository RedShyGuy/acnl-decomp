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

// 0x0074B498 slot 0x08 | virtual slot, introduced by sead::TaskBase
void sead::ControllerMgr::vf_0x08()
{
}

// 0x0074B44C slot 0x0C | virtual slot, introduced by sead::TaskBase
void sead::ControllerMgr::vf_0x0C()
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
