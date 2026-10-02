#include "sead/seadTaskBase.h"
#include "sead/seadCalculateTask.h"

namespace sead {
// TODO: default ctor added so derived stubs compile - may not exist
sead::CalculateTask::CalculateTask()
{
}

// 0x00541B18 slot 0x00 | nintendogs:bytes
sead::CalculateTask::~CalculateTask()
{
}

// 0x0074B304 slot 0x08 | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x08()
{
}

// 0x0074B2B8 slot 0x0C | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x0C()
{
}

// 0x0054194C slot 0x10 | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x10()
{
}

// 0x005419B4 slot 0x14 | slot vf_0x14 of sead::TaskBase
void sead::CalculateTask::pauseDraw(bool)
{
}

// 0x0012C978 slot 0x18 | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x18()
{
}

// 0x005418C0 slot 0x1C | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x1C()
{
}

// 0x005418CC slot 0x20 | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x20()
{
}

// 0x00541934 slot 0x24 | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x24()
{
}

// 0x00138CB8 slot 0x3C | slot vf_0x3C of sead::TaskBase
void sead::CalculateTask::attachCalcImpl()
{
}

// 0x005418C4 slot 0x40 | slot vf_0x40 of sead::TaskBase
void sead::CalculateTask::attachDrawImpl()
{
}

// 0x0054536C slot 0x44 | slot vf_0x44 of sead::TaskBase
void sead::CalculateTask::detachCalcImpl()
{
}

// 0x005418C8 slot 0x48 | slot vf_0x48 of sead::TaskBase
void sead::CalculateTask::detachDrawImpl()
{
}

// 0x0074B3B8 slot 0x4C | virtual slot, introduced by sead::TaskBase
void sead::CalculateTask::vf_0x4C()
{
}

// 0x00541938 slot 0x50 | slot vf_0x50 of sead::TaskBase
void sead::CalculateTask::getMethodTreeNode(int)
{
}

// 0x00541948 slot 0x58 | virtual slot, introduced by sead::CalculateTask
void sead::CalculateTask::vf_0x58()
{
}

// 0x005419B8 | nintendogs:bytes [tier A]
sead::CalculateTask::CalculateTask(const sead::TaskConstructArg&, const char*)
{
}

} // namespace sead
