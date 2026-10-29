#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadCalculateTask.h"
#include "sead/seadMethodTreeMgr.h"

namespace sead {
// 0x005418C0 slot 0x1C
void sead::CalculateTask::pauseDrawRec(bool)
{
    // empty (in the original too)
}

// 0x005418C4 slot 0x40
void sead::CalculateTask::attachDrawImpl()
{
    // empty (in the original too)
}

// 0x005418C8 slot 0x48
void sead::CalculateTask::detachDrawImpl()
{
    // empty (in the original too)
}

// 0x005418CC slot 0x20
void sead::CalculateTask::pauseCalcChild(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_CHILDREN : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x00541934 slot 0x24
void sead::CalculateTask::pauseDrawChild(bool)
{
    // empty (in the original too)
}

// 0x00541938 slot 0x50
sead::MethodTreeNode* sead::CalculateTask::getMethodTreeNode(s32 methodType)
{
    if (methodType == 0 || methodType == 1) {
        return &mCalcNode;
    }
    return nullptr;
}

// 0x00541948 slot 0x58 (name is ours)
void sead::CalculateTask::calc()
{
    // empty (in the original too)
}

// 0x0054194C slot 0x10
void sead::CalculateTask::pauseCalc(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_SELF : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x005419B4 slot 0x14
void sead::CalculateTask::pauseDraw(bool)
{
    // empty (in the original too)
}

// (out of line: the derived constructors call it)
// 0x005419B8 | nintendogs:bytes [tier A]
DECOMP_NOINLINE sead::CalculateTask::CalculateTask(const TaskConstructArg& arg, const char* name)
    : TaskBase(arg, name), mCalcNode(nullptr)
{
    mCalcNode.pause(true);
    mCalcNode.bind(this, &CalculateTask::calc);
    if (name != nullptr) {
        mCalcNode.mINamableName = name;
    }
}

// 0x00541B18 slot 0x00 | nintendogs:bytes
// 0x00541ADC (deleting dtor)
sead::CalculateTask::~CalculateTask()
{
    // (only the members are destroyed)
}

// 0x0012C978 slot 0x18
void sead::CalculateTask::pauseCalcRec(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_ALL : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x00138CB8 slot 0x3C
void sead::CalculateTask::attachCalcImpl()
{
    CriticalSection* criticalSection = &getMethodTreeMgr()->mCriticalSection;
    criticalSection->lock();
    TaskBase* parentTask = parent() != nullptr ? parent()->val() : nullptr;
    if (mTag != TAG_SYSTEM) {
        if (parentTask != nullptr) {
            parentTask->getMethodTreeNode(1)->pushBackChild(&mCalcNode);
        } else {
            attachMethodWithCheck(1, &mCalcNode);
        }
    } else {
        attachMethodWithCheck(0, &mCalcNode);
    }
    criticalSection->unlock();
}

// 0x0054536C slot 0x44
void sead::CalculateTask::detachCalcImpl()
{
    mCalcNode.detachAll();
}
} // namespace sead

// the type information of the classes below TaskBase
// 0x00829878 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::TaskBase>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
