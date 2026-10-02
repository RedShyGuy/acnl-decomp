#include "sead/seadFaderTaskBase.h"
#include "sead/seadNullFaderTask.h"

namespace sead {
// ctor candidate(s) 0x0054323C (unverified)
sead::NullFaderTask::NullFaderTask()
{
}

// 0x00543288 slot 0x00 | slot vf_0x00 of sead::IDisposer
sead::NullFaderTask::~NullFaderTask()
{
}

// 0x00543238 slot 0x14 | slot vf_0x14 of sead::TaskBase
void sead::NullFaderTask::pauseDraw(bool)
{
}

// 0x0054321C slot 0x1C | virtual slot, introduced by sead::TaskBase
void sead::NullFaderTask::vf_0x1C()
{
}

// 0x00543228 slot 0x24 | virtual slot, introduced by sead::TaskBase
void sead::NullFaderTask::vf_0x24()
{
}

// 0x00543220 slot 0x40 | slot vf_0x40 of sead::TaskBase
void sead::NullFaderTask::attachDrawImpl()
{
}

// 0x00543224 slot 0x48 | slot vf_0x48 of sead::TaskBase
void sead::NullFaderTask::detachDrawImpl()
{
}

// 0x0074B9F8 slot 0x4C | virtual slot, introduced by sead::TaskBase
void sead::NullFaderTask::vf_0x4C()
{
}

// 0x0054322C slot 0x50 | slot vf_0x50 of sead::TaskBase
void sead::NullFaderTask::getMethodTreeNode(int)
{
}

// 0x00543234 slot 0x64 | virtual slot, introduced by sead::NullFaderTask
void sead::NullFaderTask::vf_0x64()
{
}

} // namespace sead
