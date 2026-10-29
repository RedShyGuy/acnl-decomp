#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenTask.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadUlcdTask.h"

namespace sead {
// (out of line: the derived constructors call it)
// 0x00120AD0 (name is ours)
DECOMP_NOINLINE sead::UlcdTask::UlcdTask(const TaskConstructArg& arg, const char* name)
    : DualScreenTask(arg, name), mDrawRightNode(nullptr)
{
    mDrawRightNode.pause(true);
    mDrawRightNode.bind(this, &UlcdTask::drawTopRight);
    if (name != nullptr) {
        mDrawRightNode.mINamableName = name;
    }
}

// 0x00544BF4 slot 0x50
sead::MethodTreeNode* sead::UlcdTask::getMethodTreeNode(s32 methodType)
{
    switch (methodType) {
    case UlcdMethodTreeMgr::METHOD_LEFT_SYS_DRAW:
    case UlcdMethodTreeMgr::METHOD_LEFT_APP_DRAW:
    case UlcdMethodTreeMgr::METHOD_LEFT_APP_DRAW_FINAL:
        return DualScreenTask::getMethodTreeNode(DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW);
    case UlcdMethodTreeMgr::METHOD_RIGHT_SYS_DRAW:
    case UlcdMethodTreeMgr::METHOD_RIGHT_APP_DRAW:
    case UlcdMethodTreeMgr::METHOD_RIGHT_APP_DRAW_FINAL:
        return &mDrawRightNode;
    default:
        return DualScreenTask::getMethodTreeNode(methodType);
    }
}

// 0x0056172C slot 0x1C
void sead::UlcdTask::pauseDrawRec(bool pause)
{
    DualScreenTask::pauseDrawRec(pause);
    mDrawRightNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_ALL : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x005617A0 slot 0x40 | nintendogs:bytes-fuzzy
void sead::UlcdTask::attachDrawImpl()
{
    DualScreenTask::attachDrawImpl();
    CriticalSection* criticalSection = &getMethodTreeMgr()->mCriticalSection;
    criticalSection->lock();
    TaskBase* parentTask = parent() != nullptr ? parent()->val() : nullptr;
    if (mTag != TAG_SYSTEM) {
        if (parentTask != nullptr) {
            parentTask->getMethodTreeNode(UlcdMethodTreeMgr::METHOD_RIGHT_APP_DRAW)->pushFrontChild(&mDrawRightNode);
        } else {
            attachMethodWithCheck(UlcdMethodTreeMgr::METHOD_RIGHT_APP_DRAW, &mDrawRightNode);
        }
    } else {
        attachMethodWithCheck(UlcdMethodTreeMgr::METHOD_RIGHT_SYS_DRAW, &mDrawRightNode);
    }
    criticalSection->unlock();
}

// 0x00561834 slot 0x48 | nintendogs:bytes
void sead::UlcdTask::detachDrawImpl()
{
    DualScreenTask::detachDrawImpl();
    mDrawRightNode.detachAll();
}

// 0x0056184C slot 0x5C (name is ours)
void sead::UlcdTask::drawTop()
{
    drawTopLeft();
}

// 0x00561858 slot 0x64 (name is ours)
void sead::UlcdTask::drawTopLeft()
{
    // empty (in the original too)
}

// 0x0056185C slot 0x68 (name is ours)
void sead::UlcdTask::drawTopRight()
{
    // empty (in the original too)
}

// 0x00561860 slot 0x14 | nintendogs:bytes
void sead::UlcdTask::pauseDraw(bool pause)
{
    DualScreenTask::pauseDraw(pause);
    mDrawRightNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_SELF : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x00561968 slot 0x00 | nintendogs:bytes
// 0x005618D4 (deleting dtor)
sead::UlcdTask::~UlcdTask()
{
    // (only the members are destroyed)
}
} // namespace sead

// the type information of the classes below DualScreenTask
// 0x00828EF8 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::DualScreenTask>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;
