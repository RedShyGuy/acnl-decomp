#include "sead/seadFaderTaskBase.h"
#include "sead/seadTaskBase.h"
#include "sead/seadTaskEvent.h"
#include "sead/seadTaskMgr.h"

namespace sead {
// 0x00542250 slot 0x5C (name is ours)
void sead::FaderTaskBase::onPhase_(const s32&)
{
    // empty (in the original too)
}

// 0x00542254 slot 0x18
void sead::FaderTaskBase::pauseCalcRec(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_ALL : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x005422BC (name is ours)
void sead::FaderTaskBase::onCreateTask_(TaskBase* task)
{
    mToTask = task;
    task->mInternalFlag |= 1;
    if (mTransition == TRANSITION_CREATE || mTransition == TRANSITION_DESTROY_CREATE ||
        mTransition == TRANSITION_PAUSE) {
        mTransitionDone = true;
    }
}

// 0x005422E8 slot 0x3C | nintendogs:callseq
void sead::FaderTaskBase::attachCalcImpl()
{
    mTaskMgr->mCalcDestructionNode.pushBackChild(&mCalcCoreNode);
    attachMethodWithCheck(1, &mCalcNode);
    mCalcNode.setPauseFlag(MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x00542350 slot 0x44 | slot vf_0x44 of sead::TaskBase
void sead::FaderTaskBase::detachCalcImpl()
{
    mCalcNode.detachAll();
}

// 0x00542358 slot 0x20
void sead::FaderTaskBase::pauseCalcChild(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_CHILDREN : MethodTreeNode::PAUSE_FLAG_NONE);
}

// 0x005423C0 (name is ours)
bool sead::FaderTaskBase::requestCreateTask(const TaskBase::CreateArg& arg)
{
    CriticalSection* criticalSection = &mTaskMgr->mCriticalSection;
    criticalSection->lock();
    if (mFadeState != STATE_IDLE) {
        criticalSection->unlock();
        return false;
    }
    mTransition = TRANSITION_CREATE;
    mCreateArg = arg;
    if (!requestCreateLocked_()) {
        criticalSection->unlock();
        return false;
    }
    mFromTask = nullptr;
    mFrame = mFadeOutEnd;
    mFadeState = STATE_WAIT;
    mTransitionDone = false;
    mFadeValue = 1.0f;
    attachCalcDraw();
    criticalSection->unlock();
    return true;
}

// 0x005424A4 slot 0x50 | slot vf_0x50 of sead::TaskBase
sead::MethodTreeNode* sead::FaderTaskBase::getMethodTreeNode(s32 methodType)
{
    if (methodType == 0 || methodType == 1) {
        return &mCalcNode;
    }
    return nullptr;
}

// 0x005424B8 slot 0x60 (name is ours)
void sead::FaderTaskBase::calc()
{
    switch (mFadeState) {
    case STATE_FADE_OUT: {
        f32 value;
        if (mFadeOutEnd > 0) {
            value = static_cast<f32>(mFrame) / static_cast<f32>(mFadeOutEnd + 1);
        } else {
            value = 0.0f;
        }
        mFadeValue = value;
        if (mFrame == 0) {
            s32 phase = 2;
            onPhase_(phase);
        }
        vf_0x58();
        break;
    }
    case STATE_WAIT:
        vf_0x58();
        mFadeValue = 1.0f;
        break;
    case STATE_FADE_IN:
        vf_0x58();
        if (mFadeInEnd != mWaitEnd) {
            mFadeValue = 1.0f - static_cast<f32>(mFrame - mWaitEnd) / static_cast<f32>(mFadeInEnd - mWaitEnd);
        }
        break;
    default:
        return;
    }
    mFrame++;
}

// 0x005425D8 slot 0x30 | slot vf_0x30 of sead::TaskBase
void sead::FaderTaskBase::enter()
{
    mFadeState = STATE_IDLE;
    detachCalcImpl();
    detachDrawImpl();
}

// 0x0054260C slot 0x58
void sead::FaderTaskBase::vf_0x58()
{
    // empty (in the original too)
}

// 0x00542610 (the name of its node in the original)
void sead::FaderTaskBase::calcCore_()
{
    switch (mFadeState) {
    case STATE_FADE_OUT: {
        if (mFrame <= mFadeOutEnd) {
            return;
        }
        mFrame = mFadeOutEnd + 1;
        CriticalSection* criticalSection = &mTaskMgr->mCriticalSection;
        if (!criticalSection->tryLock()) {
            return;
        }
        s32 phase = 3;
        onPhase_(phase);
        mFadeState = STATE_WAIT;
        switch (mTransition) {
        case TRANSITION_DESTROY_CREATE:
            requestCreateLocked_();
            break;
        case TRANSITION_SWITCH:
            mFromTask->pauseCalc(true);
            mFromTask->pauseDraw(true);
            mToTask->pauseCalc(false);
            mToTask->pauseDraw(false);
            mFromTask->onEvent(TaskEvent(1));
            mToTask->onEvent(TaskEvent(2));
            mTransitionDone = true;
            break;
        case TRANSITION_PAUSE:
            mFromTask->pauseCalc(true);
            mFromTask->pauseDraw(true);
            break;
        case TRANSITION_RETURN: {
            TaskBase* task = mFromTask->parent() != nullptr ? mFromTask->parent()->val() : nullptr;
            if (mToTask != nullptr) {
                mTaskMgr->doDestroyTask_(mFromTask);
                while (mToTask != task) {
                    mTaskMgr->doDestroyTask_(task);
                    task = task->parent() != nullptr ? task->parent()->val() : nullptr;
                }
            } else {
                mTaskMgr->doDestroyTask_(mFromTask);
            }
            task->pauseCalc(false);
            task->pauseDraw(false);
            task->onEvent(TaskEvent(0));
            mTransitionDone = true;
            break;
        }
        default:
            break;
        }
        criticalSection->unlock();
        break;
    }
    case STATE_WAIT: {
        if (mFrame <= mWaitEnd) {
            return;
        }
        mFrame = mWaitEnd + 1;
        if (!mTransitionDone) {
            return;
        }
        if (mTransition == TRANSITION_DESTROY_CREATE) {
            mToTask->onEvent(TaskEvent(4));
        }
        s32 phase = 0;
        onPhase_(phase);
        mFadeState = STATE_FADE_IN;
        break;
    }
    case STATE_FADE_IN: {
        if (mFrame <= mFadeInEnd) {
            return;
        }
        s32 phase = 1;
        onPhase_(phase);
        mFrame = mFadeInEnd + 1;
        mFadeState = STATE_IDLE;
        detachCalcImpl();
        detachDrawImpl();
        if (mFromTask != nullptr) {
            mFromTask->mInternalFlag &= ~1u;
        }
        if (mToTask != nullptr) {
            mToTask->mInternalFlag &= ~1u;
        }
        break;
    }
    default:
        break;
    }
}

// 0x005429B4 slot 0x10
void sead::FaderTaskBase::pauseCalc(bool pause)
{
    mCalcNode.setPauseFlag(pause ? MethodTreeNode::PAUSE_FLAG_SELF : MethodTreeNode::PAUSE_FLAG_NONE);
}

// (out of line: the derived constructors call it)
// 0x00542A1C (name is ours)
DECOMP_NOINLINE sead::FaderTaskBase::FaderTaskBase(const TaskConstructArg& arg, const char* name)
    : TaskBase(arg, name), mTransition(TRANSITION_NONE), mFromTask(nullptr), mCreateArg(), mToTask(nullptr),
      mCreatedSlot(this, &FaderTaskBase::onCreateTask_), mCalcNode(nullptr), mCalcCoreNode(nullptr), mFrame(0),
      mFadeOutEnd(30), mWaitEnd(31), mFadeInEnd(61), mFadeValue(0.0f), mTransitionDone(false),
      mFadeState(STATE_IDLE)
{
    mCalcNode.mINamableName = name;
    mCalcNode.pause(true);
    mCalcNode.bind(this, &FaderTaskBase::calc);
    if (name != nullptr) {
        mCalcNode.mINamableName = name;
    }
    mCalcCoreNode.bind(this, &FaderTaskBase::calcCore_);
    mCalcCoreNode.mINamableName = "calcCore_";
}

// 0x00542C84 slot 0x00 | nintendogs:callseq
// 0x00542C74 (deleting dtor)
sead::FaderTaskBase::~FaderTaskBase()
{
    // (only the members are destroyed)
}
} // namespace sead
