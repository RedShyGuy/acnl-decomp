#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadDualScreenMethodTreeMgr.h"
#include "sead/seadDualScreenTask.h"
#include "sead/seadMethodTreeMgr.h"

namespace sead {
// (out of line: the derived constructors call it)
// 0x001259E4 (name is ours)
DECOMP_NOINLINE sead::DualScreenTask::DualScreenTask(const TaskConstructArg& arg, const char* name)
    : TaskBase(arg, name), mCalcNode(nullptr), mDrawTopNode(nullptr), mDrawBtmNode(nullptr)
{
    mCalcNode.pause(true);
    mDrawTopNode.pause(true);
    mDrawBtmNode.pause(true);
    mCalcNode.bind(this, &DualScreenTask::calc);
    if (name != nullptr) {
        mCalcNode.mINamableName = name;
    }
    mDrawTopNode.bind(this, &DualScreenTask::drawTop);
    if (name != nullptr) {
        mDrawTopNode.mINamableName = name;
    }
    mDrawBtmNode.bind(this, &DualScreenTask::drawBtm);
    if (name != nullptr) {
        mDrawBtmNode.mINamableName = name;
    }
}

// 0x00544928 slot 0x18
void sead::DualScreenTask::pauseCalcRec(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_ALL : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x00544990 slot 0x1C
void sead::DualScreenTask::pauseDrawRec(bool pause)
{
    if (pause) {
        mDrawTopNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_ALL);
        mDrawBtmNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_ALL);
    } else {
        mDrawTopNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_NONE);
        mDrawBtmNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_NONE);
    }
}

// 0x00544A74 slot 0x3C
void sead::DualScreenTask::attachCalcImpl()
{
    CriticalSection* criticalSection = &getMethodTreeMgr()->mCriticalSection;
    criticalSection->lock();
    TaskBase* parentTask = parent() != nullptr ? parent()->val() : nullptr;
    if (mTag != TAG_SYSTEM) {
        if (parentTask != nullptr) {
            parentTask->getMethodTreeNode(DualScreenMethodTreeMgr::METHOD_APP_CALC)->pushBackChild(&mCalcNode);
        } else {
            attachMethodWithCheck(DualScreenMethodTreeMgr::METHOD_APP_CALC, &mCalcNode);
        }
    } else {
        attachMethodWithCheck(DualScreenMethodTreeMgr::METHOD_SYS_CALC, &mCalcNode);
    }
    criticalSection->unlock();
}

// 0x00544B00 slot 0x40 | nintendogs:bytes-fuzzy
void sead::DualScreenTask::attachDrawImpl()
{
    CriticalSection* criticalSection = &getMethodTreeMgr()->mCriticalSection;
    criticalSection->lock();
    TaskBase* parentTask = parent() != nullptr ? parent()->val() : nullptr;
    if (mTag != TAG_SYSTEM) {
        if (parentTask != nullptr) {
            parentTask->getMethodTreeNode(DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW)->pushFrontChild(&mDrawTopNode);
            parentTask->getMethodTreeNode(DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW)->pushFrontChild(&mDrawBtmNode);
        } else {
            attachMethodWithCheck(DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW, &mDrawTopNode);
            attachMethodWithCheck(DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW, &mDrawBtmNode);
        }
    } else {
        attachMethodWithCheck(DualScreenMethodTreeMgr::METHOD_TOP_SYS_DRAW, &mDrawTopNode);
        attachMethodWithCheck(DualScreenMethodTreeMgr::METHOD_BTM_SYS_DRAW, &mDrawBtmNode);
    }
    criticalSection->unlock();
}

// 0x00544BD0 slot 0x44
void sead::DualScreenTask::detachCalcImpl()
{
    mCalcNode.detachAll();
}

// 0x00544BD8 slot 0x48 | nintendogs:bytes
void sead::DualScreenTask::detachDrawImpl()
{
    mDrawTopNode.detachAll();
    mDrawBtmNode.detachAll();
}

// 0x00544C34 slot 0x50
sead::MethodTreeNode* sead::DualScreenTask::getMethodTreeNode(s32 methodType)
{
    switch (methodType) {
    case DualScreenMethodTreeMgr::METHOD_SYS_CALC:
    case DualScreenMethodTreeMgr::METHOD_APP_CALC:
        return &mCalcNode;
    case DualScreenMethodTreeMgr::METHOD_SYS_DRAW: {
        DualScreenMethodTreeMgr* mgr = DynamicCast<DualScreenMethodTreeMgr>(getMethodTreeMgr());
        return mgr->mSysDrawScreen != DualScreenMethodTreeMgr::SCREEN_TOP ? &mDrawBtmNode : &mDrawTopNode;
    }
    case DualScreenMethodTreeMgr::METHOD_APP_DRAW:
    case DualScreenMethodTreeMgr::METHOD_APP_DRAW_FINAL: {
        DualScreenMethodTreeMgr* mgr = DynamicCast<DualScreenMethodTreeMgr>(getMethodTreeMgr());
        return mgr->mAppDrawScreen != DualScreenMethodTreeMgr::SCREEN_TOP ? &mDrawBtmNode : &mDrawTopNode;
    }
    case DualScreenMethodTreeMgr::METHOD_TOP_SYS_DRAW:
    case DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW:
    case DualScreenMethodTreeMgr::METHOD_TOP_APP_DRAW_FINAL:
        return &mDrawTopNode;
    case DualScreenMethodTreeMgr::METHOD_BTM_SYS_DRAW:
    case DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW:
    case DualScreenMethodTreeMgr::METHOD_BTM_APP_DRAW_FINAL:
        return &mDrawBtmNode;
    default:
        return nullptr;
    }
}

// 0x00544DA4 slot 0x58 (name is ours)
void sead::DualScreenTask::calc()
{
    // empty (in the original too)
}

// 0x00544DA8 slot 0x60 (name is ours)
void sead::DualScreenTask::drawBtm()
{
    // empty (in the original too)
}

// 0x00544DAC slot 0x5C (name is ours)
void sead::DualScreenTask::drawTop()
{
    // empty (in the original too)
}

// 0x00544DB0 slot 0x10
void sead::DualScreenTask::pauseCalc(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_SELF : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x00544E18 slot 0x14 | nintendogs:bytes
void sead::DualScreenTask::pauseDraw(bool pause)
{
    if (pause) {
        mDrawTopNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_SELF);
        mDrawBtmNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_SELF);
    } else {
        mDrawTopNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_NONE);
        mDrawBtmNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_NONE);
    }
}

// 0x00544F68 slot 0x00 | nintendogs:bytes
// 0x00544EFC (deleting dtor)
sead::DualScreenTask::~DualScreenTask()
{
    // (only the members are destroyed)
}
} // namespace sead
