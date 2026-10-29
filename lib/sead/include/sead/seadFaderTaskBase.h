#pragma once

#include "decomp.h"
#include "sead/seadDelegateEvent.h"
#include "sead/seadMethodTreeNode.h"
#include "sead/seadTaskBase.h"
#include "sead/seadTaskBase_CreateArg.h"
#include "sead/seadTaskMgr.h"

namespace sead {
// RTTI N4sead13FaderTaskBaseE @ 0x008D1650
// vtable 0x00905500 (vptr 0x00905508), offset_to_top 0, 25 entries
// A task that changes between tasks behind a fade: it fades out, then destroys/creates/pauses tasks
// (mTransition), waits, and fades in. calcCore_ is the name the original gives its node; the other
// names are ours.
class FaderTaskBase : public ::sead::TaskBase
{
public:
    // what happens between fading out and fading in
    enum Transition : u8
    {
        TRANSITION_CREATE = 0,          // the task of mCreateArg is created (requestCreateTask)
        TRANSITION_DESTROY_CREATE = 1,  // mFromTask is destroyed, then the task is created
        TRANSITION_SWITCH = 2,          // mFromTask is paused, mToTask goes on
        TRANSITION_PAUSE = 3,           // mFromTask is paused
        TRANSITION_RETURN = 4,          // the tasks from mFromTask up to mToTask are destroyed
        TRANSITION_NONE = 5,
    };
    // where the fade is
    enum State : u8
    {
        STATE_IDLE = 0,
        STATE_FADE_OUT = 1,
        STATE_WAIT = 2,
        STATE_FADE_IN = 3,
    };

    FaderTaskBase(const TaskConstructArg& arg, const char* name); // 0x00542A1C
    virtual ~FaderTaskBase(); // 0x00542C84 slot 0x00 | nintendogs:callseq
    // 0x00542C74 slot 0x04 (deleting dtor)
    virtual void pauseCalc(bool pause); // 0x005429B4 slot 0x10
    virtual void pauseCalcRec(bool pause); // 0x00542254 slot 0x18
    virtual void pauseCalcChild(bool pause); // 0x00542358 slot 0x20
    virtual void enter(); // 0x005425D8 slot 0x30 | slot vf_0x30 of sead::TaskBase
    virtual void attachCalcImpl(); // 0x005422E8 slot 0x3C | nintendogs:callseq
    virtual void detachCalcImpl(); // 0x00542350 slot 0x44 | slot vf_0x44 of sead::TaskBase
    virtual MethodTreeNode* getMethodTreeNode(s32 methodType); // 0x005424A4 slot 0x50 | slot vf_0x50 of sead::TaskBase
    virtual void vf_0x58(); // 0x0054260C slot 0x58
    // told when a phase begins (0 fade in, 1 idle, 2 fade out, 3 wait)
    virtual void onPhase_(const s32& phase); // 0x00542250 slot 0x5C
    // the fade value of the frame (bound to mCalcNode)
    virtual void calc(); // 0x005424B8 slot 0x60

    // starts a fade that creates the task of arg (false while a fade runs or if the request fails)
    bool requestCreateTask(const TaskBase::CreateArg& arg); // 0x005423C0
    // the transition (bound to mCalcCoreNode, which is in the tree of TaskMgr::mCalcDestructionNode)
    void calcCore_(); // 0x00542610
    // the created task (the slot of mCreatedSlot)
    void onCreateTask_(TaskBase* task); // 0x005422BC

    // destroys mFromTask (TRANSITION_DESTROY_CREATE) and requests mCreateArg, under the lock of the task
    // manager (inline)
    bool requestCreateLocked_()
    {
        CriticalSection* criticalSection = &mTaskMgr->mCriticalSection;
        criticalSection->lock();
        if (mTransition == TRANSITION_DESTROY_CREATE) {
            mTaskMgr->doDestroyTask_(mFromTask);
            mFromTask = nullptr;
        }
        bool result = mTaskMgr->doRequestCreateTask_(mCreateArg, &mCreatedSlot);
        criticalSection->unlock();
        return result;
    }

    Transition mTransition;                      // 0x06C
    TaskBase* mFromTask;                         // 0x070
    TaskBase::CreateArg mCreateArg;              // 0x074
    TaskBase* mToTask;                           // 0x0F4
    DelegateEvent<TaskBase*>::Slot mCreatedSlot; // 0x0F8
    MethodTreeNode mCalcNode;                    // 0x134
    MethodTreeNode mCalcCoreNode;                // 0x184
    s32 mFrame;                                  // 0x1D4
    s32 mFadeOutEnd;                             // 0x1D8, last frame of the fade out
    s32 mWaitEnd;                                // 0x1DC
    s32 mFadeInEnd;                              // 0x1E0
    f32 mFadeValue;                              // 0x1E4, 0 clear .. 1 covered
    bool mTransitionDone;                        // 0x1E8
    State mFadeState;                            // 0x1E9
};
ASSERT_SIZE(FaderTaskBase, 0x1EC);
} // namespace sead
