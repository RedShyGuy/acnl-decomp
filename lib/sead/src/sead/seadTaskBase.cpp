#include "sead/seadHeap.h"
#include "sead/seadTaskBase.h"
#include "sead/seadFramework.h"
#include "sead/seadMethodTreeMgr.h"
#include "sead/seadTaskBase_CreateArg.h"
#include "sead/seadTaskMgr.h"

namespace sead {
// (out of line: the derived constructors call it)
// 0x0012CDD8 | nintendogs:bytes
DECOMP_NOINLINE sead::TaskBase::TaskBase(const TaskConstructArg& arg, const char* name)
    : TTreeNode<TaskBase*>(this), IDisposer(), INamable(""), mParameter(arg.parameter), mInternalFlag(0),
      mTaskListNode(this), mHeapArray(*arg.heapArray), mTaskMgr(arg.taskMgr), mState(STATE_CREATED), mTag(TAG_APP),
      mClassID()
{
    mINamableName = name;
}

// 0x00561188 slot 0x54
void sead::TaskBase::vf_0x54()
{
    mInternalFlag |= 4;
}

// 0x0055F8F8 (name is ours)
sead::TaskBase* sead::TaskBase::createTaskSync(const CreateArg& arg)
{
    return mTaskMgr->createTaskSync(arg);
}

// 0x0056119C slot 0x2C | slot vf_0x2C of sead::TaskBase
void sead::TaskBase::enterCommon()
{
    attachCalcImpl();
    attachDrawImpl();
    pauseCalc(false);
    pauseDraw(false);
    enter();
}

// 0x00561200 (name is ours)
void sead::TaskBase::adjustHeapAll()
{
    CriticalSection* criticalSection = &mTaskMgr->mCriticalSection;
    criticalSection->lock();
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        Heap* heap = mHeapArray.getHeap(i);
        if (heap != nullptr && !mHeapArray.mAdjusted[i]) {
            mHeapArray.mAdjusted[i] = true;
            heap->adjust();
        }
    }
    criticalSection->unlock();
}

// 0x0056128C (name is ours)
void sead::TaskBase::attachCalcDraw()
{
    attachCalcImpl();
    attachDrawImpl();
}

// 0x005612B8 slot 0x20
void sead::TaskBase::pauseCalcChild(bool)
{
    // empty (in the original too)
}

// 0x005612BC slot 0x24
void sead::TaskBase::pauseDrawChild(bool)
{
    // empty (in the original too)
}

// 0x00561350 | nintendogs:callseq-callee [tier A]
void sead::TaskBase::attachMethodWithCheck(s32 methodType, MethodTreeNode* node)
{
    getMethodTreeMgr()->attachMethod(methodType, node);
}

// 0x00561368 | nintendogs:bytes-fuzzy [tier A]
void sead::TaskBase::adjustHeapWithSlackWithoutLock_(s32 index, u32 slackSize)
{
    Heap* heap = mHeapArray.getHeap(index);
    if (heap == nullptr || mHeapArray.mAdjusted[index]) {
        return;
    }
    mHeapArray.mAdjusted[index] = true;
    // the slack stays free: it is allocated while the heap is adjusted
    void* slack = nullptr;
    if (slackSize != 0) {
        slack = heap->tryAlloc(slackSize, 4);
    }
    heap->adjust();
    if (slack != nullptr) {
        heap->free(slack);
    }
}

// 0x00561408 slot 0x34 | slot vf_0x34 of sead::TaskBase
void sead::TaskBase::exit()
{
    // empty (in the original too)
}

// 0x0056140C slot 0x30 | slot vf_0x30 of sead::TaskBase
void sead::TaskBase::enter()
{
    // empty (in the original too)
}

// 0x00561410 slot 0x38 | slot vf_0x38 of sead::TaskBase
void sead::TaskBase::onEvent(const TaskEvent&)
{
    // empty (in the original too)
}

// 0x00561414 slot 0x28 | slot vf_0x28 of sead::TaskBase
void sead::TaskBase::prepare()
{
    // empty (in the original too)
}

// 0x00561418 | nintendogs:callseq-callee [tier A]
sead::TaskBase::CreateArg::CreateArg()
    : factory(), parent(nullptr), parameter(nullptr), fader(nullptr), field_0x68(0), createdTask(nullptr),
      createdSlot(nullptr), field_0x74(-1), tag(TAG_APP), instanceCallback(nullptr)
{
    heapPolicies.primaryIndex = 0;
}

// 0x0011D89C (name is ours)
sead::TaskBase::CreateArg::CreateArg(const TaskClassID& classID)
    : factory(classID), parent(nullptr), parameter(nullptr), fader(nullptr), field_0x68(0), createdTask(nullptr),
      createdSlot(nullptr), field_0x74(-1), tag(TAG_APP), instanceCallback(nullptr)
{
    heapPolicies.primaryIndex = 0;
}

// 0x005612C0 (name is ours)
sead::SystemTaskCreateArg::SystemTaskCreateArg(const TaskClassID& classID) : CreateArg(classID)
{
    for (s32 i = 0; i < HeapMgr::s_RootHeaps.size(); i++) {
        heapPolicies.policies[i].adjust = true;
    }
    heapPolicies.setPrimaryIndex(heapPolicies.primaryIndex);
    tag = TaskBase::TAG_SYSTEM;
}

// 0x00561508 slot 0x00 | nintendogs:bytes
// 0x00561478 (deleting dtor)
sead::TaskBase::~TaskBase()
{
    if (mTaskMgr != nullptr) {
        while (child() != nullptr) {
            mTaskMgr->destroyTaskSync(child()->val());
        }
    }
    mState = STATE_DEAD;
    TTreeNode<TaskBase*>::detachAll();
    if (mTaskListNode.mList != nullptr) {
        mTaskListNode.mList->erase(&mTaskListNode);
    }
}

// 0x0074F97C (name is ours)
DECOMP_NOINLINE sead::MethodTreeMgr* sead::TaskBase::getMethodTreeMgr() const
{
    return mTaskMgr->mParentFramework->mMethodTreeMgr;
}
} // namespace sead
