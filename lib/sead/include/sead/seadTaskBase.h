#pragma once

#include "decomp.h"
#include "sead/seadHeapArray.h"
#include "sead/seadTaskClassID.h"
#include "sead/seadTaskConstructArg.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadIDisposer.h"
#include "sead/seadINamable.h"
#include "sead/seadTListNode.h"
#include "sead/seadTTreeNode.h"

namespace sead {
class MethodTreeMgr;
class MethodTreeNode;
class TaskEvent;
class TaskMgr;
class TaskParameter;

// RTTI N4sead8TaskBaseE @ 0x008D2210
// vtable 0x00906DAC (vptr 0x00906DB4), offset_to_top 0, 22 entries
// A task of the framework: a node of the task tree with its heaps; the derived classes attach their
// methods to the method trees.
class TaskBase : public ::sead::TTreeNode<sead::TaskBase*>, public ::sead::IDisposer, public ::sead::INamable
{
public:
    class CreateArg;
    // the states of a task (changed by TaskMgr::changeTaskState_; names are ours)
    enum State : u8
    {
        STATE_CREATED = 0,
        STATE_PREPARE = 1,      // in the list of the prepare thread
        STATE_PREPARE_DONE = 2,
        STATE_3 = 3,
        STATE_RUNNING = 4,      // in the list of the active tasks, the methods are attached
        STATE_5 = 5,
        STATE_DYING = 6,        // the methods are detached
        STATE_DEAD = 7,
    };
    // 0: a task of the system, 1: of the application (it chooses the method trees; names are ours)
    enum Tag : u8
    {
        TAG_SYSTEM = 0,
        TAG_APP = 1,
    };
    TaskBase(const TaskConstructArg& arg, const char* name); // 0x0012CDD8
    virtual ~TaskBase(); // 0x00561508 slot 0x00 | nintendogs:bytes
    // 0x00561478 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074F9D8 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074F98C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835D4 (guard 0x009835D0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    // (names of the slots are ours, pauseDraw/prepare/enter/exit/onEvent/attach*/detach*/getMethodTreeNode
    // after the reference symbols)
    // pauses the calc method of the task (Rec: with the tasks below, Child: only the tasks below)
    virtual void pauseCalc(bool pause) = 0; // slot 0x10
    virtual void pauseDraw(bool pause) = 0; // slot 0x14
    virtual void pauseCalcRec(bool pause) = 0; // slot 0x18
    virtual void pauseDrawRec(bool pause) = 0; // slot 0x1C
    virtual void pauseCalcChild(bool pause); // 0x005612B8 slot 0x20
    virtual void pauseDrawChild(bool pause); // 0x005612BC slot 0x24
    virtual void prepare(); // 0x00561414 slot 0x28 | slot vf_0x28 of sead::TaskBase
    // attaches the methods and starts them, then enter
    virtual void enterCommon(); // 0x0056119C slot 0x2C | slot vf_0x2C of sead::TaskBase
    virtual void enter(); // 0x0056140C slot 0x30 | slot vf_0x30 of sead::TaskBase
    virtual void exit(); // 0x00561408 slot 0x34 | slot vf_0x34 of sead::TaskBase
    virtual void onEvent(const TaskEvent& event); // 0x00561410 slot 0x38 | slot vf_0x38 of sead::TaskBase
    virtual void attachCalcImpl() = 0; // slot 0x3C
    virtual void attachDrawImpl() = 0; // slot 0x40
    virtual void detachCalcImpl() = 0; // slot 0x44
    virtual void detachDrawImpl() = 0; // slot 0x48
    // the type of the method tree manager that the task needs
    virtual const RuntimeTypeInfo::Interface* getCorrespondingMethodTreeMgrTypeInfo() const = 0; // slot 0x4C
    virtual MethodTreeNode* getMethodTreeNode(s32 methodType) = 0; // slot 0x50
    virtual void vf_0x54(); // 0x00561188 slot 0x54

    // the method tree manager of the framework
    MethodTreeMgr* getMethodTreeMgr() const; // 0x0074F97C (name is ours)
    void attachCalcDraw(); // 0x0056128C (name is ours)
    // adjusts all heaps of the task (under the lock of the task manager)
    void adjustHeapAll(); // 0x00561200 (name is ours)
    // a task of the task manager (falls into TaskMgr::createTaskSync in the original; name is ours)
    TaskBase* createTaskSync(const CreateArg& arg); // 0x0055F8F8
    void attachMethodWithCheck(s32 methodType, MethodTreeNode* node); // 0x00561350 | nintendogs:callseq-callee [tier A]
    void adjustHeapWithSlackWithoutLock_(s32 index, u32 slackSize); // 0x00561368 | nintendogs:bytes-fuzzy [tier A]

    TaskParameter* mParameter;       // 0x2C
    u32 mInternalFlag;               // 0x30
    TListNode<TaskBase*> mTaskListNode; // 0x34, in a list of the task manager
    HeapArray mHeapArray;            // 0x44
    TaskMgr* mTaskMgr;               // 0x5C
    State mState;                    // 0x60
    Tag mTag;                        // 0x61
    TaskClassID mClassID;            // 0x64, what made the task
};
ASSERT_SIZE(TaskBase, 0x6C);
} // namespace sead
