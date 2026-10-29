#include "sead/seadFaderTaskBase.h"
#include "sead/seadNullFaderTask.h"

namespace sead {
// 0x0054321C slot 0x1C
void sead::NullFaderTask::pauseDrawRec(bool)
{
    // empty (in the original too)
}

// 0x00543220 slot 0x40
void sead::NullFaderTask::attachDrawImpl()
{
    // empty (in the original too)
}

// 0x00543224 slot 0x48
void sead::NullFaderTask::detachDrawImpl()
{
    // empty (in the original too)
}

// 0x00543228 slot 0x24
void sead::NullFaderTask::pauseDrawChild(bool)
{
    // empty (in the original too)
}

// 0x0054322C slot 0x50
sead::MethodTreeNode* sead::NullFaderTask::getMethodTreeNode(s32)
{
    return nullptr;
}

// 0x00543234 slot 0x64
void sead::NullFaderTask::vf_0x64()
{
    // empty (in the original too)
}

// 0x00543238 slot 0x14
void sead::NullFaderTask::pauseDraw(bool)
{
    // empty (in the original too)
}

// 0x0054323C (name is ours)
sead::NullFaderTask::NullFaderTask(const TaskConstructArg& arg) : FaderTaskBase(arg, "NullFaderTask")
{
    mFadeOutEnd = 0;
    mWaitEnd = 0;
    mFadeInEnd = 0;
}

// 0x00543288 slot 0x00
// 0x00543278 (deleting dtor)
sead::NullFaderTask::~NullFaderTask()
{
    // empty (in the original too)
}
} // namespace sead
