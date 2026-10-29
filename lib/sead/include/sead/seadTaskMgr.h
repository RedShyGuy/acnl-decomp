#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadDelegateEvent.h"
#include "sead/seadHeapArray.h"
#include "sead/seadMethodTreeNode.h"
#include "sead/seadTaskBase.h"
#include "sead/seadTaskBase_CreateArg.h"
#include "sead/seadTListNode.h"

namespace sead {
class DelegateThread;
class Framework;
class Heap;
class NullFaderTask;
class Thread;

// RTTI N4sead7TaskMgrE @ 0x008D2150
// vtable 0x00906C94 (vptr 0x00906C9C), offset_to_top 0, 1 entries
//
// Creates and destroys the tasks of the framework: requests of task creation are queued and created in
// calcCreation_, prepared by the prepare thread and started; tasks are destroyed with their children
// and heaps. The method names are from the reference symbols where marked, the rest is ours.
class TaskMgr : public ::sead::hostio::Node
{
public:
    // what the framework makes the task manager with
    struct InitializeArg
    {
        s32 createQueueSize;                         // 0x00
        u32 prepareStackSize;                        // 0x04
        s32 preparePriority;                         // 0x08, -1: 17
        const TaskBase::CreateArg* rootTaskCreateArg; // 0x0C
        Heap* heap;                                  // 0x10
        Framework* parentFramework;                  // 0x14
    };

    // a queued request of task creation
    struct TaskCreateContext
    {
        TaskCreateContext() : task(nullptr), createArg(), createdEvent(), listNode() {}

        TaskBase* task;                         // 0x00, the next free context while unused
        TaskBase::CreateArg createArg;          // 0x04
        DelegateEvent<TaskBase*> createdEvent;  // 0x84
        ListNode listNode;                      // 0x94
    };

    // the queue of the requests and the free contexts
    struct TaskCreateContextMgr
    {
        TaskCreateContextMgr() : list(), freeHead(nullptr), buffer(nullptr), num(0) {}

        ListImpl list;                // 0x00
        TaskCreateContext* freeHead;  // 0x0C
        TaskCreateContext* buffer;    // 0x10
        s32 num;                      // 0x14
    };

    explicit TaskMgr(const InitializeArg& arg); // 0x00560800
    ~TaskMgr() {}

    // the task manager on arg.heap
    static TaskMgr* create(const InitializeArg& arg); // 0x005607DC
    // the prepare thread, the null fader and the queue of the requests
    void initialize_(); // 0x005603E8
    void finalize(); // 0x005605C0
    // the thread function of the prepare thread: prepares the first task of the prepare list
    void prepare_(Thread* thread, s32 message); // 0x00560694 | nintendogs:callseq [tier A]
    void createHeap_(HeapArray* heapArray, const TaskBase::CreateArg& arg); // 0x0055F31C | nintendogs:callseq-callee [tier A]
    // whether task and all tasks below it may be destroyed
    bool isDestroyable_(TaskBase* task); // 0x0055F4B8
    void appendToList_(TList<TaskBase*>& list, TaskBase* task); // 0x0055F530 | nintendogs:callseq [tier A]
    void calcCreation_(); // 0x0055F640 | nintendogs:callseq [tier A]
    TaskBase* doCreateTask_(const TaskBase::CreateArg& arg, HeapArray* heapArray); // 0x0055F830 | nintendogs:callseq [tier A]
    TaskBase* createTaskSync(const TaskBase::CreateArg& arg); // 0x0055F900 | nintendogs:callseq [tier A]
    void doDestroyTask_(TaskBase* task); // 0x0055FBCC | nintendogs:callseq [tier A]
    void destroyTaskSync(TaskBase* task); // 0x0055FC94 | nintendogs:callgraph [tier A]
    // calls the tree of mCalcDestructionNode and destroys the tasks that wait for it
    void calcDestruction_(); // 0x0055FCDC
    bool changeTaskState_(TaskBase* task, TaskBase::State state); // 0x0055FE00 | nintendogs:callseq-callee [tier A]
    bool doRequestCreateTask_(const TaskBase::CreateArg& arg, DelegateEvent<TaskBase*>::Slot* slot); // 0x0055FFC4 | nintendogs:callseq [tier A]
    void destroyAllAndCreateRoot(); // 0x005601B8 | nintendogs:callgraph [tier A]

    // creates the task of a singleton class: its instance callback sets the instance (inline; name is
    // ours)
    template <typename T>
    T* createSingletonTaskSync(const TaskBase::CreateArg& arg)
    {
        TaskBase::CreateArg singletonArg = arg;
        singletonArg.instanceCallback = &T::setInstance_;
        return DynamicCast<T>(createTaskSync(singletonArg));
    }

    // requests the root task (through its fader if it has one; inline in the constructor and in
    // destroyAllAndCreateRoot)
    void requestCreateRootTask_();

    CriticalSection mCriticalSection;          // 0x004
    Framework* mParentFramework;               // 0x020
    DelegateThread* mPrepareThread;            // 0x024
    NullFaderTask* mNullFaderTask;             // 0x028
    TList<TaskBase*> mPrepareList;             // 0x02C
    TList<TaskBase*> mPrepareDoneList;         // 0x038
    TList<TaskBase*> mActiveList;              // 0x044
    TList<TaskBase*> mStaticList;              // 0x050
    TList<TaskBase*> mDyingList;               // 0x05C
    TList<TaskBase*> mDestroyableList;         // 0x068
    HeapArray mHeapArray;                      // 0x074
    TaskCreateContextMgr* mTaskCreateContextMgr; // 0x08C
    s32 mMaxCreateQueueSize;                   // 0x090
    TaskBase* mRootTask;                       // 0x094
    TaskBase::CreateArg mRootTaskCreateArg;    // 0x098
    InitializeArg mInitializeArg;              // 0x118
    MethodTreeNode mCalcDestructionNode;       // 0x130
};
ASSERT_SIZE(TaskMgr, 0x180);
} // namespace sead
