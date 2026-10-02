#include "sead/seadDualScreenTask.h"
#include "sead/seadUlcdTask.h"

namespace sead {
// ctor address unknown
sead::UlcdTask::UlcdTask()
{
}

// 0x00561968 slot 0x00 | nintendogs:bytes
sead::UlcdTask::~UlcdTask()
{
}

// 0x0074FA80 slot 0x08 | virtual slot, introduced by sead::TaskBase
void sead::UlcdTask::vf_0x08()
{
}

// 0x0074FA34 slot 0x0C | virtual slot, introduced by sead::TaskBase
void sead::UlcdTask::vf_0x0C()
{
}

// 0x00561860 slot 0x14 | nintendogs:bytes
void sead::UlcdTask::pauseDraw(bool)
{
}

// 0x0056172C slot 0x1C | virtual slot, introduced by sead::TaskBase
void sead::UlcdTask::vf_0x1C()
{
}

// 0x005617A0 slot 0x40 | nintendogs:bytes-fuzzy
void sead::UlcdTask::attachDrawImpl()
{
}

// 0x00561834 slot 0x48 | nintendogs:bytes
void sead::UlcdTask::detachDrawImpl()
{
}

// 0x0074FB84 slot 0x4C | virtual slot, introduced by sead::TaskBase
void sead::UlcdTask::vf_0x4C()
{
}

// 0x00544BF4 slot 0x50 | slot vf_0x50 of sead::TaskBase
void sead::UlcdTask::getMethodTreeNode(int)
{
}

// 0x0056184C slot 0x5C | virtual slot, introduced by sead::DualScreenTask
void sead::UlcdTask::vf_0x5C()
{
}

// 0x00561858 slot 0x64 | virtual slot, introduced by sead::UlcdTask
void sead::UlcdTask::vf_0x64()
{
}

// 0x0056185C slot 0x68 | virtual slot, introduced by sead::UlcdTask
void sead::UlcdTask::vf_0x68()
{
}

} // namespace sead
