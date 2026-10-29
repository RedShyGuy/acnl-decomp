#include "sead/seadTaskMgr.h"
#include "nn/os/os_Api.h"
#include "sead/seadDelegate2.h"
#include "sead/seadDelegateThread.h"
#include "sead/seadExpHeap.h"
#include "sead/seadFaderTaskBase.h"
#include "sead/seadFramework.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadNew.h"
#include "sead/seadNullFaderTask.h"

#include <new>

namespace sead {
namespace {
// the heap of the task manager is the current heap while a task is made (nothing changes without one)
inline Heap* setCurrentHeapIfAny(Heap* heap)
{
    if (heap == nullptr) {
        return reinterpret_cast<Heap*>(1);
    }
    return HeapMgr::s_pInstance->setCurrentHeap_(heap);
}

inline void restoreCurrentHeap(Heap* previous)
{
    if (previous != reinterpret_cast<Heap*>(1)) {
        HeapMgr::s_pInstance->setCurrentHeap_(previous);
    }
}

// the policy of the root heap index (the first one for an index out of range)
inline const TaskHeapPolicy& getPolicy(const TaskHeapPolicies& policies, s32 index)
{
    if (static_cast<u32>(index) < static_cast<u32>(HeapMgr::s_RootHeaps.size())) {
        return policies.policies[index];
    }
    return policies.policies[0];
}

// the heaps of the task get its name
inline void setHeapNames(TaskBase* task)
{
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        Heap* heap = task->mHeapArray.mHeaps[i];
        if (heap != nullptr) {
            heap->mINamableName = task->mINamableName;
        }
    }
}

// takes all tasks out of list
inline void clearList(TList<TaskBase*>& list)
{
    ListNode* node = list.mStartEnd.mNext;
    while (node != &list.mStartEnd) {
        TListNode<TaskBase*>* taskNode = static_cast<TListNode<TaskBase*>*>(node);
        node = node->mNext;
        if (taskNode->mList != nullptr) {
            list.erase(taskNode);
        }
    }
}
} // namespace

// 0x0055F31C | nintendogs:callseq-callee [tier A]
void sead::TaskMgr::createHeap_(HeapArray* heapArray, const TaskBase::CreateArg& arg)
{
    heapArray->mPrimaryIndex = arg.heapPolicies.primaryIndex;
    s32 num = HeapMgr::s_RootHeaps.size();
    mCriticalSection.lock();
    for (s32 i = 0; i < num; i++) {
        const TaskHeapPolicy& policy = getPolicy(arg.heapPolicies, i);
        Heap* heap = nullptr;
        if (!policy.dontCreate) {
            Heap* parent = policy.parent;
            if (parent == nullptr) {
                if (arg.parent != nullptr) {
                    parent = arg.parent->mHeapArray.mHeaps[i];
                } else {
                    parent = mHeapArray.mHeaps[i];
                }
            }
            if (parent != nullptr) {
                Heap::HeapDirection direction =
                    policy.fromTail ? Heap::HEAP_DIRECTION_REVERSE : Heap::HEAP_DIRECTION_FORWARD;
                if (policy.sizeLeftInParent == 0 || policy.size != 0) {
                    heap = ExpHeap::create(policy.size, "TaskHeap", parent, direction, false);
                } else {
                    u32 maxSize = parent->getMaxAllocatableSize(4);
                    if (policy.sizeLeftInParent < maxSize) {
                        heap = ExpHeap::create(maxSize - policy.sizeLeftInParent, "TaskHeap", parent, direction, false);
                    }
                }
            }
        }
        heapArray->mHeaps[i] = heap;
    }
    mCriticalSection.unlock();
}

// (the original loops while a child is destroyable without destroying it; a call in the loop was
// removed by armlink)
// 0x0055F4B8 (name is ours)
bool sead::TaskMgr::isDestroyable_(TaskBase* task)
{
    mCriticalSection.lock();
    if ((task->mInternalFlag & 2) != 0 && (task->mInternalFlag & 4) != 0 && task->mState == TaskBase::STATE_RUNNING) {
        while (task->child() != nullptr) {
            if (!isDestroyable_(task->child()->val())) {
                mCriticalSection.unlock();
                return false;
            }
        }
        mCriticalSection.unlock();
        return true;
    }
    mCriticalSection.unlock();
    return false;
}

// (sorted by the tag: system tasks first)
// 0x0055F530 | nintendogs:callseq [tier A]
void sead::TaskMgr::appendToList_(TList<TaskBase*>& list, TaskBase* task)
{
    mCriticalSection.lock();
    TListNode<TaskBase*>* taskNode = &task->mTaskListNode;
    if (taskNode->mList != nullptr) {
        taskNode->mList->erase(taskNode);
    }
    for (ListNode* node = list.mStartEnd.mNext; node != &list.mStartEnd; node = node->mNext) {
        TListNode<TaskBase*>* otherNode = static_cast<TListNode<TaskBase*>*>(node);
        if (otherNode->mData->mTag < task->mTag) {
            if (taskNode->mList != nullptr) {
                taskNode->mList->erase(taskNode);
            }
            taskNode->mList = otherNode->mList;
            otherNode->insertFront_(taskNode);
            taskNode->mList->mCount++;
            mCriticalSection.unlock();
            return;
        }
    }
    list.pushBack(taskNode);
    mCriticalSection.unlock();
}

// 0x0055F640 | nintendogs:callseq [tier A]
void sead::TaskMgr::calcCreation_()
{
    if (!mCriticalSection.tryLock()) {
        return;
    }
    TaskCreateContextMgr* contextMgr = mTaskCreateContextMgr;
    if (contextMgr->list.mCount != 0) {
        TaskCreateContext* context = nullptr;
        if (contextMgr->list.mCount > 0 && contextMgr->list.mStartEnd.mNext != nullptr) {
            context = reinterpret_cast<TaskCreateContext*>(reinterpret_cast<u8*>(contextMgr->list.mStartEnd.mNext) -
                                                           offsetof(TaskCreateContext, listNode));
        }
        if (context != nullptr) {
            TaskBase* task = context->task;
            if (task == nullptr) {
                HeapArray heapArray;
                createHeap_(&heapArray, context->createArg);
                task = doCreateTask_(context->createArg, &heapArray);
                setHeapNames(task);
                context->task = task;
                changeTaskState_(task, TaskBase::STATE_PREPARE);
            } else if (task->mState == TaskBase::STATE_PREPARE_DONE) {
                changeTaskState_(task, TaskBase::STATE_RUNNING);
                if (context->createArg.createdTask != nullptr) {
                    *context->createArg.createdTask = task;
                }
                context->createdEvent.fire(task);
                TaskCreateContextMgr* mgr = mTaskCreateContextMgr;
                context->listNode.erase_();
                mgr->list.mCount--;
                context->~TaskCreateContext();
                // back to the free contexts
                *reinterpret_cast<TaskCreateContext**>(context) = mgr->freeHead;
                mgr->freeHead = context;
            }
        }
    }
    mCriticalSection.unlock();
}

// 0x0055F830 | nintendogs:callseq [tier A]
sead::TaskBase* sead::TaskMgr::doCreateTask_(const TaskBase::CreateArg& arg, HeapArray* heapArray)
{
    mCriticalSection.lock();
    TaskClassID classID = arg.factory;
    TaskBase::Tag tag = arg.tag;
    Heap* previous = setCurrentHeapIfAny(heapArray->mHeaps[heapArray->mPrimaryIndex]);
    TaskConstructArg constructArg;
    constructArg.heapArray = heapArray;
    constructArg.taskMgr = this;
    constructArg.parameter = arg.parameter;
    TaskBase* task = classID.create(constructArg);
    restoreCurrentHeap(previous);
    task->mClassID = classID;
    task->mTag = tag;
    if (arg.parent != nullptr) {
        arg.parent->pushBackChild(task);
    }
    mCriticalSection.unlock();
    return task;
}

// 0x0055F900 | nintendogs:callseq [tier A]
sead::TaskBase* sead::TaskMgr::createTaskSync(const TaskBase::CreateArg& arg)
{
    mCriticalSection.lock();
    HeapArray heapArray;
    createHeap_(&heapArray, arg);
    TaskBase* task = doCreateTask_(arg, &heapArray);
    setHeapNames(task);
    if (arg.instanceCallback != nullptr) {
        arg.instanceCallback(task);
    }
    task->mState = TaskBase::STATE_PREPARE;
    Heap* previous = setCurrentHeapIfAny(heapArray.mHeaps[heapArray.mPrimaryIndex]);
    task->prepare();
    restoreCurrentHeap(previous);
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        const TaskHeapPolicy& policy = getPolicy(arg.heapPolicies, i);
        if (policy.adjust) {
            task->adjustHeapWithSlackWithoutLock_(i, policy.adjustSlack);
        }
    }
    changeTaskState_(task, TaskBase::STATE_PREPARE_DONE);
    changeTaskState_(task, TaskBase::STATE_RUNNING);
    if (arg.createdSlot != nullptr) {
        DelegateEvent<TaskBase*> event;
        event.connect(arg.createdSlot);
        event.fire(task);
    }
    if (arg.createdTask != nullptr) {
        *arg.createdTask = task;
    }
    mCriticalSection.unlock();
    return task;
}

// (the task itself goes with its heaps)
// 0x0055FBCC | nintendogs:callseq [tier A]
void sead::TaskMgr::doDestroyTask_(TaskBase* task)
{
    mCriticalSection.lock();
    while (task->child() != nullptr) {
        doDestroyTask_(task->child()->val());
    }
    if (changeTaskState_(task, TaskBase::STATE_DEAD)) {
        task->TTreeNode<TaskBase*>::detachAll();
        HeapArray heapArray = task->mHeapArray;
        for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
            if (heapArray.mHeaps[i] != nullptr) {
                heapArray.mHeaps[i]->destroy();
            }
        }
    }
    mCriticalSection.unlock();
}

// 0x0055FC94 | nintendogs:callgraph [tier A]
void sead::TaskMgr::destroyTaskSync(TaskBase* task)
{
    if (mParentFramework->mMethodTreeMgr->mCriticalSection.tryLock()) {
        doDestroyTask_(task);
        mParentFramework->mMethodTreeMgr->mCriticalSection.unlock();
    }
}

// 0x0055FCDC (name is ours)
void sead::TaskMgr::calcDestruction_()
{
    mCalcDestructionNode.call();
    if (!mCriticalSection.tryLock()) {
        return;
    }
    ListNode* node = mActiveList.mStartEnd.mNext;
    while (node != &mActiveList.mStartEnd) {
        TaskBase* task = static_cast<TListNode<TaskBase*>*>(node)->mData;
        node = node->mNext;
        if ((task->mInternalFlag & 2) != 0 && isDestroyable_(task)) {
            changeTaskState_(task, TaskBase::STATE_DYING);
        }
    }
    while (mDestroyableList.mStartEnd.mNext != &mDestroyableList.mStartEnd) {
        TaskBase* task = static_cast<TListNode<TaskBase*>*>(mDestroyableList.mStartEnd.mNext)->mData;
        task->detachCalcImpl();
        task->detachDrawImpl();
        task->TTreeNode<TaskBase*>::detachAll();
        doDestroyTask_(task);
    }
    mCriticalSection.unlock();
}

// 0x0055FE00 | nintendogs:callseq-callee [tier A]
bool sead::TaskMgr::changeTaskState_(TaskBase* task, TaskBase::State state)
{
    mCriticalSection.lock();
    if (task->mState == state) {
        mCriticalSection.unlock();
        return false;
    }
    switch (state) {
    case TaskBase::STATE_PREPARE:
        if (task->mState != TaskBase::STATE_CREATED) {
            break;
        }
        task->mState = TaskBase::STATE_PREPARE;
        appendToList_(mPrepareList, task);
        if (mPrepareThread != nullptr && !mPrepareThread->sendMessage(1, MessageQueue::BLOCK_TYPE_NO_BLOCK)) {
            break;
        }
        mCriticalSection.unlock();
        return true;
    case TaskBase::STATE_PREPARE_DONE:
        task->mState = TaskBase::STATE_PREPARE_DONE;
        if (task->mTaskListNode.mList != nullptr) {
            task->mTaskListNode.mList->erase(&task->mTaskListNode);
        }
        mCriticalSection.unlock();
        return true;
    case TaskBase::STATE_RUNNING:
        task->mState = TaskBase::STATE_RUNNING;
        if (task->mTaskListNode.mList != nullptr) {
            task->mTaskListNode.mList->erase(&task->mTaskListNode);
        }
        appendToList_(mActiveList, task);
        task->enterCommon();
        mCriticalSection.unlock();
        return true;
    case TaskBase::STATE_5:
        task->mState = TaskBase::STATE_5;
        mCriticalSection.unlock();
        return true;
    case TaskBase::STATE_DYING:
        if (task->mState != TaskBase::STATE_RUNNING) {
            break;
        }
        task->mState = TaskBase::STATE_DYING;
        task->detachCalcImpl();
        task->detachDrawImpl();
        appendToList_(mDestroyableList, task);
        mCriticalSection.unlock();
        return true;
    case TaskBase::STATE_DEAD:
        task->exit();
        task->mState = TaskBase::STATE_DEAD;
        if (task->mTaskListNode.mList != nullptr) {
            task->mTaskListNode.mList->erase(&task->mTaskListNode);
        }
        mCriticalSection.unlock();
        return true;
    default:
        break;
    }
    mCriticalSection.unlock();
    return false;
}

// 0x0055FFC4 | nintendogs:callseq [tier A]
bool sead::TaskMgr::doRequestCreateTask_(const TaskBase::CreateArg& arg, DelegateEvent<TaskBase*>::Slot* slot)
{
    mCriticalSection.lock();
    TaskCreateContextMgr* contextMgr = mTaskCreateContextMgr;
    if (contextMgr->list.mCount >= contextMgr->num) {
        mCriticalSection.unlock();
        return false;
    }
    TaskCreateContext* context = contextMgr->freeHead;
    if (context != nullptr) {
        contextMgr->freeHead = *reinterpret_cast<TaskCreateContext**>(context);
        context = new (context) TaskCreateContext();
    }
    contextMgr->list.mStartEnd.insertFront_(&context->listNode);
    contextMgr->list.mCount++;
    if (context == nullptr) {
        mCriticalSection.unlock();
        return false;
    }
    context->createArg = arg;
    DelegateEvent<TaskBase*>::Slot* createdSlot = arg.createdSlot;
    if (slot != nullptr) {
        context->createdEvent.connect(slot);
    }
    if (createdSlot != nullptr) {
        context->createdEvent.connect(createdSlot);
    }
    mCriticalSection.unlock();
    return true;
}

// 0x005601B8 | nintendogs:callgraph [tier A]
void sead::TaskMgr::destroyAllAndCreateRoot()
{
    mCriticalSection.lock();
    if (mRootTask != nullptr && mParentFramework->mMethodTreeMgr->mCriticalSection.tryLock()) {
        doDestroyTask_(mRootTask);
        mParentFramework->mMethodTreeMgr->mCriticalSection.unlock();
    }
    clearList(mPrepareList);
    clearList(mActiveList);
    clearList(mDyingList);
    clearList(mDestroyableList);
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        if (mHeapArray.mHeaps[i] != nullptr) {
            mHeapArray.mHeaps[i]->freeAll();
        }
    }
    initialize_();
    requestCreateRootTask_();
    mCriticalSection.unlock();
}

// 0x005603E8 (name is ours)
void sead::TaskMgr::initialize_()
{
    Heap* heap = mHeapArray.mHeaps[mHeapArray.mPrimaryIndex];
    void* threadBuffer = new (heap, 4) u8[sizeof(DelegateThread)];
    Delegate2<TaskMgr, Thread*, s32>* delegate =
        new (heap, 4) Delegate2<TaskMgr, Thread*, s32>(this, &TaskMgr::prepare_);
    s32 priority = mInitializeArg.preparePriority == -1 ? 17 : mInitializeArg.preparePriority;
    mPrepareThread = new (threadBuffer) DelegateThread("Prepare Thread", delegate, heap, priority,
                                                       MessageQueue::BLOCK_TYPE_BLOCK, 0x7FFFFFFF,
                                                       mInitializeArg.prepareStackSize, 32);
    mPrepareThread->start();

    HeapArray heapArray;
    heapArray.mHeaps[0] = heap;
    TaskConstructArg constructArg;
    constructArg.heapArray = &heapArray;
    constructArg.taskMgr = this;
    constructArg.parameter = nullptr;
    mNullFaderTask = new (heap, 4) NullFaderTask(constructArg);
    mNullFaderTask->mINamableName = "NullFader";
    mNullFaderTask->mState = TaskBase::STATE_RUNNING;

    TaskCreateContextMgr* contextMgr = new (heap, 4) TaskCreateContextMgr();
    s32 num = mMaxCreateQueueSize;
    if (num > 0) {
        TaskCreateContext* buffer = static_cast<TaskCreateContext*>(
            operator new(sizeof(TaskCreateContext) * num, heap, 4, std::nothrow));
        if (buffer != nullptr) {
            contextMgr->freeHead = buffer;
            for (s32 i = 0; i < num - 1; i++) {
                *reinterpret_cast<TaskCreateContext**>(&buffer[i]) = &buffer[i + 1];
            }
            *reinterpret_cast<TaskCreateContext**>(&buffer[num - 1]) = nullptr;
            contextMgr->buffer = buffer;
            contextMgr->num = num;
        }
    }
    mTaskCreateContextMgr = contextMgr;
}

// 0x005605C0 (name is ours; symbols.json: prepare_)
void sead::TaskMgr::finalize()
{
    if (mPrepareThread != nullptr) {
        mPrepareThread->quitAndDestroySingleThread(false);
        if (mPrepareThread != nullptr) {
            delete mPrepareThread;
        }
        mPrepareThread = nullptr;
    }
    if (mRootTask != nullptr) {
        if (mParentFramework->mMethodTreeMgr->mCriticalSection.tryLock()) {
            doDestroyTask_(mRootTask);
            mParentFramework->mMethodTreeMgr->mCriticalSection.unlock();
        }
        mRootTask = nullptr;
    }
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        if (mHeapArray.mHeaps[i] != nullptr) {
            mHeapArray.mHeaps[i]->destroy();
            mHeapArray.mHeaps[i] = nullptr;
        }
    }
}

// 0x00560694 | nintendogs:callseq [tier A]
void sead::TaskMgr::prepare_(Thread*, s32)
{
    TaskBase* task = nullptr;
    mCriticalSection.lock();
    if (mPrepareList.mStartEnd.mNext != &mPrepareList.mStartEnd) {
        task = static_cast<TListNode<TaskBase*>*>(mPrepareList.mStartEnd.mNext)->mData;
    }
    mCriticalSection.unlock();
    if (task != nullptr) {
        Heap* previous = setCurrentHeapIfAny(task->mHeapArray.mHeaps[task->mHeapArray.mPrimaryIndex]);
        task->prepare();
        // the request of the task
        TaskCreateContextMgr* contextMgr = mTaskCreateContextMgr;
        TaskCreateContext* context = nullptr;
        if (contextMgr->list.mCount > 0 && contextMgr->list.mStartEnd.mNext != nullptr) {
            context = reinterpret_cast<TaskCreateContext*>(reinterpret_cast<u8*>(contextMgr->list.mStartEnd.mNext) -
                                                           offsetof(TaskCreateContext, listNode));
        }
        while (context != nullptr && context->task != task) {
            ListNode* next = context->listNode.mNext;
            context = next != &contextMgr->list.mStartEnd
                          ? reinterpret_cast<TaskCreateContext*>(reinterpret_cast<u8*>(next) -
                                                                 offsetof(TaskCreateContext, listNode))
                          : nullptr;
        }
        for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
            const TaskHeapPolicy& policy = getPolicy(context->createArg.heapPolicies, i);
            if (policy.adjust) {
                task->adjustHeapWithSlackWithoutLock_(i, policy.adjustSlack);
            }
        }
        changeTaskState_(task, TaskBase::STATE_PREPARE_DONE);
        restoreCurrentHeap(previous);
    }
    nn::os::YieldThread();
}

// 0x005607DC (name is ours)
sead::TaskMgr* sead::TaskMgr::create(const InitializeArg& arg)
{
    return new (arg.heap, 4) TaskMgr(arg);
}

// 0x00560800 (name is ours)
sead::TaskMgr::TaskMgr(const InitializeArg& arg)
    : hostio::Node(), mCriticalSection(), mParentFramework(arg.parentFramework), mPrepareThread(nullptr),
      mNullFaderTask(nullptr), mPrepareList(), mPrepareDoneList(), mActiveList(), mStaticList(), mDyingList(),
      mDestroyableList(), mHeapArray(), mMaxCreateQueueSize(arg.createQueueSize), mRootTask(nullptr),
      mRootTaskCreateArg(), mInitializeArg(arg), mCalcDestructionNode(nullptr)
{
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        Heap* parent = i < HeapMgr::s_RootHeaps.size() ? HeapMgr::s_RootHeaps.at(i) : nullptr;
        mHeapArray.mHeaps[i] = ExpHeap::create(0, "sead::TaskMgr", parent, Heap::HEAP_DIRECTION_FORWARD, false);
    }
    initialize_();
    mRootTaskCreateArg = *arg.rootTaskCreateArg;
    requestCreateRootTask_();
}

void sead::TaskMgr::requestCreateRootTask_()
{
    mRootTask = nullptr;
    mRootTaskCreateArg.tag = TaskBase::TAG_APP;
    mRootTaskCreateArg.createdTask = &mRootTask;
    mRootTaskCreateArg.fader = nullptr;
    mCriticalSection.lock();
    if (mRootTaskCreateArg.fader != nullptr) {
        mRootTaskCreateArg.fader->requestCreateTask(mRootTaskCreateArg);
    } else {
        doRequestCreateTask_(mRootTaskCreateArg, nullptr);
    }
    mCriticalSection.unlock();
}
} // namespace sead
