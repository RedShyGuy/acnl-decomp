#include "sead/seadTaskBase.h"
#include "sead/seadDualScreenTask.h"

namespace sead {
// ctor address unknown
sead::DualScreenTask::DualScreenTask()
{
}

// 0x00544F68 slot 0x00 | nintendogs:bytes
sead::DualScreenTask::~DualScreenTask()
{
}

// 0x0074BF14 slot 0x08 | virtual slot, introduced by sead::TaskBase
void sead::DualScreenTask::vf_0x08()
{
}

// 0x0074BEC8 slot 0x0C | virtual slot, introduced by sead::TaskBase
void sead::DualScreenTask::vf_0x0C()
{
}

// 0x00544DB0 slot 0x10 | virtual slot, introduced by sead::TaskBase
void sead::DualScreenTask::vf_0x10()
{
}

// 0x00544E18 slot 0x14 | nintendogs:bytes
void sead::DualScreenTask::pauseDraw(bool)
{
}

// 0x00544928 slot 0x18 | virtual slot, introduced by sead::TaskBase
void sead::DualScreenTask::vf_0x18()
{
}

// 0x00544990 slot 0x1C | virtual slot, introduced by sead::TaskBase
void sead::DualScreenTask::vf_0x1C()
{
}

// 0x00544A74 slot 0x3C | slot vf_0x3C of sead::TaskBase
void sead::DualScreenTask::attachCalcImpl()
{
}

// 0x00544B00 slot 0x40 | nintendogs:bytes-fuzzy
void sead::DualScreenTask::attachDrawImpl()
{
}

// 0x00544BD0 slot 0x44 | slot vf_0x44 of sead::TaskBase
void sead::DualScreenTask::detachCalcImpl()
{
}

// 0x00544BD8 slot 0x48 | nintendogs:bytes
void sead::DualScreenTask::detachDrawImpl()
{
}

// 0x0074BFC8 slot 0x4C | virtual slot, introduced by sead::TaskBase
void sead::DualScreenTask::vf_0x4C()
{
}

// 0x00544C34 slot 0x50 | slot vf_0x50 of sead::TaskBase
void sead::DualScreenTask::getMethodTreeNode(int)
{
}

// 0x00544DA4 slot 0x58 | virtual slot, introduced by sead::DualScreenTask
void sead::DualScreenTask::vf_0x58()
{
}

// 0x00544DAC slot 0x5C | virtual slot, introduced by sead::DualScreenTask
void sead::DualScreenTask::vf_0x5C()
{
}

// 0x00544DA8 slot 0x60 | virtual slot, introduced by sead::DualScreenTask
void sead::DualScreenTask::vf_0x60()
{
}

} // namespace sead
